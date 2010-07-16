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
#include "si.nwcontrol.label.h"

bool iNWLabel::CreateNWLabel(iNWElementContainerI *parent, const iRect &rect, const iStringT &text)
{
	m_BackColor = cColorTransparent;
	m_Dib.Create(rect);
	bool res = CreateNWElement(parent,rect);
	if (!res) return false;
	
	SetLabelText(text);
	return res;
}

void iNWLabel::DrawElement(iMemoryDC &ddc) const
{
	m_Dib.CopyToDibXY(&ddc.m_Dib,GetElementRect(),BLEND_ALPHABLEND);
}

HCURSOR iNWLabel::GetCursor(const iPoint &pos) const
{
	return LoadCursor(NULL,IDC_ARROW);
}

void iNWLabel::SetLabelFont(iFont &fnt)
{
	m_DibText.SetFont(fnt);
	DrawLabel();
}

void iNWLabel::SetLabelTextColor(const iColor &clr)
{
	m_DibText.SetTextColor(clr);
	DrawLabel();
}

void iNWLabel::SetLabelBackColor(const iColor &clr)
{
	m_BackColor = clr;
	DrawLabel();
}

void iNWLabel::SetLabelText(const iStringT &ntext)
{
	m_Text = ntext;
	DrawLabel();
}

void iNWLabel::SetLabelAlignment(iAlignment algn)
{
	m_DibText.SetTextAlignment(algn);
	DrawLabel();
}

void iNWLabel::SetLabelProps(iFont &fnt, const iColor &txt_clr, const iColor &bck_clr, iAlignment algn)
{
	m_DibText.SetFont(fnt);
	m_DibText.SetTextColor(txt_clr);
	m_BackColor = bck_clr;
	m_DibText.SetTextAlignment(algn);
	DrawLabel();
}

void iNWLabel::DrawLabel()
{
	m_Dib.Fill(m_BackColor,BLEND_SRCCOPY);
	m_DibText.TextOut(m_Dib,m_Text,m_Dib.GetSize(),iPoint(0,0));
	ForceRepaint();
}
