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
#include "OverlandView.h"
#include "GameMenuDlg.h"
#include "Dlg_Save.h"
#include "gxl.popupview.h"
#include "ai.scanner.h"
#include "Dlg_CastSpell.h"
#include "dbg.h"
#include "helpers.h"

const iRect OVERLAND_POPUP_BOUND(14,28,292,176);

/*
 *	Main Game View
 */
iMainView::iMainView()
:iChildGameView(true, iChildGameView::CV_MENU) ,m_KeyTrackMask(0), m_MouseTrackMask(0)
, m_bForceTrack(false), m_bForceHelp(false), m_pPopupView(NULL), m_OLComposer(iRect(0,15,m_Rect.w,m_Rect.h-36), this), m_bSurveyMode(false), m_holdTimer(-1)
{
	// Insert info bar
	AddChild(m_pInfoBar = new iInfoBar(&gApp.ViewMgr(), this, iRect(0,0,m_Rect.w,15)));

	// Insert toolbars
	AddChild(m_pToolBar = new iMainToolBar(&gApp.ViewMgr(), this, iRect(0,m_Rect.h-21,m_Rect.w,21)));
	AddChild(m_pOpTurnBar = new iOpTurnBar(&gApp.ViewMgr(), iRect(0,m_Rect.h-21,m_Rect.w,21)));

	m_OLComposer.Init();
	UpdateMetrics();
}

iMainView::~iMainView()
{
	m_OLComposer.Cleanup();
}

iBaseMapObject*	iMainView::ObjectHitTest(const iPoint& scr_pos)
{
	return NULL;
}

void iMainView::UpdateMetrics()
{
	//
	rcScrLeft = rcScrRight = rcScrTop = rcScrBottom = iRect(0,14,m_Rect.w,m_Rect.h-14-21);
	rcScrLeft.w = 14;
	rcScrRight.x = rcScrRight.x2() - 14;
	rcScrRight.w = 14;
	rcScrTop.h = 14;
	rcScrBottom.y = rcScrBottom.y2() - 14;
	rcScrBottom.h = 14;
}

void iMainView::EnableToolBar(bool bEnable)
{ 
	m_pToolBar->SetEnabled(bEnable); 
}

void iMainView::OnTimer(uint32 tid)
{
	if (tid == 1) {
		if (m_holdTimer == -1) {
			m_holdPos = cInvalidPoint;
			return;
		}
		m_holdTimer += 100;
		if (m_holdTimer >= 1000) {
			m_OLComposer.SetTarget(cInvalidPoint, 0);
			ShowHelpPopup(m_holdPos);
			m_holdTimer = -1;
			m_holdPos = cInvalidPoint;
		} else 	m_pMgr->SetTimer(100,1,this);
	}
}

void iMainView::OnSizeChanged(const iSize& nsiz)
{
	UpdateMetrics();
}

void iMainView::OnMouseDown(const iPoint& pos)
{
	if (m_bForceTrack) {
		m_TrackAnchor = pos;
		m_OldAnchor = m_OLComposer.GetAnchor();
	} else {
		if (rcScrTop.PtInRect(pos)) m_MouseTrackMask |= 0x01;
		if (rcScrBottom.PtInRect(pos)) m_MouseTrackMask |= 0x02;
		if (rcScrLeft.PtInRect(pos)) m_MouseTrackMask |= 0x04;
		if (rcScrRight.PtInRect(pos)) m_MouseTrackMask |= 0x08;
		if (!m_MouseTrackMask) {
			if (m_bForceHelp || m_pToolBar->InfoMode()) {
				ShowHelpPopup(pos);
			} else {
				iIsoMetric im(5);
				iPoint ncell = im.Screen2Map(m_OLComposer.ActAnchor(pos));
				iHero* pHero = gGame.Map().CurHero();
				if (pHero) {
					if (pHero->Moving()) {
						pHero->Stop();
					} else {
						if (m_holdTimer == -1) {
							m_holdTimer = 0;
							m_holdPos = pos;
							m_pMgr->SetTimer(100, 1, this);
						}

						if (pHero->Pos() == ncell){
						} else if (gGame.Map().IsValidPos(ncell.x,ncell.y) && (pHero->Path().IsEmpty() || pHero->Path().DestPoint() != ncell)) {
							iPath path(pHero->Pos());
							SpriteId cr = PDGG_TRACK;
							if (pHero->MakePath(path, ncell.x, ncell.y)) {
								if (path.PathCost() > pHero->Moves()) cr += 1;
							} else {
								cr += 2;
							}
							m_OLComposer.SetTarget(ncell, cr);
						}
					}
				} else {
					if (m_holdTimer == -1) {
						m_holdTimer = 0;
						m_holdPos = pos;
						m_pMgr->SetTimer(100, 1, this);
					}
					// Set Target and start hold timer
					m_OLComposer.SetTarget(ncell, PDGG_TRACK + 2);
				}
			}
		}
		Invalidate();
	}
}

