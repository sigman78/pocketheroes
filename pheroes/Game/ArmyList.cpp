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

#include "stdafx.h"
#include "Dlg_SplitCreatures.h"
#include "Dlg_CreatInfo.h"

const sint32 DRAG_DELTA = 5;

/*
 *	DragItem
 */
iArmyListEx::iDragItem::iDragItem(iArmyListEx* pSender, sint8 fromCell, bool bSplit, const iCreatGroup& cGroup, const iPoint& pos, const iSize& siz, bool bCanDismiss)
: m_pSender(pSender), m_fromCell(fromCell), m_bSplit(bSplit), m_cGroup(cGroup), m_pos(pos), m_siz(siz), m_bCanDismiss(bCanDismiss), m_pReceiver(NULL), m_toCell(-1)
{}

void iArmyListEx::iDragItem::ComposeDragGlyph()
{
	iRect rc(m_pos.x-(sint32)(m_siz.w/2),m_pos.y-(sint32)(m_siz.h/2),m_siz.w,m_siz.h);
	ComposeCreatureCell(gApp.Surface(),rc,m_cGroup.Type(),m_cGroup.Count(),VL_EXPERT);
	if (!m_pReceiver || m_toCell == -1) gApp.Surface().FillRect(rc,cColor_Red,32);
	else gApp.Surface().FillRect(rc,cColor_Green,32);
}


/*
 *	Extendet Army List constrol (with drag'n'drop capabilities)
 */
iArmyListEx::iArmyListEx(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iPoint& pos, iCheckButton* pSplitBtn, const iSize& itemSiz, uint32 uid)
: iBaseCtrl(pViewMgr,pCmdHandler,iRect(pos.x,pos.y,(itemSiz.w+1)*7-1,itemSiz.h),GENERIC_VIEWPORT,uid, Visible|Enabled)
, m_itemW(itemSiz.w), m_dCell(-1), m_pArmy(NULL), m_pOwner(NULL), m_bCanDismiss(false), m_pDragItem(NULL), m_pDropItem(NULL), m_lastClick(0), m_bSplit(false), m_pSplitBtn(pSplitBtn)
{
	AddCompetitor(this);
}

//
void iArmyListEx::AddCompetitor(iArmyListEx* pCompetitor)
{
	m_Competitors.Add(pCompetitor);
}

void iArmyListEx::BeginDrag(sint8 fromCell, iCreatGroup& cGroup, const iPoint& pos)
{
	check(!m_pDragItem);
	m_pDragItem = new iDragItem(this, fromCell, (m_bSplit || (m_pSplitBtn && m_pSplitBtn->IsChecked())), cGroup, pos, iSize(m_itemW,m_Rect.h), m_bCanDismiss || m_pArmy->GroupCount() > 1);
	m_pMgr->SetDragGlyph(m_pDragItem);
	Drag(pos);
}
void iArmyListEx::EndDrag()
{
	check(m_pDragItem);
	check(m_pArmy);
	m_pMgr->SetDragGlyph(NULL);
	if (m_pDragItem->m_pReceiver) m_pDragItem->m_pReceiver->Drop();
	delete m_pDragItem;
	m_pDragItem = NULL;
	if (m_pSplitBtn) m_pSplitBtn->SetChecked(false);
	Invalidate();
}

void iArmyListEx::Drag(const iPoint& pos)
{
	check(m_pDragItem);
	m_pDragItem->m_pos = pos;
	for (uint32 xx=0; xx<m_Competitors.GetSize(); ++xx) {
		if (m_Competitors[xx]->GetScrRect().PtInRect(pos)) {
			if (m_pDragItem->m_pReceiver != m_Competitors[xx]) {
				if (m_pDragItem->m_pReceiver) m_pDragItem->m_pReceiver->LeaveDrop();
				m_Competitors[xx]->EnterDrop(m_pDragItem);
			}
			if (m_pDragItem->m_pReceiver) m_pDragItem->m_pReceiver->DragDrop();
			Invalidate();
			return;
		}
	}
	if (m_pDragItem->m_pReceiver) m_pDragItem->m_pReceiver->LeaveDrop();
	Invalidate();
}

