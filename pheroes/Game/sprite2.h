/*
 * This file is a part of Pocket Heroes Game project
 * 	http://www.pocketheroes.net
 *	https://code.google.com/p/pocketheroes/
 *
 * Copyright 2004-2010 by Robert Tarasov and Anton Stuk (iO UPG)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */ 

#include "stdafx.h"

#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
#endif

/// Pixel type
typedef uint16 Pixel;

/// Blitter ops
struct SetOp
{
	always_inline void operator()( Pixel* dst, const Pixel* src ) const
	{ *dst = 0xffff; }
};

struct CopyOp
{
	always_inline void operator()( Pixel* dst, const Pixel* src ) const
	{ *dst = *src; }
};

struct ConstAlphaBlendOp
{
	uint8 alpha;
	ConstAlphaBlendOp(uint8 a) : alpha(a) {}
	always_inline void operator()( Pixel* dst, const Pixel* src) const
	{ 
/*

		if (!alpha) return;
		uint8 inv_a = 63-alpha;
		uint16 sr = alpha * ((*src & 0xF800) >> 11);
		uint16 sg = alpha * ((*src & 0x7E0) >> 5);
		uint16 sb = alpha * ((*src & 0x1F));
		uint16 dr = inv_a * ((*dst & 0xF800) >> 11);
		uint16 dg = inv_a * ((*dst & 0x7E0) >> 5);
		uint16 db = inv_a * ((*dst & 0x1F));
		*dst = (((sr+dr)>>6)<<11 | ((sg+dg)>>6)<<5 | ((sb+db)>>6));
*/
		const uint32 a  = *src;
		const uint32 b  = *dst;
		uint32 sb = a & 0x1f;
		uint32 sg = (a >> 5) & 0x3f;
		uint32 sr = (a >> 11) & 0x1f;
		uint32 db = b & 0x1f;
		uint32 dg = (b >> 5) & 0x3f;
		uint32 dr = (b >> 11) & 0x1f;

		*dst = (Pixel)(((alpha * (sb-db)) >> 6) + db
			| (((alpha * (sg-dg)) >> 6) + dg) << 5 
			| (((alpha * (sr-dr)) >> 6) + dr) << 11);
	}
};

struct Shadow50Op
{
	always_inline void operator()( Pixel* dst, const Pixel* ) const
	{ *dst = (Pixel)Darken50(*dst); }
};

struct Shadow25Op
{
	always_inline void operator()( Pixel* dst, const Pixel* ) const
	{ *dst = (Pixel)Darken25(*dst); }
};

struct DotedOp
{
	always_inline void operator()( Pixel* dst, const Pixel* ) const
	{ if ( !((size_t)dst & 3) ) *dst = (uint16)Darken50(*dst); /*else *dst = 0;*/ }
};

static Pixel tintGradient[32] = {
	0x4101,	0x4101,	0x4921,	0x4942,	0x5142,	0x5162,	0x5983,	0x61a3,
	0x61e3,	0x6a04,	0x7224,	0x7a65,	0x8285,	0x8ac6,	0x8ae6,	0x9327,
	0xa368,	0xa389,	0xabc9,	0xb40a,	0xbc4b,	0xc48c,	0xcccd,	0xd4ed,
	0xdd2e,	0xe54f,	0xed90,	0xedd0,	0xf5f1,	0xf611,	0xf632,	0xf652
};

struct TintShadow
{
	always_inline Pixel Blend( Pixel a, Pixel b ) const
	{
		return ( (a & 0xf7de) >> 1 ) + ((b & 0xf7de) >> 1);
	}
	always_inline void operator()( Pixel* dst, const Pixel* ) const
	{ 
		//static uint8 rpt = 31;
		Pixel chnl = (*dst >> 6) & 0x1f;
		//*dst = Darken25((chnl) | (chnl << 6) | (chnl << 11));
		*dst = Blend(tintGradient[chnl], 0x39e7 );
	} 
};


/// Blitter loop
/// With given: source ptr, dest ptr, counter, operation
//#define DUFFS_BLT
#ifndef DUFFS_BLT
#	define BLTLOOP( SRC, DST, CNT, OP ) \
	for( ; CNT != 0; --CNT, ++SRC, ++DST ) OP( DST, SRC ); \
	/* */
