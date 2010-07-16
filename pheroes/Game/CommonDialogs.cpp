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

//////////////////////////////////////////////////////////////////////////
iBaseGameDlg::iBaseGameDlg(iViewMgr* pViewMgr, PLAYER_ID pid)
: iDialog(pViewMgr), m_pid(pid)
{}

iRect iBaseGameDlg::ClientRect() const
{
	iRect rc = GetDlgMetrics();
	rc.DeflateRect(DLG_FRAME_SIZE);
	return rc;
}

iSize iBaseGameDlg::GetMaxClientSize() const
{
	iSize res = gApp.Surface().GetSize();
	res.w -= DLG_FRAME_SIZE * 2;
	res.h -= DLG_FRAME_SIZE * 2;
	return res;
}

void iBaseGameDlg::OnCompose()
{
	ComposeDlgBkgnd(gApp.Surface(), m_Rect, m_pid, true);
	iRect rc(m_Rect);
	rc.DeflateRect(DLG_FRAME_SIZE);
	DoCompose(rc);
}

iSize iBaseGameDlg::GetDlgMetrics() const
{
	iSize siz = ClientSize();
	iSize max = gApp.Surface().GetSize();
	siz.w = iCLAMP<uint32>(80, max.w - 6, siz.w+DLG_FRAME_SIZE*2);
	siz.h = iCLAMP<uint32>(40, max.h - 6, siz.h+DLG_FRAME_SIZE*2);
	return siz;
}

//////////////////////////////////////////////////////////////////////////
iTextDlg::iTextDlg(iViewMgr* pViewMgr, const iStringT& title, const iStringT& text, PLAYER_ID pid, iTextComposer::FontConfig fc_title, iTextComposer::FontConfig fc_text)
: iBaseGameDlg(pViewMgr, pid), m_title(title), m_text(text), m_fcTitle(fc_title), m_fcText(fc_text)
{}


void iTextDlg::OnCreateDlg()
{
	iRect clRect = ClientRect();
	AddChild(new iTextButton(m_pMgr,this,iRect(clRect.x+(clRect.w/2-20),clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_OK, DRC_OK));
}

void iTextDlg::SetText(const iStringT& text)
{
	m_text = text;
}

void iTextDlg::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);

	if (!m_title.Empty()) {
		sint32 h = gTextComposer.TextBoxOut(m_fcTitle, gApp.Surface(), m_title, rc);
		rc.y += h;
		rc.y += 10;
	}

	if (!m_text.Empty()) {
		sint32 h = gTextComposer.TextBoxOut(m_fcText, gApp.Surface(), m_text, rc);
		rc.y += h;
		h += 10;
	}
}

iSize iTextDlg::ClientSize() const
{
	sint32 w = 130;
	if (m_text.Length() > 20) w += iCLAMP<sint32>(0,100,m_text.Length()/4);
	sint32 h = 0;

	// title
	if (!m_title.Empty()) {
		iSize s = gTextComposer.GetTextBoxSize(m_title, w, m_fcTitle);
		h += s.h;
		h += 10;
	}

	// text
	if (!m_text.Empty()) {
		iSize s = gTextComposer.GetTextBoxSize(m_text, w, m_fcText);
		h += s.h;
		h += 10;
	}

	// ok button
	h += DEF_BTN_HEIGHT;

	return iSize(w, h);
}

void iTextDlg::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (cmd == CCI_BTNCLICK) EndDialog(uid);
}

//////////////////////////////////////////////////////////////////////////
iExtTextDlg::iExtTextDlg(iViewMgr* pViewMgr, const iStringT& title, const iStringT& text, const iStringT& sign, PLAYER_ID pid)
: iBaseGameDlg(pViewMgr, pid), m_title(title), m_text(text), m_sign(sign)
{}


void iExtTextDlg::OnCreateDlg()
{
	iRect clRect = ClientRect();
	AddChild(new iTextButton(m_pMgr,this,iRect(clRect.x+(clRect.w/2-20),clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_OK, DRC_OK));
}

void iExtTextDlg::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);

	if (!m_title.Empty()) {
		sint32 h = gTextComposer.TextBoxOut(dlgfc_hdr, gApp.Surface(), m_title, rc);
		rc.y += h;
		rc.y += 10;
	}

	if (!m_text.Empty()) {
		sint32 h = gTextComposer.TextBoxOut(dlgfc_plain, gApp.Surface(), m_text, rc);
		rc.y += h;
		h += 10;
	}

	if (!m_sign.Empty()) {
		rc.y += 5;
		sint32 h = gTextComposer.TextBoxOut(dlgfc_topic, gApp.Surface(), m_sign, rc);
		rc.y += h;
		h += 5;
	}
}

iSize iExtTextDlg::ClientSize() const
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

	// sign
	if (!m_text.Empty()) {
		iSize s = gTextComposer.GetTextBoxSize(m_sign, w, dlgfc_topic);
		h += s.h;
		h += 10;
	}

	// ok button
	h += DEF_BTN_HEIGHT;

	return iSize(w, h);
}

void iExtTextDlg::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (cmd == CCI_BTNCLICK) EndDialog(uid);
}
//////////////////////////////////////////////////////////////////////////
iIconDlg::iIconDlg(iViewMgr* pViewMgr, const iStringT& title, const iStringT& text, SpriteId sid, PLAYER_ID pid)
: iTextDlg(pViewMgr, title, text, pid), m_sid(sid)
{}

void iIconDlg::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);
	iTextDlg::DoCompose(clRect);
	iSize ssiz = gGfxMgr.Dimension(m_sid);
	iPoint sa = gGfxMgr.Anchor(m_sid);
	rc.y = rc.y2() - 15 - 10 - ssiz.h;
	rc.x = rc.x + (rc.w/2 - ssiz.w/2);
	gGfxMgr.Blit(m_sid,gApp.Surface(),rc.point()-sa);
}

iSize iIconDlg::ClientSize() const
{
	iSize res = iTextDlg::ClientSize();
	iSize ssiz = gGfxMgr.Dimension(m_sid);
	res.h += ssiz.h + 10;
	return res;
}
//////////////////////////////////////////////////////////////////////////
iQuestDlg::iQuestDlg(iViewMgr* pViewMgr, const iStringT& title, const iStringT& text, PLAYER_ID pid)
: iTextDlg(pViewMgr, title, text, pid)
{}

void iQuestDlg::OnCreateDlg()
{
	iRect clRect = ClientRect();
	sint32 npos = clRect.x + (clRect.w/2-45);

	AddChild(new iTextButton(m_pMgr,this,iRect(npos,clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_YES, DRC_YES));
	AddChild(new iTextButton(m_pMgr,this,iRect(npos+50,clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_NO, DRC_NO));
}


