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
#include "OverlandView.h"
#include "CastleView.h"
#include "Ctl_CnstPage.h"
#include "Ctl_RecrPage.h"
#include "Ctl_MGuildPage.h"
#include "Ctl_MarketPage.h"
#include "Ctl_TavernPage.h"

/*
 *	Common defines
 */
const SpriteId CVT_ICONS[CVT_COUNT] = {
	PDGG_TAB_BUILD_CTR,
	PDGG_TAB_ARMY,
	PDGG_TAB_MGUILD,
	PDGG_TAB_MARKET,
	PDGG_TAB_TAVERN
};

iCtlViewChild::iCtlViewChild(iViewMgr* pViewMgr)
: iView(pViewMgr,iRect(0,37,320,182),GENERIC_VIEWPORT,0,Enabled) {}

void iCtlViewChild::OnCompose()
{
	iRect rc = GetScrRect();
	gGfxMgr.BlitTile(PDGG_BKTILE, gApp.Surface(),rc);
	gApp.Surface().Darken25Rect(rc);
}

/*
 *	Castle view
 */
iCastleView::iCastleView()
: iChildGameView(false, CV_OVERLAND), m_curTab(-1), m_pCastle(NULL)
{
	AddChild(m_pChilds[0] = new iCnstTab(&gApp.ViewMgr(), this));
	AddChild(m_pChilds[1] = new iRecrTab(&gApp.ViewMgr()));
	AddChild(m_pChilds[2] = new iMGuildTab(&gApp.ViewMgr()));
	AddChild(m_pChilds[3] = new iMarketTab(&gApp.ViewMgr()));
	AddChild(m_pChilds[4] = new iTavernTab(&gApp.ViewMgr()));

	AddChild(new iInfoBar(&gApp.ViewMgr(), this, iRect(0,0,m_Rect.w,15)));
	AddChild(new iCastleTitleBar(&gApp.ViewMgr(),this,  iRect(0,15,m_Rect.w,22)));
	AddChild(m_pToolBar = new iCastleToolBar(&gApp.ViewMgr(),this, iRect(0,m_Rect.h-21,m_Rect.w,21)));

	SetCastle(gGame.Map().CurCastle());
	ShowPage(CVT_CONSTRUCT);
}

iCastleView::~iCastleView()
{}

bool iCastleView::OnKeyDown(sint32 key)
{
	if (key == KEY_UP || key == KEY_DOWN) {
		if ( m_curTab == CVT_CONSTRUCT) {
			iCnstTab* pCnstTab = (iCnstTab*)m_pChilds[CVT_CONSTRUCT];
			if (key == KEY_UP) pCnstTab->CnstList()->SelPrev();
			else if (key == KEY_DOWN) pCnstTab->CnstList()->SelNext();
		}
	} else if (key == KEY_LEFT){
		PrevTab();
	} else if (key == KEY_RIGHT){
		NextTab();
	} else {
		return false;
	}
	return true;
}

void iCastleView::ShowPage(uint32 nIdx)
{
	if (m_curTab != -1) m_pChilds[m_curTab]->SetVisible(false);
	m_pChilds[nIdx]->SetVisible(true);
	m_pChilds[nIdx]->OnActivate();
	m_curTab = nIdx;
}

void iCastleView::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	sint32 clsId = pView->GetClassId();
	uint32 uid = pView->GetUID();

	if (uid == 101) {
		SetCastle(gGame.Map().CurPlayer()->PrevCastle());
	} else if (uid == 102) {
		SetCastle(gGame.Map().CurPlayer()->NextCastle());
	} else if (uid == 103) {
		check(clsId == TABBED_SWITCH);
		if (cmd == CCI_TABCHANGED) ShowPage(param);
	} else if (uid == VPUID_BTN_CLOSECITY) {
		gGame.HideView(CV_CASTLE);
	} else if (uid == 150 && cmd == CCI_BTNCLICK) {
		iPoint pp = pView->GetScrPos();
		m_pMgr->TrackPopup(new iCastleListPopup(m_pMgr), pp, iRect(0,0,320,240),AlignBottomLeft);
	} else if (uid == VPUID_BTN_STATE) {
		m_pMgr->TrackPopup(new iGamePopupMenu(m_pMgr), iPoint(320,15), iRect(0,0,320,240),AlignTopRight);
	}
}

bool iCastleView::Process(fix32 t)
{
	return true;
}

