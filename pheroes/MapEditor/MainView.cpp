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
#include "PropertyPages.h"
#include "UltimatePropsDlg.h"

CMapEditorView::CMapEditorView()
: m_pMap(NULL), m_offset(0,0), m_NavMode(NM_Edit), m_selSurf(STYPE_DIRT), m_selBrush(3), m_bMouseOnWindow(false)
, m_bTracking(false), m_pSelNode(NULL), m_pItemBrowser(NULL), m_pMinimapView(NULL), spacePressed(false)
{	
}

void CMapEditorView::OnSelChanged(iBaseNode* pNode)
{
	m_pSelNode = pNode;
	m_NavMode = NM_Create;
}

bool CMapEditorView::SetSelPoint(const iPoint& npos)
{
	if (m_SelPoint != npos){
		m_SelPoint = npos;
		if (!m_pMap || !m_pMap->IsValidPos(m_SelPoint.x,m_SelPoint.y)) {
			m_pStatusBar->SetText(_T("N/A"));
		} else {
			iBaseMapObject* pObj = m_pMap->GetCellObject(m_SelPoint);
			iStringT txt = iFormat(_T("[%d, %d]"),m_SelPoint.x,m_SelPoint.y);
			if (iVisCnst* pVisCnst = DynamicCast<iVisCnst*>(pObj)) 	txt.Addf(_T(" - %s"),gItemMgr.m_TextMgr[pVisCnst->Proto()->NameKey()].CStr());
			else if (iOwnCnst* pOwnCnst = DynamicCast<iOwnCnst*>(pObj)) txt.Addf(_T(" - %s"),gItemMgr.m_TextMgr[pOwnCnst->Proto()->NameKey()].CStr());
			else if (iCastle* pCastle = DynamicCast<iCastle*>(pObj)) txt.Addf(_T(" - %s"),gItemMgr.m_TextMgr[pCastle->Proto()->NameKey()].CStr());
			else if (iHero* pHero = DynamicCast<iHero*>(pObj)) txt.Addf(_T(" - %s"),gItemMgr.m_TextMgr[pHero->Proto()->NameKey()].CStr());
			else if (DynamicCast<iUltimateArtNode*>(pObj)) txt.Addf(_T(" - %s"),_T("Ultimate Artifact"));
			else if (iMapGuard* pMapGuard = DynamicCast<iMapGuard*>(pObj)) txt.Addf(_T(" - %s"),pMapGuard->Title().CStr());
			else if (iMapEvent* pMapEvent = DynamicCast<iMapEvent*>(pObj)) txt.Addf(_T(" - %s"),gItemMgr.m_TextMgr[_T("MAP_EVENT")].CStr());
			else if (iMapItem* pMapItem = DynamicCast<iMapItem*>(pObj)){
				if (pMapItem->Type() == MAPITEM_MINERAL) {
					iMapItem_Mineral* pMineral = (iMapItem_Mineral*)pMapItem;
					txt.Addf(_T(" - %s"),gItemMgr.m_TextMgr[(pMineral->MineralType() == MINERAL_UNKNOWN)?_T("MAPRES_RAND_MINERAL"):RESOURCE_NAMEKEY[pMineral->MineralType()]].CStr());
				} else if (pMapItem->Type() == MAPITEM_MANACRYSTAL) txt.Addf(_T(" - %s"),gItemMgr.m_TextMgr[_T("MAPRES_MANACRYSTAL")].CStr());
				else if (pMapItem->Type() == MAPITEM_CAMPFIRE) txt.Addf(_T(" - %s"),gItemMgr.m_TextMgr[_T("MAPRES_CAMPFIRE")].CStr());
				else if (pMapItem->Type() == MAPITEM_CHEST) txt.Addf(_T(" - %s"),gItemMgr.m_TextMgr[_T("MAPRES_CHEST")].CStr());
				else if (pMapItem->Type() == MAPITEM_SPELLSCROLL) txt.Addf(_T(" - %s"),gItemMgr.m_TextMgr[_T("MAPRES_SPELLSCROLL")].CStr());
				else if (pMapItem->Type() == MAPITEM_ARTIFACT) txt.Addf(_T(" - %s"),gItemMgr.m_TextMgr[_T("MAPRES_ARTIFACT")].CStr());
			}
			m_pStatusBar->SetText(txt);
		}
		return true;
	}
	return false;
}

