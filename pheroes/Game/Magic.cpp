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
#include "Dlg_TownList.h"

//
const SPELL_LABEL_DESC_STRUCT SPELL_LABEL_DESCRIPTORS[SLB_COUNT] = {
	{SPD_POSITIVE, TRID_SPNAME_PROTEARTH},		// SLB_PROTEARTH
	{SPD_POSITIVE, TRID_SPNAME_HASTE},			// SLB_HASTE
	{SPD_POSITIVE, TRID_SPNAME_SHIELD},			// SLB_SHIELD
	{SPD_POSITIVE, TRID_SPNAME_PRECISION},		// SLB_PRECISION
	{SPD_POSITIVE, TRID_SPNAME_AIRSHIELD},		// SLB_AIRSHIELD
	{SPD_POSITIVE, TRID_SPNAME_COUNTERSTRIKE},	// SLB_COUNTERSTRIKE
	{SPD_POSITIVE, TRID_SPNAME_PROTAIR},		// SLB_PROTAIR
	{SPD_NEGATIVE, TRID_SPNAME_SLOW},			// SLB_SLOW
	{SPD_POSITIVE, TRID_SPNAME_STONESKIN},		// SLB_STONESKIN
	{SPD_NEGATIVE, TRID_SPNAME_SORROW},			// SLB_SORROW
	{SPD_POSITIVE, TRID_SPNAME_BLOODLUST},		// SLB_BLOODLUST
	{SPD_POSITIVE, TRID_SPNAME_PROTWATER},		// SLB_PROTWATER
	{SPD_NEGATIVE, TRID_SPNAME_CURSE},			// SLB_CURSE
	{SPD_NEGATIVE, TRID_SPNAME_BLIND},			// SLB_BLIND
	{SPD_NEGATIVE, TRID_SPNAME_WEAKNESS},		// SLB_WEAKNESS
	{SPD_NEGATIVE, TRID_SPNAME_MISFORTUNE},		// SLB_MISFORTUNE
	{SPD_POSITIVE, TRID_SPNAME_BLESS},			// SLB_BLESS
	{SPD_POSITIVE, TRID_SPNAME_PROTFIRE},		// SLB_PROTFIRE
	{SPD_POSITIVE, TRID_SPNAME_FORTUNE},		// SLB_FORTUNE
	{SPD_POSITIVE, TRID_SPNAME_MIRTH},			// SLB_MIRTH
	{SPD_POSITIVE, TRID_SPNAME_ANTIMAGIC},		// SLB_ANTIMAGIC
	{SPD_POSITIVE, TRID_SPNAME_PRAYER},			// SLB_PRAYER
	{SPD_NEGATIVE, TRID_SPNAME_DISRAY}			// SLB_DISRAY
};

// Magic school colors (Air, Earth, Fire, Water), (Background, Foreground)
const iDib::pixel MSCH_COLORS[MST_COUNT][2] = {
	{
		RGB16(148,190,198),
		RGB16(18,23,127)
	},{
		RGB16(142,232,111),
		RGB16(41,110,18)
	},{
		RGB16(228,156,156),
		RGB16(132,12,12)
	},{
		RGB16(128,148,255),
		RGB16(4,16,128)
	}
};

/*
 *	Base spell class
 */
iBaseSpell::iBaseSpell(MAGIC_SPELL spellId) : m_spellId(spellId) {}

MAGIC_SCHOOL_LEVEL iBaseSpell::GetSchoolLevel(MAGIC_SPELL spellId, const iHero* pCaster)
{ 
	if (!pCaster) return MSL_NONE;
	return (MAGIC_SCHOOL_LEVEL)pCaster->FurtSkill((FURTHER_SKILLS)(FSK_MAG_AIR+SPELL_DESCRIPTORS[spellId].school));
}

uint8 iBaseSpell::GetSpellCost(MAGIC_SPELL spellId, const iHero* pCaster)
{
	uint8 res = SPELL_DESCRIPTORS[spellId].bcost;
	MAGIC_SCHOOL_LEVEL msl = GetSchoolLevel(spellId, pCaster);
	if (msl > MSL_NONE) res -= (SPELL_DESCRIPTORS[spellId].level+1);
	return res;
}

