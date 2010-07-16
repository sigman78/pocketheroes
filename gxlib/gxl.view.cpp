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

#include "gxl.inc.h"
#ifdef OS_WINCE
#include <gx.h>
#endif //OS_WINCE
#include "gxl.ctr.cbuff.h"
#include "gxl.ctr.array.h"
#include "gxl.window.h"
#include "gxl.input.h"
#include "gxl.timer.h"
#include "gxl.display.h"
#include "gxl.view.h"
#include "gxl.dialog.h"
#include "gxl.viewmgr.h"
#include "gxl.application.h"


iView::iView(iViewMgr* pViewMgr, const iRect& rect, uint32 clsId, uint32 uid, uint32 state)
: m_bNeedRedraw(true), m_bTracking(false), m_pParent(NULL)
{ CreateView(pViewMgr,rect,clsId, uid, state); }

iView::~iView()
{ 
	Cleanup(); 
}

bool iView::CreateView(iViewMgr* pViewMgr, const iRect& rect, uint32 clsId, uint32 uid, uint32 state)
{
	if (!pViewMgr) return false;
	m_pMgr = pViewMgr;
	m_bVisible = (state & Visible)>0;
	m_bEnabled = (state & Enabled)>0;
	m_Rect = rect;
	m_clsId = clsId;
	m_UID = uid;
	return true;
}

void iView::Cleanup()
{
	for (uint32 xx=0; xx<m_Childs.GetSize(); ++xx) delete m_Childs[xx];
	m_Childs.RemoveAll();
}

bool iView::AddChild(iView* pChild)
{
	pChild->m_pParent = this;
	m_Childs.Add(pChild);
	return true;
}

bool iView::RemoveChild(iView* pChild)
{
	if (pChild && m_Childs.Remove(pChild)){
		pChild->m_pParent = NULL;
		return true;
	}
	return false;
}

iView* iView::GetChildByPos(const iPoint& pos) const
{
	// reverse find (in future remake to z-order)
	if (!m_Childs.GetSize()) return NULL;
	for (sint32 xx=m_Childs.GetSize()-1; xx>=0; --xx) {
		if (m_Childs[xx]->m_bVisible && m_Childs[xx]->m_bEnabled && m_Childs[xx]->m_Rect.PtInRect(pos)) return m_Childs[xx];
	}
	return NULL;
}

iView* iView::GetChildById(uint32 uid) const
{
	for (uint32 xx=0; xx<m_Childs.GetSize(); ++xx) {
		if (m_Childs[xx]->m_UID == uid) return m_Childs[xx];
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
void iView::Compose(iRect& rect)
{
	if (m_bVisible) {
		OnCompose();
		for (uint32 xx=0; xx<m_Childs.GetSize(); ++xx) m_Childs[xx]->Compose(rect);
		if (m_bNeedRedraw) {
			rect += GetScrRect();
			m_bNeedRedraw = false;
		}
	}
}

bool iView::MouseDown(const iPoint& pos)
{
	if (!m_bEnabled) {
		// only topmost window can receive messages in disabled state
		check(m_pParent == NULL);
		m_pMgr->SetViewCapture(this);
		m_bTracking = true;
		return true;
	}

	iView* vp = GetChildByPos(pos-GetScrRect().point());
	if (!vp || !vp->MouseDown(pos)) {
		OnMouseDown(pos);
		m_pMgr->SetViewCapture(this);
		m_bTracking = true;
	}
	return true;
}

bool iView::MouseUp(const iPoint& pos)
{
	if (m_bTracking) {
		m_bTracking = false;
		m_pMgr->ReleaseViewCapture();
		OnMouseUp(pos);
		if (GetScrRect().PtInRect(pos)) {
			OnMouseClick(pos);
		}
	} else {
		check(0);
	}
	
	return true;
}

bool iView::MouseTrack(const iPoint& pos)
{
	if (m_bTracking && m_bEnabled) OnMouseTrack(pos);
	return true;
}

//////////////////////////////////////////////////////////////////////////
void iView::SetSize(const iSize& nsiz)
{
	m_Rect.w = nsiz.w;
	m_Rect.h = nsiz.h;
	OnRectChanged(m_Rect);
}

void iView::SetPos(const iPoint& npos)
{
	m_Rect.x = npos.x;
	m_Rect.y = npos.y;
	OnRectChanged(m_Rect);
}

void iView::SetRect(const iRect& rect)
{
	m_Rect = rect; 
	OnRectChanged(m_Rect);
}

void iView::Invalidate()
{
	m_bNeedRedraw = true;
	if (m_pParent) m_pParent->Invalidate();
}