void iMainView::OnMouseClick(const iPoint& pos)
{
	if (!m_MouseTrackMask && !m_bForceTrack && !m_bForceHelp) {
		iPoint actPos = m_OLComposer.ActAnchor(pos);
		iIsoMetric im(5);
		iPoint ncell = im.Screen2Map(actPos);

		iBaseMapObject* pMapObj = gGame.Map().GetCellObj(ncell);
		if (ncell == m_OLComposer.GetTargetPoint() && DynamicCast<iCastle*>(pMapObj) && pMapObj->Pos() != ncell) {
			iCastle* pCastle = DynamicCast<iCastle*>(pMapObj);
			if (pCastle->Owner() == gGame.Map().CurPlayer()->PlayerId()) {
				if (pCastle != gGame.Map().CurCastle()) {
					gGame.Map().CurPlayer()->SetCurCastle(pCastle);
				} else {
					gGame.ShowView(CV_CASTLE);
				}
			}
		} else if (iHero* pHero = gGame.Map().CurHero()) {
			if (ncell == m_OLComposer.GetTargetPoint() && (DynamicCast<iOwnCnst*>(pMapObj) || DynamicCast<iVisCnst*>(pMapObj))) {
				pHero->SetPath(pMapObj->Pos());
			} else if (ncell == m_OLComposer.GetTargetPoint() || (!pHero->Path().IsEmpty() && pHero->Path().DestPoint() == ncell)) {
				pHero->SetPath(ncell);
			}
		} 
	}
	m_OLComposer.SetTarget(cInvalidPoint, 0);
	Invalidate();
}

void iMainView::OnMouseUp(const iPoint& pos)
{
	if (m_pPopupView) {
		RemoveChild(m_pPopupView);
		delete m_pPopupView;
		m_pPopupView = NULL;
		Invalidate();
	}

	if (m_MouseTrackMask) {
		m_MouseTrackMask = 0;
		Invalidate();
	}

	m_TrackAnchor=cInvalidPoint; 
	m_holdTimer = -1;
}

void iMainView::OnMouseTrack(const iPoint& pos)
{
	if (m_bForceTrack) {
		if (m_TrackAnchor == cInvalidPoint) m_TrackAnchor = pos;
		iPoint npos = m_OldAnchor+(m_TrackAnchor-pos);
		m_OLComposer.SetAnchor(npos);
		Invalidate();
	} else {
		uint8 trackMask=0;
		if (rcScrTop.PtInRect(pos)) trackMask |= 0x01;
		if (rcScrBottom.PtInRect(pos)) trackMask |= 0x02;
		if (rcScrLeft.PtInRect(pos)) trackMask |= 0x04;
		if (rcScrRight.PtInRect(pos)) trackMask |= 0x08;
		if (trackMask != m_MouseTrackMask) {
			Invalidate();
			m_MouseTrackMask = trackMask;
		}
	}

	if (!m_MouseTrackMask && m_OLComposer.GetTargetPoint() != cInvalidPoint) {
		iIsoMetric im(5);
		iPoint ncell = im.Screen2Map(m_OLComposer.ActAnchor(pos));
		if (ncell != m_OLComposer.GetTargetPoint()) {
			m_holdTimer = -1;
			if (iHero* pCurHero = gGame.Map().CurHero()) {
				iPath path(pCurHero->Pos());
				SpriteId cr = PDGG_TRACK;
				if (gGame.Map().IsValidPos(ncell.x,ncell.y) && pCurHero->Pos() != ncell && pCurHero->MakePath(path, ncell.x, ncell.y)) {
					if (path.PathCost() > pCurHero->Moves()) cr += 1;
				} else {
					cr += 2;
				}
				m_OLComposer.SetTarget(ncell, cr);
			} else {
				m_OLComposer.SetTarget(ncell, PDGG_TRACK + 2);
			}
			Invalidate();
		}
	}
}

bool iMainView::OnKeyDown(sint32 key)
{
	if (m_pPopupView) {
		RemoveChild(m_pPopupView);
		delete m_pPopupView;
		m_pPopupView = NULL;
		Invalidate();
	}

	if (key == KEY_UP || key == KEY_RIGHT || key == KEY_DOWN || key == KEY_LEFT) {
		m_KeyTrackMask |= (0x01 << (key-KEY_UP));
		if (m_KeyTrackMask) {
			SetEnabled(false);
			Invalidate();
		}
	} else if (key == gSettings.ActionKey(BAT_SURVEY_MODE)) {
		SetSurveyMode(true);
		SetEnabled(false);
		Invalidate();
	} else if (key == gSettings.ActionKey(BAT_HAND_MODE)) {
		m_pInfoBar->SetEnabled(false);
		m_pToolBar->SetEnabled(false);
		SetForceTrack(true);
	} else if (key == gSettings.ActionKey(BAT_HELP_MODE)) {
		m_pInfoBar->SetEnabled(false);
		m_pToolBar->SetEnabled(false);
		SetForceHelp(true);
	} else {
		return false;
	}
	return true;
}

