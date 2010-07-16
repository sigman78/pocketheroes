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

#include "stdafx.h"
#include "SpanMap.h"

iSpannedMap::iSpannedMap(Shape shape, sint32 radius)
{
	check(radius >= 0);
	if (shape == Circle) MakeCircleSpan(radius);
	else if (shape == Square) MakeSquareSpan(radius);
	else {
		check(0);
	}
}

void iSpannedMap::MakeCircleSpan(sint32 radius)
{
	m_SpanList.SetSize(radius*2+1);

	INT cx	= 0;
	INT cy	= radius;
	INT df	= 1 - radius;
	INT d_e	= 3;
	INT d_se= -2 * radius + 5;

	do {	// Filled circle
		m_SpanList[radius-cx] = iSpan(-cx,-cy,cy);
		if ( cx ) m_SpanList[radius+cx] = iSpan(cx,-cy,cy);
		if ( df < 0 ) {
			df	+= d_e;
			d_e	+= 2;
			d_se+= 2;
		} else {
			if( cx != cy ) {
				m_SpanList[radius-cy] = iSpan(-cy,-cx,cx);
				if ( cy ) m_SpanList[radius+cy] = iSpan(cy,-cx,cx);
			}
			df	+= d_se;
			d_e	+= 2;
			d_se+= 4;
			cy--;
		}
		cx++;
	} while ( cx <= cy );
}

void iSpannedMap::MakeSquareSpan(sint32 radius)
{
	sint32 cnt = radius*2+1;
	m_SpanList.SetSize(cnt);

	for (sint32 yy=0; yy<cnt; ++yy) {
		m_SpanList[yy].ypos = (sint16)(-radius + yy);
		m_SpanList[yy].begin = (sint16)-radius;
		m_SpanList[yy].end = (sint16)radius;
	}
}

