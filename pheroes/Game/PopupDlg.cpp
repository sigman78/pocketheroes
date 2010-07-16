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
#include "GfxHlp.h"

iTextComposer::FontConfig fc_hdr(iTextComposer::FS_MEDIUM, iDibFont::ComposeProps(RGB16(255,255,0)));
iTextComposer::FontConfig fc_topic(iTextComposer::FS_SMALL, iDibFont::ComposeProps(RGB16(160,192,255)));
iTextComposer::FontConfig fc_topic2(iTextComposer::FS_SMALL, iDibFont::ComposeProps(RGB16(255,192,160)));
iTextComposer::FontConfig fc_plain(iTextComposer::FS_SMALL, iDibFont::ComposeProps(RGB16(192,192,192),RGB16(32,32,32)));
iTextComposer::FontConfig fc_info(iTextComposer::FS_SMALL, iDibFont::ComposeProps(RGB16(255,192,0),RGB16(0,0,0)));

/*
 *	
 */
iTextPopupDlg::iTextPopupDlg(iViewMgr* pViewMgr, const iStringT& text, PLAYER_ID pid)
: iFramePopupView(pViewMgr, pid), m_Text(text){ }

iSize iTextPopupDlg::ClientSize()
{
	if (!m_Text.Empty()) {
		iSize s = gTextComposer.GetTextBoxSize(m_Text, 80, fc_topic);
		return iSize(80,s.h);
	}

	return iSize(80,10);
}

void iTextPopupDlg::DoCompose(const iRect& clRect)
{
	if (!m_Text.Empty()) {
		iSize s = gTextComposer.GetTextBoxSize(m_Text, 80, fc_topic);
		gTextComposer.TextBoxOut(fc_topic,gApp.Surface(),m_Text,iRect(clRect.x, clRect.y + (clRect.h/2-s.h/2),clRect.w,s.h));
	}
}

/*
 *	
 */
iPopupDlg_Guard::iPopupDlg_Guard(iViewMgr* pViewMgr, iMapGuard* pGuard, VISION_LEVEL vl, iHero* pHero)
: iFramePopupView(pViewMgr,PID_NEUTRAL), m_pGuard(pGuard)
{
	m_text1 = CreatGroup2Text(m_pGuard->Creatures().Type(),m_pGuard->Creatures().Count(), vl);
	if (vl == VL_EXPERT) {
		iMapGuard::MEET_RESULT mr = m_pGuard->CheckMeetResult(pHero);
		if (mr == iMapGuard::MR_ATTACK) m_text2 = gTextMgr[TRID_MSG_GUARDS_POPUP_ATTACK];
		else if (mr == iMapGuard::MR_FLEE) m_text2 = gTextMgr[TRID_MSG_GUARDS_POPUP_FLEE];
		else m_text2 = gTextMgr[TRID_MSG_GUARDS_POPUP_JOIN];
	}
}

iSize iPopupDlg_Guard::ClientSize()
{
	iSize clsiz(90,0);
	clsiz.h += 5;
	clsiz.h += gTextComposer.GetTextBoxSize(m_text1, 90, fc_topic).h;
	if (!m_text2.Empty()) clsiz.h += gTextComposer.GetTextBoxSize(m_text2,clsiz.w, fc_info).h+5;
	clsiz.h += 5;
	return clsiz;
}

void iPopupDlg_Guard::DoCompose(const iRect& clRect)
{
	if (m_text2.Empty()) {
		iSize s  = gTextComposer.GetTextBoxSize(m_text1, 90, fc_topic);
		gTextComposer.TextBoxOut(fc_topic, gApp.Surface(), m_text1, iRect(clRect.x, clRect.y + (clRect.h/2-s.h/2),clRect.w,s.h));
	} else {
		iRect orc = clRect;
		orc.y += 5;
		orc.y += gTextComposer.TextBoxOut(fc_topic, gApp.Surface(), m_text1, orc);
		orc.y += 5;
		gTextComposer.TextBoxOut(fc_info, gApp.Surface(), m_text2, orc);
	}
}


/*
 *	
 */
