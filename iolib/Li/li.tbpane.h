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
 *	@brief Toolbar pane container class implementation
 */
/** @addtogroup iolib_li
 *  @{
 */
#ifndef _IOLIB_LITHIUM_WTL_PANE_CONTAINER_WITH_TOOLBAR_H_
#define _IOLIB_LITHIUM_WTL_PANE_CONTAINER_WITH_TOOLBAR_H_

#include "li.wtl.hlp.h"

/// Toolbar pane container class template
template <class T, class TBase = CWindow, class TWinTraits = CControlWinTraits>
class ATL_NO_VTABLE iTBPaneContainerImpl : public CWindowImpl< T, TBase, TWinTraits >, public CCustomDraw< T >
{
public:
	DECLARE_WND_CLASS_EX(NULL, 0, -1)

	// Data members
	CWindow		m_wndClient;
	HWND		m_hReBar;
	HWND		m_hToolBar;
	UINT		m_ToolBarId;
	HIMAGELIST	m_ToolBarBmp;

	/// Constructor
	iTBPaneContainerImpl()
	{}

	/// Returns handle to the client window
	HWND GetClient() const
	{
		return m_wndClient;
	}

	/// Sets new client window
	HWND SetClient(HWND hWndClient)
	{
		HWND hWndOldClient = m_wndClient;
		m_wndClient = hWndClient;
		if(m_hWnd != NULL)
		{
			T* pT = static_cast<T*>(this);
			pT->UpdateLayout();
		}
		return hWndOldClient;
	}

	/// Creates tool bar pane with specified params
	HWND CreateTBPane(HWND hWndParent, UINT tb_resid, HIMAGELIST tb_bmp)
	{
		m_ToolBarId = tb_resid;
		m_ToolBarBmp = tb_bmp;
		return CWindowImpl< T, TBase, TWinTraits >::Create(hWndParent, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, NULL);
	}

	/// Updates layout
	void UpdateLayout()
	{
		RECT rcClient;
		GetClientRect(&rcClient);
		T* pT = static_cast<T*>(this);
		pT->UpdateLayout(rcClient.right, rcClient.bottom);
	}

// Message map and handlers
	BEGIN_MSG_MAP(CPaneContainerImpl)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		FORWARD_NOTIFICATIONS()
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		m_hToolBar = WTLHLP::CreateSimpleToolBarCtrl(m_hWnd, m_ToolBarId, false, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
		ATLASSERT( m_hToolBar && ::IsWindow(m_hToolBar) );
		//CToolBarCtrl(m_hToolBar).SetButtonSize(CSize(32,32));

		//
		//
		::SendMessage( m_hToolBar, TB_SETIMAGELIST, 0, (LPARAM)m_ToolBarBmp );

		// create rebar
		m_hReBar = WTLHLP::CreateSimpleReBarCtrl(m_hWnd, ATL_SIMPLE_REBAR_NOBORDER_STYLE);
		ATLASSERT( m_hReBar && ::IsWindow(m_hReBar) );
		WTLHLP::AddSimpleReBarBandCtrl(m_hReBar, m_hToolBar);
	
		return 0;
	}

	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
	{
		T* pT = static_cast<T*>(this);
		pT->UpdateLayout(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}

	LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if(m_wndClient.m_hWnd != NULL)
			m_wndClient.SetFocus();
		return 0;
	}

	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 1;	// no background needed
	}

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CPaintDC dc(m_hWnd);

		T* pT = static_cast<T*>(this);
		pT->DrawPaneTitle(dc.m_hDC);

		if(m_wndClient.m_hWnd == NULL)	// no client window
			pT->DrawPane(dc.m_hDC);

		return 0;
	}

// Custom draw overrides
	DWORD OnPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/)
	{
		return CDRF_NOTIFYITEMDRAW;	// we need per-item notifications
	}

	DWORD OnItemPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW lpNMCustomDraw)
	{
		CDCHandle dc = lpNMCustomDraw->hdc;
#if (_WIN32_IE >= 0x0400)
		RECT& rc = lpNMCustomDraw->rc;
#else //!(_WIN32_IE >= 0x0400)
		RECT rc;
		m_tb.GetItemRect(0, &rc);
#endif //!(_WIN32_IE >= 0x0400)

		dc.FillRect(&rc, COLOR_3DFACE);

		return CDRF_NOTIFYPOSTPAINT;
	}

	DWORD OnItemPostPaint(int /*idCtrl*/, LPNMCUSTOMDRAW lpNMCustomDraw)
	{
		CDCHandle dc = lpNMCustomDraw->hdc;
#if (_WIN32_IE >= 0x0400)
		RECT& rc = lpNMCustomDraw->rc;
#else //!(_WIN32_IE >= 0x0400)
		RECT rc;
		m_tb.GetItemRect(0, &rc);
#endif //!(_WIN32_IE >= 0x0400)

		RECT rcImage = { m_xBtnImageLeft, m_yBtnImageTop, m_xBtnImageRight + 1, m_yBtnImageBottom + 1 };
		::OffsetRect(&rcImage, rc.left, rc.top);
		T* pT = static_cast<T*>(this);

		if((lpNMCustomDraw->uItemState & CDIS_DISABLED) != 0)
		{
			RECT rcShadow = rcImage;
			::OffsetRect(&rcShadow, 1, 1);
			CPen pen1;
			pen1.CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_3DHILIGHT));
			pT->DrawButtonImage(dc, rcShadow, pen1);
			CPen pen2;
			pen2.CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_3DSHADOW));
			pT->DrawButtonImage(dc, rcImage, pen2);
		}
		else
		{
			if((lpNMCustomDraw->uItemState & CDIS_SELECTED) != 0)
				::OffsetRect(&rcImage, 1, 1);
			CPen pen;
			pen.CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_BTNTEXT));
			pT->DrawButtonImage(dc, rcImage, pen);
		}

		return CDRF_DODEFAULT;	// continue with the default item painting
	}

