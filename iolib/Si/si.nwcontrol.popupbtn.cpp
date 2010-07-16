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
#include "si.nwcontrol.sbutton.h"
#include "si.std.h"
#include "si.window.popup.h"
#include "si.window.popupmenu.h"
#include "si.nwcontrol.popupbtn.h"

void iNWPopupButton::OnPopupNotify(uint32 uMsg, sint32 lParam)
{
	iFLOG((_T("PopupNotify: %d - %d"),uMsg,lParam));
	if (uMsg == PWNM_DEACTIVATED)
	{
		m_PopupMenu.HidePopupMenu();
	}
	else if (uMsg == PWNM_STATUSCHANGED)
	{
		SetTabState(lParam);
		m_PopupMenu.HidePopupMenu();
	}
}

bool iNWPopupButton::CreateNWPopupButton(iNWElementContainerI *parent, const iRect &rect, const iDib &dib, uint32 state_cnt)
{
	m_CurSel = 0;

	check(state_cnt != 0);
	check(dib.GetWidth() != 0);
	check(dib.GetHeight() != 0);

	check((dib.GetWidth()%state_cnt)==0);

	iSize esiz(dib.GetWidth()/state_cnt,dib.GetHeight());
	check(esiz == rect.size());

	m_PopupMenu.InitPopupMenu(this,iStringT(_T("PopupMenu")),esiz);

	for (uint32 xx=0; xx< state_cnt; ++xx)
	{
		iDib tdib(&dib,iRect(xx*esiz.w,0,esiz.w,esiz.h));
		m_PopupMenu.InsertMenuItem(tdib);
	}

	return CreateNWElement(parent,rect);
}

void iNWPopupButton::OnMouseClick(const iPoint &pos)
{
	iNWControl::OnMouseClick(pos);

	iPoint pnt = m_Parent->NWClientToScreen(iPoint(GetElementRect().x+GetElementRect().w/2,GetElementRect().y - 10));
	// Check for enough space to show popup at top
	iAlignment algn=Align_Bottom;
	if ((sint32)m_PopupMenu.GetItem(m_CurSel)->m_Button->GetButtonDib()->GetHeight() <= m_Parent->NWClientToScreen(GetElementRect().point()).y)
	{
		algn = Align_Top;
	}

	if (pnt.y < 100 ) pnt.y += GetElementRect().h*2 + 20;


	m_PopupMenu.ShowPopupMenu(pnt,algn);
}

void iNWPopupButton::DrawElement(iMemoryDC &ddc) const
{
	m_PopupMenu.GetItem(m_CurSel)->m_Button->GetButtonDib()->CopyToDibXY(&ddc.m_Dib,GetElementRect(),BLEND_ALPHABLEND);
}

HCURSOR iNWPopupButton::GetCursor(const iPoint &pos) const
{
	return LoadCursor(NULL,IDC_HAND);
}

void iNWPopupButton::SetTabState(uint32 nstate)
{
	if (m_CurSel == nstate) return;
	m_CurSel = nstate;
	GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_TAB_CHANGED, m_CurSel,NULL));
	ForceRepaint();
}

uint32 iNWPopupButton::GetTabState(void) const
{
	return m_CurSel;
}

void iNWPopupButton::DestroyElement(void)
{
	m_PopupMenu.DestroyWindow();
	m_PopupMenu.m_hWnd = NULL;
	iNWControl::DestroyElement();
}
