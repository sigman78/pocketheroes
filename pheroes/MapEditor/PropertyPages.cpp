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
#include "PropertyPages.h"

/*
 *	
 */
iPropsPage_Army::iPropsPage_Army(iArmy& _army, LPCTSTR caption) 
: CPropertyPageImpl<iPropsPage_Army>(caption), army(_army) 
{}

LRESULT iPropsPage_Army::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LPCTSTR cnames[CREAT_COUNT];
	for (sint32 yy=0; yy<CREAT_COUNT; ++yy) cnames[yy] = gItemMgr.m_TextMgr[CREAT_NAMEKEY[yy]].CStr();
	BOOL bCustom = FALSE;
	for (uint32 xx=0; xx<7; ++xx) {
		CComboBox cb(GetDlgItem(IDC_SLOT1_CRTYPE+xx));
		CUpDownCtrl sp(GetDlgItem(IDC_SLOT1_QUANTSP+xx));
		cb.AddString(_T("- None -"));
		for (yy=0; yy<CREAT_COUNT; ++yy) cb.AddString(cnames[yy]);
		if (army[xx].IsValid()) {
			cb.SetCurSel(army[xx].Type() + 1);
			LI::SetDlgItemInt(m_hWnd,IDC_SLOT1_QUANTED+xx,army[xx].Count());
			bCustom = TRUE;
		} else cb.SetCurSel(0);
		sp.SetRange32(0,65535);
		UpdateSlot(xx);
	}
	CheckDlgButton(IDC_CUSTOMIZE_CHECK,bCustom);

	UpdatePage();
	return TRUE;
}

LRESULT iPropsPage_Army::OnCustomizeCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UpdatePage();
	return 0;
}

LRESULT iPropsPage_Army::OnSlotTypeChanged(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UpdateSlot(wID-IDC_SLOT1_CRTYPE);
	return 0;
}

void iPropsPage_Army::UpdateSlot(uint32 idx)
{
	check(idx<7);
	CComboBox cb(GetDlgItem(IDC_SLOT1_CRTYPE+idx));
	CEdit ed(GetDlgItem(IDC_SLOT1_QUANTED+idx));
	CEdit sp(GetDlgItem(IDC_SLOT1_QUANTSP+idx));
	if (cb.GetCurSel() == 0) {
		ed.SetWindowText(_T("0"));
		ed.EnableWindow(FALSE);
		sp.EnableWindow(FALSE);
	} else {
		ed.EnableWindow(TRUE);
		sp.EnableWindow(TRUE);
	}
}

void iPropsPage_Army::UpdatePage()
{
	BOOL bCustomize = IsDlgButtonChecked(IDC_CUSTOMIZE_CHECK);
	for (uint32 xx=0; xx<7; ++xx) {
		::EnableWindow(GetDlgItem(IDC_SLOT1_CRTYPE+xx), bCustomize);
		if (bCustomize)	UpdateSlot(xx);
		else {
			::EnableWindow(GetDlgItem(IDC_SLOT1_QUANTED+xx), bCustomize);
			::EnableWindow(GetDlgItem(IDC_SLOT1_QUANTSP+xx), bCustomize);
		}
	}
}

int iPropsPage_Army::OnApply()
{
	BOOL bCustomize = IsDlgButtonChecked(IDC_CUSTOMIZE_CHECK);
	for (uint32 xx=0; xx<7; ++xx){
		if (bCustomize) {
			CComboBox cb(GetDlgItem(IDC_SLOT1_CRTYPE+xx));
			CREATURE_TYPE ct = (CREATURE_TYPE)(cb.GetCurSel()-1);
			sint32 cnt = (ct == CREAT_UNKNOWN)?0:LI::GetDlgItemInt(m_hWnd,IDC_SLOT1_QUANTED+xx);
			army.SetGroup(xx, ct, cnt);
		} else {
			army[xx].Reset();
		}
	}

	return PSNRET_NOERROR;
}

/*
 *	
 */
iPropsPage_Message::iPropsPage_Message(iTextMgr& _tmgr, iStringT& _key, void* _pItem, TextEntryType _tet, LPCTSTR caption)
: CPropertyPageImpl<iPropsPage_Message>(caption), tmgr(_tmgr), key(_key), pItem(_pItem), tet(_tet) 
{
}

LRESULT iPropsPage_Message::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!key.Empty()) {
		check(tmgr.HasEntry(key));
		CheckDlgButton(IDC_CUSTOMIZE_CHECK, TRUE);
		LI::SetDlgItemText(m_hWnd, IDC_MSG_EDIT, tmgr[key]);
	} else CheckDlgButton(IDC_CUSTOMIZE_CHECK, FALSE);

	UpdatePage();
	return TRUE;
}

LRESULT iPropsPage_Message::OnCustomizeCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UpdatePage();
	return 0;
}

void iPropsPage_Message::UpdatePage()
{
	BOOL bCustomize = IsDlgButtonChecked(IDC_CUSTOMIZE_CHECK);
	::EnableWindow(GetDlgItem(IDC_MSG_EDIT), bCustomize);
}

int iPropsPage_Message::OnApply()
{
	iStringT nmsg;
	LI::GetDlgItemText(m_hWnd,IDC_MSG_EDIT, nmsg);
	if (IsDlgButtonChecked(IDC_CUSTOMIZE_CHECK) && !nmsg.Empty()) {
		if (!key.Empty()) {
			check(tmgr.HasEntry(key));
			tmgr.GetText(key, GLNG_ENGLISH) = nmsg;
		} else {
			key = MakeTextEntryName(tet, pItem);
			tmgr.AddTextEntry(key, nmsg, tet);
		}
	} else {
		if (!key.Empty()) {
			check(tmgr.HasEntry(key));
			tmgr.RemoveEntry(key);
			key = iStringT();
		}
	}

	return PSNRET_NOERROR;
}

/*
 *	Gen Map event
 */
iPropsPage_GenMapEvent::iPropsPage_GenMapEvent(iMapHandler& _map, iMapEvent& _mapEvent)
: CPropertyPageImpl<iPropsPage_GenMapEvent>(_T("General")), map(_map), mapEvent(_mapEvent) 
{
}

LRESULT iPropsPage_GenMapEvent::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Messages
	if (!mapEvent.RewardMessage().Empty()) {
		check(map.m_ctMgr.HasEntry(mapEvent.RewardMessage()));
		LI::SetDlgItemText(m_hWnd, IDC_REWARDMSG_EDIT, map.m_ctMgr[mapEvent.RewardMessage()]);
	}
	if (!mapEvent.AttackMessage().Empty()) {
		check(map.m_ctMgr.HasEntry(mapEvent.AttackMessage()));
		LI::SetDlgItemText(m_hWnd, IDC_ATTACKMSG_EDIT, map.m_ctMgr[mapEvent.AttackMessage()]);
	}

	// Affects to players
	for (uint32 pp=0; pp<PID_COUNT; ++pp) {
		::EnableWindow(GetDlgItem(IDC_PLAYER1+pp), map.m_pPlayers[pp] != NULL);
		CheckDlgButton(IDC_PLAYER1+pp, map.m_pPlayers[pp] != NULL && ((mapEvent.PlayerMask() & (1<<pp)) != 0) );
	}

	return TRUE;
}

int iPropsPage_GenMapEvent::OnApply()
{
	// Messages
	iStringT amsg,rmsg;
	LI::GetDlgItemText(m_hWnd,IDC_REWARDMSG_EDIT, rmsg);
	LI::GetDlgItemText(m_hWnd,IDC_ATTACKMSG_EDIT, amsg);
	
	// Reward
	if (!rmsg.Empty()) {
		if (!mapEvent.RewardMessage().Empty()) {
			map.m_ctMgr.GetText(mapEvent.RewardMessage(), GLNG_ENGLISH) = rmsg;
		} else {
			mapEvent.RewardMessage() = MakeTextEntryName(TET_MEVENT_REWMSG,&mapEvent);
			map.m_ctMgr.AddTextEntry(mapEvent.RewardMessage(), rmsg, TET_MEVENT_REWMSG);
		}
	} else {
		if (!mapEvent.RewardMessage().Empty()) {
			check(map.m_ctMgr.HasEntry(mapEvent.RewardMessage()));
			map.m_ctMgr.RemoveEntry(mapEvent.RewardMessage());
			mapEvent.RewardMessage() = iStringT();
		}
	}

	// Attack
	if (!amsg.Empty()) {
		if (!mapEvent.AttackMessage().Empty()) {
			map.m_ctMgr.GetText(mapEvent.AttackMessage(), GLNG_ENGLISH) = amsg;
		} else {
			mapEvent.AttackMessage() = MakeTextEntryName(TET_MEVENT_ATTMSG,&mapEvent);
			map.m_ctMgr.AddTextEntry(mapEvent.AttackMessage(), amsg, TET_MEVENT_ATTMSG);
		}
	} else {
		if (!mapEvent.AttackMessage().Empty()) {
			check(map.m_ctMgr.HasEntry(mapEvent.AttackMessage()));
			map.m_ctMgr.RemoveEntry(mapEvent.AttackMessage());
			mapEvent.AttackMessage() = iStringT();
		}
	}

	// Affects to players
	mapEvent.PlayerMask() = PIM_NONE;
	for (uint32 pp=0; pp<PID_COUNT; ++pp) {
		if (map.m_pPlayers[pp] == NULL || IsDlgButtonChecked(IDC_PLAYER1+pp)) mapEvent.PlayerMask() |= (1<<pp);
	}

	return PSNRET_NOERROR;
}

