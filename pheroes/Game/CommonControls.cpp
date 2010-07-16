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
#include "GfxHlp.h"
#include "Dlg_Settings.h"
#include "helpers.h"


//////////////////////////////////////////////////////////////////////////
iFramePopupView::iFramePopupView(iViewMgr* pViewMgr, PLAYER_ID pid)
: iPopupView(pViewMgr), m_pid(pid)
{}

void iFramePopupView::OnCompose()
{
	ComposeDlgBkgnd(gApp.Surface(), m_Rect, m_pid, false);
	iRect rc(m_Rect);
	rc.DeflateRect(12);
	DoCompose(rc);
}

iSize iFramePopupView::PopupViewSize()
{
	iSize result = ClientSize();
	result.w += 24;
	result.h += 24;
	return result;
}

iSize iFramePopupView::PopupViewMinSize()
{
	return iSize(80,40);
}

/*
 *	Common controls
 */
iPHLabel::iPHLabel(iViewMgr* pViewMgr, const iRect& rect, const iStringT& text, Alignment align, iTextComposer::FontConfig fc)
: iBaseCtrl(pViewMgr, NULL, rect, GENERIC_VIEWPORT, 0, Visible), m_text(text), m_align(align), m_fc(fc) {}

void iPHLabel::OnCompose()
{
	iRect rc(GetScrRect());
	iTextComposer::FontConfig fc = m_fc;
	gTextComposer.TextOut(fc, gApp.Surface(), rc.point(), m_text, rc, m_align);
}

//////////////////////////////////////////////////////////////////////////
iPHTextBox::iPHTextBox(iViewMgr* pViewMgr, const iRect& rect, const iStringT& text, iTextComposer::FontConfig fc)
: iBaseCtrl(pViewMgr, NULL, rect, GENERIC_VIEWPORT, 0, Visible), m_text(text), m_fc(fc) {}

void iPHTextBox::OnCompose()
{
	iRect rc(GetScrRect());
	iTextComposer::FontConfig fc = m_fc;
	gTextComposer.TextBoxOut(fc, gApp.Surface(), m_text, rc);
}

//////////////////////////////////////////////////////////////////////////
iTBSplitter::iTBSplitter(iViewMgr* pViewMgr, const iRect& rect)
: iBaseCtrl(pViewMgr, NULL, rect, GENERIC_VIEWPORT, 0, Visible)
{}

void iTBSplitter::OnCompose()
{
	iRect rc(GetScrRect());
	gGfxMgr.BlitTile(PDGG_BKTILE, gApp.Surface(), rc);
	ButtonFrame(gApp.Surface(),rc,0);
	gApp.Surface().Darken25Rect(rc);
}
//////////////////////////////////////////////////////////////////////////
iTextButton::iTextButton(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, TextResId textKey, uint32 uid, uint32 state)
: iButton(pViewMgr, pCmdHandler, rect, uid, state), m_TextKey(textKey)
{}

void iTextButton::OnBtnDown() const
{
//	gSfxMgr.PlaySound(CSND_BUTTON);
}

void iTextButton::OnCompose()
{
	ComposeDlgTextButton(gApp.Surface(),GetScrRect(),GetButtonState(),m_TextKey);
}

//////////////////////////////////////////////////////////////////////////
iIconButton::iIconButton(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, SpriteId sid, uint32 uid, uint32 state)
: iButton(pViewMgr, pCmdHandler, rect, uid, state), m_spriteId(sid)
{}

void iIconButton::OnBtnDown() const
{
//	gSfxMgr.PlaySound(CSND_BUTTON);
}

void iIconButton::OnCompose()
{
	ComposeIconButton(gApp.Surface(),GetScrRect(),GetButtonState(),m_spriteId);
}

//////////////////////////////////////////////////////////////////////////
iDlgIconButton::iDlgIconButton(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, SpriteId sid, uint32 uid, uint32 state)
: iButton(pViewMgr, pCmdHandler, rect, uid, state), m_spriteId(sid)
{}

