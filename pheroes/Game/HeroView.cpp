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
#include "HeroView.h"
#include "GfxHlp.h"
#include "Hero_Skills.h"
#include "Hero_Artifacts.h"
#include "Hero_Spells.h"

/*
 *	Common defines
 */
const SpriteId HVT_ICONS[HVT_COUNT] = {
	PDGG_TAB_SKILLS,
	PDGG_TAB_ARTIFACTS,
	PDGG_TAB_SPELLBOOK
	//PDGG_TAB_QUESTLOG
};

iHeroViewChild::iHeroViewChild(iViewMgr* pViewMgr)
: iView(pViewMgr,iRect(57,22,320-57,197),GENERIC_VIEWPORT,0,Enabled), m_pHero(NULL) {}

void iHeroViewChild::SetHero(iHero* pHero)
{
	m_pHero = pHero;
	OnHeroChanged();
}

void iHeroViewChild::OnCompose()
{
	iRect rc = GetScrRect();
	gGfxMgr.BlitTile(PDGG_BKTILE, gApp.Surface(),rc);
	gApp.Surface().Darken25Rect(rc);
}

/*
 *	
 */
class iQuestLogTab : public iHeroViewChild
{
public:
	iHero*	m_pHero;

	iQuestLogTab(iViewMgr* pViewMgr) : iHeroViewChild(pViewMgr){}
};

/*
 *	Hero view
 */
iHeroView::iHeroView(CHILD_VIEW parentView)
: iChildGameView(false, parentView), m_curTab(-1), m_pHero(NULL)
{
	AddChild(m_pChilds[0] = new iSkillsTab(&gApp.ViewMgr()));
	AddChild(m_pChilds[1] = new iArtifactsTab(&gApp.ViewMgr()));
	AddChild(m_pChilds[2] = new iSpellBookTab(&gApp.ViewMgr()));
	//AddChild(m_pChilds[3] = new iQuestLogTab(&gApp.ViewMgr()));

	AddChild(m_pPrSkillsView = new iHeroPrSkillView(&gApp.ViewMgr(), this, iRect(0,22,56,197)));
	AddChild(m_pTitleBar = new iHeroTitleBar(&gApp.ViewMgr(), this, iRect(0,0,m_Rect.w,22)));
	AddChild(m_pToolBar = new iHeroToolBar(&gApp.ViewMgr(), this, iRect(0,m_Rect.h-21,m_Rect.w,21)));

	SetHero(gGame.Map().CurHero());
	ShowPage(HVT_SKILLS);
}

iHeroView::~iHeroView()
{}


bool iHeroView::OnKeyDown(sint32 key)
{
	if (key == KEY_UP || key == KEY_DOWN) {
		if ( m_curTab == HVT_SPELLBOOK) {
			iSpellBookTab* pSBTab = (iSpellBookTab*)m_pChilds[HVT_SPELLBOOK];
			if (key == KEY_UP) pSBTab->SpellList()->SelPrev();
			else if (key == KEY_DOWN) pSBTab->SpellList()->SelNext();
		}
	} else if (key == KEY_LEFT) {
		PrevTab();
	} else if (key == KEY_RIGHT) {
		NextTab();
	} else {
		return false;
	}

	return true;
}

void iHeroView::ShowPage(uint32 nIdx)
{
	if (m_curTab != -1) m_pChilds[m_curTab]->SetVisible(false);
	m_pChilds[nIdx]->SetVisible(true);
	m_pChilds[nIdx]->OnActivate();
	m_curTab = nIdx;
}

void iHeroView::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	sint32 clsId = pView->GetClassId();
	uint32 uid = pView->GetUID();

	if (uid == 101) {
	} else if (uid == 103) {
		check(clsId == TABBED_SWITCH);
		if (cmd == CCI_TABCHANGED) ShowPage(param);
	} else if (uid == 104 && cmd == CCI_BTNCLICK) {
		iQuestDlg qdlg(m_pMgr, m_pHero->Name(), gTextMgr[TRID_MSG_HDISMISS], m_pHero->Owner()->PlayerId());
		if (qdlg.DoModal() == DRC_YES) {
			// Remove hero from the map
			gGame.Map().RemoveHero(m_pHero, true);
		}
	} else if (uid == VPUID_BTN_CLOSEHERO) {
		gGame.HideView(CV_HERO);
	} else if (uid == 150 && cmd == CCI_BTNCLICK) {
		iPoint pp = pView->GetScrPos();
		m_pMgr->TrackPopup(new iHeroListPopup(m_pMgr), pp, iRect(0,0,320,240),AlignBottomLeft);
	} else if (uid == 201) {
		gGame.Map().CurPlayer()->PrevHero();
	} else if (uid == 202) {
		gGame.Map().CurPlayer()->NextHero();
	}
}

