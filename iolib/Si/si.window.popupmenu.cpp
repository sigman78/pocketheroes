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
#include "si.std.h"
#include "si.window.popup.h"
#include "si.nwcontrol.sbutton.h"
#include "si.window.popupmenu.h"

iPopupMenu::~iPopupMenu()
{
	DestroyPopupMenuContent();
}

void iPopupMenu::InsertMenuItem(const iDib &dib, const iStringT &txt, bool enb)
{
	check((dib.GetSize()==m_ElementSize));

	iNWSimpleButton *btn = new iNWSimpleButton();
	btn->CreateNWSimpleButton(this,m_ElementSize,dib);
	RegisterElement(btn);

	iMenuItem *mitem = new iMenuItem(btn,txt,enb);
	m_Items.Add(mitem);
}

bool iPopupMenu::OnNWMessage( iNWMessage& msg )
{
	if (msg.m_Message == NWMSG_BUTTON_CLICK)
	{
		for (uint32 xx=0; xx<m_Items.GetSize(); ++xx)
		{
			if (msg.m_Element == m_Items[xx]->m_Button)
			{
				m_PopupNotifySubscriber->OnPopupNotify(PWNM_STATUSCHANGED,xx);
				break;
			}
		}
	}
	return true;
}

void iPopupMenu::MsgActivate(bool fActive, bool fMinimized)
{
	iPopupWindow::MsgActivate(fActive,fMinimized);
	if (!fActive) 
	{
		m_PopupNotifySubscriber->OnPopupNotify(PWNM_DEACTIVATED,0);
	}
}

void iPopupMenu::HidePopupMenu()
{
	ShowWindow(SW_HIDE);
}

void iPopupMenu::ShowPopupMenu(const iPoint &pos, iAlignment algn)
{
	if (m_Items.GetSize() == 0) return;

	iRect nrect(0,0,0,0);

	for (uint32 xx=0; xx< m_Items.GetSize(); ++xx)
	{
		m_Items[xx]->m_Button->SetElementPos(iPoint(nrect.w,0));
		nrect.w += m_Items[xx]->m_Button->GetElementRect().w;
		nrect.h = iMAX(nrect.h,m_Items[xx]->m_Button->GetElementRect().h);
	}

	nrect.y = pos.y-nrect.h;
	nrect.x = pos.x - nrect.w/2;

	iSize ssize = iWindowManager::GetScreenSize();

	if (nrect.x<0) nrect.x=0;
	else if (nrect.x2() > (sint32)ssize.w) nrect.x = ssize.w - nrect.w;

	if (nrect.y<0) nrect.y=0;
	else if (nrect.y2() > (sint32)ssize.h) nrect.y = ssize.h - nrect.h;

	SetWndRect(nrect);

	ShowWindow(SW_SHOW);
	SetActiveWindow();
}


void iPopupMenu::DrawBackground(iDib &odib, const iRect &rect)
{
	// Erase rect
	odib.Fill(iColor(0,0,0,0),BLEND_SRCCOPY);
}

bool iPopupMenu::InitPopupMenu(iPopupNotifySubscriber *pwns, const iStringT &capt, const iSize &elsiz)
{
	m_Caption = capt;
	m_ElementSize = elsiz;

	if (!CreatePopupWindow(pwns,iRect(0,0,elsiz.w,elsiz.h))) return false;
	AddMessageSubscriber(this);

	return true;
}

const iMenuItem* iPopupMenu::GetItem(uint32 item) const
{
	return m_Items[item];
}