/*
 *	Gen Mineral
 */
iPropsPage_GenMineral::iPropsPage_GenMineral(MINERAL_TYPE _mt, sint32& _cnt)
: CPropertyPageImpl<iPropsPage_GenMineral>(_T("General")), mt(_mt), cnt(_cnt)
{}

LRESULT iPropsPage_GenMineral::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LI::SetDlgItemText(m_hWnd, IDC_MT_STATIC, gItemMgr.m_TextMgr[RESOURCE_NAMEKEY[mt]]);
	CUpDownCtrl sp(GetDlgItem(IDC_QUANTITY_SPIN));
	sp.SetRange32(1,65535);

	if (cnt == RANDOM_QUANTITY) {
		CheckDlgButton(IDC_RADIO_RANDOM, TRUE);
		CheckDlgButton(IDC_RADIO_CUSTOM, FALSE);
	} else {
		CheckDlgButton(IDC_RADIO_RANDOM, FALSE);
		CheckDlgButton(IDC_RADIO_CUSTOM, TRUE);
		LI::SetDlgItemInt(m_hWnd,IDC_QUANTITY_EDIT,cnt);
	}
	UpdatePage();
	return TRUE;
}

LRESULT iPropsPage_GenMineral::OnCustomRadio(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UpdatePage();
	return 0;
}

void iPropsPage_GenMineral::UpdatePage()
{
	BOOL bCustomize = IsDlgButtonChecked(IDC_RADIO_CUSTOM);
	::EnableWindow(GetDlgItem(IDC_QUANTITY_EDIT), bCustomize);
	::EnableWindow(GetDlgItem(IDC_QUANTITY_SPIN), bCustomize);
}

int iPropsPage_GenMineral::OnApply()
{
	if (IsDlgButtonChecked(IDC_RADIO_CUSTOM)) {
		cnt = LI::GetDlgItemInt(m_hWnd,IDC_QUANTITY_EDIT);
	} else {
		cnt = RANDOM_QUANTITY;
	}
	return PSNRET_NOERROR;
}

/*
 *	Gen Spell Scroll
 */
iPropsPage_GenSpellScroll::iPropsPage_GenSpellScroll(iMapItem_SpellScroll& spellScroll)
: CPropertyPageImpl<iPropsPage_GenSpellScroll>(_T("General")), m_spellScroll(spellScroll)
{}

LRESULT iPropsPage_GenSpellScroll::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CComboBox cb(GetDlgItem(IDC_SPELL_COMBO));
	for (uint32 xx=0; xx<MSP_COUNT; ++xx) {
		cb.AddString(gItemMgr.m_TextMgr[SPELL_NAMEKEY[xx]].CStr());
	}

	cb.SetCurSel(m_spellScroll.SpellId());

	return TRUE;
}

int iPropsPage_GenSpellScroll::OnApply()
{
	CComboBox cb(GetDlgItem(IDC_SPELL_COMBO));
	m_spellScroll.SpellId() = (uint16)cb.GetCurSel();
	return PSNRET_NOERROR;
}

/*
 *	Map Guard general page
 */
iPropsPage_GenGuard::iPropsPage_GenGuard(iMapGuard& guard)
: CPropertyPageImpl<iPropsPage_GenGuard>(_T("General")), m_guard(guard) {}

LRESULT iPropsPage_GenGuard::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Type
	LI::SetDlgItemText(m_hWnd, IDC_CT_STATIC, m_guard.Title());

	// Quantity
	CUpDownCtrl sp(GetDlgItem(IDC_QUANTITY_SPIN));
	sp.SetRange32(1,65535);
	if (m_guard.Creatures().Count() == RANDOM_QUANTITY) {
		CheckDlgButton(IDC_RADIO_RANDOM, TRUE);
		CheckDlgButton(IDC_RADIO_CUSTOM, FALSE);
	} else {
		CheckDlgButton(IDC_RADIO_RANDOM, FALSE);
		CheckDlgButton(IDC_RADIO_CUSTOM, TRUE);
	}
	LI::SetDlgItemInt(m_hWnd,IDC_QUANTITY_EDIT,m_guard.Creatures().Count());

	// States
	CheckDlgButton(IDC_DISP01+m_guard.Disposition(),TRUE);
	CheckDlgButton(IDC_NOTGROW,m_guard.NotGrow());

	UpdatePage();
	return TRUE;
}

LRESULT iPropsPage_GenGuard::OnCustomRadio(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UpdatePage();
	return 0;
}

void iPropsPage_GenGuard::UpdatePage()
{
	BOOL bCustomize = IsDlgButtonChecked(IDC_RADIO_CUSTOM);
	::EnableWindow(GetDlgItem(IDC_QUANTITY_EDIT), bCustomize);
	::EnableWindow(GetDlgItem(IDC_QUANTITY_SPIN), bCustomize);
}

int iPropsPage_GenGuard::OnApply()
{
	// Quantity
	if (IsDlgButtonChecked(IDC_RADIO_CUSTOM)) {
		m_guard.Creatures().Count() = LI::GetDlgItemInt(m_hWnd,IDC_QUANTITY_EDIT);
	} else {
		m_guard.Creatures().Count() = RANDOM_QUANTITY;
	}

	// States
	for (uint32 xx=0; xx<GDISP_COUNT; ++xx) if (IsDlgButtonChecked(IDC_DISP01+xx)) { m_guard.Disposition() = (GUARD_DISPOS)xx; break;}
	m_guard.NotGrow() = IsDlgButtonChecked(IDC_NOTGROW)>0;

	return PSNRET_NOERROR;
}

/*
 *	Ownerable construction general page
 */
iPropsPage_GenOwnCnst::iPropsPage_GenOwnCnst(iOwnCnst& ownCnst, const iMapHandler& map)
: CPropertyPageImpl<iPropsPage_GenOwnCnst>(_T("General")), m_ownCnst(ownCnst), m_map(map) {}

LRESULT iPropsPage_GenOwnCnst::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	sint32 o = m_ownCnst.Owner();
	for (uint32 xx=PID_RED; xx<PID_COUNT; ++xx) ::EnableWindow(GetDlgItem(ID_PLAYER1+xx),m_map.m_pPlayers[xx]!=NULL);
	CheckDlgButton(ID_PLAYER_NEUTRAL+m_ownCnst.Owner()+1,TRUE);
	return TRUE;
}

int iPropsPage_GenOwnCnst::OnApply()
{
	for (sint32 xx=-1; xx<PID_COUNT; ++xx) if (IsDlgButtonChecked(ID_PLAYER_NEUTRAL+xx+1)) { m_ownCnst.SetOwner((PLAYER_ID)xx); break;}
	return PSNRET_NOERROR;
}

/*
 *	Castle general page
 */
iPropsPage_GenCastle::iPropsPage_GenCastle(iCastle& castle, iMapHandler& map)
: CPropertyPageImpl<iPropsPage_GenCastle>(_T("General")), m_castle(castle), m_map(map) {}

LRESULT iPropsPage_GenCastle::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!m_castle.CustomName().Empty()) {
		check(m_map.m_ctMgr.HasEntry(m_castle.CustomName()));
		SetDlgItemText(IDC_CUSTOMIZE_EDIT,m_map.m_ctMgr[m_castle.CustomName()].CStr());
		CheckDlgButton(IDC_CUSTOMIZE_CHECK,TRUE);
	} else {
		SetDlgItemText(IDC_CUSTOMIZE_EDIT,_T("Default"));
		::EnableWindow(GetDlgItem(IDC_CUSTOMIZE_EDIT), FALSE);
	}

	for (uint32 xx=PID_RED; xx<PID_COUNT; ++xx) ::EnableWindow(GetDlgItem(ID_PLAYER1+xx),m_map.m_pPlayers[xx]!=NULL);
	CheckDlgButton(ID_PLAYER_NEUTRAL+m_castle.Owner()+1,TRUE);
	return TRUE;
}

LRESULT iPropsPage_GenCastle::OnCustomizeCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (IsDlgButtonChecked(IDC_CUSTOMIZE_CHECK)) {
		::EnableWindow(GetDlgItem(IDC_CUSTOMIZE_EDIT), TRUE);
	} else {
		SetDlgItemText(IDC_CUSTOMIZE_EDIT,_T("Default"));
		::EnableWindow(GetDlgItem(IDC_CUSTOMIZE_EDIT), FALSE);
	}
	return TRUE;
}

int iPropsPage_GenCastle::OnApply()
{
	iStringT cname = LI::GetDlgItemText(m_hWnd, IDC_CUSTOMIZE_EDIT);
	if (IsDlgButtonChecked(IDC_CUSTOMIZE_CHECK) && !cname.Empty()) {
		if (!m_castle.CustomName().Empty()) {
			m_map.m_ctMgr.GetText(m_castle.CustomName(), GLNG_ENGLISH) = cname;
		} else {
			m_castle.CustomName() = MakeTextEntryName(TET_CASTLE_CNAME,&m_castle);
			m_map.m_ctMgr.AddTextEntry(m_castle.CustomName(), cname, TET_CASTLE_CNAME);
		}
	} else {
		if (!m_castle.CustomName().Empty()) {
			check(m_map.m_ctMgr.HasEntry(m_castle.CustomName()));
			m_map.m_ctMgr.RemoveEntry(m_castle.CustomName());
		}
	}

	for (sint32 xx=-1; xx<PID_COUNT; ++xx) 
		if (IsDlgButtonChecked(ID_PLAYER_NEUTRAL+xx+1)) {
			m_map.ChangeCastleOwner(&m_castle, (PLAYER_ID)xx);
			break;
		}
	return PSNRET_NOERROR;
}

