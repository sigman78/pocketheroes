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

#include "gxl.inc.h"
#ifdef OS_WINCE
#include <gx.h>
#endif //OS_WINCE
#include "gxl.ctr.cbuff.h"
#include "gxl.ctr.array.h"
#include "gxl.window.h"
#include "gxl.input.h"
#include "gxl.timer.h"
#include "gxl.display.h"
#include "gxl.view.h"
#include "gxl.dialog.h"
#include "gxl.topmostview.h"
#include "gxl.popupview.h"
#include "gxl.viewmgr.h"
#include "gxl.application.h"

iViewMgr::iViewMgr(iGXApp* pApp)
: m_pApp(pApp), m_Surf(pApp->Display().GetSurface()), m_pCurView(NULL), m_pCapView(NULL), m_pPopupView(NULL), m_timerCounter(0) {}

iViewMgr::~iViewMgr()
{}

bool iViewMgr::ProcessMessage(const iInput::iEntry& msg)
{
	if (m_pPopupView) {
		// In case of popup view
		switch(msg.taskType) {
			case iInput::iEntry::MouseMove: if (m_pCapView) m_pCapView->MouseTrack(iPoint(msg.px,msg.py)); else m_pPopupView->MouseTrack(iPoint(msg.px,msg.py)); break;
			case iInput::iEntry::MouseDown: 
				if (m_pCapView) m_pCapView->MouseDown(iPoint(msg.px,msg.py)); 
				else {
					if (!m_pPopupView->GetRect().PtInRect(msg.px,msg.py)) {
						m_pPopupView->HidePopup();
						if (m_pCurView) m_pCurView->Invalidate();
					} else m_pPopupView->MouseDown(iPoint(msg.px,msg.py)); 
				}
				break;
			case iInput::iEntry::MouseUp: 
				if (m_pCapView) m_pCapView->MouseUp(iPoint(msg.px,msg.py)); 
				else {
					if (m_pPopupView->IsVisible()) m_pPopupView->MouseUp(iPoint(msg.px,msg.py));
					else HidePopup();
				}
				break;
			case iInput::iEntry::KeyDown: m_pApp->KeyDown(msg.key);	break;
			case iInput::iEntry::KeyUp: m_pApp->KeyUp(msg.key);	break;
		}
	} else if (m_dlgStack.GetSize()) {
		// In case of dialog stack
		iDialog* pDlg = m_dlgStack.GetLast();
		switch(msg.taskType) {
			case iInput::iEntry::MouseMove: if (m_pCapView) m_pCapView->MouseTrack(iPoint(msg.px,msg.py)); else pDlg->MouseTrack(iPoint(msg.px,msg.py)); break;
			case iInput::iEntry::MouseDown: if (m_pCapView) m_pCapView->MouseDown(iPoint(msg.px,msg.py)); else pDlg->MouseDown(iPoint(msg.px,msg.py)); break;
			case iInput::iEntry::MouseUp: if (m_pCapView) m_pCapView->MouseUp(iPoint(msg.px,msg.py)); else pDlg->MouseUp(iPoint(msg.px,msg.py)); break;
			case iInput::iEntry::KeyDown: if (!pDlg->KeyDown(msg.key)) m_pApp->KeyDown(msg.key); break;
			case iInput::iEntry::KeyUp: if (!pDlg->KeyUp(msg.key)) m_pApp->KeyUp(msg.key); break;
		}
	} else if (m_pCurView) {
		// In general case
		switch(msg.taskType) {
			case iInput::iEntry::MouseMove: 
				if (m_pCapView) m_pCapView->MouseTrack(iPoint(msg.px,msg.py)); else m_pCurView->MouseTrack(iPoint(msg.px,msg.py)); 
				break;
			case iInput::iEntry::MouseDown: 
				if (m_pCapView) m_pCapView->MouseDown(iPoint(msg.px,msg.py)); else 	m_pCurView->MouseDown(iPoint(msg.px,msg.py)); 
				break;
			case iInput::iEntry::MouseUp: 
				if (m_pCapView) m_pCapView->MouseUp(iPoint(msg.px,msg.py)); else m_pCurView->MouseUp(iPoint(msg.px,msg.py)); 
				break;
			case iInput::iEntry::KeyDown: 
				if (!m_pCurView->KeyDown(msg.key)) {
					m_pApp->KeyDown(msg.key);
				}
				break;
			case iInput::iEntry::KeyUp: 
				if (!m_pCurView->KeyUp(msg.key)) m_pApp->KeyUp(msg.key);
				break;
		}
	} else {
		// 
		switch(msg.taskType) {
			case iInput::iEntry::KeyDown: m_pApp->KeyDown(msg.key);	break;
			case iInput::iEntry::KeyUp: m_pApp->KeyUp(msg.key);	break;
		}
	}

	return true;
}

