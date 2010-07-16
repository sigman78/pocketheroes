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

void prvBlurChannelMMX(const iDib &src, iDib &dst, uint8 weight)
{
	__m64 accum;
	__m64 matrix[3];
	__m64 temp;
	__m64 zero =  _mm_setzero_si64();
	__m64 shift_div = _m_from_int( 7 );

	uint8 mb0=CLIP_BYTE((24*weight)/128);
	uint8 mb1=CLIP_BYTE((16*weight)/128);
	uint8 mb2=CLIP_BYTE((10*weight)/128);

	matrix[0] = _m_from_int(BYTES_TO_LONG(mb0,mb0,mb0,mb0));
	matrix[0] = _mm_unpacklo_pi8( matrix[0], zero );

	matrix[1] = _m_from_int(BYTES_TO_LONG(mb1,mb1,mb1,mb1));
	matrix[1] = _mm_unpacklo_pi8( matrix[1], zero );

	matrix[2] = _m_from_int(BYTES_TO_LONG(mb2,mb2,mb2,mb2));
	matrix[2] = _mm_unpacklo_pi8( matrix[2], zero );

	uint32 width = src.GetWidth();
	

	for(uint32 j=0;j<src.GetHeight()-2;j++)
	{
		const uint32* srcPtr = (uint32*)(src.GetLine( j ));
		uint32* dstPtr = (uint32*)(dst.GetLine( j + 1 ) + 1); // + (1,1)

		for(uint32 i=0;i<src.GetWidth()-2;i++) 
		{
			const uint32* tempPtr = srcPtr;
			accum = _mm_setzero_si64();
			// 0,0
			temp = _m_from_int( (sint32)( *srcPtr++ ) );
			temp = _mm_unpacklo_pi8( temp, zero );	// interleave
			temp = _mm_mullo_pi16( temp, matrix[2] );
			accum = _mm_adds_pu16( accum, temp );
			// 1,0
			temp = _m_from_int( (sint32)( *srcPtr++ ) );
			temp = _mm_unpacklo_pi8( temp, zero );	// interleave
			temp = _mm_mullo_pi16( temp, matrix[1] );
			accum = _mm_adds_pu16( accum, temp );

			// 2,0
			temp = _m_from_int( (sint32)( *srcPtr ) );
			temp = _mm_unpacklo_pi8( temp, zero );	// interleave
			temp = _mm_mullo_pi16( temp, matrix[2] );
			accum = _mm_adds_pu16( accum, temp );

			srcPtr += ( width - 2 );

			// 0,1
			temp = _m_from_int( (sint32)( *srcPtr++ ) );
			temp = _mm_unpacklo_pi8( temp, zero );	// interleave
			temp = _mm_mullo_pi16( temp, matrix[1] );
			accum = _mm_adds_pu16( accum, temp );
			// 1,1
			temp = _m_from_int( (sint32)( *srcPtr++ ) );
			temp = _mm_unpacklo_pi8( temp, zero );	// interleave
			temp = _mm_mullo_pi16( temp, matrix[0] );
			accum = _mm_adds_pu16( accum, temp );

			// 2,1
			temp = _m_from_int( (sint32)( *srcPtr ) );
			temp = _mm_unpacklo_pi8( temp, zero );	// interleave
			temp = _mm_mullo_pi16( temp, matrix[1] );
			accum = _mm_adds_pu16( accum, temp );

			srcPtr += ( width - 2 );

			// 0,2
			temp = _m_from_int( (sint32)( *srcPtr++ ) );
			temp = _mm_unpacklo_pi8( temp, zero );	// interleave
			temp = _mm_mullo_pi16( temp, matrix[2] );
			accum = _mm_adds_pu16( accum, temp );
			// 1,2
			temp = _m_from_int( (sint32)( *srcPtr++ ) );
			temp = _mm_unpacklo_pi8( temp, zero );	// interleave
			temp = _mm_mullo_pi16( temp, matrix[1] );
			accum = _mm_adds_pu16( accum, temp );

			// 2,2
			temp = _m_from_int( (sint32)( *srcPtr ) );
			temp = _mm_unpacklo_pi8( temp, zero );	// interleave
			temp = _mm_mullo_pi16( temp, matrix[2] );
			accum = _mm_adds_pu16( accum, temp );

			// finally, divide results by 128
			temp = _mm_srl_pi16( accum, shift_div );
			
			// unpack and place
			accum = _mm_packs_pu16( temp, zero );

			*dstPtr++ = (uint32)( _m_to_int( accum ) );

			srcPtr = tempPtr + 1;
		}
	}
	_mm_empty();
}