/*
 *	Castle's constructions
 */
iPropsPage_CtlCnsts::iPropsPage_CtlCnsts(iCastle& castle, const iMapHandler& map)
: CPropertyPageImpl<iPropsPage_CtlCnsts>(_T("Constructions")), m_castle(castle), m_map(map), m_bldFont(NULL) {}

iPropsPage_CtlCnsts::~iPropsPage_CtlCnsts()
{
	if (m_bldFont != NULL) DeleteObject(m_bldFont);
}

LRESULT iPropsPage_CtlCnsts::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CheckDlgButton(IDC_CUSTOMIZE_CHECK,m_castle.CtlCnsts().IsCustomized());

	HFONT font = GetFont();
	LOGFONT lf;
	GetObject(font, sizeof(lf),&lf);
	lf.lfWeight = FW_BOLD;
	m_bldFont = CreateFontIndirect (&lf);

	CListBox lb(GetDlgItem(IDC_CNST_LIST));
	for (uint32 xx=0; xx<m_castle.CtlCnsts().Count(); ++xx) {
		lb.AddString(gItemMgr.m_TextMgr[CTLCNST_NAMEKEY[m_castle.CtlCnsts().Entry(xx).type]].CStr());
	}

	UpdatePage();
	return TRUE;
}

LRESULT iPropsPage_CtlCnsts::OnCustomizeCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_castle.CtlCnsts().SetCustomized(IsDlgButtonChecked(IDC_CUSTOMIZE_CHECK)>0);
	UpdatePage();
	return TRUE;
}

LRESULT iPropsPage_CtlCnsts::OnBuilt(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	sint32 csel = CListBox(GetDlgItem(IDC_CNST_LIST)).GetCurSel();
	if (csel != -1) m_castle.CtlCnsts().Entry(csel).state = IsDlgButtonChecked(IDC_BUILT);
	::InvalidateRect(GetDlgItem(IDC_CNST_LIST), NULL, FALSE);
	return TRUE;
}

LRESULT iPropsPage_CtlCnsts::OnCnstList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (wNotifyCode == LBN_SELCHANGE) {
		UpdatePage();
	}
	return TRUE;
}

void iPropsPage_CtlCnsts::UpdatePage()
{
	sint32 csel = CListBox(GetDlgItem(IDC_CNST_LIST)).GetCurSel();
	::EnableWindow(GetDlgItem(IDC_CNST_LIST), IsDlgButtonChecked(IDC_CUSTOMIZE_CHECK));
	::EnableWindow(GetDlgItem(IDC_BUILT), IsDlgButtonChecked(IDC_CUSTOMIZE_CHECK) && csel != -1);
	CheckDlgButton(IDC_BUILT, csel != -1 && m_castle.CtlCnsts().Entry(csel).IsBuilt());

	if (IsDlgButtonChecked(IDC_CUSTOMIZE_CHECK)) {
	} else {
	}
}

int iPropsPage_CtlCnsts::OnApply()
{
	return PSNRET_NOERROR;
}

void iPropsPage_CtlCnsts::DrawItem(LPDRAWITEMSTRUCT lpdi)
{
	if (lpdi->CtlID == IDC_CNST_LIST) {
		if (lpdi->itemID == -1)	return;

		// Drawing background rect
		if (lpdi->itemState & ODS_DISABLED) {
			HBRUSH nrm_br = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
			FillRect(lpdi->hDC, &lpdi->rcItem,nrm_br);
			DeleteObject(nrm_br);
		} else if ((lpdi->itemAction | ODA_SELECT) && (lpdi->itemState & ODS_SELECTED)) {
			HBRUSH sel_br = CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
			FillRect(lpdi->hDC, &lpdi->rcItem,sel_br);
			DeleteObject(sel_br);
		} else {
			HBRUSH nrm_br = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
			FillRect(lpdi->hDC, &lpdi->rcItem,nrm_br);
			DeleteObject(nrm_br);
		}

		if ((lpdi->itemAction | ODA_FOCUS) && (lpdi->itemState & ODS_FOCUS)) {
			// FrameRect(lpdi->hDC, &lpdi->rcItem,black_br);
		}


		// Drawing text
		InflateRect(&lpdi->rcItem, -4, 0);
		if (lpdi->itemState & ODS_DISABLED) {
			SetTextColor(lpdi->hDC, GetSysColor(COLOR_GRAYTEXT));
			SetBkColor(lpdi->hDC, GetSysColor(COLOR_WINDOW));
		} else if ((lpdi->itemAction | ODA_SELECT) && (lpdi->itemState & ODS_SELECTED)) {
			SetTextColor(lpdi->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
			SetBkColor(lpdi->hDC, GetSysColor(COLOR_HIGHLIGHT));
		} else {
			SetTextColor(lpdi->hDC, GetSysColor(COLOR_WINDOWTEXT));
			SetBkColor(lpdi->hDC, GetSysColor(COLOR_WINDOW));
		}

		LPCTSTR txt = (LPCTSTR)lpdi->itemData;
		if (m_castle.CtlCnsts().Entry(lpdi->itemID).IsBuilt()) {
			HGDIOBJ oldFont = ::SelectObject(lpdi->hDC, m_bldFont);
			::DrawText(lpdi->hDC, txt, _tcslen(txt), &lpdi->rcItem, DT_VCENTER);
			::SelectObject(lpdi->hDC, oldFont);
		} else ::DrawText(lpdi->hDC, txt, _tcslen(txt), &lpdi->rcItem, DT_VCENTER);

		// Clean
	}
}

/*
 *	Hero general page
 */
iPropsPage_GenHero::iPropsPage_GenHero(iHero& hero, iMapHandler& map)
: CPropertyPageImpl<iPropsPage_GenHero>(_T("General")), m_hero(hero), m_map(map) {}

LRESULT iPropsPage_GenHero::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!m_hero.CustomName().Empty()) {
		check(m_map.m_ctMgr.HasEntry(m_hero.CustomName()));
		SetDlgItemText(IDC_CUSTOMIZE_EDIT,m_map.m_ctMgr[m_hero.CustomName()].CStr());
		CheckDlgButton(IDC_CUSTOMIZE_CHECK,TRUE);
	} else {
		SetDlgItemText(IDC_CUSTOMIZE_EDIT,gItemMgr.m_TextMgr[m_hero.Proto()->NameKey()].CStr());
		::EnableWindow(GetDlgItem(IDC_CUSTOMIZE_EDIT), FALSE);
	}

	for (uint32 xx=PID_RED; xx<PID_COUNT; ++xx) ::EnableWindow(GetDlgItem(ID_PLAYER1+xx),m_map.m_pPlayers[xx]!=NULL);
	CheckDlgButton(ID_PLAYER_NEUTRAL+m_hero.Owner()+1,TRUE);

	CUpDownCtrl(GetDlgItem(IDC_PRSKILL_SPIN1)).SetRange32(0,255);
	CUpDownCtrl(GetDlgItem(IDC_PRSKILL_SPIN1)).SetPos(m_hero.FurtSkills().Value(FSK_ATTACK));
	CUpDownCtrl(GetDlgItem(IDC_PRSKILL_SPIN2)).SetRange32(0,255);
	CUpDownCtrl(GetDlgItem(IDC_PRSKILL_SPIN2)).SetPos(m_hero.FurtSkills().Value(FSK_DEFENCE));
	CUpDownCtrl(GetDlgItem(IDC_PRSKILL_SPIN3)).SetRange32(1,255);
	CUpDownCtrl(GetDlgItem(IDC_PRSKILL_SPIN3)).SetPos(m_hero.FurtSkills().Value(FSK_POWER));
	CUpDownCtrl(GetDlgItem(IDC_PRSKILL_SPIN4)).SetRange32(1,255);
	CUpDownCtrl(GetDlgItem(IDC_PRSKILL_SPIN4)).SetPos(m_hero.FurtSkills().Value(FSK_KNOWLEDGE));

	return TRUE;
}

LRESULT iPropsPage_GenHero::OnCustomizeCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (IsDlgButtonChecked(IDC_CUSTOMIZE_CHECK)) {
		::EnableWindow(GetDlgItem(IDC_CUSTOMIZE_EDIT), TRUE);
	} else {
		SetDlgItemText(IDC_CUSTOMIZE_EDIT,gItemMgr.m_TextMgr[m_hero.Proto()->NameKey()].CStr());
		::EnableWindow(GetDlgItem(IDC_CUSTOMIZE_EDIT), FALSE);
	}
	return TRUE;
}

