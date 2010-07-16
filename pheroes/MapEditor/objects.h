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

#ifndef _HMM_EDITOR_OBJECTS_DEFS_H_
#define _HMM_EDITOR_OBJECTS_DEFS_H_

//////////////////////////////////////////////////////////////////////////
class iBaseMapObject : public TypeAware
{
public:
	IMPL_TYPEAWARE( iBaseMapObject );
	iBaseMapObject(const iPoint& pos) : m_Pos(pos){}
	
	inline const iPoint& Pos() const { return m_Pos; }
	void SetPos(const iPoint& pos) { m_Pos = pos; }

private:
	iPoint	m_Pos;
};

//////////////////////////////////////////////////////////////////////////
class iMapItem : public iBaseMapObject
{
public:
	IMPL_TYPEAWARE( iMapItem );
	iMapItem(const iPoint& pos, MAP_ITEM_TYPE mit, const SpriteId& sid);
	virtual ~iMapItem() {}

	inline const iStringT& Message() const { return m_message; }
	inline iStringT& Message() { return m_message; }
	inline MAP_ITEM_TYPE Type() const { return m_mapItemType; }
	inline const SpriteId& Sprite() const { return m_spriteId; }
	inline const iArmy& Guard() const { return m_guard; }
	inline iArmy& Guard() { return m_guard; }

protected:
	iArmy			m_guard;
	MAP_ITEM_TYPE	m_mapItemType;
	SpriteId		m_spriteId;
	iStringT		m_message;
};

//
class iMapItem_Mineral : public iMapItem
{
public:
	iMapItem_Mineral(const iPoint& pos, MINERAL_TYPE mt, sint32 count);

	inline MINERAL_TYPE& MineralType() { return m_mineralType; }
	inline sint32& MineralCount() { return m_count; }

	inline MINERAL_TYPE MineralType() const { return m_mineralType; }
	inline sint32 MineralCount() const { return m_count; }

private:
	MINERAL_TYPE	m_mineralType;
	sint32			m_count;
};

//
class iMapItem_ManaCrystal : public iMapItem
{
public:
	iMapItem_ManaCrystal(const iPoint& pos, sint32 count);
	inline sint32 Count() const { return m_count; }

private:
	sint32			m_count;
};

//
class iMapItem_CampFire : public iMapItem
{
public:
	iMapItem_CampFire(const iPoint& pos);
};

//
class iMapItem_Chest : public iMapItem
{
public:
	iMapItem_Chest(const iPoint& pos);
};

//
class iMapItem_SpellScroll : public iMapItem
{
public:
	iMapItem_SpellScroll(const iPoint& pos, uint16 spellId);
	inline uint16 SpellId() const { return m_spellId; }
	inline uint16& SpellId() { return m_spellId; }

private:
	uint16	m_spellId;
};
//
class iMapItem_Lamp : public iMapItem
{
public:
	iMapItem_Lamp(const iPoint& pos);
};
//
class iMapItem_KeyGuard : public iMapItem
{
public:
	iMapItem_KeyGuard(const iPoint& pos, uint8 key);
	uint8 GetKeyType() const { return m_key; }

private:
	uint8	m_key;
};

//////////////////////////////////////////////////////////////////////////
class iMapGuard : public iBaseMapObject
{
public:
	IMPL_TYPEAWARE( iMapGuard );

	iMapGuard(const iPoint& pos, CREATURE_TYPE ct=CREAT_UNKNOWN, sint32 count=RANDOM_QUANTITY, GUARD_DISPOS disp = GDISP_AGGRESSIVE, bool bNotGrow = false);
	const iStringT& Title() const;
	inline const iCreatGroup& Creatures() const { return m_creat; }
	inline iCreatGroup& Creatures() { return m_creat; }
	inline GUARD_DISPOS& Disposition() { return m_disp; } 
	inline GUARD_DISPOS Disposition() const { return m_disp; } 
	inline bool& NotGrow() { return m_bNotGrow; } 
	inline bool NotGrow() const { return m_bNotGrow; }
	inline const SpriteId& Sprite() const { return m_spriteId; }
	inline const iStringT& Message() const { return m_message; }
	inline iStringT& Message() { return m_message; }

private:
	iStringT		m_message;
	SpriteId		m_spriteId;
	iCreatGroup		m_creat;
	GUARD_DISPOS	m_disp;
	bool			m_bNotGrow;
};

