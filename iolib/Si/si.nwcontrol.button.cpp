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
#include "si.nwcontrol.button.h"

iNWButton::iNWButton()
: m_BtnState(BtnUnpressed)
{
}

bool iNWButton::CreateNWButton(iNWElementContainerI *parent, const iRect &rect, const iDib &dib, iToolTipDispatcherI* pDisp, const iStringT &ttip)
{
	// copy bitmaps
	uint32 dw=dib.GetWidth()/4;
	uint32 dh= dib.GetHeight();

	if (dib.GetWidth()%4 != 0 || dw != rect.w || dh != rect.h) return false;

	for (uint32 xx=0; xx<4; ++xx) {
		if (!m_Dibs[xx].Create(&dib, iRect(dw*xx,0,dw,dh))) return false;
	}

	return CreateNWElementTT(parent,rect,pDisp,ttip);
}

void iNWButton::OnMouseEnter(void)
{
	m_BtnState = BtnFocused;
	ForceRepaint();
	iNWControlTT::OnMouseEnter();
}

void iNWButton::OnMouseLeave(void)
{
	m_BtnState = BtnUnpressed;
	ForceRepaint();
	iNWControlTT::OnMouseLeave();
}

void iNWButton::OnMouseClick(const iPoint &pos)
{
	iNWControlTT::OnMouseClick(pos);
	GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_BUTTON_CLICK,0,NULL));
}

void iNWButton::OnLButtonDown(const iPoint &pos, uint32 vkey)
{
	ForceRepaint();
	iNWControlTT::OnLButtonDown(pos,vkey);
}

void iNWButton::OnLButtonUp(const iPoint &pos, uint32 vkey)
{
	ForceRepaint();
	iNWControlTT::OnLButtonUp(pos,vkey);
}

void iNWButton::DrawElement(iMemoryDC &ddc) const
{
	if (!IsElementEnabled()) {
		m_Dibs[0].CopyToDibXY(&ddc.m_Dib,GetElementRect().point(),BLEND_ALPHABLEND);
	} else if (IsTrack()) {
		if (m_BtnState==BtnUnpressed) m_Dibs[3].CopyToDibXY(&ddc.m_Dib,GetElementRect().point(),BLEND_ALPHABLEND);
		else if (m_BtnState==BtnFocused) m_Dibs[2].CopyToDibXY(&ddc.m_Dib,GetElementRect().point(),BLEND_ALPHABLEND);
	} else {
		if (m_BtnState==BtnUnpressed) m_Dibs[3].CopyToDibXY(&ddc.m_Dib,GetElementRect().point(),BLEND_ALPHABLEND);
		else if (m_BtnState==BtnFocused) m_Dibs[1].CopyToDibXY(&ddc.m_Dib,GetElementRect().point(),BLEND_ALPHABLEND);
	}
}

HCURSOR iNWButton::GetCursor(const iPoint &pos) const
{
	return LoadCursor(NULL,IDC_HAND);
}
