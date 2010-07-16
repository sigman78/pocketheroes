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

const sint32 SBUFF_ALIGN = 32;
const fix32 CELL_EVT_DELAY = fix32(2.0f);

iOLComposer::iOLComposer(const iRect& rc, iView* pOwner)
: m_Rect(rc), m_pOwner(pOwner), m_lastCompAnchor(cInvalidPoint), m_Anchor(0,0), m_pAniObj(NULL), m_ptTarget(cInvalidPoint), m_bSurveyMode(false)
{}

iOLComposer::~iOLComposer()
{ Cleanup(); }

bool iOLComposer::Init()
{
	return true;
}

void iOLComposer::Cleanup()
{
	m_ComposeList.Cleanup();
	if (m_pAniObj) {
		delete m_pAniObj;
		m_pAniObj = NULL;
	}
}

void iOLComposer::SetAnchor(const iPoint& pos) 
{ 
	iPoint ccell = GetCenterCell(pos);
	// Valid range is (-1,-1, MapWidth, MapHeight)
	if (ccell.x >= -1 && ccell.y >= -1 && ccell.x <= gGame.Map().GetWidth() && ccell.y <= gGame.Map().GetHeight()) {
		m_Anchor = pos;
	}
	m_pOwner->Invalidate();
}

iPoint iOLComposer::GetCenterCell(const iPoint& anchor) const
{
	iIsoMetric im(5);
	return im.Screen2Map(iPoint(anchor.x + m_Rect.w/2 - MapPixW()/2-20,anchor.y+m_Rect.h/2-30));
}

iPoint iOLComposer::GetCenterViewAnchor(const iPoint& cell, const iPoint& offset)
{
	iIsoMetric im(5);
	iPoint cpos = im.Map2Screen(cell);
	cpos.x += MapPixW()/2+40;
	cpos.y += 40;
	cpos.x -= m_Rect.w/2;
	cpos.y -= m_Rect.h/2;
	return cpos + offset;
}

iPoint iOLComposer::GetCenterCell() const
{
	return GetCenterCell(m_Anchor);
}

void iOLComposer::CenterView(const iPoint& cell, const iPoint& offset)
{
	SetAnchor(GetCenterViewAnchor(cell, offset));
}

// Animation CB
void iOLComposer::AniDone()
{
	iAniObj* pAniObj = RemoveAniObj();
	if (iAniObj_MovHero* pHeroAni = DynamicCast<iAniObj_MovHero*>(pAniObj)) {
		// Need to center view with new hero's position (after step)
		pHeroAni->Hero()->Step();
	}

	delete pAniObj;
}

bool iOLComposer::StepAni(fix32 t)
{
	bool bRedraw = false;
	
	// animation
	if (m_pAniObj) {
		m_pAniObj->Process(t);
		if (iAniObj_MovHero* pHeroAni = DynamicCast<iAniObj_MovHero*>(m_pAniObj)) {
			CenterView(pHeroAni->Hero()->Pos(), pHeroAni->PtOffset());
		}
		bRedraw = true;
	}

	// cell events
	iEvtIt it=m_cellEvents.First();
	while(it != m_cellEvents.End()) {
		if (!bRedraw) bRedraw = true;
		(*it)->time -= t;
		if ( (*it)->time < fix32::zero){
			it = m_cellEvents.Delete(it);
			continue;
		}
		(*it)->dstate = (uint8)iCLAMP<sint32>(0,255,((*it)->time * fix32(255.0f) / CELL_EVT_DELAY).ceil());
		++it;
	}

	return bRedraw;
}

void iOLComposer::SetAniObj(iAniObj* pObj)
{
	check(pObj && !m_pAniObj); 
	m_pAniObj = pObj; 
}

iAniObj* iOLComposer::RemoveAniObj()
{
	check(m_pAniObj); 
	iAniObj* pRes = m_pAniObj; 
	m_pAniObj=NULL; 
	return pRes; 
}

void iOLComposer::AddCellMsg(const iStringT& msg, const iPoint& pos)
{
	m_cellEvents.Add(new iGameEvent(CELL_EVT_DELAY, msg, pos));
}

// iPoint	m_Anchor;
// iRect	m_Rect;

always_inline uint32 CalcCellIndex(const iPoint& pos)
{
	return (pos.x + pos.y)*10 + pos.x;
}