void iCastleView::SetCastle(iCastle* pCastle)
{
	m_pCastle = pCastle;
	m_pCastle->UpdateCnsts();
	for (uint32 xx=0; xx<CVT_COUNT; ++xx) m_pChilds[xx]->SetCastle(m_pCastle);
	((iCastleTitleBar*)GetChildById(VPUID_CTLVIEWTITLELBAR))->SetCastle(m_pCastle);
	((iCastleToolBar*)GetChildById(VPUID_CTLVIEWTOOLBAR))->SetCastle(m_pCastle);
	UpdateTabs();
	if (!m_pToolBar->m_pTabSwitch->IsTabEnabled(m_pToolBar->m_pTabSwitch->GetCurrentTab())) {
		m_pToolBar->m_pTabSwitch->SetCurrentTab(CVT_CONSTRUCT);
		ShowPage(CVT_CONSTRUCT);
	}
}

void iCastleView::OnBuildCnst()
{
	UpdateTabs();
}

void iCastleView::OnCompose()
{
	gApp.Surface().FillRect(GetScrRect(),RGB16(32,64,128));
}

void iCastleView::NextTab()
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

void iCastleView::PrevTab()
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

void iCastleView::UpdateTabs()
{
	m_pToolBar->m_pTabSwitch->EnableTab(CVT_MARKET, m_pCastle->FindCnst(CTLCNST_MARKETPLACE)->IsBuilt());
	m_pToolBar->m_pTabSwitch->EnableTab(CVT_TAVERN, m_pCastle->FindCnst(CTLCNST_TAVERN)->IsBuilt());
}

/*
 *	Castle Titlebar
 */
iCastleTitleBar::iCastleTitleBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect)
: iView(pViewMgr,rect,GENERIC_VIEWPORT,VPUID_CTLVIEWTITLELBAR,Visible|Enabled), m_pCastle(NULL)
{
	AddChild(new iDlgIconButton(pViewMgr, pCmdHandler, iRect(0,1,32,rect.h-2),PDGG_BTN_PREV,101));
	AddChild(new iDlgIconButton(pViewMgr, pCmdHandler, iRect(rect.w-32,1,32,rect.h-2),PDGG_BTN_NEXT,102));
}

void iCastleTitleBar::SetCastle(iCastle* pCastle)
{
	m_pCastle = pCastle;
	Invalidate();
}

void iCastleTitleBar::OnCompose()
{
	iRect rect = GetScrRect();
	gApp.Surface().FillRect(rect,cColor_Black);
	rect.DeflateRect(33,1,33,1);
	
	gGfxMgr.BlitTile(PDGG_BKTILE,gApp.Surface(),rect);
	ComposeDlgButton(gApp.Surface(), rect, iButton::Disabled);

	iStringT txt;
	txt.Addf(_T("#S1#FFF0 %s #S0#FC90(%s)"),m_pCastle->Name().CStr(),gTextMgr[TRID_CASTLE_TYPE_CITADEL+m_pCastle->Proto()->Type()]);
	gTextComposer.TextOut(gApp.Surface(),rect,txt,rect,AlignCenter);

	//iTextComposer::FontConfig fc(dlgfc_hdr);
	//gTextComposer.TextOut(fc, gApp.Surface(),rect.point(),m_pCastle->Name(),rect,AlignCenter);
}

/*
 *	Castle Toolbar
 */
iCastleToolBar::iCastleToolBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect)
: iView(pViewMgr,rect,GENERIC_VIEWPORT,VPUID_CTLVIEWTOOLBAR,Visible|Enabled), m_pCastle(NULL)
{
	// Button with Popup
	AddChild(m_pCastleGlyphBtn = new iCastleGlyphBtn(pViewMgr, pCmdHandler, iRect(0,1,32,20), 150));

	// Tab switch
	m_pTabSwitch = new iBarTabSwitch(pViewMgr, pCmdHandler, iRect(33,1,CVT_COUNT*32,20), CVT_COUNT, 103);
	for (uint32 xx=0; xx<CVT_COUNT; ++xx) m_pTabSwitch->SetTabIcon(CVT_ICONS[xx]);
	m_pTabSwitch->SetCurrentTab(CVT_CONSTRUCT);
	AddChild(m_pTabSwitch);

	// Close button
	AddChild(new iIconButton(pViewMgr, pCmdHandler, iRect(rect.w-30,1,30,20),PDGG_BTN_CLOSE,VPUID_BTN_CLOSECITY));
}

void iCastleToolBar::SetCastle(iCastle* pCastle)
{
	m_pCastle = pCastle;
	m_pCastleGlyphBtn->SetCastle(pCastle);
	Invalidate();
}

void iCastleToolBar::OnCompose()
{
	iRect rect = GetScrRect();
	gApp.Surface().FillRect(rect,cColor_Black);
	sint32 rval = m_pTabSwitch->GetSize().w+1+33;

	iRect rc(rect.x+rval,rect.y+1,rect.w-(30+rval)-1,20);
	gGfxMgr.BlitTile(PDGG_BKTILE, gApp.Surface(), rc);
	ButtonFrame(gApp.Surface(), rc, 0);
}


