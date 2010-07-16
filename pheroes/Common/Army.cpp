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

//////////////////////////////////////////////////////////////////////////
iCreatGroup::iCreatGroup(CREATURE_TYPE ct, sint32 count)
: m_creatType(ct), m_count(count) 
{
}

const sint32 CREAT_GROWTH_DIVIDER[6] = {
	9, 9, 10, 10, 11, 12
};

void iCreatGroup::Grow(uint32 weeks)
{
	check(m_count != RAND_VAL);
	while (weeks) {
		sint32 div = CREAT_GROWTH_DIVIDER[CREAT_DESC[m_creatType].level-1];
		if (m_count < CREAT_DESC[m_creatType].growth*2) div /= 2;
		
		m_count = m_count + (m_count+(div-1)) / div;

		weeks--;
	}
}

//////////////////////////////////////////////////////////////////////////
bool iArmy::CanAddGroup(CREATURE_TYPE ct) const
{
	check(ct != CREAT_UNKNOWN);
	for (uint32 xx=0; xx<7; ++xx){
		if (!m_creatGroups[xx].IsValid() ||  m_creatGroups[xx].Type() == ct) return true;
	}
	return false;
}

sint32 iArmy::CreatureCount(CREATURE_TYPE ct) 
{
	sint32 res = 0;
	for (uint32 xx=0; xx<7; ++xx)
		if (m_creatGroups[xx].Type() == ct)
			res += m_creatGroups[xx].Count();
	return res;
}

bool iArmy::AddGroup(CREATURE_TYPE ct, sint32 count)
{
	sint32 fEmpty = -1;

	check(ct != CREAT_UNKNOWN);
	
	//first try to find similar type group
	for (uint32 xx=0; xx<7; ++xx){
		if (m_creatGroups[xx].Type() == ct){
			m_creatGroups[xx].Count() += count;			
			if(m_creatGroups[xx].Count() <= 0)
				m_creatGroups[xx].Type() = CREAT_UNKNOWN;
			return true;
		} else if (fEmpty == -1 && !m_creatGroups[xx].IsValid()) {
			fEmpty = xx;
		}
	}

	if(count > 0){
		if (fEmpty != -1){
			m_creatGroups[fEmpty].Type() = ct;
			m_creatGroups[fEmpty].Count() = count; 
			return true;
		}
	}
	else
		return true; //no need to select a place for negative

	// there is no free cell
	return false;
}

sint32 iArmy::SlowestSpeed() const
{ 
	sint32 res=SPEED_MAX; 
	for (uint32 xx=0; xx<7; ++xx) 
		if (m_creatGroups[xx].IsValid() && CREAT_DESC[m_creatGroups[xx].Type()].speed < res) 
			res = CREAT_DESC[m_creatGroups[xx].Type()].speed; 
	return res; 
}

iCreatGroup& iArmy::WeakestCreatures()
{
	sint32 idx=-1;
	for (sint32 xx=0; xx<7; ++xx) {
		if (m_creatGroups[xx].Type() != CREAT_UNKNOWN && (idx == -1 || CREAT_DESC[m_creatGroups[idx].Type()].pidx > CREAT_DESC[m_creatGroups[xx].Type()].pidx)) idx=xx;
	}
	check(idx != -1);
	return m_creatGroups[idx];
}

iCreatGroup& iArmy::WeakestGroup()
{
	sint32 idx=-1;
	for (sint32 xx=0; xx<7; ++xx) {
		if (m_creatGroups[xx].Type() != CREAT_UNKNOWN && (idx == -1 || m_creatGroups[idx].GroupPower()>m_creatGroups[xx].GroupPower())) idx=xx;
	}
	check(idx != -1);
	return m_creatGroups[idx];
}

bool iArmy::JoinGroups()
{
	bool bJoined = false;
	for (uint32 xx=0; xx<7; ++xx) {
		for (uint32 yy=(xx+1); yy<7; ++yy) {
			if (m_creatGroups[xx].Type() != CREAT_UNKNOWN && m_creatGroups[xx].Type() == m_creatGroups[yy].Type()) {
				m_creatGroups[xx].Count() += m_creatGroups[yy].Count();
				m_creatGroups[yy].Reset();
				bJoined = true;
			}
		}
	}
	return bJoined;
}

void iArmy::ResetGroup(uint32 idx)
{ 
	m_creatGroups[idx].Reset();
}

void iArmy::ResetArmy()
{
	for (uint32 xx=0; xx<7; ++xx) m_creatGroups[xx].Reset();
}

void iArmy::SetGroup(uint32 idx, CREATURE_TYPE ct, sint32 count)
{
	check(idx<7);
	m_creatGroups[idx].Type() = ct;
	m_creatGroups[idx].Count() = count;
}

CREATURE_TYPE iArmy::StrongestCreature() const
{
	CREATURE_TYPE ct = CREAT_UNKNOWN;
	sint32 pidx = 0;
	for (uint32 xx=0; xx<7; ++xx) {
		if (m_creatGroups[xx].Type() != CREAT_UNKNOWN && CREAT_DESC[m_creatGroups[xx].Type()].pidx > pidx) {
			pidx = CREAT_DESC[m_creatGroups[xx].Type()].pidx;
			ct = m_creatGroups[xx].Type();
		}
	}
	check(ct != CREAT_UNKNOWN);
	return ct;
}

sint32 iArmy::MoraleModifier() const
{
	sint32 res = 0;
	bool bUndeads = false;
	uint32 alignments = 0;
	uint32 nat_mask = 0;
	for (uint32 xx=0; xx<7; ++xx) {
		if (m_creatGroups[xx].Type() != CREAT_UNKNOWN && (nat_mask & (1<<CREAT_DESC[m_creatGroups[xx].Type()].nation)) == 0 ) {
			alignments ++;
			nat_mask |= (1<<CREAT_DESC[m_creatGroups[xx].Type()].nation);
			if (CREAT_DESC[m_creatGroups[xx].Type()].nation == NATION_UNDEADS) bUndeads = true;
		}
	}

	// Troops of >=5 alignment -3
	if (alignments >= 5) res -= 3;
	// Troops of 4 alignment -2
	else if (alignments == 4) res -= 2;
	// Troops of 3 alignment -1
	else if (alignments == 3) res -= 1;
	// All troops of one alignment +1
	else if (alignments <= 1) res += 1;
	// Undead in army -1
	if (bUndeads) res -= 1;
	return res;
}