bool iHeroView::Process(fix32 t)
{
	return true;
}

void iHeroView::OnCompose()
{
	gApp.Surface().FillRect(GetScrRect(),0);
}

void iHeroView::SetHero(iHero* pHero)
{
	m_pHero = pHero;
	for (uint32 xx=0; xx<HVT_COUNT; ++xx) m_pChilds[xx]->SetHero(m_pHero);
	m_pPrSkillsView->SetHero(m_pHero);
	m_pTitleBar->SetHero(m_pHero);
	m_pToolBar->SetHero(m_pHero, ParentView() != CV_OVERLAND);
}

void iHeroView::NextTab()
{
	sint32 ntab = m_curTab + 1;
	while (ntab >= 0 && ntab < m_pToolBar->m_pTabSwitch->GetTabsCount() && ntab != m_curTab) {
		if (m_pToolBar->m_pTabSwitch->IsTabEnabled(ntab)) {
			m_pToolBar->m_pTabSwitch->SetCurrentTab(ntab);
			ShowPage(ntab);
			break;
		}
		ntab++;
	}
}

void iHeroView::PrevTab()
{
	sint32 ntab = m_curTab - 1;
	while (ntab >= 0 && ntab < m_pToolBar->m_pTabSwitch->GetTabsCount() && ntab != m_curTab) {
		if (m_pToolBar->m_pTabSwitch->IsTabEnabled(ntab)) {
			m_pToolBar->m_pTabSwitch->SetCurrentTab(ntab);
			ShowPage(ntab);
			break;
		}
		ntab--;
	}
}

/*
 *	Primary Skills view
 */
iPrSkillBtn::iPrSkillBtn(iViewMgr* pViewMgr, const iRect& rect, FURTHER_SKILLS skill) 
: iButton(pViewMgr, this, rect, 500 + skill, Visible|Enabled), m_skill(skill), m_pHero(NULL) {}

void iPrSkillBtn::OnCompose()
{
	iRect rc = GetScrRect();
	//gApp.Surface().FrameRect(rc, cColor_Black);
	if (!m_pHero) return;
	SpriteId sid  = PDGG_DGLYPH_PRSKILLS + (m_skill-FSK_ATTACK);
	iSize siz = gGfxMgr.Dimension(sid);
	sint32 opx = rc.x + (sint32)(rc.w/2) - (sint32)(siz.w/2) - gGfxMgr.Anchor(sid).x;
	sint32 opy = rc.y - gGfxMgr.Anchor(sid).y + 1;
	gGfxMgr.Blit(sid, gApp.Surface(), iPoint(opx,opy));

	iRect trc(rc.x+(sint32)(rc.w/2)-14,rc.y+rc.h-13, 28,13);
	DrawRoundRect(gApp.Surface(), trc, RGB16(192,128,32), cColor_Black);

	iTextComposer::FontConfig fc(iTextComposer::FS_LARGE, RGB16(255,255,0));
	gTextComposer.TextOut(fc, gApp.Surface(), iPoint(), FormatNumber(m_pHero->FurtSkill(m_skill)), trc, AlignCenter);
}

void iPrSkillBtn::SetHero(iHero* pHero)
{
	m_pHero = pHero;
	Invalidate();
}

void iPrSkillBtn::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	iIconDlg dlg(m_pMgr, gTextMgr[TRID_SKILL_ATTACK + (m_skill - FSK_ATTACK)], gTextMgr[TRID_INFO_ATTACK + (m_skill - FSK_ATTACK)], PDGG_DGLYPH_PRSKILLS + (m_skill - FSK_ATTACK), m_pHero->Owner()->PlayerId());
	dlg.DoModal();
}

//////////////////////////////////////////////////////////////////////////
iHeroPrSkillView::iHeroPrSkillView(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect)
: iView(pViewMgr,rect,GENERIC_VIEWPORT,VPUID_HEROVIEWTITLELBAR,Visible|Enabled), m_pHero(NULL)
{
	sint32 ypos = 55;
	for (uint32 xx=0; xx<4; ++xx) {
		AddChild(m_pBtns[xx] = new iPrSkillBtn(pViewMgr, iRect(1,ypos,rect.w-2,35), (FURTHER_SKILLS)(FSK_ATTACK+xx)));
		ypos += 35;
	}
}

