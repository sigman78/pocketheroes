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
 *	@brief Static iDib image control class implementation
 */
/** @addtogroup iolib_li
 *  @{
 */
#ifndef _IOLIB_LITHIUM_CONTROL_STATIC_IMAGE_H_
#define _IOLIB_LITHIUM_CONTROL_STATIC_IMAGE_H_

/// Static iDib image control class
class iCtrlStaticDib : public CWindowImpl<iCtrlStaticDib, CStatic>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CStatic::GetWndClassName())
	
	BEGIN_MSG_MAP(iCtrlStaticDib)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
	{
		PAINTSTRUCT ps;
		iDC dc = BeginPaint(&ps);

		iDibDC::DibToDCXY(m_Dib,dc,iPoint(0,0));

		EndPaint(&ps);
		bHandler=TRUE;
		return 0;
	}

	/// Sets new image 
	void SetDib(const iStringT &fname)
	{
		iDib dib;
		iDibLoader::Load(dib,fname);
		SetDib(dib);
	}

	/// Sets new image 
	void SetDib(const iDib &dib)
	{
		RECT rc;
		GetClientRect(&rc);
		iSize clsize(rc.right-rc.left,rc.bottom-rc.top);
		iColor clr(COLORREF2ICOLOR(GetSysColor(COLOR_BTNFACE)));
		m_Dib.Create(dib.GetSize());
		m_Dib.Fill(clr,BLEND_SRCCOPY);
		dib.CopyToDib(&m_Dib,BLEND_ALPHABLEND);
		m_Dib.ResizeCanvas(clsize,Align_Center,clr);
		//iDibPrimitives::DrawFrame(m_Dib,m_Dib.GetSize(),cColorBlack,BLEND_SRCCOPY);
		Invalidate();
	}

private:
	iDib	m_Dib;
};


#endif //_IOLIB_LITHIUM_CONTROL_STATIC_IMAGE_H_
/** @} */ // iolib_li
