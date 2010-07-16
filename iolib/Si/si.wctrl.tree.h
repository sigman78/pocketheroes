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
 *	@brief Tree control class implementation
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_WCTRL_TREE_CONTROL_H_
#define _IOLIB_SILICON_WCTRL_TREE_CONTROL_H_

#include "../he/he.container.tree.h"
#include "../xe/xe.std.h"
#include "../xe/xe.memorydc.h"
#include "../pa/pa.autovalue.h"
#include <atlctrls.h>

#ifndef WM_MOUSEWHEEL
#	define WM_MOUSEWHEEL WM_MOUSELAST+1
#endif

/// Tree node class 
class iTreeNode : public iTree<iTreeNode>
{
public:
	/// Destructor
	virtual ~iTreeNode(){}

	/// Constructs tree node with specified parent
	iTreeNode(iTreeNode *parent=NULL)
	{ SetParent(parent); }

	/// Returns - is node opened or not
	inline bool IsOpen() const
	{ return m_IsOpen; }

	/// Sets open state
	inline void SetOpen( bool nState = true)
	{ m_IsOpen = nState; }

	/// Toggles open state
	inline bool ToggleOpen()
	{ return (m_IsOpen = !m_IsOpen); }

	/// Gets order index
	inline uint32 GetOrderIdx() const
	{ return m_OrdIdx; }

	/// Sets order index
	inline void SetOrderIdx(uint32 oidx)
	{ m_OrdIdx = oidx; }

	/// Finds and returns previous opened node
	iTreeNode* WalkPrevOpened(iTreeNode* endNode = NULL);
	/// Finds and returns next opened node
	iTreeNode* WalkNextOpened();

private:
	AutoVal<bool>	m_IsOpen;
	AutoVal<uint32>	m_OrdIdx;
};

typedef iSimpleArray<iTreeNode*>	iNodeArray;

/// Returns - are these two nodes siblings or not
inline bool IsSiblings(iTreeNode* pNode1, iTreeNode* pNode2)
{
	if (pNode1 != NULL && *pNode1->Parent() == *pNode2->Parent()) return true;
	return false;
}

/// Returns - are these nodes siblings or not
inline bool IsSiblings(const iNodeArray& nodes)
{
	check(nodes.GetSize() != 0);
	iTreeNode *ref_parent = NULL;
	for (uint32 xx=0; xx < nodes.GetSize(); ++xx) {
		if (nodes[xx] == NULL) return false;
		if (ref_parent == NULL) ref_parent = *nodes[xx]->Parent();
		else if (ref_parent != *nodes[xx]->Parent()) return false;
	}
	return true;
}

/// Returns - are these nodes neighbourhoods or not
inline bool IsNeighbourhood(const iNodeArray& nodes)
{
	uint32 sneigh_cnt = 0;
	for (uint32 xx=0; xx<nodes.GetSize(); ++xx){
		if (nodes.Find(nodes[xx]->GetPrevPtr()) == -1) ++sneigh_cnt;
		if (nodes.Find(nodes[xx]->GetNextPtr()) == -1) ++sneigh_cnt;
		if (sneigh_cnt > 2)	return false;
	}
	return true;
}

//
//
//
const uint32 TREE_INVALID_ITEM = 0xffffffff;

const uint32 TREE_LEVEL_STEP = 15;
const uint32 TREE_DIB_ALLOCATION_STEP = 100;
const uint32 TREE_DRAG_DELTA = 15;

#ifndef GET_WHEEL_DELTA_WPARAM
#	define GET_WHEEL_DELTA_WPARAM(wParam)  ((short)HIWORD(wParam))
#endif //GET_WHEEL_DELTA_WPARAM

//
// Common color constants
//
const iColor tclrBlackText = iColor(0,0,0,255);
const iColor tclrGrayText = iColor(128,128,128,255);
const iColor tclrBlackText_del = iColor(128,0,0,255);
const iColor tclrGrayText_del = iColor(255,128,128,255);

const uint32 TID_DRAGNDROP	= 6721;
const uint32 TINT_DRAGNDROP = 100;

typedef iSimpleArray<iRect> iRectArray;

//
// iTreeCtrl Configuration flags
//
const uint32 ITC_MULTISELECTION		= 0x00000001;
const uint32 ITC_CANDRAG			= 0x00000002;
const uint32 ITC_ACCEPTDROP			= 0x00000004;

