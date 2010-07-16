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
#include "MapGen.h"

//////////////////////////////////////////////////////////////////////////
inline bool IsWaterNode(iMapHandler& map, sint32 x, sint32 y) 
{ return map.IsValidNode(x,y) && map.m_Nodes.At(x,y).surf == STYPE_WATER; }

inline bool IsLandNode(iMapHandler& map, sint32 x, sint32 y) 
{ return map.IsValidNode(x,y) && map.m_Nodes.At(x,y).surf != STYPE_WATER; }

//////////////////////////////////////////////////////////////////////////
struct iCellEntry
{
	iCellEntry(sint32 _x, sint32 _y)
	: x(_x), y(_y) {}

	sint32	x,y,px,py;
	sint32	p;
};
typedef iSimpleArray<iCellEntry> iCellList;

//////////////////////////////////////////////////////////////////////////
inline bool AddCellEntry(iMapHandler& map, iCellList& queue, sint32 x, sint32 y)
{
	if (IsWaterNode(map,x,y)) {
		queue.Add(iCellEntry(x,y));
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
void DistPriorities(iMapHandler& map, iCellList& queue, sint32 px, sint32 py)
{
	for (uint32 xx=0; xx<queue.GetSize(); ++xx) {
		queue[xx].p = 0;
		if (IsLandNode(map, queue[xx].x+1, queue[xx].y)) ++queue[xx].p;
		if (IsLandNode(map, queue[xx].x-1, queue[xx].y)) ++queue[xx].p;
		if (IsLandNode(map, queue[xx].x, queue[xx].y+1)) ++queue[xx].p;
		if (IsLandNode(map, queue[xx].x, queue[xx].y-1)) ++queue[xx].p;

		double angle = (queue[xx].x==px && queue[xx].y==py)?0:atan2f(queue[xx].x-px, queue[xx].y-py);
		queue[xx].p += (sint32)((1 + sin(angle*8)) * 5);

		queue[xx].p -= abs(queue[xx].x-px) + abs(queue[xx].y-py);
	}

	sint32 n = (sint32)queue.GetSize();
	for( sint32 i = 0; i < n; i++ ) {
		for( sint32 j = i + 1; j < n; j++ ) {
			if ( queue[j].p > queue[i].p ) iSwap(queue[i], queue[j]);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
inline bool PopEntry(iCellList& queue, sint32 &x, sint32 &y)
{
	if (!queue.GetSize()) return false;
	uint32 rid = rand()%(iMAX<sint32>(1,queue.GetSize()/3));
	x = queue[rid].x;
	y = queue[rid].y;
	queue.RemoveAt(rid);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool iMapGenerator::Generate()
{
	srand(GetTickCount());
	sint32 sw = m_Map.GetWidth()+1;
	sint32 sh = m_Map.GetHeight()+1;

	sint32 reqcnt = sw*sh/2;
	sint32 cnt = 0;

	sint32 px = sw/2;
	sint32 py = sh/2;
	iCellList queue;
	queue.Add(iCellEntry(px,py));
	sint32 x, y;

	while (cnt<reqcnt) {
		if (!PopEntry(queue,x,y)) {
			check(0);
		}
		
		m_Map.SetNodeSurf(x,y,STYPE_DIRT);
		cnt++;

		AddCellEntry(m_Map,queue,x+1,y);
		AddCellEntry(m_Map,queue,x-1,y);
		AddCellEntry(m_Map,queue,x,y+1);
		AddCellEntry(m_Map,queue,x,y-1);
		
		DistPriorities(m_Map,queue,px,py);
	}

	return true;
}