int iPropsPage_GenHero::OnApply()
{
	iStringT cname = LI::GetDlgItemText(m_hWnd, IDC_CUSTOMIZE_EDIT);
	if (IsDlgButtonChecked(IDC_CUSTOMIZE_CHECK) && !cname.Empty()) {
		if (!m_hero.CustomName().Empty()) {
			m_map.m_ctMgr.GetText(m_hero.CustomName(), GLNG_ENGLISH) = cname;
		} else {
			m_hero.CustomName() = MakeTextEntryName(TET_HERO_CNAME,&m_hero);
			m_map.m_ctMgr.AddTextEntry(m_hero.CustomName(), cname, TET_HERO_CNAME);
		}
	} else {
		if (!m_hero.CustomName().Empty()) {
			check(m_map.m_ctMgr.HasEntry(m_hero.CustomName()));
			m_map.m_ctMgr.RemoveEntry(m_hero.CustomName());
			m_hero.CustomName() = iStringT();
		}
	}

	for (sint32 xx=-1; xx<PID_COUNT; ++xx) if (IsDlgButtonChecked(ID_PLAYER_NEUTRAL+xx+1)) { m_hero.Owner() = (PLAYER_ID)xx; break;}

	m_hero.FurtSkills().Value(FSK_ATTACK) = iCLAMP<sint32>(0,255,CUpDownCtrl(GetDlgItem(IDC_PRSKILL_SPIN1)).GetPos());
	m_hero.FurtSkills().Value(FSK_DEFENCE) = iCLAMP<sint32>(0,255,CUpDownCtrl(GetDlgItem(IDC_PRSKILL_SPIN2)).GetPos());
	m_hero.FurtSkills().Value(FSK_POWER) = iCLAMP<sint32>(1,255,CUpDownCtrl(GetDlgItem(IDC_PRSKILL_SPIN3)).GetPos());
	m_hero.FurtSkills().Value(FSK_KNOWLEDGE) = iCLAMP<sint32>(1,255,CUpDownCtrl(GetDlgItem(IDC_PRSKILL_SPIN4)).GetPos());

	return PSNRET_NOERROR;
}
/*
 *	Hero secondary skills
 */
iPropsPage_SecSkills::iPropsPage_SecSkills(iHero& hero, const iMapHandler& map)
: CPropertyPageImpl<iPropsPage_SecSkills>(_T("Secondary Skills")), m_hero(hero), m_map(map) {}

LRESULT iPropsPage_SecSkills::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	for (uint32 xx=0; xx<8; ++xx) {
		CComboBox cb(GetDlgItem(IDC_SKILL_TYPE1+xx));
		cb.AddString(_T("-none-"));
		for (uint32 ssid = 0; ssid<SECSK_COUNT; ++ssid){
			cb.AddString(gItemMgr.m_TextMgr[SECSK_NAMEKEY[ssid]].CStr());
		}

		CComboBox mcb(GetDlgItem(IDC_SKILL_LEVEL1+xx));
		for (uint32 smid = 0; smid<SSLVL_COUNT; ++smid){
			mcb.AddString(gItemMgr.m_TextMgr[SSLVL_NAMEKEY[smid]].CStr());
		}

		if (xx < m_hero.SecSkills().GetSize()) {
			cb.SetCurSel(m_hero.SecSkills()[xx].m_skill+1);
			sint32 lvl = m_hero.SecSkills()[xx].m_level;
			UpdateSlot(xx);
			mcb.SetCurSel(m_hero.SecSkills()[xx].m_level);
		} else {
			cb.SetCurSel(0);
			UpdateSlot(xx);
			mcb.SetCurSel(0);
		}
	}

	return TRUE;
}

LRESULT iPropsPage_SecSkills::OnSlotTypeChanged(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UpdateSlot(wID-IDC_SKILL_TYPE1);
	return 0;
}

void iPropsPage_SecSkills::UpdateSlot(sint32 idx)
{
	CComboBox cb(GetDlgItem(IDC_SKILL_TYPE1+idx));
	CComboBox mcb(GetDlgItem(IDC_SKILL_LEVEL1+idx));
	mcb.EnableWindow(cb.GetCurSel() != 0);
}

int iPropsPage_SecSkills::OnApply()
{
	m_hero.SecSkills().RemoveAll();
	for (uint32 xx=0; xx<8; ++xx) {
		CComboBox cb(GetDlgItem(IDC_SKILL_TYPE1+xx));
		CComboBox mcb(GetDlgItem(IDC_SKILL_LEVEL1+xx));
		if (cb.GetCurSel() != 0) {
			SECONDARY_SKILLS ss = (SECONDARY_SKILLS)(cb.GetCurSel()-1);
			for (uint32 yy=0; yy<m_hero.SecSkills().GetSize(); ++yy) {
				if (m_hero.SecSkills()[yy].m_skill == ss) {
					MessageBox(_T("Dublicated secondary skill!"));
					return PSNRET_INVALID;
				}
			}
			m_hero.SecSkills().Add(iSecSkillEntry(ss,(SECSKILL_LEVEL)mcb.GetCurSel()));
		}
	}

	return PSNRET_NOERROR;
}

/*
 *	Rewards
 */
class iRewardTypeDlg : public CDialogImpl<iRewardTypeDlg>
{
public:
	enum { IDD = IDD_REWARD_TYPE };

	BEGIN_MSG_MAP(iRewardTypeDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	iRewardTypeDlg() : m_rewardType(RIT_INVALID) {}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CheckDlgButton(IDC_REWARDT_1, TRUE);
		return 0;
	}

	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		for (uint32 xx=0; xx<RIT_COUNT; ++xx) {
			if (IsDlgButtonChecked(IDC_REWARDT_1+xx)) {
				m_rewardType = (REWARD_ITEM_TYPE)xx;
				break;
			}
		}
		check(m_rewardType != RIT_INVALID);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

	REWARD_ITEM_TYPE	m_rewardType;
};

//////////////////////////////////////////////////////////////////////////
class iRewDlg_Mineral : public CDialogImpl<iRewDlg_Mineral>
{
public:
	enum { IDD = IDD_REW_MINERAL };

	BEGIN_MSG_MAP(iRewDlg_Mineral)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CComboBox cb(GetDlgItem(IDC_MIN_TYPE));
		for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) {
			cb.AddString(gItemMgr.m_TextMgr[MINERAL_NAMEKEY[xx]].CStr());
		}
		cb.SetCurSel(MINERAL_GOLD);
		CUpDownCtrl span(GetDlgItem(IDC_MINERAL_QUANTSP));
		span.SetRange32(-32768, 32767);
		LI::SetDlgItemInt(m_hWnd,IDC_MINERAL_QUANTED, 1);
		return 0;
	}

	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_mineralType = (MINERAL_TYPE)CComboBox(GetDlgItem(IDC_MIN_TYPE)).GetCurSel();
		m_quant = LI::GetDlgItemInt(m_hWnd,IDC_MINERAL_QUANTED);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{ EndDialog(wID); return 0; }

	MINERAL_TYPE	m_mineralType;
	sint32			m_quant;
};
//////////////////////////////////////////////////////////////////////////
class iRewDlg_Experience : public CDialogImpl<iRewDlg_Experience>
{
public:
	enum { IDD = IDD_REW_EXPERIENCE };

	BEGIN_MSG_MAP(iRewDlg_Experience)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CUpDownCtrl span(GetDlgItem(IDC_QUANTSP));
		span.SetRange32(-100000,100000);
		LI::SetDlgItemInt(m_hWnd,IDC_QUANTED, 1000);
		return 0;
	}

	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_quant = LI::GetDlgItemInt(m_hWnd,IDC_QUANTED);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{ EndDialog(wID); return 0; }

	sint32			m_quant;
};
//////////////////////////////////////////////////////////////////////////
class iRewDlg_ManaPoints : public CDialogImpl<iRewDlg_ManaPoints>
{
public:
	enum { IDD = IDD_REW_MANAPTS };

	BEGIN_MSG_MAP(iRewDlg_ManaPoints)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CUpDownCtrl span(GetDlgItem(IDC_QUANTSP));
		span.SetRange32(-1000,1000);
		LI::SetDlgItemInt(m_hWnd,IDC_QUANTED, 10);
		return 0;
	}

	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_quant = LI::GetDlgItemInt(m_hWnd,IDC_QUANTED);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{ EndDialog(wID); return 0; }

	sint32			m_quant;
};
//////////////////////////////////////////////////////////////////////////
class iRewDlg_TravPoints : public CDialogImpl<iRewDlg_TravPoints>
{
public:
	enum { IDD = IDD_REW_TRAVELPTS };

	BEGIN_MSG_MAP(iRewDlg_TravPoints)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CUpDownCtrl span(GetDlgItem(IDC_QUANTSP));
		span.SetRange32(-1000,1000);
		LI::SetDlgItemInt(m_hWnd,IDC_QUANTED, 20);
		return 0;
	}

	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_quant = LI::GetDlgItemInt(m_hWnd,IDC_QUANTED);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{ EndDialog(wID); return 0; }

	sint32			m_quant;
};
//////////////////////////////////////////////////////////////////////////
class iRewDlg_Morale : public CDialogImpl<iRewDlg_Morale>
{
public:
	enum { IDD = IDD_REW_MORALE };

