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
 *	@brief iDib handling classes implementation
 */
/** @addtogroup iolib_xe
 *  @{
 */
#ifndef _IOLIB_XENON_DIB_H_
#define _IOLIB_XENON_DIB_H_


#include "xe.dib.llproc.h"
#include "..\\he\\he.string.h"
#include "..\\he\\he.file.h"
#include "..\\he\\he.debug.log.h"
#include "xe.gray.dib.h"

enum BlendMode
{
	BLEND_SRCCOPY		= 0,
	BLEND_ALPHABLEND	= 1,
	BLEND_ALPHAKEY		= 2,
	BLEND_COLORKEY		= 3
};

struct LL_DIB_FUNCTIONS
{
	void (*LLFillFunction)  (uint32 *dst, const uint32 src, uint32 size, void *lpParam);
	void (*LLBlendFunction)  (uint32* dst, const uint32* src, uint32 size, void *lpParam);
};

/// Windows device-independent bitmap (DIB) wrapper class
class iDib
{
	friend class iDibPrimitives;
	friend class iDibTransformer;
	friend class iDibColorChannel;
public:
	/// iDib default constructor
	iDib ();
	/// constructs iDib instance with specified size and color
	iDib ( const iSize& size, const iColor &clr );
	/// constructs iDib instance with specified size 
	iDib ( const iSize& size );
	/// constructs iDib instance from another iDib object
	iDib ( const iDib* pDib );
	/// constructs iDib instance from rect of another iDib object
	iDib ( const iDib* pDib, const iRect& rect);

	/// copy constructor
	iDib ( const iDib& other );
	/// assignment operator
	iDib& operator = ( const iDib& other );

	/// Initialize iDib object with specified size and color
	bool Create ( const iSize& size, const iColor &clr );
	/// Initialize iDib object with specified size
	bool Create ( const iSize& size );
	/// Initialize iDib object from another iDib object
	bool Create ( const iDib* pDib );
	/// Initialize iDib object from rect of another iDib object
	bool Create ( const iDib* pDib, const iRect& rect);

	/// Resizes dimension of iDib object
	void Resize ( const iSize& size );
	/// Resizes canvas dimension of iDib object
	void ResizeCanvas(const iSize& size, iAlignment algn, const iColor &fill);

	/// Destructor
	~iDib ();
	/// Cleanups everything 
	void Destroy ();

	/// Swaps content of two iDib objects
	void Swap( iDib& other );

	/// Validates Width equality
	inline bool CheckW ( uint32 w ) const 
	{ return ( m_Width == w ); }

	/// Validates Height equality
	inline bool CheckH ( uint32 h ) const
	{ return ( m_Height == h ); }

	/// Validates Width and Height equality
	inline bool CheckWH ( const iSize& size ) const
	{ return ( ( m_Width == size.w ) && ( m_Height == size.h ) ); }

	/// Validates X coordinate 
	inline bool IsValidX ( sint32 x ) const
	{ return ( ( x >= 0 ) && ( x < (sint32)m_Width ) ); }
	
	/// Validates Y coordinate 
	inline bool IsValidY ( sint32 y ) const
	{ return ( ( y >= 0 ) && ( y < (sint32)m_Height ) ); }
	
	/// Validates X and Y coordinates
	inline bool IsValidPoint ( const iPoint& pos ) const
	{ return ( ( pos.y >= 0 ) && ( pos.y < (sint32)m_Height ) && ( pos.x >= 0 ) && ( pos.x < (sint32)m_Width ) ); }

	/// Modifies specified pixel's channel value
	void SetPixelChannel(const iPoint& point, ColorChannel ch, uint8 value);
	/// Obtain pointer to the specified pixel
	iColor* GetPixelPtr(const iPoint& point);
	/// Obtain constant pointer to the specified pixel
	const iColor* GetPixelPtr(const iPoint& point) const;
	/// Obtain specified pixel
	iColor GetPixel(const iPoint& point) const;
	/// Put pixel
	void PutPixel ( const iPoint& point, const iColor& color, BlendMode mode = BLEND_SRCCOPY);
	
	/// Obtain specified pixel's channel value
	inline uint8 GetPixelChannel(const iPoint& point, ColorChannel cch) const
	{
		if (!IsValidPoint(point)) return 0;
		return *((uint8*)(GetLine(point.y)+point.x)+cch);
	}

