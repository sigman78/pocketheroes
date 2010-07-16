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

#ifndef _HMM_GAME_HERO_VIEW_ARTIFACTS_PAGE_H_
#define _HMM_GAME_HERO_VIEW_ARTIFACTS_PAGE_H_

const sint32 doX = 65;
const sint32 doY = 5;
const sint32 acW = 30;
const sint32 acH = 28;

#include "ArtifactList.h"

//////////////////////////////////////////////////////////////////////////
const sint32 HERO_DOLL_CELLS[AC_COUNT][2] = {
	{74, -2}, {74,29}, {74,60}, {-6,23}, {109,91}, {-6,54}, {142,91}, {109,122}, {76,125}, {76,162}, {27,65}, {27,96}, {27,127}, {27,158}
};

//////////////////////////////////////////////////////////////////////////
class iArtifactsTab : public iHeroViewChild, public IViewCmdHandler
{
public:
	iArtifactsTab(iViewMgr* pViewMgr) : iHeroViewChild(pViewMgr) 
	{
		iRect rc = GetScrRect();
		AddChild(m_pArtBackPackCtrl = new iArtBackPackCtrl(m_Competitors, pViewMgr, this, iPoint(10,10), iSize(acW,acH), 5, 0/*iArtBackPackCtrl::Horizontal*/, 101));
		for (uint32 xx=0; xx<AC_COUNT; ++xx) {
			AddChild(m_pDollItems[xx] = new iArtItemCtrl(m_Competitors, pViewMgr, this, iPoint(HERO_DOLL_CELLS[xx][0]+doX,HERO_DOLL_CELLS[xx][1]+doY), iSize(acW,acH), 110+xx));
		}
	}
	
	void OnHeroChanged() 
	{
		m_pArtBackPackCtrl->SetBackPack(&m_pHero->Artifacts().BackPack(), m_pHero);
		for (uint32 xx=0; xx<AC_COUNT; ++xx) {
			m_pDollItems[xx]->SetOwner(m_pHero);
			m_pDollItems[xx]->SetArtCell(&m_pHero->Artifacts().DressedItem((HERO_ART_CELL)xx));
		}
		Invalidate();
	}

	void ComposeArtCell(const iRect& rc, uint16 artId)
	{
		gApp.Surface().FillRect(rc,RGB16(255,192,64),64);
		ButtonFrame(gApp.Surface(),rc, 0);
		if (artId != 0xFFFF) BlitIcon(gApp.Surface(),gGame.ItemMgr().m_artMgr[artId].Sprite(),rc);
	}

	void OnCompose()
	{
		iHeroViewChild::OnCompose();
		iRect rc = GetScrRect();

		// Draw Hero doll
		iPoint hdPos(rc.x+doX,rc.y+doY);
		gGfxMgr.Blit(PDGG_HDOLL_WIRE,gApp.Surface(),hdPos);
		gGfxMgr.BlitEffect(PDGG_HDOLL_BODY,gApp.Surface(),hdPos,iGfxManager::EfxTransparent);
		gGfxMgr.BlitEffect(PDGG_HDOLL_CAPE,gApp.Surface(),hdPos,iGfxManager::EfxTransparent);
		gGfxMgr.BlitEffect(PDGG_HDOLL_CAPE,gApp.Surface(),hdPos,iGfxManager::EfxTransparent);

		/*
		iRect bpRect(rc.x+3,rc.y+3,acW,17);
		bpRect.y += 17; bpRect.h=acH;
		for (uint32 xx=0; xx<5; ++xx){
			ComposeArtCell(bpRect, 0xFFFF);
			bpRect.y += acH+1;
		}
		bpRect.h=17;

		// Compose doll's cells
		for (uint32 xx=0; xx<m_dollCells.GetSize(); ++xx) {
			ComposeArtCell(m_dollCells[xx],m_pHero->Artifacts().DressedItem((HERO_ART_CELL)xx).artId);
		}
		*/
	}

	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
	{
		uint32 uid = pView->GetUID();
	}

private:
	sint32 CellByPos(const iPoint& pos)
	{
		sint32 result = -1;
		// find in doll's cells
		for (uint32 xx=0; xx<m_dollCells.GetSize(); ++xx) {
			if (m_dollCells[xx].PtInRect(pos)) return xx;
		}

		return result;
	}

	void OnMouseDown(const iPoint& pos) 
	{
	}

	void OnMouseUp(const iPoint& pos) 
	{
	}


protected:
	iSimpleArray<iArtDragDropItem*>		m_Competitors;
	iArtItemCtrl*						m_pDollItems[AC_COUNT];
	iArtBackPackCtrl*					m_pArtBackPackCtrl;
	iSimpleArray<iRect>					m_dollCells;
};

#endif //_HMM_GAME_HERO_VIEW_ARTIFACTS_PAGE_H_

