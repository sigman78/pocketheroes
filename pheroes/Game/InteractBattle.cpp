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

#ifdef _DEEP_DEBUG_
#	define new DEBUG_NEW
#endif 


#include "InteractBattle.h"
#include "BattleView.h"

iInteractBattle::iInteractBattle()
: iBattleWrapper(false), m_pBattleView(NULL)
{
}

void iInteractBattle::OnBeginBattle()
{
	gGame.ShowView(iChildGameView::CV_BATTLE);
	m_pBattleView = (iBattleView*)gGame.View(iChildGameView::CV_BATTLE);
	check(m_pBattleView);
	iPoint dpos = m_engine.GetBattleInfo().Defender()->Pos();
	iCell c = gGame.Map().GetAt(dpos.x,dpos.y);
	sint32 upSurf = iMAX(iMAX(iMAX(c.SurfNode(0),c.SurfNode(1)),c.SurfNode(2)),c.SurfNode(3));
	m_surfType = (SURF_TYPE)upSurf;
	m_pBattleView->BeginBattle(this, m_surfType);
}

void iInteractBattle::OnStart()
{
	m_pBattleView->Start();
}

void iInteractBattle::OnEndBattle()
{
	// Hide BattleView
	gGame.HideView(iChildGameView::CV_BATTLE);
	m_pBattleView = NULL;
}

void iInteractBattle::AddLogEvent(const iStringT& msg)
{
	if (m_pBattleView) m_pBattleView->AddLogEvent(msg);
}

void iInteractBattle::AddCellEvent(const iStringT& msg, const iPoint& pos)
{
	if (m_pBattleView) m_pBattleView->AddCellEvent(msg, pos);
}