void CMapEditorView::CenterView(const iSize& clsiz)
{
	m_offset.y = 0;
	m_offset.x = (m_Composer.GetMapPixelWidth()+32)/2 - clsiz.w/2;
	UpdateMiniMapFrame();
}

HWND CMapEditorView::CreateMainView(HWND hParent, IStatusBar* pStatusBar, iItemBrowser* pItemBrowser, iMinimapView* pMinimapView)
{
	m_pItemBrowser = pItemBrowser;
	m_pStatusBar = pStatusBar;
	m_pMinimapView = pMinimapView;
	RECT rect={0,0,1,1};
	if (Create(hParent, rect,NULL,WS_VISIBLE|WS_CHILD|WS_VSCROLL|WS_HSCROLL,WS_EX_STATICEDGE) == NULL) return NULL;
	return m_hWnd;
}

void CMapEditorView::OnMapContentChanged()
{
}

void CMapEditorView::HandleDrawing(const iPoint& cell)
{
	if (!m_pMap->IsValidPos(cell.x, cell.y)) return;

	if (iDecorNode* pDecorNode = DynamicCast<iDecorNode*>(m_pSelNode)) {
		if (m_pMap->GetCellObject(cell)) return;
		m_pMap->AddDecoration(m_SelPoint.x,m_SelPoint.y,pDecorNode);
	} else if (iPathElNode* pPathElNode = DynamicCast<iPathElNode*>(m_pSelNode)) {
		m_pMap->AddPathEl(m_SelPoint.x,m_SelPoint.y,pPathElNode);
	} else if (iHeroNode* pHeroNode = DynamicCast<iHeroNode*>(m_pSelNode)) {
		if (m_pMap->GetCurPlayerId() == PID_NEUTRAL) MessageBox(_T("Unable to put neutral hero!"));
		else if (!pHeroNode->IsAvailable()) MessageBox(_T("This hero is already in scenario!"));
		else {
			m_pMap->AddHero(pHeroNode->CreateObject(cell,m_pMap->GetCurPlayerId()));
			check(m_pItemBrowser);
			m_pItemBrowser->Update();
		}
	} else if (DynamicCast<iUltimateArtNode*>(m_pSelNode)) {
		m_pMap->m_posUltimateArt = cell;
	} else if (
		DynamicCast<iGuardNode*>(m_pSelNode) || 
		DynamicCast<iArtifactNode*>(m_pSelNode) || 
		DynamicCast<iManaCrystalNode*>(m_pSelNode) || 
		DynamicCast<iMineralNode*>(m_pSelNode) || 
		DynamicCast<iCampFireNode*>(m_pSelNode) || 
		DynamicCast<iChestNode*>(m_pSelNode) || 
		DynamicCast<iSpellScrollNode*>(m_pSelNode) || 
		DynamicCast<iLampNode*>(m_pSelNode) || 
		DynamicCast<iKeyGuardNode*>(m_pSelNode) || 
		DynamicCast<iMapEventNode*>(m_pSelNode) || 
		DynamicCast<iDecorNode*>(m_pSelNode) || 
		DynamicCast<iBaseVisCnstNode*>(m_pSelNode) || 
		DynamicCast<iBaseOwnCnstNode*>(m_pSelNode) || 
		DynamicCast<iCastleNode*>(m_pSelNode)) {
		m_pMap->AddObject(m_pSelNode,m_pMap->GetCurPlayerId(),m_SelPoint);
	}
	m_pMinimapView->OnMapContentChanged();
}

