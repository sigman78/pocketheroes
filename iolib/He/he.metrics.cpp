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

/*************************************************************************
 *
 * Name: He.Metrics.cpp
 * Desc: Helium metrics
 * Auth: Tutankhamen (Robert Tarasov)
 * Date: 2002/11/07
 *
 *************************************************************************/
#include "he.std.h"
#include "he.metrics.h"

const iPoint	cInvalidPoint(0x7fffffff,0x7fffffff);
const iSize		cInvalidSize(0xffffffff,0xffffffff);
const iRect		cInvalidRect(0x7fffffff,0x7fffffff,0xffffffff,0xffffffff);


#define CLIPCODE( p, c ) \
	c = 0;\
	if ( (p.x) < clip.x1() )	c |= 8;\
	if ( (p.x) > clip.x2() )	c |= 4;\
	if ( (p.y) < clip.y1() )	c |= 2;\
	if ( (p.y) > clip.y2() )	c |= 1;\

bool iClipper::ClipLine(iPoint &pnt1, iPoint &pnt2, const iRect &rect)
{
	uint32 c1, c2;
	iPoint d;
	iRect clip(rect);
	CLIPCODE( pnt1, c1 );
	CLIPCODE( pnt2, c2 );
	while ( (c1) || c2 ) {
		if ( c1 & c2 ) {
			return false;
		}
		d.x = pnt2.x - pnt1.x;
		d.y = pnt2.y - pnt1.y;
		if ( c1 != 0 ) {
			if ( pnt1.x < clip.x1() ) 
			{
				pnt1.y += d.y * ( clip.x1() - pnt1.x ) / d.x;
				pnt1.x = clip.x1();
			} else if ( pnt1.x > clip.x2() ) {
				pnt1.y += d.y * ( clip.x2() - pnt1.x ) / d.x;
				pnt1.x = clip.x2();
			} else if ( pnt1.y < clip.y1() ) {
				pnt1.x += d.x * ( clip.y1() - pnt1.y ) / d.y;
				pnt1.y = clip.y1();
			} else if ( pnt1.y > clip.y2() ) {
				pnt1.x += d.x * ( clip.y2() - pnt1.y ) / d.y;
				pnt1.y = clip.y2();
			}
			CLIPCODE( pnt1, c1 );
		} else {
			if ( pnt2.x < clip.x1() ) {
				pnt2.y += d.y * ( clip.x1() - pnt2.x ) / d.x;
				pnt2.x = clip.x1();
			} else if ( pnt2.x > clip.x2() ) {
				pnt2.y += d.y * ( clip.x2() - pnt2.x ) / d.x;
				pnt2.x = clip.x2();
			} else if ( pnt2.y < clip.y1() ) {
				pnt2.x += d.x * ( clip.y1() - pnt2.y ) / d.y;
				pnt2.y = clip.y1();
			} else if ( pnt2.y > clip.y2() ) {
				pnt2.x += d.x * ( clip.y2() - pnt2.y ) / d.y;
				pnt2.y = clip.y2();
			}
			CLIPCODE( pnt2, c2 );
		}
	}
	return true;
}

#undef CLIPCODE

bool iClipper::ClipRect(iRect &rc, const iRect &rect)
{
	if (rc.x < rect.x) {
		sint32 val = rect.x-rc.x;
		if (val > (sint32)rc.w) return false;
		rc.w -= val;
		rc.x = rect.x;
	}

	if (rc.y < rect.y) {
		sint32 val = rect.y-rc.y;
		if (val > (sint32)rc.h) return false;
		rc.h -= val;
		rc.y = rect.y;
	}

	if (rect.x2() < rc.x2()) {
		sint32 val = rc.x2()-rect.x2();
		if (val > (sint32)rc.w) return false;
		rc.w -= val;
	}

	if (rect.y2() < rc.y2()) {
		sint32 val = rc.y2()-rect.y2();
		if (val > (sint32)rc.h) return false;
		rc.h -= val;
	}
	return (rc.w>0 && rc.h>0);
}

bool iClipper::iClipRectRect(iRect &dst_rc, const iRect &dst_rect, iRect &src_rc, const iRect &src_rect)
{

	//iSize cl = src_rc;
	sint32 clw = src_rc.w;
	sint32 clh = src_rc.h;

	// check ridiculous cases
	/*
	if ( ( src_rc.x >= (sint32)src_rect.w ) || ( src_rc.y >= (sint32)src_rect.h )  ||
		( dst_rc.x >= dst_rect.x2()+1 ) || ( dst_rc.y >= dst_rect.y2()+1 ) ) 
		return false;*/
	if (!iClipper::IsIntersectRect(dst_rc,dst_rect) || !iClipper::IsIntersectRect(src_rc,src_rect)) return false;
	
	// clip src left
	if ( src_rc.x < 0 ) {
		clw += src_rc.x;
		dst_rc.x -= src_rc.x;
		src_rc.x = 0;
	}
	// clip src top
	if ( src_rc.y < 0 ) {
		clh += src_rc.y;
		dst_rc.y -= src_rc.y;
		src_rc.y = 0;
	}
	// clip src right
	if ( src_rc.x + clw > (sint32)src_rect.w ) {
		clw = src_rect.w - src_rc.x;
	}
	// clip src bottom
	if ( src_rc.y + clh > (sint32)src_rect.h ) {
		clh = src_rect.h - src_rc.y;
	}
	// clip dest left
	if ( dst_rc.x < dst_rect.x ) {
		dst_rc.x -= dst_rect.x;
		clw += dst_rc.x;
		src_rc.x -= dst_rc.x;
		dst_rc.x = dst_rect.x; 
	}
	// clip dest top
	if ( dst_rc.y < dst_rect.y ) {
		dst_rc.y -= dst_rect.y;
		clh += dst_rc.y;
		src_rc.y -= dst_rc.y;
		dst_rc.y = dst_rect.y;
	}
	// clip dest right
	if ( dst_rc.x + (sint32)clw >= dst_rect.x2()+1 ) {
		clw = dst_rect.x2() - dst_rc.x + 1;
	}
	// clip dest bottom
	if ( dst_rc.y + (sint32)clh >= dst_rect.y2()+1 ) {
		clh = dst_rect.y2() - dst_rc.y + 1;
	}
	// bail out on zero size
	if ( ( clw <= 0 ) || ( clh <= 0 ) )
		return false;

	dst_rc.w = clw;	dst_rc.h = clh;
	src_rc.w = clw;	src_rc.h = clh;

	return true;
}