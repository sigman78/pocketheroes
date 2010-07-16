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
#include "si.nwcontrol.base.ttip.h"

bool iNWControlTT::CreateNWElementTT(iNWElementContainerI *parent, const iRect &rect, iToolTipDispatcherI* pDisp, const iStringT &ttip)
{
	m_pttipDisp = pDisp;
	m_ttipText = ttip;
	return CreateNWElement(parent, rect);
}

void iNWControlTT::SetToolTipText(const iStringT& ttip)
{
	m_ttipText = ttip;
	ForceRepaint();
}

const iStringT& iNWControlTT::GetToolTipText() const
{
	return m_ttipText;
}

void iNWControlTT::SetToolTipDisp(iToolTipDispatcherI* pDisp)
{
	m_pttipDisp = pDisp;
}

iToolTipDispatcherI* iNWControlTT::GetToolTipDisp() const
{
	return m_pttipDisp;
}

//
void iNWControlTT::OnMouseEnter()
{
	if (m_pttipDisp) m_pttipDisp->DispToolTip(m_ttipText);
	iNWControl::OnMouseEnter();
}

void iNWControlTT::OnMouseLeave()
{
	if (m_pttipDisp) m_pttipDisp->DispToolTip(_T(""));
	iNWControl::OnMouseLeave();
}
