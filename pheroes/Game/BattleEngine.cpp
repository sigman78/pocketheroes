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

#ifdef _DEEP_DEBUG_
#	define new DEBUG_NEW
#endif 

#include "BattlePF.h"
/* Combat rules:
How the damage is counted?
DAM= (1+((A-D)*X))) * A_count * RND_damage
In words: Number of attackers times damage caused by a single unit [this
one is the only random factor - damage lies somewhere within damage
range] times modifier based on Attack skill of Attacker and Defense skill
of defender.
Value of X: X equals 0.1 if A is bigger than D, which means that the
damage is increased by 10% for every point between A and D X equals 0.05
if D is bigger than A, so damage is decreased by 5% for every point
between D and A.
We'll show a black dragon fighting different opponents. Damage modifier
counted below will in combat be multiplied by number of dragons and
random value between 25 and 50, which is black dragon's damage range.
So, black dragon attacking peasants: [Assume heroes with A and D=0]
Modifier = (1+(14-1)*0.1) = 2.30 or 230%, which gives damage from 68 to
115.
Now, the same dragon, attacking other black dragon:
Modifier = (1+(14-14)*0.1) = 1.00 or 100% which gives, of course, damage
from 25 to 50
And now, the same dragon attacking other black dragon, whose commanding
hero has the ultimate cloak of Protection, having defense bonus +12:
Modifier = (1+(14-26)*0.05) = ((1-12)*0.05) = 0.4 or 40%
There are limits to those numbers. Attack modifier is limited to 300%,
defense modifier is limited to 20%. That means that there's no need to
cast bloodlust on your army if it's attack is higher than it's target
defense + 20, or steelskin, if your unit's defense is higher than
attacking unit's attack + 16. Is that clear?

 */

/*
 *	Debug
 */

//#define DEBUG_BATTLE
#ifdef DEBUG_BATTLE
#define _LOG(x) gLogMgr.Log(x)
#else
#define _LOG(x)
#endif //DEBUG_BATTLE


LPCTSTR const SIDE_TEXT[2] = {
	_T("->"), _T("<-")
};

void DumpBattleUnit(iBattleUnit* pUnit)
{
	if (DynamicCast<iBattleUnit_Catapult*>(pUnit)) {
		_LOG(iFormat(_T("* Catapult at %d,%d"),-1,-1));
	} else if (DynamicCast<iBattleUnit_Moat*>(pUnit)) {
		_LOG(iFormat(_T("* Moat at %d,%d"),-1,-1));
	} else if (DynamicCast<iBattleUnit_Turret*>(pUnit)) {
		_LOG(iFormat(_T("* Turret at %d,%d"),-1,-1));
	} else if (iBattleUnit_CreatGroup* pCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(pUnit)) {
		iBattleGroup* pGroup = pCreatGroup->GetCreatGroup();
		_LOG(iFormat(_T("* %s at %d,%d (Speed: %d) %s"),gTextMgr[TRID_CREATURE_PEASANT_F2+pGroup->Type()*3], pGroup->Pos().x,pGroup->Pos().y,pGroup->Speed(),SIDE_TEXT[pGroup->Owner()->GetSide()]));
	}
}

/*
 *	
 */
const sint8 _armyA1[] = { 5 };
const sint8 _armyA2[] = { 3,7 };
const sint8 _armyA3[] = { 2,5,8 };
const sint8 _armyA4[] = { 0,4,6,10 };
const sint8 _armyA5[] = { 0,2,5,8,10 };
const sint8 _armyA6[] = { 0,2,4,6,8,10 };
const sint8 _armyA7[] = { 0,2,4,5,6,8,10 };
const sint8* const armyArrangement[7] = { _armyA1,_armyA2,_armyA3,_armyA4,_armyA5,_armyA6,_armyA7 };

// [even, odd row][MDIR_NORTHWEST - MDIR_LSPTOP][x,y offset]
const sint16 DCELL_MELEE_OFFSET[2][MDIR_COUNT][2] = {
	{
		{1,1},{/**/0,0},{0,1},{1,1},{-1,0},{1,-1},{0,-1},{/**/0,0},{1,-1},{0,-1},{1,0},{0,1}
	},{
		{0,1},{/**/0,0},{-1,1},{0,1},{-1,0},{0,-1},{-1,-1},{/**/0,0},{0,-1},{-1,-1},{1,0},{-1,1}
	}
};

// Direction masks for melee [right,left direction][id 0 - 3]
const uint16 HEX_DIR_MASKS[2][3] = {
	{
		1 << MDIR_NORTHEAST,
		1 << MDIR_EAST,
		1 << MDIR_SOUTHEAST,
	},{
		1 << MDIR_NORTHWEST,
		1 << MDIR_WEST,
		1 << MDIR_SOUTHWEST
	}
};

// Cover areas for multi-cell spells
const uint32 BALL_MODE_CELLS_COUNT = 7;
const sint32 BALL_MODE_CELLS[2][BALL_MODE_CELLS_COUNT][2] = {
	{
		{0,-1}, {1,-1}, {-1,0}, {0,0}, {1,0}, {0,1}, {1,1}
	},{
		{-1,-1}, {0,-1}, {-1,0}, {0,0}, {1,0}, {-1,1}, {0,1}
	}
};

const uint32 BLAST_MODE_CELLS_COUNT = 19;
const sint32 BLAST_MODE_CELLS[2][BLAST_MODE_CELLS_COUNT][2] = {
	{
		{-1,-2}, {0,-2}, {1,-2}, 
		{-1,-1}, {0,-1}, {1,-1}, {2,-1},
		{-2,0}, {-1,0}, {0,0}, {1,0}, {2,0},
		{-1,1}, {0,1}, {1,1}, {2,1},
		{-1,2}, {0,2}, {1,2}, 
	},{
		{-1,-2}, {0,-2}, {1,-2}, 
		{-2,-1}, {-1,-1}, {0,-1}, {1,-1},
		{-2,0}, {-1,0}, {0,0}, {1,0}, {2,0},
		{-2,1}, {-1,1}, {0,1}, {1,1},
		{-1,2}, {0,2}, {1,2}, 
	}
};

const uint32 RING_MODE_CELLS_COUNT = 6;
const sint32 RING_MODE_CELLS[2][RING_MODE_CELLS_COUNT][2] = {
	{
		{0,-1}, {1,-1}, {-1,0}, {1,0}, {0,1}, {1,1}
	},{
		{-1,-1}, {0,-1}, {-1,0}, {1,0}, {-1,1}, {0,1}
	}
};

/*
 *	Action list
 */
//const float ANI_SPEED_COEF[5] = { 2.0f, 1.5f, 1.0f, 0.75f, 0.5f };
const fix32 ANI_SPEED_COEF[5] = { 
	fix32::fp( 0x20000 ), fix32::fp( 0x18000 ), fix32::fp( 0x10000 ), fix32::fp( 0xc000 ), fix32::fp( 0x8000 )
};


iBattleAct::iBattleAct(fix32 delay) 
: m_delay(delay * fix32(ANI_SPEED_COEF[gSettings.GetEntryValue(CET_COMBATSPEED)])) 
{
}
//////////////////////////////////////////////////////////////////////////
iBattleAct_Move::iBattleAct_Move(iBattleGroup* pActor, const iPoint& target) 
: iBattleAct(fix32(0.3f)), m_pActor(pActor), m_target(target), m_bridgeFlag(0) {}

void iBattleAct_Move::BeginAct()
{ 
	m_pActor->SetState(iBattleGroup::Moving); 
}

void iBattleAct_Move::EndAct()
{ 
	m_pActor->Place(m_target); 
	m_pActor->SetState(iBattleGroup::Idle); 
}
//////////////////////////////////////////////////////////////////////////
iBattleAct_Rotate::iBattleAct_Rotate(iBattleGroup* pActor) 
: iBattleAct(fix32(0.3f)), m_pActor(pActor) {}

void iBattleAct_Rotate::BeginAct() 
{ 
	m_pActor->SetState(iBattleGroup::Rotating); 
}

void iBattleAct_Rotate::EndAct() 
{ 
	m_pActor->Rotate(); 
	m_pActor->SetState(iBattleGroup::Idle); 
}

//////////////////////////////////////////////////////////////////////////
iBattleAct_Attack::iBattleAct_Attack(iBattleGroup* pActor, const iPoint& cell, iBattleGroup* pTarget, sint32 penalty, uint32 joustBonus, bool bRetail, iBattleGroup::MORLUCK_MOD luck)
: iBattleAct(fix32(0.6f)), m_pActor(pActor), m_cell(cell), m_penalty(penalty), m_joustBonus(joustBonus), m_bRetail(bRetail), m_luck(luck)
{
	m_targetList.Add(pTarget);
}

bool iBattleAct_Attack::Normalize()
{
	for (uint32 xx=0; xx<m_targetList.GetSize();) {
		if (!m_targetList[xx]->IsAlive()) m_targetList.RemoveAt(xx);
		else ++xx;
	}
	return m_targetList.GetSize() > 0;
}

void iBattleAct_Attack::BeginAct() 
{ 
	m_pActor->SetState(iBattleGroup::Melee); 
	for (uint32 xx=0; xx<m_targetList.GetSize(); ++xx) {
		m_targetList[xx]->SetState(iBattleGroup::RecDamage); 
	}
}

void iBattleAct_Attack::EndAct() 
{ 
	for (uint32 xx=0; xx<m_targetList.GetSize(); ++xx) {
		m_pActor->Attack(m_targetList[xx], false, m_penalty, m_joustBonus, m_luck);
		if (m_targetList[xx]->IsAlive()) m_targetList[xx]->SetState(iBattleGroup::Idle);
	}
	m_pActor->SetState(iBattleGroup::Idle);
	if (m_bRetail) m_pActor->SetRetailed();
}
//////////////////////////////////////////////////////////////////////////
iBattleAct_Shoot::iBattleAct_Shoot(iBattleGroup* pActor, const iPoint& cell, iBattleGroup* pTarget, sint32 penalty, iBattleGroup::MORLUCK_MOD luck) 
: iBattleAct(fix32(0.5f)), m_pActor(pActor), m_cell(cell), m_penalty(penalty), m_luck(luck)
{
	m_targetList.Add(pTarget);
}

bool iBattleAct_Shoot::Normalize()
{
	for (uint32 xx=0; xx<m_targetList.GetSize();) {
		if (!m_targetList[xx]->IsAlive()) m_targetList.RemoveAt(xx);
		else ++xx;
	}
	return m_targetList.GetSize() > 0;
}

void iBattleAct_Shoot::BeginAct()
{ 
	m_pActor->SetState(iBattleGroup::Shooting); 
	for (uint32 xx=0; xx<m_targetList.GetSize(); ++xx) {
		m_targetList[xx]->SetState(iBattleGroup::RecDamage); 
	}
}

void iBattleAct_Shoot::EndAct()
{ 
	uint32 aflag = iBattleGroup::RangeAttack;
	for (uint32 xx=0; xx<m_targetList.GetSize(); ++xx) {
		m_pActor->Attack(m_targetList[xx], aflag, m_penalty, 0, m_luck);
		if (m_targetList[xx]->IsAlive()) m_targetList[xx]->SetState(iBattleGroup::Idle);
		if (xx == 0 && m_pActor->HasPerk(CPERK_LICHSHOOT)) aflag |= iBattleGroup::LichCloud;
	}
	m_pActor->SetState(iBattleGroup::Idle); 
}
//////////////////////////////////////////////////////////////////////////
iBattleAct_Delay::iBattleAct_Delay() 
: iBattleAct(fix32(0.9f)) {}
void iBattleAct_Delay::BeginAct() {}
void iBattleAct_Delay::EndAct() {}
//////////////////////////////////////////////////////////////////////////
iBattleAct_GoodMorale::iBattleAct_GoodMorale(iBattleGroup* pActor) 
: iBattleAct(fix32(0.9f)), m_pActor(pActor) {}
void iBattleAct_GoodMorale::BeginAct() { m_pActor->GoodMorale(); }
void iBattleAct_GoodMorale::EndAct(){}
//////////////////////////////////////////////////////////////////////////
iBattleAct_BadMorale::iBattleAct_BadMorale(iBattleGroup* pActor) 
: iBattleAct(fix32(0.9f)), m_pActor(pActor) {}
void iBattleAct_BadMorale::BeginAct() { m_pActor->BadMorale(); }
void iBattleAct_BadMorale::EndAct(){}
//////////////////////////////////////////////////////////////////////////
iBattleAct_GoodLuck::iBattleAct_GoodLuck(iBattleGroup* pActor) 
: iBattleAct(fix32(0.9f)), m_pActor(pActor) {}
void iBattleAct_GoodLuck::BeginAct() { m_pActor->GoodLuck(); }
void iBattleAct_GoodLuck::EndAct() { }
//////////////////////////////////////////////////////////////////////////
iBattleAct_BadLuck::iBattleAct_BadLuck(iBattleGroup* pActor) 
: iBattleAct(fix32(0.9f)), m_pActor(pActor) {}
void iBattleAct_BadLuck::BeginAct() { m_pActor->BadLuck(); }
void iBattleAct_BadLuck::EndAct(){}

