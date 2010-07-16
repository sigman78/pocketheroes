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

#ifndef _GXLIB_RANDOMIZER_H_
#define _GXLIB_RANDOMIZER_H_

/*
inline DumpF(char* format, ...)
{
    char buff[256];
	va_list argptr;
	int res;

	va_start( argptr, format );
    res = _vsnprintf(buff, 1024, format, argptr);
    va_end(argptr);
	if (res == -1) OutputDebugStringA("== Invalid format ==\n");
	OutputDebugStringA(buff);
	OutputDebugStringA("\n");
}*/


//
class iRandomizer
{
public:
	enum {
		MaxVal = 0x7FFF
	};

public:
	// c-tor
	iRandomizer(uint32 seed = 1)
	{ SetNewSeed(seed); }

	inline uint32 SetNewSeed(uint32 seed) 
	{ 
		m_holdrand = seed;
		//DumpF("---- RND initialization with %d", m_holdrand);
		return m_holdrand;
	}

	inline uint32 GetSeed() const { return m_holdrand; }

	inline sint32 Rand(sint32 maxVal = MaxVal) 
	{ 
		check(maxVal > 0);

		sint32 res = (sint32)(((m_holdrand = m_holdrand * 214013L + 2531011L) >> 16) & 0x7fff);
		//DumpF("- RND value %d", res);
		if (maxVal != MaxVal) res %= maxVal;
		//DumpF("---- RND returns %d", res);
		return res;
	}

	inline sint32 Rand(sint32 minVal, sint32 maxVal) 
	{ 
		check(maxVal > minVal);
		return minVal + (Rand(maxVal - minVal));
	}

private:
	uint32	m_holdrand;
};

#endif //_GXLIB_RANDOMIZER_H_