void iViewMgr::SetCurView(iTopmostView* pCurView)
{
	m_pCurView = pCurView;
}

void iViewMgr::SetViewCapture(iView* pCapView)
{
	check(!m_pCapView);
	m_pCapView = pCapView;
}

iView* iViewMgr::ReleaseViewCapture()
{
	check(m_pCapView);
	iView* nView = m_pCapView;
	m_pCapView = NULL;
	return nView;
}

// Modal stack
void iViewMgr::PushModalDlg(iDialog* pDlg)
{
	m_dlgStack.Push(pDlg);
}

iDialog* iViewMgr::PopModalDlg()
{
	if (m_pCurView) m_pCurView->Invalidate();
	return m_dlgStack.Pop();
}

// Popup windows
void iViewMgr::TrackPopup(iPopupView* pPopupView, const iPoint& pos, const iRect& bound, Alignment al)
{
	check(!m_pPopupView && pPopupView);
	m_pPopupView = pPopupView;
	m_pPopupView->TrackPopup(pos, bound, al);
}

void iViewMgr::HidePopup()
{
	check(m_pPopupView);
	delete m_pPopupView;
	m_pPopupView = NULL;
	if (m_pCurView) m_pCurView->Invalidate();
}

void iViewMgr::Compose(iRect& rect)
{
	// Compose current topmost view
	if (m_pCurView && m_pCurView->NeedRedraw()) m_pCurView->Compose(rect);

	// Compose dialog stack
	for (uint32 xx=0; xx< m_dlgStack.GetSize(); ++xx) m_dlgStack[xx]->Compose(rect);

	// Compose Popup view
	if (m_pPopupView) m_pPopupView->Compose(rect);

	// Compose drag'n'drop glyph
	if (m_pDragGlyph) m_pDragGlyph->ComposeDragGlyph();
}

// Timer processing
void iViewMgr::Process(uint32 interval)
{
	if (m_timers.GetSize()) {
		m_timerCounter += interval;
		while (m_timers.GetSize() && m_timers.GetLast().timer <= m_timerCounter) {
			iTimerHandler item = m_timers.Pop();
			item.pHandler->OnTimer(item.tid);
		}
	}
}

void iViewMgr::SetTimer(uint32 period, uint32 tid, iView* pHandler)
{
	if (!m_timers.GetSize()) {
		// reset timer counter
		m_timerCounter = 0;
		m_timers.Add(iTimerHandler(period, tid, pHandler));
	} else if (m_timers.GetLast().timer >= (period+m_timerCounter) ) {
		m_timers.Add(iTimerHandler(period+m_timerCounter, tid, pHandler));
	} else {
		uint32 ntimer = period+m_timerCounter;
		for (uint32 xx=0; xx<m_timers.GetSize(); ++xx) {
			if (ntimer >= m_timers[xx].timer) {
				m_timers.InsertBefore(xx, iTimerHandler(ntimer, tid, pHandler));
				break;
			}
		}
	}
}


void iViewMgr::CleanupTimers(iView* pHandler)
{
	if (!pHandler) m_timers.RemoveAll();
	else {
		for (uint32 xx=0; xx<m_timers.GetSize();){
			if (m_timers[xx].pHandler == pHandler) m_timers.RemoveAt(xx);
			else ++xx;
		}
	}
}