void iDlgIconButton::OnBtnDown() const
{
//	gSfxMgr.PlaySound(CSND_BUTTON);
}

void iDlgIconButton::OnCompose()
{
	ComposeDlgIconButton(gApp.Surface(),GetScrRect(),GetButtonState(),m_spriteId);
}

//////////////////////////////////////////////////////////////////////////
iCheckButton::iCheckButton(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, SpriteId sid, uint32 uid, uint32 state, bool bChecked)
: iButton(pViewMgr, this, rect, uid, state), m_pOwnCmdHandler(pCmdHandler), m_spriteId(sid), m_bChecked(bChecked)
{}

void iCheckButton::SetChecked(bool bChecked, bool bNotify) 
{ 
	m_bChecked = bChecked; 
	if (bNotify) m_pOwnCmdHandler->iCMDH_ControlCommand(this, CCI_CHECKED, m_bChecked);
}

void iCheckButton::OnCompose()
{
	uint32 state = GetButtonState();
	if (m_bChecked) state |= Selected;
	iRect rc = GetScrRect();
	ComposeIconButton(gApp.Surface(),rc,state,m_spriteId);
	rc.InflateRect(1);
	gApp.Surface().FrameRect(rc,cColor_Black);
}

void iCheckButton::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	if (cmd == CCI_BTNCLICK) SetChecked(!m_bChecked);
	m_pOwnCmdHandler->iCMDH_ControlCommand(pView, cmd, param);
}

//////////////////////////////////////////////////////////////////////////
iCheckBox::iCheckBox(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, const iStringT& text, uint32 uid, uint32 state, bool bChecked)
: iButton(pViewMgr, this, rect, uid, state), m_pOwnCmdHandler(pCmdHandler), m_text(text), m_bChecked(bChecked)
{}

void iCheckBox::SetChecked(bool bChecked, bool bNotify) 
{ 
	m_bChecked = bChecked; 
	if (bNotify) m_pOwnCmdHandler->iCMDH_ControlCommand(this, CCI_CHECKED, m_bChecked);
}

void iCheckBox::OnCompose()
{
	uint32 state = GetButtonState();
	if (m_bChecked) state |= Selected;
	iRect rc = GetScrRect();

	iRect icnRect(rc.x, rc.y+1, 11,11);
	DrawRoundRect(gApp.Surface(), icnRect, cColor_Black, RGB16(63,32,0));
	icnRect.DeflateRect(1);
	/*
	gApp.Surface().FrameRect(icnRect, cColor_Black);
	icnRect.DeflateRect(1);
	gApp.Surface().Darken50Rect(icnRect);
	*/
	
	if (m_bChecked) BlitIcon(gApp.Surface(), PDGG_CHECKBOX, icnRect);
	if (state & Pressed) gApp.Surface().Darken25Rect(icnRect);

	iTextComposer::FontConfig fc(GetButtonFont(state));
	gTextComposer.TextOut(fc,gApp.Surface(),iPoint(rc.x+14, rc.y), m_text, iRect(rc.x+14, rc.y, rc.w-14, rc.h),AlignTopLeft);
}

void iCheckBox::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	if (cmd == CCI_BTNCLICK) SetChecked(!m_bChecked);
	m_pOwnCmdHandler->iCMDH_ControlCommand(pView, cmd, m_bChecked);
}
//////////////////////////////////////////////////////////////////////////
iPHScrollBar::iPHScrollBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid, uint32 flags)
: iScrollBar(pViewMgr, pCmdHandler, rect, uid, flags)
{}

