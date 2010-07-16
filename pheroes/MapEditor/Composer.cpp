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

const uint32 COMPOSE_DIB_STEP = 50;
const uint32 COMPOSE_DIB_MIN = 200;

void ParseCK(iDib& dib, const iColor& ck)
{
	iColor* ptr = dib.GetBits();
	uint32 len = dib.GetWidth()*dib.GetHeight();
	while (len--){
		if (*ptr == ck) ptr->SetA(0);
		ptr ++;
	}
}

bool iComposer::InitComposer(iMapHandler* pmap)
{
	m_pMap=pmap;

	iDibLoader::LoadPng(m_dibSelCell,GetAppPath()+_T("cell_sel.png"));
	iDibLoader::LoadPng(m_dibSelNode,GetAppPath()+_T("node_sel.png"));
	iDibLoader::LoadPng(m_dibGrid,GetAppPath()+_T("cell_grid.png"));
	iDibLoader::LoadPng(m_dibRedCell,GetAppPath()+_T("cell_red.png"));
	iDibLoader::LoadPng(m_dibYelCell,GetAppPath()+_T("cell_yel.png"));

	m_ComposeMemDC.InitDC(iSize(100,100));
	return true;
}

void iComposer::Cleanup()
{
}

bool iComposer::ResizeComposeDib(const iSize &new_siz)
{
	iSize ns( iMAX<uint32>(COMPOSE_DIB_MIN,iALIGN(new_siz.w,COMPOSE_DIB_STEP)),	iMAX<uint32>(COMPOSE_DIB_MIN,iALIGN(new_siz.h,COMPOSE_DIB_STEP)));
	if (ns != m_ComposeMemDC.GetSize()) {
		m_ComposeMemDC.Resize(ns);
		return true;
	}
	return false;
}

void ComposeShadow(const iDib& src, iDib& dst, const iPoint& pos)
{
	uint32 nh = (src.GetHeight()+1) / 2;
	uint32 nw = src.GetWidth() + nh;
	iDib sdib(iSize(nw,nh));

	for (sint32 yy = 0; yy<(sint32)src.GetHeight(); yy+=2){
		sint32 ny = yy/2;
		iColor* pDst = sdib.GetLine(ny) + ny;
		const iColor* pSrc = src.GetLine(yy);
		for (uint32 xx=0; xx<src.GetWidth(); ++xx){
			if (pSrc->GetA()) {
				pDst->SetA(64);
			}
			pDst++; pSrc++;
		}
	}

	sdib.CopyToDibXY(&dst,iPoint(pos.x-nh,pos.y+nh),BLEND_ALPHABLEND);
}

//////////////////////////////////////////////////////////////////////////

/*
struct ComposeItem {
	inline ComposeItem(SpriteId sid, const iPoint& pos, uint8 shadow, SPRITE_LEVEL level, uint8 a)
	: m_sid(sid), m_pos(pos), m_shadow(shadow), m_level(level), m_alpha(a) {}

	inline bool Solid() const
	{ return m_alpha == 255; }

	SpriteId	m_sid;
	iPoint		m_pos;
	uint8		m_shadow;
	uint8		m_level;
	uint8		m_alpha;
};

class iComposeList : public iSortArray<ComposeItem>
{
public:
	iComposeList()
	: m_gfxMgr(gAppCtrl.GuiMgr().GfxManager()) {}

	void AddItem(SpriteId sid, const iPoint& pos, uint8 shadow, SPRITE_LEVEL level, uint32 snum, uint8 a=255)
	{
		check(m_ComposeRect != cInvalidRect);
		iRect srect(pos+m_gfxMgr.Anchor(sid),m_gfxMgr.Dimension(sid));
		if (iClipper::IsIntersectRect(srect,m_ComposeRect)) {
			Insert(ComposeItem(sid,pos,shadow,level, a),snum);
		}
	}

	void Cleanup()
	{
		m_Array.RemoveAll();
		m_ComposeRect = cInvalidRect;
	}

	void SetComposeRect(const iRect& rect)
	{
		m_ComposeRect = rect;
	}

private:
	iRect				m_ComposeRect;
	const iGfxManager&	m_gfxMgr;
};
*/

