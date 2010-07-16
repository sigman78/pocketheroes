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
#include "Dlg_SkillInfo.h"

iSecSkillInfoDlg::iSecSkillInfoDlg(iViewMgr* pViewMgr, SECONDARY_SKILLS skill, SECSKILL_LEVEL level, PLAYER_ID pid)
: iBaseGameDlg(pViewMgr, pid), m_skill(skill), m_level(level)
{
}

void iSecSkillInfoDlg::OnCreateDlg()
{
	iRect clRect = ClientRect();
	AddChild(new iTextButton(m_pMgr,this,iRect(clRect.x+(clRect.w/2-20),clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_OK, DRC_OK));
}

void iSecSkillInfoDlg::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);
	const iStringT& skName = gTextMgr[TRID_HSKILL_ESTATES+m_skill];
	const iStringT& skLevel = gTextMgr[TRID_HSKILL_LEVEL_BASIC+m_level];
	const iStringT& skDesc = gTextMgr[TRID_HSKILL_ESTATES_BAS_DESC + (m_skill*SSLVL_COUNT) + m_level];

	// Skill name
	if (!skName.Empty()) {
		sint32 h = gTextComposer.TextBoxOut(dlgfc_hdr, gApp.Surface(), skName, rc);
		rc.y += h;
	}

	// Level
	gTextComposer.TextOut(dlgfc_topic, gApp.Surface(), rc.point(), skLevel, rc, AlignTop);
	rc.y += 10;
	rc.y += 10;

	// Icon
	iRect orc(rc.x + (rc.w/2 - 18), rc.y, 36, 30);
	gApp.Surface().Darken25Rect(orc);
	ButtonFrame(gApp.Surface(),orc,0);
	BlitIcon(gApp.Surface(), PDGG_SEC_SKILLS + m_skill, orc);
	orc.InflateRect(1);
	FrameRoundRect(gApp.Surface(), orc, cColor_Black);
	rc.y += 32;


	// Description
	if (!skDesc.Empty()) {
		sint32 h = gTextComposer.TextBoxOut(dlgfc_plain, gApp.Surface(), skDesc, rc);
		rc.y += h;
		rc.y += 10;
	}
}

iSize iSecSkillInfoDlg::ClientSize() const
{
	sint32 w = 130;
	const iStringT& skName = gTextMgr[TRID_HSKILL_ESTATES+m_skill];
	const iStringT& skDesc = gTextMgr[TRID_HSKILL_ESTATES_BAS_DESC + (m_skill*SSLVL_COUNT) + m_level];

	if (skDesc.Length() > 20) w += iCLAMP<sint32>(0,70,skDesc.Length()/4);
	sint32 h = 0;

	// Skill name
	if (!skName.Empty()) {
		iSize s = gTextComposer.GetTextBoxSize(skName, w, dlgfc_hdr);
		h += s.h;
	}

	// Level
	h += 10;
	h += 10;

	// Icon
	h += 32;

	// Description
	if (!skDesc.Empty()) {
		iSize s = gTextComposer.GetTextBoxSize(skDesc, w, dlgfc_plain);
		h += s.h;
		h += 10;
	}

	// ok button
	h += DEF_BTN_HEIGHT + 5;

	return iSize(w, h);
}

void iSecSkillInfoDlg::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (cmd == CCI_BTNCLICK) EndDialog(uid);
}

