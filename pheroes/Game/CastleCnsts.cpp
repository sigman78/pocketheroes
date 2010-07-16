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
#include "helpers.h"

//////////////////////////////////////////////////////////////////////////
const CTL_CONSTR DEFAULT_CONSTRUCTIONS[CTLT_COUNT][2] = {
	{ CTLCNST_TAVERN, CTLCNST_DWELL_PEASNHUT },
	{ CTLCNST_TAVERN, CTLCNST_DWELL_GBARRACKS },
	{ CTLCNST_TAVERN, CTLCNST_DWELL_MAGICSCHOOL },
	{ CTLCNST_TAVERN, CTLCNST_DWELL_CENTCAVE },
	{ CTLCNST_TAVERN, CTLCNST_DWELL_TREEHOUSE },
	{ CTLCNST_TAVERN, CTLCNST_DWELL_EXCAVATION }
};


//////////////////////////////////////////////////////////////////////////
iCtlCnst::iCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst)
: pCastle(_pCastle), cnst(_cnst), state(Unavailable), bInited(false)
{}

void iCtlCnst::Init()
{
	check(state == Built && !bInited);
	OnInit();
	if (pCastle->Visitor()) OnVisitorEnter(pCastle->Visitor());
	bInited = true;
}

void iCtlCnst::Build()
{
	check(state != Built);
	state = Built;
	OnBuild();
	Init();
}

iStringT iCtlCnst::Name(CTL_CONSTR _cnst)
{
	return gTextMgr[TRID_DWEL_CITD_L1 + _cnst]; 
}

iStringT iCtlCnst::Desc() const
{
	switch (cnst){
	case CTLCNST_MAGICNODE: return gTextMgr[TRID_CTLCNST_COMMON_MNODE_DESC];
	case CTLCNST_TAVERN: return gTextMgr[TRID_CTLCNST_COMMON_TAVERN_DESC];
	case CTLCNST_MARKETPLACE: return gTextMgr[TRID_CTLCNST_COMMON_MARKET_DESC];
	case CTLCNST_MOAT: return gTextMgr[TRID_CTLCNST_COMMON_MOAT_DESC];
	case CTLCNST_MTURRET: return gTextMgr[TRID_CTLCNST_COMMON_MTURRET_DESC];
	case CTLCNST_RTURRET: return gTextMgr[TRID_CTLCNST_COMMON_RTURRET_DESC];
	case CTLCNST_LTURRET: return gTextMgr[TRID_CTLCNST_COMMON_LTURRET_DESC];

	case CTLCNST_TOWNHALL: return gTextMgr[TRID_CTLCNST_COMMON_TOWNHALL_DESC];
	case CTLCNST_CITYHALL: return gTextMgr[TRID_CTLCNST_COMMON_CITYHALL_DESC];
	case CTLCNST_OREMINE: return gTextMgr[TRID_CTLCNST_COMMON_OREMINE_DESC];
	case CTLCNST_SAWMILL: return gTextMgr[TRID_CTLCNST_COMMON_SAWMILL_DESC];
	case CTLCNST_ALCHLAB: return gTextMgr[TRID_CTLCNST_COMMON_ALCHLAB_DESC];
	case CTLCNST_GEMSMINE: return gTextMgr[TRID_CTLCNST_COMMON_GEMSMINE_DESC];
	case CTLCNST_CRYSTALMINE: return gTextMgr[TRID_CTLCNST_COMMON_CRYSTALMINE_DESC];
	case CTLCNST_SULFURMINE: return gTextMgr[TRID_CTLCNST_COMMON_SULFURMINE_DESC];
	case CTLCNST_ALTAR: return gTextMgr[TRID_CTLCNST_ALTAR_DESC];

	case CTLCNST_OBSERVPOST: return gTextMgr[TRID_CTLCNST_OBSERVPOST_DESC];
	case CTLCNST_FORTIFICATION: return gTextMgr[TRID_CTLCNST_FORTIFICATION_DESC];
	case CTLCNST_HALLOFVALHALLA: return gTextMgr[TRID_CTLCNST_HALLOFVALHALLA_DESC];
	case CTLCNST_WALLOFKNOWLEDGE: return gTextMgr[TRID_CTLCNST_WALLOFKNOWLEDGE_DESC];
	case CTLCNST_LIBRARY: return gTextMgr[TRID_CTLCNST_LIBRARY_DESC];
	case CTLCNST_MANAVORTEX: return gTextMgr[TRID_CTLCNST_MANAVORTEX_DESC];
	case CTLCNST_TREASURY: return gTextMgr[TRID_CTLCNST_TREASURY_DESC];
	case CTLCNST_MYSTICPOUND: return gTextMgr[TRID_CTLCNST_MYSTICPOUND_DESC];
	case CTLCNST_NECRAMPLIFIER: return gTextMgr[TRID_CTLCNST_NECRAMPLIFIER_DESC];
	case CTLCNST_COVEROFDARKNESS: return gTextMgr[TRID_CTLCNST_COVEROFDARKNESS_DESC];
	};

	check("Unknown construction" == 0);
	return gTextMgr[TRID_VOID];
}