void iArmyListEx::EnterDrop(iDragItem* pDragItem)
{
	if (!m_pArmy) return;
	check(!m_pDropItem);
	m_pDropItem = pDragItem;
	m_pDropItem->m_pReceiver = this;
	m_pDropItem->m_toCell = -1;
	Invalidate();
}

void iArmyListEx::LeaveDrop()
{
	check(m_pDropItem);
	m_pDropItem->m_pReceiver = NULL;
	m_pDropItem->m_toCell = -1;
	m_pDropItem = NULL;
	Invalidate();
}

bool iArmyListEx::Drop()
{
	if (m_pDropItem->m_toCell == -1) {
		LeaveDrop();
		Invalidate();
		return false;
	}

	if (m_pDropItem->m_pSender == m_pDropItem->m_pReceiver && m_pDropItem->m_fromCell == m_pDropItem->m_toCell) {
		// (same cell) do nothing
	} else if (m_pArmy->At(m_pDropItem->m_toCell).Type() == m_pDropItem->m_cGroup.Type()) {
		// (similar type of creatures)
		if (m_pDropItem->m_bSplit) {
			// split mode (show split dialog)
			uint32 flags = iDlg_SplitCreatures::Normal;
			if (m_pDropItem != m_pDragItem){
				if (!m_pDropItem->m_bCanDismiss && m_pDropItem->m_pSender->Army()->GroupCount() <=1) flags |= iDlg_SplitCreatures::SafeG1;
				if (!m_bCanDismiss && m_pArmy->GroupCount() <= 1) flags |= iDlg_SplitCreatures::SafeG2;
			}
			iDlg_SplitCreatures scdlg(m_pMgr, PID_NEUTRAL, m_pDropItem->m_pSender->Army()->At(m_pDropItem->m_fromCell), m_pArmy->At(m_pDropItem->m_toCell), flags);
			scdlg.DoModal();
		} else {
			// normal mode (merge groups)
			m_pArmy->At(m_pDropItem->m_toCell).Count() += m_pDropItem->m_cGroup.Count();
			m_pDropItem->m_pSender->Army()->At(m_pDropItem->m_fromCell).Reset();
		}
	} else {
		if (m_pDropItem->m_bSplit) {
			// show split dialog
			uint32 flags = iDlg_SplitCreatures::Normal;
			if (m_pDropItem != m_pDragItem && !m_pDropItem->m_bCanDismiss && m_pDropItem->m_pSender->Army()->GroupCount() <= 1) flags |= iDlg_SplitCreatures::SafeG1;
			iDlg_SplitCreatures scdlg(m_pMgr, PID_NEUTRAL, m_pDropItem->m_pSender->Army()->At(m_pDropItem->m_fromCell), m_pArmy->At(m_pDropItem->m_toCell), flags);
			scdlg.DoModal();
		} else {
			// swap cells
			iSwap(m_pArmy->At(m_pDropItem->m_toCell),m_pDropItem->m_pSender->Army()->At(m_pDropItem->m_fromCell));
		}
	}
	LeaveDrop();
	Invalidate();
	return true;
}

bool iArmyListEx::DragDrop() const
{
	sint8 toCell = Pos2Cell(m_pDropItem->m_pos);
	if (m_pDropItem->m_bSplit) {
		if (toCell != -1) {
			iCreatGroup& fGroup = m_pDropItem->m_pSender->Army()->At(m_pDropItem->m_fromCell);
			iCreatGroup& tGroup = m_pArmy->At(toCell);
			if (!tGroup.IsValid() || tGroup.Type() == fGroup.Type()) m_pDropItem->m_toCell = toCell;
		}
	} else {
		if (toCell == -1 
			|| m_pDropItem->m_bCanDismiss 
			|| m_pDropItem == m_pDragItem 
			|| (m_pArmy->At(toCell).IsValid() && m_pArmy->At(toCell).Type() != m_pDropItem->m_pSender->Army()->At(m_pDropItem->m_fromCell).Type())) 
			m_pDropItem->m_toCell = toCell;
		else
			m_pDropItem->m_toCell = -1;
	}
	return true;
}