void CMapEditorView::SetMap(iMapHandler* pMap)
{
	m_pMap = pMap;
	m_Composer.InitComposer(m_pMap);
	CenterView(iWMetrics::GetClSize(m_hWnd));
	UpdateScrollBars(iWMetrics::GetClSize(m_hWnd));
	UpdateMiniMapFrame();
}

void CMapEditorView::UpdateScrollBars(const iSize &clsiz)
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE|SIF_RANGE|SIF_POS;
	si.nMin = 0;

	if (m_pMap) si.nMax = m_Composer.GetMapPixelWidth()+40*2;
	else si.nMax = 0;

	si.nPage = clsiz.w;
	si.nPos = m_offset.x;
	SetScrollInfo(SB_HORZ,&si);
	
	if (m_pMap) si.nMax = si.nMax = m_Composer.GetMapPixelHeight()+20*2;
	else si.nMax = 0;
	si.nPage = clsiz.h;
	si.nPos = m_offset.y;
	SetScrollInfo(SB_VERT,&si);
}

LRESULT CMapEditorView::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if (wParam == 7777) {
		if (!iWMetrics::IsMouseOnWindow(m_hWnd)) {
			m_bMouseOnWindow = false;
			KillTimer(7777);
			SetSelPoint(cInvalidPoint);
			Invalidate();
		}
	}
	return 0;
}

