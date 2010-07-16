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
#include "Dlg_Level.h"
#include "ArtifactList.h"
#include "Dlg_CapArt.h"
#include "Dlg_RiseSkeletons.h"
#include "Dlg_ArmyRoom.h"
#include "serialize.h"
#include "helpers.h"

/*
 *	Secondary skills container
 */
iSecSkillsCtr::iSecSkillsCtr(iFurtSkills& furtSkills) 
: m_furtSkills(furtSkills)
{}

SECSKILL_LEVEL iSecSkillsCtr::SkillLevel(SECONDARY_SKILLS secSkill) const
{
	for (uint32 xx=0; xx<m_secSkills.GetSize(); ++xx) if (m_secSkills[xx].m_skill == secSkill) return (SECSKILL_LEVEL)m_secSkills[xx].m_level;
	return SSLVL_NONE;
}

void iSecSkillsCtr::Inc(SECONDARY_SKILLS secSkill)
{
	for (uint32 xx=0; xx<m_secSkills.GetSize(); ++xx){
		if (m_secSkills[xx].m_skill == secSkill){
			check(m_secSkills[xx].m_level < SSLVL_EXPERT && m_secSkills[xx].m_level > SSLVL_NONE);
			m_furtSkills.Value((FURTHER_SKILLS)SEKSKILL_DESC[m_secSkills[xx].m_skill][m_secSkills[xx].m_level][0]) -= SEKSKILL_DESC[m_secSkills[xx].m_skill][m_secSkills[xx].m_level][1];
			m_secSkills[xx].m_level++;
			m_furtSkills.Value((FURTHER_SKILLS)SEKSKILL_DESC[m_secSkills[xx].m_skill][m_secSkills[xx].m_level][0]) += SEKSKILL_DESC[m_secSkills[xx].m_skill][m_secSkills[xx].m_level][1];
			return;
		}
	}

	Set(secSkill,SSLVL_BASIC);
}

void iSecSkillsCtr::Set(SECONDARY_SKILLS secSkill, SECSKILL_LEVEL lvl)
{
	check(SkillLevel(secSkill) == SSLVL_NONE && m_secSkills.GetSize() < 8);
	m_secSkills.Add(iSecSkillEntry(secSkill,lvl));
	m_furtSkills.Value((FURTHER_SKILLS)SEKSKILL_DESC[secSkill][lvl][0]) += SEKSKILL_DESC[secSkill][lvl][1];
}

void iSecSkillsCtr::Set(const iSecSkills& secSkills)
{
	check(!m_secSkills.GetSize());
	for (uint32 xx=0; xx<secSkills.GetSize(); ++xx) 
		Set((SECONDARY_SKILLS)secSkills[xx].m_skill, (SECSKILL_LEVEL)secSkills[xx].m_level);
}

void iSecSkillsCtr::Reset()
{
	for (uint32 xx=0; xx<m_secSkills.GetSize(); ++xx) {
		m_furtSkills.Value((FURTHER_SKILLS)SEKSKILL_DESC[m_secSkills[xx].m_skill][m_secSkills[xx].m_level][0]) -= SEKSKILL_DESC[m_secSkills[xx].m_skill][m_secSkills[xx].m_level][1];
	}
	m_secSkills.RemoveAll();
}

/*
 *	Hero
 */
iHero::iHero(iHeroT* pProto, sint32 fake)
: iBaseMapObject(cInvalidPoint, true)
, m_pProto(pProto), m_pOwner(NULL), m_Path(cInvalidPoint), m_bStop(false), m_bMoving(false)
, m_pLocation(NULL), m_Artifacts(this), m_bDead(true), m_Enchs(m_varFurtSkills), m_SecSkills(m_varFurtSkills)
, m_Experience(InitialExpPts()), m_Level(1), m_spellSetMode(SSM_ALL), m_bCanDig(true)
{
	// Init FurtSkills
	m_constFurtSkills = pProto->m_furtSkills;
	// Init secondary skills
	m_SecSkills.Set(pProto->m_secSkills);
	// Init Army
	ResetArmy();
	InitArmy(m_Army);
	// Init Spellbook
	m_spellBook.Add(pProto->m_spells);
	// Mana points
	m_manaPts = MaxManaPts();
	// Action points
	m_Moves = TotalMoves();
	// Angle
	m_Angle = 0;
}

// Serialize / Deserialize hero object
void iHero::Serialize(iDynamicBuffer& dbuff) const
{
	// Proto Id
	dbuff.Write((uint16)m_pProto->m_protoId);
	// Owner Id
	if (m_pOwner) dbuff.Write((sint8)m_pOwner->PlayerId());
	else dbuff.Write((sint8)PID_NEUTRAL);
	// Position
	::Serialize(dbuff, m_Pos);
	// Angle
	dbuff.Write((sint8)m_Angle);
	// Target
	if (m_Path.IsEmpty()) ::Serialize(dbuff, m_Pos);
	else ::Serialize(dbuff, m_Path.DestPoint());
	// Custom Name
	::Serialize(dbuff, m_CustomName);
	// Level
	dbuff.Write(m_Level);
	// Experience
	dbuff.Write((uint32)m_Experience);
	//Secondary skills
	::Serialize(dbuff, m_SecSkills.SecSkills());
	// Artifact list
	iArtifactList artList;
	for (uint32 artIdx=0; artIdx<m_Artifacts.BackPackCount(); ++artIdx) artList.Add(m_Artifacts.BackPackItem(artIdx), AC_UNDEFINED);
	for (uint32 artIdx=0; artIdx<AC_COUNT; ++artIdx) if (!m_Artifacts.DressedItem((HERO_ART_CELL)artIdx).Empty()) artList.Add(m_Artifacts.DressedItem((HERO_ART_CELL)artIdx).artId, (HERO_ART_CELL)artIdx);
	::Serialize(dbuff, artList);
	// Enchancement list
	dbuff.Write((uint16)m_Enchs.Count());
	for (uint32 xx=0; xx<m_Enchs.Count(); ++xx) {
		// provider
		int a1=m_Enchs[xx].provider;
		dbuff.Write((sint32)m_Enchs[xx].provider);
		// type (FURTHER_SKILLS)
		int a2=m_Enchs[xx].type;
		dbuff.Write((sint16)m_Enchs[xx].type);
		// duration
		dbuff.Write((uint16)m_Enchs[xx].dur);
		// mod
		dbuff.Write((sint32)m_Enchs[xx].mod);
	}
	// FurtSkills
	::Serialize(dbuff, m_constFurtSkills);
	// Army
	::Serialize(dbuff, m_Army);
	// Spellbook
	dbuff.Write((uint8)1);
	::Serialize(dbuff, m_spellBook.Spells());
	dbuff.Write((uint8)m_spellBook.FavoritesCount());
	for (uint32 xx=0; xx<m_spellBook.FavoritesCount(); ++xx) dbuff.Write(m_spellBook.Favorite(xx));
	// Mana points
	dbuff.Write(m_manaPts);
	// Action points
	dbuff.Write(m_Moves);
	// Can dig flag
	dbuff.Write((uint8)m_bCanDig);
}

