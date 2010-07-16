//
// huffman coder internals
// http://www.compressconsult.com/huffman/

#include "stdafx.h"
//#include "xjp.boot.h"

#include "xbs.bs.h"
#include "xbs.bs.impl.h"
#include "xjp.huff.h"
#include "xjp.huff.impl.h"

//#include <stdio.h>

char* tobin( uint32 val, uint32 len );


namespace xjp {

void init_huff_enc( bool isdc, const uint8* bits, const uint8* vals, huff_etab_t& etab )
{
	uint32 huffcode[257];
	uint8  huffsize[257];
	
	// decode code length table
	size_t p = 0;
	for ( size_t l = 1; l <= 16; l++ ) {
		uint8 inum = bits[l];
		// check constraints
		check( p + inum <= 256 );
		while ( inum-- ) huffsize[p++] = (uint8)l;
	}
	// finalize table
	huffsize[p] = 0;
	size_t lastp = p;

	// generate codes
	uint32 code = 0;
	uint8  sndx = huffsize[0];
	p = 0;
	while ( huffsize[p] ) {
		while ( huffsize[p] == sndx ) {
			huffcode[p++] = code;
			code++;
		}
		// check constraint
		check( code < ( 1UL << sndx ));
		// move to next
		code <<=1;
		sndx++;
	}

	memset( etab.vlc, 0, sizeof(etab.vlc) );
	// build table
	for( p = 0; p != lastp; p++ ) {
		size_t i = vals[p];
		//printf("%d = {%d,0x%04x}\n", i, huffsize[p], huffcode[p] );
		etab.vlc[i].bits = (sint16)huffsize[p];
		etab.vlc[i].code = (uint16)huffcode[p];
	}
}

#if 0
#define DBG(x) printf x
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
#else
#define DBG(x) 
#endif

static 
uint32
build_lookup( huff_dtab_t& dtab, uint32 table_nbits, uint32 ncodes, const uint8* bits, const uint32* codes, uint32 code_pfx, uint32 npfx )
{
	size_t table_size	= 1 << table_nbits;
	// "allocate" table of size (table_size)
	size_t table_index	= dtab.allocated;
	dtab.allocated += table_size;  
	// check bounds!

	DBG(("new table #%d of %d pfx:%x\n", table_index, table_size, code_pfx) );

	// current table to work
	vlc_t* table = &dtab.vlc[table_index];

	// clear table range
	uint32 i;
	for( i = 0; i != table_size; i++ ) {
		table[i].bits = 0;
		table[i].code = (sint16)-1;
	}

	DBG(("--- first pass ---\n"));

	// first pass - map codes and compute sizes
	for( i = 0; i != ncodes; i++ ) {
		sint32 n	= bits[i];
		uint32 code	= codes[i];

		DBG(("sym #%d  [%d] <%s>\n", i, n, tobin( code, n ) ));
		// hole in codes
		if ( n <= 0 ) continue;
		
		n -= npfx;
		uint32 code_pfx2 = code >> n;
		// if code matches the prefix, its in the table
		if ( n > 0 && code_pfx2 == code_pfx ) {
			if ( n <= (sint32)table_nbits ) {
				// inner data
				size_t entry = ( code << (table_nbits-n)) & (table_size - 1);
				size_t nb = 1 << (table_nbits-n);
			
				DBG(("%d %d %08x [%d](%d)\n", i, n, code, entry, nb ));
				for( size_t k = 0; k != nb; k++ ) {
					check( table[entry].bits == 0 );
					table[entry].bits = (sint16)n;
					table[entry].code = (uint16)i;
					DBG(("%s => (%d [%d])\n", tobin( entry, 8 ), i, n ));
					entry++;
				}
			} else {
				// aux table
				n -= table_nbits;
				size_t entry = ( code >> n  ) & ( (1<<table_nbits) - 1 );
				DBG(("%04x subtable  %d\n", entry, n ));
				
				// new subtable size
				sint32 n1 = -table[entry].bits;
				if ( n > n1 ) n1 = n;
				table[entry].bits = (sint16)-n1;
			}
		}
	} 

	DBG(("--- second pass ---\n"));
	// second pass - fill subtables
	for( i = 0; i < table_size; i++ ) {
		sint32 n = table[i].bits;
		// is it chained entry?
		if ( n < 0 ) {
			n = -n;
			if ( n > (sint32)table_nbits ) {
				// this shouldnt happen (we support only 2 stage lookup)
				check( 0 );
				n = table_nbits;
				table[i].bits = (sint16)-n; // marks subtable
			}
			
			size_t new_index = build_lookup( dtab, n, ncodes, bits, codes, ((code_pfx<<table_nbits) | i), npfx + table_nbits );
			// watch the realloc moved array
			table = &dtab.vlc[table_index];
			table[i].code =  new_index;
		}	
	}

	return table_index;
}

void init_huff_dec( bool isdc, const uint8* bits, const uint8* vals, huff_dtab_t& dtab )
{
	uint32 huffcode[257];
	uint8  huffsize[257];

	size_t p = 0;
	uint32 code = 0;
	memset( huffsize, 0, sizeof(huffsize) );
	for( size_t i = 1; i <= 16; i++ ) {
		uint32 nb = bits[i];
		for( size_t j = 0; j != nb; j++ ) {
			uint32 sym = vals[p++];
			//DBG(("#%d = %d <%s>\n",sym, i, tobin(code,i)));
			huffsize[sym] = (uint8)i;
			huffcode[sym] = code;
			//printf("%d = {%d,0x%04x}\n", sym, huffsize[sym], huffcode[sym] );
			code++;
		}
		code <<= 1;
	}

	size_t numsy = p;

	// prepare lookup tables 
	// (courtesy of ffmpeg library ;)
	memset( dtab.vlc, 0, sizeof(dtab.vlc ));
	dtab.allocated = 0;
	build_lookup( dtab, 8, 256, huffsize, huffcode, 0, 0 );
	DBG(("\ntotal allocated %d entries\n", dtab.allocated ));

	// validate
}

// TODO:: allocate memory dynamically
// as the current amount (~16K) is more then PPC has on stack
bool build_huff( uint32 syfreq[], huff_params_t& res )
{
	uint32 freq[ huff_hisize ];
	uint32 hist[ huff_maxinitclen + 1]; // histogram of symbols by length
	sint32 codesize[huff_hisize];	// length of code for symb K
	sint32 others[huff_hisize];		// next symbol in current branch
	sint32 n;

	// cleanup
	memcpy( freq, syfreq, sizeof(uint32)*256) ;
	memset( &res, 0, sizeof(res) );

	for( n = 0; n != 257; n++ ) {
		codesize[n] = 0;
		others[n] = -1;
	}

	// add end marker symbol with frequency 1
	freq[256] = 1;

	for(;;) {
		// find smallest nonzero freq, set to c1
		sint32 c1 = -1;
		uint32 v  =  1000000000L;

		for( n = 0; n != 257; n++ ) {
			if ( freq[n] && freq[n] <= v ) {
				v = freq[n];
				c1 = n;
			}
		}

		// find next smallest nonzero freq, set to c2
		sint32 c2 = -1;
		v  =  1000000000L;
		for( n = 0; n != 257; n++ ) {
			if ( freq[n] && freq[n] <= v && n != c1 ) {
				v = freq[n];
				c2 = n;
			}
		}

		// if not found - we are done
		if ( c2 == -1 ) break;

		// merge two  subtrees
		freq[c1] += freq[c2];
		freq[c2] = 0;

		// increment c1 branch
		codesize[c1]++;
		while ( others[c1] != -1 ) {
			c1 = others[c1];
			codesize[c1]++;
		}

		// chain c2 into c1
		others[c1] = c2;

		// increment c2 branch
		codesize[c2]++;
		while ( others[c2] != -1 ) {
			c2 = others[c2];
			codesize[c2]++;
		}
	}
	// cleanup 'hist' 
	memset( hist, 0, sizeof(hist) );

	// now count the number of symbols for each code length
	for( n = 0; n != 257; n++ ) {
		if ( codesize[n] ) {
			check( codesize[n] <= huff_maxinitclen );
			hist[codesize[n]]++;
		}
	}

	// readjust code length to meet criteria
	for ( n = huff_maxinitclen; n > huff_maxcodelen; --n ) {
		while ( hist[n] > 0 ) {
			uint32 j = n - 2; // find length of new prefix
			while ( hist[j] == 0 ) --j;

			hist[n] 	-= 2;	// remove two sy
			hist[n-1]   ++;		// one goes here
			hist[j+1]	+=2;	// two goes there
			hist[j]		--;		// that sy now is prefix
		}
	}

	// fix statistics for the EOS symbol
	while ( n && hist[n] == 0 )	n--;
	hist[n]--;

	// prepare output
	for( n = 0; n != 17; n++ )  
		res.bits[n] = (uint8)hist[n];

	// sort list of symbols according to code length
	size_t p = 0;
	for( n = 1; n <= huff_maxinitclen; n++ ) {
		for( size_t m = 0; m != 256; m++ ) {
			if ( codesize[m] == n ) {
				res.vals[p] = (uint8)m;
				++p;
			}
		}
	}
	check( res.bits[0] == 0 );
	return true;
}


} // xjp

