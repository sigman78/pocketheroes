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

#include "gxl.inc.h"
#include "gxl.memory.h"
#include "gxl.file.h"
#include "gxl.dib.h"

/*
 *	
 */
const uint32 BWPAL[32] = {
	0x0000,0x0841,0x1082,0x18C3,0x2104,0x2945,0x3186,0x39C7,
	0x4208,0x4A49,0x528A,0x5ACB,0x630C,0x6B4D,0x738E,0x7BCF,
	0x8410,0x8C51,0x9492,0x9CD3,0xA514,0xAD55,0xB596,0xBDD7,
	0xC618,0xCE59,0xD69A,0xDEDB,0xE71C,0xEF5D,0xF79E,0xFFDF
};

/*
 *	
 */
const uint16 RED_MASK[iDib::TypeCount] = {		0x1F<<11,	0xF<<12,	0x1F<<11 };
const uint16 GREEN_MASK[iDib::TypeCount] = {	0x3F<<5,	0xF<<8,		0x1F<<6 };
const uint16 BLUE_MASK[iDib::TypeCount] = {		0x1F,		0xF<<4,		0x1F<<1 };
const uint16 COLOR_MASK[iDib::TypeCount] = {	0xFFFF,		0xFFF<<4,	0x7FFF<<1 };
const uint16 ALPHA_MASK[iDib::TypeCount] = {	0,			0xF,		0x1 };

/*
 *	Blit functions
 */
inline void BlitDibBlock_RGB(iDib::pixel *dst, const iDib::pixel *src, uint32 size)
{
	memcpy(dst,src,size*sizeof(iDib::pixel));
}

inline void BlitDibBlock_RGBA(iDib::pixel *dst, const iDib::pixel *src, uint32 size)
{
	uint16* dpix = dst;
	const uint16* spix = src;
	for (uint32 xx=0; xx<size; ++xx) {
		if ((*spix & ALPHA_MASK[iDib::RGBA]) == 0xF) {
			*dpix = (*spix & RED_MASK[iDib::RGBA]) | (*spix & GREEN_MASK[iDib::RGBA])>>1 | (*spix & BLUE_MASK[iDib::RGBA])>>3;
		} else if ((*spix & ALPHA_MASK[iDib::RGBA]) > 0) {
			uint16 aa = (*spix & ALPHA_MASK[iDib::RGBA])<<2;
			uint16 RB1 = *dpix & (RED_MASK[iDib::RGB] | BLUE_MASK[iDib::RGB]); 
			uint16 G1  = *dpix & (GREEN_MASK[iDib::RGB] ); 
			uint16 RB2 = (*spix & RED_MASK[iDib::RGBA]) | (*spix & BLUE_MASK[iDib::RGBA])>>3; 
			uint16 G2  = (*spix & (GREEN_MASK[iDib::RGBA] ) ) >> 1; 
			uint16 RB = RB1 + (((RB2-RB1) * (aa)) >> 6); 
			uint16 G  = G1 + (((G2-G1)*(aa))>>6); 
			RB &= (RED_MASK[iDib::RGB] | BLUE_MASK[iDib::RGB]); 
			G  &= (GREEN_MASK[iDib::RGB]); 
			*dpix = RB | G; 
		}
		spix++;
		dpix++;
	}
}

inline void BlitDibBlock_RGBCK(iDib::pixel *dst, const iDib::pixel *src, uint32 size)
{
	uint16* dpix = dst;
	const uint16* spix = src;
	for (uint32 xx=0; xx<size; ++xx) {
		if ((*spix & ALPHA_MASK[iDib::RGBCK]) > 0) *dpix = (*spix & RED_MASK[iDib::RGBCK]) | (*spix & GREEN_MASK[iDib::RGBCK]) | (*spix & BLUE_MASK[iDib::RGBCK]) >> 1;
		dpix++;
		spix++;
	}
}

