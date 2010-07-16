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
 *	@brief HFONT wrapper class implementation
 */
/** @addtogroup iolib_xe
 *  @{
 */
#ifndef _IOLIB_XENON_FONT_H_
#define _IOLIB_XENON_FONT_H_

#undef DeleteFont

/// LOGFONT structure manipulations wrapper class
class iLogFont : public LOGFONT
{
public:

	/// Default constructor
	iLogFont() 
	{ 
	  ::ZeroMemory( (LOGFONT*) this, sizeof(LOGFONT) );
	}

	/// Constructs iLogFont object with LOGFONT
	iLogFont(const LOGFONT& lf) 
	{ 
	  Copy(&lf);
	}

	/// Constructs iLogFont object with HFONT
	iLogFont(HFONT hFont)
	{
	  check(::GetObjectType(hFont)==OBJ_FONT);
	  ::GetObject(hFont, sizeof(LOGFONT), (LOGFONT*) this);
	}

	/// Creates a logical font that has the specified characteristics. 
	/// The font can subsequently be selected as the current font for any device context. 
	HFONT CreateFontIndirect() 
	{ 
	  return ::CreateFontIndirect(this); 
	}

	/// Sets Bold flag
	void SetBold() 
	{ 
	  lfWeight = FW_BOLD; 
	}

	/// Checks Bold flag
	bool IsBold() const 
	{ 
	  return lfWeight>=FW_BOLD; 
	}

	/// Set height value
	void SetHeight(sint32 PointSize, const iDC &dc) 
	{ 
	  // For MM_TEXT mapping mode...
	  // MulDiv() gives correct rounding.
	  lfHeight = -MulDiv(PointSize, ::GetDeviceCaps(dc, LOGPIXELSY), 72); 
	}

	/// Gets height value
	sint32 GetHeight(const iDC &dc) const
	{
	  // For MM_TEXT mapping mode...
	  // MulDiv() gives correct rounding.
	  return ::MulDiv(-lfHeight, 72, ::GetDeviceCaps(dc, LOGPIXELSY));
	}

	/// Gets deci point height value
	sint32 GetDeciPointHeight(const iDC &dc)
	{
	  iPoint ptOrg(0, 0);
	  ::DPtoLP(dc, &POINT(ptOrg), 1);
	  iPoint pt(0, 0);
	  pt.y = abs(lfHeight) + ptOrg.y;
	  ::LPtoDP(dc,&POINT(pt),1);
	  return MulDiv(pt.y, 720, ::GetDeviceCaps(dc,LOGPIXELSY)); // 72 points/inch, 10 decipoints/point
	}

	/// Sets deci point height value
	void SetHeightFromDeciPoint(sint32 DeciPtHeight, const iDC &dc)
	{
	  iPoint pt;
	  pt.y = MulDiv(::GetDeviceCaps(dc, LOGPIXELSY), DeciPtHeight, 720); // 72 points/inch, 10 decipoints/point
	  ::DPtoLP(dc, &POINT(pt), 1);
	  iPoint ptOrg(0, 0);
	  ::DPtoLP(dc, &POINT(ptOrg), 1);
	  lfHeight = -abs(pt.y - ptOrg.y);
	}

	/// Sets default Windows font, used in windows caption bar
	void SetCaptionFont()
	{
	  NONCLIENTMETRICS ncm = { 0 };
	  ncm.cbSize = sizeof(ncm);
	  ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
	  Copy(&ncm.lfCaptionFont);
	}

	/// Sets default Windows font, used in windows menu
	void SetMenuFont()
	{
	  NONCLIENTMETRICS ncm = { 0 };
	  ncm.cbSize = sizeof(ncm);
	  ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
	  Copy(&ncm.lfMenuFont);
	}

	/// Sets default Windows font, used in windows status bar
	void SetStatusFont()
	{
	  NONCLIENTMETRICS ncm = { 0 };
	  ncm.cbSize = sizeof(ncm);
	  ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
	  Copy(&ncm.lfStatusFont);
	}

