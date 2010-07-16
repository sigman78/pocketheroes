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

#include "stdafx.h"
#include ".\ibattlegroupbtn.h"

iBattleGroupBtn::iBattleGroupBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iPoint& pos, uint32 uid, iBattleGroup *grp, iStringT text):
	iBaseCtrl(pViewMgr,pCmdHandler,iRect(pos.x,pos.y,30, 30), GENERIC_VIEWPORT,uid, Visible|Enabled), m_group(grp), m_text(text), m_selected(false)
{

}


void iBattleGroupBtn::Deselect() 
{
	m_selected = false;
	Invalidate();
}

void iBattleGroupBtn::OnMouseClick(const iPoint& pos)
{
	this->GetParent()->OnMouseClick(pos);
	m_selected = true;
	Invalidate();
}

void iBattleGroupBtn::OnCompose()
{
	iRect rc = GetScrRect();	
	ComposeCreatureButtonWithText(gApp.Surface(), rc, m_group->Type(), m_text, VL_EXPERT, m_selected, false,false);
}
