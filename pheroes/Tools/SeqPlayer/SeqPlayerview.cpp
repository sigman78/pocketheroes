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

// SeqPlayerView.cpp : implementation of the CSeqPlayerView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Player.h"
#include "SeqPlayerView.h"
#include "he/he.wnd.helpers.h"

BOOL CSeqPlayerView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

CSeqPlayerView::CSeqPlayerView(IPlayerCtrl* pPlayerCtrl)
: m_player(pPlayerCtrl)
{
}

HWND CSeqPlayerView::CreateView(HWND hParent)
{
	return Create(hParent, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
}

LRESULT CSeqPlayerView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	m_player.Compose(dc, iWMetrics::GetClRect(m_hWnd));
	//TODO: Add your drawing code here

	return 0;
}
