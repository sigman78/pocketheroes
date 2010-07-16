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
#include "ArtifactList.h"
#include "Dlg_ArtInfo.h"

enum ART_BUTTON_FLAGS {
	ABF_PRESSED = 0x1
};

const sint32 DRAG_DELTA = 5;
const uint16 INVART = 0xFFFF;

//
inline void ComposeArtifactCell(const iRect& rect, uint16 artId, uint32 flags)
{
	iRect rc(rect);
	rc.InflateRect(1);
	gApp.Surface().FrameRect(rc,cColor_Black);

	/*
	SpriteId bsid;
	if (artId == INVART) bsid = PDGG_ART_CELLS;
	else if (flags & ABF_PRESSED) bsid = PDGG_ART_CELLS + 1;
	else bsid = PDGG_ART_CELLS + 2;
	Tile3x3Sprite(gApp.Surface(), rect, bsid);*/

	gApp.Surface().FillRect(rect,(artId == INVART)?RGB16(64,64,64):RGB16(128,128,0),64);
	ButtonFrame(gApp.Surface(),rect, (flags & ABF_PRESSED)?iButton::Pressed:0);
	if (artId != INVART) BlitIcon(gApp.Surface(), gGame.ItemMgr().m_artMgr[artId].Sprite(), rect);
}

/*
 *	DragItem
 */
iArtDragDropItem::iDragItem::iDragItem(iArtDragDropItem* pSender, sint32 fromCell,const iArtCell& artCell, const iPoint& pos, const iSize& siz)
: m_pSender(pSender), m_fromCell(fromCell), m_artCell(artCell), m_pos(pos), m_siz(siz), m_pReceiver(NULL), m_toCell(-1)
{}

void iArtDragDropItem::iDragItem::ComposeDragGlyph()
{
	iRect rc(m_pos.x-(sint32)(m_siz.w/2),m_pos.y-(sint32)(m_siz.h/2),m_siz.w,m_siz.h);
	ComposeArtifactCell(rc, m_artCell.artId, 0);
	if (!m_pReceiver || m_toCell == -1) gApp.Surface().FillRect(rc,cColor_Red,32);
	else gApp.Surface().FillRect(rc,cColor_Green,32);
}

/*
 *	iArtDragDropItem
 */
iArtDragDropItem::iArtDragDropItem(iSimpleArray<iArtDragDropItem*>& competitors, const iSize& cellSiz, iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid)
: iBaseCtrl(pViewMgr,pCmdHandler, rect, GENERIC_VIEWPORT, uid, Visible|Enabled)
, m_cellSiz(cellSiz)
, m_Competitors(competitors)
, m_pDragItem(NULL), m_pDropItem(NULL)
{
	m_Competitors.Add(this);
}

void iArtDragDropItem::BeginDrag(sint32 fromCell, const iArtCell& artCell, const iPoint& pos)
{
	check(!m_pDragItem);
	m_pDragItem = new iDragItem(this, fromCell, artCell, pos, m_cellSiz);
	m_pMgr->SetDragGlyph(m_pDragItem);
	Drag(pos);
	for (uint32 xx=0; xx<m_Competitors.GetSize(); ++xx) {
		m_Competitors[xx]->Highlight(m_pDragItem);
	}	
}

void iArtDragDropItem::EndDrag()
{
	check(m_pDragItem);
	if (m_pDragItem->m_pReceiver) m_pDragItem->m_pReceiver->Drop();
	m_pMgr->SetDragGlyph(NULL);
	delete m_pDragItem;
	m_pDragItem = NULL;
	for (uint32 xx=0; xx<m_Competitors.GetSize(); ++xx) {
		m_Competitors[xx]->Unhighlight();
	}	
	Invalidate();
}

void iArtDragDropItem::Drag(const iPoint& pos)
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

/*
 *	iArtItemCtrl
 */