iPopupDlg_VisCnst::iPopupDlg_VisCnst(iViewMgr* pViewMgr, iVisCnst* pVisCnst, VISION_LEVEL vl, const iHero* pVisitor)
: iFramePopupView(pViewMgr,PID_NEUTRAL), m_pVisCnst(pVisCnst), m_vLevel(vl), m_pVisitor(pVisitor)
{ }

iSize iPopupDlg_VisCnst::ClientSize()
{
	iSize clsiz(120,0);
	// title
	clsiz.h += 14;
	// Custom popup message
	iStringT cmsg;
	if (m_pVisCnst->CustomPopupText(m_pVisitor, cmsg)){
		clsiz.h += 12 + 5;
	}

	// guard ?
	if (!m_pVisCnst->Guard().Empty()){
		clsiz.h += 12 + m_pVisCnst->Guard().GroupCount()*10 + 5;
	}
	// visited ?
	if (m_pVisCnst->ShowVisitedLabel()) clsiz.h += 12;

	return clsiz;
}

void iPopupDlg_VisCnst::DoCompose(const iRect& clRect)
{
	sint32 ypos = clRect.y;
	// Construction title
	gTextComposer.TextOut(fc_hdr,gApp.Surface(),clRect, gTextMgr[m_pVisCnst->Proto()->NameKey()], iRect(clRect.x,ypos,clRect.w,14),AlignCenter);
	ypos += 14;

	// Custom popup message
	iStringT cmsg;
	if (m_pVisCnst->CustomPopupText(m_pVisitor, cmsg)){
		gTextComposer.TextOut(fc_plain,gApp.Surface(),clRect, iStringT(_T("(")) + cmsg + iStringT(_T(")")), iRect(clRect.x,ypos,clRect.w,15),AlignCenter);
		ypos += 12 + 5;
	}

	// guard?
	if (!m_pVisCnst->Guard().Empty()){
		gTextComposer.TextOut(fc_topic,gApp.Surface(),clRect, gTextMgr[TRID_GUARD], iRect(clRect.x,ypos,clRect.w,15),AlignCenter);
		ypos+=12;
		for (uint32 xx=0; xx<7; ++xx) {
			if (m_pVisCnst->Guard()[xx].IsValid()){
				gTextComposer.TextOut(fc_plain,gApp.Surface(),clRect,CreatGroup2Text( m_pVisCnst->Guard()[xx].Type(), m_pVisCnst->Guard()[xx].Count(), m_vLevel), iRect(clRect.x,ypos,clRect.w,15),AlignCenter);
				ypos+=10;
			}
		}
		ypos += 5;
	}

	// Visited ?
	if (m_pVisCnst->ShowVisitedLabel()) {
		TextResId trid = (m_pVisitor && m_pVisCnst->Visited(m_pVisitor))?TRID_VISITED:TRID_NOT_VISITED;
		gTextComposer.TextOut(fc_topic,gApp.Surface(),clRect, iFormat((_T("- %s -")),gTextMgr[trid]), iRect(clRect.x,ypos,clRect.w,12),AlignCenter);
	}
}
/*
 *	
 */
iPopupDlg_OwnCnst::iPopupDlg_OwnCnst(iViewMgr* pViewMgr, iOwnCnst* pOwnCnst, VISION_LEVEL vl)
: iFramePopupView(pViewMgr, pOwnCnst->Owner()), m_pOwnCnst(pOwnCnst), m_vLevel(vl)
{ }

iSize iPopupDlg_OwnCnst::ClientSize()
{
	iSize clsiz(120,0);
	// title
	clsiz.h += 20;
	// guard ?
	if (!m_pOwnCnst->Guard().Empty()){
		clsiz.h += 12 + m_pOwnCnst->Guard().GroupCount()*10 + 5;
	}

	return clsiz;
}

