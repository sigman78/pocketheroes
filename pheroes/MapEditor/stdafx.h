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

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__5F6C77AC_E99A_46F1_B287_9D8CF954E8A6__INCLUDED_)
#define AFX_STDAFX_H__5F6C77AC_E99A_46F1_B287_9D8CF954E8A6__INCLUDED_

// Change these values to use different versions
//#define WINVER		0x0400
#define _WIN32_WINNT	0x0500
#define _WIN32_IE	0x0400
#define _RICHEDIT_VER	0x0100

#define _WTL_NEW_PAGE_NOTIFY_HANDLERS
#include <atlbase.h>
#include <atlapp.h>
#define _HMM_EDITOR_

#include "he/he.std.h"
#include "he/he.tables.h"
#include "he/he.container.map.h"
#include "he/he.container.cache.h"
#include "he/he.wnd.helpers.h"
#include "xe/xe.std.h"
#include "xe/xe.dc.hlp.h"
#include "li/li.std.h"
#include "li/li.cmndlg.h"
#include "si/si.std.h"
#include "si/si.wctrl.tree.h"
#include "pa/pa.xml.builder.h"
#include "pa/pa.map.h"
#include "pa/pa.sax.pathhandler.h"

// Parse string in format of "{x},{y}" and return iPoint(x,y)
// TODO: Move to the more reasonable place
#include "ifaces.h"

#include "isometric.h"
#include "rtti.h"
#include "common.h"
#include "cm_constructions.h"
#include "cm_creatures.h"
#include "cm_magic.h"
#include "cm_castles.h"
#include "cm_events.h"
#include "defines.h"
#include "TextMgr.h"
#include "army.h"
#include "objects.h"
#include "nodes.h"
#include "Artifacts.h"
#include "treasury_cnst.h"
#include "Constructions.h"
#include "SpriteMgr.h"
#include "SoundMgr.h"
#include "AvatarMgr.h"
#include "resmgr.h"
#include "MapHandler.h"

extern CAppModule _Module;

#include <atlwin.h>
#include <atlframe.h>
#include <atlsplit.h>
#include <atlctrlw.h>
#include <atlctrlx.h>
#include <atlctrls.h>
#include <atlcrack.h>

#include "ChildDlgs.h"
#include "composer.h"

extern iItemMgr gItemMgr;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__5F6C77AC_E99A_46F1_B287_9D8CF954E8A6__INCLUDED_)
