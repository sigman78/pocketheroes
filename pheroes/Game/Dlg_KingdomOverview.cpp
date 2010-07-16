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
#include "Dlg_KingdomOverview.h"

iKigdomOverviewDlg::iKigdomOverviewDlg(iViewMgr* pViewMgr, const iPlayer& player)
: iBaseGameDlg(pViewMgr, player.PlayerId()), m_player(player)
{
}

void iKigdomOverviewDlg::OnCreateDlg()
{
	iRect clRect = ClientRect();
	AddChild(new iTextButton(m_pMgr,this,iRect(clRect.x+(clRect.w/2-20),clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_OK, DRC_OK));
}

void iKigdomOverviewDlg::ComposeCell(iDib& surf, const iRect& rect, MINERAL_TYPE mt, sint32 quant)
{
	BlitIcon(surf, PDGG_RES_BIG_ICONS + mt, rect);
	gTextComposer.TextOut((quant>0)?dlgfc_topic:dlgfc_plain, surf, rect.point(), FormatNumber(quant, true), iRect(rect.x, rect.y+rect.h, rect.w, 14), AlignTop);
}

void iKigdomOverviewDlg::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);
	gTextComposer.TextOut(dlgfc_hdr, gApp.Surface(), rc, gTextMgr[TRID_KINGDOM_OVERVIEW], rc, AlignTop);
	rc.DeflateRect(0,15,0,0);
	gTextComposer.TextOut(dlgfc_topic, gApp.Surface(), rc, gTextMgr[TRID_DAILY_INCOME], rc, AlignTop);
	rc.DeflateRect(0,15,0,0);

	iMineralSet ms;
	ms.Reset();
	for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) ms.quant[xx] += m_player.FurtSkills().Value((FURTHER_SKILLS)(FSK_MIN_GOLD+xx));
	for (iPlayer::iCHLIt hit = m_player.HeroFirst(); hit != m_player.HeroEnd(); ++hit) {
		for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) ms.quant[xx] += (*hit)->ConstFurtSkills().Value((FURTHER_SKILLS)(FSK_MIN_GOLD+xx)) + (*hit)->VarFurtSkills().Value((FURTHER_SKILLS)(FSK_MIN_GOLD+xx));
	}

	// 
	sint32 xpos = rc.x + (rc.w/2 - 40*4/2);
	sint32 ypos = rc.y;
	for (uint32 xx=0; xx<4; ++xx) {
		MINERAL_TYPE mt = (MINERAL_TYPE)(MINERAL_GOLD+xx);
		ComposeCell(gApp.Surface(), iRect(xpos, ypos, 40,30), mt, ms.quant[mt]);
		xpos += 40;
	}
	
	xpos = rc.x + (rc.w/2 - 40*3/2);
	ypos = rc.y + 50;
	for (uint32 xx=0; xx<3; ++xx) {
		MINERAL_TYPE mt = (MINERAL_TYPE)(MINERAL_GOLD+4+xx);
		ComposeCell(gApp.Surface(), iRect(xpos, ypos, 40,30), mt, ms.quant[mt]);
		xpos += 40;
	}
}

iSize iKigdomOverviewDlg::ClientSize() const
{
	return iSize(180, 135 + DEF_BTN_HEIGHT);
}

void iKigdomOverviewDlg::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (cmd == CCI_BTNCLICK) {
		EndDialog(uid);
	}
}


