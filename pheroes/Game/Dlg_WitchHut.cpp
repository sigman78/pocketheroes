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
#include "Dlg_WitchHut.h"

iWitchHutDlg::iWitchHutDlg(iViewMgr* pViewMgr, SECONDARY_SKILLS skill, PLAYER_ID pid)
: iBaseGameDlg(pViewMgr, pid), m_skill(skill)
{}

void iWitchHutDlg::OnCreateDlg()
{
	iRect clRect = ClientRect();
	AddChild(new iTextButton(m_pMgr,this,iRect(clRect.x+(clRect.w/2-20),clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_OK, DRC_OK));
}

void iWitchHutDlg::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);

	// Title
	rc.y += gTextComposer.TextBoxOut(dlgfc_hdr, gApp.Surface(), gTextMgr[TRID_OBJ_WITCHHUT], rc) + 5;

	// Message
	iStringT msg = iFormat(gTextMgr[TRID_OBJ_WITCHHUT_MSG1], gTextMgr[TRID_HSKILL_ESTATES+m_skill]);
	rc.y += gTextComposer.TextBoxOut(dlgfc_plain, gApp.Surface(), msg, rc) + 10;

	// Icon
	iRect orc(rc.x + (rc.w/2 - 18), rc.y, 36, 30);
	gApp.Surface().Darken25Rect(orc);
	ButtonFrame(gApp.Surface(),orc,0);
	BlitIcon(gApp.Surface(), PDGG_SEC_SKILLS + m_skill, orc);
	orc.InflateRect(1);
	FrameRoundRect(gApp.Surface(), orc, cColor_Black);
	rc.y += 35;
}

iSize iWitchHutDlg::ClientSize() const
{
	sint32 w = 130;

	if (wcslen(gTextMgr[TRID_OBJ_WITCHHUT_MSG1]) > 20) w += iCLAMP<sint32>(0,70,wcslen(gTextMgr[TRID_OBJ_WITCHHUT_MSG1])/4);
	sint32 h = 0;

	// Title
	h += gTextComposer.GetTextBoxSize(gTextMgr[TRID_OBJ_WITCHHUT], w, dlgfc_hdr).h + 5;

	// Message
	iStringT msg = iFormat(gTextMgr[TRID_OBJ_WITCHHUT_MSG1], gTextMgr[TRID_HSKILL_ESTATES+m_skill]);
	h += gTextComposer.GetTextBoxSize(msg, w, dlgfc_plain).h + 10;

	// Icon
	h += 35;

	// ok button
	h += DEF_BTN_HEIGHT + 5;

	return iSize(w, h);
}

void iWitchHutDlg::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (cmd == CCI_BTNCLICK) EndDialog(uid);
}


