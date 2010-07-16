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

#include "stdafx.h"
#include "game.h"
#include "gxl.cmdline.h"
#include "gxl.power.h"

//
#include <stdlib.h> 
#include "IntroDlg.h"

struct Dummy {
	~Dummy() {
		mem_report();
	}
};

Dummy _dummy;

#ifdef _DEEP_DEBUG_
#define DBG( x ) x 
void SEO( int stream ) 
{ 
   _CrtSetReportMode( stream, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG ); 
   _CrtSetReportFile( stream, _CRTDBG_FILE_STDOUT ); 
} 

inline void InitDBG() 
{ 
    DBG( SEO( _CRT_ERROR ) ); 
    DBG( SEO( _CRT_WARN ) ); 
    DBG( SEO( _CRT_ASSERT ) ); 
 
    DBG(_CrtSetDbgFlag( _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG ) | ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_CHECK_CRT_DF | _CRTDBG_LEAK_CHECK_DF )) ); 
} 

inline void DoneDBG() 
{ 
    DBG( _CrtCheckMemory() ); 
} 

struct DebugCls
{
	DebugCls()
	{ InitDBG();}
	~DebugCls()
	{ DoneDBG(); }
};
DebugCls cls_;

#else 
#define DBG( x ) 
#endif 


//hssSpeaker	gSpeaker;
iStringT		gRootPath;
iStringT		gDataPath;
iStringT		gSavePath;
iStringT		gMapsPath;
iGXApp			gApp;
iDibReader		gDibReader;
iTextComposer	gTextComposer;
iTextManager	gTextMgr;
iGfxManager		gGfxMgr;
iSfxManager		gSfxMgr;
iGame			gGame;
iSettings		gSettings;

void WaitFor(sint32 msec)
{
	do {
		MSG  msg;
		if (::PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE )) {
			while (::PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE)) {
				if ( msg.message == WM_QUIT ) return;
				DispatchMessage( &msg );
			}
		} 
		Sleep(30);
		msec -= 30;
	} while (!gApp.Input().EntryCount() && msec > 0);
}

void ShowProgressReport( uint32 curProg, bool initial = true )
{
	//
	iDisplay& display = gApp.Display();
	iRect rcScreen = display.SurfMetrics();
	iPoint ldrOrg  = rcScreen.Center();
	ldrOrg.y += rcScreen.h / 3;
	uint32 pbarWidth = (rcScreen.w * 5) / 6;
	iRect rcPbar( ldrOrg.x - pbarWidth/2, ldrOrg.y, pbarWidth, 10 );
	uint16 pbarColor = 48;
	if ( !initial ) pbarColor = 127;
	ComposeProgressBar( false, display.GetSurface(), rcPbar, pbarColor, curProg, 100 );
	//gGfxMgr.Blit(12,gApp.Display().GetSurface(), iPoint(0,0) );
	if ( !initial ) {
		rcPbar.MoveY( -16 );
		iTextComposer::FontConfig loadingFC (GetButtonFont(0));
		gTextComposer.TextOut( loadingFC, gApp.Display().GetSurface(), rcPbar.TopLeft(), _T("Prepare for battle...") );
	}
	display.DoPaint( display.SurfMetrics() );
	Sleep(1);
}

bool EnsureMemAvail( DWORD mega )
{
#ifdef OS_WINCE
	uint32 bytesNeeded = mega * (1024*1024);
	MEMORYSTATUS mst;
	mst.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus( &mst );
	if ( mst.dwTotalPhys < bytesNeeded ) {
		// out of luck, just report failure
		return false;
	}
	if ( mst.dwAvailPhys < bytesNeeded ) {
		// try to free up closing apps
		HINSTANCE hAygShell = LoadLibrary( _T("aygshell.dll") );		
		void* pCloseApps = hAygShell ? (void*)GetProcAddress( hAygShell, _T("SHCloseApps") ) : NULL;
		if ( pCloseApps != NULL ) {
			for( size_t ntries = 0; ntries != 2; ntries++ ) {
				if ( ((BOOL(*)(DWORD))pCloseApps)( bytesNeeded ) ) 	return true;
			}
			return false;
		}
	}
#endif
	// no aygshell, just report success
	return true;
}

#ifdef DEBUG_LOG
iLogMgr gLogMgr;
#endif //DEBUG_LOG

