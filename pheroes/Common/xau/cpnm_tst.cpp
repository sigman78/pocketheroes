#include <stdio.h>
#include <stdlib.h>

#include "xjp.boot.h"

#include "xjp.codec.h"
#include <time.h>

void* pic;
void* buf;
void* pid;

#define QFACTOR 0x66

int main( int argc, char* argv[] )
{
	if ( argc < 3 ) {
		printf("test rgb-raw-name raw-width raw-height\n");
		return -1;
	}

	FILE* f = fopen( argv[1], "rb" );
	if ( f == NULL ) {
		printf("cant open\n");
		return -1;
	}

	uint32 pw = atoi( argv[2] );
	uint32 ph = atoi( argv[3] );

	pic = malloc( pw * ph * 3 );
	buf = malloc( pw * ph * 3 );
	pid = malloc( pw * ph * 3 );

	memset( pic, 0, sizeof(pic) );
	memset( buf, 0, sizeof(buf));

	fread( pic, pw*ph*3, 1, f );
	fclose(f);

	xjp::encoder_t en;

	printf("encoding prepare...\n");
	xjp::enc_init( &en, pw, ph, 3, QFACTOR );
	printf("profiling...\n");
	//xjp::enc_prof( &en, pic );
	printf("working...\n");
	uint32 sz = xjp::enc_do( &en, pic, buf );
	xjp::enc_done( &en );

	printf("to size: %d bytes\n", sz );

	f = fopen("out.bin", "wb");
	fwrite( buf, sz, 1, f );
	fclose(f);

	printf("decoding...\n");
	xjp::decoder_t de;
	xjp::dec_init( &de, 0, buf );

	time_t start = clock();
	for( size_t n = 0; n != 10; n++ ) {
		xjp::dec_do( &de, pid, 3 );
	}
	time_t end = clock();
	printf("%f per sec\n", 10.f / ((end - start) / 1000.f)  );
	xjp::dec_done( &de );

	printf("done dec..\n");

	f = fopen("out.ppm", "wb");
	fprintf( f, "P6\n%d %d\n255\n", pw, ph );
	fwrite( pid, pw*ph*3, 1, f );
	fclose(f);

	free( pid );
	free( buf );
	free( pic );


	return 0;
}

