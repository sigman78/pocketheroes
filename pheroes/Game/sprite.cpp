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

/*
#include "sprite2.h"

 *	16bit Dib Sprite Class

bool iSprite16::Init(iDynamicBuffer &buff)
{
	uint16 sx,sy;
	sint16 ax,ay;
	uint32 len;

	buff.Read(sx);
	buff.Read(sy);
	buff.Read(ax);
	buff.Read(ay);
	buff.Read(len);

	m_Anchor.x = ax;
	m_Anchor.y = ay;

	m_Dib.Init(iSize(sx,sy),iDib::RGB);
	check(len == sx*sy*sizeof(iDib::pixel));
	buff.Read(m_Dib.GetPtr(),len);
	m_bInited = true;
	return m_bInited;
}

void iSprite16::Cleanup()
{}

void iSprite16::Compose(iDib& srf, const iPoint& pos) const
{
	check(m_bInited);
	m_Dib.CopyToDibXY(srf,pos);
}

void iSprite16::ComposeRect(iDib& srf, const iRect& srect, const iPoint& pos) const
{
	check(m_bInited);
	m_Dib.CopyRectToDibXY(srf,srect,pos);
}

void iSprite16::ComposeTranslucent(iDib& srf, const iPoint& pos) const
{
	check(m_bInited);
	check( 0 );
	// *** NOT IMPLEMENTED ***
	//m_Dib.CopyToDibXY(srf,pos);
}

void iSprite16::TileRect(iDib& srf, const iRect& src_rect, const iRect& dst_rect) const
{
	check(m_bInited);
	if (!src_rect.w || !src_rect.h) return;

	// Tile Dib
	uint32 numx = dst_rect.w / src_rect.w;
	uint32 numy = dst_rect.h / src_rect.h;
	if ((dst_rect.w % src_rect.w) != 0) numx ++;
	if ((dst_rect.h % src_rect.h) != 0) numy ++;

	for (uint32 yy=0; yy<numy; ++yy) {
		for (uint32 xx=0; xx<numx; ++xx) {
			sint32 w = iMIN(src_rect.w,dst_rect.w - (xx*src_rect.w)); 
			sint32 h = iMIN(src_rect.h,dst_rect.h - (yy*src_rect.h)); 
			m_Dib.CopyRectToDibXY(srf,iRect(src_rect.x,src_rect.y,w,h),iPoint(dst_rect.x+xx*src_rect.w,dst_rect.y+yy*src_rect.h));
		}
	}
}

void iSprite16::H3TileRect(iDib& srf, const iRect& src_rect, const iRect& dst_rect) const
{
	check(m_bInited);

	uint32 sH = src_rect.h;
	uint32 sW = 0;
	if (!sH || src_rect.w%3 != 0 || !(sW=src_rect.w/3)){
		check(0);
		return;
	}

	m_Dib.CopyRectToDibXY(srf,iRect(src_rect.x,src_rect.y,sW,sH),dst_rect.point());
	m_Dib.CopyRectToDibXY(srf,iRect(src_rect.x+sW*2,src_rect.y,sW,sH),iPoint(dst_rect.x+dst_rect.w-sW,dst_rect.y));
	TileRect(srf, iRect(src_rect.x+sW,src_rect.y,sW,sH), iRect(dst_rect.x+sW,dst_rect.y,dst_rect.w-sW*2,sH));
}

*/

/*
 *	16bit SpanSprite
 
bool iSpanSprite16::Init(iDynamicBuffer &buff)
{
	uint16 sx,sy;
	sint16 ax,ay;
	uint32 len;

	buff.Read(sx);
	buff.Read(sy);
	buff.Read(ax);
	buff.Read(ay);
	buff.Read(len);

	m_Anchor.x = ax;
	m_Anchor.y = ay;
	m_Size.w = sx;
	m_Size.h = sy;

	m_Buff.Allocate(len/2);
	buff.Read(m_Buff,len);

	m_bInited = true;
	return m_bInited;
}

void iSpanSprite16::Cleanup()
{
}
*/