void iHeroPrSkillView::OnCompose()
{
	iRect rc = GetScrRect();
	gGfxMgr.BlitTile(PDGG_BKTILE, gApp.Surface(),rc);
	ButtonFrame(gApp.Surface(),rc,0);

	if (m_pHero) {
		// Portrait
		ButtonFrame(gApp.Surface(),iRect(rc.x+3,rc.y+3,50,50),iButton::Pressed);
		gGfxMgr.Blit(m_pHero->Proto()->m_icn48,gApp.Surface(),iPoint(rc.x+4,rc.y+4));
		rc.y += 55;
	}
}

void iHeroPrSkillView::SetHero(iHero* pHero)
{
	m_pHero = pHero;
	for (uint32 xx=0; xx<4; ++xx) {
		m_pBtns[xx]->SetHero(pHero);
	}
	Invalidate();
}

void iHeroPrSkillView::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
}

/*
 *	Hero Titlebar
 */
iHeroTitleBar::iHeroTitleBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect)
: iView(pViewMgr,rect,GENERIC_VIEWPORT,VPUID_HEROVIEWTITLELBAR,Visible|Enabled), m_pHero(NULL)
{
	AddChild(new iDlgIconButton(pViewMgr, pCmdHandler, iRect(0,1,32,rect.h-2),PDGG_BTN_PREV,201));
	AddChild(new iDlgIconButton(pViewMgr, pCmdHandler, iRect(rect.w-32,1,32,rect.h-2),PDGG_BTN_NEXT,202));
}

void iHeroTitleBar::OnCompose()
{
	iRect rect = GetScrRect();
	gApp.Surface().FillRect(rect,cColor_Black);
	rect.DeflateRect(33,1,33,1);

	gGfxMgr.BlitTile(PDGG_BKTILE,gApp.Surface(),rect);
	ComposeDlgButton(gApp.Surface(), rect, iButton::Disabled);
	if (m_pHero) {
		iStringT txt;
		txt.Addf(_T("#S0#FC90%s#S1#FFF0 %s #S0#FC90%s %d"),gTextMgr[TRID_HERO_TYPE_KNIGHT+m_pHero->Type()],m_pHero->Name().CStr(),gTextMgr[TRID_LEVEL],m_pHero->Level());
		gTextComposer.TextOut(gApp.Surface(),rect,txt,rect,AlignCenter);
	}
}

void iHeroTitleBar::SetHero(iHero* pHero)
{
	m_pHero = pHero;
	Invalidate();
}

/*
 *	Hero Toolbar
 */
iHeroToolBar::iHeroToolBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect)
: iView(pViewMgr,rect,GENERIC_VIEWPORT,VPUID_HEROVIEWTOOLBAR,Enabled|Visible), m_pHero(NULL)
{
	// Button with Popup
	AddChild(m_pHeroPortBtn = new iHeroPortBtn(pViewMgr, pCmdHandler, iRect(0,1,32,20), 150));

	// Tab switch
	m_pTabSwitch = new iBarTabSwitch(pViewMgr, pCmdHandler, iRect(33,1,HVT_COUNT*32,20), HVT_COUNT, 103);
	for (uint32 xx=0; xx<HVT_COUNT; ++xx) m_pTabSwitch->SetTabIcon(HVT_ICONS[xx]);
	m_pTabSwitch->SetCurrentTab(HVT_SKILLS);
	//m_pTabSwitch->EnableTab(HVT_QUESTLOG, false);
	AddChild(m_pTabSwitch);

	// Dismiss button
	AddChild(m_pDismissBtn = new iIconButton(pViewMgr, pCmdHandler, iRect(rect.w-61,1,30,20),PDGG_BTN_DISMISS,104));

	// Close button
	AddChild(new iIconButton(pViewMgr, pCmdHandler, iRect(rect.w-30,1,30,20),PDGG_BTN_CLOSE,VPUID_BTN_CLOSEHERO));
}

void iHeroToolBar::OnCompose()
{
	iRect rect = GetScrRect();
	sint32 rval = 33 + m_pTabSwitch->GetSize().w+1;
	iRect rc(rect.x+rval,rect.y+1,rect.w-(60+rval)-2,20);
	gGfxMgr.BlitTile(PDGG_BKTILE, gApp.Surface(), rc);
	ButtonFrame(gApp.Surface(), rc, 0);
}

void iHeroToolBar::SetHero(iHero* pHero, bool bReadOnly)
{
	m_pDismissBtn->SetEnabled(!bReadOnly && pHero && (pHero->Owner()->CastleCount() || pHero->Owner()->HeroCount() > 1));
	m_pHeroPortBtn->SetHero(pHero);
	m_pHero = pHero;
	Invalidate();
}

