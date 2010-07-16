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
#include "xe.dib.llproc.h"

void FillBlockOpaqueStd  (uint32 *dst, const uint32 src, uint32 size, void *lpParam);
void FillBlockOpaqueInit  (uint32 *dst, const uint32 src, uint32 size, void *lpParam);

void FillBlockAlphaStd  (uint32 *dst, const uint32 src, uint32 size, void *lpParam);
void FillBlockAlphaMMX  (uint32 *dst, const uint32 src, uint32 size, void *lpParam);
void FillBlockAlphaSSE  (uint32 *dst, const uint32 src, uint32 size, void *lpParam);
void FillBlockAlphaInit  (uint32 *dst, const uint32 src, uint32 size, void *lpParam);

void CopyBlockOpaqueStd  (uint32 *dst, const uint32 *src, uint32 size, void *lpParam);
void CopyBlockOpaqueInit (uint32 *dst, const uint32 *src, uint32 size, void *lpParam);

void BlitBlockAlphaStd  (uint32* dst, const uint32* src, uint32 size, void *lpParam);
void BlitBlockAlphaMMX  (uint32* dst, const uint32* src, uint32 size, void *lpParam);
void BlitBlockAlphaSSE  (uint32* dst, const uint32* src, uint32 size, void *lpParam);
void BlitBlockAlphaInit (uint32* dst, const uint32* src, uint32 size, void *lpParam);

void BlitBlockAlphaKeyStd  (uint32* dst, const uint32* src, uint32 size, void *lpParam);
void BlitBlockAlphaKeyInit (uint32* dst, const uint32* src, uint32 size, void *lpParam);

void BlitBlockCKStd ( uint32 *dst, const uint32 *src, uint32 size, void *lpParam);
void BlitBlockCKInit ( uint32 *dst, const uint32 *src, uint32 size, void *lpParam);


void (*FillBlockOpaque)  (uint32 *dst, const uint32 src, uint32 size, void *lpParam)=FillBlockOpaqueInit;
void (*FillBlockAlpha)  (uint32 *dst, const uint32 src, uint32 size, void *lpParam)=FillBlockAlphaInit;
void (*CopyBlockOpaque)  (uint32* dst, const uint32* src, uint32 size, void *lpParam) = CopyBlockOpaqueInit;
void (*BlitBlockAlpha)  (uint32* dst, const uint32* src, uint32 size, void *lpParam) = BlitBlockAlphaInit;
void (*BlitBlockAlphaKey)  (uint32* dst, const uint32* src, uint32 size, void *lpParam) = BlitBlockAlphaKeyInit;
void (*BlitBlockCK)  ( uint32* dst, const uint32* src, uint32 size, void *lpParam)=BlitBlockCKInit;

//
// Common defines und f-ns
//
static __declspec(align(8)) uint16 alphaTable[4*257];
static __m64* bias = (__m64*)(alphaTable) + 128;

void InitAT()
{
	for( uint32 i = 0; i < 257; i++ ) 
	{
		alphaTable[i*4+0] = (uint16)i;
		alphaTable[i*4+1] = (uint16)i;
		alphaTable[i*4+2] = (uint16)i;
		alphaTable[i*4+3] = (uint16)i;
	}
}


//
// Opaque Block Filling
//
void FillBlockOpaqueInit (uint32 *dst, const uint32 src, uint32 size, void *lpParam)
{
    FillBlockOpaque = FillBlockOpaqueStd;
	FillBlockOpaque(dst,src,size,lpParam);
}

void FillBlockOpaqueStd (uint32 *dst, const uint32 src, uint32 siz, void *lpParam)
{
	FillBlock32(dst,src,siz);
}


//
// Alpha Block Filling
//
void FillBlockAlphaInit (uint32 *dst, const uint32 src, uint32 size, void *lpParam)
{
	if (!gIsKernelLibraryInited)
	{
        FillBlockAlphaStd (dst, src, size, lpParam);
        return;
    }

    FillBlockAlpha = FillBlockAlphaStd;

    if (gCPUFeatures.x86_MMX)
        FillBlockAlpha = FillBlockAlphaMMX;

    if (gCPUFeatures.x86_SSE  ||  gCPUFeatures.x86_3DNowExt)
        FillBlockAlpha = FillBlockAlphaSSE;

    FillBlockAlpha (dst, src, size, lpParam);
}

void FillBlockAlphaStd (uint32 *dst, const uint32 src, uint32 siz, void *lpParam)
{
	// FAKE!!!
}

