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

#ifndef _HMM_GAME_OBJECTS_H_
#define _HMM_GAME_OBJECTS_H_

/*
 * Week descriptor
 */
struct iWeekDesc {
	enum Type {
		Normal,			// Any week: Normal population growth
		Plague,			// Only first week: All (dwellings) population halves
		CreatPlus,		// Any week except first: Normal population growth, +5 of specified creature
		CreatDoubles	// Only first week: Normal population growth, Specified creature population doubles (dwellings), generate map guards
	};

	void Init(bool _bNewMonth = false, Type _wkType = Normal, CREATURE_TYPE _crType = CREAT_UNKNOWN, sint32 _fparam = 0)
	{ bNewMonth = _bNewMonth; wkType = _wkType; crType = _crType; fparam = _fparam; }

	bool			bNewMonth;
	Type			wkType;
	CREATURE_TYPE	crType;
	sint32			fparam;
};

/*
 *	Temp hero enchancements
 */
enum ENCH_DURATION {
	ENDUR_NEXTDAY = 0,
	ENDUR_NEXTWEEK,
	ENDUR_NEXTBATTLE,
	ENDUR_COUNT
};
//
struct iEnchItem {
	enum { 
		PROVIDER_FLAG = 0x80000000
	};

	iEnchItem(sint32 _provider, FURTHER_SKILLS _type, ENCH_DURATION _dur, sint32 _mod)
	: provider(_provider), type(_type), dur(_dur), mod(_mod) {}

	sint32			provider;
	FURTHER_SKILLS	type;
	ENCH_DURATION	dur;
	sint32			mod;
};
/*
 *	Map Objects
 */
class iHero;
class iBaseMapObject : public TypeAware
{
public:
	IMPL_TYPEAWARE( iBaseMapObject );
	
public:
	iBaseMapObject(const iPoint& pos, bool bDisap)
	: m_Pos(pos), m_bDisap(bDisap) {}
	virtual ~iBaseMapObject() {}

	void SetPos(const iPoint& pos) { m_Pos = pos; }
	inline const iPoint& Pos() const { return m_Pos; }
	const bool Disap() const { return m_bDisap;}

protected:
	iPoint	m_Pos;
	bool	m_bDisap;
};
//////////////////////////////////////////////////////////////////////////
class iVanishItem : public iIListNode
{
public:
	iVanishItem(iBaseMapObject* pObj, fix32 clockWork)
	: m_pContent(pObj), m_LifeTime(clockWork), m_ClockWork(clockWork) {}

	inline void Process(fix32 t)
	{ (m_ClockWork >= t)?m_ClockWork-=t:m_ClockWork=fix32::zero;}

	inline bool Vanished() const
	{ return m_ClockWork == fix32::zero; }

	inline uint8 State(sint32 maxVal)
	{ return (uint8)((m_ClockWork * fix32(maxVal)) / m_LifeTime).floor(); }

	iBaseMapObject* DettachContent()
	{ iBaseMapObject* res = m_pContent; m_pContent=NULL; return m_pContent; }

	iBaseMapObject* Content() 
	{ return m_pContent; }

private:
	iBaseMapObject*	m_pContent;
	fix32			m_LifeTime;
	fix32			m_ClockWork;
};

//////////////////////////////////////////////////////////////////////////
class iMapItem : public iIListNode, public iBaseMapObject
{
public:
	iMapItem(const iPoint& pos, MAP_ITEM_TYPE mit, const SpriteId& sid);
	virtual ~iMapItem() {}

	inline MAP_ITEM_TYPE Type() const { return m_mapItemType; }
	inline const SpriteId& Sprite() const { return m_spriteId; }
	inline const iStringT& Message() const { return m_message; }
	inline iStringT& Message() { return m_message; }
	inline iArmy& Guard() { return m_Guard; }
	inline const iArmy& Guard() const { return m_Guard; }
	virtual bool Activate(iHero* pHero, bool bActive, bool bShowMessage);
	virtual void Serialize(iDynamicBuffer& buff) {}
	virtual TextResId NameKey()  =0;

	IMPL_TYPEAWARE( iMapItem );
	
protected:
	iArmy			m_Guard;
	iStringT		m_message;
	SpriteId		m_spriteId;
	MAP_ITEM_TYPE	m_mapItemType;
};
//
class iMapItem_Mineral : public iMapItem
{
public:
	iMapItem_Mineral(const iPoint& pos, MINERAL_TYPE mt, sint32 count);
	inline MINERAL_TYPE MineralType() const	{ return m_mineralType; }
	inline sint32 MineralCount() const { return m_count; }
	bool Activate(iHero* pHero, bool bActive, bool bShowMessage);
	void Serialize(iDynamicBuffer& buff);
	TextResId NameKey() { return TRID_MAPRES_GOLD+m_mineralType; }

	IMPL_TYPEAWARE( iMapItem_Mineral );
private:
	MINERAL_TYPE	m_mineralType;
	sint32			m_count;
};
//
class iMapItem_ManaCrystal : public iMapItem
{
public:
	iMapItem_ManaCrystal(const iPoint& pos, sint32 count);
	inline sint32 ManaPtCount() const { return m_count; }
	bool Activate(iHero* pHero, bool bActive, bool bShowMessage);
	void Serialize(iDynamicBuffer& buff);
	TextResId NameKey() { return TRID_MAPRES_MANACRYSTAL; }

