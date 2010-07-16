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
#include "serialize.h"

/*
 *	
 */
bool iMapInfo::Supported() const
{
	return true;
}

bool iMapInfo::ReadMapInfo(iFileI* pFile)
{
	// Map version
	uint16 ver;
	pFile->Read(&ver,sizeof(ver));
	if (ver != GMAP_FILE_VERSION) return false;

	// Save timestamp
	pFile->Read(&m_saveTime, sizeof(m_saveTime));
	
	// Random seed
	pFile->Read(&m_rseed, sizeof(m_rseed));

	// Map size
	uint8 mapSiz; pFile->Read(&mapSiz,sizeof(mapSiz));
	m_Size = (MAP_SIZE)mapSiz;

	// Map name and description (string of text)
	Unserialize(pFile, m_Name);
	Unserialize(pFile, m_Description);

	// Map version and author (string of text)
	Unserialize(pFile, m_Version);
	Unserialize(pFile, m_Author);

	// Current date (1 is default value for new game)
	pFile->Read(&m_curDay, sizeof(m_curDay));

	// Game mode (GM_UNDEFINED for new map)
	uint16 gameMode;
	pFile->Read(&gameMode,sizeof(gameMode));
	m_gameMode = (GameMode)gameMode;

	// Difficulty level (DFC_UNDEFINED for new game)
	sint8 gameDifLvl;
	pFile->Read(&gameDifLvl,sizeof(gameDifLvl));
	m_Difficulty = (DIFFICULTY_LEVEL)gameDifLvl;
	
	// Read Player config
	uint16 pCount;
	pFile->Read(&pCount, sizeof(pCount));
	bool bHumanDefined = false;
	for (uint32 xx=0; xx<pCount; ++xx){
		// Player Id
		sint8 playerId;	
		pFile->Read(&playerId,sizeof(playerId));
		// Nation type
		uint8 nation;
		pFile->Read(&nation,sizeof(nation));
		// Player Type Mask
		uint8 playerTypeMask;
		pFile->Read(&playerTypeMask,sizeof(playerTypeMask));
		// Player Type (PT_UNDEFINED for new game)
		uint8 playerType;
		pFile->Read(&playerType,sizeof(playerType));
		// Create playerInfo descriptor
		iPlayerInfo playerInfo((PLAYER_ID)playerId, (PLAYER_TYPE_MASK)playerTypeMask, (PLAYER_TYPE)playerType, (CTL_TYPE)nation);
		if (playerType == PT_UNDEFINED) {
			if (!bHumanDefined && (playerTypeMask == PTM_HUMAN_ONLY || playerTypeMask == PTM_HUMAN_OR_COMPUTER)) {
				bHumanDefined = true;
				playerInfo.m_Type = PT_HUMAN;
			} else {
				playerInfo.m_Type = PT_COMPUTER;
			}
		}
		// Player resources (undefined for new game)
		Unserialize(pFile, playerInfo.m_Minerals);
		// Current Hero idx (0xFFFF = undefined for new game)
		pFile->Read(&playerInfo.m_curHeroId,sizeof(playerInfo.m_curHeroId));
		// Current Castle idx (0xFFFF = undefined for new game)
		pFile->Read(&playerInfo.m_curCastleIdx,sizeof(playerInfo.m_curCastleIdx));
		// Keys state
		pFile->Read(&playerInfo.m_keys,sizeof(playerInfo.m_keys));
		// Insert Player descriptor
		m_Players.Add(playerInfo);
	}

	// Current player Id (PID_NEUTRAL = undefined for new game)
	uint16 curPlayerId;
	pFile->Read(&curPlayerId,sizeof(curPlayerId));
	if (curPlayerId == 0xFFFF)m_curPlayerId = PID_NEUTRAL;
	else m_curPlayerId = (PLAYER_ID)curPlayerId;

	// Map metrics
	uint16 w,h;
	pFile->Read(&w,sizeof(w));
	pFile->Read(&h,sizeof(h));
	m_metrics.w = w;
	m_metrics.h = h;

	return true;
}

// Reorder player list - PT_HUMAN players first, and then PT_COMPUTER type players.
void iMapInfo::ReorderPlayers()
{
	iSimpleArray<iPlayerInfo> defPl;
	for (uint32 pidx = 0; pidx<m_Players.GetSize();) {
		check(m_Players[pidx].m_Type != PT_UNDEFINED);
		if (m_Players[pidx].m_Type == PT_HUMAN) {
			defPl.Add(m_Players[pidx]);
			m_Players.RemoveAt(pidx);
		} else ++pidx;
	}
	check(defPl.GetSize() > 0);
	m_gameMode = (defPl.GetSize() == 1)?GM_SPLAYER:GM_HOTSEAT;
	defPl.Add(m_Players);
	m_Players = defPl;
}

/*
 *	
 */
iGameMap::iGameMap()
{
}

iGameMap::~iGameMap()
{
}


