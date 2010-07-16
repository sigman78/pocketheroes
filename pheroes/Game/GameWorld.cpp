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

#include "Stdafx.h"
#include "serialize.h"
#include "helpers.h"
#include "Dlg_TimeEvent.h"
#include "lzo.h"

//////////////////////////////////////////////////////////////////////////
const sint32 SCR_DIFMODIF[DFC_COUNT] = { 70, 100, 120, 140, 160 };
const sint32 SCR_SIZMODIF[MSIZ_COUNT] = { 120, 100, 80, 60 };

//#pragma optimization_level(1)
/*
 *	
 */
iGameWorld::iGameWorld()
: m_pCurPlayer(NULL), m_pActPlayer(NULL), m_pVanItem(NULL), m_CurDay(1)
{}

iGameWorld::~iGameWorld()
{ 
	CleanupGameWorld(); 
}

uint32 iGameWorld::CalcGameScore() const
{
	// m_CurDay
	sint32 bscore = 200 - m_CurDay * SCR_SIZMODIF[m_mapSiz] / 100;
	if (bscore < 1) return 0;
	return bscore * SCR_DIFMODIF[m_Difficulty] / 100;
}

iBaseMapObject* iGameWorld::GetCellObj(const iPoint& cell)
{
	if (!IsValidPos(cell.x,cell.y)) return 0;
	return m_CoverMap.GetAt(cell.x,cell.y);
}

iBaseMapObject* iGameWorld::GetCellObj(const iPoint& cell, iStringT& name)
{
	if ( iBaseMapObject* pObj = m_CoverMap.GetAt(cell.x,cell.y)) return pObj;

	uint16 avatar = GetAt(cell.x,cell.y).avatar;
	if ( avatar != 0xFFFF ){
		if ( ( avatar & 0xFF00) == 0xFF00) {
			// path element
			name = gTextMgr[gGame.ItemMgr().m_PathElProts[avatar & 0xFF]->NameKey()];
		} else {
			// avatar title
			name = gTextMgr[gGame.ItemMgr().m_AvatarMgr[avatar].Name()];
		}
	} else {
		// surface type
		iCell c = GetAt(cell.x,cell.y);
		sint32 upSurf = iMAX(iMAX(iMAX(c.SurfNode(0),c.SurfNode(1)),c.SurfNode(2)),c.SurfNode(3));
		sint32 btSurf = iMIN(iMIN(iMIN(c.SurfNode(0),c.SurfNode(1)),c.SurfNode(2)),c.SurfNode(3));
		if (btSurf == STYPE_WATER && btSurf != upSurf) name = gTextMgr[TRID_SURF_COAST];
		else name = gTextMgr[TRID_SURF_WATER+(sint16)upSurf];
#ifdef _DEBUG
		name.Addf(_T(" (%d,%d)"),cell.x,cell.y);
#endif
	}

	return NULL;
}
//
void iGameWorld::StartGame(PLAYER_ID curPlayer, bool bAfterLoad)
{
	check(!m_pCurPlayer);

	// Set current player (first player must be PT_HUMAN type)
	iPlayer* pCurPlayer = (curPlayer==PID_NEUTRAL)?(*m_Players.First()):FindPlayer(curPlayer);
	check(pCurPlayer && pCurPlayer->PlayerType() == PT_HUMAN);
	m_pCurPlayer = pCurPlayer;

	// 

	if (m_gameMode == iMapInfo::GM_SPLAYER) m_pActPlayer = m_pCurPlayer;
	SetNewActor(m_pCurPlayer, false);

	gGame.OnPlayerChanged(m_pCurPlayer, true);
	m_pCurPlayer->OnSelect(false, bAfterLoad);
}

//
iPlayer* iGameWorld::AddPlayer(PLAYER_TYPE_MASK playerTypeMask, PLAYER_ID pid, NATION_TYPE nation, PLAYER_TYPE pt)
{
	iPlayer *pPlayer; 
	if (pt == PT_HUMAN) pPlayer = new iPlayer(playerTypeMask, pid,nation,iSize(m_Width,m_Height));
	else if (pt == PT_COMPUTER) pPlayer = new iAI_Player(playerTypeMask, pid,nation,iSize(m_Width,m_Height));
	else return NULL;
	m_Players.Add(pPlayer);
	return pPlayer;
}

void iGameWorld::RemovePlayer(iPlayer* pPlayer)
{
	// Release all constructions and castles
	iPlayer::iCLIt cit = pPlayer->m_Cnsts.First();
	while (cit != pPlayer->m_Cnsts.End()){
		iOwnCnst* pCnst = *cit;
		cit = pPlayer->m_Cnsts.Remove(cit);
		pCnst->SetOwner(PID_NEUTRAL);
		m_OwnCnsts.Add(pCnst);
	}

	iPlayer::iCtLIt ctit = pPlayer->CastleFirst();
	while (ctit != pPlayer->CastleEnd()){
		iCastle* pCastle= *ctit;
		ctit = pPlayer->RemoveCastle(*ctit);
		pCastle->SetOwner(PID_NEUTRAL);
		m_Castles.Add(pCastle);
	}

	// are we removing active player?
	bool bAct = (m_pActPlayer == pPlayer);
	// are we removing current player?
	bool bCur = (m_pCurPlayer == pPlayer);
	// are we removing human player?
	bool bHum = (pPlayer->PlayerType() == PT_HUMAN);

	// Player 'x' Vanquished
	gGame.OnPlayerVanquished(pPlayer->PlayerId());

	// Find Next Actor
	iPlayer* pNextActor = NULL;
	if (bAct) {
		// Find another human player
		iPlayer* pNewActPlayer = NULL;
		for (iPLIt pit = m_Players.First(); pit != m_Players.End(); ++pit) {
			if ( *pit != pPlayer && (*pit)->PlayerType() == PT_HUMAN) {
				pNextActor = *pit;
				break;
			}
		}
		if (!pNextActor) {
			// Show Defeat dlg and exit game
			gGame.OnDefeat(true);
			return;
		} else {
			// Show Defeat dlg 
			gGame.OnDefeat(false);
		}
	}

	// SiGMan: I moved check here from (*) to declare victory 
	// before new day comes #0000245
	// Check for victory condition
	if (m_Players.Count() <= 2) {
		//check(!bAct);
		gGame.OnVictory();
	}


	// If Current player vanqushed, then select next player
	if (bCur) {
		// Selects next player in the list
		// if iteration is over
		//		- choose first player
		//		- increment day and month
		// set new actor if human player
		//	- update m_pActPlayer
		//	- notify OnActorChanged
		// notify OnPlayerChanged
		// notify player OnSelect
		NextPlayer();
	}

	// Select new actor if not already selected
	if (bAct && pPlayer == m_pActPlayer) {
		check(pNextActor);
		SetNewActor(pNextActor, false);
	}

	// (*)

	// Delete player
	iPLIt ppli(pPlayer);
	m_Players.Delete(ppli);

}

void iGameWorld::SetHeroPos(iHero* pHero, const iPoint& pos)
{
	// Check for map construction entrance
	if (iBaseMapObject* pMapObj = m_CoverMap.GetAt(pos.x,pos.y)){
		// it must be map construction
		if (DynamicCast<iCastle*>(pMapObj) || DynamicCast<iVisCnst*>(pMapObj) || DynamicCast<iOwnCnst*>(pMapObj)){
			pHero->SetLocation((iMapCnst*)pMapObj);
		} else {
			check(0);
		}
	} else {
		pHero->SetLocation(NULL);
	}

	m_CoverMap.GetAt(pos.x,pos.y) = pHero;

	// Hide the hero
	if (pHero->Artifacts().HasArtType(ARTT_CURSWORD)) {
		for (iPLIt pit = m_Players.First(); pit != m_Players.End(); ++pit) {
			if ( (*pit)->PlayerId() != pHero->Owner()->PlayerId() ){
				(*pit)->FogMap().DiscoverMap(pos.x, pos.y, 2, true);
			}
		}
	}
}