bool iMainView::OnKeyUp(sint32 key)
{
	if (key == KEY_UP || key == KEY_RIGHT || key == KEY_DOWN || key == KEY_LEFT) {
		m_KeyTrackMask ^= (0x01 << (key-KEY_UP));
		Invalidate();
	} else if (key == gSettings.ActionKey(BAT_SURVEY_MODE)) {
		SetSurveyMode(false);
		Invalidate();
	} else if (key == gSettings.ActionKey(BAT_HAND_MODE)) {
		SetForceTrack(false);
	} else if (key == gSettings.ActionKey(BAT_HELP_MODE)) {
		SetForceHelp(false);
	} else {
		return false;
	}

	m_pToolBar->SetEnabled(!m_bForceTrack && !m_bForceHelp);
	m_pInfoBar->SetEnabled(!m_bForceTrack && !m_bForceHelp);
	SetEnabled(!m_bSurveyMode && !m_KeyTrackMask);
	return true;
}

void iMainView::SetSurveyMode(bool bSurveyMode)
{
	if (bSurveyMode == m_bSurveyMode) return;
	m_bSurveyMode = bSurveyMode;
	if (m_bSurveyMode) {
		uint32 scale = gSettings.GetEntryValue(CET_SURVMAPSCALE)?2:1;
		iDib tmpDib(iSize(320 << scale, 206 << scale), iDib::RGB);
		m_dibSurveyBuff.Init(iSize(320, 206), iDib::RGB);
		iPoint anchor = m_OLComposer.GetAnchor();
		anchor.x -= 160 * ((1<<scale)-1);
		anchor.y -= 103 * ((1<<scale)-1);
		m_OLComposer.Compose(tmpDib, anchor);
		if (gSettings.GetEntryValue(CET_SURVMAPFILTR)) iDibTransform::PyramidalStretch(tmpDib, m_dibSurveyBuff,scale);
		else iDibTransform::FastStretch(tmpDib, m_dibSurveyBuff,scale);
	} else {
		m_dibSurveyBuff.Cleanup();
	}
}

//
//
void iMainView::ShowHelpPopup(const iPoint& pos)
{
	iIsoMetric im(5);
	iPoint npos = im.Screen2Map(m_OLComposer.ActAnchor(pos));
	if (gGame.Map().IsValidPos(npos.x,npos.y)){
		uint8 dcfg, fcfg;
		check(!m_pPopupView);
		if (!gGame.Map().ActPlayer()->FogMap().GetFogCfg(npos,dcfg,fcfg) && dcfg==15) {
			AddChild(m_pPopupView = new iTextPopupDlg(m_pMgr,gTextMgr[TRID_UNCHARTED_TERRITORY],PID_NEUTRAL));
		} else {
			iStringT name;
			if (iBaseMapObject* pObj = gGame.Map().GetCellObj(npos, name)) {
				VISION_LEVEL vl = VL_BASIC;
				iHero* pHero = gGame.Map().CurHero();
				sint32 radius=0;
				if (!gGame.Map().ActPlayer()->FogMap().IsHidden(pObj->Pos()) && pHero) vl = pHero->VisionLevel(radius);
				if (vl == VL_EXPERT && pHero->Pos().GetDelta(pObj->Pos()) > radius) vl = VL_BASIC;
				if (iVisCnst* pVisCnst = DynamicCast<iVisCnst*>(pObj)) {
					AddChild(m_pPopupView = new iPopupDlg_VisCnst(m_pMgr,pVisCnst,vl,pHero));
				} else if (iOwnCnst* pOwnCnst = DynamicCast<iOwnCnst*>(pObj)) {
					if (pOwnCnst->Owner() == gGame.Map().ActPlayer()->PlayerId()) vl = VL_EXPERT;
					AddChild(m_pPopupView = new iPopupDlg_OwnCnst(m_pMgr,pOwnCnst, vl));
				} else if (iCastle* pCastle = DynamicCast<iCastle*>(pObj)) {
					if (pCastle->Owner() == gGame.Map().ActPlayer()->PlayerId()) vl = VL_EXPERT;
					AddChild(m_pPopupView = new iPopupDlg_Castle(m_pMgr,pCastle,vl));
				} else if (DynamicCast<iMapItem_Mineral*>(pObj) || DynamicCast<iMapItem_ManaCrystal*>(pObj) || DynamicCast<iMapItem_CampFire*>(pObj) || DynamicCast<iMapItem_Artifact*>(pObj) || DynamicCast<iMapItem_Chest*>(pObj) || DynamicCast<iMapItem_SpellScroll*>(pObj) || DynamicCast<iMapItem_KeyGuard*>(pObj) || DynamicCast<iMapItem_Lamp*>(pObj)) {
					iMapItem* pMapItem = (iMapItem*)pObj;
					AddChild(m_pPopupView = new iTextPopupDlg(m_pMgr,gTextMgr[pMapItem->NameKey()],PID_NEUTRAL));
				} else if (iMapGuard* pGuard = DynamicCast<iMapGuard*>(pObj)) {
					AddChild(m_pPopupView = new iPopupDlg_Guard(m_pMgr, pGuard, vl, pHero));
				} else if (iHero* pHero = DynamicCast<iHero*>(pObj)) {
					if (pHero->Owner()->PlayerId() == gGame.Map().ActPlayer()->PlayerId()) vl = VL_EXPERT;
					AddChild(m_pPopupView = new iPopupDlg_Hero(m_pMgr,pHero, vl));
					//OutputDebugString(iFormat(_T("Mana: %d/%d\n"), pHero->ManaPts(), pHero->MaxManaPts()).CStr());
				} else {
					check(0);
				}
			} else {
				AddChild(m_pPopupView = new iTextPopupDlg(m_pMgr,name,PID_NEUTRAL));
			}
		}
		check( m_pPopupView != NULL );
		m_pPopupView->TrackPopup(pos,OVERLAND_POPUP_BOUND,AlignBottomRight);
	}
}

