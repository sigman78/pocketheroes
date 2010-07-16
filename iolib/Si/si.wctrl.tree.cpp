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

#include "si.common.h"
#include "si.wctrl.tree.h"
#include "../he/he.wnd.helpers.h"

//
//
// iTreeNode Implementation
//
//

iTreeNode* iTreeNode::WalkPrevOpened(iTreeNode* endNode)
{
	if (_prev && _prev->IsOpen() && _prev->Count()){
		iTreeNode *newNode = *_prev->Last();

		while (newNode) {
			if (!newNode->IsOpen() || !newNode->Count()) return newNode;
			newNode = *newNode->Last();
		}
		check(0);
		return NULL;
	} else if (_prev) {
		return _prev;
	} else {
		return _parent;
	}
}

iTreeNode* iTreeNode::WalkNextOpened()
{
	if (IsOpen() && Count()) {
		return *First();
	} else if (_next) {
		return _next;
	} else {
		iTreeNode* newNode = _parent;
		while (newNode) {
			if ( newNode->_next ) return newNode->_next;
			newNode = newNode->_parent;
		}
	}
	return NULL;
}

//
//
// iTreeCtrl Implementation
//
//

/*
 *	C-tor
 */
iTreeCtrl::iTreeCtrl()
: m_pEventsHnd(NULL)
, m_pComposerHnd(NULL)
, m_pDragDropHnd(NULL)
, m_pRootNode(NULL)
, m_CanDrag(false)
, m_AcceptDrop(false)
, m_Dragging(false)
, m_ForceDrop(false)
, m_DropTarget(NULL)
, m_pDragSender(NULL)
, m_DragAnchor(cInvalidPoint)
, m_nrmFont(NULL)
, m_bldFont(NULL)
, m_clrBkgnd(cColorWhite)
{}

/*
 *	D-tor
 */
iTreeCtrl::~iTreeCtrl()
{
	if (m_nrmFont) DeleteObject(m_nrmFont);
	if (m_bldFont) DeleteObject(m_bldFont);
}


/*
 *	Creates Tree control and inits members
 */
bool iTreeCtrl::CreateTreeCtrl(HWND parent, RECT &rect, uint32 cid, uint32 flags, iEvents* pEventsHnd, iComposer* pComposerHnd, iDragDrop* pDragDropHnd)
{
	m_pEventsHnd = pEventsHnd;
	m_pComposerHnd = pComposerHnd;
	m_pDragDropHnd = pDragDropHnd;

	m_OutMemDC.InitDC(iSize(300,300));
	m_OutMemDC.m_Dib.Fill(cColorBlue128);

	m_nrmFont = CreateFont(-12,0,0,0, FW_NORMAL, FALSE,FALSE,FALSE, HANGUL_CHARSET, OUT_RASTER_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("MS Sans Serif"));
	m_bldFont = CreateFont(-12,0,0,0, FW_BOLD, FALSE,TRUE,FALSE, HANGUL_CHARSET, OUT_RASTER_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("MS Sans Serif"));

	m_MultiSel = ((flags & ITC_MULTISELECTION) == ITC_MULTISELECTION);
	m_CanDrag = ((flags & ITC_CANDRAG) == ITC_CANDRAG);

	DWORD style = WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_VSCROLL | WS_TABSTOP;
	
	// Create window
	if (!Create(parent,rect,0,style,0,cid)) return false;

	return true;
}

/*
 *	Drag'n'Drop
 */

void iTreeCtrl::SetDragDropIcons(const iDib& dibOk, const iDib& dibNo)
{
	check(m_CanDrag);
	// Init Drag'n'Drop ImageList
	m_DragList.Create(64,64,ILC_COLOR32,4,1);
	m_DragList.Add(dibOk.GetBitmap());
	m_DragList.Add(dibNo.GetBitmap());
}

// Add Drop target
void iTreeCtrl::AddDropTarget(iTreeCtrl *ptarget)
{
	check(m_CanDrag);
	m_DropTargets.Add(ptarget);
}

// Remove Drop target
void iTreeCtrl::RemoveDropTarget(iTreeCtrl *ptarget)
{
	check(m_CanDrag);
	m_DropTargets.Remove(ptarget);
}

// Remove All Drop targets
void iTreeCtrl::RemoveAllDropTargets()
{
	check(m_CanDrag);
	m_DropTargets.RemoveAll();
}

