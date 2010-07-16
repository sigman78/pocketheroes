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

#ifndef _GXLIB_DIB_H_
#define _GXLIB_DIB_H_

#define RGB16(r,g,b) (uint16)((r&0xF8)<<8 | (g&0xFC)<<3 | (b&0xF8)>>3)
//////////////////////////////////////////////////////////////////////////


always_inline uint32 
Darken50( uint32 pixel )
{
	return (pixel & 0xf7de) >> 1;
}

always_inline uint32 
Darken25( uint32 pixel )
{
	pixel = (pixel & 0xf7de) >> 1;
	pixel += (pixel & 0xf7de) >> 1;
	return pixel;
}

extern const uint32 BWPAL[32];

always_inline uint32 
DarkenBWBlend( uint32 a, uint32 b )
{ return ( (a & 0xf7de) >> 1 ) + ((b & 0xf7de) >> 1); }

always_inline uint32 
DarkenBW( uint32 pixel )
{ 
	static uint8 rpt = 31;
	uint32 chnl = (pixel >> 6) & 0x1f;
	return DarkenBWBlend(BWPAL[chnl], 0x39e7 );
} 


/*
 * iDib
 */
class iDib
{
public:
	typedef uint16 pixel;
	enum Type {
		RGB = 0,	// R5G6G5
		RGBA,		// R4G4B4A4
		RGBCK,		// R5G5G5A1
		TypeCount
	};

private:
	iBuff<pixel>	m_RGB;
	Type			m_dibType;
	iSize			m_Siz;

public:
	iDib();
	iDib(const iDib& dib);
	iDib(const iDib& dib, const iRect& rect);
	iDib(const iSize& siz, Type dType);
	void Init(const iDib& dib);
	void Init(const iDib& dib, const iRect& rect);
	void Init(const iSize& siz, Type dType);
	void Cleanup();
	void Resize(const iSize& siz);
	~iDib();

	iDib& operator = ( const iDib& other );
	
	inline bool IsValidPos(sint32 x, sint32 y)
	{ return (x>=0 && y>=0 && x<(sint32)m_Siz.w && y<(sint32)m_Siz.h);}

	inline void PutPixelSafe(sint32 x, sint32 y, pixel clr)
	{ if (IsValidPos(x,y)) m_RGB[y*m_Siz.w+x] = clr; }

	inline void PutPixel(sint32 x, sint32 y, pixel clr)
	{ m_RGB[y*m_Siz.w+x] = clr; }

	inline void PutPixel(const iPoint& pos, pixel clr)
	{ m_RGB[pos.y*m_Siz.w+pos.x] = clr; }

	inline pixel GetPixel(sint32 x, sint32 y) const
	{ return m_RGB[y*m_Siz.w+x]; }

	inline pixel GetPixel(const iPoint& pos) const
	{ return m_RGB[pos.y*m_Siz.w+pos.x]; }

	inline uint32 GetBuffLen() const
	{ return m_Siz.w * m_Siz.h * sizeof(pixel); }

	inline pixel* GetPtr(const iPoint& pos)
	{ return m_RGB + (pos.y*m_Siz.w)+pos.x; }

	inline const pixel* GetPtr(const iPoint& pos) const
	{ return m_RGB + (pos.y*m_Siz.w)+pos.x; }

	inline pixel* GetPtr()
	{ return m_RGB; }

	inline const pixel* GetPtr() const
	{ return m_RGB; }

	inline Type GetType() const
	{ return m_dibType; }

	inline bool IsEmpty() const
	{ return m_Siz.IsZero(); }

	inline iSize GetSize() const
	{ return m_Siz; }

	inline uint32 GetWidth() const
	{ return m_Siz.w; }

	inline uint32 GetHeight() const
	{ return m_Siz.h; }

	// Primitives
	void Fade(uint8 a);
	void Fill(pixel clr, uint8 a=255);
	void FillRect(const iRect& rc, pixel clr, uint8 a=255);
	void DarkenBWRect(const iRect& rc);
	void Darken50Rect(const iRect& rc);
	void Darken25Rect(const iRect& rc);

	void HGradientRect(const iRect& rc, pixel c1, pixel c2);
	void VGradientRect(const iRect& rc, pixel c1, pixel c2);

	void FrameRect(const iRect& rc, pixel clr, uint8 a=255);
	void HLine(const iPoint& pos, sint32 x2, pixel clr, uint8 a=255);
	void VLine(const iPoint& pos, sint32 y2, pixel clr, uint8 a=255);
	void Line(const iPoint& p1, const iPoint& p2, pixel clr, uint8 a=255);

	void Triangle(const iPoint& p1, const iPoint& p2, const iPoint& p3, pixel clr);

	// Copy/Blend
	void CopyToDibXY(iDib& dib, const iPoint& pos, uint8 a) const;
	void CopyToDibXY(iDib& dib, const iPoint& pos) const;
	void CopyRectToDibXY(iDib& dib, const iRect& srect, const iPoint& pos, uint8 a = 255) const;
	void BlendToDibXY(iDib& dib, const iPoint& pos, pixel ck, uint8 bval) const;

#ifndef UNDER_CE
	void BlitToDCXY(HDC hdc, const iPoint& pos, bool bDoubleSize=false) const;
#endif

public:
	static inline pixel BlendPixels(iDib::pixel dst, const iDib::pixel src, uint8 a)
	{
		uint8 inv_a = 255-a;
		uint32 sr = a * ((src & (0x1F<<11)) >> 11);
		uint32 sg = a * ((src & (0x3F<<5)) >> 5);
		uint32 sb = a * ((src & 0x1F));
		uint32 dr = inv_a * ((dst & (0x1F<<11)) >> 11);
		uint32 dg = inv_a * ((dst & (0x3F<<5)) >> 5);
		uint32 db = inv_a * ((dst & 0x1F));
		return (uint16)(((sr+dr)>>8)<<11 | ((sg+dg)>>8)<<5 | ((sb+db)>>8));
	}


private:
	void Swap(iDib& other);
	void Allocate(const iSize& siz);
	void Allocate(const iDib& dib);
};

