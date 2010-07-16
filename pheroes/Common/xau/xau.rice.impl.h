//
// 2007.01.08 : SiGMan - unary prefixes inverted, e.g. 000001 for 5
// 2007.01.09 : SiGMan - additional bit optimization and bit hunting
// 2007.01.10 : SiGMan - fixed problem in unary-code reader related for large codes
// 2007.12.20 : SiGMan - bs refactored to separate source
#ifndef XAU_RICE_IMPL_H__
#define XAU_RICE_IMPL_H__

#include "xau.cfg.h"
#include "xbs.bs.h"
#include "xbs.math.h"
#include "xau.rice.h"

// bitmask	= exp2(x)-1                 { 0, 1, 3, 7 ... }
// bitshift = min( exp2(x), 0x80000000 )
// shift16  = bitshift( x + 4 )

// handles arbitrary length unary
static always_inline
void put_unary( put_bits_t* pb, uint32 val )
{
	while ( val > 24 ) {
		put_bits( pb, 24, 0 );
		val -= 24;
	}

	put_bits( pb, val+1, 1 ); 
}

#if 1
// handles arbitrary length unary code
static always_inline
uint32 get_unary( get_bits_t* gb )
{
	register uint32 tmp;
	uint32 val = 0;

	OPEN_READER( reader, gb );
	UPDATE_CACHE( reader, gb );
	tmp = GET_CACHE( reader, gb ); // & 0xffffff00;

	while ( 0 == tmp )  {
		val += 24;
		SKIP_COUNTER( reader, gb, 24 );	
		UPDATE_CACHE( reader, gb );
		tmp = GET_CACHE( reader, gb ); // & 0xffffff00;
	}

	// now we definitely have a zero within, find where
#if 1
	tmp = 31-xbs_log2( tmp );
	val += tmp;
	LAST_SKIP_BITS( reader, gb, tmp+1 );
#else
	uint32 count = 0;
	while ( !(tmp & 0x80000000) ) { tmp <<= 1; count++; }
	val += count;
	LAST_SKIP_BITS( reader, gb, count+1 );
#endif

	CLOSE_READER( reader, gb );

	return val;
}
#else
static always_inline
uint32 get_unary( get_bits_t* gb )
{
	register uint32 tmp;
	uint32 val = 0;

	OPEN_READER( reader, gb );
	UPDATE_CACHE( reader, gb );

	tmp = SHOW_UBITS( reader, gb, 24 );
	while ( !tmp ) {
		val += 24;
		SKIP_COUNTER( reader, gb, 24 );	
		UPDATE_CACHE( reader, gb );
		tmp = SHOW_UBITS( reader, gb, 24 );
	}

	// now we definitely have a zero within, find where
	register uint32 count = 0;
	while ( !(tmp & xbs_bitshift(23)) ) {
		count++;
		tmp <<= 1;
	}

	// gotcha!
	val += count;
	LAST_SKIP_BITS( reader, gb, count+1 );
	CLOSE_READER( reader, gb );

	return val;
}
#endif

static always_inline
void put_golomb( put_bits_t* pb, uint32 v, uint32 k, uint32 limit, uint32 esc )
{
	uint32 prefix = v >> k;

	if ( prefix < limit ) {
		put_bits( pb, prefix + k + 1, (1UL << k) + (v & xbs_bitmask(k)) );
	} else {
		put_bits( pb, limit + esc, v - limit + 1 );
	}
}

static always_inline
uint32 get_golomb( get_bits_t* gb, uint32 v, uint32 k, uint32 limit, uint32 esc )
{
	OPEN_READER( rd, gb );
	UPDATE_CACHE( rd, gb );
	uint32 buf = GET_CACHE( rd, gb );

	uint32 l2 = xbs_log2( buf );
	
	if ( l2 > 31-limit ) {
		buf >>= l2 - k;
		buf += (30-l2)<<k;
		LAST_SKIP_BITS( rd, gb, 32 + k - l2 );
		CLOSE_READER( rd, gb );
		return buf;
	} else {
		buf >>= 32 - limit - esc;
		LAST_SKIP_BITS( rd, gb, esc + limit );
		CLOSE_READER( rd, gb );
		return buf + limit - 1;
	}
}

static inline 
void rice_init( rice_ctx_t& c, uint32 k0, uint32 k1 )
{
	c[0].k	= k0;
	c[0].sum= xbs_shift16( k0 );
	c[1].k	= k1;
	c[1].sum= xbs_shift16( k1 );
}


static always_inline
void rice_adapt( rice_param_t& r, uint32 val )
{
	const uint32 sum = r.sum += val - (r.sum >> 4);
#if 1
	if ( r.k > 0 && sum < xbs_shift16( r.k ) ) 
		r.k--;
	else if ( sum > xbs_shift16( r.k + 1 ) ) 
		r.k++;
#else
	uint32 addon = r.k ? -(sum < xbs_shift16( r.k )) : 0;
	addon = addon ? addon : sum > xbs_shift16( r.k + 1 );
	r.k += addon;
#endif
}

static always_inline
void rice_enc( put_bits_t* pb, rice_param_t* r, uint32 val )
{
	uint32 k = r[0].k;

	rice_adapt( r[0], val );

	uint32 unary;
	if ( val >= xbs_bitshift(k) ) {
		val -= xbs_bitshift(k);
		
		k = r[1].k;
		rice_adapt( r[1], val );

		unary = 1 + (val >> k);
	} else unary = 0;

	put_unary( pb, unary );
	
	if ( k ) {
		uint32 binary = val & xbs_bitmask(k);
		put_bits( pb, k, binary );
	}
}

static always_inline
uint32 rice_dec( get_bits_t* gb, rice_param_t* r )
{
	uint32 value = get_unary( gb );

	if ( value == 0 ) {
		uint32 k = r[0].k;

		if ( k ) {
			uint32 binary = get_bits( gb, k );
			value = (value << k) + binary;
		}

		rice_adapt( r[0], value );
	} else {
		uint32 k = r[1].k;
		value--;

		if ( k ) {
			uint32 binary = get_bits( gb, k );
			value = (value << k) + binary;
		}

		rice_adapt( r[1], value );
		value += xbs_bitshift( r[0].k );
		rice_adapt( r[0], value );
	}
	return value;
}

#endif //XAU_RICE_IMPL_H__

