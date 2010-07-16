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
#include "../he/he.vector.h"

//
// Pyramidal Stretch
//
inline void SummRow( uint64* buffer, const uint32* source, uint32 iters, uint32 rowsz )
{
	__asm 
	{
		mov esi, [source]
		mov edi, [buffer]
		mov edx, rowsz
		mov ebx, iters

		pxor mm0, mm0
		movq mm1, mm0
	lp1:
		movq mm2,[edi]
		mov ecx, ebx
	lp2:
		movd mm0, [esi]
		punpcklbw mm0, mm1
		paddw mm2, mm0
		add esi, 4 
		sub ecx, 1
		jnz lp2

		movq [edi], mm2
		add edi, 8
		sub edx, 1
		jnz lp1
	}
}

inline void UnpackRow( uint32* dest, const uint64* buffer, uint32 rowsz )
{
	__asm 
	{
		mov esi,[buffer]
		mov edi,[dest]
		mov ecx,rowsz
		pxor mm3,mm3
	lp1:
		movq mm0, [esi]
		psrlw mm0, 6
		packuswb mm0, mm3
		movd [edi], mm0
		add esi, 8
		add edi, 4
		sub ecx, 1
		jnz lp1
	}
}

void PyramidalStretch(iDib &dst_dib, const iDib &src_dib)
{
	uint32 sw = src_dib.GetWidth();
	uint32 sh = src_dib.GetHeight();
	uint32 dw = dst_dib.GetWidth();
	uint32 dh = dst_dib.GetHeight();

	uint32 poww = sw/dw;
	uint32 powh = sh/dh;
	check( poww == powh );
	check( (dw*poww) == sw );
	check( (dh*powh) == sh );
	if (poww < 2) return;

	uint64 *buff = new uint64[dw];

	uint32 ddd = poww*poww;
	for (uint32 yy=0; yy<dh; ++yy)
	{
		 uint32* dst = (uint32*)dst_dib.GetLine(yy);

		memset(buff,0,dw * sizeof(uint64));

		for( uint32 m = 0; m < poww; m++ ) 
		{
			const uint32 *src = (uint32*)src_dib.GetLine(yy*poww + m);

			SummRow(buff,src,poww,dw);
			
		}
		//memcpy(dst,src,4*newsize.w);
//		UnpackRow( dst, buff, newsize.w );

		uint16* usrc = (uint16*)buff;
		uint8*  udst = (uint8*)dst;
		for( uint32 z = 0; z < 4 * dw; z++ ) {
			*udst++ = uint8( *usrc++ / ddd );
		}

	}

	__asm emms;
}


