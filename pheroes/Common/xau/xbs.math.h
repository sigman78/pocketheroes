// 
// 2007.01.06 : SiGMan - initial version
// 2007.01.08 : SiGMan - signed int packing/unpacking revised
// 2007.01.09 : SiGMan - tweaked for optimal ARM asm
// 2007.12.20 : SiGMan - refactored as part of XBS

#ifndef XBS_MATH_H__
#define XBS_MATH_H__

#include "xbs.cfg.h"

// Since small CPUs have tiny cache and are bad in rnd mem read
// we provide two alternative versions of our math functions.
#ifdef XBS_NOLOOKUP

// returns bitmask to 'cover' given number of bits starting from LSB
static always_inline
uint32 xbs_bitmask( uint32 x ) 
{ return (1UL << x)-1; }

// returns 2^x
static always_inline
uint32 xbs_bitshift( uint32 x ) 
{ return (1UL << x); }

// returns 'capped' : 2^(x+4) or 0x80000000
static always_inline
uint32 xbs_shift16( uint32 x ) 
{ const uint32 r = 1UL << (4+x); return r ? r : 0x80000000; }

#else

static const uint32 xbs_bitmask_tbl[] = 
{
    0x00000000, 0x00000001, 0x00000003, 0x00000007,
    0x0000000f, 0x0000001f, 0x0000003f, 0x0000007f,
    0x000000ff, 0x000001ff, 0x000003ff, 0x000007ff,
    0x00000fff, 0x00001fff, 0x00003fff, 0x00007fff,
    0x0000ffff, 0x0001ffff, 0x0003ffff, 0x0007ffff,
    0x000fffff, 0x001fffff, 0x003fffff, 0x007fffff,
    0x00ffffff, 0x01ffffff, 0x03ffffff, 0x07ffffff,
    0x0fffffff, 0x1fffffff, 0x3fffffff, 0x7fffffff,
    0xffffffff
};

static const uint32 xbs_bitshift_tbl[] = 
{
    0x00000001, 0x00000002, 0x00000004, 0x00000008,
    0x00000010, 0x00000020, 0x00000040, 0x00000080,
    0x00000100, 0x00000200, 0x00000400, 0x00000800,
    0x00001000, 0x00002000, 0x00004000, 0x00008000,
    0x00010000, 0x00020000, 0x00040000, 0x00080000,
    0x00100000, 0x00200000, 0x00400000, 0x00800000,
    0x01000000, 0x02000000, 0x04000000, 0x08000000,
    0x10000000, 0x20000000, 0x40000000, 0x80000000,
    0x80000000, 0x80000000, 0x80000000, 0x80000000,
    0x80000000, 0x80000000, 0x80000000, 0x80000000
};

static const uint32* xbs_shift16_tbl = xbs_bitshift_tbl + 4;

// table lookup versions
#define xbs_bitmask( x ) 	xbs_bitmask_tbl[x]
#define xbs_bitshift( x )   xbs_bitshift_tbl[x]
#define xbs_shift16( x )	xbs_shift16_tbl[x]

#endif

// which type to use - byte or int ?
static const uint32 xbs_log_tab[] = 
{
  0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
  5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};

// calculates integer log2 using table lookup
static always_inline 
uint32 xbs_log2( uint32 val )
{ 
	register unsigned int t, tt;
	// uses only 1 load !
	if ( tt = val >> 16 ) {
		return (t = tt >> 8) ? 24 + xbs_log_tab[t] : 16 + xbs_log_tab[tt];
	} else {
		return (t = val >> 8) ? 8 + xbs_log_tab[t] : xbs_log_tab[val];
	}
}

// same but defined only for 'val' which <= 0xffff
static always_inline 
uint32 xbs_log2_16bit( uint32 val )
{ 
	register unsigned int t;

	return (t = val >> 8) ? 8 + xbs_log_tab[t] : xbs_log_tab[val];
}

// pack the signed quantity into unsigned with sign shift
static always_inline
uint32 xbs_sign_pack( sint32 x )
{
//	return (uint32)(x > 0 ? (x<<1)-1 : -(x<<1));
  sint32 v = -2*(-x)-1;
  return v ^ (v>>31);
	
}

// unpacks sign shifted unsigned into original signed value
static always_inline
sint32 xbs_sign_unpack( uint32 x )
{
//	return (sint32)( x&1 ? (++x)>>1 : -(x>>1) );
//	x++;
//	return x&1 ? x>>1 : -(x>>1);
	sint32 r = (sint32)x;
	return -((r>>1) ^ -(r&1));
}

#endif //XBS_MATH_H__

