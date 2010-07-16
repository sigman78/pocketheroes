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
 *	@brief Common [He] library defines
 */
/** @addtogroup iolib_he
 *  @{
 */

#ifndef _IOLIB_HELIUM_DEFINES_H_
#define _IOLIB_HELIUM_DEFINES_H_

#include <assert.h>
#include <math.h>

#ifdef WIN32
#	define VARARG __cdecl
#else
#	define VARARG
#endif

#ifdef _DEBUG
#	define check(x) assert(x)
#	define verify(x) assert(x)
#else
#	define check(x)
#	define verify(x) x
#endif


/// Predefined value of Pi
const float PI = 3.1415926535897932384626433832795f;

/// Swaps content of two objects/variables
template< class T >
inline void iSwap( T& a, T& b )
{
	T tmp = a;
	a = b;
	b = tmp;
}

/// Clamps values of two variables according to specified minimum and maximum values
template <class T>
inline T iCLAMP(T mn, T mx, T vl)
{
	check(mn <= mx);
	if (vl >= mn && vl <= mx ) return vl;
	else if (vl < mn) return mn;
	else return mx;
}

/// Returns larger of two values
template <class T>
inline T iMAX(T x, T y)
{
	if (x > y) return x;
	else return y;
}

/// Returns smaller of two values
template <class T>
inline T iMIN(T x, T y)
{
	if (x < y) return x;
	else return y;
}

/// Returns absolute difference between two values
template <class T>
inline T iDIF(T x, T y)
{
	return iMAX(x,y) - iMIN(x,y);
}

/// Returns absolute value of given variable
template <class T>
inline T iABS(T x)
{
	if (x > 0) return x;
	else return -x;
}

/// Returns sign of given variable (-1,0,+1)
template <class T>
inline sint32 iSIGN(T x)
{
	if (x > 0) return 1;
	else if (x < 0) return -1;
	else return 0;
}

/// Align given value according alignment step
inline uint32 iALIGN(uint32 val, uint32 al)
{ return ((val+(al-1))/al)*al; }

/// Mathematics rounding of float number
inline float RoundFloat(float f)
{ 
	if (f < 0.0f) return (float)ceil(f-0.5f);
	else return (float)floor(f+0.5f);
}

#endif //_IOLIB_HELIUM_DEFINES_H_
/** @} */ // iolib_he
