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
#include "IntroDlg.h"
#include "lzo.h"

iIntroDlg::iIntroDlg(iViewMgr* pViewMgr)
: iDialog(pViewMgr), m_dibLogo(iSize(320,240), iDib::RGB), m_aniStep(0)
{
	iFilePtr sdataFile = OpenWin32File(gDataPath + L"game.pix");
	if (!sdataFile) return;
	// disabled for now
	check(0); 
	uint32 tmpBuffLen;
	sdataFile->Read(&tmpBuffLen, sizeof(tmpBuffLen));
	unsigned char* pTmpBuff = (unsigned char*)malloc(tmpBuffLen);
	sdataFile->Read(pTmpBuff, tmpBuffLen);
	iDynamicBuffer tmpData;
	LZO::Decompress(pTmpBuff, tmpBuffLen, tmpData );
	memcpy(m_dibLogo.GetPtr(), tmpData.GetData(), tmpData.GetSize());
	m_pMgr->SetTimer(1000, 1, this);
}

void iIntroDlg::OnTimer(uint32 tid)
{
	if (m_aniStep < 255) {
		m_aniStep += 25;
		m_pMgr->SetTimer(30, 1, this);
		Invalidate();
	} else {
		EndDialog(DRC_OK);
	}
}

void iIntroDlg::OnCompose()
{
	if (m_aniStep < 255) {
		m_dibLogo.CopyToDibXY(gApp.Surface(), iPoint());
		gApp.Surface().Fade((uint8)(255 - m_aniStep));
	}
}

iSize iIntroDlg::GetDlgMetrics()
{
	return iSize(10,10);
}
