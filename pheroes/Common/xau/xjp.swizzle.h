//
// Swizzle - unswizzle functions
// to copy from/to image to DCT buffer block

#ifndef XJP_SWIZZLE_H__
#define XJP_SWIZZLE_H__

// no includes since its intended to be included in proper place

namespace xjp {

/*
static uint8 clamp_data[] = 
{ 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
	49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
	65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
	81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
	97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
	113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128,
	129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144,
	145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
	161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176,
	177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192,
	193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208,
	209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224,
	225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
	241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

static uint8* clamp_tab = clamp_data + 64;
*/

#define CLAMP_256( x ) ((x) < 0 ? 0 : ((x) > 255 ? 255 : (x)))
//#define CLAMP_256( x ) (clamp_tab[(x)])

#define _SWIZZLE_RGB32 \
		pel = *src++;  \
		rpel= pel & 0xff;\
		gpel= (pel >> 8) & 0xff;\
		bpel= (pel >>16) & 0xff;\
		buf[0]  = (sint16)gpel;\
		buf[64] = (sint16)(rpel - gpel);\
		buf[128]= (sint16)(bpel - gpel);\
		/* */

#define _UNSWIZZLE_RGB32 \
		gpel = CLAMP_256( buf[0] ); \
		rpel = CLAMP_256( buf[64] + gpel ); \
		bpel = CLAMP_256( buf[128]+ gpel ); \
		*dst++ = (bpel << 16) | (gpel << 8) | rpel; \
		/* */

#define SWIZZLE_RGB24_ \
		rpel= *src++;\
		gpel= *src++;\
		bpel= *src++;\
		buf[0]  = (sint16)rpel;\
		buf[64] = (sint16)(gpel);\
		buf[128]= (sint16)(bpel);\
		/* */

#define UNSWIZZLE_RGB24_ \
		rpel = buf[0]; \
		gpel = buf[64] ;\
		bpel = buf[128];\
		*dst++ = CLAMP_256( rpel ); \
		*dst++ = CLAMP_256( gpel ); \
		*dst++ = CLAMP_256( bpel ); \
		/* */

#define RGB_TO_YCOCG_Y( r, g, b )   ( ( (    r +   (g<<1) +  b     ) + 2 ) >> 2 )
#define RGB_TO_YCOCG_CO( r, g, b )  ( ( (   (r<<1)        - (b<<1) ) + 2 ) >> 2 )
#define RGB_TO_YCOCG_CG( r, g, b )  ( ( ( -  r +   (g<<1) -  b     ) + 2 ) >> 2 )
 
#define COCG_TO_R( co, cg )         ( co - cg )
#define COCG_TO_G( co, cg )         ( cg )
#define COCG_TO_B( co, cg )         ( - co - cg )

#define SWIZZLE_RGB24 \
		rpel= *src++;\
		gpel= *src++;\
		bpel= *src++;\
		buf[0]  = (sint16)RGB_TO_YCOCG_Y( rpel, gpel, bpel );\
		buf[64] = (sint16)RGB_TO_YCOCG_CO( rpel, gpel, bpel ) + 128;\
		buf[128]= (sint16)RGB_TO_YCOCG_CG( rpel, gpel, bpel ) + 128;\
		/* */

#define UNSWIZZLE_RGB24 \
		gpel = buf[0]; \
		rpel = buf[64] - 128;\
		bpel = buf[128]- 128;\
		*dst++ = (uint8)CLAMP_256( gpel + COCG_TO_R(rpel,bpel) ); \
		*dst++ = (uint8)CLAMP_256( gpel + COCG_TO_G(rpel,bpel) ); \
		*dst++ = (uint8)CLAMP_256( gpel + COCG_TO_B(rpel,bpel) ); \
		/* */

#define SWIZZLE_RGB32 \
		bpel= *src++;\
		gpel= *src++;\
		rpel= *src++;\
		buf[0]  = (sint16)RGB_TO_YCOCG_Y( rpel, gpel, bpel );\
		buf[64] = (sint16)RGB_TO_YCOCG_CO( rpel, gpel, bpel ) + 128;\
		buf[128]= (sint16)RGB_TO_YCOCG_CG( rpel, gpel, bpel ) + 128;\
		src++;\
		/* */

#define UNSWIZZLE_RGB32 \
		gpel = buf[0]; \
		rpel = buf[64] - 128;\
		bpel = buf[128]- 128;\
		*dst++ = (uint8)CLAMP_256( gpel + COCG_TO_B(rpel,bpel) ); \
		*dst++ = (uint8)CLAMP_256( gpel + COCG_TO_G(rpel,bpel) ); \
		*dst++ = (uint8)CLAMP_256( gpel + COCG_TO_R(rpel,bpel) ); \
		dst++;\
		/* */

#define UNSWIZZLE_RGB16 \
		gpel = buf[0]; \
		rpel = buf[64] - 128;\
		bpel = buf[128]- 128;\
		pel  = 0xf800 & (CLAMP_256( gpel + COCG_TO_R(rpel,bpel) ) << 8); \
		pel |= 0x07e0 & (CLAMP_256( gpel + COCG_TO_G(rpel,bpel) ) << 3); \
		pel |= 0x001f & (CLAMP_256( gpel + COCG_TO_B(rpel,bpel) ) >> 3); \
		*(uint16*)dst = (uint16)pel; dst+=2; \
		/* */

#define SWIZZLE_RGBA \
		bpel= *src++;\
		gpel= *src++;\
		rpel= *src++;\
		apel= *src++;\
		buf[0]  = (sint16)RGB_TO_YCOCG_Y( rpel, gpel, bpel );\
		buf[64] = (sint16)RGB_TO_YCOCG_CO( rpel, gpel, bpel ) + 128;\
		buf[128]= (sint16)RGB_TO_YCOCG_CG( rpel, gpel, bpel ) + 128;\
		buf[192]= (sint16)apel;
		/* */

#define UNSWIZZLE_RGBA \
		gpel = buf[0]; \
		rpel = buf[64] - 128;\
		bpel = buf[128]- 128;\
		apel = buf[192]; \
		*dst++ = (uint8)CLAMP_256( gpel + COCG_TO_B(rpel,bpel) ); \
		*dst++ = (uint8)CLAMP_256( gpel + COCG_TO_G(rpel,bpel) ); \
		*dst++ = (uint8)CLAMP_256( gpel + COCG_TO_R(rpel,bpel) ); \
		*dst++ = (uint8)CLAMP_256( apel ); \
		/* */

#define UNSWIZZLE_RGB16Ax2 \
		apel = ((CLAMP_256( buf[192] ) ) << 8) | (CLAMP_256( buf[193] ) ); \
		*(uint16*)dst = (uint16)apel; dst+=2; \
		gpel = buf[0]; \
		rpel = buf[64] - 128;\
		bpel = buf[128]- 128;\
		pel  = 0xf800 & (CLAMP_256( gpel + COCG_TO_R(rpel,bpel) ) << 8); \
		pel |= 0x07e0 & (CLAMP_256( gpel + COCG_TO_G(rpel,bpel) ) << 3); \
		pel |= 0x001f & (CLAMP_256( gpel + COCG_TO_B(rpel,bpel) ) >> 3); \
		*(uint16*)dst = (uint16)pel; dst+=2; \
		buf++; \
		gpel = buf[0]; \
		rpel = buf[64] - 128;\
		bpel = buf[128]- 128;\
		pel  = 0xf800 & (CLAMP_256( gpel + COCG_TO_R(rpel,bpel) ) << 8); \
		pel |= 0x07e0 & (CLAMP_256( gpel + COCG_TO_G(rpel,bpel) ) << 3); \
		pel |= 0x001f & (CLAMP_256( gpel + COCG_TO_B(rpel,bpel) ) >> 3); \
		*(uint16*)dst = (uint16)pel; dst+=2; \
		buf++; \
		/* */

		
static always_inline 
void swizzle_blockRGB24( const uint8* src, uint32 stride, dpel_t* buf) 
{
	const uint32 dstride = stride - 3 * 8;
	for( uint32 row = 0; row != 8; row++ ) {
		sint32 rpel, gpel, bpel;
		SWIZZLE_RGB24; buf++;
		SWIZZLE_RGB24; buf++;
		SWIZZLE_RGB24; buf++;
		SWIZZLE_RGB24; buf++;
		SWIZZLE_RGB24; buf++;
		SWIZZLE_RGB24; buf++;
		SWIZZLE_RGB24; buf++;
		SWIZZLE_RGB24; buf++;
		src += dstride;
	}
}

static always_inline 
void unswizzle_blockRGB24( uint8* dst, uint32 stride, const dpel_t* buf) 
{
	const uint32 dstride = stride - 3 * 8;
	for( uint32 row = 0; row != 8; row++ ) {
		sint32 rpel, gpel, bpel;
		UNSWIZZLE_RGB24; buf++;
		UNSWIZZLE_RGB24; buf++;
		UNSWIZZLE_RGB24; buf++;
		UNSWIZZLE_RGB24; buf++;
		UNSWIZZLE_RGB24; buf++;
		UNSWIZZLE_RGB24; buf++;
		UNSWIZZLE_RGB24; buf++;
		UNSWIZZLE_RGB24; buf++;
		dst += dstride;
	}
}

static always_inline 
void swizzle_blockRGB32( const uint8* src, uint32 stride, dpel_t* buf) 
{
	const uint32 dstride = stride - 4 * 8;
	for( uint32 row = 0; row != 8; row++ ) {
		sint32 rpel, gpel, bpel;
		SWIZZLE_RGB32; buf++;
		SWIZZLE_RGB32; buf++;
		SWIZZLE_RGB32; buf++;
		SWIZZLE_RGB32; buf++;
		SWIZZLE_RGB32; buf++;
		SWIZZLE_RGB32; buf++;
		SWIZZLE_RGB32; buf++;
		SWIZZLE_RGB32; buf++;
		src += dstride;
	}
}

static always_inline 
void unswizzle_blockRGB32( uint8* dst, uint32 stride, const dpel_t* buf) 
{
	const uint32 dstride = stride - 4 * 8;
	for( uint32 row = 0; row != 8; row++ ) {
		sint32 rpel, gpel, bpel;
		UNSWIZZLE_RGB32; buf++;
		UNSWIZZLE_RGB32; buf++;
		UNSWIZZLE_RGB32; buf++;
		UNSWIZZLE_RGB32; buf++;
		UNSWIZZLE_RGB32; buf++;
		UNSWIZZLE_RGB32; buf++;
		UNSWIZZLE_RGB32; buf++;
		UNSWIZZLE_RGB32; buf++;
		dst += dstride;
	}
}

static always_inline 
void unswizzle_blockRGB16( uint8* dst, uint32 stride, const dpel_t* buf) 
{
	const uint32 dstride = stride - 2 * 8;
	for( uint32 row = 0; row != 8; row++ ) {
		uint32 pel; sint32 rpel, gpel, bpel;
		UNSWIZZLE_RGB16; buf++;
		UNSWIZZLE_RGB16; buf++;
		UNSWIZZLE_RGB16; buf++;
		UNSWIZZLE_RGB16; buf++;
		UNSWIZZLE_RGB16; buf++;
		UNSWIZZLE_RGB16; buf++;
		UNSWIZZLE_RGB16; buf++;
		UNSWIZZLE_RGB16; buf++;
		dst += dstride;
	}
}

static always_inline 
void swizzle_blockRGBA( const uint8* src, uint32 stride, dpel_t* buf) 
{
	const uint32 dstride = stride - 4 * 8;
	for( uint32 row = 0; row != 8; row++ ) {
		sint32 rpel, gpel, bpel, apel;
		SWIZZLE_RGBA; buf++;
		SWIZZLE_RGBA; buf++;
		SWIZZLE_RGBA; buf++;
		SWIZZLE_RGBA; buf++;
		SWIZZLE_RGBA; buf++;
		SWIZZLE_RGBA; buf++;
		SWIZZLE_RGBA; buf++;
		SWIZZLE_RGBA; buf++;
		src += dstride;
	}
}

static always_inline 
void unswizzle_blockRGBA( uint8* dst, uint32 stride, const dpel_t* buf) 
{
	const uint32 dstride = stride - 4 * 8;
	for( uint32 row = 0; row != 8; row++ ) {
		sint32 rpel, gpel, bpel, apel;
		UNSWIZZLE_RGBA; buf++;
		UNSWIZZLE_RGBA; buf++;
		UNSWIZZLE_RGBA; buf++;
		UNSWIZZLE_RGBA; buf++;
		UNSWIZZLE_RGBA; buf++;
		UNSWIZZLE_RGBA; buf++;
		UNSWIZZLE_RGBA; buf++;
		UNSWIZZLE_RGBA; buf++;
		dst += dstride;
	}
}

// interlaced format, optimized for blitting
// in 16bit words :
// [A0 | A1] [Color0] [Color1]
static always_inline 
void unswizzle_blockRGB16A( uint8* dst, uint32 stride, const dpel_t* buf) 
{
	const uint32 dstride = stride - 3 * 8;
	for( uint32 row = 0; row != 8; row++ ) {
		sint32 pel, rpel, gpel, bpel, apel;
		UNSWIZZLE_RGB16Ax2;
		UNSWIZZLE_RGB16Ax2;
		UNSWIZZLE_RGB16Ax2;
		UNSWIZZLE_RGB16Ax2;
		dst += dstride;
	}
}

} //xjp

#endif //XJP_SWIZZLE_H__
