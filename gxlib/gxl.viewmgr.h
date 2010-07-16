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

#ifndef _GXLIB_VIEW_MANAGER_H_
#define _GXLIB_VIEW_MANAGER_H_

class iGXApp;
class iTopmostView;
class iPopupView;

class iViewMgr
{
public:
	struct IDragGlyph {
		virtual void ComposeDragGlyph() =0;
	};

public:
	iViewMgr(iGXApp* pApp);
	~iViewMgr();

	bool ProcessMessage(const iInput::iEntry& msg);
	void Compose(iRect& rect);

	inline iGXApp* App() { return m_pApp; }
	inline iDib& Surface() { return m_Surf; }
	inline iSize Metrics() { return m_Surf.GetSize(); }

	void SetCurView(iTopmostView* pCurView);
	inline iView* CurView() { return (iView*)m_pCurView; }

	// Capture view
	void SetViewCapture(iView* pCapView);
	iView* ReleaseViewCapture();
	inline iView* CapturedView() { return m_pCapView; };

	// Modal stack
	inline bool HasModalDlg() const { return m_dlgStack.GetSize()>0; }
	void PushModalDlg(iDialog* pDlg);
	iDialog* PopModalDlg();

	// Popup windows
	void TrackPopup(iPopupView* pPopupView, const iPoint& pos, const iRect& bound, Alignment al);
	void HidePopup();

	// Drag'n'Drop glyph
	inline void SetDragGlyph(IDragGlyph* pDragGlyph) { m_pDragGlyph = pDragGlyph; }
	inline IDragGlyph* DragGlyph() { return m_pDragGlyph; }

	// Timer processing
	void Process(uint32 interval);
	void SetTimer(uint32 period, uint32 tid, iView* pHandler);
	void CleanupTimers(iView* pHandler = NULL);

private:
	iDib&				m_Surf;
	iGXApp*				m_pApp;
	iTopmostView*		m_pCurView;
	iView*				m_pCapView;
	IDragGlyph*			m_pDragGlyph;
	iPopupView*			m_pPopupView;

	typedef iSimpleArray<iDialog*>	iDialogStack;
	iDialogStack	m_dlgStack;

private:
	struct iTimerHandler {
		iTimerHandler(uint32 _timer, uint32 _tid, iView* _pHandler) : timer(_timer), tid(_tid), pHandler(_pHandler) {}
		uint32	timer;
		uint32	tid;
		iView*	pHandler;
	};
	typedef iSimpleArray<iTimerHandler>	iTimerList;
	iTimerList	m_timers;
	uint32		m_timerCounter;
};

#endif //_GXLIB_VIEW_MANAGER_H_

