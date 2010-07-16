#ifndef MIPS
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <time.h>
#endif // MIPS

typedef char sint8;
typedef unsigned char uint8;
typedef short sint16;
typedef unsigned short uint16;
typedef int sint32;
typedef unsigned int uint32;
typedef unsigned __int64 uint64;

#include "xau.cfg.h"
#include "xau.audio.h"
#include "xau.audio.cpp"

/*
inline char* binstr( uint32 val, int bits )
{
	static char buff[128];
	uint32 m = 1 << (bits-1);
	char* b = buff;
	while ( m ) {
		*b++ = (m & val)?'1':'0';
		m >>= 1;
	}
	*b = 0;
	return buff;
}

sint32 smp[85536];
sint32 org[85536];
size_t slen;

void read( const char* fn )
{
	uint8 t;
	FILE* f = fopen( fn, "rb" );
	fseek( f, 0, SEEK_END );
	slen = ftell(f);
	fseek( f, 0, SEEK_SET );
	sint32* p = smp;
	for( int n = 0; n != slen; n++ ) {
		fread( &t, 1, 1, f );
		*p++ = (sint8)(t^0x80);
	}
	fclose(f);
}

void write( const char* fo )
{
	FILE* f = fopen( fo, "wb");
	fwrite( smp, 4, slen, f );
	fclose(f);
}

int main()
{
	uint8 	buf[40000];

	read( "sawmill.raw" );
	memcpy( org, smp, sizeof(smp) );

	xau_codec_t codec;
	init_put_bits( &codec.pb, (uint8*)buf, sizeof(buf) );
	rice_init( codec.rice, 2, 2 );
	ttafilter_init( &codec.flt, 9 );

	max_k = max_u = max_l = 0;

	printf("starting..\n");
	encode( &codec, smp, slen );

	printf("done!\n");

	align_put_bits( &codec.pb );
	uint32 written = put_bits_count( &codec.pb );
	printf( "%d bits / %d bytes writen\n", written, written / 8 );
	printf( "%8.5f bits / sample\n", (float)written / slen );
	printf( "maxk = %d maxu = %d maxl = %d\n", max_k, max_u, max_l );

	for( int k = 0; k != 32; ++k ) {
		printf("%s ", binstr( buf[k],8 ) );
	}
	printf("\n");

	write("prc.int");

	printf("decoding...\n");

	
	init_get_bits( &codec.gb, (uint8*)buf, sizeof(buf) );
	rice_init( codec.rice, 2, 2 );
	ttafilter_init( &codec.flt, 9 );
	
	decode( &codec, smp, slen );

	for( uint32 z = 0; z != slen; ++z ) {
		if ( org[z] != smp[z] ) {	
			printf("error at %d !\n", z );
			return -1;
		}
	}

	printf("decode benchmark\n");

	time_t run_time = clock();
	for( int zu = 0; zu != 100; ++zu ) {
		init_get_bits( &codec.gb, (uint8*)buf, sizeof(buf) );
		rice_init( codec.rice, 2, 2 );
		ttafilter_init( &codec.flt, 9 );
	
		decode( &codec, smp, slen );
	}
	run_time = clock() - run_time;
	float one_time = run_time * (1.f/CLOCKS_PER_SEC) * 0.01f;
	printf("time %f sec \n", one_time );
	printf("%8.5f k-samples / per sec\n", (slen/1024.f) / one_time );


	return 0;
}
*/

size_t fsize( FILE* f )
{
	size_t old = ftell( f );
	fseek( f, 0, SEEK_END );
	size_t siz = ftell( f );
	fseek( f, old, SEEK_SET );
	return siz;
}

int main( int argc, char* argv[] )
{
	FILE* fIn = 0;

   	if ( argc < 1 || argv[1] == 0 || 0==(fIn=fopen(argv[1],"rb")) ) {
   		printf("cpcm [pcm-file-name]\n");
   		return -1;
   	}

	size_t inSize = fsize( fIn );
	sint8* pcmIn = (sint8*)malloc( inSize );
	fread( pcmIn, inSize, 1, fIn );
	fclose( fIn );
	printf("'%s' pcm file readen, %d samples\n", argv[1], inSize );

	uint8* outBuf  = (uint8*)malloc( inSize * 2 ); // outbuffer size 
	size_t outSize = xau_pcm8m_encode( pcmIn, inSize, outBuf, inSize );

	printf("%d samples compressed to %d bytes\n", inSize, outSize );
	printf("%6.3f bits per sample, total %d bits\n", (8.f*outSize)/inSize, 8*outSize );
	printf("%5.2f %% ratio\n", outSize*100.f / inSize );

	sint8* pcmCheck = (sint8*)malloc( inSize );
	size_t pcmSize = xau_pcm8m_decode( outBuf, outSize, pcmCheck, inSize );

	if ( pcmSize != inSize ) {
		printf("self-check: unpacked size mismatch!\n");
	} else {
		for( size_t n = 0; n != inSize; ++n ) {
			if ( pcmIn[n] != pcmCheck[n] ) {
				printf("self-check: mismatch at %d sample!\n", n );
				break;
			}
		}
	}

	free( pcmCheck );
	free( outBuf );
	free( pcmIn );
}