void iPHScrollBar::ComposeSBElement(Element el, const iRect& rc, uint32 flags)
{ 
	if (el == EL_Bkgnd) {
		gGfxMgr.BlitTile(PDGG_BKTILE, gApp.Surface(), rc);
		gApp.Surface().Darken50Rect(rc);
		gApp.Surface().FrameRect(iRect(rc.x-1,rc.y-1,rc.w+2,rc.h+2),cColor_Black);
	} else if (el == EL_Thumb) {
		SpriteId bksid = (!IsEnabled())?PDGG_BKTILE:PDGG_CTILE;
		gGfxMgr.BlitTile(bksid, gApp.Surface(), rc);
		ComposeDlgButton(gApp.Surface(), rc, (flags&iButton::Disabled)?iButton::Disabled:0);
	} else {
		iRect orc(rc);
		if (el == EL_BtnUp) {
			orc.h -= 1;
			ComposeDlgIconButton(gApp.Surface(), orc, flags, PDGG_SCRBAR_BTNS+0);
		} else if (el == EL_BtnDown) {
			orc.y += 1;
			orc.h -= 1;
			ComposeDlgIconButton(gApp.Surface(), orc, flags, PDGG_SCRBAR_BTNS+1);
		} else if (el == EL_BtnLeft) {
			orc.w -= 1;
			ComposeDlgIconButton(gApp.Surface(), orc, flags, PDGG_SCRBAR_BTNS+2);
		} else if (el == EL_BtnRight) {
			orc.x += 1;
			orc.w -= 1;
			ComposeDlgIconButton(gApp.Surface(), orc, flags, PDGG_SCRBAR_BTNS+3);
		} else {
			check(0);
		}
	}
}

// Mini slider (used as slider)
iPHMiniSlider::iPHMiniSlider(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid)
: iScrollBar(pViewMgr, pCmdHandler, rect, uid, Horizontal)
{}

void iPHMiniSlider::ComposeSBElement(Element el, const iRect& rc, uint32 flags)
{ 
	if (el == EL_Bkgnd) {
		gGfxMgr.BlitTile(PDGG_BKTILE, gApp.Surface(), rc);
		gApp.Surface().Darken50Rect(rc);
		gApp.Surface().FrameRect(iRect(rc.x-1,rc.y-1,rc.w+2,rc.h+2),cColor_Black);
	} else if (el == EL_Thumb) {
		SpriteId bksid = (!IsEnabled())?PDGG_BKTILE:PDGG_CTILE;
		gGfxMgr.BlitTile(bksid, gApp.Surface(), rc);
		ComposeDlgButton(gApp.Surface(), rc, 0);
		//gApp.Surface().FrameRect(iRect(rc.x-1,rc.y-1,rc.w+2,rc.h+2),cColor_Black);
	} else {
		iRect orc(rc);
		if (el == EL_BtnLeft) {
			orc.w -= 1;
			ComposeDlgIconButton(gApp.Surface(), orc, flags, PDGG_MINISLD_BTNS+1);
		} else if (el == EL_BtnRight) {
			orc.x += 1;
			orc.w -= 1;
			ComposeDlgIconButton(gApp.Surface(), orc, flags, PDGG_MINISLD_BTNS+0);
		} else {
			check(0);
		}
	}
}


/*
 *	Specialized controls
 */

// Tool bar tabbed switch
iBarTabSwitch::iBarTabSwitch(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 tabcnt, uint32 uid)
: iTabbedSwitch(pViewMgr, pCmdHandler, rect, tabcnt, uid) 
{}

void iBarTabSwitch::SetTabIcon(SpriteId sid)
{
	m_Tabs.Add(sid);
}

void iBarTabSwitch::ComposeTabItem(uint32 idx, uint32 itemState, const iRect& rect)
{
	ComposeIconButton(gApp.Surface(),rect,itemState,m_Tabs[idx]);
	if (!IsEnabled()) gApp.Surface().Darken25Rect(rect);
}


// Hero portrait button
iHeroPortBtn::iHeroPortBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid)
: iButton(pViewMgr, pCmdHandler, rect, uid, Visible), m_pHero(NULL) {}

void iHeroPortBtn::OnCompose()
{
	iRect rc = GetScrRect();
	SpriteId sid;
	if (m_pHero) sid = m_pHero->Proto()->m_icn32;
	else sid = PDGG_UNK_HERO;
	gGfxMgr.Blit(sid,gApp.Surface(), rc);
	ButtonFrame(gApp.Surface(),rc,GetButtonState());
	if (!IsEnabled()) gApp.Surface().Darken25Rect(rc);
}

