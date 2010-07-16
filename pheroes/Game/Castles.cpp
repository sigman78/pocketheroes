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
iCastle::iCastle(iCastleT* pProto, const iPoint& pos, const iStringT& cname, bool bCreateHero)
: iMapCnst(pos), m_pProto(pProto), m_Owner(PID_NEUTRAL), m_bCanBuild(true), m_Cnsts(this), m_bCreateHero(bCreateHero), m_scoutMod(0)
{
	// Castle's name
	if (!cname.Empty()) m_Name = cname;
	else m_Name = gTextMgr[gGame.ItemMgr().m_CastleRepos.GetCastleName(m_pProto->Type())];

	// Init constructions
	for (uint32 xx=0; xx<CTLCNST_COUNT; ++xx){
		if (CTLCNSTS_DESC[xx].caps.Support(m_pProto->Type(),m_pProto->Size())) {
			iCtlCnst* pCtlCnst = NULL;
			CTL_CONSTR cnstId = (CTL_CONSTR)xx;
			switch (CTLCNSTS_DESC[xx].type) {
			case CCT_GENERIC: pCtlCnst = new iCtlCnst(this, cnstId); break;
			case CCT_DWELLING: pCtlCnst = new iDwellCtlCnst(this, cnstId, (CREATURE_TYPE)CTLCNSTS_DESC[cnstId].fparam); break;
			case CCT_MAGEGUILD: pCtlCnst = new iMGuildCtlCnst(this, cnstId,CTLCNSTS_DESC[cnstId].fparam); break;
			case CCT_TAVERN: pCtlCnst = pCtlCnst = new iTavernCtlCnst(this, cnstId); break;
			case CCT_MAGICNODE: pCtlCnst = new iMagicNodeCtlCnst(this, cnstId); break;
			case CCT_MINE: pCtlCnst = new iMineCtlCnst(this, cnstId, (MINERAL_TYPE)CTLCNSTS_DESC[cnstId].fparam, CTLCNSTS_DESC[cnstId].sparam); break;
			case CCT_PERM_FSK_MOD: pCtlCnst = new iPermFSKModCtlCnst(this, cnstId, (FURTHER_SKILLS)CTLCNSTS_DESC[cnstId].fparam, CTLCNSTS_DESC[cnstId].sparam); break;
			case CCT_DWELL_ENC: pCtlCnst = new iDwellEncCtlCnst(this, cnstId, (CTL_CONSTR)CTLCNSTS_DESC[cnstId].fparam, CTLCNSTS_DESC[cnstId].sparam); break;
			case CCT_OBSERVPOST: pCtlCnst = new iObsPostCtlCnst(this, cnstId); break;
			case CCT_LIBRARY: pCtlCnst = new iLibraryCtlCnst(this, cnstId); break;
			case CCT_MANAVORTEX: pCtlCnst = new iManaVtxCtlCnst(this, cnstId); break;
			case CCT_TREASURY: pCtlCnst = new iTreasuryCtlCnst(this, cnstId); break;
			case CCT_MYSTICPOUND: pCtlCnst = new iMystPoundCtlCnst(this, cnstId); break;
			case CCT_NECRAMPLIFIER: pCtlCnst = new iNecrAmpCtlCnst(this, cnstId); break;
			case CCT_COVEROFDARKNESS: pCtlCnst = new iCovOfDarkCtlCnst(this, cnstId); break;
			default: check("Unknown construction type" == 0);
			};
			m_Cnsts.Add(pCtlCnst);
		}
	}
	
	UpdateCnsts();
}

void iCastle::Init()
{
	// Init garrison
	if (m_Owner == PID_NEUTRAL && m_Garrison.Empty()) {
		GenerateGuards();
	}

	// Pre build constructions for debug
	//if (iCtlCnst* pMoat = m_Cnsts.GetCnst(CTLCNST_MOAT)) pMoat->Build();
	//if (iCtlCnst* pLTurret = m_Cnsts.GetCnst(CTLCNST_LTURRET)) pLTurret->Build();
	//if (iCtlCnst* pMTurret = m_Cnsts.GetCnst(CTLCNST_MTURRET)) pMTurret->Build();
	m_Cnsts.Init();
	UpdateCnsts();

	InitArmy(m_Garrison);

	// Create Hero
	if (m_bCreateHero) {
		uint16 selhid = gGame.ItemMgr().HeroesMgr().Select(CTL_HEROES[m_pProto->Type()]);
		if (selhid == iHeroesMgr::INVALID_HERO_ID) selhid = gGame.ItemMgr().HeroesMgr().Select(HTM_ALL);
		if (selhid != iHeroesMgr::INVALID_HERO_ID) {
			iHero* pHero = gGame.ItemMgr().HeroesMgr().Get(selhid);
			iPlayer* pOwner = gGame.Map().FindPlayer(m_Owner);
			check(pHero && pOwner);
			gGame.Map().AddHero(pHero, pOwner, m_Pos, HERO_ORIENT[m_pProto->Orient()]);
		} else {
			check(0);
		}
	}
}

