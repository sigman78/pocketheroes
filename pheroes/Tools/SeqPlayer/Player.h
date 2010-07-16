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

#ifndef _HMM_SEQUENCE_PLAYER_ENGINE_H_
#define _HMM_SEQUENCE_PLAYER_ENGINE_H_


//////////////////////////////////////////////////////////////////////////
interface IPlayerCtrl
{
	virtual void OnPlayerFileLoaded(const iStringT& fname, sint32 seqcnt) PURE;
	virtual void OnPlayerPosChanged(sint32 frame) PURE;
};

//////////////////////////////////////////////////////////////////////////
class iSeqPlayer
{
public:
	iSeqPlayer(IPlayerCtrl* pPlayerCtrl);
	~iSeqPlayer();

	bool LoadSequence(const iStringT& fname);
	void Compose(HDC hdc, const iRect& orc);

	inline sint32 GetSeqCnt() const { return m_seqCnt; }
	inline sint32 GetCurFrame() const { return m_curFrame; }
	inline void SetCurFrame(sint32 curFrame) { m_curFrame = curFrame; m_pPlayerCtrl->OnPlayerPosChanged(m_curFrame); }

private:
	IPlayerCtrl*	m_pPlayerCtrl;
	iDib			m_dib;
	iDib			m_buff;
	sint32			m_seqCnt;
	sint32			m_curFrame;
	sint32			m_lastCmpFr;
};

#endif //_HMM_SEQUENCE_PLAYER_ENGINE_H_