	BEGIN_MSG_MAP(iRewDlg_Morale)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CUpDownCtrl span(GetDlgItem(IDC_QUANTSP));
		span.SetRange32(-3,+3);
		LI::SetDlgItemInt(m_hWnd,IDC_QUANTED, +1);
		return 0;
	}

	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_quant = LI::GetDlgItemInt(m_hWnd,IDC_QUANTED);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{ EndDialog(wID); return 0; }

	sint32			m_quant;
};
//////////////////////////////////////////////////////////////////////////
class iRewDlg_Luck : public CDialogImpl<iRewDlg_Luck>
{
public:
	enum { IDD = IDD_REW_LUCK };

	BEGIN_MSG_MAP(iRewDlg_Luck)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CUpDownCtrl span(GetDlgItem(IDC_QUANTSP));
		span.SetRange32(-3,+3);
		LI::SetDlgItemInt(m_hWnd,IDC_QUANTED, +1);
		return 0;
	}

	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_quant = LI::GetDlgItemInt(m_hWnd,IDC_QUANTED);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{ EndDialog(wID); return 0; }

	sint32			m_quant;
};
//////////////////////////////////////////////////////////////////////////
class iRewDlg_FurtSkill : public CDialogImpl<iRewDlg_FurtSkill>
{
public:
	enum { IDD = IDD_REW_FURTSKILL };

	BEGIN_MSG_MAP(iRewDlg_FurtSkill)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CComboBox cb(GetDlgItem(IDC_SKILL_TYPE));
		for (uint32 xx=0; xx<PRSKILL_COUNT; ++xx) {
			cb.AddString(gItemMgr.m_TextMgr[PRSKILL_NAMEKEY[xx]].CStr());
		}
		cb.SetCurSel(PRSKILL_ATTACK);
		CUpDownCtrl span(GetDlgItem(IDC_SKILL_VALSP));
		span.SetRange32(-255,255);
		LI::SetDlgItemInt(m_hWnd,IDC_SKILL_VALED, 1);
		return 0;
	}

	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_skillType = (FURTHER_SKILLS)(CComboBox(FSK_ATTACK+GetDlgItem(IDC_SKILL_TYPE)).GetCurSel());
		m_val = LI::GetDlgItemInt(m_hWnd,IDC_SKILL_VALED);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{ EndDialog(wID); return 0; }

	FURTHER_SKILLS	m_skillType;
	sint32			m_val;
};
//////////////////////////////////////////////////////////////////////////
class iRewDlg_Artifact : public CDialogImpl<iRewDlg_Artifact>
{
public:
	enum { IDD = IDD_REW_ARTIFACT };

	BEGIN_MSG_MAP(iRewDlg_Artifact)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		for (iObjGroupNode::Iterator ait = gItemMgr.m_pArtifactNode->First(); ait != gItemMgr.m_pArtifactNode->End(); ++ait) {
			iArtifactNode* pArt = DynamicCast<iArtifactNode*>((iBaseNode*)*ait);
			check(pArt);
			m_arts.Add(iPair<iStringT,iStringT>(pArt->Id(), gItemMgr.m_TextMgr[pArt->NameKey()]));
		}
		CComboBox cb(GetDlgItem(IDC_ARTIFACT_LIST));
		for (uint32 xx=0; xx<m_arts.GetSize(); ++xx) {
			cb.AddString(m_arts[xx].second.CStr());
		}
		cb.SetCurSel(0);
		return 0;
	}

	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_artIdx = gItemMgr.m_pArtifactNode->GetArtifactNode(m_arts[CComboBox(GetDlgItem(IDC_ARTIFACT_LIST)).GetCurSel()].first)->Idx();
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{ EndDialog(wID); return 0; }

	iSimpleArray< iPair<iStringT,iStringT> >	m_arts;
	sint32										m_artIdx;
};
//////////////////////////////////////////////////////////////////////////
class iRewDlg_Spell : public CDialogImpl<iRewDlg_Spell>
{
public:
	enum { IDD = IDD_REW_SPELL };

	BEGIN_MSG_MAP(iRewDlg_Spell)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CComboBox cb(GetDlgItem(IDC_SPELL_LIST));
		for (uint32 xx=0; xx<MSP_COUNT; ++xx) {
			cb.AddString(gItemMgr.m_TextMgr[MAGIC_SPELL_NAMEKEY[xx]].CStr());
		}
		cb.SetCurSel(0);
		return 0;
	}

	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_spellIdx = CComboBox(GetDlgItem(IDC_SPELL_LIST)).GetCurSel();
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{ EndDialog(wID); return 0; }

	sint32										m_spellIdx;
};
//////////////////////////////////////////////////////////////////////////
class iRewDlg_CreatGroup : public CDialogImpl<iRewDlg_CreatGroup>
{
public:
	enum { IDD = IDD_REW_CREATGROUP };

	BEGIN_MSG_MAP(iRewDlg_CreatGroup)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CComboBox cb(GetDlgItem(IDC_CREAT_TYPE));
		for (uint32 xx=0; xx<CREAT_COUNT; ++xx) {
			cb.AddString(gItemMgr.m_TextMgr[CREAT_NAMEKEY[xx]].CStr());
		}
		cb.SetCurSel(CREAT_PEASANT);
		CUpDownCtrl span(GetDlgItem(IDC_CREAT_QUANTSP));
		span.SetRange32(-65535,65535);
		LI::SetDlgItemInt(m_hWnd,IDC_CREAT_QUANTED, 30);
		return 0;
	}

	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_creatType = (CREATURE_TYPE)CComboBox(GetDlgItem(IDC_CREAT_TYPE)).GetCurSel();
		m_quant = LI::GetDlgItemInt(m_hWnd,IDC_CREAT_QUANTED);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{ EndDialog(wID); return 0; }

	CREATURE_TYPE	m_creatType;
	sint32			m_quant;
};
//////////////////////////////////////////////////////////////////////////
iPropsPage_Rewards::iPropsPage_Rewards(iRewardsCtr& rewards)
: CPropertyPageImpl<iPropsPage_Rewards>(_T("Rewards")), m_rewards(rewards) {}

void iPropsPage_Rewards::RefreshList()
{
	CListBox lb(GetDlgItem(IDC_REWARDS_LIST));
	lb.ResetContent();
	for (uint32 xx=0; xx<m_rewards.GetSize(); ++xx) {
		switch(m_rewards[xx].m_type) {
		case RIT_MINERAL:
			lb.AddString(iFormat(_T("Mineral: %d unit(s) of %s"),m_rewards[xx].m_sParam, gItemMgr.m_TextMgr[MINERAL_NAMEKEY[m_rewards[xx].m_fParam]].CStr()).CStr());
			break;
		case RIT_EXPERIENCE:
			lb.AddString(iFormat(_T("Experience: %d points"),m_rewards[xx].m_sParam).CStr());
			break;
		case RIT_MANAPTS:
			lb.AddString(iFormat(_T("Mana points: %d points"),m_rewards[xx].m_sParam).CStr());
			break;
		case RIT_TRAVELPTS:
			lb.AddString(iFormat(_T("Travel points: %d points"),m_rewards[xx].m_sParam).CStr());
			break;
		case RIT_MORALE:
			lb.AddString(iFormat(_T("Morale: %d"),m_rewards[xx].m_sParam).CStr());
			break;
		case RIT_LUCK:
			lb.AddString(iFormat(_T("Luck: %d"),m_rewards[xx].m_sParam).CStr());
			break;
		case RIT_FURTSKILL:
			lb.AddString(iFormat(_T("Primary skill: %s +%d"), gItemMgr.m_TextMgr[PRSKILL_NAMEKEY[m_rewards[xx].m_fParam-FSK_ATTACK]].CStr(),m_rewards[xx].m_sParam).CStr());
			break;
		case RIT_ARTIFACT:
			lb.AddString(iFormat(_T("Artifact: %s"), gItemMgr.m_TextMgr[gItemMgr.m_pArtifactNode->GetArtifactNode(m_rewards[xx].m_fParam)->NameKey()].CStr()).CStr());
			break;
		case RIT_MAGICSPELL:
			lb.AddString(iFormat(_T("Spell: %s"), gItemMgr.m_TextMgr[MAGIC_SPELL_NAMEKEY[m_rewards[xx].m_fParam]].CStr()).CStr());
			break;
		case RIT_CREATGROUP:
			lb.AddString(iFormat(_T("Creatures: %d %s"), m_rewards[xx].m_sParam, gItemMgr.m_TextMgr[CREAT_NAMEKEY[m_rewards[xx].m_fParam]].CStr()).CStr());
		default:
			check("Invalid reward type == 0");
		}
	}
	UpdatePage();
}

LRESULT iPropsPage_Rewards::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RefreshList();
	return TRUE;
}


