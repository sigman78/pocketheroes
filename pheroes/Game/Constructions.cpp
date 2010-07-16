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
#include "Dlg_LeaveGuards.h"
#include "Dlg_ArmyRoom.h"
#include "Dlg_FightGuard.h"
#include "Dlg_Rewards.h"
#include "Dlg_Recruit.h"
#include "Dlg_PuzzleMap.h"
#include "serialize.h"
#include "helpers.h"

/*
 *	Common construction type
 */
iHero* iMapCnst::Visitor() const
{ 
	return DynamicCast<iHero*>(gGame.Map().m_CoverMap.GetAt(Pos().x,Pos().y)); 
}

/*
 *	Visitables
 */
void iVisCnst::NewDay(){}

void iVisCnst::NewWeek(const iWeekDesc& weekDesk) {}

bool iVisCnst::Activate(iHero* pVisitor, bool bActive)
{
	if (!m_Guard.Empty()) {
		if (bActive) {
			iDlg_FightGuard dlg(&gApp.ViewMgr(), gTextMgr[m_pProto->NameKey()], m_Guard, pVisitor->Owner()->PlayerId(), VL_BASIC);
			if (dlg.DoModal() == DRC_YES) {
				iBattleInfo bi(pVisitor, this);
				gGame.BeginBattle(bi);
			}
		} else {
			iBattleInfo bi(pVisitor, this);
			gGame.BeginBattle(bi);
		}
		return false;
	} else {
		if (bActive) {
			if (!m_customMsg.Empty()) {
				iTextDlg dlg(&gApp.ViewMgr(),_T(""),m_customMsg,pVisitor->Owner()->PlayerId());
				dlg.DoModal();
			} else if (!m_bCustomGUI){
				if (Visited(pVisitor)) {
					iTextDlg dlg(&gApp.ViewMgr(),gTextMgr[m_pProto->NameKey()],gTextMgr[m_pProto->VisMessage()],pVisitor->Owner()->PlayerId());
					dlg.DoModal();
					return false;
				} else {
					iTextDlg dlg(&gApp.ViewMgr(),gTextMgr[m_pProto->NameKey()],gTextMgr[m_pProto->Message()],pVisitor->Owner()->PlayerId());
					dlg.DoModal();
				}
			}
		} 
	}

	OnActivate(pVisitor, bActive);
	return true;
}

void iVisCnst::OnActivate(iHero* pVisitor, bool bActive)
{
	m_Visitors.Add(pVisitor->Proto()->m_protoId);
}

bool iVisCnst::Visited(const iHero* pVisitor) const
{
	return (m_Visitors.Find(pVisitor->Proto()->m_protoId) != -1);
}

void iVisCnst::Serialize(iDynamicBuffer& dbuff)
{
	// Guard
	::Serialize(dbuff, m_Guard);
	// Custom message
	::Serialize(dbuff, m_customMsg);
	// Visitors
	dbuff.Write((uint16)m_Visitors.GetSize());
	for (uint32 vtrid=0; vtrid<m_Visitors.GetSize(); ++vtrid) dbuff.Write(m_Visitors[vtrid]);
	// flag
	dbuff.Write((uint8)1);
}

uint8 iVisCnst::Unserialize(iDynamicBuffer& dbuff)
{
	// Guard
	::Unserialize(dbuff, m_Guard);
	::InitArmy(m_Guard);
	// Custom message
	::Unserialize(dbuff, m_customMsg);
	// Visitors
	uint16 vcnt; dbuff.Read(vcnt);
	while (vcnt--) {
		uint16 visitor; dbuff.Read(visitor);
		m_Visitors.Add(visitor);
	}
	// flag
	uint8 flag;
	dbuff.Read(flag);
	return flag;
}

//////////////////////////////////////////////////////////////////////////
void iVisCnst_Stable::NewWeek(const iWeekDesc& weekDesk)
{
	iVisCnst::NewWeek(weekDesk);
	m_Visitors.RemoveAll();
}

void iVisCnst_Stable::OnActivate(iHero* pVisitor, bool bActive)
{
	iVisCnst::OnActivate(pVisitor, bActive);
	iVisCnstT_Stable* pStable = DynamicCast<iVisCnstT_Stable*>(m_pProto);
	check(pStable);
	pVisitor->AddMoves(pStable->ActPts());
	pVisitor->Enchs().Add(m_pProto->Type(), FSK_LOGISTICS, ENDUR_NEXTWEEK, pStable->ActPts());
}

