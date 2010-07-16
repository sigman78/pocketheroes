//
//

#include "stdafx.h"

#include <stdio.h>
//#include "xjp.boot.h"

#include "xbs.bs.h"
#include "xbs.bs.impl.h"
#include "xbs.math.h"

#include "xjp.huff.h"
#include "xjp.huff.impl.h"

#include "xjp.dct.h"

#include "xjp.quant.h"
#include "xjp.block.h"
#include "xjp.codec.h"

#include "xjp.swizzle.h"

namespace xjp {

// ---------------------------
// header format & such
//
// width:12, height:12
// channels:4
// qfactor:4
// [ 'n' tables 
//   dc-bits : 8*17
//   dc-vals : 8*17
//	 ac-bits : 8*17
//	 ac-vals : 8*162?
// ] = n * 205 bytes
// {raw bits}
// padding to byte 
// ---------------------------

#define MAX_DC 17

always_inline
uint32 num_nonzero( const uint8* elem, uint32 n )
{
	const uint8* tail = elem + n;
	while ( tail != elem && *--tail == 0 ) n--;
	return n;
}

void put_params( huff_params_t& params, uint32 val_size, put_bits_t* pb )
{
	uint8* ptr = ((uint8*)pb->buf) + (pb->index>>3);
	uint32 n_bits = num_nonzero( params.bits, MAX_DC );
	*ptr++ = (uint8)n_bits;
	memcpy( ptr, &params.bits, n_bits );
	ptr += n_bits;
	uint32 n_vals = num_nonzero( params.vals, val_size );
	*ptr++ = (uint8)n_vals;
	memcpy( ptr, &params.vals, n_vals );
	pb->index += ( 2 + n_bits + n_vals ) * 8;
}

void get_params( huff_params_t& params, uint32 val_size, get_bits_t* gb )
{
	const uint8* ptr = ((const uint8*)gb->buf) + (gb->index>>3);
	uint32 n_bits = *ptr++;
	memset( &params, 0, sizeof(params) );
	memcpy( params.bits, ptr, n_bits );
	ptr += n_bits;
	uint32 n_vals = *ptr++;
	memcpy( params.vals, ptr, n_vals );
	
	gb->index += ( 2 + n_bits + n_vals ) * 8;
}

//
//
void enc_init( encoder_t* enc, uint32 width, uint32 height, uint32 chnl, uint32 qfact )
{
	enc->width 	  = width;
	enc->height	  = height;
	enc->channels = chnl;
	enc->qfactor= qfact & 0xff;
}

struct enc_component
{
	huff_etab_t*	dcenc;
	huff_etab_t*	acenc;
	uint32			cdidx;
	dpel_t   		qmatrix[64];
	dpel_t			last_dc;
};
struct enc_info
{
	void (*func)( const uint8* dst, uint32 stride, dpel_t* buf );
	uint32	nchn;
	uint32	ncod;
	uint32	pitch;
	uint32	stride;
	uint32	aligned_width;
	uint32	aligned_height;
};

struct channel_desc {
	uint8	qf_idx;
	uint8	cd_idx;
	uint8	qm_type;
	uint8	is_alpha;
};

static channel_desc ch_desc[4] = 
{
	{ 1, 0, 0, 0 }, // Y
	{ 0, 2, 1, 0 }, // C0
	{ 0, 2, 1, 0 }, // C1
	{ 1, 4, 0, 1 }, // Alpha
};

struct pixelformat_desc {
	uint8	pitch;
	uint8	nchn;
	uint8	has_alpha;
	uint8	ncod;
	void (*efunc)( const uint8* src, uint32 stride, dpel_t* buf);
	void (*dfunc)( uint8* src, uint32 stride, const dpel_t* buf);
} pix_desc[] =  {
	{ 0,0, 0, 0, 0, 0 }, // unused
	{ 0,0, 0, 0, 0, 0 }, 
	{ 2,3, 0, 4, 0, &unswizzle_blockRGB16 }, // 16
	{ 3,3, 0, 4, &swizzle_blockRGB24, &unswizzle_blockRGB24 }, // 24
	{ 4,3, 0, 4, &swizzle_blockRGB32, &unswizzle_blockRGB32 }, // 32
	{ 4,4, 1, 6, &swizzle_blockRGBA, &unswizzle_blockRGBA }, // 32+A
	{ 3,4, 1, 6, 0, &unswizzle_blockRGB16A } // 16+A interleaved
};

void enc_setup_ch( encoder_t* enc, enc_component* comp, enc_info& swiz )
{
	uint32 bpp = pix_desc[ enc->channels ].pitch;
	uint32 nch = pix_desc[ enc->channels ].nchn;

	check( (bpp != 0) && (nch != 0) );

	// setup component description
	for ( uint32 n = 0; n != nch; n++ ) {
		uint32 qfactor = (enc->qfactor>>(ch_desc[n].qf_idx*4))  & 0x0f;

		comp[n].cdidx = ch_desc[n].cd_idx + 0;
		comp[n].dcenc = &enc->enc[ ch_desc[n].cd_idx + 0 ];
		comp[n].acenc = &enc->enc[ ch_desc[n].cd_idx + 1 ];
		comp[n].last_dc = 0;

		quant_init( comp[n].qmatrix, ch_desc[n].qm_type, qfactor, NULL );
	}

	// setup pel description
	swiz.func = pix_desc[ enc->channels ].efunc;
	swiz.pitch= bpp;

	swiz.aligned_width = enc->width;
	swiz.aligned_height= enc->height;
	swiz.stride	= swiz.pitch * swiz.aligned_width;
	swiz.ncod = pix_desc[ enc->channels ].ncod;
	swiz.nchn = pix_desc[ enc->channels ].nchn;
}

void enc_prof( encoder_t* enc, const void* bits, enc_component* eco, const enc_info& swiz )
{
	uint32 freqs[6*256];
	dpel_t blocks[4*64];

	// clear statistics
	memset( freqs, 0, sizeof(freqs) );

	for( uint32 h = 0; h < swiz.aligned_height; h+= 8 ) {
		for( uint32 w = 0; w < swiz.aligned_width; w+= 8 ) {
			// copy lines to the buffer
			const uint8* src = (const uint8*)bits + h * swiz.stride + w * swiz.pitch;
			swiz.func( src, swiz.stride, blocks );

			for( uint32 n = 0; n != swiz.nchn; n++ ) {
				dpel_t* blk = &blocks[64 * n];
				dpel_t* qmtx= eco[n].qmatrix;
				// dct blocks
				fdct16( blk );
				// quantize
				for( uint32 k = 0; k != 64; k++ ) blk[k] = quantize( blk[k], qmtx[k] );
				// profile
				uint32* frp = &freqs[256*eco[n].cdidx];
				profile_block( blk, eco[n].last_dc, frp, frp + 256 );
			}
		}
	}

	// now build huffman codes from gathered statistics
	// totally 4 - 6 tables (DCl,ACl,DCc,ACc)
	for( uint32 n = 0; n != swiz.ncod; n++ ) {
		build_huff( &freqs[n*256], enc->params[n] );
	}
}

uint32 enc_comp( encoder_t* enc, const void* in, void* out, enc_component* eco, const enc_info& swiz )
{
	// prepare bitstream
	put_bits_t pbits;
	init_put_bits( &pbits, (uint8*)out, 123456 );
	// write header info
	put_bits( &pbits, 12, enc->width );
	put_bits( &pbits, 12, enc->height );
	put_bits( &pbits, 8, enc->channels );
	put_bits( &pbits, 8, enc->qfactor );
	// write encoder data
	uint32 n;
	for( n = 0; n != swiz.ncod; n++ ) {
		uint32 valsize = n & 1 ? 162 : 17;
		put_params( enc->params[n], valsize, &pbits );
		// prepare encoder structures
		init_huff_enc( false, enc->params[n].bits, enc->params[n].vals, enc->enc[n] );
	}
	align_put_bits( &pbits );

	// proceed
	dpel_t blocks[4*64];

	eco[0].last_dc = eco[1].last_dc =
		eco[2].last_dc = eco[3].last_dc = 0;

	for( uint32 h = 0; h < swiz.aligned_height; h+= 8 ) {
		for( uint32 w = 0; w < swiz.aligned_width; w+= 8 ) {
			// copy lines to the buffer
			const uint8* src = (const uint8*)in + h * swiz.stride + w * swiz.pitch;
			swiz.func( src, swiz.stride, blocks );

			for( uint32 n = 0; n != swiz.nchn; n++ ) {
				dpel_t* blk = &blocks[64 * n];
				dpel_t* qmtx= eco[n].qmatrix;
				// dct blocks
				fdct16( blk );
				// quantize
				for( uint32 k = 0; k != 64; k++ ) blk[k] = quantize( blk[k], qmtx[k] );
				// encode
				encode_block( blk, eco[n].last_dc, *eco[n].dcenc, *eco[n].acenc, &pbits );
			}
		}
	}

	// flush bitstream
	flush_put_bits( &pbits );
	return (7+put_bits_count( &pbits )) / 8;
}

uint32 enc_do( encoder_t* enc, const void* in, void* out )
{
	// initalize info
	enc_component eco[4];
	enc_info swiz;

	// init channel information
	enc_setup_ch( enc, eco, swiz );
	// profile
	enc_prof( enc, in, eco, swiz );
	// compress
	return enc_comp( enc, in, out, eco, swiz );
}

void enc_done( encoder_t* enc )
{
}

void dec_init( decoder_t* dec, desc_t* desc, const void* in )
{
	dec->data_in = (void*)in;
	// read parameters from the stream
	get_bits_t gbits;
	init_get_bits( &gbits, (uint8*)in, 123456 );
	// read header info
	dec->width	= get_bits( &gbits, 12 );
	dec->height	= get_bits( &gbits, 12 );
	dec->channels=get_bits( &gbits, 8 );
	dec->qfactor =get_bits( &gbits, 8 );
	// save bitstream position
	dec->body_pos = gbits.index;

	// prepare quantization tables
	// (moved inside channel setup)
	//quant_init( dec->qluma, 0, (dec->qfactor>>4)&0x0f, ZAG );
	//quant_init( dec->qchro, 1, (dec->qfactor)&0x0f, ZAG );

	// write out description
	if ( desc != 0 ) {
		desc->width	= dec->width;
		desc->height= dec->height;
		desc->mode	= dec->channels;
	}
}

typedef void (*unswizzle_fn)(uint8* dst, uint32 stride, const dpel_t* buf);

struct dec_component
{
	huff_dtab_t*	dcdec;
	huff_dtab_t*	acdec;
	dpel_t   		qmatrix[64];
	dpel_t			last_dc;
};

struct dec_info
{
	void (*func)( uint8* dst, uint32 stride, const dpel_t* buf );
	uint32	pitch;
	uint32	stride;
	uint32	aligned_width;
	uint32	aligned_height;
	uint32	ncod;
	uint32	nchn;
};

void dec_setup_ch( decoder_t* dec, uint32 pixfmt, dec_component* comp, dec_info& unsw )
{
	// setup component description
	uint32 nchn = pix_desc[ pixfmt ].nchn;

	for ( uint32 n = 0; n != nchn; n++ ) {
		uint32 qfactor = (dec->qfactor >> (ch_desc[n].qf_idx*4)) & 0x0f;

		uint32 cdidx = ch_desc[n].cd_idx + 0;
		comp[n].dcdec = &dec->dec[ cdidx + 0 ];
		comp[n].acdec = &dec->dec[ cdidx + 1 ];
		comp[n].last_dc = 0;

		quant_init( comp[n].qmatrix, ch_desc[n].qm_type, qfactor, ZAG );
	}

	unsw.ncod = pix_desc[ pixfmt ].ncod;

	unsw.func		= pix_desc[ pixfmt ].dfunc;
	unsw.pitch		= pix_desc[ pixfmt ].pitch;
	unsw.stride		= pix_desc[ pixfmt ].pitch * dec->width;
	unsw.nchn		= pix_desc[ pixfmt ].nchn;

	unsw.aligned_width = dec->width;
	unsw.aligned_height= dec->height;
}

void dec_do( decoder_t* dec, void* out, uint32 bpp )
{
	// 1. initialize bitstream
	get_bits_t gbits;
	init_get_bits( &gbits, (uint8*)dec->data_in, 123456 );
	// rewind to position
	gbits.index = dec->body_pos;

	// 2. setup chanel structure
	dec_component dco[4];
	dec_info unsw;
	dec_setup_ch( dec, bpp, dco, unsw );

	// 3. read decoder tables
	// prepare encoder structures
	uint32 n;
	for( n = 0; n != unsw.ncod ; n++ ) {
		uint32 valsize = n & 1 ? 162 : 17;
		huff_params_t params;
		get_params( params, valsize, &gbits );
		init_huff_dec( false, params.bits, params.vals, dec->dec[n] );
	}
	// realign to start of coded bitstream
	//align_get_bits( &gbits );

	dpel_t blocks[4*64];

	// 4. proceed
	for( uint32 h = 0; h < unsw.aligned_height; h+= 8 ) {
		for( uint32 w = 0; w < unsw.aligned_width; w+= 8 ) {
			// clear data
			memset( blocks, 0, unsw.nchn*sizeof(dpel_t)*64 );

			for( n = 0; n != unsw.nchn; n++ ) {
				decode_block( &blocks[64*n], dco[n].last_dc, *dco[n].dcdec, *dco[n].acdec, dco[n].qmatrix, &gbits );
				idct16( &blocks[64*n] );
			}

			// entropy decode blocks
			// & dequantize + rearrange blocks
//			decode_block( &blocks[  0], last_dc1, dec->dec[0], dec->dec[1], dec->qluma, &gbits );
//			decode_block( &blocks[ 64], last_dc2, dec->dec[2], dec->dec[3], dec->qchro, &gbits );
//			decode_block( &blocks[128], last_dc3, dec->dec[2], dec->dec[3], dec->qchro, &gbits );
			// dct blocks
//			idct16( &blocks[0] );
//			idct16( &blocks[64] );
//			idct16( &blocks[128] );

			// unswizzle and copy back to memory
			uint8* dst = (uint8*)out + h * unsw.stride + w * unsw.pitch;
			unsw.func( dst, unsw.stride, blocks );
//			unswizzle_blockRGB24( dst, dec->width*3, blocks );
		}
	}

}

void dec_done( decoder_t* dec )
{
}



} //xjp