inline void BlitDibBlockAlpha(iDib::pixel *dst, const iDib::pixel *src, uint8 a, uint32 size)
{
	const uint32 alpha = a;
#if 1
	for (uint32 xx=0; xx<size; ++xx, ++dst, ++src) {
		const uint32 a = *src;
		const uint32 b = *dst;
		uint32 sb = a & 0x1f;
		uint32 sg = (a >> 5) & 0x3f;
		uint32 sr = (a >> 11) & 0x1f;
		uint32 db = b & 0x1f;
		uint32 dg = (b >> 5) & 0x3f;
		uint32 dr = (b >> 11) & 0x1f;

		*dst = (iDib::pixel)((((alpha * (sb-db)) >> 8) + db)
		| (((alpha * (sg-dg)) >> 8) + dg) << 5 
		| (((alpha * (sr-dr)) >> 8) + dr) << 11);
	}
#else
	uint8 inv_a = 256-a;
	for (uint32 xx=0; xx<size; ++xx, ++dst, ++src) {
		uint16 sr = a * ((*src & RED_MASK[iDib::RGB]) >> 11);
		uint16 sg = a * ((*src & GREEN_MASK[iDib::RGB]) >> 5);
		uint16 sb = a * ((*src & BLUE_MASK[iDib::RGB]));
		uint16 dr = inv_a * ((*dst & RED_MASK[iDib::RGB]) >> 11);
		uint16 dg = inv_a * ((*dst & GREEN_MASK[iDib::RGB]) >> 5);
		uint16 db = inv_a * ((*dst & BLUE_MASK[iDib::RGB]));
		*dst = (((sr+dr)>>8)<<11 | ((sg+dg)>>8)<<5 | ((sb+db)>>8));
	}
#endif
}

inline void FillDibBlock(iDib::pixel *dst, const iDib::pixel src, uint32 size)
{
	for (uint32 xx=0; xx<size; ++xx, ++dst) *dst = src;
}

inline void FillDibBlockAlpha(iDib::pixel *dst, const iDib::pixel src, uint8 a, uint32 size)
{
#if 1
	const uint32 alpha = a;
	const uint32 sb = src & 0x1f;
	const uint32 sg = (src >> 5) & 0x3f;
	const uint32 sr = (src >> 11) & 0x1f;

	for (uint32 xx=0; xx<size; ++xx, ++dst) {
		const uint32 b = *dst;
		uint32 db = b & 0x1f;
		uint32 dg = (b >> 5) & 0x3f;
		uint32 dr = (b >> 11) & 0x1f;

		*dst = (iDib::pixel)((((alpha * (sb-db)) >> 8) + db)
		| (((alpha * (sg-dg)) >> 8) + dg) << 5 
		| (((alpha * (sr-dr)) >> 8) + dr) << 11);
	}
#else
	uint8 inv_a = 255-a;
	uint16 sr = a * ((src & RED_MASK[iDib::RGB]) >> 11);
	uint16 sg = a * ((src & GREEN_MASK[iDib::RGB]) >> 5);
	uint16 sb = a * ((src & BLUE_MASK[iDib::RGB]));
	for (uint32 xx=0; xx<size; ++xx, ++dst) {
		uint16 dr = inv_a * ((*dst & RED_MASK[iDib::RGB]) >> 11);
		uint16 dg = inv_a * ((*dst & GREEN_MASK[iDib::RGB]) >> 5);
		uint16 db = inv_a * ((*dst & BLUE_MASK[iDib::RGB]));
		*dst = (((sr+dr)>>8)<<11 | ((sg+dg)>>8)<<5 | ((sb+db)>>8));
	}
#endif
}

inline void SetDibPixelAlpha(iDib::pixel *dst, const iDib::pixel src, uint8 a)
{
	uint8 inv_a = 255-a;
	uint16 sr = a * ((src & RED_MASK[iDib::RGB]) >> 11);
	uint16 sg = a * ((src & GREEN_MASK[iDib::RGB]) >> 5);
	uint16 sb = a * ((src & BLUE_MASK[iDib::RGB]));
	uint16 dr = inv_a * ((*dst & RED_MASK[iDib::RGB]) >> 11);
	uint16 dg = inv_a * ((*dst & GREEN_MASK[iDib::RGB]) >> 5);
	uint16 db = inv_a * ((*dst & BLUE_MASK[iDib::RGB]));
	*dst = (((sr+dr)>>8)<<11 | ((sg+dg)>>8)<<5 | ((sb+db)>>8));
}

/*
 *	iDib
 */
iDib::iDib()
: m_dibType(RGB)
{}

iDib::iDib(const iDib& dib)
{
	Init(dib);
}

iDib::iDib(const iSize& siz, Type dType)
{
	Init(siz,dType);
}

iDib::iDib(const iDib& dib, const iRect& rect)
{
	Init(dib, rect);
}


iDib::~iDib()
{
	Cleanup();
}

void iDib::Init(const iDib& dib)
{
	m_dibType = dib.GetType();
	Allocate(dib);
}

void iDib::Init(const iDib& dib, const iRect& rect)
{
	Allocate(rect);
	m_dibType = dib.m_dibType;
	const pixel *src_ptr = dib.m_RGB + dib.m_Siz.w*rect.y + rect.x;
	pixel *dst_ptr = m_RGB;
	for (uint32 yy=0; yy<rect.h; ++yy){
		memcpy(dst_ptr,src_ptr,rect.w*sizeof(pixel));
		dst_ptr+=rect.w;
		src_ptr+=dib.m_Siz.w;
	}
}

