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

#include <windows.h>
#ifdef OS_WINCE
#	ifdef HPC_JORNADA
#	else
#		include <aygshell.h>
#	endif
#include <gx.h>
#endif //OS_WINCE
#include "gxl.inc.h"
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

// ceGCC SDK lacks this define
void WINAPI	SystemIdleTimerReset( void );

typedef void (WINAPI *sitr_fn)(void);

//
iGXApp::iGXApp()
: m_viewMgr(this)
, m_pGame(NULL)
, m_bInited(false)
, m_bSuspended(false)
, m_bExit(false)
, m_CycleDelay(30)
, m_processTimer(0)
{}

//
iGXApp::~iGXApp()
{ 
	check(!m_bInited);
}

//
bool iGXApp::Init(HINSTANCE hInst, LPCWSTR appName, IGame* pGame, uint32 cdelay, uint32 flags)
{
	check(!m_bInited);
	InitMath();
	m_pGame = pGame;
	m_CycleDelay = cdelay;
	m_Flags = flags;
#if defined (HPC_JORNADA)
	m_BaseMetrics = iSize(320,240);
	flags |= GXLF_DEV_LANDSCAPE;
	iSize scrsiz(m_BaseMetrics);
	iSize osiz(m_BaseMetrics);
#elif defined (OS_WINCE)
	m_BaseMetrics = iSize(GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	if (m_BaseMetrics.w > m_BaseMetrics.h) flags |= GXLF_DEV_LANDSCAPE;
	iSize scrsiz(m_BaseMetrics);
	iSize osiz(m_BaseMetrics);
#elif defined (OS_WIN32)
	if (m_Flags & GXLF_REALVGA) m_BaseMetrics = iSize(480,640);
	else m_BaseMetrics = iSize(240,320);
	iSize scrsiz(m_BaseMetrics);
	if (m_Flags & GXLF_LANDSCAPE) {scrsiz.w=m_BaseMetrics.h; scrsiz.h=m_BaseMetrics.w; }
	iSize osiz(scrsiz);
	if (m_Flags & GXLF_DOUBLESIZE) {osiz.w *= 2; osiz.h *=2; }
#endif

	bool bOk = false;
	do {
		if (!m_Window.Init(hInst,appName,osiz, this)) break;
		if (!m_Input.Init(osiz,flags)) break;
		if (!m_Timer.Init()) break;
		if (!m_Display.Init(m_Window,scrsiz, flags)) break;
		if ( (m_Flags & GXLF_ENABLESOUND) && !m_SoundPlayer.Init() ) break;
		m_Window.SetOwner(this);
		m_bInited = true;
	} while(0);

	if (!m_bInited) {
		m_SoundPlayer.Close();
		m_Display.Destroy();
		m_Input.Destroy();
		m_Window.Destroy();
	}

#ifdef OS_WINCE
	m_coreDLL= LoadLibrary( L"COREDLL.DLL" );
	m_sysidletimer = (void*)GetProcAddress( m_coreDLL, L"SystemIdleTimerReset" );
	m_aygDLL = LoadLibrary( L"AYGSHELL.DLL" );
	m_shfullscreen = 0;
	if ( m_aygDLL != 0 ) {
		m_shfullscreen = (void*)GetProcAddress( m_aygDLL, L"SHFullScreen" );
	}
#endif	

	return m_bInited;
}

void iGXApp::Destroy()
{
	check(m_bInited);
	FreeLibrary( m_coreDLL );
	FreeLibrary( m_aygDLL );

	m_Display.Destroy();
	m_Input.Destroy();
	m_Window.Destroy();
	m_pGame = NULL;
	m_bInited = false;
}

void iGXApp::SetOrientation(bool bLandscape, bool bLeftHander)
{
	if (bLandscape && (m_Flags & GXLF_LANDSCAPE) == 0) m_Flags |= GXLF_LANDSCAPE;
	else if (!bLandscape && (m_Flags & GXLF_LANDSCAPE) != 0) m_Flags ^= GXLF_LANDSCAPE;

#ifdef OS_WIN32
	iSize csiz = m_Window.GetSize();
	iSwap(csiz.w,csiz.h);
	m_Window.SetSize(csiz);
#endif //OS_WIN32

	m_Input.SetOrientation(bLandscape, bLeftHander);
	m_Display.SetOrientation(bLandscape, bLeftHander);
}

void iGXApp::ToggleOrientation()
{
	SetOrientation(!IsLandscape(), (m_Flags&GXLF_LHANDER) != 0);
}

//////////////////////////////////////////////////////////////////////////
sint32 iGXApp::Cycle()
{
	MSG  msg;
	if (::PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE )) {
		while (::PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE)) {
			if ( msg.message == WM_QUIT ) return false;
			DispatchMessage( &msg );
		}
	} 
	
	while (m_Input.EntryCount()) m_viewMgr.ProcessMessage(m_Input.Get());
	
	static uint32 m_LastUpdate = m_Timer.GetCurTime();
	uint32 step = m_Timer.GetStep();