void iHeroPortBtn::SetHero(iHero* pHero)
{
	m_pHero = pHero;
	SetEnabled(m_pHero != NULL);
	Invalidate();
}

// Castle glyph button
iCastleGlyphBtn::iCastleGlyphBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid)
: iButton(pViewMgr, pCmdHandler, rect, uid, Visible) {}

void iCastleGlyphBtn::OnCompose()
{
	iRect rc = GetScrRect();
	SpriteId sid;
	if (m_pCastle) sid = PDGG_CTL_SICONS + (m_pCastle->Proto()->Size()-1)*CTLT_COUNT + m_pCastle->Proto()->Type();
	else sid = PDGG_UNK_CASTLE;
	gGfxMgr.Blit(sid,gApp.Surface(), rc);
	if (!m_pCastle->CanBuild()) {
		gGfxMgr.Blit(PDGG_ICN_BUILT,gApp.Surface(), iPoint(rc.x+23,rc.y+11));
	}
	ButtonFrame(gApp.Surface(),rc,GetButtonState());
	if (!IsEnabled()) gApp.Surface().Darken25Rect(rc);
}

void iCastleGlyphBtn::SetCastle(iCastle* pCastle)
{
	m_pCastle = pCastle;
	SetEnabled(m_pCastle != NULL);
	Invalidate();
}

// Spell button
iSpellBtn::iSpellBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid)
: iButton(pViewMgr, pCmdHandler, rect, uid, Visible), m_pSpell(NULL) {}

void iSpellBtn::OnCompose()
{
	iRect rc = GetScrRect();
	if (m_pSpell){
		BlitIcon(gApp.Surface(), PDGG_SPELLSCROLL_UNROLLED, rc);
		BlitIcon(gApp.Surface(), m_pSpell->Icon(), rc);
	} else {
		BlitIcon(gApp.Surface(), PDGG_SPELLSCROLL_ROLLED, rc);
	}
}

// Popup list box
iPopupList::iPopupList(iViewMgr* pViewMgr, PopupListHandler* pHandler, const iSize& itemMetrics, sint32 itemsCount, sint32 selItem, sint32 pageSize)
: iPopupView(pViewMgr), m_pHandler(pHandler), m_itemMetrics(itemMetrics), m_itemsCount(itemsCount), m_selItem(selItem), m_pageSize(pageSize), m_sidx(-1), m_scrPos(0)
{}

iSize iPopupList::PopupViewSize()
{
	uint32 w = m_itemMetrics.w + 2;
	if (m_itemsCount>m_pageSize) w += 16;
	return iSize(w, m_itemMetrics.h * iMIN<uint32>(m_pageSize,m_itemsCount) + 2);
}

iSize iPopupList::PopupViewMinSize()
{
	return iSize(10,10);
}

void iPopupList::OnTrackPopup(const iRect& clRect)
{
	if (m_itemsCount > m_pageSize) {
		AddChild(m_pScrollBar = new iPHScrollBar(m_pMgr, this, iRect((sint32)(clRect.w)-16,1,15,clRect.h-2), 1024, 0));
		m_pScrollBar->SetMetrics(m_itemsCount, m_pageSize);
		if (m_selItem >= m_pageSize) m_scrPos = (m_selItem-m_pageSize) + 1;
		m_pScrollBar->SetCurPos(m_scrPos);
	}
}

void iPopupList::OnHidePopup()
{
}

void iPopupList::OnMouseDown(const iPoint& pos) 
{
	sint32 nidx = m_scrPos + (pos.y - m_Rect.y - 3) / (sint32)m_itemMetrics.h;
	if (nidx >= 0 && nidx < m_itemsCount) m_sidx = nidx;
	Invalidate();
}

void iPopupList::OnMouseClick(const iPoint& pos)
{
	sint32 nidx = m_scrPos + (pos.y - m_Rect.y - 3) / (sint32)m_itemMetrics.h;
	if (nidx >= 0 && nidx < m_itemsCount && m_sidx == nidx) {
		sint32 curIdx = m_sidx;
		PopupListHandler* pHandler = m_pHandler;
		m_pMgr->HidePopup();
		(*pHandler)(curIdx);
	} else m_sidx = -1;
}

