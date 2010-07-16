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

#ifndef _GXLIB_DISPLAY_H_
#define _GXLIB_DISPLAY_H_

#ifdef OS_WINCE
#else
#include "gxl.memdc.h"
#endif //OS_WINCE

/*
 * iDisplay
 */
class iDisplay : public iDispMsgHnd
{
private:
#if defined (OS_WINCE)
#	if !defined (GXL_NOGAPI)
	GXDisplayProperties	m_Props;
	uint8*				m_pBuff;
#	endif
	bool				m_bVGA;
#else
	iMemoryDC			m_memDC;
#endif //OS_WINCE
	
	iDib				m_BackBuff;
	float				m_gamma;
	iSize				m_Siz;
	HWND				m_hWnd;
	uint32				m_Flags;
	bool				m_bInited;

private:
	void RebuildGammaTables();

public:
	iDisplay();
	~iDisplay();
	bool Init(HWND hwnd, const iSize& siz, uint32 flags);
	void SetGamma(float gamma);
	void SetOrientation(bool bLandscape, bool bLefthander);
	void Destroy();
	void msg_OnPaint(HDC hdc);
	void msg_Suspend();
	void msg_Resume();
	void DoPaint(const iRect& rc, HDC hdc = 0);

	inline iDib& GetSurface() { return m_BackBuff; }
	inline iSize SurfMetrics() const { return m_BackBuff.GetSize(); }
};

#endif //_GXLIB_DISPLAY_H_

