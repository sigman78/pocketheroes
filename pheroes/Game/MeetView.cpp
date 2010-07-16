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
#include "ArtifactList.h"
#include "MeetView.h"

//////////////////////////////////////////////////////////////////////////
class iHeroBigPortBtn : public iButton 
{
public:
	iHeroBigPortBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid)
	: iButton(pViewMgr, pCmdHandler, rect, uid, Visible), m_pHero(NULL) {}

	void OnCompose()
	{
		if (m_pHero) {
			iRect rc = GetScrRect();
			SpriteId sid = m_pHero->Proto()->m_icn48;
			gGfxMgr.Blit(sid,gApp.Surface(), rc);
			ButtonFrame(gApp.Surface(),rc,GetButtonState());
			if (!IsEnabled()) gApp.Surface().Darken25Rect(rc);
			rc.InflateRect(1);
			gApp.Surface().FrameRect(rc,cColor_Black);
		}
	}

	void SetHero(iHero* pHero)
	{
		m_pHero = pHero;
		SetEnabled(m_pHero != NULL);
		Invalidate();
	}

	inline const iHero* Hero() const { return m_pHero; }
	inline iHero* Hero() { return m_pHero; }

private:
	iHero*	m_pHero;
};

/*
 *	Hero Child
 */
iHeroChild::iHeroChild(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, iSimpleArray<iArtDragDropItem*>& competitors, const iPoint& pos)
: iView(pViewMgr, iRect(pos.x, pos.y, 320, 109), GENERIC_VIEWPORT, 200, Enabled | Visible), m_pHero(NULL), m_pFriend(NULL)
{
	AddChild(m_pPortrait = new iHeroBigPortBtn(pViewMgr, this, iRect(3,20,48,48), 201));
	AddChild(m_pArtBackPackCtrl = new iArtBackPackCtrl(competitors, pViewMgr, this, iPoint(55,20+18), iSize(32,30), 7, iArtBackPackCtrl::Horizontal, 120));
	AddChild(m_pSplitBtn = new iCheckButton(pViewMgr, this, iRect(m_Rect.w-23,m_Rect.h-38,20,36), PDGG_BTN_SPLIT, 140));
	AddChild(m_pArmyList = new iArmyListEx(pViewMgr,this,iPoint(3,53+18), m_pSplitBtn, iSize(41,36),110));
}

void iHeroChild::OnCompose()
{
	iRect rc = GetScrRect();
	gGfxMgr.BlitTile(PDGG_BKTILE, gApp.Surface(),rc);
	gApp.Surface().Darken25Rect(rc);

	check(m_pHero);

	// Name
	iRect trc(rc.x,rc.y,rc.w,17);
	ButtonFrame(gApp.Surface(),trc,0);
	iStringT txt;
	txt.Addf(_T("#S0#FEEE%s#S2#FAFF %s #S0#FEEE%s %d"),gTextMgr[TRID_HERO_TYPE_KNIGHT+m_pHero->Type()],m_pHero->Name().CStr(),gTextMgr[TRID_LEVEL],m_pHero->Level());
	gTextComposer.TextOut(gApp.Surface(),trc,txt,trc,AlignCenter);
	gApp.Surface().HLine(iPoint(rc.x,rc.y+17),rc.x2(),cColor_Black);

	rc.DeflateRect(0,18,0,0);

	// Portrait
	/*
	gApp.Surface().FrameRect(iRect(rc.x+2,rc.y+1,50,50),cColor_Black);
	gGfxMgr.Blit(m_pHero->Proto()->m_icn48,gApp.Surface(),iPoint(rc.x+3,rc.y+2));
	ButtonFrame(gApp.Surface(),iRect(rc.x+3,rc.y+2,48,48),0);
	*/
}

void iHeroChild::SetHero(iHero* pHero)
{
	m_pHero = pHero;
	m_pArmyList->SetArmy(&pHero->Army(), pHero, false);
	m_pArtBackPackCtrl->SetBackPack(&m_pHero->Artifacts().BackPack(), m_pHero);
	m_pPortrait->SetHero(m_pHero);
}

void iHeroChild::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	sint32 clsId = pView->GetClassId();
	uint32 uid = pView->GetUID();

	if (uid == 201) {
		check(m_pHero);
		if (gGame.Map().CurHero() != m_pHero) m_pHero->Owner()->SetCurHero(m_pHero);
		gGame.ShowView(iChildGameView::CV_HERO);
	} else if (uid == 140 && cmd == CCI_CHECKED) {
		if (m_pFriend) m_pFriend->m_pSplitBtn->SetChecked(m_pSplitBtn->IsChecked(), false);
	}
}

/*
 *	MeetView
 */
iMeetView::iMeetView()
: iChildGameView(false, CV_OVERLAND)
{
	AddChild(m_pChilds[0] = new iHeroChild(&gApp.ViewMgr(), this, m_Competitors, iPoint(0,0)));
	AddChild(m_pChilds[1] = new iHeroChild(&gApp.ViewMgr(), this, m_Competitors, iPoint(0,110)));
	m_pChilds[0]->ArmyList()->AddCompetitor(m_pChilds[1]->ArmyList());
	m_pChilds[1]->ArmyList()->AddCompetitor(m_pChilds[0]->ArmyList());
	m_pChilds[0]->SetFriend(m_pChilds[1]);
	m_pChilds[1]->SetFriend(m_pChilds[0]);
	AddChild(m_pToolBar = new iMeetToolBar(&gApp.ViewMgr(), this, iRect(0,m_Rect.h-21,m_Rect.w,21)));
}

iMeetView::~iMeetView()
{}


void iMeetView::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	sint32 clsId = pView->GetClassId();
	uint32 uid = pView->GetUID();
	
	if (uid == 101) {
		gGame.HideView(CV_MEET);
	}
}

bool iMeetView::Process(fix32 t)
{
	return true;
}

void iMeetView::OnCompose()
{
	gApp.Surface().FillRect(GetScrRect(),0);
}

void iMeetView::SetHeroes(iHero* pHero1, iHero* pHero2)
{
	m_pChilds[0]->SetHero(pHero1);
	m_pChilds[1]->SetHero(pHero2);
}

/*
 *	MeetToolbar
 */
iMeetToolBar::iMeetToolBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect)
: iView(pViewMgr,rect,GENERIC_VIEWPORT,VPUID_HEROVIEWTOOLBAR,Enabled|Visible)
{
	// Close button
	AddChild(new iIconButton(pViewMgr, pCmdHandler, iRect(rect.w-30,1,30,20),PDGG_BTN_CLOSE,101));
}

void iMeetToolBar::OnCompose()
{
	iRect rect = GetScrRect();
	gGfxMgr.BlitTile(PDGG_BKTILE, gApp.Surface(), iRect(rect.x,rect.y+1,rect.w-30,20));
	ButtonFrame(gApp.Surface(), iRect(rect.x,rect.y+1,rect.w-30,20), 0);
}