void iDib::Init(const iSize& siz, Type dType)
{
	m_dibType = dType;
	Allocate(siz);
}

void iDib::Resize(const iSize& siz)
{
	Cleanup();
	Allocate(siz);
}

void iDib::Allocate(const iSize& siz)
{
	check(m_RGB.IsClean());
	m_Siz = siz;
	m_RGB.Allocate(siz.w*siz.h);
}

void iDib::Allocate(const iDib& dib)
{
	check(m_RGB.IsClean());
	m_Siz = dib.GetSize();
	m_RGB.Allocate(dib.m_RGB, dib.m_Siz.w*dib.m_Siz.h);
}

void iDib::Cleanup()
{
	m_RGB.Clean();
	m_Siz.Zero();
	m_dibType = RGB;
}

void iDib::Swap(iDib& other)
{
	iBuff<pixel> tmp_buff(m_RGB);
	m_RGB.Allocate(other.m_RGB);
	other.m_RGB.Allocate(tmp_buff);

	iSwap(m_Siz,other.m_Siz);
	iSwap(m_dibType,other.m_dibType);
}

iDib& iDib::operator = ( const iDib& other )
{
	iDib clone( other );
	Swap( clone );
	return *this;
}

/*
 *	iPaletteDib
 */
iPaletteDib::iPaletteDib()
{}

iPaletteDib::~iPaletteDib()
{
	Cleanup();
}

iPaletteDib::iPaletteDib(const iSize& siz)
{
	Init(siz);
}

void iPaletteDib::Init(const iSize& siz)
{
	Allocate(siz);
}

void iPaletteDib::Allocate(const iSize& siz)
{
	check(m_RGB.IsClean());
	m_Siz = siz;
	m_RGB.Allocate(siz.w*siz.h);
}

void iPaletteDib::Cleanup()
{
	m_RGB.Clean();
	m_Siz.Zero();
}

void iPaletteDib::CopyToDibXY(iDib& dib, const iPoint& pos, const iPalette& pal, uint8 alpha) const
{
	if ( (pos.x + (sint32)m_Siz.w) <= 0 || (pos.y + (sint32)m_Siz.w) <= 0) return;
	iRect src_rect(GetSize());
	iSize siz = iSize(dib.GetWidth() - pos.x, dib.GetHeight() - pos.y);
	iRect dst_rect(pos,siz);
	if (!iClipper::iClipRectRect(dst_rect,iRect(0,0,dib.GetWidth(),dib.GetHeight()),src_rect,GetSize())) return;
	const uint8* src_clr = m_RGB+src_rect.y*m_Siz.w+src_rect.x;
	uint16* dst_clr=dib.GetPtr()+dst_rect.y*dib.GetWidth()+dst_rect.x;
	for (uint32 yy=0; yy<dst_rect.h; yy++) {
		for (uint32 xx=0; xx<dst_rect.w; ++xx) {
			if (src_clr[xx]) {
				if (alpha == 255) dst_clr[xx] = pal[src_clr[xx]];
				else SetDibPixelAlpha(&dst_clr[xx], pal[src_clr[xx]],alpha);
			}
		}
		src_clr+=m_Siz.w;
		dst_clr+=dib.GetWidth();
	}
}

void iPaletteDib::CopyRectToDibXY(iDib& dib, const iRect& srect, const iPoint& pos, const iPalette& pal, uint8 alpha) const
{
	iRect src_rect(srect);
	iSize siz = iSize(dib.GetWidth() - pos.x, dib.GetHeight() - pos.y);
	iRect dst_rect(pos,siz);
	if (!iClipper::iClipRectRect(dst_rect,dib.GetSize(),src_rect,GetSize())) return;
	const uint8* src_clr = m_RGB+src_rect.y*m_Siz.w+src_rect.x;
	uint16* dst_clr=dib.GetPtr()+dst_rect.y*dib.GetWidth()+dst_rect.x;

	for (uint32 yy=0; yy<dst_rect.h; yy++) {
		for (uint32 xx=0; xx<dst_rect.w; ++xx) {
			if (src_clr[xx] && pal[src_clr[xx]] != RGB16(0xF0,0,0xF0)) {
				if (alpha == 255) dst_clr[xx] = pal[src_clr[xx]];
				else SetDibPixelAlpha(&dst_clr[xx], pal[src_clr[xx]],alpha);
			}
		}
		src_clr+=m_Siz.w;
		dst_clr+=dib.GetWidth();
	}
}

/*
 *	Primitives		
 */