iHero* iGameWorld::AddHero(iHero* pHero, iPlayer* pOwner, const iPoint& pos, sint8 angle, bool bUpdateFog)
{
	pHero->Init(pOwner, pos, angle);
	pHero->Owner()->AddHero(pHero, bUpdateFog);
	SetHeroPos(pHero, pos);
	return pHero;
}

void iGameWorld::RemoveHero(iHero* pHero, bool bResetArmy)
{
	iPlayer* pPlayer = pHero->Owner();
	check(DynamicCast<iHero*>(m_CoverMap.GetAt(pHero->Pos().x,pHero->Pos().y)));
	if (iMapCnst* pCnst = pHero->GetLocation()) {
		m_CoverMap.GetAt(pHero->Pos().x,pHero->Pos().y) = pHero->GetLocation();
	} else {
		m_CoverMap.GetAt(pHero->Pos().x,pHero->Pos().y) = NULL;
	}
	pPlayer->RemoveHero(pHero);
	pHero->Deinit(bResetArmy);
	gGame.ItemMgr().HeroesMgr().Put(pHero);
	m_pCurPlayer->UpdateVisItems();
	// Fake 
	if (!pPlayer->DefeatState()) RemovePlayer(pPlayer);
}

void iGameWorld::MoveHero(const iPoint& oldPos, const iPoint& newPos)
{
	iHero* pHero = DynamicCast<iHero*>(m_CoverMap.GetAt(oldPos.x,oldPos.y));
	check(pHero);

	// Leave construction if required
	if (iMapCnst* pMapCnst = pHero->GetLocation()) {
		m_CoverMap.GetAt(oldPos.x,oldPos.y) = pMapCnst;
	} else {
		m_CoverMap.GetAt(oldPos.x,oldPos.y) = NULL;
	}

	SetHeroPos(pHero, newPos);
}


iVisCnst* iGameWorld::AddVisCnst(iVisCnst* pCnst)
{
	m_VisCnsts.Add(pCnst);
	for (uint32 xx=0; xx<pCnst->Proto()->CoversCount(); ++xx) {
		sint32 px = pCnst->Pos().x+pCnst->Proto()->CoverEntry(xx).ox;
		sint32 py = pCnst->Pos().y+pCnst->Proto()->CoverEntry(xx).oy;
		if (m_CoverMap.IsValidPos(px,py)) {
			m_CoverMap.GetAt(px,py) = pCnst;
		} else {
			check(0 == "Construction's cell is out of map!");
		}
	}
	return pCnst;
}

iOwnCnst* iGameWorld::AddOwnCnst(iOwnCnst* pCnst, bool bUpdateFog)
{
	if (pCnst->Owner() != PID_NEUTRAL) {
		FindPlayer(pCnst->Owner())->AddCnst(pCnst, bUpdateFog);
	} else {
		m_OwnCnsts.Add(pCnst);
	}

	for (uint32 xx=0; xx<pCnst->Proto()->CoversCount(); ++xx){
		sint32 px = pCnst->Pos().x+pCnst->Proto()->CoverEntry(xx).ox;
		sint32 py = pCnst->Pos().y+pCnst->Proto()->CoverEntry(xx).oy;
		if (m_CoverMap.IsValidPos(px, py)) {
			m_CoverMap.GetAt(px,py) = pCnst;
		} else {
			//check(0 == "Construction's cell is out of map!");
		}
	}
	return pCnst;
}

iCastle* iGameWorld::AddCastle(iCastle* pCastle, bool bUpdateFog)
{
	if (pCastle->Owner() != PID_NEUTRAL) {
		FindPlayer(pCastle->Owner())->AddCastle(pCastle, bUpdateFog);
	} else {
		m_Castles.Add(pCastle);
	}

	for (uint32 xx=0; xx<pCastle->Proto()->CoversCount(); ++xx){
		sint32 ox = pCastle->Pos().x + pCastle->Proto()->CoverEntry(xx).ox;
		sint32 oy = pCastle->Pos().y + pCastle->Proto()->CoverEntry(xx).oy;
		if (m_CoverMap.IsValidPos(ox,oy)) {
			m_CoverMap.GetAt(ox,oy) = pCastle;
		} else {
			check(0 == "Castle's cell is out of map!");
		}
	}

	return pCastle;
}

iMapItem* iGameWorld::AddMapItem(iMapItem* pMapItem)
{
	m_MapItems.Add(pMapItem);
	m_CoverMap.GetAt(pMapItem->Pos().x,pMapItem->Pos().y) = pMapItem;
	return pMapItem;
}

void iGameWorld::RemoveMapItem(iMapItem* pMapItem)
{
	m_CoverMap.GetAt(pMapItem->Pos().x,pMapItem->Pos().y) = NULL;
	m_MapItems.Remove(iMIt(pMapItem));
	delete pMapItem;
}

iMapEvent* iGameWorld::AddMapEvent(iMapEvent* pMapEvent)
{
	m_MapEvents.Add(pMapEvent);
	return pMapEvent;
}

void iGameWorld::RemoveMapEvent(iMapEvent* pMapEvent)
{
	m_MapEvents.Remove(iEIt(pMapEvent));
	delete pMapEvent;
}

iMapEvent* iGameWorld::FindMapEvent(const iPoint& cell, PLAYER_ID pid)
{
	check(pid != PID_NEUTRAL);
	for (iMapEventList::Iterator meit = m_MapEvents.First(); meit != m_MapEvents.End(); ++meit) {
		if ((*meit)->Pos() == cell) {
			if ( ((*meit)->PlayerMask() & (1<<pid)) != 0 ) return *meit;
			else return NULL;
		}
	}
	return NULL;
}

iMapGuard* iGameWorld::AddMapGuard(iMapGuard* pMapGuard)
{
	m_MapGuards.Add(pMapGuard);
	m_CoverMap.GetAt(pMapGuard->Pos().x,pMapGuard->Pos().y) = pMapGuard;
	return pMapGuard;
}

void iGameWorld::RemoveMapGuard(iMapGuard* pMapGuard)
{
	m_CoverMap.GetAt(pMapGuard->Pos().x,pMapGuard->Pos().y) = NULL;
	m_MapGuards.Remove(iGIt(pMapGuard));
	delete pMapGuard;
}

void iGameWorld::CaptureCastle(iHero* pHero, iCastle* pCastle)
{
}

bool iGameWorld::CanDig(const iPoint& cell) const
{
	iCell c = GetAt(cell.x,cell.y);
	// Not passable cell
	if (c.SurfNode(0) == STYPE_WATER || c.SurfNode(1) == STYPE_WATER || c.SurfNode(2) == STYPE_WATER || c.SurfNode(3) == STYPE_WATER || m_PassMap.GetAt(cell.x, cell.y) <= 0 || GetAt(cell.x,cell.y).avatar != 0xFFFF) return false;
	iBaseMapObject* pObj = m_CoverMap.GetAt(cell.x,cell.y);
	// No object
	if (!pObj) return true;
	if (pObj->Disap()) {
		if (iHero* pHero = DynamicCast<iHero*>(pObj)) return pHero->GetLocation() == NULL;
		return true;
	}

	return  false;

	/*
	iCell c = GetAt(cell.x,cell.y);
	if (c.SurfNode(0) == STYPE_WATER || c.SurfNode(1) == STYPE_WATER || c.SurfNode(2) == STYPE_WATER || c.SurfNode(3) == STYPE_WATER) return false;
	return m_PassMap.GetAt(cell.x, cell.y) > 0 && (!m_CoverMap.GetAt(cell.x,cell.y) || (DynamicCast<iHero*>(m_CoverMap.GetAt(cell.x,cell.y)) && !DynamicCast<iHero*>(m_CoverMap.GetAt(cell.x,cell.y))->GetLocation())) && GetAt(cell.x,cell.y).avatar == 0xFFFF;
	*/
}

