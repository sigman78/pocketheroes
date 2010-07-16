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
#include "serialize.h"
#include "SpanMap.h"

//////////////////////////////////////////////////////////////////////////
enum BRUSH_PRESETS {
	BRUSH_POINT = 0,
	BRUSH_SQUARE1,
	BRUSH_SQUARE2,
	BRUSH_CIRCLE1,
	BRUSH_CIRCLE2,
	BRUSH_COUNT
};

struct BRUSH_PROTO {
	iSpannedMap::Shape	shape;
	sint32				radius;
};

BRUSH_PROTO BrushProtos[BRUSH_COUNT] = {
	{iSpannedMap::Square, 0},
	{iSpannedMap::Square, 1},
	{iSpannedMap::Square, 2},
	{iSpannedMap::Circle, 1},
	{iSpannedMap::Circle, 2}
};

//////////////////////////////////////////////////////////////////////////
iMapHandler::iMapHandler()
: m_CurPlayerId(PID_NEUTRAL), m_posUltimateArt(cInvalidPoint), m_radUltimateArt(0)
{
	memset(m_pPlayers,0,sizeof(m_pPlayers));
}

iMapHandler::~iMapHandler()
{
	ClearMap();
}

void iMapHandler::ClearMap()
{
	for (uint32 xx=0; xx<PID_COUNT; ++xx){
		if (m_pPlayers[xx]){
			delete m_pPlayers[xx];
			m_pPlayers[xx] = NULL;
		}
	}

	for (xx=0; xx<m_HeroList.GetSize(); ++xx) {
		delete m_HeroList[xx];
	}
	m_HeroList.RemoveAll();

	for (xx=0; xx<m_OwnCnstList.GetSize(); ++xx) {
		delete m_OwnCnstList[xx];
	}
	m_OwnCnstList.RemoveAll();
	
	for (xx=0; xx<m_VisCnstList.GetSize(); ++xx) {
		delete m_VisCnstList[xx];
	}
	m_VisCnstList.RemoveAll();

	for (xx=0; xx<m_CastleList.GetSize(); ++xx) {
		delete m_CastleList[xx];
	}
	m_CastleList.RemoveAll();

	
	for (xx=0; xx<m_MapItemList.GetSize(); ++xx) {
		delete m_MapItemList[xx];
	}
	m_MapItemList.RemoveAll();

	for (xx=0; xx<m_MapEventList.GetSize(); ++xx) {
		delete m_MapEventList[xx];
	}
	m_MapEventList.RemoveAll();

	for (xx=0; xx<m_MapGuardList.GetSize(); ++xx) {
		delete m_MapGuardList[xx];
	}
	m_MapGuardList.RemoveAll();

	m_timeEventMgr.DeleteAll();

	m_MapAuthor = _T("");
	m_MapVersion = _T("");

	m_posUltimateArt = cInvalidPoint;
	m_radUltimateArt = 0;
	m_ctMgr.Reset();
}


bool iMapHandler::InitMap(MAP_SIZE siz)
{
	ClearMap();
	m_Siz = siz;
	m_Nodes.Init(MAP_SIZ_SIZE[siz]+1,MAP_SIZ_SIZE[siz]+1);
	Init(MAP_SIZ_SIZE[siz],MAP_SIZ_SIZE[siz]);
	m_PassMap.Init(MAP_SIZ_SIZE[siz],MAP_SIZ_SIZE[siz]);

	return true;
}

void iMapHandler::PreparePassMap(iAvatarMgr& avatarMgr, bool bDynObj)
{
	m_PassMap.FillMem(-2);
	// Surfaces and decors
	for (sint32 yy=0; yy<m_Height; ++yy){
		for (sint32 xx=0; xx<m_Width; ++xx){
			if (m_PassMap.At(xx,yy) == -2){
				iMapHandler::iSurfCell cell;
				GetCellSurf(iPoint(xx,yy),cell);
				m_PassMap.At(xx,yy) = SURF_MOVE_COST[cell.lowestLayer];
			}
			
			if (const iAvatar* pAvatar = GetCell(xx,yy).pDecor) {
				for (uint32 pm=0; pm<pAvatar->PassModsCount(); ++pm){
					sint32 nx = xx+pAvatar->PassModEntry(pm).offset.x;
					sint32 ny = yy+pAvatar->PassModEntry(pm).offset.y;
					if (m_PassMap.IsValidPos(nx,ny)) m_PassMap.At(nx,ny) = pAvatar->PassModEntry(pm).mod;
				}
			}

			if (iPathElNode* pPathElNode = At(xx,yy).pPathEl){
				if (pPathElNode->PassModVal()) m_PassMap.At(xx,yy) = pPathElNode->PassModVal();
			}
		}
	}

	// Constructions
	for (uint32 vcid=0; vcid<m_VisCnstList.GetSize(); ++vcid) {
		for (uint32 aid=0; aid<m_VisCnstList[vcid]->Proto()->AvatarsCount(); ++aid) {
			const iAvatar& avt = avatarMgr[m_VisCnstList[vcid]->Proto()->Avatar(aid).id];
			for (uint32 pm=0; pm<avt.PassModsCount(); ++pm){
				sint32 nx = m_VisCnstList[vcid]->Pos().x+m_VisCnstList[vcid]->Proto()->Avatar(aid).offset.x+avt.PassModEntry(pm).offset.x;
				sint32 ny = m_VisCnstList[vcid]->Pos().y+m_VisCnstList[vcid]->Proto()->Avatar(aid).offset.y+avt.PassModEntry(pm).offset.y;
				if (m_PassMap.IsValidPos(nx, ny)) m_PassMap.At(nx, ny) = avt.PassModEntry(pm).mod;
			}
		}
	}
	
	for (uint32 ocid=0; ocid<m_OwnCnstList.GetSize(); ++ocid) {
		for (uint32 aid=0; aid<m_OwnCnstList[ocid]->Proto()->AvatarsCount(); ++aid) {
			const iAvatar& avt = avatarMgr[m_OwnCnstList[ocid]->Proto()->Avatar(aid).id];
			for (uint32 pm=0; pm<avt.PassModsCount(); ++pm){
				sint32 nx = m_OwnCnstList[ocid]->Pos().x+m_OwnCnstList[ocid]->Proto()->Avatar(aid).offset.x+avt.PassModEntry(pm).offset.x;
				sint32 ny = m_OwnCnstList[ocid]->Pos().y+m_OwnCnstList[ocid]->Proto()->Avatar(aid).offset.y+avt.PassModEntry(pm).offset.y;
				if (m_PassMap.IsValidPos(nx, ny)) m_PassMap.At(nx,ny) = avt.PassModEntry(pm).mod;
			}
		}
	}

	for (uint32 cid=0; cid<m_CastleList.GetSize(); ++cid) {
		for (uint32 aid=0; aid<m_CastleList[cid]->Proto()->AvatarsCount(); ++aid) {
			const iAvatar& avt = avatarMgr[m_CastleList[cid]->Proto()->Avatar(aid).id];
			for (uint32 pm=0; pm<avt.PassModsCount(); ++pm){
				sint32 nx = m_CastleList[cid]->Pos().x+m_CastleList[cid]->Proto()->Avatar(aid).offset.x+avt.PassModEntry(pm).offset.x;
				sint32 ny = m_CastleList[cid]->Pos().y+m_CastleList[cid]->Proto()->Avatar(aid).offset.y+avt.PassModEntry(pm).offset.y;
				if (m_PassMap.IsValidPos(nx,ny)) m_PassMap.At(nx, ny) = avt.PassModEntry(pm).mod;
			}
		}
	}

	// Map items and guards
	if (bDynObj) {
		for (uint32 rid=0; rid<m_MapItemList.GetSize(); ++rid) m_PassMap.At(m_MapItemList[rid]->Pos().x,m_MapItemList[rid]->Pos().y) = -2;
		for (uint32 gid=0; gid<m_MapGuardList.GetSize(); ++gid) m_PassMap.At(m_MapGuardList[gid]->Pos().x,m_MapGuardList[gid]->Pos().y) = -2;
	}
}

