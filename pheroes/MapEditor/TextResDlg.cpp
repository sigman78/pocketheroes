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
#include "resource.h"
#include "TextResDlg.h"

//////////////////////////////////////////////////////////////////////////
class iTextEditDlg : public CDialogImpl<iTextEditDlg>
{
public:
	enum { IDD = IDD_TEXTEDIT };
	
	BEGIN_MSG_MAP(iTextEditDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		END_MSG_MAP()
		
		iTextEditDlg(const iStringT& title, iStringT& orig_text, iStringT& text, bool bMultiline) : m_title(title), m_orig_text(orig_text), m_text(text), m_bMultiline(bMultiline) {}
	
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		SetWindowText(m_title.CStr());
		//if (!m_bMultiline) ::SetWindowLong(GetDlgItem(IDC_TEXT), GWL_STYLE, ::GetWindowLong(GetDlgItem(IDC_TEXT), GWL_STYLE) & (~ES_MULTILINE));
		SetDlgItemText(IDC_ORIG_TEXT, m_orig_text.CStr());
		SetDlgItemText(IDC_TEXT, m_text.CStr());
		CEdit ectrl(GetDlgItem(IDC_TEXT));
		// Select
		ectrl.SetSelAll();
		ectrl.SetFocus();
		return 0;
	}
	
	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		LI::GetDlgItemText(m_hWnd, IDC_TEXT, m_text);
		EndDialog(wID);
		return 0;
	}
	
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
	
	iStringT	m_title;
	iStringT&	m_orig_text;
	iStringT&	m_text;
	bool		m_bMultiline;
};

//////////////////////////////////////////////////////////////////////////
iTextResDlg::iTextResDlg(iMapHandler* pMap) : m_pMap(pMap), m_curLng(GLNG_ENGLISH) {}

void iTextResDlg::UpdateTextResources()
{
	m_textEntries.RemoveAll();
	for (iTextMgr::TextIt it = m_pMap->m_ctMgr.GetUnsafeHash().First(); it != m_pMap->m_ctMgr.GetUnsafeHash().End(); ++it) {
		iTextMgr::iTextEntry& te = *it;
		m_textEntries.Add(iTextEntry(te.key, te.text[GLNG_ENGLISH], te.text[m_curLng], TEXT_ENTRY_ML[te.tet]));
	}

	//
	// Insert text items into list box
	//
	CListBox lb(GetDlgItem(IDC_TEXTRES_LIST));
	lb.ResetContent();
	for (uint32 nn=0; nn<m_textEntries.GetSize(); ++nn) {
		lb.AddString( (m_textEntries[nn].title + _T(" : ") + m_textEntries[nn].text).CStr() );
	}

	CButton(GetDlgItem(IDC_EDITBTN)).EnableWindow(FALSE);
}

LRESULT iTextResDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// List of supported languages
	for (uint32 lid=0; lid<GLNG_COUNT; ++lid) if (m_pMap->m_lngMask & (1 << lid))  m_langs.Add(lid);
	CComboBox cb(GetDlgItem(IDC_LNGLIST));
	for (lid = 0; lid < m_langs.GetSize(); ++lid) {
		cb.AddString(LANG_NAME[m_langs[lid]]);
	}
	cb.SetCurSel(0);

	UpdateTextResources();

	return 0;
}

void iTextResDlg::EditCurSel()
{
	CListBox lb(GetDlgItem(IDC_TEXTRES_LIST));
	sint32 csel = lb.GetCurSel();
	check(csel >= 0 && csel < m_textEntries.GetSize());
	iTextEditDlg tedlg(m_textEntries[csel].title, m_textEntries[csel].orig_text, m_textEntries[csel].text, m_textEntries[csel].bMultiline);
	if (tedlg.DoModal() == IDOK) {
		lb.DeleteString(csel);
		lb.InsertString(csel, (m_textEntries[csel].title + _T(" : ") + m_textEntries[csel].text).CStr());
	}
	CButton(GetDlgItem(IDC_EDITBTN)).EnableWindow(CListBox(GetDlgItem(IDC_TEXTRES_LIST)).GetCurSel() != -1);
}

LRESULT iTextResDlg::OnLngChanged(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (wNotifyCode == LBN_SELCHANGE) {
		uint32 nlng = m_langs[CComboBox(GetDlgItem(IDC_LNGLIST)).GetCurSel()];
		if (nlng != m_curLng) {
			m_curLng = (GAME_LANG)nlng;
		}
		UpdateTextResources();
	}

	return 0;
}

LRESULT iTextResDlg::OnSelChanged(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (wNotifyCode == LBN_SELCHANGE) {
		CButton(GetDlgItem(IDC_EDITBTN)).EnableWindow(CListBox(GetDlgItem(IDC_TEXTRES_LIST)).GetCurSel() != -1);
	} else if (wNotifyCode == LBN_DBLCLK) {
		CListBox lb(GetDlgItem(IDC_TEXTRES_LIST));
		sint32 csel = lb.GetCurSel();
		if (csel != -1) {
			iTextEditDlg tedlg(m_textEntries[csel].title, m_textEntries[csel].orig_text, m_textEntries[csel].text, m_textEntries[csel].bMultiline);
			if (tedlg.DoModal() == IDOK) {
				lb.DeleteString(csel);
				lb.InsertString(csel, (m_textEntries[csel].title + _T(" : ") + m_textEntries[csel].text).CStr());
			}
			CButton(GetDlgItem(IDC_EDITBTN)).EnableWindow(CListBox(GetDlgItem(IDC_TEXTRES_LIST)).GetCurSel() != -1);
		}
	}
	return 0;
}

LRESULT iTextResDlg::OnEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EditCurSel();
	return 0;
}

LRESULT iTextResDlg::OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}