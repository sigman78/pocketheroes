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
#include "BattleView.h"
#include "MinimapView.h"
#include "CastleView.h"
#include "HeroView.h"
#include "MeetView.h"
#include "OverlandView.h"
#include "Credits.h"
#include "MenuView.h"
#include "game.h"
#include "AutoBattle.h"
#include "InteractBattle.h"
#include "Dlg_HallOfFame.h"

/*
 *	BlackBack
 */
iBlackBackView::iBlackBackView() : iTopmostView(&gApp.ViewMgr())
{
}

void iBlackBackView::OnCompose()
{	
	iRect rect = GetScrRect();
	gGfxMgr.BlitTile(PDGG_BKTILE, gApp.Surface(), rect);
	gApp.Surface().Darken50Rect(rect);
}


/*
 *	Generic GameView
 */
iChildGameView::iChildGameView(bool bEternal, CHILD_VIEW parentView) : iTopmostView(&gApp.ViewMgr()), m_bEternal(bEternal), m_parentView(parentView) { }

/*
 *	Game 
 */
iGame::iGame()
: m_bInited(false)
, m_bStarted(false)
, m_tActView(iChildGameView::CV_UNDEFINED)
, m_pMainView(NULL)
, m_pBattle(NULL)
, m_bGoToMainMenu(false)
, m_hmChannel(-1)
, m_hmSound(0xFFFF)
{ memset(m_pChildView,0,sizeof(m_pChildView)); }

iGame::~iGame()
{
	Cleanup();
}

bool iGame::Init(const iStringT& fname)
{
	check(!m_bInited);

	// Load items
	if (!LoadResources(m_itemMgr) ) return false;


	// check last session
	if (iFile::Exists(gSavePath + _T("lastses.phs"))) {
		iFilePtr pSaveFile = OpenWin32File(gSavePath + _T("lastses.phs"));
		check(pSaveFile);
		if (pSaveFile) {
			uint32 fourcc; pSaveFile->Read(&fourcc,sizeof(fourcc));
			iMapInfo mapInfo;
			mapInfo.m_FileName = gSavePath + _T("lastses.phs");
			mapInfo.m_bNewGame = false;
			mapInfo.ReadMapInfo(pSaveFile.get());
			mapInfo.ReorderPlayers();
			if (StartNewGame(mapInfo, false)) {
				pSaveFile.reset();
				iFile::Delete(gSavePath + _T("lastses.phs"));
				return m_bInited = true;
			}
			pSaveFile.reset();
			iFile::Delete(gSavePath + _T("lastses.phs"));
		}
	}

	// Init main menu
	ShowView(iChildGameView::CV_MENU);
	((iMenuView*)m_pChildView[iChildGameView::CV_MENU])->Start();

	//

	return m_bInited = true;
}

void iGame::Quit()
{
	gApp.Exit(0);
}

void iGame::MainMenu()
{
	m_bGoToMainMenu = true;
}

void ShowProgressReport( uint32 curProg, bool initial = true );

bool iGame::StartNewGame(const iMapInfo& mapInfo, bool bNewGame)
{
	ShowProgressReport( 0, false );
	// Load map info
	iMapInfo _mapInfo;
	iFilePtr pFile( OpenWin32File( mapInfo.m_FileName ));
	check(pFile);
	ShowProgressReport( 10, false );
	uint32 fourcc; pFile->Read(&fourcc,sizeof(fourcc));
	if (fourcc != GMAP_FILE_HDR_KEY || !_mapInfo.ReadMapInfo(pFile.get())) {
		return false;
	}

	ShowProgressReport( 25, false );
	// Show Main view
	ShowView(iChildGameView::CV_OVERLAND);

	ShowProgressReport( 30, false );
	// Load map
	m_Map.LoadFromFile(mapInfo, pFile.get());

	ShowProgressReport( 50, false );
	// Init Pass map
	m_Map.InitPassMap();

	ShowProgressReport( 60, false );
	// Setup ultimate artifact position
	m_Map.InitUltimateArt();

	ShowProgressReport( 75, false );
	// Init Sound map
	m_soundMap.Init(&m_Map);

	ShowProgressReport( 85, false );
	// Start the game
	m_Map.StartGame(mapInfo.m_curPlayerId, !bNewGame);

	ShowProgressReport( 100, false );

	return true;
}

