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

#ifndef _HMM_MAP_EDITOR_ULTIMATE_ARTIFACT_PROPS_DLG_H_
#define _HMM_MAP_EDITOR_ULTIMATE_ARTIFACT_PROPS_DLG_H_

class iUltArtPropsDlg : public CDialogImpl<iUltArtPropsDlg>
{
public:
	enum { IDD = IDD_ULTIMATE_ART };

	BEGIN_MSG_MAP(iUltArtPropsDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	iMapHandler&	m_map;

	iUltArtPropsDlg(iMapHandler& map)
	: m_map(map)
	{
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CUpDownCtrl(GetDlgItem(IDC_RADIUS_SP)).SetRange(0, 255);
		LI::SetDlgItemInt(m_hWnd,IDC_RADIUS_ED,m_map.m_radUltimateArt);
		return 0;
	}

	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_map.m_radUltimateArt = LI::GetDlgItemInt(m_hWnd,IDC_RADIUS_ED);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
};

#endif //_HMM_MAP_EDITOR_ULTIMATE_ARTIFACT_PROPS_DLG_H_