void iDib::Fill(pixel clr, uint8 a)
{
	if (a==255) FillDibBlock(m_RGB,clr,m_Siz.w*m_Siz.h);
	else FillDibBlockAlpha(m_RGB,clr,a,m_Siz.w*m_Siz.h);
}

void iDib::FillRect(const iRect& rc, pixel clr, uint8 a)
{
	iRect drect(rc);
	if (!iClipper::ClipRect(drect,m_Siz)) return;
	pixel* dstPtr = m_RGB + drect.y*m_Siz.w+drect.x;
	uint32 h = drect.h;
	while (h--) {
		if (a==255) FillDibBlock(dstPtr,clr,drect.w);
		else FillDibBlockAlpha(dstPtr,clr,a,drect.w);
		dstPtr += m_Siz.w;
	}
}

const uint32 tintGradient[32] = {
	0x4101,	0x4101,	0x4921,	0x4942,	0x5142,	0x5162,	0x5983,	0x61a3,
	0x61e3,	0x6a04,	0x7224,	0x7a65,	0x8285,	0x8ac6,	0x8ae6,	0x9327,
	0xa368,	0xa389,	0xabc9,	0xb40a,	0xbc4b,	0xc48c,	0xcccd,	0xd4ed,
	0xdd2e,	0xe54f,	0xed90,	0xedd0,	0xf5f1,	0xf611,	0xf632,	0xf652
};

const uint32 hcbwGradient[32] = {
	0x4101,	0x4101,	0x4921,	0x4942,	0x5142,	0x5162,	0x5983,	0x61a3,
	0x61e3,	0x6a04,	0x7224,	0x7a65,	0x8285,	0x8ac6,	0x8ae6,	0x9327,
	0xa368,	0xa389,	0xabc9,	0xb40a,	0xbc4b,	0xc48c,	0xcccd,	0xd4ed,
	0xdd2e,	0xe54f,	0xed90,	0xedd0,	0xf5f1,	0xf611,	0xf632,	0xf652
};

always_inline uint32 
TintedShadow( uint32 pixel)
{ 
	//static uint8 rpt = 31;
	uint32 chnl = (pixel >> 6) & 0x1f;
	return ( (BWPAL[chnl] & 0xf7de) >> 1 ) + ((0x39e7 & 0xf7de) >> 1);
} 

void iDib::Fade(uint8 a)
{
	if (!a){
		Fill(cColor_Black);
	} else if (a != 255) {
		Fill(cColor_Black, 255 - a);
	}
}

void iDib::DarkenBWRect(const iRect& rc)
{
	iRect drect(rc);
	if (!iClipper::ClipRect(drect,m_Siz)) return;
	pixel* dstPtr = m_RGB + drect.y*m_Siz.w+drect.x;
	uint32 h = drect.h;
	while (h--) {
		pixel* pptr = dstPtr;
		for (uint32 xx=0; xx<drect.w; ++xx, ++pptr){
			*pptr = (pixel)TintedShadow(*pptr);
		}
		dstPtr += m_Siz.w;
	}
}

void iDib::Darken25Rect(const iRect& rc)
{
	iRect drect(rc);
	if (!iClipper::ClipRect(drect,m_Siz)) return;
	pixel* dstPtr = m_RGB + drect.y*m_Siz.w+drect.x;
	uint32 h = drect.h;
	while (h--) {
		pixel* pptr = dstPtr;
		for (uint32 xx=0; xx<drect.w; ++xx, ++pptr){
			*pptr = (pixel)Darken25(*pptr);
		}
		dstPtr += m_Siz.w;
	}
}

void iDib::Darken50Rect(const iRect& rc)
{
	iRect drect(rc);
	if (!iClipper::ClipRect(drect,m_Siz)) return;
	pixel* dstPtr = m_RGB + drect.y*m_Siz.w+drect.x;
	uint32 h = drect.h;
	while (h--) {
		pixel* pptr = dstPtr;
		for (uint32 xx=0; xx<drect.w; ++xx, ++pptr){
			*pptr = (pixel)Darken50(*pptr);
		}
		dstPtr += m_Siz.w;
	}
}

