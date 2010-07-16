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
#include "MinimapView.h"

iMinimapView::iMinimapView()
: iChildGameView(false, CV_OVERLAND), m_CenterCell(0,0)
{
	AddChild(new iMinimapToolBar(&gApp.ViewMgr(),this,iRect(m_Rect.x,m_Rect.y2()-20,m_Rect.w,21)));
	SetZoom(3);
	PrepareSurface();
}

void iMinimapView::OnMouseDown(const iPoint& pos)
{
	m_TrackCell = Screen2Map(pos - GetOffset());
	Invalidate();
}

void iMinimapView::OnMouseUp(const iPoint& pos)
{
	SetCenterCell(m_TrackCell);
	Invalidate();
}

void iMinimapView::OnMouseTrack(const iPoint& pos)
{
	m_TrackCell = Screen2Map(pos - GetOffset());
	Invalidate();
}

void iMinimapView::SetCenterCell(const iPoint& pos)
{ 
	m_CenterCell.x = iCLAMP<sint32>(0, gGame.Map().GetWidth()-1, pos.x);
	m_CenterCell.y = iCLAMP<sint32>(0, gGame.Map().GetHeight()-1, pos.y);
}

void iMinimapView::OnZoomChanged()
{
	iView* pTB = GetChildById(VPUID_MMAPTOOLBARVIEW);
	check(pTB);
	pTB->GetChildById(VPUID_BTN_ZOOMIN)->SetEnabled(m_Scale < ZOOM_MAX);
	pTB->GetChildById(VPUID_BTN_ZOOMOUT)->SetEnabled(m_Scale > ZOOM_MIN);
	Invalidate();
}

void iMinimapView::PrepareSurface()
{
	m_Surf.Allocate(gGame.Map().GetWidth()*gGame.Map().GetHeight());
	uint16 *surfPtr = m_Surf.GetPtr();

	for (sint32 yy=0; yy<gGame.Map().GetHeight(); ++yy){
		for (sint32 xx=0; xx<gGame.Map().GetWidth(); ++xx){
			iCell cell = gGame.Map().GetAt(xx,yy);
			*surfPtr = (uint16)((cell.avatar)?Darken25(SURF_COLOR[cell.SurfNode(1)]):SURF_COLOR[cell.SurfNode(1)]);
			surfPtr++;
		}
	}
}

