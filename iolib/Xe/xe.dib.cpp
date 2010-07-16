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

#include "xe.std.h"
#include "xe.dib.h"
#include "xe.dib.llproc.h"


LL_DIB_FUNCTIONS iDib::LLFunctions[4]= 
{
	{ FillBlockOpaque,	CopyBlockOpaque },		// BLEND_SRCCOPY
	{ FillBlockAlpha,	BlitBlockAlpha},		// BLEND_ALPHABLEND
	{ FillBlockOpaque,	BlitBlockAlphaKey},		// BLEND_SRCCOPY
	{ FillBlockOpaque,	BlitBlockCK }			// BLEND_COLORKEY
};


bool iDib::InitBitmapContent ( const iSize& size )
{
	m_BitmapInfo.bmiHeader.biSize = sizeof ( BITMAPINFOHEADER );
	m_BitmapInfo.bmiHeader.biWidth = size.w;
	m_BitmapInfo.bmiHeader.biHeight = -(sint32)size.h;
	m_BitmapInfo.bmiHeader.biPlanes = 1;
	m_BitmapInfo.bmiHeader.biBitCount = 32; 
	m_BitmapInfo.bmiHeader.biCompression = BI_RGB;
	m_BitmapInfo.bmiHeader.biSizeImage = size.w * size.h * 4;
	
	m_Bitmap = CreateDIBSection ( NULL, &m_BitmapInfo, DIB_RGB_COLORS, (void * *)&m_Bits, NULL, NULL ); 

	check(m_Bitmap != 0);
	check(m_Bits != 0);

	if ( m_Bitmap == 0 )
	{
		Destroy ();
		return false;
	}

	if ( m_Bits == NULL )
	{
		Destroy ();
		return false;
	}
	return true;
}


iDib::iDib ()
: m_Width ( 0 )
, m_Height ( 0 )
, m_Bitmap ( 0 )
, m_Bits ( NULL )
{
	FillBlock8( (uint8*)&m_BitmapInfo, 0, sizeof ( BITMAPINFO ) );
}

iDib::iDib ( const iSize& size, const iColor &clr )
: m_Width ( 0 )
, m_Height ( 0 )
, m_Bitmap ( 0 )
, m_Bits ( NULL )
{
	FillBlock8( (uint8*)&m_BitmapInfo, 0, sizeof ( BITMAPINFO ) );
	verify( Create ( size, clr ) );
}

iDib::iDib ( const iSize& size )
: m_Width ( 0 )
, m_Height ( 0 )
, m_Bitmap ( 0 )
, m_Bits ( NULL )
{
	FillBlock8( (uint8*)&m_BitmapInfo, 0, sizeof ( BITMAPINFO ) );
	verify( Create ( size ) );
}

iDib::iDib ( const iDib* pDib )
: m_Width ( 0 )
, m_Height ( 0 )
, m_Bitmap ( 0 )
, m_Bits ( NULL )
{
	FillBlock8( (uint8*)&m_BitmapInfo, 0, sizeof ( BITMAPINFO ) );
	if (pDib->m_Width && pDib->m_Height) verify( Create ( pDib ) );
}

iDib::iDib ( const iDib* pDib, const iRect& rect)
: m_Width ( 0 )
, m_Height ( 0 )
, m_Bitmap ( 0 )
, m_Bits ( NULL )
{
	verify( Create ( pDib, rect ) );
}

iDib::iDib( const iDib& other )
: m_Width ( 0 )
, m_Height ( 0 )
, m_Bitmap ( 0 )
, m_Bits ( NULL )
{
	FillBlock8( (uint8*)&m_BitmapInfo, 0, sizeof ( BITMAPINFO ) );
	if (other.m_Width && other.m_Height) verify( Create ( &other ) );
}

iDib& iDib::operator = ( const iDib& other )
{
	iDib clone( other );
	Swap( clone );
	return *this;
}

bool iDib::Create ( const iSize& size, const iColor &clr )
{
	Destroy ();

	if ( InitBitmapContent ( size ) == false ) return false;

	m_Width = size.w;
	m_Height = size.h;

	Fill(clr,BLEND_SRCCOPY);

	return true;
}

bool iDib::Create ( const iSize& size )
{
	Destroy ();

	if ( InitBitmapContent ( size ) == false ) return false;

	m_Width = size.w;
	m_Height = size.h;

	return true;
}

bool iDib::Create ( const iDib* pDib )
{
	Destroy ();

	if ( pDib->GetWidth() <= 0 || pDib->GetHeight() <= 0 ) return false;
	if ( InitBitmapContent ( pDib->GetSize() ) == false ) return false;

	m_Width = pDib->GetWidth();
	m_Height = pDib->GetHeight();

	CopyFromDib(pDib, BLEND_SRCCOPY);

	return true;
}

