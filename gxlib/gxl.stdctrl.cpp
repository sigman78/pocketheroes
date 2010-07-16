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

#include "gxl.inc.h"
#ifdef OS_WINCE
#include <gx.h>
#endif //OS_WINCE
#include "gxl.ctr.cbuff.h"
#include "gxl.ctr.array.h"
#include "gxl.window.h"
#include "gxl.input.h"
#include "gxl.timer.h"
#include "gxl.display.h"
#include "gxl.view.h"
#include "gxl.stdctrl.h"
#include "gxl.dialog.h"
#include "gxl.viewmgr.h"
#include "gxl.application.h"

/*
 *	Base view-port based control
 */
iBaseCtrl::iBaseCtrl(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 clsId, uint32 uid, uint32 state)
: iView(pViewMgr, rect, clsId, uid, state ), m_pCmdHandler(pCmdHandler) {}

void iBaseCtrl::SetCommandHandler(IViewCmdHandler* pCmdHandler)
{
	m_pCmdHandler = pCmdHandler;
}

/*
 *	Generic push putton
 */
iButton::iButton(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid, uint32 state)
: iBaseCtrl(pViewMgr, pCmdHandler, rect, PUSH_BUTTON, uid, state), m_lastClick(0), m_state(0) {}

void iButton::OnMouseDown(const iPoint& pos)
{
	m_state |= Pressed;
	OnBtnDown();
	Invalidate();
}

void iButton::OnMouseUp(const iPoint& pos)
{
	if (m_state & Pressed) {
		m_state ^= Pressed;
		OnBtnUp();
		Invalidate();
	}
}

void iButton::OnMouseClick(const iPoint& pos)
{
	if (m_pCmdHandler && IsEnabled()) {
		uint32 nt = GetTickCount();
		if ( m_lastClick && nt > m_lastClick && (nt-m_lastClick) < 500) {
			m_lastClick = 0;
			m_pCmdHandler->iCMDH_ControlCommand(this,CCI_BTNDBLCLICK,0);
		} else {
			m_lastClick = nt;
			m_pCmdHandler->iCMDH_ControlCommand(this,CCI_BTNCLICK,0);
		}
	}
}

void iButton::OnMouseTrack(const iPoint& pos)
{
	if (GetScrRect().PtInRect(pos)) {
		if ( (m_state & Pressed) != Pressed) {
			m_state |= Pressed;
			Invalidate();
		}
	} else if (m_state & Pressed) {
		m_state ^= Pressed;
		Invalidate();
	}
}

/*
 * Tabbed switch control	
 */
iTabbedSwitch::iTabbedSwitch(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 tabcnt, uint32 uid, uint32 state)
: iBaseCtrl(pViewMgr, pCmdHandler, rect, TABBED_SWITCH, uid, state)
, m_TabsCount(tabcnt), m_CurTab(0), m_FocTab(-1), m_CurFocTab(-1)
{
	m_tabStates = new uint32[tabcnt]; for (uint32 xx=0; xx<tabcnt; ++xx) m_tabStates[xx] = 1;
	m_ItemWidth = m_Rect.w / m_TabsCount;
	check(m_ItemWidth);
	check((rect.w%m_TabsCount) == 0);
}

iTabbedSwitch::~iTabbedSwitch()
{
	delete[] m_tabStates;
}


void iTabbedSwitch::OnMouseDown(const iPoint& pos)
{
	sint32 ntab = GetTabByPos(pos-GetScrRect().point());
	if (ntab >= 0 && ntab < m_TabsCount && m_tabStates[ntab]) m_CurFocTab = m_FocTab = ntab;
	Invalidate();
}

void iTabbedSwitch::OnMouseUp(const iPoint& pos)
{
	sint32 tab = GetTabByPos(pos-GetScrRect().point());
	if (tab == m_FocTab){
		if (m_CurTab == m_FocTab) {
			if (m_pCmdHandler && IsEnabled()) m_pCmdHandler->iCMDH_ControlCommand(this,CCI_BTNCLICK,m_CurTab);
		} else {
			m_CurTab = m_FocTab;
			if (m_pCmdHandler && IsEnabled()) m_pCmdHandler->iCMDH_ControlCommand(this,CCI_TABCHANGED, m_CurTab);
		}
		Invalidate();
	}
	m_CurFocTab = m_FocTab = -1;
}

