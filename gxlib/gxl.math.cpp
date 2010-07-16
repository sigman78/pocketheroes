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

#include <windows.H>
#include <MATH.H>
#include "gxl.common.def.h"
#include "gxl.fixpoint.h"
#include "gxl.math.h"

float SIN_TABLE_FLT[256];
float COS_TABLE_FLT[256];
fix32 SIN_TABLE_FIX[256];
fix32 COS_TABLE_FIX[256];

void InitMath()
{
	float pi2 = PI*2.0f;
	float pi05 = PI/2.0f;
	for (uint32 xx=0; xx<256; ++xx){
		float fxx = (float)xx;
		float sval = (float)sin((fxx/256.0f*pi2) - pi05);
		float cval = (float)cos((fxx/256.0f*pi2) - pi05);

		SIN_TABLE_FLT[xx] = sval;
		COS_TABLE_FLT[xx] = cval;
		SIN_TABLE_FIX[xx] = sval;
		COS_TABLE_FIX[xx] = cval;
	}
}
