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

#ifndef _GXLIB_COMMON_METRICS_H_
#define _GXLIB_COMMON_METRICS_H_

/*
 *	iSize
 */
class iSize
{
public:
	iSize ()
	: w(0), h(0) {}

	iSize ( uint32 _w, uint32 _h )
	: w(_w), h(_h) {}

	iSize (SIZE &siz)
	: w(siz.cx), h(siz.cy) {}

	inline void operator += (uint32 size)
	{
		w=w+size;
		h=h+size;
	}

	inline void operator -= (uint32 size)
	{
		w=w-size;
		h=h-size;
	}

	inline iSize operator + (const iSize& size) const
	{ return (iSize(w+size.w,h+size.h)); }

	inline iSize operator + (uint32 size) const
	{ return (iSize(w+size,h+size)); }

	inline iSize operator - (uint32 size) const
	{ return (iSize(w-size,h-size)); }

	inline bool operator== (const iSize &siz) const
	{ return ((w==siz.w) && (h==siz.h)); }

	inline bool operator!= (const iSize &siz) const
	{ return ! operator == ( siz ); }

	inline void InflateSize(uint32 w_offs, uint32 h_offs)
	{
		w+=w_offs;
		h+=h_offs;
	}

	inline void InflateSize(uint32 offs)
	{ InflateSize(offs,offs); }

	// Deflate size
	inline void DeflateSize(uint32 w_offs, uint32 h_offs)
	{
		w-=w_offs;
		h-=h_offs;
	}

	// Deflate size
	inline void DeflateSize(uint32 offs)
	{ DeflateSize(offs,offs); }

	// returns aspect ratio width:height
	inline float GetAspectRatio() const
	{ return (float)w / float(h); }

	inline operator SIZE() const
	{
		SIZE siz={w,h};
		return siz;
	}

	inline void Zero()
	{ w=h=0; }

	inline bool IsZero() const
	{ return (w==0 && h==0); }

	uint32 w;
	uint32 h;
};

/*
 *	iPoint
 */
class iPoint
{
public:
	iPoint ()
	: x(0), y(0) {}

	iPoint ( sint32 _x, sint32 _y )
	: x(_x), y(_y) {}

	iPoint (POINT &pnt)
	: x(pnt.x), y(pnt.y) {}

	//
	inline void operator += (const iPoint &pos)
	{
		x=x+pos.x;
		y=y+pos.y;
	}

	inline void operator -= (const iPoint &pos)
	{
		x=x-pos.x;
		y=y-pos.y;
	}

	inline iPoint operator + (const iPoint &pos) const
	{ return (iPoint(x+pos.x,y+pos.y)); }

	inline iPoint operator - (const iPoint &pos) const
	{ return (iPoint(x-pos.x,y-pos.y)); }

	//
	inline void operator += (const iSize &siz)
	{
		x=x+siz.w;
		y=y+siz.h;
	}

	inline void operator -= (const iSize &siz)
	{
		x=x-siz.w;
		y=y-siz.h;
	}

	inline iPoint operator + (const iSize &siz) const
	{ return (iPoint(x+siz.w,y+siz.h)); }

	inline iPoint operator - (const iSize &siz) const
	{ return (iPoint(x-siz.w,y-siz.h)); }

	//
	inline void operator += (sint32 offs)
	{
		x=x+offs;
		y=y+offs;
	}

	inline void operator -= (sint32 offs)
	{
		x=y-offs;
		x=y-offs;
	}

	inline iPoint operator + (sint32 offs) const
	{ return (iPoint(x+offs,y+offs)); }

	inline iPoint operator - (sint32 offs) const
	{ return (iPoint(x-offs,y-offs)); }


	inline bool operator== (const iPoint &pos) const
	{ return ((x==pos.x) && (y==pos.y)); }

	inline bool operator!= (const iPoint &pos) const
	{ return ! operator == ( pos ); }


	inline operator POINT() const
	{
		POINT pnt={x,y};
		return pnt;
	}

	inline uint32 GetSqDelta(const iPoint& pnt) const
	{
		return iMAX( iDIF(pnt.x,x), iDIF(pnt.y,y));
	}