void iHero::Unserialize(iDynamicBuffer& dbuff, bool bInit)
{
	//DbgDumpFurtSkills(Name() + _T(" (initial constant)"), m_constFurtSkills);
	//DbgDumpFurtSkills(Name() + _T(" (initial var)"), m_varFurtSkills);

	check(m_bDead);
	if (bInit) m_bDead = false;

	// Owner
	sint8 owner; dbuff.Read(owner);
	if (owner != PID_NEUTRAL) {
		m_pOwner = gGame.Map().FindPlayer((PLAYER_ID)owner);
		check(m_pOwner);
		m_pOwner->AddHero(this, false);
	}

	// Position
	iPoint pos; ::Unserialize(dbuff, pos); 
	SetPos(pos);
	// Angle
	sint8 angle; dbuff.Read(angle);
	if (angle < 0) m_Angle = gGame.Map().Rand(8); else m_Angle = (uint8)angle;
	// Target
	iPoint target; ::Unserialize(dbuff, target);
	if (target != pos) SetPath(target);
	// Custom name
	::Unserialize(dbuff, m_CustomName);
	// Level
	uint32 level; dbuff.Read(level);
	if (level) m_Level = level;
	// Experience
	uint32 exp; dbuff.Read(exp);
	if (exp) m_Experience = exp;
	// Secondary skills
	iSecSkills secSkills; 	
	::Unserialize(dbuff, secSkills);
	m_SecSkills.Reset();
	m_SecSkills.Set(secSkills);
	// Artifact list
	iArtifactList artList; ::Unserialize(dbuff, artList);
	for (uint32 artIdx=0; artIdx<artList.Count(); ++artIdx) {
		if (artList.At(artIdx).assign == AC_UNDEFINED) m_Artifacts.PushToBackPack(artList.At(artIdx).id);
		else m_Artifacts.PushDressed(artList.At(artIdx).id, (HERO_ART_CELL)artList.At(artIdx).assign);
	}
	// Enchancement list
	uint16 enCnt; dbuff.Read(enCnt);
	for (uint32 xx=0; xx<enCnt; ++xx) {
		// provider
		sint32 provider; dbuff.Read(provider);
		// type (FURTHER_SKILLS)
		sint16 type; dbuff.Read(type);
		// duration
		uint16 dur; dbuff.Read(dur);
		// mod
		sint32 mod; dbuff.Read(mod);
		// Add enchancement item
		m_Enchs.Add(provider, (FURTHER_SKILLS)type, (ENCH_DURATION)dur, mod);
	}
	// FurtSkills
	iFurtSkills furtSkills; ::Unserialize(dbuff, furtSkills);
	m_constFurtSkills = furtSkills;
	// Army
	iArmy army; ::Unserialize(dbuff, army);
	if (!army.Empty()) {
		m_Army = army;
		InitArmy(m_Army);
	}
	// Spellbook
	uint8 spflag; dbuff.Read(spflag);
	if (spflag != 0) {
	iSpellList spellList; ::Unserialize(dbuff, spellList);
		m_spellBook.Reset();
		if (spellList.GetSize()) m_spellBook.Add(spellList);
	}
	// Favorite spells
	uint8 fspcnt; dbuff.Read(fspcnt);
	while (fspcnt--) {
		uint16 spellId;
		dbuff.Read(spellId);
		m_spellBook.AddFavorite(spellId);
	}

	// Mana points
	sint32 mana;
	dbuff.Read(mana);
	if (mana >= 0) m_manaPts = mana;
	else m_manaPts = MaxManaPts();

	// Action points
	sint32 action; dbuff.Read(action);
	if (action >= 0) m_Moves = action;
	else m_Moves = TotalMoves();

	// Can dig flag
	uint8 bCanDig;
	dbuff.Read(bCanDig);
	m_bCanDig = (bCanDig==0)?false:true;

	//DbgDumpFurtSkills(Name() + _T(" (unserialized constant)"), m_constFurtSkills);
	//DbgDumpFurtSkills(Name() + _T(" (unserialized var)"), m_varFurtSkills);
}

void iHero::Deinit(bool bResetArmy)
{
	check(!m_bDead);
	m_bDead = true;
	m_pOwner = NULL;
	SetPos(cInvalidPoint);
	m_Path.SetEmpty(m_Pos);
	if (m_pLocation) SetLocation(NULL);
	if (bResetArmy) {
		ResetArmy();
		InitArmy(m_Army);
	}
}

void iHero::Init(iPlayer* pOwner, const iPoint& pos, sint8 angle)
{
	check(m_bDead && !m_pOwner && m_Pos == cInvalidPoint);
	m_pOwner = pOwner;
	m_spellSetMode = SSM_ALL;
	SetPos(pos);
	if (angle < 0) m_Angle = gGame.Map().Rand(8); else m_Angle = (uint8)angle;
	m_bDead = false;
}

