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
 *	@brief Tooltips handling implementation
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_TOOLTIP_DISPATCHERS_IMPLEMENTATION_H_
#define _IOLIB_SILICON_TOOLTIP_DISPATCHERS_IMPLEMENTATION_H_

#include "si.std.h"

/// Common ToolTip Dispatcher interface
class iToolTipDispatcherI
{
public:
	/// Dispatches ToolTip notifycation
	virtual void DispToolTip(const iStringT& text, const iDib* pIcon = NULL) = NULL;
};

/// Windows tool bar based ToolTip Dispatcher
class iToolBarTTDisp : public iToolTipDispatcherI
{
public:
	/// Constructor
	iToolBarTTDisp()
	: m_hToolBar(NULL)
	{}

	/// Initializes ToolTip dispatcher
	void InitToolBarTTDisp(HWND toolbar)
	{
		m_hToolBar= toolbar;
	}

	/// Dispatches ToolTip notifycation
	virtual void DispToolTip(const iStringT& text, const iDib* pIcon = NULL)
	{
		if (m_hToolBar && m_Text.Compare(text)!=0) {
			::SendMessage(m_hToolBar, SB_SETTEXT, 0, (LPARAM)text.CStr());
			m_Text = text;
		}
	}

private:
	iStringT	m_Text;
	HWND		m_hToolBar;
};

/// NonWindowed control based ToolTip Dispatcher
class iNWCtrlTTDisp : public iNWControl, public iToolTipDispatcherI
{
public:
	/// Constructor
	iNWCtrlTTDisp()
	: m_pIcon(NULL)
	{}

	/// Initializes ToolTip dispatcher
	bool CreateNWCtrlTTDisp(iNWElementContainerI *parent, const iRect &rect, const iDib& bdib)
	{
		m_dibBack = bdib;
		return CreateNWElement(parent,rect);
	}

	/// Dispatches ToolTip notifycation
	virtual void DispToolTip(const iStringT& text, const iDib* pIcon = NULL)
	{
		if (m_Text.Compare(text)!=0 || m_pIcon != pIcon) {
			m_pIcon = pIcon;
			m_Text = text;
			ForceRepaint();
			//::SendMessage(m_hToolBar, SB_SETTEXT, 0, (LPARAM)text.CStr());
			//m_Text = text;
		}
	}

	/// Draws element
	virtual void DrawElement(iMemoryDC &ddc) const
	{
		iRect elrc = GetElementRect();

		// Fill background
		iSize belsiz(m_dibBack.GetWidth()/3, m_dibBack.GetHeight());
		m_dibBack.CopyRectToDibXY(&ddc.m_Dib,iRect(0,0,belsiz.w,belsiz.h),iPoint(elrc.x,elrc.y),BLEND_ALPHABLEND);
		m_dibBack.TileRectToDibRect(iRect(belsiz.w,0,belsiz.w,belsiz.h),ddc.m_Dib,iRect(elrc.x+belsiz.w,elrc.y,elrc.w-belsiz.w*2,belsiz.h),BLEND_ALPHABLEND);
		m_dibBack.CopyRectToDibXY(&ddc.m_Dib,iRect(belsiz.w*2,0,belsiz.w,belsiz.h),iPoint(elrc.x+elrc.w-belsiz.w,elrc.y),BLEND_ALPHABLEND);
		//iDibPrimitives::DrawFrame(ddc.m_Dib,elrc,m_clrFrame,BLEND_ALPHABLEND);
		//elrc.DeflateRect(1);
		//ddc.m_Dib.FillRect(elrc,m_clrBack,BLEND_ALPHABLEND);

		uint32 opos = elrc.x + 5;

		// Draw icon (if avail.)
		if (m_pIcon){
			opos += (16+5);
		}

		// Draw text (if avail.)
		if (!m_Text.Empty()){
			ddc.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
			ddc.SetTextBkMode(TRANSPARENT);
			iSize tsiz = ddc.GetTextSize(m_Text);
			iPoint bpos(opos, elrc.y + (elrc.h/2)-(tsiz.h/2));
			ddc.DrawTextRectPos(m_Text,bpos,elrc);
		}
	}

private:
	iColor		m_clrFrame;
	iColor		m_clrBack;
	iStringT	m_Text;
	iDib		m_dibBack;
	const iDib*	m_pIcon;
};


#endif //_IOLIB_SILICON_TOOLTIP_DISPATCHERS_IMPLEMENTATION_H_
/** @} */ // iolib_si