	/// Obtain pointer to the specified line
	iColor* GetLine ( uint32 y );
	/// Obtain constant pointer to the specified line
	const iColor* GetLine ( uint32 y ) const;
	/// Put line
	void PutLine ( uint32 y, iColor* line );

	/// Fills whole iDib with specified color
	void Fill ( const iColor& color, BlendMode mode = BLEND_SRCCOPY);
	/// Fills iDib's rect with specified color
	void FillRect ( const iRect& rect, const iColor& color, BlendMode mode = BLEND_SRCCOPY);

	/// Blends specified dib to itself
	void CopyFromDib ( const iDib* pDib, BlendMode mode );
	/// Blends itself to the specified dib
	void CopyToDib ( iDib* pDib, BlendMode mode ) const;
	/// Blends itself to the specified position of given dib 
	void CopyToDibXY ( iDib* pDib, const iPoint& pos, BlendMode mode ) const;

	/// Blends rectangle area of dib to specified position of itself
	void CopyRectFromDib ( const iDib* pDib, const iRect& src_rect, const iPoint& dst_pos, BlendMode mode  );
	/// Blends rectangle area of itself to specified position of given dib
	void CopyRectToDibXY ( iDib* pDib, const iRect& src_rect, const iPoint& dst_pos, BlendMode mode  ) const;

	/// Blends itself to the specified dib (with clipping)
	void CopyToDibXYClip ( iDib* pDib, const iPoint& pos, const iRect& clip_rect, BlendMode mode ) const;
	/// Blends rectangle area of itself to specified position of given dib (with clipping)
	void CopyRectToDibXYClip ( iDib* pDib, const iRect& src_rect, const iPoint& dst_pos, const iRect& clip_rect, BlendMode mode  ) const;

	/// Tiles rectangle area of itself to the specified rect of given dib
	void TileRectToDibRect(const iRect src_rect, iDib &dib, const iRect dst_rect, BlendMode mode) const;
	/// Tiles itself to the specified rect of given dib
	void TileToDibRect(iDib &dib, const iRect dst_rect, BlendMode mode) const;
	/// Tiles itself to the given dib
	void TileToDib(iDib &dib, BlendMode mode) const;

	/// Returns width of allocated image
	inline uint32 GetWidth() const
	{ return m_Width; }

	/// Returns height of allocated image
	inline uint32 GetHeight() const
	{ return m_Height; }

	/// Returns size of allocated image
	inline iSize GetSize() const
	{ return iSize(m_Width,m_Height); }

	/// Returns size (in bytes) of allocated buffer
	inline uint32 GetByteSize() const
	{ return m_Width*m_Height*sizeof(iColor); }

	/// Returns constant pointer to the first pixel of the image
	inline const iColor* GetBits() const
	{ return m_Bits; }

	/// Returns pointer to the first pixel of the image
	inline iColor* GetBits()
	{ return m_Bits; }

	/// Returns pointer to BITMAPINFO struct
	inline BITMAPINFO* GetBitmapInfo()
	{ return &m_BitmapInfo; }

	/// Returns constant pointer to BITMAPINFO struct
	inline const BITMAPINFO* GetBitmapInfo() const
	{ return &m_BitmapInfo; }

	/// returns HANDLE to bitmap
	inline HBITMAP GetBitmap()
	{ return m_Bitmap; }

	/// returns HANDLE to bitmap
	inline HBITMAP GetBitmap() const
	{ return m_Bitmap; }

private:
	HBITMAP		m_Bitmap;
	BITMAPINFO	m_BitmapInfo;
	uint32		m_Width;
	uint32		m_Height;
	iColor		*m_Bits;

	bool InitBitmapContent ( const iSize& size );

	static LL_DIB_FUNCTIONS LLFunctions[4];
};

namespace iDibHelpers {
	iRect FindSolidArea(const iDib& dib);
 
}; //namespace iDibHelpers


/// Windows Device Context along with iDib manipulation
class iDibDC
{
public:
	/// Blends source iDib to the specified position of given Device Context
	static void DibToDCXY(const iDib &dib, HDC hdc, const iPoint &pos);
	/// Blends rectangle of source iDib to the specified position of given Device Context
	static void DibRectToDCXY(const iDib &dib, const iRect &src_rect, HDC hdc, const iPoint &pos);
	/// Stretches according to output rectangle's metrics and blends source iDib to the specified position of given Device Context
	static void StretchDibToDCRect(const iDib &dib, const iRect &dst_rect, HDC hdc);
	/// Stretches according to output rectangle's metrics and blends rectangle of the source iDib to the specified position of given Device Context
	static void StretchDibRectToDCRect(const iDib &dib, const iRect &src_rect, const iRect &dst_rect, HDC hdc);
	/// Copies rectangle of Device Context to the specified iDib object
	static void DCRectToDib(HDC hdc, const iRect &src_rect, iDib &dib, const iPoint &pos);

