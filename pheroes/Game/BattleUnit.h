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

#ifndef _HMM_GAME_BATTLE_UNIT_H_
#define _HMM_GAME_BATTLE_UNIT_H_

class iBattleWrapper;
//////////////////////////////////////////////////////////////////////////
extern const sint16 TAIL_OFFSET[2];
extern const sint16 HEX_MELEE_NEBS1[2][6][2];
extern const sint16 HEX_MELEE_NEBS2[2][2][8][2];

//////////////////////////////////////////////////////////////////////////
class iBattleGroup 
{
public:
	struct iDistCell {
		iDistCell() : data(0xFFFFFFFF){}
		struct Dirs {
			uint8	val;
			uint8	dir;
		};

		union {
			uint32	data;
			Dirs	dirs[2];
		};
	};

	struct iMeleeEntry {
		iMeleeEntry(const iPoint& pos, uint16 cfg = 0) : m_pos(pos), m_cfg(cfg) {}
		uint16	m_cfg; // melee config 12 bits mask
		iPoint	m_pos;
	};

	struct iShootEntry {
		iShootEntry(const iPoint& pos, sint32 penalty) : m_pos(pos), m_penalty(penalty) {}
		sint32	m_penalty;
		iPoint	m_pos;
	};

	struct iSpellEntry {
		enum DurationType { DurInfinit	= -1 };
		iSpellEntry(const iCombatSpell* pSpell, sint32 duration, sint32 param) : m_pSpell(pSpell), m_duration(duration), m_param(param) {}
		const iCombatSpell*	m_pSpell;
		sint32				m_duration;
		sint32				m_param;
	};

	typedef iMapT<iDistCell>			iDistMap;
	typedef iSimpleArray<iMeleeEntry>	iMeleeList;
	typedef iSimpleArray<iShootEntry>	iShotList;
	typedef iSimpleArray<iSpellEntry>	iSpellList;

	enum ORIENT {
		Right = 0,
		Left
	};

	enum STATE {
		Idle = 0,
		Moving,
		Rotating,
		Melee,
		Shooting,
		RecDamage,
		CastingSpell,
		ResistSpell,
		Dead,
		STATE_COUNT
	};
	
	enum MORLUCK_MOD {
		MLM_NEUTRAL = 0,
		MLM_NEGATIVE,
		MLM_POSITIVE
	};

	enum AttackFlags {
		RangeAttack	= 0x1,
		LichCloud	= 0x2,
	};

public:
	// c-tor, d-tor
	iBattleGroup(const iCreatGroup& cg, sint32 idx, iBattleMember* pOwner, iBattleWrapper* pWrapper, sint32 moraleModifier);

	// Grop specs
	inline const iBattleMember* Owner() const { return m_pOwner; }
	inline iBattleMember* Owner() { return m_pOwner; }
	inline CREATURE_TYPE Type() const { return m_creatType; }
	inline sint32 Count() const { return m_creatCount; }
	inline sint32 InitialCount() const { return m_initCount; }
	inline uint32 ArmyGroupIndex() const { return m_idx; }
	inline ORIENT Orient() const { return m_Orient; }
	inline STATE State() const { return m_State; }
	inline bool IsAlive() const { return m_State != Dead; }
	inline bool CanShoot() const { return CREAT_DESC[m_creatType].shots != 0; }
	inline sint32 Shots() const { return m_shots; }
	inline sint32 HitPointsLeft() const { return m_hitPoints; }
	inline sint32 HitPoints() const { return CREAT_DESC[m_creatType].hits+m_furtSkills.Value(FSK_HITS); }
	inline sint32 TotalCasHitPoints() const { return iMAX<sint32>(0, m_initCount * HitPoints() - TotalHitPoints());}
	inline sint32 TotalHitPoints() const { return (HitPoints()*(m_creatCount-1))+m_hitPoints; }
	inline sint32 AttackSkill() const { return iMAX<sint32>(1, CREAT_DESC[m_creatType].attack + m_furtSkills.Value(FSK_ATTACK)); }
	inline sint32 DefenceSkill() const { return iMAX<sint32>(1, CREAT_DESC[m_creatType].defence + m_furtSkills.Value(FSK_DEFENCE)); }
	inline bool CanWait() const { return !m_bWaited;}
	inline bool CanAct() const { return !m_bBlinded; }
	inline bool CanRetail() const { return (!m_bBlinded || m_retPenalty!=0) && ((CREAT_DESC[m_creatType].perks&CPERK_RETALTOALL) || m_retails < (1+m_furtSkills.Value(FSK_COUNTERSTRIKE)));}
	inline void SetRetailed() { m_retails++; }
	inline uint8 Size() const { return m_Size; }
	inline sint16 Morale() const { return iCLAMP<sint16>(-3,3,(sint16)((CREAT_DESC[m_creatType].perks&CPERK_UNDEAD)?0:m_furtSkills.Value(FSK_MORALE))); }
	inline sint16 Luck() const { return iCLAMP<sint16>(-3,3,(sint16)m_furtSkills.Value(FSK_LUCK)); }
	inline MORLUCK_MOD RoundMorale() const { return m_roundMorale; }
	inline void ResetRoundMorale() { m_roundMorale = MLM_NEUTRAL; }
	MORLUCK_MOD CalcLuck() const;

