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
#include "Dlg_Chest.h"
#include "Dlg_Other.h"
#include "Dlg_ArmyRoom.h"
#include "Dlg_FightGuard.h"
#include "Dlg_ArtInfo.h"
#include "Dlg_Recruit.h"
#include "Dlg_Rewards.h"

//////////////////////////////////////////////////////////////////////////
iMapItem::iMapItem(const iPoint& pos, MAP_ITEM_TYPE mit, const SpriteId& sid)
: iBaseMapObject(pos, true), m_mapItemType(mit), m_spriteId(sid) 
{
	
}

bool iMapItem::Activate(iHero* pHero, bool bActive, bool bShowMessage)
{
	if (bActive) {
		if (bShowMessage && !m_message.Empty()) {
			iTextDlg dlg(&gApp.ViewMgr(), _T(""), m_message,pHero->Owner()->PlayerId());
			dlg.DoModal();
		}
		if (!Guard().Empty()) {
			if (bActive) {
				iDlg_FightGuard dlg(&gApp.ViewMgr(), gTextMgr[NameKey()], m_Guard, PID_NEUTRAL, VL_EXPERT);
				if (dlg.DoModal() == DRC_YES) {
					iBattleInfo bi(pHero, this);
					gGame.BeginBattle(bi);
				}
				return false;
			}
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
iMapItem_Mineral::iMapItem_Mineral(const iPoint& pos, MINERAL_TYPE mt, sint32 count)
: iMapItem(pos, MAPITEM_MINERAL, PDGG_RES_ICONS+mt), m_mineralType(mt), m_count(count) 
{ 
	if (mt == MINERAL_UNKNOWN) {
		mt = (MINERAL_TYPE)(MINERAL_GOLD + gGame.Map().Rand(MINERAL_COUNT));
		m_spriteId = PDGG_RES_ICONS+mt;
		m_mineralType = mt;
	}

	if (m_count == RANDOM_QUANTITY) {
		if (mt == MINERAL_GOLD)	m_count = 500 + gGame.Map().Rand(501); 
		else if (mt == MINERAL_WOOD || mt == MINERAL_ORE) m_count = 5 + gGame.Map().Rand(6); 
		else m_count = 3 + gGame.Map().Rand(4); 
	}
}

bool iMapItem_Mineral::Activate(iHero* pHero, bool bActive, bool bShowMessage)
{
	if (!iMapItem::Activate(pHero, bActive, bShowMessage)) return false;
	pHero->Owner()->AddMineral(m_mineralType, m_count);
	if (bActive) {
		gSfxMgr.PlaySound(CSND_PICKUP01 + (m_Pos.x+m_Pos.y) % 5);
		gGame.AddCellMsg(iFormat(_T("+%d#I%04X"),m_count,PDGG_RES_MINI_ICONS + m_mineralType), m_Pos);
	}
	return true;
}

void iMapItem_Mineral::Serialize(iDynamicBuffer& buff)
{
	buff.Write((sint8)m_mineralType);
	buff.Write((sint32)m_count);
}

//////////////////////////////////////////////////////////////////////////
iMapItem_ManaCrystal::iMapItem_ManaCrystal(const iPoint& pos, sint32 count)
: iMapItem(pos, MAPITEM_MANACRYSTAL, PDGG_RES_MCRYST), m_count(count)
{ if (m_count == RANDOM_QUANTITY) m_count = 5 + gGame.Map().Rand(20); }

bool iMapItem_ManaCrystal::Activate(iHero* pHero, bool bActive, bool bShowMessage)
{
	if (!iMapItem::Activate(pHero, bActive, bShowMessage)) return false;
	pHero->ManaPts() += m_count;
	if (bActive) {
		gGame.AddCellMsg(iFormat(_T("+%d#I%04X"),m_count,PDGG_ICN_MANA), m_Pos);
		gSfxMgr.PlaySound(CSND_PICKUP01 + (m_Pos.x+m_Pos.y) % 5);
	}
	return true;
}

void iMapItem_ManaCrystal::Serialize(iDynamicBuffer& buff)
{
	buff.Write((sint32)m_count);
}

//////////////////////////////////////////////////////////////////////////
iMapItem_CampFire::iMapItem_CampFire(const iPoint& pos, sint32 goldCount, MINERAL_TYPE addMinType, sint32 addMinCount)
: iMapItem(pos, MAPITEM_CAMPFIRE, PDGG_RES_CMFIRE)
, m_addMinType(addMinType) 
, m_addMinCount(addMinCount)
, m_goldCount(goldCount)
{
	if (m_goldCount == RAND_VAL) m_goldCount = 400 + (gGame.Map().Rand(3))*100;
	if (m_addMinType == MINERAL_UNKNOWN) m_addMinType = (MINERAL_TYPE)(MINERAL_ORE + gGame.Map().Rand(6)); 
	if (m_addMinCount == RAND_VAL) m_addMinCount = 4+gGame.Map().Rand(3);
}

bool iMapItem_CampFire::Activate(iHero* pHero, bool bActive, bool bShowMessage)
{
	if (!iMapItem::Activate(pHero, bActive, bShowMessage)) return false;

	pHero->Owner()->AddMineral(MINERAL_GOLD, m_goldCount);
	pHero->Owner()->AddMineral(m_addMinType, m_addMinCount);
	if (bActive) {
		gGame.AddCellMsg(iFormat(_T("+%d#I%04X +%d#I%04X"),m_goldCount,PDGG_RES_MINI_ICONS, m_addMinCount, PDGG_RES_MINI_ICONS + m_addMinType), m_Pos);
		gSfxMgr.PlaySound(CSND_PICKUP01 + (m_Pos.x+m_Pos.y) % 5);
	}
	return true;
}

void iMapItem_CampFire::Serialize(iDynamicBuffer& buff)
{
	buff.Write(m_goldCount);
	buff.Write((sint8)m_addMinType);
	buff.Write(m_addMinCount);
}

//////////////////////////////////////////////////////////////////////////
iMapItem_Chest::iMapItem_Chest(const iPoint& pos, sint32 count)
: iMapItem(pos, MAPITEM_CHEST, PDGG_RES_CHEST), m_count(count) 
{
	if (m_count == RAND_VAL) {
		m_count = 500 + (gGame.Map().Rand(10) * 100);
	}
}

bool iMapItem_Chest::Activate(iHero* pHero, bool bActive, bool bShowMessage)
{
	if (!iMapItem::Activate(pHero, bActive, bShowMessage)) return false;
	if (bActive) {
		iDlg_Chest dlg(&gApp.ViewMgr(), pHero, this);
		sint32 res = dlg.DoModal();
		if (res == iDlg_Chest::Gold) {
			pHero->Owner()->AddMineral(MINERAL_GOLD, dlg.GoldCount(), true);
			if (bActive) gGame.AddCellMsg(iFormat(_T("+%d#I%04X"),dlg.GoldCount(),PDGG_RES_MINI_ICONS + MINERAL_GOLD), m_Pos);
		} else if (res == iDlg_Chest::Experience) {
			pHero->ReceiveExperience(dlg.ExpCount());
		} else {
			check(0);
		}
		gSfxMgr.PlaySound(CSND_PICKUP01 + (m_Pos.x+m_Pos.y) % 5);
	} else {
		iAI_Player* pOwner = DynamicCast<iAI_Player*>(pHero->Owner());
		check(pOwner);
		pOwner->ProcessChest(pHero, this);
	}

	return true;
}

void iMapItem_Chest::Serialize(iDynamicBuffer& buff)
{
	buff.Write(m_count);
}

/////////////////////////////////////////////////////////////////////////
iMapItem_Artifact::iMapItem_Artifact(const iPoint& pos, uint16 aid)
: iMapItem(pos, MAPITEM_ARTIFACT, gGame.ItemMgr().m_artMgr[aid].Sprite()), m_artId(aid) {}

bool iMapItem_Artifact::Activate(iHero* pHero, bool bActive, bool bShowMessage)
{
	if (!iMapItem::Activate(pHero, bActive, bShowMessage)) return false;
	if (bActive) {
		iDlg_ArtInfo dlg(&gApp.ViewMgr(), m_artId, pHero);
		dlg.DoModal();
		gSfxMgr.PlaySound(CSND_PICKUP01 + (m_Pos.x+m_Pos.y) % 5);
		pHero->Artifacts().Push(m_artId);
	} else {
		pHero->Artifacts().SmartPush(m_artId);
	}

	return true;
}

void iMapItem_Artifact::Serialize(iDynamicBuffer& buff)
{
	buff.Write((uint16)m_artId);
}
//////////////////////////////////////////////////////////////////////////
iMapItem_Lamp::iMapItem_Lamp(const iPoint& pos, sint32 count)
: iMapItem(pos, MAPITEM_LAMP, PDGG_RES_LAMP), m_count(count) 
{
	if (m_count == RAND_VAL) {
		m_count = 2 + gGame.Map().Rand(4);
	}
}

bool iMapItem_Lamp::Activate(iHero* pHero, bool bActive, bool bShowMessage)
{
	if (!iMapItem::Activate(pHero, bActive, bShowMessage)) return false;
	check(m_count > 0);
	if (bActive) {
		iCreatGroup cg(CREAT_GENIE, m_count);
		iDlg_Recruit rdlg(&gApp.ViewMgr(), cg, pHero->Army(), pHero->Owner()->PlayerId());
		if (rdlg.DoModal() == DRC_OK) {
			m_count = cg.Count();
			if (!m_count) gSfxMgr.PlaySound(CSND_PICKUP01 + (m_Pos.x+m_Pos.y) % 5);
		}
	} else {
		iAI_Player* pOwner = DynamicCast<iAI_Player*>(pHero->Owner());
		check(pOwner);
		iCreatGroup cg(CREAT_GENIE, m_count);
		if (pOwner->ProcessRecruitCreatures(pHero->Army(), cg)) {
			m_count -= cg.Count();
		}
	}

	return m_count == 0;
}

void iMapItem_Lamp::Serialize(iDynamicBuffer& buff)
{
	buff.Write(m_count);
}
/////////////////////////////////////////////////////////////////////////
iMapItem_SpellScroll::iMapItem_SpellScroll(const iPoint& pos, uint16 spellId)
: iMapItem(pos, MAPITEM_SPELLSCROLL, PDGG_RES_SPELL_SCROLL), m_spellId(spellId) {}

bool iMapItem_SpellScroll::Activate(iHero* pHero, bool bActive, bool bShowMessage)
{
	if (!iMapItem::Activate(pHero, bActive, bShowMessage)) return false;
	if (bActive) {
		iIconDlg dlg(&gApp.ViewMgr(), gTextMgr[TRID_MAPRES_SPELLSCROLL], iFormat(gTextMgr[TRID_MAPRES_SPELL_SCROLL_MSG], gTextMgr[TRID_SPNAME_MAGICARROW+m_spellId]), PDGG_SPELL_ICONS+m_spellId,pHero->Owner()->PlayerId());
		dlg.DoModal();
		gSfxMgr.PlaySound(CSND_PICKUP01 + (m_Pos.x+m_Pos.y) % 5);
	}

	pHero->LearnSpell(m_spellId, true);
	return true;
}

void iMapItem_SpellScroll::Serialize(iDynamicBuffer& buff)
{
	buff.Write((uint16)m_spellId);
}
/////////////////////////////////////////////////////////////////////////
iMapItem_KeyGuard::iMapItem_KeyGuard(const iPoint& pos, uint8 key)
: iMapItem(pos, MAPITEM_KEYGUARD, PDGG_KEY_GUARDS+key), m_key(key) {}

bool iMapItem_KeyGuard::Activate(iHero* pHero, bool bActive, bool bShowMessage)
{
	if (!iMapItem::Activate(pHero, bActive, bShowMessage)) return false;
	if (bActive) {
		if (pHero->Owner()->HasKey(m_key)) {
			iQuestDlg dlg(&gApp.ViewMgr(), gTextMgr[TRID_MAPRES_KEYGUARD], gTextMgr[TRID_MAPRES_KEYGUARD_OK_MSG], pHero->Owner()->PlayerId());
			if (dlg.DoModal() == DRC_YES) {
				gSfxMgr.PlaySound(CSND_DEL_GUARD);
				return true;
			}
		} else {
			iTextDlg dlg(&gApp.ViewMgr(), gTextMgr[TRID_MAPRES_KEYGUARD], gTextMgr[TRID_MAPRES_KEYGUARD_NOPASS_MSG], pHero->Owner()->PlayerId());
			dlg.DoModal();
			return false;
		}
	} else {
		check(pHero->Owner()->HasKey(m_key));
		if (pHero->Owner()->HasKey(m_key)) {
			return true;
		}
	}

	return false;
}

void iMapItem_KeyGuard::Serialize(iDynamicBuffer& buff)
{
	buff.Write((uint8)m_key);
}
//////////////////////////////////////////////////////////////////////////
iMapEvent::iMapEvent(const iPoint& pos)
: iBaseMapObject(pos, false)
{
}

bool iMapEvent::Activate(iHero* pHero, bool bActive, bool bShowMessage)
{

	// Guards
	if (!m_guard.Empty()) {
		// Show Attack message
		if (bActive && bShowMessage) {
			iTextDlg dlg(&gApp.ViewMgr(), iStringT(), (m_attackMessage.Empty())?gTextMgr[TRID_MSG_MAPEVENT_ATTACK]:m_attackMessage, pHero->Owner()->PlayerId());
			dlg.DoModal();
		}
		// Here is battle
		iBattleInfo bi(pHero, this);
		gGame.BeginBattle(bi);
		return false;
	} else if (bActive && bShowMessage && !m_attackMessage.Empty()) {
		iTextDlg dlg(&gApp.ViewMgr(), iStringT(), m_attackMessage, pHero->Owner()->PlayerId());
		dlg.DoModal();
	}

	// Rewards
	if (m_rewards.GetSize()) {
		// Show Reward message
		iRewardsCtr received;
		iRewardItem *temp;
		for (uint32 xx=0; xx<m_rewards.GetSize(); ++xx) 
			if((temp = pHero->ReceiveReward(m_rewards[xx])))
				received.Add(*temp);
		if (bActive && received.GetSize() > 0) {
			iDlg_Rewards dlg(&gApp.ViewMgr(),iStringT(), m_rewardMessage, pHero, received);
			dlg.DoModal();
		}				

	} else if (bActive && !m_rewardMessage.Empty()) {
		iTextDlg dlg(&gApp.ViewMgr(), iStringT(), m_rewardMessage, pHero->Owner()->PlayerId());
		dlg.DoModal();
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
CREATURE_TYPE NormalizeCreatType(CREATURE_TYPE ct)
{
	if (ct < CREAT_RANDOM) return ct;
	else if (ct == CREAT_RANDOM) return (CREATURE_TYPE)gGame.Map().Rand(CREAT_COUNT);
	else {
		sint32 lvl = ct - CREAT_RANDOM;
		iSimpleArray<sint32> cid;
		for (sint32 xx=0; xx<CREAT_COUNT; ++xx) {
			if (CREAT_DESC[xx].level == lvl) cid.Add(xx);
		}
		check(cid.GetSize()>0);
		return (CREATURE_TYPE)cid[gGame.Map().Rand(cid.GetSize())];
	}
}
//////////////////////////////////////////////////////////////////////////
iMapGuard::iMapGuard(const iPoint& pos, CREATURE_TYPE ct, sint32 cnt, sint32 joinVal, GUARD_DISPOS disp, bool bNotGrow)
: iBaseMapObject(pos, true), m_creatures(NormalizeCreatType(ct),cnt), m_disp(disp), m_bNotGrow(bNotGrow) 
{
	m_spriteId = PDGG_MINIMON+m_creatures.Type();

	// Init quantity
	if (cnt == RANDOM_QUANTITY) {
		m_creatures.Count() = (150 + gGame.Map().Rand(80) + CREAT_DESC[m_creatures.Type()].level * 10 ) / int_sqrt(CREAT_DESC[m_creatures.Type()].pidx);
	}

	m_joinVal = joinVal;
}

iMapGuard::MEET_RESULT iMapGuard::CheckMeetResult(iHero* pHero)
{
	bool bFlee = m_disp != GDISP_SAVAGE && pHero->Army().ArmyPower() > (m_creatures.GroupPower()*2);

	// join ?
	sint32 dip = iMIN<sint32>(100, 5 + pHero->FurtSkill(FSK_DIPLOMACY) / 2);
	bool bJoin = (m_disp == GDISP_COMPLIANT || (m_disp == GDISP_AGGRESSIVE && pHero->Army().ArmyPower() > m_creatures.GroupPower() && (dip > m_joinVal)));

	if (bFlee && bJoin) return MR_JOIN_OR_FLEE;
	else if (bFlee) return MR_FLEE;
	else if (bJoin) return MR_JOIN_OR_ATTACK;
	return MR_ATTACK;
}

bool iMapGuard::Activate(iHero* pHero, bool bActive)
{
	MEET_RESULT mr = CheckMeetResult(pHero);
	if (bActive) {
		// Show message
		if (!m_message.Empty()) {
			iTextDlg dlg(&gApp.ViewMgr(), _T(""), m_message, pHero->Owner()->PlayerId());
			dlg.DoModal();
		}

		if (mr == MR_JOIN_OR_FLEE || mr == MR_JOIN_OR_ATTACK) {
			// Join Army
			iQuestDlg dj(&gApp.ViewMgr(), _T(""), iFormat(gTextMgr[TRID_CREAT_JOIN_MESSAGE], gTextMgr[TRID_CREATURE_PEASANT_F2+Creatures().Type()*3]), pHero->Owner()->PlayerId());
			if (dj.DoModal() == DRC_YES) {
				if (!pHero->Army().AddGroup(m_creatures.Type(),m_creatures.Count())) {
					iDlg_ArmyRoom arDlg(&gApp.ViewMgr(), *pHero, m_creatures);
					arDlg.DoModal();
				}
				gSfxMgr.PlaySound(CSND_DEL_GUARD);
				return true;
			}
		}

		if (mr == MR_JOIN_OR_FLEE || mr == MR_FLEE) {
			iQuestDlg de(&gApp.ViewMgr(), _T(""), iFormat(gTextMgr[TRID_CREAT_ESCAPE_MESSAGE], gTextMgr[TRID_CREATURE_PEASANT_F2+Creatures().Type()*3]),pHero->Owner()->PlayerId());
			if (de.DoModal() == DRC_NO) {
				gSfxMgr.PlaySound(CSND_DEL_GUARD);
				return true;
			}
		}
		// Here is battle
		iBattleInfo bi(pHero, this);
		gGame.BeginBattle(bi);
		return false;

	} else {
		iAI_Player* pOwner = DynamicCast<iAI_Player*>(pHero->Owner());
		check(pOwner);
		// ------------------------------
		if (mr == MR_FLEE || mr == MR_ATTACK || !pOwner->ProcessJoinCreatures(pHero->Army(), m_creatures)) {
			// Here is battle
			iBattleInfo bi(pHero, this);
			gGame.BeginBattle(bi);
			return false;
		}
	}
	return true;
}

void iMapGuard::NewWeek(const iWeekDesc& weekDesk)
{
	if (!m_bNotGrow) m_creatures.Grow();
}