//////////////////////////////////////////////////////////////////////////
iBattleAct_CastSpell::iBattleAct_CastSpell(iBattleUnit_Hero* pCaster, iCombatSpell* pSpell, const iPoint& cell) 
: iBattleAct(fix32(0.6f)), m_pCaster(pCaster), m_pSpell(pSpell), m_cell(cell) {}

void iBattleAct_CastSpell::BeginAct()
{ 
	if (m_pSpell->SpellClass() == SPC_DAMAGE) {
		for (uint32 xx=0; xx<m_targetList.GetSize(); ++xx) m_targetList[xx]->SetState(iBattleGroup::RecDamage); 
	} else if (m_pSpell->SpellClass() == SPC_EARTHQUAKE) {
		for (uint32 xx=0; xx<m_wallList.GetSize(); ++xx) m_wallList[xx]->bRecDmg = true;
	} 
	for (uint32 xx=0; xx<m_resistList.GetSize(); ++xx) m_resistList[xx]->SetState(iBattleGroup::ResistSpell); 
}

void iBattleAct_CastSpell::EndAct()
{ 
	if (m_pSpell->SpellClass() == SPC_EARTHQUAKE) {
		check(m_wallList.GetSize() && !m_targetList.GetSize());
		for (uint32 xx=0; xx<m_wallList.GetSize(); ++xx) {
			m_pSpell->Cast(m_pCaster, NULL);
			sint32 damage = 50;
			damage += gGame.Map().Rand(100);
			m_wallList[xx]->Attack(damage);
			m_wallList[xx]->bRecDmg = false;
		}
	} else {
		for (uint32 xx=0; xx<m_targetList.GetSize(); ++xx) {
			m_pSpell->Cast(m_pCaster, m_targetList[xx]);
			if (m_pSpell->SpellClass() == SPC_DAMAGE && m_targetList[xx]->IsAlive()) m_targetList[xx]->SetState(iBattleGroup::Idle);
		}
		for (uint32 xx=0; xx<m_resistList.GetSize(); ++xx) m_resistList[xx]->SetState(iBattleGroup::Idle);
	}
	uint8 cost = iBaseSpell::GetSpellCost(m_pSpell->Id(), m_pCaster->GetHero());
	m_pCaster->GetHero()->ManaPts() -= cost;
	m_pCaster->Owner()->SetCastFlag(true);
}
//////////////////////////////////////////////////////////////////////////
iBattleAct_Catapult::iBattleAct_Catapult(iBattleUnit_Catapult* pActor, iCastleFort* pFort, iCastleFort::ElementType target) 
: iBattleAct(fix32(1.0f)), m_pActor(pActor), m_pFort(pFort), m_target(target) 
{
}

void iBattleAct_Catapult::BeginAct()
{
	if (m_target == iCastleFort::InvalidElement) {
		m_target = m_pFort->GetTarget();
	}
	if (m_target != iCastleFort::InvalidElement) {
		iCastleFort::iElement* pTarget = m_pFort->GetElement(m_target);
		check(pTarget);
		pTarget->bRecDmg = true;
	}
}

void iBattleAct_Catapult::EndAct()
{ 
	if (m_target != iCastleFort::InvalidElement) {
		iCastleFort::iElement* pTarget = m_pFort->GetElement(m_target);
		check(pTarget);
		iBattleMember_Hero* pHero = DynamicCast<iBattleMember_Hero*>(m_pActor->Owner());
		check(pHero);
		sint32 bal = pHero->GetHero()->FurtSkill(FSK_BALLISTICS);
		sint32 damage = CATAPULT_PROPS[bal][0];
		damage += gGame.Map().Rand(CATAPULT_PROPS[bal][1] - CATAPULT_PROPS[bal][0]);
		
		pTarget->bRecDmg = false;
		pTarget->Attack(damage);
	}
}
//////////////////////////////////////////////////////////////////////////
iBattleAct_Turret::iBattleAct_Turret(iBattleUnit_Turret* pActor, iBattleGroup* pTarget) 
: iBattleAct(fix32(0.5f)), m_pActor(pActor), m_pTarget(pTarget) {}

void iBattleAct_Turret::BeginAct()
{
	m_pTarget->SetState(iBattleGroup::RecDamage);
}

void iBattleAct_Turret::EndAct()
{
	m_pActor->Attack(m_pTarget);
	if (m_pTarget->IsAlive()) m_pTarget->SetState(iBattleGroup::Idle);
}
//////////////////////////////////////////////////////////////////////////
iBattleAct_Gate::iBattleAct_Gate(iBattleGroup* pActor, iCastleFort::iElement_Bridge* pGate, bool bEnter)
: iBattleAct(fix32(0.3f)), m_pActor(pActor), m_pGate(pGate), m_bEnter(bEnter) {}

void iBattleAct_Gate::BeginAct()
{ 
}

void iBattleAct_Gate::EndAct()
{ 
	if (m_bEnter) m_pGate->Enter(m_pActor);
	else m_pGate->Leave(m_pActor);
}
//////////////////////////////////////////////////////////////////////////
iBattleAct_MoatDmg::iBattleAct_MoatDmg(iBattleUnit_Moat* pActor, iBattleGroup* pTarget)
: iBattleAct(fix32(0.3f)), m_pActor(pActor), m_pTarget(pTarget) {}

void iBattleAct_MoatDmg::BeginAct()
{ 
	m_pTarget->SetState(iBattleGroup::RecDamage);
}

void iBattleAct_MoatDmg::EndAct()
{ 
	m_pActor->Attack(m_pTarget);
	if (m_pTarget->IsAlive()) m_pTarget->SetState(iBattleGroup::Idle);
}

/*
 *	Battle Army
 */
iBattleArmy::iBattleArmy(iBattleWrapper* pWrapper)
: m_pWrapper(pWrapper)
{}

iBattleArmy::~iBattleArmy()
{
	for (uint32 xx=0; xx<m_groups.GetSize(); ++xx) {
		delete m_groups[xx];
	}
	m_groups.RemoveAll();
}

void iBattleArmy::Init(const iArmy& army, iBattleMember* pOwner)
{
	m_pOwner = pOwner;
	sint32 moraleModifier = army.MoraleModifier();
	for (uint32 xx=0; xx<7; ++xx) {
		if (army[xx].Type() != CREAT_UNKNOWN) m_groups.Add(new iBattleGroup(army[xx], xx, pOwner, m_pWrapper, moraleModifier));
	}
}

iBattleGroup* iBattleArmy::Summon(const iCreatGroup& cg, const iPoint& pos)
{
	iBattleGroup* pGroup = new iBattleGroup(cg, -1, m_pOwner, m_pWrapper, 0);
	pGroup->Place(pos);
	m_groups.Add(pGroup);
	return pGroup;
}

void iBattleArmy::Synchronize(iArmy& army)
{
	for (uint32 xx=0; xx<m_groups.GetSize(); ++xx) m_groups[xx]->Synchronize(army);
}

sint32 iBattleArmy::Cost() const
{
	sint32 result = 0;
	for (uint32 xx=0; xx< m_groups.GetSize(); ++xx) {
		iBattleGroup* pGroup = m_groups[xx];
		if (pGroup->IsAlive()) {
			result += pGroup->Count() * CREAT_DESC[pGroup->Type()].cost.quant[MINERAL_GOLD];
		}
	}
	return result;
}

uint32 iBattleArmy::ArmyPower() const
{
	uint32 result = 0;
	for (uint32 xx=0; xx< m_groups.GetSize(); ++xx) if (m_groups[xx]->IsAlive()) result += m_groups[xx]->Count() * CREAT_DESC[m_groups[xx]->Type()].pidx;
	return result;
}

/*
 *	Turn sequence
 */
iTurnSeq::iTurnSeq(iBattleWrapper* pWrapper)
: m_LastSide(1) // setup last side to defender (to provide high priority for assaulter in turn sequence)
, m_pCurUnit(NULL)
, m_pSpellCaster(NULL)
, m_pWrapper(pWrapper)
, m_bHasCatapult(false)
{
}

iTurnSeq::~iTurnSeq()
{
	if (m_pCurUnit) {
		delete m_pCurUnit;
		m_pCurUnit = NULL;
	}
	Cleanup();
}

iBattleUnit*
iTurnSeq::CurUnit() const
{ 
	return (m_pSpellCaster!=0) ? m_pSpellCaster : m_pCurUnit; 
}

void iTurnSeq::Cleanup()
{
	for (uint32 xx=0; xx<m_turnSeq.GetSize(); ++xx) {
		iBattleUnit* pUnit = m_turnSeq[xx];
		delete m_turnSeq[xx];
	}
	m_turnSeq.RemoveAll();
}

bool iTurnSeq::StepSequence()
{
	_LOG(_T("TurnSequence::StepSequence {\r\n"));
	if (m_pCurUnit) {
		delete m_pCurUnit;
		m_pCurUnit = NULL;
	}
	if (!m_turnSeq.GetSize()) {
		_LOG(_T("} TurnSequence::StepSequence (false)\r\n"));
		return false;
	}

	m_pCurUnit = m_turnSeq.PopFirst();
	m_LastSide = m_pCurUnit->Owner()->GetSide();

	_LOG(_T("Select new unit: "));
	DumpBattleUnit(m_pCurUnit);
	_LOG(_T("\r\n"));

	if (iBattleUnit_CreatGroup* pCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(m_pCurUnit)) {
		// 
		iBattleGroup* pGroup = pCreatGroup->GetCreatGroup();
		if (pGroup->CanWait()) {
			iBattleGroup* pActGroup = m_active[pGroup->Owner()->GetSide()].Pop();
			check(pGroup == pActGroup);
		} else {
			iBattleGroup* pWaitGroup = m_waiting[pGroup->Owner()->GetSide()].Pop();
			check(pGroup == pWaitGroup);
		}
	} else if (iBattleUnit_Catapult* pCatapult = DynamicCast<iBattleUnit_Catapult*>(m_pCurUnit)) {
		check(m_bHasCatapult);
		m_bHasCatapult = false;
	} else if (iBattleUnit_Turret* pTurret = DynamicCast<iBattleUnit_Turret*>(m_pCurUnit)) {
		bool bFound = false;
		for (uint32 xx=0; xx<m_turrets.GetSize(); ++xx) {
			if (m_turrets[xx] == pTurret->m_et) {
				m_turrets.RemoveAt(xx);
				bFound = true;
				break;
			}
		}
		check(bFound);
	} else {
		check(0);
	}

	_LOG(_T("} TurnSequence::StepSequence (true)\r\n"));
	return true;
}

void iTurnSeq::NewRoundSequence(iBattleArmy& aa, iBattleArmy& da, iCastleFort* pFort)
{
	check(!m_turnSeq.GetSize() && !m_pCurUnit);
	check(!m_active[0].Size() && !m_active[1].Size());
	
	//check(!m_dead.GetSize());
	m_dead.RemoveAll();

	// Add assaulter army creatures
	for (uint32 xx=0; xx<aa.Count(); ++xx) {
		if (aa[xx]->IsAlive() && aa[xx]->CanAct()) m_active[0].Insert(aa[xx], aa[xx]->Speed()*20-aa[xx]->Pos().y);
		else m_dead.Add(aa[xx]);
	}

	// Add defender army creatures
	for (uint32 xx=0; xx<da.Count(); ++xx) {
		if (da[xx]->IsAlive() && da[xx]->CanAct()) m_active[1].Insert(da[xx], da[xx]->Speed()*20-da[xx]->Pos().y);
		else m_dead.Add(da[xx]);
	}

	// Add turrets and catapult
	if (pFort) {
		if (((iCastleFort::iElement_Tower*)pFort->GetElement(iCastleFort::RightTower3))->bTurret && pFort->GetElement(iCastleFort::MainTower)->state != iCastleFort::Destroyed) m_turrets.Add(iCastleFort::RightTower3);
		if (((iCastleFort::iElement_Tower*)pFort->GetElement(iCastleFort::LeftTower3))->bTurret && pFort->GetElement(iCastleFort::MainTower)->state != iCastleFort::Destroyed) m_turrets.Add(iCastleFort::LeftTower3);
		if (((iCastleFort::iElement_Tower*)pFort->GetElement(iCastleFort::MainTower))->bTurret && pFort->GetElement(iCastleFort::MainTower)->state != iCastleFort::Destroyed) m_turrets.Add(iCastleFort::MainTower);
		if (pFort->HasTarget()) {
			m_bHasCatapult = true;
		}
	}

	UpdateSequence(0);
	StepSequence();
}

