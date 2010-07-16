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
#include "si.nwcontrol.swbutton.h"

iNWSwitchButton::iNWSwitchButton()
: m_StateCnt(0)
, m_CurrentState(0)
{
}

bool iNWSwitchButton::CreateNWSwitchButton(iNWElementContainerI *parent, const iRect &rect, const iDib &dib, uint32 state_cnt, iToolTipDispatcherI* pDisp, const iStringT &ttip)
{
	m_StateCnt = state_cnt;

	uint32 dw = dib.GetWidth() / (m_StateCnt+1); 
	uint32 dh = dib.GetHeight();

	check(state_cnt != 0);
	check((dib.GetWidth() % (state_cnt+1) == 0));
	check(dw == rect.w);
	check(dh == rect.h);

	m_Dib.Create(&dib);

	return CreateNWElementTT(parent,rect,pDisp,ttip);
}

void iNWSwitchButton::OnMouseClick(const iPoint &pos)
{
	if (m_StateCnt < 2) SetSwitchState();
	if (m_CurrentState >= (m_StateCnt - 1)) SetSwitchState();
	else SetSwitchState(m_CurrentState+1);
	iNWControl::OnMouseClick(pos);
}

void iNWSwitchButton::OnLButtonDown(const iPoint &pos, uint32 vkey)
{
	iNWControl::OnLButtonDown(pos,vkey);
}

void iNWSwitchButton::OnLButtonUp(const iPoint &pos, uint32 vkey)
{
	iNWControl::OnLButtonUp(pos,vkey);
}

void iNWSwitchButton::DrawElement(iMemoryDC &ddc) const
{
	uint32 dw = m_Dib.GetWidth() / (m_StateCnt+1); 
	uint32 dh = m_Dib.GetHeight();

	iRect src_rect(m_IsEnabled?dw*(m_CurrentState+1):0,0,dw,dh);
	iRect dst_rect=GetElementRect();

	iClipper::iClipRectRect(dst_rect,ddc.GetSize(),src_rect,iRect(src_rect));

	m_Dib.CopyRectToDibXY(&ddc.m_Dib,src_rect,dst_rect.point(),BLEND_ALPHABLEND);
}

HCURSOR iNWSwitchButton::GetCursor(const iPoint &pos) const
{
	return LoadCursor(NULL,IDC_HAND);
}

void iNWSwitchButton::SetSwitchState(uint32 nstate)
{
	if (nstate > (m_StateCnt - 1)) nstate = 0;
	if (m_CurrentState == nstate) return;
	m_CurrentState = nstate;
	GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_SWITCH_CHANGED, m_CurrentState,NULL));
	ForceRepaint();
}

uint32 iNWSwitchButton::GetSwitchState(void) const
{
	return m_CurrentState;
}
