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
#include "ai.h"
#include "Dlg_NewWeek.h"

/*
 *	Player
 */

iPlayer::iPlayer(PLAYER_TYPE_MASK playerTypeMask, PLAYER_ID playerId, NATION_TYPE nation, const iSize& map_siz)
: m_playerTypeMask(playerTypeMask)
, m_PlayerId(playerId)
, m_Nation(nation)
, m_pCurHero(NULL)
, m_pCurCastle(NULL)
, m_bDirtPassMap(true)
, m_pTavernVisitor(NULL)
{
	m_Minerals.Reset();
	m_FogMap.InitFogMap(map_siz.w,map_siz.h, gGame.Map().DifficultyLevel() != DFC_EASY);
	
	//
	//m_FogMap.OpenWholeMap();
	//
	
	m_passMap.Init(gGame.Map().m_PassMap.GetWidth(),gGame.Map().m_PassMap.GetHeight());
}


iPlayer::~iPlayer()
{
	// Cleanup tavern visitor
	if (m_pTavernVisitor) {
		gGame.ItemMgr().HeroesMgr().Put(m_pTavernVisitor);
		m_pTavernVisitor = NULL;
	}
}

iHero* iPlayer::AddHero(iHero* pHero, bool bUpdateFog)
{
	m_Heroes.Add(pHero);
	if (!m_pCurHero) m_pCurHero = pHero;
	if (bUpdateFog) UpdateFogMap();
	gGame.OnAddHero(pHero);
	return pHero;
}

iHero* iPlayer::RemoveHero(iHero* pHero)
{
	iHLIt nhit = m_Heroes.Remove(pHero);
	if (pHero == m_pCurHero) {
		if (m_Heroes.Count()) {
			SetCurHero(*m_Heroes.First());
		} else {
			m_pCurHero = NULL;
		}
	}
	UpdateFogMap();
	gGame.OnDelHero(pHero);
	return pHero;
}

uint32 iPlayer::CurHeroIdx() const
{
	uint32 idx = 0;
	for (iHeroList::ConstIterator hit = m_Heroes.First(); hit != m_Heroes.End(); ++hit, ++idx) if ( *hit == m_pCurHero) break;
	return idx;
}

iOwnCnst* iPlayer::AddCnst(iOwnCnst* pCnst, bool bUpdateFog)
{
	m_Cnsts.Add(pCnst);
	if (bUpdateFog) UpdateFogMap();
	return pCnst;
}

iCastle* iPlayer::AddCastle(iCastle* pCastle, bool bUpdateFog)
{
	m_Castles.Add(pCastle);
	if (!m_pCurCastle) m_pCurCastle = pCastle;
	if (bUpdateFog) UpdateFogMap();
	gGame.OnAddCastle(pCastle);
	return pCastle;
}

iCastle* iPlayer::RemoveCastle(iCastle* pCastle)
{
	iCtLIt ctit = m_Castles.Remove(pCastle);
	if (pCastle == m_pCurCastle) {
		if (m_Castles.Count()) m_pCurCastle = *m_Castles.First();
		else m_pCurCastle = NULL;
	}
	UpdateFogMap();
	gGame.OnDelCastle(pCastle);
	return pCastle;
}

uint32 iPlayer::CurCastleIdx() const
{
	uint32 idx = 0;
	bool bFound = false;
	for (iCastleList::ConstIterator ctit = m_Castles.First(); ctit != m_Castles.End(); ++ctit, ++idx) if ( *ctit == m_pCurCastle) { bFound = true; break; }
	check(bFound);
	return idx;
}

sint32 iPlayer::GetMineral(MINERAL_TYPE mtype)
{ 
	return m_Minerals.quant[mtype];
}


sint32 iPlayer::AddMineral(MINERAL_TYPE mtype, sint32 count, bool bShowMessage)
{ 
	return m_Minerals.quant[mtype] = iMAX<sint32>(m_Minerals.quant[mtype]+count,0); 
}

