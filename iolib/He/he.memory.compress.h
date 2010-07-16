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
 *	@brief Generic data compression/decompression handling (RLE) implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_MEMORY_COMPRESSION_H_
#define _IOLIB_HELIUM_MEMORY_COMPRESSION_H_

#include "he.memory.dynbuff.h"

/// Generic data compression/decompression handling (RLE)
class iRLE
{
public:
	/// Compresses source buffer using RLE compression
	static bool Compress(const iDynamicBuffer& src, iDynamicBuffer& dst);
	/// Decompresses RLE compressed source buffer
	static bool Decompress(const iDynamicBuffer& src, iDynamicBuffer& dst);
};

#endif //_IOLIB_HELIUM_MEMORY_COMPRESSION_H_
/** @} */ // iolib_he