iArtItemCtrl::iArtItemCtrl(iSimpleArray<iArtDragDropItem*>& competitors, iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iPoint& pos, const iSize& cellSiz, uint32 uid)
: iArtDragDropItem(competitors, cellSiz, pViewMgr, pCmdHandler, iRect(pos, cellSiz) , uid)
, m_pOwner(NULL)
, m_pArtCell(NULL)
, m_bPressed(false)
, m_bClickMode(false)
, m_bHighlighted(false)
{
}

void iArtItemCtrl::OnCompose()
{
	iRect rc = GetScrRect();
	ComposeArtifactCell(rc, (m_pArtCell)?m_pArtCell->artId:0xFFFF, (m_bPressed)?ABF_PRESSED:0);
	if (m_pDragItem) gApp.Surface().FrameRect(rc,cColor_Blue,64);
	if (m_bHighlighted) gApp.Surface().FrameRect(rc,cColor_Yello,64);
}

void iArtItemCtrl::OnMouseDown(const iPoint& pos)
{
	if (!m_pArtCell || m_pArtCell->artId == INVART) return;
	m_bPressed = true;
	m_dragAnchor = pos;
	m_bClickMode = true;
	Invalidate();
}

void iArtItemCtrl::OnMouseUp(const iPoint& pos)
{
	if (m_pDragItem) {
		EndDrag();
	}
	m_bPressed = false;
	Invalidate();
}

void iArtItemCtrl::OnMouseClick(const iPoint& pos)
{
	if (m_bClickMode) {
		iDlg_ArtInfo dlg(m_pMgr, m_pArtCell->artId, m_pOwner);
		dlg.DoModal();
		m_bClickMode = false;
	}
}

void iArtItemCtrl::OnMouseTrack(const iPoint& pos)
{
	if (m_pDragItem) {
		Drag(pos);
	} else if (m_pArtCell && !m_pArtCell->Empty() && pos.GetSqDelta(m_dragAnchor) > DRAG_DELTA) {
		m_bClickMode = false;
		BeginDrag(0, *m_pArtCell, pos);
		m_bPressed = false;
	}
}

void iArtItemCtrl::EnterDrop(iDragItem* pDragItem)
{
	check(!m_pDropItem);
	m_pDropItem = pDragItem;
	m_pDropItem->m_pReceiver = this;
	m_pDropItem->m_toCell = -1;
	Invalidate();
}

void iArtItemCtrl::LeaveDrop()
{
	check(m_pDropItem);
	m_pDropItem->m_pReceiver = NULL;
	m_pDropItem->m_toCell = -1;
	m_pDropItem = NULL;
	Invalidate();
}

bool iArtItemCtrl::Drop()
{
	if (m_pDropItem->m_toCell == -1) {
		LeaveDrop();
		Invalidate();
		return false;
	}

	if (m_pDropItem->m_pSender == m_pDropItem->m_pReceiver) {
		// (same cell) do nothing
	} else {
		if (m_pArtCell->artId != INVART) Replace(0, m_pDropItem->m_pSender->Replace(m_pDropItem->m_fromCell, *m_pArtCell));
		else Replace(0, m_pDropItem->m_pSender->Remove(m_pDropItem->m_fromCell));
	}
	LeaveDrop();
	Invalidate();
	return true;
}

void iArtItemCtrl::Highlight(iDragItem *dr) 
{
	m_bHighlighted = ( gGame.ItemMgr().m_artMgr[dr->m_artCell.artId].Assign() == m_pArtCell->AssignType() && 
		m_pOwner->Artifacts().CanAttach(dr->m_artCell.artId));
	Invalidate();
}

void iArtItemCtrl::Unhighlight() 
{
	m_bHighlighted = false;
	Invalidate();
}

bool iArtItemCtrl::DragDrop() const
{
	if ( gGame.ItemMgr().m_artMgr[m_pDropItem->m_artCell.artId].Assign() == m_pArtCell->AssignType() && m_pOwner->Artifacts().CanAttach(m_pDropItem->m_artCell.artId)) m_pDropItem->m_toCell = 0;
	else m_pDropItem->m_toCell = -1;
	return true;
}