/// Tree control class 
class iTreeCtrl : public CWindowImpl< iTreeCtrl >
{
public:
	DECLARE_WND_CLASS(NULL)
	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}

public:
	typedef iTreeNode::Iterator			It;
	typedef iTreeNode::ConstIterator	cIt;

	typedef iSimpleArray<iTreeCtrl*>	iDropTargetCtr;

public:
	/// Drop position descriptor class
	class iDropPosition
	{
	public:
		/// Drop position type enumeration
		enum PosType {
			PosInvalid = 0,	///< Invalid target (pNode = NULL)
			PosBefore,		///< Drop before pNode
			PosInto,		///< Drop into pNode
			PosEnd			///< Drop after all items (end) (pNode = NULL)
		};

		/// Constructs iDropPosition with given props
		iDropPosition(PosType _pType = PosInvalid, iTreeNode* _pNode = NULL)
		: pType(_pType)
		, pNode(_pNode)
		{}

		/// Equality operator
		bool operator == (const iDropPosition &dpos) const
		{
			return (pType == dpos.pType && pNode == dpos.pNode);
		}

		/// Inequality operator
		bool operator != (const iDropPosition &dpos) const
		{
			return ! operator==(dpos);
		}

		PosType		pType;
		iTreeNode*	pNode;
	};

	/// Tree control event handler interface
	class iEvents
	{
	public:
		/// Selection changing notification
		virtual void iTree_OnSelChanged( iTreeCtrl* sender, iNodeArray &sel) = NULL;
		/// Right mouse button up notification
		virtual void iTree_OnRButtonUp( iTreeCtrl* sender, iNodeArray &sel, const iPoint &mpos) = NULL;
		/// Right mouse button down notification
		virtual void iTree_OnRButtonDown( iTreeCtrl* sender, iNodeArray &sel, const iPoint &mpos) = NULL;
		/// Left mouse button double click notification
		virtual void iTree_OnLDblClick( iTreeCtrl* sender, iNodeArray &sel) = NULL;
		/// Right mouse button double click notification
		virtual void iTree_OnRDblClick( iTreeCtrl* sender, iNodeArray &sel) = NULL;
	};

	/// Tree control composer interface
	class iComposer
	{
	public:
		/// Overriding tree control background composing
		virtual bool iTree_ComposeBkgnd( iTreeCtrl* sender, iMemoryDC &memdc, const iRect& irect) { return false; }
		/// Overriding item's composing
		virtual void iTree_ComposeItem( iTreeCtrl* sender, iMemoryDC &memdc, const iRect& irect, iTreeNode* node, bool bSel ) = NULL;
		/// Overriding specified item's height
		virtual uint32 iTree_GetItemHeight( iTreeCtrl* sender, iTreeNode* node ) = NULL;
	};

	/// Tree control drag'n'drop handling interface
	class iDragDrop
	{
	public:
		/// Process drag query (if possible, then returns true, otherwise returns false)
		virtual bool iTree_DragQuery(iTreeCtrl* sender, const iNodeArray &sel) = NULL;
		/// Process drop query (if accept drop, then returns true, otherwise returns false)
		virtual bool iTree_DropQuery(iTreeCtrl* sender, iTreeCtrl* drag_from, const iNodeArray &ditems, const iDropPosition &ndpos) = NULL;
		/// Process drop (if dropped successfuly, then returns true, otherwise returns false)
		virtual bool iTree_Drop(iTreeCtrl* sender, iTreeCtrl* drag_from, const iNodeArray &ditems, const iDropPosition &ndpos) = NULL;
	};

