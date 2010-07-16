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

#ifndef _HMM_GAME_ARMY_DRAG_N_DROP_LIST_H_
#define _HMM_GAME_ARMY_DRAG_N_DROP_LIST_H_

/*
 *	Extendet Army List control (with drag'n'drop capabilities)
 */
class iArmyListEx : public iBaseCtrl
{
public:
	struct iDragItem : public iViewMgr::IDragGlyph {
		iDragItem(iArmyListEx* pSender, sint8 fromCell, bool bSplit, const iCreatGroup& cGroup, const iPoint& pos, const iSize& siz, bool bCanDismiss);
		void ComposeDragGlyph();
		
		bool			m_bCanDismiss;
		bool			m_bSplit;
		iSize			m_siz;
		iArmyListEx*	m_pSender;
		sint8			m_fromCell;
		iArmyListEx*	m_pReceiver;
		sint8			m_toCell;
		iCreatGroup		m_cGroup;
		iPoint			m_pos;
	};

public:
	iArmyListEx(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iPoint& pos, iCheckButton* pSplitBtn, const iSize& itemSiz, uint32 uid);

	// Drag'n'Drop
	void AddCompetitor(iArmyListEx* pCompetitor);
	void BeginDrag(sint8 fromCell, iCreatGroup& cGroup, const iPoint& pos);
	void EndDrag();
	void Drag(const iPoint& pos);
	void EnterDrop(iDragItem* pDragItem);
	void LeaveDrop();
	bool Drop();
	bool DragDrop() const;

	//
	void SetSplitMode(bool bSplit);
	void ToggleSplitMode();
	inline bool IsSplitMode() const { return m_bSplit; }
	void SetArmy(iArmy* pArmy, iHero* pOwner, bool bCanDismiss);
	inline iArmy* Army() { return m_pArmy; }

private:
	void OnCompose();
	void OnMouseDown(const iPoint& pos);
	void OnMouseUp(const iPoint& pos);
	void OnMouseClick(const iPoint& pos);
	void OnMouseTrack(const iPoint& pos);

	inline sint8 Pos2Cell(const iPoint& pos) const { return iMIN<sint8>((sint8)((pos.x-GetScrRect().x) / m_itemW),6);}

private:
	typedef iSimpleArray<iArmyListEx*> iCompList;

	bool			m_bCanDismiss;
	bool			m_bSplit;
	sint8			m_dCell;
	iArmy*			m_pArmy;
	iHero*			m_pOwner;
	sint32			m_itemW;
	uint32			m_lastClick;

	iPoint			m_dragAnchor;
	iDragItem*		m_pDragItem;
	iDragItem*		m_pDropItem;
	iCompList		m_Competitors;
	iCheckButton*	m_pSplitBtn;
};


#endif //_HMM_GAME_ARMY_DRAG_N_DROP_LIST_H_