LRESULT CMapEditorView::OnLButtonDblClk(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT CMapEditorView::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(spacePressed)
			return 0;
	SetCapture();
	m_bTracking = true;

	if (!m_pMap || !m_pMap->IsValidPos(m_SelPoint.x,m_SelPoint.y)) return 0;
	
	

	if (m_NavMode == NM_Erase) {
		if (m_SelPoint == m_pMap->m_posUltimateArt) {
			m_pMap->m_posUltimateArt = cInvalidPoint;
			m_pMap->m_radUltimateArt = 0; 
		} else if (iBaseMapObject* pObj = m_pMap->GetCellObject(m_SelPoint)) {
			if (iHero* pHero = DynamicCast<iHero*>(pObj)) {
				pHero->Proto()->Reset();
				check(m_pItemBrowser);
				m_pItemBrowser->Update();
			}
			m_pMap->RemoveMapObject(pObj);
		} else {
			m_pMap->RemoveDecoration(m_SelPoint);
		}
		m_pMinimapView->OnMapContentChanged();
		Invalidate();
	} else if (m_NavMode == NM_Edit) {
		if (m_pMap && m_pMap->IsValidPos(m_SelPoint.x,m_SelPoint.y)) {
			iBaseMapObject* pObj = m_pMap->GetCellObject(m_SelPoint);
			if (m_pMap->m_posUltimateArt == m_SelPoint) {
				iUltArtPropsDlg dlg(*m_pMap);
				dlg.DoModal();
			} else if (iMapItem* pMapItem = DynamicCast<iMapItem*>(pObj)){
				MAP_ITEM_TYPE mit = pMapItem->Type();
				CPropertySheetEx sheet( _T("Map item props") );
				iPropsPage_Message pageMsg(m_pMap->m_ctMgr, pMapItem->Message(), pMapItem, TET_MAPITEM_MSG, _T("Message"));
				iPropsPage_Army pageGuard(pMapItem->Guard(), _T("Guard"));
				iPropsPage_GenMineral* pageGenMineralPtr=NULL;
				iPropsPage_GenSpellScroll* pageGenSpellScrollPtr=NULL;
				if (mit == MAPITEM_MINERAL) {
					iMapItem_Mineral* pMineral =  (iMapItem_Mineral*)pMapItem;
					if (pMineral->MineralType() == MINERAL_UNKNOWN) return 0;
					pageGenMineralPtr = new iPropsPage_GenMineral(pMineral->MineralType() , pMineral->MineralCount());
					sheet.AddPage(*pageGenMineralPtr);
				} else if (mit == MAPITEM_SPELLSCROLL) {
					iMapItem_SpellScroll* pSpellScroll =  (iMapItem_SpellScroll*)pMapItem;
					pageGenSpellScrollPtr = new iPropsPage_GenSpellScroll(*pSpellScroll);
					sheet.AddPage(*pageGenSpellScrollPtr);
				}
				sheet.AddPage(pageMsg);
				sheet.AddPage(pageGuard);
				sheet.DoModal(m_hWnd);
				if (pageGenMineralPtr) delete pageGenMineralPtr;
				if (pageGenSpellScrollPtr) delete pageGenSpellScrollPtr;
			} else if (iMapEvent* pEvent = DynamicCast<iMapEvent*>(pObj)) {
				CPropertySheetEx sheet( _T("Map event props") );
				iPropsPage_GenMapEvent pageGen(*m_pMap, *pEvent);
				iPropsPage_Army pageGuard(pEvent->Guard(), _T("Guard"));
				iPropsPage_Rewards pageRewards(pEvent->Rewards());
				sheet.AddPage(pageGen);
				sheet.AddPage(pageGuard);
				sheet.AddPage(pageRewards);
				sheet.DoModal(m_hWnd);
			} else if (iMapGuard* pGuard = DynamicCast<iMapGuard*>(pObj)) {
				CPropertySheetEx sheet( _T("Guard props") );
				iPropsPage_GenGuard pageGenGuard(*pGuard);
				iPropsPage_Message pageMsg(m_pMap->m_ctMgr, pGuard->Message(), pGuard, TET_MAPGUARD_MSG, _T("Message"));
				sheet.AddPage(pageGenGuard);
				sheet.AddPage(pageMsg);
				sheet.DoModal(m_hWnd);
			} else if (iVisCnst* pVisCnst = DynamicCast<iVisCnst*>(pObj)) {
				CPropertySheetEx sheet( gItemMgr.m_TextMgr[pVisCnst->Proto()->NameKey()].CStr() );
				iPropsPage_Army* pageGuardPtr = NULL;
				iPropsPage_Message* pageMsgPtr = NULL;
				if (!pVisCnst->Proto()->FixedGuard()) {
					pageGuardPtr = new iPropsPage_Army(pVisCnst->Guard(), _T("Guard"));
					sheet.AddPage(*pageGuardPtr);
				}
				if (pVisCnst->Proto()->CustomizableMessage()) {
					pageMsgPtr = new iPropsPage_Message(m_pMap->m_ctMgr, pVisCnst->CustomMessage(), pVisCnst, TET_VIS_MSG, _T("Message"));
					sheet.AddPage(*pageMsgPtr);
				}
				sheet.DoModal(m_hWnd);
				if (pageGuardPtr) delete pageGuardPtr;
				if (pageMsgPtr) delete pageMsgPtr;
			} else if (iOwnCnst* pOwnCnst = DynamicCast<iOwnCnst*>(pObj)) {
				CPropertySheetEx sheet( gItemMgr.m_TextMgr[pOwnCnst->Proto()->NameKey()].CStr() );
				iPropsPage_GenOwnCnst pageGenOwnCnst(*pOwnCnst,*m_pMap);
				iPropsPage_Army pageGuard(pOwnCnst->Guard(), _T("Guard"));
				sheet.AddPage(pageGenOwnCnst);
				sheet.AddPage(pageGuard);
				sheet.DoModal(m_hWnd);
			} else if (iCastle* pCastle = DynamicCast<iCastle*>(pObj)) {
				CPropertySheetEx sheet( _T("Castle props") );
				iPropsPage_GenCastle pageGenCastle(*pCastle,*m_pMap);
				iPropsPage_CtlCnsts pageCtlCnsts(*pCastle,*m_pMap);
				iPropsPage_Army pageGarrison(pCastle->Garrison(), _T("Garrison"));
				sheet.AddPage(pageGenCastle);
				sheet.AddPage(pageCtlCnsts);
				sheet.AddPage(pageGarrison);
				sheet.DoModal(m_hWnd);
			} else if (iHero* pHero = DynamicCast<iHero*>(pObj)) {
				CPropertySheetEx sheet( _T("Hero props") );
				iPropsPage_GenHero pageGenHero(*pHero, *m_pMap);
				iPropsPage_SecSkills pageSecSkills(*pHero, *m_pMap);
				iPropsPage_Spellbook pageSpellBook(*pHero);
				iPropsPage_Army pageArmy(pHero->Army(), _T("Army"));
				sheet.AddPage(pageGenHero);
				sheet.AddPage(pageSecSkills);
				sheet.AddPage(pageSpellBook);
				sheet.AddPage(pageArmy);
				sheet.DoModal(m_hWnd);
			}
		}
		m_pMinimapView->OnMapContentChanged();
	} else if (m_NavMode == NM_Surface){
		m_pMap->SetSurf(m_SelPoint.x,m_SelPoint.y, m_selBrush, m_selSurf);
		m_pMinimapView->OnMapContentChanged();
		Invalidate();
	} else if (m_NavMode == NM_Create){
		HandleDrawing(m_SelPoint);
		Invalidate();
	}

	return 0;
}

