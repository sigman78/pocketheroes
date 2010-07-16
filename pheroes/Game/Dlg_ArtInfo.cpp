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
#include "Dlg_ArtInfo.h"

iDlg_ArtInfo::iDlg_ArtInfo(iViewMgr* pViewMgr, uint16 artId, const iHero* pOwner)
: iBaseGameDlg(pViewMgr, (pOwner)?(pOwner->Owner()->PlayerId()):PID_NEUTRAL), m_pOwner(pOwner), m_artId(artId), m_fcReqMsg(dlgfc_splain)
{
	SECONDARY_SKILLS reqSkill = gGame.ItemMgr().m_artMgr[artId].ReqSkill();
	uint16 reqLevel = gGame.ItemMgr().m_artMgr[artId].ReqLevel();
	if (reqLevel && reqSkill != SECSK_NONE) {
		m_reqMsg.Setf(gTextMgr[TRID_MSG_ARTREQ_LVL_SKILL], reqLevel, gTextMgr[TRID_HSKILL_ESTATES+reqSkill]);
	} else if (reqLevel) {
		m_reqMsg.Setf(gTextMgr[TRID_MSG_ARTREQ_LVL], reqLevel);
	} else if (reqSkill != SECSK_NONE) {
		m_reqMsg.Setf(gTextMgr[TRID_MSG_ARTREQ_SKILL], gTextMgr[TRID_HSKILL_ESTATES+reqSkill]);
	}

	if (pOwner && gGame.ItemMgr().m_artMgr[artId].CanAttach(pOwner)) {
		m_fcReqMsg.cmpProps.faceColor = RGB16(96,255,96);
	} else {
		m_fcReqMsg.cmpProps.faceColor = RGB16(255,96,96);
	}
}

void iDlg_ArtInfo::OnCreateDlg()
{
	iRect clRect = ClientRect();
	AddChild(new iTextButton(m_pMgr,this,iRect(clRect.x+(clRect.w/2-20),clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_OK, DRC_OK));
}

void iDlg_ArtInfo::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);
	// title
	sint32 th = gTextComposer.TextBoxOut(dlgfc_topic, gApp.Surface(), gTextMgr[gGame.ItemMgr().m_artMgr[m_artId].NameKey(m_pOwner)], iRect(rc.x+5,rc.y,rc.w-10,30));
	th += 5;
	// description
	th += gTextComposer.TextBoxOut(dlgfc_plain, gApp.Surface(), gTextMgr[gGame.ItemMgr().m_artMgr[m_artId].DescKey(m_pOwner)], iRect(rc.x+5,rc.y+th,rc.w-10,30)) + 5;
	// requirements
	if (!m_reqMsg.Empty()) th += gTextComposer.TextBoxOut(m_fcReqMsg, gApp.Surface(), m_reqMsg, iRect(rc.x+5,rc.y+th,rc.w-10,30)) + 5;
	// icon
	SpriteId sid = gGame.ItemMgr().m_artMgr[m_artId].Sprite();
	iSize ssiz = gGfxMgr.Dimension(sid);
	iPoint op(rc.x + rc.w/2 - ssiz.w/2, rc.y+th+5);
	gGfxMgr.Blit(sid,gApp.Surface(),op-gGfxMgr.Anchor(sid));
		/*
	iPoint sa = gGfxMgr.Anchor(sid);
	rc.y = rc.y2() - 15 - 10 - ssiz.h;
	rc.x = rc.x + (rc.w/2 - ssiz.w/2);
	*/

}

iSize iDlg_ArtInfo::ClientSize() const
{
	sint32 w = 150;
	sint32 h = 0;


	// title
	h += gTextComposer.GetTextBoxSize(gTextMgr[gGame.ItemMgr().m_artMgr[m_artId].NameKey(m_pOwner)], w-10, dlgfc_topic).h +5;

	// description
	h += gTextComposer.GetTextBoxSize(gTextMgr[gGame.ItemMgr().m_artMgr[m_artId].DescKey(m_pOwner)], w-10, dlgfc_plain).h + 5;

	// requirements
	if (!m_reqMsg.Empty()) h += gTextComposer.GetTextBoxSize(m_reqMsg, w-10, m_fcReqMsg).h;

	// icon
	h += gGfxMgr.Dimension(gGame.ItemMgr().m_artMgr[m_artId].Sprite()).h + 10;

	// ok button
	h += DEF_BTN_HEIGHT + 10;

	return iSize(w,h);
}

void iDlg_ArtInfo::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	if (cmd == CCI_BTNCLICK) {
		EndDialog(pView->GetUID());
	}
}