void iDibFilters::Blur(const iGrayDib &source, iGrayDib &output, uint8 weight, uint8 pass)
{
	check( &source != &output );
	
	iGrayDib *pDib1 = new iGrayDib();
	iGrayDib *pDib2 = new iGrayDib();
	source.MakeClone(*pDib1);
	source.MakeClone(*pDib2);

	uint32	accum;

	uint8 mb0=CLIP_BYTE((24*weight)/128);
	uint8 mb1=CLIP_BYTE((16*weight)/128);
	uint8 mb2=CLIP_BYTE((10*weight)/128);

	for (uint32 pp=0; pp<pass; ++pp)
	{
		for(uint32 j=0;j<pDib1->GetHeight()-2;j++)
		{
			const uint8* srcPtr = pDib1->GetBuffer( j );
			uint8* dstPtr = pDib2->GetBuffer( j + 1 ) + 1;

			for(uint32 i=0;i<pDib1->GetWidth()-2;i++) 
			{
				const uint8* tempPtr = srcPtr;
				accum = 0;

				// 0,0
				accum += ((uint32)(*srcPtr++) * mb2);
				// 1,0
				accum += ((uint32)(*srcPtr++) * mb1);
				// 2,0
				accum += ((uint32)(*srcPtr++) * mb2);

				srcPtr += (pDib1->GetWidth() - 3);

				// 0,1
				accum += ((uint32)(*srcPtr++) * mb1);
				// 1,1
				accum += ((uint32)(*srcPtr++) * mb0);
				// 2,1
				accum += ((uint32)(*srcPtr++) * mb1);

				srcPtr += (pDib1->GetWidth() - 3);

				// 0,2
				accum += ((uint32)(*srcPtr++) * mb2);
				// 1,2
				accum += ((uint32)(*srcPtr++) * mb1);
				// 2,2
				accum += ((uint32)(*srcPtr++) * mb2);

				*dstPtr++ = (uint8)iMIN((accum >> 7),(uint32)255);
				srcPtr = tempPtr + 1; 
			}
		}
		iSwap(pDib1,pDib2);
		//
		//
	}
	pDib2->MakeClone(output);
	_mm_empty();
}

