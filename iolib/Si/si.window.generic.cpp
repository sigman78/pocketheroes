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
#include "../he/he.wnd.helpers.h"
#include "si.window.generic.h"

const uint32 TIMER_ID_MOSESEEK = 541223;

#ifndef GET_WHEEL_DELTA_WPARAM
#	define GET_WHEEL_DELTA_WPARAM(wParam)  ((short)HIWORD(wParam))
#endif //GET_WHEEL_DELTA_WPARAM

iGenericWindow::iGenericWindow()
{}

iGenericWindow::~iGenericWindow()
{
	if (IsWindow()) DestroyWindow();
}

void iGenericWindow::DestroyContent()
{ }

bool iGenericWindow::CreateGenericWindow(const iRect &rect, HWND parent, uint32 style, uint32 exstyle, bool layered)
{
	m_HoldRedraw = false;
	m_Enabled=true;
	m_bTracked=false;
	m_MouseInside=false;
	m_IsLayered = layered;


	// child window can't be layered
	if (layered && parent != NULL) {
		check(0);
		return false;
	}

	GetWndClassInfo().m_wc.style = GetWndClassInfo().m_wc.style &~ CS_DBLCLKS;

	if (Create(parent,RECT(rect),_T(""),style,exstyle)==NULL) return false;
	else {
		if (m_IsLayered) RedrawWnd();
		return true;
	}
}

iRect iGenericWindow::GetClRect() const
{
	return iWMetrics::GetClRect(m_hWnd);
}

iRect iGenericWindow::GetWndRect() const
{
	return iWMetrics::GetWndRect(m_hWnd);
}

iSize iGenericWindow::GetWndSize() const
{
	return iWMetrics::GetWndSize(m_hWnd);
}

iPoint iGenericWindow::GetWndPos() const
{
	RECT rect;
	GetWindowRect(&rect);
	return iPoint(rect.left ,rect.top);
}

void iGenericWindow::SetWndRect(const iRect &rect)
{
	SetWindowPos(0,rect.x,rect.y,rect.w,rect.h,SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}

void iGenericWindow::SetWndSize(const iSize &siz)
{
	SetWindowPos(0,0,0,siz.w,siz.h,SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOMOVE);
}

void iGenericWindow::SetWndPos(const iPoint &pos)
{
	SetWindowPos(0,pos.x,pos.y,0,0,SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOSIZE);
}

iPoint iGenericWindow::GetCurPoint() const
{
	POINT pnt;
	GetCursorPos(&pnt);
	return iPoint(pnt);
}

bool iGenericWindow::IsMouseOnWindow() const
{
	return GetWndRect().PtInRect(GetCurPoint());
}

iPoint iGenericWindow::ScreenToClient(const iPoint& pnt) const
{
	return iPoint(pnt - GetWndRect().point());
}

iPoint iGenericWindow::ClientToScreen(const iPoint& pnt) const
{
	return iPoint(pnt + GetWndRect().point());
}


LRESULT iGenericWindow::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	MsgSize(iSize(LOWORD(lParam),HIWORD(lParam)));
	return 0;
}

LRESULT iGenericWindow::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE) {
		m_Activated=true;
		if (HIWORD(wParam)==0) MsgActivate(true,false);
		else MsgActivate(true,true);
	} else if (LOWORD(wParam) == WA_INACTIVE) {
		m_Activated=false;
		if (HIWORD(wParam)==0) MsgActivate(false,false);
		else MsgActivate(false,true);
	}
	return 0;
}

LRESULT iGenericWindow::OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	MsgWindowPosChanged((LPWINDOWPOS) lParam);
	bHandler=FALSE;
	return 0;
}

LRESULT iGenericWindow::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	MsgWindowPosChanging((LPWINDOWPOS) lParam);
	return 0;
}

LRESULT iGenericWindow::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	return 0;
}

LRESULT iGenericWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	PAINTSTRUCT ps;
	iDC dc = BeginPaint(&ps);
	MsgPaint(dc, ps);
	EndPaint(&ps);
	bHandler=TRUE;
	return 0;
}

LRESULT iGenericWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	MsgDestroy();
	DestroyContent();
	return 0;
}

LRESULT iGenericWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	DestroyWindow();
	return 0;
}

LRESULT iGenericWindow::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	if (wParam==TIMER_ID_MOSESEEK) {
		if (!IsMouseOnWindow() && m_MouseInside==true) {
			KillTimer(TIMER_ID_MOSESEEK);
			m_MouseInside=false;
			if (IsEnabled()) MsgMouseLeave();
		}
	}

	MsgTimer(wParam);
	return 0;
}

