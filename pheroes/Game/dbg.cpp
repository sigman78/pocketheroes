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
#include "dbg.h"

void DumpResources(const iMineralSet& mset, bool bShowEmpty)
{
#ifndef NDEBUG
	for (sint16 xx=0; xx<7; ++xx) {
		if (bShowEmpty || mset.quant[xx]){
			OutputDebugString(iFormat(_T("%s: %d, "), gTextMgr[TRID_MINERAL_GOLD+xx], mset.quant[xx]).CStr());
		}
	}
	OutputDebugString(_T("\n"));
#endif
}

void DumpArmy(const iArmy& army, const iStringT& hdr)
{
#ifndef NDEBUG
	OutputDebugString(_T("\n"));
	OutputDebugString(hdr.CStr());
	OutputDebugString(_T("\n----------------------\n"));

	for (uint32 xx=0; xx<7; ++xx) {
		if (army[xx].IsValid()) OutputDebugString(iFormat(_T("\tSlot %d: %d %s (Power: %d, Speed: %d)\n"), xx+1, army[xx].Count(), gTextMgr[TRID_CREATURE_PEASANT_F3+army[xx].Type()*3], army[xx].GroupPower(), CREAT_DESC[army[xx].Type()].speed).CStr());
	}

	OutputDebugString(_T("\n"));
#endif
}

#ifndef NDEBUG
LPCTSTR const ART_CELL_NAME[AC_COUNT] = {
	_T("Head"),
	_T("Neck"),
	_T("Torso"),
	_T("Left hand"),
	_T("Right hand"),
	_T("Left fingers"),
	_T("Right fingers"),
	_T("Shoulders"),
	_T("Legs"),
	_T("Feet"),
	_T("Misc 1"),
	_T("Misc 2"),
	_T("Misc 3"),
	_T("Misc 4")
};
#endif

void DumpArtifacts(iHero* pHero)
{
#ifndef NDEBUG
	OutputDebugString(iFormat(_T("\n%s artifacts:\n--------------------\n"), pHero->Name().CStr()).CStr());
	OutputDebugString(_T("--[ Weared artifacts ]--\n"));
	for (uint32 nn=0; nn<AC_COUNT; ++nn) {
		const iArtCell& art = pHero->Artifacts().DressedItem((HERO_ART_CELL)nn);
		if (!art.Empty()) {
			OutputDebugString(iFormat(_T("* %s: "), ART_CELL_NAME[nn]).CStr());
			OutputDebugString(gTextMgr[gGame.ItemMgr().m_artMgr[art.artId].NameKey(pHero)]);
			OutputDebugString(_T("\n"));
		}
	}
	OutputDebugString(_T("--[ Backpack artifacts ]--\n"));
	for (uint32 nn=0; nn<pHero->Artifacts().BackPackCount(); ++nn) {
		uint16 artId = pHero->Artifacts().BackPackItem(nn);
		OutputDebugString(_T("* "));
		OutputDebugString(gTextMgr[gGame.ItemMgr().m_artMgr[artId].NameKey(pHero)]);
		OutputDebugString(_T("\n"));
	}
#endif
}