bool iVisCnst_Stable::Visited(const iHero* pVisitor) const
{
	return pVisitor->Enchs().Registered(m_pProto->Type());
}
//////////////////////////////////////////////////////////////////////////
void iVisCnst_Gazebo::OnActivate(iHero* pVisitor, bool bActive)
{
	if (Visited(pVisitor)) {
		if (bActive) {
			iTextDlg tdlg(&gApp.ViewMgr(),gTextMgr[m_pProto->NameKey()], gTextMgr[m_pProto->VisMessage()], pVisitor->Owner()->PlayerId());
			tdlg.DoModal();
		}
	} else {
		sint32 exp = pVisitor->ConvExpPts(1000);
		if (bActive) {
			iRewardsCtr rew;
			rew.Add(iRewardItem(RIT_EXPERIENCE, 0, exp));
			iDlg_Rewards dlg(&gApp.ViewMgr(),gTextMgr[m_pProto->NameKey()], gTextMgr[m_pProto->Message()], pVisitor, rew);
			dlg.DoModal();
		}
		pVisitor->ReceiveExperience(exp);
	}

	iVisCnst::OnActivate(pVisitor, bActive);
}
//////////////////////////////////////////////////////////////////////////
void iVisCnst_ManaSource::NewWeek(const iWeekDesc& weekDesk)
{
	iVisCnst::NewWeek(weekDesk);
	m_Visitors.RemoveAll();
}

sint32 iVisCnst_ManaSource::ManaPts() const
{
	iVisCnstT_ManaSource* pManaSource = DynamicCast<iVisCnstT_ManaSource*>(m_pProto);
	check(pManaSource);
	return pManaSource->ManaPts();
}

void iVisCnst_ManaSource::OnActivate(iHero* pVisitor, bool bActive)
{
	iVisCnst::OnActivate(pVisitor, bActive);
	sint32 act_pts = pVisitor->MaxManaPts() * ManaPts();
	if (pVisitor->ManaPts() < act_pts) pVisitor->ManaPts() = act_pts;
	pVisitor->Enchs().Add(m_pProto->ProtoId() | iEnchItem::PROVIDER_FLAG, FSK_INVALID, ENDUR_NEXTWEEK, 0);
}

bool iVisCnst_ManaSource::Visited(const iHero* pVisitor) const
{
	return pVisitor->Enchs().Registered(m_pProto->ProtoId() | iEnchItem::PROVIDER_FLAG);
}
//////////////////////////////////////////////////////////////////////////
void iVisCnst_MLModifier::NewWeek(const iWeekDesc& weekDesk)
{
	iVisCnst::NewWeek(weekDesk);
	m_Visitors.RemoveAll();
}

void iVisCnst_MLModifier::OnActivate(iHero* pVisitor, bool bActive)
{
	if (Visited(pVisitor)) {
		if (bActive) {
			iTextDlg tdlg(&gApp.ViewMgr(),gTextMgr[m_pProto->NameKey()], gTextMgr[m_pProto->VisMessage()], pVisitor->Owner()->PlayerId());
			tdlg.DoModal();
		}
	} else {
		iVisCnstT_MLModifier* pProto = DynamicCast<iVisCnstT_MLModifier*>(m_pProto);
		check(pProto);
		if (bActive) {
			iRewardsCtr rew;
			if (pProto->MoraleModifier()) rew.Add(iRewardItem(RIT_MORALE, 0, pProto->MoraleModifier()));
			if (pProto->LuckModifier()) rew.Add(iRewardItem(RIT_LUCK, 0, pProto->LuckModifier()));
			iDlg_Rewards dlg(&gApp.ViewMgr(),gTextMgr[m_pProto->NameKey()], gTextMgr[m_pProto->Message()], pVisitor, rew);
			dlg.DoModal();
		}
		if (pProto->MoraleModifier()) pVisitor->Enchs().Add(m_pProto->ProtoId() | iEnchItem::PROVIDER_FLAG, FSK_MORALE, ENDUR_NEXTBATTLE, pProto->MoraleModifier());
		if (pProto->LuckModifier()) pVisitor->Enchs().Add(m_pProto->ProtoId() | iEnchItem::PROVIDER_FLAG, FSK_LUCK, ENDUR_NEXTBATTLE, pProto->LuckModifier());
	}

	iVisCnst::OnActivate(pVisitor, bActive);
}

