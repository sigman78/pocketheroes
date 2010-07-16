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
#include "serialize.h"

static sint32 EventFreqDays[iTimeEvent::EFREQ_COUNT] = {
	0, 1, 2, 3, 4, 5, 6, 7, 14, 21, 28
};

bool iTimeEvent::IsConform(PLAYER_ID pid, uint32 curDay) const
{
	// player
	if ((m_playerMask & (1<<pid)) == 0) return false;
	//
	if (curDay < m_fTime) {
		return false;
	} else if ( curDay == m_fTime ) {
		return true;
	} else if ( m_repTime != EFREQ_NEVER ) {
		sint32 repeat = EventFreqDays[m_repTime];
		check( repeat != 0 );
		return 0 == ( (curDay-m_fTime) % repeat );
	} else {
		return false;
	}
}