bool iMapHandler::ValidateMap(iStringT& res, bool bWarnings) const
{
	bool bOk = true;
	iMapT<uint8> pmap(m_Width,m_Height);
	sint32 xx,yy;
	// 0 - Passable cell
	// 1 - Not passable surface
	// 2 - Occuped by decoration or object
	// 3 - Occuped by map item (resource, artifact or map guard)

	// Surfaces
	for (yy=0; yy<m_Height; ++yy){
		for (xx=0; xx<m_Width; ++xx){
			iMapHandler::iSurfCell cell;
			GetCellSurf(iPoint(xx,yy),cell);
			if (cell.lowestLayer == STYPE_WATER) pmap.At(xx,yy) = 1;
			pmap.At(xx,yy) = 0;
		}
	}

	// Decorations
	for (yy=0; yy<m_Height; ++yy){
		for (xx=0; xx<m_Width; ++xx){
			if (const iAvatar* pAvatar = GetCell(xx,yy).pDecor) {
				for (uint32 pm=0; pm<pAvatar->PassModsCount(); ++pm){
					sint32 nx = xx+pAvatar->PassModEntry(pm).offset.x;
					sint32 ny = yy+pAvatar->PassModEntry(pm).offset.y;
					if (pmap.IsValidPos(nx,ny)) {
						if (pmap.At(nx,ny) > 1) {
							if (bWarnings) {
								res.Addf(_T("Warning at (%d,%d): Cell contains two or more decorations in one cell!\r\n"), nx, ny);
								bOk = false;
							}
						}
						pmap.At(nx,ny) = 2;
					} else {
						if (bWarnings) {
							res.Addf(_T("Warning at (%d,%d): Decoration is out of map!\r\n"), nx, ny);
							bOk = false;
						}
					}
				}
			}
		}
	}

	// Path elements
	for (yy=0; yy<m_Height; ++yy){
		for (xx=0; xx<m_Width; ++xx){
			if (iPathElNode* pPathElNode = At(xx,yy).pPathEl){
				if (pmap.IsValidPos(xx,yy)) {
					if (pPathElNode->PassModVal() == -1) {
						// Walls and other not passable path elements
						if (pmap.At(xx,yy) > 1) {
							if (bWarnings) {
								res.Addf(_T("Warning at (%d,%d): Cell contains two or more decorations in one cell!\r\n"), xx, yy);
								bOk = false;
							}
						}
						pmap.At(xx,yy) = 2;
					} else if (pPathElNode->PassModVal() > 0){
						// Roads and other passable path elements
						if (pmap.At(xx,yy) != 0) {
							res.Addf(_T("Error at (%d,%d): Road at cell with obstacle!\r\n"), xx, yy);
							bOk = false;
						}
						pmap.At(xx,yy) = 0;
					} else {
						// Unknown objects
						check(0);
					}
				} else {
					if (bWarnings) {
						res.Addf(_T("Warning at (%d,%d): Path element is out of map!\r\n"), xx, yy);
						bOk = false;
					}
				}
			}
		}
	}

	// Constructions
	for (uint32 vcid=0; vcid<m_VisCnstList.GetSize(); ++vcid) {
		if(m_VisCnstList[vcid]->Proto()->Type() == VCNST_TELEPORT) {			
			bool found = false;
			iVisCnstNode_Teleport* pTelep = (iVisCnstNode_Teleport*)m_VisCnstList[vcid]->Proto();
			for (uint32 iter=0; iter<m_VisCnstList.GetSize(); ++iter) {				
				if(m_VisCnstList[iter]->Proto()->Type() == VCNST_TELEPORT) { 
					iVisCnstNode_Teleport* pIter = (iVisCnstNode_Teleport*)m_VisCnstList[iter]->Proto();
					if(iter != vcid && pTelep->GetIndex() == pIter->GetIndex())	{
						found = true;
						break;
					}
				}
			}
			if(!found) {
				res.Addf(_T("Error at (%d,%d): Teleport doesn't have any pair!\r\n"), m_VisCnstList[vcid]->Pos().x, 
					m_VisCnstList[vcid]->Pos().y);
				bOk = false;
			}			
		}
			
	for (uint32 aid=0; aid<m_VisCnstList[vcid]->Proto()->AvatarsCount(); ++aid) {
			const iAvatar& avt = gItemMgr.m_AvatarMgr[m_VisCnstList[vcid]->Proto()->Avatar(aid).id];
			sint32 ax = m_VisCnstList[vcid]->Pos().x+m_VisCnstList[vcid]->Proto()->Avatar(aid).offset.x;
			sint32 ay = m_VisCnstList[vcid]->Pos().y+m_VisCnstList[vcid]->Proto()->Avatar(aid).offset.y;
			if (pmap.IsValidPos(ax,ay)) {
				for (uint32 pm=0; pm<avt.PassModsCount(); ++pm){
					sint32 nx = ax+avt.PassModEntry(pm).offset.x;
					sint32 ny = ay+avt.PassModEntry(pm).offset.y;
					if (pmap.IsValidPos(nx,ny)) {
						if (pmap.At(nx,ny) != 1) {
							if (bWarnings) {
								res.Addf(_T("Warning at (%d,%d): Visitable construction at cell with obstacle!\r\n"), nx, ny);
								bOk = false;
							}
						}
						pmap.At(nx,ny) = 2;
					} else {
						if (bWarnings) {
							res.Addf(_T("Warning at (%d,%d): Visitable construction is out of map!\r\n"), nx, ny);
							bOk = false;
						}
					}
				}
			} else {
				res.Addf(_T("Error at (%d,%d): Visitable construction is out of map!\r\n"), ax, ay);
				bOk = false;
			}
		}
	}
	
	for (uint32 ocid=0; ocid<m_OwnCnstList.GetSize(); ++ocid) {
		for (uint32 aid=0; aid<m_OwnCnstList[ocid]->Proto()->AvatarsCount(); ++aid) {
			const iAvatar& avt = gItemMgr.m_AvatarMgr[m_OwnCnstList[ocid]->Proto()->Avatar(aid).id];
			sint32 ax = m_OwnCnstList[ocid]->Pos().x+m_OwnCnstList[ocid]->Proto()->Avatar(aid).offset.x;
			sint32 ay = m_OwnCnstList[ocid]->Pos().y+m_OwnCnstList[ocid]->Proto()->Avatar(aid).offset.y;
			if (pmap.IsValidPos(ax,ay)) {
				for (uint32 pm=0; pm<avt.PassModsCount(); ++pm){
					sint32 nx = ax+avt.PassModEntry(pm).offset.x;
					sint32 ny = ay+avt.PassModEntry(pm).offset.y;
					if (pmap.IsValidPos(nx,ny)) {
						if (pmap.At(nx,ny) != 1) {
							if (bWarnings) {
								res.Addf(_T("Warning at (%d,%d): Ownerable construction at cell with obstaclel!\r\n"), nx, ny);
								bOk = false;
							}
						}
						pmap.At(nx,ny) = 2;
					} else {
						if (bWarnings) {
							res.Addf(_T("Warning at (%d,%d): Ownerable construction is out of map!\r\n"), nx, ny);
							bOk = false;
						}
					}
				}
			} else {
				res.Addf(_T("Error at (%d,%d): Ownerable construction is out of map!\r\n"), ax, ay);
				bOk = false;
			}
		}
	}
	for (uint32 cid=0; cid<m_CastleList.GetSize(); ++cid) {
		for (uint32 aid=0; aid<m_CastleList[cid]->Proto()->AvatarsCount(); ++aid) {
			const iAvatar& avt = gItemMgr.m_AvatarMgr[m_CastleList[cid]->Proto()->Avatar(aid).id];
			sint32 ax = m_CastleList[cid]->Pos().x+m_CastleList[cid]->Proto()->Avatar(aid).offset.x;
			sint32 ay = m_CastleList[cid]->Pos().y+m_CastleList[cid]->Proto()->Avatar(aid).offset.y;
			if (pmap.IsValidPos(ax,ay)) {
				for (uint32 pm=0; pm<avt.PassModsCount(); ++pm){
					sint32 nx = ax+avt.PassModEntry(pm).offset.x;
					sint32 ny = ay+avt.PassModEntry(pm).offset.y;
					if (pmap.IsValidPos(nx,ny)) {
						if (pmap.At(nx,ny) != 1) {
							if (bWarnings) {
								res.Addf(_T("Warning at (%d,%d): Castle at cell with obstacle!\r\n"), nx, ny);
								bOk = false;
							}
						}
						pmap.At(nx,ny) = 2;
					} else {
						if (bWarnings) {
							res.Addf(_T("Warning at (%d,%d): Castle is out of map!\r\n"), nx, ny);
							bOk = false;
						}
					}
				}
			} else {
				res.Addf(_T("Error at (%d,%d): Castle is out of map!\r\n"), ax, ay);
				bOk = false;
			}
		}
	}

	// Map items and guards
	for (uint32 rid=0; rid<m_MapItemList.GetSize(); ++rid) {
		sint32 nx = m_MapItemList[rid]->Pos().x;
		sint32 ny = m_MapItemList[rid]->Pos().y;
		if (pmap.At(nx,ny) == 3) {
			res.Addf(_T("Error at (%d,%d): Two or more map items at one cell!\r\n"), nx, ny);
			bOk = false;
		} else if (pmap.At(nx,ny) != 0) {
			res.Addf(_T("Error at (%d,%d): Map item at non cell with obstacle!\r\n"), nx, ny);
			bOk = false;
		} else {
			pmap.At(nx,ny) = 3;
		}
	}
	for (uint32 gid=0; gid<m_MapGuardList.GetSize(); ++gid) {
		sint32 nx = m_MapGuardList[gid]->Pos().x;
		sint32 ny = m_MapGuardList[gid]->Pos().y;
		if (pmap.At(nx,ny) == 3) {
			res.Addf(_T("Error at (%d,%d): Two or more map items at one cell!\r\n"), nx, ny);
			bOk = false;
		} else if (pmap.At(nx,ny) != 0) {
			res.Addf(_T("Error at (%d,%d): Map item at cell with obstacle!\r\n"), nx, ny);
			bOk = false;
		} else {
			pmap.At(nx,ny) = 3;
		}
	}

	// no heroes and no castles
	for (uint32 pid=0; pid<PID_COUNT; ++pid){
		if (m_pPlayers[pid]){			
			if (!m_pPlayers[pid]->m_pMainCastle) {								
				bool hasHero = false;
				for (uint32 hid = 0; hid<m_HeroList.GetSize(); ++hid){
					if (m_HeroList[hid]->Owner() == m_pPlayers[pid]->m_PlayerId) {						
						hasHero = true;
						break;
					}
				}
				if(!hasHero) {
					bOk = false;
					res.Addf(_T("%s player has no castles and no heroes!\r\n"), PLAYER_WORD_COLORS[pid]);
				}
			}
		}
	}
	return bOk;
}