void iComposer::ComposeAvatar(const iAvatar& avt, const iPoint& pos, bool bShadow, PLAYER_ID pid)
{
	iSpriteMgr& spriteMgr = gItemMgr.m_SpriteMgr;

	// Shadow
	for (uint32 sid=0; sid<avt.SpritesCount(); ++sid) {
		const iSprite* pSprite = spriteMgr.GetSpritePtr(avt.SpriteEntry(sid).spriteId);
		if (bShadow && pSprite->shadow) ComposeShadow(pSprite->dib,m_ComposeMemDC.m_Dib,pos + pSprite->anchor + avt.SpriteEntry(sid).anchor);
	}
	// Sprites
	for (sid=0; sid<avt.SpritesCount(); ++sid) {
		const iSprite* pSprite = spriteMgr.GetSpritePtr(avt.SpriteEntry(sid).spriteId);
		pSprite->dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,pos + pSprite->anchor + avt.SpriteEntry(sid).anchor,BLEND_ALPHABLEND);
	}

	// Flags
	if (pid != PID_NEUTRAL){
		const iDib& fdib = spriteMgr.GetSpritePtr(iFormat(_T("common.flags_%04d"),pid))->dib;
		// Compose flags
		for (uint32 fid=0; fid<avt.FlagsCount(); ++fid){
			iPoint fop = pos + avt.FlagEntry(fid);
			fdib.CopyToDibXY(&m_ComposeMemDC.m_Dib, fop,BLEND_ALPHABLEND);
		}
	}
}

