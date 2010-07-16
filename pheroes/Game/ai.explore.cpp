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

#include "ai.explore.h"

iExplorePoint::iExplorePoint(iHero& hero)
: m_hero(hero), m_pos(hero.Pos()), m_fogMap(hero.Owner()->FogMap()) 
{  }

/*
sint32 iExplorePoint::FindExplorePoint(sint32 radius, iPoint& target)
{
	m_hero.Owner()->UpdatePassMap();
	// now we are ready for search
	// preallocate opened points array

	OutputDebugString(_T("Not Found!\n"));
	return -1;
}
*/

sint32 iExplorePoint::FindExplorePoint(sint32 radius, iPoint& target)
{
	OutputDebugString(_T("\tScanning map for passable hidden cell: "));
	sint32 r=1;
	m_hero.Owner()->UpdatePassMap();

	iShPointList scanRes;
	while(r<radius){
		if (ScanRadius(r, scanRes, m_hero.Owner()->PassMap())) {
			iPath path(m_hero.Pos());
			for (uint32 resId = 0; resId < scanRes.GetSize(); ++resId) {
				if (m_hero.MakePath(path, SHPOINT_X(scanRes[resId]),SHPOINT_Y(scanRes[resId]))) {
					target.x = SHPOINT_X(scanRes[resId]);
					target.y = SHPOINT_Y(scanRes[resId]);
					return path.PathCost();
				}
			}
			scanRes.RemoveAll();
		}
		++r;
	}
	iPathFinder::Purge();
	return -1;
}

void iExplorePoint::EnumAllExplorePoints(iShPointList& targets)
{
	check(0); 			// not optimal, but not used anyway
	int* p = 0; *p = 0; // to be sure foe do not pass ^^;
	m_hero.Owner()->UpdatePassMap();
	iPlayer::iPassMap pm = m_hero.Owner()->PassMap();
	for (sint32 yy=0; yy<pm.GetHeight(); ++yy ) {
		for (sint32 xx=0; xx<pm.GetWidth(); ++xx ) {
			if (pm.GetAt(xx,yy) != -1 && m_fogMap.IsVisBound(xx,yy)) targets.Add(MAKE_SHPOINT(xx,yy));
		}
	}
}

bool iExplorePoint::ScanRadius(sint32 radius, iShPointList& scanRes, iPlayer::iPassMap& passMap) const
{
	sint32 ix1 = m_pos.x-radius;
	sint32 iy1 = m_pos.y-radius;
	sint32 ix2 = m_pos.x+radius;
	sint32 iy2 = m_pos.y+radius;

	sint32 px1 = iMAX<sint32>(0,ix1);
	sint32 py1 = iMAX<sint32>(0,iy1);
	sint32 px2 = iMIN<sint32>(gGame.Map().GetWidth()-1,ix2);
	sint32 py2 = iMIN<sint32>(gGame.Map().GetHeight()-1,iy2);
	
	if (ix1 == px1) {
		for (sint32 yy=py1; yy<=py2; ++yy){
			if (passMap.GetAt(px1,yy) != -1 && m_fogMap.IsVisBound(px1,yy)) scanRes.Add(MAKE_SHPOINT(px1,yy));
		}
		px1++;
	}

	if (iy1 == py1) {
		for (sint32 xx=px1; xx<=px2; ++xx){
			if (passMap.GetAt(xx,py1) != -1 && m_fogMap.IsVisBound(xx,py1)) scanRes.Add(MAKE_SHPOINT(xx,py1));
		}
		py1++;
	}

	if (ix2 == px2) {
		for (sint32 yy=py1; yy<=py2; ++yy){
			if (passMap.GetAt(px2,yy) != -1 && m_fogMap.IsVisBound(px2,yy)) scanRes.Add(MAKE_SHPOINT(px2,yy));
		}
		px2--;
	}
	
	if (iy2 == py2) {
		for (sint32 xx=px1; xx<=px2; ++xx){
			if (passMap.GetAt(xx,py2) != -1 && m_fogMap.IsVisBound(xx,py2)) scanRes.Add(MAKE_SHPOINT(xx,py2));
		}
		py2++;
	}

	return scanRes.GetSize()>0;
}