void iMapHandler::SetSurf(sint32 x, sint32 y, sint32 brushIdx, SURF_TYPE _surf)
{
	check (brushIdx>=0 && brushIdx<BRUSH_COUNT);
	iSpannedMap spanMap(BrushProtos[brushIdx].shape, BrushProtos[brushIdx].radius);

	for (uint32 yy=0; yy<spanMap.SpanLinesCount(); ++yy) {
		const iSpannedMap::iSpan& sp = spanMap[yy];
		for (sint32 xx=sp.begin; xx<=sp.end; ++xx) SetNodeSurf(x+xx, y+ sp.ypos, _surf);
	}
}

void iMapHandler::AddPathEl(sint32 x, sint32 y, iPathElNode* pPathEl)
{
	GetPtr(x,y)->pDecor = NULL;
	GetPtr(x,y)->pPathEl = pPathEl;
}

void iMapHandler::AddDecoration(sint32 x, sint32 y, iDecorNode* pDecor)
{
	for (uint32 xx=0; xx<pDecor->AvatarsCount(); ++xx){
		sint32 xpos = x+pDecor->Avatar(xx).offset.x;
		sint32 ypos = y+pDecor->Avatar(xx).offset.y;
		if (IsValidPos(xpos,ypos)){
			GetPtr(xpos,ypos)->pPathEl = NULL;
			GetPtr(xpos,ypos)->pDecor = &gItemMgr.m_AvatarMgr[pDecor->Avatar(xx).id];
		}
	}
}

void iMapHandler::RemoveDecoration(const iPoint& pos)
{
	// removing decorations and/or path elements
	if (GetCell(pos.x,pos.y).pDecor || GetCell(pos.x,pos.y).pPathEl) {
		GetPtr(pos.x,pos.y)->pDecor = NULL;
		GetPtr(pos.x,pos.y)->pPathEl = NULL;
	}
}

void iMapHandler::AddPlayer(PLAYER_ID pid, PLAYER_TYPE_MASK ptm)
{
	check(!m_pPlayers[pid]);
	m_pPlayers[pid] = new iPlayer(pid, ptm);
}

bool iMapHandler::CanDeletePlayer(PLAYER_ID pid) const
{
	for (uint32 xx=0; xx<m_HeroList.GetSize(); ++xx) if (m_HeroList[xx]->Owner() == pid) return false;
	for (xx=0; xx<m_HeroList.GetSize(); ++xx) if (m_OwnCnstList[xx]->Owner() == pid) return false;
	for (xx=0; xx<m_HeroList.GetSize(); ++xx) if (m_CastleList[xx]->Owner() == pid) return false;
	return true;
}

void iMapHandler::DeletePlayer(PLAYER_ID pid)
{
	check(m_pPlayers[pid]);
	delete m_pPlayers[pid];
	m_pPlayers[pid] = NULL;
}

void iMapHandler::ChangeCastleOwner(iCastle* pCastle, PLAYER_ID npid) const
{
	PLAYER_ID opid = pCastle->Owner();
	if (opid != npid) {
		pCastle->SetOwner(npid); 
		if (opid != PID_NEUTRAL && m_pPlayers[opid]->m_pMainCastle == pCastle) {
			m_pPlayers[opid]->m_pMainCastle = NULL;
			m_pPlayers[opid]->m_bCreateHero = false;
			UpdateMainCastle(opid);
		}
		if (npid != PID_NEUTRAL && !m_pPlayers[npid]->m_pMainCastle) {
			UpdateMainCastle(npid);
		}
	}
}

void iMapHandler::UpdateMainCastle(PLAYER_ID pid) const
{
	check(pid != PID_NEUTRAL && m_pPlayers[pid]);
	if (!m_pPlayers[pid]->m_pMainCastle) {
		for (uint32 cidx = 0; cidx<m_CastleList.GetSize(); ++cidx) {
			if (m_CastleList[cidx]->Owner() == pid) {
				m_pPlayers[pid]->m_pMainCastle = m_CastleList[cidx];
				m_pPlayers[pid]->m_bCreateHero = false;
				break;
			}
		}
	}
}

bool iMapHandler::prvSetAltitude(sint32 x, sint32 y, uint8 na)
{
	m_Nodes.GetPtr(x,y)->alt= na;
	sint32 dif;
	if (IsValidNode(x-1,y)) {
		dif = (sint32)na - m_Nodes.GetCell(x-1,y).alt;
		if (dif>1 || dif < -1) if (!prvSetAltitude(x-1,y,(uint8)(na-iSIGN(dif)))) return false;
	}

	if (IsValidNode(x+1,y)) {
		dif = (sint32)na - m_Nodes.GetCell(x+1,y).alt;
		if (dif>1 || dif < -1) if (!prvSetAltitude(x+1,y,(uint8)(na-iSIGN(dif)))) return false;
	}

	if (IsValidNode(x,y+1)) {
		dif = (sint32)na - m_Nodes.GetCell(x,y+1).alt;
		if (dif>1 || dif < -1) if (!prvSetAltitude(x,y+1,(uint8)(na-iSIGN(dif)))) return false;
	}

	if (IsValidNode(x,y-1)) {
		dif = (sint32)na - m_Nodes.GetCell(x,y-1).alt;
		if (dif>1 || dif < -1) if (!prvSetAltitude(x,y-1,(uint8)(na-iSIGN(dif)))) return false;
	}
	return true;
}

void iMapHandler::Cmd_AddObject(iBaseNode* pObjNode, PLAYER_ID pid, const iPoint& pos )
{
	CommandPtr cmd( new AddObjectCommand( pObjNode, pos, pid ) );
	m_cmdMgr.CommitCommand( cmd.Giveup(), this );
}


iHero* iMapHandler::AddHero(iHero* pHero)
{
	m_HeroList.Add(pHero);
	return pHero;
}