LRESULT iGenericWindow::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	iPoint pos=iPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));

	if (m_MouseInside==false && IsMouseOnWindow()) {
		m_MouseInside=true;
		SetTimer(TIMER_ID_MOSESEEK,50);
		if (IsEnabled()) this->MsgMouseEnter();
	}

	if (IsEnabled()){
		if (m_bTracked) this->MsgTracking(pos);
		else {
			this->MsgMouseMove(pos);
		}
	}

	return 0;
}

LRESULT iGenericWindow::OnNcMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	iPoint pos=iPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));

	if (m_MouseInside==false && IsMouseOnWindow()){
		m_MouseInside=true;
		SetTimer(TIMER_ID_MOSESEEK,50);
		if (IsEnabled()) this->MsgMouseEnter();
	}

	if (m_bTracked) {
		if (IsEnabled()) this->MsgTracking(pos);
	} else {
		if (IsEnabled()) this->MsgMouseMove(pos);
	}

	return 0;
}

LRESULT iGenericWindow::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	iPoint pos=iPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));

	m_bTracked=true;

	SetCapture();
	if (IsEnabled()) {
		MsgLButtonDown(pos, wParam);
		MsgBeginTrack(pos);
	}

	return 0;
}

LRESULT iGenericWindow::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	iPoint pos=iPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
	if (m_bTracked) {
		ReleaseCapture();
		m_bTracked=false;
	}
	if (IsEnabled()) MsgLButtonUp(pos,wParam);
	if (IsWindow() && IsMouseOnWindow()) {
		if (IsEnabled()) MsgLButtonClick(pos);
	}
	if (IsEnabled()) MsgEndTrack(pos);

	return 0;
}

LRESULT iGenericWindow::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	iPoint pos=iPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
	if (IsEnabled()) MsgRButtonDown(pos, wParam);

	return 0;
}

LRESULT iGenericWindow::OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	iPoint pos=iPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
	if (IsEnabled()) MsgRButtonUp(pos,wParam);
	if (IsMouseOnWindow()) {
		if (IsEnabled()) MsgRButtonClick(pos);
	}

	return 0;
}

LRESULT iGenericWindow::OnMButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	iPoint pos=iPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
	if (IsEnabled()) MsgMButtonDown(pos,wParam);

	return 0;
}

LRESULT iGenericWindow::OnMButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	iPoint pos=iPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
	if (IsEnabled()) MsgMButtonUp(pos,wParam);
	if (IsMouseOnWindow()) {
		if (IsEnabled()) MsgMButtonClick(pos);
	}

	return 0;
}

LRESULT iGenericWindow::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	iPoint pos=iPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
	sint32 mDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	if (IsEnabled()) MsgMouseWheel(pos,mDelta);
	return 0;
}

LRESULT iGenericWindow::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	if (IsEnabled()) MsgSetCursor();
	return TRUE;
}

LRESULT iGenericWindow::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	if (IsEnabled()) MsgKeyDown(wParam);
	return 0;
}

LRESULT iGenericWindow::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	if (IsEnabled()) MsgKeyUp(wParam);
	return 0;
}

LRESULT iGenericWindow::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	MINMAXINFO *mInfo=(MINMAXINFO*)lParam;
	iSize minsize(mInfo->ptMinTrackSize.x,mInfo->ptMinTrackSize.y);
	iSize maxsize(mInfo->ptMaxTrackSize.x,mInfo->ptMaxTrackSize.y);
	
	if (MsgGetMinMaxInfo(minsize,maxsize)) {
		mInfo->ptMinTrackSize.x=minsize.w;
		mInfo->ptMinTrackSize.y=minsize.h;
		mInfo->ptMaxTrackSize.x=maxsize.w;
		mInfo->ptMaxTrackSize.y=maxsize.h;
		return 0;
	}
	else return 0;
}

LRESULT iGenericWindow::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
{
	iPoint mpos(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
	return MsgNcHitTest(mpos);
}

void iGenericWindow::HoldRedraw()
{
	m_HoldRedraw = true;
}

void iGenericWindow::ReleaseRedraw()
{
	m_HoldRedraw = false;
	RedrawWnd();
}

void iGenericWindow::RedrawWnd(const iRect &rect)
{
	if (!::IsWindow(m_hWnd) || m_HoldRedraw) return;
	if (m_IsLayered) {
		MsgLayeredPaint(rect);
	} else {
		if (rect == cInvalidRect) Invalidate();
		else InvalidateRect(&RECT(rect));
	}
}

void iGenericWindow::Enable()
{
	if (!IsEnabled()) {
		m_Enabled=true;
		RedrawWnd();
	}
}

void iGenericWindow::Disable()
{
	if (IsEnabled()) {
		m_Enabled=false;
		Invalidate();
	}
}

bool iGenericWindow::IsEnabled()
{
	return m_Enabled;
}
