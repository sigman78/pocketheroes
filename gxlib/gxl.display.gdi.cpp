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

iDisplay::iDisplay()
: m_bVGA(false), m_bInited(false)
{}

iDisplay::~iDisplay()
{
	check(!m_bInited);
}

bool iDisplay::Init(HWND hwnd, const iSize& siz, uint32 flags)
{
	check(!m_bInited);
	m_Flags = flags;
	m_hWnd = hwnd;
	
	if ( (m_Flags & GXLF_LANDSCAPE) && !(m_Flags & GXLF_DEV_LANDSCAPE)) m_Siz = iSize(siz.h,siz.w);
	else m_Siz = siz;
	m_BackBuff.Init(m_Siz,iDib::RGB);

	return m_bInited = true;
}

void iDisplay::SetOrientation(bool bLandscape, bool bLefthander)
{
	if (bLandscape != ((m_Flags&GXLF_LANDSCAPE)>0) ) {
		if (bLandscape)  m_Flags |= GXLF_LANDSCAPE;
		else m_Flags ^= GXLF_LANDSCAPE;
		iSwap(m_Siz.w,m_Siz.h);
		m_BackBuff.Resize(m_Siz);
	}
	if (bLefthander && (m_Flags & GXLF_LHANDER) == 0) m_Flags |= GXLF_LHANDER;
	else if (!bLefthander && (m_Flags & GXLF_LHANDER) != 0) m_Flags ^= GXLF_LHANDER;
}

void iDisplay::Destroy()
{
	check(m_bInited);
	m_BackBuff.Cleanup();
	m_bInited = false;
}

void iDisplay::msg_OnPaint(HDC hdc)
{
	DoPaint(m_Siz, hdc);
}

void iDisplay::msg_Suspend()
{
}

void iDisplay::msg_Resume()
{
}

void iDisplay::DoPaint(const iRect& rc, HDC hdc)
{
	// blit
	const iDib::pixel* pSrc = m_BackBuff.GetPtr();

	HDC odc = hdc;
	if (!odc) odc = GetDC(m_hWnd);
	if (!odc) return;

	int SetDIBitsToDevice(
		HDC hdc,                 // handle to DC
		int XDest,               // x-coord of destination upper-left corner
		int YDest,               // y-coord of destination upper-left corner 
		DWORD dwWidth,           // source rectangle width
		DWORD dwHeight,          // source rectangle height
		int XSrc,                // x-coord of source lower-left corner
		int YSrc,                // y-coord of source lower-left corner
		UINT uStartScan,         // first scan line in array
		UINT cScanLines,         // number of scan lines
		CONST VOID *lpvBits,     // array of DIB bits
		CONST BITMAPINFO *lpbmi, // bitmap information
		UINT fuColorUse          // RGB or palette indexes
);

	if (!hdc) ReleaseDC(m_hWnd, odc);
}
