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
 *	@brief Basic low level memory manipulation handling
 */
/** @addtogroup iolib_he
 *  @{
 */

#ifndef _IOLIB_HELIUM_MEMORY_H_
#define _IOLIB_HELIUM_MEMORY_H_

/// Fill typed 8bit buffer with given value function prototype
extern void (*FillBlock8)  (uint8 *dst, const uint8 src, uint32 size);
/// Fill typed 8bit buffer with given value function (standard implementation)
extern void FillBlock8Std  (uint8 *dst, const uint8 src, uint32 size);
/// Fill typed 8bit buffer with given value function (initialization)
extern void FillBlock8Init  (uint8 *dst, const uint8 src, uint32 size);

/// Fill typed 32bit buffer with given value function prototype
extern void (*FillBlock32)  (uint32 *dst, const uint32 src, uint32 size);
/// Fill typed 32bit buffer with given value function (standard implementation)
extern void FillBlock32Std  (uint32 *dst, const uint32 src, uint32 size);
/// Fill typed 32bit buffer with given value function (with MMX optimizization)
extern void FillBlock32MMX  (uint32 *dst, const uint32 src, uint32 size);
/// Fill typed 32bit buffer with given value function (initialization)
extern void FillBlock32Init  (uint32 *dst, const uint32 src, uint32 size);

/// Copy specified ammount of data from source typed 32bit buffer to the destination function prototype
extern void (*CopyBlock8)  (uint8 *dst, const uint8 *src, uint32 size);
/// Copy specified ammount of data from source typed 32bit buffer to the destination function (standard implementation)
extern void CopyBlock8Std  (uint8 *dst, const uint8 *src, uint32 size);
/// Copy specified ammount of data from source typed 32bit buffer to the destination function (initialization)
extern void CopyBlock8Init (uint8 *dst, const uint8 *src, uint32 size);

/// Copy specified ammount of data from source typed 8bit buffer to the destination function prototype
extern void (*CopyBlock32)  (uint32 *dst, const uint32 *src, uint32 size);
/// Copy specified ammount of data from source typed 8bit buffer to the destination function (standard implementation)
extern void CopyBlock32Std  (uint32 *dst, const uint32 *src, uint32 size);
/// Copy specified ammount of data from source typed 8bit buffer to the destination function (with MMX optimizization)
extern void CopyBlock32MMX  (uint32 *dst, const uint32 *src, uint32 size);
/// Copy specified ammount of data from source typed 8bit buffer to the destination function (with SSE optimizization)
extern void CopyBlock32SSE  (uint32 *dst, const uint32 *src,uint32 size);
/// Copy specified ammount of data from source typed 8bit buffer to the destination function (initialization)
extern void CopyBlock32Init (uint32 *dst, const uint32 *src, uint32 size);

/// Move specified ammount of data from source typed 8bit buffer to the destination function prototype
extern void (*MoveBlock8)  (uint8 *dst, const uint8 *src, uint32 size);
/// Move specified ammount of data from source typed 8bit buffer to the destination function (standard implementation)
extern void MoveBlock8Std  (uint8 *dst, const uint8 *src, uint32 size);
/// Move specified ammount of data from source typed 8bit buffer to the destination function (initialization)
extern void MoveBlock8Init (uint8 *dst, const uint8 *src, uint32 size);

/// Fill typed buffer with given value function template
template< class T >
void FillBlock( T* dst, const T& val, int n = 1 )
{
	FillBlock8( (uint8*)dst, (uint8*)&val, sizeof( T ) * n );
}

/// Copy specified ammount of data from source typed buffer to the destination function template
template< class T >
void CopyBlock( T* dst, T* src, int n = 1 )
{
	CopyBlock8( (uint8*)dst, (uint8*)src, sizeof( T ) * n );
}

/// Move specified ammount of data from source typed buffer to the destination function template
template< class T >
void MoveBlock( T* dst, T* src, int n = 1 )
{
	MoveBlock8( (uint8*)dst, (uint8*)src, sizeof( T ) * n );
}


#endif //_IOLIB_HELIUM_MEMORY_H_
/** @} */ // iolib_he
