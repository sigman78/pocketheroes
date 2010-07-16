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

#ifndef _STD_AFX_H_
#define _STD_AFX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _WIN32_WCE
#	if (_WIN32_WCE <= 211)
#	error This project can not be built for H/PC Pro 2.11 or earlier platforms.
#	endif
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#pragma warning(disable:4786)

#include <windows.h>

//
// Define file size value ( "magic number" xor "file size")
//
//#define MAGIC_NUMBER (0x17a63e0f)
//#define FILE_SIZE_VALUE (MAGIC_NUMBER ^ 256782)

//#define _DEEP_DEBUG_
//#define DEBUG_LOG
//#define FOG_MODE 2
//#define TRANSLATOR_VERSION

#ifdef _DEEP_DEBUG_
#	include <crtdbg.h>
#	define _CRTDBG_MAP_ALLOC
#	define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#else
#	define DEBUG_NEW new
#endif

#if (_MSC_VER < 1310) && !defined(for)
#define for if (0) {} else for
#endif


#ifdef OS_WINCE
#	include <gx.h>
#if UNDER_CE == 300
#	include <dbgapi.H>
#elif UNDER_CE == 420
#	include <ASSERT.H>
#else
//#	error Unknown version of WindowsCE
#endif
#endif

#ifdef OS_WIN32
#include <stdio.h>
#include <objidl.h>
#include <tchar.h>
#include <ASSERT.H>
#endif

#include "gxl.inc.h"
#include "rtti.h"
#include "gxl.window.h"
#include "gxl.ctr.cbuff.h"
#include "gxl.ctr.array.h"
#include "gxl.ctr.map.h"
#include "gxl.ctr.list.h"
#include "gxl.input.h"
#include "gxl.timer.h"
#include "gxl.display.h"

#include "gxl.view.h"
#include "gxl.dialog.h"
#include "gxl.topmostview.h"
#include "gxl.popupview.h"
#include "gxl.stdctrl.h"
#include "gxl.viewmgr.h"
#include "gxl.audio.h"

#include "cnst_text.h"
#include "cnst_gfx.h"
#include "cnst_sfx.h"

#include "gxl.application.h"
#include "gxl.dibfont.h"
#include "isometric.h"
#include "SortArray.h"
#include "TextManager.h"
#include "TextComposer.h"

#include "common.h"
#include "cm_constructions.h"
#include "cm_creatures.h"
#include "cm_magic.h"
#include "cm_castles.h"
#include "cm_events.h"
#include "GfxManager.h"
#include "SfxManager.h"

#include "Magic.h"
#include "PathFinding.h"
#include "fogmap.h"
#include "AvatarMgr.h"
#include "Army.h"
#include "treasury_cnst.h"
#include "ItemMgr.h"
#include "Objects.h"
#include "Artifacts.h"
#include "Constructions.h"
#include "CastleCnsts.h"
#include "Castles.h"
#include "hero.h"
#include "player.h"
#include "AniObject.h"

#include "Map.h"
#include "GameWorld.h"
#include "SoundMap.h"
#include "ai.h"
#include "BattleInfo.h"
#include "BattleFort.h"
#include "BattleUnit.h"
#include "BattleEngine.h"
#include "Game.h"
#include "Settings.h"

#ifdef DEBUG_LOG
#include "gxl.logmgr.h"
extern iLogMgr gLogMgr;
#endif //DEBUG_LOG


extern	iStringT		gRootPath;
extern	iStringT		gDataPath;
extern	iStringT		gSavePath;
extern	iStringT		gMapsPath;
extern	iGXApp			gApp;
extern	iDibReader		gDibReader;
extern	iTextComposer	gTextComposer;
extern	iTextManager	gTextMgr;
extern	iGfxManager		gGfxMgr;
extern	iSfxManager		gSfxMgr;
extern	iGame			gGame;
extern	iSettings		gSettings;

#include "GfxHlp.h"
#include "Composer.h"

#include "ViewDefs.h"
#include "CommonControls.h"
#include "ArmyList.h"
#include "CommonDialogs.h"
#include "PopupDlg.h"

#endif //_STD_AFX_H_


