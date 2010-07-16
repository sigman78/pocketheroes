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

#include "si.common.h"
#include "si.osspec.h"
#include "si.window.generic.h"
#include "si.window.basenw.h"
#include "../he/he.wnd.helpers.h"
#include <mmsystem.h>

#pragma comment (lib,"winmm")

const uint32 NWCONTAINER_IMAGE_TRUNC = 32;

iBaseWindowNW::iBaseWindowNW()
: m_Destroying(false)
, m_LastFocusedElement(NULL)
, m_TrackElement(NULL)
{}

bool iBaseWindowNW::CreateBaseWindowNW(const iRect &rect, HWND parent, uint32 style, uint32 exstyle, bool layered)
{
	m_dclTime = 0;
	m_dclElement = NULL;
	m_dclPoint = cInvalidPoint;

	m_Destroying = false;
	iSize wsiz(rect);
	if (wsiz.w<NWCONTAINER_IMAGE_TRUNC) wsiz.w=NWCONTAINER_IMAGE_TRUNC;
	if (wsiz.h<NWCONTAINER_IMAGE_TRUNC) wsiz.h=NWCONTAINER_IMAGE_TRUNC;

	ResizeComposingDib(wsiz);
	if (layered){
		m_ScreenDC = ::GetDC(NULL);
	}

	return CreateGenericWindow(rect,parent,style,exstyle,layered);
}


void iBaseWindowNW::RepaintElement(const iNWElementI *el, const iRect &rect)
{
	RedrawWnd(rect);
}

void iBaseWindowNW::DrawBackground(iMemoryDC &ddc, const iRect &rect)
{
	if (rect == cInvalidRect) ddc.m_Dib.Fill(iColor(255,0,0,128),BLEND_SRCCOPY);
	else ddc.m_Dib.FillRect(rect,iColor(128,0,0,128),BLEND_SRCCOPY); 
}

void iBaseWindowNW::RegisterElement(iNWElementI *el)
{
	m_Elements.Add(el);
	RedrawWnd(el->GetElementRect());
}

void iBaseWindowNW::UnRegisterElement(iNWElementI *el)
{
	m_Elements.FindRemove(el);
}

void iBaseWindowNW::UnRegisterAllElements(void)
{
	m_Elements.RemoveAll();
}

void iBaseWindowNW::AddMessageSubscriber(iNWSubscriberI* subs)
{
	m_Subscribers.Add(subs);
}

void iBaseWindowNW::RemoveMessageSubscriber(iNWSubscriberI* subs)
{
	m_Subscribers.FindRemove(subs);
}

void iBaseWindowNW::RemoveAllMessageSubscribers(void)
{
	m_Subscribers.RemoveAll();
}

void iBaseWindowNW::ControlMessage(iNWMessage& msg)
{
	for (iFastArray<iNWSubscriberI*>::Iterator it=m_Subscribers.First(); it != m_Subscribers.End(); ++it) {
		if ((*it)->OnNWMessage(msg) == true) return;
	}
}

iNWElementI* iBaseWindowNW::GetElementByPoint(const iPoint &pnt)
{
	for (iFastArray<iNWElementI*>::Iterator it=m_Elements.First(); it != m_Elements.End(); it++) {
		iNWElementI* ctrl = *it;

		if (ctrl!=NULL && ctrl->IsMyPoint(pnt)) {
			return ctrl;
		}
	}
	return NULL;
}

iPoint iBaseWindowNW::NWClientToScreen(const iPoint &pnt) const
{
	return iPoint(pnt + GetWndRect().point());
}

iRect iBaseWindowNW::NWClientToScreen(const iRect &rect) const
{
	return iRect(rect.point() + GetWndRect().point(),rect.size());
}

//
//
//

void iBaseWindowNW::MsgPaint(iDC &dc, PAINTSTRUCT &ps)
{
	DrawBackground(m_outDC, iRect(ps.rcPaint));

	for (iFastArray<iNWElementI*>::Iterator it = m_Elements.First(); it != m_Elements.End(); ++it) {
		if ((*it)->IsElementVisible()) (*it)->DrawElement(m_outDC);
	}
	::BitBlt(dc,0,0,m_outDC.GetSize().w,m_outDC.GetSize().h,m_outDC,0,0,SRCCOPY);
}