// Is acceptable target
bool iTreeCtrl::IsAcceptableTarget(HWND hwnd)
{
	for (uint32 xx=0; xx<m_DropTargets.GetSize(); ++xx) {
		if (m_DropTargets[xx]->m_hWnd == hwnd) return true;
	}
	return false;
}

// Returns iTreeCtrl pointer by window handle
iTreeCtrl* iTreeCtrl::FindTarget(HWND hwnd)
{
	for (uint32 xx=0; xx<m_DropTargets.GetSize(); ++xx)
	{
		if (m_DropTargets[xx]->m_hWnd == hwnd) return m_DropTargets[xx];
	}
	check(0);
	return NULL;
}

// Enter to control dropable area
bool iTreeCtrl::DropEnter(iTreeCtrl* sender)
{
	m_pDragSender = sender;
	SetTimer(TID_DRAGNDROP,TINT_DRAGNDROP);
	m_ForceDrop = true;
	m_DropPos.pType = iDropPosition::PosInvalid;
	m_pDragSender->GetSelNodes(m_DropItems);

	return true;
}

// Exit to control dropable area
void iTreeCtrl::DropLeave()
{
	KillTimer(TID_DRAGNDROP);
	m_ForceDrop = false;
	m_AcceptDrop = false;
	m_DropPos.pType = iDropPosition::PosInvalid;
	
	m_pDragSender->m_DragList.DragShowNolock(FALSE);
	ForceRepaint();
	m_pDragSender->m_DragList.DragShowNolock(TRUE);

	m_DropItems.RemoveAll();
	m_pDragSender = NULL;
}

// Process drag query (if possible, then returns true, otherwise returns false)
bool iTreeCtrl::DragQuery() 
{ 
	check(m_CanDrag);
	check(m_pDragDropHnd);
	
	
	return m_pDragDropHnd->iTree_DragQuery(this,m_SelItems); 
}

// Process drop (if dropped successfuly, then returns true, otherwise returns false)
bool iTreeCtrl::Drop()
{
	check(m_pDragDropHnd);
	if (!m_AcceptDrop) return false;
	return m_pDragDropHnd->iTree_Drop(this,m_pDragSender,m_DropItems,m_DropPos);
}

// Process drop query (if accept drop, then returns true, otherwise returns false)
bool iTreeCtrl::DropQuery()
{
	check(m_pDragDropHnd);
	iDropPosition ndpos = GetPositionByPoint(iWMetrics::GetCursorPos(m_hWnd));
	m_AcceptDrop = m_pDragDropHnd->iTree_DropQuery(this, m_pDragSender, m_DropItems, ndpos);
	if (ndpos != m_DropPos)
	{
		m_DropPos = ndpos;
		m_pDragSender->m_DragList.DragShowNolock(FALSE);
		ForceRepaint();
		m_pDragSender->m_DragList.DragShowNolock(TRUE);
	}

	return m_AcceptDrop;
}

// End drag
void iTreeCtrl::EndDrag(bool cancel)
{
	iLOG(_T("- End"));
	ReleaseCapture();
	m_DragAnchor = cInvalidPoint;
	m_Dragging = false;
	m_DragList.EndDrag();
	if (m_DropTarget)
	{
		if (!cancel) 
		{
			// Send Drop Message
			m_DropTarget->Drop();
			iLOG(_T("- Drop"));
		}
		iLOG(_T("- Leave"));
		m_DropTarget->DropLeave();
		m_DropTarget = 0;
	}
	ResetSelection();
}

/*
 *	Helpers
 */


// Retreive height of all items
uint32 iTreeCtrl::GetTotalHeight()
{
	if (!m_pRootNode || !m_pRootNode->Count()) return 0;

	iTreeNode* node = *m_pRootNode->First();
	uint32 height = 0;

	while (node)
	{
		height += m_pComposerHnd->iTree_GetItemHeight(this,node);
		node = node->WalkNextOpened();
	}

	return height;
}

// Get last opened item (if no items, then returns NULL)
iTreeNode* iTreeCtrl::GetLastOpenedItem()
{
	if (!m_pRootNode || !m_pRootNode->Count()) return NULL;

	iTreeNode* node = *m_pRootNode->Last();

	while (node)
	{
		if (!node->Count() || !node->IsOpen()) return node;
		node = *node->Last();
	}

	check(0);
	return NULL;
}


