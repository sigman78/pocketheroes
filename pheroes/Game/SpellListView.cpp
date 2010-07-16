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
#include "SpellListView.h"
#include "Dlg_SpellInfo.h"

//////////////////////////////////////////////////////////////////////////
const uint32 SPELL_SET_MASKS[SSM_COUNT - 1] = {
	MST_MASK_ALL,
	MST_MASK_AIR,
	MST_MASK_EARTH,
	MST_MASK_FIRE,
	MST_MASK_WATER
};

//////////////////////////////////////////////////////////////////////////
class iSpellListBox : public iListBox
{
public:

	iSpellListBox(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid, iHero* pOwner)
	: iListBox(pViewMgr, pCmdHandler, rect, uid), m_pOwner(pOwner)
	{ }

	inline uint32 LBItemHeight() const { return 15; }
	inline uint32 LBItemsCount() const { return m_spells.GetSize(); }

	MAGIC_SPELL SelSpell() const { if (SelItem() == -1) return MSP_INVALID; else return (MAGIC_SPELL)m_spells[SelItem()]; }

	void SetOwner(iHero* pOwner)
	{ m_pOwner = pOwner; m_spells.RemoveAll(); }

	void UpdateSpellList(uint32 typeMask, SPELL_SET_MODES setMode)
	{
		if (!m_pOwner) return;
		iSortArray<uint16> spells;
		if (setMode == SSM_FAVORITES) {
			for (uint32 xx=0; xx<m_pOwner->SpellBook().FavoritesCount(); ++xx) {
				if (typeMask & (1<<m_pOwner->SpellBook().FavoriteByIdx(xx)->Type())) {
					spells.Insert(m_pOwner->SpellBook().Favorite(xx), SPELL_DESCRIPTORS[m_pOwner->SpellBook().Favorite(xx)].level);
				}
			}
		} else {
			uint32 spmask = SPELL_SET_MASKS[setMode];
			for (uint32 xx=0; xx<m_pOwner->SpellBook().SpellsCount(); ++xx) {
				if ( (typeMask & (1<<m_pOwner->SpellBook().SpellByIdx(xx)->Type())) && (spmask & (1<<m_pOwner->SpellBook().SpellByIdx(xx)->School()))) {
					spells.Insert(m_pOwner->SpellBook().Spell(xx), SPELL_DESCRIPTORS[m_pOwner->SpellBook().Spell(xx)].level);
				}
			}
		}

		m_spells.RemoveAll();
		for (uint32 xx=0; xx<spells.Size(); ++xx) m_spells.Add(spells.Value(xx));
		UpdateListBox();
	}

private:
	void ComposeLBBackground(const iRect& rect)
	{
		iRect rc = rect;
		gApp.Surface().Darken25Rect(rc);
		rc.InflateRect(1);
		gApp.Surface().FrameRect(rc, cColor_Black);
	}

	void ComposeLBItem(uint32 iIdx, bool bSel, const iRect& irc)
	{
		iRect rc=irc;

		if (bSel) {
			gGfxMgr.BlitTile(PDGG_CTILE, gApp.Surface(),rc);
			ButtonFrame(gApp.Surface(),rc,0);
		}

		rc.DeflateRect(2);

		if (!m_pOwner) return;

		// School Level Name and Cost
		iStringT text(gTextMgr[TRID_SPNAME_MAGICARROW+m_spells[iIdx]]);
		uint8 cost = iBaseSpell::GetSpellCost((MAGIC_SPELL)m_spells[iIdx], m_pOwner);
		uint8 school = SPELL_DESCRIPTORS[m_spells[iIdx]].school;
		text.Addf(_T(" (%d)"), cost);
		iTextComposer::FontConfig fc(iTextComposer::FS_SMALL,MSCH_COLORS[school][0]);
		SpriteId icn = PDGG_MSCHICN_S+school;
		if (m_pOwner->ManaPts() < cost) {
			fc.cmpProps.alpha = 128;
		}
		gGfxMgr.Blit(icn, gApp.Surface(), rc.point());
		rc.x += 12;
		gTextComposer.TextOut(fc, gApp.Surface(),iPoint(rc.x,rc.y),text,rc, AlignLeft);
	}

private:
	iSpellList	m_spells;
	iHero*		m_pOwner;
};