#ifdef DO_TEST2

#include <cstdlib>
#include <cstring>

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

void flush_stats( uint32 freq[] )
{
	FILE* f = fopen("stats.txt","wt");

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
		fprintf( f, "%3d %3d %16s\n", n, et.len[n], tobin(et.code[n], et.len[n] ));
	}

	fflush(f);

	// encode decode
	uint8 bytes[16384];
	put_bits_t pb;
	init_put_bits( &pb, bytes, 16384 );

	const size_t nsym = 256;
	for( n = 0; n != nsym; n++ ) {
		if ( et.len[n] ) 
			xjp::put_huffman( &pb, et, n );
	}


	flush_put_bits( &pb );
	printf(">> %d\n", put_bits_count( &pb ) );

	xjp::huff_dtab_t dt;
	memset( &dt, 0, sizeof(dt) );
	xjp::init_huff_dec( false, tbl.bits, tbl.vals, dt );

	get_bits_t gb;
	init_get_bits( &gb, bytes, 16384 );
	for( n = 0; n != nsym; n++ ) {
		if ( !et.len[n] ) continue;
		uint32 s = xjp::get_huffman( &gb, dt );
		if ( s != n ) {
			printf("decode error at %d\n", n );
			break;
		}
	}

	fclose(f);
}

int main()
{
	uint32 freq[257] = { 0 };

//	for( size_t n = 0; n != 256; n++ ) freq[n] = n < 16 ? 256 - n : 1;
	freq[0] = 255;

	flush_stats( freq );

	printf("done...\n");
	return 0;
}

#endif //DO_TEST2

