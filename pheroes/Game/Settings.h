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

#ifndef _HMM_GAME_SETTINGS_H_
#define _HMM_GAME_SETTINGS_H_

//////////////////////////////////////////////////////////////////////////
enum ButtonActionType {
	BAT_INVALID = -1,
	BAT_HELP_MODE = 0,
	BAT_HAND_MODE,
	BAT_SURVEY_MODE,
	BAT_MINIMIZE_APP,
	BAT_MAKE_SCREENSHOT,
	BAT_COUNT
};

//////////////////////////////////////////////////////////////////////////
enum ConfigEntryType {
	CET_SFXVOLUME,
	CET_DISPGAMMA,
	CET_MAPSCROLLSPEED,
	CET_HEROSPEED,
	CET_COMBATSPEED,
	CET_QUICKCOMBAT,
	CET_ENDTURNCONF,
	CET_LHANDMAODE,
	CET_OVRLANDGRID,
	CET_COMBATGRID,
	CET_TRVPTSMODE,
	CET_SURVMAPFILTR,
	CET_SURVMAPSCALE,
	CET_NGDSORT,
	CET_NGDPOS,
	CET_COUNT
};

//////////////////////////////////////////////////////////////////////////
struct file_size_struct { uint32 name; uint32 size; };
typedef iSimpleArray<file_size_struct> file_sizez;

//////////////////////////////////////////////////////////////////////////
class iSettings
{
public:
	iSettings();
	~iSettings();
	bool Init(const iStringT& cmdLine);
	void Save();

	void SetActivationKey(const iStringT& newKey);
	const iStringT& GetActivationKey() const { return m_ActKey; }
	bool HasActivationKey() const { return !m_ActKey.Empty(); }

	void SetGameSN(uint32 newSN) { m_gameSN = newSN; }
	uint32 GetGameSN() const { return m_gameSN; }

	void SetMagicNumbers(uint32 nums[4]) { memcpy(m_magNums, nums, sizeof(m_magNums)); }
	uint32 GetMagicNumber(uint32 idx) const { check(idx<4); return m_magNums[idx]; }

	sint32 GetEntryValue(ConfigEntryType entry) const { return m_cfgEntries[entry]; }
	void SetEntryValue(ConfigEntryType entry, sint32 val);

	inline sint32 ActionKey(ButtonActionType bat) const { return m_actionKeys[bat]; }
	inline void AssignActionKey(ButtonActionType bat, iKbdKey key) { m_actionKeys[bat] = key; }
	inline ButtonActionType GetActionByKey(iKbdKey key) { for (sint32 nn=0; nn<BAT_COUNT; ++nn) if (m_actionKeys[nn] == key) return (ButtonActionType)nn; return BAT_INVALID; }

	inline bool ForceNoSound() const { return m_bNoSound; }
	inline bool FogOfWar() const { return m_bFogOfWar; }
	inline bool ShowEnemyTurn() const { return m_bShowEnemyTurn; }
	inline bool MapSpriteFile() const { return m_bMapSpriteFile; }

protected:
	sint32		m_cfgEntries[CET_COUNT];
	sint32		m_actionKeys[BAT_COUNT];
	bool		m_bFogOfWar;
	bool		m_bShowEnemyTurn;
	bool		m_bMapSpriteFile;
	bool		m_bNoSound;
	iStringT	m_ActKey;
	uint32		m_gameSN;
	uint32		m_magNums[4];

	file_sizez	m_filez;
};

#endif //_HMM_GAME_SETTINGS_H_

