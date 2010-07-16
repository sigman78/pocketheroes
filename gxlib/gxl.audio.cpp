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

#include "gxl.inc.h"
#include "gxl.audio.h"

//////////////////////////////////////////////////////////////////////////
const sint16 LOG_GAIN_VALS[257] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02,
	0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x04, 0x04,
	0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07,
	0x07, 0x08, 0x08, 0x08, 0x08, 0x08, 0x09, 0x09,
	0x09, 0x09, 0x09, 0x0A, 0x0A, 0x0A, 0x0A, 0x0B,
	0x0B, 0x0B, 0x0B, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
	0x0D, 0x0D, 0x0D, 0x0D, 0x0E, 0x0E, 0x0E, 0x0E,
	0x0F, 0x0F, 0x0F, 0x0F, 0x10, 0x10, 0x10, 0x10,
	0x11, 0x11, 0x11, 0x11, 0x12, 0x12, 0x12, 0x13,
	0x13, 0x13, 0x13, 0x14, 0x14, 0x14, 0x14, 0x15,
	0x15, 0x15, 0x16, 0x16, 0x16, 0x17, 0x17, 0x17,
	0x17, 0x18, 0x18, 0x18, 0x19, 0x19, 0x19, 0x1A,
	0x1A, 0x1A, 0x1B, 0x1B, 0x1B, 0x1C, 0x1C, 0x1C,
	0x1D, 0x1D, 0x1D, 0x1E, 0x1E, 0x1E, 0x1F, 0x1F,
	0x1F, 0x20, 0x20, 0x20, 0x21, 0x21, 0x21, 0x22,
	0x22, 0x23, 0x23, 0x23, 0x24, 0x24, 0x25, 0x25,
	0x25, 0x26, 0x26, 0x27, 0x27, 0x27, 0x28, 0x28,
	0x29, 0x29, 0x2A, 0x2A, 0x2B, 0x2B, 0x2C, 0x2C,
	0x2C, 0x2D, 0x2D, 0x2E, 0x2E, 0x2F, 0x2F, 0x30,
	0x30, 0x31, 0x31, 0x32, 0x33, 0x33, 0x34, 0x34,
	0x35, 0x35, 0x36, 0x37, 0x37, 0x38, 0x38, 0x39,
	0x3A, 0x3A, 0x3B, 0x3B, 0x3C, 0x3D, 0x3E, 0x3E,
	0x3F, 0x40, 0x40, 0x41, 0x42, 0x43, 0x43, 0x44,
	0x45, 0x46, 0x47, 0x47, 0x48, 0x49, 0x4A, 0x4B,
	0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53,
	0x54, 0x55, 0x56, 0x58, 0x59, 0x5A, 0x5B, 0x5D,
	0x5E, 0x60, 0x61, 0x63, 0x64, 0x66, 0x67, 0x69,
	0x6B, 0x6D, 0x6F, 0x71, 0x73, 0x75, 0x78, 0x7A,
	0x7D, 0x80, 0x83, 0x86, 0x89, 0x8D, 0x91, 0x95,
	0x9A, 0xA0, 0xA6, 0xAD, 0xB5, 0xC0, 0xCD, 0xE0,
	0x100
};

//////////////////////////////////////////////////////////////////////////
const sint32 DEF_STREAM_VOLUME = 256;

iSoundPlayer::iStream::iStream() 
: m_state(Available), m_pBuff(NULL), m_bufLen(0), m_smpLen(0), m_smpPos(0), m_volume(DEF_STREAM_VOLUME), m_newVol(DEF_STREAM_VOLUME), m_bNeedStop(false)
{

}

iSoundPlayer::iStream::~iStream() 
{
	Cleanup();
}

void iSoundPlayer::iStream::Cleanup()
{
	if (m_pBuff) {
		free(m_pBuff);
		m_bufLen = m_smpLen = m_smpPos = 0;
	}

	m_state = Available;
}

void iSoundPlayer::iStream::Reset()
{
	m_smpLen = m_smpPos = 0;
	m_state = Available;
}

bool iSoundPlayer::iStream::PushSample(sint8* pSample, uint32 smpLen, uint32 flags)
{
	check(m_state == Available);
	check(smpLen);
	if (m_bufLen < smpLen) {
		Cleanup();
		m_pBuff = (sint8*)malloc(smpLen);
		m_bufLen = smpLen;
	}

	memcpy(m_pBuff, pSample, smpLen);
	m_smpPos = 0;
	m_smpLen = smpLen;
	m_flags = flags;
	m_state = Busy;
	m_volume = m_newVol = DEF_STREAM_VOLUME;
	m_bNeedStop = false;

	return true;
}

void iSoundPlayer::iStream::Stop()
{
	m_bNeedStop = true;
	m_newVol = 0;
}

