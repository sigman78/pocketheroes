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
#include "Dlg_PuzzleMap.h"

iPuzzleMapDlg::iPuzzleMapDlg(iViewMgr* pViewMgr, const iPlayer& player, uint16 newOpened)
: iBaseGameDlg(pViewMgr, player.PlayerId()), m_player(player), m_newOpened(newOpened), m_aniTimer( (newOpened)?255:0 )
{
}


void iPuzzleMapDlg::PrepareMap()
{
	iDib tmpDib(iSize(502,278), iDib::RGB);
	m_dibMap.Init(iSize(251,139), iDib::RGB);
	iIsoMetric im(5);
	iPoint anchor = im.Map2Screen(gGame.Map().UltimatePos());
	anchor.x += gGame.Map().GetWidth() * 20 + 40;
	anchor.y += 50;
	anchor.x -= tmpDib.GetWidth()/2;
	anchor.y -= tmpDib.GetHeight()/2;

	iOLComposer::Compose(tmpDib, anchor, true, gGame.Map().UltimatePos());
	iDibTransform::PyramidalStretch(tmpDib, m_dibMap, 1);
	m_dibMap.DarkenBWRect(m_dibMap.GetSize());

	// Prepare puzzled cell dib
	m_puzzledCell.Init(iSize(29,29), iDib::RGB);
	m_puzzledCell.FrameRect(m_puzzledCell.GetSize(), cColor_Black);
	gGfxMgr.BlitTile(PDGG_BKTILE,m_puzzledCell,iRect(1,1,27,27));
	m_puzzledCell.Darken25Rect(iRect(1,1,27,27));
	ButtonFrame(m_puzzledCell, iRect(1,1,27,27), 0);

}

void iPuzzleMapDlg::OnCreateDlg()
{
	iRect clRect = ClientRect();
	AddChild(new iTextButton(m_pMgr,this,iRect(clRect.x+(clRect.w/2-20),clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_OK, DRC_OK));
	PrepareMap();
	if (m_newOpened) m_pMgr->SetTimer(500, 1, this);
}

void iPuzzleMapDlg::OnTimer(uint32 tid)
{
	if (m_aniTimer > 32) {
		m_aniTimer -= 32;
		m_pMgr->SetTimer(100, 1, this);
	} else m_aniTimer = 0;
	Invalidate();
}

void iPuzzleMapDlg::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);
	gTextComposer.TextOut(dlgfc_hdr, gApp.Surface(), rc, gTextMgr[TRID_PUZZLE_MAP], rc, AlignTop);

	iRect orc(rc.x + (rc.w/2-m_dibMap.GetWidth()/2)-2, rc.y+20, m_dibMap.GetWidth()+4, m_dibMap.GetHeight()+4);
	ButtonFrame(gApp.Surface(), orc, iButton::Pressed);
	orc.DeflateRect(1);
	gApp.Surface().FrameRect(orc, cColor_Black);

	// Compose map
	m_dibMap.CopyToDibXY(gApp.Surface(), iPoint(orc.x+1, orc.y+1));

	// Compose puzzles
	for (uint32 xx=m_player.OpenedPuzzles(); xx<45; ++xx ) {
		sint32 xp = m_player.PuzzledCard(xx) % 9;
		sint32 yp = m_player.PuzzledCard(xx) / 9;
		m_puzzledCell.CopyToDibXY(gApp.Surface(), iPoint(orc.x+xp*28,orc.y+yp*28));
	}

	if (m_aniTimer > 0) {
		for (uint32 xx=m_player.OpenedPuzzles()-m_newOpened; xx<m_player.OpenedPuzzles(); ++xx ) {
			sint32 xp = m_player.PuzzledCard(xx) % 9;
			sint32 yp = m_player.PuzzledCard(xx) / 9;
			m_puzzledCell.CopyRectToDibXY(gApp.Surface(), iRect(1,1,27,27), iPoint(orc.x+xp*28+1,orc.y+yp*28+1), m_aniTimer);
		}
	}
}

iSize iPuzzleMapDlg::ClientSize() const
{
	return iSize(280, 170 + DEF_BTN_HEIGHT);
}

void iPuzzleMapDlg::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (cmd == CCI_BTNCLICK) {
		m_pMgr->CleanupTimers(this);
		EndDialog(uid);
	}
}


