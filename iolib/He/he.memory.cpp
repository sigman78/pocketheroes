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

/*************************************************************************
 *
 * Name: He.Memory.h
 * Desc: Basic memory operations implementation
 * Auth: Tutankhamen (Robert Tarasov)
 * Date: 2003/01/11
 *
 *************************************************************************/
#include "he.std.h"

void FillBlock8Std  (uint8 *dst, const uint8 src, uint32 size);
void FillBlock8Init  (uint8 *dst, const uint8 src, uint32 size);

void FillBlock32Std  (uint32 *dst, const uint32 src, uint32 size);
void FillBlock32MMX  (uint32 *dst, const uint32 src, uint32 size);
void FillBlock32Init  (uint32 *dst, const uint32 src, uint32 size);

void CopyBlock8Std  (uint8 *dst, const uint8 *src, uint32 size);
void CopyBlock8Init (uint8 *dst, const uint8 *src, uint32 size);

void CopyBlock32Std  (uint32 *dst, const uint32 *src, uint32 size);
void CopyBlock32MMX  (uint32 *dst, const uint32 *src, uint32 size);
void CopyBlock32SSE  (uint32 *dst, const uint32 *src, uint32 size);
void CopyBlock32Init (uint32 *dst, const uint32 *src, uint32 size);

void MoveBlock8Std  (uint8 *dst, const uint8 *src, uint32 size);
void MoveBlock8Init (uint8 *dst, const uint8 *src, uint32 size);


void (*FillBlock8)  (uint8 *dst, const uint8 src, uint32 size)=FillBlock8Init;
void (*FillBlock32)  (uint32 *dst, const uint32 src, uint32 size)=FillBlock32Init;
void (*CopyBlock8)  (uint8* dst, const uint8* src, uint32 size) = CopyBlock8Init;
void (*CopyBlock32)  (uint32* dst, const uint32* src, uint32 size) = CopyBlock32Init;
void (*MoveBlock8)  (uint8* dst, const uint8* src, uint32 size) = MoveBlock8Init;


//
// 8bit Block Filling
//
void FillBlock8Init (uint8 *dst, const uint8 src, uint32 size)
{
    FillBlock8 = FillBlock8Std;
	FillBlock8(dst,src,size);
}

void FillBlock8Std (uint8 *dst, const uint8 src, uint32 size)
{
	memset(dst,src,size);
}

//
// 32bit Block Filling
//
void FillBlock32Init (uint32 *dst, const uint32 src, uint32 size)
{
	if (!gIsKernelLibraryInited)
	{
		check(0);
        FillBlock32Std (dst, src, size);
        return;
	}

    FillBlock32 = FillBlock32Std;

    if (gCPUFeatures.x86_MMX)
      FillBlock32 = FillBlock32MMX;

	FillBlock32(dst,src,size);
}

void FillBlock32Std (uint32 *dst, const uint32 src, uint32 siz)
{
	__asm
	{
		MOV EDI, dst
		MOV EAX, src
		MOV ECX, siz
		REP STOSD
	}
}

void FillBlock32MMX (uint32 *dst, const uint32 src, uint32 siz)
{
	__asm
	{
		mov edi, dst
		mov eax, src
		movd mm0, eax
		movq mm1, mm0
		psllq mm1,32
		por mm0, mm1

		mov edx, siz
		mov ecx, edx
		shr ecx, 1
		test ecx,ecx
		jz w32

		w64:
		movntq [edi], mm0
		add edi,8
		sub ecx,1
		jnz w64

		w32:
		test edx, 1
		jz finish

		mov [edi], eax
finish:
		emms
	}
}

//
// 8bit Block Copying
//

void CopyBlock8Init (uint8* dst, const uint8* src, uint32 size)
{
    CopyBlock8 = CopyBlock8Std;
    CopyBlock8 (dst, src, size);
}

void CopyBlock8Std (uint8* dst, const uint8* src, uint32 size)
{
    memcpy (dst, src, size);
}

//
// 32bit Block Copying
//
void CopyBlock32Init (uint32* dst, const uint32* src, uint32 size)
{
	if (!gIsKernelLibraryInited)
	{
		check(0);
        CopyBlock32Std (dst, src, size);
        return;
	}

    CopyBlock32 = CopyBlock32Std;

    if (gCPUFeatures.x86_MMX)
      CopyBlock32 = CopyBlock32MMX;

    CopyBlock32 (dst, src, size);
}

void CopyBlock32Std (uint32* dst, const uint32* src, uint32 size)
{
    memcpy (dst, src, size*4);
}