void iTabbedSwitch::OnMouseTrack(const iPoint& pos)
{
	sint32 tab = GetTabByPos(pos-GetScrRect().point());
	if (tab != m_CurFocTab) Invalidate();
	if (tab == m_FocTab) m_CurFocTab = tab;
	else m_CurFocTab = -1;
}

void iTabbedSwitch::OnCompose()
{
	iRect rc = GetScrRect();
	rc.w = m_ItemWidth;
	for (sint32 xx=0; xx<m_TabsCount; ++xx) {
		uint32 state=0;
		if (xx==GetFocusedTab()) state |= iButton::Pressed;
		if (xx==GetCurrentTab()) state |= iButton::Selected;
		if (!m_tabStates[xx]) state |= iButton::Disabled;
		ComposeTabItem(xx, state, rc);
		rc.x += m_ItemWidth;
	}
}

/*
 *	Scroll Bar
 */
iScrollBar::iTBButton::iTBButton(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid, uint32 state, iScrollBar* pScrollBar, Element el)
: iButton(pViewMgr, pCmdHandler, rect, uid, state), m_pScrollBar(pScrollBar), m_el(el) {}

void iScrollBar::iTBButton::OnCompose()
{
	m_pScrollBar->ComposeSBElement(m_el, GetScrRect(), GetButtonState());
}

//
iScrollBar::iScrollBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid, uint32 flags)
: iBaseCtrl(pViewMgr, pCmdHandler, rect, SCROLL_BAR, uid, Visible|Enabled)
, m_flags(flags), m_totSiz(0), m_pagSiz(0), m_curPos(0), m_bThumbTrack(false)
{
	if (IsHorizontal()) {
		AddChild(m_pUp = new iTBButton(pViewMgr,this,iRect(0,0,GetDefBtnSiz(),rect.h),uid+1, Visible | Enabled, this, EL_BtnLeft));
		AddChild(m_pDown = new iTBButton(pViewMgr,this,iRect(rect.w-GetDefBtnSiz(),0,GetDefBtnSiz(),rect.h),uid+2, Visible | Enabled, this, EL_BtnRight));
	} else {
		AddChild(m_pUp = new iTBButton(pViewMgr,this,iRect(0,0,rect.w,GetDefBtnSiz()),uid+1, Visible | Enabled, this, EL_BtnUp));
		AddChild(m_pDown = new iTBButton(pViewMgr,this,iRect(0,rect.h-GetDefBtnSiz(),rect.w,GetDefBtnSiz()),uid+2, Visible | Enabled, this, EL_BtnDown));
	}
	CalcThumbMetrix();
}

void iScrollBar::SetMetrics(sint32 totSiz, sint32 pagSiz)
{
	m_totSiz = totSiz;
	m_pagSiz = pagSiz;
	m_curPos = 0;
	CalcThumbMetrix();
}

bool iScrollBar::SetCurPos(sint32 nVal)
{
	nVal = iCLAMP<sint32>(0,iMAX<sint32>(0,m_totSiz-m_pagSiz),nVal);
	if (!m_totSiz || nVal==m_curPos) return false;
	m_curPos = nVal;
	CalcThumbMetrix();
	return true;
}
	
bool iScrollBar::PageUp() 
{ 
	if (SetCurPos(m_curPos-m_pagSiz)) {
		if (m_pCmdHandler) m_pCmdHandler->iCMDH_ControlCommand(this,CCI_SBPAGEUP,m_curPos);
		Invalidate();
		return true;
	}
	return false;
}

bool iScrollBar::PageDown() 
{ 
	if (SetCurPos(m_curPos+m_pagSiz)) {
		if (m_pCmdHandler) m_pCmdHandler->iCMDH_ControlCommand(this,CCI_SBPAGEDOWN,m_curPos);
		Invalidate();
		return true;
	}
	return false;
}