void iMapHandler::AddObject(iBaseNode* pObjNode, PLAYER_ID owner, const iPoint& pos)
{
	if (iBaseVisCnstNode* pVisCnstNode = DynamicCast<iBaseVisCnstNode*>(pObjNode)) {
		m_VisCnstList.Add(pVisCnstNode->CreateObject(pos));
	} else if (iBaseOwnCnstNode* pOwnCnstNode = DynamicCast<iBaseOwnCnstNode*>(pObjNode)) {
		m_OwnCnstList.Add(pOwnCnstNode->CreateObject(pos, owner));
	} else if (iCastleNode* pCastleNode = DynamicCast<iCastleNode*>(pObjNode)) {
		m_CastleList.Add(pCastleNode->CreateObject(pos,owner));
		if (owner != PID_NEUTRAL && !m_pPlayers[owner]->m_pMainCastle) UpdateMainCastle(owner);
	} else if (iMineralNode* pMineralNode = DynamicCast<iMineralNode*>(pObjNode)) {
		AddMapObject(new iMapItem_Mineral(pos,pMineralNode->MineralType(),RANDOM_QUANTITY));
	} else if (iManaCrystalNode* pManaCrystalNode = DynamicCast<iManaCrystalNode*>(pObjNode)) {
		AddMapObject(new iMapItem_ManaCrystal(pos,RANDOM_QUANTITY));
	} else if (iCampFireNode* pCampfireNode = DynamicCast<iCampFireNode*>(pObjNode)) {
		AddMapObject(new iMapItem_CampFire(pos));
	} else if (iChestNode* pChestNode = DynamicCast<iChestNode*>(pObjNode)) {
		AddMapObject(new iMapItem_Chest(pos));
	} else if (iArtifactNode* pArtNode = DynamicCast<iArtifactNode*>(pObjNode)) {
		AddMapObject(new iMapItem_Artifact(pos,pArtNode));
	} else if (iSpellScrollNode* pSpellScrollNode = DynamicCast<iSpellScrollNode*>(pObjNode)) {
		AddMapObject(new iMapItem_SpellScroll(pos, MSP_MAGICARROW));
	} else if (DynamicCast<iLampNode*>(pObjNode)) {
		AddMapObject(new iMapItem_Lamp(pos));
	} else if (iKeyGuardNode* pKeyGuardNode = DynamicCast<iKeyGuardNode*>(pObjNode)) {
		AddMapObject(new iMapItem_KeyGuard(pos, pKeyGuardNode->m_key));
	} else if (DynamicCast<iMapEventNode*>(pObjNode)) {
		AddMapObject(new iMapEvent(pos));
	} else if (iGuardNode* pGuardNode = DynamicCast<iGuardNode*>(pObjNode)) {
		AddMapObject(new iMapGuard(pos, pGuardNode->CreatureType()));
	} else {
		check(0);
	}
}

void iMapHandler::AddMapObject(iBaseMapObject* pObj)
{
	if (iVisCnst* pVisCnst=DynamicCast<iVisCnst*>(pObj)) {
		m_VisCnstList.Add(pVisCnst);
	} else if (iOwnCnst* pOwnCnst=DynamicCast<iOwnCnst*>(pObj)) {
		m_OwnCnstList.Add(pOwnCnst);
	} else if (iCastle* pCastle=DynamicCast<iCastle*>(pObj)) {
		m_CastleList.Add(pCastle);
		if (pCastle->Owner() != PID_NEUTRAL && !m_pPlayers[pCastle->Owner()]->m_pMainCastle) UpdateMainCastle(pCastle->Owner());
	} else if (iMapItem* pMapItem = DynamicCast<iMapItem*>(pObj)) {
		m_MapItemList.Add(pMapItem);
	} else if (iMapEvent* pMapEvent = DynamicCast<iMapEvent*>(pObj)) {
		m_MapEventList.Add(pMapEvent);
	} else if (iMapGuard* pGuard = DynamicCast<iMapGuard*>(pObj)) {
		m_MapGuardList.Add(pGuard);
	} else {
		check(0);
	}
}

void iMapHandler::RemoveMapObject(iBaseMapObject* pObj)
{
	if (iVisCnst* pVisCnst=DynamicCast<iVisCnst*>(pObj)) {
		m_VisCnstList.Remove(pVisCnst);
		if (!pVisCnst->CustomMessage().Empty()) {
			check(m_ctMgr.HasEntry(pVisCnst->CustomMessage()));
			m_ctMgr.RemoveEntry(pVisCnst->CustomMessage());
		}
	} else if (iOwnCnst* pOwnCnst=DynamicCast<iOwnCnst*>(pObj)) {
		m_OwnCnstList.Remove(pOwnCnst);
	} else if (iCastle* pCastle=DynamicCast<iCastle*>(pObj)) {
		PLAYER_ID pid = pCastle->Owner();
		m_CastleList.Remove(pCastle);
		if (pid != PID_NEUTRAL && m_pPlayers[pid]->m_pMainCastle == pCastle) {
			// Reset main castle
			m_pPlayers[pid]->m_pMainCastle = NULL;
			m_pPlayers[pid]->m_bCreateHero = false;
			UpdateMainCastle(pid);
		}
		if (!pCastle->CustomName().Empty()) {
			check(m_ctMgr.HasEntry(pCastle->CustomName()));
			m_ctMgr.RemoveEntry(pCastle->CustomName());
		}

	} else if (iHero* pHero=DynamicCast<iHero*>(pObj)) {
		m_HeroList.Remove(pHero);
		if (!pHero->CustomName().Empty()) {
			check(m_ctMgr.HasEntry(pHero->CustomName()));
			m_ctMgr.RemoveEntry(pHero->CustomName());
		}
	} else if (iMapItem* pMapItem = DynamicCast<iMapItem*>(pObj)) {
		m_MapItemList.Remove(pMapItem);
		if (!pMapItem->Message().Empty()) {
			check(m_ctMgr.HasEntry(pMapItem->Message()));
			m_ctMgr.RemoveEntry(pMapItem->Message());
		}
	} else if (iMapEvent* pMapEvent = DynamicCast<iMapEvent*>(pObj)) {
		m_MapEventList.Remove(pMapEvent);
		if (!pMapEvent->RewardMessage().Empty()) {
			check(m_ctMgr.HasEntry(pMapEvent->RewardMessage()));
			m_ctMgr.RemoveEntry(pMapEvent->RewardMessage());
		}
		if (!pMapEvent->AttackMessage().Empty()) {
			check(m_ctMgr.HasEntry(pMapEvent->AttackMessage()));
			m_ctMgr.RemoveEntry(pMapEvent->AttackMessage());
		}
	} else if (iMapGuard* pMapGuard = DynamicCast<iMapGuard*>(pObj)) {
		m_MapGuardList.Remove(pMapGuard);
		if (!pMapGuard->Message().Empty()) {
			check(m_ctMgr.HasEntry(pMapGuard->Message()));
			m_ctMgr.RemoveEntry(pMapGuard->Message());
		}
	} else {
		check(0);
	}
	delete pObj;
}

iBaseMapObject* iMapHandler::GetCellObject(const iPoint& pos)
{
	// Heroes
	for (uint32 xx=0; xx<m_HeroList.GetSize(); ++xx){
		if (m_HeroList[xx]->Pos() == pos){
			return m_HeroList[xx];
		}
	}

	// Map items
	for (xx=0; xx<m_MapItemList.GetSize(); ++xx){
		if (m_MapItemList[xx]->Pos() == pos){
			return m_MapItemList[xx];
		}
	}

	// Map events
	for (xx=0; xx<m_MapEventList.GetSize(); ++xx){
		if (m_MapEventList[xx]->Pos() == pos){
			return m_MapEventList[xx];
		}
	}

	// Map guards
	for (xx=0; xx<m_MapGuardList.GetSize(); ++xx){
		if (m_MapGuardList[xx]->Pos() == pos){
			return m_MapGuardList[xx];
		}
	}

	// Visitables
	for (xx=0; xx<m_VisCnstList .GetSize(); ++xx){
		if (m_VisCnstList[xx]->Pos() == pos){
			return m_VisCnstList[xx];
		}
	}
	// Ownerables
	for (xx=0; xx<m_OwnCnstList .GetSize(); ++xx){
		if (m_OwnCnstList[xx]->Pos() == pos){
			return m_OwnCnstList[xx];
		}
	}
	// Castles
	for (xx=0; xx<m_CastleList.GetSize(); ++xx){
		if (m_CastleList[xx]->Pos() == pos){
			return m_CastleList[xx];
		}
	}
	return NULL;
}

/*
 *	
 */
struct iMainCtlDesc {
	PLAYER_ID	pid;
	iPoint		pos;
	uint8		bCreateHero;
};

