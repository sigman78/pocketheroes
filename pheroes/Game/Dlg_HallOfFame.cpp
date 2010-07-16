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
#include "Dlg_HallOfFame.h"
#include "helpers.h"

void iHighScore::Reset()
{
	m_entries.RemoveAll();
}

void iHighScore::Load(const iStringT& fname)
{
	iFileStream file(OpenWin32File(fname));
	if (file.IsValid()) {
		uint32 cnt;
		file.Read(cnt);
		iEntry entry;
		while (cnt--) {
			file.Read(entry.land);
			file.Read(entry.date);
			file.Read(entry.days);
			file.Read(entry.score);
			m_entries.Add(entry);
		}
	}
}

void iHighScore::Save(const iStringT& fname) const
{
	iFileStream file(CreateWin32File(fname));
	if (file.IsValid()) {
		file.Write(m_entries.GetSize());
		for (uint32 xx=0; xx<m_entries.GetSize(); ++xx) {
			file.Write(m_entries[xx].land);
			file.Write(m_entries[xx].date);
			file.Write(m_entries[xx].days);
			file.Write(m_entries[xx].score);
		}
	}
}

sint32 iHighScore::AddEntry(const iHighScore::iEntry& entry)
{
	uint32 idx;
	for (idx=0; idx<m_entries.GetSize(); ++idx) {
		if (entry.score > m_entries[idx].score) {
			m_entries.InsertBefore(idx, entry);
			if (m_entries.GetSize() > 10) m_entries.RemoveAt(10);
			return idx;
		}
	}
	if (m_entries.GetSize() < 10) {
		m_entries.Add(entry);
		return m_entries.GetSize() - 1;
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////////
iDlg_HallOfFame::iDlg_HallOfFame(iViewMgr* pViewMgr, const iStringT& fname)
: iBaseGameDlg(pViewMgr,PID_NEUTRAL), m_fname(fname), m_curScore(-1)
{
	m_hScore.Load(fname);
}

iDlg_HallOfFame::iDlg_HallOfFame(iViewMgr* pViewMgr, const iStringT& fname, const iHighScore::iEntry& entry)
: iBaseGameDlg(pViewMgr,PID_NEUTRAL), m_fname(fname)
{
	m_hScore.Load(fname);
	m_curScore = m_hScore.AddEntry(entry);
	m_hScore.Save(fname);
}

void iDlg_HallOfFame::OnCreateDlg()
{
	iRect clRect = ClientRect();

	// Buttons
	AddChild(new iTextButton(m_pMgr, this, iRect(clRect.x + clRect.w/2-55, clRect.y2()-DEF_BTN_HEIGHT, 50, DEF_BTN_HEIGHT), TRID_OK, DRC_OK));
	AddChild(new iTextButton(m_pMgr, this, iRect(clRect.x + clRect.w/2+5, clRect.y2()-DEF_BTN_HEIGHT, 50, DEF_BTN_HEIGHT), TRID_RESET, 1000));
}

void iDlg_HallOfFame::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);

	// title
	gTextComposer.TextOut(dlgfc_hdr, gApp.Surface(),rc.point(),gTextMgr[TRID_MENU_HIGHSCORE], iRect(rc.x,rc.y,rc.w,15),AlignCenter);
	rc.y+=17;

	// Header
	gTextComposer.TextOut(dlgfc_topic, gApp.Surface(),rc, gTextMgr[TRID_HOF_RANK], iRect(rc.x, rc.y, 30, 15),AlignTop);
	gTextComposer.TextOut(dlgfc_topic, gApp.Surface(),rc, gTextMgr[TRID_HOF_LAND], iRect(rc.x+30, rc.y, 120, 15),AlignTop);
	gTextComposer.TextOut(dlgfc_topic, gApp.Surface(),rc, gTextMgr[TRID_HOF_DATE], iRect(rc.x+30+120, rc.y, 50, 15),AlignTop);
	gTextComposer.TextOut(dlgfc_topic, gApp.Surface(),rc, gTextMgr[TRID_HOF_DAYS], iRect(rc.x+30+120+50, rc.y, 40, 15),AlignTop);
	gTextComposer.TextOut(dlgfc_topic, gApp.Surface(),rc, gTextMgr[TRID_HOF_SCORE], iRect(rc.x+30+120+50+40, rc.y, 40, 15),AlignTop);
	rc.y += 15;

	// Entries
	uint32 eid;
	iTextComposer::FontConfig fc(iTextComposer::FS_MEDIUM, RGB16(192,192,192));
	for (eid = 0; eid<m_hScore.Count() && eid<10; ++eid) {
		if (eid == (uint32)m_curScore) fc.cmpProps.faceColor = RGB16(255,192,128);
		else fc.cmpProps.faceColor = RGB16(192,192,192);
		gTextComposer.TextOut(fc, gApp.Surface(),rc, iFormat(_T("%d."),eid+1), iRect(rc.x, rc.y, 30, 15),AlignTop);
		gTextComposer.TextOut(fc, gApp.Surface(),rc, m_hScore.Entry(eid).land, iRect(rc.x+30, rc.y, 120, 15),AlignTopLeft);
		gTextComposer.TextOut(fc, gApp.Surface(),rc, FormatDate(m_hScore.Entry(eid).date, false), iRect(rc.x+30+120, rc.y, 50, 15),AlignTop);
		gTextComposer.TextOut(fc, gApp.Surface(),rc, iFormat(_T("%d"),m_hScore.Entry(eid).days), iRect(rc.x+30+120+50, rc.y, 40, 15),AlignTop);
		gTextComposer.TextOut(fc, gApp.Surface(),rc, iFormat(_T("%d"),m_hScore.Entry(eid).score), iRect(rc.x+30+120+50+40, rc.y, 40, 15),AlignTop);
		rc.y += 14;
	}
	fc.cmpProps.faceColor = RGB16(192,192,192);
	for (; eid<10; ++eid) {
		gTextComposer.TextOut(fc, gApp.Surface(),rc, iFormat(_T("%d."),eid+1), iRect(rc.x, rc.y, 30, 15),AlignTop);
		gTextComposer.TextOut(fc, gApp.Surface(),rc, _T("-"), iRect(rc.x+30, rc.y, 120, 15),AlignTopLeft);
		gTextComposer.TextOut(fc, gApp.Surface(),rc, _T("-"), iRect(rc.x+30+120, rc.y, 50, 15),AlignTop);
		gTextComposer.TextOut(fc, gApp.Surface(),rc, _T("-"), iRect(rc.x+30+120+50, rc.y, 40, 15),AlignTop);
		gTextComposer.TextOut(fc, gApp.Surface(),rc, _T("-"), iRect(rc.x+30+120+50+40, rc.y, 40, 15),AlignTop);
		rc.y += 14;
	}
}

iSize iDlg_HallOfFame::ClientSize() const
{
	return iSize(280,180 + DEF_BTN_HEIGHT);
}

void iDlg_HallOfFame::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (uid == DRC_OK) {
		EndDialog(DRC_OK);
	} else if (uid == 1000) {
		iQuestDlg qdlg(&gApp.ViewMgr(), _T(""), gTextMgr[TRID_MSG_RESET_HIGHSCORE], PID_RED);
		if (qdlg.DoModal() == DRC_YES) {
			m_hScore.Reset();
			m_hScore.Save(m_fname);
		}
	}
}