//
void iArmyListEx::SetSplitMode(bool bSplit)
{
	m_bSplit = bSplit;
}

void iArmyListEx::ToggleSplitMode()
{
	SetSplitMode(!m_bSplit);
}

void iArmyListEx::SetArmy(iArmy* pArmy, iHero* pOwner, bool bCanDismiss)
{
	m_pArmy = pArmy;
	m_pOwner = pOwner;
	m_bCanDismiss = bCanDismiss;
	m_dCell = -1;
	Invalidate();
}

void iArmyListEx::OnCompose()
{
	iRect rc = GetScrRect();

	iRect cMetrix(rc.x,rc.y,m_itemW,rc.h);
	for (sint32 xx=0; xx<7; ++xx) {
		gApp.Surface().FrameRect(iRect(cMetrix.x-1,cMetrix.y-1,cMetrix.w+2,cMetrix.h+2), cColor_Black);
		if (m_pArmy) ComposeCreatureButton(gApp.Surface(), cMetrix, m_pArmy->At(xx).Type(), m_pArmy->At(xx).Count(), VL_EXPERT, m_dCell == xx, (m_pDragItem && xx == m_pDragItem->m_fromCell), (m_pDropItem && xx == m_pDropItem->m_toCell));
		else ComposeCreatureButton(gApp.Surface(), cMetrix, CREAT_UNKNOWN, 0, VL_EXPERT, m_dCell == xx, false,false);
		cMetrix.x += (cMetrix.w+1);
	}
}

void iArmyListEx::OnMouseDown(const iPoint& pos)
{
	if (!m_pArmy) return;
	sint8 ncell = Pos2Cell(pos);
	if (!m_pArmy->At(ncell).IsValid()) return;
	m_dCell = ncell;
	m_dragAnchor = pos;
	Invalidate();
}

void iArmyListEx::OnMouseUp(const iPoint& pos)
{
	if (m_pDragItem) {
		EndDrag();
	}
}

void iArmyListEx::OnMouseClick(const iPoint& pos)
{
	if (m_pDragItem) return;
	if (m_pArmy && m_dCell != -1 && Pos2Cell(pos) == m_dCell) {
		uint32 nt = GetTickCount();
		sint32 nv = m_dCell;
		m_dCell = -1;
		Invalidate();
		if ( m_lastClick && nt > m_lastClick && (nt-m_lastClick) < 500) {
			m_lastClick = 0;
			m_pCmdHandler->iCMDH_ControlCommand(this,CCI_BTNDBLCLICK,nv);
			if (m_pArmy->At(nv).Type() != CREAT_UNKNOWN) {
				iFurtSkills fs;
				if (m_pOwner) fs = m_pOwner->GetFullFurtSkills();
				else fs.Reset();
				iDlg_CreatInfo dlg(m_pMgr, PID_NEUTRAL, m_pArmy->At(nv), fs, m_bCanDismiss || m_pArmy->GroupCount()>1, m_pArmy->MoraleModifier());
				dlg.DoModal();
			}
		} else {
			m_lastClick = nt;
			m_pCmdHandler->iCMDH_ControlCommand(this,CCI_BTNCLICK,nv);
		}
	}
}

void iArmyListEx::OnMouseTrack(const iPoint& pos)
{
	if (m_pDragItem) {
		Drag(pos);
	} else if (m_pArmy && pos.GetSqDelta(m_dragAnchor) > DRAG_DELTA && m_dCell != -1 && m_pArmy->At(m_dCell).IsValid()) {
		BeginDrag(m_dCell, m_pArmy->At(m_dCell), pos);
		m_dCell = -1;
	}
}


