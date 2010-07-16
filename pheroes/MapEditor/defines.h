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

#ifndef _HMM_MAP_EDITOR_COMMON_DEFINES_
#define _HMM_MAP_EDITOR_COMMON_DEFINES_

/*
 *	
 */
#define RGB16(r,g,b) (uint16)((r&0xF8)<<8 | (g&0xFC)<<3 | (b&0xF8)>>3)


template <class KeyType>
struct SpriteElT {
	SpriteElT(const KeyType& _spriteId, SPRITE_LEVEL _level, const iPoint& _anchor)
	: spriteId(_spriteId), level(_level), anchor(_anchor) {}

	KeyType			spriteId;
	SPRITE_LEVEL	level;
	iPoint			anchor;
};

struct PassMod {
	PassMod(const iPoint& _offset, sint8 _mod)
	: offset(_offset), mod(_mod) {}
	iPoint	offset;
	sint8	mod;
};

typedef iStringT	SpriteId;
typedef SpriteElT<SpriteId>		SpriteEl;
typedef iSimpleArray<SpriteEl>	SpriteList;

typedef iSimpleArray<PassMod>	PassModList;
typedef iSimpleArray<iPoint>	FlagList;

//
class iAvatar
{
public:
	iAvatar(const iStringT& id, const iStringT& nameKey, uint32 _idx)
	: m_id(id), m_nameKey(nameKey), m_idx(_idx) {}

	// Index
	void SetIdx(uint32 nIdx)
	{ m_idx = nIdx;}
	inline uint32 Idx() const
	{ return m_idx; }

	// Id
	void SetId(const iStringT& nId)
	{ m_id = nId; }
	inline const iStringT& Id() const
	{ return m_id; }

	// Name key
	void SetNameKey(const iStringT& nNameKey)
	{ m_nameKey = nNameKey; }
	inline const iStringT& NameKey() const
	{ return m_nameKey; }
	inline bool HasName() const
	{ return !m_nameKey.Empty(); } 

	// Pass mods
	void AddPassMod(const iPoint& pos, sint8 mod)
	{ m_passMods.Add(PassMod(pos,mod)); }
	inline uint32 PassModsCount() const
	{ return m_passMods.GetSize(); }
	inline const PassMod& PassModEntry(uint32 idx) const
	{ return m_passMods[idx]; }

	// Sound
	void SetSoundId(const iStringT& soundId) { m_soundId = soundId; }
	const iStringT& GetSoundId() const { return m_soundId; }

	// Sprites
	void AddSprite(const SpriteId& sid, SPRITE_LEVEL level, const iPoint& anchor)
	{ m_sprites.Add(SpriteEl(sid,level, anchor)); }
	inline uint32 SpritesCount() const
	{ return m_sprites.GetSize(); }
	inline const SpriteEl& SpriteEntry(uint32 idx) const
	{ return m_sprites[idx]; }

	// Flags
	void AddFlag(const iPoint& pos)
	{ m_flags.Add(pos); }
	inline uint32 FlagsCount() const
	{ return m_flags.GetSize(); }
	inline const iPoint& FlagEntry(uint32 idx) const
	{ return m_flags[idx]; }

protected:
	FlagList	m_flags;
	SpriteList	m_sprites;
	PassModList	m_passMods;
	uint32		m_idx;
	iStringT	m_soundId;
	iStringT	m_id;
	iStringT	m_nameKey;
};
//
typedef iStringT	AvatarId;
struct AvatarEntry {
	AvatarEntry(const iPoint& _offset, const AvatarId& _id) : offset(_offset), id(_id) {}
	iPoint		offset;
	AvatarId	id;
};
typedef iSimpleArray<AvatarEntry>	AvatarList;
//
struct iVisualObject {
	void AddAvatar(const AvatarId& aid, const iPoint& offset)
	{ m_AvatarList.Add(AvatarEntry(offset,aid)); }
	void AddAvatar(const AvatarEntry& avt)
	{ m_AvatarList.Add(avt); }

	inline uint32 AvatarsCount() const
	{ return m_AvatarList.GetSize(); }
	inline const AvatarEntry Avatar(uint32 idx) const
	{ check(idx<m_AvatarList.GetSize()); return m_AvatarList[idx]; }

private:
	AvatarList	m_AvatarList;
};

