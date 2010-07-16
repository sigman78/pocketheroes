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

#ifndef _HMM_GAME_SOUND_MANAGER_H_
#define _HMM_GAME_SOUND_MANAGER_H_

//////////////////////////////////////////////////////////////////////////
class iSfxEntry 
{
public:
	iSfxEntry()
	: m_pBuff(NULL), m_siz(0)
	{ }

	~iSfxEntry()
	{
		if (m_pBuff) {
			free(m_pBuff);
			m_pBuff = NULL;
			m_siz = 0;
		}
	}

	void Allocate(uint32 siz)
	{
		check(m_pBuff == NULL);
		m_pBuff = (sint8*)malloc(siz);
		m_siz = siz;
	}

	uint32 Length() const { return m_siz; }
	const sint8* Data() const { return m_pBuff; }
	sint8* Data() { return m_pBuff; }

private:
	sint8*	m_pBuff;
	uint32	m_siz;
};

//////////////////////////////////////////////////////////////////////////
class iSfxManager
{
public:
	iSfxManager();
	~iSfxManager();

	bool Init(const iStringT& fname);
	void Cleanup();

	// 
	static uint16 GetMovSound(SURF_TYPE st, bool bRoad);
	
	//
	sint32 PlaySound(uint16 idx, bool bLoop = false);
	sint32 ReplaceSound(sint32 channel, uint16 idx);

private:
	uint32 ReadSample(uint16 idx);

private:
	typedef iSimpleArray<uint32> iEntryList;

	sint8*		m_pPcmBuff;
	uint32		m_pcmBuffLen;
	void*		m_pXauBuff;
	uint32		m_xauBuffLen;
	iFilePtr	m_pDataFile;
	iEntryList	m_entries;
};

#endif //_HMM_GAME_SOUND_MANAGER_H_

