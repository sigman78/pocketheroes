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

#ifndef _HMM_GAME_ARTIFICIAL_INTELLIGENCE_H_
#define _HMM_GAME_ARTIFICIAL_INTELLIGENCE_H_

/*
 *	Danger map
 */
class iDangerMap 
{
private:
	typedef iMapT<uint8>		iDMap;

public:
	typedef iSimpleArray<iHero*>	iActorList;

public:
	iDangerMap(PLAYER_ID owner);
	void UpdateDangerMap();

	inline uint32 CellDangerPower(sint32 x, sint32 y) const
	{ return (m_dangerMap.GetAt(x,y) == 0xFF)?0:m_enActors[m_dangerMap.GetAt(x,y)]->Army().ArmyPower(); }
	inline const iHero* CellDangerActor(sint32 x, sint32 y) const
	{ return (m_dangerMap.GetAt(x,y) == 0xFF)?NULL:m_enActors[m_dangerMap.GetAt(x,y)]; }
	inline bool IsDangerActor( iHero* pActor ) const { return m_danActors.Find(pActor) != iActorList::NOT_FOUND; }
	inline iSize GetSize() const { return iSize(m_dangerMap.GetWidth(), m_dangerMap.GetHeight());}

private:
	PLAYER_ID	m_owner;
	iDMap		m_dangerMap;
	iActorList	m_enActors;
	iActorList	m_danActors;
};

//////////////////////////////////////////////////////////////////////////
enum ARMY_TOSS_FLAGS {
	ATF_TYPE_POWER	= 0x01,
	ATF_TYPE_SPEED	= 0x02,
	ATF_NOT_EMPTY	= 0x04
};

// forward declarations
class iAI_Player;

/*
 *	AI Paymaster
 */
class iAI_Paymaster
{
public:
	enum RequestType {
		Construction,
		Creatures
	};

	struct ISubscriber {
		virtual bool PaySubs_Process(iMineralSet& receipt, RequestType reqType) =0;
	};
	
	struct iRequest {
		iRequest(const iMineralSet& _minerals, ISubscriber* _pSubs, RequestType _reqType) : minerals(_minerals), pSubs(_pSubs), reqType(_reqType) {}
		iMineralSet		minerals;
		ISubscriber*	pSubs;
		RequestType		reqType;
	};
public:
	iAI_Paymaster(const iMineralSet& minerals);
	bool RequestMinerals(const iMineralSet& request, sint32 priority, iMineralSet& receipt) const;
	void ReserveMinerals(const iMineralSet& request, sint32 priority, ISubscriber* pSubscriber, RequestType reqType);
	void ProcessSubscribers();
	const iMineralSet& Reserved() const { return m_resMinerals;}

private:
	iSortArray<iRequest>	m_reservations;
	iMineralSet				m_resMinerals;
	const iMineralSet&		m_minerals;
};


/*
 *	AI Castle
 */
class iAI_Castle : public iAI_Paymaster::ISubscriber, public iIListNode
{
public:
	enum Strategy {
		None,
		Defense,
		Might1,
		Might2,
		Magic1,
		Magic2,
		Resources,
		StratCount
	};

private:
	iAI_Player*	m_pPlayer;
	iCastle*	m_pCastle;
	uint32		m_dangerVal;
	iCtlCnst*	m_pCnstToBuild;
	Strategy	m_Strategy;

private:
	iCtlCnst* SelectCnst(const uint16* cnsts, uint32 cnt);
	void DefineStrategy();

public:
	iAI_Castle(iAI_Player* pPlayer, iCastle* pCastle);
	~iAI_Castle();

	bool PaySubs_Process(iMineralSet& receipt, iAI_Paymaster::RequestType reqType);

	//inline const iCtlCnst* CnstToBuild() const { return m_pCnstToBuild; }
	//iMineralSet FullBuildPrice() const;
	inline uint32 DangerVal() const { return m_dangerVal; }
	inline Strategy GetStrategy() const { return m_Strategy; }
	const iHero* CanRecruitHero(iMineralSet& cost, iTavernCtlCnst*& tavern);
	inline iCastle* GetCastle() { return m_pCastle; }
};

/*
 *	AI Player
 */
class iAI_Player : public iPlayer
{
public:
	enum TaskType {
		TT_DefeatHero,
		TT_DefendCastle,
		TT_CaptureCastle,
		TT_CaptureCnst,
	};

	struct iTask : public TypeAware {
		IMPL_TYPEAWARE(iTask)
		iTask(TaskType _taskType, sint32 _priority) : taskType(_taskType), priority(_priority) {}
		TaskType	taskType;
		sint32		priority;
	};