LRESULT CMapEditorView::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_bTracking = false;
	ReleaseCapture();
	return 0;
}

LRESULT CMapEditorView::OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = false;
 	if(wParam == VK_SPACE)
		spacePressed = true;
	return 1;
}

LRESULT CMapEditorView::OnKeyUp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(wParam == VK_SPACE)
		spacePressed = false;
	return 0;
}


LRESULT CMapEditorView::OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	static int lastXPos = 0, lastYPos = 0;
	int xPos = GET_X_LPARAM(lParam); 
	int yPos = GET_Y_LPARAM(lParam); 

	if(LOWORD(wParam) == MK_LBUTTON && spacePressed) {
		if(xPos < lastXPos) {
			this->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINERIGHT, 0));	
			this->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINERIGHT, 0));	
		}
		else if(xPos > lastXPos) {
			this->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINELEFT, 0));	
			this->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINELEFT, 0));	
		}
		if(yPos < lastYPos) {
			this->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0));	
			this->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0));	
		}
		else if(yPos > lastYPos) {
			this->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0));			
			this->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0));			
		}
		lastXPos = xPos;
		lastYPos = yPos;
		return 0;
	}
	
	
	if (!m_bMouseOnWindow) {
		m_bMouseOnWindow = true;
		SetTimer(7777,100);
	}

	if (!m_pMap) return 0;

	iPoint pos(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
	if (m_NavMode == NM_Create){
		iIsoMetric im(5);
		iPoint nsel = im.Screen2Map(iPoint(m_offset.x-m_Composer.GetMapPixelWidth()/2+pos.x-40,m_offset.y+pos.y-20+10));
		if (DynamicCast<iPathElNode*>(m_pSelNode)) {
			if (SetSelPoint(nsel)) {
				if (m_bTracking && m_pMap->IsValidPos(m_SelPoint.x,m_SelPoint.y)) {
					HandleDrawing(m_SelPoint);
				}
				Invalidate();
			}
		} else {
			if (SetSelPoint(nsel)) Invalidate();
		}
	} else if (m_NavMode == NM_Surface){
		iIsoMetric im(5);
		iPoint nsel = im.Screen2Map(iPoint(m_offset.x-m_Composer.GetMapPixelWidth()/2+pos.x-40,m_offset.y+pos.y-20+20));
		if (SetSelPoint(nsel)) {
			if (m_bTracking && m_pMap->IsValidNode(m_SelPoint.x,m_SelPoint.y)) {
				m_pMap->SetSurf(m_SelPoint.x,m_SelPoint.y, m_selBrush, m_selSurf);
			}
			m_pMinimapView->OnMapContentChanged();
			Invalidate();
		}
	} else if (m_NavMode == NM_Erase || m_NavMode == NM_Edit){
		iIsoMetric im(5);
		iPoint nsel = im.Screen2Map(iPoint(m_offset.x-m_Composer.GetMapPixelWidth()/2+pos.x-40,m_offset.y+pos.y-20+10));
		if (SetSelPoint(nsel)) Invalidate();
	}

	return 0;
}

