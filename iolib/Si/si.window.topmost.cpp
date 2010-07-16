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
#include "si.window.generic.h"
#include "si.window.basenw.h"
#include "si.window.topmost.h"
#include "si.winmgr.h"

bool iTopmostWindow::CreateTopmostWindow(const iRect &rect, bool layered, bool sticky, bool appwnd)
{
	m_Sticky = sticky;
	m_AppWindow = appwnd;

	uint32 style = WS_POPUP | WS_CLIPCHILDREN;
	uint32 exstyle = 0;
	
	if (!m_AppWindow) exstyle |= WS_EX_TOOLWINDOW;
	else exstyle |= WS_EX_APPWINDOW;

	if (layered) exstyle |= 0x00080000 /* means WS_EX_LAYERED*/;

	return CreateBaseWindowNW(rect,NULL,style,exstyle,layered);
}

void iTopmostWindow::MsgWindowPosChanging(LPWINDOWPOS wndpos)
{
	if ( !m_Sticky ) return;

	iRect wa = iWindowManager::GetWorkAreaRect();

	int STICKY=15;

    if (((wndpos->x - wa.x) < STICKY) && ((wndpos->x - wa.x) > -STICKY))
	{
		wndpos->x = wa.x;
	}
	else if (((wa.x2() + 1 - (wndpos->x+wndpos->cx)) < STICKY) && ((wa.x2() + 1 - (wndpos->x+wndpos->cx)) > -STICKY))
	{
		wndpos->x = wa.x2() + 1 - wndpos->cx;
	}

	if (((wndpos->y - wa.y) < STICKY) && ((wndpos->y - wa.y) > -STICKY))
	{
		wndpos->y = wa.y;
	}
    else if (((wa.y2() + 1 - (wndpos->y+wndpos->cy)) < STICKY) && ((wa.y2() + 1 - (wndpos->y+wndpos->cy)) > -STICKY))
	{
		wndpos->y = wa.y2() + 1 - wndpos->cy;
	}
}