iArtCell iArtItemCtrl::Remove(sint32 cell)
{
	iArtCell result = *m_pArtCell;
	if (result.artId != INVART) gGame.ItemMgr().m_artMgr[result.artId].Dettach(m_pOwner);
	m_pArtCell->Reset();
	return result;
}

iArtCell iArtItemCtrl::Replace(sint32 cell, const iArtCell& newArt)
{
	iArtCell result = *m_pArtCell;
	if (result.artId != INVART) gGame.ItemMgr().m_artMgr[result.artId].Dettach(m_pOwner);
	if (newArt.artId != INVART) gGame.ItemMgr().m_artMgr[newArt.artId].Attach(m_pOwner);
	m_pArtCell->artId = newArt.artId;
	return result;
}

/*
 *	iArtBackPackCtrl
 */
const uint32 SCROLL_BTN_SIZE = 15;
inline iRect BackPackListMetrics(const iPoint& pos, const iSize& cellSiz, uint32 cellsCount, uint32 flags)
{
	uint32 w = cellSiz.w;
	uint32 h = cellSiz.h;

	if (flags & iArtBackPackCtrl::Horizontal) {
		w = SCROLL_BTN_SIZE * 2 + cellSiz.w * cellsCount + cellsCount + 1;
	} else {
		h = SCROLL_BTN_SIZE * 2 + cellSiz.h * cellsCount + cellsCount + 1;
	}

	return iRect(pos.x, pos.y, w, h);
}

iArtBackPackCtrl::iArtBackPackCtrl(iSimpleArray<iArtDragDropItem*>& competitors, iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iPoint& pos, const iSize& cellSiz, uint32 cellsCount, uint32 flags, uint32 uid)
: iArtDragDropItem(competitors, cellSiz, pViewMgr,pCmdHandler,BackPackListMetrics(pos, cellSiz, cellsCount, flags), uid)
, m_flags(flags)
, m_cellSize(cellSiz)
, m_cellsCount(cellsCount)
, m_pBackPack(NULL)
, m_listOffset(0)
, m_dCell(-1)
, m_bClickMode(false)
, m_pOwner(NULL)
{
	if (flags & Horizontal) {
		AddChild(new iIconButton(pViewMgr, this, iRect(0,0,SCROLL_BTN_SIZE, m_Rect.h),PDGG_SCRBAR_BTNS+2,101));
		AddChild(new iIconButton(pViewMgr, this, iRect(m_Rect.w-SCROLL_BTN_SIZE,0,SCROLL_BTN_SIZE, m_Rect.h),PDGG_SCRBAR_BTNS+3,102));
	} else {
		AddChild(new iIconButton(pViewMgr, this, iRect(0,0,m_Rect.w,SCROLL_BTN_SIZE),PDGG_SCRBAR_BTNS,101));
		AddChild(new iIconButton(pViewMgr, this, iRect(0,m_Rect.h-SCROLL_BTN_SIZE,m_Rect.w,SCROLL_BTN_SIZE),PDGG_SCRBAR_BTNS+1,102));
	}
}

void iArtBackPackCtrl::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (uid == 101) ListPageUp();
	else if (uid == 102) ListPageDown();
}

void iArtBackPackCtrl::ListPageUp()
{
	if (!m_pBackPack || !m_pBackPack->GetSize()) {
		m_listOffset = 0;
		return;
	}

	if (m_pBackPack->GetSize() > m_cellsCount) {
		if (m_listOffset == 0) SetListOffset(m_pBackPack->GetSize() - 1);
		else SetListOffset(m_listOffset - 1);
	}
}

void iArtBackPackCtrl::ListPageDown()
{
	if (!m_pBackPack || !m_pBackPack->GetSize()) {
		m_listOffset = 0;
		return;
	}

	if (m_pBackPack->GetSize() > m_cellsCount) {
		if (m_listOffset == ((sint32)m_pBackPack->GetSize()-1)) SetListOffset(0);
		else SetListOffset(m_listOffset + 1);
	}
}

