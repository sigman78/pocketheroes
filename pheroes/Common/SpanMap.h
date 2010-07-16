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

#ifndef _HMM_COMMON_SPANNED_MAP_H_
#define _HMM_COMMON_SPANNED_MAP_H_

class iSpannedMap 
{
public:
	enum Shape {
		Circle = 0,
		Square
	};

	struct iSpan {
		iSpan(sint16 _ypos = 0, sint16 _begin = 0, sint16 _end = 0)
		: ypos(_ypos),begin(_begin), end(_end){}

		sint16	ypos;
		sint16	begin;
		sint16	end;
	};

	// Constructs with circle or square spaned map
	iSpannedMap(Shape shape = Square, sint32 radius = 1);

	// D-tor
	~iSpannedMap(){}

	// inlines
	inline uint32 SpanLinesCount() const
	{ return m_SpanList.GetSize(); }

	inline const iSpan& operator[](sint32 idx) const
	{ check(idx>=0 && idx<(sint32)m_SpanList.GetSize()); return m_SpanList[idx]; }

private:
	void MakeCircleSpan(sint32 radius);
	void MakeSquareSpan(sint32 radius);

	iSimpleArray<iSpan>	m_SpanList;
};

#endif //_HMM_COMMON_SPANNED_MAP_H_