	/// Tiles rectangle area of given iDib object to the specified rect of Device Context
	static void TileDibRectToDCRect(const iDib& dib, const iRect& src_rect, const iRect& dst_rect, HDC hdc);
	/// Tiles given iDib object to the specified rect of Device Context
	static void TileDibToDCRect(const iDib& dib, const iRect& dst_rect, HDC hdc);

	/// Draws frame to the specified Device Context using given (3x3 elements) iDib template
	static void DrawDibFrame(const iDib& dib, uint32 seg_siz, const iRect& dst_rect,HDC hdc);
	/// Tiles (3x1 elements) iDib template to the specified Device Context
	static void DrawDibHTile(const iDib& dib, const iRect& dst_rect, HDC hdc);
	/// Tiles (1x3 elements) iDib template to the specified Device Context
	static void DrawDibVTile(const iDib& dib, const iRect& dst_rect, HDC hdc);
};

/// Text drawing wrapper class
class iDibText
{
public:
	/// Default constructor
	iDibText();
	/// Sets the text color
	void SetTextColor(const iColor &clr);
	/// Sets background color
	void SetBackColor(const iColor &clr);
	/// Sets font
	void SetFont(iFont& fnt);
	/// Sets text alignmemnt
	void SetTextAlignment(iAlignment algn=Align_LeftTop);
	/// Computes the width and height of the specified string of text
	iSize GetTextSize(const iStringT &text);
	/// Draws a character string by using the currently selected font.
	/// An optional rectangle may be provided, to be used for clipping, opaquing, or both
	void TextOut(iDib &dib, const iStringT &text, const iRect &rect=cInvalidRect, const iPoint &offset=cInvalidPoint);

private:
	void BlitText_SolidBackground(iDib &dib);
	void BlitText_TransparentBackground(iDib &dib);
	void BlitText_NormalBackground(iDib &dib);

	iColor		m_TextColor;
	iColor		m_BackColor;
	iFont		m_Font;
	iAlignment	m_Alignment;
};

/// iDib Transformation wrapper class (includes strething)
class iDibTransformer
{
public:
	/// Stretching methods enumeration
	enum Quality {
		Fake = 0,
		Fast,
		Normal,
		High,
		Pyramidal,
		SmartPyr
	};

	/// Multiplies given iDib image and returns result
	static iDib* GetMultipliedDib(const iDib& src, uint16 mby);
	/// Multiplies given iDib image
	static void MultiplySize(iDib &dib, uint16 mby);
	/// Stretches given iDib image using ultrafast draft algorithm
	static void FastStretch(iDib &dib, const iSize newsize);
	/// Stretches given iDib image using specified algorithm
	static void Stretch(const iDib& src, iDib& dst, Quality q);

	/// Flips given iDib image horizontally
	static void FlipHorizontal(iDib& dib);
	/// Flips given iDib image vertically
	static void FlipVertical(iDib& dib);
};

/// Primitives drawing wrapper class
class iDibPrimitives
{
public:
	/// Draws horizontal line with given parameters
	static void DrawHLine(iDib &dib, const iPoint &pos1, sint32 x2, const iColor& color, BlendMode mode);
	/// Draws vertical line with given parameters
	static void DrawVLine(iDib &dib, const iPoint &pos1, sint32 y2, const iColor& color, BlendMode mode);
	/// Draws line with given parameters
	static void DrawLine(iDib &dib, const iPoint &pos1, const iPoint &pos2, const iColor& color, BlendMode mode);
	/// Draws frame with given parameters
	static void DrawFrame(iDib &dib, const iRect &rect, const iColor &color, BlendMode mode);
	/// Blends 3D looks line with given parameters
	static void Compose3DFrame(iDib &dib, const iRect &rect, uint8 a, bool rised=true);
	/// Draws line with given parameters
	static void Draw3DFrame(iDib &dib, const iRect &rect, const iColor &clr1, const iColor &clr2, BlendMode mode);
};