const SpriteId hole_sprites[STYPE_COUNT] = {
	0xFFFF,			// water
	PDGG_HOLES + 0,	// sand (beach)
	PDGG_HOLES + 1,	// dirt
	PDGG_HOLES + 1,	// grass
	PDGG_HOLES + 1,	// swamp
	PDGG_HOLES + 2,	// lava
	PDGG_HOLES + 3,	// wasteland
	PDGG_HOLES + 4,	// desert
	PDGG_HOLES + 1,	// snow
	PDGG_HOLES + 4,	// ndesert
	PDGG_HOLES + 1,	// pavement
	PDGG_HOLES + 3	// nwasteland
};

bool iGameWorld::Dig(const iPoint& cell)
{
	check(CanDig(cell));
	iCell c = GetAt(cell.x,cell.y);
	sint32 upSurf = iMAX(iMAX(iMAX(c.SurfNode(0),c.SurfNode(1)),c.SurfNode(2)),c.SurfNode(3));
	check(upSurf != STYPE_WATER);
	uint16 aid = gGame.ItemMgr().m_AvatarMgr.FindAvatarBySprite(hole_sprites[upSurf]);
	check(aid != 0xFFFF);
	GetAt(cell.x,cell.y).avatar = aid;
	if (m_posUltimate == cell) {
		m_statUltimate = true;
		return true;
	}
	return false;
}