void iPlayer::NewDay()
{
	for (iHLIt hit = m_Heroes.First(); hit != m_Heroes.End(); ++hit) (*hit)->NewDay();
	for (iCLIt cit = m_Cnsts.First(); cit != m_Cnsts.End(); ++cit) (*cit)->NewDay();
	for (iCtLIt ctit = m_Castles.First(); ctit != m_Castles.End(); ++ctit) (*ctit)->NewDay();

	// Process player's minerals income
	for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) m_Minerals.quant[xx] += m_furtSkills.Value((FURTHER_SKILLS)(FSK_MIN_GOLD+xx));
	UpdateFogMap();
}

void iPlayer::NewWeek(const iWeekDesc& weekDesk)
{
	// Update tavern visitor
	UpdateTavernVisitor();

	//
	for (iHLIt hit = m_Heroes.First(); hit != m_Heroes.End(); ++hit) (*hit)->NewWeek(weekDesk);
	for (iCLIt cit = m_Cnsts.First(); cit != m_Cnsts.End(); ++cit) (*cit)->NewWeek(weekDesk);
	for (iCtLIt ctit = m_Castles.First(); ctit != m_Castles.End(); ++ctit) (*ctit)->NewWeek(weekDesk);
}

void iPlayer::OnSelect(bool bNewWeek, bool bAfterLoad)
{
	check(PlayerType() == PT_HUMAN);

	// Show new week dialog
	if (bNewWeek) {
		iDlg_NewWeek dlg(&gApp.ViewMgr(), m_PlayerId, gGame.Map().WeekDesc());
		dlg.DoModal();
	}

	// show current day
	uint32 days = gGame.Map().m_CurDay-1;
	gGame.AddMsg(iFormat(_T("#F4B4%s: #F9F9%d  #F4B4%s: #F9F9%d  #F4B4%s: #F9F9%d"),gTextMgr[TRID_MONTH], days/28+1,gTextMgr[TRID_WEEK], (days%28)/7+1,gTextMgr[TRID_DAY],days%7+1));

	// update fog map
	UpdateFogMap();

	// Select current hero and castle
	if (!m_pCurHero && m_Heroes.Count()){
		m_pCurHero = *m_Heroes.First();
	}
	if (m_pCurHero) m_pCurHero->OnSelect();
	else if (m_pCurCastle) m_pCurCastle->OnSelect();

	// Process time events
	if (!bAfterLoad) gGame.Map().ProcessTimeEvents(this);
}

void iPlayer::OnCaptureCastle(iCastle* pNewCastle)
{
	SetCurCastle(pNewCastle);
}

iHero* iPlayer::NextHero()
{
	if (!m_pCurHero && !m_Heroes.Count()) return NULL;
	iHLIt hit(m_pCurHero);
	if (++hit == m_Heroes.End()) hit = m_Heroes.First();
	m_pCurHero = *hit;
	m_bDirtPassMap = true;
	m_pCurHero->OnSelect();
	return m_pCurHero;
}

iHero* iPlayer::PrevHero()
{
	if (!m_pCurHero && !m_Heroes.Count()) return NULL;
	iHLIt hit(m_pCurHero);
	if (hit == m_Heroes.First()) hit = m_Heroes.Last();
	else --hit;
	m_pCurHero = *hit;
	m_pCurHero->OnSelect();
	m_bDirtPassMap = true;
	return m_pCurHero;
}

void iPlayer::PrvSetCurHero(uint16 hid)
{
	check(hid != 0xFFFF);
	iHero* pHero = NULL;
	for (iHLIt hit = m_Heroes.First(); hit != m_Heroes.End(); ++hit) {
		if ( (*hit)->Proto()->m_protoId == hid ) {
			m_pCurHero = *hit;
			return;
		}
	}
	check(0);
}

void iPlayer::SetCurHero(iHero* pHero)
{
	m_pCurHero = pHero;
	m_pCurHero->OnSelect();
	m_bDirtPassMap = true;
}

iCastle* iPlayer::NextCastle()
{
	if (!m_pCurCastle && !m_Castles.Count()) return NULL;
	iCtLIt ctit(m_pCurCastle);
	if (++ctit == m_Castles.End()) ctit = m_Castles.First();
	m_pCurCastle = *ctit;
	m_pCurCastle->OnSelect();
	return m_pCurCastle;
}