LRESULT iPropsPage_Rewards::OnAddReward(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	iRewardTypeDlg rtdlg;
	if (rtdlg.DoModal(m_hWnd) == IDOK) {
		switch(rtdlg.m_rewardType) {
		case RIT_MINERAL:
			{
				iRewDlg_Mineral dlg;
				if (dlg.DoModal() == IDOK) {
					m_rewards.Add(iRewardItem(RIT_MINERAL, dlg.m_mineralType, dlg.m_quant));
					RefreshList();
				}
			}
			break;
		case RIT_EXPERIENCE:
			{
				iRewDlg_Experience dlg;
				if (dlg.DoModal() == IDOK) {
					m_rewards.Add(iRewardItem(RIT_EXPERIENCE, 0, dlg.m_quant));
					RefreshList();
				}
			}
			break;
		case RIT_MANAPTS:
			{
				iRewDlg_ManaPoints dlg;
				if (dlg.DoModal() == IDOK) {
					m_rewards.Add(iRewardItem(RIT_MANAPTS, 0, dlg.m_quant));
					RefreshList();
				}
			}
			break;		
		case RIT_TRAVELPTS:
			{
				iRewDlg_TravPoints dlg;
				if (dlg.DoModal() == IDOK) {
					m_rewards.Add(iRewardItem(RIT_TRAVELPTS, 0, dlg.m_quant));
					RefreshList();
				}
			}
			break;		
		case RIT_MORALE:
			{
				iRewDlg_Morale dlg;
				if (dlg.DoModal() == IDOK) {
					m_rewards.Add(iRewardItem(RIT_MORALE, 0, dlg.m_quant));
					RefreshList();
				}
			}
			break;		
		case RIT_LUCK:
			{
				iRewDlg_Luck dlg;
				if (dlg.DoModal() == IDOK) {
					m_rewards.Add(iRewardItem(RIT_LUCK, 0, dlg.m_quant));
					RefreshList();
				}
			}
			break;		
		case RIT_FURTSKILL:
			{
				iRewDlg_FurtSkill dlg;
				if (dlg.DoModal() == IDOK) {
					m_rewards.Add(iRewardItem(RIT_FURTSKILL, dlg.m_skillType, dlg.m_val));
					RefreshList();
				}
			}
			break;
		case RIT_ARTIFACT:
			{
				iRewDlg_Artifact dlg;
				if (dlg.DoModal() == IDOK) {
					m_rewards.Add(iRewardItem(RIT_ARTIFACT, dlg.m_artIdx, 0));
					RefreshList();
				}
			}
			break;
		case RIT_MAGICSPELL:
			{
				iRewDlg_Spell dlg;
				if (dlg.DoModal() == IDOK) {
					m_rewards.Add(iRewardItem(RIT_MAGICSPELL, dlg.m_spellIdx, 0));
					RefreshList();
				}
			}
			break;
		case RIT_CREATGROUP:
			{
				iRewDlg_CreatGroup dlg;
				if (dlg.DoModal() == IDOK) {
					m_rewards.Add(iRewardItem(RIT_CREATGROUP, dlg.m_creatType, dlg.m_quant));
					RefreshList();
				}
			}
			break;
		default:
			check("Invalid reward type" == 0);
		}
	}

	return TRUE;
}

LRESULT iPropsPage_Rewards::OnRemoveReward(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CListBox lb(GetDlgItem(IDC_REWARDS_LIST));
	sint32 csel = lb.GetCurSel();
	check(csel != -1);
	lb.DeleteString(csel);
	m_rewards.RemoveAt(csel);
	UpdatePage();

	return TRUE;
}


LRESULT iPropsPage_Rewards::OnRewardsList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (wNotifyCode == LBN_SELCHANGE) {
		UpdatePage();
	} 
	return TRUE;
}

void iPropsPage_Rewards::UpdatePage()
{
	sint32 csel = CListBox(GetDlgItem(IDC_REWARDS_LIST)).GetCurSel();
	::EnableWindow(GetDlgItem(IDC_REMOVE), csel != -1);
}

int iPropsPage_Rewards::OnApply()
{
	/*
	m_hero.SetCustomSpells(m_bCustom);
	if (!m_bCustom) ResetSpells();
	m_hero.Spells() = m_spellList;
	*/
	return PSNRET_NOERROR;
}


/*
 *	Spell book
 */
class iSpellListDlg : public CDialogImpl<iSpellListDlg>
{
public:
	enum { IDD = IDD_SPELL_LIST };

	BEGIN_MSG_MAP(iSpellListDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDC_SPELL_LIST, OnSpellList)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	iSpellListDlg() : m_selSpell(0xFFFF) {}

	int DoModal(iSpellList& spellList)
	{
		for (uint16 xx=0; xx<MSP_COUNT; ++xx) {
			if (spellList.Find(xx) == -1) m_spellList.Add(xx);
		}
		return CDialogImpl<iSpellListDlg>::DoModal();
	}

	void UpdatePage()
	{
		sint32 csel = CListBox(GetDlgItem(IDC_SPELL_LIST)).GetCurSel();
		::EnableWindow(GetDlgItem(IDOK), csel != -1);
	}

	
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CListBox lb(GetDlgItem(IDC_SPELL_LIST));
		for (uint32 xx=0; xx<m_spellList.GetSize(); ++xx) {
			lb.AddString(gItemMgr.m_TextMgr[SPELL_NAMEKEY[m_spellList[xx]]].CStr());
		}
		UpdatePage();
		return 0;
	}

	LRESULT OnSpellList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if (wNotifyCode == LBN_SELCHANGE) {
			UpdatePage();
		} else if (wNotifyCode == LBN_DBLCLK) {
			CListBox lb(GetDlgItem(IDC_SPELL_LIST));
			if (lb.GetCurSel() != -1) {
				m_selSpell =  m_spellList[lb.GetCurSel()];
				EndDialog(IDOK);
			}
		}
		return TRUE;
	}

	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		CListBox lb(GetDlgItem(IDC_SPELL_LIST));
		if (lb.GetCurSel() != -1) {
			m_selSpell =  m_spellList[lb.GetCurSel()];
			EndDialog(wID);
		}
		return 0;
	}

	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_selSpell = 0xFFFF;
		EndDialog(wID);
		return 0;
	}

	uint16		m_selSpell;
	iSpellList	m_spellList;
};

//////////////////////////////////////////////////////////////////////////
iPropsPage_Spellbook::iPropsPage_Spellbook(iHero& hero)
: CPropertyPageImpl<iPropsPage_Spellbook>(_T("Spell book")), m_hero(hero), m_bCustom(hero.IsCustomSpells()), m_spellList(hero.Spells()) {}

void iPropsPage_Spellbook::RefreshList()
{
	CListBox lb(GetDlgItem(IDC_SPELL_LIST));
	lb.ResetContent();
	for (uint32 xx=0; xx<m_spellList.GetSize(); ++xx) {
		lb.AddString(gItemMgr.m_TextMgr[SPELL_NAMEKEY[m_spellList[xx]]].CStr());
	}
	UpdatePage();
}

void iPropsPage_Spellbook::ResetSpells()
{
	m_spellList = m_hero.Proto()->Spells();
}

LRESULT iPropsPage_Spellbook::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CheckDlgButton(IDC_CUSTOMIZE_CHECK, m_bCustom);
	RefreshList();
	return TRUE;
}

LRESULT iPropsPage_Spellbook::OnCustomizeCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_bCustom = IsDlgButtonChecked(IDC_CUSTOMIZE_CHECK)>0;
	//m_castle.CtlCnsts().SetCustomized(IsDlgButtonChecked(IDC_CUSTOMIZE_CHECK)>0);
	UpdatePage();
	return TRUE;
}

LRESULT iPropsPage_Spellbook::OnAddSpell(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	iSpellListDlg slDlg;
	if (slDlg.DoModal(m_spellList) == IDOK) {
		check(slDlg.m_selSpell != 0xFFFF);
		m_spellList.Add(slDlg.m_selSpell);
		RefreshList();
	}
	return TRUE;
}

LRESULT iPropsPage_Spellbook::OnAddAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_spellList.RemoveAll();
	for (uint16 xx=0; xx<MSP_COUNT; ++xx) m_spellList.Add(xx);
	RefreshList();
	return TRUE;
}

LRESULT iPropsPage_Spellbook::OnRemoveSpell(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CListBox lb(GetDlgItem(IDC_SPELL_LIST));
	sint32 csel = lb.GetCurSel();
	check(csel != -1);
	lb.DeleteString(csel);
	m_spellList.RemoveAt(csel);
	UpdatePage();

	return TRUE;
}

LRESULT iPropsPage_Spellbook::OnResetSpellBook(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ResetSpells();
	RefreshList();
	return TRUE;
}

LRESULT iPropsPage_Spellbook::OnSpellList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (wNotifyCode == LBN_SELCHANGE) {
		UpdatePage();
	} 
	return TRUE;
}

void iPropsPage_Spellbook::UpdatePage()
{
	sint32 csel = CListBox(GetDlgItem(IDC_SPELL_LIST)).GetCurSel();
	::EnableWindow(GetDlgItem(IDC_SPELL_LIST), IsDlgButtonChecked(IDC_CUSTOMIZE_CHECK));
	::EnableWindow(GetDlgItem(IDC_ADD), IsDlgButtonChecked(IDC_CUSTOMIZE_CHECK));
	::EnableWindow(GetDlgItem(IDC_ADDALL), IsDlgButtonChecked(IDC_CUSTOMIZE_CHECK));
	::EnableWindow(GetDlgItem(IDC_REMOVE), IsDlgButtonChecked(IDC_CUSTOMIZE_CHECK) && csel != -1);
	::EnableWindow(GetDlgItem(IDC_RESET), IsDlgButtonChecked(IDC_CUSTOMIZE_CHECK));
	//CheckDlgButton(IDC_BUILT, csel != -1 && m_castle.CtlCnsts().Entry(csel).IsBuilt());
}

int iPropsPage_Spellbook::OnApply()
{
	m_hero.SetCustomSpells(m_bCustom);
	if (!m_bCustom) ResetSpells();
	m_hero.Spells() = m_spellList;

	return PSNRET_NOERROR;
}

