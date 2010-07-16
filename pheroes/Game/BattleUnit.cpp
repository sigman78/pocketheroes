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
#include "battlePF.h"
// Tail offsets [right, left direction]
const sint16 TAIL_OFFSET[2] = { -1, +1 };

// Cell's nebs for single cell creatures [even, odd row][id 0-5][x,y offset] (clockwise from NW)
const sint16 HEX_MELEE_NEBS1[2][6][2] = {
	{
		{0,-1}, {1,-1},{1,0},{1,1},{0,1},{-1,0}
	},{
		{-1,-1},{0,-1},{1,0},{0,1},{-1,1},{-1,0}
	}
};

// Cell's nebs for double cell creatures [even, odd row][right,left orientation][id 0-7][x,y offset] (clockwise from NW)
const sint16 HEX_MELEE_NEBS2[2][2][8][2] = {
	{
		{
			{-1,-1},{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-2,0}
		},{
			{0,-1},{1,-1},{2,-1},{2,0},{2,1},{1,1},{0,1},{-1,0}
		}
	},{
		{
			{-2,-1},{-1,-1},{0,-1},{1,0},{0,1},{-1,1},{-2,1},{-2,0}
		},{
			{-1,-1},{0,-1},{1,-1},{2,0},{1,1},{0,1},{-1,1},{-1,0}
		}
	}
};


/*
 *	Battle Group
 */
iBattleGroup::iBattleGroup(const iCreatGroup& cg, sint32 idx, iBattleMember* pOwner, iBattleWrapper* pWrapper, sint32 moraleModifier)
: m_creatType(cg.Type())
, m_initCount(cg.Count())
, m_creatCount(cg.Count())
, m_idx(idx)
, m_bWaited(false)
, m_toDefend(0)
, m_spImmunity(0)
, m_retails(0)
, m_bMoraleAttack(false)
, m_bInMoat(false)
, m_roundMorale(MLM_NEUTRAL)
, m_bBlinded(false)
, m_retPenalty(1)
, m_blessState(0)
, m_hitPoints(CREAT_DESC[cg.Type()].hits + pOwner->FurtSkills().Value(FSK_HITS))
, m_shots(CREAT_DESC[cg.Type()].shots)
, m_Pos(cInvalidPoint)
, m_State(Idle)
, m_pOwner(pOwner)
, m_pWrapper(pWrapper)
, m_Orient(ORIENT(pOwner->GetSide()))
, m_Size(CREAT_DESC[cg.Type()].size)
, m_passMap(13,11)
, m_distMap(13,11)
, m_casualties(0)
, m_furtSkills(pOwner->FurtSkills())
{ 
	// special creatures processing
	if (CREAT_DESC[m_creatType].perks & CPERK_40PROCRESIST) m_furtSkills.Value(FSK_RESIST) += 40;

	m_furtSkills.Value(FSK_MORALE) += moraleModifier;

	NewRound();
}

void iBattleGroup::EnterMoat()
{
	check(!m_bInMoat);
	m_bInMoat = true;
	m_furtSkills.Value(FSK_DEFENCE) -= 3;
}

void iBattleGroup::LeaveMoat()
{
	check(m_bInMoat);
	m_bInMoat = false;
	m_furtSkills.Value(FSK_DEFENCE) += 3;
}

void iBattleGroup::AddSpell(const iCombatSpell* pSpell, sint32 rounds, sint32 param)
{
	m_spells.Add(iSpellEntry(pSpell, rounds, param));
}

void iBattleGroup::RemoveSpell(MAGIC_SPELL spell)
{
	for (uint32 xx=0; xx<m_spells.GetSize();) {
		if (m_spells[xx].m_pSpell->Id() == spell) {
			m_spells[xx].m_pSpell->OnRemove(this, m_spells[xx].m_param);
			m_spells.RemoveAt(xx);
		} else ++xx;
	}
}

void iBattleGroup::RemoveSpellAt(uint32 idx)
{
	m_spells[idx].m_pSpell->OnRemove(this, m_spells[idx].m_param);
	m_spells.RemoveAt(idx);
}