// Set New root
iTreeNode* iTreeCtrl::SetRootNode(iTreeNode *nroot)
{
	check(*nroot->Parent() == NULL );
	nroot->SetOpen(true);

	iTreeNode* old_root = m_pRootNode;
	m_pRootNode = nroot;

	return old_root;
}

//
iTreeNode* iTreeCtrl::GetNodeByPoint(const iPoint &ipnt)
{
	if (!m_pRootNode || !m_pRootNode->Count()) return 0;
	iTreeNode* node = *m_pRootNode->First();
	sint32 vPos = -GetScrollPos(SB_VERT);
	sint32 vSize;

	while (node)
	{
		vSize = m_pComposerHnd->iTree_GetItemHeight(this,node);

		if ( ipnt.y >= vPos && ipnt.y < (vPos+vSize) ) return node;

		vPos += vSize;
		node = node->WalkNextOpened();
	}

	return NULL;
}

// Calculate drop position
iTreeCtrl::iDropPosition iTreeCtrl::GetPositionByPoint(const iPoint &ipnt)
{
	iDropPosition dpos;
	if (!m_pRootNode || !m_pRootNode->Count()) return dpos;

	iRect clrect = iWMetrics::GetClRect(m_hWnd);
	if (!clrect.PtInRect(ipnt)) return dpos;

	iTreeNode* node = *m_pRootNode->First();
	sint32 vPos = -GetScrollPos(SB_VERT);
	sint32 vSize;
	sint32 vOffs;
	sint32 vthq;
	bool bFound = false;

	while (node)
	{
		vSize = m_pComposerHnd->iTree_GetItemHeight(this,node);

		if ( vPos <= ipnt.y &&  (vPos+vSize) >= ipnt.y )
		{
			vOffs = ipnt.y - vPos;
			vthq = vSize/4;
			bFound = true;
			break;
		}

		vPos += vSize;
		node = node->WalkNextOpened();
	}

	if (!bFound)
	{
		dpos.pType = iDropPosition::PosEnd;
	}
	else if ( vOffs <= vthq)
	{
		dpos.pType = iDropPosition::PosBefore;
		dpos.pNode = node;
	}
	else if ( vOffs >= vthq*3 )
	{
		iTreeNode* tNode = node->WalkNextOpened();

		if (!tNode)
		{
			dpos.pType = iDropPosition::PosEnd;
		}
		else
		{
			dpos.pType = iDropPosition::PosBefore;
			dpos.pNode = tNode;
		}
	}
	else
	{
		dpos.pType = iDropPosition::PosInto;
		dpos.pNode = node;
	}
	return dpos;
}

// Get container of currently visible items (returns height of all visible items)
uint32 iTreeCtrl::GetVisItems(iNodeArray &tna, iRectArray &rca, const iSize &clsiz)
{
	if (!m_pRootNode || !m_pRootNode->Count()) return 0;

	iTreeNode* node = *m_pRootNode->First();
	iRect clRect(clsiz);
	sint32 vPos = -GetScrollPos(SB_VERT);
	sint32 vSize;
	bool bFound = false;
	uint32 toth=0;

	while (node)
	{
		vSize = m_pComposerHnd->iTree_GetItemHeight(this,node);
		if ( clRect.PtInRect(0,vPos) || clRect.PtInRect(0,vPos+vSize))
		{
			tna.Add(node);
			uint32 offs = (node->GetDepth()-1)*TREE_LEVEL_STEP;
			rca.Add(iRect(offs,vPos,clsiz.w - offs,vSize));
			toth += vSize;
		}
		vPos += vSize;
		node = node->WalkNextOpened();
	}

	return toth;
}

void iTreeCtrl::ReparseOrderIdx()
{
	if (!m_pRootNode || !m_pRootNode->Count()) return;
	uint32 counter = 0;
	
	m_pRootNode->SetOrderIdx(counter++);
	
	iTreeNode::Iterator it = m_pRootNode->First();
	while (*it != NULL)
	{
		(*it)->SetOrderIdx(counter++);
		it = it.WalkNext();
	}
}

/*
 *	Selection
 */

// Normalize selecion array (sort slected items and delete invisible)
void iTreeCtrl::NormalizeSelection()
{
	if (!m_SelItems.GetSize()) return;
	ReparseOrderIdx();
	for (uint32 i=0; i < m_SelItems.GetSize(); i++){
		for (uint32 j=m_SelItems.GetSize()-1; j > 0; j--){
			if (m_SelItems[j-1]->GetOrderIdx() > m_SelItems[j]->GetOrderIdx()) iSwap(m_SelItems[j-1],m_SelItems[j]);
		}
	}
}