//
const iDib::pixel cColor_White		= 0xFFFF;
const iDib::pixel cColor_Black		= 0x0000;
const iDib::pixel cColor_Gray192	= 0xC618;
const iDib::pixel cColor_Gray128	= 0x8410;
const iDib::pixel cColor_Gray64		= 0x4208;
const iDib::pixel cColor_Red		= 0xF800;
const iDib::pixel cColor_Green64	= 0x0200;
const iDib::pixel cColor_Green128	= 0x03E0;
const iDib::pixel cColor_Green192	= 0x05E0;
const iDib::pixel cColor_Green		= 0x07E0;
const iDib::pixel cColor_Blue		= 0x001F;
const iDib::pixel cColor_Blue64		= 0x0008;
const iDib::pixel cColor_Blue128	= 0x0010;
const iDib::pixel cColor_Blue192	= 0x0018;
const iDib::pixel cColor_Yello		= 0xFFE0;
const iDib::pixel cColor_Yello192	= 0xBDE0;

// 
class iGradient
{
public:
	iGradient(const iDib::pixel* pGrad, uint32 cnt) : m_pGrad(pGrad), m_cnt(cnt) {}
	iGradient(const iGradient& other) : m_pGrad(other.m_pGrad), m_cnt(other.m_cnt) {}
	inline uint32 Count() const { return m_cnt; }
	inline iDib::pixel GradValue(uint32 pos) const { if (!IsValid()) return 0; return m_pGrad[iCLAMP<uint32>(0, m_cnt-1, pos)]; }
	inline bool IsValid() const { return m_cnt > 0; }

private:
	const iDib::pixel*	m_pGrad;
	uint32				m_cnt;
};


/*
 *	iPalette
 */
class iPalette
{
public:
	iPalette()
	{}

	iPalette(const uint16* pal)
	{ Init(pal); }

	void Init(const uint16* pal)
	{ memcpy(m_Palette,pal,256*sizeof(uint16)); }

	const uint16& operator[] (uint32 nIndex) const
	{
		check(nIndex >= 0 && nIndex < 256);
		return m_Palette[nIndex];
	}

	uint16& operator[] (uint32 nIndex)
	{
		check(nIndex >= 0 && nIndex < 256);
		return m_Palette[nIndex];
	}

	uint16* GetPtr()
	{ return m_Palette; }
	

private:
	uint16	m_Palette[256];
};

/*
 *	iPalette Dib i.e. 8bit dib with palette
 */
class iPaletteDib
{
public:
	typedef uint8	pixel;
private:
	iBuff<pixel>	m_RGB;
	iSize			m_Siz;
public:
	iPaletteDib();
	~iPaletteDib();
	iPaletteDib(const iSize& siz);
	void Init(const iSize& siz);
	void CopyToDibXY(iDib& dib, const iPoint& pos, const iPalette& pal, uint8 alpha=255) const;
	void CopyRectToDibXY(iDib& dib, const iRect& srect, const iPoint& pos, const iPalette& pal, uint8 alpha=255) const;
	
	// inlines
	inline uint32 GetBuffLen() const
	{ return m_Siz.w * m_Siz.h * sizeof(pixel); }

	inline pixel* GetPtr()
	{ return m_RGB; }

	inline const pixel* GetPtr() const
	{ return m_RGB; }

	inline bool IsEmpty() const
	{ return m_Siz.IsZero(); }

	inline iSize GetSize() const
	{ return m_Siz; }

	inline uint32 GetWidth() const
	{ return m_Siz.w; }

	inline uint32 GetHeight() const
	{ return m_Siz.h; }


private:
	void Allocate(const iSize& siz);
	void Cleanup();
};

/*
 *	Dib Transform
 */
namespace iDibTransform {
	void Rotate(const iDib& src, iDib &dst, uint32 angle);
	void FastStretch(const iDib& src, iDib &dst, uint32 boffs);
	void PyramidalStretch(const iDib& src, iDib &dst, uint32 boffs);
}

/*
 * Dib Reader
 */
class iDibReaderImpl;
class iDibReader
{
private:
	iSmartPtr<iDibReaderImpl> pimpl;
public:
	enum IMG_TYPE {
		IT_UNKNOWN,
		IT_BMP,
		IT_GIF,
		IT_JPG
	};

	iDibReader();
	~iDibReader();
	bool Init();
	void Cleanup();
	bool FromFile(iDib& dib, const iStringT& fname) const;
	bool FromStream(iDib& dib, iFileI* pFile, IMG_TYPE it) const;
	bool FromFile(iPaletteDib& dib, const iStringT& fname) const;
	bool FromStream(iPaletteDib& dib, iFileI* pFile, IMG_TYPE it) const;
};

/*
 *	Fx
 */

/*
 *	Save bitmap
 */
bool SaveDibBitmap32(const iDib& dib, const iStringT& fname);
bool SaveDibBitmap16(const iDib& dib, const iStringT& fname);

#endif //_GXLIB_DIB_H_