/*
 *	

inline uint16* WordBlt( uint16*& dst, const uint16* src, size_t count )
{
	uint16* _dst = dst;
	for( ; count != 0; --count, ++src, ++_dst )  *_dst = *src;
	dst = _dst;
	return (uint16*)src;
}

inline uint16* WordBltClip( uint16*& dst, const uint16* src, size_t count, const uint16* clipIn, const uint16* clipOut )
{
	// empty clip span
	check( clipIn != clipOut ); 
	check( clipIn < clipOut );
	// WARN:: using local variable is important, because
	// compiler could not optimize away reference and produce
	// a bunch of load&stores for each operation.
	uint16* _dst = dst;
	// preclip
	// >> for( ; count != 0 && dst < clipIn; --count, ++src, ++dst );
	sint32 clipDif = clipIn - _dst;
	size_t cinc = min( (size_t)max(clipDif, 0L), count );
	check( cinc <= count );
	src += cinc;
	_dst += cinc;
	count -= cinc;
	// all span is clipped out
	// copy span until clip or end
	// >> for( ; count != 0 && dst < clipOut; --count, ++src, ++dst ) *dst = *src;
	// >> return src += count;
	if ( 0 != count && _dst < clipOut ) {
		check( _dst <= clipOut );
		size_t ccount = min( (size_t)(clipOut - _dst), count );
		check( ccount <= count );
		count -= ccount;
		for( ; ccount != 0; --ccount, ++src, ++_dst ) *_dst = *src;
	}
	dst = _dst;
	// eat tail
	return (uint16*)src + count;
}

uint16* ProcessSpanLine(const uint16* src, uint16* dst)
{
	bool bLast = false;
	do {
		uint16 code = *src;
		bLast = (code & (1<<15)) != 0;
		uint32 offset = ((code >> 8) & 0x7F);
		dst += offset;
		uint32 len = code & 0xFF;
		src = WordBlt( dst, ++src, len );
	} while (!bLast);

	return (uint16*)src;
}

inline uint16* SkipSpanLine(const uint16* src)
{
	bool bLast = false;
	do {
		uint16 code = *src;
		bLast = (code & (1<<15)) != 0;
		uint32 len = code & 0xFF;
		src += (len+1);
	} while (!bLast);
	return (uint16*)src;
}

uint16* ProcessClipSpanLine(const uint16* src, uint16* dst, const uint16* clipIn, const uint16* clipOut )
{
	bool bLast = false;
	do {
		uint code = *src;
		bLast = (code & (1<<15)) != 0;
		uint32 offset = ((code >> 8) & 0x7F);
		dst += offset;
		uint32 len = code & 0xFF;
		src = WordBltClip( dst, ++src, len, clipIn, clipOut );
	} while (!bLast);

	return (uint16*)src;
}

//
inline uint16* WordMaskBlt( uint16*& dst, const uint16* src_mask, const uint16*& src_clr, size_t count )
{
	src_mask += count;
	uint16* _dst = dst;
	const uint16* _src = src_clr;
	for( ; count != 0; --count, ++_src, ++_dst ) *_dst = *_src;
	dst = _dst;
	src_clr = _src;
	return (uint16*)src_mask;
}

inline uint16* WordMaskBltClip( uint16*& dst, const uint16* src_mask, const uint16*& src_clr, size_t count, const uint16* clipIn, const uint16* clipOut )
{
	// empty clip span
	check( clipIn != clipOut ); 
	check( clipIn < clipOut );
	uint16* _dst = dst;
	const uint16* _src = src_clr;
	// preclip
	sint32 clipDif = clipIn - _dst;
	size_t cinc = min( (size_t)max(clipDif, 0L), count );
	check( cinc <= count );
	src_mask += cinc;
	_src += cinc;
	_dst += cinc;
	count -= cinc;
	// all span is clipped out
	// copy span until clip or end
	if ( 0 != count && dst < clipOut ) {
		check( _dst <= clipOut );
		size_t ccount = min( (size_t)(clipOut - _dst), count );
		check( ccount <= count );
		count -= ccount;
		src_mask += ccount;
		for( ; ccount != 0; --ccount, ++_src, ++_dst ) *_dst = *_src;
	}
	/// *** WARN *** Here is problem, something is not incremented!
	dst = _dst;
	src_clr = _src + count;
	// eat tail
	return (uint16*)src_mask + count;
}

uint16* ProcessMaskSpanLine(const uint16* src_mask, const uint16* src_clr, uint16* dst)
{
	bool bLast = false;
	do {
		uint16 code = *src_mask;
		bLast = (code & (1<<15)) != 0;
		uint32 offset = ((code >> 8) & 0x7F);
		dst += offset;
		src_clr += offset;
		uint32 len = code & 0xFF;
		src_mask = WordMaskBlt( dst, ++src_mask, src_clr, len );
	} while (!bLast);

	return (uint16*)src_mask;
}

uint16* ProcessClipMaskSpanLine(const uint16* src_mask, const uint16* src_clr, uint16* dst, const uint16* clipIn, const uint16* clipOut )
{
	bool bLast = false;
	do {
		uint16 code = *src_mask;
		bLast = (code & (1<<15)) != 0;
		uint32 offset = ((code >> 8) & 0x7F);
		dst += offset;
		src_clr += offset;
		uint32 len = code & 0xFF;
		src_mask = WordMaskBltClip( dst, ++src_mask, src_clr, len, clipIn, clipOut );
	} while (!bLast);

	return (uint16*)src_mask;
}

inline uint32 
Modulate50( uint32 pixel )
{
	return (pixel & 0xf7de) >> 1;
}

inline uint32 
Modulate25( uint32 pixel )
{
	pixel = (pixel & 0xf7de) >> 1;
	pixel += (pixel & 0xf7de) >> 1;
	return pixel;
}


//
inline uint16* ShadowWordBlt( uint16*& dst, const uint16* src, size_t count )
{
	uint16* _dst = dst;
	for( ; count != 0; --count, ++src, ++_dst ) *_dst = (uint16)Modulate50( *_dst ) ;
	dst = _dst;
	return (uint16*)src;
}

inline uint16* ShadowWordBltClip( uint16*& dst, const uint16* src, size_t count, const uint16* clipIn, const uint16* clipOut )
{
	// empty clip span
	check( clipIn != clipOut ); 
	check( clipIn < clipOut );
//	*const_cast<uint16*>(clipIn) = 0x000f;
//	*const_cast<uint16*>(clipOut-1) = 0x00ff;
	// preclip
	size_t tcount = 0;
	size_t ocount =  count;

	uint16* _dst = dst;
	sint32 clipDif = clipIn - _dst;
	size_t cinc = min( (size_t)max(clipDif, 0L), count );
	check( cinc <= count );
	src += cinc;
	_dst += cinc;
	count -= cinc;

	tcount += cinc;
	// all span is clipped out
	// copy span until clip or end
	if ( 0 != count && _dst < clipOut ) {
		check( _dst <= clipOut );
		size_t ccount = min( (size_t)(clipOut - _dst), count );
		check( ccount <= count );
		count -= ccount;
		tcount += ccount;
		for( ; ccount != 0; --ccount, ++src, ++_dst ) *_dst = (uint16)Modulate50( *_dst ) ;
	}
	tcount += count;
	check( tcount == ocount );
	dst += count;
	dst = _dst;
	// eat tail
	return (uint16*)src + count;
}

// TODO:: Refactor tons of those blitters!
uint16* ProcessShadowSpanLine(const uint16* src, uint16* dst)
{
	bool bLast = false;
	do {
		uint16 code = *src++;

		if ( code == 0x8000 ) break;	// skip empty

		bLast = (code & (1<<15)) != 0;
		uint32 offset = ((code >> 8) & 0x7F);
		dst += offset;
		uint32 len = code & 0xFF;
		check( len != 0 );
		src = ShadowWordBlt( dst, src, len );
		//src = ShadowWordBltClip( dst, ++src, len, dst, dst + len + 1);
	} while (!bLast);

	return (uint16*)src;
}

uint16* ProcessShadowClipSpanLine(const uint16* src, uint16* dst, const uint16* clipIn, const uint16* clipOut )
{
	bool bLast = false;
	do {
		uint16 code = *src++;
		if ( code == 0x8000 ) break;	// skip empty
		bLast = (code & (1<<15)) != 0;
		uint32 offset = ((code >> 8) & 0x7F);
		dst += offset;
		uint32 len = code & 0xFF;
		src = ShadowWordBltClip( dst, src, len, clipIn, clipOut );
	} while (!bLast);

	return (uint16*)src;
}
*/