void iPopupList::OnCompose()
{
	iRect rc(GetScrRect());
	// frame and bkgnd
	gApp.Surface().FrameRect(rc, cColor_Black);
	rc.DeflateRect(1);
	gGfxMgr.BlitTile(PDGG_BKTILE, gApp.Surface(), rc);

	// Items
	iRect brc(rc.x,rc.y,m_itemMetrics.w, m_itemMetrics.h);
	for (sint32 xx=m_scrPos; xx<iMIN<uint32>(m_scrPos+m_pageSize,m_itemsCount); ++xx) {
		DrawItem(xx, gApp.Surface(), brc, ((xx == m_sidx)?iButton::Pressed:0)|((xx == m_selItem)?iButton::Selected:0));
		brc.y += m_itemMetrics.h;
	}
}
void iPopupList::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	check(pView == m_pScrollBar);
	m_scrPos = param;
	Invalidate();
}

// Hero list popup
static void HeroListHandler(sint32 itemId)
{
	gGame.Map().ActPlayer()->SetCurHero( *(gGame.Map().ActPlayer()->HeroFirst() + itemId) );
}

iHeroListPopup::iHeroListPopup(iViewMgr* pViewMgr)
: iPopupList(pViewMgr, HeroListHandler, iSize(165,26), gGame.Map().ActPlayer()->HeroCount(), gGame.Map().ActPlayer()->CurHeroIdx(), 7)
{
}

void iHeroListPopup::DrawItem(sint32 itemId, iDib& surf, const iRect& rc, uint32 flags)
{
	iPlayer* pPlayer = gGame.Map().ActPlayer();
	iHero* pHero = *(pPlayer->HeroFirst() + itemId);
	ButtonFrame(surf,rc,0 );
	iRect frc(rc);
	if (flags & iButton::Selected) {
		gGfxMgr.BlitTile(PDGG_CTILE, surf,frc);
		ButtonFrame(surf,frc,0);
	}
	frc.DeflateRect(1);

	iTextComposer::FontConfig fc_name(iTextComposer::FS_MEDIUM, RGB16(255,255,0));
	// portrait
	surf.FillRect(iRect(frc.x+1,frc.y+1,44,22),cColor_Black);
	SpriteId sid = pHero->Proto()->m_icn32;
	gGfxMgr.Blit(sid,surf, iPoint(frc.x+2,frc.y+2));

	// act points and mana
	ComposeProgressBar(true, gApp.Surface(), iRect(frc.x+35,frc.y+2,4,20), cColor_Green192, pHero->Moves(), 250);
	ComposeProgressBar(true, gApp.Surface(), iRect(frc.x+40,frc.y+2,4,20), RGB16(32,128,255), pHero->ManaPts(), 100);

	// name
	gTextComposer.TextOut(fc_name, gApp.Surface(), iPoint(frc.x+42,frc.y-1), pHero->Name(), iRect(frc.x+42,frc.y,frc.w-42,frc.h),AlignTop);

	// Primary skills
	iStringT psText;
	for (sint32 psid=0; psid<PRSKILL_COUNT; ++psid) {
		sint32 sval = pHero->FurtSkill((FURTHER_SKILLS)(FSK_ATTACK+psid));
		psText.Addf(_T("#I%04X %d "),PDGG_ICN_PSKILLS+psid, sval);
		if (sval <= 10) psText += _T(" ");
	}
	gTextComposer.TextOut(gApp.Surface(), iPoint(frc.x+42,frc.y+12), psText, iRect(frc.x+42,frc.y+12,frc.w-42,frc.h-12),AlignTop);
}

// Castle list popup
static void CastleListHandler(sint32 itemId)
{
	gGame.Map().ActPlayer()->SetCurCastle( *(gGame.Map().ActPlayer()->CastleFirst() + itemId) );
}