void iCastle::SetOwner(PLAYER_ID newOwner)
{
	if (newOwner == m_Owner) return;

	PLAYER_ID oldOwner = m_Owner;
	m_Owner = newOwner;

	if (oldOwner != PID_NEUTRAL) gGame.Map().FindPlayer(oldOwner)->FurtSkills().Value(FSK_MIN_GOLD) -= CTL_INCOME;
	if (newOwner != PID_NEUTRAL) gGame.Map().FindPlayer(newOwner)->FurtSkills().Value(FSK_MIN_GOLD) += CTL_INCOME;
	m_Cnsts.OnOwnerChanged(newOwner, oldOwner);
}

void iCastle::OnSelect()
{
	gGame.OnCastleChanged(this);
}

void iCastle::OnVisitorEnter(iHero* pHero)
{
	m_Cnsts.OnVisitorEnter(pHero);
}

void iCastle::OnVisitorLeave(iHero* pHero)
{
	m_Cnsts.OnVisitorLeave(pHero);
}

void iCastle::NewDay()
{
	m_bCanBuild = true;
	m_Cnsts.NewDay();
	m_Cnsts.UpdateState(m_bCanBuild);
}

void iCastle::NewWeek(const iWeekDesc& weekDesk)
{
	m_Cnsts.NewWeek(weekDesk);
	if (m_Owner == PID_NEUTRAL) {
		GenerateGuards();
	}
}

uint32 iCastle::GetConstrCaps()
{
	return 0;
}

iTavernCtlCnst* iCastle::GetTavern(bool bOnlyBuilt)
{
	if (iTavernCtlCnst* pTavern = DynamicCast<iTavernCtlCnst*>(m_Cnsts.GetCnst(CTLCNST_TAVERN))) {
		if (!bOnlyBuilt || pTavern->IsBuilt()) return pTavern;
	}
	return NULL;
}

uint32 iCastle::GarrisonPower(bool bOnlyFort) const
{
	uint32 result = 0;
	
	if (!bOnlyFort){
		result += Garrison().ArmyPower();
		if (!result) return 0;
	}
	const iCtlCnst* pCnst;
	if (pCnst = m_Cnsts.GetCnst(CTLCNST_LTURRET)) if (pCnst->IsBuilt()) result += 1000;
	if (pCnst = m_Cnsts.GetCnst(CTLCNST_RTURRET)) if (pCnst->IsBuilt()) result += 1000;
	if (pCnst = m_Cnsts.GetCnst(CTLCNST_MTURRET)) if (pCnst->IsBuilt()) result += 1000;
	if (pCnst = m_Cnsts.GetCnst(CTLCNST_MOAT)) if (pCnst->IsBuilt()) result += 1000;
	return result;
}

sint32 iCastle::Scouting() const
{
	return m_pProto->Scouting() + m_scoutMod;
}

void iCastle::Build(iCtlCnst* pCnst)
{
	check(m_bCanBuild);
	gGame.Map().FindPlayer(Owner())->Minerals() -= pCnst->Price();
	pCnst->Build();
	m_bCanBuild = false;
	UpdateCnsts();
}

void iCastle::Build(uint32 cnstIdx)
{
	Build(m_Cnsts[cnstIdx]);
}

void iCastle::UpdateCnsts()
{
	m_Cnsts.UpdateState(m_bCanBuild);
}

void iCastle::GenerateGuards()
{
	for (uint32 xx=0; xx<3; ++xx) {
		if ( gGame.Map().Rand(3) == 0 ) {
			CREATURE_TYPE ct = (CREATURE_TYPE)(CREAT_PEASANT + m_pProto->Type()*6 + xx);
			m_Garrison.AddGroup(ct, CREAT_DESC[ct].growth);
		}
	}
}

