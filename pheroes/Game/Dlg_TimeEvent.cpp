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
#include "Dlg_TimeEvent.h"

iDlg_TimeEvent::iDlg_TimeEvent(iViewMgr* pViewMgr, PLAYER_ID pid, const iTimeEvent& timeEvent)
: iTextDlg(pViewMgr, _T(""), timeEvent.m_text + _T("\n\n#FFF0") + MineralSet2Text(timeEvent.m_minerals), pid), m_timeEvent(timeEvent)
{}

void iDlg_TimeEvent::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);
	iTextDlg::DoCompose(clRect);
	
	/*
	iSize ssiz = gGfxMgr.Dimension(m_sid);
	iPoint sa = gGfxMgr.Anchor(m_sid);
	rc.y = rc.y2() - 15 - 10 - ssiz.h;
	rc.x = rc.x + (rc.w/2 - ssiz.w/2);
	gGfxMgr.Blit(m_sid,gApp.Surface(),rc.point()-sa);
	*/
}

iSize iDlg_TimeEvent::ClientSize() const
{
	iSize res = iTextDlg::ClientSize();
	//iSize ssiz = gGfxMgr.Dimension(m_sid);
	//res.h += ssiz.h + 10;
	return res;
}

