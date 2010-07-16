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
 *	@brief iDib based Memory Device Context class implementation
 */
/** @addtogroup iolib_xe
 *  @{
 */
#ifndef _IOLIB_XENON_MEMORY_DC_WRAPER_H_
#define _IOLIB_XENON_MEMORY_DC_WRAPER_H_

/// iDib based Memory Device Context class
class iMemoryDC
{
public:

	/// Default constructor
	iMemoryDC()
	: m_hDC(NULL)
	, m_oldFnt(NULL)
	{ }

	/// Destructor
	~iMemoryDC()
	{
		Destroy();
	}

	/// Constructs iMemoryDC object with specified size
	iMemoryDC(const iSize& siz)
	: m_hDC(NULL)
	, m_oldFnt(NULL)
	{
		InitDC(siz);
	}

	/// Constructs iMemoryDC object with specified iDib image
	iMemoryDC(const iDib& dib)
	: m_hDC(NULL)
	, m_oldFnt(NULL)
	{
		InitDC(dib);
	}

	/// Deinitializes object
	void Destroy()
	{
		if (m_hDC) {
			::SelectObject(m_hDC,m_oldBitmap);
			m_Dib.Destroy();

			if (m_oldFnt) {
				::SelectObject(m_hDC,m_oldFnt);
				m_oldFnt = NULL;
			}

			::DeleteDC(m_hDC);
			m_hDC = NULL;
		}
	}

	/// Initializes iMemoryDC object with specified size
	void InitDC(const iSize &siz = cInvalidSize)
	{
		if (siz != cInvalidSize) m_Dib.Create(siz);
		else m_Dib.Create(iSize(GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN)));

		HDC ddc = ::GetDC(0);
		m_hDC = CreateCompatibleDC(ddc);
		::ReleaseDC(0,ddc);

