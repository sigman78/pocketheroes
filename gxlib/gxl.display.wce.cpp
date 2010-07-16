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


#define GETRAWFRAMEBUFFER   0x00020001

#define FORMAT_565 1
#define FORMAT_555 2
#define FORMAT_OTHER 3

typedef struct _RawFrameBufferInfo
{
   WORD wFormat;
   WORD wBPP;
   VOID *pFramePointer;
   int  cxStride;
   int  cyStride;
   int  cxPixels;
   int  cyPixels;
} RawFrameBufferInfo;

#include "gxl.gxdyn.h"
extern iGAPILoader gapi;
/////////////////////////

iDisplay::iDisplay()
: m_bVGA(false), m_pBuff(0), m_bInited(false)
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
	
	// init dynamic loader
	if ( !gGapi.Init(false) ) return false;

	if (!gGapi.GXOpenDisplay(m_hWnd, GX_FULLSCREEN)) {
		return false;				// we won't be able to draw.
	}
	
	m_Props = gGapi.GXGetDisplayProperties();

	//MessageBox(NULL, iFormat(_T("Landscape: %d, Width: %d, Height: %d, PitchX: %d, PitchY: %d\n"),m_Props.ffFormat & kfLandscape, m_Props.cxWidth, m_Props.cyHeight, m_Props.cbxPitch, m_Props.cbyPitch).CStr(), NULL, MB_OK);

	if ( (m_Flags & GXLF_LANDSCAPE) && !(m_Flags & GXLF_DEV_LANDSCAPE)) m_Siz = iSize(siz.h,siz.w);
	else m_Siz = siz;
	m_BackBuff.Init(m_Siz,iDib::RGB);


	RawFrameBufferInfo rfbi;
	HDC hdc = GetDC(NULL);

	if (ExtEscape(hdc, GETRAWFRAMEBUFFER, 0, NULL, sizeof(RawFrameBufferInfo), (char *) &rfbi) > 0 && rfbi.pFramePointer) {
		m_pBuff = (uint8*)rfbi.pFramePointer;
		m_bVGA = (rfbi.cxPixels == 480 && rfbi.cyPixels == 640);
	} 
	ReleaseDC(NULL, hdc);

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
	gGapi.GXCloseDisplay();
	m_bInited = false;
}

void iDisplay::msg_OnPaint(HDC hdc)
{
	DoPaint(m_Siz);
}

void iDisplay::msg_Suspend()
{
	int res = gGapi.GXSuspend();
	check(res == 1);
}

void iDisplay::msg_Resume()
{
	int res = gGapi.GXResume();
	check(res == 1);
}

const sint32 QVGA_CX_STRIDE = 2;
const sint32 QVGA_CY_STRIDE = 240 * 2;
const sint32 QVGA_CX_PIXELS = 240;
const sint32 QVGA_CY_PIXELS = 320;

const sint32 VGA_CX_STRIDE = 2;
const sint32 VGA_CY_STRIDE = 480 * 2;
const sint32 VGA_CX_PIXELS = 480;
const sint32 VGA_CY_PIXELS = 640;