bool iDib::Create ( const iDib* pDib, const iRect& rect)
{
	Destroy ();

	if ( rect.w <= 0 || rect.h <= 0 ) return false;
	if ( pDib->GetWidth() <= 0 || pDib->GetHeight() <= 0 ) return false;
	if ( !pDib->IsValidX ( rect.x1() ) || !pDib->IsValidX ( rect.x2() ) || !pDib->IsValidY ( rect.y1() ) || !pDib->IsValidY ( rect.y2() ) ) return false;

	if (InitBitmapContent(rect.size())==false) return false;

	m_Width = rect.w;
	m_Height = rect.h;

	CopyRectFromDib(pDib,rect,iPoint(0,0), BLEND_SRCCOPY);

	return true;
}

void iDib::Swap( iDib& other )
{
	HBITMAP tmp;
	CopyBlock( &tmp, &m_Bitmap );
	CopyBlock( &m_Bitmap, &other.m_Bitmap );
	CopyBlock( &other.m_Bitmap, &tmp );

	BITMAPINFO tmp2;
	CopyBlock( &tmp2, &m_BitmapInfo );
	CopyBlock( &m_BitmapInfo, &other.m_BitmapInfo );
	CopyBlock( &other.m_BitmapInfo, &tmp2 );

	iSwap( m_Width, other.m_Width );
	iSwap( m_Height, other.m_Height );
	iSwap( m_Bits, other.m_Bits );
}

void iDib::Resize ( const iSize& size )
{
	if ( !CheckWH ( size ) ) Create ( size );
}

void iDib::ResizeCanvas(const iSize& size, iAlignment algn, const iColor &fill)
{
	iDib newdib(size);
	newdib.Fill(fill,BLEND_SRCCOPY);

	if (algn==Align_LeftTop) CopyToDibXY(&newdib,iPoint(0,0),BLEND_SRCCOPY);
	else if (algn==Align_Top) CopyToDibXY(&newdib,iPoint((size.w/2)-(m_Width/2),0),BLEND_SRCCOPY);
	else if (algn==Align_RightTop) CopyToDibXY(&newdib,iPoint(size.w-m_Width,0),BLEND_SRCCOPY);
	else if (algn==Align_Left) CopyToDibXY(&newdib,iPoint(0,(size.h/2)-(m_Height/2)),BLEND_SRCCOPY);
	else if (algn==Align_Center) CopyToDibXY(&newdib,iPoint((size.w/2)-(m_Width/2),(size.h/2)-(m_Height/2)),BLEND_SRCCOPY);
	else if (algn==Align_Right) CopyToDibXY(&newdib,iPoint(size.w-m_Width,(size.h/2)-(m_Height/2)),BLEND_SRCCOPY);
	else if (algn==Align_LeftBottom) CopyToDibXY(&newdib,iPoint(0,size.h-m_Height),BLEND_SRCCOPY);
	else if (algn==Align_Bottom) CopyToDibXY(&newdib,iPoint((size.w/2)-(m_Width/2),size.h-m_Height),BLEND_SRCCOPY);
	else if (algn==Align_RightBottom) CopyToDibXY(&newdib,iPoint(size.w-m_Width,size.h-m_Height),BLEND_SRCCOPY);
	
	Create(&newdib);
}

iDib::~iDib()
{
	Destroy();
}

void iDib::Destroy()
{
	m_Bits = NULL;
	if ( m_Bitmap ) {
		DeleteObject ( m_Bitmap );
		m_Bitmap = NULL;
	}

	m_Width = m_Height = 0;

	FillBlock8( (uint8*)&m_BitmapInfo, 0, sizeof ( BITMAPINFO ) );
}

// Pixel Operations
void iDib::SetPixelChannel(const iPoint& point, ColorChannel ch, uint8 value)
{
	*((uint8*)((m_Bits+(point.y*m_Width)+point.x))+(ch))=value;
}

iColor* iDib::GetPixelPtr(const iPoint& point)
{
	if (!IsValidPoint(point)) return NULL;
	return (GetLine(point.y)+point.x);
}

const iColor* iDib::GetPixelPtr(const iPoint& point) const
{
	if (!IsValidPoint(point)) return NULL;
	return (GetLine(point.y)+point.x);
}

iColor iDib::GetPixel(const iPoint& point) const
{
	if (!IsValidPoint(point)) return cColorBlack;
	return iColor(*(GetLine(point.y)+point.x));
}

void iDib::PutPixel ( const iPoint& point, const iColor& color, BlendMode mode)
{
	if (!IsValidPoint(point)) return;

	if (mode == BLEND_SRCCOPY) m_Bits[m_Width*point.y+point.x]=color;
	else if (mode == BLEND_ALPHABLEND) m_Bits[m_Width*point.y+point.x].AlphaBlend(color);
}