void iHero::StartMoving()
{
	m_bMoving = true;
}

void iHero::StopMoving()
{
	if (m_bMoving) {
		m_bMoving = false;
		gGame.OnHeroStopMoving(this);
	}
}

bool iHero::Step()
{
	if (m_bDead || m_Path.IsEmpty() || m_Moves < m_Path.GetStepCost()) {
		StopMoving();
		return false;
	}

	bool bNeedAction = false;

	if (m_NextPoint == m_Path.GetStepPos()) {
		m_bCanDig = false;
		m_Moves -= m_Path.GetStepCost();
		iPath::iStep step = m_Path.GetStep();
		if (step.m_action == iPath::MoveTo || step.m_action == iPath::Enter) {
			iPoint oldPos = Pos();
			iPath::iStep nStep = m_Path.Step();
			SetPos(nStep.m_Pos);
			gGame.Map().MoveHero(oldPos,nStep.m_Pos);
			m_pOwner->UpdateFogMap();
			gGame.OnHeroPosChanged(this, nStep.m_Pos);

			if (!m_Path.IsEmpty()){
				// Check target
				if (m_Path.TargetAction() != iPath::MoveTo) {
					uint8 act = GetTarget((sint16)m_Path.DestPoint().x,(sint16)m_Path.DestPoint().y).action;
					if (act != m_Path.TargetAction()) {
						StopMoving();
						m_Path.SetEmpty(m_Pos);
						return false;
					}
				}
			} else if (step.m_action == iPath::Enter) {
				// process action (if end point)
				check(m_Path.IsEmpty() && m_pLocation);
				StopMoving();
				if (iCastle* pCastle = DynamicCast<iCastle*>(m_pLocation)) {
					if (pCastle->Owner() == m_pOwner->PlayerId()) EnterCastle(pCastle);
					else CaptureCastle(pCastle, true);
				} else ActivateConstruction(m_pLocation);
			}
		} else if (step.m_action == iPath::Touch || step.m_action == iPath::Attack || step.m_action == iPath::Meet){
			iPath::iStep nStep = m_Path.Step();
			//
			iBaseMapObject* pMapObj = gGame.Map().m_CoverMap.GetAt(nStep.m_Pos.x, nStep.m_Pos.y);
			check(pMapObj);

			if (iHero* pHero = DynamicCast<iHero*>(pMapObj)){
				if (step.m_action == iPath::Meet) {
					check(pHero->Owner() == Owner());
					StopMoving();
					bNeedAction = true;
					MeetHero(pHero);
				} else if (step.m_action == iPath::Attack){
					check(pHero->Owner() != Owner());
					if (iCastle* pCastle = DynamicCast<iCastle*>(pHero->GetLocation())){
						StopMoving();
						bNeedAction = true;
						SiegeCastle(pCastle);
					} else {
						StopMoving();
						bNeedAction = true;
						AttackHero(pHero);
					}
				} else {
					check(0);
				}
			} else if (iMapGuard* pGuard = DynamicCast<iMapGuard*>(pMapObj)) {
				check(step.m_action == iPath::Attack);
				StopMoving();
				bNeedAction = true;
				TouchMapGuard(pGuard, false);
			} else if (DynamicCast<iMapItem_Artifact*>(pMapObj) || DynamicCast<iMapItem_CampFire*>(pMapObj) || DynamicCast<iMapItem_Chest*>(pMapObj) || DynamicCast<iMapItem_Lamp*>(pMapObj) || DynamicCast<iMapItem_ManaCrystal*>(pMapObj) || DynamicCast<iMapItem_Mineral*>(pMapObj) || DynamicCast<iMapItem_SpellScroll*>(pMapObj) || DynamicCast<iMapItem_KeyGuard*>(pMapObj)) {
				check(step.m_action == iPath::Touch);
				StopMoving();
				bNeedAction = true;
				iMapItem* pMapItem = (iMapItem*)pMapObj;
				TouchMapItem(pMapItem, false);
			} else if (iCastle* pCastle = DynamicCast<iCastle*>(pMapObj)) {
				StopMoving();
				bNeedAction = true;
				SiegeCastle(pCastle);
			} else {
				// unknown object
				check(0);
			}
		} else {
			// unknown action
			check(0);
		}
		if (m_bDead) return false;

		if (bNeedAction) return false;
		if (iMapEvent* pEvent = gGame.Map().FindMapEvent(m_NextPoint, m_pOwner->PlayerId())) {
			StopMoving();
			if (pEvent->Activate(this, m_pOwner->PlayerType() == PT_HUMAN, true)) {
				gGame.Map().RemoveMapEvent(pEvent);
			}
			return false;
		} else if (NeedStop()) {
			m_bStop = false;
			StopMoving();
			return false;
		}
	} else {
		StartMoving();
	}

	m_Angle = m_Path.GetAngle();
	m_NextPoint = m_Path.GetStepPos();


	gGame.OnHeroMoveTo(this, m_Path.GetStep());
	m_bStop = false;

	return true;
}

void iHero::Stop()
{
	m_bStop = true;
}

bool iHero::NeedStop()
{
	if (m_pOwner->NeedStopHero()) {
		ProcessItemChanges();
		m_pOwner->ResetVistItemChanges();
	}
	return m_bStop || m_Path.IsEmpty() || m_Moves < m_Path.GetStepCost();
}

void iHero::ProcessItemChanges()
{
	if (iAI_Player* pAIPlayer =  DynamicCast<iAI_Player*>(m_pOwner)){
		Stop();
		m_Path.SetEmpty(m_Pos);
		pAIPlayer->ProcessItemChanges();
	} else {
		for (uint32 xx=0; xx<m_pOwner->NewVisItems().Size(); ++xx){
			if (m_Path.BelongToPath(m_pOwner->NewVisItems()[xx].value->Pos())) {
				// Stop hero if new items belong to path
				Stop();
				m_Path.SetEmpty(m_Pos);
			}
		}
	}
}

