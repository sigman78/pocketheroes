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
#include "MainView.h"

inline iPoint Screen2Map(const iPoint& cell, uint32 mm)
{
	return iPoint(mm-1 + (cell.x-cell.y), cell.x+cell.y);
}

iMinimapView::iMinimapView()
: m_pMap(NULL), m_frameLT(cInvalidPoint), m_frameRB(cInvalidPoint), m_pMainView(NULL), m_bTrack(false)
{
	
}

HWND iMinimapView::CreateMinimapView(HWND hParent, CMapEditorView* pMainView)
{
	RECT rect={0,0,1,1};
	m_pMainView = pMainView;
	if (Create(hParent, rect,NULL,WS_VISIBLE|WS_CHILD) == NULL) return NULL;
	return m_hWnd;
}

LRESULT iMinimapView::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	iSize siz(LOWORD(lParam),HIWORD(lParam));
	iSize ns( iMAX<uint32>(200,iALIGN(siz.w,32)), iMAX<uint32>(200,iALIGN(siz.h,32)));
	if (ns != m_outBuff.GetSize()) {
		m_outBuff.Resize(ns);
		OnMapContentChanged();
	}
	return 0;
}

void iMinimapView::ScrollMap(iPoint pos) 
{
		pos.x -= 20;
		pos.y -= 25;
		SCROLLINFO si;
		
		iRect clRect = iWMetrics::GetClRect(this->m_hWnd);				
		//this->ClientToScreen(

		int left;
		int width;
		int top = 0;
		int height = 100;

		left = 24;		
		width = 128;
		top = -8;
		height = 128;

		iRect frc(Screen2Map(m_frameLT, m_pMap->GetWidth()), Screen2Map(m_frameRB, m_pMap->GetWidth()));
		if (m_dibMap.GetWidth() > clRect.w || m_dibMap.GetHeight() > clRect.h){
			uint32 factor = iMAX<uint32>(m_dibMap.GetWidth()/clRect.w, m_dibMap.GetHeight()/clRect.h )+1;
			iDib ndib(iSize(m_dibMap.GetWidth()/factor,m_dibMap.GetHeight()/factor));			
			iPoint offset(clRect.w/2 - ndib.GetSize().w/2, clRect.h/2 - ndib.GetSize().h/2);			
			frc.x /= factor;
			frc.y /= factor;
			frc.w /= factor;
			frc.h /= factor;
			frc.Move(offset.x, offset.y);
		} else {
			uint32 factor = iMIN<uint32>(clRect.w / m_dibMap.GetWidth(), clRect.h / m_dibMap.GetHeight());
			if (factor > 1) {
				iDib ndib(m_dibMap);
				iDibTransformer::MultiplySize(ndib, factor);
				iPoint offset(clRect.w/2 - ndib.GetSize().w/2, clRect.h/2 - ndib.GetSize().h/2);
				ndib.CopyToDibXY(&m_outBuff.m_Dib, offset, BLEND_ALPHABLEND);
				frc.x *= factor;
				frc.y *= factor;
				frc.w *= factor;
				frc.h *= factor;
				frc.Move(offset.x, offset.y);
			} else {
				iPoint offset(clRect.w/2 - m_dibMap.GetSize().w/2, clRect.h/2 - m_dibMap.GetSize().h/2);				
				frc.Move(offset.x, offset.y);
			}
		}				

		pos.x -= frc.w / 2;
		pos.y -= frc.h / 2;

		double hrz = 0, vrt = 0;		
		
		if(pos.x <= left)
			hrz = 0.0;
		else if((unsigned)(pos.x) >= left + width)
			hrz = 1.0;
		else
			hrz = (double)(pos.x - left) / (double)width;


		if(pos.y <= top)
			vrt = 0.0;
		else if((unsigned)(pos.y) >= top + height)
			vrt = 1.0;
		else
			vrt = (double)(pos.y - top) / (double)height;

		if(pos.y < 0)
			vrt = 0.0;


		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_RANGE;
		m_pMainView->GetScrollInfo(SB_HORZ, &si);
		m_pMainView->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_THUMBPOSITION, (int)((double)si.nMax * hrz)));		
		
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_RANGE;		
		m_pMainView->GetScrollInfo(SB_VERT, &si);
		m_pMainView->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_THUMBPOSITION, si.nMax * vrt));		

}

LRESULT iMinimapView::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	iPoint pos(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
	m_bTrack = true;
	if (m_pMainView) {
		ScrollMap(pos);
	}
	return 0;
}

LRESULT iMinimapView::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	iPoint pos(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
	m_bTrack = false;
	return 0;
}

LRESULT iMinimapView::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	iPoint pos(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
	if (m_bTrack) {
		ScrollMap(pos);
	}
	return 0;
}


void iMinimapView::SetMap(iMapHandler* pMap)
{
	m_pMap = pMap;
	OnMapContentChanged();
}

void iMinimapView::OnMapContentChanged()
{
	PrepareMap();
	HDC hdc = GetDC();
	Compose(hdc);
	ReleaseDC(hdc);
}

