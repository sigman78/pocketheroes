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

#ifndef _HMM_EDITOR_PROPERTY_PAGES_H_
#define _HMM_EDITOR_PROPERTY_PAGES_H_

#include "resource.h"

/*
 *	Extended class used to automatically centre a property sheet
 */

class CPropertySheetEx : public CPropertySheet
{	
private:
	bool m_bCentered;
public:	
	CPropertySheetEx(_U_STRINGorID title = (LPCTSTR)NULL, UINT uStartPage = 0, HWND hWndParent = NULL)
	: CPropertySheet(title, uStartPage, hWndParent), m_bCentered(false)	{ }

	BEGIN_MSG_MAP(CPropertySheetEx)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		CHAIN_MSG_MAP(CPropertySheet)
	END_MSG_MAP()

	LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if (wParam == TRUE)	Center();
		bHandled = FALSE;
		return 0;
	}

	void Center(void)
	{
		if (!m_bCentered) {
			CenterWindow();
			m_bCentered = true;
		}
	}
};

/*
 *	Pages
 */
class iPropsPage_Army : public CPropertyPageImpl<iPropsPage_Army>
{
public:
	enum { IDD = IDD_PAGE_ARMY };

	iPropsPage_Army(iArmy& _army, LPCTSTR caption);

    BEGIN_MSG_MAP(iPropsPage_Army)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_RANGE_HANDLER(IDC_SLOT1_CRTYPE,IDC_SLOT7_CRTYPE, OnSlotTypeChanged)
		COMMAND_ID_HANDLER(IDC_CUSTOMIZE_CHECK, OnCustomizeCheck)
        CHAIN_MSG_MAP(CPropertyPageImpl<iPropsPage_Army>)
    END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSlotTypeChanged(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCustomizeCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	void UpdateSlot(uint32 idx);
	void UpdatePage();
    int OnApply();

private:
	iArmy&	army;
};

//////////////////////////////////////////////////////////////////////////
class iPropsPage_Message : public CPropertyPageImpl<iPropsPage_Message>
{
public:
	enum { IDD = IDD_PAGE_MESSAGE };

	iPropsPage_Message(iTextMgr& _tmgr, iStringT& _key, void* _pItem, TextEntryType _tet, LPCTSTR caption);

    BEGIN_MSG_MAP(iPropsPage_Message)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDC_CUSTOMIZE_CHECK, OnCustomizeCheck)
        CHAIN_MSG_MAP(CPropertyPageImpl<iPropsPage_Message>)
    END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCustomizeCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	void UpdatePage();
    int OnApply();

private:
	iTextMgr&		tmgr;
	iStringT&		key;
	void*			pItem;
	TextEntryType	tet;
};
//////////////////////////////////////////////////////////////////////////
class iPropsPage_GenMapEvent : public CPropertyPageImpl<iPropsPage_GenMapEvent>
{
public:
	enum { IDD = IDD_PAGE_GEN_MAPEVENT };

	iPropsPage_GenMapEvent(iMapHandler& _map, iMapEvent& _mapEvent);

    BEGIN_MSG_MAP(iPropsPage_GenMapEvent)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        CHAIN_MSG_MAP(CPropertyPageImpl<iPropsPage_GenMapEvent>)
    END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    int OnApply();

private:
	iMapHandler&	map;
	iMapEvent&		mapEvent;
};
//////////////////////////////////////////////////////////////////////////
class iPropsPage_GenMineral : public CPropertyPageImpl<iPropsPage_GenMineral>
{
public:
	enum { IDD = IDD_PAGE_GEN_MINERAL };

	iPropsPage_GenMineral(MINERAL_TYPE _mt, sint32& _cnt);

    BEGIN_MSG_MAP(iPropsPage_GenMineral)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_RANGE_HANDLER(IDC_RADIO_RANDOM, IDC_RADIO_CUSTOM, OnCustomRadio)
        CHAIN_MSG_MAP(CPropertyPageImpl<iPropsPage_GenMineral>)
    END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCustomRadio(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	void UpdatePage();
    int OnApply();

private:
	MINERAL_TYPE	mt;
	sint32&			cnt;
};

//////////////////////////////////////////////////////////////////////////
class iPropsPage_GenSpellScroll : public CPropertyPageImpl<iPropsPage_GenSpellScroll>
{
public:
	enum { IDD = IDD_PAGE_GEN_SPELLSCROLL };

	iPropsPage_GenSpellScroll(iMapItem_SpellScroll& spellScroll);

    BEGIN_MSG_MAP(iPropsPage_GenSpellScroll)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        CHAIN_MSG_MAP(CPropertyPageImpl<iPropsPage_GenSpellScroll>)
    END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    int OnApply();

private:
	iMapItem_SpellScroll&	m_spellScroll;
};
//////////////////////////////////////////////////////////////////////////
class iPropsPage_GenGuard : public CPropertyPageImpl<iPropsPage_GenGuard>
{
public:
	enum { IDD = IDD_PAGE_GEN_GUARD };