void FillBlockAlphaMMX (uint32 *dst, const uint32 src, uint32 siz, void *lpParam)
{
	__asm 
	{
		mov	ebx, src
		mov edi, dst
		mov ecx, [siz]
		mov edx, [bias]

		// pfetch first
		mov al, [edi]
		mov al, [edi + ecx*4 - 1]

loop1:
		movd mm0, ebx
		pxor mm3, mm3

		movd mm2, [edi]
		punpcklbw mm0, mm3

		punpcklbw mm2, mm3
		movq mm1, mm0
		punpckhwd mm1, mm1
		psubw mm0, mm2
		punpckhdq mm1, mm1
		psllw mm2, 8
		pmullw mm0, mm1
		paddw mm2,[edx]
		paddw mm2, mm0
		psrlw mm2, 8
		packuswb mm2, mm3
		movd [edi], mm2

		add edi, 4
		sub ecx, 1

		jnz loop1
		emms
	};
}

void FillBlockAlphaSSE (uint32 *dst, const uint32 src, uint32 siz, void *lpParam)
{
	__asm {
		mov	ebx, src
		mov edi, dst
		mov ecx, [siz]
		mov edx, [bias]

		// pfetch first
		mov al, [edi]
		mov al, [edi + ecx*4 - 1]

loop1:

		prefetchnta [edi+8]

		movd mm0, ebx
		pxor mm3, mm3

		movd mm2, [edi]
		punpcklbw mm0, mm3

		punpcklbw mm2, mm3
		movq mm1, mm0
		punpckhwd mm1, mm1
		psubw mm0, mm2
		punpckhdq mm1, mm1
		psllw mm2, 8
		pmullw mm0, mm1
		paddw mm2,[edx]
		paddw mm2, mm0
		psrlw mm2, 8
		packuswb mm2, mm3
		movd [edi], mm2

		add edi, 4
		sub ecx, 1


		jnz loop1
		emms
	};
}

//
// 32bit Block Copying
//
void CopyBlockOpaqueInit (uint32* dst, const uint32* src, uint32 size, void *lpParam)
{
    CopyBlockOpaque = CopyBlockOpaqueStd;
    CopyBlockOpaque (dst, src, size, lpParam);
}

void CopyBlockOpaqueStd (uint32* dst, const uint32* src, uint32 size, void *lpParam)
{
    CopyBlock32(dst,src,size);
}

//
// 32 bit Block Blending by Alpha
//

__inline void BlitBlockAlphaInit (uint32* dst, const uint32* src, uint32 size, void *lpParam)
{
	if (!gIsKernelLibraryInited)
	{
        BlitBlockAlphaStd (dst, src, size, lpParam);
        return;
    }

    BlitBlockAlpha = BlitBlockAlphaStd;

    if (gCPUFeatures.x86_MMX)
        BlitBlockAlpha = BlitBlockAlphaMMX;

    BlitBlockAlpha (dst, src, size, lpParam);
}

void BlitBlockAlphaStd  (uint32* dst, const uint32* src, uint32 size, void *lpParam)
{
/*
	for (uint32 yy=0; yy<m_Width * m_Height; yy++)
	{

//		for (uint32 xx=0; xx<m_Width; xx++)
//		{
			//dst_clr->BlendByAlpha(*src_clr);
			dst_clr->r=_sint32((src_clr->r*src_clr->a) + _sint32(dst_clr->r*(255-src_clr->a)))/256;
			dst_clr->g=_sint32((src_clr->g*src_clr->a) + _sint32(dst_clr->g*(255-src_clr->a)))/256;
			dst_clr->b=_sint32((src_clr->b*src_clr->a) + _sint32(dst_clr->b*(255-src_clr->a)))/256;
			dst_clr->a=255;
			src_clr++;
			dst_clr++;

//		}
	}
	*/
}

void BlitBlockAlphaMMX  (uint32* dst, const uint32* src, uint32 siz, void *lpParam)
{
	const uint64 blendmask = 0x00ff000000000000;
	__asm {
		mov	esi, src
		mov edi, dst
		mov ecx, [siz]
		mov edx, [bias]

		// pfetch first
		mov al, [esi]
		mov al, [edi]
		mov al, [esi + ecx*4 - 1]
		mov al, [edi + ecx*4 - 1]
		movq mm4, blendmask

loop1:

		movd mm0, [esi]			// mm0 = src.000argb
		pxor mm3, mm3			// mm3 = 0000

		movd mm2, [edi]			// mm2 = dst.0000argb
		punpcklbw mm0, mm3		// mm0 = src.0a0r0g0b

		punpcklbw mm2, mm3		// mm2 = dst.0a0r0g0b
		movq mm1, mm0			// mm1 = src.0a0r0g0b
		por mm0, blendmask
		punpckhwd mm1, mm1		// mm1 = src.0a0r0a0r
		psubw mm0, mm2			// mm0 = mm0.(0a)(0r)(0g)(0b) - mm2.(0a)(0r)(0g)(0b)
		punpckhdq mm1, mm1		// mm1 = src.0a0a0a0a
		psllw mm2, 8			// mm2 = dst.a0r0g0b0
		pmullw mm0, mm1			// mm0 = mm0 w* m1
		paddw mm2,[edx]			// mm2 += bias
		paddw mm2, mm0			// mm2 == mm0
		psrlw mm2, 8			// mm2 = res.a0r0g0b0
		packuswb mm2, mm3		// mm2 = res.0000argb
		movd [edi], mm2			//

		add esi, 4
		add edi, 4
		sub ecx, 1


		jnz loop1
		emms
	};
}