void iCtlCnst::Serialize(iDynamicBuffer& buff)
{

}

void iCtlCnst::Unserialize(iDynamicBuffer& buff)
{
	bInited = true;
}


//////////////////////////////////////////////////////////////////////////
iDwellCtlCnst::iDwellCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst, CREATURE_TYPE _crType)
: iCtlCnst(_pCastle, _cnst), crType(_crType), crCount(CREAT_DESC[_crType].growth)
{ }

iStringT iDwellCtlCnst::Desc() const
{
	return iFormat(gTextMgr[TRID_CTLCNST_DWELLINGS_DESC],Name().CStr(),gTextMgr[TRID_CREATURE_PEASANT_F1+crType*3+2]);
}

uint32 iDwellCtlCnst::Growth()
{
	uint32 res = CREAT_DESC[crType].growth;

	// try to find dwel modifiers
	for (uint32 xx=0; xx<pCastle->Cnsts().Count(); ++xx) {
		const iCtlCnst* pCnst = pCastle->Cnsts().At(xx);
		if (pCnst->IsBuilt() && CTLCNSTS_DESC[pCnst->Type()].type == CCT_DWELL_ENC) {
			iDwellEncCtlCnst* pDwellEnc = (iDwellEncCtlCnst*)pCnst;
			if (pDwellEnc->DwellingType() == Type()) res += pDwellEnc->GrowthMod();
		}
	}

	return res;
}

void iDwellCtlCnst::OnBuild()
{
	crCount = Growth();
}

void iDwellCtlCnst::NewWeek(const iWeekDesc& weekDesk)
{
	if (weekDesk.wkType == iWeekDesc::Normal) {
		crCount += Growth();
	} else if (weekDesk.wkType == iWeekDesc::Plague) {
		crCount /= 2;
	} else if (weekDesk.wkType == iWeekDesc::CreatPlus) {
		crCount += Growth();
		if (weekDesk.crType == crType) crCount += 5;
	} else if (weekDesk.wkType == iWeekDesc::CreatDoubles) {
		crCount += Growth();
		if (weekDesk.crType == crType) crCount *= 2;
	}
}

void iDwellCtlCnst::Serialize(iDynamicBuffer& buff)
{
	iCtlCnst::Serialize(buff);
	// Creatures quantity
	buff.Write(crCount);
}

void iDwellCtlCnst::Unserialize(iDynamicBuffer& buff)
{
	iCtlCnst::Unserialize(buff);
	// Creatures quantity
	buff.Read(crCount);
}


//////////////////////////////////////////////////////////////////////////
iTavernCtlCnst::iTavernCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst)
: iCtlCnst(_pCastle, _cnst), m_bRecruited(false)
{
	check(_cnst == CTLCNST_TAVERN);
}

void iTavernCtlCnst::OnInit()
{
	m_curRumor = gGame.Map().Rand(39);
}

void iTavernCtlCnst::NewWeek(const iWeekDesc& weekDesk)
{
	m_bRecruited = false;
	m_curRumor = gGame.Map().Rand(39);
}

void iTavernCtlCnst::Serialize(iDynamicBuffer& buff)
{
	iCtlCnst::Serialize(buff);
	buff.Write((uint16)m_curRumor);
	buff.Write((uint8)m_bRecruited);
}

void iTavernCtlCnst::Unserialize(iDynamicBuffer& buff)
{
	iCtlCnst::Unserialize(buff);
	uint16 curRumor;
	uint8 bRecruited;
	buff.Read(curRumor);
	buff.Read(bRecruited);
	m_curRumor = curRumor;
	m_bRecruited = bRecruited > 0;
}

