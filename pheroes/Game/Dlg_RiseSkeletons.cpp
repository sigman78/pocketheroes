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
#include "Dlg_RiseSkeletons.h"

iDlg_RiseSkeletons::iDlg_RiseSkeletons(iViewMgr* pViewMgr, PLAYER_ID pid, CREATURE_TYPE ct, sint32 quant)
: iBaseGameDlg(pViewMgr,pid), m_ct(ct), m_quant(quant)
{}

void iDlg_RiseSkeletons::OnCreateDlg()
{
	iRect clRect = ClientRect();
	AddChild(new iTextButton(m_pMgr, this, iRect((clRect.x+clRect.w/2)-20, clRect.y2()-DEF_BTN_HEIGHT, 40, DEF_BTN_HEIGHT), TRID_OK, DRC_OK));
}

void iDlg_RiseSkeletons::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);

	// title
	gTextComposer.TextOut(dlgfc_hdr, gApp.Surface(),rc.point(),gTextMgr[TRID_HSKILL_NECROMANCY], iRect(rc.x,rc.y,rc.w,15),AlignCenter);
	rc.y += 20;

	// text	

	rc.y += gTextComposer.TextBoxOut(dlgfc_splain, gApp.Surface(), iFormat(gTextMgr[TRID_MSG_RISE_SKELETONS], m_quant, gTextMgr[TRID_CREATURE_PEASANT_F3+m_ct*3]), iRect(rc.x,rc.y,rc.w,rc.h));
	rc.y += 5;

	// icon and quantity
	ComposeCreatureCell(gApp.Surface(), iRect((rc.x+rc.w/2)-18,rc.y,36,42), m_ct, m_quant, VL_EXPERT);
}

iSize iDlg_RiseSkeletons::ClientSize() const
{
	sint32 w = 150;
	sint32 h = 0;

	// title
	h += 20;
	// text
	h += gTextComposer.GetTextBoxSize(iFormat(gTextMgr[TRID_MSG_RISE_SKELETONS], m_quant, gTextMgr[TRID_CREATURE_PEASANT_F3+m_ct*3]), w, dlgfc_splain).h;
	h += 5;
	// icon and quantity
	h += 42;
	// button
	h += DEF_BTN_HEIGHT + 10;

	return iSize(w,h);
}

void iDlg_RiseSkeletons::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (uid == DRC_OK) {
		EndDialog(DRC_OK);
	}
}