bool iVisCnst_MLModifier::Visited(const iHero* pVisitor) const
{
	return pVisitor->Enchs().Registered(m_pProto->ProtoId() | iEnchItem::PROVIDER_FLAG);
}
//////////////////////////////////////////////////////////////////////////
void iVisCnst_PSModifier::OnActivate(iHero* pVisitor, bool bActive)
{
	if (Visited(pVisitor)) {
		if (bActive) {
			iTextDlg tdlg(&gApp.ViewMgr(),gTextMgr[m_pProto->NameKey()], gTextMgr[m_pProto->VisMessage()], pVisitor->Owner()->PlayerId());
			tdlg.DoModal();
		}
	} else {
		iVisCnstT_PSModifier* pPSModifier = DynamicCast<iVisCnstT_PSModifier*>(m_pProto);
		check(pPSModifier);
		if (bActive) {
			iRewardsCtr rew;
			for (uint32 psid = 0; psid<PRSKILL_COUNT; ++psid) {
				if (pPSModifier->PSModifier().val[psid]) rew.Add(iRewardItem(RIT_FURTSKILL, FSK_ATTACK + psid, pPSModifier->PSModifier().val[psid]));
			}
			iDlg_Rewards dlg(&gApp.ViewMgr(),gTextMgr[m_pProto->NameKey()], gTextMgr[m_pProto->Message()], pVisitor, rew);
			dlg.DoModal();
		}
		pVisitor->ConstFurtSkills() += pPSModifier->PSModifier();
	}

	iVisCnst::OnActivate(pVisitor, bActive);
}
//////////////////////////////////////////////////////////////////////////
void iVisCnst_Obelisk::OnActivate(iHero* pVisitor, bool bActive)
{
	check(!Visited(pVisitor));
	uint16 opened = pVisitor->Owner()->OpenObelisk();
	m_Visitors.Add(pVisitor->Owner()->PlayerId());
	if (bActive) {
		iPuzzleMapDlg pmdlg(&gApp.ViewMgr(), *pVisitor->Owner(), opened);
		pmdlg.DoModal();
	} else {
		iAI_Player* pOwner = DynamicCast<iAI_Player*>(pVisitor->Owner());
		check(pOwner);
		pOwner->UpdateUltSeekPoint();
	}
}