void iTavernCtlCnst::OnVisitorEnter(iHero* pHero)
{
	pHero->VarFurtSkills().Value(FSK_MORALE) += 1;
}

void iTavernCtlCnst::OnVisitorLeave(iHero* pHero)
{
	pHero->VarFurtSkills().Value(FSK_MORALE) -= 1;
}

const iHero* iTavernCtlCnst::Visitor() const
{
	if (m_bRecruited) return NULL;
	iPlayer* pOwner = gGame.Map().FindPlayer(pCastle->Owner());
	check(pOwner);
	return pOwner->TavernVisitor();
}

iHero* iTavernCtlCnst::RecruitVisitor()
{
	check(CanRecruit());
	iPlayer* pOwner = gGame.Map().FindPlayer(pCastle->Owner());
	check(pOwner);
	m_bRecruited = true;
	return pOwner->RecruitTavernVisitor(pCastle);
}

bool iTavernCtlCnst::CanRecruit() const
{
	iPlayer* pOwner = gGame.Map().FindPlayer(pCastle->Owner());
	uint32 max;
	if(gGame.Map().GameWorldSize() == MSIZ_SMALL)
		max = 3;
	else if(gGame.Map().GameWorldSize() == MSIZ_MEDIUM)
		max = 4;
	else if(gGame.Map().GameWorldSize() == MSIZ_LARGE)
		max = 6;
	else if(gGame.Map().GameWorldSize() == MSIZ_EXTRALARGE)
		max = 8;

	return pOwner->TavernVisitor() && !m_bRecruited && !pCastle->Visitor() && pOwner->Minerals().Has(pOwner->TavernVisitor()->Proto()->m_Cost) && pOwner->HeroCount() < max;
}
//////////////////////////////////////////////////////////////////////////
iMagicNodeCtlCnst::iMagicNodeCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst)
: iCtlCnst(_pCastle, _cnst)
{
	check(_cnst == CTLCNST_MAGICNODE);
}

void iMagicNodeCtlCnst::OnBuild()
{
	if (pCastle->Owner() != PID_NEUTRAL) {
		iPlayer* pPlayer = gGame.Map().FindPlayer(pCastle->Owner());
		pPlayer->FurtSkills().Value(FSK_MANAPTS) += 1;
	}
}

void iMagicNodeCtlCnst::OnOwnerChanged(PLAYER_ID newOwner, PLAYER_ID oldOwner)
{
	if (oldOwner != PID_NEUTRAL) {
		iPlayer* pPlayer = gGame.Map().FindPlayer(oldOwner);
		pPlayer->FurtSkills().Value(FSK_MANAPTS) -= 1;
	}

	if (newOwner != PID_NEUTRAL) {
		iPlayer* pPlayer = gGame.Map().FindPlayer(newOwner);
		pPlayer->FurtSkills().Value(FSK_MANAPTS) += 1;
	}
}

//////////////////////////////////////////////////////////////////////////
iMineCtlCnst::iMineCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst, MINERAL_TYPE mt, sint32 income)
: iCtlCnst(_pCastle, _cnst), m_mt(mt), m_income(income)
{
}

void iMineCtlCnst::OnBuild()
{
	if (pCastle->Owner() != PID_NEUTRAL) {
		iPlayer* pPlayer = gGame.Map().FindPlayer(pCastle->Owner());
		pPlayer->FurtSkills().Value((FURTHER_SKILLS)(FSK_MIN_GOLD+m_mt)) += m_income;
	}
}

void iMineCtlCnst::OnOwnerChanged(PLAYER_ID newOwner, PLAYER_ID oldOwner)
{
	if (oldOwner != PID_NEUTRAL) {
		iPlayer* pPlayer = gGame.Map().FindPlayer(oldOwner);
		pPlayer->FurtSkills().Value((FURTHER_SKILLS)(FSK_MIN_GOLD+m_mt)) -= m_income;
	}

	if (newOwner != PID_NEUTRAL) {
		iPlayer* pPlayer = gGame.Map().FindPlayer(newOwner);
		pPlayer->FurtSkills().Value((FURTHER_SKILLS)(FSK_MIN_GOLD+m_mt)) += m_income;
	}
}

//////////////////////////////////////////////////////////////////////////
iMGuildCtlCnst::iMGuildCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst, uint8 _level)
: iCtlCnst(_pCastle, _cnst), level(_level)
{
}

