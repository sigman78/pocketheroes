//
//

#include "stdafx.h"
//#include <stdio.h>
//#include "xjp.boot.h"

#include "xbs.bs.h"
#include "xbs.bs.impl.h"
#include "xbs.math.h"

#include "xjp.huff.h"
#include "xjp.huff.impl.h"

#include "xjp.dct.h"
#include "xjp.quant.h"
#include "xjp.block.h"

namespace xjp {

const uint8 ZAG[64] =
{
  0,  1,  8, 16,  9,  2,  3, 10,
 17, 24, 32, 25, 18, 11,  4,  5,
 12, 19, 26, 33, 40, 48, 41, 34,
 27, 20, 13,  6,  7, 14, 21, 28,
 35, 42, 49, 56, 57, 50, 43, 36,
 29, 22, 15, 23, 30, 37, 44, 51,
 58, 59, 52, 45, 38, 31, 39, 46,
 53, 60, 61, 54, 47, 55, 62, 63,
};

// H-V order
const uint8 _ZAG[64] =
{
 0, 1, 9, 8,16,17,18,10,
 2, 3,11,19,27,26,25,24,
32,33,34,35,36,28,20,12,
 4, 5,13,21,29,37,45,44,
43,42,41,40,48,49,50,51,
52,53,54,46,38,30,22,14,
 6, 7,15,23,31,39,47,55,
63,62,61,60,59,58,57,56
};

// hsnake
const uint8 ___ZAG[64] =
{
 0, 1, 2, 3, 4, 5, 6, 7,
15,14,13,12,11,10, 9, 8,
16,17,18,19,20,21,22,23,
31,30,29,28,27,26,25,24,
32,33,34,35,36,37,38,39,
47,46,45,44,43,42,41,40,
48,49,50,51,52,53,54,55,
63,62,61,60,59,58,57,56
};

// hsnake-zag
const uint8 ____ZAG[64] =
{
 0, 1, 8, 9, 2, 3,10,11,
 4, 5,12,13, 6, 7,14,15,
16,17,24,25,18,19,26,27,
20,21,28,29,22,23,30,31,
32,33,40,41,34,35,42,43,
36,37,44,45,38,39,46,47,
48,49,56,57,50,51,58,59,
52,53,60,61,54,55,62,63
};

// hilbert
const uint8 _____ZAG[64] = 
{
 0, 1, 9, 8,16,24,25,17,
18,26,27,19,11,10, 2, 3,
 4,12,13, 5, 6, 7,15,14,
22,23,31,30,29,21,20,28,
36,44,45,37,38,39,47,46,
54,55,63,62,61,53,52,60,
59,58,50,51,43,35,34,42,
41,33,32,40,48,49,57,56
};

// helpers
#define num_bits(x) (x ? 1 + xbs_log2_16bit(x) : 0 )
#define num_bits_nz(x) (1 + xbs_log2_16bit(x))

// read jpeg style code vlc (sign bit + matnissa w/ no MSB )
static 
always_inline sint32 get_xbits(get_bits_t *s, sint32 n)
{
    register sint32 sign;
    register sint32 cache;
    OPEN_READER(re, s)
    UPDATE_CACHE(re, s)
    cache = GET_CACHE(re,s);
    sign=(~cache)>>31;
    LAST_SKIP_BITS(re, s, n)
    CLOSE_READER(re, s)
    return (NEG_USR32(sign ^ cache, n) ^ sign) - sign;
}

//
// encode single pel block in bitstream
void
encode_block( const dpel_t* pels, dpel_t& last_dc, const huff_etab_t& dcoder, const huff_etab_t& acoder, put_bits_t* pb )
{
	sint32 tmp1, tmp2;

	// encode dc coeff
	tmp1 = tmp2 = pels[0] - last_dc;
	last_dc = pels[0];

	if ( tmp1 < 0 ) {
		tmp1 = -tmp1;
		tmp2--;
	}

	uint32 nbits = num_bits( tmp1 );
	check( nbits <= 16 );

	// emit huffman for dc exp
	put_huffman( pb, dcoder, nbits );
	if ( nbits ) {
		put_bits( pb, nbits, tmp2 & xbs_bitmask(nbits) );
	}

	// encode ac coefficients
	uint32 runlen = 0;
	for( size_t k = 1; k != 64; k++ ) {
		if ((tmp1 = pels[ZAG[k]]) == 0) {
			// its a run
			runlen++;
		} else {
			// if runlen > 15, emit 16-run code
			while ( runlen > 15 ) {
				put_huffman( pb, acoder, 0xf0 );
				runlen -= 16;
			}

			tmp2 = tmp1;
			if ( tmp1 < 0 ) {
				tmp1 = -tmp1;
				tmp2--;
			}

			// must be at least 1 bit long
			nbits = num_bits_nz( tmp1 );
			//if ( !nbits ) nbits = 1;
			check( nbits <= 16 );

//			get_bits_t gb;
//			init_get_bits( &gb, pb->buf, 123123 );
//			gb.index = pb->index;
			// emit huffman prefix for combined {r-len,exp}
			uint32 code = (runlen << 4) + nbits;
			put_huffman( pb, acoder, code );
			put_bits( pb, nbits, tmp2 & xbs_bitmask(nbits) );
			// verify
//			uint32 cw = get_bits( &gb, acoder.len[code] );
//			uint32 mw = get_bits( &gb, nbits );
//			check( cw == acoder.code[code] );
//			check( mw = tmp2 & xbs_bitmask(nbits) );

			// reset run
			runlen = 0;
		}
	}

	// put end mark if last coeff was zero
	if ( runlen != 0 ) {
		put_huffman( pb, acoder, 0 );
	}
}

//
// profiles single block to retrieve symbol frequencies
void
profile_block( const dpel_t* pels, dpel_t& last_dc, uint32 freq_dc[], uint32 freq_ac[] )
{
	sint32 tmp1, tmp2;

	// encode dc coeff
	tmp1 = tmp2 = pels[0] - last_dc;
	last_dc = pels[0];

	if ( tmp1 < 0 ) {
		tmp1 = -tmp1;
		tmp2--;
	}

	uint32 nbits = num_bits( tmp1 );
	check( nbits <= 16 );

	// emit huffman for dc exp
	freq_dc[nbits]++;

	// encode ac coefficients
	uint32 runlen = 0;
	for( size_t k = 1; k != 64; k++ ) {
		if ((tmp1 = pels[ZAG[k]]) == 0) {
			// its a run
			runlen++;
		} else {
			// if runlen > 15, emit 16-run code
			while ( runlen > 15 ) {
				freq_ac[0xf0]++;
				runlen -= 16;
			}

			tmp2 = tmp1;
			if ( tmp1 < 0 ) {
				tmp1 = -tmp1;
				tmp2--;
			}

			// must be at least 1 bit long
			nbits = num_bits_nz( tmp1 );
			//if ( !nbits ) nbits = 1;
			check( nbits <= 16 );

			// emit huffman prefix for combined {r-len,exp}
			uint32 code = (runlen << 4) + nbits;
			freq_ac[code]++;

			// reset run
			runlen = 0;
		}
	}

	// put end mark if last coeff was zero
	if ( runlen != 0 ) {
		freq_ac[0]++;
	}
}

//
////////////////////////////////////////////////////////////////////////

#if XAU_NOLOOKUP
#	define HUFF_EXTEND(x,s)  ((x) < (1<<((s)-1)) ? (x) + (((-1)<<(s)) + 1) : (x))
#else
#	define HUFF_EXTEND(x,s)  ((x) < extend_test[s] ? (x) + extend_offset[s] : (x))
static const sint32 extend_test[16] =   /* entry n is 2**(n-1) */
  { 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080,
    0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000 };
static const sint32 extend_offset[16] = /* entry n is (-1 << n) + 1 */
  { 0, ((-1)<<1) + 1, ((-1)<<2) + 1, ((-1)<<3) + 1, ((-1)<<4) + 1,
    ((-1)<<5) + 1, ((-1)<<6) + 1, ((-1)<<7) + 1, ((-1)<<8) + 1,
    ((-1)<<9) + 1, ((-1)<<10) + 1, ((-1)<<11) + 1, ((-1)<<12) + 1,
    ((-1)<<13) + 1, ((-1)<<14) + 1, ((-1)<<15) + 1 };
#endif 


//
// decodes single block
// [RRRRMMMM]
// R={ 0..15 }
// S={ 1..10 }
// totaly 160 codewords + 2 special
// {0,0} - EOB
// {F,0} - ZRL = run of 16
void
decode_block_( dpel_t* pels, dpel_t& last_dc, const huff_dtab_t& dcoder, const huff_dtab_t& acoder, get_bits_t* gb )
{
    //memset( pels, 0, sizeof(dpel_t)*64);
	sint32 r;
	sint32 s = get_huffman( gb, dcoder );
	if ( s ) {
		s = get_xbits( gb, s );
		//r = get_bits( gb, s );
		//s = HUFF_EXTEND( r, s );
	}

	s += last_dc;
	last_dc = pels[0] = (sint16)s;
	

	size_t k;
	for( k = 1; k < 64; k++ ) {
		s = get_huffman( gb, acoder );
		r = s >> 4;
		s &= 0x0f;

		if ( s ) {
		    // clear r here
			k += r;
			//r = get_bits( gb, s );
			//s = HUFF_EXTEND( r, s );
			s = get_xbits( gb, s );
			// output pel
			pels[ZAG[k]]= (dpel_t)s;
		} else {
			// correct run15 code is (1111 0000 = 0xf0)
			// otherwise ( 1111 xxxx ) is eob
			if ( r != 15 ) break;
			k += 15;
		}
	}
}


void
decode_block( dpel_t* pels, dpel_t& last_dc, const huff_dtab_t& dcoder, const huff_dtab_t& acoder, const dpel_t* qm, get_bits_t* gb )
{
	// moved to outer loop - memset( pels, 0, sizeof(dpel_t)*64);
	OPEN_READER( reader, gb );
	UPDATE_CACHE( reader, gb );

	uint32 index= SHOW_UBITS( reader, gb, huff_maxlookahead ); 
	sint32 bits	= dcoder.vlc[index].bits;
	sint32 s    = dcoder.vlc[index].code;

	if ( bits < 0 ) {
		SKIP_BITS( reader, gb, 8 );
		bits  = -bits;
		index = SHOW_UBITS( reader, gb, bits ) + s;
		bits  = dcoder.vlc[index].bits;
		s 	  = dcoder.vlc[index].code;
	}
	LAST_SKIP_BITS( reader, gb, bits );
    UPDATE_CACHE( reader, gb )

	if ( s ) {
	    sint32 cache = GET_CACHE( reader, s );
	    sint32 sign  = (~cache)>>31;
	    LAST_SKIP_BITS( reader, gb, s )
    	s = (NEG_USR32(sign ^ cache, s) ^ sign) - sign;
	}

	s += last_dc;
	last_dc = (sint16)s;
	pels[0] = dequantize( s, qm[0] );
	

	size_t k;
	for( k = 1; k < 64; k++ ) {
		UPDATE_CACHE( reader, gb );
		
		index= SHOW_UBITS( reader, gb, huff_maxlookahead ); 
		bits = acoder.vlc[index].bits;
		s    = acoder.vlc[index].code;

		if ( bits < 0 ) {
			SKIP_BITS( reader, gb, 8 );
			bits  = -bits;
			index = SHOW_UBITS( reader, gb, bits ) + s;
			bits  = acoder.vlc[index].bits;
			s 	  = acoder.vlc[index].code;
		}
		SKIP_BITS( reader, gb, bits );

		uint32 r = s >> 4;
		s &= 0x0f;

		if ( s ) {
		    // clear r here
			k += r;

			if ( s > 8 ) {
				UPDATE_CACHE( reader, gb );
			}

		    sint32 cache = GET_CACHE( reader, s );
		    sint32 sign  = (~cache)>>31;
		    LAST_SKIP_BITS( reader, gb, s )
    		s = (NEG_USR32(sign ^ cache, s) ^ sign) - sign;
			
			// output pel
			pels[ZAG[k]]= dequantize( s, qm[k] );
		} else {
			// early out?
			if ( r != 15 ) break;
			k += 15;
		}
	}
	CLOSE_READER( reader, gb );
}


} // xjp