	iPropsPage_GenGuard(iMapGuard& guard);

    BEGIN_MSG_MAP(iPropsPage_GenGuard)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_RANGE_HANDLER(IDC_RADIO_RANDOM, IDC_RADIO_CUSTOM, OnCustomRadio)
        CHAIN_MSG_MAP(CPropertyPageImpl<iPropsPage_GenGuard>)
    END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCustomRadio(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	void UpdatePage();
    int OnApply();

private:
	iMapGuard& m_guard;
};
//////////////////////////////////////////////////////////////////////////
class iPropsPage_GenOwnCnst : public CPropertyPageImpl<iPropsPage_GenOwnCnst>
{
public:
	enum { IDD = IDD_PAGE_GEN_OWNCNST };

	iPropsPage_GenOwnCnst(iOwnCnst& ownCnst, const iMapHandler& map);

    BEGIN_MSG_MAP(iPropsPage_GenOwnCnst)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        CHAIN_MSG_MAP(CPropertyPageImpl<iPropsPage_GenOwnCnst>)
    END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    int OnApply();

private:
	iOwnCnst& m_ownCnst;
	const iMapHandler& m_map;
};
//////////////////////////////////////////////////////////////////////////
class iPropsPage_GenCastle : public CPropertyPageImpl<iPropsPage_GenCastle>
{
public:
	enum { IDD = IDD_PAGE_GEN_CASTLE  };

	iPropsPage_GenCastle(iCastle& castle, iMapHandler& map);

    BEGIN_MSG_MAP(iPropsPage_GenCastle)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDC_CUSTOMIZE_CHECK, OnCustomizeCheck)
        CHAIN_MSG_MAP(CPropertyPageImpl<iPropsPage_GenCastle>)
    END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCustomizeCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    int OnApply();

private:
	iCastle&		m_castle;
	iMapHandler&	m_map;
};
//////////////////////////////////////////////////////////////////////////
class iPropsPage_CtlCnsts : public CPropertyPageImpl<iPropsPage_CtlCnsts>, public COwnerDraw<iPropsPage_CtlCnsts>
{
public:
	enum { IDD = IDD_PAGE_CTL_CNSTS };

	iPropsPage_CtlCnsts(iCastle& castle, const iMapHandler& map);
	~iPropsPage_CtlCnsts();

    BEGIN_MSG_MAP(iPropsPage_CtlCnsts)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDC_CUSTOMIZE_CHECK, OnCustomizeCheck)
		COMMAND_ID_HANDLER(IDC_BUILT, OnBuilt)
		COMMAND_ID_HANDLER(IDC_CNST_LIST, OnCnstList)
        CHAIN_MSG_MAP(CPropertyPageImpl<iPropsPage_CtlCnsts>)
		CHAIN_MSG_MAP(COwnerDraw<iPropsPage_CtlCnsts>)
    END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCustomizeCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBuilt(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCnstList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	void UpdatePage();
    int OnApply();
	void DrawItem(LPDRAWITEMSTRUCT lpdi);

private:
	HFONT				m_bldFont;
	iCastle&			m_castle;
	const iMapHandler&	m_map;
};
//////////////////////////////////////////////////////////////////////////
class iPropsPage_GenHero : public CPropertyPageImpl<iPropsPage_GenHero>
{
public:
	enum { IDD = IDD_PAGE_GEN_HERO };

	iPropsPage_GenHero(iHero& hero, iMapHandler& map);

    BEGIN_MSG_MAP(iPropsPage_GenHero)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDC_CUSTOMIZE_CHECK, OnCustomizeCheck)
        CHAIN_MSG_MAP(CPropertyPageImpl<iPropsPage_GenHero>)
    END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCustomizeCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    int OnApply();

private:
	iHero&			m_hero;
	iMapHandler&	m_map;
};
//////////////////////////////////////////////////////////////////////////
class iPropsPage_Rewards : public CPropertyPageImpl<iPropsPage_Rewards>
{
public:
	enum { IDD = IDD_PAGE_REWARDS };

	iPropsPage_Rewards(iRewardsCtr& rewards);