// Get Ammouunt of selected items ( 0 - if no selected items)
uint32 iTreeCtrl::GetSelCount()
{
	return m_SelItems.GetSize();
}

// Single and Multiple Selection
void iTreeCtrl::GetSelNodes(iNodeArray &sel)
{
	sel = m_SelItems;
}

// Reset selection
void iTreeCtrl::ResetSelection(bool bInvalidate)
{
	m_SelItems.RemoveAll();
	m_pEventsHnd->iTree_OnSelChanged(this,m_SelItems);
	if (bInvalidate) ForceRepaint();
}

void iTreeCtrl::ReplaceSelection(iTreeNode *nsel, bool bInvalidate)
{
	m_SelItems.RemoveAll();
	m_SelItems.Add(nsel);
	m_pEventsHnd->iTree_OnSelChanged(this,m_SelItems);
	if (bInvalidate) ForceRepaint();
}

void iTreeCtrl::ReplaceSelection(const iNodeArray &nsel, bool bInvalidate)
{
	m_SelItems = nsel;
	m_pEventsHnd->iTree_OnSelChanged(this,m_SelItems);
	if (bInvalidate) ForceRepaint();
}


/*
 *	Window messages handling
 */
LRESULT iTreeCtrl::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	if (wParam == 1)
	{
		DWORD dwPos = ::GetMessagePos();
		POINT pnt = {LOWORD (dwPos), HIWORD (dwPos)};
		ScreenToClient (&pnt);

		RECT clrect;
		GetClientRect(&clrect);
		int cy = clrect.bottom - clrect.top;

		if (pnt.y >= 0 && pnt.y <= 15) 
		{
			m_pDragSender->m_DragList.DragShowNolock (FALSE);
			SendMessage(WM_VSCROLL, MAKEWPARAM (SB_LINEUP, 0), NULL);
			m_pDragSender->m_DragList.DragShowNolock (TRUE);
		}
		else if (pnt.y >= (cy - 15) && pnt.y <= cy) 
		{
			m_pDragSender->m_DragList.DragShowNolock (FALSE);
			SendMessage(WM_VSCROLL, MAKEWPARAM (SB_LINEDOWN, 0), NULL);
			m_pDragSender->m_DragList.DragShowNolock (TRUE);
		}
	}

	return 0;
}

LRESULT iTreeCtrl::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	SetFocus();
	iPoint pnt(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));

	iTreeNode* iNode = GetNodeByPoint(pnt);

	if (iNode)
	{
		sint32 offs = (iNode->GetDepth() - 1)*TREE_LEVEL_STEP;

		if (pnt.x > offs && pnt.x < (offs+10))
		{
			iNode->ToggleOpen();
			ReplaceSelection(iNode);
		}
		else
		{
			if (m_SelItems.Find(iNode) == -1)
			{
				ReplaceSelection(iNode);
			}
		}
	}
	else
	{
		m_SelItems.RemoveAll();
		m_pEventsHnd->iTree_OnSelChanged(this,m_SelItems);
		ForceRepaint();
	}

	m_DragAnchor = pnt;

	POINT pt = {GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)};
	ClientToScreen(&pt);
	m_pEventsHnd->iTree_OnRButtonDown(this,m_SelItems,iPoint(pt));


	return 0;
}

LRESULT iTreeCtrl::OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	POINT pt = {GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)};
	ClientToScreen(&pt);

	m_pEventsHnd->iTree_OnRButtonUp(this,m_SelItems,iPoint(pt));
	return 0;
}

