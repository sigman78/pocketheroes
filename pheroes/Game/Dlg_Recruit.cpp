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
#include "Dlg_Recruit.h"
#include "Dlg_CreatInfo.h"

iDlg_Recruit::iDlg_Recruit(iViewMgr* pViewMgr, iCreatGroup& creats, iArmy& army, PLAYER_ID owner)
: iBaseGameDlg(pViewMgr, owner), m_creats(creats), m_army(army), m_avail(0), m_recr(0), m_max(0)
{ }

void iDlg_Recruit::OnCreateDlg()
{
	m_avail = m_creats.Count();
	check(m_pid != PID_NEUTRAL);
	
	iPlayer* pOwner = gGame.Map().FindPlayer(m_pid);
	check(pOwner);
	m_crCost = CREAT_DESC[m_creats.Type()].cost;
	m_max = iMIN<sint32>(m_avail, pOwner->Minerals().Has(m_crCost));

	/*
	iMineralSet ms = m_crCost;
	while (m_max < m_avail && pOwner->Minerals().Has(ms)) {
		ms += m_crCost;
		++m_max;
	}*/

	iRect clrc = ClientRect();
	// Creature button
	AddChild(m_pIcnButton = new iIconButton(m_pMgr, this, iRect(clrc.x + (clrc.w/2-22), 34,44,56), PDGG_MINIMON + m_creats.Type(), 201));
	// Slider
	AddChild(m_pSlider = new iPHScrollBar(m_pMgr, this, iRect(clrc.x + (clrc.w/2-80), clrc.y2()-20-10-15,160,15), 101, iScrollBar::Horizontal));
	m_pSlider->SetMetrics(m_max+1,1);
	// Button size 40x15 (3*40+10)
	iRect rc(clrc.x + (clrc.w/2-65), clrc.y2()-DEF_BTN_HEIGHT, 40, DEF_BTN_HEIGHT);

	AddChild(new iDlgIconButton(m_pMgr,this,rc,PDGG_BTN_MAX, 100));
	rc.x+=45;
	AddChild(new iTextButton(m_pMgr,this,rc,TRID_OK, DRC_OK));
	rc.x+=45;
	AddChild(new iTextButton(m_pMgr,this,rc,TRID_CANCEL, DRC_CANCEL));
	

	GetChildById(100)->SetEnabled(m_max>0);
	GetChildById(DRC_OK)->SetEnabled(m_recr>0);
}

void iDlg_Recruit::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);

	// title
	gTextComposer.TextOut(dlgfc_hdr, gApp.Surface(), rc.point(), iFormat(_T("%s %s"), gTextMgr[TRID_RECRUIT], gTextMgr[m_creats.Type()*3+TRID_CREATURE_PEASANT_F3]), rc, AlignTop);
	rc.DeflateRect(0,20,0,0);
	

	// information
	iRect orc(rc.x + (rc.w/2-100), rc.y, 78, 56);
	//gApp.Surface().Darken25Rect(orc);
	//ButtonFrame(gApp.Surface(), orc, 0);

	iRect trc(orc);
	trc.DeflateRect(3);
	gTextComposer.TextOut(dlgfc_splain, gApp.Surface(), trc, iStringT(gTextMgr[TRID_AVAILABLE]) + _T(":"), trc, AlignTop);
	trc.y += 11;
	gTextComposer.TextOut(dlgfc_topic, gApp.Surface(), trc, FormatNumber(m_avail), trc, AlignTop);
	trc.y += 14;
	gTextComposer.TextOut(dlgfc_splain, gApp.Surface(), trc, iStringT(gTextMgr[TRID_COST_PER_TROOP]) + _T(":"), trc, AlignTop);
	trc.y += 11;
	gTextComposer.TextOut(dlgfc_topic, gApp.Surface(), rc.point(), MineralSet2Text(m_crCost), trc, AlignTop);

	orc.x += 78;
	orc.w = 42;
	//BlitIcon(gApp.Surface(), PDGG_MINIMON +m_dwel.CrType(), orc);

	orc.w = 78;
	orc.x += 44;
	//gApp.Surface().Darken25Rect(orc);
	//ButtonFrame(gApp.Surface(), orc, 0);
	trc = orc;
	trc.DeflateRect(3);
	gTextComposer.TextOut(dlgfc_splain, gApp.Surface(), trc, iStringT(gTextMgr[TRID_RECRUIT]) + _T(":"), trc, AlignTop);
	trc.y += 11;
	gTextComposer.TextOut(dlgfc_topic, gApp.Surface(), trc, FormatNumber(m_recr), trc, AlignTop);
	trc.y += 14;
	gTextComposer.TextOut(dlgfc_splain, gApp.Surface(), trc, iStringT(gTextMgr[TRID_TOTAL_COST]) + _T(":"), trc, AlignTop);
	trc.y += 11;
	iMineralSet ms;	ms.Reset();
	for(uint32 xx=0; xx<m_recr; ++xx) ms += m_crCost;
	gTextComposer.TextOut(dlgfc_topic, gApp.Surface(), rc.point(), MineralSet2Text(ms), trc, AlignTop);
}

iSize iDlg_Recruit::ClientSize() const
{
	return iSize(220,110 + DEF_BTN_HEIGHT);
}

void iDlg_Recruit::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	if (!IsValidDialog()) return;
	uint32 uid = pView->GetUID();
	if (uid == 101) {
		m_recr = m_pSlider->CurPos();
		GetChildById(DRC_OK)->SetEnabled(m_recr>0);
	} else if (cmd == CCI_BTNCLICK) {
		if (uid == 100) {
			m_pSlider->SetCurPos(m_max);
			m_recr = m_max;
			GetChildById(DRC_OK)->SetEnabled(m_recr>0);
		} else if (uid == 201) {
			iCreatGroup group(m_creats.Type(),m_avail);
			iDlg_CreatInfo cidlg(m_pMgr, m_pid, group, iFurtSkills(), false, 0);
			cidlg.DoModal();
		} else if (uid == DRC_OK) {
			// Check space in target army
			if (m_army.CanAddGroup(m_creats.Type())) {
				m_army.AddGroup(m_creats.Type(), m_recr);
				m_creats.Count() -= m_recr;
				gGame.Map().FindPlayer(m_pid)->Minerals() -= m_crCost * m_recr;
				EndDialog(DRC_OK);
			} else {
				iTextDlg tdlg(m_pMgr, _T(""), gTextMgr[TRID_MSG_NO_ROOM], m_pid);
				tdlg.DoModal();
			}
		} else if (uid == DRC_CANCEL) {
			EndDialog(DRC_CANCEL);
		}
	}
}