iBattleUnit* iTurnSeq::NextGroup()
{
	check(m_pCurUnit);

	// Last unit is hero (spell casted)
	if (m_pSpellCaster) {
		delete m_pSpellCaster;
		m_pSpellCaster = NULL;
		if (m_pCurUnit->IsAlive()) return m_pCurUnit;
	}

	// Check for good morale case
	if (iBattleUnit_CreatGroup* pCurCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(m_pCurUnit)) {
		iBattleGroup* pCurGroup = pCurCreatGroup->GetCreatGroup();
		if (pCurGroup->IsAlive() && pCurGroup->RoundMorale() == iBattleGroup::MLM_POSITIVE) {
			// Good morale (reset round morale and return current unit)
			pCurGroup->ResetRoundMorale();
			return m_pCurUnit;
		} 
	}

	// Sequence is empty, so, reset current unit and return NULL
	if (!m_turnSeq.GetSize()) {
		delete m_pCurUnit;
		m_pCurUnit = NULL;
		return m_pCurUnit;
	} 

	StepSequence();
	return m_pCurUnit;
}

void iTurnSeq::SetSpellCaster(iBattleUnit_Hero* pSpellCaster)
{
	check(!m_pSpellCaster && m_pCurUnit && m_pCurUnit->Owner()->GetSide() == pSpellCaster->Owner()->GetSide());
	m_pSpellCaster = pSpellCaster;
}

iBattleUnit* iTurnSeq::Wait()
{
	iBattleUnit_CreatGroup* pCurCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(m_pCurUnit);
	check(pCurCreatGroup);

	iBattleGroup* pGroup = pCurCreatGroup->GetCreatGroup();
	m_waiting[pCurCreatGroup->Owner()->GetSide()].Insert(pGroup, -(pGroup->Speed()*20-pGroup->Pos().y));
	UpdateSequence(0);
	StepSequence();
	return m_pCurUnit;
}

iBattleUnit* iTurnSeq::Defend()
{
	check(DynamicCast<iBattleUnit_CreatGroup*>(m_pCurUnit));
	UpdateSequence(0);
	StepSequence();
	return m_pCurUnit;
}

void iTurnSeq::UpdateSequence(uint32 flags)
{
	// Cleanup current sequence
	Cleanup();

	// update active creatures
	if (flags & USF_ACTIVES) {
		for (uint32 yy=0; yy<2; ++yy) {
			// Sort active units
			iSimpleArray<iBattleGroup*> group;
			while (m_active[yy].Size()) group.Add(m_active[yy].Pop());
			while (group.GetSize()) {
				iBattleGroup* pGroup = group.Pop();
				if (pGroup->IsAlive() && pGroup->CanAct()) {
					//OutputDebugString(iFormat(_T("Add active %s to normals\n"), gTextMgr[TRID_CREATURE_PEASANT_F2+pGroup->Type()*3]).CStr());
					m_active[yy].Insert(pGroup, pGroup->Speed()*20-pGroup->Pos().y);
				} else {
					//OutputDebugString(iFormat(_T("Add active %s to deads\n"), gTextMgr[TRID_CREATURE_PEASANT_F2+pGroup->Type()*3]).CStr());
					m_dead.Add(pGroup);
				}
			}
			// Sort waiting units
			while (m_waiting[yy].Size()) group.Add(m_waiting[yy].Pop());
			while (group.GetSize()) {
				iBattleGroup* pGroup = group.Pop();
				if (pGroup->IsAlive() && pGroup->CanAct()) {
					//OutputDebugString(iFormat(_T("Add waiting %s to normals\n"), gTextMgr[TRID_CREATURE_PEASANT_F2+pGroup->Type()*3]).CStr());
					m_waiting[yy].Insert(pGroup, -(pGroup->Speed()*20-pGroup->Pos().y));
				} else {
					//OutputDebugString(iFormat(_T("Add waiting %s to deads\n"), gTextMgr[TRID_CREATURE_PEASANT_F2+pGroup->Type()*3]).CStr());
					m_dead.Add(pGroup);
				}
			}
		}
	}

	// parse dead creatures
	if (flags & USF_DEADS) {
		for (uint32 xx=0; xx<m_dead.GetSize();) {
			iBattleGroup* pGroup = m_dead[xx];
			if (pGroup->IsAlive() && pGroup->CanAct()) {
				_LOG(iFormat(_T("%s Restored from dead/blind state\r\n"),gTextMgr[TRID_CREATURE_PEASANT_F2+pGroup->Type()*3]));
				m_dead.RemoveAt(xx);
				if (pGroup->CanWait()) m_active[pGroup->Owner()->GetSide()].Insert(pGroup, pGroup->Speed()*20-pGroup->Pos().y);
				else m_waiting[pGroup->Owner()->GetSide()].Insert(pGroup, -(pGroup->Speed()*20-pGroup->Pos().y));
			} else ++xx;
		}
	}

	// Add active units to current sequence
	bool bNeedCatapult = m_bHasCatapult;
	uint32 pr = !m_LastSide;
	sint32 pos[2] = { m_active[0].Size()-1, m_active[1].Size()-1 };
	while ( pos[0] >= 0 || pos[1] >=0 ) {
		if (pos[0] >=0 && pos[1] < 0) {
			m_turnSeq.Add(new iBattleUnit_CreatGroup(m_active[0].Value(pos[0]--)));
			pr = 1;
		} else if (pos[0] < 0 && pos[1] >= 0) {
			m_turnSeq.Add(new iBattleUnit_CreatGroup(m_active[1].Value(pos[1]--)));
			pr = 0;
		} else if (m_active[pr].Value(pos[pr])->Speed() < m_active[!pr].Value(pos[!pr])->Speed()) {
			m_turnSeq.Add(new iBattleUnit_CreatGroup(m_active[!pr].Value(pos[!pr]--)));
		} else {
			m_turnSeq.Add(new iBattleUnit_CreatGroup(m_active[pr].Value(pos[pr]--)));
			pr = !pr;
		}
	}

	// Add waiting units to current sequence
	pos[0] = m_waiting[0].Size()-1;
	pos[1] = m_waiting[1].Size()-1;
	while ( pos[0] >= 0 || pos[1] >=0 ) {
		if (pos[0] >=0 && pos[1] < 0) {
			m_turnSeq.Add(new iBattleUnit_CreatGroup(m_waiting[0].Value(pos[0]--)));
			pr = 1;
		} else if (pos[0] < 0 && pos[1] >= 0) {
			m_turnSeq.Add(new iBattleUnit_CreatGroup(m_waiting[1].Value(pos[1]--)));
			pr = 0;
		} else if (m_waiting[pr].Value(pos[pr])->Speed() > m_waiting[!pr].Value(pos[!pr])->Speed()) {
			m_turnSeq.Add(new iBattleUnit_CreatGroup(m_waiting[!pr].Value(pos[!pr]--)));
		} else {
			m_turnSeq.Add(new iBattleUnit_CreatGroup(m_waiting[pr].Value(pos[pr]--)));
			pr = !pr;
		}
	}


	// Add turrets and catapult
	if (m_turrets.GetSize()) {
		for (uint32 xx=0; xx<m_turrets.GetSize(); ++xx) {
			m_turnSeq.InsertBefore(0, new iBattleUnit_Turret(m_pWrapper->Engine().DArmy().Owner(), m_turrets[xx], m_pWrapper));
		}
	}
	if (m_bHasCatapult) {
		for (uint32 xx=0; xx<m_turnSeq.GetSize(); ++xx) {
			if (m_turnSeq[xx]->Owner()->GetSide() == iBattleMember::Assaulter) {
				m_turnSeq.InsertBefore(xx, new iBattleUnit_Catapult(m_turnSeq[xx]->Owner()));
				break;
			}
		}
	}

	//OutputDebugString(iFormat(_T("UpdateSequence: %d left\n------------------\n"), m_turnSeq.GetSize()).CStr());
	//for (uint32 zz=0; zz<m_turnSeq.GetSize(); ++zz) DumpBattleUnit(m_turnSeq[zz]);
	//OutputDebugString(_T("--------------\n"));
}

void iTurnSeq::AddNewGroup(iBattleGroup* pNewGroup)
{
	m_active[pNewGroup->Owner()->GetSide()].Insert(pNewGroup, pNewGroup->Speed()*20-pNewGroup->Pos().y);
	UpdateSequence(0);
}


/*
 *	Battle Engine
 */
iBattleEngine::iBattleEngine(iBattleWrapper* pWrapper, bool bFakeMode)
: m_pWrapper(pWrapper), 
m_bFakeMode(m_bFakeMode),
m_aArmy(pWrapper), m_dArmy(pWrapper), 
m_turnSeq(pWrapper), 
m_bInited(false), m_iRound(0), m_obsMap(13,11), m_pCastleFort(NULL), m_pCurAct(NULL), m_bAutoBattle(false)
{
	m_obsMap.FillMem(CT_PASSABLE);
}

iBattleEngine::~iBattleEngine()
{
}

void iBattleEngine::BeginBattle(const iBattleInfo& bi)
{
	m_bi = bi;

	const iArmy& aArmy = m_bi.m_pAssaulter->Army();
	const iArmy& dArmy = m_bi.m_pDefender->Army();

	// Init armies
	m_aArmy.Init(aArmy, m_bi.m_pAssaulter);
	m_dArmy.Init(dArmy, m_bi.m_pDefender);

	// Init castle's fortification
	check(!m_pCastleFort);
	//OutputDebugString(_T("\n\n--- Battle Begins ---\n"));
	if (iBattleMember_Castle* pCastleMember = DynamicCast<iBattleMember_Castle*>(bi.m_pDefender)) {
		m_pCastleFort = new iCastleFort(pCastleMember->GetCastle());
		//OutputDebugString(_T("Castle assault\n\n"));
	} else {
		//OutputDebugString(_T("Generic battle\n\n"));
	}

	// Place creatures to battlefield
	for (uint32 xx=0; xx<m_aArmy.Count(); ++xx) {
		m_aArmy[xx]->InitPassMap(m_obsMap, m_pCastleFort);
		iPoint p((m_aArmy[xx]->Size()==2)?1:0,armyArrangement[m_aArmy.Count()-1][xx]);
		m_aArmy[xx]->Place(p);
	}
	for (uint32 xx=0; xx<m_dArmy.Count(); ++xx) {
		m_dArmy[xx]->InitPassMap(m_obsMap, m_pCastleFort);
		iPoint p((m_dArmy[xx]->Size()==2)?11:12,armyArrangement[m_dArmy.Count()-1][xx]);
		m_dArmy[xx]->Place(p);
	}

	m_bInited = true;
}

void iBattleEngine::Start()
{
	NextRound();
	PrepareDistMap();
}

void iBattleEngine::EndBattle(iBattleResult br)
{
	// Synchronize armies according to battle results (casualties)
	m_aArmy.Synchronize(m_bi.m_pAssaulter->Army());
	m_dArmy.Synchronize(m_bi.m_pDefender->Army());

	br.m_pWinner->OnWin(br);
	br.m_pLoser->OnLose(br);

	// Check winner's army (at least one group should be in army)
	check(!br.m_pWinner->Army().Empty());

	// 
	if (m_pCastleFort) {
		delete m_pCastleFort;
		m_pCastleFort = NULL;
	}

	//
	m_bInited = false;
}

void iBattleEngine::MakePassMap(iBattleMap& pmap)
{
	pmap.Init(m_obsMap);
	if (m_pCastleFort) m_pCastleFort->ProcessPassMap(pmap, false);
	for (uint32 xx=0; xx<m_aArmy.Groups().GetSize(); ++xx) {
		if (!m_aArmy.Groups()[xx]->IsAlive()) continue;
		pmap.GetAt(m_aArmy.Groups()[xx]->Pos().x,m_aArmy.Groups()[xx]->Pos().y) = CT_ENEMYC;
		if (m_aArmy.Groups()[xx]->Size() == 2) pmap.GetAt(m_aArmy.Groups()[xx]->Pos().x+TAIL_OFFSET[m_aArmy.Groups()[xx]->Orient()],m_aArmy.Groups()[xx]->Pos().y) = CT_ENEMYC;
	}
	for (uint32 xx=0; xx<m_dArmy.Groups().GetSize(); ++xx) {
		if (!m_dArmy.Groups()[xx]->IsAlive()) continue;
		pmap.GetAt(m_dArmy.Groups()[xx]->Pos().x,m_dArmy.Groups()[xx]->Pos().y) = CT_ENEMYC;
		if (m_dArmy.Groups()[xx]->Size() == 2) pmap.GetAt(m_dArmy.Groups()[xx]->Pos().x+TAIL_OFFSET[m_dArmy.Groups()[xx]->Orient()],m_dArmy.Groups()[xx]->Pos().y) = CT_ENEMYC;
	}
}