iNewLevelInfo iHero::NextLevelInfo()
{
	iNewLevelInfo result;

	// Primary skill
	sint32 pskill = CalcRandValue(HERO_PRIM_SKILL[Type()],PRSKILL_COUNT);
	check(pskill != -1);
	result.prSkillType = (PRSKILL_TYPE)pskill;

	// Secondary skill
	uint8 frSkills[SECSK_COUNT];
	memcpy(frSkills,HERO_SEC_SKILL[Type()],sizeof(frSkills));
	uint8 upSkills[SECSK_COUNT];
	memset(upSkills,0,sizeof(upSkills));

	sint32 frCnt = 8-m_SecSkills.Count();
	sint32 upCnt = 0;

	for (uint32 xx=0; xx<m_SecSkills.Count(); ++xx) {
		if (m_SecSkills.At(xx).m_level < SSLVL_EXPERT) {
			upSkills[m_SecSkills.At(xx).m_skill] = (uint8)iMAX<sint32>(1,frSkills[m_SecSkills.At(xx).m_skill]);
			upCnt++;
		}
		frSkills[m_SecSkills.At(xx).m_skill] = 0;
	}

	if (upCnt) {
		sint32 sskill = CalcRandValue(upSkills, SECSK_COUNT);
		result.secSkill[0] = (SECONDARY_SKILLS)sskill;
		upSkills[result.secSkill[0]] = 0;
		upCnt--;
	} else {
		if (frCnt) {
			result.secSkill[0] = (SECONDARY_SKILLS)CalcRandValue(frSkills, SECSK_COUNT);
			frSkills[result.secSkill[0]] = 0;
		} else {
			// nothing to learn
			result.secSkill[0] = result.secSkill[1] = SECSK_NONE;
			return result;
		}
	}

	if (frCnt){
		result.secSkill[1] = (SECONDARY_SKILLS)CalcRandValue(frSkills, SECSK_COUNT);
		frSkills[result.secSkill[1]] = 0;
	} else {
		if (upCnt){
			result.secSkill[1] = (SECONDARY_SKILLS)CalcRandValue(upSkills, SECSK_COUNT);
			upSkills[result.secSkill[1]] = 0;
			upCnt--;
		} else {
			result.secSkill[1] = SECSK_NONE;
		}
	}

	return result;
}

void iHero::Process(fix32 t)
{
}

bool iHero::NeedArtifact(uint16 artId) const
{
	// Process negative case for special artifacts
	if (gGame.ItemMgr().m_artMgr[artId].Type() == ARTT_NEGSPHERE && Owner()->PlayerType() == PT_COMPUTER) {
		sint32 mval = FurtSkill(FSK_POWER) + FurtSkill(FSK_KNOWLEDGE);
		sint32 sval = FurtSkill(FSK_ATTACK) + FurtSkill(FSK_DEFENCE);
		return mval > 20 || (mval > 3 && mval > (sval/2));
	}
	return true;
}

void iHero::CaptureArtifacts(const iArtList& artList)
{
	// show dialog with captured artifacts (for active player)
	if (m_pOwner->PlayerType() == PT_HUMAN) {
		iDlg_CapArt cadlg(&gApp.ViewMgr(), this, artList);
		cadlg.DoModal();
		// Add artifacts to heroe's inventory
		for (uint32 xx=0; xx<artList.GetSize(); ++xx) m_Artifacts.Push(artList[xx]);
	} else {
		// Add artifacts to heroe's inventory using SmartPush method
		for (uint32 xx=0; xx<artList.GetSize(); ++xx) m_Artifacts.SmartPush(artList[xx]);
	}

}

bool iHero::HasSpecFlag(SPEC_HERO_FLAGS flag) const
{
	return m_Artifacts.HasArtType(ARTT_ULTIMATE) && ULTART_SPECFLAGS[m_pProto->m_hType] == flag;
}

sint32 iHero::RiseSkeletons(sint32 enCas)
{
	sint32 val = FurtSkill(FSK_NECROMANCY);
	if (!val || !enCas || m_SecSkills.SkillLevel(SECSK_NECROMANCY) == SSLVL_NONE) return 0;

	sint32 quant = iCLAMP<sint32>(1,enCas,(enCas * val) / 100);

	CREATURE_TYPE ct = HasSpecFlag(SHF_NECRBONUS)?CREAT_ZOMBIE:CREAT_SKELETON;

	// show dialog with raised skeletons
	if (m_pOwner == gGame.Map().ActPlayer()) {
		iDlg_RiseSkeletons rsdlg(&gApp.ViewMgr(), m_pOwner->PlayerId(), ct, quant);
		rsdlg.DoModal();
	}

	// Add Skeletons to army
	m_Army.AddGroup(ct, quant);

	return quant;
}