//
class iIdxItem
{
public:
	iIdxItem(const iStringT& id, uint16 idx = 0xFFFF) : m_Id(id), m_Idx(idx) {}

	inline const iStringT& Id() const
	{ return m_Id; }
	inline uint16 Idx() const
	{ return m_Idx; }
	inline uint16& Idx()
	{ return m_Idx; }

private:
	iStringT	m_Id;
	uint16		m_Idx;
};
/*
 *	
 */

enum TextEntryType {
	TET_DEFAULT = 0,
	TET_MAP_NAME,		// * New map dialog, Map props dialog 
	TET_MAP_DESC,		// * New map dialog, Map props dialog
	TET_TEVENT_TEXT,	// Map props (time events dialog)
	TET_HERO_CNAME,		// * Hero edit and delete
	TET_MAPITEM_MSG,	// * Map item edit and delete
	TET_MAPGUARD_MSG,	// * Map guard edit and delete
	TET_MEVENT_REWMSG,	// Map event create, edit and delete
	TET_MEVENT_ATTMSG,	// Map event create, edit and delete
	TET_VIS_MSG,		// * Visitable edit and delete
	TET_CASTLE_CNAME,	// * Castle edit and delete
	TET_COUNT
};

extern bool TEXT_ENTRY_ML[TET_COUNT];

iStringT MakeTextEntryName(TextEntryType t, void* pObj);

extern LPCTSTR LANG_NAME[GLNG_COUNT];
extern LPCTSTR SURF_ID[STYPE_COUNT];
extern LPCTSTR SURF_NAMEKEY[STYPE_COUNT];

extern LPCTSTR MAP_SIZ_SNAME[MSIZ_COUNT];
extern LPCTSTR MAP_SIZ_NAMEKEY[MSIZ_COUNT];

extern LPCTSTR DIFF_NAMEKEY[DFC_COUNT];

extern LPCTSTR PLAYER_NAMEKEY[PID_COUNT];
extern LPCTSTR PLAYER_TYPE_NAMEKEY[PTM_COUNT];
extern LPCTSTR PLAYER_TYPE_MASK_NAMEKEY[PTM_COUNT];

extern LPCTSTR HERO_TYPE_ID[HERO_TYPE_COUNT];
extern LPCTSTR HERO_TYPE_NAMEKEY[HERO_TYPE_COUNT];

extern LPCTSTR VCNSTTYPE_ID[VCNST_COUNT];
extern LPCTSTR OCNSTTYPE_ID[OCNST_COUNT];
extern LPCTSTR ARTTYPE_ID[ARTT_COUNT];
extern LPCTSTR ARTASSIGN_ID[ART_ASSIGN_COUNT];

extern LPCTSTR CASTLE_PREFIXES[CTLT_COUNT];
extern LPCTSTR CASTLE_NAMEKEY[CTLT_COUNT];
extern LPCTSTR CTLCNST_NAMEKEY[CTLCNST_COUNT];

extern LPCTSTR FURTSKILL_ID[FSK_COUNT];
extern LPCTSTR PRSKILL_ID[PRSKILL_COUNT];
extern LPCTSTR SECSKILL_ID[SECSK_COUNT];
extern LPCTSTR PRSKILL_NAMEKEY[PRSKILL_COUNT];
extern LPCTSTR SECSK_NAMEKEY[SECSK_COUNT];
extern LPCTSTR SSLVL_NAMEKEY[SSLVL_COUNT];
extern LPCTSTR SPELL_NAMEKEY[MSP_COUNT];

extern LPCTSTR MINERAL_ID[MINERAL_COUNT];
extern LPCTSTR MINERAL_NAMEKEY[MINERAL_COUNT];
extern LPCTSTR RESOURCE_NAMEKEY[MINERAL_COUNT];
extern LPCTSTR RAND_GUARDS_NAMEKEY[7];

extern LPCTSTR ART_LEVEL_ID[ART_LEVEL_COUNT];

extern LPCTSTR CREAT_ID[CREAT_COUNT];
extern LPCTSTR CREAT_NAMEKEY[CREAT_COUNT];

extern LPCTSTR MAGIC_SPELL_ID[MSP_COUNT];
extern LPCTSTR MAGIC_SPELL_NAMEKEY[MSP_COUNT];

extern LPCTSTR REWARD_TYPE_ID[RIT_COUNT];


#endif //_HMM_MAP_EDITOR_COMMON_DEFINES_