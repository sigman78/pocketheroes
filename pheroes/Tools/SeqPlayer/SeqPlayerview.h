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

// SeqPlayerView.h : interface of the CSeqPlayerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEQPLAYERVIEW_H__E1431EA1_12C7_4997_B2E7_EDDD28306CF4__INCLUDED_)
#define AFX_SEQPLAYERVIEW_H__E1431EA1_12C7_4997_B2E7_EDDD28306CF4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CSeqPlayerView : public CWindowImpl<CSeqPlayerView>
{
public:

	DECLARE_WND_CLASS(NULL)

	CSeqPlayerView(IPlayerCtrl* pPlayerCtrl);
	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CSeqPlayerView)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	HWND CreateView(HWND hParent);

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) { return 0; }

	inline iSeqPlayer& Player() { return m_player; }

private:
	iSeqPlayer		m_player;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEQPLAYERVIEW_H__E1431EA1_12C7_4997_B2E7_EDDD28306CF4__INCLUDED_)
