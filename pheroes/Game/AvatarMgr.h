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

#ifndef _HMM_GAME_AVATAR_MANAGER_H_
#define _HMM_GAME_AVATAR_MANAGER_H_

/*
 *	
 */
class iCoverHolder
{
public:
	struct iCoverEntry {
		inline iCoverEntry(sint16 _ox, sint16 _oy) : ox(_ox), oy(_oy) {}
		sint16 ox,oy;
	};
	typedef iSimpleArray<iCoverEntry>	CoverList;

public:
	inline void AddCover(sint16 ox, sint16 oy)
	{ m_Covers.Add(iCoverEntry(ox,oy)); }
	inline uint32 CoversCount() const
	{ return m_Covers.GetSize(); }
	inline iCoverEntry CoverEntry(uint32 idx) const
	{ return m_Covers[idx]; }

private:
	CoverList	m_Covers;
};

//////////////////////////////////////////////////////////////////////////
class iAvatarsHolder
{
public:
	struct iAvatarEntry {
		inline iAvatarEntry(uint16 _aid, sint16 _ox, sint16 _oy) : aid(_aid), ox(_ox), oy(_oy) {}
		uint16 aid;
		uint16 reserved;
		sint16 ox,oy;
	};
	typedef iSimpleArray<iAvatarEntry>	AvatarsList;

public:
	void AddAvatar(uint16 aid, sint16 ox, sint16 oy);

	inline uint32 AvatarsCount() const
	{ return m_Avatars.GetSize(); }
	inline iAvatarEntry AvatarEntry(uint32 idx) const
	{ return m_Avatars[idx]; }
	inline uint32 CoversCount() const
	{ return m_Covers.CoversCount(); }
	inline iCoverHolder::iCoverEntry CoverEntry(uint32 idx) const
	{ return m_Covers.CoverEntry(idx); }

private:
	AvatarsList		m_Avatars;
	iCoverHolder	m_Covers;
};

//////////////////////////////////////////////////////////////////////////
class iAvatar
{
public:
	struct iSpriteEntry {
		inline iSpriteEntry(uint16 _sid, sint16 _ox, sint16 _oy, SPRITE_LEVEL _level) : sid(_sid), ox(_ox), oy(_oy), level(_level) {}
		uint16	sid;
		sint16	ox,oy;
		uint8	level;
		uint8	reserved;
	};

	struct iFlagEntry {
		inline iFlagEntry(sint16 _ox, sint16 _oy) : ox(_ox), oy(_oy) {}
		sint16 ox,oy;
	};

	struct iPassModEntry {
		inline iPassModEntry(sint16 _ox, sint16 _oy, uint8 _mod) : ox(_ox), oy(_oy), mod(_mod) {}
		sint16	ox,oy;
		uint8	mod;
		uint8	reserved;
	};
	
	typedef iSimpleArray<iSpriteEntry>	SpriteList;
	typedef iSimpleArray<iFlagEntry>	FlagList;
	typedef iSimpleArray<iPassModEntry>	PassModList;

public:
	iAvatar(TextResId nameKey) : m_NameKey(nameKey), m_soundId(0xFFFF) {}

	inline TextResId Name() const { return m_NameKey; }

	// Sound
	uint16 GetSoundId() const { return m_soundId; }
	void SetSoundId(uint16 soundId) { m_soundId = soundId; }

	// Sprites
	inline void AddSprite(uint16 sid, sint16 ox, sint16 oy, SPRITE_LEVEL level)
	{ m_Sprites.Add(iSpriteEntry(sid,ox,oy,level)); }
	inline uint32 SpritesCount() const { return m_Sprites.GetSize(); }
	inline iSpriteEntry SpriteEntry(uint32 idx) const { return m_Sprites[idx]; }
	inline uint16 GetSpriteId(uint32 idx) const { return m_Sprites[idx].sid; }
	inline iPoint GetSpriteOffset(uint32 idx) const { return iPoint(m_Sprites[idx].ox,m_Sprites[idx].oy); }
	inline SPRITE_LEVEL GetSpriteLevel(uint32 idx) const { return (SPRITE_LEVEL)m_Sprites[idx].level; }

	// Flags
	inline void AddFlag(sint16 ox, sint16 oy) { m_Flags.Add(iFlagEntry(ox,oy)); }
	inline uint32 FlagsCount() const { return m_Flags.GetSize(); }
	inline iFlagEntry FlagEntry(uint32 idx) const { return m_Flags[idx]; }

	// PassMods
	inline void AddPassMod(sint16 ox, sint16 oy, uint8 mod)
	{ m_passMods.Add(iPassModEntry(ox,oy,mod)); }
	inline uint32 PassModsCount() const { return m_passMods.GetSize(); }
	inline iPassModEntry PassModEntry(uint32 idx) const { return m_passMods[idx]; }

private:
	TextResId	m_NameKey;
	uint16		m_soundId;
	uint16		m_Cfg;
	SpriteList	m_Sprites;
	FlagList	m_Flags;
	PassModList	m_passMods;
};

/*
 *	
 */
class iAvatarMgr
{
public:
	iAvatarMgr();
	~iAvatarMgr();

	iAvatar& AddAvatar(TextResId nameKey);

	inline const iAvatar& operator[] (uint16 idx) const
	{ check(idx<m_Avatars.GetSize()); return *m_Avatars[idx]; }

	uint16 FindAvatarBySprite(SpriteId sid) const;

private:
	iSimpleArray<iAvatar*>	m_Avatars;
};

bool LoadAvatars(iDynamicBuffer& buff, iAvatarMgr &amgr);

#endif //_HMM_GAME_AVATAR_MANAGER_H_

