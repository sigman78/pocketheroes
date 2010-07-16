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

#ifndef _HMM_LANGUAGE_EDITOR_ITEM_LIST_CONTROL_H_
#define _HMM_LANGUAGE_EDITOR_ITEM_LIST_CONTROL_H_

#include "LngMgr.h"

//////////////////////////////////////////////////////////////////////////
class CItemListCtrl : public iBaseChildDialog, iTreeCtrl::iEvents, iTreeCtrl::iComposer, iTreeCtrl::iDragDrop
{
public:
	iLngMgr		m_lngMgr;
	iTreeCtrl	m_TreeCtrl;

    BEGIN_MSG_MAP(CItemListCtrl)
		CHAIN_MSG_MAP(iBaseChildDialog)
    END_MSG_MAP()

	CItemListCtrl()
	{ }

	HWND CreateItemList(HWND parent)
	{
	if (!CreateBaseChildDlg(parent, iRect(0,0,1,1))) return NULL;
		m_TreeCtrl.CreateTreeCtrl( m_hWnd, RECT(iRect(0,0,1,1)), 7777,0, this,this,this);
		m_lngMgr.ReadOldLngFile(L"C:\\DEV\\Projects\\WCE\\pheroes\\bin\\Resources\\language.dat", true);
		m_TreeCtrl.SetRootNode(&m_lngMgr);

		return m_hWnd;
	}

	void MsgSize(iSize &nsize)
	{ if (m_TreeCtrl.IsWindow()) m_TreeCtrl.MoveWindow(3,3,nsize.w-6,nsize.h-6); }

	void iTree_OnSelChanged( iTreeCtrl* sender, iNodeArray &sel)
	{
		/*
		if (!sel.GetSize()){
			m_pSelHandler->OnSelChanged(NULL);
		} else {
			m_pSelHandler->OnSelChanged((iBaseNode*)sel[0]);
		}*/
	}

	void iTree_OnRButtonUp( iTreeCtrl* sender, iNodeArray &sel, const iPoint &mpos){}
	void iTree_OnRButtonDown( iTreeCtrl* sender, iNodeArray &sel, const iPoint &mpos){}
	void iTree_OnLDblClick( iTreeCtrl* sender, iNodeArray &sel){}
	void iTree_OnRDblClick( iTreeCtrl* sender, iNodeArray &sel){}

	void iTree_ComposeItem( iTreeCtrl* sender, iMemoryDC &memdc, const iRect& irect, iTreeNode* node, bool bSel )
	{
		if (bSel){
			memdc.m_Dib.FillRect(irect, iColor(192,220,255,255));
			iDibPrimitives::DrawFrame(memdc.m_Dib,irect,iColor(128,160,255,255),BLEND_SRCCOPY);
		} else 	{
			memdc.m_Dib.FillRect(irect, cColorWhite);
		}
		iRect text_rc(irect.x+5,irect.y+2,irect.w-10,irect.h-4);

		iLngMgr::iBaseLngNode* pBaseNode = (iLngMgr::iBaseLngNode*)node;
		if (pBaseNode->nodeType == iLngMgr::iBaseLngNode::EntryNode) {
			iLngMgr::iLngEntry* pLngEntry = (iLngMgr::iLngEntry*)node;
			memdc.DrawTextRect(pLngEntry->origText,text_rc,true);
		} else if (pBaseNode->nodeType == iLngMgr::iBaseLngNode::GroupNode) {
		} else {
			check (0 == "Invalid node type!");
		}
	}

	uint32 iTree_GetItemHeight( iTreeCtrl* sender, iTreeNode* node )
	{
		return 20;
	}

	void Update()
	{
		m_TreeCtrl.ForceRepaint();
	}

	bool iTree_DragQuery(iTreeCtrl* sender, const iNodeArray &sel) { return false; }
	bool iTree_DropQuery(iTreeCtrl* sender, iTreeCtrl* drag_from, const iNodeArray &ditems, const iTreeCtrl::iDropPosition &ndpos) { return false;}
	bool iTree_Drop(iTreeCtrl* sender, iTreeCtrl* drag_from, const iNodeArray &ditems, const iTreeCtrl::iDropPosition &ndpos) { return false; }


};

#endif //_HMM_LANGUAGE_EDITOR_ITEM_LIST_CONTROL_H_