#ifndef HPC_JORNADA
#	include <psapi.h>
#endif 

#ifdef OS_WINCE
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpstrCmdLine, int nCmdShow)
#endif
#ifdef OS_WIN32
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpstrCmdLine, int nCmdShow)
#endif
{
	// Checks mem availability and closes background apps
	// NB: This is approximate amount of memory (in MB) required to start application
	if ( !EnsureMemAvail( 4 ) ) {
		MessageBox( GetForegroundWindow(), _T("Not enough mana found!"), _T("Startup error"), MB_OK );
		return 0;
	}


	// NB: Now check is performed at the iWindow class - i.e. if creation fails 
	// - other app instance is running (we don't know wndClass name here)
//// Check second instance of the game
//   HANDLE hMutex = CreateMutex( NULL, TRUE, _T("PocketHeroesSingleInstanceMutex") );
//   if ( GetLastError() == ERROR_ALREADY_EXISTS ) {
//       return TRUE;
//   }
//   scope_guard guardAppMutex = make_guard( CloseHandle, hMutex );

	// Root, Save and Maps folders
	iFileName::GetAppPath(gRootPath);
	gDataPath = gRootPath + _T("Data\\");
	gSavePath = gRootPath + _T("Save\\");
	gMapsPath = gRootPath + _T("Maps\\");

	// Init log manager
#ifdef DEBUG_LOG
	gLogMgr.Init(gRootPath+_T("game.log"));
#endif //DEBUG_LOG

	// Load settings
	if (!gSettings.Init(lpstrCmdLine)) {
		MessageBox(NULL, _T("Unable to init game settings!"), NULL, MB_OK);
	}

	// Init application
	uint32 flags = GXLF_LANDSCAPE;
	if (!gSettings.ForceNoSound() && gSettings.GetEntryValue(CET_SFXVOLUME) != 0) flags |= GXLF_ENABLESOUND;
#ifdef OS_WINCE
	if (gSettings.GetEntryValue(CET_LHANDMAODE)) flags |= GXLF_LHANDER;
#endif
#ifdef OS_WIN32
	flags |= GXLF_DOUBLESIZE;
#endif
	if (!gApp.Init(hInstance,L"Palm Heroes",&gGame,30,flags)){
		return -1;
	}

	// ShowLogo and intro image (disabled for this version)
	//iIntroDlg idlg(&gApp.ViewMgr());
	//idlg.DoModal();
	

	// Setup global sfx volume
	if (gApp.SndPlayer().Inited()) {
		gApp.SndPlayer().SetVolume(gSettings.GetEntryValue(CET_SFXVOLUME)*256/10);
	}

	// Setup initial gamma value
	gApp.Display().SetGamma(1.0f + 0.05f * gSettings.GetEntryValue(CET_DISPGAMMA));

	// Init text composer
	if (!gTextComposer.Init()) {
		MessageBox(NULL, _T("Unable to init text composer!"), NULL, MB_OK);
		return -1;
	}

	// Init text resources
	if (!gTextMgr.Init()) {
		MessageBox(NULL, _T("Unable to init text manager!"), NULL, MB_OK);
		return -1;
	}

	// Init gfx resources and fill secret dll
	gGfxMgr.SetGamma( gSettings.GetEntryValue(CET_DISPGAMMA) );
	if (!gGfxMgr.Load(0,(gDataPath+_T("game.gfx")).CStr(), gSettings.MapSpriteFile()?(iGfxManager::LM_MappedFile):(iGfxManager::LM_Memory))) {
		MessageBox(NULL, _T("Unable to open sprite file!"), NULL, MB_OK);
		return -1;
	}


	// Init sfx resources
	if (gSettings.GetEntryValue(CET_SFXVOLUME) != 0 && !gSfxMgr.Init(gDataPath+_T("game.sfx"))) {
		MessageBox(NULL, _T("Unable to open sound resources file!"), NULL, MB_OK);
		return -1;
	}

	// Initialize and start new game
	if (!gGame.Init(lpstrCmdLine) ) {
		return -1;
	}

#ifdef OS_WINCE
	iLightHolder	lightHolder;
#endif //OS_WINCE

	// Core loop
	sint32 retCode =  gApp.Run();


	// Cleanup
	gApp.Destroy();

	return retCode;
}