// Old heroes list
LPCTSTR OLD_HEROES[24][2] = {
	{ _T("orrin"), _T("hero_dimitry")},
	{ _T("christian"), _T("hero_ambrose")},
	{ _T("valeska"), _T("hero_gveneth")},
	{ _T("sylvia"), _T("hero_fineous")},

	{ _T("craghack"), _T("hero_krag")},
	{ _T("grottekvarnen"), _T("hero_grottekvarnen")},
	{ _T("gretchin"), _T("hero_yog")},
	{ _T("terek"), _T("hero_tyro")},

	{ _T("gallatin"), _T("hero_gallatin")},
	{ _T("astral"), _T("hero_barok")},
	{ _T("iona"), _T("hero_iona")},
	{ _T("theodorus"), _T("hero_crodo")},

	{ _T("jeddite"), _T("hero_agar")},
	{ _T("sabbat"), _T("hero_sabbat")},
	{ _T("alamar"), _T("hero_alamar")},
	{ _T("arlach"), _T("hero_kastore")},

	{ _T("cyra"), _T("hero_ariel")},
	{ _T("aine"), _T("hero_gem")},
	{ _T("neela"), _T("hero_carlawn")},
	{ _T("rissa"), _T("hero_astra")},

	{ _T("sandro"), _T("hero_sandro")},
	{ _T("urof"), _T("hero_urof")},
	{ _T("khasam"), _T("hero_khasam")},
	{ _T("xsi"), _T("hero_ranloo")}
};