void iBattleEngine::PreparePassMap()
{
	uint32 xx;
	for (xx=0; xx<m_aArmy.Count(); ++xx) if(m_aArmy[xx]->IsAlive()) m_aArmy[xx]->UpdatePassMap(m_obsMap,m_aArmy.Groups(),m_dArmy.Groups(), m_pCastleFort);
	for (xx=0; xx<m_dArmy.Count(); ++xx) if(m_dArmy[xx]->IsAlive()) m_dArmy[xx]->UpdatePassMap(m_obsMap,m_dArmy.Groups(),m_aArmy.Groups(), m_pCastleFort);
}

void iBattleEngine::PrepareDistMap()
{
	if (iBattleUnit_CreatGroup* pCurCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(m_turnSeq.CurUnit())) {
		PreparePassMap();
		iBattleGroup* pCurGroup = pCurCreatGroup->GetCreatGroup();
		iSimpleArray<iBattleGroup*> groups;
		bool bFull = false;

		PLAYER_ID pid = m_turnSeq.CurUnit()->Owner()->Owner();
		if (pid == PID_NEUTRAL || gGame.Map().FindPlayer(pid)->PlayerType() == PT_COMPUTER || IsAutoBattle()) {
			if (m_bFakeMode) {
				groups.Add(pCurGroup);
			} else {
				for (uint32 xx=0; xx<m_aArmy.Count(); ++xx) if(m_aArmy[xx]->IsAlive() && m_aArmy[xx]->CanAct()) groups.Add(m_aArmy[xx]);
				for (uint32 xx=0; xx<m_dArmy.Count(); ++xx) if(m_dArmy[xx]->IsAlive() && m_dArmy[xx]->CanAct()) groups.Add(m_dArmy[xx]);
			}
			bFull = true;
		} else {
			groups.Add(pCurGroup);
		}

		for (uint32 gid=0; gid<groups.GetSize(); ++gid) {
			iBattleGroup* pg = groups[gid];
			iBattlePF pf(groups[gid]);
			pf.MakeDistMap(bFull && groups[gid] == pCurGroup);

			iBattleArmy& eg = (groups[gid]->Owner()->GetSide() == iBattleMember::Assaulter)?m_dArmy:m_aArmy;

			//
			// melee list
			for (uint32 zz=0; zz<eg.Count(); ++zz) {
				if (eg[zz]->IsAlive()) {
					const iBattleGroup::iDistMap& distMap = groups[gid]->DistMap();
					const iBattleMap& passMap = groups[gid]->PassMap();
					sint16 sv = eg[zz]->Pos().y&1;
					iPoint ep(eg[zz]->Pos());
					iBattleGroup::ORIENT eo = eg[zz]->Orient();
					iPoint tmc(ep.x + HEX_MELEE_NEBS2[sv][eo][1][0], ep.y + HEX_MELEE_NEBS2[sv][eo][1][1]);
					iPoint bmc(ep.x + HEX_MELEE_NEBS2[sv][eo][5][0], ep.y + HEX_MELEE_NEBS2[sv][eo][5][1]);
					uint16 tdir = MDIR_RSPTOP;
					uint16 bdir = MDIR_RSPBOTTOM;
					if (eg[zz]->Size() == 2) {
						// for double cell enemy groups
						iPoint cp = (eg[zz]->Orient() == 1)?eg[zz]->Pos():eg[zz]->TailPos(); // left cell

						for (uint32 cc=0; cc<2; ++cc) {
							uint16 mask = 0;
							uint16 pmask = 0;
							// Check all target nebs (standard move directions)
							for (uint32 yy=0; yy<2; ++yy) {
								for (uint32 xx=0; xx<3; ++xx) {
									iPoint cell(cp.x+HEX_NEBS[sv][yy][xx][0],cp.y+HEX_NEBS[sv][yy][xx][1]);
									if (distMap.IsValidPos(cell.x,cell.y) && pCurGroup->CanMove(cell.x,cell.y) && (pCurGroup->Size() == 1 || (distMap.IsValidPos(cell.x+TAIL_OFFSET[!yy],cell.y) && pCurGroup->CanMove(cell.x+TAIL_OFFSET[!yy],cell.y)))){
										pmask |= HEX_DIR_MASKS[yy][xx];
										if (distMap.GetAt(cell.x,cell.y).dirs[!yy].val <= groups[gid]->Speed() && ((passMap.GetAt(cell.x,cell.y)&0x7F) != CT_MOAT || pCurGroup->Pos() == cell)) {
											mask |= HEX_DIR_MASKS[yy][xx];
										}
									}
								}
							}

							// check special directions (up and bottom vertices of the cell)
							if (pCurGroup->Size() == 2) {
								// only double cell groups can melee in such direction
								if (distMap.IsValidPos(cp.x+HEX_NEBS[sv][0][0][0],cp.y+HEX_NEBS[sv][0][0][1]) 
									&& pCurGroup->CanMove(cp.x+HEX_NEBS[sv][0][0][0],cp.y+HEX_NEBS[sv][0][0][1])
									&& distMap.IsValidPos(cp.x+HEX_NEBS[sv][1][0][0],cp.y+HEX_NEBS[sv][1][0][1]) 
									&& pCurGroup->CanMove(cp.x+HEX_NEBS[sv][1][0][0],cp.y+HEX_NEBS[sv][1][0][1])){
									pmask |= (1<<MDIR_NORTH);
									if (distMap.GetAt(cp.x+HEX_NEBS[sv][0][0][0],cp.y+HEX_NEBS[sv][0][0][1]).dirs[0].val <= groups[gid]->Speed()
										&& distMap.GetAt(cp.x+HEX_NEBS[sv][1][0][0],cp.y+HEX_NEBS[sv][1][0][1]).dirs[1].val  <= groups[gid]->Speed()){
										mask |= (1<<MDIR_NORTH);
									}
								}
								if (distMap.IsValidPos(cp.x+HEX_NEBS[sv][0][2][0],cp.y+HEX_NEBS[sv][0][2][1]) 
									&& pCurGroup->CanMove(cp.x+HEX_NEBS[sv][0][2][0],cp.y+HEX_NEBS[sv][0][2][1])
									&& distMap.IsValidPos(cp.x+HEX_NEBS[sv][1][2][0],cp.y+HEX_NEBS[sv][1][2][1]) 
									&& pCurGroup->CanMove(cp.x+HEX_NEBS[sv][1][2][0],cp.y+HEX_NEBS[sv][1][2][1])){
									pmask |= (1<<MDIR_SOUTH);
									if (distMap.GetAt(cp.x+HEX_NEBS[sv][0][2][0],cp.y+HEX_NEBS[sv][0][2][1]).dirs[0].val <= groups[gid]->Speed()
										&& distMap.GetAt(cp.x+HEX_NEBS[sv][1][2][0],cp.y+HEX_NEBS[sv][1][2][1]).dirs[1].val <= groups[gid]->Speed()) {
										mask |= (1<<MDIR_SOUTH);
									}
								}
							}

							// check special directions (up and bottom vertices between two cells)
							if (distMap.IsValidPos(tmc.x,tmc.y) 
								&& (pCurGroup->CanMove(tmc.x,tmc.y) || pCurGroup->CanMove(tmc.x,tmc.y))){
								pmask |= (1<<tdir);
								if (distMap.GetAt(tmc.x,tmc.y).dirs[0].val <= groups[gid]->Speed() || distMap.GetAt(tmc.x,tmc.y).dirs[1].val <= groups[gid]->Speed()) {
									mask |= (1<<tdir);
								}
							}
							if (distMap.IsValidPos(bmc.x,bmc.y) 
								&& (pCurGroup->CanMove(bmc.x,bmc.y) || pCurGroup->CanMove(bmc.x,bmc.y))) {
								pmask |= (1<<bdir);
								if ((distMap.GetAt(bmc.x,bmc.y).dirs[0].val <= groups[gid]->Speed() || distMap.GetAt(bmc.x,bmc.y).dirs[1].val <= groups[gid]->Speed())) {
									mask |= (1<<bdir);
								}
							}

							if (mask) groups[gid]->AddMeleeMask(cp,mask);
							else if (pmask) groups[gid]->AddPotTargetMask(cp,pmask);
							// offset point to right cell
							cp.x += 1;
							// change directions for right cell
							tdir = MDIR_LSPTOP;
							bdir = MDIR_LSPBOTTOM;
						}
					} else {
						// for single cell enemy groups
						iPoint cp = eg[zz]->Pos();
						uint16 mask = 0;
						uint16 pmask = 0;
						// Check all target nebs (standard move directions)
						for (uint32 yy=0; yy<2; ++yy) {
							for (uint32 xx=0; xx<3; ++xx) {
								iPoint cell(cp.x+HEX_NEBS[sv][yy][xx][0],cp.y+HEX_NEBS[sv][yy][xx][1]);
								if (distMap.IsValidPos(cell.x,cell.y) && pCurGroup->CanMove(cell.x,cell.y) && (pCurGroup->Size() == 1 || (distMap.IsValidPos(cell.x+TAIL_OFFSET[!yy],cell.y) && pCurGroup->CanMove(cell.x+TAIL_OFFSET[!yy],cell.y)))){
									pmask |= HEX_DIR_MASKS[yy][xx];
									if (groups[gid]->DistMap().GetAt(cell.x,cell.y).dirs[!yy].val <= groups[gid]->Speed() && ((passMap.GetAt(cell.x,cell.y)&0x7F) != CT_MOAT || pCurGroup->Pos() == cell)){
										mask |= HEX_DIR_MASKS[yy][xx];
									}
								}
							}
						}
						// check special directions (up and bottom vertices of the cell)
						if (pCurGroup->Size() == 2) {
							// only double cell groups can melee in such direction
							if (distMap.IsValidPos(cp.x+HEX_NEBS[sv][0][0][0],cp.y+HEX_NEBS[sv][0][0][1]) 
								&& pCurGroup->CanMove(cp.x+HEX_NEBS[sv][0][0][0],cp.y+HEX_NEBS[sv][0][0][1])
								&& distMap.IsValidPos(cp.x+HEX_NEBS[sv][1][0][0],cp.y+HEX_NEBS[sv][1][0][1]) 
								&& pCurGroup->CanMove(cp.x+HEX_NEBS[sv][1][0][0],cp.y+HEX_NEBS[sv][1][0][1])){
								pmask |= (1<<MDIR_NORTH);
								if (distMap.GetAt(cp.x+HEX_NEBS[sv][0][0][0],cp.y+HEX_NEBS[sv][0][0][1]).dirs[0].val <= groups[gid]->Speed()
									&& distMap.GetAt(cp.x+HEX_NEBS[sv][1][0][0],cp.y+HEX_NEBS[sv][1][0][1]).dirs[1].val  <= groups[gid]->Speed()) {
									mask |= (1<<MDIR_NORTH);
								}
							}
							if (distMap.IsValidPos(cp.x+HEX_NEBS[sv][0][2][0],cp.y+HEX_NEBS[sv][0][2][1]) 
								&& pCurGroup->CanMove(cp.x+HEX_NEBS[sv][0][2][0],cp.y+HEX_NEBS[sv][0][2][1])
								&& distMap.IsValidPos(cp.x+HEX_NEBS[sv][1][2][0],cp.y+HEX_NEBS[sv][1][2][1]) 
								&& pCurGroup->CanMove(cp.x+HEX_NEBS[sv][1][2][0],cp.y+HEX_NEBS[sv][1][2][1])){
								pmask |= (1<<MDIR_SOUTH);
								if (distMap.GetAt(cp.x+HEX_NEBS[sv][0][2][0],cp.y+HEX_NEBS[sv][0][2][1]).dirs[0].val <= groups[gid]->Speed()
									&& distMap.GetAt(cp.x+HEX_NEBS[sv][1][2][0],cp.y+HEX_NEBS[sv][1][2][1]).dirs[1].val <= groups[gid]->Speed()) {
									mask |= (1<<MDIR_SOUTH);
								}

							}
						}
						if (mask) groups[gid]->AddMeleeMask(cp,mask);
						else if (pmask) groups[gid]->AddPotTargetMask(cp,pmask);
					}
				}
			}

			//
			// shot list
			if (groups[gid]->Shots()) {
				bool bHinder = false;
				sint16 sv = groups[gid]->Pos().y&1;
				// check all neighbor cells
				if (groups[gid]->Size() == 2) {
					iPoint cp = (groups[gid]->Orient() == 1)?groups[gid]->Pos():groups[gid]->TailPos(); // left cell
					for (uint32 cc=0; cc<2; ++cc) {
						for (uint32 yy=0; yy<2; ++yy) {
							for (uint32 xx=0; xx<3; ++xx) {
								iPoint cell(cp.x+HEX_NEBS[sv][yy][xx][0],cp.y+HEX_NEBS[sv][yy][xx][1]);
								if ( groups[gid]->PassMap().IsValidPos(cell.x,cell.y) && (groups[gid]->PassMap().GetAt(cell.x, cell.y)&0x7F) == CT_ENEMYC) {
									bHinder = true;
									break;
								}
							}
						}
						cp.x++;
					}
				} else {
					for (uint32 yy=0; yy<2; ++yy) {
						for (uint32 xx=0; xx<3; ++xx) {
							iPoint cell(groups[gid]->Pos().x+HEX_NEBS[sv][yy][xx][0],groups[gid]->Pos().y+HEX_NEBS[sv][yy][xx][1]);
							if ( groups[gid]->PassMap().IsValidPos(cell.x,cell.y) && (groups[gid]->PassMap().GetAt(cell.x, cell.y)&0x7F) == CT_ENEMYC) {
								bHinder = true;
								break;
							}
						}
					}
				}

				// Add all alive enemy groups in range attack target list
				if (!bHinder) {
					for (uint32 xx=0; xx<eg.Count(); ++xx) {
						if (eg[xx]->IsAlive()) {
							groups[gid]->AddShootEntry(eg[xx]->Pos(), CalcShotPenalty(groups[gid],eg[xx]->Pos()));
							if (eg[xx]->Size() == 2) groups[gid]->AddShootEntry(iPoint(eg[xx]->Pos().x+TAIL_OFFSET[eg[xx]->Orient()],eg[xx]->Pos().y), CalcShotPenalty(groups[gid],iPoint(eg[xx]->Pos().x+TAIL_OFFSET[eg[xx]->Orient()],eg[xx]->Pos().y)));
						}
					}
				}
			}
		}

		/*
		FILE* pfile = _tfopen(iFormat(_T("C:\\!!!\\%s_info.txt"), gTextMgr[TRID_CREATURE_PEASANT_F2+(pCurGroup->Type()*3)]).CStr(),_T("wt"));
		_ftprintf(pfile,_T("Melee targets: %d:\n"), pCurGroup->MeleeListCount());
		_ftprintf(pfile,_T("Shoot targets: %d:\n"), pCurGroup->ShootListCount());
		fclose(pfile);
		*/
	} else if (iBattleUnit_Catapult* pCurCreatGroup = DynamicCast<iBattleUnit_Catapult*>(m_turnSeq.CurUnit())){
	} else if (iBattleUnit_Turret* pCurCreatGroup = DynamicCast<iBattleUnit_Turret*>(m_turnSeq.CurUnit())){
	}
}