void iMainView::OnCompose()
{	
	iRect rect = GetScrRect();
	rect.DeflateRect(0,14,0,21);

	if (m_bSurveyMode) {
		m_dibSurveyBuff.CopyToDibXY(gApp.Surface(), rect.point());
	} else {
		m_OLComposer.Compose();
	
		// Control Decorations
		gApp.Surface().HLine(iPoint(rect.x,rect.y+14),18,0xFFFF,64);
		gApp.Surface().HLine(iPoint(rect.x+rect.w/2-9,rect.y+14),rect.x+rect.w/2+9,0xFFFF,64);
		gApp.Surface().HLine(iPoint(rect.x2()-18,rect.y+14),rect.x2(),0xFFFF,64);
		gApp.Surface().HLine(iPoint(rect.x,rect.y2()-14),18,0xFFFF,64);
		gApp.Surface().HLine(iPoint(rect.x+rect.w/2-9,rect.y2()-14),rect.x+rect.w/2+9,0xFFFF,64);
		gApp.Surface().HLine(iPoint(rect.x2()-18,rect.y2()-14),rect.x2(),0xFFFF,64);
		gApp.Surface().VLine(iPoint(rect.x+14,rect.y),rect.y+18,0xFFFF,64);
		gApp.Surface().VLine(iPoint(rect.x+14,rect.y+rect.h/2-9),rect.y+rect.h/2+9,0xFFFF,64);
		gApp.Surface().VLine(iPoint(rect.x+14,rect.y2()-18),rect.y2(),0xFFFF,64);
		gApp.Surface().VLine(iPoint(rect.x2()-14,rect.y),rect.y+18,0xFFFF,64);
		gApp.Surface().VLine(iPoint(rect.x2()-14,rect.y+rect.h/2-9),rect.y+rect.h/2+9,0xFFFF,64);
		gApp.Surface().VLine(iPoint(rect.x2()-14,rect.y2()-18),rect.y2(),0xFFFF,64);
		uint8 mtm = GetMouseTrackMask();
		if (m_KeyTrackMask & 0x01 || mtm & 0x01) gApp.Surface().FillRect(iRect(rect.x,rect.y,rect.w,14),0xFFFF,48);
		if (m_KeyTrackMask & 0x02 || mtm & 0x02) gApp.Surface().FillRect(iRect(rect.x,rect.y2()-13,rect.w,14),0xFFFF,48);
		if (m_KeyTrackMask & 0x04 || mtm & 0x04) gApp.Surface().FillRect(iRect(rect.x,rect.y,14,rect.h),0xFFFF,48);
		if (m_KeyTrackMask & 0x08 || mtm & 0x08) gApp.Surface().FillRect(iRect(rect.x2()-13,rect.y,14,rect.h),0xFFFF,48);

		ComposeEventList();

		gApp.Surface().Darken50Rect(iRect(rect.x,rect.y,rect.w,1));
		gApp.Surface().Darken25Rect(iRect(rect.x,rect.y,rect.w,1));
		gApp.Surface().Darken50Rect(iRect(rect.x,rect.y+1,rect.w,1));
		gApp.Surface().Darken25Rect(iRect(rect.x,rect.y+2,rect.w,1));

	}
}

void iMainView::AddMsg(const iStringT& text)
{
	m_EventList.Add(new iGameEvent(fix32(4.0f),text));
	Invalidate();
}

void iMainView::AddCellMsg(const iStringT& msg, const iPoint& pos)
{
	m_OLComposer.AddCellMsg(msg,pos);
}

void iMainView::ComposeEventList()
{
	iTextComposer::FontConfig fc(iTextComposer::FS_MEDIUM, iDibFont::ComposeProps(cColor_White, cColor_Black,iDibFont::DecBorder));
	iPoint pos(18,18+15);
	if (iPlayer* pPlayer = gGame.Map().ActPlayer()){
		for (iEvtCIt it=m_EventList.First(); it != m_EventList.End(); ++it) {
			if ( (*it)->dstate ){
				pos.y -= (*it)->dstate;
			}
			
			uint8 a = (*it)->dstate?255-(25*(*it)->dstate):255;
			
			gTextComposer.TextOut(fc, gApp.Surface(), pos, iFormat(_T("#A%02X"),a)+(*it)->text);
			pos.y+=10;
		}
	}
}

const sint32 SCROLL_SPEED[5] = { 100, 175, 250, 400, 600 };