void iBaseWindowNW::MsgLayeredPaint(const iRect &rect)
{
	DrawBackground(m_outDC);
	for (iFastArray<iNWElementI*>::Iterator it = m_Elements.First(); it != m_Elements.End(); ++it){
		if ((*it)->IsElementVisible()) (*it)->DrawElement(m_outDC);
	}

	// Create Temporal dib
	iMemoryDC prdc(m_outDC.GetSize());


	// pre-multiply rgb channels with alpha channel
	for (uint32 yy=0; yy<m_outDC.GetSize().h; ++yy){
		const uint8 *src= (const uint8*)m_outDC.m_Dib.GetLine(yy);
		uint8 *dst= (uint8*)prdc.m_Dib.GetLine(yy);

		for (uint32 xx=0; xx<m_outDC.GetSize().w; ++xx){
			if (src[3]==0) {
				*((uint32*)dst) = 0;
			} else if (src[3]==255) {
				*((uint32*)dst) = *((uint32*)src);
			} else {
				dst[0]= (uint8)((uint32)(src[0]*src[3])>>8);
				dst[1]= (uint8)((uint32)(src[1]*src[3])>>8);
				dst[2]= (uint8)((uint32)(src[2]*src[3])>>8);
				dst[3]= (uint8)src[3];
			}
			dst+=4;
			src+=4;
		}
	}

	iPoint pos(0,0);
	iPoint wnd_pos = GetWndPos();
	iSize wnd_siz = GetWndSize();

	BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	BOOL bRet = OSSPECFNC::UpdateLayeredWindow(m_hWnd, m_ScreenDC, &POINT(wnd_pos), &SIZE(wnd_siz), prdc, &POINT(pos),0,&blendPixelFunction,0x00000002/*means ULW_ALPHA*/);
}

void iBaseWindowNW::MsgSize(const iSize &size)
{

	ResizeComposingDib(size);
	RedrawWnd();

	iGenericWindow::MsgSize(size);
}

void iBaseWindowNW::ResizeComposingDib(const iSize &size)
{
	iSize s(size);
	if (s.w<10) s.w=10;
	if (s.h<10) s.h=10;
	// here is two mods --- if window is layered, then out buffer will be
	// same as window size, otherwise dib's size will be align to NWCONTAINER_IMAGE_TRUNC
	if (m_IsLayered) {
		if (s != m_outDC.GetSize()) m_outDC.Resize(s);
	} else {
		uint32 oldx=m_outDC.GetSize().w/NWCONTAINER_IMAGE_TRUNC;
		if ((m_outDC.GetSize().w % NWCONTAINER_IMAGE_TRUNC) > 0) oldx++;

		uint32 oldy=m_outDC.GetSize().h/NWCONTAINER_IMAGE_TRUNC;
		if ((m_outDC.GetSize().h % NWCONTAINER_IMAGE_TRUNC) > 0) oldy++;

		uint32 newx=s.w/NWCONTAINER_IMAGE_TRUNC+1;
		if ((s.w % NWCONTAINER_IMAGE_TRUNC) > 0) newx++;

		uint32 newy=s.h/NWCONTAINER_IMAGE_TRUNC+1;
		if ((s.h % NWCONTAINER_IMAGE_TRUNC) > 0) newy++;

		if (oldx == newx && oldy==newy) return;

		iSize wsiz=iSize(newx*NWCONTAINER_IMAGE_TRUNC, newy*NWCONTAINER_IMAGE_TRUNC);
		if (wsiz.w<NWCONTAINER_IMAGE_TRUNC) wsiz.w=NWCONTAINER_IMAGE_TRUNC;
		if (wsiz.h<NWCONTAINER_IMAGE_TRUNC) wsiz.h=NWCONTAINER_IMAGE_TRUNC;

		m_outDC.Resize(wsiz);
	}
}

void iBaseWindowNW::MsgDestroy(void)
{
	ReleaseDC(m_ScreenDC);
	UnRegisterAllElements();
	RemoveAllMessageSubscribers();
	m_LastFocusedElement = NULL;
	m_TrackElement = NULL;
	m_Destroying=true;

	iGenericWindow::MsgDestroy();
}

