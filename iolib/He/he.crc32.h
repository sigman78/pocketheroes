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
 *	@brief A class that can be used to compute the CRC-32 of a data stream implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_CRC32_CALCULATION_H_
#define _IOLIB_HELIUM_CRC32_CALCULATION_H_

/// A class that can be used to compute the CRC-32 of a data stream
class iCrc32
{
public:
	/// Default constructor
	iCrc32()
	{
	}

	/// Destructor
	virtual ~iCrc32()
	{
	}

	/// Computes the CRC-32 of the given buffer
	/// initial dwCrc32 == 0xFFFFFFFF and dwCrc32 must be postinverted
	static inline void CalcBlock(uint8 *buffer, uint32 buffsize, uint32 &dwCrc32)
	{
		for(uint32 xx = 0; xx < buffsize; ++xx)	dwCrc32 = ((dwCrc32) >> 8) ^ iTables::crc32[(buffer[xx]) ^ ((dwCrc32) & 0x000000FF)];
	}
	
	/// Computes the CRC-32 of specified file
	static uint32 CalcFile(const iStringT &fname);
};

#endif //_IOLIB_HELIUM_CRC32_CALCULATION_H_
/** @} */ // iolib_he
