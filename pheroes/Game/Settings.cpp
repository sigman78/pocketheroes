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
#include "Settings.h"

const uint32 CONFIG_FILE_HDR = 0xA173B7C3;

//////////////////////////////////////////////////////////////////////////
const sint32 CONFIG_ENTRY_DESC[CET_COUNT][3] = { // min, max, default
	{0, 10,5},	// Sfx volume
	{0, 4, 0},	// Display gamma
	{0, 4, 2},	// Map scroll speed
	{0, 4, 2},	// Hero speed
	{0, 4, 2},	// Combat speed
	{0, 1, 0},	// Quick combat
	{0, 1, 1},	// End turn confitmation
	{0, 1, 0},	// Lefthander mode
	{0, 1, 0},	// Overland grid
	{0, 1, 1},	// Combat grid
	{0, 1, 1},	// Travel points mode
	{0, 1, 1},	// Survey map filtration
	{0, 1, 0},	// Survey map scale
	{0, 2, 2},	// New game dialog list sort order
	{0, -1, 0}	// New game dialog list position
};

//////////////////////////////////////////////////////////////////////////
const sint32 CONFIG_DEFKEYS[BAT_COUNT] = { 
	KEY_ENTER,	// Help mode
	KEY_VKA,	// Hand mode
	KEY_VKB,	// Survey map
	KEY_VKC,	// Minimize
	KEY_INVALID	// Make screenshot
};
//////////////////////////////////////////////////////////////////////////
LPCTSTR const CONFIG_FILE = _T("PalmHeroes.cfg");

//////////////////////////////////////////////////////////////////////////
uint32 hashname( wchar_t* ptr, unsigned seed = 0x1234abcd )
{
	uint32 h = seed;
	wchar_t ch;
	while ( 0 != (ch = *ptr++) ) {
		h ^= ( (h << 5) + (ch&0xff) + (h >> 2) );
		h ^= ( (h << 5) + (ch>>8) + (h >> 2) );
	}
	return h;
}

void InitFileSizez(file_sizez& sizez)
{
	sizez.RemoveAll();
	iStringT strText =  gRootPath + _T("*.*");
	iStringT appName;
	iFileName::GetAppName(appName);
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile(strText.CStr(), &wfd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (appName.CompareIgnoreCase(wfd.cFileName) == 0) {
				file_size_struct fss;
				fss.name = hashname(wfd.cFileName);
				fss.size = wfd.nFileSizeLow;
				sizez.Add(fss);
			}
		} while (FindNextFile(hFind, &wfd));
		FindClose(hFind);
	}
}
//////////////////////////////////////////////////////////////////////////
iSettings::iSettings() : m_gameSN(0)
{}

iSettings::~iSettings()
{ Save(); }

void iSettings::SetActivationKey(const iStringT& newKey)
{
	m_ActKey = newKey;
}

bool iSettings::Init(const iStringT& cmdLine)
{
	//
	InitFileSizez(m_filez);
	// Reset config entries
	m_bMapSpriteFile = false;
	m_bShowEnemyTurn = false;
	m_bFogOfWar = true;
	m_bNoSound = false;
	for (uint32 nn=0; nn<CET_COUNT; ++nn) m_cfgEntries[nn] = CONFIG_ENTRY_DESC[nn][2];
	for (uint32 nn=0; nn<BAT_COUNT; ++nn) m_actionKeys[nn] = CONFIG_DEFKEYS[nn];

	// Parse command line
	if (cmdLine.Length()) {
		sint32 spos = 0;
		while (spos < (sint32)cmdLine.Length()) {
			sint32 epos = cmdLine.Find(_T("--"), spos+1);
			if (epos == -1) epos = cmdLine.Length();
			iStringT arg(cmdLine.CStr()+spos, epos - spos);
			arg.Trim();
			if (arg.Compare(_T("--show_enemy_turn")) == 0) {
				m_bShowEnemyTurn = true;
			} //else if (arg.Compare(_T("--disable_fow")) == 0) { //disabled as of 1.04b (Hedin)
				//m_bFogOfWar = false;
			else if (arg.Compare(_T("--no_sound")) == 0) {
				m_bNoSound = true;
			} else if (arg.Compare(_T("--map_sprite_file")) == 0) {
				m_bMapSpriteFile = true;
			}
			spos = epos;
		}
	}
	// Read the config file
	iFileStream pConfigFile(OpenWin32File(gRootPath + CONFIG_FILE));
	if (!pConfigFile.IsValid()) return true;
	uint32 hdr; pConfigFile.Read(hdr);
	if (hdr != CONFIG_FILE_HDR) return true;
	for (uint32 nn = 0; nn<CET_COUNT; ++nn) pConfigFile.Read(m_cfgEntries[nn]);
	for (uint32 nn = 0; nn<BAT_COUNT; ++nn) pConfigFile.Read(m_actionKeys[nn]);
	pConfigFile.Read(m_ActKey);
	return true;
}

void iSettings::Save()
{
	//
	iFileStream pConfigFile(CreateWin32File(gRootPath + CONFIG_FILE));
	if (!pConfigFile.IsValid()) return;
	pConfigFile.Write(CONFIG_FILE_HDR);
	for (uint32 nn = 0; nn<CET_COUNT; ++nn) pConfigFile.Write(m_cfgEntries[nn]);
	for (uint32 nn = 0; nn<BAT_COUNT; ++nn) pConfigFile.Write(m_actionKeys[nn]);
	pConfigFile.Write(m_ActKey);
}

void iSettings::SetEntryValue(ConfigEntryType entry, sint32 val) 
{ 
	if (CONFIG_ENTRY_DESC[entry][1] == -1) m_cfgEntries[entry] = iMAX<sint32>(CONFIG_ENTRY_DESC[entry][0], val);
	else m_cfgEntries[entry] = iCLAMP<sint32>(CONFIG_ENTRY_DESC[entry][0],CONFIG_ENTRY_DESC[entry][1], val); 
}

