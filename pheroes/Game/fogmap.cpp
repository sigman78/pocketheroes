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
#include "SpanMap.h"

bool iFogMap::InitFogMap(uint32 w, uint32 h, bool bFogMap)
{
	Init(w+32,h+32);
	ZeroMem(0);
	m_bFogMap = bFogMap;
	return true;
}

void iFogMap::ResetFog()
{
	uint8 *pPtr = m_Buff;
	uint32 cnt = m_Width*m_Height;
	if (m_bFogMap) {
		while(cnt--){
			if ( *pPtr == 2) *pPtr = 1;
			pPtr++;
		}
	}
}

void iFogMap::DiscoverMap(sint32 x, sint32 y, sint32 radius, bool bHide)
{
	check (radius > 0 && radius < 256);
	iSpannedMap sm(iSpannedMap::Circle, radius);
	sint32 cnt = sm.SpanLinesCount();
	uint8 val = bHide?0:2;
	for (sint32 yy=0; yy<cnt; ++yy){
		const iSpannedMap::iSpan& sp = sm[yy];
		for (sint32 xx=sp.begin; xx<=sp.end; ++xx ){
			sint32 px = x+xx+16;
			sint32 py = y+16+sp.ypos;
			if (IsValidPos(px,py)) GetAt(px,py) = val;
			if (IsValidPos(px+1,py)) GetAt(px+1,py) = val;
			if (IsValidPos(px+1,py+1)) GetAt(px+1,py+1) = val;
			if (IsValidPos(px,py+1)) GetAt(px,py+1) = val;
		}
	}
}

bool iFogMap::GetFogCfg(const iPoint& pos, uint8& dcfg, uint8& fcfg) const
{
	dcfg = fcfg = 0;
	if (IsValidPos(pos.x+16,pos.y+16)) {
		if (GetAt(pos.x+16,pos.y+16) == 0) {
			dcfg |= (1<<0);
			fcfg |= (1<<0);
		} else if (m_bFogMap && GetAt(pos.x+16,pos.y+16) == 1) {
			fcfg |= (1<<0);
		}
	}

	if (IsValidPos(pos.x+1+16,pos.y+16)) {
		if (GetAt(pos.x+1+16,pos.y+16) == 0) {
			dcfg |= (1<<1);
			fcfg |= (1<<1);
		} else if (m_bFogMap && GetAt(pos.x+1+16,pos.y+16) == 1) {
			fcfg |= (1<<1);
		}
	}

	if (IsValidPos(pos.x+1+16,pos.y+1+16)) {
		if (GetAt(pos.x+1+16,pos.y+1+16) == 0) {
			dcfg |= (1<<2);
			fcfg |= (1<<2);
		} else if (m_bFogMap && GetAt(pos.x+1+16,pos.y+1+16) == 1) {
			fcfg |= (1<<2);
		}
	}

	if (IsValidPos(pos.x+16,pos.y+1+16)) {
		if (GetAt(pos.x+16,pos.y+1+16) == 0) {
			dcfg |= (1<<3);
			fcfg |= (1<<3);
		} else if (m_bFogMap && GetAt(pos.x+16,pos.y+1+16) == 1) {
			fcfg |= (1<<3);
		}
	}

	// 0 - totaly visible
	return ( !fcfg && !dcfg);
}

bool iFogMap::IsHidden(const iPoint& pos) const
{
	uint8 dcfg,fcfg;
	GetFogCfg(pos, dcfg, fcfg);
	return fcfg == 0xf;
}

bool iFogMap::IsInvisible(const iPoint& pos) const
{
	uint8 dcfg,fcfg;
	GetFogCfg(pos, dcfg, fcfg);
	return dcfg == 0xf;
}


