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

#ifndef _HMM_EDITOR_CHILD_DIALOGS_H_
#define _HMM_EDITOR_CHILD_DIALOGS_H_

/*
 *	
 */
interface ISelHandler
{
	virtual void OnSelChanged(iBaseNode* pNode) = NULL;
};

/*
 *
 */
class iItemBrowser : public iBaseChildDialog, iTreeCtrl::iEvents, iTreeCtrl::iComposer, iTreeCtrl::iDragDrop
{
public:
	ISelHandler*	m_pSelHandler;
	iTreeCtrl		m_TreeCtrl;
	iDib			m_dibClp;
	iDib			m_dibExp;
	iDib			m_dibFolder;
	iDib			m_dibCtlCell_r;
	iDib			m_dibCtlCell_y;

	BEGIN_MSG_MAP(iItemBrowser)
		CHAIN_MSG_MAP(iBaseChildDialog)
	END_MSG_MAP()

	HWND CreateItemBrowser(HWND parent, ISelHandler* pSelHandler);

	void MsgSize(iSize &nsize)
	{ if (m_TreeCtrl.IsWindow()) m_TreeCtrl.MoveWindow(3,3,nsize.w-6,nsize.h-6); }

	void iTree_OnSelChanged( iTreeCtrl* sender, iNodeArray &sel)
	{
		if (!sel.GetSize()){
			m_pSelHandler->OnSelChanged(NULL);
		} else {
			m_pSelHandler->OnSelChanged((iBaseNode*)sel[0]);
		}
	}

	void iTree_OnRButtonUp( iTreeCtrl* sender, iNodeArray &sel, const iPoint &mpos){}
	void iTree_OnRButtonDown( iTreeCtrl* sender, iNodeArray &sel, const iPoint &mpos){}
	void iTree_OnLDblClick( iTreeCtrl* sender, iNodeArray &sel){}
	void iTree_OnRDblClick( iTreeCtrl* sender, iNodeArray &sel){}

	void ScaleDibToRect(const iDib* src, iDib& dst)
	{
		if (src->GetWidth() > dst.GetWidth() || src->GetHeight() > dst.GetHeight()){
			iSize nsiz(src->GetSize());
			ScaleSize(nsiz,dst.GetSize());
			iDib tdib(nsiz);
			iDibTransformer::Stretch(*src,tdib,iDibTransformer::High);
			tdib.CopyToDibXY(&dst,iPoint(dst.GetWidth()/2 - tdib.GetWidth()/2,dst.GetHeight()/2 - tdib.GetHeight()/2),BLEND_SRCCOPY);
		} else {
			src->CopyToDibXY(&dst,iPoint(dst.GetWidth()/2 - src->GetWidth()/2,dst.GetHeight()/2 - src->GetHeight()/2),BLEND_SRCCOPY);
		}
	}
	void iTree_ComposeItem( iTreeCtrl* sender, iMemoryDC &memdc, const iRect& irect, iTreeNode* node, bool bSel );

	uint32 iTree_GetItemHeight( iTreeCtrl* sender, iTreeNode* node )
	{
		if (DynamicCast<iGroupNode*>((iBaseNode*)node)) return 34;
		else if (DynamicCast<iPathElNode*>((iBaseNode*)node)) return 36;
		else return 32;
	}

	void Update()
	{
		m_TreeCtrl.ForceRepaint();
	}

	bool iTree_DragQuery(iTreeCtrl* sender, const iNodeArray &sel) { return false; }
	bool iTree_DropQuery(iTreeCtrl* sender, iTreeCtrl* drag_from, const iNodeArray &ditems, const iTreeCtrl::iDropPosition &ndpos) { return false;}
	bool iTree_Drop(iTreeCtrl* sender, iTreeCtrl* drag_from, const iNodeArray &ditems, const iTreeCtrl::iDropPosition &ndpos) { return false; }

private:
	void MakeObjectScheme(iVisualObject* pObj, iDib& dib);

};

#endif //_HMM_EDITOR_CHILD_DIALOGS_H_