/*
 *	Overland spell
 */
bool iOverlandSpell::CanCast(iHero* pCaster)
{
	return true;
}

bool iOverlandSpell::Cast(iHero* pCaster)
{
	return CanCast(pCaster);
}

void iOverlandSpell::OnRemove(iHero* pCaster, sint32 param)
{

}

//////////////////////////////////////////////////////////////////////////
bool iSpell_TownPortal::Cast(iHero* pCaster)
{
	iOverlandSpell::Cast(pCaster);
	MAGIC_SCHOOL_LEVEL msl = GetSchoolLevel(m_spellId, pCaster);

	iPlayer* pOwner = pCaster->Owner();
	if (pOwner->PlayerType() == PT_HUMAN) {
		if (SPELL_DESCRIPTORS[m_spellId].eff[msl].fparam) {
			// Town portal
			iDlg_TownList tlDlg(&gApp.ViewMgr(), pOwner, gTextMgr[TRID_SELECT_DESTINATION], TRID_OK);
			if (tlDlg.DoModal() == DRC_OK) {
				iCastle* pDest = tlDlg.Destination();
				check(pDest && !pDest->Visitor());
				iPoint oldPos = pCaster->Pos();
				gGame.Map().MoveHero(pCaster->Pos(), pDest->Pos());
				pCaster->SetPos(pDest->Pos());
				pCaster->ResetPath();
				pCaster->Angle() = HERO_ORIENT[pDest->Proto()->Orient()];
				gGame.OnHeroChanged(pCaster);
				gGame.OnHeroTeleport(pCaster, oldPos, pDest->Pos());
				gSfxMgr.PlaySound(CSND_TELEPORT_OUT);
				return true;
			}
		} else {
			// Town gate
			iPoint hpos = pCaster->Pos();
			iCastle* pDest = NULL;
			sint32 dist = 0;
			for (iPlayer::iCtLIt cit = pOwner->CastleFirst(); cit != pOwner->CastleEnd(); ++cit) {
				sint32 nval = iABS(hpos.x - (*cit)->Pos().x) + iABS(hpos.y - (*cit)->Pos().y);
				if (!pDest || nval < dist) {
					pDest = *cit;
					dist = nval;
				}
			}
			if (pDest && !pDest->Visitor()) {
				//pCaster->Deinit(false);
				//pCaster->Init(pOwner, pDest->Pos(), HERO_ORIENT[pDest->Proto()->Orient()]);
				//gGame.Map().MoveHero(hpos, pDest->Pos());
				gGame.Map().MoveHero(pCaster->Pos(), pDest->Pos());
				pCaster->SetPos(pDest->Pos());
				pCaster->ResetPath();
				pCaster->Angle() = HERO_ORIENT[pDest->Proto()->Orient()];
				gGame.OnHeroChanged(pCaster);
				gGame.OnHeroTeleport(pCaster, hpos, pDest->Pos());
				gSfxMgr.PlaySound(CSND_TELEPORT_OUT);
				return true;
			}
		}
	} else {
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool iSpell_Vision::Cast(iHero* pCaster)
{
	if (!iOverlandSpell::Cast(pCaster)) return false;
	MAGIC_SCHOOL_LEVEL msl = GetSchoolLevel(m_spellId, pCaster);
	pCaster->AddCharm(m_spellId, SPELL_DESCRIPTORS[m_spellId].eff[msl].fparam);
	return true;
}

void iSpell_Vision::OnRemove(iHero* pCaster, sint32 param)
{
}

/*
 *	Combat spell
 */
bool AddSpellToList(iBattleGroup* pTarget, const iCombatSpell* pSpell, sint32 duration, sint32 param)
{
	for (uint32 xx=0; xx<pTarget->SpellList().GetSize(); ++xx) {
		if (pTarget->SpellList()[xx].m_pSpell->Id() == pSpell->Id()) {
			pTarget->SpellList()[xx].m_duration = duration;
			return false;
		}
	}
	pTarget->AddSpell(pSpell, duration, param);
	return true;
}

//////////////////////////////////////////////////////////////////////////
iCombatSpell::iCombatSpell(MAGIC_SPELL spellId) 
: iBaseSpell(spellId) 
{ 
	check(SPELL_DESCRIPTORS[spellId].type == SPT_COMBAT); 
}

bool iCombatSpell::CanCast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const
{
	check(pCaster);
	MAGIC_SCHOOL_LEVEL msl = GetSchoolLevel(pCaster->GetHero());
	uint16 tt = TargetType(msl);
	bool bFriendly = pCaster->Owner()->GetSide() == pTarget->Owner()->GetSide();
	// decline if we need a dead stack, but its not
	if ( !AffectsToDead() && !pTarget->IsAlive() ) return false;
	// decline if ?
	if ( pTarget->Perks() & (1 << (School() + 12)) ) return false;
	// decline if target is immune
	if ( pTarget->SpellImmunity((SPELL_LEVEL)Level()) ) return false;
	// decline if we dont want friendly but it is
	if ( (tt & STT_FRIENDLY) == 0 && bFriendly ) return false;
	// decline if we dont want enemy but it is
	if ( (tt & STT_ENEMY) == 0 && !bFriendly ) return false;
	// decline if we dont want undead but it is
	if ( (tt & STT_UNDEAD) == 0 && (pTarget->Perks() & CPERK_UNDEAD) != 0 ) return false;
	if ( (tt & STT_LIFELESS) == 0 && (pTarget->Perks() & CPERK_LIFELESS) != 0 ) return false;
	if ( (tt & STT_NORMAL) == 0 && (pTarget->Perks() & CPERK_LIFELESS) == 0 && (pTarget->Perks() & CPERK_UNDEAD) == 0) return false;
	if ( (tt & STT_TROOPS) == 0 && !pTarget->CanShoot() ) return false;
	if ( (tt & STT_SHOOTERS) == 0 && pTarget->CanShoot() ) return false;
	return true;
}

bool iCombatSpell::Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
sint32 iSpell_Damage::EstimateDamage(iHero* pCaster, iBattleGroup* pTarget) const
{
	MAGIC_SCHOOL_LEVEL msl = GetSchoolLevel(pCaster);
	// basic damage
	sint32 damage = pCaster->FurtSkill(FSK_POWER) * SPELL_DESCRIPTORS[m_spellId].eff[msl].fparam;
	// sorcery modifier
	if (sint32 sorcery = pCaster->FurtSkill(FSK_SORCERY)) damage += (damage * sorcery) / 100;
	// SHF_DMGSPBONUS
	if (pCaster->HasSpecFlag(SHF_DMGSPBONUS)) damage += (damage>>1);
	if (pTarget) {
		// resistance modifier
		if (sint32 resist = pTarget->FurtSkills().Value((FURTHER_SKILLS)(FSK_RES_AIR + School()))) damage -= (damage * resist) / 100;
		// Special creatures modifier
		if (CREAT_DESC[pTarget->Type()].perks & CPERK_QUARTERDMG) damage = iMAX<sint32>(damage>>2,1);
	}
	return damage;
}

bool iSpell_Damage::Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const
{
	if (!iCombatSpell::Cast(pCaster, pTarget)) return false;
	sint32 damage = EstimateDamage(pCaster->GetHero(), pTarget);
	iBattleWrapper* pWrap = pTarget->BattleWrapper();

	iStringT cellMsg(FormatNumber(damage));
	if (sint32 dead = pTarget->ReceiveDamage(damage, false)){
		cellMsg.Addf(_T(" (-%d)"), dead);
	}

	if (pTarget->Owner()->GetSide() != pCaster->Owner()->GetSide()) {
		pCaster->Owner()->AddExperience(damage);
	}
	pWrap->AddCellEvent(cellMsg, pTarget->Pos());

	return true;
}

iStringT iSpell_Damage::ActionText(iHero* pCaster) const
{
	return iFormat(gTextMgr[TRID_MSG_SPELL_DOES_DAMAGE], EstimateDamage(pCaster));
}

//////////////////////////////////////////////////////////////////////////
bool iSpell_FurtSkill::Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const
{
	if (!iCombatSpell::Cast(pCaster, pTarget)) return false;

	MAGIC_SCHOOL_LEVEL msl = GetSchoolLevel(pCaster->GetHero());
	sint32 duration = pCaster->GetHero()->FurtSkill(FSK_POWER);
	sint32 param = SPELL_DESCRIPTORS[m_spellId].eff[msl].sparam;
	if (AddSpellToList(pTarget, this, duration, param)) {
		pTarget->FurtSkills().Value((FURTHER_SKILLS)SPELL_DESCRIPTORS[m_spellId].eff[MSL_NONE].fparam) += param;
	}
	return true;
}

void iSpell_FurtSkill::OnRemove(iBattleGroup* pTarget, sint32 param) const
{
	pTarget->FurtSkills().Value((FURTHER_SKILLS)SPELL_DESCRIPTORS[m_spellId].eff[MSL_NONE].fparam) -= param;
}
//////////////////////////////////////////////////////////////////////////
bool iSpell_DisRay::Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const
{
	if (!iCombatSpell::Cast(pCaster, pTarget)) return false;
	MAGIC_SCHOOL_LEVEL msl = GetSchoolLevel(pCaster->GetHero());
	sint32 param = SPELL_DESCRIPTORS[m_spellId].eff[msl].fparam;
	pTarget->FurtSkills().Value(FSK_DEFENCE) -= param;
	pTarget->AddSpell(this, -1, param);
	return true;
}

void iSpell_DisRay::OnRemove(iBattleGroup* pTarget, sint32 param) const
{
	pTarget->FurtSkills().Value(FSK_DEFENCE) += param;
}

//////////////////////////////////////////////////////////////////////////
bool iSpell_Resurrect::CanCast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const
{
	if (!iCombatSpell::CanCast(pCaster, pTarget)) return false;
	return pTarget->CanResurrect();
}

sint32 iSpell_Resurrect::EstimateHitPoints(iHero* pCaster) const
{
	// Restore hit points
	MAGIC_SCHOOL_LEVEL msl = GetSchoolLevel(pCaster);
	sint32 hp = pCaster->FurtSkill(FSK_POWER) * SPELL_DESCRIPTORS[m_spellId].eff[msl].fparam;
	// sorcery modifier
	if (sint32 sorcery = pCaster->FurtSkill(FSK_SORCERY)) hp += (hp * sorcery) / 100;
	// SHF_SUMRESBOUNS
	if (pCaster->HasSpecFlag(SHF_SUMRESBOUNS)) hp += (hp>>1);
	return hp;
}

bool iSpell_Resurrect::Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const
{
	if (!iCombatSpell::Cast(pCaster, pTarget)) return false;

	sint32 hp = EstimateHitPoints(pCaster->GetHero());
	pTarget->Resurrect(hp);

	return true;
}

iStringT iSpell_Resurrect::ActionText(iHero* pCaster) const
{
	return iCombatSpell::ActionText(pCaster);
}

//////////////////////////////////////////////////////////////////////////
iPoint FindSummonCell(const iBattleMap& pmap, iBattleMember::Side side, bool bDoubleSize)
{
	sint32 x;
	sint32 found = 0;
	sint32 ylist[11];

	if (side == iBattleMember::Assaulter) x = 0; else x = 12;
	if (bDoubleSize) x += TAIL_OFFSET[!side];

	while (!found) {
		for (uint32 yy=0; yy<11; ++yy) {
			uint8 pp = pmap.GetAt(x, yy);
			if ( (pmap.GetAt(x, yy)&0x7F) == CT_PASSABLE && (!bDoubleSize || (pmap.GetAt(x+TAIL_OFFSET[side],yy)&0x7F)== CT_PASSABLE)) {
				ylist[found++] = yy;
			}
		}
		if (!found)	x += TAIL_OFFSET[!side];
	}

	check(found>0 && found<=11);
	return iPoint(x,ylist[gGame.Map().Rand(found)]);
}

sint32 iSpell_Summon::EstimateQuant(iHero* pCaster) const
{
	MAGIC_SCHOOL_LEVEL msl = GetSchoolLevel(pCaster);
	// quantity
	sint32 quant = SPELL_DESCRIPTORS[m_spellId].eff[msl].sparam * pCaster->FurtSkill(FSK_POWER);
	// sorcery modifier
	if (sint32 sorcery = pCaster->FurtSkill(FSK_SORCERY)) quant += (quant * sorcery) / 100;
	// SHF_SUMRESBOUNS
	if (pCaster->HasSpecFlag(SHF_SUMRESBOUNS)) quant += (quant>>1);
	return quant;
}

CREATURE_TYPE iSpell_Summon::CreatType() const
{
	return (CREATURE_TYPE)SPELL_DESCRIPTORS[m_spellId].eff[MSL_NONE].fparam;
}

bool iSpell_Summon::Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const
{
	if (!iCombatSpell::Cast(pCaster, pTarget)) return false;
	MAGIC_SCHOOL_LEVEL msl = GetSchoolLevel(pCaster->GetHero());

	sint32 quant = EstimateQuant(pCaster->GetHero());
	iCreatGroup cg(CreatType(), quant);
	iBattleMap pmap;
	pTarget->BattleWrapper()->Engine().MakePassMap(pmap);
	pTarget->BattleWrapper()->Engine().Summon(pCaster->Owner()->GetSide(), cg, FindSummonCell(pmap, pCaster->Owner()->GetSide(), CREAT_DESC[cg.Type()].size==2));
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool iSpell_Bless::Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const
{
	if (!iCombatSpell::Cast(pCaster, pTarget)) return false;
	MAGIC_SCHOOL_LEVEL msl = GetSchoolLevel(pCaster->GetHero());

	pTarget->RemoveSpell((MAGIC_SPELL)SPELL_DESCRIPTORS[m_spellId].eff[msl].sparam);
	sint32 duration = pCaster->GetHero()->FurtSkill(FSK_POWER);
	if (AddSpellToList(pTarget, this, duration, 0)){
		pTarget->SetBlessState(SPELL_DESCRIPTORS[m_spellId].eff[msl].fparam);
	}
	return true;
}

void iSpell_Bless::OnRemove(iBattleGroup* pTarget, sint32 param) const
{
	pTarget->SetBlessState(0);
}
//////////////////////////////////////////////////////////////////////////
bool iSpell_Blind::Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const
{
	if (!iCombatSpell::Cast(pCaster, pTarget)) return false;
	MAGIC_SCHOOL_LEVEL msl = GetSchoolLevel(pCaster->GetHero());

	pTarget->RemoveSpell((MAGIC_SPELL)SPELL_DESCRIPTORS[m_spellId].eff[msl].sparam);
	sint32 duration = pCaster->GetHero()->FurtSkill(FSK_POWER);
	if (AddSpellToList(pTarget, this, duration, 0)){
		pTarget->SetBlindState( true, SPELL_DESCRIPTORS[m_spellId].eff[msl].fparam );
	}
	return true;
}

void iSpell_Blind::OnRemove(iBattleGroup* pTarget, sint32 param) const
{
	pTarget->SetBlindState( false, 1 );
}
//////////////////////////////////////////////////////////////////////////
bool iSpell_Earthquake::CanCast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const
{
	if (!iCombatSpell::CanCast(pCaster, pTarget)) return false;
	return (pTarget->Owner()->GetSide() == iBattleMember::Defender && pTarget->BattleWrapper()->Engine().CastleFort() && pTarget->BattleWrapper()->Engine().CastleFort()->HasWalls());
}

bool iSpell_Earthquake::Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool iSpell_Dispel::CanCast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const
{
	if (!iCombatSpell::CanCast(pCaster, pTarget)) return false;
	return pTarget->SpellList().GetSize() > 0;
}

bool iSpell_Dispel::Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const
{
	if (!iCombatSpell::Cast(pCaster, pTarget)) return false;

	// Remove All spells
	while (pTarget->SpellList().GetSize()) {
		pTarget->RemoveSpellAt(pTarget->SpellList().GetSize() - 1);
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
sint32 iSpell_Cure::EstimateHitPoints(iHero* pCaster) const
{
	MAGIC_SCHOOL_LEVEL msl = GetSchoolLevel(pCaster);
	sint32 hp = pCaster->FurtSkill(FSK_POWER) * SPELL_DESCRIPTORS[m_spellId].eff[msl].fparam;
	// sorcery modifier
	if (sint32 sorcery = pCaster->FurtSkill(FSK_SORCERY)) hp += (hp * sorcery) / 100;
	return hp;
}

bool iSpell_Cure::CanCast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const
{
	if (!iCombatSpell::CanCast(pCaster, pTarget)) return false;
	return pTarget->HitPoints() > pTarget->HitPointsLeft() || pTarget->SpellList().GetSize() > 0;
}

bool iSpell_Cure::Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const
{
	if (!iCombatSpell::Cast(pCaster, pTarget)) return false;

	// Remove all negative spells
	for (uint32 xx=0; xx<pTarget->SpellList().GetSize();) {
		if (pTarget->SpellList()[xx].m_pSpell->SpellDispos() == SPD_NEGATIVE) pTarget->RemoveSpellAt(xx);
		else ++xx;
	}

	// Restore hit points
	sint32 hp = EstimateHitPoints(pCaster->GetHero());
	pTarget->Restore(hp);

	return true;
}
//////////////////////////////////////////////////////////////////////////
bool iSpell_Antimagic::Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const
{
	if (!iCombatSpell::Cast(pCaster, pTarget)) return false;
	MAGIC_SCHOOL_LEVEL msl = GetSchoolLevel(pCaster->GetHero());

	// Remove All spells
	while (pTarget->SpellList().GetSize()) {
		pTarget->RemoveSpellAt(pTarget->SpellList().GetSize() - 1);
	}

	//
	sint32 duration = pCaster->GetHero()->FurtSkill(FSK_POWER);
	if (AddSpellToList(pTarget, this, duration, 0)) {
		pTarget->SetSpellImmunity(SPELL_DESCRIPTORS[m_spellId].eff[msl].fparam);
	}
	return true;
}

void iSpell_Antimagic::OnRemove(iBattleGroup* pTarget, sint32 param) const
{
	pTarget->SetSpellImmunity(SPL_MASK_NONE);
}
//////////////////////////////////////////////////////////////////////////
bool iSpell_Prayer::Cast(iBattleUnit_Hero* pCaster, iBattleGroup* pTarget) const
{
	if (!iCombatSpell::Cast(pCaster, pTarget)) return false;
	MAGIC_SCHOOL_LEVEL msl = GetSchoolLevel(pCaster->GetHero());
	
	sint32 duration = pCaster->GetHero()->FurtSkill(FSK_POWER);
	sint32 param = SPELL_DESCRIPTORS[m_spellId].eff[msl].fparam;
	if (AddSpellToList(pTarget, this, duration,param)) {
		pTarget->FurtSkills().Value(FSK_ATTACK) += param;
		pTarget->FurtSkills().Value(FSK_DEFENCE) += param;
		pTarget->FurtSkills().Value(FSK_SPEED) += param;
	}
	return true;
}

void iSpell_Prayer::OnRemove(iBattleGroup* pTarget, sint32 param) const
{
	pTarget->FurtSkills().Value(FSK_ATTACK) -= param;
	pTarget->FurtSkills().Value(FSK_DEFENCE) -= param;
	pTarget->FurtSkills().Value(FSK_SPEED) -= param;
}

/*
 *	Spell Book
 */
iSpellBook::iSpellBook()
{
}

void iSpellBook::Add(uint16 spellId)
{
	m_spells.AddUnique(spellId);
}

void iSpellBook::Add(const iSpellList& spells)
{
	for (uint32 xx=0; xx<spells.GetSize(); ++xx) {
		Add(spells[xx]);
	}
}

void iSpellBook::AddFavorite(uint16 spellId)
{
	m_favorites.AddUnique(spellId);
}

const iBaseSpell* iSpellBook::SpellByIdx(uint32 idx) const
{
	check(idx<m_spells.GetSize());
	check(m_spells[idx] < (sint32)gGame.ItemMgr().m_spellMgr.SpellsCount());
	return gGame.ItemMgr().m_spellMgr.Spell((MAGIC_SPELL)m_spells[idx]);
}

const iBaseSpell* iSpellBook::FavoriteByIdx(uint32 idx) const
{
	check(idx<m_favorites.GetSize());
	check(m_favorites[idx] < (sint32)gGame.ItemMgr().m_spellMgr.SpellsCount());
	return gGame.ItemMgr().m_spellMgr.Spell((MAGIC_SPELL)m_favorites[idx]);
}

void iSpellBook::Reset()
{
	m_spells.RemoveAll();
}

