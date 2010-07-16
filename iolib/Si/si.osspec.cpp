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
#include "si.osspec.h"

OSSPECFNC gOsSpecFnc;


OSSPECFNC::PFN_UpdateLayeredWindow	OSSPECFNC::m_pfnUpdateLayeredWindow;


OSSPECFNC::OSSPECFNC()
{
	m_pfnUpdateLayeredWindow = NULL;
	Init();
}

OSSPECFNC::~OSSPECFNC()
{
	UnInit();
}

bool OSSPECFNC::Init()
{
	m_hUser32DLL = ::LoadLibraryA(("user32.dll"));

//	check(m_hUser32DLL != NULL);

	if (m_hUser32DLL)
	{
		m_pfnUpdateLayeredWindow = (PFN_UpdateLayeredWindow)::GetProcAddress(m_hUser32DLL, "UpdateLayeredWindow");
	}

	return true;
}

void OSSPECFNC::UnInit()
{
	if (m_hUser32DLL) 
	{
		::FreeLibrary(m_hUser32DLL);
		m_hUser32DLL = NULL;

		m_pfnUpdateLayeredWindow = NULL;
	}
}


int OSSPECFNC::UpdateLayeredWindow( HWND hwnd, HDC hdcDest, POINT *pptDst, SIZE *psize, HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags)
{
	if (m_pfnUpdateLayeredWindow) return m_pfnUpdateLayeredWindow(hwnd,hdcDest,pptDst,psize,hdcSrc, pptSrc, crKey, pblend, dwFlags);
	return 0;
}
