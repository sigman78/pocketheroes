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
#include "Dlg_ArmyRoom.h"

iDlg_ArmyRoom::iDlg_ArmyRoom(iViewMgr* pViewMgr, iHero& hero, const iCreatGroup& cgroup)
: iBaseGameDlg(pViewMgr, hero.Owner()->PlayerId()), m_hero(hero)
{
	m_fArmy.SetGroup(0, cgroup.Type(), cgroup.Count());
	m_Title.Setf(gTextMgr[TRID_MAKE_ROOM_MESSAGE], gTextMgr[TRID_CREATURE_PEASANT_F3+cgroup.Type()*3]);
}

void iDlg_ArmyRoom::OnCreateDlg()
{
	iRect clRect = ClientRect();

	iCheckButton* pSplitBtn = new iCheckButton(m_pMgr, this, iRect(clRect.x+5+35*7+7,clRect.y+20,20,35+35+1), PDGG_BTN_SPLIT, 100);
	AddChild(pSplitBtn);

	iArmyListEx* pFakeArmyCtrl = new iArmyListEx(m_pMgr, this, iPoint(clRect.x+5,clRect.y+20), pSplitBtn, iSize(35,35), 101);
	pFakeArmyCtrl->SetArmy(&m_fArmy, NULL, true);
	AddChild(pFakeArmyCtrl);
	iArmyListEx* pHeroArmyCtrl = new iArmyListEx(m_pMgr, this, iPoint(clRect.x+5, clRect.y+20+36), pSplitBtn, iSize(35,35), 102);
	pHeroArmyCtrl->SetArmy(&m_hero.Army(), &m_hero, false);
	AddChild(pHeroArmyCtrl);


	pFakeArmyCtrl->AddCompetitor(pHeroArmyCtrl);
	pHeroArmyCtrl->AddCompetitor(pFakeArmyCtrl);

	AddChild(new iTextButton(m_pMgr,this,iRect(clRect.x+(clRect.w/2-20),clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_OK, DRC_OK));
}

void iDlg_ArmyRoom::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);
	// title
	gTextComposer.TextOut(dlgfc_topic, gApp.Surface(),rc.point(),m_Title, iRect(rc.x,rc.y,rc.w,15),AlignTop);
}

iSize iDlg_ArmyRoom::ClientSize() const
{
	sint32 w = 282;
	sint32 h = 0;

	// title
	h += 20;

	// fake army control
	h += 36;

	// hero's army control
	h += 36;

	// ok button
	h += DEF_BTN_HEIGHT + 10;
	return iSize(w,h);
}

void iDlg_ArmyRoom::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (uid == DRC_OK && cmd == CCI_BTNCLICK) {
		EndDialog(pView->GetUID());
	}
}

