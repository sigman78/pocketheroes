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

#ifndef _HMM_MAP_EDITOR_MINIMAP_VIEW_H_
#define _HMM_MAP_EDITOR_MINIMAP_VIEW_H_

class CMapEditorView;
class iMinimapView :  public CWindowImpl<iMinimapView>, public iMapHandler::iSubs
{
public:
	DECLARE_WND_CLASS(NULL)

	iMinimapView();

	HWND CreateMinimapView(HWND hParent, CMapEditorView* pMainView);

	BEGIN_MSG_MAP(iMinimapView)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	END_MSG_MAP()

	void SetMap(iMapHandler* pMap);
	void OnMapContentChanged();
	static void PrepareMap(iMapHandler* pMap, iDib& odib, bool bShowCnsts);
	void UpdateSelFrame(const iPoint& lt, const iPoint& rb);


private:
	void PrepareMap();
	void Compose(HDC hdc);
	void ScrollMap(iPoint pos);

private:
	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) { return 0; }

private:
	iPoint			m_frameLT, m_frameRB;
	iMapHandler*	m_pMap;
	iMemoryDC		m_outBuff;
	iDib			m_dibMap;
	CMapEditorView*	m_pMainView;
	bool			m_bTrack;
};

#endif //_HMM_MAP_EDITOR_MINIMAP_VIEW_H_