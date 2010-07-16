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
 *	@brief Windows Device Context wrapper class implementation
 */
/** @addtogroup iolib_xe
 *  @{
 */
#ifndef _IOLIB_XENON_DEVICE_CONTEXT_H_
#define _IOLIB_XENON_DEVICE_CONTEXT_H_

#undef SelectBitmap
#undef SelectFont

/// Windows Device Context wrapper class
class iDC
{
public:

	/// Constructs iDC object with specified handle
	iDC(HDC hdc = NULL, bool bman = true)
	: m_hDC(hdc)
	, m_Managed(bman)
	{ }

	/// Destructor
	~iDC()
	{
		if (m_hDC != NULL && m_Managed) ::DeleteDC(m_hDC);
	}

	/// Asignment operator
	iDC& operator = (HDC hdc)
	{
		m_hDC = hdc;
		return *this;
	}

	/// Attaches specified handle
	void Attach(HDC hdc, bool bman = true)
	{
		if (m_hDC != NULL) ::DeleteDC(m_hDC);
		m_hDC = hdc;
	}

	/// Dettaches specified handle
	HDC Detach()
	{
		HDC hdc = m_hDC;
		m_hDC = NULL;
		return hdc;
	}

	/// Returns context handle
	operator HDC() const
	{
		return m_hDC;
	}

	/// Validate device context handle
	bool IsNull() const
	{
		return (m_hDC == NULL);
	}

	/// Creates a device context (DC) for a device with specified parameters
	HDC CreateDC(const iStringT &sDriverName, const iStringT &sDeviceName, const iStringT sOutput, const DEVMODE* lpInitData)
	{
		assert(m_hDC == NULL);
		m_hDC = ::CreateDC(sDriverName.CStr(), sDeviceName.CStr(), sOutput.CStr(), lpInitData);
		return m_hDC;
	}

	/// Creates a memory device context (DC) compatible with the specified device
	HDC CreateCompatibleDC(HDC hDC = NULL)
	{
		assert(m_hDC == NULL);
		m_hDC = ::CreateCompatibleDC(hDC);
		return m_hDC;
	}

	/// Deletes device context handle
	bool DeleteDC()
	{
		if(m_hDC == NULL) return false;
		bool bRet = ::DeleteDC(m_hDC)?true:false;
		if(bRet) m_hDC = NULL;
		return bRet;
	}

	/// Selects an HBITMAP object into device context. 
	/// The new object replaces the previous object of the same type
	HBITMAP SelectBitmap(HBITMAP hbmp)
	{
		return (HBITMAP)::SelectObject(m_hDC,hbmp);
	}

	/// Selects an HFONT object into device context. 
	/// The new object replaces the previous object of the same type
	HFONT SelectFont(HFONT hfnt)
	{
		return (HFONT)::SelectObject(m_hDC,hfnt);
	}

	/// Returns a handle to the window associated with device context (DC). 
	/// Output functions that use the specified device context draw into this window.
	HWND GetOwnerWindow()
	{
		check(m_hDC != NULL);
		return ::WindowFromDC(m_hDC);
	}

	/// Returns the current background color for the device context
	iColor GetBkColor() const
	{
		assert(m_hDC != NULL);
		return iColor(::GetBkColor(m_hDC));
	}

	/// Sets the current background color for the device context
	void SetBkColor(const iColor &crColor)
	{
		assert(m_hDC != NULL);
		::SetBkColor(m_hDC, crColor);
	}

	/// Returns the current background mix mode for a specified device context. 
	/// The background mix mode of a device context affects text, hatched brushes, and pen styles that are not solid lines.
	sint32 GetBkMode() const
	{
		assert(m_hDC != NULL);
		return ::GetBkMode(m_hDC);
	}

	/// Sets the background mix mode of the specified device context. 
	/// The background mix mode is used with text, hatched brushes, and with non-solid pen styles.
	void SetBkMode(sint32 nBkMode)
	{
		assert(m_hDC != NULL);
		::SetBkMode(m_hDC, nBkMode);
	}

	/// Retrieves the current polygon fill mode
	sint32 GetPolyFillMode() const
	{
		assert(m_hDC != NULL);
		return ::GetPolyFillMode(m_hDC);
	}

	/// Sets the polygon fill mode for functions that fill polygons
	void SetPolyFillMode(sint32 nPolyFillMode)
	{
		assert(m_hDC != NULL);
		::SetPolyFillMode(m_hDC, nPolyFillMode);
	}

	/// Retrieves the text-alignment setting for the device context
	uint32 GetTextAlign()
	{
		assert(m_hDC != NULL);
		return ::GetTextAlign(m_hDC);
	}

	/// Sets the text-alignment flags for the device context
	void SetTextAlign(iAlignment nalign)
	{
		assert(m_hDC != NULL);
		::SetTextAlign(m_hDC,nalign);
	}

	/// Retrieves the current text color for the device context
	iColor GetTextColor() const
	{
		assert(m_hDC != NULL);
		return iColor(::GetTextColor(m_hDC));
	}

	/// Sets the text color for the device context to the specified color
	void SetTextColor(const iColor &crColor)
	{
		assert(m_hDC != NULL);
		::SetTextColor(m_hDC, crColor);
	}

	/// Writes a character string at the specified location, using the currently selected font, background color, and text color
	void TextOut(const iPoint &pos, const iStringT &text)
	{
		assert(m_hDC != NULL);
		::TextOut(m_hDC, pos.x, pos.y, text.CStr(), text.Length());
	}

	/// Draws a character string by using the currently selected font. 
	/// An optional rectangle may be provided, to be used for clipping, opaquing, or both
	void TextOutRect(const iPoint &pos, const iRect &rect, const iStringT &text)
	{
		assert(m_hDC != NULL);
		::ExtTextOut(m_hDC, pos.x, pos.y, ETO_CLIPPED, &RECT(rect), text.CStr(), text.Length() ,NULL);
	}

	/// Computes the width and height of the specified string of text
	iSize GetTextSize(const iStringT &text)
	{
		assert(m_hDC != NULL);
		SIZE siz;
		::GetTextExtentPoint(m_hDC,text.CStr(),text.Length(),&siz);

		return iSize(siz);
	}

	/// Fits string of text to specified bounds
	iStringT MakeShortString(const iStringT &str, uint32 nColumnLen, uint32 nOffset)
	{
		static const iStringT dots(_T("..."));
		uint32 nStringLen = str.Length();
		SIZE siz;
		::GetTextExtentPoint(m_hDC,str.CStr(),str.Length(),&siz);
		if(str.Empty() ||	siz.cx + nOffset <= nColumnLen){
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
		result += dots;
		return result;
	}
	

	/// Fills a rectangle using the specified brush. 
	/// This function fills the rectangle’s left and top borders, but excludes the right and bottom borders
	void FillRect(const iRect &rc)
	{
		assert(m_hDC != NULL);
		HBRUSH br = CreateSolidBrush(RGB(255,255,255));
		::FillRect(m_hDC,&RECT(rc),br);
		::DeleteObject(br);
	}

private:
	HDC	m_hDC;
	bool m_Managed;
};

#endif //_IOLIB_XENON_DEVICE_CONTEXT_H_
/** @} */ // iolib_xe
