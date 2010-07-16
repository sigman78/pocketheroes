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

void iDibDC::DibToDCXY(const iDib &dib, HDC hdc, const iPoint &pos)
{
	SetDIBitsToDevice ( 
		hdc, 
		pos.x, pos.y, dib.GetWidth(), dib.GetHeight(), 
		0, 0, 
		0, dib.GetHeight(), 
		dib.GetBits(), dib.GetBitmapInfo(), DIB_RGB_COLORS 
		);
}

void iDibDC::DibRectToDCXY(const iDib &dib, const iRect &src_rect, HDC hdc, const iPoint &pos)
{
	iRect srect = src_rect;
	if (!iClipper::ClipRect(srect,dib.GetSize())) return;
	iPoint srcPnt( srect.x,dib.GetHeight() - srect.y2()-1);
	SetDIBitsToDevice( 
		hdc,
		pos.x,pos.y,srect.w, srect.h, 
		srcPnt.x, srcPnt.y, 
		0, dib.GetHeight(), 
		dib.GetBits(), dib.GetBitmapInfo(),DIB_RGB_COLORS 
		);
}

void iDibDC::DCRectToDib(HDC hdc, const iRect &src_rect, iDib &dib, const iPoint &pos)
{
	HDC buffDC;
	HBITMAP buffBitmap, oldBitmap;
	buffDC = CreateCompatibleDC ( hdc );
	buffBitmap = CreateCompatibleBitmap ( hdc, src_rect.w, src_rect.h );
	
	oldBitmap = (HBITMAP) SelectObject ( buffDC, buffBitmap );

	BitBlt ( buffDC, src_rect.x, src_rect.y, src_rect.w, src_rect.h, hdc, src_rect.x, src_rect.y, SRCCOPY );
	SelectObject ( buffDC, oldBitmap );

	GetDIBits ( hdc, buffBitmap, 0, src_rect.h, dib.GetBits(), dib.GetBitmapInfo(), DIB_RGB_COLORS );

	DeleteObject(buffBitmap);
	DeleteDC(buffDC);
}

void iDibDC::StretchDibToDCRect(const iDib &dib, const iRect &dst_rect, HDC hdc)
{
	StretchDIBits( hdc , dst_rect.x, dst_rect.y, dst_rect.w, dst_rect.h, 0, 0, dib.GetWidth(), dib.GetHeight(), dib.GetBits(), dib.GetBitmapInfo(), DIB_RGB_COLORS,SRCCOPY );
}


// tile dib to dst rect
void iDibDC::TileDibRectToDCRect(const iDib& dib, const iRect& src_rect, const iRect& dst_rect, HDC hdc)
{
	if (!dib.GetWidth() || !dib.GetHeight() || !src_rect.w || !src_rect.h) return;

	// Tile Dib
	uint32 numx = dst_rect.w / src_rect.w;
	uint32 numy = dst_rect.h / src_rect.h;
	if ((dst_rect.w % src_rect.w) != 0) numx ++;
	if ((dst_rect.h % src_rect.h) != 0) numy ++;

	iDib tdib(dst_rect);
	tdib.Fill(cColorTransparent,BLEND_SRCCOPY);

	for (uint32 yy=0; yy<numy; ++yy) {
		for (uint32 xx=0; xx<numx; ++xx) {
			dib.CopyRectToDibXY(&tdib,src_rect,iPoint(xx*src_rect.w,yy*src_rect.h),BLEND_SRCCOPY);
		}
	}
	DibRectToDCXY(tdib,iRect(0,0,dst_rect.w,dst_rect.h),hdc,iPoint(dst_rect.x,dst_rect.y));
}

void iDibDC::TileDibToDCRect(const iDib& dib, const iRect& dst_rect, HDC hdc)
{
	TileDibRectToDCRect(dib,dib.GetSize(),dst_rect,hdc);
}

