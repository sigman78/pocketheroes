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
#include "player.h"

iSeqPlayer::iSeqPlayer(IPlayerCtrl* pPlayerCtrl)
: m_pPlayerCtrl(pPlayerCtrl), m_seqCnt(0), m_curFrame(-1)
{
}

iSeqPlayer::~iSeqPlayer()
{
}

bool iSeqPlayer::LoadSequence(const iStringT& fname)
{
	FILE* seqFile = fopen(CvtT2A<>(fname.CStr()),"rt");
	if (!seqFile) return false;
	char pngname[128];
	uint32 scnt;
	fscanf(seqFile, "{%[^}]}{%ul}", pngname, &scnt);
	fclose(seqFile);
	if (!scnt || !iDibLoader::Load(m_dib, iStringT(CvtA2T<>(pngname))) || m_dib.GetWidth() < scnt || (m_dib.GetWidth() % scnt) != 0) return false;
	
	m_seqCnt = scnt;
	m_curFrame = 0;
	m_pPlayerCtrl->OnPlayerFileLoaded(fname, m_seqCnt);
	m_pPlayerCtrl->OnPlayerPosChanged(m_curFrame);
	m_buff.Create(iSize(m_dib.GetWidth() / scnt, m_dib.GetHeight()));
	return true;
}

const sint32 ZOOM = 2;

void iSeqPlayer::Compose(HDC hdc, const iRect& orc)
{
	HRGN hrgn = CreateRectRgn(orc.x,orc.y,orc.x+orc.w,orc.y+orc.h);

	if (m_curFrame >= 0 && m_curFrame < m_seqCnt) {
		iRect src(m_curFrame*m_buff.GetWidth(),0,m_buff.GetWidth(),m_buff.GetHeight());
		m_buff.Fill(iColor(128,32,64,255));
		m_dib.CopyRectToDibXY(&m_buff, src, iPoint(), BLEND_ALPHABLEND);

		sint32 dw = m_buff.GetWidth() * ZOOM;
		sint32 dh = m_buff.GetHeight() * ZOOM;
		sint32 dx = (sint32)(orc.w/2) - dw/2;
		sint32 dy = (sint32)(orc.h/2) - dh/2;
		iDibDC::StretchDibToDCRect(m_buff, iRect(dx,dy,dw,dh), hdc);

		HRGN hdrgn = CreateRectRgn(dx,dy,dx+dw,dy+dh);
		CombineRgn(hrgn, hrgn, hdrgn, RGN_DIFF);
	}

	HBRUSH hb = CreateSolidBrush(RGB(128,32,64));
	FillRgn(hdc, hrgn, hb);
	DeleteObject(hb);
}