iCastleListPopup::iCastleListPopup(iViewMgr* pViewMgr)
: iPopupList(pViewMgr, CastleListHandler, iSize(151,26), gGame.Map().ActPlayer()->CastleCount(), gGame.Map().ActPlayer()->CurCastleIdx(), 7)
{}

void iCastleListPopup::DrawItem(sint32 itemId, iDib& surf, const iRect& rc, uint32 flags)
{
	iPlayer* pPlayer = gGame.Map().ActPlayer();
	iCastle* pCastle = *(pPlayer->CastleFirst() + itemId);
	ButtonFrame(surf,rc,0 );
	iRect frc(rc);
	if (flags & iButton::Selected) {
		gGfxMgr.BlitTile(PDGG_CTILE, surf,frc);
		ButtonFrame(surf,frc,0);
	}
	frc.DeflateRect(1);

	iTextComposer::FontConfig fc_name(iTextComposer::FS_MEDIUM, RGB16(255,255,0));
	// glyph
	surf.FillRect(iRect(frc.x+1,frc.y+1,34,22),cColor_Black);
	SpriteId sid = PDGG_CTL_SICONS + (pCastle->Proto()->Size()-1)*CTLT_COUNT + pCastle->Proto()->Type();
	gGfxMgr.Blit(sid, surf, iPoint(frc.x+2,frc.y+2));
	if (!pCastle->CanBuild()) {
		gGfxMgr.Blit(PDGG_ICN_BUILT, surf, iPoint(frc.x+26,frc.y+14));
	}

	// name
	gTextComposer.TextOut(fc_name, surf, iPoint(), pCastle->Name(), iRect(frc.x+34,frc.y,frc.w-34,frc.h),AlignTop);

	// income
	iMineralSet ms; ms.Reset();
	ms.quant[MINERAL_GOLD] = CTL_INCOME;
	for (uint32 xx=0; xx<pCastle->Cnsts().Count(); ++xx) {
		if (iMineCtlCnst* pMine = DynamicCast<iMineCtlCnst*>(pCastle->Cnst(xx))) {
			if (pMine->IsBuilt()) ms.quant[pMine->m_mt] += pMine->m_income;
		}
	}

	gTextComposer.TextOut(dlgfc_plain, gApp.Surface(), iPoint(), MineralSet2Text(ms), iRect(frc.x+34,frc.y+12,frc.w-34,frc.h),AlignTop);

}

// Game Popup menu
#include "CommonDialogs.h"
#include "GameMenuDlg.h"
#include "Dlg_Save.h"
#include "Dlg_ScenProps.h"
#include "Dlg_PuzzleMap.h"
#include "Dlg_KingdomOverview.h"

enum GameMenuItems {
	GMI_KOVERVIEW,
	GMI_PUZZLEMAP,
	GMI_SETTINGS,
	GMI_GAMEMENU,
	GMI_COUNT
};

const TextResId GameMenuItemText[GMI_COUNT] = {
	TRID_KINGDOM_OVERVIEW,
	TRID_PUZZLE_MAP,
	TRID_MENU_GAMESETTINGS,
	TRID_SYSTEM_MENU
};

const SpriteId GameMenuItemIcons[GMI_COUNT] = {
	PDGG_BTN_CASTLE,
	PDGG_BTN_PUZZLEMAP,
	PDGG_BTN_MAINMENU,
	PDGG_BTN_DISK
};