//
// Ultrafast Stretch
//
void ResizeImage (iDib& dst, const iDib& src)
{
    uint32			y;
    uint32			mx;
    uint32			my;
    uint32			InRow;
    uint32			OutRow;
    uint32			DestWidth;
    uint32			DestHeight;
    const iColor*	InPtr;
    iColor*			OutPtr;

    // figure out "slopes" for both horizontal and vertical, using fixed-point math
    // the slop is the x/y increment for the original image. ie, increment the x value
    // every 2 pixels for every 1 pixel being written to the Dest (that would be
    // if half-sizing Source into Dest)
    mx = (src.GetWidth() << 12) / dst.GetWidth();
    my = (src.GetHeight() << 12) / dst.GetHeight();

    OutRow = 0;
    DestWidth = dst.GetWidth ();
    DestHeight = dst.GetHeight ();
    InRow = 0;

    // P3/P4/C2/Athlon/Duron version. 
    // Reads/writes around the cache as much as possible.
    if (gCPUFeatures.x86_SSE  ||  gCPUFeatures.x86_3DNowExt) { 
        for (y = 0; y < DestHeight; y++, InRow += my) {
            InPtr  = src.GetLine(InRow >> 12);
            OutPtr = dst.GetLine(OutRow);

            __asm
            {
                push ebx                   // Have to save EBX otherwise VC++ will complain.

                mov esi, InPtr
                mov edi, OutPtr
                xor eax, eax
                mov ecx, mx
                mov edx, DestWidth
                shr edx, 3
                cmp edx, 0

                jz SSEPtSampleLoop2Setup

            SSEPtSampleLoop1:

                // 1
                mov ebx, eax                  // 1 a
                shr ebx, 12                   // 1 a 
                movd mm0, [esi + ebx * 4]     // 1 a 
                prefetchnta [esi + ebx * 4 + 128]  // This was just a guess but it seems to work really well!
                add eax, ecx                  // 1 b

                // 2
                mov ebx, eax                  // 1 b
                shr ebx, 12                   // 1 b
                movd mm1, [esi + ebx * 4]     // 1 c
                add eax, ecx                  // 1 c

                // Combine 1 + 2 in to mm0
                psllq mm1, 32                 // 1 c
                por mm0, mm1                  // 1 d

                // 3
                mov ebx, eax                  // 1 d
                shr ebx, 12                   // 1 d
                movd mm2, [esi + ebx * 4]     // 1 e
                add eax, ecx                  // 1 e

                // 4
                mov ebx, eax                  // 1 e
                shr ebx, 12                   // 1 f
                movd mm3, [esi + ebx * 4]     // 1 f
                add eax, ecx                  // 1 f

                // Combine 3 + 4 in to mm2
                psllq mm3, 32                 // 1 g
                por mm2, mm3                  // 1 g

                // 5
                mov ebx, eax
                shr ebx, 12
                movd mm4, [esi + ebx * 4]
                add eax, ecx

                // 6
                mov ebx, eax
                shr ebx, 12
                movd mm5, [esi + ebx * 4]
                add eax, ecx

                // Combine 5 + 6 in to mm4
                psllq mm5, 32
                por mm4, mm5

                // 7
                mov ebx, eax
                shr ebx, 12
                movd mm6, [esi + ebx * 4]
                add eax, ecx

                // 8
                mov ebx, eax
                shr ebx, 12
                movd mm7, [esi + ebx * 4]
                add eax, ecx

                // Combine 7 + 8 in to mm6
                psllq mm7, 32
                por mm6, mm7

                // Store
                movntq [edi], mm0             // 2 h
                movntq [edi + 8], mm2         // 2 i
                movntq [edi + 16], mm4
                movntq [edi + 24], mm6

                // loop
                add edi, 32                   // 1 a
                dec edx                       // 1 h
                jnz SSEPtSampleLoop1          // 1 i

            SSEPtSampleLoop2Setup:
                mov edx, DestWidth
                and edx, 7
                cmp edx, 0
                jz SSEPtSampleEnd

            SSEPtSampleLoop2:

                // Cleanup
                mov ebx, eax
                shr ebx, 12                // shr by 12 for pixel count, shl by 2 for byte count
                mov ebx, [esi + ebx * 4]
                mov [edi], ebx
                add eax, ecx

                add edi, 4
                dec edx
                cmp edx, 0
                jnz SSEPtSampleLoop2

            SSEPtSampleEnd:

                pop ebx
            }

            OutRow++;
        }

        _mm_empty ();
    } else if (gCPUFeatures.x86_MMX) {
	    // Pentium MMX/P2/Celeron version. Removes cache management (prefetch/movntq)
        for (y = 0; y < DestHeight; y++, InRow += my) {
            InPtr  = src.GetLine(InRow >> 12);
            OutPtr = dst.GetLine(OutRow);
            __asm
            {
                push ebx                   // Have to save EBX otherwise VC++ will complain.

                mov esi, InPtr
                mov edi, OutPtr
                xor eax, eax
                mov ecx, mx
                mov edx, DestWidth
                shr edx, 3
                cmp edx, 0

                jz MMXPtSampleLoop2Setup

            MMXPtSampleLoop1:
                // 1
                mov ebx, eax                  
                shr ebx, 12                   
                movd mm0, [esi + ebx * 4]     
                add eax, ecx                  

                // 2
                mov ebx, eax                  
                shr ebx, 12                   
                movd mm1, [esi + ebx * 4]     
                add eax, ecx                  

                // Combine 1 + 2
                psllq mm1, 32                 
                por mm0, mm1                  

                // 3
                mov ebx, eax                  
                shr ebx, 12                   
                movd mm2, [esi + ebx * 4]     
                add eax, ecx                  

                // 4
                mov ebx, eax                  
                shr ebx, 12                   
                movd mm3, [esi + ebx * 4]     
                add eax, ecx                  

                // Combine 3 + 4 
                psllq mm3, 32                 
                por mm2, mm3                  

                // 5
                mov ebx, eax
                shr ebx, 12
                movd mm4, [esi + ebx * 4]
                add eax, ecx

                // 6
                mov ebx, eax
                shr ebx, 12
                movd mm5, [esi + ebx * 4]
                add eax, ecx

                // Combine 5 + 6
                psllq mm5, 32
                por mm4, mm5

                // 7
                mov ebx, eax
                shr ebx, 12
                movd mm6, [esi + ebx * 4]
                add eax, ecx

                // 8
                mov ebx, eax
                shr ebx, 12
                movd mm7, [esi + ebx * 4]
                add eax, ecx

                // Combine 7 + 8
                psllq mm7, 32
                por mm6, mm7

                // Store
                movq [edi], mm0            
                movq [edi + 8], mm2        
                movq [edi + 16], mm4
                movq [edi + 24], mm6

                // loop
                add edi, 32                
                dec edx                    
                jnz MMXPtSampleLoop1       

            MMXPtSampleLoop2Setup:
                mov edx, DestWidth
                and edx, 7
                cmp edx, 0
                jz MMXPtSampleEnd

            MMXPtSampleLoop2:

                // Cleanup
                mov ebx, eax
                shr ebx, 12                
                mov ebx, [esi + ebx * 4]
                mov [edi], ebx
                add eax, ecx

                add edi, 4
                dec edx
                cmp edx, 0
                jnz MMXPtSampleLoop2

            MMXPtSampleEnd:

                pop ebx
            }

            OutRow++;
        }

        _mm_empty ();
    } else {   
		// Scalar version.
        for (y = 0; y < DestHeight; y++, InRow += my) {
            InPtr  = src.GetLine(InRow >> 12);
            OutPtr = dst.GetLine(OutRow);

            /*
                esi = In
                edi = Out
                eax = InColumn
                ebx = InColumn >> 12, then In[ebx]
                ecx = mx
                edx = DestWidth >> 2  (loop counter)
            */

        #ifdef GABX_VCASM
            __asm
            {
                push ebx                   // Have to save EBX otherwise VC++ will complain.

                mov esi, InPtr
                mov edi, OutPtr
                xor eax, eax
                mov ecx, mx
                mov edx, DestWidth
                shr edx, 2
                cmp edx, 0

                jz PtSampleLoop2Setup

            PtSampleLoop1:

                // 1
                mov ebx, eax
                shr ebx, 12                // shr by 12 for pixel count
                mov ebx, [esi + ebx * 4]
                mov [edi], ebx
                add eax, ecx

                // 2
                mov ebx, eax
                shr ebx, 12                // shr by 12 for pixel count
                mov ebx, [esi + ebx * 4]
                mov [edi + 4], ebx
                add eax, ecx

                // 3
                mov ebx, eax
                shr ebx, 12                // shr by 12 for pixel count
                mov ebx, [esi + ebx * 4]
                mov [edi + 8], ebx
                add eax, ecx

                // 4
                mov ebx, eax
                shr ebx, 12                // shr by 12 for pixel count
                mov ebx, [esi + ebx * 4]
                mov [edi + 12], ebx
                add eax, ecx

                // loop
                add edi, 16
                dec edx
                cmp edx, 0
                jnz PtSampleLoop1

            PtSampleLoop2Setup:
                mov edx, DestWidth
                and edx, 3
                cmp edx, 0
                jz PtSampleEnd

            PtSampleLoop2:

                // Cleanup
                mov ebx, eax
                shr ebx, 12                // shr by 12 for pixel count
                mov ebx, [esi + ebx * 4]
                mov [edi], ebx
                add eax, ecx

                add edi, 4
                dec edx
                cmp edx, 0
                jnz PtSampleLoop2

            PtSampleEnd:
                pop ebx
            }
        #else  
            // No assembly? Use C code

            // unroll dat loop
            uint32 InColumn = 0;
            uint32 x;
            for (x = 0; x < (DestWidth >> 2); x++)
            {
                OutPtr->rgba = InPtr[InColumn >> 12].rgba;
                OutPtr++;
                InColumn += mx;

                OutPtr->rgba = InPtr[InColumn >> 12].rgba;
                OutPtr++;
                InColumn += mx;

                OutPtr->rgba = InPtr[InColumn >> 12].rgba;
                OutPtr++;
                InColumn += mx;

                OutPtr->rgba = InPtr[InColumn >> 12].rgba;
                OutPtr++;
                InColumn += mx;
            }

            for (x = 0; x < (DestWidth & 3); x++)
            {
                OutPtr->rgba = InPtr[InColumn >> 12].rgba;
                OutPtr++;
                InColumn += mx;
            }

        #endif // GABX_VCASM

            OutRow++;
        }
    }
}