inline iColor Cvt16to32(uint16 clr)
{
	return iColor( (clr&0xF800)>>8, (clr&0x7E0)>>3, (clr&0x1F)<<3, 255 );
}

void iMinimapView::PrepareMap(iMapHandler* pMap, iDib& odib, bool bShowCnsts)
{	
	if (pMap) {
		iSize nsiz(pMap->GetWidth()*2, pMap->GetHeight()*2);
		if (nsiz != odib.GetSize()) odib.Resize(nsiz);
		else odib.Fill(cColorTransparent);
		sint32 xx,yy;

		// Prepare map
		iMapT<iColor> tmap(pMap->GetWidth(), pMap->GetHeight());
		for (yy=0; yy<tmap.GetHeight(); ++yy){
			for (xx=0; xx<tmap.GetWidth(); ++xx){
				iMapHandler::iSurfCell cell;
				pMap->GetCellSurf(iPoint(xx,yy),cell);
				tmap.At(xx,yy) = Cvt16to32(SURF_COLOR[cell.lowestLayer]);
				if (const iAvatar* pAvatar = pMap->GetCell(xx,yy).pDecor) {
					for (uint32 pmid = 0; pmid<pAvatar->PassModsCount(); ++pmid) {
						sint32 nx = xx+pAvatar->PassModEntry(pmid).offset.x;
						sint32 ny = yy+pAvatar->PassModEntry(pmid).offset.y;
						if (tmap.IsValidPos(nx,ny)) {
							tmap.At(nx,ny).AlphaBlend(iColor(0,0,0,96));
						} 
					}
				} else if (const iPathElNode* pPathEl = pMap->GetCell(xx,yy).pPathEl) {
					if (pPathEl->PassModVal() == -1) {
						tmap.At(xx,yy).AlphaBlend(iColor(0,0,0,96));
					}
				}
			}
		}

		if (bShowCnsts) {
			// Visitables
			for (uint32 vcid = 0; vcid<pMap->m_VisCnstList.GetSize(); ++vcid) {
				for (uint32 aid=0; aid<pMap->m_VisCnstList[vcid]->Proto()->AvatarsCount(); ++aid) {
					const iAvatar& avt = gItemMgr.m_AvatarMgr[pMap->m_VisCnstList[vcid]->Proto()->Avatar(aid).id];
					for (uint32 pm=0; pm<avt.PassModsCount(); ++pm){
						sint32 nx = pMap->m_VisCnstList[vcid]->Pos().x+pMap->m_VisCnstList[vcid]->Proto()->Avatar(aid).offset.x+avt.PassModEntry(pm).offset.x;
						sint32 ny = pMap->m_VisCnstList[vcid]->Pos().y+pMap->m_VisCnstList[vcid]->Proto()->Avatar(aid).offset.y+avt.PassModEntry(pm).offset.y;
						if (tmap.IsValidPos(nx, ny)) tmap.At(nx,ny).AlphaBlend(iColor(0,0,0,96));//tmap.At(nx, ny) = iColor(192,192,192,255);
					}
				}
			}

			// Ownerables
			for (uint32 ocid = 0; ocid<pMap->m_OwnCnstList.GetSize(); ++ocid) {
				for (uint32 aid=0; aid<pMap->m_OwnCnstList[ocid]->Proto()->AvatarsCount(); ++aid) {
					const iAvatar& avt = gItemMgr.m_AvatarMgr[pMap->m_OwnCnstList[ocid]->Proto()->Avatar(aid).id];
					for (uint32 pm=0; pm<avt.PassModsCount(); ++pm){
						sint32 nx = pMap->m_OwnCnstList[ocid]->Pos().x+pMap->m_OwnCnstList[ocid]->Proto()->Avatar(aid).offset.x+avt.PassModEntry(pm).offset.x;
						sint32 ny = pMap->m_OwnCnstList[ocid]->Pos().y+pMap->m_OwnCnstList[ocid]->Proto()->Avatar(aid).offset.y+avt.PassModEntry(pm).offset.y;
						if (tmap.IsValidPos(nx, ny)) {
							if (pMap->m_OwnCnstList[ocid]->Owner() == PID_NEUTRAL) tmap.At(nx, ny) = iColor(192,192,192,255);
							else tmap.At(nx, ny) = Cvt16to32(PLAYER_COLORS[pMap->m_OwnCnstList[ocid]->Owner()]);
						}
					}
				}
			}

			// Castles
			for (uint32 ctlid = 0; ctlid<pMap->m_CastleList.GetSize(); ++ctlid) {
				for (uint32 aid=0; aid<pMap->m_CastleList[ctlid]->Proto()->AvatarsCount(); ++aid) {
					const iAvatar& avt = gItemMgr.m_AvatarMgr[pMap->m_CastleList[ctlid]->Proto()->Avatar(aid).id];
					for (uint32 pm=0; pm<avt.PassModsCount(); ++pm){
						sint32 nx = pMap->m_CastleList[ctlid]->Pos().x+pMap->m_CastleList[ctlid]->Proto()->Avatar(aid).offset.x+avt.PassModEntry(pm).offset.x;
						sint32 ny = pMap->m_CastleList[ctlid]->Pos().y+pMap->m_CastleList[ctlid]->Proto()->Avatar(aid).offset.y+avt.PassModEntry(pm).offset.y;
						if (tmap.IsValidPos(nx, ny)) {
							if (pMap->m_CastleList[ctlid]->Owner() == PID_NEUTRAL) tmap.At(nx, ny) = iColor(192,192,192,255);
							else tmap.At(nx, ny) = Cvt16to32(PLAYER_COLORS[pMap->m_CastleList[ctlid]->Owner()]);
						}
					}
				}
			}

			// Heroes
			for (uint32 hid = 0; hid<pMap->m_HeroList.GetSize(); ++hid) {
				tmap.At(pMap->m_HeroList[hid]->Pos().x, pMap->m_HeroList[hid]->Pos().y) = Cvt16to32(PLAYER_COLORS[pMap->m_HeroList[hid]->Owner()]);
			}
		}

		// Compose
		const sint32 const_x = (sint32)(nsiz.w/2);
		const sint32 const_y = 0;
		sint32 base_x = const_x;
		sint32 base_y = const_y;
		sint32 pos_x=base_x;
		sint32 pos_y=base_y;

		for (yy=0; yy<tmap.GetHeight(); ++yy){
			for (xx=0; xx<tmap.GetWidth(); ++xx){
				iRect orc(pos_x,pos_y,2,1);
				odib.FillRect(orc, tmap.At(xx,yy));
				pos_x+=1;
				pos_y+=1;
			}
			base_x-=1;
			base_y+=1;
			pos_x = base_x;
			pos_y = base_y;
		}
		tmap.Cleanup();
	}
}

