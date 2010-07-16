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

#ifndef _HMM_GAME_H_
#define _HMM_GAME_H_

/*
 *	BlackBack
 */
class iBlackBackView : public iTopmostView
{
public:
	iBlackBackView();
	void OnCompose();
};


/*
 *	Generic GameView
 */
class iChildGameView : public iTopmostView, public IViewCmdHandler
{
public:
	enum CHILD_VIEW {
		CV_UNDEFINED	= -1,
		CV_MENU			= 0,
		CV_OVERLAND,
		CV_BATTLE,
		CV_CASTLE,
		CV_HERO,
		CV_MEET,
		CV_MINIMAP,
		CV_COUNT
	};

public:
	iChildGameView(bool bEternal, CHILD_VIEW parentView);
	virtual ~iChildGameView() {}
	inline bool Eternal() const { return m_bEternal; }
	inline CHILD_VIEW ParentView() const { return m_parentView; }
	virtual bool Process(fix32 t) { return true; }
	virtual void OnActivate(bool bActivate) {}

private:
	CHILD_VIEW	m_parentView;
	bool		m_bEternal;
};

//////////////////////////////////////////////////////////////////////////
struct IMsgComposer
{
	virtual void AddMsg(const iStringT& text) =0;
};

/*
 *	Game 
 */
class iMainView;
class iGame : public IGame
{
public:
	// c-tor, d-tor
	iGame();
	~iGame();

	// Init/cleanup game resources
	bool Init(const iStringT& fname);
	void Cleanup();

	// Game control
	void Quit();
	void MainMenu();
	bool StartNewGame(const iMapInfo& mapInfo, bool bNewGame);
	void ExitGame(bool bChangeView);

	// inlines
	inline bool Inited() const { return m_bInited; }
	inline bool Started() const { return m_bStarted; }
	inline iGameWorld& Map() { return m_Map; }
	inline iItemMgr& ItemMgr() { return m_itemMgr; }
	inline iSoundMap& SoundMap() { return m_soundMap; }

	// Child views
	void ShowView(iChildGameView::CHILD_VIEW cv);
	void HideView(iChildGameView::CHILD_VIEW cv);
	inline iChildGameView* View(iChildGameView::CHILD_VIEW view) { return (view==iChildGameView::CV_UNDEFINED)?NULL:m_pChildView[view]; }
	inline iChildGameView* ActView() { return  View(m_tActView); }
	inline iMainView* MainView() { return m_pMainView; }

	// Game process
	void BeginBattle(const iBattleInfo& bi);
	void EndBattle();
	void MeetHeroes(iHero* pHero1, iHero* pHero2, bool bAct);
	sint32 Process(fix32 t);
	void OnKeyDown(sint32 key);
	void OnKeyUp(sint32 key);
	void OnSuspend();
	void OnResume();

	// Msgs
	void AddMsg(const iStringT& msg);
	void AddCellMsg(const iStringT& msg, const iPoint& pos);

	// Animation
	void OnDisapObject(SpriteId sid, const iPoint& pos, const iPoint& offset=iPoint(0,0));

	// Game notifications
	void OnVictory();
	void OnDefeat(bool bExitGame);
	void OnActorChanged(iPlayer* pNewActor, bool bAttack);
	void OnPlayerChanged(iPlayer* pNewPlayer, bool bAct);
	void OnPlayerVanquished(PLAYER_ID pid);
	
	void OnAddCastle(iCastle* pCastle);
	void OnDelCastle(iCastle* pCastle);
	void OnCastleChanged(iCastle *pCastle);

	void OnAddHero(iHero* pHero);
	void OnDelHero(iHero* pHero);
	void OnHeroChanged(iHero* pHero);
	void OnHeroLevel(iHero* pHero, uint8 level, const iNewLevelInfo& linfo);
	void OnHeroMoveTo(iHero* pHero, const iPath::iStep& step);
	void OnHeroStopMoving(iHero* pHero);
	void OnHeroPosChanged(iHero* pHero, const iPoint& npos);
	void OnHeroTeleport(iHero* pHero, const iPoint& src, const iPoint& dst);
	void OnAttackHero(iHero* pHero1, iHero* pHero2);

private:
	bool			m_bGoToMainMenu;
	iItemMgr		m_itemMgr;
	iGameWorld		m_Map;
	iSoundMap		m_soundMap;
	iBattleWrapper*	m_pBattle;
	bool			m_bInited;
	bool			m_bStarted;

	sint32			m_hmChannel;
	uint16			m_hmSound;
	iMainView*		m_pMainView;
	iChildGameView*	m_pChildView[iChildGameView::CV_COUNT];
	iChildGameView::CHILD_VIEW		m_tActView;
};


#endif //_HMM_GAME_H_