void SmartPyrStretch(iDib &dst_dib, const iDib &src_dib)
{
	uint32 sw = src_dib.GetWidth();
	uint32 sh = src_dib.GetHeight();
	uint32 dw = dst_dib.GetWidth();
	uint32 dh = dst_dib.GetHeight();

	check(sw > dw);
	check(sh > dh);

	iDib tmp_dib(iSize(dw*4,dh*4));
	ResizeImage(tmp_dib,src_dib);
	PyramidalStretch(dst_dib,tmp_dib);
}

/*
 *	Abstrace Scale Filter class
 */
class iScaleFilter
{
public:
	virtual float GetSupport() const = 0;
	virtual float Apply( float t ) const = 0;
};
/*
 *	Triangle ( mag )	
 */
class TriangleFilter : public iScaleFilter
{
public:
	inline float GetSupport() const
	{
		return 1.0f;
	}

	inline float Apply( float t ) const
	{
		if ( t < 0 ) t = -t;
		if ( t < 1.0f ) return ( 1.0f - t );
		return 0.0f;
	}
};

/*
 *	Bell ( Min )	
 */
class BellFilter : public iScaleFilter
{
public:
	inline float GetSupport() const
	{
		return 1.5f;
	}
	
	inline float Apply( float t ) const
	{
		if ( t < 0 ) t = -t;
		if ( t < .5f ) return (.75f - ( t * t ) );
		if ( t < 1.5f ) {
			t = t - 1.5f;
			return ( .5f * ( t * t ));
		}
		return 0.0f;
	}
};