void iGame::ExitGame(bool bChangeView)
{
	m_soundMap.Cleanup();
	m_Map.CleanupGameWorld();
	m_itemMgr.OnGameEnd();
	if (bChangeView) {
		ShowView(iChildGameView::CV_MENU);
		((iMenuView*)m_pChildView[iChildGameView::CV_MENU])->Start();
	}
}

void iGame::Cleanup()
{
	for (uint32 xx=0; xx<iChildGameView::CV_COUNT; ++xx){
		if (m_pChildView[xx]) {
			delete m_pChildView[xx];
			m_pChildView[xx] = NULL;
		}
	}
	m_tActView = iChildGameView::CV_UNDEFINED;

	if (m_bInited) {
		m_itemMgr.Cleanup();
		m_bInited = false;
	}
}


void iGame::OnSuspend()
{
	//
	m_soundMap.ResetEnvSounds();
}

void iGame::OnResume()
{
	//
	if (m_Map.CurHero()) SoundMap().SetupEnvSounds(m_Map.CurHero()->Pos());
}


// Child views
void iGame::ShowView(iChildGameView::CHILD_VIEW cv)
{
	check (cv != iChildGameView::CV_UNDEFINED);
	if (cv == m_tActView) return;

	// Delete/Hide old active view
	if (m_tActView != iChildGameView::CV_UNDEFINED) {
		m_pChildView[m_tActView]->OnActivate(false);
		m_pChildView[m_tActView]->SetVisible(false);
		gApp.ViewMgr().SetCurView(NULL);
		if (cv == iChildGameView::CV_HERO && m_tActView != iChildGameView::CV_OVERLAND) {
			// do not delete parent view
		} else {
			if (m_tActView == iChildGameView::CV_MINIMAP) {
				m_pMainView->Composer().CenterView(((iMinimapView*)m_pChildView[m_tActView])->GetCenterCell());
			}
			if (!m_pChildView[m_tActView]->Eternal() || cv == iChildGameView::CV_MENU) {
				delete m_pChildView[m_tActView];
				m_pChildView[m_tActView] = NULL;
				if (m_tActView == iChildGameView::CV_OVERLAND) m_pMainView = NULL;
			}
		}
	}

	// Create/Show new active view
	switch (cv) {
	case iChildGameView::CV_MENU:
		check(!m_pChildView[cv]);
		m_pChildView[cv] = new iMenuView();
		break;
	case iChildGameView::CV_OVERLAND:
		if (!m_pMainView) {
			m_pMainView = new iMainView();
			m_pChildView[iChildGameView::CV_OVERLAND] = m_pMainView;
		} else m_pMainView->SetVisible(true);
		break;
	case iChildGameView::CV_BATTLE:
		check(!m_pChildView[cv]);
		m_pChildView[cv] = new iBattleView();
		break;
	case iChildGameView::CV_CASTLE:
		if (m_tActView != iChildGameView::CV_HERO || !m_pChildView[cv]) {
			check(!m_pChildView[cv]);
			m_pChildView[cv] = new iCastleView();
		} else m_pChildView[cv]->SetVisible(true);
		break;
	case iChildGameView::CV_HERO:
		check(!m_pChildView[cv]);
		m_pChildView[cv] = new iHeroView(m_tActView);
		break;
	case iChildGameView::CV_MEET:
		if (m_tActView != iChildGameView::CV_HERO || !m_pChildView[cv]) {
			check(!m_pChildView[cv]);
			m_pChildView[cv] = new iMeetView();
		} else m_pChildView[cv]->SetVisible(true);
		break;
	case iChildGameView::CV_MINIMAP:
		check(!m_pChildView[cv]);
		iMinimapView* pMMView = new iMinimapView();
		pMMView->SetCenterCell(m_pMainView->Composer().GetCenterCell());
		m_pChildView[cv] = pMMView;
		break;
	};

	m_pChildView[cv]->OnActivate(true);
	gApp.ViewMgr().SetCurView(m_pChildView[cv]);
	m_tActView = cv;
}

void iGame::HideView(iChildGameView::CHILD_VIEW cv)
{
	check(cv != iChildGameView::CV_UNDEFINED);
	check(cv != iChildGameView::CV_OVERLAND);
	check(m_pChildView[cv]);

	ShowView(m_pChildView[cv]->ParentView());
}

