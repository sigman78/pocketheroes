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
#include "gxl.winapp.h"
#include "gx.h"

//
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	static iWinApp* cls = 0;
	LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
	if (cls == 0) {
		cls = (iWinApp*)GetWindowLong(hWnd,GWL_USERDATA);
	}

	if ( cls ) {
		return cls->OnMessage( hWnd, uMsg, wParam, lParam );
	} else return 1;
}

//
iWinApp::iWinApp()
: m_bInited(false)
, m_bExit(false)
, m_hWnd(NULL)
{}

//
iWinApp::~iWinApp()
{ 
	Destroy();
}

//
bool iWinApp::Init(HINSTANCE hInst, LPCWSTR appName, uint32 flags)
{
	m_Flags = flags;
	InitMath();
	iSize scrsiz(GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	iSize osiz(scrsiz);

	check(!m_hWnd);
	// Create Window
	WNDCLASS wndClass = { 0, WndProc, 4, 4, hInst, NULL, NULL, (HBRUSH)GetStockObject(BLACK_BRUSH), NULL, L"GXLMWND" };
	RegisterClass( &wndClass );
	m_hWnd = CreateWindow( L"GXLMWND", L"iDXFrameWorkWnd", WS_POPUP, 0, 0, osiz.w, osiz.h, 0L, 0, hInst, 0 );
	if (!m_hWnd) return false;
	SetWindowLong(m_hWnd,GWL_USERDATA,(LONG)this);
	ShowWindow(m_hWnd, SW_SHOW);
	
	if (!GXOpenDisplay(m_hWnd, GX_FULLSCREEN)) return false;
	iSize ssiz;
	if (m_Flags & GXLF_LANDSCAPE) ssiz = iSize(osiz.h,osiz.w);
	else ssiz = iSize(osiz.w,osiz.h);
	m_BackBuff.Init(ssiz,iDib::RGB);

	m_bInited = true;
	return true;
}

void iWinApp::Destroy()
{
	if (m_bInited){
		if (m_hWnd) {
			GXCloseDisplay();
			DestroyWindow(m_hWnd);
			m_hWnd = NULL;
		}
		m_bInited = false;
	}
}

//////////////////////////////////////////////////////////////////////////
sint32 iWinApp::Run()
{
	bool bGotMsg;
	MSG  msg;
	msg.message = WM_NULL;
	::PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );
	while( WM_QUIT != msg.message  ){
		bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );
		if( bGotMsg ) {
			// (do not!)Translate but dispatch the message
			// TranslateMessage( &msg );
			DispatchMessage( &msg );
		} else {
			//iTask::Result res = m_TaskMgr.ProcessQueue(m_Timer.GetCurTime());
			if (m_bExit/* || res == iTask::Completed*/) return 0;
			Sleep(10);
		}
	}

	return (INT)msg.wParam;
}
//////////////////////////////////////////////////////////////////////////
void iWinApp::Exit(uint32 code)
{
	m_bExit = true;
}
//////////////////////////////////////////////////////////////////////////
void iWinApp::DoPaint()
{
	uint8 *pBuff = (uint8*)GXBeginDraw();
	if(pBuff) {
		if (m_Flags & GXLF_LANDSCAPE){
			iDib::pixel* pDst = (iDib::pixel*)pBuff;
			const iDib::pixel* pSrc = (const iDib::pixel*)m_BackBuff.GetPtr();
			for (uint32 yy=0; yy<m_BackBuff.GetHeight(); ++yy){
				sint32 wpos = (m_BackBuff.GetWidth()-1)*m_BackBuff.GetHeight();
				for (uint32 xx=0; xx<m_BackBuff.GetWidth(); ++xx){
					pDst[wpos+yy] = *pSrc;
					wpos-=m_BackBuff.GetHeight();
					pSrc++;
				}
			}
		} else {
			memcpy(pBuff,m_BackBuff.GetPtr(),m_BackBuff.GetBuffLen());
		}
	    GXEndDraw();
	}
}
//////////////////////////////////////////////////////////////////////////
LRESULT iWinApp::OnMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg ) {
		case WM_KILLFOCUS:
			//m_bActive = false;
			//if (m_pDispMsgHnd) m_pDispMsgHnd->msg_Suspend();
			return 0;
		case WM_SETFOCUS:
			//m_bActive = true;
			//if (m_pDispMsgHnd) m_pDispMsgHnd->msg_Resume();
			return 0;
		case WM_ERASEBKGND:
			return 0;
		case WM_PAINT:
			if (/*m_bActive && m_pDispMsgHnd*/1){
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				DoPaint();
				//msg_Paint(hdc);
				EndPaint(m_hWnd,&ps);
			}
			return 0;
		case WM_LBUTTONDOWN:
			msg_MouseDown(iPoint(LOWORD(lParam),HIWORD(lParam)));
			return 0;
		case WM_LBUTTONUP:
			msg_MouseUp(iPoint(LOWORD(lParam),HIWORD(lParam)));
			return 0;
		case WM_RBUTTONDOWN:
			msg_MouseDown(iPoint(LOWORD(lParam),HIWORD(lParam)));
			return 0;
		case WM_RBUTTONUP:
			msg_MouseUp(iPoint(LOWORD(lParam),HIWORD(lParam)));
			return 0;
		case WM_MOUSEMOVE:
			msg_MouseMove(iPoint(LOWORD(lParam),HIWORD(lParam)));
			return 0;
		case WM_KEYDOWN:
			//if (m_pInputMsgHnd) m_pInputMsgHnd->msg_OnKeyDown(wParam);
			return 0;
		case WM_KEYUP:
			//if (m_pInputMsgHnd) m_pInputMsgHnd->msg_OnKeyUp(wParam);
			return 0;
		case WM_CLOSE:	
			PostQuitMessage(0); 
			return 0;
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}
