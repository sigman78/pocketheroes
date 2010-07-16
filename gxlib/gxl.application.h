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

#ifndef _GXLIB_APPLICATION_H_
#define _GXLIB_APPLICATION_H_

#include "gxl.audio.h"

/*
 *	
 */
struct IGame
{
	virtual sint32 Process(fix32 t) 	=0;
	virtual void OnKeyDown(sint32 key)	=0;
	virtual void OnKeyUp(sint32 key)	=0;
	virtual void OnSuspend() 			=0;
	virtual void OnResume() 			=0;
};

/*
 *	
 */
class iGXApp
{
public:
	iGXApp();
	~iGXApp();
	bool Init(HINSTANCE hInst, LPCWSTR appName, IGame* pGame, uint32 cdelay, uint32 flags);
	void Destroy();
	sint32 Cycle();
	sint32 Run();
	void Exit(uint32 code);
	void Minimize();
	void Suspend();
	void Resume();

	//
	void KeyDown(sint32 key) { check(m_pGame); m_pGame->OnKeyDown(key); }
	void KeyUp(sint32 key) { check(m_pGame); m_pGame->OnKeyUp(key); }

	//
	inline iWindow& Window() { return m_Window; }
	inline iViewMgr& ViewMgr() { return m_viewMgr; }
	inline iInput& Input() { return m_Input; }
	inline iTimer& Timer() { return m_Timer; }
	inline iDisplay& Display() { return m_Display; }
	inline iDib& Surface() { return m_Display.GetSurface(); }
	inline iSoundPlayer& SndPlayer() { return m_SoundPlayer; }
	inline bool IsActive() const { return m_bInited && !m_bSuspended; }

	//
	void SetOrientation(bool bLandscape, bool bLeftHander);
	void ToggleOrientation();
	inline bool IsLandscape() const { return (m_Flags & GXLF_LANDSCAPE) != 0; }

	BOOL ShellFullscreen( HWND, DWORD );

protected:
	//
	iSize			m_BaseMetrics;
	uint32			m_Flags;
	uint32			m_CycleDelay;
	uint32			m_processTimer;
	bool			m_bExit;

	IGame*			m_pGame;
	iWindow			m_Window;
	iInput			m_Input;
	iTimer			m_Timer;
	iDisplay		m_Display;

	iViewMgr		m_viewMgr;
	iSoundPlayer	m_SoundPlayer;
	bool			m_bInited;
	bool			m_bSuspended;

	HINSTANCE		m_aygDLL;
	HINSTANCE		m_coreDLL;

	void*			m_shfullscreen;
	void*			m_sysidletimer;
};

#endif //_GXLIB_APPLICATION_H_

