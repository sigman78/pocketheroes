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
#include "Dlg_SurrenderCond.h"

iDlg_SurrenderCond::iDlg_SurrenderCond(iViewMgr* pViewMgr, const iHero* pHero,  uint32 cost)
: iBaseGameDlg(pViewMgr,PID_NEUTRAL), m_pHero(pHero), m_cost(cost)
{
}

void iDlg_SurrenderCond::OnCreateDlg()
{
	iRect clRect = ClientRect();

	sint32 npos = clRect.x + (clRect.w/2-55);
	AddChild(new iTextButton(m_pMgr,this,iRect(npos,clRect.y2()-DEF_BTN_HEIGHT,50,DEF_BTN_HEIGHT),TRID_OK, DRC_OK));
	AddChild(new iTextButton(m_pMgr,this,iRect(npos+60,clRect.y2()-DEF_BTN_HEIGHT,50,DEF_BTN_HEIGHT),TRID_CANCEL, DRC_CANCEL));
}

void iDlg_SurrenderCond::DoCompose(const iRect& rect)
{
	iRect rc(rect);
	rc.DeflateRect(5);

	// Portrait
	ButtonFrame(gApp.Surface(),iRect(rc.x,rc.y,50,50),iButton::Pressed);
	gGfxMgr.Blit(m_pHero->Proto()->m_icn48,gApp.Surface(),iPoint(rc.x+1,rc.y+1));

	// text
	iRect trc(rc.x+57,rc.y,rc.w-55,50);
	gTextComposer.TextOut(dlgfc_topic, gApp.Surface(), trc.point(), iFormat(gTextMgr[TRID_SURRENDER_STATES],m_pHero->Name().CStr()), trc, AlignTop);
	trc.DeflateRect(0,13,0,0);
	gTextComposer.TextBoxOut(dlgfc_splain, gApp.Surface(), iFormat(gTextMgr[TRID_SURRENDER_CONDITION],m_cost), trc);
	/*
	rc.y += 15;
	rc.y += gTextComposer.TextBoxOut(dlgfc_splain, gApp.Surface(), m_scProps.m_Description, rc);
	*/
}

iSize iDlg_SurrenderCond::ClientSize() const
{
	return iSize(200, 65 + DEF_BTN_HEIGHT); 
}

void iDlg_SurrenderCond::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (uid == DRC_OK || uid == DRC_CANCEL) {
		EndDialog(uid); 
	}
}