void iBaseWindowNW::MsgActivate(bool fActive, bool fMinimized)
{
	if (!fActive)
	{
		if (m_LastFocusedElement!=NULL)
		{
			iNWElementI* ctrl=m_LastFocusedElement;
			if (ctrl!=NULL && ctrl->IsElementEnabled() && ctrl->IsElementVisible())
			{
				ctrl->OnMouseLeave();
			}
			m_LastFocusedElement=NULL;
		}
	}
	//else if (fActive) OutputDebugString("ActiveWindow\n\n");

	iGenericWindow::MsgActivate(fActive,fMinimized);
}

void iBaseWindowNW::MsgTracking(const iPoint &pos)
{
	if (m_TrackElement){
		m_TrackElement->OnMouseMove(pos - m_TrackElement->GetElementRect().point());
	}
	iNWElementI* ctrl = GetElementByPoint(pos);

	if (m_bTracked && m_TrackElement) {
		m_TrackElement->OnMouseMove(pos - m_TrackElement->GetElementRect().point());
		if (ctrl != m_LastFocusedElement && m_TrackElement->IsElementEnabled() && m_TrackElement->IsElementVisible()) {
			if (ctrl == m_TrackElement) {
				m_LastFocusedElement=ctrl;
				m_TrackElement->OnMouseEnter();
			} else if (m_LastFocusedElement == m_TrackElement) {
				m_LastFocusedElement = NULL;
				m_TrackElement->OnMouseLeave();
			}
		}
	} else if (ctrl!=NULL && ctrl->IsElementEnabled() && ctrl->IsElementVisible()){
		ctrl->OnMouseMove(pos - ctrl->GetElementRect().point());
		if (ctrl != NULL && ctrl->IsElementEnabled() && ctrl->IsElementVisible()) SetCursor(ctrl->GetCursor(pos - ctrl->GetElementRect().point()));
	}

	
	//else SetCursor(LoadCursor(NULL, IDC_ARROW));

	iGenericWindow::MsgTracking(pos);
}

void iBaseWindowNW::MsgMouseMove(const iPoint &pos)
{
	iNWElementI* ctrl=GetElementByPoint(pos);
	if (ctrl!=NULL && ctrl->IsElementEnabled() && ctrl->IsElementVisible()) ctrl->OnMouseMove(pos - ctrl->GetElementRect().point());
	if (ctrl != m_LastFocusedElement) {
		iNWElementI *oldctrl = m_LastFocusedElement;
		iNWElementI *newctrl = ctrl;

		if (m_LastFocusedElement != NULL && oldctrl!=NULL && oldctrl->IsElementEnabled() && oldctrl->IsElementVisible()) {
			oldctrl->OnMouseLeave();
		}
		
		if (ctrl != NULL && newctrl!=NULL && newctrl->IsElementEnabled() && newctrl->IsElementVisible()){
			newctrl->OnMouseEnter();
		}
		m_LastFocusedElement = ctrl;
	}

	if (ctrl != NULL && ctrl->IsElementEnabled() && ctrl->IsElementVisible()) SetCursor(ctrl->GetCursor(pos - ctrl->GetElementRect().point()));
	else SetCursor(LoadCursor(NULL, IDC_ARROW));

	iGenericWindow::MsgMouseMove(pos);
}

void iBaseWindowNW::MsgMouseLeave(void)
{
	iNWElementI *oldctrl = m_LastFocusedElement;
	if (m_LastFocusedElement != NULL && oldctrl!=NULL && oldctrl->IsElementEnabled() && oldctrl->IsElementVisible()) {
		oldctrl->OnMouseLeave();
		m_LastFocusedElement = NULL;
	}

	iGenericWindow::MsgMouseLeave();
}

void iBaseWindowNW::MsgLButtonDown(const iPoint &pos, uint32 vkey)
{
	m_TrackElement = GetElementByPoint(pos);

	if (m_TrackElement!=NULL && m_TrackElement->IsElementEnabled() && m_TrackElement->IsElementVisible()) {
		iPoint elpt = pos - m_TrackElement->GetElementRect().point();
		m_TrackElement->BeginTrack(elpt);
		m_TrackElement->OnLButtonDown(elpt,vkey);
		SetCursor(m_TrackElement->GetCursor(pos - m_TrackElement->GetElementRect().point()));
	}

	iGenericWindow::MsgLButtonDown(pos,vkey);
}