iStringT iMGuildCtlCnst::Desc() const
{
	return gTextMgr[TRID_CTLCNST_COMMON_MGUILD_DESC];
}

void iMGuildCtlCnst::OnInit()
{
	InitSpells();
}

void iMGuildCtlCnst::NewDay()
{
	if (pCastle->Visitor() && pCastle->Visitor()->ManaPts() < pCastle->Visitor()->MaxManaPts()) pCastle->Visitor()->ManaPts() = pCastle->Visitor()->MaxManaPts();
}

void iMGuildCtlCnst::Serialize(iDynamicBuffer& buff)
{
	iCtlCnst::Serialize(buff);
	// Spells
	buff.Write((uint16)spells.GetSize());
	for (uint32 xx=0; xx<spells.GetSize(); ++xx) buff.Write(spells[xx]);
	// Visitors
	buff.Write((uint16)visitors.GetSize());
	for (uint32 xx=0; xx<visitors.GetSize(); ++xx) buff.Write(visitors[xx]);
}

void iMGuildCtlCnst::Unserialize(iDynamicBuffer& buff)
{
	iCtlCnst::Unserialize(buff);
	// Spells
	uint16 spcnt; buff.Read(spcnt);
	spells.RemoveAll();
	while (spcnt--) { uint16 spell; buff.Read(spell); spells.Add(spell); }
	// Visitors
	uint16 viscnt; buff.Read(viscnt);
	visitors.RemoveAll();
	while (viscnt--) { uint16 visitor; buff.Read(visitor); visitors.Add(visitor); }
}

void iMGuildCtlCnst::OnVisitorEnter(iHero* pHero)
{
	check(pHero);
	for (uint32 xx=0; xx<spells.GetSize(); ++xx) if (pHero->CanLearnSpell(spells[xx])) pHero->LearnSpell(spells[xx]);
	visitors.AddUnique(pHero->Proto()->m_protoId);
}

bool iMGuildCtlCnst::Visited(const iHero* pHero) const
{
	return (visitors.Find(pHero->Proto()->m_protoId) != -1);
}

void iMGuildCtlCnst::InitSpells()
{
	spells.RemoveAll();
	CTL_TYPE ctype = pCastle->Proto()->Type();
	uint32 count = CTL_MAGE_GUILD_SPELLS[ctype][level-1];

	MAGIC_SPELL reqSpell = MSP_INVALID;

	// Process exceptions
	if (ctype == CTLT_NECROPOLIS && level == 2) reqSpell = MSP_DEATHRIPPLE;
	else if (ctype == CTLT_FORTRESS && level == 2) reqSpell = MSP_SUMMONSPRITES; 

	SelectSpells(spells, CTL_MAGE_GUILD_FILTER[ctype][level-1], count, reqSpell);
}

void iMGuildCtlCnst::RefreshSpells()
{
	InitSpells();
	visitors.RemoveAll();
	if (pCastle->Visitor()) OnVisitorEnter(pCastle->Visitor());
}

//////////////////////////////////////////////////////////////////////////
iPermFSKModCtlCnst::iPermFSKModCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst, FURTHER_SKILLS _skill, sint32 _mod)
: iCtlCnst(_pCastle, _cnst), skill(_skill), mod(_mod)
{
}

void iPermFSKModCtlCnst::Serialize(iDynamicBuffer& buff)
{
	iCtlCnst::Serialize(buff);
	// Visitors
	buff.Write((uint16)visitors.GetSize());
	for (uint32 xx=0; xx<visitors.GetSize(); ++xx) buff.Write(visitors[xx]);
}

void iPermFSKModCtlCnst::Unserialize(iDynamicBuffer& buff)
{
	iCtlCnst::Unserialize(buff);
	// Visitors
	uint16 viscnt; buff.Read(viscnt);
	visitors.RemoveAll();
	while (viscnt--) { uint16 visitor; buff.Read(visitor); visitors.Add(visitor); }
}

void iPermFSKModCtlCnst::OnVisitorEnter(iHero* pHero)
{
	check(pHero);
	if (!Visited(pHero)) {
		pHero->ConstFurtSkills().Value(skill) += mod;
		visitors.Add(pHero->Proto()->m_protoId);
	}
}

bool iPermFSKModCtlCnst::Visited(const iHero* pHero) const
{
	return (visitors.Find(pHero->Proto()->m_protoId) != -1);
}
//////////////////////////////////////////////////////////////////////////
iDwellEncCtlCnst::iDwellEncCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst, CTL_CONSTR _dwel, sint32 _mod)
: iCtlCnst(_pCastle, _cnst), dwel(_dwel), mod(_mod)
{
	sint32 aa=0;
}

