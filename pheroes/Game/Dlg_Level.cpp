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
#include "Dlg_Level.h"
#include "Dlg_SkillInfo.h"

//////////////////////////////////////////////////////////////////////////
class iLevelDlgBtn : public iButton
{
public:
	iLevelDlgBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid, SECONDARY_SKILLS ss, SECSKILL_LEVEL sl)
	: iButton(pViewMgr, pCmdHandler, rect, uid, Visible | Enabled), m_ss(ss), m_sl(sl) {}

	static void ComposeLevelBtn(iDib& surf, iRect rc, uint32 state, SECONDARY_SKILLS ss, SECSKILL_LEVEL sl)
	{
		//ComposeTextButton(surf, rc, state, TRID_VOID);
		gGfxMgr.BlitTile(PDGG_BKTILE,surf,rc);
		surf.Darken25Rect(rc);
		ButtonFrame(surf,rc,state);
		BlitIcon(surf,PDGG_SEC_SKILLS+ss,iRect(rc.x+2,rc.y+2,32,24));
		gTextComposer.TextOut(surf,iPoint(rc.x+34,rc.y),iFormat(_T("#FFF5%s"),gTextMgr[TRID_HSKILL_ESTATES+ss]),iRect(rc.x+34,rc.y,rc.w-30,18),AlignLeft);
		gTextComposer.TextOut(surf,iPoint(rc.x+34,rc.y+13),iFormat(_T("#S0#FCCC(%s)"),gTextMgr[TRID_HSKILL_LEVEL_BASIC+sl]),iRect(rc.x+34,rc.y+13,rc.w-30,10),AlignLeft);
		rc.InflateRect(1);
		FrameRoundRect(surf, rc, cColor_Black);
	}

	void OnCompose()
	{
		ComposeLevelBtn(gApp.Surface(), GetScrRect(), GetButtonState(), m_ss, m_sl);
	}

	SECONDARY_SKILLS	m_ss;
	SECSKILL_LEVEL		m_sl;
};
//////////////////////////////////////////////////////////////////////////
class iSkillInfoBtn : public iButton, public IViewCmdHandler
{
public:
	iSkillInfoBtn(iViewMgr* pViewMgr, const iRect& rect, SECONDARY_SKILLS ss, SECSKILL_LEVEL sl, PLAYER_ID pid)
	: iButton(pViewMgr, this, rect, 200, Visible | Enabled), m_ss(ss), m_sl(sl), m_pid(pid) {}

	void OnCompose()
	{
		iRect orc(GetScrRect());
		gApp.Surface().Darken25Rect(orc);
		ButtonFrame(gApp.Surface(),orc,GetButtonState());
		BlitIcon(gApp.Surface(), PDGG_BTN_INFO, orc);
		orc.InflateRect(1);
		FrameRoundRect(gApp.Surface(), orc, cColor_Black);
	}

	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
	{
		if (cmd == CCI_BTNCLICK) {
			iSecSkillInfoDlg dlg(m_pMgr, m_ss, m_sl, m_pid);
			dlg.DoModal();
		}
	}


	PLAYER_ID			m_pid;
	SECONDARY_SKILLS	m_ss;
	SECSKILL_LEVEL		m_sl;
};


//////////////////////////////////////////////////////////////////////////
iDlg_NewLevel::iDlg_NewLevel(iViewMgr* pViewMgr, iHero* pHero, const iNewLevelInfo& linfo)
: iBaseGameDlg(pViewMgr,pHero->Owner()->PlayerId()), m_pHero(pHero), m_linfo(linfo)
{ }