	inline uint32 GetDelta(const iPoint& pnt) const
	{
		sint32 dx = pnt.x - x;
		sint32 dy = pnt.y - y;

		return (uint32)sqrt(float((dx*dx)+(dy*dy)));
	}

	inline void MoveX(sint32 offset_x)
	{ x += offset_x; }

	inline void MoveY(sint32 offset_y)
	{ y += offset_y; }

	inline void Move(sint32 offset_x, sint32 offset_y)
	{
		x += offset_x;
		y += offset_y;
	}

	sint32 x;
	sint32 y;
};

/*
 *	iRect
 */
class iRect : public iPoint, public iSize
{
public:

	iRect() {}

	iRect ( sint32 _x, sint32 _y, uint32 _w, uint32 _h )
	: iPoint(_x,_y), iSize(_w,_h) {}

	iRect ( const iPoint& point,const iSize& size )
	: iPoint(point), iSize(size) {}

	iRect ( const iPoint& p1,const iPoint& p2 )
	{
		sint32 min_x = iMIN(p1.x, p2.x);
		sint32 min_y = iMIN(p1.y, p2.y);
		sint32 max_x = iMAX(p1.x, p2.x);
		sint32 max_y = iMAX(p1.y, p2.y);

		x = min_x;
		y = min_y;
		w = (max_x - min_x) + 1;
		h = (max_y - min_y) + 1;
	}

	iRect ( const iSize& size )
	: iSize(size) {}

	iRect ( const RECT& rect )
	: iPoint(rect.left,rect.top), iSize(rect.right-rect.left, rect.bottom-rect.top) {}

	inline sint32 x1 () const
	{ return x; }

	inline sint32 y1 () const
	{ return y; }

	inline sint32 x2 () const 
	{ return x+w-1; }
	
	inline sint32 y2 () const 
	{ return y+h-1; }

	inline iPoint Center() const
	{ return iPoint(x+w/2,y+h/2); }

	inline iPoint TopRight() const
	{ return iPoint(x2(),y); }

	inline iPoint TopLeft() const
	{ return iPoint(x,y); }

	inline iPoint BottomRight() const
	{ return iPoint(x2(),y2()); }

	inline iPoint BottomLeft() const
	{ return iPoint(x,y2()); }

	inline iSize size () const 
	{ return iSize(w,h); }

	inline iPoint point () const 
	{ return iPoint(x,y); }

	inline bool PtInRect(const sint32 _x, const sint32 _y) const
	{ return (_x>=x && _y>=y && _x<sint32(x+w) && _y<sint32(y+h)); }

	inline bool PtInRect(const iPoint& pnt) const
	{ return PtInRect(pnt.x,pnt.y); }

	void Reset()
	{ x=y=w=h=0; }

	inline bool IsEmpty() const { return (w==0 || h==0); }
	
	operator RECT() const
	{
		RECT rect={x,y,x+w,y+h};
		return rect;
	}
	
	inline bool operator== (const iRect &rect) const
	{ return ((x==rect.x) && (y==rect.y) && (w==rect.w) && (h==rect.h)); }

	inline bool operator!= (const iRect &rect) const
	{ return ! operator == ( rect ); }

	inline iRect operator + (const iPoint &pos) const
	{ return (iRect(x+pos.x,y+pos.y,w,h)); }

	inline iRect operator - (const iPoint &pos) const
	{ return (iRect(x-pos.x,y-pos.y,w,h)); }

	inline iRect operator + (const iRect &rect) const
	{ 
		iRect rc( *this );
		return  rc += rect;
	}

	iRect& operator += (const iPoint &point)
	{
		x += point.x;
		y += point.y;
		return *this;
	}

	iRect& operator += (const iRect &rect)
	{
		if (IsEmpty()) *this = rect;
		
		sint32 min_x = iMIN(rect.x, x);
		sint32 min_y = iMIN(rect.y, y);

		sint32 max_x = iMAX(rect.x2(), x2());
		sint32 max_y = iMAX(rect.y2(), y2());

		x = min_x;
		y = min_y;
		w = max_x-min_x+1;
		h = max_y-min_y+1;

		return *this;
	}