void iDib::HGradientRect(const iRect& rc, pixel c1, pixel c2)
{
	iRect drect(rc);
	if (!iClipper::ClipRect(drect,m_Siz)) return;
	pixel* dstPtr = m_RGB + drect.y*m_Siz.w+drect.x;
	sint32 tw = rc.w-1;
	if (!tw){
		return;
	}

	pixel gr[320];
	pixel* pgr = gr;
	sint32 tp = drect.x-rc.x;
	uint32 cnt = iMIN<uint32>(drect.w,320);
	uint32 w = cnt;
	while (w--) {
		sint32 dr = ((c2&0xF800)>>11) - ((c1&0xF800)>>11);
		sint32 nr  = iCLAMP<sint32>(0,0x1F,  ((c1&0xF800)>>11) + ((dr*tp)/tw));
		sint32 dg = ((c2&0x7E0)>>5) - ((c1&0x7E0)>>5);
		sint32 ng  = iCLAMP<sint32>(0,0x3F,  ((c1&0x7E0)>>5) + ((dg*tp)/tw));
		sint32 db = (c2&0x1F) - (c1&0x1F);
		sint32 nb  = iCLAMP<sint32>(0,0x1F,  (c1&0x1F) + ((db*tp)/tw));
		*pgr = (pixel)((nr<<11)|(ng<<5)|nb);
		pgr++;
		tp++;
	}

	
	uint32 h = drect.h;
	while (h--) {
		memcpy(dstPtr, gr, cnt * sizeof(pixel));
		dstPtr += m_Siz.w;
	}
}

void iDib::VGradientRect(const iRect& rc, pixel c1, pixel c2)
{
	iRect drect(rc);
	if (!iClipper::ClipRect(drect,m_Siz)) return;
	pixel* dstPtr = m_RGB + drect.y*m_Siz.w+drect.x;
	sint32 th = rc.h-1;
	if (!th){
		FillDibBlock(dstPtr,c1,drect.w);
		return;
	}
	sint32 tp = drect.y-rc.y;
	uint32 h = drect.h;
	while (h--) {
		sint32 dr = ((c2&0xF800)>>11) - ((c1&0xF800)>>11);
		sint32 nr  = iCLAMP<sint32>(0,0x1F,  ((c1&0xF800)>>11) + ((dr*tp)/th));
		sint32 dg = ((c2&0x7E0)>>5) - ((c1&0x7E0)>>5);
		sint32 ng  = iCLAMP<sint32>(0,0x3F,  ((c1&0x7E0)>>5) + ((dg*tp)/th));
		sint32 db = (c2&0x1F) - (c1&0x1F);
		sint32 nb  = iCLAMP<sint32>(0,0x1F,  (c1&0x1F) + ((db*tp)/th));
		FillDibBlock(dstPtr,(pixel)((nr<<11)|(ng<<5)|nb),drect.w);
		dstPtr += m_Siz.w;
		tp++;
	}
}

void iDib::FrameRect(const iRect& rc, pixel clr, uint8 a)
{
	iRect drect(rc);
	if (!iClipper::ClipRect(drect,m_Siz)) return;

	HLine(rc.TopLeft(),rc.x2(),clr,a);
	HLine(rc.BottomLeft(),rc.x2(),clr,a);
	VLine(rc.TopLeft(),rc.y2(),clr,a);
	VLine(rc.TopRight(),rc.y2(),clr,a);
}

void iDib::HLine(const iPoint& pos, sint32 x2, pixel clr, uint8 a)
{
	iPoint dpos1 = iPoint(iMIN(pos.x,x2),pos.y);
	sint32 dx2 = iMAX(pos.x,x2);
	if (!iClipper::ClipHLine(dpos1,dx2,m_Siz)) return;
	uint32 len = dx2-dpos1.x+1;
	pixel* dstPtr = m_RGB + dpos1.y*m_Siz.w+dpos1.x;
	if (a==255) FillDibBlock(dstPtr,clr,len);
	else FillDibBlockAlpha(dstPtr,clr,a,len);
}

void iDib::VLine(const iPoint& pos, sint32 y2, pixel clr, uint8 a)
{
	iPoint dpos1 = iPoint(pos.x,iMIN(pos.y,y2));
	sint32 dy2 = iMAX(pos.y,y2);
	if (!iClipper::ClipVLine(dpos1,dy2,m_Siz)) return;
	uint32 len = dy2-dpos1.y+1;
	pixel* dstPtr = m_RGB + dpos1.y*m_Siz.w+dpos1.x;
	uint32 h = dy2 - dpos1.y;
	while (h--) {
		if (a==255) *dstPtr = clr;
		else SetDibPixelAlpha(dstPtr,clr,a);
		dstPtr += m_Siz.w;
	}
}

