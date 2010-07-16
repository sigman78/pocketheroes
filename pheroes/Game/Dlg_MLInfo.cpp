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
#include "Dlg_MLInfo.h"

iMLInfoDlg::iMLInfoDlg(iViewMgr* pViewMgr, bool bLuck, sint8 val, PLAYER_ID pid)
: iBaseGameDlg(pViewMgr, pid), m_bLuck(bLuck), m_val(val)
{
	if (m_bLuck){
		m_title = gTextMgr[TRID_SKILL_LUCK];
		if (val < 0) m_text = gTextMgr[TRID_INFO_BAD_LUCK];
		else if (val > 0) m_text = gTextMgr[TRID_INFO_GOOD_LUCK];
		else m_text = gTextMgr[TRID_INFO_NEUT_LUCK];
	} else {
		m_title = gTextMgr[TRID_SKILL_MORALE];
		if (val < 0) m_text = gTextMgr[TRID_INFO_BAD_MORALE];
		else if (val > 0) m_text = gTextMgr[TRID_INFO_GOOD_MORALE];
		else m_text = gTextMgr[TRID_INFO_NEUT_MORALE];
	}
}

void iMLInfoDlg::OnCreateDlg()
{
	iRect clRect = ClientRect();
	AddChild(new iTextButton(m_pMgr,this,iRect(clRect.x+(clRect.w/2-20),clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_OK, DRC_OK));
}

void iMLInfoDlg::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);

	if (!m_title.Empty()) {
		sint32 h = gTextComposer.TextBoxOut(dlgfc_hdr, gApp.Surface(), m_title, rc);
		rc.DeflateRect(0,h + 10, 0, 0);
	}

	if (!m_text.Empty()) {
		sint32 h = gTextComposer.TextBoxOut(dlgfc_plain, gApp.Surface(), m_text, rc);
		rc.DeflateRect(0,h + 10, 0, 0);
	}

	rc.DeflateRect(0,0,0,20);
	if (m_bLuck) {
		ComposeLuckGlyph(gApp.Surface(), rc, m_val);
	} else {
		ComposeMoraleGlyph(gApp.Surface(), rc, m_val);
	}

}

iSize iMLInfoDlg::ClientSize() const
{
	sint32 w = 130;
	if (m_text.Length() > 20) w += iCLAMP<sint32>(0,70,m_text.Length()/4);
	sint32 h = 0;

	// title
	if (!m_title.Empty()) {
		iSize s = gTextComposer.GetTextBoxSize(m_title, w, dlgfc_hdr);
		h += s.h;
		h += 10;
	}

	// text
	if (!m_text.Empty()) {
		iSize s = gTextComposer.GetTextBoxSize(m_text, w, dlgfc_plain);
		h += s.h;
		h += 10;
	}

	// icon
	if (!m_text.Empty()) {
		h += 50;
	}

	// ok button
	h += DEF_BTN_HEIGHT;

	return iSize(w, h);
}

void iMLInfoDlg::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (cmd == CCI_BTNCLICK) EndDialog(uid);
}


