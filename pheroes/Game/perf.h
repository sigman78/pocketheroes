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


#pragma once

inline uint64  htime()
{
	LARGE_INTEGER t;
	QueryPerformanceCounter( &t );
	return t.QuadPart;
}

#define TIMER_START(tname) \
	uint64 tname##_start = htime(); \
	/* */

#define TIMER_END(tname) \
	uint64 tname##_end = htime(); \
	/* */

#define TIMER_VAL(tname) (uint32)((tname##_end - tname##_start) / 100)

