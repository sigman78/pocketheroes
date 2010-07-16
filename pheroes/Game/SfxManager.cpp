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
#include "SfxManager.h"
#include "xau/xau.audio.h"

iSfxManager::iSfxManager()
: m_pDataFile(NULL), m_pPcmBuff(NULL), m_pcmBuffLen(0), m_pXauBuff(NULL), m_xauBuffLen(0)
{

}

iSfxManager::~iSfxManager()
{
	Cleanup();
}

bool iSfxManager::Init(const iStringT& fname)
{
	Cleanup();
	m_pDataFile = OpenWin32File(fname);
	if (!m_pDataFile) return false;
	uint32 cnt;
	m_pDataFile->Read(&cnt, sizeof(uint32));
	while (cnt--) {
		uint32 pcmLen, xauLen;
		m_entries.Add(m_pDataFile->GetPosition());
		m_pDataFile->Read(&pcmLen, sizeof(uint32));
		m_pDataFile->Read(&xauLen, sizeof(uint32));
		m_pDataFile->Seek(xauLen, iFileI::FSEEK_CUR);
	}

	m_pcmBuffLen = m_xauBuffLen = 16*1024;
	m_pPcmBuff = (sint8*)malloc(m_pcmBuffLen);
	m_pXauBuff = malloc(m_xauBuffLen);
	
	return true;
}

void iSfxManager::Cleanup()
{
	m_entries.RemoveAll();
	if (m_pPcmBuff) {
		free(m_pPcmBuff);
		m_pPcmBuff = NULL;
		m_pcmBuffLen = 0;
	}
	if (m_pXauBuff) {
		free(m_pXauBuff);
		m_pXauBuff = NULL;
		m_xauBuffLen = 0;
	}
	if (m_pDataFile) {
		m_pDataFile.reset();
		m_pDataFile = NULL;
	}
}

const uint16 MOV_SOUNDS[STYPE_COUNT] = {
	0xFFFF,			// water
	CSND_HMOVE03,	// sand (beach)
	CSND_HMOVE06,	// dirt
	CSND_HMOVE01,	// grass
	CSND_HMOVE01,	// swamp
	CSND_HMOVE04,	// lava
	CSND_HMOVE03,	// wasteland
	CSND_HMOVE05,	// desert
	CSND_HMOVE05,	// snow
	CSND_HMOVE05,	// new desert
	CSND_HMOVE03,	// pavement
	CSND_HMOVE03,	// new wasteland
};

uint16 iSfxManager::GetMovSound(SURF_TYPE st, bool bRoad)
{
	if (bRoad) return MOV_SOUNDS[STYPE_DIRT];
	check(st != STYPE_WATER);
	return MOV_SOUNDS[st];
}

uint32 iSfxManager::ReadSample(uint16 idx)
{
	if (idx >= m_entries.GetSize()) return -1;

	uint32 pcmLen, xauLen;
	m_pDataFile->Seek(m_entries[idx]);
	m_pDataFile->Read(&pcmLen, sizeof(uint32));
	m_pDataFile->Read(&xauLen, sizeof(uint32));
	if (m_xauBuffLen < xauLen) {
		free(m_pXauBuff);
		m_pXauBuff = malloc(xauLen);
		m_xauBuffLen = xauLen;
	}
	m_pDataFile->Read(m_pXauBuff, xauLen);
	if (m_pcmBuffLen < pcmLen) {
		free(m_pPcmBuff);
		m_pPcmBuff = (sint8*)malloc(pcmLen);
		m_pcmBuffLen = pcmLen;
	}
	uint32 decoded = xau_pcm8m_decode( m_pXauBuff, xauLen, m_pPcmBuff, pcmLen );
	check(decoded == pcmLen);
	return pcmLen;
}

sint32 iSfxManager::PlaySound(uint16 idx, bool bLoop)
{
	if (gSettings.ForceNoSound() || !gApp.SndPlayer().Inited()) return -1;
	uint32 len = ReadSample(idx);
	if (len == -1) return -1;
	return gApp.SndPlayer().PlaySample(m_pPcmBuff, len, (bLoop)?iSoundPlayer::PlayLoop:0);
}

sint32 iSfxManager::ReplaceSound(sint32 channel, uint16 idx)
{
	if (gSettings.ForceNoSound() || !gApp.SndPlayer().Inited()) return -1;
	uint32 len = ReadSample(idx);
	if (len == -1) return -1;
	return gApp.SndPlayer().ReplaceSample(channel, m_pPcmBuff, len);
}

