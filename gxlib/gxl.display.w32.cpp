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
: m_gamma(1.0f)
{}

iDisplay::~iDisplay()
{
	Destroy();
}

bool iDisplay::Init(HWND hwnd, const iSize& siz, uint32 flags)
{
	m_Flags = flags;
	m_hWnd = hwnd;
	m_Siz = siz;
	m_BackBuff.Init(m_Siz,iDib::RGB);
	if (flags & GXLF_DOUBLESIZE) m_memDC.InitDC(iSize(siz.w*2,siz.h*2));
	else m_memDC.InitDC(siz);
	
	return true;
}

void iDisplay::SetOrientation(bool bLandscape, bool bLefthander)
{
	if (bLandscape != (m_Flags&GXLF_LANDSCAPE)) {
		if (bLandscape)  m_Flags |= GXLF_LANDSCAPE;
		else m_Flags ^= GXLF_LANDSCAPE;
		iSwap(m_Siz.w,m_Siz.h);
		m_BackBuff.Resize(m_Siz);
		if (m_Flags& GXLF_DOUBLESIZE) m_memDC.InitDC(iSize(m_Siz.w*2,m_Siz.h*2));
		else m_memDC.InitDC(m_Siz);
	}
	if (bLefthander && (m_Flags & GXLF_LHANDER) == 0) m_Flags |= GXLF_LHANDER;
	else if (!bLefthander && (m_Flags & GXLF_LHANDER) != 0) m_Flags ^= GXLF_LHANDER;
}

void iDisplay::Destroy()
{
}

void iDisplay::msg_OnPaint(HDC hdc)
{
	DoPaint(m_BackBuff.GetSize(), hdc);
}

void iDisplay::msg_Suspend()
{
}

void iDisplay::msg_Resume()
{
}

__forceinline uint32 Cvt16to32(iDib::pixel pix)
{ return (uint32(pix & (0x1f<<11))) << 8 | (uint32(pix & (0x3f<<5))) << 5 | (uint32(pix & (0x1f))) << 3; }

void iDisplay::DoPaint(const iRect& rc, HDC hdc)
{
	iMemoryDC::pixel* pDst = m_memDC.GetPtr();
	const iDib::pixel* pSrc = m_BackBuff.GetPtr();


	// blit
	if (m_Flags & GXLF_DOUBLESIZE) {
		uint32 dstStride = m_memDC.GetWidth();
		for (uint32 yy=0; yy<m_Siz.h; ++yy) {
			for (uint32 xx=0; xx<m_Siz.w; ++xx) {
#ifdef GXL_FBGAMMA
				uint16 pix = BlitPixel(*pSrc);
#else
				uint16 pix = *pSrc;
#endif
				uint32 val = Cvt16to32(pix);
				*pDst = val;
				pDst++;
				*pDst = val;
				pDst++;
				pSrc++;
			}
			memcpy(pDst,pDst-dstStride,(dstStride*sizeof(iMemoryDC::pixel)));
			pDst+=dstStride;
		}
	} else {
		uint32 len = m_Siz.w * m_Siz.h;
		for (uint32 xx=0; xx<len; ++xx) {
#ifdef GXL_FBGAMMA
			uint16 pix = BlitPixel(*pSrc);
#else
			uint16 pix = *pSrc;
#endif
			uint32 val = Cvt16to32(pix);
			*pDst = val;
			pSrc++; pDst++;
		}
	}

	if (!hdc) {
		HDC thdc = GetDC(m_hWnd);
		BOOL bres = BitBlt(thdc,0,0,m_memDC.GetWidth(),m_memDC.GetHeight(),m_memDC,0,0,SRCCOPY);
		ReleaseDC(m_hWnd,thdc);
	} else {
		BitBlt(hdc,0,0,m_memDC.GetWidth(),m_memDC.GetHeight(),m_memDC,0,0,SRCCOPY);
	}
}