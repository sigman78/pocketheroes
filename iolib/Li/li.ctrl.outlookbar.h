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
 *	@brief Outlook style side bar control class implementation
 */
/** @addtogroup iolib_li
 *  @{
 */
#ifndef _IOLIB_LITHIUM_CONTROL_OUTLOOK_BAR_H_
#define _IOLIB_LITHIUM_CONTROL_OUTLOOK_BAR_H_

const sint32 ITEM_SPACE = 10;

/// Outlook style side bar control 
class iCtrlOutlookBar : public CWindowImpl<iCtrlOutlookBar, CListBox >
{
public:
	DECLARE_WND_SUPERCLASS(_T("OUTLOOKBAR"),_T("LISTBOX"))

	/// Side bar item descriptor structure
	struct Item {
		Item(const iStringT& title, uint32 id)
		: m_Title(title)
		, m_Id(id)
		{}

		iStringT	m_Title;
		uint32		m_Id;
	};

	/// Item flags enumeration
	enum ItemFlags {
		Selected = 0x01,
		Focused  = 0x02
	};

	/// Message code enumeration
	enum MsgCode {
		StateChanged = 0
	};

	/// Message handler interface
	interface MsgHandler {
		/// Notifies subscriber about all state changes
		virtual void iCtrlOutlookBar_Notify(MsgCode mc, uint32 param1) = NULL;
		/// Notifies subscriber before item state changing
		virtual bool iCtrlOutlookBar_ItemChanging(uint32 curId, uint32 newId) = NULL;
		/// Override this methot to customize control visualization
		virtual void iCtrlOutlookBar_ComposeItem(iMemoryDC &memDC, const iRect& itemRect, const Item& item, uint32 flags) = NULL;
	};

	BEGIN_MSG_MAP(iCtrlOutlookBar)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	END_MSG_MAP()


	/// Adds new item into list
	void AddItem(const iStringT& title, uint32 id)
	{
		m_Items.Add(Item(title,id));
	}

	/// Removes all items
	void RemoveAllItems()
	{
		m_Items.RemoveAll();
	}

	/// Setup control's props (including message subscriber)
	void SetBarProps(MsgHandler* pMsgHandler, const iSize& itemSize)
	{
		m_MouseInside = false;
		m_idFocused = -1;
		m_idSelected = -1;
		m_pMsgHandler = pMsgHandler;
		m_itemSize = itemSize;
	}

	/// Sets selected item
	void SetCurTab(uint32 ntab)
	{
		m_idSelected = ntab;
		m_pMsgHandler->iCtrlOutlookBar_Notify(StateChanged,m_idSelected);
		Invalidate();
	}

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
	{
		if (wParam==7777) {
			if (!iWMetrics::IsMouseOnWindow(m_hWnd) && m_MouseInside==true) {
				KillTimer(7777);
				m_MouseInside=false;
				m_idFocused = -1;
				Invalidate();
			}
		}
		return 0;
	}

	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
	{
		if (!m_MouseInside && iWMetrics::IsMouseOnWindow(m_hWnd)) {
			m_MouseInside=true;
			SetTimer(7777,50);
		}

		uint32 nid = GetIdByPos(iWMetrics::LParam2Pos(lParam));
		if (nid != m_idFocused) {
			m_idFocused = nid;
			Invalidate();
		}
		return 0;
	}

	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
	{
		m_idTracked = GetIdByPos(iWMetrics::LParam2Pos(lParam));
		return 0;
	}

	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
	{
		uint32 nid = GetIdByPos(iWMetrics::LParam2Pos(lParam));
		if (m_idTracked == nid && m_idTracked != -1 ) {
			if (m_pMsgHandler->iCtrlOutlookBar_ItemChanging(m_idSelected, m_idTracked)){
				m_idSelected = m_idTracked;
				m_pMsgHandler->iCtrlOutlookBar_Notify(StateChanged,m_idTracked);
				Invalidate();
			}
		}
		return 0;
	}

	uint32 GetIdByPos(const iPoint& pos)
	{
		sint32 idx = pos.y/(m_itemSize.h+24);
		if (idx<0 || idx >= (sint32)m_Items.GetSize()) return -1;
		return m_Items[idx].m_Id;
	}

	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
	{ return 0; }
	
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
	{
		if (m_memDC.GetSize() != iWMetrics::GetClSize(m_hWnd)) {
			m_memDC.Resize(iWMetrics::GetClSize(m_hWnd));
		}

		HGDIOBJ oldFont = ::SelectObject(m_memDC,GetStockObject(DEFAULT_GUI_FONT));
		m_memDC.SetTextBkMode(TRANSPARENT);

		m_memDC.m_Dib.Fill(iColor(COLORREF2ICOLOR(GetSysColor(COLOR_BTNFACE))));
		
		for (uint32 xx=0; xx< m_Items.GetSize(); ++xx) {
			uint32 flags = 0;
			if (m_Items[xx].m_Id==m_idSelected) flags |= Selected;
			if (m_Items[xx].m_Id==m_idFocused) flags |= Focused;

			m_pMsgHandler->iCtrlOutlookBar_ComposeItem(
				m_memDC,
				iRect(2,xx*(m_itemSize.h+24),m_memDC.GetSize().w-4,m_itemSize.h+24),
				m_Items[xx],
				flags);
		}

		::SelectObject(m_memDC,oldFont);

		PAINTSTRUCT ps;
		iDC dc = BeginPaint(&ps);
		BitBlt(dc,0,0,m_memDC.GetSize().w,m_memDC.GetSize().h,m_memDC,0,0,SRCCOPY);
		EndPaint(&ps);
		bHandler=TRUE;
		return 0;
	}

	typedef iSimpleArray<Item>	ItemList;

private:
	bool		m_MouseInside;
	uint32		m_idTracked;
	uint32		m_idFocused;
	uint32		m_idSelected;
	iSize		m_itemSize;
	iMemoryDC	m_memDC;
	ItemList	m_Items;
	MsgHandler*	m_pMsgHandler;
};


#endif //_IOLIB_LITHIUM_CONTROL_OUTLOOK_BAR_H_
/** @} */ // iolib_li