iRewardItem* iHero::ReceiveReward(const iRewardItem& reward)
{
	iRewardItem *res = new iRewardItem(reward.m_type, reward.m_fParam, reward.m_sParam);
	switch(reward.m_type) {
	case RIT_MINERAL:		
		if(m_pOwner->GetMineral((MINERAL_TYPE)reward.m_fParam) + reward.m_sParam < 0)
			res->m_sParam = - m_pOwner->GetMineral((MINERAL_TYPE)reward.m_fParam);
		m_pOwner->AddMineral((MINERAL_TYPE)reward.m_fParam, res->m_sParam);	
		return res->m_sParam != 0 ? res : NULL;
	case RIT_EXPERIENCE:
		if(reward.m_sParam > 0) {
			ReceiveExperience(reward.m_sParam);
			return res;
		} 
		return NULL;		
	case RIT_MANAPTS:	
		if(m_manaPts + reward.m_sParam< 0)
			res->m_sParam = -m_manaPts;
		m_manaPts += res->m_sParam;
		return res->m_sParam != 0 ? res : NULL;
	case RIT_TRAVELPTS:
		if(m_Moves + reward.m_sParam < 0)
			res->m_sParam = -m_Moves;
		m_Moves += res->m_sParam;
		return res->m_sParam != 0 ? res : NULL;		
	case RIT_MORALE:
		m_Enchs.Add((sint32)(&reward), FSK_MORALE,ENDUR_NEXTBATTLE,reward.m_sParam);
		return res;
	case RIT_LUCK:
		m_Enchs.Add((sint32)(&reward), FSK_LUCK,ENDUR_NEXTBATTLE,reward.m_sParam);
		return res;
	case RIT_FURTSKILL:
		check(reward.m_fParam >= FSK_ATTACK && reward.m_fParam <= FSK_KNOWLEDGE);
		if(m_constFurtSkills.Value((FURTHER_SKILLS)reward.m_fParam) + reward.m_sParam < 0)
			res->m_sParam = -m_constFurtSkills.Value((FURTHER_SKILLS)reward.m_fParam);
		m_constFurtSkills.Value((FURTHER_SKILLS)reward.m_fParam) = res->m_sParam;
		return res->m_sParam != 0 ? res : NULL;	
	case RIT_ARTIFACT:
		if(reward.m_fParam == ARTT_RAND)
			res->m_fParam = gGame.ItemMgr().m_artMgr.SelectRandomArtifact(ART_LEVEL_NONE);
		else if(reward.m_fParam == ARTT_RAND_L1)
			res->m_fParam = gGame.ItemMgr().m_artMgr.SelectRandomArtifact(ART_LEVEL_TREASURE);
		else if(reward.m_fParam == ARTT_RAND_L2)
			res->m_fParam = gGame.ItemMgr().m_artMgr.SelectRandomArtifact(ART_LEVEL_MINOR);
		else if(reward.m_fParam == ARTT_RAND_L3)
			res->m_fParam = gGame.ItemMgr().m_artMgr.SelectRandomArtifact(ART_LEVEL_MAJOR);
		else if(reward.m_fParam == ARTT_RAND_L4)
			res->m_fParam = gGame.ItemMgr().m_artMgr.SelectRandomArtifact(ART_LEVEL_RELICT);

		if (m_pOwner->PlayerType() == PT_HUMAN) m_Artifacts.Push(res->m_fParam);
		else m_Artifacts.SmartPush(res->m_fParam);
		return res;
	case RIT_MAGICSPELL:
		if (CanLearnSpell(reward.m_fParam)) { LearnSpell(reward.m_fParam); return res; }
		return NULL;		
	case RIT_CREATGROUP: 
		{
			if(m_Army.CreatureCount((CREATURE_TYPE)reward.m_fParam) + reward.m_sParam < 0)
				res->m_sParam = -m_Army.CreatureCount((CREATURE_TYPE)reward.m_fParam);

			iCreatGroup cg((CREATURE_TYPE)reward.m_fParam,res->m_sParam);
			if (m_pOwner->PlayerType() == PT_COMPUTER) {
				iAI_Player* pAIOwner = DynamicCast<iAI_Player*>(m_pOwner);
				check(pAIOwner);
				pAIOwner->ProcessJoinCreatures(m_Army, cg);
			} else {				
				if (!m_Army.AddGroup(cg.Type(), cg.Count())) {
					iDlg_ArmyRoom arDlg(&gApp.ViewMgr(), *this, cg);
					arDlg.DoModal();
				}
			}
			return res->m_sParam != 0 ? res : NULL;
		}
		break;
	default:
		check(0);
	}
	return NULL;
}

bool iHero::Dig()
{
	if (m_pOwner->PlayerType() == PT_HUMAN) {
		if (!m_bCanDig) {
			iTextDlg tdlg(&gApp.ViewMgr(), _T(""), gTextMgr[TRID_MSG_DIG_TRY_TOMORROW], m_pOwner->PlayerId());
			tdlg.DoModal();
		} else if (!gGame.Map().CanDig(m_Pos)) {
			iTextDlg tdlg(&gApp.ViewMgr(), _T(""), gTextMgr[TRID_MSG_DIG_TRY_ON_CLEAR_LAND], m_pOwner->PlayerId());
			tdlg.DoModal();
		} else {
			// dig here
			gSfxMgr.PlaySound(CSND_DIG);
			m_Moves = 0;
			if (gGame.Map().Dig(m_Pos)) {
				m_Artifacts.SmartPush(ARTT_ULTIMATE);
				iIconDlg idlg(&gApp.ViewMgr(), _T(""), gTextMgr[TRID_MSG_DIG_FOUND], PDGG_ULTIMATE_ART, m_pOwner->PlayerId());
				idlg.DoModal();
				return true;
			} else {
				iTextDlg tdlg(&gApp.ViewMgr(), _T(""), gTextMgr[TRID_MSG_DIG_NOTHING], m_pOwner->PlayerId());
				tdlg.DoModal();
			}
		}
	} else {
		if (!m_bCanDig || !gGame.Map().CanDig(m_Pos)) {
			check(0);
		} else {
			// dig here
			if (gGame.Map().Dig(m_Pos)) {
				m_Artifacts.SmartPush(ARTT_ULTIMATE);
				m_Moves = 0;
				return true;
			}
			m_Moves = 0;
		}
	}
	return false;
}

iStringT iHero::Name() const
{
	if (!m_CustomName.Empty()) return m_CustomName;
	return gTextMgr[m_pProto->m_NameKey];
}

const HERO_TYPE iHero::Type() const
{
	return m_pProto->m_hType;
}

uint8 iHero::Scouting() const
{ 
	return (uint8)iCLAMP<sint32>(1,128,DEF_HERO_SCOUTING+FurtSkill(FSK_SCOUTING));
}

