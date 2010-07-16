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

#include "xe.std.h"

//
//
//

iDib* iDibTransformer::GetMultipliedDib(const iDib& src, uint16 mby)
{
	iDib *result = new iDib(iSize(src.GetWidth()*mby, src.GetHeight()*mby));

	for (uint32 yy=0; yy<src.GetHeight(); ++yy)
	{
		for (uint32 xx=0; xx<src.GetWidth(); ++xx)
		{
			result->FillRect(iRect(xx*mby,yy*mby,mby,mby),src.GetPixel(iPoint(xx,yy)),BLEND_SRCCOPY);

		}
	}

	return result;
}

void iDibTransformer::MultiplySize(iDib &dib, uint16 mby)
{
	iDib temp(&dib);
	dib.Resize(iSize(dib.GetWidth()*mby,dib.GetHeight()*mby));

	for (uint32 yy=0; yy < temp.GetHeight(); ++yy)
	{
		for (uint32 xx=0; xx < temp.GetWidth(); ++xx)
		{
			dib.FillRect(iRect(xx*mby,yy*mby,mby,mby),temp.GetPixel(iPoint(xx,yy)),BLEND_SRCCOPY);
		}
	}
}

//
void iDibTransformer::FastStretch(iDib &dib, const iSize newsize)
{
	iDib temp(dib);
	dib.Resize(newsize);

	// Copy (stretched) from dib rect
	iColor *dst = dib.m_Bits;
	const iColor *src = temp.m_Bits;

	int sw = (int)(( (float)temp.GetWidth() / (float)newsize.w ) * 65536.0),
		sh = (int)(( (float)temp.GetHeight() / (float)newsize.h ) * 65536.0),
		t1, t2 = 0;

	for ( uint32 j = 0; j < newsize.h; j++ )
	{
		t1 = ( j * sh >> 16 ) * temp.GetWidth();

		for ( uint32 i = 0; i < newsize.w; i++ )
		{
			*dst = src[t1 + ( i * sw >> 16 )];
			dst++;
		}
	}
}


void iDibTransformer::FlipHorizontal(iDib& dib)
{
	iDib src(&dib);
	for (uint32 yy=0; yy<src.GetHeight(); ++yy){
		const iColor* pSrc = src.GetLine(yy) + src.GetWidth() - 1;
		iColor* pDst = dib.GetLine(yy);
		for (uint32 xx=0; xx<src.GetWidth(); ++xx){
			*pDst = *pSrc;
			pSrc--;
			pDst++;
		}
	}
}

void iDibTransformer::FlipVertical(iDib& dib)
{
	check(0);
}