/*
 *	B-spline ( mag )	
 */
class BSplineFilter : public iScaleFilter
{
public:
	inline float GetSupport() const
	{
		return 2.0f;
	}
	
	inline float Apply( float t ) const
	{
		float tt;
		if ( t < 0 ) t = -t;
		if ( t < 1.0f ) {
			tt = t * t;	
			return ((.5f * tt * t) - tt + ( 2.0f / 3.0f) );
		} else if ( t < 2.0f ) {
			t = 2 - t;
			return ((1.0f / 6.0f) * ( t * t * t ));
		}
		return 0.0f;
	}
};

/*
 *	Lanczos3 ( min )	
 */
class Lanczos3Filter : public iScaleFilter
{
public:
	inline float GetSupport() const
	{
		return 3.0f;
	}

	inline float Apply( float t ) const
	{
		if ( t < 0 ) t = -t;
		if ( t < 3.0f ) {
			t *= PI;
			if( t == 0 ) return 1.0f;
			float t3 = t / 3.0f;
			return ( (sinf( t ) / t) * (sinf( t3 ) / t3) );
		}
		return 0.0f;
	}
};

/*
 *	
 */

struct WPixel {
	sint32	pixel;
	float	weight;

	inline WPixel( sint32 p, float w ) : pixel( p ), weight( w ) {}
};

typedef iSimpleArray< WPixel > WeightsArray;

/*
 *	
 */