#ifdef DO_TEST

char* tobin( uint32 val, uint32 len )
{
	static char buf[65];

	if ( len == 0 || len > 31 ) {
		strcpy( buf, "<?>" );
	} else {
		uint32 mask = 1 << ( len - 1 );
		char* p = buf;
		while ( mask ) {
			*p++ = (val&mask) ? '1' : '0';
			mask >>= 1;
		}
		*p = 0;
	}
	return buf;
}

void flush_stats( uint32 freq[], const char* fname )
{
	FILE* f = fopen(fname,"wt");

	uint32 n, total = 0;
	for( n = 0; n != xjp::huff_maxsym; ++n ) total += freq[n];

	for( n = 0; n != xjp::huff_maxsym; ++n ) {
		//if ( n == 4 ) fprintf( f, "---\n" );
		fprintf( f, "%2d %6d %4.3f%%\n", n, freq[n], 100.f * freq[n] / total );
	}

	fprintf( f, "---\n" );	

	xjp::huff_params_t tbl;
	xjp::build_huff( freq, tbl );

	for ( n = 0; n != 17; n++ )
		fprintf( f, "%3d ", tbl.bits[n] );
	fprintf(f,"\n");
	for ( n = 0; n != 256; n++ ) {
		fprintf( f, "%3d ", tbl.vals[n] );
		if ( n % 16 == 15 ) fprintf(f,"\n");
	}

	xjp::huff_etab_t et;
	memset( &et, 0, sizeof(et) );
	xjp::init_huff_enc( false, tbl.bits, tbl.vals, et );

	for( n = 0; n != xjp::huff_maxsym; ++n ) {
		fprintf( f, "%3d %3d %16s\n", n, et.len[n], tobin(et.code[n], et.len[n] ) );
	}

	fprintf( f, "---\n");
	xjp::huff_dtab_t dt;
	xjp::init_huff_dec( false, tbl.bits, tbl.vals, dt );
	for( n = 0; n != dt.allocated; ++n ) {
		fprintf( f, "%s %02x %02x\n", tobin( n, 8 ), dt.vlc[n].bits, dt.vlc[n].code );
	}

	fflush(f);

	fclose(f);
}