bool iGameWorld::InitUltimateArt()
{
	if (m_statUltimate) return 0;
	if (m_radUltimate != 0 || m_posUltimate == iPoint(-1,-1)) {
		if (m_radUltimate == 0) m_radUltimate = 255;
		iRect rcMap(8,8, m_Width-15, m_Height-15);
		if (m_posUltimate != iPoint(-1,-1)) {
			iRect rcUlt(m_posUltimate.x, m_posUltimate.y, m_radUltimate*2+1, m_radUltimate*2+1);
			iClipper::IntersectRect(rcMap, rcMap, rcUlt);
		}
		do {
			m_posUltimate.x = rcMap.x + m_rand.Rand(rcMap.w);
			m_posUltimate.y = rcMap.y + m_rand.Rand(rcMap.h);
		} while (!CanDig(m_posUltimate));
	}
	check(IsValidPos(m_posUltimate.x, m_posUltimate.y) && (m_statUltimate  || CanDig(m_posUltimate)));
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool iGameWorld::InitMap(sint32 w, sint32 h)
{
	CleanupGameWorld();
	m_PassMap.Init(w,h);
	m_CoverMap.Init(w,h);
	m_CoverMap.FillMem(0);
	return Init(w,h);
}

void iGameWorld::CleanupGameWorld()
{
	Cleanup();
	m_PassMap.Cleanup();
	m_CoverMap.Cleanup();
	m_VisCnsts.DeleteAll();
	m_OwnCnsts.DeleteAll();
	m_Castles.DeleteAll();
	m_MapItems.DeleteAll();
	m_MapGuards.DeleteAll();
	m_MapEvents.DeleteAll();
	m_Players.DeleteAll();
	m_teleports.RemoveAll();

	m_pCurPlayer = m_pActPlayer = NULL;
}

void iGameWorld::Process(fix32 t)
{
	if (CurPlayer()) CurPlayer()->Process(t);

	//
	if (m_pVanItem) {
		if (m_pVanItem->Vanished()) {
			delete m_pVanItem->DettachContent();
			delete m_pVanItem;
			m_pVanItem = NULL;
		} else {
			m_pVanItem->Process(t);
		}
	}
}

bool iGameWorld::LoadFromFile(const iMapInfo& mapInfo, iFileI* pFile)
{
	// Reinit random generator
	uint32 seed = mapInfo.m_rseed;
	if (!seed) seed = GetTickCount();
	m_rand.SetNewSeed(seed);

	// Reinit Item manager
	gGame.ItemMgr().OnGameStart();

	// Read data to buffer
	uint32 fileSize = pFile->GetSize();
	iDynamicBuffer tmpbuff( fileSize );
//	tmpbuff.SetSize( fileSize );
//	pFile->Read(tmpbuff.GetData(),fileSize );
	tmpbuff.SetSize( pFile->Read(tmpbuff.GetData(),fileSize ) );

#if 0
	FILE* ff1 = fopen( "dumpmap.lzo", "wb" );
	fwrite( tmpbuff.GetData(), tmpbuff.GetSize(), 1, ff1 );
	fclose(ff1);
#endif

	// Decompress
	iDynamicBuffer dbuff;
	LZO::Decompress((const unsigned char*)tmpbuff.GetData(), tmpbuff.GetSize(), dbuff);
	tmpbuff.Clean();
	
#if 0
	FILE* ff = fopen( "dumpmap.bin", "wb" );
	fwrite( dbuff.GetData(), dbuff.GetSize(), 1, ff );
	fclose(ff);
	ff = fopen( "dumpmap.txt", "wt" );
	fprintf( ff, "%d %d %d\n", mapInfo.m_Size, mapInfo.m_gameMode, mapInfo.m_gameMode );
	fprintf( ff, "%d\n", dbuff.GetSize() );
	fclose(ff);
#endif

#if 0
	FILE* ff = fopen( "dumpmap.bin", "wb" );
	fwrite( dbuff.GetData(), dbuff.GetSize(), 1, ff );
	fclose(ff);
	ff = fopen( "dumpmap.txt", "wt" );
	fprintf( ff, "%d %d %d\n", mapInfo.m_Size, mapInfo.m_gameMode, mapInfo.m_gameMode );
	fprintf( ff, "%d\n", dbuff.GetSize() );
	fclose(ff);
#endif

	// Map size
	m_mapSiz = mapInfo.m_Size;

	// Game mode
	m_gameMode = mapInfo.m_gameMode;

	// Difficulty
	m_Difficulty = mapInfo.m_Difficulty;

	// Name and description
	m_Name = mapInfo.m_Name;
	m_Desc = mapInfo.m_Description;

	// Version and Author
	m_Version = mapInfo.m_Version;
	m_Author = mapInfo.m_Author;

	// Current date
	m_CurDay = mapInfo.m_curDay;

	// Current week descriptor (always initialize as Normal)
	m_weekDesc.Init();

	// Time Events
	m_timeEventMgr.DeleteAll();
	uint16 cnt; 
	dbuff.Read(cnt);
	iTimeEvent te;
	for (uint16 xx=0; xx<cnt; ++xx) {
		::Unserialize(dbuff, te.m_text);
		dbuff.Read(te.m_playerMask);
		dbuff.Read(te.m_fTime);
		sint32 repTime;
		dbuff.Read(repTime);
		te.m_repTime = (iTimeEvent::EventFreq)repTime;
		::Unserialize(dbuff, te.m_minerals);
		m_timeEventMgr.AddEvent(te);
	}

	// Surface map dump
	InitMap(mapInfo.m_metrics.w, mapInfo.m_metrics.h);
	
	// Surface and avatars
	dbuff.Read(m_Buff,mapInfo.m_metrics.w*mapInfo.m_metrics.h*sizeof(iCell));

	// Players
	for (uint32 xx=0; xx<mapInfo.m_Players.GetSize(); ++xx){
		NATION_TYPE nation;
		if (mapInfo.m_Players[xx].m_Nation==CTLT_RANDOM) {
			nation = (NATION_TYPE)(NATION_HIGHMEN + Rand(NATION_COUNT-NATION_HIGHMEN));
		} else {
			nation = (NATION_TYPE)(mapInfo.m_Players[xx].m_Nation+NATION_HIGHMEN);
		}

		iPlayer* pPlayer = AddPlayer(mapInfo.m_Players[xx].m_TypeMask, mapInfo.m_Players[xx].m_Id, nation, mapInfo.m_Players[xx].m_Type);
		pPlayer->SetKeys(mapInfo.m_Players[xx].m_keys);
		if (mapInfo.m_bNewGame) {
			pPlayer->Minerals() = INITIAL_RESOURCES[(pPlayer->PlayerType() == PT_HUMAN)?0:1][mapInfo.m_Difficulty];
		} else {
			pPlayer->Minerals() = mapInfo.m_Players[xx].m_Minerals;
		}
	}

	// Fog maps and puzzle cards
	for (uint32 xx=0; xx<mapInfo.m_Players.GetSize(); ++xx) {
		iPlayer* pPlayer = *(m_Players.First() + xx);
		// Puzzles
		dbuff.Read(pPlayer->m_openedObelisks);
		if (pPlayer->m_openedObelisks != 0xFFFF) {
			dbuff.Read(pPlayer->m_puzzleCards, 45);
		} else {
			// init default puzzle cards
			pPlayer->m_openedObelisks = 0;
			iSimpleArray<uint8> cards(45);
			for (uint32 cid=0; cid<45; ++cid) cards.Add(cid);
			cards.RemoveAt(22);
			pPlayer->m_puzzleCards[44] = 22;
			for (uint8 pp=0; pp < 44; ++pp) {
				uint8 rval = (uint8)m_rand.Rand(cards.GetSize());
				pPlayer->m_puzzleCards[pp] = cards[rval];
				cards.RemoveAt(rval);
			}
		}

		// Fog map
		uint32 fms; dbuff.Read(fms);
		if (fms) {
			dbuff.Read(pPlayer->FogMap().GetFogMapPtr(), fms);
		}
	}

	// Visitables
	uint16 vcCount; dbuff.Read(vcCount);
	for (uint32 vcId=0; vcId<vcCount; ++vcId){
		// Proto Id
		uint16 proto;	dbuff.Read(proto);
		// Position
		iPoint pos;		Unserialize(dbuff, pos);
		// Create object
		iVisCnst* pVisCnst = gGame.ItemMgr().m_VisProts[proto]->CreateObject(pos);
		// Unserialize
		pVisCnst->Unserialize(dbuff);
		// Add object
		AddVisCnst(pVisCnst);
	}

	// Ownerables
	uint16 ocCount; dbuff.Read(ocCount);
	for (uint32 ocId=0; ocId<ocCount; ++ocId){
		// Proto Id
		uint16 proto;	dbuff.Read(proto);
		// Owner Id
		sint8 owner;	dbuff.Read(owner);
		// Position
		iPoint pos;		Unserialize(dbuff, pos);
		// Create object
		iOwnCnst* pOwnCnst = gGame.ItemMgr().m_OwnProts[proto]->CreateObject(pos);
		pOwnCnst->SetOwner((PLAYER_ID)owner);
		// Guard
		Unserialize(dbuff, pOwnCnst->Guard());
		InitArmy(pOwnCnst->Guard());
		// Add object
		AddOwnCnst(pOwnCnst, false);
	}

	// Castles
	uint16 cCount; dbuff.Read(cCount);
	iSimpleArray<iCastle*> ctlList;
	for (uint32 cId=0; cId<cCount; ++cId){
		// Proto Id
		uint16 proto;	dbuff.Read(proto);
		// Custom name
		iStringT cname; Unserialize(dbuff, cname);
		// Owner
		sint8 owner;	dbuff.Read(owner);
		// Position
		iPoint pos;		Unserialize(dbuff, pos);
		// Flags
		uint8 flags; dbuff.Read(flags); 
		// if castle type is random (only for new game)
		if (gGame.ItemMgr().IsRandomCtl(proto)) {
			// main castle
			if (flags & 0x01) {
				check(owner != PID_NEUTRAL);
				iPlayer* pPlayer = FindPlayer((PLAYER_ID)owner);
				check(pPlayer);
				// change proto according to owner's nation (same configuration, but different nation)
				proto = gGame.ItemMgr().AlterCtlNation(proto, (CTL_TYPE)(pPlayer->Nation()-NATION_HIGHMEN));
			} else {
				// change proto to random nation (same configuration, but different nation)
				proto = gGame.ItemMgr().AlterCtlNation(proto, (CTL_TYPE)Rand(CTLT_COUNT-1));
			}
		}
		// Create object
		iCastle* pCastle = new iCastle(gGame.ItemMgr().m_CastleProts[proto],pos, cname, (flags & 0x2) != 0);
		// Garrison
		Unserialize(dbuff, pCastle->Garrison());
		// Can build
		uint8 canBuild; dbuff.Read(canBuild); pCastle->SetBuilt(canBuild>0);
		// Constructions
		pCastle->m_Cnsts.Unserialize(dbuff);
		if (owner != PID_NEUTRAL){
			// Set owner
			pCastle->SetOwner((PLAYER_ID)owner);
			// Update constructions
			pCastle->UpdateCnsts();
		} 
		// Init and add an object
		ctlList.Add(pCastle);
		AddCastle(pCastle, false);
	}

	// Obelisks count
	dbuff.Read(m_obelisksCount);

	// Ultimate artifact position and radius
	::Unserialize(dbuff, m_posUltimate);
	dbuff.Read(m_radUltimate);
	dbuff.Read(m_statUltimate);

	// Map items
	uint16 mCount; dbuff.Read(mCount);
	while(mCount--) {
		// Map item type
		uint8 type;		dbuff.Read(type);
		// Position
		iPoint pos;		Unserialize(dbuff, pos);
		// Guard
		iArmy guard;	Unserialize(dbuff, guard);
		// Message
		iStringT msg;	Unserialize(dbuff, msg);
		
		// Read item speciffic data and create an object
		iMapItem* pItem = NULL;
		if (type == MAPITEM_MINERAL) {
			sint8 mtype;
			sint32 mcount;
			dbuff.Read(mtype);
			dbuff.Read(mcount);
			pItem = new iMapItem_Mineral(pos,(MINERAL_TYPE)mtype,mcount);
		} else if (type == MAPITEM_MANACRYSTAL) {
			sint32 mcount;
			dbuff.Read(mcount);
			pItem = new iMapItem_ManaCrystal(pos,mcount);
		} else if (type == MAPITEM_CAMPFIRE) {
			sint32 goldCount, addMinCount;
			sint8 addMinType;
			dbuff.Read(goldCount);
			dbuff.Read(addMinType);
			dbuff.Read(addMinCount);
			pItem = new iMapItem_CampFire(pos, goldCount, (MINERAL_TYPE)addMinType, addMinCount);
		} else if (type == MAPITEM_CHEST) {
			sint32 count;
			dbuff.Read(count);
			pItem = new iMapItem_Chest(pos, count);
		} else if (type == MAPITEM_LAMP) {
			sint32 count;
			dbuff.Read(count);
			pItem = new iMapItem_Lamp(pos, count);
		} else if (type == MAPITEM_ARTIFACT) {
			uint16 aidx;
			dbuff.Read(aidx);
			if (aidx >= ARTT_RAND) {
				// random artifact
				ART_LEVEL_TYPE alt = (ART_LEVEL_TYPE)(aidx-ARTT_RAND);
				aidx = gGame.ItemMgr().m_artMgr.SelectRandomArtifact(alt);
			}
			pItem = new iMapItem_Artifact(pos,aidx);
		} else if (type == MAPITEM_SPELLSCROLL) {
			uint16 spellIdx;
			dbuff.Read(spellIdx);
			pItem = new iMapItem_SpellScroll(pos,spellIdx);
		} else if (type == MAPITEM_KEYGUARD) {
			uint8 key; dbuff.Read(key);
			pItem = new iMapItem_KeyGuard(pos,key);
		} else {
			check(0);
		}

		// Init guard
		pItem->Guard() = guard;
		InitArmy(pItem->Guard());
		// Init message
		pItem->Message() = msg;
		// Add object
		AddMapItem(pItem);
	}

	// Map events
	uint16 meCount; dbuff.Read(meCount);
	while(meCount--) {
		iPoint pos;		Unserialize(dbuff, pos);
		iMapEvent* pEvent = new iMapEvent(pos);
		Unserialize(dbuff, pEvent->Guard());
		InitArmy(pEvent->Guard());
		Unserialize(dbuff, pEvent->RewardMessage());
		Unserialize(dbuff, pEvent->AttackMessage());
		dbuff.Read(pEvent->PlayerMask());
		Unserialize(dbuff, pEvent->Rewards());
		AddMapEvent(pEvent);
	}

	// Map guards
	uint16 gCount; dbuff.Read(gCount);
	while(gCount--) {
		// Guard group
		iCreatGroup cg;	Unserialize(dbuff, cg);
		// Join value
		sint8 rjoinVal; dbuff.Read(rjoinVal);
		sint32 joinVal = rjoinVal;
		if (joinVal == RAND_VAL)  joinVal = gGame.Map().Rand(100);
		// Disposition
		uint8 disp;		dbuff.Read(disp);
		// Grow flag
		uint8 bNotGrow; dbuff.Read(bNotGrow);
		// Position
		iPoint pos;		Unserialize(dbuff, pos);
		// Create an object
		iMapGuard* pGuard = new iMapGuard(pos, cg.Type(), cg.Count(), joinVal, (GUARD_DISPOS)disp,bNotGrow>0);
		// Message
		iStringT msg; Unserialize(dbuff, pGuard->Message());
		// Add object
		AddMapGuard(pGuard);
	}

	// Heroes
	uint16 hCount; dbuff.Read(hCount);
	for (uint32 xx=0; xx<hCount; ++xx){
		// Proto Id
		uint16 proto; dbuff.Read(proto);
		// Get object from hero manager
		iHero* pHero = gGame.ItemMgr().HeroesMgr().Get(proto);
		check(pHero);
		// Unserialize
		pHero->Unserialize(dbuff, true);
		// Setup position
		SetHeroPos(pHero, pHero->Pos());
	}

	// Hero manager
	uint16 hmCount; dbuff.Read(hmCount);
	for (uint32 xx=0; xx<hmCount; ++xx){
		// Proto Id
		uint16 proto; dbuff.Read(proto);
		// Get object from hero manager
		iHero* pHero = gGame.ItemMgr().HeroesMgr().Get(proto);
		check(pHero);
		// Unserialize
		pHero->Unserialize(dbuff, false);
		// Put hero back to hero manager
		gGame.ItemMgr().HeroesMgr().Put(pHero);
	}

	// Init castles
	for (uint32 xx=0; xx<ctlList.GetSize(); ++xx) ctlList[xx]->Init();

	// Setup Current hero for all human players and update tavern visitors
	uint32 pidx = 0;
	for (iPLIt plit = m_Players.First(); plit != m_Players.End(); ++plit, ++pidx) {
		iPlayer* pPlayer = *plit;
		pPlayer->UpdateTavernVisitor();
		if (pPlayer->PlayerType() == PT_HUMAN && mapInfo.m_Players[pidx].m_curHeroId != 0xFFFF) {
			pPlayer->PrvSetCurHero(mapInfo.m_Players[pidx].m_curHeroId);
		}
	}

	// Init teleports
	for (iVCIt vcit = m_VisCnsts.First(); vcit != m_VisCnsts.End(); ++vcit) {
		if ((*vcit)->Proto()->Type() == VCNST_TELEPORT) {
			iVisCnst_Teleport* pTeleport = (iVisCnst_Teleport*)(*vcit);
			iPoint pos = pTeleport->Pos();
			uint8 tidx = pTeleport->Index();
			// Try to find the pair
			for (uint32 nn=0; nn<m_teleports.GetSize(); ++nn) {
				uint8 ntidx = m_teleports[nn]->Index();
				if (ntidx == tidx) {
					m_teleports[nn]->SetTarget(m_teleports.GetSize());
					pTeleport->SetTarget(nn);
					break;
				}
			}
			// Add teleport into list
			m_teleports.Add(pTeleport);
		}
	}

	return true;
}

bool iGameWorld::SaveToFile(iFileI* pFile)
{
	iDynamicBuffer dbuff;
	// Fourcc
	dbuff.Write(GMAP_FILE_HDR_KEY);
	// Map file version
	dbuff.Write(GMAP_FILE_VERSION);
	// Save timestamp
	SYSTEMTIME st;
	FILETIME ft;
	GetLocalTime(&st);
	SystemTimeToFileTime(&st, &ft);
	uint32 ut = (ft.dwLowDateTime + ((LONGLONG)(ft.dwHighDateTime)<<32)) / 600000000;
	dbuff.Write(ut);
	// Random seed
	dbuff.Write(m_rand.GetSeed());
	// Map size
	dbuff.Write((uint8)m_mapSiz);
	// Map name and description (string of text)
	Serialize(dbuff, m_Name);
	Serialize(dbuff, m_Desc);
	// Map version and author (string of text)
	Serialize(dbuff, m_Version);
	Serialize(dbuff, m_Author);
	// Current date
	dbuff.Write(m_CurDay);
	// Game mode 
	dbuff.Write((uint16)m_gameMode);
	// Difficulty
	dbuff.Write((sint8)m_Difficulty);
	// Player list
	dbuff.Write((uint16)m_Players.Count());
	iPlayer* players[PID_COUNT] = {NULL};
	for (iPlayerList::Iterator pit = m_Players.First(); pit != m_Players.End(); ++pit) players[(*pit)->PlayerId()] = *pit;
	for (uint32 pid = 0; pid<PID_COUNT; ++pid) {
		const iPlayer* pPlayer = players[pid];
		if (!pPlayer) continue;
		// Player Id
		dbuff.Write((sint8)pPlayer->PlayerId());
		//Nation type
		dbuff.Write((uint8)(pPlayer->Nation()-NATION_HIGHMEN));
		// Player Type Mask
		dbuff.Write((uint8)pPlayer->PlayerTypeMask());
		// Player Type
		dbuff.Write((uint8)pPlayer->PlayerType());
		// Player resources
		Serialize(dbuff, pPlayer->Minerals());
		// Current Hero Id
		if (pPlayer->CurHero()) {
			dbuff.Write(pPlayer->CurHero()->Proto()->m_protoId);
		} else {
			dbuff.Write((uint16)0xFFFF);
		}
		// Current Castle idx
		dbuff.Write((uint16)0xFFFF);
		// Keys
		dbuff.Write(pPlayer->GetKeys());
	}
	// Current player
	dbuff.Write((uint16)m_pCurPlayer->PlayerId());
	// Map width and height
	dbuff.Write((uint16)m_Width);
	dbuff.Write((uint16)m_Height);

	//////////////////////////////////////////////////////////////////////////
	pFile->Write(dbuff.GetData(), dbuff.GetSize());
	dbuff.Reset();
	//////////////////////////////////////////////////////////////////////////

	// Time events
	uint16 cecnt = m_timeEventMgr.EventsCount();
	dbuff.Write(cecnt);
	for (uint32 xx=0; xx<cecnt; ++xx){
		const iTimeEvent& evt = m_timeEventMgr.Event(xx);
		::Serialize(dbuff, evt.m_text);
		dbuff.Write(evt.m_playerMask);
		dbuff.Write(evt.m_fTime);
		dbuff.Write((sint32)evt.m_repTime);
		::Serialize(dbuff, evt.m_minerals);
	}
	// Surface and avatars map dump
	dbuff.Write(m_Buff, m_Width*m_Height*sizeof(iCell));
	
	// Fog maps and puzz;e cards
	for (iPlayerList::Iterator pit = m_Players.First(); pit != m_Players.End(); ++pit) {
		iPlayer* pPlayer = (*pit);
		// Puzzles
		dbuff.Write(pPlayer->m_openedObelisks);
		dbuff.Write(pPlayer->m_puzzleCards, 45);

		// Fog map
		uint32 siz = pPlayer->FogMap().GetFogMapDataSize();
		dbuff.Write(siz);
		dbuff.Write(pPlayer->FogMap().GetFogMapPtr(), siz);
	}

	// Visitables
	dbuff.Write((uint16)m_VisCnsts.Count());
	for (iVisCnstList::Iterator vcit = m_VisCnsts.First(); vcit != m_VisCnsts.End(); ++vcit) {
		iVisCnst* pVisCnst = *vcit;
		// Proto Id
		dbuff.Write(pVisCnst->Proto()->ProtoId());
		// Position
		::Serialize(dbuff, pVisCnst->Pos());
		// Serialize
		pVisCnst->Serialize(dbuff);
	}
	
	// Ownerables
	iSimpleArray<iOwnCnst*> ownCnsts;
	for (iOwnCnstList::Iterator ocit = m_OwnCnsts.First(); ocit != m_OwnCnsts.End(); ++ocit) ownCnsts.Add(*ocit);
	for (iPlayerList::Iterator pit = m_Players.First(); pit != m_Players.End(); ++pit)
		for (iPlayer::iCLIt ocit = (*pit)->m_Cnsts.First(); ocit != (*pit)->m_Cnsts.End(); ++ocit) ownCnsts.Add(*ocit);
	dbuff.Write((uint16)ownCnsts.GetSize());
	for (uint32 ocidx=0; ocidx<ownCnsts.GetSize(); ++ocidx) {
		// Proto Id
		dbuff.Write(ownCnsts[ocidx]->Proto()->ProtoId());
		// Owner Id
		dbuff.Write((sint8)ownCnsts[ocidx]->Owner());
		// Position
		::Serialize(dbuff, ownCnsts[ocidx]->Pos());
		// Guard
		::Serialize(dbuff, ownCnsts[ocidx]->Guard());
	}

	
	// Castles
	iSimpleArray<iCastle*> castleList;
	for (iCastleList::Iterator ctlit = m_Castles.First(); ctlit != m_Castles.End(); ++ctlit) castleList.Add(*ctlit);
	for (iPlayerList::Iterator pit = m_Players.First(); pit != m_Players.End(); ++pit)
		for (iPlayer::iCtLIt ctlit = (*pit)->CastleFirst(); ctlit != (*pit)->CastleEnd(); ++ctlit) castleList.Add(*ctlit);
	dbuff.Write((uint16)castleList.GetSize());
	for (uint32 ctlidx=0; ctlidx<castleList.GetSize(); ++ctlidx) {
		// Proto Id
		dbuff.Write(castleList[ctlidx]->Proto()->ProtoId());
		// Name
		::Serialize(dbuff, castleList[ctlidx]->Name());
		// Owner
		dbuff.Write((sint8)castleList[ctlidx]->Owner());
		// Position
		::Serialize(dbuff, castleList[ctlidx]->Pos());
		// Flags (always zero for saved map)
		dbuff.Write((uint8)0);
		// Garrison
		::Serialize(dbuff, castleList[ctlidx]->Garrison());
		// Can build
		dbuff.Write((uint8)castleList[ctlidx]->CanBuild());
		// Constructions
		castleList[ctlidx]->m_Cnsts.Serialize(dbuff);
	}

	// Obelisks count
	dbuff.Write(m_obelisksCount);

	// Ultimate artifact position and radius
	::Serialize(dbuff, m_posUltimate);
	dbuff.Write((uint8)0);
	dbuff.Write(m_statUltimate);

	// Map items
	dbuff.Write((uint16)m_MapItems.Count());
	for (iMapItemList::Iterator miit = m_MapItems.First(); miit != m_MapItems.End(); ++miit) {
		iMapItem* pMapItem = *miit;
		// Map item type
		dbuff.Write((uint8)pMapItem->Type());
		// Position
		::Serialize(dbuff, pMapItem->Pos());
		// Guard
		::Serialize(dbuff, pMapItem->Guard());
		// Message
		::Serialize(dbuff, pMapItem->Message());
		// Item speciffic data
		pMapItem->Serialize(dbuff);
	}

	// Map events
	dbuff.Write((uint16)m_MapEvents.Count());
	for (iMapEventList::Iterator meit = m_MapEvents.First(); meit != m_MapEvents.End(); ++meit) {
		iMapEvent* pMapEvent = *meit;
		::Serialize(dbuff, pMapEvent->Pos());
		::Serialize(dbuff, pMapEvent->Guard());
		::Serialize(dbuff, pMapEvent->RewardMessage());
		::Serialize(dbuff, pMapEvent->AttackMessage());
		dbuff.Write(pMapEvent->PlayerMask());
		::Serialize(dbuff, pMapEvent->Rewards());
	}

	// Map guards
	dbuff.Write((uint16)m_MapGuards.Count());
	for (iMapGuardList::Iterator mgit = m_MapGuards.First(); mgit != m_MapGuards.End(); ++mgit) {
		iMapGuard* pMapGuard = *mgit;
		// Guard group
		::Serialize(dbuff, pMapGuard->Creatures());
		// Join value
		dbuff.Write((sint8)pMapGuard->JoinVal());
		// Disposition
		dbuff.Write((uint8)pMapGuard->Disposition());
		// Grow flag
		dbuff.Write((uint8)pMapGuard->NotGrow());
		// Position
		::Serialize(dbuff, pMapGuard->Pos());
		// Message
		::Serialize(dbuff, pMapGuard->Message());
	}

	// Heroes
	uint16 hcnt = 0;
	for (iPlayerList::Iterator pit = m_Players.First(); pit != m_Players.End(); ++pit) for (iPlayer::iHLIt hit = (*pit)->HeroFirst(); hit != (*pit)->HeroEnd(); ++hit) hcnt++;
	dbuff.Write(&hcnt,sizeof(hcnt));
	for (iPlayerList::Iterator pit = m_Players.First(); pit != m_Players.End(); ++pit) {
		for (iPlayer::iHLIt hit = (*pit)->HeroFirst(); hit != (*pit)->HeroEnd(); ++hit) {
			(*hit)->Serialize(dbuff);
		}
	}

	// Hero manager
	dbuff.Write((uint16)gGame.ItemMgr().HeroesMgr().m_heroes.GetSize());
	for (uint32 xx=0; xx<gGame.ItemMgr().HeroesMgr().m_heroes.GetSize(); ++xx){
		// Serialize
		gGame.ItemMgr().HeroesMgr().m_heroes[xx]->Serialize(dbuff);
	}

	// Compress
	iDynamicBuffer cmpBuff;
	LZO::Compress((const unsigned char*)dbuff.GetData(), dbuff.GetSize(), cmpBuff);

	// Save to file
	pFile->Write(cmpBuff.GetData(), cmpBuff.GetSize());

	return true;
}

void iGameWorld::InitPassMap()
{
	const iCell* pSrc = GetPtr();
	sint8* pDst  = m_PassMap.GetPtr();
	sint32 mpy,mpx;
	// Surface
	for (mpy = 0; mpy<m_Height; ++mpy) {
		for (mpx = 0; mpx<m_Width; ++mpx, pSrc++, pDst++ ) {
			sint32 btSurf = iMIN(iMIN(iMIN(pSrc->SurfNode(0),pSrc->SurfNode(1)),pSrc->SurfNode(2)),pSrc->SurfNode(3));
			*pDst = SURF_MOVE_COST[btSurf];
		}
	}

	// Avatars
	pSrc = GetPtr();
	for (mpy = 0; mpy<m_Height; ++mpy) {
		for (mpx = 0; mpx<m_Width; ++mpx, pSrc++) {
			if (pSrc->avatar != 0xFFFF) {
				if ( (pSrc->avatar & 0xFF00) == 0xFF00) {
					// path element
					if (m_PassMap.IsValidPos(mpx,mpy) && gGame.ItemMgr().m_PathElProts[pSrc->avatar & 0xFF]->PassModVal() != 0) m_PassMap.GetAt(mpx,mpy) = gGame.ItemMgr().m_PathElProts[pSrc->avatar & 0xFF]->PassModVal();
				} else {
					// avatar
					const iAvatar& avatar = gGame.ItemMgr().m_AvatarMgr[pSrc->avatar];
					for (uint32 pmid=0; pmid<avatar.PassModsCount(); ++pmid) {
						sint32 ox = mpx+avatar.PassModEntry(pmid).ox;
						sint32 oy = mpy+avatar.PassModEntry(pmid).oy;
						if (m_PassMap.IsValidPos(ox,oy)) m_PassMap.GetAt(ox,oy) = avatar.PassModEntry(pmid).mod;
					}
				}
			}
		}
	}

	// Visitables
	for (iVisCnstList::Iterator vcit = m_VisCnsts.First(); vcit != m_VisCnsts.End(); ++vcit) {
		iVisCnst* pCnst = *vcit;
		const iVisCnstT* pProto = pCnst->Proto();
		for (uint32 aid = 0; aid<pProto->AvatarsCount(); ++aid) {
			const iAvatar& avatar = gGame.ItemMgr().m_AvatarMgr[pProto->AvatarEntry(aid).aid];
			for (uint32 pmid=0; pmid<avatar.PassModsCount(); ++pmid) {
				sint32 ox = pCnst->Pos().x+pProto->AvatarEntry(aid).ox+avatar.PassModEntry(pmid).ox;
				sint32 oy = pCnst->Pos().y+pProto->AvatarEntry(aid).oy+avatar.PassModEntry(pmid).oy;
				if (m_PassMap.IsValidPos(ox,oy)) m_PassMap.GetAt(ox,oy) = avatar.PassModEntry(pmid).mod;
			}
		}
	}

	// Neutral ownerables
	for (iOwnCnstList::Iterator ocit = m_OwnCnsts.First(); ocit != m_OwnCnsts.End(); ++ocit) {
		iOwnCnst* pCnst = *ocit;
		const iOwnCnstT* pProto = pCnst->Proto();
		for (uint32 aid = 0; aid<pProto->AvatarsCount(); ++aid) {
			const iAvatar& avatar = gGame.ItemMgr().m_AvatarMgr[pProto->AvatarEntry(aid).aid];
			for (uint32 pmid=0; pmid<avatar.PassModsCount(); ++pmid) {
				sint32 ox = pCnst->Pos().x+pProto->AvatarEntry(aid).ox+avatar.PassModEntry(pmid).ox;
				sint32 oy = pCnst->Pos().y+pProto->AvatarEntry(aid).oy+avatar.PassModEntry(pmid).oy;
				if (m_PassMap.IsValidPos(ox,oy)) m_PassMap.GetAt(ox,oy) = avatar.PassModEntry(pmid).mod;
			}
		}
	}

	// Neutral castles
	for (iCastleList::Iterator ctit = m_Castles.First(); ctit != m_Castles.End(); ++ctit) {
		iCastle* pCastle = *ctit;
		const iCastleT* pProto = pCastle->Proto();
		for (uint32 aid = 0; aid<pProto->AvatarsCount(); ++aid) {
			const iAvatar& avatar = gGame.ItemMgr().m_AvatarMgr[pProto->AvatarEntry(aid).aid];
			for (uint32 pmid=0; pmid<avatar.PassModsCount(); ++pmid) {
				sint32 ox = pCastle->Pos().x+pProto->AvatarEntry(aid).ox+avatar.PassModEntry(pmid).ox;
				sint32 oy = pCastle->Pos().y+pProto->AvatarEntry(aid).oy+avatar.PassModEntry(pmid).oy;
				if (m_PassMap.IsValidPos(ox,oy)) m_PassMap.GetAt(ox,oy) = avatar.PassModEntry(pmid).mod;
			}
		}
	}

	for (iPlayerList::Iterator pit = m_Players.First(); pit != m_Players.End(); ++pit) {
		// Ownerables
		for (iOwnCnstList::Iterator ocit = (*pit)->m_Cnsts.First(); ocit != (*pit)->m_Cnsts.End(); ++ocit) {
			iOwnCnst* pCnst = *ocit;
			const iOwnCnstT* pProto = pCnst->Proto();
			for (uint32 aid = 0; aid<pProto->AvatarsCount(); ++aid) {
				const iAvatar& avatar = gGame.ItemMgr().m_AvatarMgr[pProto->AvatarEntry(aid).aid];
				for (uint32 pmid=0; pmid<avatar.PassModsCount(); ++pmid) {
					sint32 ox = pCnst->Pos().x+pProto->AvatarEntry(aid).ox+avatar.PassModEntry(pmid).ox;
					sint32 oy = pCnst->Pos().y+pProto->AvatarEntry(aid).oy+avatar.PassModEntry(pmid).oy;
					if (m_PassMap.IsValidPos(ox,oy)) m_PassMap.GetAt(ox,oy) = avatar.PassModEntry(pmid).mod;
				}
			}
		}

		// Castles
		for (iCastleList::Iterator ctit = (*pit)->CastleFirst(); ctit != (*pit)->CastleEnd(); ++ctit) {
			iCastle* pCastle = *ctit;
			const iCastleT* pProto = pCastle->Proto();
			for (uint32 aid = 0; aid<pProto->AvatarsCount(); ++aid) {
				const iAvatar& avatar = gGame.ItemMgr().m_AvatarMgr[pProto->AvatarEntry(aid).aid];
				for (uint32 pmid=0; pmid<avatar.PassModsCount(); ++pmid) {
					sint32 ox = pCastle->Pos().x+pProto->AvatarEntry(aid).ox+avatar.PassModEntry(pmid).ox;
					sint32 oy = pCastle->Pos().y+pProto->AvatarEntry(aid).oy+avatar.PassModEntry(pmid).oy;
					if (m_PassMap.IsValidPos(ox,oy)) m_PassMap.GetAt(ox,oy) = avatar.PassModEntry(pmid).mod;
				}
			}
		}
	}
}

void iGameWorld::SetNewActor(iPlayer* pNewActor, bool bAttack)
{
	if (m_gameMode == iMapInfo::GM_SPLAYER) {
		// in case of single player, actor always initialized and cannot be changed
		check(m_pActPlayer == pNewActor);
	} else if (m_gameMode == iMapInfo::GM_HOTSEAT) {
		bool bActorChanged = (m_pActPlayer != pNewActor);
		m_pActPlayer = pNewActor;
		if (bActorChanged) gGame.OnActorChanged(pNewActor, bAttack);
	} else {
		check(0);
	}
}

iPlayer* iGameWorld::NextPlayer()
{
	check(m_pCurPlayer);
	bool bNewWeek = false;

	iPLIt nit = ++iPLIt(m_pCurPlayer);
	if ( nit == m_Players.End() ) {
		NewDay();
		if ( m_CurDay % 7 == 1 ) {
			bNewWeek = true;
			NewWeek();
		}
		nit = m_Players.First();
	}
	m_pCurPlayer = *nit;

	//
	if (m_pCurPlayer->PlayerType() == PT_HUMAN) {
		SetNewActor(m_pCurPlayer, false);
	}

	gGame.OnPlayerChanged(m_pCurPlayer, m_pActPlayer == m_pCurPlayer);
	m_pCurPlayer->OnSelect(bNewWeek, false);

	return m_pCurPlayer;
}

void iGameWorld::NewDay()
{
	for (iPLIt pit = m_Players.First(); pit != m_Players.End(); ++pit) (*pit)->NewDay();
	for (iVCIt vcit = m_VisCnsts.First(); vcit != m_VisCnsts.End(); ++vcit) (*vcit)->NewDay();
	for (iOCIt ocit = m_OwnCnsts.First(); ocit != m_OwnCnsts.End(); ++ocit) (*ocit)->NewDay();

	m_CurDay ++;
}

void iGameWorld::ProcessTimeEvents(iPlayer* pActor)
{
	for (uint32 xx=0; xx<m_timeEventMgr.EventsCount(); ++xx) {
		const iTimeEvent& evt = m_timeEventMgr.Event(xx);
		if (evt.IsConform(pActor->PlayerId(), m_CurDay)) {
			for (uint32 mm = 0; mm<MINERAL_COUNT; ++mm) {
				if (evt.m_minerals.quant[mm]) pActor->Minerals().quant[mm] = iMAX<sint32>(pActor->Minerals().quant[mm]+evt.m_minerals.quant[mm],0);
			}
			if (pActor->PlayerType() == PT_HUMAN) {
				iDlg_TimeEvent tedlg(&gApp.ViewMgr(), pActor->PlayerId(), evt);
				tedlg.DoModal();
			}
		}
	}
}

void iGameWorld::NewWeek()
{
	// Prepare new week descriptor
	m_weekDesc.bNewMonth = ((m_CurDay/7)%4) == 0;
	uint32 rval = m_rand.Rand(100);

	if (m_weekDesc.bNewMonth) {
		m_weekDesc.fparam = rval % 10;
		if ( rval < 20 ) {
			// Doubles (20%)
			m_weekDesc.wkType = iWeekDesc::CreatDoubles;
			m_weekDesc.crType = (CREATURE_TYPE)(CREAT_PEASANT + m_rand.Rand(6)*6 + m_rand.Rand(3));
			GenerateGuards(m_weekDesc.crType, m_CurDay/7);
		} else if ( rval < 40) {
			// Plague (20%)
			m_weekDesc.wkType = iWeekDesc::Plague;
			m_weekDesc.crType = CREAT_UNKNOWN;
		} else {
			// Normal (60%)
			m_weekDesc.wkType = iWeekDesc::Normal;
			m_weekDesc.crType = CREAT_UNKNOWN;
		}
	} else {
		m_weekDesc.fparam = rval % 15;
		if ( rval < 40) {
			// +5 (40%)
			m_weekDesc.wkType = iWeekDesc::CreatPlus;
			m_weekDesc.crType = (CREATURE_TYPE)(CREAT_PEASANT + m_rand.Rand(6)*6 + m_rand.Rand(6));
		} else {
			// Normal (60%)
			m_weekDesc.wkType = iWeekDesc::Normal;
			m_weekDesc.crType = CREAT_UNKNOWN;
		}
	}

	for (iPLIt pit = m_Players.First(); pit != m_Players.End(); ++pit) (*pit)->NewWeek(m_weekDesc);
	for (iVCIt vcit = m_VisCnsts.First(); vcit != m_VisCnsts.End(); ++vcit) (*vcit)->NewWeek(m_weekDesc);
	for (iOCIt ocit = m_OwnCnsts.First(); ocit != m_OwnCnsts.End(); ++ocit) (*ocit)->NewWeek(m_weekDesc);
	for (iGIt git = m_MapGuards.First(); git != m_MapGuards.End(); ++git) (*git)->NewWeek(m_weekDesc);
	for (iCtIt ctit = m_Castles.First(); ctit != m_Castles.End(); ++ctit) (*ctit)->NewWeek(m_weekDesc);
}

void iGameWorld::GenerateGuards(CREATURE_TYPE ct, sint32 week)
{
	// Prepare events map
	iMapT<uint8> emap(m_PassMap.GetWidth(), m_PassMap.GetHeight());
	emap.FillMem(0);
	for (iMapEventList::Iterator meit = m_MapEvents.First(); meit != m_MapEvents.End(); ++meit) {
		emap.GetAt((*meit)->Pos().x, (*meit)->Pos().y) = 1;
	}

	// Generate guards
	for (sint32 yy=0; yy<m_Height; yy +=16 ) {
		for (sint32 xx=0; xx<m_Width; xx +=16 ) {
			for (uint32 cnt=0; cnt<6; ++cnt) {
				sint32 px = xx + m_rand.Rand(16);
				sint32 py = yy + m_rand.Rand(16);
				if (m_PassMap.GetAt(px,py) >= 0 && m_CoverMap.GetAt(px, py) == NULL && emap.GetAt(px, py) == 0) {
					iMapGuard* pGuard = new iMapGuard(iPoint(px,py), ct, RANDOM_QUANTITY, m_rand.Rand(100), GDISP_AGGRESSIVE, false);
					AddMapGuard(pGuard);
				}
			}
		}
	}

	/*
	sint32 count = (m_Width*m_Height)/64;
	while (count) {
		sint32 px = 1+m_rand.Rand(m_Width-2);
		sint32 py = 1+m_rand.Rand(m_Height-2);
		if (m_PassMap.GetAt(px,py) >= 0 && m_CoverMap.GetAt(px, py) == NULL) {
			iMapGuard* pGuard = new iMapGuard(iPoint(px,py), ct, RANDOM_QUANTITY, gGame.Map().Rand(100), GDISP_AGGRESSIVE, false);
			pGuard->Creatures().Grow(week);
			AddMapGuard(pGuard);
			count--;
		}
	}*/
}

