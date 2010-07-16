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

#ifndef _HMM_COMMON_EVENTS_H_
#define _HMM_COMMON_EVENTS_H_

/*
 *	Time-events
 */
struct iTimeEvent 
{
	enum EventFreq {
		EFREQ_NEVER = 0,
		EFREQ_1DAY, EFREQ_2DAYS, EFREQ_3DAYS, EFREQ_4DAYS, EFREQ_5DAYS, EFREQ_6DAYS, 
		EFREQ_1WEEK,EFREQ_2WEEKS, EFREQ_3WEEKS, EFREQ_1MONTH, 
		EFREQ_COUNT
	};

	iTimeEvent(const iStringT& name, const iStringT& text, uint32 playerMask, uint32 ftime, EventFreq repTime, const iMineralSet& minerals)
	: m_name(name), m_text(text), m_playerMask(playerMask), m_fTime(ftime), m_repTime(repTime), m_minerals(m_minerals) {}
	iTimeEvent()
	: m_playerMask(PIM_ALL), m_fTime(1), m_repTime(EFREQ_NEVER) { m_minerals.Reset(); }

	bool IsConform(PLAYER_ID pid, uint32 curDay) const;

	iStringT	m_name;
	iStringT	m_text;
	uint32		m_playerMask;
	uint32		m_fTime;
	EventFreq	m_repTime;
	iMineralSet	m_minerals;
};

// Time events manager
class iTimeEventMgr
{
public:
	// c-tor, d-tor
	iTimeEventMgr() {}
	~iTimeEventMgr() {}

	//
	inline uint32 EventsCount() const { return m_events.GetSize(); }
	inline const iTimeEvent& Event(uint32 idx) const { return m_events[idx]; }
	inline iTimeEvent& Event(uint32 idx) { return m_events[idx]; }
	inline void AddEvent(const iTimeEvent& event) { m_events.Add(event); }
	inline void DeleteEvent(uint32 idx) { m_events.RemoveAt(idx); }
	inline void DeleteAll() { m_events.RemoveAll(); }

private:
	iSimpleArray<iTimeEvent>	m_events;
};

#endif //_HMM_COMMON_EVENTS_H_