// Game process
void iGame::BeginBattle(const iBattleInfo& bi)
{
	check(!m_pBattle);

	// Assaulter cannot be neutral
	check(bi.m_pAssaulter->Owner() != PID_NEUTRAL);
	iPlayer* pAssaulter = m_Map.FindPlayer(bi.m_pAssaulter->Owner());
	iPlayer* pDefender = (bi.m_pDefender->Owner()==PID_NEUTRAL)?NULL:m_Map.FindPlayer(bi.m_pDefender->Owner());
	// Assaulter and Defender can't be owned by one player or side
	check(pAssaulter && pAssaulter != pDefender);

	// Reset env sounds
	if (pAssaulter->PlayerType() == PT_HUMAN) m_soundMap.ResetEnvSounds();

	// If one of side is controlled by human, the battle must be interactive
	bool bInt = pAssaulter->PlayerType() == PT_HUMAN || (pDefender && pDefender->PlayerType() == PT_HUMAN);
	if (bInt) {
		// Computer player attack the not active human player, so, change the active player first
		if (pDefender && pDefender->PlayerType() == PT_HUMAN && pAssaulter->PlayerType() != PT_HUMAN && pDefender != m_Map.ActPlayer()) {
			m_Map.SetNewActor(pDefender, true);
		}

		OnHeroStopMoving(gGame.Map().CurHero());
		if (gSettings.GetEntryValue(CET_QUICKCOMBAT)) {
			// Autobattle with result
			check(gGame.Map().CurHero() == bi.m_pAssaulter->SpellCaster());
			m_pBattle = new iAutoBattle(true);
		} else {
			// Interactive battle
			m_pBattle = new iInteractBattle();
		}
	} else {
		// Auto battle
		m_pBattle = new iAutoBattle();
	}

	m_pBattle->BeginBattle(bi);
}

void iGame::EndBattle()
{
	check(m_pBattle);
	delete m_pBattle;
	m_pBattle = NULL;
}

void iGame::MeetHeroes(iHero* pHero1, iHero* pHero2, bool bAct)
{
	if (bAct){
		ShowView(iChildGameView::CV_MEET);
		((iMeetView*)m_pChildView[iChildGameView::CV_MEET])->SetHeroes(pHero1, pHero2);
	}
}

sint32 iGame::Process(fix32 t)
{
	if (m_bGoToMainMenu) {
		m_bGoToMainMenu = false;
		ExitGame(true);
	} else if (ActView() && ActView()->Process(t) && m_tActView == iChildGameView::CV_OVERLAND ) {
		m_Map.Process(t);
	}

	return 0;
}

void iGame::OnKeyDown(sint32 key)
{
}

void iGame::OnKeyUp(sint32 key)
{
	if (key == gSettings.ActionKey(BAT_MINIMIZE_APP)) {
		gApp.Minimize();
	} else if (key == gSettings.ActionKey(BAT_MAKE_SCREENSHOT)) {
		iStringT fname = gRootPath + _T("screenshot.bmp");
		SaveDibBitmap16(gApp.Surface(), fname);
		AddMsg(iStringT(_T("#F4B4")) + gTextMgr[TRID_MSG_SCREENSHOT_SAVED]);
	}
}


// Msgs
void iGame::AddMsg(const iStringT& msg)
{
	if (m_pMainView) m_pMainView->AddMsg(msg);
}

void iGame::AddCellMsg(const iStringT& msg, const iPoint& pos)
{
	if (m_pMainView) m_pMainView->AddCellMsg(msg, pos);
}


// Animation
void iGame::OnDisapObject(SpriteId sid, const iPoint& pos, const iPoint& offset)
{
	if (!gGame.Map().ActPlayer()->FogMap().IsHidden(pos)) {
		m_pMainView->Composer().SetAniObj(new iAniObj_FadeOut(&m_pMainView->Composer(), sid,pos));
	}
}

// Game notifications
void iGame::OnVictory()
{
	// Show victory dialog
	iTextDlg tdlg(&gApp.ViewMgr(), gTextMgr[TRID_VICTORY], gTextMgr[TRID_MSG_VICTORY],PID_NEUTRAL);
	tdlg.DoModal();

	// Show Hall of Fame
	SYSTEMTIME st;
	FILETIME ft;
	GetLocalTime(&st);
	SystemTimeToFileTime(&st, &ft);
	uint32 ut = (uint32)((ft.dwLowDateTime + ((LONGLONG)(ft.dwHighDateTime)<<32)) / 600000000);
	iHighScore::iEntry entry(m_Map.MapName(), ut, m_Map.m_CurDay, m_Map.CalcGameScore());
	iDlg_HallOfFame dlg(&gApp.ViewMgr(), gRootPath + _T("PalmHeroes.hsc"), entry);
	dlg.DoModal();

	MainMenu();
}

