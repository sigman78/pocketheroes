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

#ifndef _HMM_EDITOR_NEW_MAP_DLG_H_
#define _HMM_EDITOR_NEW_MAP_DLG_H_

#include "MapGen.h"

class iNewMapDlg : public CDialogImpl<iNewMapDlg>
{
public:
	enum { IDD = IDD_NEWMAP };

	BEGIN_MSG_MAP(iNewMapDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	iMapHandler*	m_pMap;

	int DoNewMapModal(iMapHandler* pMap )
	{
		m_pMap = pMap;
		return DoModal();
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		SetDlgItemText(IDC_MAP_NAME, _T("New Map"));

		for (uint32 msiz=MSIZ_SMALL; msiz<MSIZ_COUNT; ++msiz){
			iStringT str;
			str.Setf(_T("%s (%dx%d)"),MAP_SIZ_SNAME[msiz],MAP_SIZ_SIZE[msiz],MAP_SIZ_SIZE[msiz]);
			CComboBox(GetDlgItem(IDC_MAP_SIZE)).AddString(str.CStr());
		}
		CComboBox(GetDlgItem(IDC_MAP_SIZE)).SetCurSel(MSIZ_MEDIUM);

		return 0;
	}

	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		iStringT mapName;
		LI::GetDlgItemText(m_hWnd,IDC_MAP_NAME,mapName);
		int ssel = CComboBox(GetDlgItem(IDC_MAP_SIZE)).GetCurSel();
		m_pMap->InitMap((MAP_SIZE)ssel);
		m_pMap->AddPlayer(PID_RED,PTM_HUMAN_OR_COMPUTER);
		m_pMap->AddPlayer(PID_GREEN,PTM_HUMAN_OR_COMPUTER);

		m_pMap->m_lngMask = 1 << GLNG_ENGLISH;
		m_pMap->m_ctMgr.AddTextEntry(MakeTextEntryName(TET_MAP_NAME, NULL), mapName, TET_MAP_NAME);
		m_pMap->m_ctMgr.AddTextEntry(MakeTextEntryName(TET_MAP_DESC, NULL), L"Default desctiption", TET_MAP_DESC);


		if (CButton(GetDlgItem(IDC_GEN_LSCAPE)).GetCheck()) {
			iMapGenerator mg(*m_pMap);
			mg.Generate();
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

#endif //_HMM_EDITOR_NEW_MAP_DLG_H_