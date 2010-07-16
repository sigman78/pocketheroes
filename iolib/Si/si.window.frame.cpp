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
#include "si.window.frame.h"

bool iFrameWindow::CreateFrameWindow(const iRect &rect, bool layered, bool sticky, bool sizable)
{
	m_Sizable = sizable;

	// layered window can't be sizable
	if (layered && sizable)
	{
		iLOG(_T("Warning: Created Sizable Layered window!"));
	}

	return CreateTopmostWindow(rect, layered, sticky);
}

void iFrameWindow::SetMinMaxInfo(const iSize &minsiz, const iSize &maxsiz)
{
	m_MinSize = minsiz;
	m_MaxSize = maxsiz;
}

sint32 iFrameWindow::MsgNcHitTest(const iPoint &mpos)
{
	//if (m_WindowStatus == IOWINDOW_STATUS_MAXIMIZED) return HTCLIENT;
	iRect cr=GetWndRect();
	
	iRect r_b(0+cr.x,cr.h-10+cr.y,cr.w,10);
	iRect r_r(cr.w-10+cr.x,0+cr.y,10,cr.h);
	iRect r_l(0+cr.x,0+cr.y,10,cr.h);
	
	if ((mpos.y-cr.y)<18)	return HTCAPTION;
	else if (m_Sizable && r_b.PtInRect(mpos) && r_r.PtInRect(mpos)) return HTBOTTOMRIGHT;
	else if (m_Sizable && r_b.PtInRect(mpos) && r_l.PtInRect(mpos)) return HTBOTTOMLEFT;
	else if (m_Sizable && r_r.PtInRect(mpos)) return HTRIGHT;
	else if (m_Sizable && r_l.PtInRect(mpos)) return HTLEFT;
	else if (m_Sizable && r_b.PtInRect(mpos)) return HTBOTTOM;
	else return HTCLIENT;
}