void iGame::OnDefeat(bool bExitGame)
{
	iTextDlg tdlg(&gApp.ViewMgr(), gTextMgr[TRID_DEFEAT], gTextMgr[TRID_MSG_DEFEAT],PID_NEUTRAL);
	tdlg.DoModal();
	if (bExitGame) MainMenu();
}

void iGame::OnActorChanged(iPlayer* pNewActor, bool bAttack)
{
	m_pChildView[m_tActView]->SetVisible(false);
	iBlackBackView bbv;
	gApp.ViewMgr().SetCurView(&bbv);
	iStringT msg;
	if (bAttack) msg = iFormat(gTextMgr[TRID_MSG_PLAYER_UNDER_ATTACK], gTextMgr[TRID_PLAYER_RED+pNewActor->PlayerId()]);
	else msg = iFormat(gTextMgr[TRID_MSG_PLAYER_TURN], gTextMgr[TRID_PLAYER_RED+pNewActor->PlayerId()]);
	iIconDlg dlg(&gApp.ViewMgr(), iStringT(), msg, PDGG_EMBLEMS+pNewActor->PlayerId(), pNewActor->PlayerId());
	dlg.DoModal();
	m_pChildView[m_tActView]->SetVisible(true);
	gApp.ViewMgr().SetCurView(m_pChildView[m_tActView]);
}

void iGame::OnPlayerChanged(iPlayer* pNewPlayer, bool bAct)
{
	m_soundMap.ResetEnvSounds();
	m_pMainView->OnPlayerChanged(pNewPlayer,bAct);
}

void iGame::OnPlayerVanquished(PLAYER_ID pid)
{
	iTextDlg tdlg(&gApp.ViewMgr(), _T(""), iFormat(gTextMgr[TRID_MSG_PLAYER_VANQUISHED], gTextMgr[TRID_PLAYER_RED+pid]),pid);
	tdlg.DoModal();
}

void iGame::OnAddCastle(iCastle* pCastle)
{
	m_pMainView->OnCastleListChanged();
}

void iGame::OnDelCastle(iCastle* pCastle)
{
	m_pMainView->OnCastleListChanged();
}

void iGame::OnCastleChanged(iCastle *pCastle)
{
	if (!gGame.Map().ActPlayer()->FogMap().IsHidden(pCastle->Pos())) {
		m_pMainView->Composer().CenterView(pCastle->Pos(),iPoint());
	}

	if (m_tActView == iChildGameView::CV_CASTLE) {
		if (iCastle* pCurCastle = gGame.Map().ActPlayer()->CurCastle()) ((iCastleView*)(m_pChildView[iChildGameView::CV_CASTLE]))->SetCastle(pCurCastle);
	}
}

void iGame::OnAddHero(iHero* pHero)
{
	m_pMainView->OnHeroListChanged();
}

void iGame::OnDelHero(iHero* pHero)
{
	if (m_tActView == iChildGameView::CV_HERO) {
		if (iHero* pCurHero = gGame.Map().ActPlayer()->CurHero()) ((iHeroView*)(m_pChildView[iChildGameView::CV_HERO]))->SetHero(pCurHero);
		else HideView(iChildGameView::CV_HERO);
	}

	m_pMainView->OnHeroListChanged();
	if (!gGame.Map().ActPlayer()->FogMap().IsHidden(pHero->Pos()) && m_tActView == iChildGameView::CV_OVERLAND) {
		m_pMainView->Composer().SetAniObj(new iAniObj_FadeOut(&m_pMainView->Composer(), GetHeroSprite(pHero),pHero->Pos()));
		if (gGame.Map().CurPlayer()->PlayerType() == PT_HUMAN) {
			if (!gGame.Map().ActPlayer()->CurHero()) m_soundMap.ResetEnvSounds();
			gSfxMgr.PlaySound(CSND_DEL_GUARD);
		}
	} 

	if (iAI_Player* pAIPlayer = DynamicCast<iAI_Player*>(gGame.Map().CurPlayer())) {
		pAIPlayer->OnHeroDead(pHero);
	}
}