//////////////////////////////////////////////////////////////////////////
class iMapEvent : public iBaseMapObject
{
public:
	IMPL_TYPEAWARE( iMapEvent );
	iMapEvent(const iPoint& pos);

	inline const iArmy& Guard() const { return m_guard; }
	inline iArmy& Guard() { return m_guard; }
	inline const iStringT& RewardMessage() const { return m_rewardMessage; }
	inline iStringT& RewardMessage() { return m_rewardMessage; }
	inline const iStringT& AttackMessage() const { return m_attackMessage; }
	inline iStringT& AttackMessage() { return m_attackMessage; }
	inline uint32 PlayerMask() const { return m_playerMask; }
	inline uint32& PlayerMask() { return m_playerMask; }
	inline const iRewardsCtr& Rewards() const { return m_rewards; }
	inline iRewardsCtr& Rewards() { return m_rewards; }
	
private:
	iRewardsCtr	m_rewards;
	uint32		m_playerMask;
	iStringT	m_rewardMessage;
	iStringT	m_attackMessage;
	iArmy		m_guard;
};


//////////////////////////////////////////////////////////////////////////
class iHeroNode;
class iHero : public iBaseMapObject
{
public:
	IMPL_TYPEAWARE( iHero );
	iHero(const iPoint& pos, iHeroNode* proto, PLAYER_ID owner);

	inline const iHeroNode* Proto() const { return m_pProto; }
	inline iHeroNode* Proto() { return m_pProto; }
	
	// Owner
	inline PLAYER_ID Owner() const { return m_Owner; }
	inline PLAYER_ID& Owner() { return m_Owner; }
	// Level
	inline uint32 Level() const { return m_Level; }
	inline uint32& Level() { return m_Level; }
	// Experience
	inline sint32 Experience() const { return m_Experience; }
	inline sint32& Experience() { return m_Experience; }
	// FurtSkills
	inline const iFurtSkills& FurtSkills() const { return m_FurtSkills; }
	inline iFurtSkills& FurtSkills() { return m_FurtSkills; }
	// Secondary skills
	inline const iSecSkills& SecSkills() const { return m_SecSkills; }
	inline iSecSkills& SecSkills() { return m_SecSkills; }
	// Army
	inline const iArmy& Army() const { return m_army; }
	inline iArmy& Army() { return m_army; }
	// Artifacts
	inline const iArtifactList& Artifacts() const { return m_artifacts; }
	inline iArtifactList& Artifacts() { return m_artifacts; }
	// Spellbook
	inline const iSpellList& Spells() const { return m_spells; }
	inline iSpellList& Spells() { return m_spells; }
	inline bool IsCustomSpells() const { return m_bCustomSpells; }
	inline void SetCustomSpells(bool bCustomSpells = true) { m_bCustomSpells = bCustomSpells; }
	// Custom name
	inline const iStringT& CustomName() const { return m_CustomName; }
	inline iStringT& CustomName() { return m_CustomName; }

private:
	iArmy			m_army;
	iHeroNode*		m_pProto;
	PLAYER_ID		m_Owner;
	uint32			m_Level;
	sint32			m_Experience;
	iFurtSkills		m_FurtSkills;
	iSecSkills		m_SecSkills;
	iArtifactList	m_artifacts;
	iSpellList		m_spells;
	bool			m_bCustomSpells;
	iStringT		m_CustomName;
};
//
class iCastle;
class iPlayer
{
public:
	iPlayer(PLAYER_ID playerId, PLAYER_TYPE_MASK ptm) : m_PlayerId(playerId), m_PlayerTypeMask(ptm), m_pMainCastle(NULL), m_bCreateHero(false) {}

	iCastle*			m_pMainCastle;
	bool				m_bCreateHero;
	PLAYER_ID			m_PlayerId;
	PLAYER_TYPE_MASK	m_PlayerTypeMask;
};

// 
struct iBattleObstacle {
	iBattleObstacle(const SpriteId& sprite, const iPoint& offset, uint8 size, uint8 level)
	: m_surfMask(0), m_sprite(sprite), m_offset(offset), m_size(size), m_level(level) {}
	uint16		m_surfMask;
	SpriteId	m_sprite;
	iPoint		m_offset;
	uint8		m_size;
	uint8		m_level;
};

#endif //_HMM_EDITOR_OBJECTS_DEFS_H_