void iBattleEngine::NextGroup()
{
	if (!m_turnSeq.NextGroup()) {
		NextRound();
	}
	PrepareDistMap();
}

void iBattleEngine::SelectSpellTargets(iCombatSpell* pSpell, iSimpleArray<iBattleGroup*>& targetList)
{
	const iSimpleArray<iBattleGroup*>* pGroups = &m_aArmy.Groups();
	iBattleUnit_Hero spellCaster(m_turnSeq.CurUnit()->Owner());
	for (uint32 yy=0; yy<2; ++yy) {
		for (uint32 xx=0; xx<pGroups->GetSize(); ++xx) {
			if (pSpell->CanCast(&spellCaster, pGroups->At(xx))) {
				// We unable to resurrect creature at occuped cell
				if (pSpell->AffectsToDead() && !pGroups->At(xx)->IsAlive()) {
					if (FindGroup(pGroups->At(xx)->Pos()) || (pGroups->At(xx)->Size() == 2 &&  FindGroup(pGroups->At(xx)->TailPos()))) {
						continue;
					}
				}
				targetList.Add(pGroups->At(xx));
			}
		}
		pGroups = &m_dArmy.Groups();
	}
}

void iBattleEngine::GetValidSpellTargets(iCombatSpell* pSpell, MAGIC_SCHOOL_LEVEL msl, iSimpleArray<iPoint>& targetList)
{
	check(pSpell->TargetMode(msl) == STM_CREAT_GROUP);
	iSimpleArray<iBattleGroup*> targets;
	SelectSpellTargets(pSpell, targets);
	for (uint32 xx=0; xx<targets.GetSize(); ++xx) {
		targetList.Add(targets[xx]->Pos());
		if (targets[xx]->Size() == 2) targetList.Add(iPoint(targets[xx]->Pos().x+TAIL_OFFSET[targets[xx]->Orient()], targets[xx]->Pos().y));
	}

}

void iBattleEngine::GetSpellCovers(iCombatSpell* pSpell, MAGIC_SCHOOL_LEVEL msl, const iPoint& pos, iSimpleArray<iPoint>& targetList)
{
	check(targetList.GetSize() == 0);
	iPoint tp;
	sint32 yv = (pos.y & 1);
	if (pSpell->TargetMode(msl) == STM_BALL_SET) {
		for (uint32 xx=0; xx<BALL_MODE_CELLS_COUNT; ++xx) {
			tp.x = pos.x + BALL_MODE_CELLS[yv][xx][0];
			tp.y = pos.y + BALL_MODE_CELLS[yv][xx][1];
			if (m_obsMap.IsValidPos(tp.x, tp.y)) targetList.Add(tp);
		}
	} else if (pSpell->TargetMode(msl) == STM_BLAST_SET) {
		for (uint32 xx=0; xx<BLAST_MODE_CELLS_COUNT; ++xx) {
			tp.x = pos.x + BLAST_MODE_CELLS[yv][xx][0];
			tp.y = pos.y + BLAST_MODE_CELLS[yv][xx][1];
			if (m_obsMap.IsValidPos(tp.x, tp.y)) targetList.Add(tp);
		}
	} else if (pSpell->TargetMode(msl) == STM_RING_SET) {
		for (uint32 xx=0; xx<RING_MODE_CELLS_COUNT; ++xx) {
			tp.x = pos.x + RING_MODE_CELLS[yv][xx][0];
			tp.y = pos.y + RING_MODE_CELLS[yv][xx][1];
			if (m_obsMap.IsValidPos(tp.x, tp.y)) targetList.Add(tp);
		}
	} else {
		check(0);
	}
}

uint32 iBattleEngine::GetSurrenderCost(iBattleMember_Hero* pActor)
{
	check(pActor);
	uint32 result = (pActor->GetSide() == iBattleMember::Defender)?m_dArmy.Cost():m_aArmy.Cost();
	if (sint32 dip = pActor->FurtSkills().Value(FSK_DIPLOMACY)) {
		check(dip > 0 && dip <= 100);
		result -= (result * dip) / 100;
	}

	check(result);
	return result;
}

//////////////////////////////////////////////////////////////////////////
inline iPoint Cell2Screen(const iPoint& cell)
{
	// constants
	const sint32 cellW = 22;
	const sint32 cellWh = cellW>>1;
	const sint32 cellH = 21;
	const sint32 cellHh = cellH>>1;
	const sint32 cellOffX = 22;
	const sint32 cellOffY = 17;

	//
	return iPoint(cellW + cell.x * cellW - (cell.y&1) * cellWh, cellHh + cell.y * cellOffY);
}

inline bool IsIntersected(const iPoint& sc, const iPoint& tc, const iPoint& oc, sint32 org )
{
	iPoint sPos = Cell2Screen(sc);	// source
	iPoint tPos = Cell2Screen(tc);	// target
	iPoint oPos = Cell2Screen(oc);	// obstacle

	iPoint d(tPos.x - sPos.x, tPos.y - sPos.y);
	sint32 a = d.x * d.x + d.y * d.y;
	sint32 b = 2 * (d.x * (sPos.x - oPos.x) + d.y * (sPos.y - oPos.y));
	sint32 c = (oPos.x * oPos.x + oPos.y * oPos.y) + (sPos.x * sPos.x + sPos.y * sPos.y);
	c -= 2 * (oPos.x * sPos.x + oPos.y * sPos.y) + org * org;

	return (b * b - 4 * a * c) >= 0;
}

//////////////////////////////////////////////////////////////////////////
const sint32 TEMP_FORT_BOUND[11] = {
	9, 9, 8, 8, 7, 7, 7, 8, 8, 9, 9
};

sint32 iBattleEngine::CalcShotPenalty(iBattleGroup* pRanger, const iPoint& ptTarget)
{
	const sint32 obstR = 3;
	const sint32 maxRange = 7;

	// init
	sint32 result = 1;
	check(pRanger->CanShoot() && pRanger->Shots() > 0);

	// Check for "No range penalty" flag
	if (iHero* pOwner = pRanger->Owner()->SpellCaster()) {
		if (pOwner->HasSpecFlag(SHF_NORANGEPENALTY)) return 1;
	}

	// Check distance
	if (BattleCellsDelta(pRanger->Pos().x, pRanger->Pos().y, ptTarget.x, ptTarget.y) > maxRange) result *= 2;


	// Check fortifications
	if (pRanger->Owner()->GetSide() == iBattleMember::Assaulter && m_pCastleFort) {
		iPoint p1 = Cell2Screen(pRanger->Pos());
		p1.x /= 4; p1.y /= 4;
		iPoint p2 = Cell2Screen(ptTarget);
		p2.x /= 4; p2.y /= 4;
		for (sint32 nn=0; nn<iCastleFort::ElementsCount(); ++nn) {
			iCastleFort::iElement* pEl = m_pCastleFort->GetElement(nn);
			iPoint obstPos = Cell2Screen(iPoint(iCastleFort::ElCells[pEl->type][0], iCastleFort::ElCells[pEl->type][1]));
			obstPos.x /= 4; obstPos.y /= 4;
			if (!pEl->Passable() && iClipper::IsLineIntersectCircle(obstPos, obstR, p1, p2)) {
				result *= 2;
				break;
			}
		}
	}

	// 1 - no penalty, 2 - 2x penalty, 4 - 4x penalty
	return result;
}

void iBattleEngine::ProcessMoat(iBattleGroup* pCurGroup)
{
	if (m_pCastleFort && m_pCastleFort->HasMoat()) {
		bool bMoved = false;
		for (iBattleActList::bIt it = m_ActList.First(); it != m_ActList.End(); ++it) {
			if (iBattleAct_Move* pMoveAct = DynamicCast<iBattleAct_Move*>(*it)) {
				iPoint npos = pMoveAct->m_target;
				if ((pCurGroup->PassMap().GetAt(npos.x,npos.y)&0x7F) == CT_MOAT) {
					m_ActList.InsertActionAfter(it, new iBattleAct_MoatDmg(new iBattleUnit_Moat(m_dArmy.Owner(), m_pWrapper), pCurGroup));
					break;
				}
				bMoved = true;
			}
		}
		if (!bMoved && pCurGroup->InMoat()) m_ActList.AddAction(new iBattleAct_MoatDmg(new iBattleUnit_Moat(m_dArmy.Owner(), m_pWrapper), pCurGroup));
	}
}