void iArtBackPackCtrl::SetListOffset(sint32 offs)
{
	if (m_pBackPack) {
		m_listOffset = iCLAMP<sint32>(0,m_pBackPack->GetSize()-1, offs);
	} else m_listOffset = 0;
}

sint32 iArtBackPackCtrl::CellByPos(const iPoint& pos) const
{
	sint32 val;
	if (m_flags & Horizontal) {
		val = (pos.x - GetScrRect().x - SCROLL_BTN_SIZE) / (sint32)(m_cellSize.w+1);
	} else {
		val = (pos.y - GetScrRect().y - SCROLL_BTN_SIZE) / (sint32)(m_cellSize.h+1);
	}

	if (val<0 || val >= (sint32)m_cellsCount) return -1;
	return val;
}

sint32 iArtBackPackCtrl::BackPackIdxByCell(sint32 cellIdx) const
{
	if (!m_pBackPack || cellIdx == -1 || cellIdx >= (sint32)m_pBackPack->GetSize()) return -1;
	return (m_listOffset + cellIdx) % m_pBackPack->GetSize();
}

void iArtBackPackCtrl::OnCompose()
{
	iRect rc = GetScrRect();
	iRect crc(rc.x, rc.y, m_cellSize.w, m_cellSize.h);

	// Draw frame arround up/down buttons
	for (uint32 ctrlId=0; ctrlId<m_Childs.GetSize(); ++ctrlId) {
		iRect ctrlRc = m_Childs[ctrlId]->GetScrRect();
		ctrlRc.InflateRect(1);
		gApp.Surface().FrameRect(ctrlRc,cColor_Black);
	}


	if (m_flags & Horizontal) crc.x += SCROLL_BTN_SIZE + 1;
	else crc.y += SCROLL_BTN_SIZE + 1;

	// Compose cells
	for (uint32 xx=0; xx<m_cellsCount; ++xx) {
		sint32 bpidx = BackPackIdxByCell(xx);
		ComposeArtifactCell(crc, (bpidx==-1)?INVART:m_pBackPack->At(bpidx), ((sint32)xx==m_dCell)?ABF_PRESSED:0);
		if (m_flags & Horizontal) crc.x += m_cellSize.w + 1;
		else crc.y += m_cellSize.h + 1;
	}
}

void iArtBackPackCtrl::OnMouseDown(const iPoint& pos)
{
	if (!m_pBackPack) return;
	sint32 ncell = CellByPos(pos);
	if (ncell == -1 || BackPackIdxByCell(ncell) == -1) return;
	m_dCell = ncell;
	m_dragAnchor = pos;
	m_bClickMode = true;
	Invalidate();
}

void iArtBackPackCtrl::OnMouseUp(const iPoint& pos)
{
	if (m_pDragItem) {
		EndDrag();
	}
	m_dCell = -1;
	Invalidate();
}

void iArtBackPackCtrl::OnMouseClick(const iPoint& pos)
{
	if (m_bClickMode) {
		sint32 bpIdx = BackPackIdxByCell(CellByPos(pos));
		if (bpIdx != -1) {
			iDlg_ArtInfo dlg(m_pMgr, m_pBackPack->At(bpIdx), m_pOwner);
			dlg.DoModal();
		}
		m_bClickMode = false;
	}
}

void iArtBackPackCtrl::OnMouseTrack(const iPoint& pos)
{
	if (m_pDragItem) {
		Drag(pos);
	} else if (m_pBackPack && m_dCell != -1 && pos.GetSqDelta(m_dragAnchor) > DRAG_DELTA) {
		sint32 bpidx = BackPackIdxByCell(m_dCell);
		if (bpidx != -1) {
			iArtCell ac(AC_UNDEFINED, m_pBackPack->At(bpidx));
			m_bClickMode = false;
			BeginDrag(m_dCell, ac, pos);
		}
		m_dCell = -1;
	}
}

