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

#ifndef _HMM_GAME_WORLD_H_
#define _HMM_GAME_WORLD_H_
/*
 *	Game world
 */
class iGameWorld : public iGameMap
{
public:
	typedef iMapT<iBaseMapObject*>		iCoverMap;
	typedef iMapT<sint8>				iPassMap;

public:
	iGameWorld();
	~iGameWorld();

	inline const iStringT MapName() const { return m_Name; }

	inline MAP_SIZE GameWorldSize() const { return m_mapSiz; }
	inline sint32 Rand(sint32 maxval) { return m_rand.Rand(maxval); }
	
	uint32 CalcGameScore() const;

	iBaseMapObject* GetCellObj(const iPoint& cell);
	iBaseMapObject* GetCellObj(const iPoint& cell, iStringT& name);
	void StartGame(PLAYER_ID curPlayer, bool bAfterLoad);
	iPlayer* AddPlayer(PLAYER_TYPE_MASK playerTypeMask, PLAYER_ID pid, NATION_TYPE nation, PLAYER_TYPE pt);
	void RemovePlayer(iPlayer* pPlayer);
	void SetHeroPos(iHero* pHero, const iPoint& pos);
	iHero* AddHero(iHero* pHero, iPlayer* pOwner, const iPoint& pos, sint8 angle, bool bUpdateFog = true);
	void RemoveHero(iHero* pHero, bool bResetArmy);
	void MoveHero(const iPoint& oldPos, const iPoint& newPos);
	iVisCnst* AddVisCnst(iVisCnst* pCnst);
	iOwnCnst* AddOwnCnst(iOwnCnst* pCnst, bool bUpdateFog = true);
	iCastle* AddCastle(iCastle* pCastle, bool bUpdateFog = true);
	iMapEvent* AddMapEvent(iMapEvent* pMapEvent);
	void RemoveMapEvent(iMapEvent* pMapEvent);
	iMapEvent* FindMapEvent(const iPoint& cell, PLAYER_ID pid);
	iMapItem* AddMapItem(iMapItem* pMapItem);
	void RemoveMapItem(iMapItem* pMapItem);
	iMapGuard* AddMapGuard(iMapGuard* pMapGuard);
	void RemoveMapGuard(iMapGuard* pMapGuard);

	void CaptureCastle(iHero* pHero, iCastle* pCastle);
	bool CanDig(const iPoint& cell) const;
	bool Dig(const iPoint& cell);
	bool InitUltimateArt();

	bool InitMap(sint32 w, sint32 h);
	void CleanupGameWorld();
	bool LoadFromFile(const iMapInfo& mapInfo, iFileI* pFile);
	bool SaveToFile(iFileI* pFile);
	void InitPassMap();

	void Process(fix32 t);
	void SetNewActor(iPlayer* pNewActor, bool bAttack);
	iPlayer* NextPlayer();
	void NewDay();
	void NewWeek();
	void ProcessTimeEvents(iPlayer* pActor);

	inline iPoint UltimatePos() const { return m_posUltimate; }
	inline bool IsUltimateExists() const { return m_statUltimate == 0; }
	inline uint16 ObelisksCount() const { return m_obelisksCount; }

	inline uint16 GetPathEl(sint32 xpos, sint32 ypos) const
	{
		uint16 avatar = GetAvatar(xpos,ypos);
		if (avatar != 0xFFFF && (avatar & 0xFF00) == 0xFF00) return (avatar & 0xFF);
		return 0xFFFF;
	}

	inline uint8 PathElConfig(sint32 xpos, sint32 ypos) const
	{
		uint16 pathEl = GetPathEl(xpos,ypos);
		check(pathEl != 0xFFFF);
		uint8 cfg = 0;
		if (IsValidPos(xpos, ypos-1) && GetPathEl(xpos, ypos-1) == pathEl) cfg |= 1;
		if (IsValidPos(xpos+1, ypos) && GetPathEl(xpos+1, ypos) == pathEl) cfg |= 2;
		if (IsValidPos(xpos, ypos+1) && GetPathEl(xpos, ypos+1) == pathEl) cfg |= 4;
		if (IsValidPos(xpos-1, ypos) && GetPathEl(xpos-1, ypos) == pathEl) cfg |= 8;
		return cfg;
	}

	inline const iWeekDesc& WeekDesc() const { return m_weekDesc; }
	inline uint32 GameDay() const { return m_CurDay; }

	inline iPlayer* FindPlayer(PLAYER_ID pid)
	{
		for (iPLIt it=m_Players.First(); it != m_Players.End(); ++it) {
			if ( (*it)->PlayerId() == pid) return *it;
		}
		check(0);
		return NULL;
	}

	DIFFICULTY_LEVEL DifficultyLevel() const { check(m_Difficulty!=DFC_UNDEFINED); return m_Difficulty; }

	uint32 TeleportsCount() const { return m_teleports.GetSize(); }
	iVisCnst_Teleport* GetTeleport(sint32 idx) { return m_teleports[idx]; }
	iVisCnst_Teleport* FindTeleport(sint32 x, sint32 y) 
	{ for (uint32 nn=0; nn<m_teleports.GetSize(); ++nn) if (m_teleports[nn]->Pos().x == x && m_teleports[nn]->Pos().y == y) return m_teleports[nn]; return NULL; }

	inline uint16 GetAvatar(sint32 x, sint32 y) const { return GetAt(x,y).avatar; }

	inline iPlayer* ActPlayer() { return m_pActPlayer; }

	inline iPlayer* CurPlayer() { return m_pCurPlayer; }
	inline const iPlayer* CurPlayer() const { return m_pCurPlayer; }

	inline iHero* CurHero() { return (m_pCurPlayer)?m_pCurPlayer->CurHero():NULL; }
	inline const iHero* CurHero() const { return (m_pCurPlayer)?m_pCurPlayer->CurHero():NULL; }

	inline iCastle* CurCastle() { return (m_pCurPlayer)?m_pCurPlayer->CurCastle():NULL; }
	inline const iCastle* CurCastle() const { return (m_pCurPlayer)?m_pCurPlayer->CurCastle():NULL; }
	inline bool IsCurPlayerActive() const { return m_pCurPlayer == m_pActPlayer; }

	iPlayerList		m_Players;
	iVisCnstList	m_VisCnsts;
	iOwnCnstList	m_OwnCnsts;
	iCastleList		m_Castles;
	iMapItemList	m_MapItems;
	iMapEventList	m_MapEvents;
	iMapGuardList	m_MapGuards;
	uint32			m_CurDay;
	iCoverMap		m_CoverMap;
	iVanishItem*	m_pVanItem;
	iPassMap		m_PassMap;

private:
	void GenerateGuards(CREATURE_TYPE ct, sint32 week);

private:
	typedef iSimpleArray<iVisCnst_Teleport*> iTeleportList;

	iTeleportList		m_teleports;
	iTimeEventMgr		m_timeEventMgr;
	iRandomizer			m_rand;
	iWeekDesc			m_weekDesc;
	iPlayer*			m_pCurPlayer;
	iPlayer*			m_pActPlayer;
	iStringT			m_Name;
	iStringT			m_Desc;
	iStringT			m_Version;
	iStringT			m_Author;
	MAP_SIZE			m_mapSiz;
	iMapInfo::GameMode	m_gameMode;
	DIFFICULTY_LEVEL	m_Difficulty;

	iPoint				m_posUltimate;
	uint8				m_radUltimate;
	uint8				m_statUltimate;
	uint16				m_obelisksCount;
};

#endif //_HMM_GAME_WORLD_H_