void iDlg_NewLevel::OnCreateDlg()
{
	iRect clRect = ClientRect();

	sint32 cnt = 0;
	if (m_linfo.secSkill[0] != SECSK_NONE) cnt++;
	if (m_linfo.secSkill[1] != SECSK_NONE) cnt++;

	if (cnt == 2) {
		sint32 ypos = clRect.y2() - 61;
		AddChild(new iLevelDlgBtn(m_pMgr,this,iRect(clRect.x+5, ypos,clRect.w-36,28),101,m_linfo.secSkill[0], (SECSKILL_LEVEL)(m_pHero->SecSkills().SkillLevel(m_linfo.secSkill[0])+1)));
		AddChild(new iSkillInfoBtn(m_pMgr, iRect(clRect.x+clRect.w-30, ypos,25,28),m_linfo.secSkill[0], (SECSKILL_LEVEL)(m_pHero->SecSkills().SkillLevel(m_linfo.secSkill[0])+1), m_pHero->Owner()->PlayerId()));
		ypos += 33;
		AddChild(new iLevelDlgBtn(m_pMgr,this,iRect(clRect.x+5,ypos,clRect.w-36,28),102,m_linfo.secSkill[1], (SECSKILL_LEVEL)(m_pHero->SecSkills().SkillLevel(m_linfo.secSkill[1])+1)));
		AddChild(new iSkillInfoBtn(m_pMgr, iRect(clRect.x+clRect.w-30, ypos,25,28),m_linfo.secSkill[1], (SECSKILL_LEVEL)(m_pHero->SecSkills().SkillLevel(m_linfo.secSkill[1])+1), m_pHero->Owner()->PlayerId()));
	} else {
		AddChild(new iTextButton(m_pMgr,this,iRect(clRect.x+(clRect.w/2-20),clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_OK, DRC_OK));
	}
}

void iDlg_NewLevel::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);

	// Message
	gTextComposer.TextOut(dlgfc_topic, gApp.Surface(),rc.point() ,m_pHero->Name() + _T(" ") +iFormat(gTextMgr[TRID_MSG_REACHLEVEL],m_pHero->Level()), iRect(rc.x,rc.y,rc.w,12),AlignCenter);
	rc.y += 17;

	// Portrait
	iRect prc(rc);
	prc.DeflateRect(20,0,0,0);
	ButtonFrame(gApp.Surface(),iRect(prc.x,prc.y,34,22),true);
	gGfxMgr.Blit(m_pHero->Proto()->m_icn32,gApp.Surface(),iPoint(prc.x+1,prc.y+1));
	prc.DeflateRect(36,0,0,0);

	// Primary skills
	iStringT psText;
	for (sint32 psid=0; psid<PRSKILL_COUNT; ++psid) {
		psText.Addf(_T(" #I%04X %s%d"),PDGG_ICN_PSKILLS+psid,(psid==m_linfo.prSkillType)?_T("#FAF5"):_T("#FCCC"),m_pHero->FurtSkill((FURTHER_SKILLS)(FSK_ATTACK+psid)));
	}
	gTextComposer.TextOut(gApp.Surface(), iPoint(prc.x,rc.y), psText);
	prc.y += 11;
	gTextComposer.TextOut(dlgfc_splain, gApp.Surface(), iPoint(prc.x,prc.y),iFormat(_T(" %s +1"),gTextMgr[TRID_SKILL_ATTACK+m_linfo.prSkillType]));
	rc.y += 30;

	// Question
	if (m_linfo.secSkill[0] != SECSK_NONE && m_linfo.secSkill[1] != SECSK_NONE) {
		gTextComposer.TextBoxOut(dlgfc_stopic,gApp.Surface(),gTextMgr[TRID_SECSKILL_CHOOSE_MESSAGE], rc);
	} else if (m_linfo.secSkill[0] != SECSK_NONE) {
		check(m_linfo.secSkill[1] == SECSK_NONE);
		rc.y += gTextComposer.TextBoxOut(dlgfc_stopic,gApp.Surface(),gTextMgr[TRID_SECSKILL_LEARN_MESSAGE], rc) + 5;
		iLevelDlgBtn::ComposeLevelBtn(gApp.Surface(), iRect(rc.x + 5, rc.y, rc.w-10,30), iButton::Disabled, m_linfo.secSkill[0], (SECSKILL_LEVEL)(m_pHero->SecSkills().SkillLevel(m_linfo.secSkill[0])+1));
	}
}

iSize iDlg_NewLevel::ClientSize() const
{
	sint32 w = 160;
	sint32 h = 0;

	// Message
	h += 17;
	
	// Portrait and skills
	h += 30;

	// Question
	if (m_linfo.secSkill[0] != SECSK_NONE && m_linfo.secSkill[1] != SECSK_NONE)
		h += gTextComposer.GetTextBoxSize(gTextMgr[TRID_SECSKILL_CHOOSE_MESSAGE], w, dlgfc_stopic).h + 5;
	else if (m_linfo.secSkill[0] != SECSK_NONE || m_linfo.secSkill[1] != SECSK_NONE)
		h += gTextComposer.GetTextBoxSize(gTextMgr[TRID_SECSKILL_LEARN_MESSAGE], w, dlgfc_stopic).h + 5;

	// buttons
	if (m_linfo.secSkill[0] == SECSK_NONE && m_linfo.secSkill[1] == SECSK_NONE) h += DEF_BTN_HEIGHT + 10;
	else if (m_linfo.secSkill[0] != SECSK_NONE) h += 60;
	else h += 65;

	return iSize(w,h);
}

void iDlg_NewLevel::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (cmd == CCI_BTNCLICK) {
		if (uid == DRC_OK) m_selSecSkill = m_linfo.secSkill[0];
		else m_selSecSkill = ((iLevelDlgBtn*)pView)->m_ss;
		EndDialog(DRC_OK);
	}
}

