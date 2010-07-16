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

#ifndef _SEQUENCE_PLAYER_CONTROL_BAR_H_
#define _SEQUENCE_PLAYER_CONTROL_BAR_H_

class iPlayerCtrlBar : public CDialogImpl<iPlayerCtrlBar>
{
public:

	enum { IDD = IDD_PLAYER_CTRL_BAR };

	BEGIN_MSG_MAP(iPlayerCtrlBar)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	iPlayerCtrlBar(iSeqPlayer& player) : m_player(player)
	{}

	HWND CreatePlayerCtrlBar(HWND hParent)
	{
		return Create(hParent);
	}

	void OnFileChanged()
	{
		EnableWindow(TRUE);
		CTrackBarCtrl tb(GetDlgItem(IDC_SLIDER1));
		tb.SetRange(0, m_player.GetSeqCnt()-1);
		tb.SetPos(m_player.GetCurFrame());
		UpdateInfo();
	}

	void UpdateInfo()
	{
		SetDlgItemText(IDC_INFO,iFormat(_T("%d / %d\n0 - 0"), m_player.GetCurFrame(), m_player.GetSeqCnt()-1).CStr());
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		SetDlgItemText(IDC_INFO,_T("-n/a-"));
		EnableWindow(FALSE);
		return 0;
	}

	LRESULT OnHScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		WORD msg = LOWORD(wParam);
		if (msg == TB_THUMBTRACK || msg == TB_ENDTRACK) {
			m_player.SetCurFrame(CTrackBarCtrl(GetDlgItem(IDC_SLIDER1)).GetPos());
			UpdateInfo();
		}
		/*
		if (wNotifyCode == TB_THUMBTRACK) {
			CTrackBarCtrl tb(GetDlgItem(IDC_SLIDER1));
			m_pPlayer->SetCurFrame(tb.GetPos());
		}*/

		return 0;
	}
	
	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		return 0;
	}

	iSeqPlayer&	m_player;
};


#endif //_SEQUENCE_PLAYER_CONTROL_BAR_H_