#ifdef DO_CHECK
#include <time.h>

uint32 pic[128*128];
uint32 buads[128];
uint8  buf[16384];
uint32 buadz[128];
uint32 pid[128*128];

#define QFACTOR 0x66


int main( int argc, char* argv[] )
{
	memset( pic, 0, sizeof(pic) );
	memset( buf, 0, sizeof(buf));
	FILE* f = fopen("title128.raw","rb");
	fread( pic, 128*128*3, 1, f );
	fclose(f);

	xjp::encoder_t en;

	printf("encoding...\n");
	xjp::enc_init( &en, 128, 128, 3, QFACTOR );
	//xjp::enc_prof( &en, pic );
	uint32 sz = xjp::enc_do( &en, pic, buf );
	xjp::enc_done( &en );

	f = fopen("out.bin", "wb");
	fwrite( buf, sz, 1, f );
	fclose(f);

	printf("decoding...\n");
	xjp::decoder_t de;
	xjp::dec_init( &de, 0, buf );

	time_t start = clock();
	for( size_t n = 0; n != 100; n++ ) {
		xjp::dec_do( &de, pid );
	}
	time_t end = clock();
	printf("%f per sec\n", 100.f / ((end - start) / 1000.f)  );
	xjp::dec_done( &de );

	printf("done dec..\n");
	f = fopen("out.ppm", "wb");
	fprintf( f, "P6\n%d %d\n255\n", 128, 128 );
	fwrite( pid, 128*128*3, 1, f );
	fclose(f);


	return 0;
}

#endif //DO_CHECK