LRESULT CMapEditorView::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	iSize nsiz(LOWORD(lParam),HIWORD(lParam));
	UpdateScrollBars(nsiz);
	UpdateMiniMapFrame();
	return 0;
}

LRESULT CMapEditorView::OnVScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(!m_pMap)
		return 0;
	
	uint32 vis_h = iWMetrics::GetClSize(m_hWnd).h;
	sint32 totalh = m_Composer.GetMapPixelHeight()+20*2;
	sint32 yNewPos;
	sint32 yCurPos = GetScrollPos(SB_VERT);

	switch(LOWORD(wParam)) {
		case SB_PAGEUP:
			yNewPos = yCurPos - 30;
			break;
		case SB_PAGEDOWN:
			yNewPos = yCurPos + 30;
			break;
		case SB_LINEUP:
			yNewPos = yCurPos - 10;
			break;
		case SB_LINEDOWN:
			yNewPos = yCurPos + 10;
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			yNewPos = HIWORD(wParam);
			break;
		default:
			yNewPos = yCurPos;
	}
	yNewPos = iCLAMP<sint32>(0,totalh-vis_h,yNewPos);
	if (yNewPos != yCurPos) {
		SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_POS;
		si.nPos = yNewPos;
		SetScrollInfo(SB_VERT,&si,TRUE);
		m_offset.y = yNewPos;
		Invalidate();
		UpdateMiniMapFrame();
	}
	return 0;
}

LRESULT CMapEditorView::OnHScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(!m_pMap)
		return 0;
	uint32 vis_w = iWMetrics::GetClSize(m_hWnd).w;
	sint32 totalw = m_Composer.GetMapPixelWidth()+40*2;
	sint32 xNewPos;
	sint32 xCurPos = GetScrollPos(SB_HORZ);
	
	switch(LOWORD(wParam)){
		case SB_PAGELEFT:
			xNewPos = xCurPos - 30;
			break;
		case SB_PAGERIGHT:
			xNewPos = xCurPos + 30;
			break;
		case SB_LINELEFT:
			xNewPos = xCurPos - 10;
			break;
		case SB_LINERIGHT:
			xNewPos = xCurPos + 10;
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			xNewPos = HIWORD(wParam);
			break;
		default:
			xNewPos = xCurPos;
	}

	xNewPos = iCLAMP<sint32>(0,totalw-vis_w,xNewPos);
	if (xNewPos != xCurPos) {
		SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_POS;
		si.nPos = xNewPos;
		SetScrollInfo(SB_HORZ,&si,TRUE);
		m_offset.x = xNewPos;
		Invalidate();
		UpdateMiniMapFrame();
	}
	return 0;
}

LRESULT CMapEditorView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	PAINTSTRUCT ps;
	HDC hdc=BeginPaint(&ps);
	iSize cl_siz = iWMetrics::GetClSize(m_hWnd);
	if (m_pMap) {
		m_Composer.Compose(hdc,iWMetrics::GetClSize(m_hWnd),m_offset, m_SelPoint, (m_NavMode==NM_Create)?m_pSelNode:NULL);
	} else {
		iDCHLP::FillWnd(hdc, cl_siz, cColorBlue128);
	}
	EndPaint(&ps);
	return 0;
}

void CMapEditorView::UpdateMiniMapFrame()
{
	if (!m_pMap) return;
	iSize clSiz = iWMetrics::GetClSize(m_hWnd);
	iIsoMetric im(5);
	iPoint lt = im.Screen2Map(iPoint(m_offset.x-m_Composer.GetMapPixelWidth()/2,m_offset.y));
	iPoint rb = im.Screen2Map(iPoint(m_offset.x-m_Composer.GetMapPixelWidth()/2+clSiz.w,m_offset.y+clSiz.h));
	m_pMinimapView->UpdateSelFrame(lt,rb);
}