/*
 *	
 */
iMSPropsPage_General::iMSPropsPage_General(iMapHandler& map)
: CPropertyPageImpl<iMSPropsPage_General>(_T("General")), m_map(map) {}

LRESULT iMSPropsPage_General::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CComboBox cb(GetDlgItem(IDC_DIFFICULTY));
	for (uint32 xx=0; xx<DFC_COUNT; ++xx) cb.AddString(gItemMgr.m_TextMgr[DIFF_NAMEKEY[xx]].CStr());
	cb.SetCurSel(DFC_NORMAL);

	LI::SetDlgItemText(m_hWnd, IDC_MAP_NAME, m_map.m_ctMgr[MakeTextEntryName(TET_MAP_NAME, NULL)]);
	LI::SetDlgItemText(m_hWnd, IDC_DESCRIPTION, m_map.m_ctMgr[MakeTextEntryName(TET_MAP_DESC, NULL)]);
	LI::SetDlgItemText(m_hWnd, IDC_MAP_VERSION, m_map.m_MapVersion);
	LI::SetDlgItemText(m_hWnd, IDC_MAP_AUTHOR, m_map.m_MapAuthor);
	
	return TRUE;
}

int iMSPropsPage_General::OnApply()
{
	LI::GetDlgItemText(m_hWnd,IDC_MAP_NAME, m_map.m_ctMgr.GetText(MakeTextEntryName(TET_MAP_NAME, NULL), GLNG_ENGLISH));
	LI::GetDlgItemText(m_hWnd,IDC_DESCRIPTION, m_map.m_ctMgr.GetText(MakeTextEntryName(TET_MAP_DESC, NULL), GLNG_ENGLISH));
	LI::GetDlgItemText(m_hWnd,IDC_MAP_VERSION, m_map.m_MapVersion);
	LI::GetDlgItemText(m_hWnd,IDC_MAP_AUTHOR, m_map.m_MapAuthor);
	return PSNRET_NOERROR;
}

/*
 *
 */
class iPlayerSettingDlg : public CDialogImpl<iPlayerSettingDlg>
{
public:
	enum { IDD = IDD_PLAYER_INFO };

	BEGIN_MSG_MAP(iPlayerSettingDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	iMapHandler&			m_map;
	iPlayer*				m_pPlayer;
	iSimpleArray<iCastle*>	m_Castles;

	iPlayerSettingDlg(iPlayer* pPlayer, iMapHandler& map)
	: m_pPlayer(pPlayer), m_map(map)
	{
		check(pPlayer);

		// Prepare castles list
		for (uint32 ctlId = 0; ctlId < m_map.m_CastleList.GetSize(); ++ctlId) {
			if (m_map.m_CastleList[ctlId]->Owner() == m_pPlayer->m_PlayerId) m_Castles.Add(m_map.m_CastleList[ctlId]);
		}

		// Find main castle
		check(m_pPlayer->m_pMainCastle || !m_Castles.GetSize());
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// Title
		SetWindowText((gItemMgr.m_TextMgr[PLAYER_NAMEKEY[m_pPlayer->m_PlayerId]] + _T(" Player")).CStr());

		// Castle list
		if (m_Castles.GetSize()) {
			CComboBox ctlcb(GetDlgItem(IDC_CTL_LIST));
			for (uint32 xx=0; xx<m_Castles.GetSize(); ++xx) {
				ctlcb.AddString(iFormat(_T("%s at (%d, %d)"),m_Castles[xx]->Name().CStr(), m_Castles[xx]->Pos().x, m_Castles[xx]->Pos().y).CStr());
			}
			sint32 ctlIdx = m_Castles.Find(m_pPlayer->m_pMainCastle);
			check(ctlIdx != -1);
			ctlcb.SetCurSel(ctlIdx);
			CheckDlgButton(IDC_CREATE_HERO, m_pPlayer->m_bCreateHero == true);
		} else {
			::EnableWindow(GetDlgItem(IDC_CTL_LIST), FALSE);
			::EnableWindow(GetDlgItem(IDC_CREATE_HERO), FALSE);
		}
		return 0;
	}


	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if (::IsWindowEnabled(GetDlgItem(IDC_CTL_LIST))) {
			m_pPlayer->m_pMainCastle = m_Castles[CComboBox(GetDlgItem(IDC_CTL_LIST)).GetCurSel()];
			m_pPlayer->m_bCreateHero = IsDlgButtonChecked(IDC_CREATE_HERO) == TRUE;
		}

		EndDialog(wID);
		return 0;
	}

	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

};

/*
 *	
 */
iMSPropsPage_Players::iMSPropsPage_Players(iMapHandler& map)
: CPropertyPageImpl<iMSPropsPage_Players>(_T("Players")), m_map(map) {}

LRESULT iMSPropsPage_Players::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	for (uint32 xx=0; xx<PID_COUNT; ++xx){
		CComboBox cb(GetDlgItem(IDC_TYPE_PLAYER1+xx));
		for (uint32 yy=0; yy<PTM_COUNT; ++yy) cb.AddString(gItemMgr.m_TextMgr[PLAYER_TYPE_MASK_NAMEKEY[yy]].CStr());
		if (m_map.m_pPlayers[xx]) {
			CheckDlgButton(IDC_PLAYER1+xx, TRUE);
			cb.SetCurSel(m_map.m_pPlayers[xx]->m_PlayerTypeMask);
		} else {
			cb.SetCurSel(PTM_HUMAN_OR_COMPUTER);
		}
	}

	UpdatePage();
	return TRUE;
}

LRESULT iMSPropsPage_Players::OnPlayerCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	sint32 pid = wID - IDC_PLAYER1;
	check(pid >= PID_RED && pid <= PID_YELLOW);
	if (IsDlgButtonChecked(wID) && !m_map.m_pPlayers[pid]) {
		m_map.AddPlayer((PLAYER_ID)pid,(PLAYER_TYPE_MASK)CComboBox(GetDlgItem(IDC_TYPE_PLAYER1+pid)).GetCurSel());
	}

	UpdatePage();
	return TRUE;
}

LRESULT iMSPropsPage_Players::OnPlayerSettings(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	sint32 pid = wID - IDC_SETTINGS_PLAYER1;
	check(pid >= PID_RED && pid <= PID_YELLOW);
	iPlayerSettingDlg psdlg(m_map.m_pPlayers[pid], m_map);
	psdlg.DoModal();

	return TRUE;
}

void iMSPropsPage_Players::UpdatePage()
{
	for (uint32 pid=PID_RED; pid<PID_COUNT; ++pid ){
		::EnableWindow(GetDlgItem(IDC_TYPE_PLAYER1+pid-PID_RED), IsDlgButtonChecked(IDC_PLAYER1+pid-PID_RED));
		::EnableWindow(GetDlgItem(IDC_SETTINGS_PLAYER1+pid), IsDlgButtonChecked(IDC_PLAYER1+pid-PID_RED));
	}
}

int iMSPropsPage_Players::OnApply()
{
	BOOL bOk = PSNRET_NOERROR;
	for (uint32 xx=0; xx<PID_COUNT; ++xx) {
		if (!m_map.m_pPlayers[xx] && IsDlgButtonChecked(IDC_PLAYER1+xx)) {
			check(0);
			//m_map.AddPlayer((PLAYER_ID)xx,(PLAYER_TYPE_MASK)CComboBox(GetDlgItem(IDC_TYPE_PLAYER1+xx)).GetCurSel());
		} else if (m_map.m_pPlayers[xx] && IsDlgButtonChecked(IDC_PLAYER1+xx)){
			m_map.m_pPlayers[xx]->m_PlayerTypeMask = (PLAYER_TYPE_MASK)CComboBox(GetDlgItem(IDC_TYPE_PLAYER1+xx)).GetCurSel();
		} else if (m_map.m_pPlayers[xx] && !IsDlgButtonChecked(IDC_PLAYER1+xx)) {
			// remove player ?
			if (!m_map.CanDeletePlayer((PLAYER_ID)xx)) {
				MessageBox(iFormat(_T("Unable to delete %s player!"),gItemMgr.m_TextMgr[PLAYER_NAMEKEY[xx]].CStr()).CStr());
				CheckDlgButton(IDC_PLAYER1+xx,TRUE);
				UpdatePage();
				bOk = PSNRET_INVALID;
			} else {
				m_map.DeletePlayer((PLAYER_ID)xx);
			}
		}
	}

	return bOk;
}

/*
 *	
 */
iMSPropsPage_Languages::iMSPropsPage_Languages(iMapHandler& map)
: CPropertyPageImpl<iMSPropsPage_Languages>(_T("Languages")), m_map(map) {}

LRESULT iMSPropsPage_Languages::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	for (uint32 xx=0; xx<GLNG_COUNT; ++xx){
		if (m_map.m_lngMask & (1<<xx)) {
			CheckDlgButton(IDC_LNG_ENG_01+xx, TRUE);
		}
	}

	return TRUE;
}