void iBattleEngine::ProcessGate(iBattleGroup* pCurGroup)
{
	/*
	if (m_pCastleFort && pCurGroup->Owner()->GetSide() == iBattleMember::Defender) {
		iCastleFort::iElement_Bridge* pBridge = (iCastleFort::iElement_Bridge*)m_pCastleFort->GetElement(iCastleFort::Bridge);
		check(pBridge);
		if (pBridge->state == iCastleFort::Destroyed) return;
		iPoint cpos = pCurGroup->Pos();
		uint8 corient = pCurGroup->Orient();
		for (iBattleActList::bIt it = m_ActList.First(); it != m_ActList.End(); ++it) {
			if (iBattleAct_Move* pMoveAct = DynamicCast<iBattleAct_Move*>(*it)) {
				iPoint npos = pMoveAct->m_target;
				if ( (pCurGroup->PassMap().GetAt(cpos.x,cpos.y)&0x7F) == CT_BRIDGE || (pCurGroup->Size() == 2 && (pCurGroup->PassMap().GetAt(cpos.x+TAIL_OFFSET[corient],cpos.y)&0x7F) == CT_BRIDGE)) {
					if ( (pCurGroup->PassMap().GetAt(npos.x, npos.y)&0x7F) != CT_BRIDGE && (pCurGroup->Size() != 2 || (pCurGroup->PassMap().GetAt(npos.x+TAIL_OFFSET[corient],npos.y)&0x7F) != CT_BRIDGE)) {
						// Step out of moat
						m_ActList.InsertActionAfter(it, new iBattleAct_Gate(pBridge, false));
					}
				} else if ( (pCurGroup->PassMap().GetAt(cpos.x, cpos.y)&0x7F) != CT_BRIDGE && (pCurGroup->Size() != 2 || (pCurGroup->PassMap().GetAt(cpos.x+TAIL_OFFSET[corient],cpos.y)&0x7F) != CT_BRIDGE)) {
					if ( (pCurGroup->PassMap().GetAt(npos.x,npos.y)&0x7F) == CT_BRIDGE || (pCurGroup->Size() == 2 && (pCurGroup->PassMap().GetAt(npos.x+TAIL_OFFSET[corient],npos.y)&0x7F) == CT_BRIDGE)) {
						// Step into moat
						m_ActList.InsertActionBefore(it, new iBattleAct_Gate(pBridge, true));
					}
				}

				cpos = npos;
			} else if (DynamicCast<iBattleAct_Rotate*>(*it)) {
				corient = !corient;
			}
		}
	}*/
}

void iBattleEngine::NextRound()
{
	m_iRound++;

	for (uint32 xx=0; xx<m_aArmy.Count(); ++xx) m_aArmy[xx]->NewRound();
	for (uint32 xx=0; xx<m_dArmy.Count(); ++xx) m_dArmy[xx]->NewRound();
	m_turnSeq.NewRoundSequence(m_aArmy, m_dArmy, m_pCastleFort);
	m_pWrapper->AddLogEvent(iStringT(_T("#S1#FFFF")) + iFormat(gTextMgr[TRID_MSG_BAT_NEXTROUND],m_iRound));
	m_bi.m_pAssaulter->SetCastFlag(false);
	m_bi.m_pDefender->SetCastFlag(false);
}

BATTLE_RESULT iBattleEngine::CheckBattleState()
{
       if (!m_dArmy.AliveGroups()) return BR_ASSAULTER_WIN;
       if (!m_aArmy.AliveGroups()) return BR_DEFENDER_WIN;	
/*	uint32 bState = m_dArmy.AliveGroups() | (m_aArmy.AliveGroups() << 1);
	// Is anybody alive on battlefield ?
	if (!bState) {
		//if ()
	}

	if ( (bState & 1) == 0) return BR_ASSAULTER_WIN;
	else if ( (bState & 2) == 0 ) return BR_DEFENDER_WIN;
*/
	return BR_NA;
}

void iBattleEngine::PrepareBattleResult(iBattleResult& br)
{
	// 
	if (br.m_result == BR_ASSAULTER_WIN) {
		br.m_pWinner = m_bi.m_pAssaulter;
		br.m_pLoser = m_bi.m_pDefender;
		br.m_experience = m_bi.m_pAssaulter->GetExperience();
		br.m_winCas = m_aArmy.Casualties();
		br.m_losCas = m_dArmy.Casualties();
	} else if (br.m_result == BR_DEFENDER_WIN) {
		br.m_pWinner = m_bi.m_pDefender;
		br.m_pLoser = m_bi.m_pAssaulter;
		br.m_experience = m_bi.m_pDefender->GetExperience();
		br.m_winCas = m_dArmy.Casualties();
		br.m_losCas = m_aArmy.Casualties();
	} else {
		check(0);
	}

	if (br.m_defCause == DC_DEFEAT){
		if (iBattleMember_Hero* pHeroMember = DynamicCast<iBattleMember_Hero*>(br.m_pLoser)) {
			br.m_experience += pHeroMember->GetHero()->Level() * 500;
		}
	} else if (br.m_defCause == DC_RETREAT){
		if (iBattleMember_Hero* pHeroMember = DynamicCast<iBattleMember_Hero*>(br.m_pLoser)) {
			br.m_experience += pHeroMember->GetHero()->Level() * 250;
		}
	}
	
	if (iBattleMember_Castle* pCastleMember = DynamicCast<iBattleMember_Castle*>(br.m_pLoser)) {
		br.m_experience += pCastleMember->GetCastle()->Proto()->Size() * 1000;
		if (iHero* pVisitor = pCastleMember->GetCastle()->Visitor()) {
			br.m_experience += pVisitor->Level() * 500;
		}
	}

	if (iBattleMember_Hero* pHeroMember = DynamicCast<iBattleMember_Hero*>(br.m_pWinner)) {
		br.m_experience = pHeroMember->GetHero()->ConvExpPts(br.m_experience);
	}
}

iSimpleArray<iBattleGroup*> iBattleEngine::FindAllGroups(const iPoint& pos, iBattleMember *owner, bool bIncDead) 
{
	iSimpleArray<iBattleGroup*> res;
	uint32 xx;
	if (bIncDead) {
		// For resurrect spells (try to find alive unit, then dead)		
		for (xx=0; xx<m_aArmy.Count(); ++xx) {
			if (m_aArmy[xx]->IsGroupCell(pos)) {
				if(!owner || m_aArmy[xx]->Owner() == owner)
					res.Add(m_aArmy[xx]);
			}
		}
		for (xx=0; xx<m_dArmy.Count(); ++xx) {
			if (m_dArmy[xx]->IsGroupCell(pos)) {
				if(!owner || m_dArmy[xx]->Owner() == owner)
					res.Add(m_dArmy[xx]);
			}
		}		
	} else {
		// General case (return only alive unit)
		for (xx=0; xx<m_aArmy.Count(); ++xx) if (m_aArmy[xx]->IsGroupCell(pos) && m_aArmy[xx]->IsAlive() && (!owner || m_aArmy[xx]->Owner() == owner)) res.Add(m_aArmy[xx]);
		for (xx=0; xx<m_dArmy.Count(); ++xx) if (m_dArmy[xx]->IsGroupCell(pos) && m_dArmy[xx]->IsAlive() && (!owner || m_dArmy[xx]->Owner() == owner)) res.Add(m_dArmy[xx]);
	}	
	return res;
}

iBattleGroup* iBattleEngine::FindGroup(const iPoint& pos, bool bIncDead)
{
	uint32 xx;
	if (bIncDead) {
		// For resurrect spells (try to find alive unit, then dead)
		iBattleGroup* pDead = NULL;
		for (xx=0; xx<m_aArmy.Count(); ++xx) {
			if (m_aArmy[xx]->IsGroupCell(pos)) {
				if (m_aArmy[xx]->IsAlive()) return m_aArmy[xx];	
				else if (!pDead) pDead = m_aArmy[xx];
			}
		}
		for (xx=0; xx<m_dArmy.Count(); ++xx) {
			if (m_dArmy[xx]->IsGroupCell(pos)) {
				if (m_dArmy[xx]->IsAlive()) return m_dArmy[xx];	
				else if (!pDead) pDead = m_dArmy[xx];
			}
		}
		return pDead;
	} else {
		// General case (return only alive unit)
		for (xx=0; xx<m_aArmy.Count(); ++xx) if (m_aArmy[xx]->IsGroupCell(pos) && m_aArmy[xx]->IsAlive()) return m_aArmy[xx];
		for (xx=0; xx<m_dArmy.Count(); ++xx) if (m_dArmy[xx]->IsGroupCell(pos) && m_dArmy[xx]->IsAlive()) return m_dArmy[xx];
	}
	return NULL;
}

sint32 iBattleEngine::Summon(iBattleMember::Side side, const iCreatGroup& cg, const iPoint& pos)
{
	iBattleArmy& fa = (side == iBattleMember::Assaulter) ? m_aArmy : m_dArmy;
	iBattleGroup* pSumGroup = fa.Summon(cg, pos);
	m_turnSeq.AddNewGroup(pSumGroup);
	return 0;
}

bool iBattleEngine::CastSpell(iCombatSpell* pSpell, const iPoint& pos, iBattleGroup *pSpellTarget)
{
	//iBattleMember_Hero* pCurHero = DynamicCast<iBattleMember_Hero*>(m_turnSeq.CurUnit()->Owner());
	//iHero* pSpellCaster = pCurHero->SpellCaster();
	//check(pSpellCaster);
	iBattleMember* pCurMember = m_turnSeq.CurUnit()->Owner();
	iBattleUnit_Hero* pHeroUnit = new iBattleUnit_Hero(pCurMember);
	iBattleAct_CastSpell* pAction = new iBattleAct_CastSpell(pHeroUnit, pSpell, pos);
	MAGIC_SCHOOL_LEVEL msl = pSpell->GetSchoolLevel(pHeroUnit->GetHero());

	iSimpleArray<iBattleGroup*> targets;
	if (pSpell->NeedTarget(msl)) {
		check(pos != cInvalidPoint);
		if (pSpell->TargetMode(msl) == STM_CREAT_GROUP) {			
			check(pSpellTarget);
			targets.Add(pSpellTarget);
		} else {
			check(pSpell->HasCoverArea(msl));
			iSimpleArray<iPoint> spellCovers;
			GetSpellCovers(pSpell, msl, pos, spellCovers);
			for (uint32 xx=0; xx<spellCovers.GetSize(); ++xx) {
				iBattleGroup* pTarget = FindGroup(spellCovers[xx]);
				if (pTarget && pSpell->CanCast(pHeroUnit, pTarget)) {
					targets.AddUnique(pTarget);
				} 
			}

		}
	} else if (pSpell->TargetMode(msl) == STM_SUMMON) {
		iBattleGroup* pTarget = m_aArmy.Groups()[0];
		targets.Add(pTarget);
	} else if (pSpell->TargetMode(msl) == STM_ALL) {
		SelectSpellTargets(pSpell, targets);
	} else if (pSpell->TargetMode(msl) == STM_EARTHQUAKE) {
		if (m_pCastleFort) {
			iSimpleArray<iCastleFort::iElement*> walls;
			if (m_pCastleFort->GetElement(iCastleFort::RightWall1)->state != iCastleFort::Destroyed) walls.Add(m_pCastleFort->GetElement(iCastleFort::RightWall1));
			if (m_pCastleFort->GetElement(iCastleFort::RightWall2)->state != iCastleFort::Destroyed) walls.Add(m_pCastleFort->GetElement(iCastleFort::RightWall2));
			if (m_pCastleFort->GetElement(iCastleFort::LeftWall1)->state != iCastleFort::Destroyed) walls.Add(m_pCastleFort->GetElement(iCastleFort::LeftWall1));
			if (m_pCastleFort->GetElement(iCastleFort::LeftWall2)->state != iCastleFort::Destroyed) walls.Add(m_pCastleFort->GetElement(iCastleFort::LeftWall2));
			sint32 wmax = SPELL_DESCRIPTORS[pSpell->Id()].eff[msl].fparam;
			for (uint32 xx=0; (sint32)xx<wmax && walls.GetSize(); ++xx) {
				sint32 wallIdx = gGame.Map().Rand(walls.GetSize());
				pAction->m_wallList.Add(walls[wallIdx]);
				walls.RemoveAt(wallIdx);
			}
		}
		if (pAction->m_wallList.GetSize()) {
			m_turnSeq.SetSpellCaster(pHeroUnit);
			m_ActList.AddAction(pAction);
			check(!m_pCurAct);
			StepAction();
			m_pWrapper->AddLogEvent(iStringT(_T("#S0")) + GetUnitsColor(pHeroUnit->GetHero()->Owner()->PlayerId()) + iFormat(gTextMgr[TRID_MSG_BAT_CASTS_SPELL], pHeroUnit->GetHero()->Name().CStr(), gTextMgr[pSpell->NameKey()]));
			return true;
		} else {
			delete pAction;
			return false;
		}
	} else {
		check(0 == "Invalid target mode");
	}

	if (targets.GetSize()) {
		pHeroUnit->GetHero()->AddFavoriteSpell(pSpell->Id());
		for (uint32 xx=0; xx<targets.GetSize(); ++xx) {
			sint32 rval = gGame.Map().Rand(100);
			if ( (pSpell->SpellClass() == SPC_DAMAGE || pSpell->SpellDispos() == SPD_NEGATIVE) && rval < targets[xx]->FurtSkills().Value(FSK_RESIST)) {
				pAction->m_resistList.Add(targets[xx]);
			} else {
				pAction->m_targetList.Add(targets[xx]);
			}
		}
		m_turnSeq.SetSpellCaster(pHeroUnit);
		m_ActList.AddAction(pAction);
		check(!m_pCurAct);
		StepAction();

		iStringT lmsg(_T("#S0"));
		if (pSpell->TargetMode(msl) == STM_CREAT_GROUP) {
			lmsg += GetUnitsColor(pHeroUnit->GetHero()->Owner()->PlayerId()) + iFormat(gTextMgr[TRID_MSG_BAT_CASTS_SPELL_TO], pHeroUnit->GetHero()->Name().CStr(), gTextMgr[pSpell->NameKey()], gTextMgr[TRID_CREATURE_PEASANT_F3+targets[0]->Type()*3]);
		} else {
			lmsg += GetUnitsColor(pHeroUnit->GetHero()->Owner()->PlayerId()) + iFormat(gTextMgr[TRID_MSG_BAT_CASTS_SPELL], pHeroUnit->GetHero()->Name().CStr(), gTextMgr[pSpell->NameKey()]);
		}
		m_pWrapper->AddLogEvent(lmsg);

		// Add spell resistance info into log
		if (pAction->m_targetList.GetSize() == 0 && pAction->m_resistList.GetSize() == 1) {
			iBattleGroup* pGroup = pAction->m_resistList[0]; 
			lmsg = _T("#S0");
			lmsg += GetUnitsColor(pGroup->Owner()->Owner());
			lmsg += iFormat(gTextMgr[TRID_MSG_BAT_SPELL_RESSPELL], gTextMgr[TRID_CREATURE_PEASANT_F3+pGroup->Type()*3]);
			m_pWrapper->AddLogEvent(lmsg);
		}

		// Add damage spell info into log
		if (pSpell->SpellClass() == SPC_DAMAGE && pAction->m_targetList.GetSize() != 0) {
			iSpell_Damage* pDmgSpell = (iSpell_Damage*)pSpell;
			lmsg = _T("#S0");
			lmsg += GetUnitsColor(pHeroUnit->GetHero()->Owner()->PlayerId());
			sint32 dmg = pDmgSpell->EstimateDamage(pHeroUnit->GetHero(), (pAction->m_targetList.GetSize() == 1)?pAction->m_targetList[0]:NULL);
			lmsg += iFormat(gTextMgr[TRID_MSG_BAT_SPELL_DODAMAGE], gTextMgr[pSpell->NameKey()], dmg);
			m_pWrapper->AddLogEvent(lmsg);
		}

		return true;
	} else {
		delete pAction;
		return false;
	}
}