LRESULT iTreeCtrl::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	POINT pt = {GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)};

	if (m_CanDrag && 
		!m_Dragging && 
		((wParam & MK_LBUTTON) && m_DragAnchor != cInvalidPoint && m_DragAnchor.GetDelta(pt) >= TREE_DRAG_DELTA) &&
		DragQuery())
	{
		iLOG(_T("- Begin"));
		NormalizeSelection();
		m_Dragging = true;
		m_DragList.SetDragCursorImage(0, 32, 32);
		m_DragList.BeginDrag(0,32,32);
		m_DragList.DragEnter(GetDesktopWindow(),0,0);
		SetCapture();
	}

	if (m_Dragging)
	{
		ClientToScreen(&pt);
		HWND fwnd = ::WindowFromPoint(pt);


		if (m_DropTarget && fwnd != m_DropTarget->m_hWnd)
		{
			iLOG(_T("- Leave"));
			m_DropTarget->DropLeave();
			m_DropTarget = NULL;
		}

		if (fwnd && (!m_DropTarget || (m_DropTarget && fwnd != m_DropTarget->m_hWnd)) && IsAcceptableTarget(fwnd))
		{
			iLOG(_T("- Enter"));
			m_DropTarget = FindTarget(fwnd);
			m_DropTarget->DropEnter(this);
		}

		if (m_DropTarget)
		{
			bool bRes = m_DropTarget->DropQuery();
			m_DragList.DragMove(pt);

			if (bRes) m_DragList.SetDragCursorImage(0, 32, 32);
			else m_DragList.SetDragCursorImage(1, 32, 32);
		}
		else 
		{
			m_DragList.DragMove(pt);
			m_DragList.SetDragCursorImage(1, 32, 32);
		}
	}


	return 0;
}

LRESULT iTreeCtrl::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	if (m_Dragging)
	{
		EndDrag(false);
	}
	else
	{
		iPoint pnt(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));

		iTreeNode* iNode = GetNodeByPoint(pnt);

		if (iNode && m_SelItems.Find(iNode) != -1 && !(wParam & MK_CONTROL) && !(wParam & MK_SHIFT))
		{
			ReplaceSelection(iNode);
		}
	}

	bHandler = FALSE;
	
	return 0;
}

LRESULT iTreeCtrl::OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	iTreeNode* iNode = GetNodeByPoint(iPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)));
	if (iNode) {
		iNode->ToggleOpen();
		ReplaceSelection(iNode,false);
		m_pEventsHnd->iTree_OnLDblClick(this,m_SelItems);
		UpdateTreeCtrl();
	} else {
		ResetSelection();
		m_pEventsHnd->iTree_OnLDblClick(this,m_SelItems);
	}

	return 0;
}

LRESULT iTreeCtrl::OnRButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	iTreeNode* iNode = GetNodeByPoint(iPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)));
	if (iNode) {
		ReplaceSelection(iNode,false);
		m_pEventsHnd->iTree_OnRDblClick(this,m_SelItems);
		UpdateTreeCtrl();
	} else {
		ResetSelection();
		m_pEventsHnd->iTree_OnRDblClick(this,m_SelItems);
	}

	return 0;
}

LRESULT iTreeCtrl::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	SetFocus();
	iPoint pnt(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
	iTreeNode* iNode = GetNodeByPoint(pnt);

	if (iNode) {
		sint32 offs = (iNode->GetDepth()-1) * TREE_LEVEL_STEP;

		if (m_MultiSel &&  wParam & MK_CONTROL)	{
			if (m_SelItems.Find(iNode) == -1) m_SelItems.Add(iNode);
			else m_SelItems.Remove(iNode);
			m_pEventsHnd->iTree_OnSelChanged(this,m_SelItems);
			ForceRepaint();
		} else if (m_MultiSel &&  wParam & MK_SHIFT)	{
			if (m_SelItems.GetSize() == 0) {
				m_SelItems.Add(iNode);
			} else if (!IsSiblings(m_SelItems) || !IsSiblings(iNode,m_SelItems[0])) {
				m_SelItems.RemoveAll();
				m_SelItems.Add(iNode);
			} else {
				NormalizeSelection();
				iTreeNode *bgn_Node = m_SelItems[0];
				iTreeNode *end_Node = iNode;
				
				if (iNode->GetOrderIdx() < m_SelItems[0]->GetOrderIdx()) {
					bgn_Node = iNode;
					end_Node = m_SelItems[m_SelItems.GetSize()-1];
				}

				m_SelItems.RemoveAll();
				check(bgn_Node);
				check(end_Node);

				m_SelItems.Add(end_Node);
				for (It it = It(bgn_Node); it != It(end_Node); ++it){
					m_SelItems.Add(*it);
				}
			}
			m_pEventsHnd->iTree_OnSelChanged(this,m_SelItems);
			ForceRepaint();
		} else {
			if (pnt.x > offs && pnt.x < (offs+10)) {
				iNode->ToggleOpen();
				ReplaceSelection(iNode);
				UpdateScrollBar(iWMetrics::GetClSize(m_hWnd));
			} else {
				if (m_SelItems.Find(iNode) == -1) {
					ReplaceSelection(iNode);
					m_pEventsHnd->iTree_OnSelChanged(this,m_SelItems);
				}
			}
		}
	} else {
		ResetSelection();
	}

	m_DragAnchor = pnt;

	return 0;
}

