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
#include "gxl.memory.h"
#include "gxl.file.h"
#include "gxl.dib.h"

namespace iDibTransform{

void Rotate(const iDib& src, iDib &dst, uint32 angle)
{
//	check(src.GetSize() == dst.GetSize());
//	check(src.GetType() == dst.GetType());
	check(angle < 256);

	float rad = (2*PI*angle)/256.0f;
	float deg = (angle*360)/256.0f;
	float cosine = (float)cos(rad);
	float sine = (float)sin(rad);
	sint32 srcw = src.GetWidth();
	sint32 srch = src.GetHeight();

	int x1 = (int)(-srch * sine);
	int y1 = (int)(srch * cosine);
	int x2 = (int)(srcw * cosine - srch * sine);
	int y2 = (int)(srch * cosine + srcw * sine);
	int x3 = (int)(srcw * cosine);
	int y3 = (int)(srcw * sine);

	int minx = iMIN(0,iMIN(x1, iMIN(x2,x3)));
	int miny = iMIN(0,iMIN(y1, iMIN(y2,y3)));
	int maxx = iMAX(0,iMAX(x1, iMAX(x2,x3)));
	int maxy = iMAX(0,iMAX(y1, iMAX(y2,y3)));

	int w = maxx - minx;
	int h = maxy - miny;
	dst.Init(iSize(w,h),src.GetType());

/*
	WCHAR buff[128] = {0};
	wsprintf(buff,L"Angle: %.2f deg --- (%d,%d), %d,%d,%d -- %d,%d,%d\n",deg,w,h,x1,x2,x3,y1,y2,y3);
	ODS(buff);
*/
	iDib::pixel* dst_ptr = dst.GetPtr();
	for( int y = 0; y < h; y++ ){
		for( int x = 0; x < w; x++ ){
			int sourcex = (int)((x+minx)*cosine + (y+miny)*sine);
			int sourcey = (int)((y+miny)*cosine - (x+minx)*sine);
			if( sourcex >= 0 && sourcex < srcw && sourcey >= 0 	&& sourcey < srch ){
				*dst_ptr = src.GetPixel(iPoint(sourcex,sourcey));
			} else {
				*dst_ptr = 0;
			}
			dst_ptr++;
		}
	}
}

void FastStretch(const iDib& src, iDib &dst, uint32 boffs)
{
	iDib::pixel* pDst = dst.GetPtr();
	for (uint32 yy=0; yy<dst.GetHeight(); ++yy){
		const iDib::pixel* pSrc = src.GetPtr(iPoint(0,yy<<boffs));
		for (uint32 xx=0; xx<dst.GetWidth(); ++xx) {
			*pDst = *pSrc;
			pSrc += (1<<boffs);
			pDst ++;
		}
	}
}

void PyramidalStretch(const iDib& src, iDib &dst, uint32 boffs)
{
	iDib::pixel* pDst = dst.GetPtr();
	for (uint32 yy=0; yy<dst.GetHeight(); ++yy){
		const iDib::pixel* pSrc = src.GetPtr(iPoint(0,yy<<boffs));
		for (uint32 xx=0; xx<dst.GetWidth(); ++xx) {
			uint32 rval = 0;
			uint32 gval = 0;
			uint32 bval = 0;
			for (sint32 cy = 0; cy < (1<<boffs); ++cy){
				for (sint32 cx = 0; cx < (1<<boffs); ++cx){
					iDib::pixel pix = *(pSrc + cy*src.GetWidth()+cx);
					rval += (pix & (0x1F<<11))>>11;
					gval += (pix & (0x3F<<5))>>5;
					bval += pix & (0x1F);
				}
			}
			*pDst++ = ((rval>>(boffs<<1)) << 11) | ((gval>>(boffs<<1)) << 5) | (bval>>(boffs<<1));
			pSrc += (1<<boffs);
		}
	}
}


} // namespace iDibTransform