SPELL_DISPOS iBattleGroup::SpellsDispos() const
{
	sint32 positive = 0;
	sint32 negative = 0;
	for (uint32 xx=0; xx<m_spells.GetSize() && (!positive || !negative); ++xx) {
		if (m_spells[xx].m_pSpell->SpellDispos() == SPD_POSITIVE) ++positive;
		else if (m_spells[xx].m_pSpell->SpellDispos() == SPD_NEGATIVE) ++negative;
	}

	if (positive && negative) return SPD_NEUTRAL;
	else if (positive) return SPD_POSITIVE;
	else if (negative) return SPD_NEGATIVE;
	else return SPD_NONE;
}

void iBattleGroup::NewRound()
{
	m_bWaited = false;
	m_retails = 0;

	// Defend
	if (m_toDefend) {
		m_furtSkills.Value(FSK_DEFENCE) -= m_toDefend;
		m_toDefend = 0;
	}

	// Calculate morale state
	sint16 mm = Morale();
	sint32 rval = gGame.Map().Rand(100);
	if (mm > 0 && (mm*5 > rval)) {
		m_roundMorale = MLM_POSITIVE;
	} else if (mm < 0 && ((-mm)*5 > rval)) {
		m_roundMorale = MLM_NEGATIVE;
	} else {
		m_roundMorale = MLM_NEUTRAL;
	}

	// Process special perks
	if (CREAT_DESC[m_creatType].perks & CPERK_REGENERATES) {
		m_hitPoints = HitPoints();
	}

	// Update spells
	for (uint32 xx=0; xx<m_spells.GetSize();) {
		if (m_spells[xx].m_duration == iSpellEntry::DurInfinit) {
			++xx;
		} else if (m_spells[xx].m_duration == 1) {
			m_spells[xx].m_pSpell->OnRemove(this, m_spells[xx].m_param);
			m_spells.RemoveAt(xx);
			//OutputDebugString(_T("Remove spell!\n"));
		} else {
			m_spells[xx].m_duration--;
			++xx;
			//OutputDebugString(_T("Decrease spell duration.\n"));
		}
	}
}

iBattleGroup::MORLUCK_MOD iBattleGroup::CalcLuck() const
{
	sint16 lm = Luck();
	sint32 rval = gGame.Map().Rand(100);
	if (lm > 0 && (lm*5 > rval)) {
		return MLM_POSITIVE;
	} else if (lm < 0 && ((-lm)*5 > rval)) {
		return MLM_NEGATIVE;
	} else {
		return MLM_NEUTRAL;
	}
}

void iBattleGroup::DestroyGroup()
{
	m_casualties += m_creatCount;
	m_creatCount = 0;
	m_hitPoints = 0;
	SetState(Dead);
}

void iBattleGroup::Wait()
{
	check(!m_bWaited);
	m_bWaited = true;
}

void iBattleGroup::Defend()
{
	check(!m_toDefend);
	m_toDefend = (( 3 + DefenceSkill() ) >> 2);
	m_furtSkills.Value(FSK_DEFENCE) += m_toDefend;
}

void iBattleGroup::GoodMorale()
{
	m_pWrapper->AddLogEvent(iStringT(_T("#S0")) + GetUnitsColor(m_pOwner->Owner()) + iFormat(gTextMgr[TRID_MSG_GOOD_MORALE], gTextMgr[TRID_CREATURE_PEASANT_F3+m_creatType*3]));
	m_pWrapper->AddCellEvent(iFormat(_T("#I%04X"), PDGG_ICN_MORALE), m_Pos);
}

void iBattleGroup::BadMorale()
{
	m_pWrapper->AddLogEvent(iStringT(_T("#S0")) + GetUnitsColor(m_pOwner->Owner()) + iFormat(gTextMgr[TRID_MSG_BAD_MORALE], gTextMgr[TRID_CREATURE_PEASANT_F3+m_creatType*3]));
	m_pWrapper->AddCellEvent(iFormat(_T("#I%04X"), PDGG_ICN_MORALE+2), m_Pos);
}