bool iMainView::Process(fix32 t)
{
	if (gApp.ViewMgr().HasModalDlg()) return false;

	// Keyboard and mouse state
	uint8 mtm = GetMouseTrackMask();
	uint8 ktm = GetKeyTrackMask();
	if (ktm || mtm) {
		sint32 val = (t * SCROLL_SPEED[gSettings.GetEntryValue(CET_MAPSCROLLSPEED)]).ceil();
		iPoint npos = m_OLComposer.GetAnchor();
		if (ktm & 0x01 || mtm & 0x01) npos.y -= val;
		if (ktm & 0x02 || mtm & 0x02) npos.y += val;
		if (ktm & 0x04 || mtm & 0x04) npos.x -= val;
		if (ktm & 0x08 || mtm & 0x08) npos.x += val;
		m_OLComposer.SetAnchor(npos);
		Invalidate();
	}

	// Process events
	fix32 hD(0.5f);
	fix32 fani(18.0f);
	iEvtIt it=m_EventList.First();
	bool bMsgRedraw = false;
	while(it != m_EventList.End()) {
		(*it)->time -= t;
		if ( (*it)->time < fix32::zero){
			it = m_EventList.Delete(it);
			bMsgRedraw = true;
			continue;
		} else if ((*it)->time < hD) {
			(*it)->dstate = (uint8)((hD-(*it)->time) * fani).ceil();
			bMsgRedraw = true;
		}
		++it;
	}

	// Animation
	if (m_OLComposer.StepAni(t)) {
		Invalidate();
		return false;
	}

	if (bMsgRedraw) Invalidate();

	return true;
}

void iMainView::OnActivate(bool bActivate)
{
	m_KeyTrackMask = 0;
	SetSurveyMode(false);
	SetForceTrack(false);
	SetForceHelp(false);
}

// Game notifications
void iMainView::OnPlayerChanged(iPlayer* pPlayer, bool bAct)
{
	SetEnabled(bAct);
	m_pToolBar->SetVisible(bAct);
	m_pOpTurnBar->SetVisible(!bAct);
	m_pToolBar->EnableCnstBar(bAct && gGame.Map().CurCastle());
	m_pToolBar->EnableHeroBar(bAct && gGame.Map().CurHero());
}

void iMainView::OnCastleListChanged()
{
	m_pToolBar->EnableCnstBar(gGame.Map().ActPlayer() && gGame.Map().CurCastle());
}

void iMainView::OnHeroListChanged()
{
	m_pToolBar->EnableHeroBar(gGame.Map().ActPlayer() && gGame.Map().CurHero());
}

void iMainView::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (uid == VPUID_BTN_NTURN) {
		iPlayer* pPlayer = gGame.Map().CurPlayer();

		// Are you sure you want to end your turn?
		if (gSettings.GetEntryValue(CET_ENDTURNCONF)) {
			for (iPlayer::iHLIt hit = pPlayer->HeroFirst(); hit != pPlayer->HeroEnd(); ++hit) {
				if ( (*hit)->Moves() > 27 ) {
					iQuestDlg qdlg(m_pMgr, _T(""), gTextMgr[TRID_MSG_END_TURN], pPlayer->PlayerId());
					if (qdlg.DoModal() != DRC_YES) {
						return;
					}
					break;
				}
			}
		}

		// Autosave
		iStringT fname;
		iSaveDlg::GetSaveFileName(fname,SAVE_GAME_SLOTS-1);
		SaveGameFile( fname, gGame.Map() );

		// Next turn
		gGame.Map().NextPlayer();
	} else if (uid == VPUID_BTN_STATE) {
		m_pMgr->TrackPopup(new iGamePopupMenu(m_pMgr), iPoint(320,15), iRect(0,0,320,240),AlignTopRight);
	}
}

/*
 *	Overland view Toolbar 
 */

//////////////////////////////////////////////////////////////////////////
iHeroTB::iPortBtn::iPortBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid)
: iButton(pViewMgr, pCmdHandler, rect, uid, Visible|Enabled) {}

void iHeroTB::iPortBtn::OnCompose()
{
	iRect rc = GetScrRect();
	iHero* pHero = gGame.Map().CurHero();
	SpriteId sid;
	if (pHero) sid = pHero->Proto()->m_icn32;
	else sid = PDGG_UNK_HERO;
	gGfxMgr.Blit(sid,gApp.Surface(), rc);
	ButtonFrame(gApp.Surface(),rc,GetButtonState());
	if (!IsEnabled()) gApp.Surface().Darken25Rect(rc);
}
//
iHeroTB::iMoveGaugeBtn::iMoveGaugeBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid)
: iButton(pViewMgr, pCmdHandler, rect, uid, Visible|Enabled) {}

void iHeroTB::iMoveGaugeBtn::OnCompose()
{
	iRect rc = GetScrRect();
	iHero* pHero = gGame.Map().CurHero();
	if (pHero) ComposeProgressBar(false, gApp.Surface(), rc, cColor_Green192, pHero->Moves(), 250);
	else ComposeProgressBar(false, gApp.Surface(), rc, cColor_Gray192, 0, 250);
	if (!IsEnabled()) gApp.Surface().Darken25Rect(rc);
}
//
iHeroTB::iManaGaugeBtn::iManaGaugeBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid)
: iButton(pViewMgr, pCmdHandler, rect, uid, Visible|Enabled) {}