//
template< typename Filter >
bool CalcXWeights( WeightsArray& weights, float xScale, sint32 dstWidth, sint32 srcWidth, Filter filter, sint32 column )
{
	float width;
	float fscale;
	float center, left, right;
	float weight;
	sint32 j, n;

	float fWidth = filter.GetSupport();
	if ( xScale < 1.0f ) {
		// Shrink
		width = fWidth / xScale;
		fscale = 1.0f / xScale;

		//weights.Preallocate( (sint32)(width * 2 + 1) );

		center	= (float)column / xScale;
		left	= ceilf( center - width );
		right	= floorf( center + width );
		float ifScale = 1.0f / fscale;
		for ( j = (sint32)left; j <= right; ++j ) {
			weight = center - (float)j;
			weight = filter.Apply( weight * ifScale ) * ifScale;
			if ( j < 0 ) 
				n = -j;
			else if ( j >= srcWidth )
				n = (srcWidth - j) + srcWidth - 1;
			else
				n = j;
			weights.Add( WPixel( n, weight ) );
		}
	} else {
		// Expand
		//weights.Preallocate( (sint32)(fWidth * 2 + 1) );

		center	= (float)column	/ xScale;
		left	= ceilf( center - fWidth );
		right	= floorf( center + fWidth );

		for ( j = (sint32)left; j <= right; ++j ) {
			weight = center - (float)j;
			weight = filter.Apply( weight );
			if ( j < 0 ) 
				n = -j;
			else if ( j >= srcWidth ) 
				n = (srcWidth -j) + srcWidth - 1;
			else 
				n = j;
			weights.Add( WPixel( n, weight ) );
			
		}
	}
	return true;
}

//
__forceinline void ApplyWeight( i4DVector& weight, iColor clr, float w )
{
	weight.w += clr.r * w;
	weight.x += clr.g * w;
	weight.y += clr.b * w;
	weight.z += clr.a * w;
}

//
__forceinline sint32 FloatToIntChop(float v) 
{ 
	const sint32& u = *(const sint32*)&v; 
	sint32 s = u >> 31; 
	sint32 e = ((u & 0x7fffffff) >> 23) - 127; 
	return (((((uint32)((u & ((1<<23) - 1)) | (1<<23)) << 8) >> (31-e)) ^ (s)) - s ) &~ (e>>31); 
}

//
__forceinline iColor PackPixel( const i4DVector& weight )
{
	return iColor( 
		(uint8)iCLAMP<sint32>(0, 255, FloatToIntChop(weight.w)),
		(uint8)iCLAMP<sint32>(0, 255, FloatToIntChop(weight.x)),
		(uint8)iCLAMP<sint32>(0, 255, FloatToIntChop(weight.y)),
		(uint8)iCLAMP<sint32>(0, 255, FloatToIntChop(weight.z)));
}