uint32 iSoundPlayer::iStream::FillBuffer(sint32* pProcBuff, uint32 bufSamples)
{
	check(m_pBuff);

	sint8* src = m_pBuff + m_smpPos;
	sint32* dst = pProcBuff;

	uint32 copied = 0;
	while (1) {
		uint32 toCopy = iMIN<uint32>(bufSamples-copied, m_smpLen - m_smpPos);
		sint32 volDif = m_newVol - m_volume;
		uint32 toFade = iMIN<uint32>(iABS(volDif), toCopy);
		sint32 fadeStep = iSIGN(volDif);

		uint32 bb = 0;
		for (; bb<toFade; ++bb) {
			m_volume += fadeStep;
			*dst += *src * m_volume;
			dst++;
			src++;

		}
		for (; bb<toCopy; ++bb) {
			*dst += *src * m_volume;
			dst++;
			src++;
		}

		m_smpPos += toCopy;
		copied += toCopy;

		if (m_flags & PlayLoop) {
			if (m_bNeedStop && m_volume == 0) {
				Reset();
				break;
			} else if (copied < bufSamples) {
				src = m_pBuff;
				m_smpPos = 0;
			} else break;
		} else {
			if (m_smpPos == m_smpLen) {
				Reset();
			}
			break;
		}
	}
	return copied;
}

void iSoundPlayer::iStream::SetVolume(sint32 volume)
{
	m_newVol = volume;
}

//////////////////////////////////////////////////////////////////////////
const uint32 STREAM_COUNT	= 8;	// streams count
const uint32 BUFFER_COUNT	= 3;	// buffers count
const uint32 SAMPLE_SIZE	= 2;	// size of sample in bytes
const uint32 BUFFER_LENGTH	= 2000;	// length of buffer in samples

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
DWORD iSoundPlayer::MixerThread(LPVOID pData)
{
	iSoundPlayer* _this = (iSoundPlayer*)pData;
	sint32*	procBuff = new sint32[BUFFER_LENGTH];
	MSG msg;

	while (GetMessage(&msg, 0, 0, 0) && msg.message != WM_QUIT) {
		if (msg.message == MM_WOM_DONE) {
			WAVEHDR& wh = _this->m_buffers[ _this->m_nextBuffer ];

			// cleanup temp buffer
			memset(procBuff, 0, BUFFER_LENGTH * sizeof(sint32));

			// process all streams
			sint32 streams = 0;
			::EnterCriticalSection(&_this->m_csGuard);
			for (sint32 n=0; n<STREAM_COUNT; ++n) {
				if (_this->m_streams[n].m_state != iStream::Available) {
					_this->m_streams[n].FillBuffer(procBuff, BUFFER_LENGTH);
					streams++;
				}
			}
			::LeaveCriticalSection(&_this->m_csGuard);

			// fill out buffer
			sint32* src = procBuff;
			sint16* dst = (sint16*)wh.lpData;
			for (uint32 bb=0; bb<BUFFER_LENGTH; ++bb) {
				*dst = (sint16)iCLAMP<sint32>(-32768, 32767,((*src * _this->m_volume)>>8));
				src++;
				dst++;
			}

			MMRESULT result = waveOutWrite( _this->m_hWaveOut, &wh, sizeof( wh ) );
			_this->m_nextBuffer = ( _this->m_nextBuffer + 1 ) % BUFFER_COUNT;
		}
		Sleep(10);
	}
	delete[] procBuff;
	return 0;
}

//////////////////////////////////////////////////////////////////////////

iSoundPlayer::iSoundPlayer()
: m_hWaveOut(NULL), m_buffers(NULL), m_samples(NULL), m_streams(NULL), m_hWorkThread(NULL), m_volume(0)
{
}

iSoundPlayer::~iSoundPlayer()
{
	Close();
}

bool iSoundPlayer::HasAudioDev()
{
	WAVEFORMATEX wfex = {WAVE_FORMAT_PCM, 1, 22050, 44100, 4, 16, 0 };
	HWAVEOUT hWaveOut = NULL;
	if ( MMSYSERR_NOERROR != ::waveOutOpen( &hWaveOut, WAVE_MAPPER, &wfex, 0, 0, CALLBACK_NULL) ) {
		return false;
	}
	waveOutClose(hWaveOut);
	return true;
}