/*
 *	

void iSpanSprite16::Compose(iDib& srf, const iPoint& pos) const
{
	typedef Blitter<CopyOp> Blt;

	check(m_bInited);
	if ( (pos.x + (sint32)m_Size.w) <= 0 || (pos.y + (sint32)m_Size.h) <= 0) return;
	iRect src_rect(GetSize());
	iSize siz = iSize(srf.GetWidth() - pos.x, srf.GetHeight() - pos.y);
	iRect dst_rect(pos,siz);
	if (!iClipper::iClipRectRect(dst_rect,srf.GetSize(),src_rect,GetSize())) return;
	uint16* dst_clr = srf.GetPtr(dst_rect);


	const uint16* ptr = m_Buff.GetPtr();
	const uint16* eptr= ptr + m_Buff.GetSize();
	const size_t dstStride = srf.GetWidth();
	if (src_rect.size() == m_Size){
		while (ptr != eptr) {
			//ptr = ProcessSpanLine(ptr, dst_clr);
			ptr = Blt::SpanFast( ptr,dst_clr );
			dst_clr += dstStride;
		}
	} else {
		dst_clr -= src_rect.x;
		
		sint32 toSkip = src_rect.y;
		while (toSkip--) ptr = Blt::SpanSkip( ptr );

		const uint16* clipIn = dst_clr + src_rect.x;
		for (sint32 yy=0; yy<(sint32)src_rect.h; ++yy){
			//ptr = ProcessClipSpanLine(ptr, dst_clr, clipIn, clipOut );
			ptr = Blt::Span(ptr, dst_clr, clipIn, clipIn + src_rect.w );
			dst_clr	+= dstStride;
			clipIn	+= dstStride;
		}
	}
}

void iSpanSprite16::ComposeTranslucent(iDib& srf, const iPoint& pos) const
{
	typedef Blitter<Shadow25Op> Blt;

	check(m_bInited);
	if ( (pos.x + (sint32)m_Size.w) <= 0 || (pos.y + (sint32)m_Size.h) <= 0) return;
	iRect src_rect(GetSize());
	iSize siz = iSize(srf.GetWidth() - pos.x, srf.GetHeight() - pos.y);
	iRect dst_rect(pos,siz);
	if (!iClipper::iClipRectRect(dst_rect,srf.GetSize(),src_rect,GetSize())) return;
	uint16* dst_clr = srf.GetPtr(dst_rect);

	const uint16* ptr = m_Buff.GetPtr();
	const uint16* eptr= ptr + m_Buff.GetSize();
	const size_t dstStride = srf.GetWidth();
	if (src_rect.size() == m_Size){
		while (ptr != eptr) {
			//ptr = ProcessShadowSpanLine(ptr, dst_clr);
			ptr = Blt::SpanFast( ptr,dst_clr );
			dst_clr += dstStride;
		}
	} else {
		dst_clr -= src_rect.x;
		
		sint32 toSkip = src_rect.y;
		//while (toSkip--) ptr = SkipSpanLine(ptr);
		while (toSkip--) ptr = Blt::SpanSkip( ptr );

		const uint16* clipIn = dst_clr + src_rect.x;
		for (sint32 yy=0; yy<(sint32)src_rect.h; ++yy){
			//ptr = ProcessShadowClipSpanLine(ptr, dst_clr, clipIn, clipOut );
			ptr = Blt::Span(ptr, dst_clr, clipIn, clipIn + src_rect.w );
			dst_clr	+= dstStride;
			clipIn	+= dstStride;
		}
	}
}


void iSpanSprite16::ComposeRect(iDib& srf, const iRect& srect, const iPoint& pos) const
{
	check(0);
}

void iSpanSprite16::TileRect(iDib& srf, const iRect& src_rect, const iRect& dst_rect) const
{
	check(0);
}

void iSpanSprite16::H3TileRect(iDib& srf, const iRect& src_rect, const iRect& dst_rect) const
{
	check(0);
}
*/