void iBattleGroup::GoodLuck()
{
	m_pWrapper->AddLogEvent(iStringT(_T("#S0")) + GetUnitsColor(m_pOwner->Owner()) + iFormat(gTextMgr[TRID_MSG_GOOD_LUCK], gTextMgr[TRID_CREATURE_PEASANT_F3+m_creatType*3]));
	m_pWrapper->AddCellEvent(iFormat(_T("#I%04X"), PDGG_ICN_LUCK), m_Pos);
}

void iBattleGroup::BadLuck()
{
	m_pWrapper->AddLogEvent(iStringT(_T("#S0")) + GetUnitsColor(m_pOwner->Owner()) + iFormat(gTextMgr[TRID_MSG_BAD_LUCK], gTextMgr[TRID_CREATURE_PEASANT_F3+m_creatType*3]));
	m_pWrapper->AddCellEvent(iFormat(_T("#I%04X"), PDGG_ICN_LUCK+2), m_Pos);
}

uint32 iBattleGroup::CalcJoustingBonus(const iBattleGroup* pTarget) const
{
	if (CREAT_DESC[m_creatType].perks & CPERK_JOUSTING) {
		return iCLAMP<sint32>(0, 50, (BattleCellsDelta(m_Pos.x, m_Pos.y, pTarget->Pos().x, pTarget->Pos().y)-1)*5);
	}
	return 0;
}

sint32 iBattleGroup::CalcDamage(const iBattleGroup* pTarget, sint32 damage, bool bRange, sint32 penalty, uint32 joustBonus) const
{
	float udmg = (float)damage;

	// NB: Replace with fix-point but beware 64K overflow!!
	//////////////////////////////////////////////////////////////
	
	float adif = float(AttackSkill() - pTarget->DefenceSkill() );
	// Increase melle attack skill according to Melee attack skill
	if (bRange) adif += m_furtSkills.Value(FSK_RANGEATTACK);
	else adif += m_furtSkills.Value(FSK_MELEEATTACK);

	if (adif >= 0) damage = iMAX<sint32>(1,(sint32)(udmg * (1.0f + 0.1f*iMIN(adif,20.0f)) * m_creatCount));
	else damage = iMAX<sint32>(1,(sint32)(udmg * (1.0f + 0.05f*iMAX(adif,-16.0f)) * m_creatCount));

	// Check half damage cases (far range attack and melee for shooters w/o NO_MELEE_PENALTY perk)
	if (bRange && penalty > 1) {
		damage = iMAX<sint32>(damage/penalty,1);
	} else if (!bRange && CREAT_DESC[m_creatType].shots && !(CREAT_DESC[m_creatType].perks&CPERK_NOMELEEPENALTY)) {
		damage = iMAX<sint32>(damage/2,1);
	}

	// Increase melee damage according to Offence skill
	if (!bRange && m_furtSkills.Value(FSK_OFFENCE)) {
		damage += (damage * m_furtSkills.Value(FSK_OFFENCE))/100;
	}
	
	// Increase range attack damage according to Archery skill
	if (bRange && m_furtSkills.Value(FSK_ARCHERY)) {
		damage += (damage * m_furtSkills.Value(FSK_ARCHERY))/100;
	}

	// Increase damage according to jousting bonus
	if (joustBonus) damage += (damage * joustBonus) / 100;

	// Decrease damage according to Armorer skill
	if (pTarget->m_furtSkills.Value(FSK_ARMORER)) {
		damage -= iMIN((damage * pTarget->m_furtSkills.Value(FSK_ARMORER))/100, damage-1);
	}

	// Decrease damage according to air shield skill
	if (bRange && pTarget->m_furtSkills.Value(FSK_AIRSHIELD)) {
		damage -= iMIN((damage * pTarget->m_furtSkills.Value(FSK_AIRSHIELD))/100, damage-1);
	}

	// Decrease damage according to shield skill
	if (!bRange && pTarget->m_furtSkills.Value(FSK_SHIELD)) {
		damage -= iMIN((damage * pTarget->m_furtSkills.Value(FSK_SHIELD))/100, damage-1);
	}

	return damage;
}