sint32 iHero::TotalMoves() const
{ 
	// initial speed according to slowest creature
	sint32 tmoves = (DEF_HERO_MOVES + (m_Army.SlowestSpeed()*2));
	tmoves += (tmoves * FurtSkill(FSK_LOGISTICS)) / 100;
	tmoves += FurtSkill(FSK_ACTPTS);

	return tmoves;
}

sint32 iHero::MoveCostBonus() const
{
	// 0 - 100;
	return FurtSkill(FSK_PATHFINDING);
}

const sint32 DEF_LEVEL_BOUNDS[12] = { 0, 1000, 2000, 3200, 4600, 6200, 8000, 10000, 12200, 14700, 17500, 20600 };

sint32 iHero::LevelBound(uint32 level) 
{ 
	// old formula: return (level-1)*(level-1)*500; 
	if (level < 2) return 0;
	if (level <= 12) return DEF_LEVEL_BOUNDS[level-1];

	sint32 cdif = (DEF_LEVEL_BOUNDS[11] - DEF_LEVEL_BOUNDS[10]);
	sint32 val = DEF_LEVEL_BOUNDS[11];

	for (uint32 nn=12; nn<level; ++nn) {	
		cdif += cdif / 5;
		val += cdif;
	}
	return val;
}

sint32 iHero::FurtSkill(FURTHER_SKILLS furtSkill) const 
{ 
	sint32 res = m_constFurtSkills.Value(furtSkill) + m_varFurtSkills.Value(furtSkill);
	if (m_pOwner) res += m_pOwner->FurtSkills().Value(furtSkill);
	return  res;
}

iFurtSkills iHero::GetFullFurtSkills() const 
{ 
	iFurtSkills res = m_constFurtSkills + m_varFurtSkills;
	if (m_pOwner) res += m_pOwner->FurtSkills();
	return  res; 
}

uint8 iHero::ReceiveExperience(sint32 points)
{
	m_Experience += points;
	
	while (ReachNextLevel()) {
		m_Level++;
		iNewLevelInfo nli = NextLevelInfo();
		m_constFurtSkills.Value((FURTHER_SKILLS)(FSK_ATTACK+nli.prSkillType))++;
		if (m_pOwner == gGame.Map().ActPlayer()) {
			gSfxMgr.PlaySound(CSND_NEW_LEVEL_DLG);
			iDlg_NewLevel dlg(&gApp.ViewMgr(), this, nli);
			dlg.DoModal();
			SECONDARY_SKILLS ss = dlg.SecSkill();
			if (ss != SECSK_NONE) m_SecSkills.Inc(ss);
		} else {
			check(Owner()->PlayerType() == PT_COMPUTER);
			if ( 1 /* difficulty <= normal */ ) {
				if (nli.secSkill[0] != SECSK_NONE && nli.secSkill[1] != SECSK_NONE)	m_SecSkills.Inc(nli.secSkill[gGame.Map().Rand(2)]);
				else if (nli.secSkill[0] != SECSK_NONE) m_SecSkills.Inc(nli.secSkill[0]);
				else if (nli.secSkill[1] != SECSK_NONE) m_SecSkills.Inc(nli.secSkill[1]);
			} else {
				// more smart upgrade policy
				// sort offered skills according the order of preference
				// than by upgrade/add status
				// pick the first
			}
		}
	}


	if (m_pOwner == gGame.Map().ActPlayer()) gGame.AddCellMsg(iFormat(_T("+%d#I%04X"),points,PDGG_ICN_EXPERIENCE), m_Pos);

	return 0;
}

sint32 iHero::MaxManaPts() const
{
	return (FurtSkill(FSK_KNOWLEDGE) * 10) + ((FurtSkill(FSK_KNOWLEDGE) * 10 * FurtSkill(FSK_INTELLIGENCE))/100);
}

void iHero::SetLocation(iMapCnst* pLocation) 
{
	if (m_pLocation == pLocation) return;
	iCastle* pCastle;
	
	if (pCastle = DynamicCast<iCastle*>(m_pLocation)) {
		pCastle->OnVisitorLeave(this);
	}
	
	m_pLocation = pLocation;

	if (pCastle = DynamicCast<iCastle*>(m_pLocation)) {
		pCastle->OnVisitorEnter(this);
	}
}

void iHero::CastSpell(MAGIC_SPELL spell)
{
	check(spell != MSP_INVALID && SPELL_DESCRIPTORS[spell].type == SPT_OVERLAND);

	iOverlandSpell* pSpell =  (iOverlandSpell*)gGame.ItemMgr().m_spellMgr.Spell(spell);

	uint8 cost = iBaseSpell::GetSpellCost(pSpell->Id(),this);
	if (m_Moves < cost) {
		iTextDlg tdlg(&gApp.ViewMgr(), iStringT(), gTextMgr[TRID_MSG_TIRED_TO_CAST],m_pOwner->PlayerId());
		tdlg.DoModal();
		return;
	}

	if (pSpell->Cast(this)) {
		m_manaPts -= cost;
		m_Moves -= cost;
	}
}

void iHero::AddCharm(MAGIC_SPELL spell, sint32 flag)
{
	m_charms.Add(iCharmEntry(spell,flag));
}

