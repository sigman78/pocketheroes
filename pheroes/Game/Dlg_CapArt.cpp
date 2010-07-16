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
#include "Dlg_CapArt.h"

const sint32 acW = 32;
const sint32 acH = 32;

iDlg_CapArt::iDlg_CapArt(iViewMgr* pViewMgr, const iHero* pHero, const iArtList& artList)
: iBaseGameDlg(pViewMgr,pHero->Owner()->PlayerId()), m_pHero(pHero), m_artList(artList)
{ 
	m_bSingle = artList.GetSize() == 1;
	if (!m_bSingle) m_Title.Setf(gTextMgr[TRID_MSG_CAP_ARTS],m_artList.GetSize());
	else m_Title = gTextMgr[TRID_MSG_CAP_ART];
}

void iDlg_CapArt::OnCreateDlg()
{
	iRect clRect = ClientRect();

	if (!m_bSingle) {
		iArtListCtrl* pArtCtrl = new iArtListCtrl(m_pMgr, this, m_artList, iPoint(20,clRect.y2()-60), iSize(acW,acH), 6, iArtBackPackCtrl::Horizontal, 101);
		AddChild(pArtCtrl);
	}
	AddChild(new iTextButton(m_pMgr,this,iRect(clRect.x+(clRect.w/2-20),clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_OK, DRC_OK));
}

void iDlg_CapArt::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);
	sint32 th = gTextComposer.TextBoxOut(dlgfc_topic, gApp.Surface(), m_Title, iRect(rc.x+5,rc.y,rc.w-10,30));

	if (m_bSingle) {
		BlitIcon(gApp.Surface(), gGame.ItemMgr().m_artMgr[m_artList[0]].Sprite(), iRect(rc.x,rc.y+th,rc.w, 32));
		th += 32;
		gTextComposer.TextOut(dlgfc_topic, gApp.Surface(), iPoint(), gTextMgr[gGame.ItemMgr().m_artMgr[m_artList[0]].NameKey(m_pHero)], iRect(rc.x+5,rc.y+th,rc.w-10,30), AlignTop);
		th += 15;
		th += gTextComposer.TextBoxOut(dlgfc_plain, gApp.Surface(), gTextMgr[gGame.ItemMgr().m_artMgr[m_artList[0]].DescKey(m_pHero)], iRect(rc.x+5,rc.y+th,rc.w-10,30));
	}
}

iSize iDlg_CapArt::ClientSize() const
{
	sint32 w = (m_bSingle)?200:250;
	sint32 h = 0;


	// title
	h += gTextComposer.GetTextBoxSize(m_Title, w-10, dlgfc_topic).h + 5;

	if (m_bSingle) {
		// Artifact icon
		h += 32;
		// Artifact name
		h += 15;
		// Artifact description
		h += gTextComposer.GetTextBoxSize(gTextMgr[gGame.ItemMgr().m_artMgr[m_artList[0]].DescKey(m_pHero)], w-10, dlgfc_plain).h;
	} else {
		// Artifact list
		h += 40;
	}

	// ok button
	h += DEF_BTN_HEIGHT + 15;

	return iSize(w,h);
}

void iDlg_CapArt::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	if (cmd == CCI_BTNCLICK) {
		EndDialog(pView->GetUID());
	}
}