public:
	/// Default constructor
	iTreeCtrl();
	/// Destructor
	~iTreeCtrl();
	/// Initializes Tree control with specified params
	bool CreateTreeCtrl(HWND parent, RECT &rect, uint32 cid, uint32 flags, iEvents* pEventsHnd, iComposer* pComposerHnd, iDragDrop* pDragDropHnd);

	//
	// Message map
	//
	BEGIN_MSG_MAP(iTreeCtrl)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		MESSAGE_HANDLER(WM_RBUTTONDBLCLK, OnRButtonDblClk)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_VSCROLL, OnVScroll)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	//
	// Drag'n'Drop handling
	//

	/// Setup Drag'n'Drop icons
	void SetDragDropIcons(const iDib& dibOk, const iDib& dibNo);

	/// Add Drop target
	void AddDropTarget(iTreeCtrl *ptarget);

	/// Remove Drop target
	void RemoveDropTarget(iTreeCtrl *ptarget);

	/// Remove All Drop targets
	void RemoveAllDropTargets();

	/// Is acceptable target
	bool IsAcceptableTarget(HWND hwnd);

	/// Returns iTreeCtrl pointer by window handle
	iTreeCtrl* FindTarget(HWND hwnd);

	/// Enter to control dropable area
	bool DropEnter(iTreeCtrl* sender);

	/// Exit to control dropable area
	void DropLeave();

	/// Process drag query (if possible, then returns true, otherwise returns false)
	bool DragQuery();

	/// Process drop (if dropped successfuly, then returns true, otherwise returns false)
	bool Drop();

	/// Process drop query (if accept drop, then returns true, otherwise returns false)
	bool DropQuery();

	/// End drag
	void EndDrag(bool cancel);

	//
	// Helpers and operations
	//

	/// Retreive height of all items
	uint32 GetTotalHeight();

	/// Get last opened item (if no items, then returns NULL)
	iTreeNode* GetLastOpenedItem();

	/// Returns pointer to tree node under specified point
	iTreeNode* GetNodeByPoint(const iPoint &ipnt);

	/// Calculate drop position
	iDropPosition GetPositionByPoint(const iPoint &pos);

	/// Get container of currently visible items (returns height of all visible items)
	uint32 GetVisItems(iNodeArray &tna, iRectArray &rca, const iSize &clsiz);

	/// Set New root
	iTreeNode* SetRootNode(iTreeNode *nroot);

	/// Get Root Node
	inline iTreeNode* GetRootNode() { return m_pRootNode; }

	/// Reparse order indexation
	void ReparseOrderIdx();

	//
	// Selection
	//

	/// Normalize selecion array (sort slected items and delete invisible)
	void NormalizeSelection();

	/// Get Ammouunt of selected items ( 0 - if no selected items)
	uint32 GetSelCount();

	/// Single and Multiple Selection
	void GetSelNodes(iNodeArray &sel);

	/// Reset selection
	void ResetSelection(bool bInvalidate = true);

	/// Replace selection
	void ReplaceSelection(iTreeNode *nsel, bool bInvalidate = true);
	/// Replace selection
	void ReplaceSelection(const iNodeArray &nsel, bool bInvalidate = true);

	//
	// Window messages handling
	//

	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnRButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);

	//
	// Scrollbars
	//

	/// Updates scrollbar
	void UpdateScrollBar(iSize &clsiz);
	LRESULT OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);

	//
	// Painting
	//

	/// Sets default background color
	void SetBkColor(const iColor& clr)
	{ m_clrBkgnd = clr; }
	LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	/// Updates (invalidates) tree control
	void UpdateTreeCtrl();
	/// Force repainting of control
	void ForceRepaint();
	void MsgPaint(HDC dc, iTreeNode* pNode = NULL, const iRect &prc = cInvalidRect);

	/// Gets normal (non-bold) font handle
	inline HFONT GetNormalFont() const
	{ return m_nrmFont; }

	/// Gets bold font handle
	inline HFONT GetBoldFont() const
	{ return m_bldFont; }

	/// Sets new normal (non-bold) font
	inline void ChangeNormalFont( HFONT newFont )
	{ DeleteObject(m_nrmFont); m_nrmFont = newFont; }

	/// Sets new bold font
	inline void ChangeBoldFont( HFONT newFont )
	{ DeleteObject(m_bldFont); m_bldFont = newFont; }

protected:

	// Colors
	iColor			m_clrBkgnd;
	// Members
	iEvents*		m_pEventsHnd;
	iComposer*		m_pComposerHnd;
	iDragDrop*		m_pDragDropHnd;
	iTreeNode*		m_pRootNode;
	iNodeArray		m_SelItems;

	iMemoryDC		m_OutMemDC;
	CImageList		m_DragList;

	HFONT			m_nrmFont;
	HFONT			m_bldFont;

	// Props
	bool			m_MultiSel;
	bool			m_CanDrag;

	// Drag
	iDropTargetCtr	m_DropTargets;	
	bool			m_Dragging;
	iPoint			m_DragAnchor;
	iTreeCtrl*		m_DropTarget;

	// Drop
	bool			m_ForceDrop;
	iTreeCtrl*		m_pDragSender;
	iNodeArray		m_DropItems;
	bool			m_AcceptDrop;
	iDropPosition	m_DropPos;
};

#endif //_IOLIB_SILICON_WCTRL_TREE_CONTROL_H_
/** @} */ // iolib_si