#define BLTLOOPBACK( SRC, DST, CNT, OP ) \
	for( ; CNT != 0; --CNT, ) OP ( --DST, --SRC ); \
	/* */
#else
#	define BLTLOOP( SRC, DST, CNT, OP ) \
	size_t tcnt_ = (CNT + 7) / 8;		\
	check( tcnt_ != 0 );				\
	switch( CNT % 8 ) {					\
	case 0: do{	OP( DST++, SRC++ );	\
	case 7:		OP( DST++, SRC++ );	\
	case 6:		OP( DST++, SRC++ );	\
	case 5:		OP( DST++, SRC++ );	\
	case 4:		OP( DST++, SRC++ );	\
	case 3:		OP( DST++, SRC++ );	\
	case 2:		OP( DST++, SRC++ );	\
	case 1:		OP( DST++, SRC++ );	\
			} while ( --tcnt_ != 0 );	\
	};									\
	/* */
#	define BLTLOOPBACK( SRC, DST, CNT, OP ) \
	size_t tcnt_ = (CNT + 7) / 8;		\
	check( tcnt_ != 0 );				\
	switch( CNT % 8 ) {					\
	case 0: do{	OP( --DST, --SRC );	\
	case 7:		OP( --DST, --SRC );	\
	case 6:		OP( --DST, --SRC );	\
	case 5:		OP( --DST, --SRC );	\
	case 4:		OP( --DST, --SRC );	\
	case 3:		OP( --DST, --SRC );	\
	case 2:		OP( --DST, --SRC );	\
	case 1:		OP( --DST, --SRC );	\
			} while ( --tcnt_ != 0 );	\
	};									\
	/* */
#endif

struct DstSrc
{
	Pixel*			dst;	// ptr to the surface
	const Pixel*	src;	// ptr to the spanned sprite
	always_inline DstSrc( Pixel* d, const Pixel* s ) : dst( d ), src( s ) {}
	always_inline void operator+=( size_t val ) { src += val; dst += val; }
};

struct DstSrcMask
{
	Pixel*			dst;	// ptr to the surface
	const Pixel*	src;	// ptr to the raw texture image 
	const Pixel*	mask;	// ptr to the spanned mask sprite
	always_inline DstSrcMask( Pixel* d, const Pixel* s, const Pixel* m ) : dst( d ), src( s ), mask( m ) {}
	always_inline void operator+=( size_t val ) { src += val; dst += val; mask += val; }
};

struct ClipSpan
{
	const Pixel*	in;
	const Pixel*	out;
	always_inline ClipSpan( const Pixel* i, const Pixel* o ) : in( i ), out( o ) {}
};

//////////////////////////////////////////////////////////////////////////

template< typename OP >
struct Blitter {

	typedef OP OpType;

	//// Non clipped span blit
	static always_inline
	void SegmentFast( OP op, DstSrc ptr, size_t count )
	{
		BLTLOOP( ptr.src, ptr.dst, count, op );
	}

	//// Non clipped masked span blit
	static always_inline
	void SegmentFast( OP op, DstSrcMask ptr, size_t count )
	{
		BLTLOOP( ptr.src, ptr.dst, count, op );
	}

	//// Processing segment
	static always_inline
	void ProcessSegment( OP op, Pixel* ptr, size_t count )
	{
		for( ; count != 0; --count, ++ptr ) op(ptr);
	}

	//// Clipped span blit
	static always_inline
	void Segment( OP op, DstSrc ptr, size_t count, ClipSpan clip )
	{
		// validate clip params
		check( clip.in < clip.out );
		sint32 clipBoundDiff = clip.in - ptr.dst;
		size_t leftClipSkips = min( (size_t)max( clipBoundDiff, 0L ), count );
		check( leftClipSkips <= count );
		
		ptr.src += leftClipSkips;
		ptr.dst += leftClipSkips;
		
		count -= leftClipSkips;

		if ( 0 != count && ptr.dst < clip.out ) {
			check( ptr.dst <= clip.out );
			size_t copyCount = min( (size_t)(clip.out - ptr.dst), count );
			check( copyCount <= count );
			count -= copyCount;
			BLTLOOP( ptr.src, ptr.dst, copyCount, op );
		}
	}