iStringT iDwellEncCtlCnst::Desc() const
{
	return iFormat(gTextMgr[TRID_CTLCNST_DWELL_ENC_DESC], Name().CStr(), gTextMgr[TRID_CREATURE_PEASANT_F3 + CTLCNSTS_DESC[dwel].fparam * 3], mod);
}

//////////////////////////////////////////////////////////////////////////
iObsPostCtlCnst::iObsPostCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst)
: iCtlCnst(_pCastle, _cnst)
{}

void iObsPostCtlCnst::OnBuild()
{
	pCastle->ScoutMod() += 4;
	if (pCastle->Owner() != PID_NEUTRAL) {
		iPlayer* pPlayer = gGame.Map().FindPlayer(pCastle->Owner());
		pPlayer->FurtSkills().Value(FSK_ACTPTS) += 5;
		pPlayer->UpdateFogMap();
	}
}

void iObsPostCtlCnst::OnOwnerChanged(PLAYER_ID newOwner, PLAYER_ID oldOwner)
{
	if (oldOwner != PID_NEUTRAL) {
		iPlayer* pPlayer = gGame.Map().FindPlayer(oldOwner);
		pPlayer->FurtSkills().Value(FSK_ACTPTS) -= 5;
	}

	if (newOwner != PID_NEUTRAL) {
		iPlayer* pPlayer = gGame.Map().FindPlayer(newOwner);
		pPlayer->FurtSkills().Value(FSK_ACTPTS) += 5;
	}
}
//////////////////////////////////////////////////////////////////////////
iLibraryCtlCnst::iLibraryCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst)
: iCtlCnst(_pCastle, _cnst)
{}