void iArtBackPackCtrl::EnterDrop(iDragItem* pDragItem)
{
	check(!m_pDropItem);
	m_pDropItem = pDragItem;
	m_pDropItem->m_pReceiver = this;
	m_pDropItem->m_toCell = -1;
	Invalidate();
}

void iArtBackPackCtrl::LeaveDrop()
{
	check(m_pDropItem);
	m_pDropItem->m_pReceiver = NULL;
	m_pDropItem->m_toCell = -1;
	m_pDropItem = NULL;
	Invalidate();
}

bool iArtBackPackCtrl::Drop()
{
	if (m_pDropItem->m_toCell == -1) {
		LeaveDrop();
		Invalidate();
		return false;
	}

	sint32 tobpidx = BackPackIdxByCell(m_pDropItem->m_toCell);
	if (m_pDropItem->m_pSender == m_pDropItem->m_pReceiver && tobpidx != -1) {
		sint32 frombpidx = BackPackIdxByCell(m_pDropItem->m_fromCell);
		if (tobpidx != frombpidx) {
			iSwap(m_pBackPack->At(frombpidx), m_pBackPack->At(tobpidx));
		}
	} else {
		if (tobpidx == -1) m_pBackPack->Add(m_pDropItem->m_pSender->Remove(m_pDropItem->m_fromCell).artId);
		else m_pBackPack->InsertBefore(tobpidx, m_pDropItem->m_pSender->Remove(m_pDropItem->m_fromCell).artId);
	}
	LeaveDrop();
	Invalidate();
	return true;
}

bool iArtBackPackCtrl::DragDrop() const
{
	m_pDropItem->m_toCell = CellByPos(m_pDropItem->m_pos);
	return true;
}

iArtCell iArtBackPackCtrl::Remove(sint32 cell)
{
	check(m_pBackPack);
	sint32 bpidx = BackPackIdxByCell(cell);
	check(bpidx != -1);
	iArtCell result(AC_UNDEFINED, m_pBackPack->At(bpidx));
	m_pBackPack->RemoveAt(bpidx);
	return result;
}

iArtCell iArtBackPackCtrl::Replace(sint32 cell, const iArtCell& newArt)
{
	check(m_pBackPack);
	sint32 bpidx = BackPackIdxByCell(cell);
	check(bpidx != -1);
	iArtCell result(AC_UNDEFINED, m_pBackPack->At(bpidx));
	m_pBackPack->At(bpidx) = newArt.artId;
	return result;
}
/*
 *	simple Artifact list control
 */
iArtListCtrl::iArtListCtrl(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iArtList& artList, const iPoint& pos, const iSize& cellSiz, uint32 cellsCount, uint32 flags, uint32 uid)
: iBaseCtrl(pViewMgr, pCmdHandler, BackPackListMetrics(pos, cellSiz, cellsCount, flags), GENERIC_VIEWPORT, uid, Visible|Enabled)
, m_flags(flags)
, m_cellSize(cellSiz)
, m_cellsCount(cellsCount)
, m_artList(artList)
, m_listOffset(0)
, m_dCell(-1)
, m_clickItem(-1)
{
	if (flags & Horizontal) {
		AddChild(new iIconButton(pViewMgr, this, iRect(0,0,SCROLL_BTN_SIZE, m_Rect.h),PDGG_SCRBAR_BTNS+2,101));
		AddChild(new iIconButton(pViewMgr, this, iRect(m_Rect.w-SCROLL_BTN_SIZE,0,SCROLL_BTN_SIZE, m_Rect.h),PDGG_SCRBAR_BTNS+3,102));
	} else {
		AddChild(new iIconButton(pViewMgr, this, iRect(0,0,m_Rect.w,SCROLL_BTN_SIZE),PDGG_SCRBAR_BTNS,101));
		AddChild(new iIconButton(pViewMgr, this, iRect(0,m_Rect.h-SCROLL_BTN_SIZE,m_Rect.w,SCROLL_BTN_SIZE),PDGG_SCRBAR_BTNS+1,102));
	}
}
void iArtListCtrl::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (uid == 101) ListPageUp();
	else if (uid == 102) ListPageDown();
}

