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
 *	@brief Common [iOLiB::Si] definations
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_COMMON_DEFINES_H_
#define _IOLIB_SILICON_COMMON_DEFINES_H_

#include "../he/he.std.h"
#include "../he/he.container.fast.array.h"
#include "../xe/xe.std.h"
#include "../xe/xe.memorydc.h"

#include <atlbase.h>
#include <atlapp.h>
extern CAppModule _Module;
#include <atlwin.h>

/// Predefined system timer id
const uint32 IOLIB_SYSTIMER_ID		= 1978;
/// Predefined system timer period
const uint32 IOLIB_SYSTIMER_DELAY	= 50;

#endif //_IOLIB_SILICON_COMMON_DEFINES_H_
/** @} */ // iolib_si
