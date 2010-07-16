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

//
//

#include "pa/pa.convert.utf8.h"

namespace Charsets {

	/*
	* Bits  Hex Min  Hex Max  Byte Sequence in Binary
	*   7  00000000 0000007f 0vvvvvvv
	*  11  00000080 000007FF 110vvvvv 10vvvvvv
	*  16  00000800 0000FFFF 1110vvvv 10vvvvvv 10vvvvvv
	*  21  00010000 001FFFFF 11110vvv 10vvvvvv 10vvvvvv 10vvvvvv
	*  26  00200000 03FFFFFF 111110vv 10vvvvvv 10vvvvvv 10vvvvvv 10vvvvvv
	*  31  04000000 7FFFFFFF 1111110v 10vvvvvv 10vvvvvv 10vvvvvv 10vvvvvv 10vvvvvv
	*/

	const t_utf8_ucs_map g_utf8_ucs_map[sz_utf8_ucs_map] =
	{//cmask    cval         lmask       lval
		{ 0x00,    0x00, 0 * 0, 0x00      , 0x0         },
		{ 0x80,    0x00, 0 * 6, 0x7F      , 0x0         }, /* 1 byte sequence */
		{ 0xE0,    0xC0, 1 * 6, 0x7FF     , 0x80        }, /* 2 byte sequence */
		{ 0xF0,    0xE0, 2 * 6, 0xFFFF    , 0x800       }, /* 3 byte sequence */
		{ 0xF8,    0xF0, 3 * 6, 0x1FFFFF  , 0x10000     }, /* 4 byte sequence */
		{ 0xFC,    0xF8, 4 * 6, 0x3FFFFFF , 0x200000    }, /* 5 byte sequence */
		{ 0xFE,    0xFC, 5 * 6, 0x7FFFFFFF, 0x4000000   }, /* 6 byte sequence */
	};//g_utf8_ucs_map



} // namespace
