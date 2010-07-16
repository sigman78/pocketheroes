//
// 2007-01-09 : SiGMan - Replacing byteswap with the more optimal
// 2007-01-09 : SiGMan - Profiling the unaligned read code for ARM

#ifndef XBS_BSWAP_H__
#define XBS_BSWAP_H__

#include "xbs.cfg.h"

static always_inline 
uint16 bswap_16( uint16 x )
{
    return (x>>8) | (x<<8);
}

static always_inline 
uint32 bswap_32( uint32 x )
{
//    x= ((x<<8)&0xFF00FF00) | ((x>>8)&0x00FF00FF);  return (x>>16) | (x<<16);
//  	return ( (x&0xff)<<24) | ((x&0xff00)<<8) | ((x>>8)&0xff00) | ((x>>24)&0xff);
	return (x>>24) | ((x>>8) & 0xff00) | ((x<<8) & 0xff0000) | (x<<24);
}

// be2me ... BigEndian to MachineEndian
// le2me ... LittleEndian to MachineEndian
#ifdef XBS_CPU_LE
#define be2me_16(x) bswap_16(x)
#define be2me_32(x) bswap_32(x)
#define le2me_16(x) (x)
#define le2me_32(x) (x)
#else
#define be2me_16(x) (x)
#define be2me_32(x) (x)
#define le2me_16(x) bswap_16(x)
#define le2me_32(x) bswap_32(x)
#endif


#ifdef XBS_NO_UNALIGNED_ACCESS
static always_inline uint16 unaligned16( const void* t )
{ return *(const __unaligned uint16*)t; }
static always_inline uint32 unaligned32( const void* t )
{ return *(const __unaligned uint32*)t; }
#else
#    define unaligned16(a) ( *(const uint16*)(a) )
#    define unaligned32(a) ( *(const uint32*)(a) )
#endif

static always_inline uint32 unaligned32_be(const void *v)
{
#ifdef XBS_UNALIGNED_BYTE_READ
        const uint8 *p= (const uint8*)v;
        return (((uint32)p[0]<<24) | ((uint32)p[1])<<16) | ((uint32)p[2]<<8) | (p[3]);
#else
		const uint32 val = unaligned32(v);
        return be2me_32( *val ); //original
#endif
}

static always_inline uint32 unaligned32_be_( const void* p )
{
	const uint8* bitptr = (const uint8*)p;
	uint32 bits = *bitptr++;		
	bits = (bits << 8) | *bitptr++;		
	bits = (bits << 8) | *bitptr++;		
	bits = (bits << 8) | *bitptr++;		
	return bits;
}

static always_inline uint32 unaligned32_le(const void *v)
{
#ifdef XBS_UNALIGNED_BYTE_READ
       const uint8 *p= (const uint8*)v;
       return (((p[3]<<8) | p[2])<<16) | (p[1]<<8) | (p[0]);
#else
       return le2me_32( unaligned32(v)); //original
#endif
}

#define NEG_SSR32(a,s) (((sint32)(a))>>(32-(s)))
#define NEG_USR32(a,s) (((uint32)(a))>>(32-(s)))

#endif //XBS_BSWAP_H__

