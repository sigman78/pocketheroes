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
#include "AutoBattle.h"
#include "BattleAI.h"
#include "Dlg_BattleResult.h"

iAutoBattle::iAutoBattle(bool bShowResult)
: iBattleWrapper(true), m_bShowResult(bShowResult)
{
}

void iAutoBattle::OnBeginBattle()
{
}

void iAutoBattle::OnStart()
{
	iBattleResult br;
	while (1) {
		bool bCasted = ProcessSpellCastAI(m_engine);

		if (!CheckBattleStateAI(m_engine, br)) {
			break;
		}

		if (!bCasted && !ProcessBattleStepAI(m_engine)) continue;
		while (m_engine.ActionCount()) {
			iBattleAct* pAct = m_engine.StepAction();
			check(pAct);
			delete pAct;
		}
		if (m_engine.CheckBattleState() != BR_NA) {
			br.m_result = m_engine.CheckBattleState();
			br.m_defCause = DC_DEFEAT;
			break;
		}
		m_engine.NextGroup();
	}
	
	m_engine.PrepareBattleResult(br);
	if (m_bShowResult) {
		iDlg_BattleResult brd(&gApp.ViewMgr(), m_engine, br);
		brd.DoModal();
	}

	EndBattle(br);
}

void iAutoBattle::OnEndBattle()
{
	/*
	int a = 0;
	const iArmy& aArmy = m_engine.GetBattleInfo().m_pAssaulter->Army();
	for (uint32 aidx=0; aidx<m_engine.AArmy().Count(); ++aidx) {
	}

	const iArmy& dArmy = m_engine.GetBattleInfo().m_pDefender->Army();
	for (uint32 didx=0; didx<m_engine.DArmy().Count(); ++didx) {
	}
	*/
}

void iAutoBattle::AddLogEvent(const iStringT& msg) 
{
}

void iAutoBattle::AddCellEvent(const iStringT& msg, const iPoint& pos)
{
}