void iHero::NewDay()
{
	// Remove "next day" enchancers
	m_Enchs.RemoveDur(ENDUR_NEXTDAY);

	// Restore moves
	m_Moves = TotalMoves();

	// Reset "can dig" flag
	m_bCanDig = true;

	// Restore mana points
	if (m_manaPts < MaxManaPts()) {
		if (HasSpecFlag(SHF_MANARESTORE)) {
			m_manaPts = MaxManaPts();
		} else {
			m_manaPts = iMIN<sint32>(MaxManaPts(),m_manaPts + DEF_HERO_MYSTICISM + FurtSkill(FSK_MANAPTS));
		}
	}

	// Increase minerals without owner's income
	for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) m_pOwner->Minerals().quant[xx] += m_constFurtSkills.Value((FURTHER_SKILLS)(FSK_MIN_GOLD+xx)) + m_varFurtSkills.Value((FURTHER_SKILLS)(FSK_MIN_GOLD+xx));

	// Remove all charms
	for (uint32 xx=0; xx<m_charms.GetSize(); ++xx) {
		MAGIC_SPELL spell = m_charms[xx].spellId;
		check(spell != MSP_INVALID && SPELL_DESCRIPTORS[spell].type == SPT_OVERLAND);
		iOverlandSpell* pSpell =  (iOverlandSpell*)gGame.ItemMgr().m_spellMgr.Spell(spell);
		pSpell->OnRemove(this, m_charms[xx].flag);
	}
	m_charms.RemoveAll();
}

void iHero::NewWeek(const iWeekDesc& weekDesk)
{
	m_Enchs.RemoveDur(ENDUR_NEXTWEEK);

	// this is a fix for 'Stables bonus move' problem #0000203
	// its caused by wrong order of NewDay / NewWeek calls
	// where NewWeek cancels stables effect but does not recalculate move points
	// normally I would refactor NewDay/NewWeek into one function with parameter
	// or factor out effects/updates separately but for now simply...
	// NB: Calling it twice also affects anti-hack trick in a bad way:(
	m_Moves = TotalMoves();
}

void iHero::OnEndBattle(bool bWin)
{
	m_Enchs.RemoveDur(ENDUR_NEXTBATTLE);
	if (m_Army.Empty()) {
		m_Army.AddGroup((CREATURE_TYPE)INITIAL_HERO_ARMY[Proto()->m_hType][0][0],1);
	}
	if (bWin && m_pOwner->PlayerType() == PT_HUMAN) {
		m_pOwner->SetCurHero(this);
		gGame.OnHeroChanged(this);
	}
}

void iHero::OnSelect()
{
	//
	//if (m_pOwner == gGame.Map().ActPlayer()) gGame.AddMsg(iFormat(gTextMgr[TRID_MSG_HEROSELECTED].CStr(),Name().CStr(),gTextMgr[TRID_HERO_TYPE_KNIGHT+Type()].CStr(),Level()));
	gGame.OnHeroChanged(this);
	// Recalculate path
	if (m_pOwner->PlayerType() == PT_COMPUTER) {
		m_Path.SetEmpty(Pos());
	} else if (!m_Path.IsEmpty()) {
		iPoint dpos = m_Path.DestPoint();
		m_Path.SetEmpty(Pos());
		SetPath(dpos);
	}
}

iPath::Target iHero::GetTarget(sint16 x, sint16 y, bool bIgnoreFog)
{
	iPath::Target res;
	iBaseMapObject* pObj = gGame.Map().m_CoverMap.GetAt(x,y);
	check(pObj);
	if (DynamicCast<iHero*>(pObj) || DynamicCast<iMapGuard*>(pObj) || DynamicCast<iMapItem_Mineral*>(pObj) || DynamicCast<iMapItem_ManaCrystal*>(pObj) || DynamicCast<iMapItem_CampFire*>(pObj) || DynamicCast<iMapItem_Chest*>(pObj) || DynamicCast<iMapItem_Lamp*>(pObj) || DynamicCast<iMapItem_Artifact*>(pObj) || DynamicCast<iMapItem_SpellScroll*>(pObj)  || DynamicCast<iMapItem_KeyGuard*>(pObj)) {
		res.epmask = 0xFF;
		if (bIgnoreFog || !Owner()->FogMap().IsHidden(iPoint(x,y))) {
			if (iHero* pHero = DynamicCast<iHero*>(pObj)) {
				if (pHero->Owner() == Owner()) res.action = iPath::Meet;
				else res.action = iPath::Attack;
				if (iMapCnst* pCnst = pHero->GetLocation()) {
					if (DynamicCast<iCastle*>(pCnst)) {
						res.epmask = 0x82;
					} else {
						res.epmask = 0x83;
					}
				}
			} else if (DynamicCast<iMapGuard*>(pObj)) {
				res.action = iPath::Attack;
			} else {
				res.action = iPath::Touch;
			}
		}
	} else if (iVisCnst* pVisCnst = DynamicCast<iVisCnst*>(pObj) ){
		res.action = iPath::Enter;
		res.epmask = 0x83;
	} else if (iOwnCnst* pOwnCnst = DynamicCast<iOwnCnst*>(pObj)) {
		res.action = iPath::Enter;
		res.epmask = 0x83;
	} else if (iCastle* pCastle = DynamicCast<iCastle*>(pObj)) {
		if (pCastle->Owner() != Owner()->PlayerId() && !pCastle->Garrison().Empty()) res.action = iPath::Attack;
		else res.action = iPath::Enter;
		res.epmask = 0x82;
	} else {
		check(0);
	}

	return res;
}

bool iHero::MakePath(iPath& path, sint16 x, sint16 y)
{
	m_pOwner->UpdatePassMap();

	if (m_Pos.x == x && m_Pos.y == y) {
		// inline activation
		check(m_pLocation);
		ActivateConstruction(m_pLocation);
		return true;
	}

	sint8 oval = m_pOwner->PassMap().GetAt(x,y);
	if (oval == -1) return false;

	iPath::Target tgt;
	if (oval == -2) {
		tgt	= GetTarget(x,y);
		m_pOwner->PassMap().GetAt(x,y) = 5;
	}
	
	uint8 spMask = 0xFF;
	if (m_pLocation) {
		if (DynamicCast<iVisCnst*>(m_pLocation) || DynamicCast<iOwnCnst*>(m_pLocation)) spMask = CNST_PASS_MASK;
		else if (DynamicCast<iCastle*>(m_pLocation)) spMask = CSTL_PASS_MASK;
	}

	bool bRes = false;
	iPathFinder pf(m_pOwner->PassMap());
	path.SetEmpty(Pos());
	if (pf.FindPath(Pos(),iPoint(x,y),path,spMask, tgt.epmask) != -1){
		path.SetTargetAction((iPath::Action)tgt.action);
		m_NextPoint = Pos();
		bRes = true;
	}
	m_pOwner->PassMap().GetAt(x,y) = oval;
	return bRes;
}