//
bool LoadMapFromFile(iMapHandler& map, const iStringT& fname)
{
	//
	map.ClearMap();
	iFileI* pFile = OpenWin32File(fname);
	if (!pFile) return false;
	iDynamicBuffer fileBuff((uint32)pFile->GetSize());
	pFile->Read(fileBuff.GetData(),(uint32)pFile->GetSize());
	delete pFile;

	try {
		// File Header
		uint32 hdr;
		fileBuff.Read(hdr);
		if (hdr != EMAP_FILE_HDR_KEY) throw iStringT(_T("Invalid file header!"));
		// File Version
		uint32 ver;
		fileBuff.Read(ver);
		// Map Size
		uint8 siz; fileBuff.Read(siz);

		if (!map.InitMap((MAP_SIZE)siz)) throw iStringT(_T("Unable to allocate map!"));
		
		if (ver > 0x18) {
			// Languages configuration (uint32 bit mask)
			fileBuff.Read(map.m_lngMask);
			// Text resources (for all languages (Key, lang_1, lang_2, lang_n)
			uint32 cnt;
			fileBuff.Read(cnt);
			iStringT key;
			sint16 tet;
			while (cnt--){
				fileBuff.Read(key);
				fileBuff.Read(tet);
				iTextMgr::iTextEntry& te = map.m_ctMgr.AddTextEntry(key, (TextEntryType)tet);
				for ( uint32 xx=0; xx<GLNG_COUNT; ++xx ) {
					if ( map.m_lngMask & (1<<xx) ) {
						fileBuff.Read(te.text[xx]);
					}
				}
			}
		} else {
			map.m_lngMask = 1 << GLNG_ENGLISH;
		}


		// Map name and description
		if (ver <= 0x18) {
			// Name and description
			iStringT name, desc;
			fileBuff.Read(name);
			fileBuff.Read(desc);
			map.m_ctMgr.AddTextEntry(MakeTextEntryName(TET_MAP_NAME, NULL), name, TET_MAP_NAME);
			map.m_ctMgr.AddTextEntry(MakeTextEntryName(TET_MAP_DESC, NULL), desc, TET_MAP_DESC);
		}

		// Map version and author
		if (ver >= 0x15) {
			iStringT map_version, map_author;
			fileBuff.Read(map_version);
			map.m_MapVersion = map_version;
			fileBuff.Read(map_author);
			map.m_MapAuthor = map_author;
		}


		// Time events
		map.m_timeEventMgr.DeleteAll();
		if (ver > 0x12) {
			uint16 cnt; 
			fileBuff.Read(cnt);
			iTimeEvent te;
			for (uint16 xx=0; xx<cnt; ++xx) {
				::Unserialize(fileBuff, te.m_name);
				::Unserialize(fileBuff, te.m_text);
				fileBuff.Read(te.m_playerMask);
				fileBuff.Read(te.m_fTime);
				sint32 repTime;
				fileBuff.Read(repTime);
				te.m_repTime = (iTimeEvent::EventFreq)repTime;
				::Unserialize(fileBuff, te.m_minerals);
				map.m_timeEventMgr.AddEvent(te);
			}
			if (ver <= 0x18) {
				// Text for all time events
				for (uint32 ee=0; ee<map.m_timeEventMgr.EventsCount(); ++ee) {
					iStringT tkey = MakeTextEntryName(TET_TEVENT_TEXT, &map.m_timeEventMgr.Event(ee));
					map.m_ctMgr.AddTextEntry(tkey, map.m_timeEventMgr.Event(ee).m_text, TET_TEVENT_TEXT);
					map.m_timeEventMgr.Event(ee).m_text = tkey;
				}
			}
		}

		// Ultimate artifact
		if (ver >= 0x18) {
			Unserialize(fileBuff, map.m_posUltimateArt);
			fileBuff.Read(map.m_radUltimateArt);
		}

		// Players
		iSimpleArray<iMainCtlDesc> mainCtls;
		uint16 pCount;
		fileBuff.Read(pCount);
		for (uint32 pid=0; pid<pCount; ++pid){
			sint8 playerId, playerTypeMask;
			fileBuff.Read(playerId);
			fileBuff.Read(playerTypeMask);
			if (ver >= 0x12) {
				uint8 hasMainCtl;
				fileBuff.Read(hasMainCtl);
				if (hasMainCtl) {
					iMainCtlDesc mainCtl;
					mainCtl.pid = (PLAYER_ID)playerId;
					Unserialize(fileBuff, mainCtl.pos);
					fileBuff.Read(mainCtl.bCreateHero);
					mainCtls.Add(mainCtl);
				}
			}
			map.AddPlayer((PLAYER_ID)playerId,(PLAYER_TYPE_MASK)playerTypeMask);
		}
		
		// Heroes
		uint16 hCount;
		fileBuff.Read(hCount);
		for (uint32 hid=0; hid<hCount; ++hid){
			iStringT	id;
			sint8		owner;
			iPoint		pos;
			sint32		experience;

			// Proto Id
			fileBuff.Read(id);
			// Owner Id
			fileBuff.Read(owner);
			// Position 
			Unserialize(fileBuff, pos);

			// Construct object
			iHeroNode* pHeroNode = gItemMgr.m_pHeroesNode->GetHeroNode(id);
			if (!pHeroNode) {

				// Old map? Try to find assoc
				for (uint32 xx=0; xx<24; ++xx){
					if (_tcsicmp(OLD_HEROES[xx][0],id.CStr()) == 0) {
						pHeroNode = gItemMgr.m_pHeroesNode->GetHeroNode(OLD_HEROES[xx][1]);
						break;
					}
				}
				if (!pHeroNode)	throw iFormat(_T("Map file contain invalid Hero '%s'!"),id.CStr());
			}
			iHero* pHero = pHeroNode->CreateObject(pos, (PLAYER_ID)owner);

			// Custom name
			if (ver > 0x18) {
				fileBuff.Read(pHero->CustomName());
			} else {
				iStringT chname;
				fileBuff.Read(chname);
				if (!chname.Empty()) {
					iStringT nkey = MakeTextEntryName(TET_HERO_CNAME, pHero);
					map.m_ctMgr.AddTextEntry(nkey, chname, TET_HERO_CNAME);
					pHero->CustomName() = nkey;
				}
			}
			// Level
			uint32 level;
			if (ver >= 0x14) {
				fileBuff.Read(level);
			} else {
				uint8 slevel;
				fileBuff.Read(slevel);
				level = slevel;
			}
			if (level) pHero->Level() = level;
			// Experience
			fileBuff.Read(experience);
			if (experience) pHero->Experience() = experience;
			// Secondary Skills
			pHero->SecSkills().RemoveAll();
			Unserialize(fileBuff, pHero->SecSkills());
			// Artifact list
			Unserialize(fileBuff, pHero->Artifacts());
			// FurtSkills
			iFurtSkills fsk;
			Unserialize(fileBuff, fsk);
			pHero->FurtSkills().Value(FSK_ATTACK) = fsk.Value(FSK_ATTACK);
			pHero->FurtSkills().Value(FSK_DEFENCE) = fsk.Value(FSK_DEFENCE);
			pHero->FurtSkills().Value(FSK_POWER) = fsk.Value(FSK_POWER);
			pHero->FurtSkills().Value(FSK_KNOWLEDGE) = fsk.Value(FSK_KNOWLEDGE);
			// Army
			Unserialize(fileBuff, pHero->Army());
			// Spellbook
			if (ver >= 0x06) {
				uint8 spflag; fileBuff.Read(spflag);
				pHero->SetCustomSpells(spflag > 0);
				if (spflag == 1) Unserialize(fileBuff, pHero->Spells());
			} else {
				pHero->SetCustomSpells(false);
				iSpellList tmpList;
				Unserialize(fileBuff, tmpList);
			}
			map.AddHero(pHero);
		}

		// Map Items
		uint16 moCount;
		fileBuff.Read(moCount);
		while (moCount--) {
			uint8 objType;	fileBuff.Read(objType);
			iPoint pos;		Unserialize(fileBuff,pos);
			iArmy army;		Unserialize(fileBuff,army);
			iStringT msg;	Unserialize(fileBuff, msg);

			iMapItem*	pItem = NULL;
			if (objType == MAPITEM_MINERAL) {
				sint8 mtype;	fileBuff.Read(mtype);
				sint32 mcount;	fileBuff.Read(mcount);
				pItem = new iMapItem_Mineral(pos,(MINERAL_TYPE)mtype,mcount);
			} else if (objType == MAPITEM_MANACRYSTAL) {
				sint32 mcount;	fileBuff.Read(mcount);
				pItem = new iMapItem_ManaCrystal(pos,mcount);
			} else if (objType == MAPITEM_CAMPFIRE) {
				pItem = new iMapItem_CampFire(pos);
			} else if (objType == MAPITEM_CHEST) {
				pItem = new iMapItem_Chest(pos);
			} else if (objType == MAPITEM_LAMP) {
				pItem = new iMapItem_Lamp(pos);
			} else if (objType == MAPITEM_ARTIFACT) {
				iStringT aid; fileBuff.Read(aid);
				iArtifactNode* pArtNode = gItemMgr.m_pArtifactNode->GetArtifactNode(aid);
				if (!pArtNode) throw iFormat(_T("Map file contain invalid Artifact '%s'!"),aid.CStr());
				pItem = new iMapItem_Artifact(pos,pArtNode);
			} else if (objType == MAPITEM_SPELLSCROLL) {
				uint16 spellId; fileBuff.Read(spellId);
				pItem = new iMapItem_SpellScroll(pos, spellId);
			} else if (objType == MAPITEM_KEYGUARD) {
				uint8 key; fileBuff.Read(key);
				pItem = new iMapItem_KeyGuard(pos, key);
			} else {
				throw _T("Map file contain invalid Map item !");
			}
			pItem->Guard() = army;

			if (!msg.Empty()) {
				if (ver > 0x18) {
					pItem->Message() = msg;
				} else {
					pItem->Message() = MakeTextEntryName(TET_MAPITEM_MSG, pItem);
					map.m_ctMgr.AddTextEntry(pItem->Message(), msg, TET_MAPITEM_MSG);
				}
			}
			map.AddMapObject(pItem);
		}
		
		// Guards
		uint16 gCount;
		fileBuff.Read(gCount);
		for (uint32 gId = 0; gId<gCount; ++gId){
			iCreatGroup ct;	Unserialize(fileBuff, ct);
			uint8 disp;		fileBuff.Read(disp);
			uint8 bNotGrow; fileBuff.Read(bNotGrow);
			iPoint pos;		Unserialize(fileBuff,pos);
			iMapGuard* pGuard = new iMapGuard(pos,ct.Type(), ct.Count(),(GUARD_DISPOS)disp,bNotGrow>0);
			if (ver > 0x18) {
				Unserialize(fileBuff, pGuard->Message());
			} else {
				iStringT msg;
				Unserialize(fileBuff, msg);
				if (!msg.Empty()) {
					pGuard->Message() = MakeTextEntryName(TET_MAPGUARD_MSG, pGuard);
					map.m_ctMgr.AddTextEntry(pGuard->Message(), msg, TET_MAPGUARD_MSG);
				}
			}
			map.AddMapObject(pGuard);
		}

		// Map events
		if (ver >= 0x16) {
			uint16 meCount;
			fileBuff.Read(meCount);
			while (meCount--) {
				iPoint pos;		Unserialize(fileBuff,pos);
				iMapEvent* pMapEvent = new iMapEvent(pos);
				if (ver >= 0x17) {
					Unserialize(fileBuff, pMapEvent->Guard());
					if (ver > 0x18) {
						Unserialize(fileBuff, pMapEvent->RewardMessage());
						Unserialize(fileBuff, pMapEvent->AttackMessage());
					} else {
						iStringT rew_msg, att_msg;
						Unserialize(fileBuff, rew_msg);
						Unserialize(fileBuff, att_msg);
						if (!rew_msg.Empty()) {
							pMapEvent->RewardMessage() = MakeTextEntryName(TET_MEVENT_REWMSG, pMapEvent);
							map.m_ctMgr.AddTextEntry(pMapEvent->RewardMessage(), rew_msg, TET_MEVENT_REWMSG);
						}
						if (!att_msg.Empty()) {
							pMapEvent->AttackMessage() = MakeTextEntryName(TET_MEVENT_ATTMSG, pMapEvent);
							map.m_ctMgr.AddTextEntry(pMapEvent->AttackMessage(), att_msg, TET_MEVENT_ATTMSG);
						}
					}
					fileBuff.Read(pMapEvent->PlayerMask());
					uint16 rewCnt;
					fileBuff.Read(rewCnt);
					while (rewCnt--) {
						sint32 fparam = 0, sparam = 0;
						sint32 rewType;
						fileBuff.Read(rewType);
						if (rewType == RIT_ARTIFACT) {
							// we have an artifact Id in case of artifact reward
							iStringT artId;
							Unserialize(fileBuff, artId);
							fileBuff.Read(sparam);
							iArtifactNode* pArtNode = gItemMgr.m_pArtifactNode->GetArtifactNode(artId);
							if (!pArtNode) {
								MessageBox(0, _T("Invalid artifact in rewards (skiped)."),0, MB_OK);
								continue;
							}
							fparam = pArtNode->Idx();
						} else {
							// in general case we have only integer values
							fileBuff.Read(fparam);
							fileBuff.Read(sparam);
						}
						pMapEvent->Rewards().Add(iRewardItem((REWARD_ITEM_TYPE)rewType, fparam, sparam));
					}
				}
				map.AddMapObject(pMapEvent);
			}
		}

		// Visitables
		uint16 vcCount;
		fileBuff.Read(vcCount);
		for (uint32 vcnstId = 0; vcnstId<vcCount; ++vcnstId){
			iStringT	id;		fileBuff.Read(id);
			iPoint		pos;	Unserialize(fileBuff, pos);


		//uncomment if conversion needed
/*			if(id == L"halflings_hut")
				id = L"magic_school";
			if(id == L"halflings_hut_snow")
				id = L"magic_school_snow";			
			if(id == L"wolf_den")
				id = L"warg_cave";
			if(id == L"wolf_den_snow")
				id = L"warg_cave_snow";
			if(id == L"boars_pen")
				id = L"white_wolf_den";
			if(id == L"boars_pen_snow")
				id = L"white_wolf_den_snow";
			if(id == L"swordsman_armory")
				id = L"monk_abbey";
			if(id == L"swordsman_armory_snow")
				id = L"monk_abbey_snow";
			if(id == L"sarcophagus")
				id = L"mausoleum";
			if(id == L"sarcophagus_snow")
				id = L"mausoleum_snow";
*/
			

			iBaseVisCnstNode* pVisCnstNode = gItemMgr.m_pVisCnstNode->GetVisCnstNode(id);
			if (pVisCnstNode) {
				iVisCnst* pVisCnst = pVisCnstNode->CreateObject(pos);
				Unserialize(fileBuff, pVisCnst->Guard());
				if (ver > 0x18) {
					Unserialize(fileBuff, pVisCnst->CustomMessage());
				} else {
					iStringT msg;
					Unserialize(fileBuff, msg);
					if (!msg.Empty()) {
						pVisCnst->CustomMessage() = MakeTextEntryName(TET_VIS_MSG, pVisCnst);
						map.m_ctMgr.AddTextEntry(pVisCnst->CustomMessage(), msg, TET_VIS_MSG);
					}
				}
				map.AddMapObject(pVisCnst);
			} else {
				throw iFormat(_T("Map file contain invalid Visitable Construction '%s'!"),id.CStr());
			}
		}

		// Ownerables
		uint16 ocCount;
		fileBuff.Read(ocCount);
		for (uint32 ocnstId = 0; ocnstId<ocCount; ++ocnstId){
			iStringT	id;		fileBuff.Read(id);
			sint8		owner;	fileBuff.Read(owner);
			iPoint		pos;	Unserialize(fileBuff, pos);
			iBaseOwnCnstNode* pOwnCnstNode = gItemMgr.m_pOwnCnstNode->GetOwnCnstNode(id);
			if (pOwnCnstNode) {
				iOwnCnst* pOwnCnst = pOwnCnstNode->CreateObject(pos,(PLAYER_ID)owner);
				Unserialize(fileBuff, pOwnCnst->Guard());
				map.AddMapObject(pOwnCnst);
			} else {
				throw iFormat(_T("Map file contain invalid Ownership Construction '%s'!"), id.CStr());
			}
		}
		
		// Castles
		uint16 ctlCount;
		fileBuff.Read(ctlCount);
		for (uint32 ctlId = 0; ctlId<ctlCount; ++ctlId){
			iStringT	id;		fileBuff.Read(id);
			uint8		type;	fileBuff.Read(type);
			sint8		owner;	fileBuff.Read(owner);
			iPoint		pos;	Unserialize(fileBuff, pos);

			iCastleNode* pCastleNode = gItemMgr.m_pCastleNode->GetCastleNode(id,(CTL_TYPE)type);
			if (pCastleNode) {
				iCastle* pCastle = pCastleNode->CreateObject(pos,(PLAYER_ID)owner);
				Unserialize(fileBuff, pCastle->Garrison());
				if (ver > 0x18) {
					Unserialize(fileBuff, pCastle->CustomName());
				} else {
					iStringT cname;
					Unserialize(fileBuff, cname);
					if (!cname.Empty()) {
						pCastle->CustomName() = MakeTextEntryName(TET_CASTLE_CNAME, pCastle);
						map.m_ctMgr.AddTextEntry(pCastle->CustomName(), cname, TET_CASTLE_CNAME);
					}
				}
				// Constructions
				if (ver >= 0x05) {
					uint16 ctlCnstCount;
					fileBuff.Read(ctlCnstCount);
					pCastle->CtlCnsts().SetCustomized(ctlCnstCount > 0);
					while (ctlCnstCount--) {
						uint8 ctlCnstType, ctlCnstState;
						fileBuff.Read(ctlCnstType);
						fileBuff.Read(ctlCnstState);
						pCastle->CtlCnsts().SetState((CTL_CONSTR)ctlCnstType, ctlCnstState);
					}
				}
				map.AddMapObject(pCastle);
			} else {
				throw iFormat(_T("Map file contain invalid Castle '%s'!"),id.CStr());
			}
		}
	
	
		// Map Dump
		uint32 len = (MAP_SIZ_SIZE[siz]+1)*(MAP_SIZ_SIZE[siz]+1)*sizeof(iMapHandler::iNode);
		fileBuff.Read(map.m_Nodes.GetPtr(),len);
		// Decorations
		uint32 decCount;
		fileBuff.Read(decCount);
		iStringT decName;
		iPoint pos;
		for (uint32 dcnt=0; dcnt<decCount; ++dcnt){
			Unserialize(fileBuff, pos);
			fileBuff.Read(decName);
			map.GetPtr(pos.x,pos.y)->pDecor = &gItemMgr.m_AvatarMgr[decName];
		}
		// Path Elements
		uint32 peCount;
		fileBuff.Read(peCount);
		iStringT peName;
		for (uint32 pcnt=0; pcnt<peCount; ++pcnt){
			Unserialize(fileBuff, pos);
			fileBuff.Read(peName);
			iPathElNode * pPathEl = gItemMgr.m_pPathElNode->GetPathElNode(peName);
			if (pPathEl) {
				map.GetPtr(pos.x,pos.y)->pPathEl = pPathEl;
			} else {
				throw iFormat(_T("Map file contain invalid Path element '%s'!"),peName);
			}
		}

		// Main castles
		for (uint32 mcidx = 0; mcidx<mainCtls.GetSize(); ++mcidx) {
			iCastle* pMainCastle = DynamicCast<iCastle*>(map.GetCellObject(mainCtls[mcidx].pos));
			check(pMainCastle);
			check(pMainCastle->Owner() == mainCtls[mcidx].pid);
			map.m_pPlayers[mainCtls[mcidx].pid]->m_pMainCastle = pMainCastle;
			map.m_pPlayers[mainCtls[mcidx].pid]->m_bCreateHero = mainCtls[mcidx].bCreateHero;
		}

		map.m_FileName = fname;

		return true;
	} catch(iStringT& msg) {
		MessageBox(NULL,msg.CStr(),_T("Error"),MB_OK);
		return false;
	}
	return true;
}


