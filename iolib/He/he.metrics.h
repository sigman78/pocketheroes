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

/** @file
 *	@brief Common metrics classes and definations
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_METRICS_H_
#define _IOLIB_HELIUM_METRICS_H_

#include <math.h>

/// Alignment enumeration
enum iAlignment {
	Align_No = 0,
	Align_LeftTop,
	Align_Top,
	Align_RightTop,
	Align_Right,
	Align_RightBottom,
	Align_Bottom,
	Align_LeftBottom,
	Align_Left,
	Align_Center
};


/// Size class
class iSize
{
public:
	/// Default constructor
	iSize ()
	{
		w = 0;
		h = 0;
	}

	/// Constructs iSize object with given width and height
	iSize ( uint32 tw, uint32 th )
	{
		w = tw;
		h = th;
	}


	/// Constructs iSize object with given SIZE struct
	iSize (SIZE &siz)
	{
		w=siz.cx;
		h=siz.cy;
	}

	/// Increments with and height by given value
	void operator += (uint32 size)
	{
		w=w+size;
		h=h+size;
	}

	/// Decriments with and height by given value
	void operator -= (uint32 size)
	{
		w=w-size;
		h=h-size;
	}

	/// operator +
	iSize operator + (uint32 size) const
	{
		return (iSize(w+size,h+size));
	}

	/// operator -
	iSize operator - (uint32 size) const
	{
		return (iSize(w-size,h-size));
	}

	/// Equality operator
	bool operator== (const iSize &siz) const
	{
		return ((w==siz.w) && (h==siz.h));
	}

	/// Inequality operator
	bool operator!= (const iSize &siz) const
	{
		return ! operator == ( siz );
	}

	/// Inflates size
	inline void InflateSize(uint32 w_offs, uint32 h_offs)
	{
		w+=w_offs;
		h+=h_offs;
	}

	/// Inflates size
	inline void InflateSize(uint32 offs)
	{
		InflateSize(offs,offs);
	}

	/// Deflates size
	inline void DeflateSize(uint32 w_offs, uint32 h_offs)
	{
		w-=w_offs;
		h-=h_offs;
	}

	/// Deflates size
	inline void DeflateSize(uint32 offs)
	{
		DeflateSize(offs,offs);
	}

	/// Returns aspect ratio width:height
	inline float GetAspectRatio() const
	{
		return (float)w / float(h);
	}

	/// Return SIZE struct
	operator SIZE() const
	{
		SIZE siz={w,h};
		return siz;
	}

	uint32 w;
	uint32 h;
};

/// Point class
class iPoint
{
public:

	/// Default constructor
	iPoint ()
	{
		x = 0;
		y = 0;
	}

	/// Constructs iPoint object with given x and y coordinates
	iPoint ( sint32 tx, sint32 ty )
	{
		x = tx;
		y = ty;
	}

	/// Constructs iPoint object with given POINT structure
	iPoint (POINT &pnt)
	{
		x=pnt.x;
		y=pnt.y;
	}

	/// Increments x and y coordinates by given value
	void operator += (const iPoint &pos)
	{
		x=x+pos.x;
		y=y+pos.y;
	}

	/// Decrements x and y coordinates by given value
	void operator -= (const iPoint &pos)
	{
		x=x-pos.x;
		y=y-pos.y;
	}

	/// operator +
	iPoint operator + (const iPoint &pos) const
	{
		return (iPoint(x+pos.x,y+pos.y));
	}

	/// operator -
	iPoint operator - (const iPoint &pos) const
	{
		return (iPoint(x-pos.x,y-pos.y));
	}

	/// Increments x and y coordinates by given value
	void operator += (const iSize &siz)
	{
		x=x+siz.w;
		y=y+siz.h;
	}

	/// Decrements x and y coordinates by given value
	void operator -= (const iSize &siz)
	{
		x=x-siz.w;
		y=y-siz.h;
	}

	/// operator +
	iPoint operator + (const iSize &siz) const
	{
		return (iPoint(x+siz.w,y+siz.h));
	}

	/// operator -
	iPoint operator - (const iSize &siz) const
	{
		return (iPoint(x-siz.w,y-siz.h));
	}

	/// Increments x and y coordinates by given value
	void operator += (sint32 offs)
	{
		x=x+offs;
		y=y+offs;
	}

	/// Decrements x and y coordinates by given value
	void operator -= (sint32 offs)
	{
		x=y-offs;
		x=y-offs;
	}

	/// operator +
	iPoint operator + (sint32 offs) const
	{
		return (iPoint(x+offs,y+offs));
	}

	/// operator -
	iPoint operator - (sint32 offs) const
	{
		return (iPoint(x-offs,y-offs));
	}

	/// Equality operator
	inline bool operator== (const iPoint &pos) const
	{
		return ((x==pos.x) && (y==pos.y));
	}

	/// Inequality operator
	inline bool operator!= (const iPoint &pos) const
	{
		return ! operator == ( pos );
	}


	/// Returns POINT structure
	operator POINT() const
	{
		POINT pnt={x,y};
		return pnt;
	}

	/// Returns delta distance to the specified point
	uint32 GetDelta(const iPoint& pnt)
	{
		sint32 dx = pnt.x - x;
		sint32 dy = pnt.y - y;

		return (uint32)sqrt(float(dx*dx)+float(dy*dy));
	}

	/// Offsets x coordinate
	void MoveX(sint32 offset_x)
	{
		x += offset_x;
	}

	/// Offsets y coordinate
	void MoveY(sint32 offset_y)
	{
		y += offset_y;
	}

	/// Offsets x and y coordinates
	void Move(sint32 offset_x, sint32 offset_y)
	{
		x += offset_x;
		y += offset_y;
	}

	sint32 x;
	sint32 y;
};

/// Line class
class iLine
{
public:
	/// Default constructor
	iLine()
	{
		m_Point1 = iPoint(0,0);
		m_Point2 = iPoint(0,0);
	}

	/// Copy constructor
	iLine (iLine &tline)
	{
		m_Point1 = tline.GetPoint1();
		m_Point2 = tline.GetPoint2();
	}
	
	/// Constructs iLine object from two points
	iLine (const iPoint &tpoint1, const iPoint &tpoint2)
	{
		m_Point1 = tpoint1;
		m_Point2 = tpoint2;
	}
	
	/// Constructs iLine object from two points
	iLine (const sint32 x1, const sint32 y1, const sint32 x2, const sint32 y2)
	{
		m_Point1 = iPoint(x1,y1);
		m_Point2 = iPoint(x2,y2);
	}

	/// Returns first point's x coordinate
	sint32 X1() const
	{
		return m_Point1.x;
	}

	/// Returns first point's y coordinate
	sint32 Y1() const
	{
		return m_Point1.y;
	}

	/// Returns second point's x coordinate
	sint32 X2() const
	{
		return m_Point2.x;
	}

	/// Returns second point's y coordinate
	sint32 Y2() const
	{
		return m_Point2.y;
	}
	
	/// Returns first point
	iPoint GetPoint1() const
	{
		return m_Point1;
	}

	/// Returns second point
	iPoint GetPoint2() const
	{
		return m_Point2;
	}
	
private:
	iPoint m_Point1;
	iPoint m_Point2;
};


/// Rect class
class iRect : public iPoint, public iSize
{
public:
	/// Default constructor
	iRect ()
	{
		x = 0;
		y = 0;
		w = 0;
		h = 0;
	}
	
	/// Constructs iRect object with given coordinate of left top corner, width and height
	iRect ( sint32 tx, sint32 ty, uint32 tw, uint32 th )
	{
		x = tx;
		y = ty;
		w = tw;
		h = th;
	}

	/// Constructs iRect object with given coordinate of left top corner, width and height
	iRect ( const iPoint& point,const iSize& size )
	{
		x = point.x;
		y = point.y;
		w = size.w;
		h = size.h;
	}

	/// Constructs iRect object with two given coordinates
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

	/// Constructs iRect object with specified size
	iRect ( const iSize& size )
	{
		x = 0;
		y = 0;
		w = size.w;
		h = size.h;
	}

	/// Constructs iRect object with RECT structure
	iRect ( const RECT& rect )
	{
		x=rect.left;
		y=rect.top;
		w=rect.right-rect.left;
		h=rect.bottom-rect.top;
	}

	/// Returns x coordinate of left-top corner
	inline sint32 x1 () const
	{ 
		return x; 
	}

	/// Returns y coordinate of left-top corner
	inline sint32 y1 () const
	{ 
		return y; 
	}

	/// Returns x coordinate of right-bottom corner
	inline sint32 x2 () const 
	{ 
		return x+(sint32)w-1; 
	}
	
	/// Returns y coordinate of right-bottom corner
	inline sint32 y2 () const 
	{ 
		return y+(sint32)h-1; 
	}

	/// Returns center point of rect
	inline iPoint Center() const
	{
		return iPoint(x+(sint32)w/2,y+(sint32)h/2);
	}

	/// Returns coordinate of right-top corner
	inline iPoint TopRight() const
	{
		return iPoint(x2(),y);
	}

	/// Returns coordinate of left-top corner
	inline iPoint TopLeft() const
	{
		return iPoint(x,y);
	}

	/// Returns coordinate of right-bottom corner
	inline iPoint BottomRight() const
	{
		return iPoint(x2(),y2());
	}

	/// Returns coordinate of left-bottom corner
	inline iPoint BottomLeft() const
	{
		return iPoint(x,y2());
	}

	/// Returns size of rect
	iSize size () const 
	{ 
		return iSize(w,h); 
	}

	/// Returns coordinate of left-top corner
	iPoint point () const 
	{ 
		return iPoint(x,y); 
	}

	/// @brief Determines whether the specified point lies within the specified rectangle. 
	/// A point is within a rectangle if it lies on the left or top side or is within all four sides. 
	inline bool PtInRect(const sint32 _x, const sint32 _y) const
	{ 
		return (_x>=x && _y>=y && _x<sint32(x+w) && _y<sint32(y+h));
	}

	/// @brief Determines whether the specified point lies within the specified rectangle. 
	/// A point is within a rectangle if it lies on the left or top side or is within all four sides. 
	inline bool PtInRect(const iPoint& pnt) const
	{ 
		return PtInRect(pnt.x,pnt.y);
	}

	/// Resets rect variables to zero
	void Reset()
	{
		x=y=w=h=0;
	}

	/// Validates width and height of the rect
	inline bool IsEmpty() const 
	{
		return (w==0 || h==0);
	}
	
	/// Returns RECT structure
	operator RECT() const
	{
		RECT rect={x,y,x+w,y+h};
		return rect;
	}
	
	/// Equality operator
	inline bool operator== (const iRect &rect) const
	{
		return ((x==rect.x) && (y==rect.y) && (w==rect.w) && (h==rect.h));
	}

	/// Inequality operator
	inline bool operator!= (const iRect &rect) const
	{
		return ! operator == ( rect );
	}

	/// operator +
	inline iRect operator + (const iPoint &pos) const
	{
		return (iRect(x+pos.x,y+pos.y,w,h));
	}

	/// operator -
	inline iRect operator - (const iPoint &pos) const
	{
		return (iRect(x-pos.x,y-pos.y,w,h));
	}

	/// operator +
	inline iRect operator + (const iRect &rect) const
	{
		iRect rc( *this );
		return  rc += rect;
	}

	/// operator +=
	iRect& operator += (const iRect &rect)
	{
		if (rect.IsEmpty()) return *this;
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

	/// Inflates rect
	inline void InflateRect(uint32 left, uint32 top, uint32 right, uint32 bottom)
	{
		x -= left;
		y -= top;
		w += (left+right);
		h += (top+bottom);
	}

	/// Inflates rect
	inline void InflateRect(uint32 x_offs, uint32 y_offs)
	{
		InflateRect(x_offs,y_offs,x_offs,y_offs);
	}

	/// Inflates rect
	inline void InflateRect(uint32 offs)
	{
		InflateRect(offs,offs,offs,offs);
	}

	/// Deflates rect
	inline void DeflateRect(uint32 left, uint32 top, uint32 right, uint32 bottom)
	{
		x += left;
		y += top;
		w -= (left+right);
		h -= (top+bottom);
	}

	/// Deflates rect
	inline void DeflateRect(uint32 x_offs, uint32 y_offs)
	{
		DeflateRect(x_offs,y_offs,x_offs,y_offs);
	}

	/// Deflates rect
	inline void DeflateRect(uint32 offs)
	{
		DeflateRect(offs,offs,offs,offs);
	}
};

/// iFixedPoint based point class
class iPointFix
{
public:
	/// Default constructor
	iPointFix() {}

	/// Constructs iPointFix object with given x and y coordinates
	iPointFix(fix32 _x, fix32 _y)
	: x(_x)
	, y(_y) {}

	/// Constructs iPointFix object with given x and y coordinates
	iPointFix(sint32 _x, sint32 _y)
	: x(_x)
	, y(_y) {}

	/// Constructs iPointFix object with given x and y coordinates
	iPointFix(float _x, float _y)
	: x(_x)
	, y(_y) {}

	/// Constructs iPointFix object with given point coordinates
	iPointFix(const iPoint& pos)
	: x(pos.x)
	, y(pos.y) {}

	/// operator +=
	void operator += (const iPointFix &pos)
	{
		x=x+pos.x;
		y=y+pos.y;
	}

	/// operator -=
	void operator -= (const iPointFix &pos)
	{
		x=x-pos.x;
		y=y-pos.y;
	}

	/// operator +
	iPointFix operator + (const iPointFix &pos) const
	{
		return (iPointFix(x+pos.x,y+pos.y));
	}

	/// operator -
	iPointFix operator - (const iPointFix &pos) const
	{
		return (iPointFix(x-pos.x,y-pos.y));
	}

	/// Equality operator
	inline bool operator== (const iPointFix &pos) const
	{
		return ((x==pos.x) && (y==pos.y));
	}

	/// Inequality operator
	inline bool operator!= (const iPointFix &pos) const
	{
		return ! operator == ( pos );
	}

	/// Returns integer values of x and y coordinates
	inline iPoint IntPoint() const
	{
		return iPoint(x.ceil(),y.ceil());
	}


	fix32	x;
	fix32	y;
};

/// Metrics primitives clipping wrapper class
class iClipper
{
public:
	/// Clips point relative to rect
	inline static bool ClipPoint(iPoint &pnt, const iRect &rect)
	{
		pnt.x = iCLAMP(rect.x,rect.x2(),pnt.x);
		pnt.y = iCLAMP(rect.x,rect.y2(),pnt.y);
		return true;
	}

	/// Clips horizontal line relative to rect
	inline static bool ClipHLine(iPoint &pnt1, sint32 &x2, const iRect &rect)
	{
		if (pnt1.y < rect.y || pnt1.y > rect.y2()) return false;
		pnt1.x = iCLAMP(rect.x,rect.x2(),pnt1.x);
		x2 = iCLAMP(rect.x,rect.x2(),x2);
		return (pnt1.x != x2);
	}

	/// Clips vertical line relative to rect
	inline static bool ClipVLine(iPoint &pnt1, sint32 &y2, const iRect &rect)
	{
		if (pnt1.x < rect.x || pnt1.x > rect.x2()) return false;
		pnt1.y = iCLAMP(rect.y,rect.y2(),pnt1.y);
		y2 = iCLAMP(rect.y,rect.y2(),y2);
		return (pnt1.y != y2);
	}

	/// Clips vertical line relative to rect
	static bool ClipLine(iPoint &pnt1, iPoint &pnt2, const iRect &rect);

	/// @brief Calculates the intersection of two source rectangles and places the coordinates of the intersection rectangle into the destination rectangle. 
	/// If the source rectangles do not intersect, an empty rectangle (in which all coordinates are set to zero) is placed into the destination rectangle
	inline static bool IntersectRect(iRect &dst_rect, const iRect &src_rect1,const iRect &src_rect2)
	{

		sint32 x1 = iMAX(src_rect1.x, src_rect2.x);
		sint32 y1 = iMAX(src_rect1.y, src_rect2.y);

		sint32 x2 = iMIN(src_rect1.x2(), src_rect2.x2()) + 1;
		sint32 y2 = iMIN(src_rect1.y2(), src_rect2.y2()) + 1;

		if ( (x2 - x1) > 0 && (y2 - y1) > 0)
		{
			dst_rect = iRect(x1,y1,x2-x1,y2-y1);
			return true;
		}
		else return false;
	}

	/// Calculates the intersection of two source rectangles and returns result of the intersection 
	inline static bool IsIntersectRect(const iRect &src_rect1,const iRect &src_rect2)
	{
		sint32 x1 = iMAX(src_rect1.x, src_rect2.x);
		sint32 y1 = iMAX(src_rect1.y, src_rect2.y);

		sint32 x2 = iMIN(src_rect1.x2(), src_rect2.x2()) + 1;
		sint32 y2 = iMIN(src_rect1.y2(), src_rect2.y2()) + 1;

		if ( (x2 - x1) > 0 && (y2 - y1) > 0) return true;
		return false;
	}

	/// Clips two rects
	static bool ClipRect(iRect &rc, const iRect &rect);

	/// Clips two rects and returns intersection results for both sources
	static bool iClipRectRect(iRect &dst_rc, const iRect &dst_rect, iRect &src_rc, const iRect &src_rect);
};

/// 2D Matrix class template
template <class T>
class iMatrix
{
public:
	/// Copy constructor
	iMatrix(const iMatrix<T> &m)
	: m_Cell(NULL)
	{
		Replicate(m);
	}

	/// Constructs matrix with specified size
	iMatrix(const iSize &ms)
	: m_Cell(NULL)
	{
		Allocate(ms);
	}

	/// Constructs matrix with specified size
	iMatrix(uint32 mw, uint32 mh)
	: m_Cell(NULL)
	{
		Allocate(iSize(mw,mh));
	}

	/// Destructor
	~iMatrix()
	{
		Destroy();
	}

	/// Returns reference to the element at given position
	T& At(uint32 pos) 
	{
		return m_Cell[pos];
	}

	/// Returns constant reference to the element at given position
	const T& At(uint32 pos) const
	{
		return m_Cell[pos];
	}

	/// Returns reference to the element at given position
	T& At(const iPoint &pos) 
	{
		return At(pos.x,pos.y);
	}

	/// Returns reference to the element at given position
	T& At(uint32 x, uint32 y)
	{
		return (m_Cell[y*(m_Size.w) + x]);
	}

	/// Returns constant reference to the element at given position
	const T& At(const iPoint &pos) const
	{
		return At(pos.x,pos.y);
	}

	/// Returns constant reference to the element at given position
	const T& At(uint32 x, uint32 y) const
	{
		return (m_Cell[y*(m_Size.w) + x]);
	}

	/// Returns dimension of the matrix
	const inline iSize& GetSize() const
	{
		return m_Size;
	}

	/// Returns number of elements in the matrix
	const inline uint32 GetCount() const
	{
		return m_Size.w * m_Size.h;
	}

	/// Fills each cell of matrix with given value
	void Fill(const T& value)
	{
		for (int xx=0; xx<(m_Size.w*m_Size.h); ++xx) m_Cell[xx] = value;
	}

	/// Equality operator
	bool operator == (const iMatrix<T> &m)
	{
		if (m.GetSize() != GetSize()) return false;
		bool result = true;
		for (int xx=0; xx<(GetSize().w*GetSize().h); ++xx) 
		{
			if (m_Cell[xx] != m.m_Cell[xx])
			{
				result = false;
				break;
			}
		}
		return result;
	}

	/// Inequality operator
	bool operator != (const iMatrix<T> &m)
	{
		return ! operator == (m);
	}

	/// Resizes matrix with new size
	void Resize(const iSize &ms)
	{
		if (GetSize() != ms)
		{
			Destroy();
			Allocate(ms);
		}
	}

	/// Assignment operator
	iMatrix<T>& operator = (const iMatrix<T>& m)
	{
		Replicate(m);
	}
	
protected:

	/// Allocates buffer for matrix
	void Allocate(const iSize &ns)
	{
		m_Size=ns;
		m_Cell = new T[m_Size.w*m_Size.h];
	}

	/// Replicates matrix (dublicates)
	void Replicate(const iMatrix<T>& m)
	{
		Resize(m.GetSize());
		for (uint32 xx=0; xx<(m_Size.w*m_Size.h); ++xx) m_Cell[xx]=m.m_Cell[xx];
	}

	/// Cleanups
	void Destroy()
	{
		delete[] m_Cell;
		m_Cell = NULL;
		m_Size=iSize(0,0);
	}

	iSize	m_Size;
	T		*m_Cell;
};

/// Typed Point matrix class
class iPointMatrix : public iMatrix<iPoint>
{
public:
	/// Constructs iPointMatrix with given size
	iPointMatrix(const iSize& siz) : iMatrix<iPoint>(siz) {}
	/// Constructs iPointMatrix with given size
	iPointMatrix(uint32 w, uint32 h) : iMatrix<iPoint>(w,h) {}

	iPointMatrix operator+ (const iPoint& pos) const
	{
		iPointMatrix tmatrix(*this);
		for (uint32 xx=0; xx<GetCount(); ++xx){
			tmatrix.At(xx) += pos;
		}
		return tmatrix;
	}
};

/// Scale specified size according to another (preserve aspect ratio)
inline bool ScaleSize(iSize& src_siz, const iSize& bbox)
{
	float srcAspect = src_siz.GetAspectRatio();
	float bbxAspect = bbox.GetAspectRatio();
	
	// test weather wndAspect is bigger than video
	if ( bbxAspect > srcAspect ) {
		// correct width
		src_siz.w = iMIN((uint32)((float)(bbox.h) * srcAspect),bbox.h);
		src_siz.h = bbox.h;
	} else {
		// correct height;
		src_siz.w = bbox.w;
		src_siz.h = iMIN((uint32)((float)(bbox.w) / srcAspect),bbox.w);

	}

	return (src_siz.w && src_siz.h);
}

/// Scale specified rect to another circumscribed rect (preserve aspect ratio)
inline bool ScaleRect2Rect(const iRect& src, const iRect& dst, iRect& out)
{
	out = src;
	if (!ScaleSize(out,dst)) return false;
	out.x = dst.w/2 - out.w/2;
	out.y = dst.h/2 - out.h/2;
	return true;
}


/// Predefine invalid point value
extern const iPoint		cInvalidPoint;
/// Predefine invalid size value
extern const iSize		cInvalidSize;
/// Predefine invalid rect value
extern const iRect		cInvalidRect;

#endif //_IOLIB_HELIUM_METRICS_H_
/** @} */ // iolib_he