void iLibraryCtlCnst::NewWeek(const iWeekDesc& weekDesk)
{
	if (weekDesk.bNewMonth) {
		// prcoess all mage guilds
		for (uint32 xx=0; xx<pCastle->Cnsts().Count(); ++xx) {
			const iCtlCnst* pCnst = pCastle->Cnsts().At(xx);
			if (pCnst->IsBuilt() && CTLCNSTS_DESC[pCnst->Type()].type == CCT_MAGEGUILD) {
				iMGuildCtlCnst* pMGuild = (iMGuildCtlCnst*)pCnst;
				pMGuild->RefreshSpells();
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
iManaVtxCtlCnst::iManaVtxCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst)
: iCtlCnst(_pCastle, _cnst), m_bVisited(false)
{}

void iManaVtxCtlCnst::NewWeek(const iWeekDesc& weekDesk)
{
	m_bVisited = false;
	if (pCastle->Visitor()) OnVisitorEnter(pCastle->Visitor());
}

void iManaVtxCtlCnst::Serialize(iDynamicBuffer& buff)
{
	iCtlCnst::Serialize(buff);
	// Visitors
	buff.Write((uint8)m_bVisited);
}

void iManaVtxCtlCnst::Unserialize(iDynamicBuffer& buff)
{
	iCtlCnst::Unserialize(buff);
	// Visitors
	uint8 bVisited;
	buff.Read(bVisited);
	m_bVisited = (bVisited==0)?false:true;
}

void iManaVtxCtlCnst::OnVisitorEnter(iHero* pHero)
{
	check(pHero);
	if (!Visited(pHero)) {
		DoubleManaPoints(pHero);
	}
}

bool iManaVtxCtlCnst::Visited(const iHero* pHero) const
{
	return m_bVisited;
}

void iManaVtxCtlCnst::DoubleManaPoints(iHero* pHero)
{
	check(!m_bVisited);
	sint32 act_pts = pHero->MaxManaPts() * 2;
	if (pHero->ManaPts() < act_pts) {
		pHero->ManaPts() = act_pts;
		m_bVisited = true;
	}
}

//////////////////////////////////////////////////////////////////////////
iTreasuryCtlCnst::iTreasuryCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst)
: iCtlCnst(_pCastle, _cnst)
{}

void iTreasuryCtlCnst::NewWeek(const iWeekDesc& weekDesk)
{
	if (pCastle->Owner() != PID_NEUTRAL) {
		iPlayer* pPlayer = gGame.Map().FindPlayer(pCastle->Owner());
		pPlayer->Minerals().quant[MINERAL_GOLD] += 500;
	}
}
//////////////////////////////////////////////////////////////////////////
iMystPoundCtlCnst::iMystPoundCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst)
: iCtlCnst(_pCastle, _cnst), m_mt(MINERAL_UNKNOWN), m_cnt(0)
{}

void iMystPoundCtlCnst::NewWeek(const iWeekDesc& weekDesk)
{
	if (pCastle->Owner() != PID_NEUTRAL) {
		iPlayer* pPlayer = gGame.Map().FindPlayer(pCastle->Owner());
		m_mt = (MINERAL_TYPE)(MINERAL_MERCURY+gGame.Map().Rand(4));
		m_cnt = 2 + gGame.Map().Rand(4);
		pPlayer->Minerals().quant[m_mt] += m_cnt;
	}
}

void iMystPoundCtlCnst::Serialize(iDynamicBuffer& buff)
{
	iCtlCnst::Serialize(buff);
	// Last week production
	buff.Write((sint8)m_mt);
	buff.Write((sint8)m_cnt);
}

void iMystPoundCtlCnst::Unserialize(iDynamicBuffer& buff)
{
	iCtlCnst::Unserialize(buff);
	// Last week production

	sint8 mt, cnt;
	buff.Read(mt);
	buff.Read(cnt);
	m_mt = (MINERAL_TYPE)mt;
	m_cnt = cnt;
}

bool iMystPoundCtlCnst::GetActivityText(iStringT& text) const
{
	check(state == Built);
	if (m_mt == MINERAL_UNKNOWN) {
		text = gTextMgr[TRID_CTLCNST_MYSTICPOUND_NPROD];
	} else {
		text.Setf(gTextMgr[TRID_CTLCNST_MYSTICPOUND_PROD], Mineral2Text(m_mt, m_cnt).CStr());
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
iNecrAmpCtlCnst::iNecrAmpCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst)
: iCtlCnst(_pCastle, _cnst)
{}

void iNecrAmpCtlCnst::OnBuild()
{
	if (pCastle->Owner() != PID_NEUTRAL) {
		iPlayer* pPlayer = gGame.Map().FindPlayer(pCastle->Owner());
		pPlayer->FurtSkills().Value(FSK_NECROMANCY) += 10;
	}
}

void iNecrAmpCtlCnst::OnOwnerChanged(PLAYER_ID newOwner, PLAYER_ID oldOwner)
{
	if (oldOwner != PID_NEUTRAL) {
		iPlayer* pPlayer = gGame.Map().FindPlayer(oldOwner);
		pPlayer->FurtSkills().Value(FSK_NECROMANCY) -= 10;
	}

	if (newOwner != PID_NEUTRAL) {
		iPlayer* pPlayer = gGame.Map().FindPlayer(newOwner);
		pPlayer->FurtSkills().Value(FSK_NECROMANCY) += 10;
	}
}
//////////////////////////////////////////////////////////////////////////
iCovOfDarkCtlCnst::iCovOfDarkCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst)
: iCtlCnst(_pCastle, _cnst)
{
}

void iCovOfDarkCtlCnst::OnBuild()
{
	HideCastle();
}

void iCovOfDarkCtlCnst::NewDay()
{
	HideCastle();
}

void iCovOfDarkCtlCnst::HideCastle()
{
	// hide the castle
	for (iGameWorld::iPLIt pit = gGame.Map().m_Players.First(); pit != gGame.Map().m_Players.End(); ++pit) {
		if ( (*pit)->PlayerId() != pCastle->Owner() ){
			(*pit)->FogMap().DiscoverMap(pCastle->Pos().x, pCastle->Pos().y, pCastle->Scouting(), true);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void iCtlCnsts::Init()
{
	for (uint32 xx=0; xx<m_items.GetSize(); ++xx) if (m_items[xx]->IsBuilt() && !m_items[xx]->IsInited()) m_items[xx]->Init();
}

void iCtlCnsts::NewDay()
{
	for (uint32 xx=0; xx<m_items.GetSize(); ++xx) if (m_items[xx]->IsBuilt()) m_items[xx]->NewDay();
}

void iCtlCnsts::NewWeek(const iWeekDesc& weekDesk)
{
	for (uint32 xx=0; xx<m_items.GetSize(); ++xx) if (m_items[xx]->IsBuilt()) m_items[xx]->NewWeek(weekDesk);
}

void iCtlCnsts::OnVisitorEnter(iHero* pHero)
{
	for (uint32 xx=0; xx<m_items.GetSize(); ++xx) if (m_items[xx]->IsBuilt()) m_items[xx]->OnVisitorEnter(pHero);
}

void iCtlCnsts::OnVisitorLeave(iHero* pHero)
{
	for (uint32 xx=0; xx<m_items.GetSize(); ++xx) if (m_items[xx]->IsBuilt()) m_items[xx]->OnVisitorLeave(pHero);
}

void iCtlCnsts::OnOwnerChanged(PLAYER_ID newOwner, PLAYER_ID oldOwner)
{
	for (uint32 xx=0; xx<m_items.GetSize(); ++xx) {
		if (m_items[xx]->IsBuilt()) {
			m_items[xx]->OnOwnerChanged(newOwner, oldOwner);
		}
	}
}

void iCtlCnsts::Serialize(iDynamicBuffer& buff)
{
	buff.Write((uint8)2); // constructions initialization flag (0 - default, 1 - customized, 2 - customized with serialized data)
	uint16 cnt=0;
	for (uint32 xx=0; xx<m_items.GetSize(); ++xx) if (m_items[xx]->IsBuilt()) cnt++;
	buff.Write(cnt);
	for (uint32 xx=0; xx<m_items.GetSize(); ++xx) {
		if (m_items[xx]->IsBuilt()) {
			buff.Write((uint16)xx);
			buff.Write((uint8)1); // Built
			m_items[xx]->Serialize(buff);
		}
	}
}

void iCtlCnsts::Unserialize(iDynamicBuffer& buff)
{
	uint8 flag; buff.Read(flag);
	if (flag == 0) {
		// init default constructions
		GetCnst(DEFAULT_CONSTRUCTIONS[m_pCastle->Proto()->Type()][0])->SetState(iCtlCnst::Built);
		GetCnst(DEFAULT_CONSTRUCTIONS[m_pCastle->Proto()->Type()][1])->SetState(iCtlCnst::Built);
	} else if (flag == 1 || flag == 2) {
		uint16 cnt; buff.Read(cnt);
		while (cnt--){
			uint16 idx; buff.Read(idx);
			uint8 state; buff.Read(state);
			m_items[idx]->SetState(iCtlCnst::Built);
			if (flag == 2) m_items[idx]->Unserialize(buff);
		}
	} else {
		check(0);
	}
}

void iCtlCnsts::DependsList(const iCtlCnst* pCnst, iSimpleArray<uint16>& depList) const
{
	iDepItem dep = pCnst->Depends();
	// check first
	for (uint32 xx=0; xx<2; ++xx) {
		if (dep.dep[xx] != CTLCNST_INVALID && depList.Find(dep.dep[xx]) == -1) {
			CTL_CONSTR ctlc = dep.dep[xx];
			const iCtlCnst* pDepCnst = GetCnst(ctlc);
			check (pDepCnst);
			if (!pDepCnst->IsBuilt()){
				depList.Add(dep.dep[xx]);
			}
			DependsList(pDepCnst, depList);
		}
	}
}

void iCtlCnsts::UpdateState(bool bCanBuild)
{
	iPlayer* pOwner = 0;
	if (m_pCastle->Owner() != PID_NEUTRAL) pOwner = gGame.Map().FindPlayer(m_pCastle->Owner());
	if (!pOwner) return;
	for (uint32 xx=0; xx<m_items.GetSize(); ++xx){
		if (m_items[xx]->GetState() != iCtlCnst::Built) {
			if (!bCanBuild) {
				m_items[xx]->SetState(iCtlCnst::Unavailable);
			} else if ( !m_items[xx]->Depends().Empty() )	{
				iSimpleArray<uint16> depList;
				DependsList(m_items[xx],depList);
				if (!depList.GetSize()) {
					if (pOwner->Minerals().Has(m_items[xx]->Price())) m_items[xx]->SetState(iCtlCnst::Available);
					else m_items[xx]->SetState(iCtlCnst::NotEnoughRes);
				} else {
					m_items[xx]->SetState(iCtlCnst::Unavailable);
				}
			} else {
				if (pOwner && pOwner->Minerals().Has(m_items[xx]->Price())) m_items[xx]->SetState(iCtlCnst::Available);
				else m_items[xx]->SetState(iCtlCnst::NotEnoughRes);
			}
		}
	}
}

