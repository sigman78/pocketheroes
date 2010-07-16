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

/*
 *	CastleFort
 */
const sint32 iCastleFort::ElCells[iCastleFort::TypeCount][2] = {
	{9,0}, {9,1}, {8,2}, {8,3}, {7,4}, {7,5}, {13,5}, {7,6}, {8,7}, {8,8}, {9,9}, {9,10}
};

const sint32 iCastleFort::MoatCells[11][2] = {
	{8,0}, {8,1}, {7,2}, {7,3}, {6,4}, {7,5}, {6,6}, {7,7}, {7,8}, {8,9}, {8,10}
};

// [Skill None-Master][min damage, max damage, shoots]
const sint32 CATAPULT_PROPS[5][3] = {
	{ 50, 150, 1},
	{ 100, 200, 1},
	{ 100, 200, 2},
	{ 200, 250, 2}
};

iCastleFort::iCastleFort(iCastle* pCastle)
{
	sint32 hits = 100;
	iCtlCnst* pFortification  = pCastle->FindCnst(CTLCNST_FORTIFICATION);
	if (pFortification && pFortification->IsBuilt()) hits *= 2;

	iCtlCnst* pMoat  = pCastle->FindCnst(CTLCNST_MOAT);
	m_bHasMoat = (pMoat && pMoat->IsBuilt());

	iCtlCnst* pRTurret  = pCastle->FindCnst(CTLCNST_RTURRET);
	m_pElements[0] = new iElement_Tower(RightTower3, pRTurret && pRTurret->IsBuilt(), hits);
	m_pElements[1] = new iElement_Wall(RightWall2, hits);
	m_pElements[2] = new iElement_Tower(RightTower2, false, hits);
	m_pElements[3] = new iElement_Wall(RightWall1, hits);
	m_pElements[4] = new iElement_Tower(RightTower1, false, hits);
	m_pElements[5] = new iElement_Bridge(hits);
	iCtlCnst* pMTurret  = pCastle->FindCnst(CTLCNST_MTURRET);
	m_pElements[6] = new iElement_Tower(MainTower, pMTurret && pMTurret->IsBuilt(), hits);
	m_pElements[7] = new iElement_Tower(LeftTower1, false, hits);
	m_pElements[8] = new iElement_Wall(LeftWall1, hits);
	m_pElements[9] = new iElement_Tower(LeftTower2, false, hits);
	m_pElements[10] = new iElement_Wall(LeftWall2, hits);
	iCtlCnst* pLTurret  = pCastle->FindCnst(CTLCNST_LTURRET);
	m_pElements[11] = new iElement_Tower(LeftTower3, pLTurret && pLTurret->IsBuilt(), hits);
}

iCastleFort::~iCastleFort()
{
	for (uint32 xx=0; xx<TypeCount; ++xx) {
		delete m_pElements[xx];
	}
}

void iCastleFort::ProcessPassMap(iBattleMap& passMap, bool bOwn) const
{
	// Moat
	if (m_bHasMoat) {
		for (uint32 yy=0; yy<11; ++yy) passMap.GetAt(MoatCells[yy][0],MoatCells[yy][1]) = CT_MOAT;
	}
	
	// Fort Elements
	for (uint32 xx=0; xx<TypeCount; ++xx) m_pElements[xx]->ProcessPassMap(passMap, bOwn);
}

iCastleFort::ElementType iCastleFort::GetTarget(ElementType target, sint32 prob) const
{
	if (target == InvalidElement || (gGame.Map().Rand(100)) > prob) {
		// have intact walls ?
		iSimpleArray<ElementType> elements;
		if (m_pElements[RightWall2]->state != Destroyed) elements.Add(RightWall2);
		if (m_pElements[RightWall1]->state != Destroyed) elements.Add(RightWall1);
		if (m_pElements[LeftWall1]->state != Destroyed) elements.Add(LeftWall1);
		if (m_pElements[LeftWall2]->state != Destroyed) elements.Add(LeftWall2);
		if (elements.GetSize()) {
			return elements[gGame.Map().Rand(elements.GetSize())];
		}
		// bridge
		if (m_pElements[Bridge]->state != Destroyed) return Bridge;
		// turrets
		if (((iElement_Tower*)m_pElements[RightTower3])->bTurret && m_pElements[RightTower3]->state != Destroyed) elements.Add(RightTower3);
		if (((iElement_Tower*)m_pElements[LeftTower3])->bTurret && m_pElements[LeftTower3]->state != Destroyed) elements.Add(LeftTower3);
		if (((iElement_Tower*)m_pElements[MainTower])->bTurret && m_pElements[MainTower]->state != Destroyed) elements.Add(MainTower);
		if (elements.GetSize()) {
			return elements[gGame.Map().Rand(elements.GetSize())];
		}
		target = InvalidElement;
	}

	check(target == InvalidElement || m_pElements[target]->state != Destroyed);
	return target;
}

bool iCastleFort::HasWalls() const 
{
	// Walls
	if (m_pElements[RightWall2]->state != Destroyed) return true;
	if (m_pElements[RightWall1]->state != Destroyed) return true;
	if (m_pElements[LeftWall1]->state != Destroyed) return true;
	if (m_pElements[LeftWall2]->state != Destroyed) return true;
	return false;
}

bool iCastleFort::HasTarget() const 
{ 
	// Walls
	if (HasWalls()) return true;

	// Bridge
	if (m_pElements[Bridge]->state != Destroyed) return true;

	// Turrets
	if (((iElement_Tower*)m_pElements[RightTower3])->bTurret && m_pElements[RightTower3]->state != Destroyed) return true;
	if (((iElement_Tower*)m_pElements[LeftTower3])->bTurret && m_pElements[LeftTower3]->state != Destroyed) return true;
	if (((iElement_Tower*)m_pElements[MainTower])->bTurret && m_pElements[MainTower]->state != Destroyed) return true;

	return false;
}

bool iCastleFort::HasTurret() const 
{ 
	if (((iElement_Tower*)m_pElements[RightTower3])->bTurret && m_pElements[RightTower3]->state != Destroyed) return true;
	if (((iElement_Tower*)m_pElements[LeftTower3])->bTurret && m_pElements[LeftTower3]->state != Destroyed) return true;
	if (((iElement_Tower*)m_pElements[MainTower])->bTurret && m_pElements[MainTower]->state != Destroyed) return true;
	return false;
}

