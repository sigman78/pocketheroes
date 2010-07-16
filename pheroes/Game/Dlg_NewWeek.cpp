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
#include "Dlg_NewWeek.h"

iDlg_NewWeek::iDlg_NewWeek(iViewMgr* pViewMgr, PLAYER_ID pid, const iWeekDesc& weekDesc)
: iBaseGameDlg(pViewMgr,pid), m_weekDesc(weekDesc)
{
	if (weekDesc.bNewMonth) {
		m_title = gTextMgr[TRID_MSG_NEWMONTH];
		if (weekDesc.wkType == iWeekDesc::Plague) m_subTitle = gTextMgr[TRID_MSG_MONTHOFPLAGUE];
		else m_subTitle = iFormat(gTextMgr[TRID_MSG_MONTHOF], gTextMgr[TRID_MONTHN_01+weekDesc.fparam]);
	} else {
		m_title = gTextMgr[TRID_MSG_NEWWEEK];
		m_subTitle = iFormat(gTextMgr[TRID_MSG_WEEKOF], gTextMgr[TRID_WEEKN_01+weekDesc.fparam]);
	}

	if (weekDesc.wkType == iWeekDesc::Plague) m_text = gTextMgr[TRID_MSG_DWHALFPOP];
	else m_text = gTextMgr[TRID_MSG_DWINCPOP];

	if (weekDesc.wkType == iWeekDesc::CreatDoubles) m_text2 = iFormat(gTextMgr[TRID_MSG_DOUBLEPOP], gTextMgr[TRID_CREATURE_PEASANT_F3+weekDesc.crType*3]);
	else if (weekDesc.wkType == iWeekDesc::CreatPlus) m_text2 = iFormat(gTextMgr[TRID_MSG_PLUSPOP], gTextMgr[TRID_CREATURE_PEASANT_F3+weekDesc.crType*3]);
}

void iDlg_NewWeek::OnCreateDlg()
{
	iRect clRect = ClientRect();

	AddChild(new iTextButton(m_pMgr,this,iRect(clRect.x+(clRect.w/2-20),clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_OK, DRC_OK));
}

void iDlg_NewWeek::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);

	// title
	sint32 th = gTextComposer.TextBoxOut(dlgfc_hdr, gApp.Surface(), m_title, iRect(rc.x+5,rc.y,rc.w-10,30));

	// subtitle
	th += gTextComposer.TextBoxOut(dlgfc_stopic, gApp.Surface(), m_subTitle, iRect(rc.x+5,rc.y+th,rc.w-10,30)) + 5;

	// text 1
	th += gTextComposer.TextBoxOut(dlgfc_plain, gApp.Surface(), m_text, iRect(rc.x+5,rc.y+th,rc.w-10,30)) + 5;

	// text 1
	if (!m_text2.Empty()) th += gTextComposer.TextBoxOut(dlgfc_plain, gApp.Surface(), m_text2, iRect(rc.x+5,rc.y+th,rc.w-10,30)) + 5;
}

iSize iDlg_NewWeek::ClientSize() const
{
	sint32 w = 150;
	sint32 h = 0;


	// title
	h += gTextComposer.GetTextBoxSize(m_title, w-10, dlgfc_hdr).h;

	// subtitle
	h += gTextComposer.GetTextBoxSize(m_subTitle, w-10, dlgfc_stopic).h + 5;

	// text 1
	h += gTextComposer.GetTextBoxSize(m_text, w-10, dlgfc_plain).h + 5;

	// text 2
	if (!m_text2.Empty())
		h += gTextComposer.GetTextBoxSize(m_text2, w-10, dlgfc_plain).h + 5;

	// ok button
	h += DEF_BTN_HEIGHT+15;

	return iSize(w,h);
}

void iDlg_NewWeek::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	if (cmd == CCI_BTNCLICK) {
		EndDialog(pView->GetUID());
	}
}