void iBattleGroup::Attack(iBattleGroup* pTarget, uint32 aflags, sint32 penalty, uint32 joustBonus, MORLUCK_MOD luck)
{
	bool bRange = (aflags & RangeAttack) != 0;

	// calculate damage
	sint32 damage = CalcDamage(pTarget, ActMinDamage(), bRange, penalty, joustBonus);
	if (ActMinDamage() != ActMaxDamage()) {
		sint32 max_dmg = CalcDamage(pTarget, ActMaxDamage(), bRange, penalty, joustBonus);
		damage += gGame.Map().Rand((max_dmg-damage+1));
	}

	//
	if (bRange && (aflags & LichCloud) == 0) {
		check(m_shots > 0);
		--m_shots;
	}

	// Modify damage according to luck
	if (luck == MLM_POSITIVE) damage *= 2;
	else if (luck == MLM_NEGATIVE) damage = iMAX<sint32>(damage/2,1);

	// Genie's half
	if (HasPerk(CPERK_DOHALF) && gGame.Map().Rand(10) == 0) {
		m_pWrapper->AddLogEvent(iStringT(_T("#S0")) + GetUnitsColor(m_pOwner->Owner()) + iFormat(gTextMgr[TRID_MSG_BAT_HALF],gTextMgr[TRID_CREATURE_PEASANT_F2+m_creatType*3]));
		sint32 halfExp;
		sint32 dead = pTarget->Half(halfExp);
		check(dead);
		//OutputDebugString(iFormat(_T("%s do half to %s"),gTextMgr[TRID_CREATURE_PEASANT_F2+m_creatType*3],gTextMgr[TRID_CREATURE_PEASANT_F2+pTarget->Type()*3]).CStr());
		m_pOwner->AddExperience(halfExp);
	} else {
		m_pWrapper->AddLogEvent(iStringT(_T("#S0")) + GetUnitsColor(m_pOwner->Owner()) + iFormat(gTextMgr[TRID_MSG_BAT_DODAMAGE],gTextMgr[TRID_CREATURE_PEASANT_F3+m_creatType*3],damage));
		sint32 dead = pTarget->ReceiveDamage(damage, bRange);
		//OutputDebugString(iFormat(_T("%s do %d damage to %s (penalty: %d)\n"),gTextMgr[TRID_CREATURE_PEASANT_F2+m_creatType*3],damage,gTextMgr[TRID_CREATURE_PEASANT_F2+pTarget->Type()*3],penalty).CStr());
		m_pOwner->AddExperience(damage);

		// special creatures
		if (HasPerk(CPERK_GHOST)) {
			// Flies, creatures killed by Ghosts become Ghosts
			if ( !pTarget->HasPerk(CPERK_LIFELESS) &&  !pTarget->HasPerk(CPERK_UNDEAD) ) {
				m_casualties = iMAX<sint32>(m_casualties-dead,0);
				m_creatCount += dead;
			}
		} else if (HasPerk(CPERK_DRAINLIFES)) {
			// Regenerates from the blood of target
			if ( !pTarget->HasPerk(CPERK_LIFELESS) &&  !pTarget->HasPerk(CPERK_UNDEAD) ) {
				uint32 rcnt = Resurrect(damage);
				//OutputDebugString(iFormat(_T("%d vampire resurrected (%dhp)\n"), rcnt, damage).CStr());
			}
		}
	}
}

sint32 iBattleGroup::ReceiveDamage(sint32& damage, bool bRange)
{
	iStringT cellMsg(FormatNumber(damage));

	// Remove Paralize and Blind spells
	if (m_bBlinded) {
		RemoveSpell(MSP_BLIND);
	}

	//OutputDebugString(iFormat(_T("%s receives %d of damage: "),gTextMgr[TRID_CREATURE_PEASANT_F2+m_creatType*3],damage).CStr());
	sint32 dead = 0;
	if (damage >= TotalHitPoints()) {
		m_casualties += m_creatCount;
		damage = TotalHitPoints();
		//OutputDebugString(iFormat(_T("%d units dead (Group destroyed)\n"),m_creatCount).CStr());
		dead = m_creatCount;
		m_creatCount = 0;
		m_hitPoints = 0;
		SetState(Dead);
	} else {
		m_hitPoints -= damage;
		while(m_hitPoints <= 0) {
			++dead;
			m_creatCount--;
			m_hitPoints += HitPoints();
		}
		//OutputDebugString(iFormat(_T("%d units dead\n"),dead).CStr());
		check(m_creatCount > 0);
		m_casualties += dead;
	}

	if (dead) {
		m_pWrapper->AddLogEvent(GetUnitsColor(m_pOwner->Owner()) + iStringT(_T("#S0--- ")) + iFormat(gTextMgr[TRID_MSG_BAT_PERISH],dead,gTextMgr[TRID_CREATURE_PEASANT_F3+m_creatType*3]));
		cellMsg.Addf(_T(" (-%d)"), dead);
	}
	m_pWrapper->AddCellEvent(cellMsg, m_Pos);

	return dead;
}