void iBattleEngine::Move(const iPoint& pos, uint8 orient)
{
	_LOG(iFormat(_T("iBattleEngine::Move(%d,%d){\r\n"),pos.x,pos.y));
	iBattleUnit_CreatGroup* pCurCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(m_turnSeq.CurUnit());
	check(pCurCreatGroup);
	iBattleGroup* pCurGroup = pCurCreatGroup->GetCreatGroup();

	// Cleanup action list
	m_ActList.Reset();
	iPoint ipos = pCurGroup->Pos();
	uint8 iorient = pCurGroup->Orient();

	// move
	if (pos == ipos) {
		_LOG(iFormat(_T("} iBattleEngine::Move(pos == ipos)\r\n")));
		return;
	} else if (pCurGroup->TransType() == TRANS_WALK) {
		iBattlePF pf(pCurGroup);
		pf.MakePath(pos, orient, true, m_ActList);
		if (iorient != orient) m_ActList.AddAction(new iBattleAct_Rotate(pCurGroup));
	} else if (pCurGroup->TransType() == TRANS_FLY) {
		bool bRotate = false;
		// check actual moving direction (rotate in case of backward movement)
		if (BattleCellsOrient(ipos.x,ipos.y, pos.x,pos.y,iorient) != (uint8)iorient) {
			m_ActList.AddAction(new iBattleAct_Rotate(pCurGroup));
			bRotate = true;
		}
		// try to move at tail cell in case of backward movement and size == 2
		iPoint mpos = pos;
		if (pCurGroup->Size() == 2) {
			if (bRotate) {
				if (pCurGroup->CanMove(pos.x + TAIL_OFFSET[iorient],pos.y)) {
					mpos.x += TAIL_OFFSET[pCurGroup->Orient()];
				}
			} else if (!pCurGroup->CanMove(pos.x + TAIL_OFFSET[iorient],pos.y)) {
				mpos.x += TAIL_OFFSET[!iorient];
			}
		}
		// Move to destination point
		m_ActList.AddAction(new iBattleAct_Move(pCurGroup, iPoint(mpos.x,mpos.y)));
		// Back to normal orientation
		if (bRotate) m_ActList.AddAction(new iBattleAct_Rotate(pCurGroup));
	} else {
		// unknown transportation type
		check(0);
	}

	// Insert open/close gate actions
	//ProcessGate(pCurGroup);

	// Process Moat
	ProcessMoat(pCurGroup);

	// Add good morale action if required
	if (pCurGroup->RoundMorale() == iBattleGroup::MLM_POSITIVE) {
		m_ActList.AddAction(new iBattleAct_GoodMorale(pCurGroup));
	}

	// Start Action
	check(m_ActList.Count());
	if (m_ActList.Count()) {
		check(!m_pCurAct);
		StepAction();
	}
	_LOG(_T("} iBattleEngine::Move()\r\n"));
}

void iBattleEngine::MakeMeleeSequence(iBattleGroup* pActor, iBattleGroup* pTarget, const iPoint& pos, sint32 penalty, uint16 mdir, bool bRetail, iBattleActList& actList)
{
	// Check luck status
	iBattleGroup::MORLUCK_MOD luck = pActor->CalcLuck();
	if (luck == iBattleGroup::MLM_POSITIVE) m_ActList.AddAction(new iBattleAct_GoodLuck(pActor));
	else if (luck == iBattleGroup::MLM_NEGATIVE) m_ActList.AddAction(new iBattleAct_BadLuck(pActor));

	// attack target
	iBattleAct_Attack* pAttackAct = new iBattleAct_Attack(pActor, pos, pTarget, penalty, pActor->CalcJoustingBonus(pTarget), bRetail, luck);
	m_ActList.AddAction(pAttackAct);

	// [CPERK_ADJACENTATTACK] attack all nebs
	if (pActor->HasPerk(CPERK_ADJACENTATTACK)) {
		iPoint spos(pActor->Pos());
		uint8 sor = pActor->Orient();
		m_ActList.EndDirPos(pActor, spos, sor);
		sint32 yseq = (spos.y&1);
		// listup all nebs
		for (uint32 xx=0; xx < 8; ++xx) {
			sint32 cx = spos.x + HEX_MELEE_NEBS2[yseq][sor][xx][0];
			sint32 cy = spos.y + HEX_MELEE_NEBS2[yseq][sor][xx][1];
			if (pActor->PassMap().IsValidPos(cx,cy) && pActor->PassMap().GetAt(cx,cy) == CT_ENEMYC) {
				iBattleGroup* pNebTarget = FindGroup(iPoint(cx,cy));
				check(pNebTarget);
				pAttackAct->m_targetList.AddUnique(pNebTarget);
			}
		}
	}

	// [CPERK_TWOHEXATTACK] attack additional cell
	if (pActor->HasPerk(CPERK_TWOHEXATTACK)) {
		sint32 yseq = (pos.y&1);
		if (mdir == MDIR_NORTH || mdir == MDIR_SOUTH) {
		} else {
			sint32 cx = pos.x + DCELL_MELEE_OFFSET[yseq][mdir][0];
			sint32 cy = pos.y + DCELL_MELEE_OFFSET[yseq][mdir][1];
			if (iBattleGroup* pNebTarget = FindGroup(iPoint(cx,cy))) {
				if (pNebTarget != pActor) pAttackAct->m_targetList.AddUnique(pNebTarget);
			}
		}
	}
}

void iBattleEngine::MakeShootSequence(iBattleGroup* pActor, iBattleGroup* pTarget, const iPoint& pos, sint32 penalty, iBattleActList& actList)
{
	// Check luck status
	iBattleGroup::MORLUCK_MOD luck = pActor->CalcLuck();
	if (luck == iBattleGroup::MLM_POSITIVE) m_ActList.AddAction(new iBattleAct_GoodLuck(pActor));
	else if (luck == iBattleGroup::MLM_NEGATIVE) m_ActList.AddAction(new iBattleAct_BadLuck(pActor));
	
	// attack target
	iBattleAct_Shoot* pShootAct = new iBattleAct_Shoot(pActor, pos, pTarget, penalty, luck);
	m_ActList.AddAction(pShootAct);

	// [CPERK_LICHSHOOT] Range attack affects adjacent hexes except undeads (Liches)
	if (pActor->HasPerk(CPERK_LICHSHOOT) ) {
		// listup all nebs
		for (uint32 xx=0; xx < 6; ++xx) {
			sint32 yseq = (pos.y&1);
			sint32 cx = pos.x + HEX_MELEE_NEBS1[yseq][xx][0];
			sint32 cy = pos.y + HEX_MELEE_NEBS1[yseq][xx][1];
			if (pActor->PassMap().IsValidPos(cx,cy) && (pActor->PassMap().GetAt(cx,cy) == CT_ENEMYC || pActor->PassMap().GetAt(cx,cy) == CT_FRIENDLYC)) {
				iBattleGroup* pNebTarget = FindGroup(iPoint(cx,cy));
				check(pNebTarget);
				if ( !pNebTarget->HasPerk(CPERK_UNDEAD) && !pNebTarget->HasPerk(CPERK_LIFELESS) ) pShootAct->m_targetList.AddUnique(pNebTarget);
			}
		}
	}
}

void iBattleEngine::Melee(const iPoint& pos, uint16 mdir)
{
	iBattleUnit_CreatGroup* pCurCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(m_turnSeq.CurUnit());
	check(pCurCreatGroup);
	iBattleGroup* pCurGroup = pCurCreatGroup->GetCreatGroup();

	// Cleanup action list
	m_ActList.Reset();

	// Find target unit
	check(pCurGroup->PassMap().GetAt(pos.x,pos.y) == CT_ENEMYC);
	iBattleGroup* pTarget = FindGroup(pos);
	check(pTarget);

	// calculate suitable position and orientation for melee
	iPoint mp;
	uint8 aor;
	pCurGroup->GetMeleePosition(pTarget, pos, mdir, mp, aor);

	//
	if (pCurGroup->TransType() == TRANS_WALK) {
		// Check condition if we're not need to move
		if (pCurGroup->Size() == 1 && pCurGroup->Pos() == mp) {
			// Rotate to enemy if needed
			if (pCurGroup->Orient() != aor) {
				m_ActList.PushAction(new iBattleAct_Rotate(pCurGroup));
			}
		} else if (pCurGroup->Size() == 2 && pCurGroup->Pos() == mp) {
			// We not need to rotate
			check(pCurGroup->Orient() == aor);
		} else if (pCurGroup->Size() == 2 && pCurGroup->TailPos() == mp) {
			// Rotate to enemy if needed
			check(pCurGroup->Orient() != aor);
			m_ActList.PushAction(new iBattleAct_Rotate(pCurGroup));
		} else {
			iBattlePF pf(pCurGroup);
			pf.MakePath(mp, aor, false, m_ActList);

		}
	} else if (pCurGroup->TransType() == TRANS_FLY) {
		// check if creature already on attack point (i.e. we not need to move)
		if (pCurGroup->IsGroupCell(mp)) {
			if (pCurGroup->Orient() != aor) m_ActList.AddAction(new iBattleAct_Rotate(pCurGroup));
		} else {
			iPoint mpos(mp);
			uint8 od = pCurGroup->Orient();
			uint8 td = od;
			uint8 dd = BattleCellsOrient(pCurGroup->Pos().x,pCurGroup->Pos().y, pos.x,pos.y, od);
			uint8 md = aor;

			// check actual moving direction (rotate in case of backward movement)
			if (td != dd) {
				m_ActList.AddAction(new iBattleAct_Rotate(pCurGroup));
				td = dd;
			}

			if (pCurGroup->Size() == 2 && td != md) {
				mpos.x += TAIL_OFFSET[md];
			}

			m_ActList.AddAction(new iBattleAct_Move(pCurGroup, iPoint(mpos.x,mpos.y)));

			if (td != md) {
				m_ActList.AddAction(new iBattleAct_Rotate(pCurGroup));
				td = md;
			}
		}
	} else {
		// unknown transportation type
		check(0);
	}

	// target rotates to assaulter (if required)
	if (pTarget->Orient() == aor) m_ActList.AddAction(new iBattleAct_Rotate(pTarget));

	// Attack
	MakeMeleeSequence(pCurGroup, pTarget, pos, 1, mdir, false, m_ActList);

	// target retails to assaulter if (not retailed yet or [CPERK_RETALTOALL]) and not [CPERK_NONRETALATTACK])
	if (pTarget->CanRetail() && !pCurGroup->HasPerk(CPERK_NONRETALATTACK)) {
		// calculate retalation pos and direction
		iPoint rpos = mp;
		uint16 rdir = iWRAP(mdir+6,0,12);
		// calculate penalty
		sint32 penalty = 1;
		if (pTarget->IsBlinded()) {
			penalty = pTarget->GetRetPenalty();
		}
		// retail
		check(penalty > 0 && penalty < 4);
		MakeMeleeSequence(pTarget, pCurGroup, rpos, penalty, rdir, true, m_ActList);
	} else if (pCurGroup->HasPerk(CPERK_DOUBLEATTACK)) {
		// insert delay in case of double attack without retalation
		m_ActList.AddAction(new iBattleAct_Delay());
	}

	// additional attack if [CPERK_DOUBLEATTACK]
	if (pCurGroup->HasPerk(CPERK_DOUBLEATTACK)) {
		MakeMeleeSequence(pCurGroup, pTarget, pos, 1, mdir, false, m_ActList);
	} 

	// target rotates to its normal orientation (if required)
	if (m_ActList.EndDir(pTarget, pTarget->Orient()) != pTarget->Orient()) {
		m_ActList.AddAction(new iBattleAct_Rotate(pTarget));
	}

	// assaulter rotates to its normal orientation (if required)
	if (m_ActList.EndDir(pCurGroup, pCurGroup->Orient()) != pCurGroup->Orient()) {
		m_ActList.AddAction(new iBattleAct_Rotate(pCurGroup));
	}

	// Delay after melee action
	m_ActList.AddAction(new iBattleAct_Delay());

	// Insert open/close gate actions
	//ProcessGate(pCurGroup);

	// Process Moat
	ProcessMoat(pCurGroup);

	// Add good morale action if required
	if (pCurGroup->RoundMorale() == iBattleGroup::MLM_POSITIVE) {
		m_ActList.AddAction(new iBattleAct_GoodMorale(pCurGroup));
	}

	// Start Action
	check(m_ActList.Count());
	if (m_ActList.Count()) {
		check(!m_pCurAct);
		StepAction();
	}
}

