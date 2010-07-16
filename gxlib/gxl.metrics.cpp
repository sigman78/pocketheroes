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

const iPoint	cInvalidPoint(0x7fff,0x7fff);
const iSize		cInvalidSize(0xffff,0xffff);
const iRect		cInvalidRect(0x7fff,0x7fff,0xffff,0xffff);

namespace iClipper {


always_inline sint32 pow2(sint32 val) { return val*val; }

bool IsLineIntersectCircle(const iPoint cp, sint32 cr, const iPoint& p1, const iPoint& p2)
{
	sint32 x01 = p1.x - cp.x;
	sint32 y01 = p1.y - cp.y;
	sint32 x02 = p2.x - cp.x;
	sint32 y02 = p2.y - cp.y;

	sint32 dx = x02 - x01;
	sint32 dy = y02 - y01;

	sint32 a = pow2(dx) + pow2(dy);
	sint32 b = 2 * (x01 * dx + y01 * dy);
	sint32 c = pow2(x01) + pow2(y01) - pow2(cr);

	if (-b < 0) return c < 0;
	else if (-b < (2 * a) ) return (4 * a * c - pow2(b)) < 0;
	return (a + b + c) < 0;

	/*
	if ( !iRect(p1,p2).PtInRect(cp) ) return false;

	float dist = (( (p1.y - p2.y) * cp.x + (p2.x - p1.x) * cp.y + (p1.x * p2.y - p2.x * p1.y)) / sqrt( (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y) ));
	if (dist < 0 ) dist = -dist;

	bool res = cr > dist;
	OutputDebugString(iFormat(_T("Circle (%d, %d, %d), Line (%d, %d -> %d, %d) : %f\n"), cp.x, cp.y, cr, p1.x, p1.y, p2.x, p2.y, dist).CStr());
	return res;
	*/
}

bool ClipPoint(iPoint &pnt, const iRect &rect)
{
	pnt.x = iCLAMP(rect.x,rect.x2(),pnt.x);
	pnt.y = iCLAMP(rect.x,rect.y2(),pnt.y);
	return true;
}

bool ClipHLine(iPoint &pnt1, sint32 &x2, const iRect &rect)
{
	if (pnt1.y < rect.y || pnt1.y > rect.y2()) return false;
	pnt1.x = iCLAMP(rect.x,rect.x2(),pnt1.x);
	x2 = iCLAMP(rect.x,rect.x2(),x2);
	return (pnt1.x != x2);
}

bool ClipVLine(iPoint &pnt1, sint32 &y2, const iRect &rect)
{
	if (pnt1.x < rect.x || pnt1.x > rect.x2()) return false;
	pnt1.y = iCLAMP(rect.y,rect.y2(),pnt1.y);
	y2 = iCLAMP(rect.y,rect.y2(),y2);
	return (pnt1.y != y2);
}


#define CLIPCODE( p, c ) \
	c = 0;\
	if ( (p.x) < clip.x1() )	c |= 8;\
	if ( (p.x) > clip.x2() )	c |= 4;\
	if ( (p.y) < clip.y1() )	c |= 2;\
	if ( (p.y) > clip.y2() )	c |= 1;\

bool ClipLine(iPoint &pnt1, iPoint &pnt2, const iRect &rect)
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
			if ( pnt1.x < clip.x1() ) {
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

bool IntersectRect(iRect &dst_rect, const iRect &src_rect1, const iRect &src_rect2)
{
	sint32 x1 = iMAX(src_rect1.x, src_rect2.x);
	sint32 y1 = iMAX(src_rect1.y, src_rect2.y);
	sint32 x2 = iMIN(src_rect1.x2(), src_rect2.x2()) + 1;
	sint32 y2 = iMIN(src_rect1.y2(), src_rect2.y2()) + 1;

	if ( (x2 - x1) > 0 && (y2 - y1) > 0) {
		dst_rect = iRect(x1,y1,x2-x1,y2-y1);
		return true;
	}
	else return false;
}

bool IsIntersectRect(const iRect &src_rect1, const iRect &src_rect2)
{
	sint32 x1 = iMAX(src_rect1.x, src_rect2.x);
	sint32 y1 = iMAX(src_rect1.y, src_rect2.y);
	sint32 x2 = iMIN(src_rect1.x2(), src_rect2.x2()) + 1;
	sint32 y2 = iMIN(src_rect1.y2(), src_rect2.y2()) + 1;
	return ( (x2 - x1) > 0 && (y2 - y1) > 0 );
}

bool ClipRect(iRect &rc, const iRect &rect)
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

bool iClipRectRect(iRect &dst_rc, const iRect &dst_rect, iRect &src_rc, const iRect &src_rect)
{

	//iSize cl = src_rc;
	sint32 clw = src_rc.w;
	sint32 clh = src_rc.h;

	// check ridiculous cases
	if (!IsIntersectRect(dst_rc,dst_rect) || !IsIntersectRect(src_rc,src_rect)) return false;
	
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


} //namespace iClipper

/*
 *	Alignment
 */
iRect AlignRect(const iSize& ss, const iRect dr, Alignment al)
{
	iRect orc(ss);
	sint32 sw = ss.w;
	sint32 sh = ss.h;
	sint32 dw = dr.w;
	sint32 dh = dr.h;

	// Vertical alignment
	if (al & AlignTop) {
		orc.y = dr.y;
	} else if (al & AlignBottom) {
		orc.y = dr.y + dh - sh;
	} else {
		orc.y = dr.y + ((dh>>1) - (sh>>1));
	}

	// Horizontal alignment
	if (al & AlignLeft) {
		orc.x = dr.x;
	} else if (al & AlignRight) {
		orc.x = dr.x + dw - sw;
	} else {
		orc.x = dr.x + ((dw>>1) - (sw>>1));
	}

	return orc;
}