sint32 iBattleGroup::Half(sint32& damage)
{
	sint32 dead = (m_creatCount+1) / 2;
	m_casualties += dead;
	m_creatCount -= dead;
	if (!m_creatCount) {
		damage = m_hitPoints;
		m_hitPoints = 0;
		SetState(Dead);
	} else damage = dead * HitPoints();

	m_pWrapper->AddLogEvent(GetUnitsColor(m_pOwner->Owner()) + iStringT(_T("#S0--- ")) + iFormat(gTextMgr[TRID_MSG_BAT_PERISH],dead,gTextMgr[TRID_CREATURE_PEASANT_F3+m_creatType*3]));
	m_pWrapper->AddCellEvent(_T("HALF"), m_Pos);

	return dead;
}

bool iBattleGroup::CanResurrect() const
{
	return (CREAT_DESC[m_creatType].perks & CPERK_GHOST) == 0 && (m_casualties > 0 || HitPoints() > HitPointsLeft());
}

sint32 iBattleGroup::CountResurrected(sint32 hp)
{
	sint32 resurrected = 0;
	sint32 mhp = m_hitPoints;
	mhp += hp;
	sint32 mcc = m_creatCount;
	while (mhp > HitPoints()) {
		mhp -= HitPoints();
		resurrected++;
		mcc++;
	}	
	if (mcc > m_initCount) {
		resurrected = m_casualties;		
	}		
	return resurrected;	
}

sint32 iBattleGroup::Resurrect(sint32 hp)
{
	sint32 resurrected = 0;
	m_hitPoints += hp;
	while (m_hitPoints > HitPoints()) {
		m_hitPoints -= HitPoints();
		resurrected++;
		m_creatCount++;
	}	
	if (m_creatCount > m_initCount) {
		resurrected = m_casualties;
		m_creatCount = m_initCount;
		m_hitPoints = HitPoints();
	}
	m_casualties -= resurrected;

	if (m_State == Dead) SetState(Idle);

	return resurrected;
}

void iBattleGroup::Restore(sint32 hp)
{
	m_hitPoints = iMIN<sint32>(m_hitPoints+hp, HitPoints());
}

LPCTSTR const STATE_NAMES[iBattleGroup::STATE_COUNT] = {
	_T("IDLE"),
	_T("MOVING"),
	_T("ROTATING"),
	_T("MELEE"),
	_T("SHOOTING"),
	_T("RECEIVING DAMAGE"),
	_T("CASTING SPELL"),
	_T("DEAD")
};

void iBattleGroup::SetState(STATE nState)
{
	//_LOG(iFormat(_T("%s changes state to '%s'\n"),gTextMgr[TRID_CREATURE_PEASANT_F2+m_creatType*3], STATE_NAMES[nState]).CStr());
	m_State = nState;
}

void iBattleGroup::Place(const iPoint& pos)
{
	bool bInMoat = (m_passMap.GetAt(pos.x,pos.y)&0x7F) == CT_MOAT || ( m_Size == 2 && (m_passMap.GetAt(pos.x+TAIL_OFFSET[m_Orient],pos.y)&0x7F) == CT_MOAT);

	/*
	FILE* nfile = fopen("C:\\!!!_!!!.dump","wb");
	fwrite(m_passMap.GetPtr(),m_passMap.GetWidth()*m_passMap.GetHeight(),1,nfile);
	fclose(nfile);*/

	if ( bInMoat && !m_bInMoat) {
		EnterMoat();
	} else if (!bInMoat && m_bInMoat) {
		LeaveMoat();
	}
	//_LOG(iFormat(_T("%s moves to %d,%d\n"),gTextMgr[TRID_CREATURE_PEASANT_F2+m_creatType*3], pos.x,pos.y).CStr());
	m_Pos = pos;
}

