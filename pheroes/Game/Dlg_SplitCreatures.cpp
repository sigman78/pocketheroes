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
#include "Dlg_SplitCreatures.h"


iDlg_SplitCreatures::iDlg_SplitCreatures(iViewMgr* pViewMgr, PLAYER_ID pid, iCreatGroup& g1, iCreatGroup& g2, uint32 flags)
: iBaseGameDlg(pViewMgr,pid), m_g1(g1), m_g2(g2), m_flags(flags)
{ 
	m_total = m_g1.Count();
	if (m_g2.IsValid()) m_total += m_g2.Count();
	m_left = m_g1.Count();
}

void iDlg_SplitCreatures::OnCreateDlg()
{
	iRect clrc = ClientRect();
	// Slider
	AddChild(m_pSlider = new iPHScrollBar(m_pMgr, this, iRect(clrc.x + (clrc.w/2-65), clrc.y2()-20-10-15,130,15), 101, iScrollBar::Horizontal));
	uint32 slen = m_total + 1;
	if (m_flags&SafeG1) slen--;
	if (m_flags&SafeG2) slen--;
	m_pSlider->SetMetrics(slen,1);

	sint32 npos = m_total-m_left;
	if (m_flags&SafeG2) npos--;
	m_pSlider->SetCurPos(npos);
	// Button size 40x15 (3*40+10)
	iRect rc(clrc.x + (clrc.w/2-45), clrc.y2()-DEF_BTN_HEIGHT, 40, DEF_BTN_HEIGHT);

	AddChild(new iTextButton(m_pMgr,this,rc,TRID_OK, DRC_OK));
	rc.x+=50;
	AddChild(new iTextButton(m_pMgr,this,rc,TRID_CANCEL, DRC_CANCEL));
}

void iDlg_SplitCreatures::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);

	// title
	gTextComposer.TextOut(dlgfc_hdr, gApp.Surface(), rc.point(), iFormat(_T("%s %s"), gTextMgr[TRID_SPLIT], gTextMgr[m_g1.Type()*3+TRID_CREATURE_PEASANT_F3]), rc, AlignTop);
	rc.DeflateRect(0,20,0,0);

	iRect orc(rc.x + (rc.w/2-65), rc.y,130,46);
	gTextComposer.TextOut(dlgfc_topic, gApp.Surface(), orc.point(), FormatNumber(m_left), iRect(orc.x,orc.y,42,46), AlignBottom);
	BlitIcon(gApp.Surface(), PDGG_MINIMON + m_g1.Type(), iRect(orc.x + 42,orc.y,46,46));
	gTextComposer.TextOut(dlgfc_topic, gApp.Surface(), orc.point(), FormatNumber(m_total-m_left), iRect(orc.x+42+46,orc.y,42,46), AlignBottom);
}

iSize iDlg_SplitCreatures::ClientSize() const
{
	return iSize(180, 100 + DEF_BTN_HEIGHT);
}

void iDlg_SplitCreatures::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (uid == 101) {
		m_left = m_total - m_pSlider->CurPos();
		if (m_flags&SafeG2) m_left--;
	} else if (cmd == CCI_BTNCLICK) {
		if (uid == DRC_OK) {
			if (m_left == m_total) {
				m_g1.Count() = m_total;
				m_g2.Reset();
			} else if (!m_left) {
				m_g2.Type() = m_g1.Type();
				m_g2.Count() = m_total;
				m_g1.Reset();
			} else {
				m_g2.Type() = m_g1.Type();
				m_g2.Count() = m_total - m_left;
				m_g1.Count() = m_left;
			}
			EndDialog(DRC_OK);
		} else if (uid == DRC_CANCEL) {
			EndDialog(DRC_CANCEL);
		}
	}
}