LRESULT iTreeCtrl::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	UpdateScrollBar(iSize(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)));
	return 0;
}

LRESULT iTreeCtrl::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

	if (zDelta < 0) SendMessage (WM_VSCROLL, MAKEWPARAM (SB_LINEDOWN, 0), NULL);
	else if (zDelta > 0) SendMessage (WM_VSCROLL, MAKEWPARAM (SB_LINEUP, 0), NULL);

	return 0;
}

LRESULT iTreeCtrl::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	if (m_Dragging && wParam == VK_ESCAPE) {
		EndDrag(true);
	} else if (!m_Dragging && (wParam == VK_DOWN || wParam == VK_UP)) {
		iTreeNode* iNode;
		if (m_SelItems.GetSize() > 0) iNode = m_SelItems[0];
		else iNode = *m_pRootNode->First();

		m_SelItems.RemoveAll();

		if (wParam == VK_UP) {
			iTreeNode* pNode = iNode->WalkPrevOpened();
			if (pNode) m_SelItems.Add(pNode);
			else m_SelItems.Add(iNode);
		} else if (wParam == VK_DOWN) {
			iTreeNode* pNode = iNode->WalkNextOpened();
			if (pNode) m_SelItems.Add(pNode);
			else m_SelItems.Add(iNode);
		}
		m_pEventsHnd->iTree_OnSelChanged(this,m_SelItems);
		ForceRepaint();
	} else if (!m_Dragging && wParam == VK_RIGHT || wParam == VK_LEFT || wParam == VK_SUBTRACT || wParam == VK_ADD) {
		if (m_SelItems.GetSize()) 
		{
			iTreeNode* iNode = m_SelItems[0];
			if (iNode->Count() 
				&& ((iNode->IsOpen() && (wParam == VK_LEFT || wParam == VK_SUBTRACT)) 
					|| (!iNode->IsOpen() && (wParam == VK_RIGHT || wParam == VK_ADD))))
			{
				iNode->ToggleOpen();
				ReplaceSelection(iNode);
			}
			else if (*iNode->Parent() != m_pRootNode )
			{
				ReplaceSelection(*iNode->Parent());
			}
		}
		else ReplaceSelection(*m_pRootNode->First());
	}

	return 0;
}

/*
 *	Scrollbars
 */

void iTreeCtrl::UpdateScrollBar(iSize &clsiz)
{
	// Setup scrollbar settings
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE|SIF_RANGE;
	si.nMin = 0;
	si.nMax = GetTotalHeight();
	si.nPage = clsiz.h;
	SetScrollInfo(SB_VERT,&si);
}

LRESULT iTreeCtrl::OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	iSize clsize=iWMetrics::GetClSize(m_hWnd);
	sint32 totalh = GetTotalHeight();

	sint32 yNewPos;
	sint32 yCurPos = GetScrollPos(SB_VERT);
	
	switch(LOWORD(wParam))
	{
		case SB_PAGEUP:
			yNewPos = yCurPos - 200;
			break;

		case SB_PAGEDOWN:
			yNewPos = yCurPos + 200;
			break;

		case SB_LINEUP:
			yNewPos = yCurPos - 20;
			break;

		case SB_LINEDOWN:
			yNewPos = yCurPos + 20;
			break;

		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			yNewPos = HIWORD(wParam);
			break;

		default:
			yNewPos = yCurPos;
	}

	yNewPos = iMAX(sint32(0),yNewPos);
	yNewPos = iMIN(sint32(totalh-clsize.h),yNewPos);

	if (yNewPos != yCurPos) {
		SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_POS;
		si.nPos = yNewPos;
		SetScrollInfo(SB_VERT,&si,TRUE);

		ForceRepaint();
	}

	return 0;
}

/*
 *	Painting
 */

LRESULT iTreeCtrl::OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	return 0;
}

LRESULT iTreeCtrl::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	PAINTSTRUCT ps;
	iDC dc = BeginPaint(&ps);

	MsgPaint(dc,NULL,iRect(ps.rcPaint));

	EndPaint(&ps);
	bHandler=TRUE;
	
	return 0;
}