void iDib::Line(const iPoint& p1, const iPoint& p2, pixel clr, uint8 a)
{
	iPoint p( p1 );
	iPoint d( iABS( p2.x - p1.x ), iABS( p2.y - p1.y ) );
	iPoint s( iSIGN( p2.x - p1.x ), iSIGN( p2.y - p1.y ) );
	sint32 d_, d1, d2;
	if ( d.y <= d.x ) {
		d_	= d.y * 2 - d.x;
		d1	= d.y * 2;
		d2	= ( d.y - d.x ) * 2;
		PutPixel(p, clr);
		p = iPoint( p1.x + s.x, p1.y );
		for( sint32 i = 0; i < d.x; i++ ) {
			if ( d_ > 0 ){
				d_	+= d2;
				p.y	+= s.y;
			} else {
				d_	+= d1;
			}
			PutPixel(p, clr);
			p.x += s.x;
		}
	} else {
		d_	= d.x * 2 - d.y;
		d1	= d.x * 2;
		d2	= ( d.x - d.y ) * 2;
		PutPixel(p, clr);
		p	= iPoint( p1.x, p1.y + s.y );
		for( sint32 i = 0; i < d.y; i++ ) {
			if ( d_ > 0 ) {
				d_	+= d2;
				p.x	+= s.x;
			} else {
				d_	+= d1;
			}
			PutPixel(p, clr);
			p.y += s.y;
		}
	}
}

void iDib::Triangle(const iPoint& p1, const iPoint& p2, const iPoint& p3, pixel clr)
{
	iPoint s, e;
	sint32 dx1, dx2, dx3;
	if (p2.y-p1.y > 0) dx1=(p2.x-p1.x)/(p2.y-p1.y); else dx1=0;
	if (p3.y-p1.y > 0) dx2=(p3.x-p1.x)/(p2.y-p1.y); else dx2=0;
	if (p3.y-p2.y > 0) dx3=(p3.x-p2.x)/(p3.y-p2.y); else dx3=0;
	
	s=e=p1;
	if(dx1 > dx2) {
		for(;s.y<=p2.y;s.y++,e.y++,s.x+=dx2,e.x+=dx1)
			HLine(iPoint(s.x,s.y), e.x, clr);
		e=p2;
		for(;s.y<=p3.y;s.y++,e.y++,s.x+=dx2,e.x+=dx3)
			HLine(iPoint(s.x,s.y), e.x, clr);
	} else {
		for(;s.y<=p2.y;s.y++,e.y++,s.x+=dx1,e.x+=dx2)
			HLine(iPoint(s.x,s.y), e.x, clr);
		s=p2;
		for(;s.y<=p3.y;s.y++,e.y++,s.x+=dx3,e.x+=dx2)
			HLine(iPoint(s.x,s.y), e.x, clr);
	}
}

/*
 *	
 */
void iDib::CopyToDibXY(iDib& dib, const iPoint& pos, uint8 a) const
{
	if ( (pos.x + (sint32)m_Siz.w) <= 0 || (pos.y + (sint32)m_Siz.w) <= 0) return;

	check(m_dibType == RGB);
	iRect src_rect(GetSize());
	iSize siz = iSize(dib.GetWidth() - pos.x, dib.GetHeight() - pos.y);
	iRect dst_rect(pos,siz);
	if (!iClipper::iClipRectRect(dst_rect,iRect(0,0,dib.GetWidth(),dib.GetHeight()),src_rect,GetSize())) return;

	const uint16* src_clr = m_RGB+src_rect.y*m_Siz.w+src_rect.x;
	uint16* dst_clr=dib.GetPtr()+dst_rect.y*dib.GetWidth()+dst_rect.x;

	for (uint32 yy=0; yy<dst_rect.h; yy++) {
		BlitDibBlockAlpha(dst_clr,src_clr,a,dst_rect.w);
		src_clr+=m_Siz.w;
		dst_clr+=dib.GetWidth();
	}
}

void iDib::CopyToDibXY( iDib& dib, const iPoint& pos) const
{
	if ( (pos.x + (sint32)m_Siz.w) <= 0 || (pos.y + (sint32)m_Siz.w) <= 0) return;

	iRect src_rect(GetSize());
	iSize siz = iSize(dib.GetWidth() - pos.x, dib.GetHeight() - pos.y);
	iRect dst_rect(pos,siz);
	if (!iClipper::iClipRectRect(dst_rect,iRect(0,0,dib.GetWidth(),dib.GetHeight()),src_rect,GetSize())) return;

	const uint16* src_clr = m_RGB+src_rect.y*m_Siz.w+src_rect.x;
	uint16* dst_clr=dib.GetPtr()+dst_rect.y*dib.GetWidth()+dst_rect.x;

	for (uint32 yy=0; yy<dst_rect.h; yy++) {
		if (m_dibType == RGB) BlitDibBlock_RGB(dst_clr,src_clr,dst_rect.w);
		else if (m_dibType == RGBA) BlitDibBlock_RGBA(dst_clr,src_clr,dst_rect.w);
		else if (m_dibType == RGBCK) BlitDibBlock_RGBCK(dst_clr,src_clr,dst_rect.w);
		src_clr+=m_Siz.w;
		dst_clr+=dib.GetWidth();
	}
}