bool iVisCnst_Obelisk::Visited(const iHero* pVisitor) const
{
	for (uint32 xx=0; xx<m_Visitors.GetSize(); ++xx) if (!pVisitor->Owner() || m_Visitors[xx] == pVisitor->Owner()->PlayerId()) return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////
void iVisCnst_Sign::OnActivate(iHero* pVisitor, bool bActive)
{
	iVisCnst::OnActivate(pVisitor, bActive);
}
//////////////////////////////////////////////////////////////////////////
void iVisCnst_Dwelling::NewWeek(const iWeekDesc& weekDesk)
{
	if (weekDesk.wkType == iWeekDesc::Normal) {
		m_creatCount += m_growth;
	} else if (weekDesk.wkType == iWeekDesc::Plague) {
		m_creatCount /= 2;
	} else if (weekDesk.wkType == iWeekDesc::CreatPlus) {
		m_creatCount += m_growth;
		if (weekDesk.crType == m_creatType) m_creatCount += 5;
	} else if (weekDesk.wkType == iWeekDesc::CreatDoubles) {
		m_creatCount += m_growth;
		if (weekDesk.crType == m_creatType) m_creatCount *= 2;
	}
}

void iVisCnst_Dwelling::OnActivate(iHero* pVisitor, bool bActive)
{
	if (Visited(pVisitor)) {
		if (bActive) {
			iCreatGroup cg(m_creatType, m_creatCount);
			iTextDlg tdlg(&gApp.ViewMgr(),gTextMgr[m_pProto->NameKey()], iFormat(gTextMgr[TRID_OBJ_DWELL_EMPTY], gTextMgr[TRID_CREATURE_PEASANT_F3+cg.Type()*3]), pVisitor->Owner()->PlayerId());
			tdlg.DoModal();
		}
	} else {
		if (bActive) {
			iCreatGroup cg(m_creatType, m_creatCount);
			if (!JoinForFree()) {
				// Buy
				iDlg_Recruit rdlg(&gApp.ViewMgr(), cg, pVisitor->Army(), pVisitor->Owner()->PlayerId());
				if (rdlg.DoModal() == DRC_OK) {
					m_creatCount = cg.Count();
				}
			} else {
				// Join Army
				iQuestDlg dj(&gApp.ViewMgr(), gTextMgr[m_pProto->NameKey()], iFormat(gTextMgr[TRID_OBJ_DWELL_RECR_QUEST], gTextMgr[TRID_CREATURE_PEASANT_F3+cg.Type()*3]), pVisitor->Owner()->PlayerId());
				if (dj.DoModal() == DRC_YES) {
					iIconDlg idlg(&gApp.ViewMgr(), gTextMgr[m_pProto->NameKey()], iFormat(gTextMgr[TRID_OBJ_DWELL_RECR_MSG], m_creatCount, gTextMgr[TRID_CREATURE_PEASANT_F3+cg.Type()*3]), PDGG_MINIMON + cg.Type(), pVisitor->Owner()->PlayerId());
					idlg.DoModal();
					if (!pVisitor->Army().AddGroup(cg.Type(),cg.Count())) {
						iDlg_ArmyRoom arDlg(&gApp.ViewMgr(), *pVisitor, cg);
						arDlg.DoModal();
					}
					m_creatCount = 0;
				}
			}
		} else {
			iAI_Player* pOwner = DynamicCast<iAI_Player*>(pVisitor->Owner());
			check(pOwner);
			iVisCnstT_Dwelling* pProto = DynamicCast<iVisCnstT_Dwelling*>(m_pProto);
			check(pProto);
			iCreatGroup cg(pProto->CreatType(),m_creatCount);
			if (!JoinForFree()) {
				// Buy
				if (pOwner->ProcessRecruitCreatures(pVisitor->Army(), cg)) {
					m_creatCount -= cg.Count();
				}
			} else {
				// Join
				if (pOwner->ProcessJoinCreatures(pVisitor->Army(), cg)) m_creatCount = 0;
			}
		}
	}
}

bool iVisCnst_Dwelling::Visited(const iHero* pVisitor) const
{
	return m_creatCount == 0;
}

void iVisCnst_Dwelling::Serialize(iDynamicBuffer& dbuff)
{
	// Common data serialization
	iVisCnst::Serialize(dbuff);
	// Write creatures count
	dbuff.Write(m_creatCount);
}

uint8 iVisCnst_Dwelling::Unserialize(iDynamicBuffer& dbuff)
{
	uint8 flag = iVisCnst::Unserialize(dbuff);
	if (flag == 0) {
		// Init by default
		m_creatCount = m_growth;
	} else {
		// Read data
		dbuff.Read(m_creatCount);
	}
	return flag;
}

//////////////////////////////////////////////////////////////////////////
#include "Dlg_WitchHut.h"
void iVisCnst_WitchHut::OnActivate(iHero* pVisitor, bool bActive)
{
	if (Visited(pVisitor)) {
		if (bActive) {
			iTextDlg tdlg(&gApp.ViewMgr(),gTextMgr[m_pProto->NameKey()], iFormat(gTextMgr[TRID_OBJ_WITCHHUT_MSG2], gTextMgr[TRID_HSKILL_ESTATES+m_skill]), pVisitor->Owner()->PlayerId());
			tdlg.DoModal();
		}
	} else {
		if (pVisitor->SecSkills().SkillLevel(m_skill) == SSLVL_NONE && pVisitor->SecSkills().Count() < 8) {
			if (bActive) {
				iWitchHutDlg whdlg(&gApp.ViewMgr(), m_skill, pVisitor->Owner()->PlayerId());
				whdlg.DoModal();
				//iIconDlg idlg(&gApp.ViewMgr(), gTextMgr[m_pProto->NameKey()], iFormat(gTextMgr[TRID_OBJ_WITCHHUT_MSG1], gTextMgr[TRID_HSKILL_ESTATES+m_skill]), PDGG_SEC_SKILLS + m_skill, pVisitor->Owner()->PlayerId());
				//idlg.DoModal();
			}
			pVisitor->SecSkills().Set(m_skill, SSLVL_BASIC);
		} else {
			if (bActive) {
				iTextDlg tdlg(&gApp.ViewMgr(), gTextMgr[m_pProto->NameKey()], iFormat(gTextMgr[TRID_OBJ_WITCHHUT_MSG3], gTextMgr[TRID_HSKILL_ESTATES+m_skill]), pVisitor->Owner()->PlayerId());
				tdlg.DoModal();
			}
		}
	}
	m_visPlayers.AddUnique(pVisitor->Owner()->PlayerId());
	iVisCnst::OnActivate(pVisitor, bActive);
}

void iVisCnst_WitchHut::Serialize(iDynamicBuffer& dbuff)
{
	// Common data serialization
	iVisCnst::Serialize(dbuff);
	// Write skill
	dbuff.Write( (uint8) m_skill);
	// Visited players
	dbuff.Write( (uint8) m_visPlayers.GetSize() );
	for (uint32 xx=0; xx<m_visPlayers.GetSize(); ++xx) dbuff.Write((uint8)m_visPlayers[xx]);
}

uint8 iVisCnst_WitchHut::Unserialize(iDynamicBuffer& dbuff)
{
	uint8 flag = iVisCnst::Unserialize(dbuff);
	if (flag == 0) {
		// Init by default
		do {
			m_skill = (SECONDARY_SKILLS)gGame.Map().Rand(SECSK_COUNT);
		} while (m_skill == SECSK_NECROMANCY || m_skill == SECSK_LEADERSHIP);
	} else {
		// Skill
		uint8 skill; dbuff.Read(skill);
		m_skill = (SECONDARY_SKILLS)skill;
		// Visited players
		uint8 vpcnt; dbuff.Read(vpcnt);
		for (uint32 xx=0; xx<vpcnt; ++xx) {
			uint8 vpid; dbuff.Read(vpid);
			m_visPlayers.Add( (PLAYER_ID) vpid);
		}
	}
	return flag;
}

bool iVisCnst_WitchHut::CustomPopupText(const iHero* pVisitor, iStringT& ctext) const
{
	if (pVisitor && m_visPlayers.Find(pVisitor->Owner()->PlayerId()) != -1 ) {
		ctext = gTextMgr[TRID_HSKILL_ESTATES+m_skill];
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
void iVisCnst_Shrine::OnActivate(iHero* pVisitor, bool bActive)
{
	if (pVisitor->CanLearnSpell(m_spell)) {
		if (bActive) {
			iIconDlg idlg(&gApp.ViewMgr(), gTextMgr[m_pProto->NameKey()], iStringT(gTextMgr[m_pProto->Message()])+_T("\"")+gTextMgr[TRID_SPNAME_MAGICARROW+m_spell]+_T("\"."), PDGG_SPELL_ICONS + m_spell, pVisitor->Owner()->PlayerId());
			idlg.DoModal();
		}
		pVisitor->LearnSpell(m_spell);
	} else {
		if (bActive) {
			iTextDlg tdlg(&gApp.ViewMgr(), gTextMgr[m_pProto->NameKey()], iStringT(gTextMgr[m_pProto->Message()])+_T("\"")+gTextMgr[TRID_SPNAME_MAGICARROW+m_spell]+_T("\". ")+gTextMgr[TRID_OBJ_SHRINE_NOWISDOM], pVisitor->Owner()->PlayerId());
			tdlg.DoModal();
		}
	}
	m_visPlayers.AddUnique(pVisitor->Owner()->PlayerId());
	iVisCnst::OnActivate(pVisitor, bActive);
}

void iVisCnst_Shrine::Serialize(iDynamicBuffer& dbuff)
{
	// Common data serialization
	iVisCnst::Serialize(dbuff);
	// Write spell
	dbuff.Write( (uint16) m_spell);
	// Visited players
	dbuff.Write( (uint8) m_visPlayers.GetSize() );
	for (uint32 xx=0; xx<m_visPlayers.GetSize(); ++xx) dbuff.Write((uint8)m_visPlayers[xx]);
}

uint8 iVisCnst_Shrine::Unserialize(iDynamicBuffer& dbuff)
{
	uint8 flag = iVisCnst::Unserialize(dbuff);
	if (flag == 0) {
		// Init by default
		iSpellList spells;
		SelectSpells(spells, SpellFilter(SPT_MASK_ALL, SPELL_LEVEL_MASKS[((iVisCnstT_Shrine*)m_pProto)->SpellLevel()], MST_MASK_ALL), 1, MSP_INVALID);
		check(spells.GetSize());
		m_spell = (MAGIC_SPELL)spells[0];
	} else {
		// Skill
		uint16 spell; dbuff.Read(spell);
		m_spell = (MAGIC_SPELL)spell;
		// Visited players
		uint8 vpcnt; dbuff.Read(vpcnt);
		for (uint32 xx=0; xx<vpcnt; ++xx) {
			uint8 vpid; dbuff.Read(vpid);
			m_visPlayers.Add( (PLAYER_ID) vpid);
		}
	}
	return flag;
}

bool iVisCnst_Shrine::CustomPopupText(const iHero* pVisitor, iStringT& ctext) const
{
	if (pVisitor && m_visPlayers.Find(pVisitor->Owner()->PlayerId()) != -1 ) {
		ctext = gTextMgr[TRID_SPNAME_MAGICARROW+m_spell];
		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
void iVisCnst_Treasury::OnActivate(iHero* pVisitor, bool bActive)
{
	if (Visited(pVisitor)) {
		if (bActive) {
			iTextDlg tdlg(&gApp.ViewMgr(),gTextMgr[m_pProto->NameKey()], iFormat(gTextMgr[TRID_OBJ_TREASURY_VISITED], gTextMgr[m_pProto->NameKey()]), pVisitor->Owner()->PlayerId());
			tdlg.DoModal();
		}
	} else {
		iRewardsCtr received;
		iRewardItem *temp;
		for (uint32 xx=0; xx<m_rewards.GetSize(); ++xx) 
			if((temp = pVisitor->ReceiveReward(m_rewards[xx])))
				received.Add(*temp);
		if (bActive && received.GetSize() > 0) {
			iDlg_Rewards dlg(&gApp.ViewMgr(),gTextMgr[m_pProto->NameKey()],gTextMgr[TRID_OBJ_TREASURY_DISCOVER],pVisitor,received);
			dlg.DoModal();
		}
				
		m_bVisited = true;
	}
	iVisCnst::OnActivate(pVisitor, bActive);
}

void iVisCnst_Treasury::Serialize(iDynamicBuffer& dbuff)
{
	// Common data serialization
	iVisCnst::Serialize(dbuff);
	// Is visited
	dbuff.Write((uint8)(m_bVisited?1:0));
	// Rewards
	::Serialize(dbuff, m_rewards);
}

uint8 iVisCnst_Treasury::Unserialize(iDynamicBuffer& dbuff)
{
	uint8 flag = iVisCnst::Unserialize(dbuff);
	if (flag == 0) {
		// Init by default
		check(m_Guard.Empty());
		iVisCnstT_Treasury* pProto = DynamicCast<iVisCnstT_Treasury*>(m_pProto);
		check(pProto);
		iTrVarCtr& tvar = pProto->GetVarCtr();
		m_bVisited = false;
		uint32 val = gGame.Map().Rand(100);
		bool bSel = false;
		for (uint32 xx=0; xx<tvar.VariantsCount(); ++xx) {
			if (val <= tvar.GetVariant(xx).probability) {
				m_Guard = tvar.GetVariant(xx).guards;
				m_rewards = tvar.GetVariant(xx).rewards;
				bSel = true;
				break;
			}
			val -= tvar.GetVariant(xx).probability;
		}
		check(bSel);
		// Initialize reward item (if necessary)
		for (uint32 xx=0; xx<m_rewards.GetSize(); ++xx) {
			if (m_rewards[xx].m_type == RIT_ARTIFACT) {
				// random artifact
				m_rewards[xx].m_fParam = gGame.ItemMgr().m_artMgr.SelectRandomArtifact((ART_LEVEL_TYPE)m_rewards[xx].m_fParam);
			} else if (m_rewards[xx].m_type == RIT_MAGICSPELL) {
				// random spell
				iSpellList spells;
				if (m_rewards[xx].m_fParam == 0) SelectSpells(spells, SpellFilter(SPT_MASK_ALL, SPL_MASK_ALL, MST_MASK_ALL), 1, MSP_INVALID);
				else SelectSpells(spells, SpellFilter(SPT_MASK_ALL, SPELL_LEVEL_MASKS[m_rewards[xx].m_fParam-1], MST_MASK_ALL), 1, MSP_INVALID);
				check(spells.GetSize());
				m_rewards[xx].m_fParam = spells[0];
			}
		}
	} else {
		// Is visited
		uint8 bVisited;
		dbuff.Read(bVisited);
		m_bVisited = (bVisited>0)?true:false;
		// Rewards
		::Unserialize(dbuff, m_rewards);
	}
	return flag;
}

//////////////////////////////////////////////////////////////////////////
void iVisCnst_Teleport::OnActivate(iHero* pVisitor, bool bActive)
{
	if ((IsTwoWay() || IsEntrance()) && m_tgtIdx != -1) {
		iVisCnst_Teleport* pTarget = gGame.Map().GetTeleport(m_tgtIdx);
		check(pTarget);
		// Check for visitor at target cell
		if (iHero* pTeleportVisitor = DynamicCast<iHero*>(gGame.Map().m_CoverMap.GetAt(pTarget->Pos().x,pTarget->Pos().y))) {
			check(pTeleportVisitor->GetLocation() == pTarget);
			if (pTeleportVisitor->Owner()->PlayerId() != pVisitor->Owner()->PlayerId()) pVisitor->AttackHero(pTeleportVisitor);
			else pVisitor->MeetHero(pTeleportVisitor);
		} else {
			gGame.Map().MoveHero(pVisitor->Pos(), pTarget->Pos());
			pVisitor->SetPos(pTarget->Pos());
			pVisitor->Owner()->UpdateFogMap();
			gGame.OnHeroPosChanged(pVisitor, pVisitor->Pos());
			gGame.OnHeroChanged(pVisitor);
			gGame.OnHeroTeleport(pVisitor, Pos(), pTarget->Pos());
		}
	} else {
		if (bActive) {
			iTextDlg tdlg(&gApp.ViewMgr(), gTextMgr[((iVisCnstT_Teleport*)m_pProto)->NameKey()], gTextMgr[TRID_OBJ_TELEPORT_EXIT_MSG], pVisitor->Owner()->PlayerId());
			tdlg.DoModal();
		}
	}
}

uint8 iVisCnst_Teleport::Index() const
{
	return ((iVisCnstT_Teleport*)m_pProto)->Index();
}

bool iVisCnst_Teleport::IsEntrance() const
{
	return ((iVisCnstT_Teleport*)m_pProto)->IsEntrance();
}

bool iVisCnst_Teleport::IsTwoWay() const
{
	return ((iVisCnstT_Teleport*)m_pProto)->IsTwoWay();
}

bool iVisCnst_Teleport::CustomPopupText(const iHero* pVisitor, iStringT& ctext) const
{
	if (IsTwoWay())	ctext = gTextMgr[TRID_OBJ_TELEPORT_TWOWAY];
	else if (IsEntrance()) ctext = gTextMgr[TRID_OBJ_TELEPORT_ONEWAY_ENT];
	else ctext = gTextMgr[TRID_OBJ_TELEPORT_ONEWAY_EXT];
	return true;
}

//////////////////////////////////////////////////////////////////////////
uint8 iVisCnst_Keymaster::GetKeyType() const
{
	return ((iVisCnstT_Keymaster*)m_pProto)->GetKeyType();
}

void iVisCnst_Keymaster::OnActivate(iHero* pVisitor, bool bActive)
{
	pVisitor->Owner()->AddKey(GetKeyType());
}

bool iVisCnst_Keymaster::Visited(const iHero* pVisitor) const
{
	return pVisitor->Owner() && pVisitor->Owner()->HasKey(GetKeyType());
}

//////////////////////////////////////////////////////////////////////////
void iVisCnst_TreeOfKnowledge::OnActivate(iHero* pVisitor, bool bActive)
{
	if (Visited(pVisitor)) {
		if (bActive) {
			iTextDlg tdlg(&gApp.ViewMgr(),gTextMgr[m_pProto->NameKey()], gTextMgr[m_pProto->VisMessage()], pVisitor->Owner()->PlayerId());
			tdlg.DoModal();
		}
	} else {
		sint32 exp = pVisitor->LevelBound(pVisitor->Level()+1) - pVisitor->Experience();
		if (bActive) {
			iRewardsCtr rew;
			rew.Add(iRewardItem(RIT_EXPERIENCE, 0, exp));
			iDlg_Rewards dlg(&gApp.ViewMgr(),gTextMgr[m_pProto->NameKey()], gTextMgr[m_pProto->Message()], pVisitor, rew);
			dlg.DoModal();
		}
		pVisitor->ReceiveExperience(exp);
	}

	iVisCnst::OnActivate(pVisitor, bActive);
}

//////////////////////////////////////////////////////////////////////////
void iVisCnst_Windmill::NewWeek(const iWeekDesc& weekDesk)
{
	m_bVisited = false;
	UpdateMinerals();
}

void iVisCnst_Windmill::OnActivate(iHero* pVisitor, bool bActive)
{
	if (Visited(pVisitor)) {
		if (bActive) {
			iTextDlg tdlg(&gApp.ViewMgr(),gTextMgr[m_pProto->NameKey()], gTextMgr[m_pProto->VisMessage()], pVisitor->Owner()->PlayerId());
			tdlg.DoModal();
		}
	} else {
		if (bActive) {
			iRewardsCtr rew;
			rew.Add(iRewardItem(RIT_MINERAL, m_mtype, m_mcnt));
			iDlg_Rewards dlg(&gApp.ViewMgr(),gTextMgr[m_pProto->NameKey()], gTextMgr[m_pProto->Message()], pVisitor, rew);
			dlg.DoModal();
		}
		pVisitor->Owner()->AddMineral(m_mtype, m_mcnt);
		m_bVisited = true;
	}
}

bool iVisCnst_Windmill::Visited(const iHero* pVisitor) const
{
	return m_bVisited;
}

void iVisCnst_Windmill::Serialize(iDynamicBuffer& dbuff)
{
	// Common data serialization
	iVisCnst::Serialize(dbuff);
	// 
	dbuff.Write((uint8)m_bVisited);
	dbuff.Write((sint32)m_mtype);
	dbuff.Write(m_mcnt);
}

uint8 iVisCnst_Windmill::Unserialize(iDynamicBuffer& dbuff)
{
	uint8 flag = iVisCnst::Unserialize(dbuff);
	if (flag == 0) {
		// Init by default
		m_bVisited = false;
		UpdateMinerals();
	} else {
		// Read data
		uint8 bVisited;
		dbuff.Read(bVisited);
		m_bVisited = (bVisited == 0)?false:true;
		sint32 mtype;
		dbuff.Read(mtype);
		m_mtype = (MINERAL_TYPE)mtype;
		dbuff.Read(m_mcnt);
	}
	return flag;
}

void iVisCnst_Windmill::UpdateMinerals()
{
	m_mtype = (MINERAL_TYPE)(MINERAL_ORE + gGame.Map().Rand(6)); 
	m_mcnt = 3 + gGame.Map().Rand(4);
}

//////////////////////////////////////////////////////////////////////////
void iVisCnst_WeeklyMin::NewWeek(const iWeekDesc& weekDesk)
{
	m_bVisited = false;
}

void iVisCnst_WeeklyMin::OnActivate(iHero* pVisitor, bool bActive)
{
	iVisCnstT_WeeklyMin* pProto = (iVisCnstT_WeeklyMin*)m_pProto;
	if (Visited(pVisitor)) {
		if (bActive) {
			iTextDlg tdlg(&gApp.ViewMgr(),gTextMgr[m_pProto->NameKey()], gTextMgr[m_pProto->VisMessage()], pVisitor->Owner()->PlayerId());
			tdlg.DoModal();
		}
	} else {
		if (bActive) {
			iRewardsCtr rew;
			for (uint32 mm=0; mm<MINERAL_COUNT; ++mm) {
				if (pProto->Minerals().quant[mm]) rew.Add(iRewardItem(RIT_MINERAL, mm, pProto->Minerals().quant[mm]));
			}
			iDlg_Rewards dlg(&gApp.ViewMgr(),gTextMgr[m_pProto->NameKey()], gTextMgr[m_pProto->Message()], pVisitor, rew);
			dlg.DoModal();
		}
		for (uint32 mm=0; mm<MINERAL_COUNT; ++mm) {
			if (pProto->Minerals().quant[mm]) 
				pVisitor->Owner()->AddMineral((MINERAL_TYPE)mm, pProto->Minerals().quant[mm]); 
		}
		
		m_bVisited = true;
	}
}

bool iVisCnst_WeeklyMin::Visited(const iHero* pVisitor) const
{
	return m_bVisited;
}

void iVisCnst_WeeklyMin::Serialize(iDynamicBuffer& dbuff)
{
	// Common data serialization
	iVisCnst::Serialize(dbuff);
	// 
	dbuff.Write((uint8)m_bVisited);
}

uint8 iVisCnst_WeeklyMin::Unserialize(iDynamicBuffer& dbuff)
{
	uint8 flag = iVisCnst::Unserialize(dbuff);
	if (flag == 0) {
		// Init by default
		m_bVisited = false;
	} else {
		// Read data
		uint8 bVisited;
		dbuff.Read(bVisited);
		m_bVisited = (bVisited == 0)?false:true;
	}
	return flag;
}
/*
 *	Ownerables
 */
iOwnCnst::iOwnCnst(iOwnCnstT* pProto, const iPoint& pos) 
: iMapCnst(pos), m_pProto(pProto), m_Owner(PID_NEUTRAL)
{
}

void iOwnCnst::NewDay() {}

void iOwnCnst::NewWeek(const iWeekDesc& weekDesk) 
{
	if (m_Owner == PID_NEUTRAL) {
		// grow guardians population for neutral construction
		for (uint32 xx=0; xx<7; ++xx) {
			if (m_Guard.At(xx).IsValid()) m_Guard.At(xx).Grow();
		}
	}
}

bool iOwnCnst::Activate(iHero* pVisitor, bool bActive)
{
	if (m_Owner != pVisitor->Owner()->PlayerId() && !m_Guard.Empty()) {
		if (bActive) {
			iDlg_FightGuard dlg(&gApp.ViewMgr(), gTextMgr[m_pProto->NameKey()], m_Guard, pVisitor->Owner()->PlayerId(), VL_BASIC);
			if (dlg.DoModal() == DRC_YES) {
				iBattleInfo bi(pVisitor, this);
				gGame.BeginBattle(bi);
			}
		} else {
			iBattleInfo bi(pVisitor, this);
			gGame.BeginBattle(bi);
		}
		return false;
	} else if (m_Owner == pVisitor->Owner()->PlayerId()) {
		if (bActive) {
			iDlg_LeaveGuards lgdlg(&gApp.ViewMgr(), pVisitor, this);
			lgdlg.DoModal();
			return true;
		} else {
			// not implemented
			//check(0);
		}
	} else {
		if (m_Owner != PID_NEUTRAL) {
			iPlayer* oldPlayer = gGame.Map().FindPlayer(m_Owner);
			check(oldPlayer);
			oldPlayer->m_Cnsts.Remove(iPlayer::iCLIt(this));
			oldPlayer->UpdateFogMap();
		} else {
			gGame.Map().m_OwnCnsts.Remove(iGameWorld::iOCIt(this));
		}
		SetOwner(pVisitor->Owner()->PlayerId());
		pVisitor->Owner()->AddCnst(this);
	}
	
	OnActivate(pVisitor, bActive);
	return true;
}

void iOwnCnst::OnActivate(iHero* pVisitor, bool bActive)
{
	if (bActive) {
		gSfxMgr.PlaySound(CSND_FLAG_MINE);
		if (m_pProto->MessageIcon() != 0xFFFF) {
			iIconDlg dlg(&gApp.ViewMgr(), gTextMgr[m_pProto->NameKey()], gTextMgr[m_pProto->Message()], m_pProto->MessageIcon(), pVisitor->Owner()->PlayerId());
			dlg.DoModal();
		} else {
			iTextDlg dlg(&gApp.ViewMgr(), gTextMgr[m_pProto->NameKey()], gTextMgr[m_pProto->Message()], pVisitor->Owner()->PlayerId());
			dlg.DoModal();
		}
	}
}

void iOwnCnst::SetOwner(PLAYER_ID newOwner)
{
	PLAYER_ID oldOwner = m_Owner;
	m_Owner = newOwner;
	OnOwnerChanged(newOwner, oldOwner);
}

void iOwnCnst::OnOwnerChanged(PLAYER_ID newOwner, PLAYER_ID oldOwner)
{
}

//////////////////////////////////////////////////////////////////////////
void iOwnCnst_FurtSkill::OnOwnerChanged(PLAYER_ID newOwner, PLAYER_ID oldOwner)
{
	iOwnCnstT_FurtSkill* pProto = DynamicCast<iOwnCnstT_FurtSkill*>(m_pProto);
	check(pProto);
	if (newOwner == oldOwner) return;
	iFurtSkills nAmount = pProto->FurtSkills();
	// subtract income
	if (oldOwner != PID_NEUTRAL) gGame.Map().FindPlayer(oldOwner)->FurtSkills() -= nAmount;
	// add income
	if (newOwner != PID_NEUTRAL) {
		iPlayer* newOwnerP = gGame.Map().FindPlayer(newOwner);
		gGame.Map().FindPlayer(newOwner)->FurtSkills() += nAmount;
	}
}
