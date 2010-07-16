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

#ifndef _GXLIB_MATH_H_
#define _GXLIB_MATH_H_

/*
 * Math constants
 */
const float PI = 3.1415926535897932384626433832795f;
extern float SIN_TABLE_FLT[256];
extern float COS_TABLE_FLT[256];
extern fix32 SIN_TABLE_FIX[256];
extern fix32 COS_TABLE_FIX[256];

#define iter1(N) \
    tval = root + (1 << (N)); \
    if (n >= tval << (N))   \
{   n -= tval << (N);   \
	root |= 2 << (N); \
}

inline uint32 int_sqrt(uint32 n)
{
    uint32 root = 0, tval;
    iter1 (15);    iter1 (14);    iter1 (13);    iter1 (12);
    iter1 (11);    iter1 (10);    iter1 ( 9);    iter1 ( 8);
    iter1 ( 7);    iter1 ( 6);    iter1 ( 5);    iter1 ( 4);
    iter1 ( 3);    iter1 ( 2);    iter1 ( 1);    iter1 ( 0);
    return root >> 1;
}
#undef iter1

void InitMath();

#endif //_GXLIB_MATH_H_

