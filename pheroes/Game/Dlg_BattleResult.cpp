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
#include "Dlg_BattleResult.h"

iDlg_BattleResult::iDlg_BattleResult(iViewMgr* pViewMgr, iBattleEngine& engine, iBattleResult br)
: iBaseGameDlg(pViewMgr,engine.GetBattleInfo().InteractorPlayer()), m_engine(engine), m_br(br)
{
	iBattleMember::Side is = engine.GetBattleInfo().InteractorSide();
	// title and text
	if ((br.m_result == BR_ASSAULTER_WIN && is == iBattleMember::Assaulter) || (br.m_result == BR_DEFENDER_WIN && is == iBattleMember::Defender)) {
		switch (br.m_defCause) {
		case DC_DEFEAT: m_Title = gTextMgr[TRID_MSG_BAT_VICTORY]; break;
		case DC_RETREAT: m_Title = gTextMgr[TRID_MSG_BAT_ENEMY_FLEES]; break;
		case DC_SURRENDER: m_Title = gTextMgr[TRID_MSG_BAT_ENEMY_SURREND]; break;
		default: check(0);
		}

		iBattleMember::Side ws = is;
		const iBattleArmy& wa = (ws == iBattleMember::Assaulter)?engine.AArmy():engine.DArmy();

		if (const iBattleMember_Hero* pHeroMember = DynamicCast<iBattleMember_Hero*>(engine.GetBattleInfo().Member(ws))){
			const iHero* pHero = pHeroMember->GetHero();
			m_Text.Setf(gTextMgr[TRID_MSG_BAT_RECEXP],pHero->Name().CStr(),br.m_experience);
		}

	} else {
		iBattleMember::Side ds = is;
		const iBattleArmy& da = (ds == iBattleMember::Assaulter)?engine.AArmy():engine.DArmy();

		if (const iBattleMember_Hero* pHeroMember = DynamicCast<iBattleMember_Hero*>(engine.GetBattleInfo().Member(ds))){
			const iHero* pHero = pHeroMember->GetHero();
			if (br.m_defCause == DC_DEFEAT) m_Text.Setf(gTextMgr[TRID_MSG_BAT_HERO_DEFEAT],pHero->Name().CStr());
			else if (br.m_defCause == DC_RETREAT) m_Text.Setf(gTextMgr[TRID_MSG_BAT_FLEES],pHero->Name().CStr());
			else if (br.m_defCause == DC_SURRENDER) m_Text.Setf(gTextMgr[TRID_MSG_BAT_SURREND],pHero->Name().CStr());
		} else {
			check(br.m_defCause == DC_DEFEAT);
			m_Text = gTextMgr[TRID_MSG_BAT_FORCES_DEFEAT];
		}
	}

	const iArmy& aArmy = engine.GetBattleInfo().m_pAssaulter->Army();
	for (uint32 aidx=0; aidx<engine.AArmy().Count(); ++aidx) {
		if (engine.AArmy()[aidx]->Casualties()) {
			m_aCas.Add(engine.AArmy()[aidx]->Type(), engine.AArmy()[aidx]->Casualties());
		}
	}

	const iArmy& dArmy = engine.GetBattleInfo().m_pDefender->Army();
	for (uint32 didx=0; didx<engine.DArmy().Count(); ++didx) {
		if (engine.DArmy()[didx]->Casualties()) {
			m_dCas.Add(engine.DArmy()[didx]->Type(), engine.DArmy()[didx]->Casualties());
		}
	}
}

void iDlg_BattleResult::OnCreateDlg()
{
	iRect clRect = ClientRect();
	AddChild(new iTextButton(m_pMgr,this,iRect(clRect.x+(clRect.w/2-20),clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_OK, DRC_OK));
}

void iDlg_BattleResult::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);

	// title
	if (!m_Title.Empty()) {
		gTextComposer.TextOut(dlgfc_hdr, gApp.Surface(),rc.point(),m_Title, iRect(rc.x,rc.y,rc.w,15),AlignCenter);
		rc.y+=15;
	}

	// text
	if (!m_Text.Empty()) {
		rc.y += gTextComposer.TextBoxOut(dlgfc_splain,gApp.Surface(),m_Text, rc);
	}

	// Battlefild casualties
	gTextComposer.TextOut(dlgfc_topic, gApp.Surface(),rc.point(), gTextMgr[TRID_BAT_CASUALTIES], iRect(rc.x,rc.y,rc.w,15),AlignCenter);
	rc.y += 18;

	// Attacker
	gTextComposer.TextOut(dlgfc_splain, gApp.Surface(),rc.point(), gTextMgr[TRID_BAT_ATTACKER], iRect(rc.x,rc.y,rc.w,15),AlignCenter);
	rc.y += 14;
	if (m_aCas.Count()) {
		sint32 xpos = rc.x + (sint32)(rc.w/2) - (sint32)((m_aCas.Count()*30)/2);
		for (uint32 xx=0; xx<m_aCas.Count(); ++xx) {
			ComposeCreatureCell(gApp.Surface(), iRect(xpos,rc.y,30,35), m_aCas[xx].Type(), m_aCas[xx].Count(), VL_EXPERT);
			xpos += 30;
		}
	} else {
		gTextComposer.TextOut(dlgfc_splain, gApp.Surface(),rc.point(), gTextMgr[TRID_NONE], iRect(rc.x,rc.y,rc.w,15),AlignCenter);
	}
	rc.y += 35;

	// Defender
	gTextComposer.TextOut(dlgfc_splain, gApp.Surface(),rc.point(), gTextMgr[TRID_BAT_DEFENDER], iRect(rc.x,rc.y,rc.w,15),AlignCenter);
	rc.y += 14;
	if (m_dCas.Count()) {
		sint32 xpos = rc.x + (sint32)(rc.w/2) - (sint32)((m_dCas.Count()*30)/2);
		for (uint32 xx=0; xx<m_dCas.Count(); ++xx) {
			ComposeCreatureCell(gApp.Surface(), iRect(xpos,rc.y,30,35), m_dCas[xx].Type(), m_dCas[xx].Count(), VL_EXPERT);
			xpos += 30;
		}
	} else {
		gTextComposer.TextOut(dlgfc_splain, gApp.Surface(),rc.point(), gTextMgr[TRID_NONE], iRect(rc.x,rc.y,rc.w,15),AlignCenter);
	}
	rc.y += 35;
}

iSize iDlg_BattleResult::ClientSize() const
{
	sint32 w = iMAX<sint32>(150, iMAX<sint32>(m_aCas.Count(),m_dCas.Count())*30);
	sint32 h = 0;

	// title
	if (!m_Title.Empty()) {
		w = iMAX<sint32>(w,gTextComposer.GetTextSize(m_Title,dlgfc_hdr).w);
		h += 15;
	}

	// text
	if (!m_Text.Empty()) {
		h += gTextComposer.GetTextBoxSize(m_Text, w, dlgfc_splain).h;
	}

	// Battlefild casualties
	h += 18;

	// Attacker
	h += 14;
	h += 35;

	// Defender
	h += 14;
	h += 35;

	// buttons
	h += DEF_BTN_HEIGHT + 10;
	return iSize(w,h);
}

void iDlg_BattleResult::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	if (cmd == CCI_BTNCLICK) {
		EndDialog(pView->GetUID());
	}
}