//
void iDib::CopyRectToDibXY(iDib& dib, const iRect& srect, const iPoint& pos, uint8 a) const
{
	iRect src_rect(srect);
	iSize siz = iSize(dib.GetWidth() - pos.x, dib.GetHeight() - pos.y);
	iRect dst_rect(pos,siz);
	if (!iClipper::iClipRectRect(dst_rect,dib.GetSize(),src_rect,GetSize())) return;

	const uint16* src_clr = m_RGB+src_rect.y*m_Siz.w+src_rect.x;
	uint16* dst_clr=dib.GetPtr()+dst_rect.y*dib.GetWidth()+dst_rect.x;
	
	if ( a == 255 ) {
		for (uint32 yy=0; yy<dst_rect.h; yy++) {
			if (m_dibType == RGB) BlitDibBlock_RGB(dst_clr,src_clr,dst_rect.w);
			else if (m_dibType == RGBA) BlitDibBlock_RGBA(dst_clr,src_clr,dst_rect.w);
			else if (m_dibType == RGBCK) BlitDibBlock_RGBCK(dst_clr,src_clr,dst_rect.w);
			src_clr+=m_Siz.w;
			dst_clr+=dib.GetWidth();
		}
	} else {
		for (uint32 yy=0; yy<dst_rect.h; yy++) {
			BlitDibBlockAlpha(dst_clr,src_clr,a,dst_rect.w);
			src_clr+=m_Siz.w;
			dst_clr+=dib.GetWidth();
		}
	}
}

void iDib::BlendToDibXY(iDib& dib, const iPoint& pos, pixel ck, uint8 bval) const
{
	if ( (pos.x + (sint32)m_Siz.w) <= 0 || (pos.y + (sint32)m_Siz.w) <= 0) return;

	iRect src_rect(GetSize());
	iSize siz = iSize(dib.GetWidth() - pos.x, dib.GetHeight() - pos.y);
	iRect dst_rect(pos,siz);
	if (!iClipper::iClipRectRect(dst_rect,iRect(0,0,dib.GetWidth(),dib.GetHeight()),src_rect,GetSize())) return;

	const uint16* src_clr = m_RGB+src_rect.y*m_Siz.w+src_rect.x;
	uint16* dst_clr=dib.GetPtr()+dst_rect.y*dib.GetWidth()+dst_rect.x;

	for (uint32 yy=0; yy<dst_rect.h; ++yy) {
		const uint16* scl = src_clr;
		uint16* dcl = dst_clr;
		for (uint32 xx=0; xx<dst_rect.w; ++xx) {
			if (*scl != ck) {
				uint8 inv_a = 255-bval;
				uint16 sr = bval * ((*scl & RED_MASK[iDib::RGB]) >> 11);
				uint16 sg = bval * ((*scl & GREEN_MASK[iDib::RGB]) >> 5);
				uint16 sb = bval * ((*scl & BLUE_MASK[iDib::RGB]));
				uint16 dr = inv_a * ((*dcl & RED_MASK[iDib::RGB]) >> 11);
				uint16 dg = inv_a * ((*dcl & GREEN_MASK[iDib::RGB]) >> 5);
				uint16 db = inv_a * ((*dcl & BLUE_MASK[iDib::RGB]));
				*dcl = (((sr+dr)>>8)<<11 | ((sg+dg)>>8)<<5 | ((sb+db)>>8));
			}
			scl++;
			dcl++;
		}
		//if (m_dibType == RGB) BlitDibBlock_RGB(dst_clr,src_clr,dst_rect.w);
		//else if (m_dibType		== RGBA) BlitDibBlock_RGBA(dst_clr,src_clr,dst_rect.w);
		//else if (m_dibType == RGBCK) BlitDibBlock_RGBCK(dst_clr,src_clr,dst_rect.w);
		src_clr+=m_Siz.w;
		dst_clr+=dib.GetWidth();
	}
}

#ifndef UNDER_CE
void iDib::BlitToDCXY(HDC hdc, const iPoint& pos, bool bDoubleSize) const
{
	uint32 bi_siz = sizeof ( BITMAPINFOHEADER ) + 3*sizeof(DWORD);
	BITMAPINFO bi;
	memset(&bi,0,sizeof(BITMAPINFOHEADER));
	DWORD* bmask = (DWORD*)bi.bmiColors;
	bmask[0] = 0x1F<<11;
	bmask[1] = 0x3F<<5;
	bmask[2] = 0x1F;
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = m_Siz.w;
	bi.bmiHeader.biHeight = -(sint32)m_Siz.h;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 16; 
	bi.bmiHeader.biCompression = BI_BITFIELDS;
	bi.bmiHeader.biSizeImage = m_Siz.w * m_Siz.h * 2;
	if (bDoubleSize) {
		StretchDIBits(hdc,pos.x,pos.y,m_Siz.w*2,m_Siz.h*2,0,0,m_Siz.w,m_Siz.h,m_RGB,&bi,DIB_RGB_COLORS,SRCCOPY);
	} else {
		SetDIBitsToDevice(hdc,pos.x,pos.y,m_Siz.w,m_Siz.h,0,0,0,m_Siz.h,m_RGB,&bi,DIB_RGB_COLORS);
	}
}
#endif