void iHeroTB::iManaGaugeBtn::OnCompose()
{
	iRect rc = GetScrRect();
	iHero* pHero = gGame.Map().CurHero();
	if (pHero) ComposeProgressBar(false, gApp.Surface(), rc, RGB16(32,128,255), pHero->ManaPts(), 100);
	else ComposeProgressBar(false, gApp.Surface(), rc, cColor_Gray192, 0, 100);
	if (!IsEnabled()) gApp.Surface().Darken25Rect(rc);
}
//
iHeroTB::iHeroTB(iViewMgr* pViewMgr, const iRect& rect)
: iView(pViewMgr, rect, GENERIC_VIEWPORT, 0, Enabled|Visible)
{
	AddChild(new iPortBtn(pViewMgr, this, iRect(0,0,32,20), VPUID_BTN_OBJICN));
	AddChild(new iMoveGaugeBtn(pViewMgr, this, iRect(32+1,0,62,3), VPUID_PRG_OBJACT));
	AddChild(new iManaGaugeBtn(pViewMgr, this, iRect(32+1+63,0,62,3), VPUID_PRG_OBJACT));

	AddChild(new iIconButton(pViewMgr, this, iRect(32+1+21*0,4,20,16), PDGG_BTN_HEROPROPS, 110));
	AddChild(new iIconButton(pViewMgr, this, iRect(32+1+21*1,4,20,16), PDGG_BTN_CENTER, 111));
	AddChild(new iIconButton(pViewMgr, this, iRect(32+1+21*2,4,20,16), PDGG_BTN_NEXTHERO, 112));
	AddChild(new iIconButton(pViewMgr, this, iRect(32+1+21*3,4,20,16), PDGG_BTN_HORSE, 113));
	AddChild(new iIconButton(pViewMgr, this, iRect(32+1+21*4,4,20,16), PDGG_BTN_CASTSPELL, 114));
	AddChild(new iIconButton(pViewMgr, this, iRect(32+1+21*5,4,20,16), PDGG_BTN_DIG, 115));
}
//
void iHeroTB::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (uid == VPUID_BTN_OBJICN) {
		// Hero portrait
		iPoint pp = pView->GetScrPos();
		m_pMgr->TrackPopup(new iHeroListPopup(m_pMgr), pp, iRect(0,0,320,240),AlignBottomLeft);
	} else if (uid == 110) {
		// Hero props
		gGame.ShowView(iChildGameView::CV_HERO);
	} else if (uid == 111) {
		// Center current hero
		gGame.Map().CurHero()->OnSelect();
		DumpArtifacts(gGame.Map().CurHero());
	} else if (uid == 112) {
		// Next hero
		gGame.Map().CurPlayer()->NextHero();
	} else if (uid == 113) {
		// Move hero
		gGame.Map().CurHero()->SetPath();
	} else if (uid == 114) {
		// Cast spell
		iDlg_CastSpell csDlg(m_pMgr, gGame.Map().ActPlayer()->CurHero(), SPT_MASK_OVERLAND);
		if (csDlg.DoModal() == DRC_YES) {
			gGame.Map().ActPlayer()->CurHero()->CastSpell((MAGIC_SPELL)csDlg.SelSpell());
		}
	} else if (uid == 115) {
		//gGame.Map().ActPlayer()->CurHero()->Army().AddGroup(CREAT_BLACK_DRAGON, 32);
		//return;
		iHero* pHero = gGame.Map().ActPlayer()->CurHero();
		check(pHero);
		pHero->Dig();
		//pHero->ReceiveExperience(500000);

		//gGame.Map().ActPlayer()->CurHero()->Army().AddGroup(CREAT_BLACK_DRAGON, 32);
		// Dig
		// iTextDlg tdlg(m_pMgr, _T(""), gTextMgr[TRID_MSG_NOTIMPL], gGame.Map().ActPlayer()->PlayerId());
		// tdlg.DoModal();
		// mem_report();
	}
}
//
void iHeroTB::OnCompose()
{
	iRect rect = GetScrRect();
}
//////////////////////////////////////////////////////////////////////////
iCnstTB::iPortBtn::iPortBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid)
: iButton(pViewMgr, pCmdHandler, rect, uid, Visible|Enabled) {}
//
void iCnstTB::iPortBtn::OnCompose()
{
	iRect rc = GetScrRect();
	iCastle* pCastle = gGame.Map().CurCastle();
	SpriteId sid;
	if (pCastle) sid = PDGG_CTL_SICONS + (pCastle->Proto()->Size()-1)*CTLT_COUNT + pCastle->Proto()->Type();
	else sid = PDGG_UNK_CASTLE;
	gGfxMgr.Blit(sid,gApp.Surface(), rc);
	if (pCastle && !pCastle->CanBuild()) {
		gGfxMgr.Blit(PDGG_ICN_BUILT,gApp.Surface(), iPoint(rc.x+23,rc.y+11));
	}
	ButtonFrame(gApp.Surface(),rc,GetButtonState());
	if (!IsEnabled()) gApp.Surface().Darken25Rect(rc);
}
//
iCnstTB::iCnstTB(iViewMgr* pViewMgr, const iRect& rect)
: iView(pViewMgr, rect, GENERIC_VIEWPORT, 0, Enabled|Visible)
{
	AddChild(new iPortBtn(pViewMgr, this, iRect(0,0,32,20), VPUID_BTN_OBJICN));
	AddChild(new iTBSplitter(pViewMgr, iRect(33,0,rect.w-33,3)));
	AddChild(new iIconButton(pViewMgr, this, iRect(32+1+21*0,4,20,16), PDGG_BTN_CASTLEPROPS, 110));
	AddChild(new iIconButton(pViewMgr, this, iRect(32+1+21*1,4,20,16), PDGG_BTN_CENTER, 111));
	AddChild(new iIconButton(pViewMgr, this, iRect(32+1+21*2,4,20,16), PDGG_BTN_NEXTCASTLE, 112));
}
//
void iCnstTB::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (uid == VPUID_BTN_OBJICN) {
		iPoint pp = pView->GetScrPos();
		m_pMgr->TrackPopup(new iCastleListPopup(m_pMgr), pp, iRect(0,0,320,240),AlignBottomLeft);
		/*
		if (cmd == CCI_BTNCLICK) gGame.OnCastleChanged(gGame.Map().CurCastle());
		else if (cmd == CCI_BTNDBLCLICK) gGame.ShowView(iGame::CV_CASTLE);
		*/
	} else if (uid == 110) {
		gGame.ShowView(iChildGameView::CV_CASTLE);
	} else if (uid == 111) {
		gGame.Map().CurCastle()->OnSelect();
	} else if (uid == 112) {
		gGame.Map().CurPlayer()->NextCastle();
	}
}
//
void iCnstTB::OnCompose()
{
	iRect rect = GetScrRect();
	//ComposeProgressBar(false, gApp.Surface(), iRect(rect.x+33,rect.y,rect.w-33,4), cColor_Gray192, 0, 250);
}
//////////////////////////////////////////////////////////////////////////
iMainToolBar::iMainToolBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect)
: iView(pViewMgr, rect, GENERIC_VIEWPORT, 0, Visible|Enabled)
{
	sint32 xpos = 0;
	AddChild(m_pHeroTB = new iHeroTB(pViewMgr,iRect(xpos,1,158,20)));
	xpos += 158+1;
	AddChild(new iTBSplitter(pViewMgr, iRect(xpos,1,7,20)));
	xpos += 7+1;
	AddChild(m_pCnstTB = new iCnstTB(pViewMgr,iRect(xpos,1,95,20)));
	xpos += 95+1;
	AddChild(new iTBSplitter(pViewMgr, iRect(xpos,1,7,20)));
	xpos += 7+1;
	AddChild(new iIconButton(pViewMgr, this, iRect(GetSize().w-49,1,24,20),PDGG_BTN_MINIMAP,VPUID_BTN_MINIMAP));
	AddChild(new iIconButton(pViewMgr, pCmdHandler, iRect(GetSize().w-24,1,24,20),PDGG_BTN_ENDTURN,VPUID_BTN_NTURN));
}

