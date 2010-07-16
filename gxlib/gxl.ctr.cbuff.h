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

#ifndef _GXLIB_CONTAINER_CIRCULAR_BUFFER_H_
#define _GXLIB_CONTAINER_CIRCULAR_BUFFER_H_

template <class T, uint32 BUFF_SIZ=64>
class iCircBuff
{
public:
	iCircBuff()
	{
		m_rCur = m_wCur = 0;
		m_aT = new T[BUFF_SIZ];
	}

	~iCircBuff()
	{ 
		delete[] m_aT;
	}

	inline uint32 Count() const
	{ if (m_wCur >= m_rCur) return (m_wCur - m_rCur); else return ((BUFF_SIZ-m_rCur)+m_wCur);}

	void Put(const T& t)
	{
		if ( (m_wCur+1) == m_rCur) {
			check(0);
		}
		m_aT[m_wCur] = t;
		if (++m_wCur == BUFF_SIZ) m_wCur = 0;
	}

	T& Get()
	{
		check(m_rCur != m_wCur);

		uint32 oPos = m_rCur;
		if (++m_rCur == BUFF_SIZ) m_rCur = 0;
		return m_aT[oPos];
	}

	void Reset()
	{
		m_rCur = m_wCur = 0;
	}

private:

	uint32	m_rCur;
	uint32	m_wCur;
	T*		m_aT;
};

#endif //_GXLIB_CONTAINER_CIRCULAR_BUFFER_H_