void iDibFilters::MaskedBlur(ChannelMask mask, const iDib &source, iDib &output, uint8 weight, uint8 pass)
{
	check( &source != &output );
	iSize out_siz(source.GetSize().w+2, source.GetSize().h+2);

	iDib *outd[2];
	outd[0] = new iDib(source);
	outd[0]->ResizeCanvas(out_siz,Align_Center,cColorTransparent);
	outd[1] = new iDib(out_siz);
	outd[1]->Fill(cColorTransparent,BLEND_SRCCOPY);

	__m64 accum;
	__m64 matrix[3];
	__m64 temp;
	__m64 zero =  _mm_setzero_si64();
	__m64 shift_div = _m_from_int( 7 );

	uint8 mb0=CLIP_BYTE((24*weight)/128);
	uint8 mb1=CLIP_BYTE((16*weight)/128);
	uint8 mb2=CLIP_BYTE((10*weight)/128);

	matrix[0] = _m_from_int(BYTES_TO_LONG(mb0,mb0,mb0,mb0));
	matrix[0] = _mm_unpacklo_pi8( matrix[0], zero );

	matrix[1] = _m_from_int(BYTES_TO_LONG(mb1,mb1,mb1,mb1));
	matrix[1] = _mm_unpacklo_pi8( matrix[1], zero );

	matrix[2] = _m_from_int(BYTES_TO_LONG(mb2,mb2,mb2,mb2));
	matrix[2] = _mm_unpacklo_pi8( matrix[2], zero );

	uint32 width = out_siz.w;
	
	iDib *src=outd[0];
	iDib *dst=outd[1];

	for (uint32 pp=0; pp<pass; ++pp)
	{
		if (pp!=0)
		{
			iDib *tpp = dst;
			dst = src;
			src = tpp;
		}

		for(uint32 j=0;j<src->GetHeight()-2;j++)
		{
			const uint32* srcPtr = (uint32*)(src->GetLine( j ));
			uint32* dstPtr = (uint32*)(dst->GetLine( j + 1 ) + 1); // + (1,1)

			for(uint32 i=0;i<src->GetWidth()-2;i++) 
			{
				const uint32* tempPtr = srcPtr;
				accum = _mm_setzero_si64();
				// 0,0
				temp = _m_from_int( (sint32)( *srcPtr++ ) );
				temp = _mm_unpacklo_pi8( temp, zero );	// interleave
				temp = _mm_mullo_pi16( temp, matrix[2] );
				accum = _mm_adds_pu16( accum, temp );
				// 1,0
				temp = _m_from_int( (sint32)( *srcPtr++ ) );
				temp = _mm_unpacklo_pi8( temp, zero );	// interleave
				temp = _mm_mullo_pi16( temp, matrix[1] );
				accum = _mm_adds_pu16( accum, temp );

				// 2,0
				temp = _m_from_int( (sint32)( *srcPtr ) );
				temp = _mm_unpacklo_pi8( temp, zero );	// interleave
				temp = _mm_mullo_pi16( temp, matrix[2] );
				accum = _mm_adds_pu16( accum, temp );

				srcPtr += ( width - 2 );

				// 0,1
				temp = _m_from_int( (sint32)( *srcPtr++ ) );
				temp = _mm_unpacklo_pi8( temp, zero );	// interleave
				temp = _mm_mullo_pi16( temp, matrix[1] );
				accum = _mm_adds_pu16( accum, temp );
				// 1,1
				temp = _m_from_int( (sint32)( *srcPtr++ ) );
				temp = _mm_unpacklo_pi8( temp, zero );	// interleave
				temp = _mm_mullo_pi16( temp, matrix[0] );
				accum = _mm_adds_pu16( accum, temp );

				// 2,1
				temp = _m_from_int( (sint32)( *srcPtr ) );
				temp = _mm_unpacklo_pi8( temp, zero );	// interleave
				temp = _mm_mullo_pi16( temp, matrix[1] );
				accum = _mm_adds_pu16( accum, temp );

				srcPtr += ( width - 2 );

				// 0,2
				temp = _m_from_int( (sint32)( *srcPtr++ ) );
				temp = _mm_unpacklo_pi8( temp, zero );	// interleave
				temp = _mm_mullo_pi16( temp, matrix[2] );
				accum = _mm_adds_pu16( accum, temp );
				// 1,2
				temp = _m_from_int( (sint32)( *srcPtr++ ) );
				temp = _mm_unpacklo_pi8( temp, zero );	// interleave
				temp = _mm_mullo_pi16( temp, matrix[1] );
				accum = _mm_adds_pu16( accum, temp );

				// 2,2
				temp = _m_from_int( (sint32)( *srcPtr ) );
				temp = _mm_unpacklo_pi8( temp, zero );	// interleave
				temp = _mm_mullo_pi16( temp, matrix[2] );
				accum = _mm_adds_pu16( accum, temp );

				// finally, divide results by 128
				temp = _mm_srl_pi16( accum, shift_div );
				
				// unpack and place
				accum = _mm_packs_pu16( temp, zero );

				*dstPtr++ = (uint32)( _m_to_int( accum ) );

				srcPtr = tempPtr + 1;
			}
		}
		//
		//
	}
	_mm_empty();

	output.ResizeCanvas(out_siz,Align_Center,cColorTransparent);
	if (dst)
	{
		dst->CopyToDib(&output,BLEND_SRCCOPY);
	}
}