void iMainToolBar::EnableHeroBar(bool bEnable)
{
	m_pHeroTB->SetEnabled(bEnable);
}

void iMainToolBar::EnableCnstBar(bool bEnable)
{
	m_pCnstTB->SetEnabled(bEnable);
}

void iMainToolBar::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (uid == VPUID_MODE_SWITCH) {
		// Overland view mode switch control
		check(pView->GetClassId() == TABBED_SWITCH);
		if (cmd == CCI_BTNCLICK){
			//if (param == NM_HERO) gGame.Map().CurPlayer()->NextHero();
			//else if (param == NM_CNST) gGame.Map().CurPlayer()->NextCastle();
		} else if (cmd == CCI_TABCHANGED) {
			//if (param == NM_HERO && gGame.Map().CurHero()) gGame.Map().CurHero()->OnSelect();
			//else if (param == NM_CNST && gGame.Map().CurPlayer()->CurCastle()) gGame.Map().CurPlayer()->CurCastle()->OnSelect();
			//SetNavMode((NavMode)param);
		}
	} else if (uid == VPUID_BTN_MINIMAP) {
		gGame.ShowView(iChildGameView::CV_MINIMAP);
	}
}

void iMainToolBar::OnCompose()
{
	iRect rc(GetScrRect());
	gApp.Surface().FillRect(rc,0);
}

/*
 *	Info bar
 */
iInfoBar::iInfoBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect)
: iView(pViewMgr, rect, GENERIC_VIEWPORT, 0, Visible|Enabled), m_mode(Minerals)
{
	AddChild(new iIconButton(pViewMgr, pCmdHandler, iRect(GetSize().w-18,0,18,15),PDGG_BTN_MAINMENU,VPUID_BTN_STATE));
}