void iMinimapView::UpdateSelFrame(const iPoint& lt, const iPoint& rb)
{
	m_frameLT = lt;
	m_frameRB = rb;
	HDC hdc = GetDC();
	Compose(hdc);
	ReleaseDC(hdc);
}

void iMinimapView::PrepareMap()
{
	PrepareMap(m_pMap, m_dibMap, true);
}

void iMinimapView::Compose(HDC hdc)
{	
	iRect clRect = iWMetrics::GetClRect(m_hWnd);
	m_outBuff.m_Dib.Fill(cColorBlack);

	if (m_pMap) {
		iRect frc(Screen2Map(m_frameLT, m_pMap->GetWidth()), Screen2Map(m_frameRB, m_pMap->GetWidth()));
		if (m_dibMap.GetWidth() > clRect.w || m_dibMap.GetHeight() > clRect.h){
			uint32 factor = iMAX<uint32>(m_dibMap.GetWidth()/clRect.w, m_dibMap.GetHeight()/clRect.h )+1;
			iDib ndib(iSize(m_dibMap.GetWidth()/factor,m_dibMap.GetHeight()/factor));
			iDibTransformer::Stretch(m_dibMap, ndib, iDibTransformer::Pyramidal);
			iPoint offset(clRect.w/2 - ndib.GetSize().w/2, clRect.h/2 - ndib.GetSize().h/2);
			ndib.CopyToDibXY(&m_outBuff.m_Dib, offset, BLEND_ALPHABLEND);
			frc.x /= factor;
			frc.y /= factor;
			frc.w /= factor;
			frc.h /= factor;
			frc.Move(offset.x, offset.y);
		} else {
			uint32 factor = iMIN<uint32>(clRect.w / m_dibMap.GetWidth(), clRect.h / m_dibMap.GetHeight());
			if (factor > 1) {
				iDib ndib(m_dibMap);
				iDibTransformer::MultiplySize(ndib, factor);
				iPoint offset(clRect.w/2 - ndib.GetSize().w/2, clRect.h/2 - ndib.GetSize().h/2);
				ndib.CopyToDibXY(&m_outBuff.m_Dib, offset, BLEND_ALPHABLEND);
				frc.x *= factor;
				frc.y *= factor;
				frc.w *= factor;
				frc.h *= factor;
				frc.Move(offset.x, offset.y);
			} else {
				iPoint offset(clRect.w/2 - m_dibMap.GetSize().w/2, clRect.h/2 - m_dibMap.GetSize().h/2);
				m_dibMap.CopyToDibXY(&m_outBuff.m_Dib, offset, BLEND_ALPHABLEND);
				frc.Move(offset.x, offset.y);
			}
		}
		iDibPrimitives::DrawFrame(m_outBuff.m_Dib, frc, iColor(255,255,0,255), BLEND_SRCCOPY);
	}

	::BitBlt(hdc,0,0, clRect.w,clRect.h,m_outBuff,0,0,SRCCOPY);
}

LRESULT iMinimapView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	PAINTSTRUCT ps;
	HDC hdc=BeginPaint(&ps);
	Compose(hdc);
	EndPaint(&ps);
	return 0;
}