void iBaseWindowNW::MsgLButtonUp(const iPoint &pos, uint32 vkey)
{
	uint32 nclTime = timeGetTime();

	iNWElementI* ctrl=GetElementByPoint(pos);
	if (ctrl!=NULL && ctrl->IsElementEnabled() && ctrl->IsElementVisible() && !m_Destroying) {
		if (ctrl == m_TrackElement) {
			ctrl->OnLButtonUp(pos - ctrl->GetElementRect().point(),vkey);

			if (m_dclElement != NULL 
			&& m_dclElement == ctrl 
			&& m_dclPoint!=cInvalidPoint 
			&& m_dclPoint.GetDelta(pos) < 5 
			&& (nclTime - m_dclTime)<500) {
				ctrl->OnDblClick(pos - ctrl->GetElementRect().point());
				m_dclPoint = cInvalidPoint;
				m_dclElement = NULL;
				m_dclTime = 0;
			} else {
				ctrl->OnMouseClick(pos - ctrl->GetElementRect().point());
				m_dclPoint = pos;
				m_dclElement = ctrl;
				m_dclTime = nclTime;
			}
		}
	} else {
		m_dclPoint = cInvalidPoint;
		m_dclElement = NULL;
		m_dclTime = 0;
	}

	if (m_TrackElement!=NULL && !m_Destroying) {
		iNWElementI* track_ctrl = m_TrackElement;
		track_ctrl->OnLButtonUp(pos - track_ctrl->GetElementRect().point(),vkey);
		track_ctrl->EndTrack();
	}

	m_TrackElement=NULL;
	iGenericWindow::MsgLButtonUp(pos,vkey);
}

void iBaseWindowNW::MsgRButtonDown(const iPoint &pos, uint32 vkey)
{
	iNWElementI* ctrl=GetElementByPoint(pos);
	if (ctrl!=NULL && ctrl->IsElementEnabled() && ctrl->IsElementVisible()) {
		ctrl->OnRButtonDown(pos - ctrl->GetElementRect().point(),vkey);
	}
	iGenericWindow::MsgRButtonDown(pos,vkey);
}

void iBaseWindowNW::MsgRButtonUp(const iPoint &pos, uint32 vkey)
{
	iNWElementI* ctrl=GetElementByPoint(pos);
	if (ctrl!=NULL && ctrl->IsElementEnabled() && ctrl->IsElementVisible()) {
		ctrl->OnRButtonUp(pos - ctrl->GetElementRect().point(),vkey);
	}
	iGenericWindow::MsgRButtonUp(pos,vkey);
}

void iBaseWindowNW::MsgMButtonDown(const iPoint &pos, uint32 vkey)
{
	iNWElementI* ctrl=GetElementByPoint(pos);
	if (ctrl!=NULL && ctrl->IsElementEnabled() && ctrl->IsElementVisible()) {
		ctrl->OnMButtonDown(pos - ctrl->GetElementRect().point(),vkey);
	}
	iGenericWindow::MsgMButtonDown(pos,vkey);
}

void iBaseWindowNW::MsgMButtonUp(const iPoint &pos, uint32 vkey)
{
	iNWElementI* ctrl=GetElementByPoint(pos);
	if (ctrl!=NULL && ctrl->IsElementEnabled() && ctrl->IsElementVisible()) {
		ctrl->OnMButtonUp(pos - ctrl->GetElementRect().point(),vkey);
	}
	iGenericWindow::MsgMButtonUp(pos,vkey);
}

void iBaseWindowNW::MsgKeyDown(uint32 vkeycode)
{
	iPoint pos = iWMetrics::GetCursorPos(m_hWnd);
	iNWElementI* ctrl=GetElementByPoint(pos);
	if (ctrl!=NULL && ctrl->IsElementEnabled() && ctrl->IsElementVisible()) {
		ctrl->OnKeyDown(vkeycode,pos - ctrl->GetElementRect().point());
	}
	iGenericWindow::MsgKeyDown(vkeycode);
}

void iBaseWindowNW::MsgKeyUp(uint32 vkeycode)
{
	iPoint pos = iWMetrics::GetCursorPos(m_hWnd);
	iNWElementI* ctrl=GetElementByPoint(pos);
	if (ctrl!=NULL && ctrl->IsElementEnabled() && ctrl->IsElementVisible()) {
		ctrl->OnKeyUp(vkeycode,pos - ctrl->GetElementRect().point());
	}
	iGenericWindow::MsgKeyUp(vkeycode);
}