// Implementation - overrideable methods
	void UpdateLayout(int cxWidth, int cyHeight)
	{
		ATLASSERT(::IsWindow(m_hWnd));

		// resize toolbar
		::SendMessage(m_hReBar, WM_SIZE, 0, MAKELONG(cxWidth,cyHeight));
		//::SetWindowPos(m_hReBar, NULL, 2,2,cxWidth-4, cyHeight-4,SWP_NOZORDER);


		// decrese client height by toolbar height
		if (m_wndClient){
			RECT tbar_rect;
			::GetWindowRect(m_hReBar, &tbar_rect);
			m_wndClient.SetWindowPos(NULL, 2, tbar_rect.bottom - tbar_rect.top + 2,cxWidth - 4 ,cyHeight - 2 - (tbar_rect.bottom - tbar_rect.top), SWP_NOZORDER);
		}

		/*
		if(IsVertical())
		{
			::SetRect(&rect, 0, 0, m_cxyHeader, cyHeight);
			if(m_tb.m_hWnd != NULL)
				m_tb.SetWindowPos(NULL, m_cxyBorder, m_cxyBorder + m_cxyBtnOffset, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

			if(m_wndClient.m_hWnd != NULL)
				m_wndClient.SetWindowPos(NULL, m_cxyHeader, 0, cxWidth - m_cxyHeader, cyHeight, SWP_NOZORDER);
			else
				rect.right = cxWidth;
		}
		else
		{
			::SetRect(&rect, 0, 0, cxWidth, m_cxyHeader);
			if(m_tb.m_hWnd != NULL)
				m_tb.SetWindowPos(NULL, rect.right - m_cxToolBar, m_cxyBorder + m_cxyBtnOffset, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

			if(m_wndClient.m_hWnd != NULL)
				m_wndClient.SetWindowPos(NULL, 0, m_cxyHeader, cxWidth, cyHeight - m_cxyHeader, SWP_NOZORDER);
			else
				rect.bottom = cyHeight;
		}
		InvalidateRect(&rect);
		*/
	}


	/// Draws pane title
	void DrawPaneTitle(CDCHandle dc)
	{
		RECT cl_rect, tb_rect;
		::GetClientRect(m_hWnd,&cl_rect);
		::GetClientRect(m_hReBar,&tb_rect);

		cl_rect.top = tb_rect.bottom;
		
		dc.DrawEdge(&cl_rect, EDGE_ETCHED, BF_LEFT | BF_TOP | BF_RIGHT | BDR_RAISEDINNER | BDR_SUNKENOUTER);
		//rect.bottom = rect.top + m_cxyHeader;


		/*
		if(IsVertical())
		{
			rect.right = rect.left + m_cxyHeader;
			dc.DrawEdge(&rect, EDGE_ETCHED, BF_LEFT | BF_TOP | BF_BOTTOM | BF_ADJUST);
			dc.FillRect(&rect, COLOR_3DFACE);
		}
		else
		{
			rect.bottom = rect.top + m_cxyHeader;
			dc.DrawEdge(&rect, EDGE_ETCHED, BF_LEFT | BF_TOP | BF_RIGHT | BF_ADJUST);
			dc.FillRect(&rect, COLOR_3DFACE);
			// draw title only for horizontal pane container
			dc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
			dc.SetBkMode(TRANSPARENT);
			T* pT = static_cast<T*>(this);
			HFONT hFontOld = dc.SelectFont(pT->GetTitleFont());
			rect.left += m_cxyTextOffset;
			rect.right -= m_cxyTextOffset;
			if(m_tb.m_hWnd != NULL)
				rect.right -= m_cxToolBar;;
			dc.DrawText(m_szTitle, -1, &rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
			dc.SelectFont(hFontOld);
		}
		*/
	}


	/// Draws pane background (called only if pane is empty)
	void DrawPane(CDCHandle dc)
	{
		RECT rect;
		GetClientRect(&rect);
		/*
		if(IsVertical())
			rect.left += m_cxyHeader;
		else
			rect.top += m_cxyHeader;
		*/
		if((GetExStyle() & WS_EX_CLIENTEDGE) == 0)
			dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT | BF_ADJUST);
		dc.FillRect(&rect, COLOR_APPWORKSPACE);
	}
};

/// Toolbar pane container class
class iTBPaneContainer : public iTBPaneContainerImpl<iTBPaneContainer>
{
public:
	DECLARE_WND_CLASS_EX(_T("ToolBarPaneContainer"), 0, -1)
};



#endif //_IOLIB_LITHIUM_WTL_PANE_CONTAINER_WITH_TOOLBAR_H_
/** @} */ // iolib_li