//
template <typename Filter>
bool Zoom( iDib &dst, const iDib& src )
{
	Filter filter;
	iSimpleArray<iColor> tempPixels;

	float xScale, yScale;
	sint32 xx;
	sint32 i, j, k, n;
	float center, left, right, weight;
	float width, fscale;
	iColor pel, pel2;
	bool pelDelta;
	i4DVector pWeight;

	WeightsArray weightsX;
	WeightsArray* weightsYlist;

	// create intermediate holder
	tempPixels.SetSize( src.GetHeight() );

	xScale = (float)dst.GetWidth() / (float)src.GetWidth();

	// Build weights
	yScale = (float)dst.GetHeight() / (float)src.GetHeight();

	if (xScale == 1.0f && yScale == 1.0f){
		dst = src;
		return true;
	}

	weightsYlist = new WeightsArray[ dst.GetHeight() ];

	if ( yScale < 1.0f ) {
		// Shrink
		width = filter.GetSupport() / yScale;
		fscale = 1.0f / yScale;

		for ( i = 0; i < (sint32)dst.GetHeight(); ++i ) {
			//weightsYlist[i].Preallocate( (INT)(width * 2 + 1 ) );
			//weightsYlist[i].SetSize((sint32)(width * 2 + 1 ) );

			center	= (float)i / yScale;
			left	= ceilf( center - width );
			right	= floorf( center + width );

			float ifScale = 1.0f / fscale;
			for( j = (sint32)left; j <= right; ++j ){
				weight = center - (float)j;
				weight = filter.Apply( weight * ifScale ) * ifScale;
				if ( j < 0 ) 
					n = -j;
				else if ( j >= (sint32)src.GetHeight())
					n = (src.GetHeight() - j) + src.GetHeight() - 1;
				else
					n = j;

				weightsYlist[i].Add( WPixel( n, weight ) );
			}
		}
	} else {
		// Expand
		for ( i = 0; i < (sint32)dst.GetHeight();++i ) {
			//weightsYlist[i].Preallocate( (INT)(fwidth * 2 + 1) );
			//weightsYlist[i].SetSize((sint32)((filter.GetSupport() / yScale) * 2 + 1 ) );

			center	= (float) i / yScale;
			left	= ceilf( center - filter.GetSupport() );
			right	= floorf( center + filter.GetSupport() );

			for( j = (sint32)left; j <= right; ++j ) {
				weight	= center - (float)j;
				weight	= filter.Apply( weight );
				if ( j < 0 )
					n = -j;
				else if ( j >= (sint32)src.GetHeight() ) 
					n = (src.GetHeight() - j) + src.GetHeight() - 1;
				else
					n = j;

				weightsYlist[i].Add( WPixel( n, weight ));
			}
		}
	}

	// do the work
	for( xx = 0; xx < (sint32)dst.GetWidth(); xx++ ) {
		CalcXWeights( weightsX, xScale, dst.GetWidth(), src.GetWidth(), filter, xx );

		// Apply horz filter to make dst column in tmp
		for ( k = 0; k < (sint32)src.GetHeight(); ++k ) {
			pWeight.Zero();
			pelDelta = false;
			const iColor* pSrcLine = src.GetLine(k);
			//pel = src.GetPixel(iPoint( weightsX[0].pixel, k ));
			pel = pSrcLine[weightsX[0].pixel];
			for ( j = 0; j < (sint32)weightsX.GetSize(); ++j ) {
				pel2 = pSrcLine[weightsX[j].pixel];
				//src.GetPixel(iPoint( weightsX[j].pixel, k ));
				if ( pel2 != pel )	pelDelta = true;

				ApplyWeight( pWeight, pel2, weightsX[j].weight );
			}
			if ( pelDelta ) {
				tempPixels[k] = PackPixel( pWeight );
			} else {
				tempPixels[k] = pel;
			}
		}
		// Free data
		weightsX.RemoveAll();

		// Temp column has been build, stretch it vertically
		for( i = 0; i < (sint32)dst.GetHeight(); ++i ) {
			pWeight.Zero();
			pelDelta = false;
			pel = tempPixels[ weightsYlist[i][0].pixel ];
			iColor* pDstLine = dst.GetLine(i);

			WeightsArray& lineWeights = weightsYlist[i];
			for( j = 0; j < (sint32)weightsYlist[i].GetSize(); ++j ) {
				pel2 = tempPixels[ weightsYlist[i][j].pixel ];
				if ( pel != pel2 ) pelDelta = true;

				ApplyWeight( pWeight, pel2, lineWeights[j].weight );
			}

			if ( pelDelta ) {
				pDstLine[xx] = PackPixel( pWeight );
			} else {
				pDstLine[xx] = pel;
			}
		}
	}

	// Clenup
	tempPixels.RemoveAll();
	delete[] weightsYlist;

	return true;
}

//
//
//
void iDibTransformer::Stretch(const iDib& src, iDib& dst, Quality q)
{
	uint32 sw = src.GetWidth();
	uint32 sh = src.GetHeight();
	uint32 dw = dst.GetWidth();
	uint32 dh = dst.GetHeight();

	check(sw && sh && dw && dh);

	bool bZoom = ( sw * sh ) < ( dw * dh );
	
	if (q == Fake) {
		ResizeImage(dst,src);
	} else if (q == Fast) {
		if (bZoom) Zoom<TriangleFilter>(dst, src);
		else Zoom<BellFilter>(dst, src);
	} else if (q == Normal) {
		if (bZoom) Zoom<BSplineFilter>(dst, src);
		else Zoom<BellFilter>(dst, src);
	} else if (q == High) {
		if (bZoom) Zoom<BSplineFilter>(dst, src);
		else Zoom<Lanczos3Filter>(dst, src);
	} else if (q == Pyramidal) {
		PyramidalStretch(dst,src);
	} else if (q == SmartPyr) {
		SmartPyrStretch(dst,src);
	} else {
		check(0);
	}
}
