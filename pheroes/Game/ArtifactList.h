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

#ifndef _HMM_GAME_ARTIFACT_LIST_CONTROLS_H_
#define _HMM_GAME_ARTIFACT_LIST_CONTROLS_H_

// Abstract artifacts Drag'n'Drop item
class iArtDragDropItem : public iBaseCtrl
{
public:
	struct iDragItem : public iViewMgr::IDragGlyph {
		iDragItem(iArtDragDropItem* pSender, sint32 fromCell,const iArtCell& artCell, const iPoint& pos, const iSize& siz);
		void ComposeDragGlyph();

		iSize				m_siz;
		iPoint				m_pos;
		iArtDragDropItem*	m_pSender;
		sint32				m_fromCell;
		iArtDragDropItem*	m_pReceiver;
		sint32				m_toCell;
		iArtCell			m_artCell;
	};
	
public:
	// c-tor, d-tor
	iArtDragDropItem(iSimpleArray<iArtDragDropItem*>& competitors, const iSize& cellSiz, iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid);
	virtual ~iArtDragDropItem() {}

	// Drag'n'Drop
	void BeginDrag(sint32 fromCell, const iArtCell& artCell, const iPoint& pos);
	void EndDrag();
	void Drag(const iPoint& pos);
	virtual void EnterDrop(iDragItem* pDragItem) =0;
	virtual void LeaveDrop()  =0;
	virtual bool Drop()  =0;
	virtual bool DragDrop() const  =0;
	virtual void Highlight(iDragItem *dr) =0;
	virtual void Unhighlight() =0;

	virtual iArtCell Remove(sint32 cell)  =0;
	virtual iArtCell Replace(sint32 cell, const iArtCell& newArt)  =0;

protected:
	iSimpleArray<iArtDragDropItem*>&	m_Competitors;
	iDragItem*							m_pDragItem;
	iDragItem*							m_pDropItem;
	iSize								m_cellSiz;
};

// Artifact item control
class iArtItemCtrl : public iArtDragDropItem
{
public:
	//
	iArtItemCtrl(iSimpleArray<iArtDragDropItem*>& competitors, iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iPoint& pos, const iSize& cellSiz, uint32 uid);

	// 
	inline void SetArtCell(iArtCell* pArtCell) { m_pArtCell = pArtCell; }
	inline const iArtCell* GetArtCell() const { return m_pArtCell; }
	inline void SetOwner(iHero* pOwner) { m_pOwner = pOwner; }
	inline const iHero* GetOwner() const { return m_pOwner; }

	//
	void EnterDrop(iDragItem* pDragItem);
	void LeaveDrop();
	bool Drop();
	bool DragDrop() const;
	void Highlight(iDragItem *dr);
	void Unhighlight();

	iArtCell Remove(sint32 cell);
	iArtCell Replace(sint32 cell, const iArtCell& newArt);

private:
	void OnCompose();
	void OnMouseDown(const iPoint& pos);
	void OnMouseUp(const iPoint& pos);
	void OnMouseTrack(const iPoint& pos);
	void OnMouseClick(const iPoint& pos);
	
private:
	bool		m_bClickMode;
	iHero*		m_pOwner;
	iArtCell*	m_pArtCell;
	iPoint		m_dragAnchor;
	bool		m_bPressed;
	bool        m_bHighlighted;
};

// Artifacts backpack control
class iArtBackPackCtrl : public iArtDragDropItem, public IViewCmdHandler
{
public:
	enum Flags {
		Vertical	= 0x0,
		Horizontal	= 0x1
	};
	
public:
	//
	iArtBackPackCtrl(iSimpleArray<iArtDragDropItem*>& competitors, iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iPoint& pos, const iSize& cellSiz, uint32 cellsCount, uint32 flags, uint32 uid);

	//
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

	//
	inline void SetBackPack(iArtList* pBackPack, iHero* pOwner) { m_pBackPack = pBackPack; m_pOwner = pOwner; }
	inline const iArtList* GetBackPack() const { return m_pBackPack; }

	//
	void EnterDrop(iDragItem* pDragItem);
	void LeaveDrop();
	bool Drop();
	bool DragDrop() const;
	inline void Highlight(iDragItem *dr) {};
	inline void Unhighlight() {};

	iArtCell Remove(sint32 cell);
	iArtCell Replace(sint32 cell, const iArtCell& newArt);

private:
	void SetListOffset(sint32 offs);
	void ListPageUp();
	void ListPageDown();
	sint32 CellByPos(const iPoint& pos) const;
	sint32 BackPackIdxByCell(sint32 cellIdx) const;
	void OnCompose();
	void OnMouseDown(const iPoint& pos);
	void OnMouseUp(const iPoint& pos);
	void OnMouseTrack(const iPoint& pos);
	void OnMouseClick(const iPoint& pos);

private:
	bool		m_bClickMode;
	sint32		m_dCell;
	iPoint		m_dragAnchor;
	sint32		m_listOffset;
	uint32		m_flags;
	uint32		m_cellsCount;
	iSize		m_cellSize;
	iArtList*	m_pBackPack;
	iHero*		m_pOwner;
};

// simple Artifact list control (w/o drag'n' drop)
class iArtListCtrl : public IViewCmdHandler, public iBaseCtrl
{
public:
	enum Flags {
		Vertical	= 0x0,
		Horizontal	= 0x1
	};
	
public:
	//
	iArtListCtrl(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iArtList& artList, const iPoint& pos, const iSize& cellSiz, uint32 cellsCount, uint32 flags, uint32 uid);

	//
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

private:
	void SetListOffset(sint32 offs);
	void ListPageUp();
	void ListPageDown();
	sint32 CellByPos(const iPoint& pos) const;
	sint32 BackPackIdxByCell(sint32 cellIdx) const;
	void OnCompose();
	void OnMouseDown(const iPoint& pos);
	void OnMouseUp(const iPoint& pos);
	void OnMouseClick(const iPoint& pos);

private:
	sint32			m_clickItem;
	bool			m_bClickMode;
	sint32			m_dCell;
	iPoint			m_dragAnchor;
	sint32			m_listOffset;
	uint32			m_flags;
	uint32			m_cellsCount;
	iSize			m_cellSize;
	const iArtList&	m_artList;
};

#endif //_HMM_GAME_ARTIFACT_LIST_CONTROLS_H_