	// Common creature abilities
	inline uint8 TransType() const { return CREAT_DESC[m_creatType].transType; }
	inline uint32 Perks() const { return CREAT_DESC[m_creatType].perks; }
	inline bool HasPerk(CREAT_PERK perk) const { return (CREAT_DESC[m_creatType].perks & perk) != 0; }
	inline sint32 Speed() const { return iMAX<sint32>(1,CREAT_DESC[m_creatType].speed + m_furtSkills.Value(FSK_SPEED)); }
	inline sint32 MinDamage() const { return CREAT_DESC[m_creatType].damage_min; }
	inline sint32 MaxDamage() const { return CREAT_DESC[m_creatType].damage_max; }
	inline sint32 ActMinDamage() const { return (m_blessState>0)?CREAT_DESC[m_creatType].damage_max:CREAT_DESC[m_creatType].damage_min; }
	inline sint32 ActMaxDamage() const { return (m_blessState<0)?CREAT_DESC[m_creatType].damage_min:CREAT_DESC[m_creatType].damage_max; }

	// Dislocation helpers
	inline bool IsGroupCell(const iPoint& pos) const { return m_Pos == pos || (m_Size==2 && TailPos() == pos); }
	inline const iPoint& Pos() const { return m_Pos; }
	inline iPoint TailPos() const { if (m_Size == 1) return m_Pos; return iPoint(m_Pos.x + TAIL_OFFSET[m_Orient], m_Pos.y); }
	
	// Moat
	inline bool InMoat() const { return m_bInMoat; }
	void EnterMoat();
	void LeaveMoat();

	// Spells
	void AddSpell(const iCombatSpell* pSpell, sint32 rounds, sint32 param);
	void RemoveSpell(MAGIC_SPELL spell);
	void RemoveSpellAt(uint32 idx);
	inline const iSpellList& SpellList() const { return m_spells; }
	inline iSpellList& SpellList() { return m_spells; }
	SPELL_DISPOS SpellsDispos() const;
	inline void SetSpellImmunity(uint32 spim) { m_spImmunity = spim; }
	inline uint32 SpellImmunity() const { return m_spImmunity; }
	inline bool SpellImmunity(SPELL_LEVEL splevel) const { return (m_spImmunity & (1<<splevel)) != 0; }

	// Bless / Curse state
	inline sint32 BlessState() const { return m_blessState; }
	inline void SetBlessState(sint32 newState) { m_blessState = newState; }

	// Blind state
	inline void SetBlindState(bool bBlinded, sint32 penalty) { m_bBlinded = bBlinded; m_retPenalty = penalty; }
	inline bool IsBlinded() const { return m_bBlinded; }
	inline sint32 GetRetPenalty() const { return m_retPenalty; }

	// 
	void NewRound();
	void DestroyGroup();
	void Wait();
	void Defend();
	void GoodMorale();
	void BadMorale();
	void GoodLuck();
	void BadLuck();
	uint32 CalcJoustingBonus(const iBattleGroup* pTarget) const;
	sint32 CalcDamage(const iBattleGroup* pTarget, sint32 damage, bool bRange, sint32 penalty, uint32 joustBonus) const;
	void Attack(iBattleGroup* pTarget, uint32 aflags, sint32 penalty, uint32 joustBonus, MORLUCK_MOD luck);
	sint32 ReceiveDamage(sint32& damage, bool bRange);
	sint32 Half(sint32& damage);
	bool CanResurrect() const;
	sint32 iBattleGroup::CountResurrected(sint32 hp);
	sint32 Resurrect(sint32 hp);
	void Restore(sint32 hp);
	void SetState(STATE nState);
	void Place(const iPoint& pos);
	void Rotate();
	void GetMeleePosition(const iBattleGroup* pTarget, const iPoint& pos, uint16 mdir, iPoint& meleePos, uint8& meleeOrient);

	//
	void Synchronize(iArmy& army);
	void InitPassMap(const iBattleMap& obsMap, const iCastleFort* pFort);
	void UpdatePassMap(const iBattleMap& obsMap, const iSimpleArray<iBattleGroup*>& mg, const iSimpleArray<iBattleGroup*>& eg, const iCastleFort* pFort);
	
