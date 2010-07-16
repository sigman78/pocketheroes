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

#ifndef _HMM_GAME_MAGIC_H_
#define _HMM_GAME_MAGIC_H_

// spell label descriptors
struct SPELL_LABEL_DESC_STRUCT {
	uint8	dispos;
	uint16	trid;
	uint8	reserved;
};
extern const SPELL_LABEL_DESC_STRUCT SPELL_LABEL_DESCRIPTORS[SLB_COUNT];

// Magic school colors (Air, Earth, Fire, Water), (Background, Foreground)
extern const iDib::pixel MSCH_COLORS[MST_COUNT][2];

/*
 *	Base spell class
 */
class iHero;
class iBaseSpell : public TypeAware
{
public:
	IMPL_TYPEAWARE(iBaseSpell)

	iBaseSpell(MAGIC_SPELL spellId);
	virtual ~iBaseSpell() {}

	static MAGIC_SCHOOL_LEVEL GetSchoolLevel(MAGIC_SPELL spellId, const iHero* pCaster);
	static uint8 GetSpellCost(MAGIC_SPELL spellId, const iHero* pCaster);

	// inlines
	inline TextResId NameKey() const { return TRID_SPNAME_MAGICARROW+m_spellId; }
	inline TextResId DescKey(MAGIC_SCHOOL_LEVEL level) const { return TRID_SPDESC_MAGICARROW_NON+m_spellId*MSL_COUNT+level; }
	inline SpriteId Icon() const { return PDGG_SPELL_ICONS+m_spellId;}
	inline uint8 Type() const { return SPELL_DESCRIPTORS[m_spellId].type; }
	inline uint8 Level() const { return SPELL_DESCRIPTORS[m_spellId].level; }
	inline uint8 School() const { return SPELL_DESCRIPTORS[m_spellId].school; }
	inline uint8 Cost(MAGIC_SCHOOL_LEVEL level) const { uint8 res = SPELL_DESCRIPTORS[m_spellId].bcost; if (level > MSL_NONE) res -= (SPELL_DESCRIPTORS[m_spellId].level+1); return res; }
	inline uint8 SpellClass() const { return SPELL_DESCRIPTORS[m_spellId].spClass; }
	inline MAGIC_SPELL Id() const { return m_spellId; }
	inline MAGIC_SCHOOL_LEVEL GetSchoolLevel(const iHero* pCaster) const { return GetSchoolLevel(m_spellId, pCaster); }
	
protected:
	MAGIC_SPELL m_spellId;
};

/*
 *	Overland spell
 */
class iOverlandSpell : public iBaseSpell
{
public:
	IMPL_TYPEAWARE(iOverlandSpell)
	iOverlandSpell(MAGIC_SPELL spellId)  : iBaseSpell(spellId) { check(SPELL_DESCRIPTORS[spellId].type == SPT_OVERLAND); }
	virtual ~iOverlandSpell() {}

	// virtuals
	virtual bool CanCast(iHero* pCaster);
	virtual bool Cast(iHero* pCaster);
	virtual void OnRemove(iHero* pCaster, sint32 param);
};
//////////////////////////////////////////////////////////////////////////
class iSpell_TownPortal : public iOverlandSpell
{
public:
	iSpell_TownPortal(MAGIC_SPELL spellId)  : iOverlandSpell(spellId) {}
	bool Cast(iHero* pCaster);
};
//////////////////////////////////////////////////////////////////////////
class iSpell_Vision : public iOverlandSpell
{
public:
	iSpell_Vision(MAGIC_SPELL spellId)  : iOverlandSpell(spellId) {}
	bool Cast(iHero* pCaster);
	void OnRemove(iHero* pCaster, sint32 param);
};

/*
 *	Combat spell
 */
class iBattleGroup;
class iBattleUnit_Hero;
class iCombatSpell : public iBaseSpell
{
public:
	IMPL_TYPEAWARE(iCombatSpell)
	iCombatSpell(MAGIC_SPELL spellId);
	virtual ~iCombatSpell() {}

	//
	inline uint16 TargetMode(MAGIC_SCHOOL_LEVEL level) const { return SPELL_DESCRIPTORS[m_spellId].eff[level].tgtMode; }
	inline uint16 TargetType(MAGIC_SCHOOL_LEVEL level) const { return SPELL_DESCRIPTORS[m_spellId].eff[level].tgtType; }
	inline bool NeedTarget(MAGIC_SCHOOL_LEVEL level) const { return TargetMode(level) == STM_CREAT_GROUP || TargetMode(level) == STM_BALL_SET || TargetMode(level) == STM_BLAST_SET || TargetMode(level) == STM_RING_SET; }
	inline bool HasCoverArea(MAGIC_SCHOOL_LEVEL level) const { return TargetMode(level) == STM_BALL_SET || TargetMode(level) == STM_BLAST_SET || TargetMode(level) == STM_RING_SET; }
	inline SPELL_DISPOS SpellDispos() const { return (SPELL_DISPOS)SPELL_LABEL_DESCRIPTORS[SPELL_DESCRIPTORS[m_spellId].label].dispos; }
	inline TextResId SpellLabel() const { check(SPELL_DESCRIPTORS[m_spellId].label != SLB_NONE); return SPELL_LABEL_DESCRIPTORS[SPELL_DESCRIPTORS[m_spellId].label].trid; }

