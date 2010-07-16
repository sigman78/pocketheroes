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

#ifndef _HMM_GAME_BATTLE_VIEW_H_
#define _HMM_GAME_BATTLE_VIEW_H_

//////////////////////////////////////////////////////////////////////////
class iCreatInfoPopup : public iFramePopupView
{
public:
	iCreatInfoPopup(iViewMgr* pViewMgr, PLAYER_ID pid, const iBattleGroup* pGroup);
	void DoCompose(const iRect& clRect);
	iSize ClientSize();

private:
	const iBattleGroup*	m_pGroup;
};


//////////////////////////////////////////////////////////////////////////
enum BattleNavMode {
	BNM_MELEE = 0,
	BNM_SHOOT,
	BNM_INFO,
	BNM_COUNT
};

//////////////////////////////////////////////////////////////////////////
struct iBatObstEntry {
	iBatObstEntry(uint16 _obstId, const iPoint& _cell) : obstId(_obstId), cell(_cell) {}
	uint16	obstId;
	iPoint	cell;
};
typedef iSimpleArray<iBatObstEntry> iBatObstList;

//////////////////////////////////////////////////////////////////////////
class iBattleToolBar;
class iCastSpellToolBar;
class iAutoBattleToolBar;
class iBattleView : public iChildGameView
{
public:
	iBattleView();
	~iBattleView();

	void BeginBattle(iBattleWrapper* pBattle, SURF_TYPE st);
	void Start();
	void EndBattle(iBattleResult br);

	bool OnKeyDown(sint32 key);
	bool OnKeyUp(sint32 key);

	void OnMouseDown(const iPoint& pos);
	void OnMouseUp(const iPoint& pos);
	void OnMouseTrack(const iPoint& pos);
	
	bool Process(fix32 t);
	void PrepareSurface();
	void OnCompose();

	void AddLogEvent(const iStringT& msg);
	void AddCellEvent(const iStringT& msg, const iPoint& pos);

private:
	void BeginAutobattle();
	void EndAutobattle();
	inline bool IsAutobattle() const { return m_pAutoBattleToolBar != NULL; }
	bool BeginSpellTrack(iCombatSpell* pSpell);
	void EndSpellTrack(const iPoint& cell);
	inline bool SpellTracking() const { return m_pCastSpellToolBar != NULL; }

private:
	bool OnGroupChanged();
	void BeginAni();
	void EndAni();
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

private:
	iCastSpellToolBar*	m_pCastSpellToolBar;
	iBattleToolBar*		m_pToolBar;
	iAutoBattleToolBar*	m_pAutoBattleToolBar;
	BattleNavMode		m_battleMode;
	iBattleWrapper*		m_pBattle;
	SURF_TYPE			m_surfType;
	iPoint				m_trackPos;
	iPoint				m_trackCell;
	iDib				m_dibSurf;
	iDib				m_dibTurnGlyph;
	bool				m_bForceInfo;
	bool				m_bHumanTurn;
	iStringT			m_toolTip;

	// Obstacles
	iBatObstList	m_obstacles;

	// Events
	iEventList				m_cellEvents;
	iSimpleArray<iStringT>	m_log;

	// Creature Info popup view
	iCreatInfoPopup*	m_pCreatInfoPopup;

	// Animation
	fix32			m_actTimer;
	bool			m_bAni;

	// Melee
	typedef const iBattleGroup::iMeleeEntry* iMeleeEntryPtr;
	iMeleeEntryPtr	m_pMeleeTrack;
	sint32			m_meleeDir;

	// Shoot
	typedef const iBattleGroup::iShootEntry* iShootEntryPtr;
	iShootEntryPtr m_pShootTrack;

	// Spells
	iSimpleArray<iPoint>	m_spellTargets;
};

/*
 *	Battle Toolbars
 */
class iBattleToolBar : public iView
{
public:
	enum StateFlags {
		EnemyTurn	= 0x01,
		Acting		= 0x02,
		CanMelee	= 0x04,
		CanShoot	= 0x08,
		CanInfo		= 0x10,
		CanCast		= 0x20,
		CanWait		= 0x40
	};
public:
	iBattleToolBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect);
	void OnCompose();
	void EnableControls(uint32 flags);

	iIconButton*	m_pBtnWait;
	iIconButton*	m_pBtnDefend;
	iIconButton*	m_pBtnCastSpell;
	iIconButton*	m_pBtnAutoBattle;
	iIconButton*	m_pBtnMsgLog;
	iIconButton*	m_pBtnSettings;
	iHeroPortBtn*	m_pBtnAssaulter;
	iHeroPortBtn*	m_pBtnDefender;
	iBarTabSwitch*	m_pModeSwitch;
};

/*
 * Spell toolbar
 */
class iCastSpellToolBar : public iView
{
public:
	iCastSpellToolBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, iCombatSpell* pSpell);
	void OnCompose();

	//
	inline iCombatSpell* Spell() { return m_pSpell; }

private:
	iCombatSpell*			m_pSpell;
};

/*
 * Autobattle toolbar
 */
class iAutoBattleToolBar : public iView
{
public:
	iAutoBattleToolBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect);
	void OnCompose();
};

#endif //_HMM_GAME_BATTLE_VIEW_H_