void iMinimapView::OnCompose()
{
	gApp.Surface().FillRect(m_Rect,cColor_Green64);

	iPoint anchor = GetOffset();
	const sint32 const_x = m_Rect.x+((sint32)(m_Rect.w/2)) + anchor.x;
	const sint32 const_y = m_Rect.y + anchor.y;
	sint32 base_x = const_x;
	sint32 base_y = const_y;
	sint32 pos_x=base_x;
	sint32 pos_y=base_y;

	uint16 *surfPtr = m_Surf.GetPtr();
	for (sint32 yy=0; yy<gGame.Map().GetHeight(); ++yy) {
		for (sint32 xx=0; xx<gGame.Map().GetWidth(); ++xx) {
			iRect orc(pos_x,pos_y,m_Scale*2,m_Scale);
			if (iClipper::IsIntersectRect(m_Rect,orc)){
				uint8 dcfg,fcfg;
				gGame.Map().ActPlayer()->FogMap().GetFogCfg(iPoint(xx,yy),dcfg,fcfg);
				if (dcfg) {
					gApp.Surface().FillRect(orc,cColor_Black);
				} else {
					gApp.Surface().FillRect(orc,*surfPtr);
					if (gGame.Map().GetAt(xx,yy).avatar != 0xFFFF) {
						if (gGame.Map().m_PassMap.GetAt(xx,yy) == -1) gApp.Surface().FillRect(orc,cColor_Black,128);
						else gApp.Surface().FillRect(orc,cColor_Black,32);
					}
					if (fcfg) {
						gApp.Surface().FillRect(orc,cColor_Black,64);
					} else if (iBaseMapObject* pObj = gGame.Map().m_CoverMap.GetAt(xx,yy)){
						if (iOwnCnst* pOwnCnst = DynamicCast<iOwnCnst*>(pObj)) {
							iDib::pixel clr = (pOwnCnst->Owner() == PID_NEUTRAL)?cColor_Gray192:PLAYER_COLORS[pOwnCnst->Owner()];
							gApp.Surface().FillRect(orc,clr);
						} else if (iCastle* pCastle = DynamicCast<iCastle*>(pObj)) {
							iDib::pixel clr = (pCastle->Owner() == PID_NEUTRAL)?cColor_Gray192:PLAYER_COLORS[pCastle->Owner()];
							gApp.Surface().FillRect(orc,clr);
						}
					}
				}
				pos_x+=m_Scale;
				pos_y+=m_Scale;
				surfPtr++;
			} else {
				pos_x+=m_Scale;
				pos_y+=m_Scale;
				surfPtr++;
			}
		}
		base_x-=m_Scale;
		base_y+=m_Scale;
		pos_x = base_x;
		pos_y = base_y;
	}

	// Heroes
	for (iGameWorld::iPLIt pit = gGame.Map().m_Players.First(); pit != gGame.Map().m_Players.End(); ++pit){
		for (iPlayer::iHLIt hit = (*pit)->HeroFirst(); hit != (*pit)->HeroEnd(); ++hit){
			if (!gGame.Map().ActPlayer()->FogMap().IsHidden((*hit)->Pos())) {
				sint32 px = const_x + ((*hit)->Pos().x - (*hit)->Pos().y)*m_Scale;
				sint32 py = const_y + ((*hit)->Pos().x + (*hit)->Pos().y)*m_Scale;
				if (m_Rect.PtInRect(iPoint(px,py))){
					gApp.Surface().FillRect(iRect(px,py,m_Scale,m_Scale),PLAYER_COLORS[(*hit)->Owner()->PlayerId()]);
				}
			}
		}
	}

	iPoint tpoint = Map2Screen(m_bTracking?m_TrackCell:m_CenterCell);
	iRect orc(tpoint.x+anchor.x+m_Rect.x-8*m_Scale,tpoint.y+anchor.y+m_Rect.y-9*m_Scale,16*m_Scale,18*m_Scale);
	gApp.Surface().FrameRect(orc,m_bTracking?cColor_Yello:cColor_White);
}

void iMinimapView::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();

	if (uid == VPUID_BTN_ZOOMIN) ZoomIn();
	else if (uid == VPUID_BTN_ZOOMOUT) ZoomOut();
	else if (uid == VPUID_BTN_CLOSEMMAP) gGame.HideView(CV_MINIMAP);
}

/*
 *	Minimap Toolbar 
 */
iMinimapToolBar::iMinimapToolBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect)
: iView(pViewMgr, rect, GENERIC_VIEWPORT,VPUID_MMAPTOOLBARVIEW, Enabled|Visible)
{
	AddChild(new iIconButton(pViewMgr, pCmdHandler, iRect(0,1,20,20),PDGG_BTN_PLUS,VPUID_BTN_ZOOMIN));
	AddChild(new iIconButton(pViewMgr, pCmdHandler, iRect(20,1,20,20),PDGG_BTN_MINUS,VPUID_BTN_ZOOMOUT));
	AddChild(new iIconButton(pViewMgr,pCmdHandler, iRect(rect.w-30,1,30,20), PDGG_BTN_CLOSE, VPUID_BTN_CLOSEMMAP));
}

void iMinimapToolBar::OnCompose()
{
	iRect rect = GetScrRect();
	gApp.Surface().FillRect(rect,cColor_Black);

	iRect rc(rect.x+41,rect.y+1,rect.w-31-41,20);
	gGfxMgr.BlitTile(PDGG_BKTILE, gApp.Surface(), rc);
	ButtonFrame(gApp.Surface(), rc, 0);

	//ComposeTextButton(gApp.Surface(), rc, 0, TRID_VOID);
}