static void GameMenuHandler(sint32 itemId)
{
	if (itemId == GMI_KOVERVIEW) {
		iKigdomOverviewDlg kodlg(&gApp.ViewMgr(), *gGame.Map().ActPlayer());
		kodlg.DoModal();
	} else if (itemId == GMI_PUZZLEMAP){
		iPuzzleMapDlg pmdlg(&gApp.ViewMgr(), *gGame.Map().ActPlayer(), 0);
		pmdlg.DoModal();
	} else if (itemId == GMI_SETTINGS){
		iSettingsDlg sdlg(&gApp.ViewMgr());
		sdlg.DoModal();
	} else if (itemId == GMI_GAMEMENU){
		iGameMenuDlg gmdlg(&gApp.ViewMgr());
		sint32 res = gmdlg.DoModal();
		if (res == GMRC_SAVE) {
			// Save game
			iSaveDlg sdlg(&gApp.ViewMgr(), true);
			if (sdlg.DoModal() == DRC_OK) {
				iStringT fname;
				sdlg.SelFile(fname);
				if ( SaveGameFile( fname, gGame.Map() ) ) {
					gGame.AddMsg(iStringT(_T("#F4B4")) + gTextMgr[TRID_MSG_GAME_SAVED]);
				}

			}
		} else if (res == GMRC_LOAD) {
			// Load game
			iQuestDlg qdlg(&gApp.ViewMgr(), iStringT(), gTextMgr[TRID_CONFIRM_RELOAD], PID_NEUTRAL);
			if (qdlg.DoModal() == DRC_YES) {
				iSaveDlg sdlg(&gApp.ViewMgr(), false);
				if (sdlg.DoModal() == DRC_OK) {
					iMapInfo scenProps = sdlg.SelScenario();
					iScenPropsDlg spdlg(&gApp.ViewMgr(), scenProps, true);
					if (spdlg.DoModal() == DRC_OK)	{
						scenProps.ReorderPlayers();
						gGame.ExitGame(false);
						gGame.StartNewGame(scenProps, false);
					}
				} 
			}
		} else if (res == GMRC_RESTART) {
			iQuestDlg qdlg(&gApp.ViewMgr(), iStringT(), gTextMgr[TRID_CONFIRM_RESTART], PID_NEUTRAL);
			if (qdlg.DoModal() == DRC_YES) {
			}
		} else if (res == GMRC_MAINMENU) {
			// Quit to main menu
			iQuestDlg qdlg(&gApp.ViewMgr(), iStringT(), gTextMgr[TRID_CONFIRM_EXITMM], PID_NEUTRAL);
			if (qdlg.DoModal() == DRC_YES) gGame.MainMenu();
		} else if (res == GMRC_QUIT) {
			// Quit to OS
			iQuestDlg qdlg(&gApp.ViewMgr(), iStringT(), gTextMgr[TRID_CONFIRM_QUIT], PID_NEUTRAL);
			if (qdlg.DoModal() == DRC_YES) {
				iStringT saveDir = gSavePath.Left(gSavePath.Length()-1);
				bool bOk = false;
				if (iFile::DirExists(saveDir) || iFile::DirCreate(saveDir)) {
					iFilePtr pFile(CreateWin32File(gSavePath + _T("lastses.tmp")));
					check(pFile);
					bOk = (pFile && gGame.Map().SaveToFile(pFile.operator->()));
				}
				if (bOk) iFile::Move(gSavePath + _T("lastses.tmp"), gSavePath + _T("lastses.phs"));
				gGame.Quit();
			}
		} else if (res == GMRC_RETURN) {
			// do nothing
		} else {
			check(0 == "Unknown menu item?");
		}
	} 
}

iGamePopupMenu::iGamePopupMenu(iViewMgr* pViewMgr)
: iPopupList(pViewMgr, GameMenuHandler, iSize(120,18), GMI_COUNT, -1, 15)
{}

void iGamePopupMenu::DrawItem(sint32 itemId, iDib& surf, const iRect& rc, uint32 flags)
{
	//ButtonFrame(surf,rc,0 );
	iRect frc(rc);
	//if (flags & iButton::Selected) {
		gGfxMgr.BlitTile(PDGG_CTILE, surf,frc);
		ButtonFrame(surf,frc,0);
	//}
	frc.DeflateRect(1);

	// icon
	BlitIcon(surf, GameMenuItemIcons[itemId], iRect(frc.x, frc.y, 18, frc.h));
	frc.DeflateRect(18,0,0,0);

	// text
	iTextComposer::FontConfig fc_name(iTextComposer::FS_MEDIUM, RGB16(220,220,220));
	gTextComposer.TextOut(fc_name, surf, iPoint(), gTextMgr[GameMenuItemText[itemId]], frc,AlignLeft);
}
