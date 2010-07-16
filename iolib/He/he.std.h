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

/** @file
 *	@brief Common [iOLiB::He] include file
 */
/** @defgroup iolib_he [iOLiB::He] Kernel library
 *  @{
 */
#ifndef _IOLIB_HELIUM_STD_H_
#define _IOLIB_HELIUM_STD_H_


#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#ifdef _UNICODE
#	ifndef UNICODE
#	define UNICODE         // UNICODE is used by Windows headers
#	endif
#endif

#include <windows.h>
#include <tchar.h>
#include <wtypes.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <process.h>

#include "he.types.h"
#include "he.fixpoint.h"
#include "he.defines.h"
#include "he.buff.h"
#include "he.metrics.h"
#include "he.cpuf.h"
#include "he.memory.h"
#include "he.memory.dynbuff.h"
#include "he.smartptr.h"

#include <mmintrin.h>

#pragma comment( user, "Compiled on " __DATE__ " at " __TIME__ )

extern bool gIsKernelLibraryInited;

bool InitKernelLibrary();

struct KernelInitializer
{
	KernelInitializer()
	{
		if ( !gIsKernelLibraryInited )
			gIsKernelLibraryInited = InitKernelLibrary();
	}
};

static KernelInitializer __labuda;

#endif //_IOLIB_HELIUM_STD_H_
/** @} */ // iolib_he