//////////////////////////////////////////////////////////////////////////
const SpriteId SCHOOL_SWITCH_ICONS[SSM_COUNT] = {
	PDGG_ICN_ALL_SCHOOLS,
	PDGG_MSCHICN_S,
	PDGG_MSCHICN_S + 1,
	PDGG_MSCHICN_S + 2,
	PDGG_MSCHICN_S + 3,
	PDGG_ICN_FAVORITES
};

//////////////////////////////////////////////////////////////////////////
iSpellListView::iSpellListView(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid, iHero* pOwner, bool bShowFavorites)
: iView(pViewMgr, rect, GENERIC_VIEWPORT, uid, Visible|Enabled)
, m_pCmdHandler(pCmdHandler)
, m_pOwner(pOwner)
, m_typeMask(0), m_curSel(MSP_INVALID) 
{
	sint32 bcnt = SSM_COUNT;
	if  (!bShowFavorites) bcnt--;

	sint32 btnW = iMAX<sint32>(rect.w/2,140) / bcnt;
	sint32 sbW = btnW*bcnt - 16;

	iPHScrollBar* pScrollBar = new iPHScrollBar(m_pMgr, this, iRect(sbW+1, 0, 15, rect.h-16), uid+1);
	AddChild(pScrollBar);
	m_pSpellListBox = new iSpellListBox(m_pMgr, this, iRect(0, 0, sbW, rect.h-16), uid, m_pOwner);
	AddChild(m_pSpellListBox);
	m_pSpellListBox->SetScrollBar(pScrollBar);

	AddChild(m_pSchoolSwitch = new iBarTabSwitch(pViewMgr, this, iRect(0, rect.h-15, btnW * bcnt, 15), bcnt, uid + 10));
	for (uint32 xx=0; xx<bcnt; ++xx) m_pSchoolSwitch->SetTabIcon(SCHOOL_SWITCH_ICONS[xx]);
	if (m_pOwner) {
		if (m_pOwner->GetSpellSetMode() == SSM_FAVORITES && !bShowFavorites) m_pSchoolSwitch->SetCurrentTab(SSM_ALL);
		else m_pSchoolSwitch->SetCurrentTab(m_pOwner->GetSpellSetMode());
	}

	iRect rrc(rect.size());
	rrc.DeflateRect(sbW + 16 + 5, 0, 0, 0);
	AddChild(m_pSpellButton = new iSpellBtn(m_pMgr, this, iRect(rrc.x + (rrc.w/2 - 30), 30, 61,36), uid + 20));
}

void iSpellListView::SetOwner(iHero* pOwner)
{
	m_pOwner = pOwner;
	m_pSpellListBox->SetOwner(m_pOwner);
	if (m_pOwner) m_pSchoolSwitch->SetCurrentTab(m_pOwner->GetSpellSetMode());
	UpdateSpellList();
}

void iSpellListView::SelNext()
{
	m_pSpellListBox->SelNext();
}

void iSpellListView::SelPrev()
{
	m_pSpellListBox->SelPrev();
}

void iSpellListView::UpdateSpellList()
{
	m_pSpellListBox->UpdateSpellList(m_typeMask, (SPELL_SET_MODES)m_pSchoolSwitch->GetCurrentTab());

	/*
	m_schoolMask = 0;
	for (uint32 xx=0; xx<4; ++xx) if (m_pSchoolFilterBtns[xx]->IsChecked()) m_schoolMask |= (1<<xx);
	m_pSpellListBox->UpdateSpellList(m_typeMask, m_schoolMask);
	*/
}

