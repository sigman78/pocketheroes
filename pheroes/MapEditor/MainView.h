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

#ifndef _HMM_EDITOR_MAIN_VIEW_H_
#define _HMM_EDITOR_MAIN_VIEW_H_

class CMapEditorView : public CWindowImpl<CMapEditorView>, public iMapHandler::iSubs, public ISelHandler
{
private:
	bool spacePressed;
public:
	DECLARE_WND_CLASS(NULL)

	enum NavMode {
		NM_Edit = 0,
		NM_Surface,
		NM_Create,
		NM_Erase,
	};

public:
	CMapEditorView();
	void OnSelChanged(iBaseNode* pNode);
	bool SetSelPoint(const iPoint& npos);
	void CenterView(const iSize& clsiz);

	NavMode			m_NavMode;
	SURF_TYPE		m_selSurf;
	uint32			m_selBrush;

	iBaseNode*		m_pSelNode;
	iPoint			m_offset;
	bool			m_bMouseOnWindow;
	bool			m_bTracking;
	iPoint			m_SelPoint;
	iPoint			m_CompAnchor;
	iComposer		m_Composer;
	iMapHandler*	m_pMap;
	IStatusBar*		m_pStatusBar;
	iItemBrowser*	m_pItemBrowser;
	iMinimapView*	m_pMinimapView;

	HWND CreateMainView(HWND hParent, IStatusBar* pStatusBar, iItemBrowser* pItemBrowser, iMinimapView* pMinimapView);
	void OnMapContentChanged();

	void HandleDrawing(const iPoint& cell);
	void SetMap(iMapHandler* pMap);
	void UpdateScrollBars(const iSize &clsiz);
	void UpdateMiniMapFrame();

	BEGIN_MSG_MAP(CMapEditorView)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_KEYUP, OnKeyUp)		
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_VSCROLL, OnVScroll)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
	END_MSG_MAP()

	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDblClk(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnVScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnHScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) { return 0; }
	LRESULT CMapEditorView::OnKeyUp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT CMapEditorView::OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
};


#endif //_HMM_EDITOR_MAIN_VIEW_H_