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
 *	@brief iDib based Cursor class implementation
 */
/** @addtogroup iolib_xe
 *  @{
 */
#ifndef _IOLIB_XENON_CURSOR_H_
#define _IOLIB_XENON_CURSOR_H_

/// iDib based Cursor class
class iDibCursor
{
public:

	/// Default constructor
	iDibCursor()
	: m_hCursor(NULL)
	{}

	/// Constructs iDibCursor object with specified iDib object and anchor point
	iDibCursor(const iDib& dib, const iPoint &anchor = iPoint(0,0))
	: m_DC(dib)
	, m_Anchor(anchor)
	{ InitCursor(dib,anchor); }

	/// Initialize iDibCursor object with specified iDib object and anchor point
	void InitCursor(const iDib& dib, const iPoint &anchor = iPoint(0,0))
	{
		m_DC.SetDib(dib);
		m_Anchor = anchor;

		// Create an empty mask bitmap.
		HBITMAP hMonoBitmap = CreateBitmap(m_DC.m_Dib.GetWidth(),m_DC.m_Dib.GetHeight(),1,1,NULL);

		ICONINFO ii;
		ii.fIcon = FALSE;  // Change fIcon to TRUE to create an alpha icon
		ii.xHotspot = 0;
		ii.yHotspot = 0;
		ii.hbmMask = hMonoBitmap;
		ii.hbmColor = m_DC.m_Dib.GetBitmap();

		// Create the alpha cursor with the alpha DIB section.
		m_hCursor = CreateIconIndirect(&ii);

		//DeleteObject(hBitmap);          
		DeleteObject(hMonoBitmap); 
	}

	/// Returns handle to cursor
	HCURSOR GetCursor() const
	{
		check(m_hCursor);
		return m_hCursor;
	}

	/// Returns cursor's dib
	const iDib& GetCursorDib() const
	{
		return m_DC.m_Dib;
	}

	/// Returns cursor's anchor point
	iPoint GetAnchor() const
	{
		return m_Anchor;
	}

	/// Returns cursor's metrics
	iSize GetSize() const
	{
		return m_DC.m_Dib.GetSize();
	}

private:
	HCURSOR		m_hCursor;
	iMemoryDC	m_DC;
	iPoint		m_Anchor;
};

#endif //_IOLIB_XENON_CURSOR_H_
/** @} */ // iolib_xe