// src y coor must be reversed
void iDibDC::StretchDibRectToDCRect(const iDib &dib, const iRect &src_rect, const iRect &dst_rect, HDC hdc)
{
	StretchDIBits( hdc , 
		dst_rect.x, dst_rect.y, dst_rect.w, dst_rect.h, 
		src_rect.x, src_rect.y, src_rect.w, src_rect.h, 
		dib.GetBits(), dib.GetBitmapInfo(), DIB_RGB_COLORS,SRCCOPY );
}

// seg_siz - segment size of frame sub element img_w = ssiz*3 , img_h = ssiz*3
void iDibDC::DrawDibFrame(const iDib& dib, uint32 seg_siz, const iRect& dst_rect,HDC hdc)
{
	iDibDC::DibRectToDCXY(dib,iRect(0,0,seg_siz,seg_siz),hdc,iPoint(dst_rect.x,dst_rect.y));
	iDibDC::DibRectToDCXY(dib,iRect(seg_siz*2,0,seg_siz,seg_siz),hdc,iPoint(dst_rect.x+dst_rect.w-seg_siz,dst_rect.y));
	iDibDC::DibRectToDCXY(dib,iRect(0,seg_siz*2,seg_siz,seg_siz),hdc,iPoint(dst_rect.x,dst_rect.y+dst_rect.h-seg_siz));
	iDibDC::DibRectToDCXY(dib,iRect(seg_siz*2,seg_siz*2,seg_siz,seg_siz),hdc,iPoint(dst_rect.x+dst_rect.w-seg_siz,dst_rect.y+dst_rect.h-seg_siz));

	iDibDC::StretchDibRectToDCRect(
		dib,
		iRect(seg_siz,seg_siz*2,seg_siz,seg_siz),
		iRect(dst_rect.x+seg_siz,dst_rect.y,dst_rect.w-seg_siz*2,seg_siz),
		hdc);

	iDibDC::StretchDibRectToDCRect(
		dib,
		iRect(seg_siz,0,seg_siz,seg_siz),
		iRect(dst_rect.x+seg_siz,dst_rect.y+dst_rect.h-seg_siz,dst_rect.w-seg_siz*2,seg_siz),
		hdc);

	iDibDC::StretchDibRectToDCRect(
		dib,
		iRect(0,seg_siz,seg_siz,seg_siz),
		iRect(dst_rect.x,dst_rect.y+seg_siz,seg_siz,dst_rect.h-seg_siz*2),
		hdc);

	iDibDC::StretchDibRectToDCRect(
		dib,
		iRect(seg_siz*2,seg_siz,seg_siz,seg_siz),
		iRect(dst_rect.x+dst_rect.w-seg_siz,dst_rect.y+seg_siz,seg_siz,dst_rect.h-seg_siz*2),
		hdc);
}

// seg_siz - segment size of frame sub element img_w = ssiz*3 , img_h = ssiz
void iDibDC::DrawDibHTile(const iDib& dib, const iRect& dst_rect, HDC hdc)
{
	uint32 ssiz = 0;
	if (dib.GetWidth()%3 != 0 || !(ssiz=dib.GetWidth()/3)){
		check(0);
		return;
	}

	iDibDC::DibRectToDCXY(dib,iRect(0,0,ssiz,dib.GetHeight()),hdc,iPoint(dst_rect.x,dst_rect.y));
	iDibDC::DibRectToDCXY(dib,iRect(ssiz*2,0,ssiz,dib.GetHeight()),hdc,iPoint(dst_rect.x+dst_rect.w-ssiz,dst_rect.y));
	iDibDC::StretchDibRectToDCRect(
		dib,
		iRect(ssiz,0,ssiz,dib.GetHeight()),
		iRect(dst_rect.x+ssiz,dst_rect.y,dst_rect.w-ssiz*2,dst_rect.h),
		hdc);
}

void iDibDC::DrawDibVTile(const iDib& dib, const iRect& dst_rect, HDC hdc)
{
	check(0);
}
