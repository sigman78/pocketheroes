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

#ifndef _HMM_MAP_EDITOR_TEXT_RESOURCES_DIALOG_H_
#define _HMM_MAP_EDITOR_TEXT_RESOURCES_DIALOG_H_

//////////////////////////////////////////////////////////////////////////
class iTextResDlg : public CDialogImpl<iTextResDlg>
{
public:
	struct iTextEntry {
		iTextEntry(const iStringT& title_, iStringT& orig_text_, iStringT& text_, bool bMultiline_) : title(title_), orig_text(orig_text_), text(text_), bMultiline(bMultiline_) {}
		iStringT	title;
		iStringT&	orig_text;
		iStringT&	text;
		bool		bMultiline;
	};

	iSimpleArray<iTextEntry>	m_textEntries;
	iMapHandler*				m_pMap;
	iSimpleArray<uint32>		m_langs;
	GAME_LANG					m_curLng;

	enum { IDD = IDD_MAPTEXT };

	BEGIN_MSG_MAP(iTextResDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnClose)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
		COMMAND_ID_HANDLER(IDC_EDITBTN, OnEdit)
		COMMAND_ID_HANDLER(IDC_TEXTRES_LIST, OnSelChanged)
		COMMAND_ID_HANDLER(IDC_LNGLIST, OnLngChanged)
	END_MSG_MAP()

	iTextResDlg(iMapHandler* pMap);

	void UpdateTextResources();

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void EditCurSel();
	LRESULT OnSelChanged(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnLngChanged(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};

#endif //_HMM_MAP_EDITOR_TEXT_RESOURCES_DIALOG_H_