void iArtListCtrl::ListPageUp()
{
	if (!m_artList.GetSize()) {
		m_listOffset = 0;
		return;
	}

	if (m_artList.GetSize() > m_cellsCount) {
		if (m_listOffset == 0) SetListOffset(m_artList.GetSize() - 1);
		else SetListOffset(m_listOffset - 1);
	}
}

void iArtListCtrl::ListPageDown()
{
	if (!m_artList.GetSize()) {
		m_listOffset = 0;
		return;
	}

	if (m_artList.GetSize() > m_cellsCount) {
		if (m_listOffset == ((sint32)m_artList.GetSize()-1)) SetListOffset(0);
		else SetListOffset(m_listOffset + 1);
	}
}

void iArtListCtrl::SetListOffset(sint32 offs)
{
	m_listOffset = iCLAMP<sint32>(0,m_artList.GetSize()-1, offs);
}

sint32 iArtListCtrl::CellByPos(const iPoint& pos) const
{
	sint32 val;
	if (m_flags & Horizontal) {
		val = (pos.x - GetScrRect().x - SCROLL_BTN_SIZE) / (sint32)(m_cellSize.w+1);
	} else {
		val = (pos.y - GetScrRect().y - SCROLL_BTN_SIZE) / (sint32)(m_cellSize.h+1);
	}

	if (val<0 || val >= (sint32)m_cellsCount) return -1;
	return val;
}

sint32 iArtListCtrl::BackPackIdxByCell(sint32 cellIdx) const
{
	if (cellIdx == -1 || cellIdx >= (sint32)m_artList.GetSize()) return -1;
	return (m_listOffset + cellIdx) % m_artList.GetSize();
}

void iArtListCtrl::OnCompose()
{
	iRect rc = GetScrRect();
	iRect crc(rc.x, rc.y, m_cellSize.w, m_cellSize.h);

	// Draw frame arround up/down buttons
	for (uint32 ctrlId=0; ctrlId<m_Childs.GetSize(); ++ctrlId) {
		iRect ctrlRc = m_Childs[ctrlId]->GetScrRect();
		ctrlRc.InflateRect(1);
		gApp.Surface().FrameRect(ctrlRc,cColor_Black);
	}


	if (m_flags & Horizontal) crc.x += SCROLL_BTN_SIZE + 1;
	else crc.y += SCROLL_BTN_SIZE + 1;

	// Compose cells
	for (uint32 xx=0; xx<m_cellsCount; ++xx) {
		sint32 bpidx = BackPackIdxByCell(xx);
		ComposeArtifactCell(crc, (bpidx==-1)?INVART:m_artList[bpidx], ((sint32)xx==m_dCell)?ABF_PRESSED:0);
		if (m_flags & Horizontal) crc.x += m_cellSize.w + 1;
		else crc.y += m_cellSize.h + 1;
	}
}

void iArtListCtrl::OnMouseDown(const iPoint& pos)
{
	sint32 ncell = CellByPos(pos);
	if (ncell == -1 || BackPackIdxByCell(ncell) == -1) return;
	m_dCell = ncell;
	Invalidate();
}

void iArtListCtrl::OnMouseUp(const iPoint& pos)
{
	sint32 ncell = CellByPos(pos);
	if (m_dCell != -1 && m_dCell == ncell) m_clickItem = m_dCell;
	m_dCell = -1;
	Invalidate();
}

void iArtListCtrl::OnMouseClick(const iPoint& pos)
{
	if (m_clickItem != -1) {
		sint32 bpIdx = BackPackIdxByCell(m_clickItem);
		if (bpIdx != -1) {
			iDlg_ArtInfo dlg(m_pMgr, m_artList[bpIdx], NULL);
			dlg.DoModal();
		}
		m_clickItem = -1;
	}
}