void iOLComposer::Compose()
{
	iTimer timer;

	iDib& outDib = (m_bSurveyMode)?m_dibSurveyBuff:gApp.Surface();
	iRect orc;
	orc = m_Rect;

	//
	iPoint anchor = m_Anchor-iPoint(MapPixW()/2+20+40,40);
	iPoint offset = iPoint(MapPixW()/2+20,40) - m_Anchor + orc.point();

	iItemMgr& itemMgr = gGame.ItemMgr();
	iIsoMetric im(5);
	// save cell under anchor point
	iPoint fpos = im.Screen2Map(anchor-iPoint(SBUFF_ALIGN,SBUFF_ALIGN));
	// offset cell
	fpos.Move(-1,-1);
	iPoint ppos = im.Map2Screen(fpos)+iPoint(SBUFF_ALIGN,SBUFF_ALIGN);
	ppos.Move(-40,0);
	iPoint offs = anchor-ppos;

	//
	// Surface
	iSize dsSiz(orc.w+SBUFF_ALIGN*2,orc.h+SBUFF_ALIGN*2);
	if (m_dibSurfLayer.GetSize() != dsSiz){
		m_lastCompAnchor = cInvalidPoint;
		m_dibSurfLayer.Resize(dsSiz);
	}

	sint32 cnt_x = dsSiz.w/40+3;
	sint32 cnt_y = dsSiz.h/10+5;

	sint32 step_x = im.GetCellStepX()*2;
	sint32 step_y = im.GetCellStepY();
	
	const iPoint anchor_coor(fpos);
	iPoint init_map_coor(fpos);
	iPoint map_coor(fpos);


	if (m_lastCompAnchor == cInvalidPoint || iDIF(anchor.x,m_lastCompAnchor.x) > SBUFF_ALIGN || iDIF(anchor.y,m_lastCompAnchor.y) > SBUFF_ALIGN) {
		m_lastCompAnchor = anchor;
		m_dibSurfLayer.Fill(0);
		for (sint32 yy=0; yy<cnt_y; ++yy) {
			for (sint32 xx=0; xx<cnt_x; ++xx){
				if (gGame.Map().IsValidPos(map_coor.x,map_coor.y)){
					sint32 xpos = xx*step_x;
					sint32 ypos = yy*step_y;
					if (yy%2!=0) xpos -= step_x >> 1;
					iPoint op(xpos-offs.x,ypos-offs.y);

					// Surf
					iGameMap::iSurfCell cell;
					uint32 cseq = CalcCellSeq(map_coor,4);
					if ( gGame.Map().GetCellSurf(map_coor, cell) ) {
						// solid cell - do nothing
						gGfxMgr.BlitMasked(PDGG_SURF_TILES + cell.lowestLayer*4 + cseq, PDGG_TRANS_TILES+14,m_dibSurfLayer,op);
					} else {
						if (cell.lowestLayer == STYPE_WATER) {
							uint8 cfg = 0;
							for (uint32 zz=0; zz<STYPE_COUNT; ++zz) cfg |= cell.layers[zz];
							gGfxMgr.Blit(PDGG_WS_TILES + (cfg-1), m_dibSurfLayer,op);
						} else {
							gGfxMgr.BlitMasked(PDGG_SURF_TILES + cell.lowestLayer * 4 + cseq, PDGG_TRANS_TILES+14,m_dibSurfLayer,op);
						}
						for (uint32 zz=2; zz<STYPE_COUNT; ++zz) {
							if (cell.layers[zz]) {
								gGfxMgr.BlitMasked(PDGG_SURF_TILES + zz*4 + cseq, PDGG_TRANS_TILES+(cell.layers[zz]-1),m_dibSurfLayer,op);
							}
						}
					}
					// Grid
					if (gSettings.GetEntryValue(CET_OVRLANDGRID)) gGfxMgr.BlitAlpha( PDGG_GRID_CELL, m_dibSurfLayer, iPoint(op.x,op.y), 32);
				}
				map_coor.x += 1;
				map_coor.y -= 1;
			}
			if (yy%2 != 0) {
				init_map_coor.x+=1;
				map_coor = init_map_coor;
			} else {
				init_map_coor.y+=1;
				map_coor = init_map_coor;
			}
		}
	}

	m_dibSurfLayer.CopyToDibXY(outDib,orc.point()-iPoint(SBUFF_ALIGN,SBUFF_ALIGN)-(anchor-m_lastCompAnchor));

	//
	// Prepare item list to draw
	//
	m_ComposeList.Cleanup();
	m_ComposeList.SetComposeRect(orc);
	iSimpleArray<iBaseMapObject*> objList;
	{
		init_map_coor = iPoint(fpos.x - 5, fpos.y - 3);
		map_coor = init_map_coor;
		cnt_x += dsSiz.w/40+10;
		cnt_y += dsSiz.h/10+15;
		for (sint32 yy=0; yy<cnt_y; ++yy) {
			for (sint32 xx=0; xx<cnt_x; ++xx) {
				iPoint op = im.Map2Screen(map_coor) + offset;

				if (gGame.Map().IsValidPos(map_coor.x,map_coor.y)) {
					uint16 avatarId = gGame.Map().GetAvatar(map_coor.x, map_coor.y);
					if ( avatarId != 0xFFFF){
						// Avatars
						if ( ( avatarId & 0xFF00) == 0xFF00) {
							// path element
							iPathElT* pPe = itemMgr.m_PathElProts[avatarId & 0xFF];
							SpriteId sid = pPe->SpriteConfig(gGame.Map().PathElConfig(map_coor.x, map_coor.y));
							m_ComposeList.AddItem(sid, op, gGfxMgr.Shadow(sid), pPe->Level(), CalcCellIndex(map_coor));
						} else {
							// Basic avatar
							const iAvatar& avatar = itemMgr.m_AvatarMgr[avatarId];
							for (uint32 sid=0; sid<avatar.SpritesCount(); ++sid){
								m_ComposeList.AddItem(avatar.GetSpriteId(sid),op+avatar.GetSpriteOffset(sid),gGfxMgr.Shadow(avatar.GetSpriteId(sid)),avatar.GetSpriteLevel(sid), CalcCellIndex(map_coor));
							}
						}
					} 
					if (iBaseMapObject* pMapObj = gGame.Map().m_CoverMap.GetAt(map_coor.x, map_coor.y)) {
						// Objects
						if (pMapObj->Pos() == map_coor) {
							if (iHero* pHero = DynamicCast<iHero*>(pMapObj)) {
								if (pHero->GetLocation()) objList.Add(pHero->GetLocation());
							}
							objList.Add(pMapObj);
						}
					}
				} else {
					// Map border
					sint32 w = (sint32)gGame.Map().GetWidth();
					sint32 h = (sint32)gGame.Map().GetHeight();
					sint32 dx = 0, dy = 0;
					if (map_coor.x < 0) dx = map_coor.x;
					else if (map_coor.x >= w) dx = map_coor.x - w + 1;
					if (map_coor.y < 0) dy = map_coor.y;
					else if (map_coor.y >= h) dy = map_coor.y - h + 1;
					if (dx || dy) {
						uint32 val = CalcCellIndex(map_coor);
						SpriteId sid = PDGG_MAP_BORDER;
						if (dx == 1 && !dy) {
							if (map_coor.y == 0) sid += 0;
							else sid +=7;
						} else if (dx == 2 && !dy && map_coor.y != 0) {
							sid += 6;
						} else if (dy == 1 && !dx) {
							if (map_coor.x == 0) sid += 1;
							else sid +=2;
						} else if (dy == 2 && !dx && map_coor.x != 0) {
							sid += 3;
						} else if (dx == 1 && dy == 1) {
							sid += 4;
						} else if (dx == 2 && dy == 2) {
							sid += 5;
						} else if (dx == 1 && dy == 2) {
							sid += 3;
						} else if (dx == 2 && dy == 1) {
							sid += 6;
						} else sid += 10 + CalcCellSeq(map_coor,4);
						m_ComposeList.AddItem(sid, op, 0, SPL_OBJECT, val);
					}
				}

				map_coor.x += 1;
				map_coor.y -= 1;
			}
			if (yy%2 != 0) {
				init_map_coor.x+=1;
				map_coor = init_map_coor;
			} else {
				init_map_coor.y+=1;
				map_coor = init_map_coor;
			}
		}
	}

	for (uint32 nn=0; nn<objList.GetSize(); ++nn) {
		iBaseMapObject* pMapObj = objList[nn];
		if (iVisCnst* pVisCnst = DynamicCast<iVisCnst*>(pMapObj)) {
			// Visitables
			const iVisCnstT* pCnstT = pVisCnst->Proto();
			for (uint32 cvr=0; cvr<pCnstT->AvatarsCount(); ++cvr){
				const iAvatar& avatar = itemMgr.m_AvatarMgr[pCnstT->AvatarEntry(cvr).aid];
				iPoint cpos(pVisCnst->Pos().x+pCnstT->AvatarEntry(cvr).ox, pVisCnst->Pos().y+pCnstT->AvatarEntry(cvr).oy);
				for (uint32 sid=0; sid<avatar.SpritesCount(); ++sid){
					iPoint opos = im.Map2Screen(cpos) + offset + avatar.GetSpriteOffset(sid);
					m_ComposeList.AddItem(avatar.GetSpriteId(sid),opos,gGfxMgr.Shadow(avatar.GetSpriteId(sid)),avatar.GetSpriteLevel(sid), CalcCellIndex(iPoint(cpos.x,cpos.y)));
				}
			}
		} else if (iOwnCnst* pOwnCnst = DynamicCast<iOwnCnst*>(pMapObj)) {
			// Ownerables
			const iOwnCnstT* pCnstT = pOwnCnst->Proto();
			for (uint32 cvr=0; cvr<pCnstT->AvatarsCount(); ++cvr){
				const iAvatar& avatar = itemMgr.m_AvatarMgr[pCnstT->AvatarEntry(cvr).aid];
				iPoint cpos(pOwnCnst->Pos().x+pCnstT->AvatarEntry(cvr).ox, pOwnCnst->Pos().y+pCnstT->AvatarEntry(cvr).oy);
				for (uint32 sid=0; sid<avatar.SpritesCount(); ++sid){
					iPoint opos = im.Map2Screen(cpos) + offset + avatar.GetSpriteOffset(sid);
					m_ComposeList.AddItem(avatar.GetSpriteId(sid),opos,gGfxMgr.Shadow(avatar.GetSpriteId(sid)),avatar.GetSpriteLevel(sid), CalcCellIndex(iPoint(cpos.x,cpos.y)));
				}
				if (pOwnCnst->Owner() != PID_NEUTRAL && !gGame.Map().ActPlayer()->FogMap().IsHidden(cpos)) {
					SpriteId fsid = PDGG_FLAGS + pOwnCnst->Owner();
					for (uint32 fid=0; fid<avatar.FlagsCount(); ++fid) {
						iPoint opos = im.Map2Screen(cpos) + offset + iPoint(avatar.FlagEntry(fid).ox,avatar.FlagEntry(fid).oy);
						m_ComposeList.AddItem(fsid,opos,0,SPL_OBJECT,CalcCellIndex(cpos));
					}
				}
			}
		} else if (iCastle* pCastle = DynamicCast<iCastle*>(pMapObj)) {
			// Castles
			const iCastleT* pCastleT = pCastle->Proto();
			for (uint32 cvr=0; cvr<pCastleT->AvatarsCount(); ++cvr){
				const iAvatar& avatar = itemMgr.m_AvatarMgr[pCastleT->AvatarEntry(cvr).aid];
				iPoint cpos(pCastle->Pos().x+pCastleT->AvatarEntry(cvr).ox, pCastle->Pos().y+pCastleT->AvatarEntry(cvr).oy);
				for (uint32 sid=0; sid<avatar.SpritesCount(); ++sid){
					iPoint opos = im.Map2Screen(cpos) + offset + avatar.GetSpriteOffset(sid);
					m_ComposeList.AddItem(avatar.GetSpriteId(sid),opos,gGfxMgr.Shadow(avatar.GetSpriteId(sid)),avatar.GetSpriteLevel(sid), CalcCellIndex(iPoint(cpos.x,cpos.y)));
				}
				if (pCastle->Owner() != PID_NEUTRAL && !gGame.Map().ActPlayer()->FogMap().IsHidden(cpos)) {
					SpriteId fsid = PDGG_FLAGS + pCastle->Owner();
					for (uint32 fid=0; fid<avatar.FlagsCount(); ++fid) {
						iPoint opos = im.Map2Screen(cpos) + offset + iPoint(avatar.FlagEntry(fid).ox,avatar.FlagEntry(fid).oy);
						m_ComposeList.AddItem(fsid,opos,0,SPL_OBJECT,CalcCellIndex(cpos));
					}
				}
			}
		} else if (iHero* pHero = DynamicCast<iHero*>(pMapObj)) {
			// Heroes
			bool bShow = (gGame.Map().ActPlayer() && !gGame.Map().ActPlayer()->FogMap().IsHidden(pHero->Pos()));
			if (bShow){
				if ( !pHero->Moving() ){
					iPoint opos = im.Map2Screen(pHero->Pos()) + offset;
					SpriteId fsid = PDGG_FLAGS + pHero->Owner()->PlayerId();
					if (pHero->Angle()>3) fsid += PID_COUNT;
					SpriteId hsid = GetHeroSprite(pHero);
					// Hero
					m_ComposeList.AddItem(hsid,opos,1,SPL_OBJECT,CalcCellIndex(pHero->Pos()));
					m_ComposeList.AddItem(fsid,opos + HERO_FLAG_ANCHOR[pHero->Angle()*9]+iPoint(-5,-41),0,SPL_OBJECT,CalcCellIndex(pHero->Pos()));
					// Hero fore layer (only for current hero)
					if (pHero == gGame.Map().CurHero()) {
						m_ComposeList.AddItem(hsid,opos,0,SPL_OBJECT,CalcCellIndex(pHero->Pos())+1024, 32);
						m_ComposeList.AddItem(fsid,opos + HERO_FLAG_ANCHOR[pHero->Angle()*9]+iPoint(-5,-41),0,SPL_OBJECT,CalcCellIndex(pHero->Pos())+1024);
					}
				}
			}
		} else if (iMapGuard* pMapGuard = DynamicCast<iMapGuard*>(pMapObj)) {
			if (gGame.Map().ActPlayer() && !gGame.Map().ActPlayer()->FogMap().IsHidden(pMapGuard->Pos())){
				iPoint opos = im.Map2Screen(pMapGuard->Pos()) + offset;
				m_ComposeList.AddItem(pMapGuard->Sprite(),opos,1,SPL_OBJECT,CalcCellIndex(pMapGuard->Pos()));
			}
		} else if (DynamicCast<iMapItem_Mineral*>(pMapObj) || DynamicCast<iMapItem_ManaCrystal*>(pMapObj) || DynamicCast<iMapItem_CampFire*>(pMapObj) || DynamicCast<iMapItem_Chest*>(pMapObj) || DynamicCast<iMapItem_Artifact*>(pMapObj) || DynamicCast<iMapItem_Lamp*>(pMapObj) || DynamicCast<iMapItem_SpellScroll*>(pMapObj) || DynamicCast<iMapItem_KeyGuard*>(pMapObj)) {
			iMapItem* pMapItem = (iMapItem*)(pMapObj);
			if (!gGame.Map().ActPlayer()->FogMap().IsHidden(pMapItem->Pos())){
				iPoint opos = im.Map2Screen(pMapItem->Pos()) + offset;
				m_ComposeList.AddItem(pMapItem->Sprite(),opos,1,SPL_OBJECT,CalcCellIndex(pMapItem->Pos()));
			}
		} else {
			check(0);
		}
	}

	// Cell animation
	if (m_pAniObj && m_pAniObj->Type() == iAniObj::CellAni){
		iPoint opos = im.Map2Screen(m_pAniObj->Pos()) + offset;
		if ( iAniObj_MovHero* pHeroAni = DynamicCast<iAniObj_MovHero*>(m_pAniObj) ){
			SpriteId spOffset=0;
			iPoint	ptOffset;
			pHeroAni->GetAniInfo(spOffset, ptOffset);
			opos += ptOffset;
			SpriteId fsid = PDGG_FLAGS + pHeroAni->Hero()->Owner()->PlayerId();
			if (pHeroAni->Hero()->Angle()>3) fsid += PID_COUNT;
			SpriteId hsid = GetHeroSprite(pHeroAni->Hero()) + spOffset;
			m_ComposeList.AddItem(hsid,opos,1,SPL_OBJECT,CalcCellIndex(pHeroAni->TargetPos()));
			m_ComposeList.AddItem(fsid,opos + HERO_FLAG_ANCHOR[pHeroAni->Hero()->Angle()*9+spOffset]+iPoint(-5,-41),0,SPL_OBJECT,CalcCellIndex(pHeroAni->TargetPos()));
		} else if ( iAniObj_FadeOut* pFadeAni = DynamicCast<iAniObj_FadeOut*>(m_pAniObj) ) {
			SpriteId sid;
			uint8 a;
			pFadeAni->GetAniInfo(sid, a);
			m_ComposeList.AddItem(sid,opos,0,SPL_OBJECT,CalcCellIndex(pFadeAni->Pos()),a);
		} else {
			check(0 == "Unknown cell animation type!");
		}
	}

	// Path
	sint32 movAcc=0;
	if (gGame.Map().CurHero() && gGame.Map().IsCurPlayerActive()){
		iHero* pCurHero = gGame.Map().CurHero();
		movAcc=pCurHero->Moves();
		if (pCurHero->Path().m_Path.GetSize()>1){
			uint32 fidx = 0;
			if (pCurHero->Moving()) {
				movAcc -= pCurHero->Path().GetStepCost();
				fidx ++;
			}
			for (uint32 xx=fidx; xx<pCurHero->Path().m_Path.GetSize()-1; ++xx) {
				movAcc -= pCurHero->Path().m_Path[xx].m_Cost;
				iPoint opos = im.Map2Screen(pCurHero->Path().m_Path[xx].m_Pos) + offset;
				m_ComposeList.AddItem((movAcc>=0)?PDGG_PATH:(PDGG_PATH+6),opos,2,SPL_OBJECT,CalcCellIndex(pCurHero->Path().m_Path[xx].m_Pos));
			}
		}
	}
	
	// 
	// Draw ground-level sprites
	for (uint32 cId=0; cId<m_ComposeList.Size(); ++cId){
		if (m_ComposeList[cId].value.m_level == SPL_GROUND) {
			if (m_ComposeList[cId].value.Solid()) gGfxMgr.Blit( m_ComposeList[cId].value.m_sid, outDib, m_ComposeList[cId].value.m_pos);
			else gGfxMgr.BlitAlpha( m_ComposeList[cId].value.m_sid, outDib, m_ComposeList[cId].value.m_pos, m_ComposeList[cId].value.m_alpha);
		}
	}
	//
	// Draw shadow
	for (uint32 cId=0; cId<m_ComposeList.Size(); ++cId){
		if (uint8 sh = m_ComposeList[cId].value.m_shadow ) {
			gGfxMgr.BlitEffect( m_ComposeList[cId].value.m_sid, outDib, m_ComposeList[cId].value.m_pos, (sh==1)?iGfxManager::EfxShadowIso:iGfxManager::EfxShadow2D);
		}
	}

	//
	// Draw other sprites
	for (uint32 cId=0; cId<m_ComposeList.Size(); ++cId){
		if (m_ComposeList[cId].value.m_level != SPL_GROUND) {
			if (m_ComposeList[cId].value.Solid()) gGfxMgr.Blit( m_ComposeList[cId].value.m_sid, outDib, m_ComposeList[cId].value.m_pos);
			else gGfxMgr.BlitAlpha( m_ComposeList[cId].value.m_sid, outDib, m_ComposeList[cId].value.m_pos, m_ComposeList[cId].value.m_alpha);
		}
	}

	//FillStaredRect(outDib, outDib.GetSize(), anchor);

	//
	// Fog
	init_map_coor = anchor_coor;
	map_coor = anchor_coor;
	for (sint32 yy=0; yy<cnt_y; ++yy) {
		for (sint32 xx=0; xx<cnt_x; ++xx){
			if (gGame.Map().ActPlayer() && gGame.Map().ActPlayer()->FogMap().IsValidCell(map_coor)){
				sint32 xpos = xx*step_x;
				sint32 ypos = yy*step_y;
				if (yy%2!=0) xpos -= step_x >> 1;
				iPoint op(xpos-offs.x+orc.x-SBUFF_ALIGN ,ypos-offs.y+orc.y-SBUFF_ALIGN);
				// Fog
				uint8 dcfg, fcfg;
				if (!gGame.Map().ActPlayer()->FogMap().GetFogCfg(map_coor,dcfg,fcfg)){
					if (fcfg && dcfg != fcfg) gGfxMgr.BlitEffect(PDGG_FOG_TILES+(fcfg-1),outDib,op,iGfxManager::EfxTransparent);
					if (dcfg) gGfxMgr.Blit(PDGG_FOG_TILES+(dcfg-1),outDib,op);
				}
			}
			map_coor.x += 1;
			map_coor.y -= 1;
		}
		if (yy%2 != 0) {
			init_map_coor.x+=1;
			map_coor = init_map_coor;
		} else {
			init_map_coor.y+=1;
			map_coor = init_map_coor;
		}
	}
	// Destination Point
	if (gGame.Map().CurHero() && gGame.Map().IsCurPlayerActive()){
		iHero* pCurHero = gGame.Map().CurHero();
		if (pCurHero->Path().m_Path.GetSize() > 0){
			iPath::iStep pstep = pCurHero->Path().m_Path.GetLast();
			movAcc -= pstep.m_Cost;
			iPoint pos = pstep.m_Pos;
			uint32 psid = PDGG_PATH;
			if (movAcc < 0) psid += 6;
			psid += pCurHero->Path().TargetAction();
			iPoint opos = im.Map2Screen(pos)+offset;
		
			gGfxMgr.BlitEffect( psid, outDib, opos, iGfxManager::EfxShadow2D);
			gGfxMgr.Blit( psid, outDib, opos);
			
			if (gSettings.GetEntryValue(CET_TRVPTSMODE)) {
				iTextComposer::FontConfig fc(iTextComposer::FS_MEDIUM, iDibFont::ComposeProps(cColor_White, cColor_Black, iDibFont::DecBorder));
				opos.Move(28,14);
				gTextComposer.TextOut(fc,outDib,opos,iFormat(_T("%d (%d)"),pCurHero->Path().PathCost(),pCurHero->Moves()));
			} else {
				if (pCurHero->Path().PathCost() > pCurHero->Moves()) {
					iTextComposer::FontConfig fc(iTextComposer::FS_MEDIUM, iDibFont::ComposeProps(RGB16(255,192,0), cColor_Black, iDibFont::DecBorder));
					opos.Move(25,11);
					sint32 steps = (pCurHero->Path().PathCost() - pCurHero->Moves()) / pCurHero->TotalMoves() + 2;
					gTextComposer.TextOut(fc,outDib,opos,iFormat(_T("%d"),steps));
				}
			}
		}
	}

	// Target point
	if (m_ptTarget != cInvalidPoint) {
		iPoint opos = im.Map2Screen(m_ptTarget)+offset;
		gGfxMgr.BlitEffect( m_crTarget, outDib, opos, iGfxManager::EfxShadow2D);
		gGfxMgr.Blit( m_crTarget, outDib, opos);
	}

	// cell events
	for (iEvtCIt it=m_cellEvents.First(); it != m_cellEvents.End(); ++it) {
		iPoint pos = im.Map2Screen((*it)->pos) + offset;;
		pos.x += im.GetCellWidth()/2;
		pos.y -= 10 + (255 - (*it)->dstate) / 7;
		uint8 a = (*it)->dstate;
		if (a < 128) a *= 2; else a = 255;
		iTextComposer::FontConfig fc(iTextComposer::FS_MEDIUM, iDibFont::ComposeProps(defGrad, cColor_Black, iDibFont::DecBorder));
		gTextComposer.TextOut(fc, outDib, pos, iFormat(_T("#A%02X"),a)+(*it)->text, iRect(pos,iSize()), AlignTop);
	}

	// Full screen animation
	if (m_pAniObj && m_pAniObj->Type() == iAniObj::FullScreenAni) {
		if ( iAniObj_Teleport* pTeleportAni = DynamicCast<iAniObj_Teleport*>(m_pAniObj) ) {
			pTeleportAni->Compose(outDib);
		} else {
			check(0 == "Unknown cell animation type!");
		}
	}
}