	//// Clipped masked span blit
	static always_inline
	void Segment( OP op, DstSrcMask ptr, size_t count, ClipSpan clip )
	{
		// validate clip params
		check( clip.in < clip.out );
		sint32 clipBoundDiff = clip.in - ptr.dst;
		size_t leftClipSkips = min( (size_t)max( clipBoundDiff, 0L ), count );
		check( leftClipSkips <= count );
		
		ptr.src += leftClipSkips;
		ptr.dst += leftClipSkips;
		ptr.mask+= leftClipSkips;
		
		count -= leftClipSkips;

		if ( 0 != count && ptr.dst < clip.out ) {
			check( ptr.dst <= clip.out );
			size_t copyCount = min( (size_t)(clip.out - ptr.dst), count );
			check( copyCount <= count );
			count	-= copyCount;
			ptr.mask+= copyCount;
			BLTLOOP( ptr.src, ptr.dst, copyCount, op );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	
	//// Skips one span
	static inline 
	const Pixel* SpanSkip( const Pixel* src )
	{
		uint32 code;
		do {
			code = (uint32)*src;
			uint32 len	= code & 0x00ff;
			src += len + 1;
		} while ( !(code & 0x8000) );
		return src;
	}

	//// Blits non clipped span line
	static inline 
	const Pixel* SpanFast( OP op, const Pixel* src, Pixel* dst )
	{
		DstSrc ptr( dst, src );
		uint32 code;
		do {
			code = *ptr.src++;
			if ( code == 0x8000 ) break;	

			uint32 len = code & 0x00ff;
			uint32 offset = ((code >> 8) & 0x007f);
			ptr.dst += offset;
			SegmentFast( op, ptr, len );
			ptr += len;
		} while ( !(code & 0x8000) );

		return ptr.src;
	}

	//// Processing span
	static inline 
	Pixel* ProcessSpan( OP op, Pixel* src )
	{
		uint32 code;
		do {
			code = *src++;
			if ( code == 0x8000 ) break;	

			uint32 len = code & 0x00ff;
			uint32 offset = ((code >> 8) & 0x007f);
			//ptr.dst += offset;
			ProcessSegment( op, src, len );
			src += len;
		} while ( !(code & 0x8000) );

		return src;
	}


	//// Blits clipped span line
	static inline 
	const Pixel* Span( OP op, const Pixel* src, Pixel* dst, const Pixel* clipIn, const Pixel* clipOut )
	{
		DstSrc ptr( dst, src );
		const ClipSpan clip( clipIn, clipOut );
		uint32 code;
		do {
			code = *ptr.src++;
			if ( code == 0x8000 ) break;	

			uint32 len = code & 0x00ff;
			uint32 offset = ((code >> 8) & 0x007f);
			ptr.dst += offset;
			Segment( op, ptr, len, clip );
			ptr += len;
		} while ( !(code & 0x8000) );

		return ptr.src;
	}

	//// Blits non clipped masked span line 
	//// ( NOTE: mask is a span sprite! ) returns ptr to the mask
	static inline 
	const Pixel* MaskedSpanFast( OP op, const Pixel* mask, const Pixel* src, Pixel* dst )
	{
		DstSrcMask ptr( dst, src, mask );
		uint32 code;
		do {
			code = *ptr.mask++;
			if ( code == 0x8000 ) break;	

			uint32 len = code & 0x00ff;
			uint32 offset = ((code >> 8) & 0x007f);
			ptr.dst += offset;
			ptr.src += offset;
			SegmentFast( op, ptr, len );
			ptr += len;
		} while ( !(code & 0x8000) );

		return ptr.mask;
	}

	//// Blits clipped masked span line
	//// ( NOTE: mask is a span sprite! ) returns ptr to the mask
	static inline 
	const Pixel* MaskedSpan( OP op, const Pixel* mask, const Pixel* src, Pixel* dst, const Pixel* clipIn, const Pixel* clipOut )
	{
		DstSrcMask ptr( dst, src, mask );
		const ClipSpan clip( clipIn, clipOut );
		uint32 code;
		do {
			code = *ptr.mask++;
			if ( code == 0x8000 ) break;	

			uint32 len = code & 0x00ff;
			uint32 offset = ((code >> 8) & 0x007f);
			ptr.dst += offset;
			ptr.src += offset;
			Segment( op, ptr, len, clip );
			ptr += len;
		} while ( !(code & 0x8000) );

		return ptr.mask;
	}
};

//////////////////////////////////////////////////////////////////////////

#undef BLTLOOP
#undef BLTLOOPBACK