void iInfoBar::OnCompose()
{
	iPlayer* pPlayer = gGame.Map().ActPlayer();
	iRect rect = GetScrRect();

	rect.w-=18;
	gGfxMgr.BlitTile(PDGG_BKTILE, gApp.Surface(),iRect(rect.x,rect.y, rect.w,rect.h));
	gApp.Surface().Darken25Rect(iRect(rect.x,rect.y2(),rect.w,1));
	gApp.Surface().Darken25Rect(iRect(rect.x2(),rect.y,1,rect.h));

	if (m_mode == Minerals) {
		iPoint tpos(1,1);
		iTextComposer::FontConfig fc(iTextComposer::FS_MEDIUM, iDibFont::ComposeProps(RGB16(255,220,192),RGB16(48,32,0)));
		for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) {
			gGfxMgr.Blit(PDGG_RES_ICONS+xx,gApp.Surface(),tpos-gGfxMgr.Anchor(PDGG_RES_ICONS+xx));
			tpos.x+=18;
			if (pPlayer) gTextComposer.TextOut(fc, gApp.Surface(),tpos,FormatNumber(pPlayer->Mineral((MINERAL_TYPE)xx)).CStr());
			tpos.x+=21;
			if (xx==MINERAL_GOLD) tpos.x += 20;
		}
	} else if (m_mode == Army) {
		iHero* pHero = pPlayer->CurHero();
		if (pHero) {
			iPoint tpos(1,1);
			iTextComposer::FontConfig fc(iTextComposer::FS_MEDIUM, iDibFont::ComposeProps(RGB16(255,220,192),RGB16(48,32,0)));
			for (uint32 xx=0; xx<7; ++xx) {
				if (pHero->Army().At(xx).IsValid()) {
					gGfxMgr.Blit(PDGG_CREAT_ICONS+pHero->Army().At(xx).Type(), gApp.Surface(), tpos);
					//gApp.Surface().FillRect(iRect(tpos.x,tpos.y,14,12), cColor_Black);
					tpos.x += 15;
					gTextComposer.TextOut(fc, gApp.Surface(),tpos,FormatNumber(pHero->Army().At(xx).Count()).CStr());
					tpos.x += 26;
				} else {
					tpos.x += 41;
				}
				if (xx < 6){
					gApp.Surface().VLine(tpos, tpos.y+12, cColor_Black, 64);
					tpos.x += 2;
				}
			}
		}
	} else if (m_mode == System) {
		// Game date
		uint32 days = gGame.Map().m_CurDay-1;
		gTextComposer.TextOut(gApp.Surface(), iPoint(3,1), iFormat(_T("#S0#FDB9%s: #S1#FFDB%d  #S0#FDB9%s: #S1#FFDB%d  #S0#FDB9%s: #S1#FFDB%d"),gTextMgr[TRID_MONTH], days/28+1,gTextMgr[TRID_WEEK], (days%28)/7+1,gTextMgr[TRID_DAY],days%7+1));

		// System time
		iTextComposer::FontConfig fc(iTextComposer::FS_MEDIUM, iDibFont::ComposeProps(RGB16(255,220,192),RGB16(48,32,0)));
		SYSTEMTIME st;
		GetLocalTime(&st);
		iPoint tp(230,1);
		gTextComposer.TextOut(fc, gApp.Surface(), tp, iFormat(_T("%02d : %02d"),st.wHour, st.wMinute));

		// Battery state
		uint32 bval = 0;
#ifdef UNDER_CE
		SYSTEM_POWER_STATUS_EX2 pwrstat;
		memset(&pwrstat,0, sizeof(pwrstat));
		if (GetSystemPowerStatusEx2(&pwrstat,sizeof(pwrstat),FALSE) && pwrstat.BatteryLifePercent != 0xFF) {
			bval = pwrstat.BatteryLifePercent;
		}
#endif // UNDER_CE
		iPoint op(260,1);
		gGfxMgr.Blit(PDGG_ICN_BATTERY, gApp.Surface(), op);
		op.x+=12;
		if (bval) gTextComposer.TextOut(fc, gApp.Surface(), op, iFormat(_T("%d%%"),bval));
		else gTextComposer.TextOut(fc, gApp.Surface(), op, _T("???%"));
	}
}

void iInfoBar::OnMouseDown(const iPoint& pos)
{
	if (m_mode == Minerals) m_mode = Army;
	else if (m_mode == Army) m_mode = System;
	else if (m_mode == System) m_mode = Minerals;
	Invalidate();
}

/*
 *	Opponent turn information bar
 */
iOpTurnBar::iOpTurnBar(iViewMgr* pViewMgr, const iRect& rect) : iView(pViewMgr,rect,GENERIC_VIEWPORT,0,false)
{ }

void iOpTurnBar::OnCompose()
{
	uint8 pid = gGame.Map().CurPlayer()->PlayerId();
	gGfxMgr.BlitTile(PDGG_PLTILES + pid, gApp.Surface() ,m_Rect);
	ButtonFrame(gApp.Surface(), m_Rect, 0);
	gTextComposer.TextOut(gApp.Surface(),m_Rect.point(), iStringT(_T("#S2"))+PLAYER_TEXT_COLORS[pid]+gTextMgr[TRID_PLAYER_RED+pid] + _T(" ") + gTextMgr[TRID_PLAYER],m_Rect,AlignCenter);
}


