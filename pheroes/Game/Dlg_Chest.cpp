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
#include "Dlg_Chest.h"

class iChestDlgBtn : public iButton
{
public:
	iChestDlgBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid, SpriteId sid, const iStringT& text)
	: iButton(pViewMgr, pCmdHandler, rect, uid, Visible | Enabled), m_sid(sid), m_text(text) {}

	void OnCompose()
	{
		iRect rc = GetScrRect();
		//gApp.Surface().FillRect(rc,cColor_Black);
		//rc.DeflateRect(1);
		//gGfxMgr.BlitTile(PDGG_CTILE,gApp.Surface(),rc);
		//ButtonFrame(gApp.Surface(),rc,GetButtonState());
		ComposeDlgButton(gApp.Surface(), rc, GetButtonState());
		iPoint offs;
		if (GetButtonState() & Pressed) offs += iPoint(1,1);
		BlitIcon(gApp.Surface(),m_sid,iRect(rc.x,rc.y,rc.w,rc.h-14), 63, offs);
		gTextComposer.TextOut(gApp.Surface(),rc.point(),m_text,iRect(rc.x,rc.y+rc.h-16,rc.w,16),AlignTop,offs);
	}

	SpriteId	m_sid;
	iStringT	m_text;
};

//////////////////////////////////////////////////////////////////////////
iDlg_Chest::iDlg_Chest(iViewMgr* pViewMgr, iHero* pHero, iMapItem_Chest* pChest)
: iBaseGameDlg(pViewMgr,pHero->Owner()->PlayerId()), m_pHero(pHero)
{ 
	m_cntExp = pHero->ConvExpPts(pChest->ExpPtCount());
	m_cntGold = pChest->ExpPtCount() + (pChest->ExpPtCount()>>2);
}

void iDlg_Chest::OnCreateDlg()
{
	iRect clRect = ClientRect();
	AddChild(new iChestDlgBtn(m_pMgr, this, iRect(34, clRect.y2()-50, 50, 50), Gold,PDGG_DGLYPH_GOLD, FormatNumber(m_cntGold)));
	AddChild(new iChestDlgBtn(m_pMgr, this, iRect(100, clRect.y2()-50, 50, 50), Experience,PDGG_DGLYPH_EXP, FormatNumber(m_cntExp)));
}

void iDlg_Chest::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);
	gTextComposer.TextOut(dlgfc_hdr, gApp.Surface(),rc.point(),gTextMgr[TRID_MAPRES_CHEST], iRect(rc.x,rc.y,rc.w,15),AlignCenter);
	rc.y+=15;
	rc.y+=gTextComposer.TextBoxOut(dlgfc_splain,gApp.Surface(),gTextMgr[TRID_MAPRES_CHEST_MSG], rc);
	rc.h = 10;
	rc.y+=5;
	gTextComposer.TextBoxOut(dlgfc_stopic, gApp.Surface(),gTextMgr[TRID_CHOOSE_QUESTION], rc);
}

iSize iDlg_Chest::ClientSize() const
{
	sint32 w = 150;
	sint32 h = 0;

	// title
	h += 15;
	// Message
	h += gTextComposer.GetTextBoxSize(gTextMgr[TRID_MAPRES_CHEST_MSG],w,dlgfc_splain).h;
	// Question
	h += 5+15+5;
	// buttons
	h += 50;

	return iSize(w,h);
}

void iDlg_Chest::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	if (cmd == CCI_BTNCLICK) {
		EndDialog(pView->GetUID());
	}
}