void iScrollBar::OnMouseDown(const iPoint& pos)
{
	HitTestRes res = HitTest(pos - GetScrRect().point());
	
	if (res == HTR_Thumb) {
		m_bThumbTrack = true;
		m_trackAnchor = pos;
		m_trackPos = m_curPos;
		Invalidate();
	} else if (res == HTR_PgUp){
		if (PageUp()) Invalidate();
	} else if (res == HTR_PgDown){
		if (PageDown()) Invalidate();
	}
}

void iScrollBar::OnMouseUp(const iPoint& pos)
{
	m_bThumbTrack = false;
	Invalidate();
}

void iScrollBar::OnMouseTrack(const iPoint& pos)
{
	if (m_bThumbTrack) {
		sint32 n;
		if (IsHorizontal()) n = pos.x - m_trackAnchor.x;
		else n = pos.y - m_trackAnchor.y;
		if (SetCurPos(m_trackPos + NItems(n))) {
			if (m_pCmdHandler) m_pCmdHandler->iCMDH_ControlCommand(this,CCI_SBTRACKING,m_curPos);
			Invalidate();
		}
	}
}

void iScrollBar::OnCompose()
{
	iRect rc(GetScrRect());
	if (IsHorizontal()){
		ComposeSBElement(EL_Bkgnd, iRect(rc.x+m_thumbMetrix.es,rc.y,m_thumbMetrix.h,rc.h), 0);
		ComposeSBElement(EL_Thumb, iRect(rc.x+m_thumbMetrix.es+m_thumbMetrix.s1,rc.y,m_thumbMetrix.th,rc.h), (m_bEnabled?0:iButton::Disabled) | (m_bThumbTrack?iButton::Pressed:0));
	} else {
		ComposeSBElement(EL_Bkgnd, iRect(rc.x,rc.y+m_thumbMetrix.es,rc.w,m_thumbMetrix.h), 0);
		ComposeSBElement(EL_Thumb, iRect(rc.x,rc.y+m_thumbMetrix.es+m_thumbMetrix.s1,rc.w,m_thumbMetrix.th), (m_bEnabled?0:iButton::Disabled) | (m_bThumbTrack?iButton::Pressed:0));
	}
}

iScrollBar::HitTestRes iScrollBar::HitTest(const iPoint& pos)
{
	sint32 val;
	if (IsHorizontal()) val = pos.x - m_thumbMetrix.es;
	else val = pos.y - m_thumbMetrix.es;
	if (val < m_thumbMetrix.s1) return HTR_PgUp;
	val -= m_thumbMetrix.s1;
	if (val < m_thumbMetrix.th) return HTR_Thumb;
	return HTR_PgDown;
}
	
void iScrollBar::CalcThumbMetrix()
{
	if (IsHorizontal()){
		m_thumbMetrix.es = GetDefBtnSiz();
		m_thumbMetrix.h = ((sint32)m_Rect.w) - m_thumbMetrix.es*2;
	} else {
		m_thumbMetrix.es = GetDefBtnSiz();
		m_thumbMetrix.h = ((sint32)m_Rect.h) - m_thumbMetrix.es*2;
	}

	if (m_totSiz == 0) {
		m_thumbMetrix.th = m_thumbMetrix.h;
		m_thumbMetrix.s1 = m_thumbMetrix.s2 = 0; 
	} else {
		m_thumbMetrix.th	= iMAX<sint32>(ItemsHeight(iMIN(m_pagSiz,m_totSiz)),m_thumbMetrix.es);
		if (!m_curPos) m_thumbMetrix.s1 = 0;
		else {
			sint32 fpx = m_thumbMetrix.h - m_thumbMetrix.th;
			m_thumbMetrix.s1 = (fpx*m_curPos)/(m_totSiz-m_pagSiz);
		}
		m_thumbMetrix.s2 = m_thumbMetrix.h - m_thumbMetrix.th - m_thumbMetrix.s1;
	}
}

void iScrollBar::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (uid == m_UID+1) {
		// Scroll up
		if (SetCurPos(m_curPos-1)) m_pCmdHandler->iCMDH_ControlCommand(this,CCI_SBLINEUP,m_curPos);
	} else if (uid == m_UID+2) {
		// Scroll down
		if (SetCurPos(m_curPos+1)) m_pCmdHandler->iCMDH_ControlCommand(this,CCI_SBLINEDOWN, m_curPos);
	}
}