// Line Operations
iColor* iDib::GetLine ( uint32 y )
{
	if (!IsValidY(y)) return NULL;
	return m_Bits+(m_Width*y);
}

const iColor* iDib::GetLine ( uint32 y ) const
{
	if (!IsValidY(y)) return NULL;
	return m_Bits+(m_Width*y);
}

void iDib::PutLine ( uint32 y, iColor* line )
{
	if (!IsValidY(y)) return;
	iColor* ptrLine=GetLine(y);
	CopyBlockOpaque((uint32*)ptrLine,(uint32*)line,m_Width,NULL);
}

// Fill Handling
void iDib::Fill ( const iColor& color, BlendMode mode )
{
	LLFunctions[mode].LLFillFunction( (uint32*)m_Bits, color.rgba, m_Width*m_Height, NULL );
}

void iDib::FillRect ( const iRect& rect, const iColor& color, BlendMode mode)
{
	iRect nrect=rect;
	if ( !iClipper::ClipRect(nrect,GetSize())) return;
	
	iColor* cds = (iColor*)m_Bits + ( nrect.x + nrect.y * m_Width );

	uint32 h=nrect.h;

	while ( h-- )
	{
		LLFunctions[mode].LLFillFunction( (uint32*)cds, (uint32)color, nrect.w, NULL );
		cds += m_Width;
	}
}

// Dib handling
void iDib::CopyFromDib ( const iDib* pDib, BlendMode mode )
{
	LLFunctions[mode].LLBlendFunction( (uint32*)m_Bits, (uint32*)pDib->m_Bits, m_Width * m_Height, NULL);
}

void iDib::CopyToDib ( iDib* pDib, BlendMode mode ) const
{
	LLFunctions[mode].LLBlendFunction( (uint32*)pDib->m_Bits, (uint32*)m_Bits, m_Width * m_Height, NULL);
}

void iDib::CopyToDibXY ( iDib* pDib, const iPoint& pos, BlendMode mode ) const
{
	if ( (pos.x + (sint32)m_Width) <= 0 || (pos.y + (sint32)m_Height) <= 0) return;

	iRect src_rect(GetSize());
	iSize siz = iSize(pDib->GetWidth() - pos.x, pDib->GetHeight() - pos.y);
	iRect dst_rect(pos,siz);
	
	if (!iClipper::iClipRectRect(dst_rect,pDib->GetSize(),src_rect,GetSize())) return;

	iColor* src_clr=m_Bits;
	if (pos.y<0) src_clr+= (-pos.y * m_Width);
	if (pos.x<0) src_clr+= (-pos.x);
	iColor* dst_clr=pDib->m_Bits+dst_rect.y*pDib->m_Width+dst_rect.x;

	for (uint32 yy=0; yy<dst_rect.h; yy++) {
		LLFunctions[mode].LLBlendFunction((uint32*)dst_clr,(uint32*)src_clr,dst_rect.w, NULL);
		src_clr+=m_Width;
		dst_clr+=pDib->m_Width;
	}
}

void iDib::CopyRectFromDib ( const iDib* pDib, const iRect& src_rect, const iPoint& dst_pos, BlendMode mode  )
{
	iColor* s_cds = (iColor*)pDib->m_Bits + ( src_rect.x + src_rect.y * pDib->m_Width );
	iColor* d_cds = (iColor*)m_Bits + ( dst_pos.x + dst_pos.y * m_Width );

	uint32 h=src_rect.h;

	while ( h-- ) {
		LLFunctions[mode].LLBlendFunction( (uint32*)d_cds, (uint32*)s_cds, src_rect.w, NULL);
		s_cds += pDib->m_Width;
		d_cds += m_Width;
	}
}

void iDib::CopyRectToDibXY ( iDib* pDib, const iRect& srect, const iPoint& dpos, BlendMode mode  ) const
{
	// clip 
	iRect src_rect(srect);
	iSize siz = iSize(pDib->GetWidth() - dpos.x, pDib->GetHeight() - dpos.y);
	iRect dst_rect(dpos,siz);

	if (!iClipper::iClipRectRect(dst_rect,pDib->GetSize(),src_rect,GetSize())) return;


	iColor* s_cds = (iColor*)m_Bits + ( src_rect.x + src_rect.y * m_Width );
	iColor* d_cds = (iColor*)pDib->m_Bits + ( dst_rect.x + dst_rect.y * pDib->m_Width );

	uint32 h=src_rect.h;

	while ( h-- ) {
		LLFunctions[mode].LLBlendFunction( (uint32*)d_cds, (uint32*)s_cds, src_rect.w, NULL);
		s_cds += m_Width;
		d_cds += pDib->m_Width;
	}
}