/*
 *	Save bitmap
 */
bool SaveDibBitmap32(const iDib& dib, const iStringT& fname)
{
	iFilePtr pFile = CreateWin32File(fname);
	if (!pFile) return false;

	uint32 pix_siz = dib.GetWidth()*dib.GetHeight();
	check(pix_siz);
	const iDib::pixel* buff = dib.GetPtr();

	// Prepare BITMAPFILEHEADER
	BITMAPFILEHEADER	bmfHeader;
	bmfHeader.bfType = ((WORD) ('M' << 8) | 'B');
	bmfHeader.bfReserved1 = 0;
	bmfHeader.bfReserved2 = 0;
	bmfHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (dib.GetWidth() * dib.GetHeight() * 4);
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	// Prepare BITMAPINFOHEADER
	BITMAPINFOHEADER bmihdr;
	bmihdr.biSize = sizeof(BITMAPINFOHEADER);
	bmihdr.biWidth = dib.GetWidth();
	bmihdr.biHeight = -((sint32)dib.GetHeight());
	bmihdr.biPlanes = 1;
	bmihdr.biBitCount = 32;
	bmihdr.biCompression = BI_RGB;
	bmihdr.biSizeImage = 0;
	bmihdr.biXPelsPerMeter = 600;
	bmihdr.biYPelsPerMeter = 600;
	bmihdr.biClrUsed = 0;
	bmihdr.biClrImportant = 0;

	pFile->Write(&bmfHeader,sizeof(BITMAPFILEHEADER));
	pFile->Write(&bmihdr,sizeof(BITMAPINFOHEADER));

	for (uint32 yy=0; yy<dib.GetHeight(); ++yy) {
		for (uint32 xx=0; xx<dib.GetWidth(); ++xx) {
			uint32 clr = ((*buff)>>11)<<19 | (((*buff)>>5) & 0x3f)<<10 | ((*buff)&0x1f)<<3;
			pFile->Write(&clr,sizeof(clr));
			buff++;
		}
	}

	return true;
}

bool SaveDibBitmap16(const iDib& dib, const iStringT& fname)
{
	iFilePtr pFile = CreateWin32File(fname);
	if (!pFile) return false;

	uint32 pix_siz = dib.GetWidth()*dib.GetHeight();
	check(pix_siz);
	const iDib::pixel* buff = dib.GetPtr();

	// Prepare BITMAPFILEHEADER
	BITMAPFILEHEADER	bmfHeader;
	bmfHeader.bfType = ((WORD) ('M' << 8) | 'B');
	bmfHeader.bfReserved1 = 0;
	bmfHeader.bfReserved2 = 0;
	bmfHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (dib.GetWidth() * dib.GetHeight() * 2);
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	// Prepare BITMAPINFOHEADER
	BITMAPINFOHEADER bmihdr;
	bmihdr.biSize = sizeof(BITMAPINFOHEADER);
	bmihdr.biWidth = dib.GetWidth();
	bmihdr.biHeight = -((sint32)dib.GetHeight());
	bmihdr.biPlanes = 1;
	bmihdr.biBitCount = 16;
	bmihdr.biCompression = BI_RGB;
	bmihdr.biSizeImage = 0;
	bmihdr.biXPelsPerMeter = 600;
	bmihdr.biYPelsPerMeter = 600;
	bmihdr.biClrUsed = 0;
	bmihdr.biClrImportant = 0;

	pFile->Write(&bmfHeader,sizeof(BITMAPFILEHEADER));
	pFile->Write(&bmihdr,sizeof(BITMAPINFOHEADER));

	uint16* obuff = new uint16[dib.GetWidth()];

	for (uint32 yy=0; yy<dib.GetHeight(); ++yy) {
		for (uint32 xx=0; xx<dib.GetWidth(); ++xx) {
			obuff[xx] = (*buff & 0x1F) | ((*buff & 0xFFC0)>>1);
			buff++;
		}
		pFile->Write(obuff,dib.GetWidth() * sizeof(uint16));
	}

	delete[] obuff;
	return true;
}
