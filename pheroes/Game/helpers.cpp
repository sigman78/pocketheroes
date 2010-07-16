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

//////////////////////////////////////////////////////////////////////////
struct RAND_QUANTITY_ITEM {
	sint32	minVal;
	sint32	maxVal;
};

const RAND_QUANTITY_ITEM	INITIAL_QUNTITY[6] = {
	{750,1500}, {1000,2000}, {1500,3000}, {2000,4000}, {3000,6000}, {10000,25000}
};

sint32 InitialTroopQuantity(CREATURE_TYPE ct)
{
	sint32 minVal = INITIAL_QUNTITY[CREAT_DESC[ct].level-1].minVal / CREAT_DESC[ct].pidx;
	sint32 maxVal = INITIAL_QUNTITY[CREAT_DESC[ct].level-1].maxVal / CREAT_DESC[ct].pidx;
	return  minVal + gGame.Map().Rand((maxVal-minVal));
}

void InitCreatGroup(iCreatGroup& cg)
{
	if (cg.IsValid() && cg.Count() == RANDOM_QUANTITY) cg.Count() = InitialTroopQuantity(cg.Type());
}

void InitArmy(iArmy& army)
{
	for (uint32 xx=0; xx<7; ++xx) InitCreatGroup(army[xx]);
}
	

//////////////////////////////////////////////////////////////////////////
uint32 SelectSpells(iSpellList& spIds, iSpellFilter filter, uint32 count, MAGIC_SPELL reqSpell)
{
	check(count > 0 && spIds.GetSize() == 0);

	uint16 tsp[MSP_COUNT];
	uint32 tsp_cnt = 0;
	for (uint16 xx=0; xx<MSP_COUNT; ++xx) {
		if ((filter.typeMask & (1<<SPELL_DESCRIPTORS[xx].type)) && (filter.levelMask & (1<<SPELL_DESCRIPTORS[xx].level)) && (filter.schoolMask & (1<<SPELL_DESCRIPTORS[xx].school)) && xx != reqSpell) {
			tsp[tsp_cnt++] = xx;
		}
	}


	if (reqSpell != MSP_INVALID) count--;
	while (count) {
		sint32 rval = gGame.Map().Rand(tsp_cnt);
		spIds.Add(tsp[rval]);
		tsp[rval] = tsp[--tsp_cnt];
		count--;
	}

	if (reqSpell != MSP_INVALID) spIds.InsertBefore(gGame.Map().Rand(spIds.GetSize()+1), reqSpell);
	check(count == 0);

	return spIds.GetSize();
}

//////////////////////////////////////////////////////////////////////////
sint32 CalcRandValue(const uint8 pVals[], sint32 siz)
{
	check(siz>0);
	sint32 tot_vals=0;
	for (sint32 xx=0; xx<siz; ++xx) tot_vals += pVals[xx];
	if (!tot_vals) return -1;
	sint32 rval = gGame.Map().Rand(tot_vals);

	sint32 ridx=0;
	sint32 sval=pVals[ridx];
	while (sval<=rval) {
		ridx++;
		sval+=pVals[ridx];
	}

	check(pVals[ridx] != 0);

	return ridx;
}

sint32 InitialExpPts()
{
	return 50 + gGame.Map().Rand(100);
}

//////////////////////////////////////////////////////////////////////////
iStringT FormatDate(uint32 timestamp, bool bShowTime)
{
	FILETIME ft;
	SYSTEMTIME st;
	LONGLONG ll;
	ll = Int32x32To64(timestamp, 600000000);
	ft.dwLowDateTime = (DWORD)ll;
	ft.dwHighDateTime = ll >> 32;
	FileTimeToSystemTime(&ft, &st);
	if (bShowTime) return iFormat(_T("%d %s %d (%d:%02d)"), st.wDay, gTextMgr[TRID_MONTH_JAN+st.wMonth-1], st.wYear, st.wHour, st.wMinute);
	else return iFormat(_T("%d %s %d"), st.wDay, gTextMgr[TRID_MONTH_JAN+st.wMonth-1], st.wYear);
}


//////////////////////////////////////////////////////////////////////////

bool SaveGameFile( const iStringT& fname, iGameWorld& gmap )
{
	// Check and create save directory (if not exists)
	iStringT saveDir = gSavePath.Left(gSavePath.Length()-1);
	bool dirIsOk = iFile::DirExists(saveDir);
	if ( !dirIsOk ) {
		dirIsOk = iFile::DirCreate(saveDir);
	}
	if (!dirIsOk) {
		DWORD dwErr = GetLastError();
		iTextDlg tdlg(&gApp.ViewMgr(), _T(""), iFormat(_T("Unable to create '%s' folder: ErrorCode: 0x%08X"), saveDir.CStr(), dwErr), PID_NEUTRAL);
		tdlg.DoModal();
	}

	// normally we should create it in the save directory
	iStringT tempSaveName( gSavePath + _T("tempsave.tmp") );
	iFilePtr pFile( CreateWin32File( tempSaveName ) );

	if ( !pFile ) {
		DWORD dwErr = GetLastError();
		iTextDlg dlg( &gApp.ViewMgr(), _T("Failure"), iFormat(_T("Unable to save [%s], ErrorCode: 0x%08X"), fname.CStr(), dwErr), PID_NEUTRAL);
		dlg.DoModal();
		return false;
	}

	if ( !gGame.Map().SaveToFile( pFile.get() ) ) {
		iTextDlg dlg( &gApp.ViewMgr(), _T("Failure"), _T("Unable to save game!"), PID_NEUTRAL );
		dlg.DoModal();
		return false;
	}
	// close file
	pFile.reset();
	// rename file to the destination
	iFile::Delete( fname );
	iFile::Rename( tempSaveName, fname );
	iFile::Delete( tempSaveName );
	return true;
}