short m3[64*4] = 
{
  128,  -512,   512,   -255,   255,   1,   -1,   0,
  2,   0,   -1,   0,   0,   3,   0,   1,
   0,   1,   0,   1,   2,   1,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0 };


short m2[64] = {
	256,0,0,0,0,0,0,0,
	154,0,0,0,0,0,0,0,
	95,0,0,0,0,0,0,0,
	44,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	-26,0,0,0,0,0,0,0,
	-30,0,0,0,0,0,0,0,
	-18,0,0,0,0,0,0,0};

short mr[64*4] = { 0 };

uint8 acbits[17] = { 0,1,0,3,0,3,0,0,0,0,0,0,0,0,0,0,0};
uint8 acvals[256]   = { 0,0x75,0x76,0x77,0x74,0x78,0xf5,0,};

int main( int argc, char* argv[] )
{
	uint32 acfreqs[257], dcfreqs[257];
	xjp::dpel_t pel = 0;

	memset( acfreqs, 0, sizeof(acfreqs) );
	memset( dcfreqs, 0, sizeof(dcfreqs) );
	xjp::profile_block( m2, pel, dcfreqs, acfreqs );

//	flush_stats( dcfreqs, "dcstat.txt");
//	flush_stats( acfreqs, "acstat.txt");

	// prepare 
	xjp::huff_params_t dcpro, acpro;
	xjp::build_huff( dcfreqs, dcpro );
	xjp::build_huff( acfreqs, acpro );

	xjp::huff_etab_t dcenc, acenc;
	memset( &dcenc, 0, sizeof(dcenc) );
	memset( &acenc, 0, sizeof(acenc) );
	xjp::init_huff_enc( false, dcpro.bits, dcpro.vals, dcenc );
//	xjp::init_huff_enc( false, acpro.bits, acpro.vals, acenc );
	xjp::init_huff_enc( false, acbits, acvals, acenc );

	uint8 bytes[16384];
	put_bits_t pb;
	init_put_bits( &pb, bytes, 16384 );

	pel = 0;
	xjp::encode_block( m2, pel, dcenc, acenc, &pb );

	flush_put_bits( &pb );
	printf(">> %d\n", put_bits_count( &pb ) );

	pel = 0;
	xjp:: huff_dtab_t dcdec, acdec;
	printf("init dec\n");
	xjp::init_huff_dec( false, dcpro.bits, dcpro.vals, dcdec );
//	xjp::init_huff_dec( false, acpro.bits, acpro.vals, acdec );
	xjp::init_huff_dec( false, acbits, acvals, acdec );

	printf("init get bits\n");
	get_bits_t gb;
	init_get_bits( &gb, bytes, 16384 );

	printf("decode...\n");	
	xjp::decode_block( mr, pel, dcdec, acdec, &gb );
	printf("decoded...\n");

	for( size_t n = 0; n != 64; n++ ) {
		if ( mr[n] != m2[n] ) {
			printf("mismatch at #%d must be %d, got %d\n", n, m2[n], mr[n] );
		}
	}

	printf("done...\n");

	return 0;
}


#endif //DO_TEST