void iPopupDlg_OwnCnst::DoCompose(const iRect& clRect)
{
	sint32 ypos = clRect.y;
	// Construction title
	gTextComposer.TextOut(fc_hdr,gApp.Surface(),clRect, gTextMgr[m_pOwnCnst->Proto()->NameKey()], iRect(clRect.x,ypos,clRect.w,20),AlignCenter);
	ypos += 20;

	// guard?
	if (!m_pOwnCnst->Guard().Empty()){
		gTextComposer.TextOut(fc_topic,gApp.Surface(),clRect, gTextMgr[TRID_GUARD], iRect(clRect.x,ypos,clRect.w,15),AlignCenter);
		ypos+=12;
		for (uint32 xx=0; xx<7; ++xx) {
			if (m_pOwnCnst->Guard()[xx].IsValid()){
				gTextComposer.TextOut(fc_plain,gApp.Surface(),clRect,CreatGroup2Text( m_pOwnCnst->Guard()[xx].Type(), m_pOwnCnst->Guard()[xx].Count(), m_vLevel), iRect(clRect.x,ypos,clRect.w,15),AlignCenter);
				ypos+=10;
			}
		}
		ypos += 5;
	}
}

/*
 *	
 */
iPopupDlg_Castle::iPopupDlg_Castle(iViewMgr* pViewMgr, iCastle* pCastle, VISION_LEVEL vl)
: iFramePopupView(pViewMgr, pCastle->Owner()), m_pCastle(pCastle), m_vLevel(vl)
{ }

iSize iPopupDlg_Castle::ClientSize()
{
	return iSize(123,104);
}

void iPopupDlg_Castle::DoCompose(const iRect& clRect)
{
	iRect orc(clRect);

	// Name
	gTextComposer.TextOut(fc_hdr, gApp.Surface(),orc.point(),m_pCastle->Name(),orc, AlignTop);
	orc.y+=14;

	// Icon
	SpriteId sid = PDGG_CTL_SICONS + (m_pCastle->Proto()->Size()-1)*CTLT_COUNT + m_pCastle->Proto()->Type();
	ButtonFrame(gApp.Surface(),iRect(orc.x+(orc.w/2-17),orc.y,34,22),iButton::Pressed);
	gGfxMgr.Blit(sid,gApp.Surface(), iRect(orc.x+(orc.w/2-16),orc.y+1,32,20));
	orc.y+=25;

	// Garrison
	ComposeArmyCells(gApp.Surface(), iPoint(orc.x,orc.y), iSize(30,32), m_pCastle->Garrison(), m_vLevel);
}

/*
 *	
 */
iPopupDlg_Hero::iPopupDlg_Hero(iViewMgr* pViewMgr, iHero*	pHero, VISION_LEVEL vl)
: iFramePopupView(pViewMgr, pHero->Owner()->PlayerId()), m_pHero(pHero), m_vLevel(vl)
{ }

iSize iPopupDlg_Hero::ClientSize()
{
	return iSize(123,118);
}

void iPopupDlg_Hero::DoCompose(const iRect& clRect)
{

	iRect orc(clRect);

	// Name
	gTextComposer.TextOut(fc_hdr, gApp.Surface(),orc.point(),m_pHero->Name(),orc, AlignTop);
	orc.y+=14;

	// Icon
	SpriteId sid = m_pHero->Proto()->m_icn32;
	ButtonFrame(gApp.Surface(),iRect(orc.x+(orc.w/2-17),orc.y,34,22),iButton::Pressed);
	gGfxMgr.Blit(sid,gApp.Surface(), iRect(orc.x+(orc.w/2-16),orc.y+1,32,20));
	orc.y+=25;

	// Skills
	iStringT psText;
	for (sint32 psid=0; psid<PRSKILL_COUNT; ++psid) {
		if (m_vLevel==VL_EXPERT) psText.Addf(_T("#I%04X #FCCC%d "), PDGG_ICN_PSKILLS+psid, m_pHero->FurtSkill((FURTHER_SKILLS)(FSK_ATTACK+psid)));
		else psText.Addf(_T("#I%04X #FCCC? "), PDGG_ICN_PSKILLS+psid);
	}
	gTextComposer.TextOut(gApp.Surface(), orc.point(), psText,orc, AlignTop);
	orc.y+=14;

	// Army
	ComposeArmyCells(gApp.Surface(), iPoint(orc.x,orc.y), iSize(30,32), m_pHero->Army(), m_vLevel);
}