	inline void InflateRect(uint32 left, uint32 top, uint32 right, uint32 bottom)
	{
		x -= left;
		y -= top;
		w += (left+right);
		h += (top+bottom);
	}

	inline void InflateRect(uint32 x_offs, uint32 y_offs)
	{ InflateRect(x_offs,y_offs,x_offs,y_offs); }

	inline void InflateRect(uint32 offs)
	{ InflateRect(offs,offs,offs,offs); }

	inline void DeflateRect(uint32 left, uint32 top, uint32 right, uint32 bottom)
	{
		x += left;
		y += top;
		w -= (left+right);
		h -= (top+bottom);
	}

	inline void DeflateRect(uint32 x_offs, uint32 y_offs)
	{ DeflateRect(x_offs,y_offs,x_offs,y_offs); }

	inline void DeflateRect(uint32 offs)
	{ DeflateRect(offs,offs,offs,offs); }
};

/*
 *	Fixed point based iPoint
 */
class iPointFix
{
public:
	iPointFix() {}

	iPointFix(fix32 _x, fix32 _y)
	: x(_x), y(_y) {}

	iPointFix(sint32 _x, sint32 _y)
	: x(_x), y(_y) {}

	iPointFix(float _x, float _y)
	: x(_x), y(_y) {}

	iPointFix(const iPoint& pos)
	: x(pos.x), y(pos.y) {}

	inline void operator += (const iPointFix &pos)
	{
		x=x+pos.x;
		y=y+pos.y;
	}

	inline void operator -= (const iPointFix &pos)
	{
		x=x-pos.x;
		y=y-pos.y;
	}

	inline iPointFix operator + (const iPointFix &pos) const
	{ return (iPointFix(x+pos.x,y+pos.y)); }

	inline iPointFix operator - (const iPointFix &pos) const
	{ return (iPointFix(x-pos.x,y-pos.y)); }

	inline bool operator== (const iPointFix &pos) const
	{ return ((x==pos.x) && (y==pos.y)); }

	inline bool operator!= (const iPointFix &pos) const
	{ return ! operator == ( pos ); }

	inline iPoint IntPoint() const
	{ return iPoint(x.ceil(),y.ceil()); }

	fix32	x;
	fix32	y;
};

extern const iPoint		cInvalidPoint;
extern const iSize		cInvalidSize;
extern const iRect		cInvalidRect;

/*
 *	iClipper	
 */
namespace iClipper {
	bool IsLineIntersectCircle(const iPoint cp, sint32 cr, const iPoint& p1, const iPoint& p2);
	bool ClipPoint(iPoint &pnt, const iRect &rect);
	bool ClipHLine(iPoint &pnt1, sint32 &x2, const iRect &rect);
	bool ClipVLine(iPoint &pnt1, sint32 &y2, const iRect &rect);
	bool ClipLine(iPoint &pnt1, iPoint &pnt2, const iRect &rect);
	bool IntersectRect(iRect &dst_rect, const iRect &src_rect1, const iRect &src_rect2);
	bool IsIntersectRect(const iRect &src_rect1, const iRect &src_rect2);
	bool ClipRect(iRect &rc, const iRect &rect);
	bool iClipRectRect(iRect &dst_rc, const iRect &dst_rect, iRect &src_rc, const iRect &src_rect);
} //namespace iClipper

/*
 *	Alignment 
 */
enum Alignment {
	AlignCenter		= 0,
	AlignTop		= 0x1,
	AlignRight		= 0x2,
	AlignBottom		= 0x4,
	AlignLeft		= 0x8,
	AlignTopRight	= AlignTop | AlignRight,
	AlignBottomRight= AlignBottom | AlignRight,
	AlignTopLeft	= AlignTop | AlignLeft,
	AlignBottomLeft	= AlignBottom | AlignLeft
};

// Returns rectangle with specified size aligned in specified dst rect
iRect AlignRect(const iSize& ss, const iRect dr, Alignment al);

#endif //_GXLIB_COMMON_METRICS_H_

