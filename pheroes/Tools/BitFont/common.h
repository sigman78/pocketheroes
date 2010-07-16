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

#ifndef _BIT_FONT_GENERATOR_COMMON_INCLUDES_H_
#define _BIT_FONT_GENERATOR_COMMON_INCLUDES_H_

#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <stdio.h>

#include "he/he.std.h"
#include "he/he.memory.dynbuff.h"
#include "xe/xe.std.h"
#include "xe/xe.memorydc.h"

const uint32 clrAnchor	= 0xFFFF0000;
const uint32 clrWidth	= 0xFFFF00FF;

const uint32 clrFace 	= 0xFFFFFFFF;
const uint32 clrShadow 	= 0xFF0000FF;
const uint32 clrFrame 	= 0xFF00FF00;

const uint32 clrWFace 	= 0x00FFFFFF;
const uint32 clrWShadow = 0x00FF0000;
const uint32 clrWFrame 	= 0x0000FF00;



#endif // _BIT_FONT_GENERATOR_COMMON_INCLUDES_H_