void iBattleEngine::Shot(const iPoint& pos, sint32 penalty)
{
	if (iBattleUnit_CreatGroup* pCurCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(m_turnSeq.CurUnit())) {
		iBattleGroup* pCurGroup = pCurCreatGroup->GetCreatGroup();

		// rotate to target
		uint8 aor = BattleCellsOrient(pCurGroup->Pos().x,pCurGroup->Pos().y,pos.x,pos.y,pCurGroup->Orient());
		if (pCurGroup->Orient() != aor) m_ActList.AddAction(new iBattleAct_Rotate(pCurGroup));

		// Find target unit
		check(pCurGroup->PassMap().GetAt(pos.x,pos.y) == CT_ENEMYC);
		iBattleGroup* pTarget = FindGroup(pos);
		check(pTarget);


		// shoot
		MakeShootSequence(pCurGroup, pTarget, pos, penalty, m_ActList);

		// [CPERK_DOUBLESHOT] shoot again
		// FIXED: SiGMan (Check we actually have enough shots to DOUBLESHOOT)
		if (pCurGroup->HasPerk(CPERK_DOUBLESHOT) && (pCurGroup->Shots() > 1) ) {
			m_ActList.AddAction(new iBattleAct_Delay());
			MakeShootSequence(pCurGroup, pTarget, pos, penalty, m_ActList);
		}

		// assaulter rotates to its normal orientation (if required)
		if (m_ActList.EndDir(pCurGroup, pCurGroup->Orient()) != pCurGroup->Orient()) {
			m_ActList.AddAction(new iBattleAct_Rotate(pCurGroup));
		}

		// Always delay after range attack
		m_ActList.AddAction(new iBattleAct_Delay());

		// In Moat?
		if (pCurGroup->InMoat()) {
			check(m_pCastleFort);
			if (!m_pCastleFort) {
				DumpBattleUnit(m_turnSeq.CurUnit());
			}
			m_ActList.AddAction(new iBattleAct_MoatDmg(new iBattleUnit_Moat(m_dArmy.Owner(), m_pWrapper), pCurGroup));
		}

		// Add good morale action if required
		if (pCurGroup->RoundMorale() == iBattleGroup::MLM_POSITIVE) {
			m_ActList.AddAction(new iBattleAct_GoodMorale(pCurGroup));
		}
	} else if (iBattleUnit_Catapult* pCatapult = DynamicCast<iBattleUnit_Catapult*>(m_turnSeq.CurUnit())) {
	} else if (iBattleUnit_Turret* pTurret = DynamicCast<iBattleUnit_Turret*>(m_turnSeq.CurUnit())) {
	}

	// Start Action
	check(m_ActList.Count());
	if (m_ActList.Count()) {
		check(!m_pCurAct);
		StepAction();
	}
}

void iBattleEngine::BadMorale()
{
	iBattleUnit_CreatGroup* pCurCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(m_turnSeq.CurUnit());
	check(pCurCreatGroup);
	iBattleGroup* pCurGroup = pCurCreatGroup->GetCreatGroup();

	m_ActList.AddAction(new iBattleAct_BadMorale(pCurGroup));
	check(!m_pCurAct);
	StepAction();
}

bool iBattleEngine::CanWait() const
{
	//check(m_turnSeq.GetSize() && m_pCurUnit == m_turnSeq[0]);
	return m_turnSeq.CurUnit()->CanWait();
}

void iBattleEngine::Wait()
{
	_LOG(_T("Wait\r\n"));
	iBattleUnit_CreatGroup* pCurCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(m_turnSeq.CurUnit());
	check(pCurCreatGroup);
	iBattleGroup* pCurGroup = pCurCreatGroup->GetCreatGroup();
	check(m_turnSeq.CurUnit()->CanWait());
	pCurGroup->Wait();
	m_pWrapper->AddLogEvent(iStringT(_T("#S0")) + GetUnitsColor(m_turnSeq.CurUnit()->Owner()->Owner()) + iFormat(gTextMgr[TRID_MSG_BAT_WAIT],gTextMgr[TRID_CREATURE_PEASANT_F3+pCurGroup->Type()*3]));
	m_turnSeq.Wait();
	PrepareDistMap();
}

bool iBattleEngine::Defend()
{
	iBattleUnit_CreatGroup* pCurCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(m_turnSeq.CurUnit());
	check(pCurCreatGroup);
	iBattleGroup* pCurGroup = pCurCreatGroup->GetCreatGroup();
	m_pWrapper->AddLogEvent(iStringT(_T("#S0")) + GetUnitsColor(m_turnSeq.CurUnit()->Owner()->Owner()) + iFormat(gTextMgr[TRID_MSG_BAT_DEFEND],gTextMgr[TRID_CREATURE_PEASANT_F3+pCurGroup->Type()*3],1));
	pCurGroup->Defend();
	// In Moat?
	if (pCurGroup->InMoat()) {
		pCurGroup->ResetRoundMorale();
		m_ActList.AddAction(new iBattleAct_MoatDmg(new iBattleUnit_Moat(m_dArmy.Owner(), m_pWrapper), pCurGroup));
		check(!m_pCurAct);
		StepAction();
		return true;
	} else {
		if (!m_turnSeq.Defend()) {
			NextRound();
		}
		PrepareDistMap();
		return false;
	}
}

void iBattleEngine::CatapultShot(iCastleFort::ElementType target)
{
	check(m_pCastleFort);
	iBattleUnit_Catapult* pCatapult = DynamicCast<iBattleUnit_Catapult*>(m_turnSeq.CurUnit());
	check(pCatapult);
	iBattleMember_Hero* pHero = DynamicCast<iBattleMember_Hero*>(pCatapult->Owner());
	check(pHero);
	sint32 bal = pHero->GetHero()->FurtSkill(FSK_BALLISTICS);

	for (sint32 xx=0; xx<CATAPULT_PROPS[bal][2]; ++xx) {
		m_ActList.AddAction(new iBattleAct_Catapult(pCatapult, m_pCastleFort, target));
		m_ActList.AddAction(new iBattleAct_Delay());
		target = iCastleFort::InvalidElement;
	}

	check(!m_pCurAct);
	StepAction();
}

void iBattleEngine::TurretShot(iBattleGroup* pTarget)
{
	check(m_pCastleFort);
	iBattleUnit_Turret* pCurTurret = DynamicCast<iBattleUnit_Turret*>(m_turnSeq.CurUnit());
	check(pCurTurret);
	m_ActList.AddAction(new iBattleAct_Turret(pCurTurret, pTarget));
	m_ActList.AddAction(new iBattleAct_Delay());
	check(!m_pCurAct);
	StepAction();
}

iBattleAct* iBattleEngine::StepAction()
{
	iBattleAct* pRes = NULL;
	if (m_pCurAct) {
		pRes = m_ActList.StepAction();
		check(pRes == m_pCurAct);
		m_pCurAct->EndAct();

		// check for end of battle
		if (CheckBattleState() != BR_NA) {
			m_ActList.Reset();
			return m_pCurAct;
		}

		// remove dead actors from action sequence and normalize current action
		while (m_ActList.Count() && (!(*m_ActList.First())->IsValid() || !(*m_ActList.First())->Normalize())) {
			iBattleAct* pAct = m_ActList.StepAction();
			check(pAct);
			delete pAct;
		}
	} 

	if (m_ActList.Count()) {
		if (!m_pCurAct) {
			_LOG(_T("{ //Start action\r\n"));
		}

		// Process gate open/close
		if (m_pCastleFort) {
			iCastleFort::iElement_Bridge* pBridge = (iCastleFort::iElement_Bridge*)m_pCastleFort->GetElement(iCastleFort::Bridge);
			if (iBattleAct_Move* pMoveAct = DynamicCast<iBattleAct_Move*>(*m_ActList.First())) {
				iBattleGroup* pActor = pMoveAct->m_pActor;
				if (!pMoveAct->m_bridgeFlag && pBridge->state !=  iCastleFort::Destroyed) {
					iPoint cpos = pActor->Pos();
					bool bCurAtBridge = (pActor->PassMap().GetAt(cpos.x,cpos.y)&0x7F) == CT_BRIDGE || (pActor->Size() == 2 && (pActor->PassMap().GetAt(cpos.x+TAIL_OFFSET[pActor->Orient()],cpos.y)&0x7F) == CT_BRIDGE);
					iPoint npos = pMoveAct->m_target;
					bool bNewAtBridge = (pActor->PassMap().GetAt(npos.x,npos.y)&0x7F) == CT_BRIDGE || (pActor->Size() == 2 && (pActor->PassMap().GetAt(npos.x+TAIL_OFFSET[pActor->Orient()],npos.y)&0x7F) == CT_BRIDGE);
					if (bCurAtBridge && !bNewAtBridge) {
						pMoveAct->m_bridgeFlag = 1;
						m_ActList.InsertActionAfter(m_ActList.First(), new iBattleAct_Gate(pActor, pBridge, false));
					} else if (!bCurAtBridge && bNewAtBridge) {
						pMoveAct->m_bridgeFlag = 1;
						m_ActList.PushAction(new iBattleAct_Gate(pActor, pBridge, true));
					}
				}
			}
		}

		m_pCurAct = *m_ActList.First();
		m_pCurAct->BeginAct();
	} else {
		check(m_pCurAct);
		m_pCurAct = NULL;
		_LOG(_T("} // End action\r\n"));
	}



	// remove all dead groups from sequence
	m_turnSeq.UpdateSequence(iTurnSeq::USF_ACTIVES | iTurnSeq::USF_DEADS);
	
	return pRes;
}

/*
 *	
 */
iBattleWrapper::iBattleWrapper(bool bFakeMode)
: m_engine(this, bFakeMode)
{}

void iBattleWrapper::BeginBattle(iBattleInfo bi)
{
	m_engine.BeginBattle(bi);
	OnBeginBattle();
	m_engine.Start();
	OnStart();
}

void iBattleWrapper::EndBattle(iBattleResult br)
{
	m_result = br;
	iBattleInfo bi = m_engine.GetBattleInfo();
	OnEndBattle();
	m_engine.EndBattle(br);
	bi.Cleanup();
	gGame.EndBattle();
}