/*
 *	Sprite Manager

iSpriteMgr::iSpriteMgr()
{}

iSpriteMgr::~iSpriteMgr()
{ Cleanup(); }

bool iSpriteMgr::InitSpriteMgr()
{
	return true;
}

uint32 iSpriteMgr::AddSprite(iDynamicBuffer &buff)
{
	uint8 flags;
	buff.Read(flags);
	uint8 stype = flags & 1;
	bool bShadow = (flags & 2)!=0;

	iSpriteI* pSprite = NULL;
	if (stype == 0) pSprite = new iSprite16();
	else if (stype == 1) pSprite = new iSpanSprite16();
	else {
		check(0);
		return 0;
	}
	
	if (!pSprite->Init(buff)) {
		check(0);
		delete(pSprite);
		return 0;
	}
	pSprite->SetShadow(bShadow);

	m_Sprites.Add(pSprite);
	return m_Sprites.GetSize();
}

void iSpriteMgr::Cleanup()
{
	for (uint32 xx=0; xx<m_Sprites.GetSize(); ++xx) {
		m_Sprites[xx]->Cleanup();
		delete m_Sprites[xx];
	}
	m_Sprites.RemoveAll();
}

void iSpriteMgr::ComposeSprite(uint32 spr_id, iDib& srf, const iPoint& pos) const
{
	check(spr_id < m_Sprites.GetSize());
	m_Sprites[spr_id]->Compose(srf,pos+m_Sprites[spr_id]->GetAnchor());
}

void iSpriteMgr::ComposeTranslucent(uint32 spr_id, iDib& srf, const iPoint& pos) const
{
	check(spr_id < m_Sprites.GetSize());
	m_Sprites[spr_id]->ComposeTranslucent(srf,pos+m_Sprites[spr_id]->GetAnchor());
}

void iSpriteMgr::ComposeSpriteRect(uint32 spr_id, iDib& srf, const iRect& srect, const iPoint& pos) const
{
	check(spr_id < m_Sprites.GetSize());
	m_Sprites[spr_id]->ComposeRect(srf,srect,pos+m_Sprites[spr_id]->GetAnchor());
}

void iSpriteMgr::ComposeMaskedSprite(uint32 spr_id, uint32 mask_id, iDib& srf, const iPoint& pos) const
{
	check(spr_id < m_Sprites.GetSize());
	check(mask_id < m_Sprites.GetSize());
	check(m_Sprites[spr_id]->GetSize() == m_Sprites[mask_id]->GetSize());
	check(m_Sprites[spr_id]->IsInited());
	check(m_Sprites[mask_id]->IsInited());
	// sprite must be in RAW format, mask nust be Spanned

	if ( (pos.x + (sint32)m_Sprites[spr_id]->GetSize().w) <= 0 || (pos.y + (sint32)m_Sprites[spr_id]->GetSize().h) <= 0) return;
	iRect src_rect(m_Sprites[spr_id]->GetSize());
	iSize siz = iSize(srf.GetWidth() - pos.x, srf.GetHeight() - pos.y);
	iRect dst_rect(pos,siz);
	if (!iClipper::iClipRectRect(dst_rect,srf.GetSize(),src_rect,m_Sprites[spr_id]->GetSize())) return;

	typedef Blitter<CopyOp> Blt;

	uint16* dptr = srf.GetPtr(dst_rect);
	const uint16* sptr = m_Sprites[spr_id]->GetPtr();
	const uint16* mptr = m_Sprites[mask_id]->GetPtr();
	const uint16* eptr= mptr + m_Sprites[mask_id]->GetBuffSiz()/2;
	const size_t dstStride = srf.GetWidth();
	const size_t srcStride = m_Sprites[spr_id]->GetSize().w;

	if (src_rect.size() == m_Sprites[spr_id]->GetSize()){

		while (mptr != eptr) {
			//mptr = ProcessMaskSpanLine(mptr, sptr, dptr);
			mptr = Blt::MaskedSpanFast(mptr, sptr, dptr);
			dptr += dstStride;
			sptr += srcStride;
		}
	} else {
		dptr -= src_rect.x;

		sint32 toSkip = src_rect.y;
		sptr += m_Sprites[spr_id]->GetSize().w * toSkip;
		//while (toSkip--) mptr = SkipSpanLine(mptr);
		while (toSkip--) mptr = Blt::SpanSkip(mptr);

		const uint16* clipIn = dptr + src_rect.x;
		for (sint32 yy=0; yy<(sint32)src_rect.h; ++yy){
			//mptr = ProcessClipMaskSpanLine(mptr, sptr, dptr, clipIn, clipOut );
			mptr = Blt::MaskedSpan( mptr, sptr, dptr, clipIn, clipIn + src_rect.w );
			dptr	+= dstStride;
			clipIn	+= dstStride;
			sptr	+= srcStride;
		}
	}
}

void iSpriteMgr::ComposeShadow(uint32 spr_id, iDib& srf, const iPoint& rpos) const
{
	check(spr_id < m_Sprites.GetSize());
	check(m_Sprites[spr_id]->IsInited());
	// sprite must be Spanned

	iSpanSprite16& spr = *(iSpanSprite16*)m_Sprites[spr_id];
	iPoint pos = spr.GetAnchor() + rpos;

	// sprite rect -> sprite shadow rect
	iSize sprSz = spr.GetSize();
	uint32 hhgt = (spr.GetSize().h + 1) / 2;
	check( hhgt > 1 );
	pos.x -= hhgt ;
	pos.y += sprSz.h - hhgt - 1;
	sprSz.w += hhgt - 1;
	sprSz.h = hhgt;

	if ( (pos.x + (sint32)sprSz.w) <= 0 || (pos.y + (sint32)sprSz.h) <= 0) return;
	iRect src_rect(sprSz);
	iSize siz = iSize(srf.GetWidth() - pos.x, srf.GetHeight() - pos.y);
	iRect dst_rect(pos,siz);
	if (!iClipper::iClipRectRect(dst_rect,srf.GetSize(),src_rect,sprSz)) return;

	uint16* dst_clr = srf.GetPtr(dst_rect);

	const uint16* ptr = spr.GetPtr();
	const uint16* eptr= spr.GetEPtr();
	const size_t dstStride = srf.GetWidth();

	typedef Blitter<Shadow50Op> Blt;

	if ( src_rect.size() == sprSz  ){
		while (ptr != eptr) {
			//ptr = ProcessShadowSpanLine(ptr, dst_clr);
			ptr = Blt::SpanFast(ptr, dst_clr);
			++dst_clr;											// shadow skew
			dst_clr += dstStride;
			//if ( ptr != eptr ) ptr = SkipSpanLine(ptr);			// shadow skip
			if ( ptr != eptr ) ptr = Blt::SpanSkip(ptr);			// shadow skip
		}
	} else {
		dst_clr -= src_rect.x;
		
		sint32 toSkip = src_rect.y;
		while (toSkip--) { 
			//ptr = SkipSpanLine(ptr); 
			ptr = Blt::SpanSkip(ptr); 
			check( ptr != eptr );								// skip by factor of two
			//ptr = SkipSpanLine(ptr); 
			ptr = Blt::SpanSkip(ptr); 
		}

		const uint16* clipIn = dst_clr + src_rect.x;
		dst_clr += src_rect.y;									// pre-skew ( after y-clipping )
		for (sint32 yy=0; yy<(sint32)src_rect.h; ++yy) {
			//ptr = ProcessShadowClipSpanLine(ptr, dst_clr, clipIn, clipIn + src_rect.w );
			ptr = Blt::Span(ptr, dst_clr, clipIn, clipIn + src_rect.w );
			clipIn += dstStride;
			dst_clr += dstStride;
			++dst_clr;
			if ( ptr != eptr ) ptr = Blt::SpanSkip(ptr); 
				else break;
			//ptr = SkipSpanLine(ptr);							// shadow skip
		}
	}
}


void iSpriteMgr::TileSpriteRect(uint32 spr_id, iDib& srf, const iRect& src_rect, const iRect& dst_rect) const
{
	check(spr_id < m_Sprites.GetSize());
	m_Sprites[spr_id]->TileRect(srf,src_rect,dst_rect);
}

void iSpriteMgr::H3TileSpriteRect(uint32 spr_id, iDib& srf, const iRect& src_rect, const iRect& dst_rect) const
{
	check(spr_id < m_Sprites.GetSize());
	m_Sprites[spr_id]->H3TileRect(srf,src_rect,dst_rect);
}

void iSpriteMgr::H3TileSprite(uint32 spr_id, iDib& srf, const iRect& dst_rect) const
{
	check(spr_id < m_Sprites.GetSize());
	m_Sprites[spr_id]->H3TileRect(srf,m_Sprites[spr_id]->GetSize(),dst_rect);
}
*/

//
//
/*
struct Span
{
	uint8 start;
	uint8 end;
	const uint16* data;
};

typedef iSimpleArray<Span> SpanList;

iSpanSprite16*
MakeOutlined( iSpanSprite16* src, uint16 outColor )
{
	check( src != 0 );
	iSize newSize = src->GetSize();
	newSize.InflateSize( 2, 2 );

	// allocate new spanlist
	ScopedArray<SpanList> spans( new SpanList[ newSize.h ] );	

	const uint16* spanPtr = 
}*/

