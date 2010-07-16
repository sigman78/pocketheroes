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
#include "si.nwcontrol.base.h"


iNWControl::iNWControl(void)
: m_Tracked(false)
, m_IsVisible(true)
, m_IsEnabled(true)
, m_IsCreated(false)
, m_Parent(NULL)
{
}

bool iNWControl::CreateNWElement(iNWElementContainerI *parent, const iRect &rect)
{
	m_ElementRect=rect;
	m_Parent=parent;
	m_IsCreated = true;
	return true;
}

void iNWControl::ForceRepaint(void) const
{
	if (m_Parent) GetOwner()->RepaintElement(this,GetElementRect());
}

void iNWControl::DrawElement(iMemoryDC &ddc) const
{
	ddc.m_Dib.FillRect(GetElementRect(),cColorGreen64,BLEND_SRCCOPY);
}

iNWElementContainerI* iNWControl::GetOwner(void) const
{
	return m_Parent;
}

uint32 iNWControl::GetElementWidth() const
{
	return m_ElementRect.w;
}

uint32 iNWControl::GetElementHeight() const
{
	return m_ElementRect.h;
}

const iRect& iNWControl::GetElementRect() const
{
	return m_ElementRect;
}

iRect iNWControl::GetElementRect()
{
	return m_ElementRect;
}

bool iNWControl::IsMyPoint(const iPoint &pnt) const
{
	return m_ElementRect.PtInRect(pnt);
}

void iNWControl::SetElementRect(const iRect &rect)
{
	m_ElementRect=rect;
	ForceRepaint();
}

void iNWControl::SetElementPos(const iPoint &pos)
{
	m_ElementRect.x = pos.x;
	m_ElementRect.y = pos.y;
	ForceRepaint();
}

void iNWControl::SetElementSize(const iSize &siz)
{
	m_ElementRect.w = siz.w;
	m_ElementRect.h = siz.h;
	ForceRepaint();
}

HCURSOR iNWControl::GetCursor(const iPoint &pos) const
{
	return ::LoadCursor(NULL, IDC_ARROW);
}

void iNWControl::BeginTrack(const iPoint &pos)
{
	m_Tracked=true;
	m_TrackAnchor = pos;
	ForceRepaint();
}

void iNWControl::EndTrack(void)
{
	m_Tracked=false;
	ForceRepaint();
}

bool iNWControl::IsTrack(void) const
{
	return m_Tracked;
}

const iPoint& iNWControl::GetTrackAnchor() const
{
	return m_TrackAnchor;
}

void iNWControl::OnMouseEnter(void)
{
	if (m_Parent) GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_MOUSE_ENTER,0,NULL));
}

void iNWControl::OnMouseLeave(void)
{
	if (m_Parent) GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_MOUSE_LEAVE,0,NULL));
}

void iNWControl::OnMouseMove(const iPoint &pos)
{
}

void iNWControl::OnMouseClick(const iPoint &pos)
{
	if (m_Parent) GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_MOUSE_CLICK,0,NULL));
}

void iNWControl::OnDblClick(const iPoint &pos)
{
	if (m_Parent) GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_DOUBLE_CLICK,0,NULL));
}

void iNWControl::OnLButtonDown(const iPoint &pos, uint32 vkey)
{
	if (m_Parent) GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_MOUSE_LDOWN,0,NULL));
}

void iNWControl::OnLButtonUp(const iPoint &pos, uint32 vkey)
{
	if (m_Parent) GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_MOUSE_LUP,0,NULL));
}

void iNWControl::OnRButtonDown(const iPoint &pos, uint32 vkey)
{
	if (m_Parent) GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_MOUSE_RDOWN,0,NULL));
}

void iNWControl::OnRButtonUp(const iPoint &pos, uint32 vkey)
{
	if (m_Parent) GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_MOUSE_RUP,0,NULL));
}

void iNWControl::OnMButtonDown(const iPoint &pos, uint32 vkey)
{
	if (m_Parent) GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_MOUSE_MDOWN,0,NULL));
}

void iNWControl::OnMButtonUp(const iPoint &pos, uint32 vkey)
{
	if (m_Parent) GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_MOUSE_MUP,0,NULL));
}

void iNWControl::OnLLMouseProc(const iPoint &pos, int flags)
{
}

void iNWControl::OnKeyDown(uint32 vkcode, const iPoint& mpos)
{
}

void iNWControl::OnKeyUp(uint32 vkcode, const iPoint& mpos)
{
}

void iNWControl::DestroyElement(void)
{
	m_IsCreated = false;
}

void iNWControl::ShowElement(void)
{
	if (m_IsVisible) return;
	m_IsVisible = true;
	if (m_Parent) GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_VIS_CHANGED,1,NULL));
	ForceRepaint();
}

void iNWControl::HideElement(void)
{
	if (!m_IsVisible) return;
	m_IsVisible = false;
	if (m_Parent) GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_VIS_CHANGED,0,NULL));
	ForceRepaint();
}

bool iNWControl::IsElementVisible(void) const
{
	return m_IsVisible;
}

bool iNWControl::IsElementCreated(void) const
{
	return m_IsCreated;
}

void iNWControl::SetElementState(bool bEn)
{
	if (m_IsEnabled == bEn) return;
	m_IsEnabled = bEn;
	if (m_Parent) GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_ENB_CHANGED,bEn?1:0,NULL));
	ForceRepaint();
}

void iNWControl::EnableElement(void)
{
	if (m_IsEnabled) return;
	m_IsEnabled = true;
	if (m_Parent) GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_ENB_CHANGED,1,NULL));
	ForceRepaint();
}

void iNWControl::DisableElement(void)
{
	if (!m_IsEnabled) return;
	m_IsEnabled = false;
	if (m_Parent) GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_ENB_CHANGED,0,NULL));
	ForceRepaint();
}

bool iNWControl::IsElementEnabled(void) const
{
	return m_IsEnabled;
}