void iGame::OnHeroChanged(iHero* pHero)
{
	if (!gGame.Map().ActPlayer()->FogMap().IsHidden(pHero->Pos()) && gGame.Map().CurPlayer()->PlayerId() == pHero->Owner()->PlayerId()) {
		m_pMainView->Composer().CenterView(pHero->Pos());
	}

	if (gGame.Map().ActPlayer()->PlayerId() == pHero->Owner()->PlayerId()) {
		m_soundMap.SetupEnvSounds(pHero->Pos());
	}

	if (m_tActView == iChildGameView::CV_HERO) {
		if (iHero* pCurHero = gGame.Map().ActPlayer()->CurHero()) ((iHeroView*)(m_pChildView[iChildGameView::CV_HERO]))->SetHero(pCurHero);
	}
}

void iGame::OnHeroLevel(iHero* pHero, uint8 level, const iNewLevelInfo& linfo)
{
}

void iGame::OnHeroMoveTo(iHero* pHero, const iPath::iStep& step)
{
	if (gGame.Map().ActPlayer()->PlayerId() == pHero->Owner()->PlayerId()) {
		if (step.m_action == iPath::MoveTo || step.m_action == iPath::Enter) {
			sint32 px = step.m_Pos.x; sint32 py = step.m_Pos.y;
			SURF_TYPE st = (SURF_TYPE)m_Map.GetAt(px,py).SurfNode(0);
			uint16 avatar = m_Map.GetAt(px,py).avatar;
			uint16 snd  = gSfxMgr.GetMovSound(st, m_Map.m_CoverMap.GetAt(px,py) != NULL  || (avatar != 0xFFFF && ( avatar & 0xFF00) == 0xFF00) );
			if (m_hmChannel == -1) {
				m_hmSound = snd;
				m_hmChannel = gSfxMgr.PlaySound(m_hmSound, true);
			} else {
				if (m_hmSound != snd) {
					m_hmSound = snd;
					gSfxMgr.ReplaceSound(m_hmChannel, m_hmSound);
				}
			}
			if (m_tActView == iChildGameView::CV_OVERLAND) {
				m_pMainView->EnableToolBar(false);
			}
		}
	}

	if (!gGame.Map().ActPlayer()->FogMap().IsHidden(pHero->Pos()) && !gGame.Map().ActPlayer()->FogMap().IsHidden(step.m_Pos)) {
		if (step.m_action == iPath::MoveTo || step.m_action == iPath::Enter) {
			if (m_pMainView->Composer().AniObj()) m_pMainView->Composer().RemoveAniObj();
			m_pMainView->Composer().SetAniObj(new iAniObj_MovHero(&m_pMainView->Composer(), pHero));
			check(pHero->Moving());
		} else pHero->Step();
	} else {
		pHero->Step();
	}
}


void iGame::OnHeroStopMoving(iHero* pHero)
{
	if (gGame.Map().ActPlayer()->PlayerId() == pHero->Owner()->PlayerId()) {
		if (m_hmChannel != -1) {
			gApp.SndPlayer().Stop(m_hmChannel);
			m_hmChannel = -1;
		}
		if (m_tActView == iChildGameView::CV_OVERLAND) {
			m_pMainView->EnableToolBar(true);
		}
	}
}

void iGame::OnHeroPosChanged(iHero* pHero, const iPoint& npos)
{
	if (pHero->Owner()->PlayerType() == PT_HUMAN) {
		m_soundMap.SetupEnvSounds(pHero->Pos());
	}
}

void iGame::OnHeroTeleport(iHero* pHero, const iPoint& src, const iPoint& dst)
{
	if (pHero->Owner()->PlayerType() == PT_HUMAN || (!gGame.Map().ActPlayer()->FogMap().IsHidden(src) && !gGame.Map().ActPlayer()->FogMap().IsHidden(dst))) {
		if (m_pMainView->Composer().AniObj()) m_pMainView->Composer().RemoveAniObj();
		m_pMainView->Composer().SetAniObj(new iAniObj_Teleport(&m_pMainView->Composer()));
		if (gGame.Map().ActPlayer()->PlayerId() == pHero->Owner()->PlayerId()) {
			gSfxMgr.PlaySound(CSND_TELEPORT_OUT);
		}
	}
}

void iGame::OnAttackHero(iHero* pHero1, iHero* pHero2)
{
}
