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

#include "gxl.ctr.array.h"
#include "gxl.ctr.cbuff.h"
#include "gxl.window.h"
#include "gxl.input.h"
#include "gxl.timer.h"
#include "gxl.display.h"
#include "gxl.view.h"
#include "gxl.dialog.h"
#include "gxl.viewmgr.h"
#include "gxl.application.h"

#ifdef OS_WINCE
#	ifdef HPC_JORNADA
#	else
#		include <aygshell.h>
		const int FMSG = WM_CREATE;
#	endif
#endif
#ifdef OS_WIN32
	const int FMSG = WM_NCCREATE;
#endif

//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	static iWindow* cls = 0;
	LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
	if (cls == 0) {
		cls = (iWindow*)GetWindowLong(hWnd,GWL_USERDATA);
	}

	if ( cls ) {
		return cls->OnMessage( hWnd, uMsg, wParam, lParam );
	} else return 1;
}
//////////////////////////////////////////////////////////////////////////
iWindow::iWindow()
: m_hWnd(NULL)
, m_pOwner(NULL)
, m_bActive(false)
, m_bTrack(false)
{
}
//////////////////////////////////////////////////////////////////////////
iWindow::~iWindow()
{
	check(!m_hWnd);
}

//////////////////////////////////////////////////////////////////////////
const WCHAR wndClassName[] = L"GXLMWND";

bool iWindow::Init(HINSTANCE hInst, LPCWSTR wndName, const iSize& siz, iGXApp* owner )
{
	check(!m_hWnd);
	check(owner != 0);
	m_pOwner = owner;
	// SiGMan: this one is proper (for PPC) app single instance check
	// first check if such window already exists (application is running)
	// NB:: Somehow in the windows application does not have any caption and search fails
	// 2ROBERT: WHY THERE IS NO WINDOW TITLE DISPLAYED???
	HWND hOtherWnd = ::FindWindow( wndClassName, NULL );
	// this parameter belongs to config actually
	bool allowMultipleInstances = false;
	if ( (hOtherWnd != NULL) && !allowMultipleInstances ) {
		SetForegroundWindow( hOtherWnd );
		return false;
	}
	// Create Window
	WNDCLASS wndClass = { CS_HREDRAW | CS_VREDRAW, WndProc, 4, 4, hInst, NULL, LoadCursor(NULL,IDC_ARROW), (HBRUSH)GetStockObject(NULL_BRUSH), NULL, wndClassName };
	RegisterClass( &wndClass );
	m_Size = siz;
#ifdef OS_WINCE
	m_hWnd = CreateWindow( wndClassName, wndName, WS_VISIBLE, 0, 0, m_Size.w, m_Size.h, 0L, 0, hInst, 0 );
#if !defined(HPC_JORNADA)
	//::SHFullScreen( m_hWnd, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON );
	owner->ShellFullscreen( m_hWnd, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON );
#endif
#endif
#ifdef OS_WIN32
	RECT wrc={100,100,100+m_Size.w,100+m_Size.h};
	DWORD dwStyle = WS_OVERLAPPED  | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&wrc,dwStyle,FALSE);
	m_hWnd = ::CreateWindow( wndClassName, wndName, dwStyle, wrc.left, wrc.top, wrc.right-wrc.left, wrc.bottom-wrc.top, 0L, 0, hInst, 0 );
#endif
	if (!m_hWnd) return false;
	SetWindowLong(m_hWnd,GWL_USERDATA,(LONG)this);
	m_bActive = true;
#ifdef OS_WIN32
	ShowWindow(m_hWnd, SW_SHOW);
#endif
	return true;
}

//////////////////////////////////////////////////////////////////////////
void iWindow::SetSize(const iSize& siz)
{
	m_Size = siz;
#ifdef OS_WIN32
	RECT wrc={100,100,100+siz.w,100+siz.h};
	AdjustWindowRect(&wrc,::GetWindowLong(m_hWnd,GWL_STYLE),FALSE);
	::SetWindowPos(m_hWnd,0,0,0,wrc.right-wrc.left,wrc.bottom-wrc.top,SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOMOVE);
#endif
}

//////////////////////////////////////////////////////////////////////////
void iWindow::Destroy()
{
	check(m_hWnd);
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;
}
//////////////////////////////////////////////////////////////////////////
LRESULT iWindow::OnMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	POINT pt;
	switch( uMsg ) {
		case WM_KILLFOCUS:
			if (m_pOwner) {
				m_pOwner->Suspend();
			}
			return 0;
		case WM_SETFOCUS:
			if (m_pOwner) {
				m_pOwner->Resume();
			}
			return 0;
		/*
		case WM_ACTIVATE:
			if (m_pOwner) {
				if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE) {
					OutputDebugString(L"+ WM_ACTIVATE 1\n");
					 m_pOwner->Resume();
				} else if (LOWORD(wParam) == WA_INACTIVE) {
					OutputDebugString(L"- WM_ACTIVATE 0\n");
					m_pOwner->Suspend();
				} else {
					check(0 == "Invalid wParam in WM_ACTIVATE");
				}

			}
			return 0;
		*/
		case WM_ERASEBKGND:
			return 0;
		case WM_PAINT:
			if (m_pOwner && m_pOwner->IsActive()){
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				m_pOwner->Display().msg_OnPaint(hdc);
				EndPaint(m_hWnd,&ps);
			}
			return 0;
		case WM_LBUTTONDOWN:
			if (m_bTrack) return 0;
			SetCapture(hWnd);
			if (m_pOwner) {
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				//ClientToScreen(m_hWnd, &pt);
				m_pOwner->Input().msg_OnMouseDown(pt.x,pt.y);
			}
			m_bTrack = true;
			return 0;
		case WM_LBUTTONUP:
			if (!m_bTrack) return 0;
			ReleaseCapture();
			if (m_pOwner) {
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				//ClientToScreen(m_hWnd, &pt);
				m_pOwner->Input().msg_OnMouseUp(pt.x, pt.y);
			}
			m_bTrack = false;
			return 0;
		case WM_RBUTTONDOWN:
			if (m_pOwner) m_pOwner->Input().msg_OnKeyDown(VK_CLEAR);
			return 0;
		case WM_RBUTTONUP:
			if (m_pOwner) m_pOwner->Input().msg_OnKeyUp(VK_CLEAR);
			return 0;
		case WM_MOUSEMOVE:
			if (m_pOwner && (wParam&MK_LBUTTON)) {
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				//ClientToScreen(m_hWnd, &pt);
				m_pOwner->Input().msg_OnMouseMove(pt.x, pt.y);
			}
			return 0;
		case WM_KEYDOWN:
			if (m_pOwner && (lParam & 0x40000000) == 0) m_pOwner->Input().msg_OnKeyDown(wParam);
			return 0;
		case WM_KEYUP:
			if (m_pOwner) m_pOwner->Input().msg_OnKeyUp(wParam);
			return 0;

		case WM_CLOSE:	
			PostQuitMessage(0); 
			return 0;
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//////////////////////////////////////////////////////////////////////////
void iWindow::SetOwner(iGXApp* pApp)
{
	m_pOwner = pApp;
}