	struct iTask_DefeatHero : public iTask {
		IMPL_TYPEAWARE(iTask_DefeatHero)
		iTask_DefeatHero(sint32 _priority, iHero* _pTarget) : iTask(TT_DefeatHero, _priority), pTarget(_pTarget) {}
		iHero*		pTarget;
	};

	struct iTask_DefendCastle : public iTask {
		IMPL_TYPEAWARE(iTask_DefendCastle)
		iTask_DefendCastle(sint32 _priority, iCastle* _pCastle, iHero* _pEnemy) : iTask(TT_DefendCastle, _priority), pCastle(_pCastle), pEnemy(_pEnemy) {}
		iCastle*	pCastle;
		iHero*		pEnemy;
	};

	struct iTask_CaptureCastle : public iTask {
		IMPL_TYPEAWARE(iTask_CaptureCastle)
		iTask_CaptureCastle(sint32 _priority, iCastle* _pTarget) : iTask(TT_CaptureCastle, _priority), pTarget(_pTarget) {}
		iCastle*	pTarget;
	};

	struct iTask_CaptureCnst : public iTask {
		IMPL_TYPEAWARE(iTask_CaptureCnst)
		iTask_CaptureCnst(sint32 _priority, iOwnCnst* _pCnst) : iTask(TT_CaptureCnst, _priority), pCnst(_pCnst) {}
		iOwnCnst*	pCnst;
	};

private:
	iAI_Paymaster				m_paymaster;
	fix32						m_acc;
	iDangerMap					m_dangerMap;
	iSimpleArray<iAI_Castle*>	m_aiCtlList;
	iHero*						m_pCommander;
	iPoint						m_ptSeekUlt;
	
private:
	void DefineCommander();
	void PrepareDangerMap();
	bool StepHero();
	void UpdatePriorityList();
	iTavernCtlCnst* SelectBestTavernToRecruit();
	uint32 OptimalHeroesCount() const;
	
public:
	IMPL_TYPEAWARE( iAI_Player );
	iAI_Player(PLAYER_TYPE_MASK playerTypeMask, PLAYER_ID playerId, NATION_TYPE nation, const iSize& map_siz);
	~iAI_Player();
	inline PLAYER_TYPE PlayerType() const { return PT_COMPUTER; }
	void Process(fix32 t);
	void OnSelect(bool bNewWeek, bool bAfterLoad);
	void OnCaptureCastle(iCastle* pNewCastle);
	void AddEvent(const iStringT& text) {}
	
	iAI_Paymaster& PayMaster() { return m_paymaster; }

	iCastle* SelectBestCastleForEscape(iHero* pHero);
	void ProcessChest(iHero* pHero, iMapItem_Chest* pChest);

	bool ProcessRecruitCreatures(iArmy& army, iCreatGroup& cgroup);
	bool ProcessJoinCreatures(iArmy& army, const iCreatGroup& cgroup);

	bool NeedMeeting(const iHero* pHero1, const iHero* pHero2) const;
	void MeetHeroes(iHero* pHero1, iHero* pHero2);
	void UpdateUltSeekPoint();

	bool NeedStopHero() const;
	void ProcessItemChanges();
	void OnHeroDead(iHero* pHero);
	sint32 MineralValue(MINERAL_TYPE mt) const;
	inline const iDangerMap& DangerMap() const { return m_dangerMap; }
	inline const iPoint& UltSeekPoint() const { return m_ptSeekUlt; } 
};

/*
 *	Dwellings manager
 */
class iDwellMgr
{
public:
	iDwellMgr(iCastle* pCastle);
	void Update();
	sint32 MaxRecruit(iMineralSet maxCost);
	void Recruit(iArmy& army, uint32 reqPower, iMineralSet& cost);

private:
	typedef iSortArray<iDwellCtlCnst*> iDwellings;
	iPlayer*	m_pOwner;
	iCastle*	m_pCastle;
	iDwellings	m_dwellings;
};

/*
 *	Dwellings manager
 */
class iMGuildMgr
{
public:
	iMGuildMgr(iCastle* pCastle);
	void Update();
	void GetSpells(iSpellList& spellList);

private:
	typedef iSimpleArray<iMGuildCtlCnst*> iMageGuilds;
	iPlayer*	m_pOwner;
	iCastle*	m_pCastle;
	iMageGuilds	m_mageGuilds;
};

/*
 * Other
 */
void TossArmies(iArmy& armyA, iArmy& armyB, uint32 flags, uint32 pwr);


#endif //_HMM_GAME_ARTIFICIAL_INTELLIGENCE_H_