void iTreeCtrl::UpdateTreeCtrl()
{
	UpdateScrollBar(iWMetrics::GetClSize(m_hWnd));
	ForceRepaint();
}

void iTreeCtrl::ForceRepaint()
{
	if (IsWindow()){
		HDC hdc = GetDC();
		MsgPaint(hdc);
		ReleaseDC(hdc);
	}
}

void iTreeCtrl::MsgPaint(HDC dc, iTreeNode* pNode, const iRect &prc)
{
	iNodeArray visNodes;
	iRectArray visRects;
	iSize clsiz=iWMetrics::GetClSize(m_hWnd);

	uint32 dh = GetVisItems(visNodes,visRects,clsiz);

	//recreate dib if required
	iSize newSize((clsiz.w/TREE_DIB_ALLOCATION_STEP+2)*TREE_DIB_ALLOCATION_STEP,iMAX(dh,((clsiz.h/TREE_DIB_ALLOCATION_STEP+2)*TREE_DIB_ALLOCATION_STEP)));
	if (m_OutMemDC.GetSize() != newSize){
		m_OutMemDC.Resize(newSize);
	}

	// SelectFont
	//HGDIOBJ hFont = (HGDIOBJ)GetStockObject(DEFAULT_GUI_FONT);
	HGDIOBJ hOldFont = ::SelectObject(m_OutMemDC,m_nrmFont);
	m_OutMemDC.SetTextAlignment(TA_LEFT);
	m_OutMemDC.SetTextBkMode(TRANSPARENT);

	
	// Draw Elements
	if (!m_pComposerHnd->iTree_ComposeBkgnd( this, m_OutMemDC, iWMetrics::GetClRect(m_hWnd))){
		m_OutMemDC.m_Dib.Fill(m_clrBkgnd);
	}

	for (uint32 xx=0; xx<visNodes.GetSize(); ++xx){
		m_pComposerHnd->iTree_ComposeItem(this,m_OutMemDC,visRects[xx],visNodes[xx], (m_SelItems.Find(visNodes[xx])!=-1));
	}

	// Draw drop mark
	if (m_ForceDrop && m_AcceptDrop && m_DropPos.pType != iDropPosition::PosInvalid){
		if (m_DropPos.pType == iDropPosition::PosEnd){
			iRect trc(0,dh-2,clsiz.w-1,4);
			m_OutMemDC.FillInvRect(trc,1);
			trc.DeflateRect(1);
			m_OutMemDC.m_Dib.FillRect(trc,iColor(0,255,0,32),BLEND_ALPHABLEND);
		} else if (m_DropPos.pType == iDropPosition::PosInto){
			sint32 idx = visNodes.Find(m_DropPos.pNode);
			if (idx != -1) {
				iRect trc(visRects[idx]);
				m_OutMemDC.FillInvRect(trc,1);
				trc.DeflateRect(1);
				m_OutMemDC.m_Dib.FillRect(trc,iColor(0,255,0,32),BLEND_ALPHABLEND);
			}
		} else if (m_DropPos.pType == iDropPosition::PosBefore) {
			sint32 idx = visNodes.Find(m_DropPos.pNode);
			if (idx != -1){
				iRect trc;
				if (idx == 0) trc = iRect(0,0,clsiz.w,4);
				else trc = iRect(iMIN(visRects[idx].x,visRects[idx-1].x),visRects[idx].y-2,iMAX(visRects[idx].w,visRects[idx-1].w),4);
				m_OutMemDC.FillInvRect(trc,1);
				trc.DeflateRect(1);
				m_OutMemDC.m_Dib.FillRect(trc,iColor(0,255,0,32),BLEND_ALPHABLEND);
			}
		}
	}

	// Select old font
	::SelectObject(m_OutMemDC,hOldFont);

	// Blit dib to DC
	iRect dib_rc(prc);
	iPoint out_pnt(prc.x,prc.y);
	if (prc == cInvalidRect) {
		RECT rect;
		GetClientRect(&rect);
		dib_rc = iRect(rect);
		out_pnt = iPoint(rect.left,rect.top);
	}

	//dib_rc.y += dcoffs;
	::BitBlt(dc,out_pnt.x,out_pnt.y,dib_rc.w,dib_rc.h,m_OutMemDC,dib_rc.x,dib_rc.y,SRCCOPY);
}
