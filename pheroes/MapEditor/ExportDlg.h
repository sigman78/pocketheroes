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

#ifndef _EXPORT_DIALOG_H_
#define _EXPORT_DIALOG_H_

class iExportDlg : public CDialogImpl<iExportDlg>
{
public:
	enum { IDD = IDD_EXPORT };

	BEGIN_MSG_MAP(iExportDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDC_EXPRES, OnExpResCheck)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	iSimpleArray<iStringT> m_mapFileList;


	iExportDlg(const iStringT& mapFile);
	iExportDlg(const iSimpleArray<iStringT>& mapFileList);

	void Log(const iStringT& msg);
	bool ExportMap(iDynamicBuffer& buff, const iMapHandler& map);
	void ExportPathElements(iDynamicBuffer& buff);
	void ExportCovers(iDynamicBuffer& buff, const iVisualObject& vol);
	bool Export();

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnExpResCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	void UpdateControls(bool bRun);

	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
};

#endif //_EXPORT_DIALOG_H_