void iBattleGroup::Rotate() 
{ 
	//_LOG(iFormat(_T("%s rotates\n"),gTextMgr[TRID_CREATURE_PEASANT_F2+m_creatType*3]).CStr());
	if (m_Size == 2) m_Pos.x += TAIL_OFFSET[m_Orient]; 
	m_Orient = (ORIENT)(!m_Orient); 
}

void iBattleGroup::GetMeleePosition(const iBattleGroup* pTarget, const iPoint& pos, uint16 mdir, iPoint& meleePos, uint8& meleeOrient)
{
	if (mdir == MDIR_RSPTOP || mdir == MDIR_RSPBOTTOM || mdir == MDIR_LSPTOP  || mdir == MDIR_LSPBOTTOM) {
		// target should be double sized and attack point should be between location cells
		check(pTarget->Size() == 2);
		uint8 sor = (mdir == MDIR_RSPTOP || mdir == MDIR_RSPBOTTOM)?0:1;
		uint8 sdir = (mdir == MDIR_RSPTOP || mdir == MDIR_LSPTOP)?0:2;
		meleePos.x = pos.x + HEX_NEBS[pos.y&1][sor][sdir][0];
		meleePos.y = pos.y + HEX_NEBS[pos.y&1][sor][sdir][1];
		meleeOrient = m_Orient;
		sint32 ptcx = meleePos.x+TAIL_OFFSET[meleeOrient];
		sint32 ptcy = meleePos.y;
		if (!m_passMap.IsValidPos(ptcx, ptcy) || (m_passMap.GetAt(ptcx, ptcy) & 0x7F) != CT_PASSABLE) {
			meleeOrient = !meleeOrient;
		}
	} else if (mdir == MDIR_NORTH || mdir == MDIR_SOUTH) {
		// assaulter should be double sized
		check(m_Size == 2);
		uint8 mmd = (mdir == MDIR_NORTH)?0:2;
		meleePos.x = pos.x + HEX_NEBS[pos.y&1][m_Orient][mmd][0];
		meleePos.y = pos.y + HEX_NEBS[pos.y&1][m_Orient][mmd][1];
		meleeOrient = m_Orient;
	} else {
		// standard move directions
		uint8 dir = mdir >> 1;
		meleeOrient = !BattleDirOrient(dir);
		meleePos.x = pos.x + HEX_MELEE_NEBS1[pos.y&1][dir][0];
		meleePos.y = pos.y + HEX_MELEE_NEBS1[pos.y&1][dir][1];
	}
}


void iBattleGroup::Synchronize(iArmy& army)
{
	if (m_idx == -1) return;
	check(army[m_idx].Type() == m_creatType);
	army[m_idx].Count() = m_creatCount;
	if (m_casualties) {
		m_casualties = 0;
		check(army[m_idx].Count() >= 0);
		if (!army[m_idx].Count()) army[m_idx].Type() = CREAT_UNKNOWN;
	}
}

void iBattleGroup::InitPassMap(const iBattleMap& obsMap, const iCastleFort* pFort)
{
	m_passMap.CopyFrom(obsMap);
	check(!m_meleeList.GetSize() && !m_shotList.GetSize() && !m_potTargets.GetSize());
	// insert fort elements
	if (pFort) pFort->ProcessPassMap(m_passMap, m_pOwner->MemberType() == iBattleMember::Castle);
}