void BlitBlockAlphaSSE  (uint32* dst, const uint32* src, uint32 siz, void *lpParam)
{
	check(0);
/*
	const uint64 blendmask = 0x00ff000000000000;
	__asm {
		mov	esi, src
		mov edi, dst
		mov ecx, [siz]
		mov edx, [bias]

		// pfetch first
		mov al, [esi]
		mov al, [edi]
		mov al, [esi + ecx*4 - 1]
		mov al, [edi + ecx*4 - 1]
		movq mm4, blendmask

loop1:

	    prefetchnta [esi+8]
		prefetchnta [edi+8]

		movd mm0, [esi]			// mm0 = src.000argb
		pxor mm3, mm3			// mm3 = 0000

		movd mm2, [edi]			// mm2 = dst.0000argb
		punpcklbw mm0, mm3		// mm0 = src.0a0r0g0b

		punpcklbw mm2, mm3		// mm2 = dst.0a0r0g0b
		movq mm1, mm0			// mm1 = src.0a0r0g0b
		por mm0, blendmask
		punpckhwd mm1, mm1		// mm1 = src.0a0r0a0r
		psubw mm0, mm2			// mm0 = mm0.(0a)(0r)(0g)(0b) - mm2.(0a)(0r)(0g)(0b)
		punpckhdq mm1, mm1		// mm1 = src.0a0a0a0a
		psllw mm2, 8			// mm2 = dst.a0r0g0b0
		pmullw mm0, mm1			// mm0 = mm0 w* m1
		paddw mm2,[edx]			// mm2 += bias
		paddw mm2, mm0			// mm2 == mm0
		psrlw mm2, 8			// mm2 = res.a0r0g0b0
		packuswb mm2, mm3		// mm2 = res.0000argb
		movd [edi], mm2			//

		add esi, 4
		add edi, 4
		sub ecx, 1


		jnz loop1
		emms
	};
	*/
}

//
// 32 bit Block Bliting by Alpha Key
//

__inline void BlitBlockAlphaKeyInit (uint32* dst, const uint32* src, uint32 size, void *lpParam)
{

    BlitBlockAlphaKey = BlitBlockAlphaKeyStd;
    BlitBlockAlphaKey (dst, src, size, lpParam);
}

void BlitBlockAlphaKeyStd(uint32* dst, const uint32* src, uint32 siz, void *lpParam)
{
	__asm {
		mov	esi, src
		mov edi, dst
		mov ecx, siz

		// pfetch first
		mov al, [esi]
		mov al, [edi]
		mov al, [esi + ecx*4 - 1]
		mov al, [edi + ecx*4 - 1]

loop1:
		mov eax, [esi]
		mov ebx, eax
		and eax, 0xff000000
		jz skip
		mov [edi], ebx

skip:
		add esi, 4
		add edi, 4
		sub ecx, 1

		jnz loop1
	};
}


//
// 32 bit Block Blending by ColorKey
//
void BlitBlockCKInit (uint32* dst, const uint32* src, uint32 size, void *lpParam)
{
	BlitBlockCK=BlitBlockCKStd;
	BlitBlockCK(dst,src,size,lpParam);
}

void BlitBlockCKStd (uint32* dst, const uint32* src, uint32 siz, void *lpParam)
{
	uint32 cK = *((uint32*)lpParam);
	__asm
	{
		MOV EDI, dst
		MOV ESI, src
		MOV ECX, siz
		MOV EDX, cK
		
		JMP l3
l1:
		ADD EDI, 4
		ADD ESI, 4
l2:
		DEC ECX
l3:
		CMP ECX, 0
		JE l4

		MOV EAX, [ESI]

		CMP EAX, EDX
		JE l1

		MOVSD

		JMP l2
l4:
	}
}