iCastle* iPlayer::PrevCastle()
{
	if (!m_pCurCastle && !m_Castles.Count()) return NULL;
	iCtLIt ctit(m_pCurCastle);
	if (ctit == m_Castles.First()) ctit = m_Castles.Last();
	else --ctit;
	m_pCurCastle = *ctit;
	m_pCurCastle->OnSelect();
	return m_pCurCastle;
}


void iPlayer::SetCurCastle(iCastle* pCastle)
{
	m_pCurCastle = pCastle;
	m_pCurCastle->OnSelect();
}

void iPlayer::Process(fix32 t)
{
	// Process heroes
	if (m_pCurHero) {
		m_pCurHero->Process(t);
	}
}

//////////////////////////////////////////////////////////////////////////
bool iPlayer::UpdateVisItems()
{
	iVisItemList tmpItems;
	iGameWorld& map = gGame.Map();

	// Map items
	for (iGameWorld::iMIt mit = map.m_MapItems.First(); mit != map.m_MapItems.End(); ++mit) {
		iPoint p((*mit)->Pos());
		if ( !m_FogMap.IsHidden(p)) {
			tmpItems.AddItem(*mit);
		}
	}

	// Map guards
	for (iGameWorld::iGIt git = map.m_MapGuards.First(); git != map.m_MapGuards.End(); ++git) {
		iPoint p((*git)->Pos());
		if (!m_FogMap.IsHidden(p)) {
			tmpItems.AddItem(*git);
		}
	}

	// Visitables
	for (iGameWorld::iVCIt vcit = map.m_VisCnsts.First(); vcit != map.m_VisCnsts.End(); ++vcit) {
		iPoint p((*vcit)->Pos());
		if ( !m_FogMap.IsInvisible(p) ) {
			tmpItems.AddItem(*vcit);
		}
	}

	// Unaccuped constructions and castles
	{
		for (iGameWorld::iOCIt ocit = map.m_OwnCnsts.First(); ocit != map.m_OwnCnsts.End(); ++ocit) {
			iPoint p((*ocit)->Pos());
			if ( !m_FogMap.IsInvisible(p) ) {
				tmpItems.AddItem(*ocit);
			}
		}
		for (iGameWorld::iCtIt ctit = map.m_Castles.First(); ctit != map.m_Castles.End(); ++ctit) {
			const iCastleT* pProto = (*ctit)->Proto();
			for (uint32 cvid=0; cvid<pProto->CoversCount(); ++cvid) {
				iPoint p((*ctit)->Pos().x + pProto->CoverEntry(cvid).ox, (*ctit)->Pos().y + pProto->CoverEntry(cvid).oy);
				if (!m_FogMap.IsInvisible(p)){
					tmpItems.AddItem(*ctit);
					break;
				}
			}
		}
	}

	// Owned constructions, castles and heroes
	for (iGameWorld::iPLIt plit = map.m_Players.First(); plit != map.m_Players.End(); ++plit) {
		for (iCLIt ocit = (*plit)->m_Cnsts.First(); ocit != (*plit)->m_Cnsts.End(); ++ocit) {
			iPoint p((*ocit)->Pos());
			if ( !m_FogMap.IsInvisible(p) ) {
				tmpItems.AddItem(*ocit);
			}
		}
		for (iCtLIt ctit = (*plit)->m_Castles.First(); ctit !=(*plit)->m_Castles.End(); ++ctit) {
			const iCastleT* pProto = (*ctit)->Proto();
			for (uint32 cvid=0; cvid<pProto->CoversCount(); ++cvid) {
				iPoint p((*ctit)->Pos().x + pProto->CoverEntry(cvid).ox, (*ctit)->Pos().y + pProto->CoverEntry(cvid).oy);
				if (!m_FogMap.IsInvisible(p)){
					tmpItems.AddItem(*ctit);
					break;
				}
			}
		}
		
		//if ( (*plit)->PlayerId() == PlayerId() ) continue;
		for (iHLIt hit = (*plit)->m_Heroes.First(); hit != (*plit)->m_Heroes.End(); ++hit) {
			iPoint p((*hit)->Pos());
			if ( !m_FogMap.IsHidden(p) ) {
				tmpItems.AddItem(*hit);
			}
		}
	}

	// Find new items
	m_newVisItems.Cleanup();
	m_oldVisItems.Cleanup();
	uint32 oCnt = m_visItems.Size();
	uint32 tCnt = tmpItems.Size();
	uint32 oPos=0;
	uint32 tPos=0;
	while (1) {
		if (tPos == tCnt) {
			while (oPos < oCnt) m_oldVisItems.AddItem(m_visItems[oPos++].value);
			break;
		}
		if (oPos == oCnt) {
			while (tPos < tCnt) {
				m_newVisItems.AddItem(tmpItems[tPos].value);
				++tPos;
			}
			break;
		}

		if (tmpItems[tPos].value == m_visItems[oPos].value && tmpItems[tPos].value->Pos() == m_visItems[oPos].value->Pos()) {
			++oPos;
			++tPos;
		} else {
			if (tmpItems[tPos].idx > m_visItems[oPos].idx){
				while (oPos < oCnt && tmpItems[tPos].idx > m_visItems[oPos].idx){
					m_oldVisItems.AddItem(m_visItems[oPos].value);
					++oPos;
				}
				continue;
			}
			m_newVisItems.AddItem(tmpItems[tPos].value);
			++tPos;
		}
	}
	m_visItems.Init(tmpItems);
	
	if (m_newVisItems.Size() || OldVisItems().Size()) m_bDirtPassMap = true;

	return false;
}

