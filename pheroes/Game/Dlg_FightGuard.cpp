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
#include "Dlg_FightGuard.h"

iDlg_FightGuard::iDlg_FightGuard(iViewMgr* pViewMgr, const iStringT& itemName, const iArmy& guard, PLAYER_ID pid, VISION_LEVEL vl)
: iBaseGameDlg(pViewMgr, pid), m_itemName(itemName), m_guard(guard), m_vl(vl)
{}

void iDlg_FightGuard::OnCreateDlg()
{
	iRect clRect = ClientRect();
	sint32 npos = clRect.x + (clRect.w/2-45);

	AddChild(new iTextButton(m_pMgr,this,iRect(npos,clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_YES, DRC_YES));
	AddChild(new iTextButton(m_pMgr,this,iRect(npos+50,clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_NO, DRC_NO));
}

void iDlg_FightGuard::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);

	// title
	sint32 th = gTextComposer.TextBoxOut(dlgfc_hdr, gApp.Surface(), m_itemName, iRect(rc.x+5,rc.y,rc.w-10,30))+5;

	// message
	th += gTextComposer.TextBoxOut(dlgfc_stopic, gApp.Surface(), iFormat(gTextMgr[TRID_OBJ_GUARDED_BY], m_itemName.CStr()), iRect(rc.x+5,rc.y+th,rc.w-10,30)) + 5;

	// guards
	for (uint32 xx=0; xx<7; ++xx) {
		if (m_guard[xx].IsValid()) {
			gTextComposer.TextOut(dlgfc_splain, gApp.Surface(), rc.point(), CreatGroup2Text(m_guard[xx].Type(),m_guard[xx].Count(), m_vl), iRect(rc.x+5,rc.y+th,rc.w-10,30), AlignTop);
			th += 10;
		}
	}
	th += 5;

	// question
	th += gTextComposer.TextBoxOut(dlgfc_topic, gApp.Surface(), gTextMgr[TRID_FIGHT_GUARD_QUESTION], iRect(rc.x+5,rc.y+th,rc.w-10,30));
}

iSize iDlg_FightGuard::ClientSize() const
{
	sint32 w = 150;
	sint32 h = 0;


	// title
	h += gTextComposer.GetTextBoxSize(m_itemName, w-10, dlgfc_hdr).h + 5;

	// message
	h += gTextComposer.GetTextBoxSize(iFormat(gTextMgr[TRID_OBJ_GUARDED_BY], m_itemName.CStr()), w-10, dlgfc_stopic).h + 5;

	// guards
	h += m_guard.GroupCount() * 10 + 5;

	// text 2
	h += gTextComposer.GetTextBoxSize(gTextMgr[TRID_FIGHT_GUARD_QUESTION], w-10, dlgfc_topic).h;

	// ok button
	h += DEF_BTN_HEIGHT + 15;

	return iSize(w,h);
}

void iDlg_FightGuard::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	if (cmd == CCI_BTNCLICK) {
		EndDialog(pView->GetUID());
	}
}

