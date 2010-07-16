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

void iDibPrimitives::DrawHLine(iDib &dib, const iPoint &pos1, sint32 x2, const iColor& color, BlendMode mode)
{
	iPoint dpos1 = pos1;
	sint32 dx2 = x2;

	if (!iClipper::ClipHLine(dpos1,dx2,dib.GetSize())) return;

	uint32 len = iMAX(dx2,dpos1.x) - iMIN(dx2,dpos1.x) + 1;
	if (len <= 0) return;
	iColor *dst = dib.m_Bits + dpos1.y * dib.m_Width + iMIN(dx2,dpos1.x);

	iDib::LLFunctions[mode].LLFillFunction( (uint32*)dst, color.rgba, len, NULL );
}

void iDibPrimitives::DrawVLine(iDib &dib, const iPoint &pos1, sint32 y2, const iColor& color, BlendMode mode)
{
	iPoint dpos1 = pos1;
	sint32 dy2 = y2;

	if (!iClipper::ClipVLine(dpos1,dy2,dib.GetSize())) return;

	DWORD len = iMAX(dy2,dpos1.y) - iMIN(dy2,dpos1.y) + 1 ;
	if (len <= 0) return;

	DWORD w = ( dib.m_Width << 2 );

	for (uint32 yy=0; yy<len; yy++)
	{
		dib.PutPixel(iPoint(dpos1.x,iMIN(dy2,dpos1.y)+yy),color,mode);
	}
}

void iDibPrimitives::DrawFrame(iDib &dib, const iRect &rect, const iColor &color, BlendMode mode)
{
	DrawHLine(dib,iPoint(rect.x,rect.y),rect.x+rect.w-1,color,mode);
	DrawHLine(dib,iPoint(rect.x,rect.y+rect.h-1),rect.x+rect.w-1,color,mode);

	DrawVLine(dib,iPoint(rect.x,rect.y),rect.y+rect.h-1,color,mode);
	DrawVLine(dib,iPoint(rect.x+rect.w-1,rect.y),rect.y+rect.h-1,color,mode);
}

void iDibPrimitives::Compose3DFrame(iDib &dib, const iRect &rect, uint8 a, bool rised)
{
	iColor clr1,clr2;

	if (rised){
		clr1 = iColor(255,255,255,a);
		clr2 = iColor(0,0,0,a);
	}else{
		clr1 = iColor(0,0,0,a);
		clr2 = iColor(255,255,255,a);
	}

	DrawHLine(dib,iPoint(rect.x,rect.y),rect.x+rect.w-1,clr1,BLEND_ALPHABLEND);
	DrawHLine(dib,iPoint(rect.x+1,rect.y+rect.h-1),rect.x+rect.w-1,clr2,BLEND_ALPHABLEND);

	DrawVLine(dib,iPoint(rect.x,rect.y),rect.y+rect.h-2,clr1,BLEND_ALPHABLEND);
	DrawVLine(dib,iPoint(rect.x+rect.w-1,rect.y+1),rect.y+rect.h-1,clr2,BLEND_ALPHABLEND);
}

void iDibPrimitives::Draw3DFrame(iDib &dib, const iRect &rect, const iColor &clr1, const iColor &clr2, BlendMode mode)
{
	DrawHLine(dib,iPoint(rect.x,rect.y),rect.x+rect.w-2,clr1,mode);
	DrawHLine(dib,iPoint(rect.x+1,rect.y+rect.h-1),rect.x+rect.w-1,clr2,mode);

	DrawVLine(dib,iPoint(rect.x,rect.y),rect.y+rect.h-2,clr1,mode);
	DrawVLine(dib,iPoint(rect.x+rect.w-1,rect.y+1),rect.y+rect.h-1,clr2,mode);
}

void iDibPrimitives::DrawLine(iDib &dib, const iPoint &pos1, const iPoint &pos2, const iColor& color, BlendMode mode)
{
	iPoint p( pos1 );
	iPoint d( iABS( pos2.x - pos1.x ), iABS( pos2.y - pos1.y ) );
	iPoint s( iSIGN( pos2.x - pos1.x ), iSIGN( pos2.y - pos1.y ) );
	sint32 d_, d1, d2;
	if ( d.y <= d.x ) {
		d_	= d.y * 2 - d.x;
		d1	= d.y * 2;
		d2	= ( d.y - d.x ) * 2;
		dib.PutPixel(p,color,mode);
		p = iPoint( pos1.x + s.x, pos1.y );
		for( sint32 i = 0; i < d.x; i++ ) {
			if ( d_ > 0 ){
				d_	+= d2;
				p.y	+= s.y;
			} else {
				d_	+= d1;
			}
			dib.PutPixel(p,color,mode);
			p.x += s.x;
		}
	} else {
		d_	= d.x * 2 - d.y;
		d1	= d.x * 2;
		d2	= ( d.x - d.y ) * 2;
		dib.PutPixel(p,color,mode);
		p	= iPoint( pos1.x, pos1.y + s.y );
		for( sint32 i = 0; i < d.y; i++ ) {
			if ( d_ > 0 ) {
				d_	+= d2;
				p.x	+= s.x;
			} else {
				d_	+= d1;
			}
			dib.PutPixel(p,color,mode);
			p.y += s.y;
		}
	}
}