	// Virtuals
	virtual bool CanCast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const;
	virtual bool Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const;
	virtual void OnRemove(iBattleGroup* pTarget, sint32 param) const { }
	virtual iStringT ActionText(iHero* pCaster) const { return iStringT(); }
	virtual bool AffectsToDead() const { return false; }
};
//////////////////////////////////////////////////////////////////////////
class iSpell_Damage : public iCombatSpell
{
public:
	iSpell_Damage(MAGIC_SPELL spellId) : iCombatSpell(spellId) {}
	sint32 EstimateDamage(iHero* pCaster, iBattleGroup* pTarget = NULL) const;
	bool Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const;
	iStringT ActionText(iHero* pCaster) const;
};
//////////////////////////////////////////////////////////////////////////
class iSpell_FurtSkill : public iCombatSpell
{
public:
	iSpell_FurtSkill(MAGIC_SPELL spellId) : iCombatSpell(spellId) {}
	bool Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const;
	void OnRemove(iBattleGroup* pTarget, sint32 param) const;
};
//////////////////////////////////////////////////////////////////////////
class iSpell_DisRay : public iCombatSpell
{
public:
	iSpell_DisRay(MAGIC_SPELL spellId) : iCombatSpell(spellId) {}
	bool Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const;
	void OnRemove(iBattleGroup* pTarget, sint32 param) const;
};

//////////////////////////////////////////////////////////////////////////
class iSpell_Resurrect : public iCombatSpell
{
public:
	iSpell_Resurrect(MAGIC_SPELL spellId) : iCombatSpell(spellId) {}
	sint32 EstimateHitPoints(iHero* pCaster) const;
	bool CanCast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const;
	bool Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const;
	iStringT ActionText(iHero* pCaster) const;
	bool AffectsToDead() const { return true; }
};
//////////////////////////////////////////////////////////////////////////
class iSpell_Summon : public iCombatSpell
{
public:
	iSpell_Summon(MAGIC_SPELL spellId) : iCombatSpell(spellId) {}
	sint32 EstimateQuant(iHero* pCaster) const;
	CREATURE_TYPE CreatType() const;
	bool Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const;
};
//////////////////////////////////////////////////////////////////////////
class iSpell_Bless: public iCombatSpell
{
public:
	iSpell_Bless(MAGIC_SPELL spellId) : iCombatSpell(spellId) {}
	bool Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const;
	void OnRemove(iBattleGroup* pTarget, sint32 param) const;
};
//////////////////////////////////////////////////////////////////////////
class iSpell_Blind : public iCombatSpell
{
public:
	iSpell_Blind(MAGIC_SPELL spellId) : iCombatSpell(spellId) {}
	bool Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const;
	void OnRemove(iBattleGroup* pTarget, sint32 param) const;
};
//////////////////////////////////////////////////////////////////////////
class iSpell_Earthquake : public iCombatSpell
{
public:
	iSpell_Earthquake(MAGIC_SPELL spellId) : iCombatSpell(spellId) {}
	bool CanCast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const;
	bool Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const;
};
//////////////////////////////////////////////////////////////////////////
class iSpell_Dispel : public iCombatSpell
{
public:
	iSpell_Dispel(MAGIC_SPELL spellId) : iCombatSpell(spellId) {}
	bool CanCast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const;
	bool Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const;
};
//////////////////////////////////////////////////////////////////////////
class iSpell_Cure : public iCombatSpell
{
public:
	iSpell_Cure(MAGIC_SPELL spellId) : iCombatSpell(spellId) {}
	sint32 EstimateHitPoints(iHero* pCaster) const;
	bool CanCast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const;
	bool Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const;
};
//////////////////////////////////////////////////////////////////////////
class iSpell_Antimagic : public iCombatSpell
{
public:
	iSpell_Antimagic(MAGIC_SPELL spellId) : iCombatSpell(spellId) {}
	bool Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const;
	void OnRemove(iBattleGroup* pTarget, sint32 param) const;
};
//////////////////////////////////////////////////////////////////////////
class iSpell_Prayer : public iCombatSpell
{
public:
	iSpell_Prayer(MAGIC_SPELL spellId) : iCombatSpell(spellId) {}
	bool Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const;
	void OnRemove(iBattleGroup* pTarget, sint32 param) const;
};

/*
 *	Spell Book
 */
class iSpellBook
{
public:
	iSpellBook();
	void Add(uint16 spellId);
	void Add(const iSpellList& spells);
	void AddFavorite(uint16 spellId);
	const iBaseSpell* SpellByIdx(uint32 idx) const;
	const iBaseSpell* FavoriteByIdx(uint32 idx) const;
	inline uint16 Spell(uint32 idx) const { return m_spells[idx]; }
	inline uint16 Favorite(uint32 idx) const { return m_favorites[idx]; }
	void Reset();

	inline uint32 SpellsCount() const { return m_spells.GetSize(); }
	inline const iSpellList& Spells() const { return m_spells; }

	inline uint32 FavoritesCount() const { return m_favorites.GetSize(); }
	inline const iSpellList& Favorites() const { return m_favorites; }

	inline bool HasSpell(uint16 spellId) const { return m_spells.Find(spellId) != iSpellList::NOT_FOUND; }
	
private:
	iSpellList	m_spells;
	iSpellList	m_favorites;
};

#endif //_HMM_GAME_MAGIC_H_