void iBattleGroup::UpdatePassMap(const iBattleMap& obsMap, const iSimpleArray<iBattleGroup*>& mg, const iSimpleArray<iBattleGroup*>& eg, const iCastleFort* pFort)
{
	// Reset
	m_passMap.CopyFrom(obsMap);
	m_meleeList.RemoveAll();
	m_shotList.RemoveAll();
	m_potTargets.RemoveAll();

	//uint32 val = m_passMap.GetAt(9,1);

	// insert fort elements
	if (pFort) pFort->ProcessPassMap(m_passMap, m_pOwner->MemberType() == iBattleMember::Castle);

	//m_passMap.GetAt(9,1);

	// insert creatures
	for (uint32 xx=0; xx<mg.GetSize(); ++xx) {
		if (!mg[xx]->IsAlive() || mg[xx] == this) continue;
		m_passMap.GetAt(mg[xx]->Pos().x,mg[xx]->Pos().y) = CT_FRIENDLYC;
		if (mg[xx]->Size() == 2) m_passMap.GetAt(mg[xx]->Pos().x+TAIL_OFFSET[mg[xx]->Orient()],mg[xx]->Pos().y) = CT_FRIENDLYC;
	}
	for (uint32 xx=0; xx<eg.GetSize(); ++xx) {
		if (!eg[xx]->IsAlive()) continue;
		m_passMap.GetAt(eg[xx]->Pos().x,eg[xx]->Pos().y) = CT_ENEMYC;
		if (eg[xx]->Size() == 2) m_passMap.GetAt(eg[xx]->Pos().x+TAIL_OFFSET[eg[xx]->Orient()],eg[xx]->Pos().y) = CT_ENEMYC;
	}
}

void iBattleGroup::AddMeleeEntry(const iPoint& pos, uint16 mdir)
{
	check(mdir<MDIR_COUNT);
	for (uint32 xx=0; xx<m_meleeList.GetSize(); ++xx){
		if (m_meleeList[xx].m_pos == pos) {
			m_meleeList[xx].m_cfg |= (1<<mdir);
			return;
		}
	}
	m_meleeList.Add(iMeleeEntry(pos, 1<<mdir));
}

void iBattleGroup::AddMeleeMask(const iPoint& pos, uint16 mmask)
{
	m_meleeList.Add(iMeleeEntry(pos, mmask));
}

void iBattleGroup::AddShootEntry(const iPoint& pos, sint32 penalty)
{
	m_shotList.Add(iShootEntry(pos, penalty));
}

void iBattleGroup::AddPotTargetMask(const iPoint& pos, uint16 mmask)
{
	m_potTargets.Add(iMeleeEntry(pos, mmask));
}

/*
 *	Battle units
 */

void iBattleUnit_Turret::Attack(iBattleGroup* pTarget)
{
	// calculate damage
	sint32 damage = 50;

	m_pWrapper->AddLogEvent(iStringT(_T("#S0")) + GetUnitsColor(m_pOwner->Owner()) + iFormat(gTextMgr[TRID_MSG_BAT_TURRET_DODAMAGE],damage));
	iStringT cellMsg(FormatNumber(damage));

	sint32 dead = pTarget->ReceiveDamage(damage, true);

	// Increase experience value (according with damage provided to enemy troops)
	//if (pTarget->m_pOwner->GetSide() != m_pOwner->GetSide()) m_experience += damage;

	//_LOG(iFormat(_T("%s do %d damage to %s"),_T("Turret"),damage,gTextMgr[TRID_CREATURE_PEASANT_F2+pTarget->Type()*3]).CStr());
}
//
void iBattleUnit_Moat::Attack(iBattleGroup* pTarget)
{
	// calculate damage
	sint32 damage = 50;

	m_pWrapper->AddLogEvent(iStringT(_T("#S0")) + GetUnitsColor(m_pOwner->Owner()) + iFormat(gTextMgr[TRID_MSG_BAT_MOAT_DODAMAGE],damage));
	iStringT cellMsg(FormatNumber(damage));

	sint32 dead = pTarget->ReceiveDamage(damage, true);

	// Increase experience value (according with damage provided to enemy troops)
	//if (pTarget->m_pOwner->GetSide() != m_pOwner->GetSide()) m_experience += damage;

	//_LOG(iFormat(_T("%s do %d damage to %s"),_T("Moat"),damage,gTextMgr[TRID_CREATURE_PEASANT_F2+pTarget->Type()*3]).CStr());
}

