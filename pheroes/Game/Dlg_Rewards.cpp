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
#include "Dlg_Rewards.h"
#include "Dlg_ArtInfo.h"
#include "Dlg_CreatInfo.h"
#include "Dlg_MLInfo.h"

const sint32 BTNS_DIST[8][2] = {
	{ 1,1 },
	{ 2,1 },
	{ 3,1 },
	{ 4,1 },
	{ 3,2 },
	{ 3,2 },
	{ 4,2 },
	{ 4,2 }
};

//////////////////////////////////////////////////////////////////////////
struct iBtnEntry {
	iBtnEntry(SpriteId _sid, const iStringT& _text) : sid(_sid), text(_text) {}
	SpriteId	sid;
	iStringT	text;
};

//////////////////////////////////////////////////////////////////////////
inline iBtnEntry GetRewardBtnEntry(const iRewardItem& reward)
{
	switch(reward.m_type) {
	case RIT_MINERAL:
		return iBtnEntry(PDGG_RES_BIG_ICONS + reward.m_fParam, FormatNumber(reward.m_sParam,true));
		break;
	case RIT_EXPERIENCE:
		return iBtnEntry(PDGG_DGLYPH_EXP, FormatNumber(reward.m_sParam,true));
		break;
	case RIT_MANAPTS:
		return iBtnEntry(PDGG_DGLYPH_MANA, FormatNumber(reward.m_sParam, true));
		break;
	case RIT_TRAVELPTS:
		return iBtnEntry(PDGG_DGLYPH_MOVES, FormatNumber(reward.m_sParam,true));
		break;
	case RIT_MORALE:
		{
			SpriteId sid = PDGG_DGLYPH_MORALE;
			if (reward.m_sParam < 0) sid += 2;
			else if (reward.m_sParam == 0) sid += 1;
			return iBtnEntry( sid, FormatNumber(reward.m_sParam,true));
		}
		break;
	case RIT_LUCK:
		{
			SpriteId sid = PDGG_DGLYPH_LUCK;
			if (reward.m_sParam < 0) sid += 2;
			else if (reward.m_sParam == 0) sid += 1;
			return iBtnEntry(sid, FormatNumber(reward.m_sParam, true));
		}
		break;
	case RIT_FURTSKILL:
		return iBtnEntry(PDGG_DGLYPH_PRSKILLS + (reward.m_fParam - FSK_ATTACK), FormatNumber(reward.m_sParam,true));
		break;
	case RIT_ARTIFACT:
		return iBtnEntry(gGame.ItemMgr().m_artMgr[reward.m_fParam].Sprite(), _T(""));
		break;
	case RIT_MAGICSPELL:
		return iBtnEntry(PDGG_SPELL_ICONS + reward.m_fParam, _T(""));
		break;
	case RIT_CREATGROUP:
		return iBtnEntry(PDGG_MINIMON + reward.m_fParam, FormatNumber(reward.m_sParam));
		break;
	default:
		check(0);
	}
	return iBtnEntry(PDGG_BTN_CLOSE, _T("Invalid Entry"));
}

//////////////////////////////////////////////////////////////////////////
void ShowRewardDlg(const iRewardItem& reward, PLAYER_ID pid)
{
	switch(reward.m_type) {
	case RIT_MINERAL:
		{
			iIconDlg dlg(&gApp.ViewMgr(), gTextMgr[TRID_MINERAL_GOLD + reward.m_fParam], gTextMgr[TRID_INFO_MINERALS], PDGG_RES_BIG_ICONS + reward.m_fParam, pid);
			dlg.DoModal();
		}
		break;
	case RIT_EXPERIENCE:
		{
			iIconDlg dlg(&gApp.ViewMgr(), gTextMgr[TRID_EXPERIENCE], gTextMgr[TRID_INFO_EXPERIENCE], PDGG_DGLYPH_EXP, pid);
			dlg.DoModal();
		}
		break;
	case RIT_MANAPTS:
		{
			iIconDlg dlg(&gApp.ViewMgr(), gTextMgr[TRID_MANA], gTextMgr[TRID_INFO_MANA], PDGG_DGLYPH_MANA, pid);
			dlg.DoModal();
		}
		break;
	case RIT_TRAVELPTS:
		{
			iIconDlg dlg(&gApp.ViewMgr(), gTextMgr[TRID_TRAVELPTS], gTextMgr[TRID_INFO_TRAVELPTS], PDGG_DGLYPH_MOVES, pid);
			dlg.DoModal();
		}
		break;
	case RIT_MORALE:
		{
			iMLInfoDlg dlg(&gApp.ViewMgr(), false, reward.m_sParam, pid);
			dlg.DoModal();
		}
		break;
	case RIT_LUCK:
		{
			iMLInfoDlg dlg(&gApp.ViewMgr(), true, reward.m_sParam, pid);
			dlg.DoModal();
		}
		break;
	case RIT_FURTSKILL:
		{
			iIconDlg dlg(&gApp.ViewMgr(), gTextMgr[TRID_SKILL_ATTACK + (reward.m_fParam - FSK_ATTACK)], gTextMgr[TRID_INFO_ATTACK + (reward.m_fParam - FSK_ATTACK)], PDGG_DGLYPH_PRSKILLS + (reward.m_fParam - FSK_ATTACK), pid);
			dlg.DoModal();
		}
		break;
	case RIT_ARTIFACT:
		{
			iDlg_ArtInfo dlg(&gApp.ViewMgr(), reward.m_fParam, NULL);
			dlg.DoModal();
		}
		break;
	case RIT_MAGICSPELL:
		{
			iIconDlg dlg(&gApp.ViewMgr(), gTextMgr[TRID_SPNAME_MAGICARROW + reward.m_fParam], gTextMgr[TRID_SPDESC_MAGICARROW_NON + reward.m_fParam*MSL_COUNT], PDGG_SPELL_ICONS+reward.m_fParam, pid);
			dlg.DoModal();
		}
		break;
	case RIT_CREATGROUP:
		{
			iCreatGroup group((CREATURE_TYPE)reward.m_fParam, reward.m_sParam);
			iDlg_CreatInfo dlg(&gApp.ViewMgr(), pid, group, iFurtSkills(), false, 0);
			dlg.DoModal();
		}
		break;
	default:
		check(0);
	}
}
//////////////////////////////////////////////////////////////////////////
class iRewardBtn : public iButton, public IViewCmdHandler
{
public:
	iRewardBtn(iViewMgr* pViewMgr, const iRect& rect, uint32 uid, const iRewardItem& reward)
	: iButton(pViewMgr, this, rect, uid, Visible | Enabled), m_reward(reward), m_btnEntry(GetRewardBtnEntry(reward)) {}

