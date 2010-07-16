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

#include "ai.scanner.h"

/*
 *	Danger map
 */
iDangerMap::iDangerMap(PLAYER_ID owner)
: m_owner(owner)
, m_dangerMap(gGame.Map().m_PassMap.GetWidth(),gGame.Map().m_PassMap.GetHeight())
{
	m_dangerMap.ZeroMem(0xFF);
}

void iDangerMap::UpdateDangerMap()
{
	iTimer timer;
	// Reset danger map
	m_dangerMap.ZeroMem(0xFF);

	// Prepare actors list
	m_enActors.RemoveAll();
	for (iGameWorld::iPLIt pit = gGame.Map().m_Players.First(); pit != gGame.Map().m_Players.End(); ++pit) {
		if ((*pit)->PlayerId() == m_owner) continue;
		for (iPlayer::iHLIt hit = (*pit)->HeroFirst(); hit != (*pit)->HeroEnd(); ++hit) {
			m_enActors.Add(*hit);
		}
	}

	// No enemy heroes, so no danger
	if (!m_enActors.GetSize()) return;

	// Make danger map
	for (uint32 actorId = 0; actorId < m_enActors.GetSize(); ++actorId) {
		// Prepare pass map
		iDistMapProcessor hdm(iSize(gGame.Map().m_PassMap.GetWidth(),gGame.Map().m_PassMap.GetHeight()));
		hdm.MakeDistMap(m_enActors[actorId]->Pos(), m_enActors[actorId]->Army().ArmyPower(), m_enActors[actorId]->MoveCostBonus(), m_enActors[actorId]->Owner()->PlayerId(), m_enActors[actorId]->TotalMoves());
		iDistMapProcessor::iMark* pMark = hdm.m_distMap.GetPtr();
		uint8* pDanger = m_dangerMap.GetPtr();
		sint32 count = m_dangerMap.GetWidth() * m_dangerMap.GetHeight();
		while (count--) {
			if (pMark->move != 0x3FFF || pMark->touch != 0x3FFF) {
				if ( *pDanger == 0xFF || m_enActors[actorId]->Army().ArmyPower() > m_enActors[*pDanger]->Army().ArmyPower()) {
					*pDanger = (uint8)actorId;
				}
			}
			pDanger++;
			pMark++;
		}
	}

	// Prepare danger enemy heroes list
	iPlayer* pOwner = gGame.Map().FindPlayer(m_owner);
	check(pOwner);
	for (iPlayer::iCtLIt cit = pOwner->CastleFirst(); cit != pOwner->CastleEnd(); ++cit) {
		uint8 eidx = m_dangerMap.GetAt((*cit)->Pos().x, (*cit)->Pos().y);
		if (eidx != 0xFF) {
			m_danActors.Add(m_enActors[eidx]);
		}
	}
}