//////////////////////////////////////////////////////////////////////////
void iPlayer::ResetVistItemChanges()
{
	m_newVisItems.Cleanup();
	m_oldVisItems.Cleanup();
}

sint32 iPlayer::GetMarketIdx()
{
	sint32 result = 0;
	for (iCastleList::Iterator ctit=m_Castles.First(); ctit!=m_Castles.End(); ++ctit) {
		iCtlCnst* pMarket = (*ctit)->FindCnst(CTLCNST_MARKETPLACE);
		if (pMarket && pMarket->IsBuilt()) result ++;
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
void iPlayer::UpdateFogMap()
{
	m_FogMap.ResetFog();
	if (gSettings.ShowEnemyTurn() && PlayerType() == PT_HUMAN) {
		for (iGameWorld::iPLIt pit = gGame.Map().m_Players.First(); pit != gGame.Map().m_Players.End(); ++pit) {
			for (iHLIt hit=(*pit)->m_Heroes.First(); hit!=(*pit)->m_Heroes.End(); ++hit)
				m_FogMap.DiscoverMap((*hit)->Pos().x,(*hit)->Pos().y,(*hit)->Scouting());
			for (iCLIt cit=(*pit)->m_Cnsts.First(); cit!=(*pit)->m_Cnsts.End(); ++cit)
				m_FogMap.DiscoverMap((*cit)->Pos().x,(*cit)->Pos().y, (*cit)->Proto()->Scouting());
			for (iCtLIt ctit=(*pit)->m_Castles.First(); ctit!=(*pit)->m_Castles.End(); ++ctit) 
				m_FogMap.DiscoverMap((*ctit)->Pos().x,(*ctit)->Pos().y, (*ctit)->Scouting());
		}
	} else {
		for (iHLIt hit=m_Heroes.First(); hit!=m_Heroes.End(); ++hit)
			m_FogMap.DiscoverMap((*hit)->Pos().x,(*hit)->Pos().y,(*hit)->Scouting());
		for (iCLIt cit=m_Cnsts.First(); cit!=m_Cnsts.End(); ++cit)
			m_FogMap.DiscoverMap((*cit)->Pos().x,(*cit)->Pos().y, (*cit)->Proto()->Scouting());
		for (iCtLIt ctit=m_Castles.First(); ctit!=m_Castles.End(); ++ctit)
			m_FogMap.DiscoverMap((*ctit)->Pos().x,(*ctit)->Pos().y, (*ctit)->Scouting());
	}

	UpdateVisItems();
	m_bDirtPassMap = true;

	if (gSettings.ShowEnemyTurn() && PlayerType() != PT_HUMAN && gGame.Map().ActPlayer()) {
		gGame.Map().ActPlayer()->UpdateFogMap();
	}
}
//////////////////////////////////////////////////////////////////////////
void iPlayer::UpdatePassMap()
{
	if (!m_bDirtPassMap) return;
	m_bDirtPassMap=false;
	bool bConstPass = (PlayerType() == PT_COMPUTER);

	check(m_pCurHero);
	sint32 mbonus = m_pCurHero->MoveCostBonus();
	
	m_passMap.CopyFrom(gGame.Map().m_PassMap);
	for (sint32 yy=0; yy<m_passMap.GetHeight(); ++yy){
		for (sint32 xx=0; xx<m_passMap.GetWidth(); ++xx){
			sint32 res;
			if (m_FogMap.IsInvisible(iPoint(xx,yy))) {
				res = -1;
			} else {
				iBaseMapObject* pObj = gGame.Map().m_CoverMap.GetAt(xx,yy);
				if (pObj && pObj->Pos() == iPoint(xx,yy) && (!pObj->Disap() || bConstPass || !m_FogMap.IsHidden(pObj->Pos()))) {
					res = -2;
				} else {
					res = m_passMap.GetAt(xx,yy);
				}
			}
			if (mbonus && res > SURF_MOVE_COST[STYPE_DIRT]) res -= (res-SURF_MOVE_COST[STYPE_DIRT]) * mbonus / 100;
			m_passMap.GetAt(xx,yy) = (sint8)res;
		}
	}
}

bool iPlayer::NeedStopHero() const
{
	return (m_newVisItems.Size() || m_oldVisItems.Size());
}

void iPlayer::UpdateTavernVisitor(iHero* pNewVisitor)
{
	if (m_pTavernVisitor) {
		gGame.ItemMgr().HeroesMgr().Put(m_pTavernVisitor);
		m_pTavernVisitor = NULL;
	}
	if (pNewVisitor) {
		m_pTavernVisitor = pNewVisitor;
	} else {
		if (m_Castles.Count()) {
			uint32 htm = 0;
			// Prepare list of nations according to owned castles
			for (iCtLIt ctlit = m_Castles.First(); ctlit != m_Castles.End(); ++ctlit) htm |= CTL_HEROES[(*ctlit)->Proto()->Type()];
			uint16 selhid = gGame.ItemMgr().HeroesMgr().Select(htm);
			if (selhid == iHeroesMgr::INVALID_HERO_ID) selhid = gGame.ItemMgr().HeroesMgr().Select(HTM_ALL);
			if (selhid != iHeroesMgr::INVALID_HERO_ID) m_pTavernVisitor = gGame.ItemMgr().HeroesMgr().Get(selhid);
		} 
	}
}

iHero* iPlayer::RecruitTavernVisitor(iCastle* pTarget)
{
	iHero* pCurVisitor = m_pTavernVisitor;
	m_pTavernVisitor = NULL;
	m_Minerals -= pCurVisitor->Proto()->m_Cost;
	gGame.Map().AddHero(pCurVisitor, this, pTarget->Pos(), HERO_ORIENT[pTarget->Proto()->Orient()]);
	UpdateTavernVisitor();
	SetCurHero(pCurVisitor);
	return pCurVisitor;
}

uint16 iPlayer::OpenObelisk( ) 
{ 
	uint16 oldOpened = OpenedPuzzles();
	++m_openedObelisks;
	return OpenedPuzzles() - oldOpened;
}

uint8 iPlayer::OpenedPuzzles() const 
{ 
	uint16 ocnt = gGame.Map().ObelisksCount();
	if (!ocnt) return 0;
	return iCLAMP<uint8>(0,TotalPuzzles(),((TotalPuzzles() * m_openedObelisks) / ocnt)); 
}

uint8 iPlayer::PuzzledCard(uint8 idx) const 
{ 
	check(idx<TotalPuzzles()); 
	return m_puzzleCards[idx]; 
}