void iDib::CopyToDibXYClip ( iDib* pDib, const iPoint& dpos, const iRect& clip_rect, BlendMode mode ) const
{
	// clip 
	iRect src_rect(GetSize());
	iRect dst_rect(dpos,pDib->GetSize());

	if (!iClipper::iClipRectRect(dst_rect,clip_rect,src_rect,GetSize())) return;


	iColor* s_cds = (iColor*)m_Bits + ( src_rect.x + src_rect.y * m_Width );
	iColor* d_cds = (iColor*)pDib->m_Bits + ( dst_rect.x + dst_rect.y * pDib->m_Width );

	uint32 h=src_rect.h;

	while ( h-- ) {
		LLFunctions[mode].LLBlendFunction( (uint32*)d_cds, (uint32*)s_cds, dst_rect.w, NULL);
		s_cds += m_Width;
		d_cds += pDib->m_Width;
	}
}

void iDib::CopyRectToDibXYClip ( iDib* pDib, const iRect& srect, const iPoint& dpos, const iRect& clip_rect, BlendMode mode  ) const
{
	// clip 
	iRect src_rect(srect);
	iRect dst_rect(dpos,pDib->GetSize());

	if (!iClipper::iClipRectRect(dst_rect,clip_rect,src_rect,GetSize())) return;


	iColor* s_cds = (iColor*)m_Bits + ( src_rect.x + src_rect.y * m_Width );
	iColor* d_cds = (iColor*)pDib->m_Bits + ( dst_rect.x + dst_rect.y * pDib->m_Width );

	uint32 h=src_rect.h;

	while ( h-- ) {
		LLFunctions[mode].LLBlendFunction( (uint32*)d_cds, (uint32*)s_cds, dst_rect.w, NULL);
		s_cds += m_Width;
		d_cds += pDib->m_Width;
	}
}

void iDib::TileRectToDibRect(const iRect src_rect, iDib &dib, const iRect dst_rect, BlendMode mode) const
{
	if (!GetWidth() || !GetHeight()) return;

	// Tile Dib
	uint32 numx = dst_rect.w / src_rect.w;
	uint32 numy = dst_rect.h / src_rect.h;
	if ((dst_rect.w % src_rect.w) != 0) numx ++;
	if ((dst_rect.h % src_rect.h) != 0) numy ++;
	if (!dst_rect.w || !dst_rect.h) return;

	iDib tdib(dst_rect);
	tdib.Fill(cColorTransparent,BLEND_SRCCOPY);

	for (uint32 yy=0; yy<numy; ++yy) {
		for (uint32 xx=0; xx<numx; ++xx) {
			CopyRectToDibXY(&tdib,src_rect,iPoint(xx*src_rect.w,yy*src_rect.h),BLEND_SRCCOPY);
		}
	}
	tdib.CopyToDibXY(&dib,dst_rect,mode);
}

void iDib::TileToDibRect(iDib &dib, const iRect dst_rect, BlendMode mode) const
{
	if (!GetWidth() || !GetHeight()) return;

	// Tile Dib
	uint32 numx = dst_rect.w / GetWidth();
	uint32 numy = dst_rect.h / GetHeight();
	if ((dst_rect.w % GetWidth()) != 0) numx ++;
	if ((dst_rect.h % GetHeight()) != 0) numy ++;

	iDib tdib(dst_rect);
	tdib.Fill(cColorTransparent,BLEND_SRCCOPY);

	for (uint32 yy=0; yy<numy; ++yy) {
		for (uint32 xx=0; xx<numx; ++xx) {
			CopyToDibXY(&tdib,iPoint(xx*GetWidth(),yy*GetHeight()),BLEND_SRCCOPY);
		}
	}
	tdib.CopyToDibXY(&dib,dst_rect,mode);
}

void iDib::TileToDib(iDib &dib, BlendMode mode) const
{
	TileToDibRect(dib,dib.GetSize(),mode);
}

//////////////////////////////////////////////////////////////////////////
iRect iDibHelpers::FindSolidArea(const iDib& dib)
{
	iPoint ltpt(dib.GetWidth()-1, dib.GetHeight()-1);
	iPoint rbpt(0,0);

	for (sint32 yy=0; yy<(sint32)dib.GetHeight(); ++yy) {
		for (sint32 xx=0; xx<(sint32)dib.GetWidth(); ++xx) {
			if (dib.GetPixelChannel(iPoint(xx,yy),Channel_Alpha)) {
				if (xx < ltpt.x ) ltpt.x = xx;
				if (yy < ltpt.y ) ltpt.y = yy;
				if (xx > rbpt.x ) rbpt.x = xx;
				if (yy > rbpt.y ) rbpt.y = yy;
			}
		}
	}

	return iRect(ltpt.x,ltpt.y,rbpt.x-ltpt.x+1,rbpt.y-ltpt.y+1);
}
