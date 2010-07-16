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
#include "Dlg_LeaveGuards.h"

iDlg_LeaveGuards::iDlg_LeaveGuards(iViewMgr* pViewMgr, iHero* pHero, iOwnCnst* pOwnCnst)
: iBaseGameDlg(pViewMgr,pHero->Owner()->PlayerId()), m_pHero(pHero), m_pOwnCnst(pOwnCnst)
{}

void iDlg_LeaveGuards::OnCreateDlg()
{
	iRect clRect = ClientRect();

	AddChild(m_pSplitBtn = new iCheckButton(m_pMgr, this, iRect(clRect.x+5+36*7,clRect.y+20,18,34*2+1), PDGG_BTN_SPLIT, 103));
	AddChild(m_pCnstArmyList = new iArmyListEx(m_pMgr,this,iPoint(clRect.x+5,clRect.y+20),m_pSplitBtn, iSize(35,34),101));
	m_pCnstArmyList->SetArmy(&m_pOwnCnst->Guard(), NULL, true);
	AddChild(m_pHeroArmyList = new iArmyListEx(m_pMgr,this,iPoint(clRect.x+5,clRect.y+55),m_pSplitBtn, iSize(35,34),102));
	m_pHeroArmyList->SetArmy(&m_pHero->Army(), m_pHero, false);
	m_pCnstArmyList->AddCompetitor(m_pHeroArmyList);
	m_pHeroArmyList->AddCompetitor(m_pCnstArmyList);

	AddChild(new iTextButton(m_pMgr, this, iRect((clRect.x+clRect.w/2)-20, clRect.y2()-DEF_BTN_HEIGHT, 40, DEF_BTN_HEIGHT), TRID_OK, DRC_OK));
}

void iDlg_LeaveGuards::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);
	// title
	gTextComposer.TextOut(dlgfc_hdr, gApp.Surface(),rc.point(),gTextMgr[TRID_LEAVE_GUARDS], iRect(rc.x,rc.y,rc.w,15),AlignCenter);
	rc.y+=15;
	
}

iSize iDlg_LeaveGuards::ClientSize() const
{
	sint32 w = 280;
	sint32 h = 0;

	// title
	h += 15;

	// Construction's army list
	h += 40;
	// Hero's army list
	h += 42;

	// buttons
	h += DEF_BTN_HEIGHT;

	return iSize(w,h);
}

void iDlg_LeaveGuards::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (uid == DRC_OK) {
		EndDialog(DRC_OK);
	}
}


