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

#ifndef _GXLIB_COMMON_DEFINES_H_
#define _GXLIB_COMMON_DEFINES_H_

/*
 *	Presets
 */

/*
 * Common defines
 */

#ifdef WIN32
#	define VARARG __cdecl
#else
#	define VARARG
#endif

#ifndef always_inline
#if defined(__GNUC__) && (__GNUC__ > 3 || __GNUC__ == 3 && __GNUC_MINOR__ > 0)
#    define always_inline __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#	define always_inline __forceinline
#else
#    define always_inline inline 
#endif
#endif


#ifdef _DEBUG
#	define ODS(x) OutputDebugString(x);
#	define check(x) assert(x);
#	define verify(x) assert(x);
#else
#	define ODS(x)
#	define check(x)
#	define verify(x) x
#endif

#define _DIM_(x) (sizeof(x) / sizeof(x[0]))
#define _END_(x) (&(x[_DIM_(x)]))

/*
 * Base types definations
 */
typedef __int8				sint8;		// -128 to 127
typedef __int16				sint16;		// -32,768 to 32,767
typedef __int32				sint32;		// -2,147,483,648 to 2,147,483,647
typedef __int64				sint64;		// -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
typedef int					sint;		// -2,147,483,648 to 2,147,483,647

typedef unsigned __int8		uint8;		// 255
typedef unsigned __int16	uint16;		// 65,535
typedef unsigned __int32	uint32;		// 4,294,967,295
typedef unsigned __int64	uint64;		// 18,446,744,073,709,551,616
typedef unsigned int		uint;		// 4,294,967,295

typedef CHAR	iCharA;					// generic 8bit char
typedef WCHAR	iCharW;					// 16bit Unicode char
typedef TCHAR	iCharT;					// depended on project defines

//
extern const uint32 iRandTable[256];


#endif //_GXLIB_COMMON_DEFINES_H_