/// Artifical image synthesizer wrapper class
class iDibFX
{
public:
	/// Synthesize clouds like iDib image
	/// Size of source dib must be power of 2 + 1
	static void GenerateClouds(iDib &dib);
};

/// Gradient factory wrapper class
class iDibGradient
{
public:
	/// Enumerate basic orientations
	enum Orientation {
		Vertical = 0,
		Horizontal
	};

	/// Generates and blends gradient according to the specified parameters
	static void ComposeGradient(iDib &dib, const iColor &clr1, const iColor &clr2, Orientation ornt = Horizontal)
	{
		ComposeGradientRect(dib, iRect(dib.GetSize()), clr1, clr2, ornt);
	}

	/// Generates and blends gradient according to the specified parameters
	static void ComposeGradientRect(iDib &dib, const iRect &rect, const iColor &clr1, const iColor &clr2, Orientation ornt = Horizontal)
	{
		iRect lrect(rect);
		if (!iClipper::ClipRect(lrect,dib.GetSize())) return;

		sint32 len;

		if (ornt == Horizontal ) len = lrect.w;
		else len = lrect.h;

		short dr = (short) ((128 * (clr2.r - clr1.r)) / len);
		short dg = (short) ((128 * (clr2.g - clr1.g)) / len);
		short db = (short) ((128 * (clr2.b - clr1.b)) / len);
		short da = (short) ((128 * (clr2.a - clr1.a)) / len);

		short sr = clr1.r * 128;
		short sg = clr1.g * 128;
		short sb = clr1.b * 128;
		short sa = clr1.a * 128;

		for (sint32 xx=0; xx < len; ++xx) {
			if (ornt ==Horizontal) 
				iDibPrimitives::DrawVLine(dib, iPoint(lrect.x+xx,lrect.y),lrect.y+lrect.h,iColor(sr >> 7,sg >> 7,sb >> 7,sa >> 7), BLEND_ALPHABLEND);
			else 
				iDibPrimitives::DrawHLine(dib, iPoint(lrect.x,lrect.y+xx),lrect.x+lrect.w,iColor(sr >> 7,sg >> 7,sb >> 7,sa >> 7), BLEND_ALPHABLEND);
			sr += dr;
			sg += dg;
			sb += db;
			sa += da;
		}
	}
};

/// Reads images from various external sources into iDib object
class iDibLoader
{
public:
	/// Loads image from resource
	static bool LoadFromResource(iDib &dib, uint32 resid);
	/// Loads image from file
	static bool Load(iDib &dib, const iStringT &fname);
	/// Loads image from Windows Bitmap (.BMP) file
	static bool LoadBmp(iDib &dib, const iStringT &fname);
	/// Loads image from (.GIF) file
	static bool LoadGif(iDib &dib, const iStringT &fname);
	/// Loads image from (.PNG) file
	static bool LoadPng(iDib &dib, const iStringT &fname);
	/// Loads image from (.TGA) file
	static bool LoadTga(iDib &dib, const iStringT &fname);
	/// Loads image from (.PNG) stream
	static bool LoadPng(iDib &dib, iFileI *fio);
};

/// Writes image into file
class iDibSaver
{
public:
	/// Saves iDib image into 32bit Windows Bitmap
	static bool SaveBmp(const iDib &dib, const iStringT &fname);
	/// Saves iDib image into 16bit (565) Windows Bitmap
	static bool SaveBmp16(const iDib &dib, const iStringT &fname);
	/// Saves iDib image into 32bit PNG
	static bool SavePNG(const iDib &dib, const iStringT &fname);
};


/// Color channels manipulation wrapper class
class iDibColorChannel
{
public:
	/// Makes alpha channel according to given key color
	static void CKey2AKey(iDib &dib, const iColor &ck)
	{
		for (uint32 yy=0; yy<dib.GetHeight(); ++yy) {
			for (uint32 xx=0; xx<dib.GetWidth(); ++xx) {
				iColor *src = dib.GetPixelPtr(iPoint(xx,yy));
				if ( *src == ck ) {
					dib.SetPixelChannel(iPoint(xx,yy),Channel_Alpha,0);
				} else dib.SetPixelChannel(iPoint(xx,yy),Channel_Alpha,255);
			}
		}
	}

	/// Half tones alpha channel (i.e. makes it more transparent)
	static void HalftoneAlpha(iDib &dib, uint8 power)
	{
		check(power > 0);
		uint8 *srcPtr = ((uint8*)dib.m_Bits)+3;
		for (uint32 pos=0; pos < dib.m_Width*dib.m_Height; ++pos) {
			*srcPtr /= power;
			srcPtr += 4;
		}
	}

