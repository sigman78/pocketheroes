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

#ifndef _GXLIB_COMMON_INCLUDE_H_
#define _GXLIB_COMMON_INCLUDE_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef OS_WIN32
#include <TCHAR.H>
#include <ASSERT.H>
#include <objidl.h>
#endif

#ifdef OS_WINCE
#if _WIN32_WCE == 300
#	include <dbgapi.H>
#elif _WIN32_WCE >= 420
#	include <ASSERT.H>
#else
#error Unknown version of WindowsCE
#endif
#endif

#if (_MSC_VER < 1310) && !defined(for)
#define for if (0) {} else for
#endif

#if !defined(NDEBUG)
//#define MEM_TRACK
#endif
#include "gxl.memdbg.h"

#include "gxl.common.def.h"
#include "gxl.common.tpl.h"
#include "gxl.fixpoint.h"
#include "gxl.math.h"
#include "gxl.rnd.h"
#include "gxl.smartptr.h"
#include "gxl.debug.h"
#include "gxl.metrics.h"
#include "gxl.string.std.h"
#include "gxl.string.h"
#include "gxl.memory.h"
#include "gxl.file.h"
#include "gxl.dib.h"

// flags 
const uint32 GXLF_DOUBLESIZE	= 0x01;
const uint32 GXLF_LANDSCAPE		= 0x02;
const uint32 GXLF_DEV_LANDSCAPE	= 0x04;
const uint32 GXLF_DEV_VGARES	= 0x08;
const uint32 GXLF_LHANDER		= 0x10;
const uint32 GXLF_ENABLESOUND	= 0x20;
const uint32 GXLF_REALVGA		= 0x40;

#endif //_GXLIB_COMMON_INCLUDE_H_

