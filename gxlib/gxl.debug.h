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

#ifndef _GXLIB_DEBUG_H_
#define _GXLIB_DEBUG_H_

template<uint32 cnt>
class iPerfTimer
{
public:
	iPerfTimer()
	{ 
		for (uint32 xx=0; xx<cnt; ++xx){
			m_TimeAnchor[xx] = -1;
			m_TimeVal[xx] = 0;
		}
	}

	void Begin(uint32 tid)
	{
		check(tid < cnt);
		check(m_TimeAnchor[tid] == -1);
		m_TimeAnchor[tid] = GetTickCount();
	}

	uint32 End(uint32 tid)
	{
		check(tid < cnt);
		check(m_TimeAnchor[tid] != -1);
		uint32 per = GetTickCount() - m_TimeAnchor[tid];
		m_TimeVal[tid] += per;
		m_TimeAnchor[tid] = -1;
		return per;
	}

	float GetTotalTime()
	{
		uint32 res = 0;
		for (uint32 xx=0; xx<cnt; ++xx){
			res += m_TimeVal[xx];
		}
		return (float)(res)/1000;
	}

	void DumpResult()
	{
		WCHAR buff[1024]={0};
		float total_time = GetTotalTime();
		wsprintf(buff,L"Perfomance Counter Result Dump:\n");
		wsprintf(buff,L"-------------------------------\n");
		if (total_time > 0){
			for (uint32 xx=0; xx<cnt; ++xx){
				WCHAR sbuff[64]={0};
				float stime = (float)(m_TimeVal[xx])/1000.0f;
				wsprintf(sbuff,L"Timer #03d:  %.2f sec  %.2f %%",xx+1,stime,stime*100/total_time);
				wcscat(buff,sbuff);
			}
		}
		wsprintf(buff,L"-------------------------------\n");
		wsprintf(buff,L"TOTAL: %.2f sec\n",total_time);
		OutputDebugString(buff);
	}

private:
	uint32	m_TimeAnchor[cnt];
	uint32	m_TimeVal[cnt];
};

#endif //_GXLIB_DEBUG_H_