//
bool SaveMapToFile(iMapHandler& map, const iStringT& fname)
{
	iFileI* pFile = CreateWin32File(fname);
	if (!pFile) return false;
	try {
		iDynamicBuffer fileBuff;

		// File Header
		fileBuff.Write(EMAP_FILE_HDR_KEY);
		// File Version
		fileBuff.Write(EMAP_FILE_VERSION);
		// Map Size
		fileBuff.Write((uint8)map.m_Siz);
		// Languages configuration (uint32 bit mask)
		fileBuff.Write(map.m_lngMask);
		// Text resources (for all languages (Key, lang_1, lang_2, lang_n)
		fileBuff.Write((uint32)map.m_ctMgr.GetHash().Count());
		for (iTextMgr::ConstTextIt cit = map.m_ctMgr.GetHash().First(); cit != map.m_ctMgr.GetHash().End(); ++cit) {
			const iTextMgr::iTextEntry& te = *cit;
			fileBuff.Write(te.key);
			fileBuff.Write(te.tet);
			for (uint32 xx=0; xx<GLNG_COUNT; ++xx) {
				if ( map.m_lngMask & (1<<xx) ) {
					fileBuff.Write(te.text[xx]);
				}
			}
		}
		
		// Map version and author
		fileBuff.Write(map.m_MapVersion);
		fileBuff.Write(map.m_MapAuthor);
		// Time events
		uint16 cecnt = map.m_timeEventMgr.EventsCount();
		fileBuff.Write(cecnt);
		for (uint32 xx=0; xx<cecnt; ++xx){
			const iTimeEvent& evt = map.m_timeEventMgr.Event(xx);
			::Serialize(fileBuff, evt.m_name);
			::Serialize(fileBuff, evt.m_text);
			fileBuff.Write(evt.m_playerMask);
			fileBuff.Write(evt.m_fTime);
			fileBuff.Write((sint32)evt.m_repTime);
			::Serialize(fileBuff, evt.m_minerals);
		}

		// Ultimate Artifact
		Serialize(fileBuff, map.m_posUltimateArt);
		fileBuff.Write(map.m_radUltimateArt);
		// Players
		iSimpleArray<iPlayer*> pPlayers;
		for (uint32 pid=PID_RED; pid<PID_COUNT; ++pid) {
			if (map.m_pPlayers[pid]) pPlayers.Add(map.m_pPlayers[pid]);
		}
		fileBuff.Write((uint16)pPlayers.GetSize());
		for (pid = 0; pid<pPlayers.GetSize(); ++pid){
			fileBuff.Write((uint8)pPlayers[pid]->m_PlayerId);
			fileBuff.Write((uint8)pPlayers[pid]->m_PlayerTypeMask);
			if (pPlayers[pid]->m_pMainCastle) {
				fileBuff.Write((uint8)1);
				Serialize(fileBuff, pPlayers[pid]->m_pMainCastle->Pos());
				fileBuff.Write((uint8)pPlayers[pid]->m_bCreateHero);
			} else {
				fileBuff.Write((uint8)0);
			}
		}
		
		// Heroes
		fileBuff.Write((uint16)map.m_HeroList.GetSize());
		for (uint32 hid = 0; hid<map.m_HeroList.GetSize(); ++hid) {
			// Proto Id
			fileBuff.Write(map.m_HeroList[hid]->Proto()->Id());
			// Owner Id
			fileBuff.Write((sint8)map.m_HeroList[hid]->Owner());
			// Position
			Serialize(fileBuff, map.m_HeroList[hid]->Pos());
			// Custom name
			fileBuff.Write(map.m_HeroList[hid]->CustomName());
			// Level
			fileBuff.Write(map.m_HeroList[hid]->Level());
			// Experience
			fileBuff.Write(map.m_HeroList[hid]->Experience());
			// Secondary Skills
			Serialize(fileBuff, map.m_HeroList[hid]->SecSkills());
			// Artifact list
			Serialize(fileBuff, map.m_HeroList[hid]->Artifacts());
			// Furt Skills
			Serialize(fileBuff, map.m_HeroList[hid]->FurtSkills());
			// Army
			Serialize(fileBuff, map.m_HeroList[hid]->Army());
			// Spellbook
			if (map.m_HeroList[hid]->IsCustomSpells()) {
				fileBuff.Write((uint8)1);
				Serialize(fileBuff, map.m_HeroList[hid]->Spells());
			} else fileBuff.Write((uint8)0);
		}

		// Map Items
		fileBuff.Write((uint16)map.m_MapItemList.GetSize());
		for (uint32 mobjId = 0; mobjId<map.m_MapItemList.GetSize(); ++mobjId) {
			iMapItem* pItem = map.m_MapItemList[mobjId];
			fileBuff.Write((uint8)pItem->Type());
			Serialize(fileBuff, pItem->Pos());
			Serialize(fileBuff, pItem->Guard());
			Serialize(fileBuff, pItem->Message());
			if (pItem->Type() == MAPITEM_MINERAL) {
				iMapItem_Mineral* pMineral = (iMapItem_Mineral*)pItem;
				fileBuff.Write((sint8)pMineral->MineralType());
				fileBuff.Write((sint32)pMineral->MineralCount());
			} else if (pItem->Type() == MAPITEM_MANACRYSTAL) {
				iMapItem_ManaCrystal* pManaCrystal = (iMapItem_ManaCrystal*)pItem;
				fileBuff.Write((sint32)pManaCrystal->Count());
			} else if (pItem->Type() == MAPITEM_ARTIFACT) {
				iMapItem_Artifact* pArtifact = (iMapItem_Artifact*)pItem;
				fileBuff.Write(pArtifact->Proto()->Id());
			} else if (pItem->Type() == MAPITEM_SPELLSCROLL) {
				iMapItem_SpellScroll* pSpellScroll = (iMapItem_SpellScroll*)pItem;
				fileBuff.Write((uint16)pSpellScroll->SpellId());
			} else if (pItem->Type() == MAPITEM_KEYGUARD) {
				iMapItem_KeyGuard* pKeyGuard = (iMapItem_KeyGuard*)pItem;
				fileBuff.Write(pKeyGuard->GetKeyType());
			}
		}

		// Guards
		fileBuff.Write((uint16)map.m_MapGuardList.GetSize());
		for (uint32 gId = 0; gId<map.m_MapGuardList.GetSize(); ++gId){
			Serialize(fileBuff, map.m_MapGuardList[gId]->Creatures());
			fileBuff.Write((uint8)map.m_MapGuardList[gId]->Disposition());
			fileBuff.Write((uint8)map.m_MapGuardList[gId]->NotGrow());
			Serialize(fileBuff, map.m_MapGuardList[gId]->Pos());
			Serialize(fileBuff, map.m_MapGuardList[gId]->Message());
		}

		// Map events
		fileBuff.Write((uint16)map.m_MapEventList.GetSize());
		for (uint32 meId = 0; meId<map.m_MapEventList.GetSize(); ++meId){
			Serialize(fileBuff, map.m_MapEventList[meId]->Pos());
			Serialize(fileBuff, map.m_MapEventList[meId]->Guard());
			Serialize(fileBuff, map.m_MapEventList[meId]->RewardMessage());
			Serialize(fileBuff, map.m_MapEventList[meId]->AttackMessage());
			fileBuff.Write(map.m_MapEventList[meId]->PlayerMask());
			fileBuff.Write((uint16)map.m_MapEventList[meId]->Rewards().GetSize());
			for (uint32 xx=0; xx<map.m_MapEventList[meId]->Rewards().GetSize(); ++xx) {
				const iRewardItem& reward = map.m_MapEventList[meId]->Rewards()[xx];
				fileBuff.Write((sint32)reward.m_type);
				if (reward.m_type == RIT_ARTIFACT) {
					// we need to save artifact Id in case of artifact reward
					iArtifactNode* pArtNode = gItemMgr.m_pArtifactNode->GetArtifactNode((uint16)reward.m_fParam);
					check(pArtNode);
					Serialize(fileBuff, pArtNode->Id());
				} else {
					// in general case we will store only integer values
					fileBuff.Write((sint32)reward.m_fParam);
				}
				fileBuff.Write((sint32)reward.m_sParam);
			}
		}

		// Visitables
		fileBuff.Write((uint16)map.m_VisCnstList.GetSize());
		for (uint32 vcnstId = 0; vcnstId<map.m_VisCnstList.GetSize(); ++vcnstId){
			fileBuff.Write(map.m_VisCnstList[vcnstId]->Proto()->Id());
			Serialize(fileBuff, map.m_VisCnstList[vcnstId]->Pos());
			Serialize(fileBuff, map.m_VisCnstList[vcnstId]->Guard());
			Serialize(fileBuff, map.m_VisCnstList[vcnstId]->CustomMessage());
		}

		// Ownerables
		fileBuff.Write((uint16)map.m_OwnCnstList.GetSize());
		for (uint32 ocnstId = 0; ocnstId<map.m_OwnCnstList.GetSize(); ++ocnstId){
			fileBuff.Write(map.m_OwnCnstList[ocnstId]->Proto()->Id());
			fileBuff.Write((uint8)map.m_OwnCnstList[ocnstId]->Owner());
			Serialize(fileBuff, map.m_OwnCnstList[ocnstId]->Pos());
			Serialize(fileBuff, map.m_OwnCnstList[ocnstId]->Guard());
		}

		// Castles
		fileBuff.Write((uint16)map.m_CastleList.GetSize());
		for (uint32 ctlId = 0; ctlId<map.m_CastleList.GetSize(); ++ctlId){
			iCastle* pCastle = map.m_CastleList[ctlId];
			fileBuff.Write(pCastle->Proto()->Id());
			fileBuff.Write((uint8)pCastle->Proto()->Type());
			fileBuff.Write((uint8)pCastle->Owner());
			Serialize(fileBuff, pCastle->Pos());
			Serialize(fileBuff, pCastle->Garrison());
			Serialize(fileBuff, pCastle->CustomName());
			// Constructions
			if (pCastle->CtlCnsts().IsCustomized()) {
				fileBuff.Write((uint16)pCastle->CtlCnsts().Count());
				for (uint32 ctlCnstId = 0; ctlCnstId<pCastle->CtlCnsts().Count(); ++ctlCnstId) {
					uint8 type = pCastle->CtlCnsts().Entry(ctlCnstId).type;
					uint8 state = pCastle->CtlCnsts().Entry(ctlCnstId).state;
					fileBuff.Write(type);
					fileBuff.Write(state);
				}
			} else {
				fileBuff.Write((uint16)0);
			}
		}

		// Map Dump
		fileBuff.Write(map.m_Nodes.GetPtr(),(map.GetWidth() +1)*(map.GetHeight()+1)*sizeof(iMapHandler::iNode));
		
		// Decorations and Path Elements
		iDynamicBuffer decBuff;
		iDynamicBuffer peBuff;
		decBuff.Write((uint32)0);
		peBuff.Write((uint32)0);
		uint32 deccnt = 0;
		uint32 pecnt = 0;
		for (sint32 mapY=0; mapY<map.GetHeight(); ++mapY){
			for (sint32 mapX=0; mapX<map.GetWidth(); ++mapX){
				if (map.GetCell(mapX,mapY).pDecor){
					Serialize(decBuff,iPoint(mapX,mapY));
					decBuff.Write(map.GetCell(mapX,mapY).pDecor->Id());
					deccnt++;
				} else if (map.GetCell(mapX,mapY).pPathEl){
					Serialize(peBuff,iPoint(mapX,mapY));
					peBuff.Write(map.GetCell(mapX,mapY).pPathEl->Id());
					pecnt++;
				}
			}
		}
		decBuff.Seek(0);
		decBuff.Write(deccnt);
		fileBuff.Write(decBuff.GetData(),decBuff.GetSize());
		peBuff.Seek(0);
		peBuff.Write(pecnt);
		fileBuff.Write(peBuff.GetData(),peBuff.GetSize());

		//
		pFile->Write(fileBuff.GetData(),fileBuff.GetSize());
		delete pFile;

		map.m_FileName = fname;

		return true;
	} catch(LPCTSTR msg) {
		MessageBox(NULL,msg,_T("Error"),MB_OK);
		delete pFile;
		return false;
	}
	return true;
}