	void OnCompose()
	{
		iRect orc = GetScrRect();
		
		//ComposeIconButton(gApp.Surface(), GetScrRect(), GetButtonState(), m_btnEntry.sid);
		Tile3x3Sprite(gApp.Surface(), GetScrRect(), PDGG_ART_CELLS);
		BlitIcon(gApp.Surface(), m_btnEntry.sid, GetScrRect());
		if (!m_btnEntry.text.Empty()) {
			orc.y += orc.h;
			orc.h = 11;
			iTextComposer::FontConfig fontcfg(iTextComposer::FS_MEDIUM, cColor_Yello);
			gTextComposer.TextOut( fontcfg ,gApp.Surface(), orc, m_btnEntry.text, orc, AlignCenter);
		}
	}

	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
	{
		uint32 uid = pView->GetUID();
		if (cmd == CCI_BTNCLICK) {
			ShowRewardDlg(m_reward, PID_NEUTRAL);
		}
	}


	const iRewardItem&	m_reward;
	iBtnEntry			m_btnEntry;
};

iDlg_Rewards::iDlg_Rewards(iViewMgr* pViewMgr, const iStringT& title, const iStringT& text, iHero* pHero, const iRewardsCtr& rewards)
: iBaseGameDlg(pViewMgr, pHero->Owner()->PlayerId()), m_title(title), m_text(text), m_pHero(pHero), m_rewards(rewards)
{
	check(rewards.GetSize() > 0 && rewards.GetSize() <= 8);
	
}

void iDlg_Rewards::OnCreateDlg()
{
	iRect clRect = ClientRect();

	sint32 xpos = clRect.x + (clRect.w/2 - BTNS_DIST[m_rewards.GetSize()-1][0] * 26);
	sint32 ypos = (sint32)(clRect.y + clRect.h) - 30 - BTNS_DIST[m_rewards.GetSize()-1][1] * 54;
	for (uint32 xx=0; xx<m_rewards.GetSize(); ++xx) {
		if (xx == BTNS_DIST[m_rewards.GetSize()-1][0]) {
			ypos += 54;
			xpos = clRect.x + (clRect.w/2 - (m_rewards.GetSize() - BTNS_DIST[m_rewards.GetSize()-1][0]) * 26);
		}
		AddChild(new iRewardBtn(m_pMgr, iRect(xpos+2, ypos+5, 48,40), 200+xx, m_rewards[xx]));
		xpos += 52;
	}

	AddChild(new iTextButton(m_pMgr,this,iRect(clRect.x+(clRect.w/2-20),clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_OK, DRC_OK));

}

void iDlg_Rewards::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);
	if (!m_title.Empty()) {
		sint32 h = gTextComposer.TextBoxOut(dlgfc_hdr, gApp.Surface(), m_title, rc);
		rc.y += h;
		rc.y += 10;
	}

	if (!m_text.Empty()) {
		sint32 h = gTextComposer.TextBoxOut(dlgfc_plain, gApp.Surface(), m_text, rc);
		rc.y += h;
		h += 10;
	}
}

iSize iDlg_Rewards::ClientSize() const
{
	sint32 w = iMAX<sint32>(180,BTNS_DIST[m_rewards.GetSize() - 1][0] * 54+20);
	sint32 h = 0;

	// title
	if (!m_title.Empty()) {
		iSize s = gTextComposer.GetTextBoxSize(m_title, w, dlgfc_hdr);
		h += s.h;
		h += 10;
	}

	// text
	if (!m_text.Empty()) {
		iSize s = gTextComposer.GetTextBoxSize(m_text, w, dlgfc_plain);
		h += s.h;
		h += 10;
	}

	// Icons
	h += BTNS_DIST[m_rewards.GetSize() - 1][1] * 54 + 10;

	// ok button
	h += DEF_BTN_HEIGHT;

	return iSize(w,h);
}

void iDlg_Rewards::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (cmd == CCI_BTNCLICK) {
		EndDialog(DRC_OK);
	}
}

bool iDlg_Rewards::MouseDown(const iPoint& pos)
{
	return true;
}