bool iSoundPlayer::Init()
{
	Close();

	InitializeCriticalSection(&m_csGuard);

	m_hWorkThread = ::CreateThread(NULL, 0, MixerThread, this, CREATE_SUSPENDED, &m_idWorkThread);

	WAVEFORMATEX wfex = {WAVE_FORMAT_PCM, 1, 22050, 44100, 4, 16, 0 };
	if ( MMSYSERR_NOERROR != ::waveOutOpen( &m_hWaveOut, WAVE_MAPPER, &wfex, m_idWorkThread, (DWORD)this, CALLBACK_THREAD ) ) {
		check(0 == "Unable to open WaveOut device!");
		return false;
	}


	m_buffers = new WAVEHDR[BUFFER_COUNT];
	m_samples = new sint16[BUFFER_COUNT * BUFFER_LENGTH];
	m_nextBuffer		= 0;
	memset(m_buffers, 0, BUFFER_COUNT * sizeof(WAVEHDR));
	memset(m_samples, 0, BUFFER_COUNT * BUFFER_LENGTH * SAMPLE_SIZE);

	for (uint32 n=0; n<BUFFER_COUNT; ++n) {
		WAVEHDR& wh = m_buffers[ n ];
		wh.lpData			= (char*)(m_samples + n * BUFFER_LENGTH);
		wh.dwBufferLength	= BUFFER_LENGTH*SAMPLE_SIZE;
		wh.dwFlags			= WHDR_DONE;
		wh.dwUser			= n;
		if (MMSYSERR_NOERROR != waveOutPrepareHeader(m_hWaveOut, &wh, sizeof(wh))) {
			check(0 == "Unable to prepare wave header!");
			return false;
		}
		if (MMSYSERR_NOERROR != waveOutWrite( m_hWaveOut, &wh, sizeof( wh ) )) {
			check(0 == "Unable to start playback!");
			return false;
		}
	}

	m_streams = new iStream[STREAM_COUNT];

	::ResumeThread(m_hWorkThread);

	return true;
}

void iSoundPlayer::Close()
{
	if (m_hWaveOut) {
		// Quit mixer thread
		PostThreadMessage(m_idWorkThread,WM_QUIT,0,0);
		if (WaitForSingleObject(m_hWorkThread, 1000) != WAIT_OBJECT_0) {
		}
		// Reset and close audio device
		waveOutReset(m_hWaveOut);
		waveOutClose(m_hWaveOut);
		// cleanup headers
		for (uint32 n=0; n<BUFFER_COUNT; ++n) {
			WAVEHDR& wh = m_buffers[ n ];
			waveOutUnprepareHeader(m_hWaveOut, &wh, sizeof(wh));
		}
		m_hWaveOut = NULL;
		// delete all buffers
		delete[] m_streams;
		m_streams = NULL;
		delete[] m_samples;
		m_samples = NULL;
		delete[] m_buffers;
		m_buffers = NULL;
	}
}

void iSoundPlayer::SetVolume(sint32 volume)
{
	::EnterCriticalSection(&m_csGuard);
	m_volume = iCLAMP<sint32>(0, 256, volume);
	::LeaveCriticalSection(&m_csGuard);
}

sint32 iSoundPlayer::GetStreamVolume(sint32 streamId)
{
	check(m_hWaveOut);
	check(streamId>=0 && streamId<STREAM_COUNT);
	return m_streams[streamId].GetVolume();
}

void iSoundPlayer::SetStreamVolume(sint32 streamId, sint32 volume)
{
	check(m_hWaveOut);
	check(streamId>=0 && streamId<STREAM_COUNT);
	::EnterCriticalSection(&m_csGuard);
	m_streams[streamId].SetVolume(volume);
	::LeaveCriticalSection(&m_csGuard);
}

bool iSoundPlayer::IsBusyStream(sint32 streamId) const
{
	check(streamId>=0 && streamId<STREAM_COUNT);
	return m_streams[streamId].m_state != iStream::Available;
}

sint32 iSoundPlayer::PlaySample(sint8* pSample, uint32 smpLen, uint32 flags)
{
	check(m_hWaveOut != NULL);

	// try to find available stream
	for (sint32 n = 0; n<STREAM_COUNT; ++n) {
		if (m_streams[n].m_state == iStream::Available) {
			::EnterCriticalSection(&m_csGuard);
			m_streams[n].PushSample(pSample, smpLen, flags);
			::LeaveCriticalSection(&m_csGuard);
			return n;
		}
	}
	return -1;
}

sint32 iSoundPlayer::ReplaceSample(sint32 streamId, sint8* pSample, uint32 smpLen)
{
	check(m_hWaveOut);
	check(streamId>=0 && streamId<STREAM_COUNT);
	check(m_streams[streamId].m_state == iStream::Busy && (m_streams[streamId].m_flags & PlayLoop));

	::EnterCriticalSection(&m_csGuard);
	m_streams[streamId].Reset();
	m_streams[streamId].PushSample(pSample, smpLen, PlayLoop);
	::LeaveCriticalSection(&m_csGuard);
	
	return streamId;
}


void iSoundPlayer::Stop(sint32 streamId)
{
	check(streamId >= 0 && streamId < STREAM_COUNT);
	::EnterCriticalSection(&m_csGuard);
	m_streams[streamId].Stop();
	::LeaveCriticalSection(&m_csGuard);
}