void iDisplay::DoPaint(const iRect& rc, HDC)
{
	uint8 *pBuff;
	if (m_pBuff) pBuff = m_pBuff;
	else pBuff = (uint8*)gGapi.GXBeginDraw();

	if (!pBuff) return;

	// blit
	const iDib::pixel* pSrc = m_BackBuff.GetPtr();


	if (m_Flags & GXLF_LANDSCAPE) {
		// Landscape mode
		if (m_bVGA) {
			// VGA
			if (m_Flags & GXLF_LHANDER) {
				// Left hander mode
				for (uint32 yy=0; yy< m_Siz.h; ++yy){
					uint32* pDst1 = (uint32*)(pBuff+(VGA_CX_STRIDE*2*(VGA_CX_PIXELS-yy-1))-VGA_CY_STRIDE);
					uint32* pDst2 = (uint32*)(pBuff+ (VGA_CX_STRIDE*2*(VGA_CX_PIXELS-yy-1)));
					for (uint32 xx=0; xx<m_Siz.w; ++xx){
#ifdef GXL_FBGAMMA
						uint32 dwVal = BlitPixel(*pSrc);
#else
						uint32 dwVal = *pSrc;
#endif
						dwVal |= (dwVal << 16);
						*pDst1 = *pDst2 = dwVal;
						pDst1 += VGA_CY_STRIDE >> 1;
						pDst2 += VGA_CY_STRIDE >> 1;
						pSrc++;
					}
				}
			} else {
				// Right hander mode
				for (uint32 yy=0; yy< m_Siz.h; ++yy){
					uint32* pDst1 = (uint32*)(pBuff+(VGA_CY_STRIDE*(VGA_CY_PIXELS-2))+(VGA_CX_STRIDE*2*yy));
					uint32* pDst2 = (uint32*)(pBuff+(VGA_CY_STRIDE*(VGA_CY_PIXELS-1))+(VGA_CX_STRIDE*2*yy));
					for (uint32 xx=0; xx<m_Siz.w; ++xx){
#ifdef GXL_FBGAMMA
						uint32 dwVal = BlitPixel(*pSrc);
#else
						uint32 dwVal = *pSrc;
#endif
						dwVal |= (dwVal << 16);
						*pDst1 = *pDst2 = dwVal;
						pDst1 -= VGA_CY_STRIDE >> 1;
						pDst2 -= VGA_CY_STRIDE >> 1;
						pSrc++;
					}
				}
			}
		} else {
			// QVGA
			if (m_Props.ffFormat & kfLandscape) {
				// Devices with landscape video memory addressation
				if (m_Flags & GXLF_LHANDER) {
					// Left hander mode
					uint32 pixcnt = m_Props.cxWidth*m_Props.cyHeight;
					iDib::pixel* pDst = (iDib::pixel*)(pBuff) + pixcnt - 1;
					while (pixcnt--) {
#ifdef GXL_FBGAMMA
						*pDst = BlitPixel(*pSrc);
#else
						*pDst = *pSrc;
#endif
						pDst--;
						pSrc++;
					}
				} else {
					// Right hander mode
					uint32 pixcnt = m_Props.cxWidth*m_Props.cyHeight;
					iDib::pixel* pDst = (iDib::pixel*)(pBuff);
					while (pixcnt--) {
#ifdef GXL_FBGAMMA
						*pDst = BlitPixel(*pSrc);
#else
						*pDst = *pSrc;
#endif
						pDst++;
						pSrc++;
					}
				}
			} else {
				// Devices with normal (portrait) video memory addressation
				if (m_Flags & GXLF_LHANDER) {
					uint32 stride = m_Props.cbyPitch >> 1;
					// Left hander mode
					for (uint32 yy=0; yy< m_Siz.h; ++yy){
						iDib::pixel* pDst = (iDib::pixel*)(pBuff+(m_Props.cbxPitch*(m_Props.cxWidth-yy-1)));
						for (uint32 xx=0; xx<m_Siz.w; ++xx){
#ifdef GXL_FBGAMMA
							*pDst = BlitPixel(*pSrc);
#else
							*pDst = *pSrc;
#endif
							pDst += stride;
							pSrc++;
						}
					}
				} else {
					uint32 stride = m_Props.cbyPitch >> 1;
					// Right hander mode
					for (uint32 yy=0; yy< m_Siz.h; ++yy){
						iDib::pixel* pDst = (iDib::pixel*)(pBuff+(m_Props.cbyPitch *(m_Props.cyHeight-1))+(m_Props.cbxPitch*yy));
						for (uint32 xx=0; xx<m_Siz.w; ++xx){
#ifdef GXL_FBGAMMA
							*pDst = BlitPixel(*pSrc);
#else
							*pDst = *pSrc;
#endif
							pDst -= stride;
							pSrc++;
						}
					}
				}
			}
		}
	} else {
		if (m_bVGA) {
		} else {
			uint32 pixcnt = m_Props.cxWidth*m_Props.cyHeight;
			iDib::pixel* pDst = (iDib::pixel*)(pBuff);
			while (pixcnt--) {
#ifdef GXL_FBGAMMA
				*pDst = BlitPixel(*pSrc);
#else
				*pDst = *pSrc;
#endif
				pDst++;
				pSrc++;
			}
		}
	}

	if (!m_pBuff) gGapi.GXEndDraw();
}