	IMPL_TYPEAWARE( iMapItem_ManaCrystal );
	
private:
	sint32			m_count;
};
//
class iMapItem_CampFire : public iMapItem
{
public:
	iMapItem_CampFire(const iPoint& pos, sint32 goldCount = RAND_VAL, MINERAL_TYPE addMinType = MINERAL_UNKNOWN, sint32 addMinCount = RAND_VAL);
	bool Activate(iHero* pHero, bool bActive, bool bShowMessage);
	void Serialize(iDynamicBuffer& buff);
	TextResId NameKey() { return TRID_MAPRES_CAMPFIRE; }

	IMPL_TYPEAWARE( iMapItem_CampFire );
private:
	MINERAL_TYPE	m_addMinType;
	sint32			m_addMinCount;
	sint32			m_goldCount;
};
//
class iMapItem_Chest : public iMapItem
{
public:
	iMapItem_Chest(const iPoint& pos, sint32 count = RAND_VAL);
	inline sint32 ExpPtCount() const { return m_count; }
	bool Activate(iHero* pHero, bool bActive, bool bShowMessage);
	void Serialize(iDynamicBuffer& buff);
	TextResId NameKey() { return TRID_MAPRES_CHEST; }

	IMPL_TYPEAWARE( iMapItem_Chest );
private:
	sint32			m_count;
};
//
class iMapItem_Artifact : public iMapItem
{
public:
	iMapItem_Artifact(const iPoint& pos, uint16 aid);
	inline uint16 ArtifactId() { return m_artId; }
	bool Activate(iHero* pHero, bool bActive, bool bShowMessage);
	void Serialize(iDynamicBuffer& buff);
	TextResId NameKey() { return TRID_MAPRES_ARTIFACT; }

	IMPL_TYPEAWARE( iMapItem_Artifact );
private:
	uint16	m_artId;
};
//
class iMapItem_Lamp : public iMapItem
{
public:
	iMapItem_Lamp(const iPoint& pos, sint32 count = RAND_VAL);
	bool Activate(iHero* pHero, bool bActive, bool bShowMessage);
	void Serialize(iDynamicBuffer& buff);
	TextResId NameKey() { return TRID_MAPRES_LAMP; }
	inline sint32 Count() const { return m_count; }

	IMPL_TYPEAWARE( iMapItem_Lamp );
private:
	sint32			m_count;
};
//
class iMapItem_SpellScroll : public iMapItem
{
public:
	iMapItem_SpellScroll(const iPoint& pos, uint16 spellId);
	inline uint16 SpellId() { return m_spellId; }
	bool Activate(iHero* pHero, bool bActive, bool bShowMessage);
	void Serialize(iDynamicBuffer& buff);
	TextResId NameKey() { return TRID_MAPRES_SPELLSCROLL; }

	IMPL_TYPEAWARE( iMapItem_SpellScroll );
private:
	uint16	m_spellId;
};
//
class iMapItem_KeyGuard : public iMapItem
{
public:
	iMapItem_KeyGuard(const iPoint& pos, uint8 key);
	inline uint8 GetKeyType() { return m_key; }
	bool Activate(iHero* pHero, bool bActive, bool bShowMessage);
	void Serialize(iDynamicBuffer& buff);
	TextResId NameKey() { return TRID_MAPRES_KEYGUARD; }

	IMPL_TYPEAWARE( iMapItem_KeyGuard );
private:
	uint8	m_key;
};
//
class iMapEvent : public iIListNode, public iBaseMapObject
{
public:
	iMapEvent(const iPoint& pos);
	bool Activate(iHero* pHero, bool bActive, bool bShowMessage);

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

	IMPL_TYPEAWARE( iMapEvent );
	
private:
	iRewardsCtr	m_rewards;
	uint32		m_playerMask;
	iStringT	m_rewardMessage;
	iStringT	m_attackMessage;
	iArmy		m_guard;
};
//
class iMapGuard : public iIListNode, public iBaseMapObject
{
public:
	enum MEET_RESULT {
		MR_FLEE = 0,
		MR_ATTACK,
		MR_JOIN_OR_FLEE,
		MR_JOIN_OR_ATTACK
	};
public:
	iMapGuard(const iPoint& pos, CREATURE_TYPE ct, sint32 cnt, sint32 joinVal, GUARD_DISPOS disp = GDISP_AGGRESSIVE, bool bNotGrow = false);
	MEET_RESULT CheckMeetResult(iHero* pHero);
	bool Activate(iHero* pHero, bool bActive);
	void NewWeek(const iWeekDesc& weekDesk);
	
	inline const iCreatGroup& Creatures() const { return m_creatures; }
	inline iCreatGroup& Creatures() { return m_creatures; }
	inline GUARD_DISPOS& Disposition() { return m_disp; } 
	inline GUARD_DISPOS Disposition() const { return m_disp; } 
	inline bool& NotGrow() { return m_bNotGrow; } 
	inline bool NotGrow() const { return m_bNotGrow; } 
	inline const SpriteId& Sprite() const { return m_spriteId; }
	inline const iStringT& Message() const { return m_message; }
	inline iStringT& Message() { return m_message; }
	inline sint32 JoinVal() const { return m_joinVal; }

	IMPL_TYPEAWARE( iMapGuard );

private:
	sint32			m_joinVal;
	iCreatGroup		m_creatures;
	iStringT		m_message;
	SpriteId		m_spriteId;
	GUARD_DISPOS	m_disp;
	bool			m_bNotGrow;
};


#endif //_HMM_GAME_OBJECTS_H_

