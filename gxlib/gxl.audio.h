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

#ifndef _GXLIB_AUDIO_PLAYBACK_ENGINE_H_
#define _GXLIB_AUDIO_PLAYBACK_ENGINE_H_

#include "mmsystem.h"

class iSoundPlayer
{
public:
	// Playback flags
	enum PlayFlags {
		PlayLoop	= 0x01,
	};

	// Stream
	class iStream 
	{
	public:
		friend class iSoundPlayer;

		// Stream state
		enum State {
			Available = 0,
			Busy
		};

	public:
		iStream();
		~iStream();
		bool PushSample(sint8* pSample, uint32 smpLen, uint32 flags);
		void Stop();

	protected:
		void Reset();
		void Cleanup();
		uint32 FillBuffer(sint32* pProcBuff, uint32 bufSamples);
		sint32 GetVolume() const { return m_volume; }
		void SetVolume(sint32 volume);

	protected:
		bool		m_bNeedStop;
		uint32		m_flags;
		uint32		m_bufLen;
		uint32		m_smpLen;
		uint32		m_smpPos;
		State		m_state;
		sint32		m_volume;
		sint32		m_newVol;
		sint8*		m_pBuff;
	};

public:
	friend class iGXApp;

	// c-tor, d-tor
	iSoundPlayer();
	~iSoundPlayer();

	//
	static bool HasAudioDev();

	// Init/close
	bool Init();
	void Close();
	bool Inited() const { return m_hWaveOut != NULL; }

	bool IsBusyStream(sint32 streamId) const;

	// Volume
	sint32 GetVolume() const { return m_volume; }
	void SetVolume(sint32 volume);
	sint32 GetStreamVolume(sint32 streamId);
	void SetStreamVolume(sint32 streamId, sint32 volume);

	// Playback
	sint32 PlaySample(sint8* pSample, uint32 smpLen, uint32 flags = 0);
	sint32 ReplaceSample(sint32 streamId, sint8* pSample, uint32 smpLen);
	void Stop(sint32 streamId);

protected:

	// Mixer thread
	static DWORD __stdcall MixerThread(LPVOID pData);

private:
	sint32		m_volume;
	uint32		m_nextBuffer;
	WAVEHDR*	m_buffers;
	sint16*		m_samples;
	HWAVEOUT	m_hWaveOut;
	iStream*	m_streams;
	HANDLE		m_hWorkThread;
	DWORD		m_idWorkThread;
	CRITICAL_SECTION 	m_csGuard;
};

#endif //_GXLIB_AUDIO_PLAYBACK_ENGINE_H_