#ifdef OS_WINCE
	static uint32 pwrTimeout = 0;
	pwrTimeout += step;
	if (pwrTimeout >= 10000) {
		pwrTimeout = 0;
		//SystemIdleTimerReset();
		((void (WINAPI *)(void))m_sysidletimer)();
	}
#endif //OS_WINCE

	if (!m_bSuspended) {
		m_pGame->Process(fix32((sint32)step)/1000L);
		m_viewMgr.Process(step);
		if ( (step + m_processTimer) > m_CycleDelay ) {
			m_processTimer = 0;
			iRect rc;
			m_viewMgr.Compose(rc);
			if (!rc.IsEmpty()) m_Display.DoPaint(rc);
		} else m_processTimer += step;
	}
	Sleep(1);

	return true;
}

sint32 iGXApp::Run()
{
	while (!m_bExit && Cycle()) {}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
void iGXApp::Exit(uint32 code)
{
	m_bExit = true;
}
//////////////////////////////////////////////////////////////////////////
void iGXApp::Minimize()
{
#if defined(OS_WINCE)
	ShowWindow(m_Window.GetHWND(), SW_MINIMIZE);
#if !defined(HPC_JORNADA)
	ShellFullscreen( m_Window.GetHWND(), SHFS_SHOWTASKBAR | SHFS_SHOWSIPBUTTON | SHFS_SHOWSTARTICON );
	
#endif
#endif
	
	// TO Bring back discussion about proper way to minimize application
	// I should admit what there is definitely more than one way to do so:
	//
	// First is SHNavigateBack() API which is used to switch back to the 
	// previously running application
	// Notably its not available on pure CE devices
	//
	// The other is implmented by bringing the 'Today' screen back foreground:
	// HWND hwnd = FindWindow(TEXT("DesktopExplorerWindow"), NULL);
	// if((NULL != hwnd) && (TRUE == IsWindow(hwnd)))
	// {
    //   ShowWindow(hwnd, SW_SHOWNA);
    //   SetForegroundWindow((HWND)(((DWORD)((HWND)hwnd)) | 0x01));
	// }
	//
	// >> PPC2003\Samples\Win32\GapiApp\GapiApp.cpp
}

void iGXApp::Suspend()
{
	if (!m_bSuspended) {
#ifdef OS_WINCE
		m_pGame->OnSuspend();
		m_Display.msg_Suspend();
#endif //OS_WINCE
		m_bSuspended = true;
	}
}

void iGXApp::Resume()
{
	ODS(L"+ iGXApp::Resume() : ");
	if (m_bSuspended) {
#ifdef OS_WINCE
		// Clear key-press states
		m_Input.ClearKeyState();
#if !defined(HPC_JORNADA)
		//::SHFullScreen( m_Window.GetHWND(), SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON );
		ShellFullscreen( m_Window.GetHWND(), SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON );
#endif
		if (m_SoundPlayer.Inited()) {
			m_SoundPlayer.Close();
			m_SoundPlayer.Init();
		}
		m_Display.msg_Resume();
		m_pGame->OnResume();
#endif //OS_WINCE
		m_bSuspended = false;
	}
}

BOOL
iGXApp::ShellFullscreen( HWND p1, DWORD p2 )
{
	if ( m_shfullscreen != 0 ) {
		return ((BOOL (*)(HWND, DWORD))m_shfullscreen)( p1, p2 );
	}
	return FALSE;
}