int iMSPropsPage_Languages::OnApply()
{
	uint32 nlngMask = 0;
	for (uint32 xx=0; xx<GLNG_COUNT; ++xx){
		if (IsDlgButtonChecked(IDC_LNG_ENG_01+xx)) {
			nlngMask |= (1<<xx);
		}
	}

	if ( !(nlngMask & 1)) {
		MessageBox(L"English language is default in Pocket Heroes game, so your map must support it.");
		return PSNRET_INVALID;
	}
	
	m_map.m_lngMask = nlngMask;

	return PSNRET_NOERROR;
}

/*
 *
 */
LPCTSTR REPEAT_FREQ_TEXT[iTimeEvent::EFREQ_COUNT] = {
	_T("Never"),
	_T("Every day"),
	_T("Every 2 days"),
	_T("Every 3 days"),
	_T("Every 4 days"),
	_T("Every 5 days"),
	_T("Every 6 days"),
	_T("Every week"),
	_T("Every 2 weeks"),
	_T("Every 3 weeks"),
	_T("Every month")
};

class iTimeEventDlg : public CDialogImpl<iTimeEventDlg>
{
public:
	enum { IDD = IDD_TIME_EVENT };

	BEGIN_MSG_MAP(iSpellListDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	iTimeEventDlg(iTimeEvent& event, iMapHandler& map) : m_event(event), m_map(map) {}

	
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// Name 
		LI::SetDlgItemText(m_hWnd, IDC_EVENT_NAME, m_event.m_name);
		// Text
		if (!m_event.m_text.Empty()) {
			check(m_map.m_ctMgr.HasEntry(m_event.m_text));
			LI::SetDlgItemText(m_hWnd, IDC_EVENT_TEXT, m_map.m_ctMgr[m_event.m_text]);
		}

		// Affects to players
		for (uint32 pp=0; pp<PID_COUNT; ++pp) {
			::EnableWindow(GetDlgItem(IDC_PLAYER1+pp), m_map.m_pPlayers[pp] != NULL);
			CheckDlgButton(IDC_PLAYER1+pp, m_map.m_pPlayers[pp] != NULL && ((m_event.m_playerMask & (1<<pp)) != 0) );
		}

		// First time occurance
		CUpDownCtrl(GetDlgItem(IDC_FTIME_SPAN)).SetRange32(1,65535);
		LI::SetDlgItemInt(m_hWnd,IDC_FTIME_EDIT, m_event.m_fTime);
		// Repeat every
		CComboBox rcmb(GetDlgItem(IDC_REPEAT_COMBO));
		for (uint32 zz=0; zz<iTimeEvent::EFREQ_COUNT; ++zz) rcmb.AddString(REPEAT_FREQ_TEXT[zz]);
		rcmb.SetCurSel(m_event.m_repTime);
		// Minerals
		for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) {
			CUpDownCtrl sp(GetDlgItem(IDC_MINERAL1_QUANTSP+xx));
			sp.SetRange32(-65535,65535);			
			LI::SetDlgItemInt(m_hWnd,IDC_MINERAL1_QUANTED+xx, m_event.m_minerals.quant[xx]);
		}		
		return 0;
	}

	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// name
		LI::GetDlgItemText(m_hWnd,IDC_EVENT_NAME, m_event.m_name);
		if (m_event.m_name.Empty()) {
			MessageBox(_T("Please specify the name for event."));
			return 0;
		}

		// text
		iStringT ntext;
		LI::GetDlgItemText(m_hWnd,IDC_EVENT_TEXT, ntext);
		if (!ntext.Empty()) {
			if (!m_event.m_text.Empty()) {
				m_map.m_ctMgr.GetText(m_event.m_text, GLNG_ENGLISH) = ntext;
			} else {
				m_event.m_text = MakeTextEntryName(TET_TEVENT_TEXT,&m_event);
				m_map.m_ctMgr.AddTextEntry(m_event.m_text, ntext, TET_TEVENT_TEXT);
			}
		} else {
			MessageBox(_T("Please specify the text message for event."));
			return 0;
		}


		// Affects to players
		m_event.m_playerMask = PIM_NONE;
		for (uint32 pp=0; pp<PID_COUNT; ++pp) {
			if (m_map.m_pPlayers[pp] == NULL || IsDlgButtonChecked(IDC_PLAYER1+pp)) m_event.m_playerMask |= (1<<pp);
		}

		// First time occurance
		m_event.m_fTime = LI::GetDlgItemInt(m_hWnd,IDC_FTIME_EDIT);

		// Repeat every
		m_event.m_repTime =  (iTimeEvent::EventFreq)CComboBox(GetDlgItem(IDC_REPEAT_COMBO)).GetCurSel();

		// minerals
		for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) {
			m_event.m_minerals.quant[xx] = LI::GetDlgItemInt(m_hWnd,IDC_MINERAL1_QUANTED+xx);
		}

		EndDialog(wID);
		return 0;
	}

	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

	iMapHandler&	m_map;
	iTimeEvent&		m_event;
};

//////////////////////////////////////////////////////////////////////////
iMSPropsPage_TimeEvents::iMSPropsPage_TimeEvents(iMapHandler& map)
: CPropertyPageImpl<iMSPropsPage_TimeEvents>(_T("Events")), m_map(map) {}

LRESULT iMSPropsPage_TimeEvents::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	/*
	for (uint32 xx=0; xx<PID_COUNT; ++xx){
		CComboBox cb(GetDlgItem(IDC_TYPE_PLAYER1+xx));
		for (uint32 yy=0; yy<PTM_COUNT; ++yy) cb.AddString(gItemMgr.m_TextMgr[PLAYER_TYPE_MASK_NAMEKEY[yy]].CStr());
		if (m_map.m_pPlayers[xx]) {
			CheckDlgButton(IDC_PLAYER1+xx, TRUE);
			cb.SetCurSel(m_map.m_pPlayers[xx]->m_PlayerTypeMask);
		} else {
			cb.SetCurSel(PTM_HUMAN_OR_COMPUTER);
		}
	}*/

	UpdateEventList();

	return TRUE;
}

void iMSPropsPage_TimeEvents::UpdateEventList()
{
	CListBox lb(GetDlgItem(IDC_EVENT_LIST));
	lb.ResetContent();
	for (uint32 xx=0; xx<m_map.m_timeEventMgr.EventsCount(); ++xx) {
		lb.AddString(m_map.m_timeEventMgr.Event(xx).m_name.CStr());
	}
	UpdateControls();
}

void iMSPropsPage_TimeEvents::UpdateControls()
{
	sint32 csel = CListBox(GetDlgItem(IDC_EVENT_LIST)).GetCurSel();
	::EnableWindow(GetDlgItem(IDC_EDIT_EVENT), csel != -1);
	::EnableWindow(GetDlgItem(IDC_DELETE_EVENT), csel != -1);
	::EnableWindow(GetDlgItem(IDC_DELETE_ALL_EVENTS), m_map.m_timeEventMgr.EventsCount()>0);
}

LRESULT iMSPropsPage_TimeEvents::OnAddEvent(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	iTimeEvent te;
	iTimeEventDlg tedlg(te, m_map);
	if (tedlg.DoModal() == IDOK) {
		m_map.m_timeEventMgr.AddEvent(te);
		UpdateEventList();
	}
	return TRUE;
}

LRESULT iMSPropsPage_TimeEvents::OnEditEvent(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	sint32 csel = CListBox(GetDlgItem(IDC_EVENT_LIST)).GetCurSel();
	check(csel != -1 && csel < (sint32)m_map.m_timeEventMgr.EventsCount());
	iTimeEvent te = m_map.m_timeEventMgr.Event(csel);
	iTimeEventDlg tedlg(te, m_map);
	if (tedlg.DoModal() == IDOK) {
		m_map.m_timeEventMgr.Event(csel) = te;
		UpdateEventList();
	}
	return TRUE;
}

LRESULT iMSPropsPage_TimeEvents::OnDeleteEvent(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	sint32 csel = CListBox(GetDlgItem(IDC_EVENT_LIST)).GetCurSel();
	check(csel != -1 && csel < (sint32)m_map.m_timeEventMgr.EventsCount());
	if (!m_map.m_timeEventMgr.Event(csel).m_text.Empty()) {
		check(m_map.m_ctMgr.HasEntry(m_map.m_timeEventMgr.Event(csel).m_text));
		m_map.m_ctMgr.RemoveEntry(m_map.m_timeEventMgr.Event(csel).m_text);
	}
	m_map.m_timeEventMgr.DeleteEvent(csel);

	UpdateEventList();
	return TRUE;
}

LRESULT iMSPropsPage_TimeEvents::OnDeleteAllEvents(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	check(m_map.m_timeEventMgr.EventsCount() > 0);
	for (uint32 nn=0; nn<m_map.m_timeEventMgr.EventsCount(); ++nn) {
		if (!m_map.m_timeEventMgr.Event(nn).m_text.Empty()) {
			check(m_map.m_ctMgr.HasEntry(m_map.m_timeEventMgr.Event(nn).m_text));
			m_map.m_ctMgr.RemoveEntry(m_map.m_timeEventMgr.Event(nn).m_text);
		}
	}
	m_map.m_timeEventMgr.DeleteAll();
	UpdateEventList();
	return TRUE;
}

LRESULT iMSPropsPage_TimeEvents::OnEventList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (wNotifyCode == LBN_SELCHANGE) {
		UpdateControls();
	}
	return TRUE;
}

int iMSPropsPage_TimeEvents::OnApply()
{
	return PSNRET_NOERROR;
}