	/// Converts image to gray scale
	static void MakeGrayscale(iDib &dib)
	{
		uint8* srcPtr = (uint8*)dib.m_Bits;
		for (uint32 pos=0; pos < dib.m_Width*dib.m_Height; ++pos) {
			uint16 val = ((uint16)(*(srcPtr)) + (uint16)(*(srcPtr+1)) + (uint16)(*(srcPtr+2)))/3;
			memset(srcPtr,val,3);
			srcPtr+=4; 
		}
	}

	/// Copies values of _src_ channel into _dst_
	static void CopyChannel(iDib &dib, ColorChannel src, ColorChannel dst)
	{
		if (src == dst) return;
		for (uint32 pos=0; pos < dib.m_Width*dib.m_Height; ++pos) {
			*((uint8*)(dib.m_Bits+pos)+dst) = *((uint8*)(dib.m_Bits+pos)+src);
		}
	}

	/// Fills specified channel with given value
	static void FillChannel(iDib &dib, ColorChannel chn, uint8 val)
	{
		for (uint32 pos=0; pos < dib.m_Width*dib.m_Height; ++pos) {
			*((uint8*)(dib.m_Bits+pos)+chn) = val;
		}
	}

};

/// iDib processing filters
class iDibFilters
{
public:
	/// Blures given iDib image with specified parameters
	static void Blur(const iGrayDib &source, iGrayDib &output, uint8 weight, uint8 pass);
	/// Blures specified channels of given iDib image
	static void MaskedBlur(ChannelMask mask, const iDib &source, iDib &output, uint8 weight, uint8 pass);
};

/// Gray scale image manipulations wrapper class
class iDibGrayDibHandl
{
public:
	/// Copies specified channel's content of given dib into iGrayDib
	static void DibChannel2GrayDib(const iDib &src, iGrayDib &dst, const ColorChannel ch);
	/// Copies rectangle area of the specified channel's content of given dib into iGrayDib
	static void DibChannelRect2GrayDib(const iDib &src, const iRect &src_rect, iGrayDib &dst, const ColorChannel ch);
	/// Copies given iGrayDib image into specified channel's content of given dib
	static void GrayDib2DibChannel(const iGrayDib &src, iDib &dst, const ColorChannel ch);
	/// Copies rectangle area of the given iGrayDib image into specified channel's content of given dib
	static void GrayDibRect2DibChannel(const iGrayDib &src, const iRect &srect, iDib &dst, const iPoint &pos, const ColorChannel ch);
	/// Blends iGrayDib image to iDib image with specified color
	static void ComposeGrayDib2Dib(const iGrayDib &src, iDib &dst, const iPoint &pos, const iColor &clr);
	/// Blends rectangle area of the iGrayDib image to iDib image with specified color
	static void ComposeGrayDibRect2Dib(const iGrayDib &src, iDib &dst, const iRect &srect, const iPoint &pos, const iColor &clr);
};

/// Generic RLE coding algorithm functions implementation
class iMemoryDib
{
public:
	/// Converts Bit sequence into Byte sequence
	static bool Bit2Byte(const iDynamicBuffer& in_buff, iDynamicBuffer& out_buff);
	/// Converts Byte sequence into Bit sequence
	static bool Byte2Bit(const iDynamicBuffer& in_buff, iDynamicBuffer& out_buff);

	/// Converts iGrayDib image into RLE bit sequence
	static bool Dib2BitRLE(const iGrayDib& in_dib, iDynamicBuffer& out_buff);
	/// Converts iGrayDib image into RLE byte sequence
	static bool Dib2ByteRLE(const iGrayDib& in_dib, iDynamicBuffer& out_buff);
	
	/// Converts RLE bit sequence into iGrayDib image 
	static bool BitRLE2Dib(const iDynamicBuffer& in_buff, const iSize& siz, iGrayDib &out_dib);
	/// Converts RLE bit sequence into iDib image 
	static bool BitRLE2Dib(const iDynamicBuffer& in_buff, const iSize& siz, iDib &out_dib);
	/// Converts RLE byte sequence into iGrayDib image 
	static bool ByteRLE2Dib(const iDynamicBuffer& in_buff, const iSize& siz, iGrayDib &out_dib);
};

#endif //_IOLIB_XENON_DIB_H_
/** @} */ // iolib_xe