		m_oldBitmap = ::SelectObject(m_hDC,m_Dib.GetBitmap());
	}

	/// Initializes iMemoryDC object with specified iDib image
	void InitDC(const iDib &dib)
	{
		m_Dib.Create(&dib);
		HDC ddc = ::GetDC(0);
		m_hDC = CreateCompatibleDC(ddc);
		::ReleaseDC(0,ddc);

		m_oldBitmap = ::SelectObject(m_hDC,m_Dib.GetBitmap());
	}

	/// Reallocates iMemoryDC object with new size
	void Resize(const iSize &nsize)
	{
		if (nsize == GetSize()) return;
		Destroy();
		InitDC(nsize);
	}

	/// Reallocates iMemoryDC object with new iDib image
	void SetDib(const iDib &dib)
	{
		Destroy();
		InitDC(dib);
	}

	/// Validate initialization state
	inline bool IsInited() const
	{
		return (m_hDC != NULL);
	}

	/// Returns size of allocated image
	inline iSize GetSize() const
	{
		return m_Dib.GetSize();
	}

	/// Returns handle to the Device Context
	operator const HDC() const
	{
		return m_hDC;
	}

	/// Returns reference to the allocated iDib object
	operator const iDib&() const
	{
		return m_Dib;
	}

	/// Returns pointer to the allocated iDib object
	operator iDib*()
	{
		return &m_Dib;
	}

	/// Returns handle to Bitmap
	operator HBITMAP()
	{
		return m_Dib.GetBitmap();
	}

	/// Draws frame rect using inverted brush
	void FillInvRect(const iRect &rect, uint32 wg = 1)
	{
		// top
		{
			RECT trect = {rect.x+wg,rect.y,rect.x2()+1-wg,rect.y+wg};
			InvertRect(m_hDC,&trect);
		}
		// bottom
		{
			RECT trect = {rect.x+wg,rect.y2()+1-wg,rect.x2()+1-wg,rect.y2()+1};
			InvertRect(m_hDC,&trect);
		}
		// left
		{
			RECT trect = {rect.x,rect.y,rect.x+wg,rect.y2()+1};
			InvertRect(m_hDC,&trect);
		}
		// right
		{
			RECT trect = {rect.x2()+1-wg,rect.y,rect.x2()+1,rect.y2()+1};
			InvertRect(m_hDC,&trect);
		}
	}

	/// Selects an HFONT object into device context. 
	/// The new object replaces the previous object of the same type
	void SetFont(HFONT hfnt)
	{
		m_oldFnt = ::SelectObject(m_hDC, hfnt);
	}

	/// Sets the text color for the device context to the specified color
	void SetTextColor(COLORREF clr)
	{
		::SetTextColor(m_hDC, clr);
	}

	/// Sets the text color for the device context to the specified color
	void SetTextColor(const iColor &clr)
	{
		::SetTextColor(m_hDC, RGB(clr.r,clr.g,clr.b));
	}

	/// Sets the current background color for the device context
	void SetTextBkColor(COLORREF clr)
	{
		::SetBkColor(m_hDC, clr);
	}

	/// Sets the background mix mode of the specified device context. 
	/// The background mix mode is used with text, hatched brushes, and with non-solid pen styles.
	void SetTextBkMode(sint32 bkmode)
	{
		::SetBkMode(m_hDC, bkmode);
	}

	/// Sets the text-alignment flags for the device context
	void SetTextAlignment(uint32 algn)
	{
		::SetTextAlign(m_hDC, algn);
	}

	/// Computes the width and height of the specified string of text for the given Device context
	static iSize GetTextSize(HDC hdc, const iStringT &text)
	{
		assert(hdc != NULL);
		SIZE siz;
		::GetTextExtentPoint(hdc,text.CStr(),text.Length(),&siz);

		return iSize(siz);
	}

	/// Computes the width and height of the specified string of text
	iSize GetTextSize(const iStringT &text)
	{
		assert(m_hDC != NULL);
		SIZE siz;
		::GetTextExtentPoint(m_hDC,text.CStr(),text.Length(),&siz);

		return iSize(siz);
	}

	/// Draws a character string by using the currently selected font. 
	/// An optional rectangle may be provided, to be used for clipping, opaquing, or both
	void DrawTextRect(const iStringT &text, const iRect &rect, bool tdots = false, uint32 uFormat = DT_LEFT)
	{
		iStringT tout;
		if (tdots) tout = CropString(text,rect.w,0);
		else tout = text;
		if (tout.Empty()) return;

		if (!tout.Empty()) ::DrawText(m_hDC,tout.CStr(),tout.Length(),&RECT(rect),uFormat);
	}

	/// Draws a character string by using the currently selected font. 
	/// An optional rectangle may be provided, to be used for clipping, opaquing, or both
	void DrawTextRectPos(const iStringT &text, const iPoint &pos, const iRect &rect, bool tdots = false, bool vcenter=false)
	{
		iStringT tout;
		if (tdots) tout = CropString(text,rect.x2() - pos.x,0);
		else tout = text;
		if (tout.Empty()) return;

		int oy;
		if (vcenter){
			SIZE siz;
			::GetTextExtentPoint(m_hDC,tout.CStr(),tout.Length(),&siz);
			oy = rect.y + (int)(rect.h/2) - siz.cy/2;
		}
		else  oy = pos.y;
		::ExtTextOut(m_hDC,pos.x,oy,ETO_CLIPPED,&RECT(rect),tout.CStr(),tout.Length(),0);
	}

	/// Crops string according to bounding rect
	iStringT CropString(const iStringT &str, uint32 nColumnLen, uint32 nOffset)
	{
		static const iStringT dots(_T("..."));
		uint32 nStringLen = str.Length();

		SIZE siz;
		::GetTextExtentPoint(m_hDC,str.CStr(),str.Length(),&siz);

		if(str.Empty() ||	siz.cx + nOffset <= nColumnLen) {
			return str;
		}

		iStringT result(str);
		::GetTextExtentPoint(m_hDC,dots.CStr(),dots.Length(),&siz);
		int nAddLen = siz.cx;

		for(int i = nStringLen-1; i > 0; i--) {
			::GetTextExtentPoint(m_hDC,result.Left(i).CStr(),result.Left(i).Length(),&siz);
			if((siz.cx + nOffset + nAddLen) <= nColumnLen) {
				break;
			}
		}

		result = str.Left(i);

		if (result.Empty()) return result;
		else result += dots;
		return result;
	}

	HDC			m_hDC;
	iDib		m_Dib;

	HGDIOBJ		m_oldBitmap;
	HGDIOBJ		m_oldFnt;
};

#endif //_IOLIB_XENON_MEMORY_DC_WRAPER_H_
/** @} */ // iolib_xe
