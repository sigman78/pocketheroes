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

bool iNWSimpleButton::CreateNWSimpleButton(iNWElementContainerI *parent, const iRect &rect, const iDib &dib)
{
	// copy bitmap
	uint32 dw=dib.GetWidth();
	uint32 dh= dib.GetHeight();

	if (dw != rect.w || dh != rect.h) return false;

	if (!m_Dib.Create(&dib)) return false;

	return CreateNWElement(parent,rect);
}

void iNWSimpleButton::OnMouseClick(const iPoint &pos)
{
	iNWControl::OnMouseClick(pos);
	GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_BUTTON_CLICK,0,NULL));
}

void iNWSimpleButton::DrawElement(iMemoryDC &ddc) const
{
	m_Dib.CopyToDibXY(&ddc.m_Dib,GetElementRect(),BLEND_ALPHABLEND);
}

HCURSOR iNWSimpleButton::GetCursor(const iPoint &pos) const
{
	return LoadCursor(NULL,IDC_HAND);
}