void iOLComposer::Compose(iDib& outDib, const iPoint& out_anchor, bool bPuzzleMap, const iPoint& tgtCell)
{
	//
	iPoint anchor = out_anchor-iPoint((gGame.Map().GetWidth() * 40)/2+20+40,40);
	iPoint offset = iPoint((gGame.Map().GetWidth() * 40)/2+20,40) - out_anchor;

	iItemMgr& itemMgr = gGame.ItemMgr();
	iIsoMetric im(5);
	// save cell under anchor point
	iPoint fpos = im.Screen2Map(anchor);
	// offset cell
	fpos.Move(-1,-1);
	iPoint ppos = im.Map2Screen(fpos);
	ppos.Move(-40,0);
	iPoint offs = anchor-ppos;

	//
	// Surface
	sint32 cnt_x = outDib.GetWidth()/40+3;
	sint32 cnt_y = outDib.GetHeight()/10+5;

	sint32 step_x = im.GetCellStepX()*2;
	sint32 step_y = im.GetCellStepY();
	
	const iPoint anchor_coor(fpos);
	iPoint init_map_coor(fpos);
	iPoint map_coor(fpos);

	sint32 xx,yy;

	for (yy=0; yy<cnt_y; ++yy) {
		for (xx=0; xx<cnt_x; ++xx){
			if (gGame.Map().IsValidPos(map_coor.x,map_coor.y)){
				sint32 xpos = xx*step_x;
				sint32 ypos = yy*step_y;
				if (yy%2!=0) xpos -= step_x >> 1;
				iPoint op(xpos-offs.x,ypos-offs.y);

				// Surf
				iGameMap::iSurfCell cell;
				uint32 cseq = CalcCellSeq(map_coor,4);
				if ( gGame.Map().GetCellSurf(map_coor, cell) ) {
					// solid cell - do nothing
					gGfxMgr.BlitMasked(PDGG_SURF_TILES + cell.lowestLayer*4 + cseq, PDGG_TRANS_TILES+14,outDib,op);
				} else {
					if (cell.lowestLayer == STYPE_WATER) {
						uint8 cfg = 0;
						for (uint32 zz=0; zz<STYPE_COUNT; ++zz) cfg |= cell.layers[zz];
						gGfxMgr.Blit(PDGG_WS_TILES + (cfg-1), outDib,op);
					} else {
						gGfxMgr.BlitMasked(PDGG_SURF_TILES + cell.lowestLayer * 4 + cseq, PDGG_TRANS_TILES+14,outDib,op);
					}
					for (uint32 zz=2; zz<STYPE_COUNT; ++zz) {
						if (cell.layers[zz]) {
							gGfxMgr.BlitMasked(PDGG_SURF_TILES + zz*4 + cseq, PDGG_TRANS_TILES+(cell.layers[zz]-1),outDib,op);
						}
					}
				}
			}
			map_coor.x += 1;
			map_coor.y -= 1;
		}
		if (yy%2 != 0) {
			init_map_coor.x+=1;
			map_coor = init_map_coor;
		} else {
			init_map_coor.y+=1;
			map_coor = init_map_coor;
		}
	}

	//
	// Prepare item list to draw
	iComposeList clist;
	clist.SetComposeRect(outDib.GetSize());

	// Avatars
	for (yy=0; yy<gGame.Map().GetHeight(); ++yy) {
		for (xx=0; xx<gGame.Map().GetWidth(); ++xx){
			uint16 avatarId = gGame.Map().GetAvatar(xx,yy);
			if ( avatarId != 0xFFFF){
				iPoint opos = im.Map2Screen(iPoint(xx,yy)) + offset;
				if ( ( avatarId & 0xFF00) == 0xFF00) {
					// path element
					if (!bPuzzleMap) {
						iPathElT* pPe = itemMgr.m_PathElProts[avatarId & 0xFF];
						SpriteId sid = pPe->SpriteConfig(gGame.Map().PathElConfig(xx, yy));
						clist.AddItem(sid, opos, gGfxMgr.Shadow(sid), pPe->Level(), CalcCellIndex(iPoint(xx,yy)));
					}
				} else {
					// avatar
					const iAvatar& avatar = itemMgr.m_AvatarMgr[avatarId];
					for (uint32 sid=0; sid<avatar.SpritesCount(); ++sid){
						clist.AddItem(avatar.GetSpriteId(sid),opos+avatar.GetSpriteOffset(sid),gGfxMgr.Shadow(avatar.GetSpriteId(sid)),avatar.GetSpriteLevel(sid), CalcCellIndex(iPoint(xx,yy)));
					}
				}
			}
		}
	}

	if (!bPuzzleMap) {

		// Visitables
		for (iGameWorld::iVCIt vcit = gGame.Map().m_VisCnsts.First(); vcit != gGame.Map().m_VisCnsts.End(); ++vcit) {
			iVisCnst* pCnst = *vcit;
			const iVisCnstT* pCnstT = pCnst->Proto();
			for (uint32 cvr=0; cvr<pCnstT->AvatarsCount(); ++cvr){
				const iAvatar& avatar = itemMgr.m_AvatarMgr[pCnstT->AvatarEntry(cvr).aid];
				iPoint cpos(pCnst->Pos().x+pCnstT->AvatarEntry(cvr).ox, pCnst->Pos().y+pCnstT->AvatarEntry(cvr).oy);
				for (uint32 sid=0; sid<avatar.SpritesCount(); ++sid){
					iPoint opos = im.Map2Screen(cpos) + offset + avatar.GetSpriteOffset(sid);
					clist.AddItem(avatar.GetSpriteId(sid),opos,gGfxMgr.Shadow(avatar.GetSpriteId(sid)),avatar.GetSpriteLevel(sid), CalcCellIndex(iPoint(cpos.x,cpos.y)));
				}
			}
			// Guards
			if (!gGame.Map().ActPlayer()->FogMap().IsHidden(pCnst->Pos()) && !pCnst->Guard().Empty()) {
				iPoint opos = im.Map2Screen(pCnst->Pos()) + offset;
				clist.AddItem(PDGG_MINIMON+pCnst->Guard().StrongestCreature(),opos,0,SPL_OBJECT,CalcCellIndex(pCnst->Pos()));
			}
		}

		// Neutral ownerables
		for (iGameWorld::iOCIt ocit = gGame.Map().m_OwnCnsts.First(); ocit != gGame.Map().m_OwnCnsts.End(); ++ocit) {
			iOwnCnst* pCnst = *ocit;
			const iOwnCnstT* pCnstT = pCnst->Proto();
			for (uint32 cvr=0; cvr<pCnstT->AvatarsCount(); ++cvr){
				const iAvatar& avatar = itemMgr.m_AvatarMgr[pCnstT->AvatarEntry(cvr).aid];
				iPoint cpos(pCnst->Pos().x+pCnstT->AvatarEntry(cvr).ox, pCnst->Pos().y+pCnstT->AvatarEntry(cvr).oy);
				for (uint32 sid=0; sid<avatar.SpritesCount(); ++sid){
					iPoint opos = im.Map2Screen(cpos) + offset + avatar.GetSpriteOffset(sid);
					clist.AddItem(avatar.GetSpriteId(sid),opos,gGfxMgr.Shadow(avatar.GetSpriteId(sid)),avatar.GetSpriteLevel(sid), CalcCellIndex(iPoint(cpos.x,cpos.y)));
				}
			}
			// Guards
			if (!gGame.Map().ActPlayer()->FogMap().IsHidden(pCnst->Pos()) && !pCnst->Guard().Empty()) {
				iPoint opos = im.Map2Screen(pCnst->Pos()) + offset;
				clist.AddItem(PDGG_MINIMON+pCnst->Guard().StrongestCreature(),opos,0,SPL_OBJECT,CalcCellIndex(pCnst->Pos()));
			}
		}

		// Neutral castles
		for (iGameWorld::iCtIt ctit = gGame.Map().m_Castles.First(); ctit != gGame.Map().m_Castles.End(); ++ctit) {
			iCastle* pCastle = *ctit;
			const iCastleT* pCastleT = pCastle->Proto();
			for (uint32 cvr=0; cvr<pCastleT->AvatarsCount(); ++cvr){
				const iAvatar& avatar = itemMgr.m_AvatarMgr[pCastleT->AvatarEntry(cvr).aid];
				iPoint cpos(pCastle->Pos().x+pCastleT->AvatarEntry(cvr).ox, pCastle->Pos().y+pCastleT->AvatarEntry(cvr).oy);
				for (uint32 sid=0; sid<avatar.SpritesCount(); ++sid){
					iPoint opos = im.Map2Screen(cpos) + offset + avatar.GetSpriteOffset(sid);
					clist.AddItem(avatar.GetSpriteId(sid),opos,gGfxMgr.Shadow(avatar.GetSpriteId(sid)),avatar.GetSpriteLevel(sid), CalcCellIndex(iPoint(cpos.x,cpos.y)));
				}
			}
		}

		// Ownerables and castles flags
		iGameWorld::iPLIt pit;
		for (pit = gGame.Map().m_Players.First(); pit != gGame.Map().m_Players.End(); ++pit){
			// Ownerables, its flags, and guards
			for (iPlayer::iCLIt cit = (*pit)->m_Cnsts.First(); cit != (*pit)->m_Cnsts.End(); ++cit){
				iOwnCnst* pCnst = (*cit);
				const iOwnCnstT* pCnstT = pCnst->Proto();
				for (uint32 cvr=0; cvr<pCnstT->AvatarsCount(); ++cvr){
					const iAvatar& avatar = itemMgr.m_AvatarMgr[pCnstT->AvatarEntry(cvr).aid];
					iPoint cpos(pCnst->Pos().x+pCnstT->AvatarEntry(cvr).ox, pCnst->Pos().y+pCnstT->AvatarEntry(cvr).oy);
					for (uint32 sid=0; sid<avatar.SpritesCount(); ++sid){
						iPoint opos = im.Map2Screen(cpos) + offset + avatar.GetSpriteOffset(sid);
						clist.AddItem(avatar.GetSpriteId(sid),opos,gGfxMgr.Shadow(avatar.GetSpriteId(sid)),avatar.GetSpriteLevel(sid), CalcCellIndex(iPoint(cpos.x,cpos.y)));
					}
					if (!gGame.Map().ActPlayer()->FogMap().IsHidden(cpos)) {
						SpriteId fsid = PDGG_FLAGS + pCnst->Owner();
						for (uint32 fid=0; fid<avatar.FlagsCount(); ++fid) {
							iPoint opos = im.Map2Screen(cpos) + offset + iPoint(avatar.FlagEntry(fid).ox,avatar.FlagEntry(fid).oy);
							clist.AddItem(fsid,opos,0,SPL_OBJECT,CalcCellIndex(pCnst->Pos()+1));
						}
					}
				}
				
				// Guards
				if (!gGame.Map().ActPlayer()->FogMap().IsHidden(pCnst->Pos()) && !pCnst->Guard().Empty()) {
					iPoint opos = im.Map2Screen(pCnst->Pos()) + offset;
					clist.AddItem(PDGG_MINIMON+pCnst->Guard().StrongestCreature(),opos,0,SPL_OBJECT,pCnst->Pos().x+pCnst->Pos().y);
				}
			}

			// Castles and its flags
			for (iPlayer::iCtLIt ctit = (*pit)->CastleFirst(); ctit != (*pit)->CastleEnd(); ++ctit){
				iCastle* pCastle = *ctit;
				const iCastleT* pCastleT = pCastle->Proto();
				for (uint32 cvr=0; cvr<pCastleT->AvatarsCount(); ++cvr){
					const iAvatar& avatar = itemMgr.m_AvatarMgr[pCastleT->AvatarEntry(cvr).aid];
					iPoint cpos(pCastle->Pos().x+pCastleT->AvatarEntry(cvr).ox, pCastle->Pos().y+pCastleT->AvatarEntry(cvr).oy);
					for (uint32 sid=0; sid<avatar.SpritesCount(); ++sid){
						iPoint opos = im.Map2Screen(cpos) + offset + avatar.GetSpriteOffset(sid);
						clist.AddItem(avatar.GetSpriteId(sid),opos,gGfxMgr.Shadow(avatar.GetSpriteId(sid)),avatar.GetSpriteLevel(sid), CalcCellIndex(iPoint(cpos.x,cpos.y)));
					}
					if (!gGame.Map().ActPlayer()->FogMap().IsHidden(cpos)) {
						SpriteId fsid = PDGG_FLAGS + pCastle->Owner();
						for (uint32 fid=0; fid<avatar.FlagsCount(); ++fid) {
							iPoint opos = im.Map2Screen(cpos) + offset + iPoint(avatar.FlagEntry(fid).ox,avatar.FlagEntry(fid).oy);
							clist.AddItem(fsid,opos,0,SPL_OBJECT,CalcCellIndex(pCastle->Pos()+1));
						}
					}
				}
			}
		}

		// Heroes
		for (pit = gGame.Map().m_Players.First(); pit != gGame.Map().m_Players.End(); ++pit){
			// Heroes sprites and flags
			for (iPlayer::iHLIt hit = (*pit)->HeroFirst(); hit != (*pit)->HeroEnd(); ++hit){
				bool bShow = (gGame.Map().ActPlayer() && !gGame.Map().ActPlayer()->FogMap().IsHidden((*hit)->Pos()));
				if (bShow){
					if ( !(*hit)->Moving() ){
						iPoint opos = im.Map2Screen((*hit)->Pos()) + offset;
						SpriteId fsid = PDGG_FLAGS + (*hit)->Owner()->PlayerId();
						if ((*hit)->Angle()>3) fsid += PID_COUNT;
						SpriteId hsid = GetHeroSprite(*hit);
						// Hero
						clist.AddItem(hsid,opos,1,SPL_OBJECT,CalcCellIndex((*hit)->Pos()));
						clist.AddItem(fsid,opos + HERO_FLAG_ANCHOR[(*hit)->Angle()*9]+iPoint(-5,-40),0,SPL_OBJECT,CalcCellIndex((*hit)->Pos()));
					}
				}
			}
		}

		// Map Items
		for (iGameWorld::iMIt mit = gGame.Map().m_MapItems.First(); mit != gGame.Map().m_MapItems.End(); ++mit){
			if (!gGame.Map().ActPlayer()->FogMap().IsHidden((*mit)->Pos())){
				iPoint opos = im.Map2Screen(iPoint((*mit)->Pos().x,(*mit)->Pos().y)) + offset;
				clist.AddItem((*mit)->Sprite(),opos,1,SPL_OBJECT,CalcCellIndex((*mit)->Pos()));
			}
		}

		// Map Guards
		for (iGameWorld::iGIt git = gGame.Map().m_MapGuards.First(); git != gGame.Map().m_MapGuards.End(); ++git) {
			iMapGuard* pGuard = *git;
			if (gGame.Map().ActPlayer() && !gGame.Map().ActPlayer()->FogMap().IsHidden((*git)->Pos())){
				iPoint opos = im.Map2Screen(iPoint((*git)->Pos().x,(*git)->Pos().y)) + offset;
				clist.AddItem((*git)->Sprite(),opos,1,SPL_OBJECT,CalcCellIndex((*git)->Pos()));
			}
		}
	}

	// 
	// Draw ground-level sprites
	for (uint32 cId=0; cId<clist.Size(); ++cId){
		if (clist[cId].value.m_level == SPL_GROUND) {
			if (clist[cId].value.Solid()) gGfxMgr.Blit( clist[cId].value.m_sid, outDib, clist[cId].value.m_pos);
			else gGfxMgr.BlitAlpha( clist[cId].value.m_sid, outDib, clist[cId].value.m_pos, clist[cId].value.m_alpha);
		}
	}
	//
	// Draw shadow
	for (uint32 cId=0; cId<clist.Size(); ++cId){
		if (uint8 sh = clist[cId].value.m_shadow ) {
			gGfxMgr.BlitEffect( clist[cId].value.m_sid, outDib, clist[cId].value.m_pos, (sh==1)?iGfxManager::EfxShadowIso:iGfxManager::EfxShadow2D);
		}
	}

	//
	// Draw other sprites
	for (uint32 cId=0; cId<clist.Size(); ++cId){
		if (clist[cId].value.m_level != SPL_GROUND) {
			if (clist[cId].value.Solid()) gGfxMgr.Blit( clist[cId].value.m_sid, outDib, clist[cId].value.m_pos);
			else gGfxMgr.BlitAlpha( clist[cId].value.m_sid, outDib, clist[cId].value.m_pos, clist[cId].value.m_alpha);
		}
	}

	// Destination Point
	if (bPuzzleMap){
		uint32 psid = PDGG_TRACK;
		iPoint opos = im.Map2Screen(tgtCell)+offset;
	
		gGfxMgr.BlitEffect( psid, outDib, opos, iGfxManager::EfxShadow2D);
		gGfxMgr.Blit( psid, outDib, opos);
	}

	//
	// Fog
	if (!bPuzzleMap)  {
		init_map_coor = anchor_coor;
		map_coor = anchor_coor;
		for (yy=0; yy<cnt_y; ++yy) {
			for (sint32 xx=0; xx<cnt_x; ++xx){
				if (gGame.Map().ActPlayer() && gGame.Map().ActPlayer()->FogMap().IsValidCell(map_coor)){
					sint32 xpos = xx*step_x;
					sint32 ypos = yy*step_y;
					if (yy%2!=0) xpos -= step_x >> 1;
					iPoint op(xpos-offs.x ,ypos-offs.y);
					// Fog
					uint8 dcfg, fcfg;
					if (!gGame.Map().ActPlayer()->FogMap().GetFogCfg(map_coor,dcfg,fcfg)){
						if (fcfg && dcfg != fcfg) gGfxMgr.BlitEffect(PDGG_FOG_TILES+(fcfg-1),outDib,op,iGfxManager::EfxTransparent);
						if (dcfg) gGfxMgr.Blit(PDGG_FOG_TILES+(dcfg-1),outDib,op);
					}
				}
				map_coor.x += 1;
				map_coor.y -= 1;
			}
			if (yy%2 != 0) {
				init_map_coor.x+=1;
				map_coor = init_map_coor;
			} else {
				init_map_coor.y+=1;
				map_coor = init_map_coor;
			}
		}	
	}
}


