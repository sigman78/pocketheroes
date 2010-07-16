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

#ifndef _HMM_GAME_SOUND_MAP_H_
#define _HMM_GAME_SOUND_MAP_H_

class iSoundMap
{
public:
	iSoundMap();
	~iSoundMap();

	bool Init(iGameWorld* pMap);
	void Cleanup();
	void Update();

	void SetupEnvSounds(const iPoint& pos);
	void ResetEnvSounds();

private:
	struct iSndEnvItem {
		iSndEnvItem(uint16 _idx, uint16 _delta = 0, sint32 _channel = -1) : idx(_idx), delta(_delta), channel(_channel) {}
		
		bool operator == (const iSndEnvItem& other) const { return idx == other.idx; }
		bool operator != (const iSndEnvItem& other) const { return idx != other.idx; }

		uint16	idx;
		uint16	delta;
		sint32	channel;
	};

	typedef iSimpleArray<iSndEnvItem> iItemList;

private:
	iItemList		m_items;
	iGameWorld*		m_pMap;
	iMapT<uint16>	m_sndMap;
};

#endif //_HMM_GAME_SOUND_MAP_H_