void iSpellListView::OnCompose() 
{
	iRect rc = GetScrRect();

	// Buttons frame;
	iRect brc = m_pSchoolSwitch->GetScrRect();
	brc.InflateRect(1);
	gApp.Surface().FrameRect(brc, cColor_Black);


	rc.DeflateRect(m_pSchoolSwitch->GetRect().x2() + 5, -1, -1, -1);

	// Frame
	gApp.Surface().FrameRect(rc, cColor_Black);
	gApp.Surface().Darken25Rect(rc);
	rc.DeflateRect(3);


	// Spell
	if (m_curSel != MSP_INVALID) {
		// Name
		gTextComposer.TextOut(iTextComposer::FontConfig(iTextComposer::FS_MEDIUM,MSCH_COLORS[SPELL_DESCRIPTORS[m_curSel].school][0]), gApp.Surface(),iPoint(rc.x,rc.y),gTextMgr[TRID_SPNAME_MAGICARROW+m_curSel],rc, AlignTop);
		rc.DeflateRect(0,13,0,0);
		// Level
		gTextComposer.TextOut(iTextComposer::FontConfig(iTextComposer::FS_SMALL,MSCH_COLORS[SPELL_DESCRIPTORS[m_curSel].school][0]), gApp.Surface(),iPoint(rc.x,rc.y),gTextMgr[TRID_LEVEL] + iFormat(_T(": %d"), SPELL_DESCRIPTORS[m_curSel].level + 1),rc, AlignTop);
		rc.DeflateRect(0,16,0,0);
		// Icon
		rc.DeflateRect(0,40,0,0);
		// Hero school level
		ComposeSpellLevel(gApp.Surface(), rc, m_curSel, m_pOwner);
		rc.DeflateRect(0,12,0,0);
		// Cost
		uint8 cost = iBaseSpell::GetSpellCost(m_curSel, m_pOwner);
		uint8 school = SPELL_DESCRIPTORS[m_curSel].school;
		gTextComposer.TextOut(iTextComposer::FontConfig(iTextComposer::FS_SMALL,MSCH_COLORS[school][0]), gApp.Surface(),iPoint(rc.x,rc.y),gTextMgr[TRID_COST]+iFormat(_T(" %d"),cost),rc, AlignTop);
		rc.DeflateRect(0,20,0,0);
		// Inaccessebility reason
		if (m_pOwner->ManaPts() < cost) {
			iStringT msgStr(gTextMgr[TRID_MSG_SPELL_NOT_ENOUGH_MANA]);
			iTextComposer::FontConfig fntSmall(iTextComposer::FS_SMALL,RGB16(255,96,0));
			gTextComposer.TextBoxOut( fntSmall, gApp.Surface(), msgStr,rc);
		} else if (iCombatSpell* pCombatSpell = DynamicCast<iCombatSpell*>(gGame.ItemMgr().m_spellMgr.Spell(m_curSel))) {
			iTextComposer::FontConfig fntSmallDifferentColor(iTextComposer::FS_SMALL,RGB16(255,255,0));
			gTextComposer.TextBoxOut( fntSmallDifferentColor, gApp.Surface(), pCombatSpell->ActionText(m_pOwner),rc);
		}
	} else {
		iTextComposer::FontConfig fntSmall(iTextComposer::FS_SMALL,RGB16(255,96,0));
		gTextComposer.TextBoxOut(fntSmall, gApp.Surface(), gTextMgr[TRID_NO_SPELL_SELECTED],rc);
	}
}

void iSpellListView::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	if (pView->GetUID() == m_UID) {
		if (cmd == CCI_LBSELCHANGED || cmd == CCI_LBSELDBLCLICK) {
			m_curSel = m_pSpellListBox->SelSpell();
			((iSpellBtn*)GetChildById(m_UID + 20))->SetSpell((m_curSel == -1)?NULL:gGame.ItemMgr().m_spellMgr.Spell(m_curSel));
			if (m_pCmdHandler) m_pCmdHandler->iCMDH_ControlCommand(pView, cmd, param);
		}
	} else if (pView->GetUID() == m_UID + 10) {
		if (m_pOwner) m_pOwner->SetSpellSetMode((SPELL_SET_MODES)m_pSchoolSwitch->GetCurrentTab());
		UpdateSpellList();
	} else if (pView->GetUID() == m_UID + 20) {
		check(m_curSel != MSP_INVALID);
		MAGIC_SCHOOL_LEVEL ssl = iBaseSpell::GetSchoolLevel(m_curSel, m_pOwner);
		iSpellInfoDlg dlg(&gApp.ViewMgr(), m_curSel, ssl, (m_pOwner)?m_pOwner->Owner()->PlayerId():PID_NEUTRAL);
		dlg.DoModal();
	}
}