void iComposer::Compose(HDC hdc, const iSize& siz, const iPoint &offset, const iPoint& snode, iBaseNode* pSelItem)
{
	DWORD btime = timeGetTime();

	const iSpriteMgr& spriteMgr = gItemMgr.m_SpriteMgr;
	iAvatarMgr& avatarMgr = gItemMgr.m_AvatarMgr;
	iIsoMetric im(5);
	iPoint aanch = offset-iPoint(GetMapPixelWidth()/2+40,20);
	iPoint fpos = im.Screen2Map(aanch);
	fpos.Move(-1,-1);
	iPoint ppos = im.Map2Screen(fpos);
	ppos.Move(-20,-8);
	iPoint offs = aanch-ppos;

	sint32 cnt_x = siz.w/40+3;
	sint32 cnt_y = siz.h/8+5;

	sint32 step_x = im.GetCellStepX()*2;
	sint32 step_y = im.GetCellStepY();
	
	const iPoint anchor_coor(fpos);
	iPoint init_map_coor(fpos);
	iPoint map_coor(fpos);

	ResizeComposeDib(siz);
	m_ComposeMemDC.m_Dib.Fill(cColorBlue64);
	// 
	// Surface and grid
	for (sint32 yy=0; yy<cnt_y; ++yy) {
		for (sint32 xx=0; xx<cnt_x; ++xx){
			if (m_pMap->IsValidPos(map_coor.x,map_coor.y)){
				sint32 xpos = xx*step_x;
				sint32 ypos = yy*step_y;
				if (yy%2!=0) xpos -= step_x >> 1;
				iPoint op(xpos-offs.x,ypos-offs.y);

				iMapHandler::iSurfCell cell;
				if ( m_pMap->GetCellSurf(map_coor,cell) ) {
					// solid cell - do nothing
					const iSprite* pSprite = spriteMgr.GetSpritePtr(iFormat(_T("common.surf_tiles_%04d"),cell.lowestLayer*4+CalcCellSeq(map_coor,4)));
					pSprite->dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op+pSprite->anchor,BLEND_ALPHABLEND);
				} else {
					// Compose inclusive transition Water->Sand
					if (cell.lowestLayer == STYPE_WATER){
						uint8 cfg = 0;//(uint8)( ((ccfg & 0xF0)>>4) | ((ccfg & 0xF00)>>8) | ((ccfg & 0xF000)>>12) | ((ccfg & 0xF0000)>>16) | ((ccfg & 0xF00000)>>20) | ((ccfg & 0xF000000)>>24));
						for (uint32 zz=0; zz<STYPE_COUNT; ++zz) cfg |= cell.layers[zz];
						const iSprite* pSprite = spriteMgr.GetSpritePtr(iFormat(_T("common.ws_tiles_%04d"),cfg-1));
						pSprite->dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op+pSprite->anchor,BLEND_ALPHABLEND);
					} else {
						const iSprite* pSprite = spriteMgr.GetSpritePtr(iFormat(_T("common.surf_tiles_%04d"),cell.lowestLayer*4+CalcCellSeq(map_coor,4)));
						pSprite->dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op+pSprite->anchor,BLEND_ALPHABLEND);
					}
					// Compose other layers
					for (uint32 zz=1; zz<STYPE_COUNT; ++zz){
						if (cell.layers[zz]){
							const iSprite* pTransMask = spriteMgr.GetSpritePtr(iFormat(_T("common.trans_tiles_%04d"),cell.layers[zz]-1));
							iGrayDib gmask;
							iDibGrayDibHandl::DibChannel2GrayDib(pTransMask->dib,gmask,Channel_Alpha);
							iDib vdib(&spriteMgr.GetSpritePtr(iFormat(_T("common.surf_tiles_%04d"),zz*4+CalcCellSeq(map_coor,4)))->dib);
							iDibGrayDibHandl::GrayDib2DibChannel(gmask,vdib,Channel_Alpha);
							vdib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op,BLEND_ALPHABLEND);
						}
					}
				}

				if (m_bGrid) {
					m_dibGrid.CopyToDibXY(&m_ComposeMemDC.m_Dib, op, BLEND_ALPHABLEND);
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
	// Decorations and objects
	init_map_coor = anchor_coor;
	map_coor = anchor_coor;
	for (yy=0; yy<cnt_y; ++yy) {
		for (sint32 xx=0; xx<cnt_x; ++xx){
			if (m_pMap->IsValidPos(map_coor.x,map_coor.y)){
				sint32 xpos = xx*step_x;
				sint32 ypos = yy*step_y;
				if (yy%2!=0) xpos -= step_x >> 1;
				iPoint op(xpos-offs.x,ypos-offs.y);
				// Selection
				if (snode == map_coor) {
					if (!pSelItem){
						m_dibSelCell.CopyToDibXY(&m_ComposeMemDC.m_Dib,op,BLEND_ALPHABLEND);
					} else if (iArtifactNode* pArtNode = DynamicCast<iArtifactNode*>(pSelItem)) {
						const iSprite& sprite = spriteMgr[pArtNode->Sprite()];
						sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op+sprite.anchor,BLEND_ALPHABLEND);
					} else if (iMineralNode* pMineral = DynamicCast<iMineralNode*>(pSelItem)) {
						if (pMineral->MineralType()==MINERAL_UNKNOWN) {
							const iSprite& sprite = spriteMgr[_T("resources.rand_res_icn")];
							sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op+sprite.anchor,BLEND_ALPHABLEND);
						} else {
							const iSprite& sprite = spriteMgr[iFormat(_T("resources.icons_%04d"),pMineral->MineralType())];
							sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op+sprite.anchor,BLEND_ALPHABLEND);
						}
					} else if (DynamicCast<iManaCrystalNode*>(pSelItem)) {
						const iSprite& sprite = spriteMgr[_T("resources.mcryst")];
						sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op+sprite.anchor,BLEND_ALPHABLEND);
					} else if (DynamicCast<iCampFireNode*>(pSelItem)) {
						const iSprite& sprite = spriteMgr[_T("resources.campfire")];
						sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op+sprite.anchor,BLEND_ALPHABLEND);
					} else if (DynamicCast<iChestNode*>(pSelItem)) {
						const iSprite& sprite = spriteMgr[_T("resources.chest")];
						sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op+sprite.anchor,BLEND_ALPHABLEND);
					} else if (DynamicCast<iSpellScrollNode*>(pSelItem)) {
						const iSprite& sprite = spriteMgr[_T("resources.spell_scroll")];
						sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op+sprite.anchor,BLEND_ALPHABLEND);
					} else if (DynamicCast<iLampNode*>(pSelItem)) {
						const iSprite& sprite = spriteMgr[_T("resources.lamp")];
						sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op+sprite.anchor,BLEND_ALPHABLEND);
					} else if (iKeyGuardNode* pKeyGuard = DynamicCast<iKeyGuardNode*>(pSelItem)) {
						const iSprite& sprite = spriteMgr[iFormat(_T("resources.key_guards_%04d"),pKeyGuard->m_key)];
						sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op+sprite.anchor,BLEND_ALPHABLEND);
					} else if (DynamicCast<iMapEventNode*>(pSelItem)) {
						const iSprite& sprite = spriteMgr[_T("common.event")];
						sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op+sprite.anchor,BLEND_ALPHABLEND);
					} else if (DynamicCast<iUltimateArtNode*>(pSelItem)) {
						const iSprite& sprite = spriteMgr[_T("common.ultimate")];
						sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op+sprite.anchor,BLEND_ALPHABLEND);
					} else if (DynamicCast<iDecorNode*>(pSelItem) || DynamicCast<iBaseVisCnstNode*>(pSelItem) || DynamicCast<iBaseOwnCnstNode*>(pSelItem) || DynamicCast<iCastleNode*>(pSelItem)) {
						iDecorNode* pDecorNode = (iDecorNode*)pSelItem;
						for (uint32 aid=0; aid<pDecorNode->AvatarsCount(); ++aid){
							const iAvatar& avt = avatarMgr[pDecorNode->Avatar(aid).id];
							ComposeAvatar(avt,op+im.GetScreenOffset(pDecorNode->Avatar(aid).offset),false);
						}
					} else if (iPathElNode* pPathEl = DynamicCast<iPathElNode*>(pSelItem)) {
						const iSprite& sprite = spriteMgr[pPathEl->SpriteConfig(0)];
						sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op + sprite.anchor,BLEND_ALPHABLEND);
					} else if (iHeroNode* pHeroNode = DynamicCast<iHeroNode*>(pSelItem)) {
						uint32 hsid = pHeroNode->Type()*72+3*9;
						const iSprite& sprite = spriteMgr[iFormat(_T("common.heroes_%04d"),hsid)];
						sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op + sprite.anchor,BLEND_ALPHABLEND);
					} else if (iGuardNode* pGuardNode = DynamicCast<iGuardNode*>(pSelItem)) {
						const iSprite& sprite = spriteMgr[pGuardNode->Sprite()];
						sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op+sprite.anchor,BLEND_ALPHABLEND);
					}
				}

				// Ultimate artifact
				if (m_pMap->m_posUltimateArt == map_coor) {
					const iSprite& sprite = spriteMgr[_T("common.ultimate")];
					if (m_bShadow && sprite.shadow) ComposeShadow(sprite.dib,m_ComposeMemDC.m_Dib,op + sprite.anchor);
					sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op + sprite.anchor,BLEND_ALPHABLEND);
				}

				// Decors
				const iAvatar* pAvatar = m_pMap->GetCell(map_coor.x,map_coor.y).pDecor;
				if (pAvatar) {
					ComposeAvatar(*pAvatar,op,m_bShadow);
				}

				// Path Elements
				if (iPathElNode* pPathEl = m_pMap->GetCell(map_coor.x,map_coor.y).pPathEl) {
					uint8 cfg = m_pMap->GetPathCfg(map_coor,pPathEl);
					const iSprite& sprite = spriteMgr[pPathEl->SpriteConfig(cfg)];
					if (m_bShadow && sprite.shadow) ComposeShadow(sprite.dib,m_ComposeMemDC.m_Dib,op + sprite.anchor);
					sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op + sprite.anchor,BLEND_ALPHABLEND);
				}

				// Visitables
				for (uint32 vcnstId=0; vcnstId<m_pMap->m_VisCnstList.GetSize(); ++vcnstId) {
					if (m_pMap->m_VisCnstList[vcnstId]->Pos() == map_coor) {
						for (uint32 aid=0; aid<m_pMap->m_VisCnstList[vcnstId]->Proto()->AvatarsCount(); ++aid){
							const iAvatar& avt = avatarMgr[m_pMap->m_VisCnstList[vcnstId]->Proto()->Avatar(aid).id];
							ComposeAvatar(avt,op+im.GetScreenOffset(m_pMap->m_VisCnstList[vcnstId]->Proto()->Avatar(aid).offset),m_bShadow);
						}
						
						if (!m_pMap->m_VisCnstList[vcnstId]->Guard().Empty()){
							const iSprite& sprite = spriteMgr[iFormat(_T("creatures.minimon_%04d"),m_pMap->m_VisCnstList[vcnstId]->Guard().StrongestCreature())];
							if (m_bShadow && sprite.shadow) ComposeShadow(sprite.dib,m_ComposeMemDC.m_Dib,op + sprite.anchor);
							sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op + sprite.anchor,BLEND_ALPHABLEND);
						}
					}
				}

				// Ownerables
				for (uint32 ocnstId=0; ocnstId<m_pMap->m_OwnCnstList.GetSize(); ++ocnstId) {
					if (m_pMap->m_OwnCnstList[ocnstId]->Pos() == map_coor) {
						for (uint32 aid=0; aid<m_pMap->m_OwnCnstList[ocnstId]->Proto()->AvatarsCount(); ++aid){
							const iAvatar& avt = avatarMgr[m_pMap->m_OwnCnstList[ocnstId]->Proto()->Avatar(aid).id];
							ComposeAvatar(avt,op+im.GetScreenOffset(m_pMap->m_OwnCnstList[ocnstId]->Proto()->Avatar(aid).offset),m_bShadow,m_pMap->m_OwnCnstList[ocnstId]->Owner());
						}
						if (!m_pMap->m_OwnCnstList[ocnstId]->Guard().Empty()){
							const iSprite& sprite = spriteMgr[iFormat(_T("creatures.minimon_%04d"),m_pMap->m_OwnCnstList[ocnstId]->Guard().StrongestCreature())];
							if (m_bShadow && sprite.shadow) ComposeShadow(sprite.dib,m_ComposeMemDC.m_Dib,op + sprite.anchor);
							sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op + sprite.anchor,BLEND_ALPHABLEND);
						}
					}
				}

				// Castles
				for (uint32 cstId=0; cstId<m_pMap->m_CastleList.GetSize(); ++cstId) {
					if (m_pMap->m_CastleList[cstId]->Pos() == map_coor) {
						for (uint32 aid=0; aid<m_pMap->m_CastleList[cstId]->Proto()->AvatarsCount(); ++aid){
							const iAvatar& avt = avatarMgr[m_pMap->m_CastleList[cstId]->Proto()->Avatar(aid).id];
							ComposeAvatar(avt,op+im.GetScreenOffset(m_pMap->m_CastleList[cstId]->Proto()->Avatar(aid).offset),m_bShadow,m_pMap->m_CastleList[cstId]->Owner());
						}
					}
				}

				// Heroes
				for (uint32 heroId=0; heroId<m_pMap->m_HeroList.GetSize(); ++heroId) {
					if (m_pMap->m_HeroList[heroId]->Pos() == map_coor) {
						uint32 hsid = m_pMap->m_HeroList[heroId]->Proto()->Type()*72+3*9;
						uint32 fsid = m_pMap->m_HeroList[heroId]->Owner();
						const iSprite& sprite = spriteMgr[iFormat(_T("common.heroes_%04d"),hsid)];
						const iDib& fdib = spriteMgr[iFormat(_T("common.flags_%04d"),fsid)].dib;
						// Compose Shadow
						if (m_bShadow) ComposeShadow(sprite.dib, m_ComposeMemDC.m_Dib, op+sprite.anchor);
						// Compose Sprite
						sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib, op + sprite.anchor, BLEND_ALPHABLEND);
						// Compose flag
						fdib.CopyToDibXY(&m_ComposeMemDC.m_Dib, op + sprite.anchor+HERO_FLAG_ANCHOR[3*9],BLEND_ALPHABLEND);
					}
				}

				// Map Items
				for (uint32 resId=0; resId<m_pMap->m_MapItemList.GetSize(); ++resId) {
					iMapItem* pItem = m_pMap->m_MapItemList[resId];
					if (pItem->Pos() == map_coor) {
						const iSprite& sprite = spriteMgr[pItem->Sprite()];
						if (m_bShadow && sprite.shadow) ComposeShadow(sprite.dib,m_ComposeMemDC.m_Dib,op + sprite.anchor);
						sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op + sprite.anchor,BLEND_ALPHABLEND);
					}
				}
				
				// Map Events
				for (uint32 meId=0; meId<m_pMap->m_MapEventList.GetSize(); ++meId) {
					iMapEvent* pEvent = m_pMap->m_MapEventList[meId];
					if (pEvent->Pos() == map_coor) {
						const iSprite& sprite = spriteMgr[_T("common.event")];
						if (m_bShadow && sprite.shadow) ComposeShadow(sprite.dib,m_ComposeMemDC.m_Dib,op + sprite.anchor);
						sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op + sprite.anchor,BLEND_ALPHABLEND);
					}
				}

				// Map guards
				for (uint32 gId=0; gId<m_pMap->m_MapGuardList.GetSize(); ++gId) {
					iMapGuard* pGuard = m_pMap->m_MapGuardList[gId];
					if (pGuard->Pos() == map_coor) {
						const iSprite& sprite = spriteMgr[pGuard->Sprite()];
						if (m_bShadow && sprite.shadow) ComposeShadow(sprite.dib,m_ComposeMemDC.m_Dib,op + sprite.anchor);
						sprite.dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,op + sprite.anchor,BLEND_ALPHABLEND);
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

	// Pass map
	if (m_bPassMap){
		m_pMap->PreparePassMap(avatarMgr,true);
		init_map_coor = anchor_coor;
		map_coor = anchor_coor;
		for (yy=0; yy<cnt_y; ++yy) {
			for (sint32 xx=0; xx<cnt_x; ++xx){
				if (m_pMap->IsValidPos(map_coor.x,map_coor.y)){
					sint32 xpos = xx*step_x;
					sint32 ypos = yy*step_y;
					if (yy%2!=0) xpos -= step_x >> 1;
					iPoint op(xpos-offs.x,ypos-offs.y);

					if (m_pMap->m_PassMap.GetCell(map_coor.x,map_coor.y) == -1){
						m_dibRedCell.CopyToDibXY(&m_ComposeMemDC.m_Dib,op,BLEND_ALPHABLEND);
					} else if (m_pMap->m_PassMap.GetCell(map_coor.x,map_coor.y) == -2){
						m_dibYelCell.CopyToDibXY(&m_ComposeMemDC.m_Dib,op,BLEND_ALPHABLEND);
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
	
	DWORD etime = timeGetTime();
	m_ComposeMemDC.DrawTextRect(iFormat(_T("%d msec"),etime-btime),iRect(0,0,100,20));

	::BitBlt(hdc,0,0, siz.w,siz.h,m_ComposeMemDC,0,0,SRCCOPY);
}