	inline const iBattleMap& PassMap() const { return m_passMap; }
	inline iBattleMap& PassMap() { return m_passMap; }
	
	inline const iDistMap& DistMap() const { return m_distMap; }
	inline iDistMap& DistMap() { return m_distMap; }

	void AddMeleeEntry(const iPoint& pos, uint16 mdir);
	void AddMeleeMask(const iPoint& pos, uint16 mmask);
	void AddShootEntry(const iPoint& pos, sint32 penalty);
	void AddPotTargetMask(const iPoint& pos, uint16 mmask);

	inline uint32 MeleeListCount() const { return m_meleeList.GetSize(); }
	inline const iMeleeEntry* GetMeleeEntry(uint32 idx) const { return &m_meleeList[idx]; }
	inline const iMeleeEntry* GetMeleeEntry(const iPoint& pos) const
	{ for (uint32 xx=0; xx<m_meleeList.GetSize(); ++xx) if (m_meleeList[xx].m_pos == pos) return &m_meleeList[xx]; return NULL; }

	inline uint32 ShootListCount() const { return m_shotList.GetSize(); }
	inline const iShootEntry* GetShootEntry(uint32 idx) const { return &m_shotList[idx]; }
	inline const iShootEntry* GetShootEntry(const iPoint& pos) const
	{ for (uint32 xx=0; xx<m_shotList.GetSize(); ++xx) if (m_shotList[xx].m_pos == pos) return &m_shotList[xx]; return NULL; }

	inline uint32 PotTargetsCount() const { return m_potTargets.GetSize(); }
	inline const iMeleeEntry* GetPotTargetEntry(uint32 idx) const { return &m_potTargets[idx]; }
	inline const iMeleeEntry* GetPotTargetEntry(const iPoint& pos) const
	{ for (uint32 xx=0; xx<m_potTargets.GetSize(); ++xx) if (m_potTargets[xx].m_pos == pos) return &m_potTargets[xx]; return NULL; }

	inline bool CanMove(sint32 x, sint32 y) const 
	{ return m_passMap.IsValidPos(x,y) && ((m_passMap.GetAt(x,y) & 0x80) != 0 || IsGroupCell(iPoint(x,y))); }

	inline sint32 Casualties() const { return m_casualties; }
	inline iBattleWrapper* BattleWrapper() { return m_pWrapper; }

	inline iFurtSkills& FurtSkills() { return m_furtSkills; }

protected:
	iFurtSkills		m_furtSkills;

private:
	uint32			m_spImmunity;
	iSpellList		m_spells;
	bool			m_bBlinded;
	sint32			m_retPenalty;

	iBattleMember*	m_pOwner;
	iBattleWrapper*	m_pWrapper;
	iPoint			m_Pos;
	uint8			m_Size;
	CREATURE_TYPE	m_creatType;
	sint32			m_creatCount;
	sint32			m_initCount;
	sint32			m_idx;
	sint32			m_blessState;

	sint32			m_casualties;

	bool			m_bWaited;
	sint32			m_toDefend;
	sint32			m_retails;
	bool			m_bMoraleAttack;
	bool			m_bInMoat;
	MORLUCK_MOD		m_roundMorale;
	sint32			m_hitPoints;
	sint32			m_shots;
	STATE			m_State;
	ORIENT			m_Orient;

	iDistMap		m_distMap;
	iBattleMap		m_passMap;
	iMeleeList		m_meleeList;
	iMeleeList		m_potTargets;
	iShotList		m_shotList;
};

/*
 *	Battle Unit
 */
class iBattleUnit : public TypeAware
{
public:
	IMPL_TYPEAWARE( iBattleUnit );
	enum UnitType {
		Hero,
		CreatGroup,
		Catapult,
		Turret,
		Moat
	};

public:
	iBattleUnit(UnitType ut, iBattleMember* pOwner) : m_ut(ut), m_pOwner(pOwner) {}
	virtual ~iBattleUnit() {}

	inline UnitType GetUnitType() const { return m_ut; }
	inline iBattleMember* Owner() { return m_pOwner; }

	virtual bool IsAlive() const  =0;
	virtual bool CanWait() const  =0;
	virtual sint32 Speed() const  =0;

protected:
	iBattleMember*	m_pOwner;
	UnitType		m_ut;
};
//
class iBattleUnit_Hero : public iBattleUnit
{
public:
	IMPL_TYPEAWARE( iBattleUnit_Hero );
	iBattleUnit_Hero(iBattleMember* pOwner) : iBattleUnit(CreatGroup, pOwner) 
	{
		if (!DynamicCast<iBattleMember_Hero*>(pOwner)) {
			iBattleMember_Castle* pCastle = DynamicCast<iBattleMember_Castle*>(pOwner);
			check(pCastle->GetVisitor());
		}
	}

