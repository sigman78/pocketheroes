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

#ifndef _GXLIB_TIMER_H_
#define _GXLIB_TIMER_H_

class iTimer
{
public:
	iTimer() : m_LastTime(GetTickCount()) {}

	bool Init()
	{
		return true;
	}

	inline uint32 GetCurTime()
	{
		return m_LastTime = GetTickCount();
	}

	inline uint32 GetStep()
	{
		uint32 ntime = GetTickCount();
		uint32 sval;
		if (ntime == m_LastTime) return 0;
		else if (ntime > m_LastTime) sval = ntime - m_LastTime;
		else sval = (0xFFFFFFFF - m_LastTime) + ntime;
		m_LastTime = ntime;
		return sval;
	}

private:
	uint32	m_LastTime;
};

#endif //_GXLIB_TIMER_H_