void CopyBlock32MMX (uint32* dst, const uint32* src, uint32 size)
{
	size *= 4;
    uint32 bytes1;
    uint32 bytes2;
    uint32 bytes3;

    bytes1 = size & 0xffffffe0; // 32 bytes per loop
    bytes2 = size & 0x0000001c; // 4 bytes per loop of the last 31 bytes
    bytes3 = size & 0x00000003; // last 3 cleanup bytes

    __asm
    {
        mov esi, src
        mov edi, dst
        mov ecx, bytes1
        shr ecx, 5             // divide by 32
        cmp ecx, 0
        jz  CBMMXLoop2Setup

    CBMMXLoop1:
        movq mm0, [esi]
        movq mm1, [esi+8]
        movq mm2, [esi+16]
        movq mm3, [esi+24]

        movq [edi],    mm0
        movq [edi+8],  mm1
        movq [edi+16], mm2
        movq [edi+24], mm3

        add esi, 32
        add edi, 32

        dec ecx
        jnz CBMMXLoop1

    CBMMXLoop2Setup:
        mov ecx, bytes2
        shr ecx, 2       // go from 4 bytes to loops
        cmp ecx, 0
        jz CBMMXLoop3Setup

    CBMMXLoop2:
        mov eax, [esi]
        mov [edi], eax

        add esi, 4
        add edi, 4
        dec ecx
        jnz CBMMXLoop2

    CBMMXLoop3Setup:
        // Set up 3rd loop
        mov ecx, bytes3
        cmp ecx, 0
        jz  CBMMXEnd

    CBMMXLoop3:
        mov al, [esi]
        mov [edi], al

        inc edi
        inc esi
        dec ecx
        jnz CBMMXLoop3

    CBMMXEnd:
		emms

    }
}

void CopyBlock32SSE (uint32* dst, const uint32* src, uint32 siz)
{
	check(0);
/*
	siz *= 4;
    __asm
    {
	    mov edi, dst
	    mov esi, src
	    mov edx, siz

	    mov ah, [esi]            // touch first byte of source
	    mov ah, [esi + edx - 1]  // touch last byte of source

	    mov ecx, edx
	    shr ecx, 6
	    cmp ecx, 0
	    jz  SSEUnalignedLoop2_4a

    SSEUnalignedLoop1_64:

	    //prefetchnta [esi+64]

        // Read
        movq mm0, [esi]
        movq mm1, [esi + 8]
        movq mm2, [esi + 16]
        movq mm3, [esi + 24]

        //
	    prefetchnta [esi+96]
        //

        movq mm4, [esi + 32]
        movq mm5, [esi + 40]
        movq mm6, [esi + 48]
        movq mm7, [esi + 56]

        prefetchnta [edi+96]

        // Write
        movntq [edi], mm0
        movntq [edi + 8], mm1
        movntq [edi + 16], mm2
        movntq [edi + 24], mm3

        //
        prefetchnta [edi+64]
        //

        movntq [edi + 32], mm4
        movntq [edi + 40], mm5
        movntq [edi + 48], mm6
        movntq [edi + 56], mm7

        // Loop
        add esi, 64
        add edi, 64

        dec ecx
        jnz SSEUnalignedLoop1_64

    SSEUnalignedLoop2_4a:
	    mov ecx, edx
	    and ecx, 63
	    shr ecx, 2
	    cmp ecx, 0
	    jz SSEUnalignedLoop3_1a

    SSEUnalignedLoop2_4b:
	    mov eax, [esi]
	    mov [edi], eax

	    add esi, 4
	    add edi, 4

	    dec ecx
	    jnz SSEUnalignedLoop2_4b

    SSEUnalignedLoop3_1a:
	    mov ecx, edx
	    and ecx, 3
	    cmp ecx, 0
	    jz SSEUnalignedLoopDone

    SSEUnalignedLoop3_1b:
	    mov ah, [esi]
	    mov [edi], ah

	    inc esi
	    inc edi

	    dec ecx
	    jnz SSEUnalignedLoop3_1b

    SSEUnalignedLoopDone:
        emms
    }
*/
}

//
// 8bit Block Copying
//

void MoveBlock8Init (uint8* dst, const uint8* src, uint32 size)
{
    MoveBlock8 = MoveBlock8Std;
    MoveBlock8 (dst, src, size);
}

void MoveBlock8Std (uint8* dst, const uint8* src, uint32 size)
{
    memmove (dst, src, size);
}