	inline iHero* GetHero() 
	{ 
		if (iBattleMember_Hero* pHero = DynamicCast<iBattleMember_Hero*>(m_pOwner)) {
			return pHero->GetHero();
		} else if (iBattleMember_Castle* pCastle = DynamicCast<iBattleMember_Castle*>(m_pOwner)){
			return pCastle->GetVisitor();
		}
		check(0);
		return NULL;
	}


	bool IsAlive() const { return true; }
	bool CanWait() const { return false; } 
	sint32 Speed() const { return 0xFF; }
};
//
class iBattleUnit_CreatGroup : public iBattleUnit
{
public:
	IMPL_TYPEAWARE( iBattleUnit_CreatGroup );
	iBattleUnit_CreatGroup(iBattleGroup* pCreatGroup) : iBattleUnit(CreatGroup, pCreatGroup->Owner()), m_pCreatGroup(pCreatGroup) {}

	inline iBattleGroup* GetCreatGroup() { return m_pCreatGroup; }
	bool IsAlive() const { return m_pCreatGroup->IsAlive(); }
	bool CanWait() const { return m_pCreatGroup->CanWait(); } 
	sint32 Speed() const { return m_pCreatGroup->Speed(); }
protected:
	iBattleGroup*	m_pCreatGroup;
};
//
class iBattleUnit_Catapult : public iBattleUnit
{
public:
	IMPL_TYPEAWARE( iBattleUnit_Catapult );
	iBattleUnit_Catapult(iBattleMember* pOwner) : iBattleUnit(Catapult, pOwner) {}
	bool IsAlive() const { return true; }
	bool CanWait() const { return false; }
	sint32 Speed() const { return 0xFF; }
};
//
class iBattleUnit_Turret : public iBattleUnit
{
public:
	IMPL_TYPEAWARE( iBattleUnit_Turret );
	iBattleUnit_Turret(iBattleMember* pOwner, iCastleFort::ElementType et, iBattleWrapper* pWrapper) : iBattleUnit(Turret, pOwner), m_et(et), m_pWrapper(pWrapper) {}
	bool IsAlive() const { return true; }
	bool CanWait() const { return false; }
	sint32 Speed() const { return 0xFF; }

	void Attack(iBattleGroup* pTarget);

	iBattleWrapper*				m_pWrapper;
	iCastleFort::ElementType	m_et;
};

//
class iBattleUnit_Moat : public iBattleUnit
{
public:
	IMPL_TYPEAWARE( iBattleUnit_Turret );
	iBattleUnit_Moat(iBattleMember* pOwner, iBattleWrapper* pWrapper) : iBattleUnit(Moat, pOwner), m_pWrapper(pWrapper) {}
	bool IsAlive() const { return true; }
	bool CanWait() const { return false; }
	sint32 Speed() const { return 0xFF; }

	void Attack(iBattleGroup* pTarget);

	iBattleWrapper*				m_pWrapper;
};

/*
 *	Helper functions
 */

// Units color
inline iStringT GetUnitsColor(PLAYER_ID pid) { return (pid == PID_NEUTRAL)?_T("#FAAA"):PLAYER_TEXT_COLORS[pid]; }


// Calculates delta distance between two cells
inline uint32 BattleCellsDelta(sint32 px1, sint32 py1, sint32 px2, sint32 py2) 
{ 
	sint32 nx1 = px1 - py1/2;
	sint32 ny1 = px1 + (py1+1)/2;
	sint32 nx2 = px2 - py2/2;
	sint32 ny2 = px2 + (py2+1)/2;

	sint32 dx = nx2 - nx1; 
	sint32 dy = ny2 - ny1;

	if (iSIGN(dx) == iSIGN(dy)) {
		return iMAX(iABS(dx), iABS(dy));
	} 
	return iABS(dx) + iABS(dy);
}

inline uint8 BattleCellsOrient(sint32 fx, sint32 fy, sint32 tx, sint32 ty, uint8 orient)
{
	sint32 xdiff = (orient==0)?(tx - fx):(fx-tx);
	if ( (!(fy&1) && (ty&1) && !orient) || ((fy&1) && !(ty&1) && orient) ) xdiff -= 1;

	if (xdiff < 0) orient = !orient;
	return orient;
}

inline uint8 BattleDirOrient(uint8 dir)
{
	if (dir > 0 && dir < 4) return iBattleGroup::Right;
	else return iBattleGroup::Left;
} 

#endif //_HMM_GAME_BATTLE_UNIT_H_

