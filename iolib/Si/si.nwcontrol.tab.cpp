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
#include "si.nwcontrol.tab.h"

iNWTabControl::iNWTabControl()
: m_CurrentState(0)
, m_StateCnt(0)
, m_TrackId(-1)
, m_pttipDisp(NULL)
, m_ttipId(-1)
{
}

bool iNWTabControl::CreateNWTabControl(iNWElementContainerI *parent, const iRect &rect, const iDib &dib, uint32 state_cnt, Orientation ornt,bool ex_mode)
{
	m_ExMode = ex_mode;
	m_StateCnt = state_cnt;
	m_Orientation = ornt;

	uint32 dw;
	uint32 dh;
 
	if (m_Orientation == Horizontal)
	{
		dw = dib.GetWidth();

		if (m_ExMode) dh = dib.GetHeight() / 3;
		else dh = dib.GetHeight() / 2;

		m_ElementWidth = dib.GetWidth()/m_StateCnt;
		m_ElementHeight = dh;
	}
	else if (m_Orientation == Vertical)
	{
		if (m_ExMode) dw = dib.GetWidth() / 3;
		else dw = dib.GetWidth() / 2;

		dh = dib.GetHeight();
		m_ElementWidth =  dw; 
		m_ElementHeight = dib.GetHeight()/m_StateCnt;
	}
	else return false;

	if ( state_cnt == 0 || dw != rect.w || dh != rect.h) return false;

	m_Dib.Create(&dib);

	return CreateNWElement(parent,rect);
}

void iNWTabControl::SetTabState(uint32 nstate)
{
	if (nstate > (m_StateCnt - 1)) nstate = m_StateCnt - 1;
	if (m_CurrentState == nstate) return;
	m_CurrentState = nstate;
	GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_TAB_CHANGED, m_CurrentState,NULL));
	ForceRepaint();
}

uint32 iNWTabControl::GetTabState(void) const
{
	return m_CurrentState;
}

void iNWTabControl::OnLButtonDown(const iPoint &pos, uint32 vkey)
{
	if (m_ExMode)
	{
		m_TrackId = GetIdByPos(pos);
		ForceRepaint();
	}

	iNWControl::OnLButtonDown(pos,vkey);
}

void iNWTabControl::OnLButtonUp(const iPoint &pos, uint32 vkey)
{
	if (m_ExMode)
	{
		m_TrackId = -1;
		ForceRepaint();
	}

	iNWControl::OnLButtonUp(pos,vkey);
}

uint32 iNWTabControl::GetIdByPos(const iPoint &pos)
{
	if (pos.x < 0 || pos.y < 0) return -1;

	if (m_Orientation == Horizontal)
	{
		return pos.x / m_ElementWidth;
	}
	else if (m_Orientation == Vertical)
	{
		return pos.y / m_ElementHeight;
	}

	return -1;
}

void iNWTabControl::OnMouseClick(const iPoint &pos)
{
	uint32 nid = GetIdByPos(pos);
	if (nid != -1) SetTabState(nid);
	iNWControl::OnMouseClick(pos);
}

void iNWTabControl::DrawElement(iMemoryDC &ddc) const
{
	if (m_Orientation == Horizontal) {
		for (uint32 xx=0; xx<m_StateCnt; ++xx) {
			if (m_ExMode && m_TrackId == xx) m_Dib.CopyRectToDibXY(&ddc.m_Dib,iRect(m_ElementWidth*xx,m_ElementHeight*2,m_ElementWidth,m_ElementHeight),iPoint(GetElementRect().x+m_ElementWidth*xx,GetElementRect().y),BLEND_ALPHABLEND);
			else if (m_CurrentState == xx) m_Dib.CopyRectToDibXY(&ddc.m_Dib,iRect(m_ElementWidth*xx,m_ElementHeight,m_ElementWidth,m_ElementHeight),iPoint(GetElementRect().x+m_ElementWidth*xx,GetElementRect().y),BLEND_ALPHABLEND);
			else m_Dib.CopyRectToDibXY(&ddc.m_Dib,iRect(m_ElementWidth*xx,0,m_ElementWidth,m_ElementHeight),iPoint(GetElementRect().x+m_ElementWidth*xx,GetElementRect().y),BLEND_ALPHABLEND);
		}
	} else if (m_Orientation == Vertical) {
		for (uint32 yy=0; yy<m_StateCnt; ++yy) {
			if (m_ExMode && m_TrackId == yy) m_Dib.CopyRectToDibXY(&ddc.m_Dib,iRect(m_ElementWidth*2,m_ElementHeight*yy,m_ElementWidth,m_ElementHeight),iPoint(GetElementRect().x,GetElementRect().y+m_ElementHeight*yy),BLEND_ALPHABLEND);
			else if (m_CurrentState == yy) m_Dib.CopyRectToDibXY(&ddc.m_Dib,iRect(m_ElementWidth,m_ElementHeight*yy,m_ElementWidth,m_ElementHeight),iPoint(GetElementRect().x,GetElementRect().y+m_ElementHeight*yy),BLEND_ALPHABLEND);
			else m_Dib.CopyRectToDibXY(&ddc.m_Dib,iRect(0,m_ElementHeight*yy,m_ElementWidth,m_ElementHeight),iPoint(GetElementRect().x,GetElementRect().y+m_ElementHeight*yy),BLEND_ALPHABLEND);
		}
	}
}

HCURSOR iNWTabControl::GetCursor(const iPoint &pos) const
{
	return LoadCursor(NULL,IDC_HAND);
}

void iNWTabControl::OnMouseMove(const iPoint &pos)
{
	if (m_pttipDisp) {
		uint32 nId = GetIdByPos(pos);
		if (nId != m_ttipId){
			m_ttipId = nId;
			if (m_ttipId != -1){
				m_pttipDisp->DispToolTip(m_ttipStrings[m_ttipId]);
			}
		}
	}
	iNWControl::OnMouseMove(pos);
}

void iNWTabControl::OnMouseLeave()
{
	if (m_pttipDisp) {
		m_ttipId = -1;
		m_pttipDisp->DispToolTip(_T(""));
	}
	iNWControl::OnMouseLeave();
}

void iNWTabControl::SetToolTipInfo(iToolTipDispatcherI* pDisp, const iStringList& ttip)
{
	m_pttipDisp = pDisp;
	m_ttipStrings = ttip;
	check(ttip.GetSize() == m_StateCnt);
}