/*
 *	List Box (virtual)
 */
iListBox::iListBox(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid)
: iBaseCtrl(pViewMgr, pCmdHandler, rect, LIST_BOX, uid, Visible|Enabled)
, m_pScrollBar(NULL), m_selItem(-1), m_scrVal(0), m_lcTime(0), m_lcIdx(-1) {}

void iListBox::SetScrollBar(iScrollBar* pScrollBar)
{
	m_pScrollBar = pScrollBar;
	if (m_pScrollBar) {
		m_pScrollBar->SetCommandHandler(this);
		m_pScrollBar->SetMetrics(LBItemsCount(),PageSize());
		m_pScrollBar->SetCurPos(m_scrVal);
	}
}

void iListBox::UpdateListBox()
{
	m_scrVal = 0;
	m_pScrollBar->SetMetrics(LBItemsCount(),PageSize());
	m_pScrollBar->SetCurPos(m_scrVal);
	if (LBItemsCount()) m_selItem = 0;
	else m_selItem = -1;
	if (m_pCmdHandler) m_pCmdHandler->iCMDH_ControlCommand(this, CCI_LBSELCHANGED, m_selItem);
	Invalidate();
}

void iListBox::SetCurPos(sint32 sval)
{
	m_scrVal = sval;
}

void iListBox::SetCurSel(sint32 idx, bool bNotify)
{
	check(idx == -1 || IsValidIdx(idx));
	if (idx != m_selItem) {
		m_selItem = idx;
		if (bNotify && m_pCmdHandler) m_pCmdHandler->iCMDH_ControlCommand(this, CCI_LBSELCHANGED, m_selItem);
		if (idx != -1) {
			if (m_selItem < m_scrVal) {
				if (m_pScrollBar) m_pScrollBar->SetCurPos(m_selItem);
				SetCurPos(m_selItem);
			} else if (m_selItem > (m_scrVal+(sint32)PageSize()-1) ) {
				sint32 npos = m_selItem-PageSize()+1;
				if (m_pScrollBar) m_pScrollBar->SetCurPos(npos);
				SetCurPos(npos);
			}
		}
		Invalidate();
	}
}

void iListBox::SelPrev()
{
	if (m_selItem > 0) SetCurSel(m_selItem - 1, true);
}

void iListBox::SelNext()
{
	if ((m_selItem+1) < LBItemsCount()) SetCurSel(m_selItem + 1, true);
}

void iListBox::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	check(m_pScrollBar && m_pScrollBar == pView);
	SetCurPos(param);
}

void iListBox::OnMouseDown(const iPoint& pos)
{
	sint32 nIdx = IdxByPos(pos);
	if (!IsValidIdx(nIdx)) nIdx = -1;
	SetCurSel(nIdx, true);
}

void iListBox::OnMouseClick(const iPoint& pos)
{
	if (m_pCmdHandler) {
		uint32 nt = GetTickCount();
		if ( m_lcTime && nt > m_lcTime && (nt-m_lcTime) < 500 && m_selItem != -1 && m_lcIdx == m_selItem) {
			m_lcTime = 0;
			m_lcIdx = -1;
			m_pCmdHandler->iCMDH_ControlCommand(this,CCI_LBSELDBLCLICK,m_selItem);
		} else {
			m_lcTime = nt;
			m_lcIdx = m_selItem;
		}
	}
}

void iListBox::OnCompose()
{
	iRect rc=GetScrRect();
	ComposeLBBackground(rc);
	sint32 ih = LBItemHeight();
	sint32 ic = LBItemsCount();
	sint32 ps = PageSize();
	iRect irc(rc.x, rc.y, rc.w, ih);
	for (sint32 xx=m_scrVal; xx<iMIN(ic,m_scrVal+ps); ++xx){
		ComposeLBItem(xx,xx==m_selItem,irc);
		irc.y += ih;
	}
}

sint32 iListBox::IdxByPos(const iPoint& pos)
{
	return m_scrVal + (pos.y-GetScrRect().y)/LBItemHeight();
}
