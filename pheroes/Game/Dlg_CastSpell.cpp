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
#include "Dlg_CastSpell.h"
#include "SpellListView.h"

//////////////////////////////////////////////////////////////////////////
iDlg_CastSpell::iDlg_CastSpell(iViewMgr* pViewMgr, iHero* pOwner, uint32 typeMask)
: iBaseGameDlg(pViewMgr,pOwner->Owner()->PlayerId()), m_pOwner(pOwner), m_typeMask(typeMask)
{
}

MAGIC_SPELL iDlg_CastSpell::SelSpell() const
{
	return m_pSpellListView->SelSpell();
}

bool iDlg_CastSpell::KeyDown(sint32 key)
{
	if (key == KEY_UP) m_pSpellListView->SelPrev();
	else if (key == KEY_DOWN) m_pSpellListView->SelNext();
	else return false;
	return true;
}

void iDlg_CastSpell::OnCreateDlg()
{
	iRect clRect = ClientRect();

	AddChild(m_pSpellListView = new iSpellListView(m_pMgr, this, iRect(clRect.x+5, clRect.y+30, clRect.w-10, clRect.h-59), 500, m_pOwner, (m_typeMask&SPT_MASK_OVERLAND)==0 ));

	sint32 npos = clRect.x + (clRect.w/2-62);
	AddChild(new iTextButton(m_pMgr,this,iRect(npos,clRect.y2()-DEF_BTN_HEIGHT,60,DEF_BTN_HEIGHT), TRID_CAST, DRC_YES, Visible));
	AddChild(new iTextButton(m_pMgr,this,iRect(npos+65,clRect.y2()-DEF_BTN_HEIGHT,60,DEF_BTN_HEIGHT), TRID_CANCEL, DRC_CANCEL));

	m_pSpellListView->SetMask(m_typeMask);
}

void iDlg_CastSpell::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);

	// Caster name
	gTextComposer.TextOut(gApp.Surface(),rc,iFormat(_T("#S0#FEEE%s#S2#FAFF %s #S0#FEEE%s %d"),gTextMgr[TRID_HERO_TYPE_KNIGHT+m_pOwner->Type()],m_pOwner->Name().CStr(),gTextMgr[TRID_LEVEL],m_pOwner->Level()),rc,AlignTop);
	rc.DeflateRect(0,13,0,0);
	gTextComposer.TextOut(gApp.Surface(),rc,iFormat(_T("#FC83%s: #FFB5%d / %d"), gTextMgr[TRID_MANA],m_pOwner->ManaPts(),m_pOwner->MaxManaPts()),rc,AlignTop);

}

iSize iDlg_CastSpell::ClientSize() const
{
	return iSize(280,180 + DEF_BTN_HEIGHT);
}

void iDlg_CastSpell::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	if (pView->GetUID() == 500) {
		if (m_pSpellListView) {
			GetChildById(DRC_YES)->SetEnabled(m_pSpellListView->CanCastSelSpell());
			if (cmd == CCI_LBSELDBLCLICK && m_pSpellListView->CanCastSelSpell()) EndDialog(DRC_YES);
		}
	} else {
		if (cmd == CCI_BTNCLICK) {
			EndDialog(pView->GetUID());
		}
	}
}