    BEGIN_MSG_MAP(iPropsPage_Rewards)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDC_ADD, OnAddReward)
		COMMAND_ID_HANDLER(IDC_REMOVE, OnRemoveReward)
		COMMAND_ID_HANDLER(IDC_REWARDS_LIST, OnRewardsList)
        CHAIN_MSG_MAP(CPropertyPageImpl<iPropsPage_Rewards>)
    END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnAddReward(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRemoveReward(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRewardsList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	void RefreshList();
	void UpdatePage();
    int OnApply();

private:
	iRewardsCtr&	m_rewards;
};
//////////////////////////////////////////////////////////////////////////
class iPropsPage_Spellbook : public CPropertyPageImpl<iPropsPage_Spellbook>
{
public:
	enum { IDD = IDD_PAGE_SPELLBOOK };

	iPropsPage_Spellbook(iHero& hero);

    BEGIN_MSG_MAP(iPropsPage_Spellbook)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDC_CUSTOMIZE_CHECK, OnCustomizeCheck)
		COMMAND_ID_HANDLER(IDC_ADD, OnAddSpell)
		COMMAND_ID_HANDLER(IDC_ADDALL, OnAddAll)
		COMMAND_ID_HANDLER(IDC_REMOVE, OnRemoveSpell)
		COMMAND_ID_HANDLER(IDC_RESET, OnResetSpellBook)
		COMMAND_ID_HANDLER(IDC_SPELL_LIST, OnSpellList)
        CHAIN_MSG_MAP(CPropertyPageImpl<iPropsPage_Spellbook>)
    END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCustomizeCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAddSpell(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAddAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRemoveSpell(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnResetSpellBook(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSpellList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	void ResetSpells();
	void RefreshList();
	void UpdatePage();
    int OnApply();

private:
	bool			m_bCustom;
	iSpellList		m_spellList;
	iHero&			m_hero;
};
//////////////////////////////////////////////////////////////////////////
class iPropsPage_SecSkills : public CPropertyPageImpl<iPropsPage_SecSkills>
{
public:
	enum { IDD = IDD_PAGE_HERO_SKILLS };

	iPropsPage_SecSkills(iHero& hero, const iMapHandler& map);

    BEGIN_MSG_MAP(iPropsPage_SecSkills)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_RANGE_HANDLER(IDC_SKILL_TYPE1,IDC_SKILL_TYPE8, OnSlotTypeChanged)
        CHAIN_MSG_MAP(CPropertyPageImpl<iPropsPage_SecSkills>)
    END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSlotTypeChanged(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    int OnApply();
	void UpdateSlot(sint32 idx);

private:
	iHero& m_hero;
	const iMapHandler& m_map;
};
//////////////////////////////////////////////////////////////////////////
class iMSPropsPage_General : public CPropertyPageImpl<iMSPropsPage_General>
{
public:
	enum { IDD = IDD_MSPAGE_GENERAL };

	iMSPropsPage_General(iMapHandler& map);

    BEGIN_MSG_MAP(iMSPropsPage_General)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        CHAIN_MSG_MAP(CPropertyPageImpl<iMSPropsPage_General>)
    END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    int OnApply();

private:
	iMapHandler& m_map;
};

//////////////////////////////////////////////////////////////////////////
class iMSPropsPage_Players : public CPropertyPageImpl<iMSPropsPage_Players>
{
public:
	enum { IDD = IDD_MSPAGE_PLAYERS };

	iMSPropsPage_Players(iMapHandler& map);

    BEGIN_MSG_MAP(iMSPropsPage_Players)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_RANGE_HANDLER(IDC_PLAYER1, IDC_PLAYER6, OnPlayerCheck)
		COMMAND_RANGE_HANDLER(IDC_SETTINGS_PLAYER1, IDC_SETTINGS_PLAYER6, OnPlayerSettings)
        CHAIN_MSG_MAP(CPropertyPageImpl<iMSPropsPage_Players>)
    END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPlayerCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPlayerSettings(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	void UpdatePage();
    int OnApply();

private:
	iMapHandler& m_map;
};

//////////////////////////////////////////////////////////////////////////
class iMSPropsPage_Languages : public CPropertyPageImpl<iMSPropsPage_Languages>
{
public:
	enum { IDD = IDD_MSPAGE_LANGS };
	
	iMSPropsPage_Languages(iMapHandler& map);
	
    BEGIN_MSG_MAP(iMSPropsPage_Languages)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        CHAIN_MSG_MAP(CPropertyPageImpl<iMSPropsPage_Languages>)
		END_MSG_MAP()
		
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    int OnApply();
	
private:
	iMapHandler& m_map;
};

//////////////////////////////////////////////////////////////////////////
class iMSPropsPage_TimeEvents : public CPropertyPageImpl<iMSPropsPage_TimeEvents >
{
public:
	enum { IDD = IDD_PAGE_TIME_EVENTS };

	iMSPropsPage_TimeEvents(iMapHandler& map);

    BEGIN_MSG_MAP(iMSPropsPage_TimeEvents)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDC_ADD_EVENT, OnAddEvent)
		COMMAND_ID_HANDLER(IDC_EDIT_EVENT, OnEditEvent)
		COMMAND_ID_HANDLER(IDC_DELETE_EVENT, OnDeleteEvent)
		COMMAND_ID_HANDLER(IDC_DELETE_ALL_EVENTS, OnDeleteAllEvents)
		COMMAND_ID_HANDLER(IDC_EVENT_LIST, OnEventList)
        CHAIN_MSG_MAP(CPropertyPageImpl<iMSPropsPage_TimeEvents>)
    END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnAddEvent(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditEvent(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDeleteEvent(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDeleteAllEvents(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEventList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    int OnApply();
	void UpdateEventList();
	void UpdateControls();

private:
	iMapHandler& m_map;
};

#endif //_HMM_EDITOR_PROPERTY_PAGES_H_