	/// Sets default Windows font, used in windows message box
	void SetMessageBoxFont()
	{
	  NONCLIENTMETRICS ncm = { 0 };
	  ncm.cbSize = sizeof(ncm);
	  ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
	  Copy(&ncm.lfMessageFont);
	}

	/// Copies content of given LOGFONT to itself
	void Copy(const LOGFONT* lf)
	{
	  check(lf);
	  ::CopyMemory( (LOGFONT*) this, lf, sizeof(LOGFONT) );
	}

	/// Assignment operator
	iLogFont& operator=(const iLogFont& src)
	{
	  Copy(&src);
	  return *this;
	}

	/// Assignment operator
	iLogFont& operator=(const LOGFONT& src)
	{
	  Copy(&src);
	  return *this;
	}

	/// Assignment operator
	iLogFont& operator=(HFONT hFont)
	{
	  check(::GetObjectType(hFont)==OBJ_FONT);
	  ::GetObject(hFont, sizeof(LOGFONT), (LOGFONT*) this);
	  return *this;
	}

	/// Equality operator
	bool operator==(const LOGFONT& logfont) const
	{
	  return( logfont.lfHeight == lfHeight &&
		 logfont.lfWidth == lfWidth &&
		 logfont.lfEscapement == lfEscapement &&
		 logfont.lfOrientation == lfOrientation &&
		 logfont.lfWeight == lfWeight &&
		 logfont.lfItalic == lfItalic &&
		 logfont.lfUnderline == lfUnderline &&
		 logfont.lfStrikeOut == lfStrikeOut &&
		 logfont.lfCharSet == lfCharSet &&
		 logfont.lfOutPrecision == lfOutPrecision &&
		 logfont.lfClipPrecision == lfClipPrecision &&
		 logfont.lfQuality == lfQuality &&
		 logfont.lfPitchAndFamily == lfPitchAndFamily &&
		 ::lstrcmp(logfont.lfFaceName, lfFaceName) == 0 );
	}

};

/// The iFont class encapsulates a Windows graphics device interface (GDI) font and provides member functions for manipulating the font.
class iFont
{
public:
	/// Default constructor
	iFont()
	: m_Font(NULL)
	, m_Managed(false)
	{
	}

	/// Construct iFont object with given font handle
	iFont(HFONT fnt, bool mng = false)
	: m_Font(fnt)
	, m_Managed(mng)
	{
	}

	/// Construct iFont object with given font descriptor
	iFont(iLogFont lfnt)
	: m_Managed(true)
	{
		m_Font = lfnt.CreateFontIndirect();
	}

	/// Destructor
	~iFont()
	{
		if (m_Managed && m_Font != NULL) DeleteFont();
	}

	/// Deletes font
	void DeleteFont()
	{
		::DeleteObject(m_Font);
	}

	/// Creates font with given parameters
	HFONT CreateFont(sint32 nHeight, sint32 nWidth, sint32 nEscapement,
			sint32 nOrientation, sint32 nWeight, uint8 bItalic, uint8 bUnderline,
			uint8 cStrikeOut, uint8 nCharSet, uint8 nOutPrecision,
			uint8 nClipPrecision, uint8 nQuality, uint8 nPitchAndFamily,
			const iStringT &sFacename)
	{
		check(m_Font == NULL);
		m_Font = ::CreateFont(nHeight, nWidth, nEscapement,
			nOrientation, nWeight, bItalic, bUnderline, cStrikeOut,
			nCharSet, nOutPrecision, nClipPrecision, nQuality,
			nPitchAndFamily, sFacename.CStr());
		m_Managed = true;
		return m_Font;
	}

	/// Attaches given font handle to the object
	void Attach(HFONT hfnt)
	{
		if (m_Font) DeleteFont();
		m_Font = hfnt;
	}

	/// Dettaches current font handle
	HFONT Detach()
	{
		HFONT hfnt = m_Font;
		m_Font = NULL;
		return hfnt;
	}

	/// Returns current font handle
	operator HFONT() const
	{
		return m_Font;
	}

private:

	HFONT	m_Font;
	bool	m_Managed;
};

#endif //_IOLIB_XENON_FONT_H_
/** @} */ // iolib_xe