void iHero::SetPath()
{
	if (!m_Path.m_Path.IsEmpty()) SetPath( m_Path.DestPoint() );
	else if (m_pLocation) ActivateConstruction(m_pLocation);
}

void iHero::SetPath(const iPoint& cell)
{
	if (m_bMoving) {
		Stop();
	} else if (m_Path.IsDestPoint(cell)) {
		Step();
	} else if (gGame.Map().IsValidPos(cell.x,cell.y) && Pos() != cell) {
		//
		MakePath(m_Path, (sint16)cell.x, (sint16)cell.y);
	}
}

void iHero::SetPath(const iPath& path)
{
	check(!m_bMoving);
	m_Path = path;
	m_NextPoint = Pos();
}

void iHero::ResetPath()
{
	m_Path.SetEmpty(Pos());
}

VISION_LEVEL iHero::VisionLevel(sint32& radius) const
{
	radius = 0;
	if (CharmedBy(MSP_VISIONS, radius)) {
		return VL_EXPERT;
	} else if (m_Army.HasCreatures(CREAT_ROGUE)) {
		radius = 5;
		return VL_EXPERT;
	} else if (m_pOwner->CastleCount()) {
		return VL_BASIC;
	}
	return VL_NONE;
}

void iHero::ResetArmy()
{
	m_Army.ResetArmy();
	
	sint8 ht = m_pProto->m_hType;
	sint32 rcnt = INITIAL_HERO_ARMY[ht][0][1] + gGame.Map().Rand(INITIAL_HERO_ARMY[ht][0][2]-INITIAL_HERO_ARMY[ht][0][1]);
	m_Army.AddGroup((CREATURE_TYPE)INITIAL_HERO_ARMY[ht][0][0], rcnt);
	rcnt = INITIAL_HERO_ARMY[ht][1][1] + gGame.Map().Rand(INITIAL_HERO_ARMY[ht][1][2]-INITIAL_HERO_ARMY[ht][1][1]);
	if (rcnt) m_Army.AddGroup((CREATURE_TYPE)INITIAL_HERO_ARMY[ht][1][0], rcnt);
}

void iHero::ActivateConstruction(iMapCnst* pCnst)
{
	pCnst->Activate(this, m_pOwner == gGame.Map().ActPlayer());
	if (m_bDead) return;
	m_pOwner->UpdateVisItems();
}

void iHero::SiegeCastle(iCastle* pCastle)
{
	check(pCastle->Owner() != m_pOwner->PlayerId());
	check(!pCastle->Garrison().Empty() || pCastle->Visitor());
	iBattleInfo bi(this, pCastle);
	gGame.BeginBattle(bi);
}

void iHero::CaptureCastle(iCastle* pCastle, bool bEnter)
{
	iPlayer* oldPlayer = NULL;
	if (pCastle->Owner() != PID_NEUTRAL) {
		oldPlayer = gGame.Map().FindPlayer(pCastle->Owner());
		check(oldPlayer);
		oldPlayer->RemoveCastle(pCastle);
	} else {
		gGame.Map().m_Castles.Remove(iGameWorld::iCtIt(pCastle));
	}

	pCastle->SetOwner(m_pOwner->PlayerId());
	m_pOwner->AddCastle(pCastle);
	m_pOwner->OnCaptureCastle(pCastle);

	if (oldPlayer && !oldPlayer->DefeatState()) gGame.Map().RemovePlayer(oldPlayer);

	if (bEnter) {
		EnterCastle(pCastle);
	}
}

void iHero::EnterCastle(iCastle* pCastle)
{
	if (Owner() == gGame.Map().ActPlayer()) {
		if (m_pOwner->CurCastle() != pCastle) {
			m_pOwner->SetCurCastle(pCastle);
		}

		gGame.ShowView(iChildGameView::CV_CASTLE);
	} 
}

void iHero::TouchMapItem(iMapItem* pMapItem, bool bPostBattle)
{
	if (pMapItem->Activate(this, m_pOwner == gGame.Map().ActPlayer(), !bPostBattle)){
		gGame.OnDisapObject(pMapItem->Sprite(), pMapItem->Pos());
		gGame.Map().RemoveMapItem(pMapItem);
		m_pOwner->UpdateVisItems();
	}
}

void iHero::TouchMapGuard(iMapGuard* pMapGuard, bool bPostBattle)
{
	if (bPostBattle || pMapGuard->Activate(this, m_pOwner == gGame.Map().ActPlayer())){
		if (m_pOwner == gGame.Map().ActPlayer()) {
			gSfxMgr.PlaySound(CSND_DEL_GUARD);
		}
		gGame.OnDisapObject(pMapGuard->Sprite(),pMapGuard->Pos());
		gGame.Map().RemoveMapGuard(pMapGuard);
		m_pOwner->UpdateVisItems();
	}
}

void iHero::AttackHero(iHero* pHero)
{
	iBattleInfo bi(this, pHero);
	gGame.BeginBattle(bi);
}

void iHero::MeetHero(iHero* pHero)
{
	check(m_pOwner->PlayerId() == pHero->Owner()->PlayerId());
	if (m_pOwner->PlayerType() == PT_HUMAN) {
		gGame.MeetHeroes(this, pHero, true);
	} else {
		iAI_Player* pAIPlyer = DynamicCast<iAI_Player*>(m_pOwner);
		check(pAIPlyer && pAIPlyer->NeedMeeting(this, pHero));
		pAIPlyer->MeetHeroes(this, pHero);
	}
}

