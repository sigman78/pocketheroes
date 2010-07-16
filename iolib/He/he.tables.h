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
 *	@brief Common tables declaration
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_COMMON_STATIC_PREDEFINED_TABLES_H_
#define _IOLIB_HELIUM_COMMON_STATIC_PREDEFINED_TABLES_H_

#include "he.pair.h"

/// Common tables declaration namespace
namespace iTables
{

/// CRC8 table
extern uint8	crc8[256];
/// CRC32 table
extern uint32	crc32[256];

/// uint32 based hash function
inline uint32 HashFn( uint32 nid )
{
	return nid;
}

/// iSize based hash function
inline uint32 HashFn( const iSize& siz )
{
	return (siz.w+1) * (siz.h+1);
}

/// iPoint based hash function
inline uint32 HashFn( const iPoint& pos )
{
	return ((uint32)(pos.x)+1) * ((uint32)(pos.y)+1);
}

/// String based hash function
inline uint32 HashFn( const iStringT& str )
{
	uint32 h1, h2, h;
	const iCharT *ptr = str.CStr();
	if ( *ptr == 0 ) return 0;
	h1 = *ptr; h2 = *ptr + 1;
	ptr++;
	while ( *ptr ) {
		h1 = crc8[ h1 ^ *ptr ];
		h2 = crc8[ h2 ^ *ptr ];
		ptr++;
	}
	h =  ((uint32)h1 << 8 ) | (uint32)h2;
	return h;
}


/// Hash function template
template <class T, class U>
inline uint32 HashFn( const iPair<T,U>& arg )
{
	return HashFn(arg.first) ^ HashFn(arg.second);
}


}; // iTables

#endif //_IOLIB_HELIUM_COMMON_STATIC_PREDEFINED_TABLES_H_
/** @} */ // iolib_he
