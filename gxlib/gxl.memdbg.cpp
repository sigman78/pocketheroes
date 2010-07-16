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

#include <malloc.h>
#include "gxl.inc.h"
#undef MEM_TRACK
#include "gxl.memdbg.h"
#undef malloc
#undef free

inline void inc_( mem_sample& s, size_t n )
{
	s.total		+= n;
	s.current	+= n;
	if ( s.current > s.peak ) s.peak = s.current;
}

inline void dec_( mem_sample& s, size_t n )
{
	s.current -= n;
}

//

#define ALIGN_PADDING 2
typedef struct _memoryBlockHeader {
    _memoryBlockHeader *next;	// pointer to next memory block
    _memoryBlockHeader *prev;	// pointer to previous memory block
    const char *file;			// pointer to constant string containing the name of the source file in which the block was allocated
    int		line;				// line number file where the block was allcoated
    size_t	size;				// size of the user portion of the block, in bytes
	int		blockType;			//
    size_t	align;				// force alignement of the block and to provide padding between this header and the user portion of the memory block
} MEMORY_BLOCK_HEADER;

//
//
class mem_tracker
{
public:

	static mem_tracker& inst()
	{
		static mem_tracker m;
		return m;
	}

	void*	Allocate( size_t size, BlockType bt );
	void	Free( void* ptr, BlockType bt );

	void SetSourceState(const char* file, int line);
	const mem_stat& info() const;
	void report() const;

private:

	mem_stat				stats;
	const char*				curFile;
	int						curLine;
	MEMORY_BLOCK_HEADER		memBlockList;

	mem_tracker()
	{
		memset( &stats, 0, sizeof(stats) );
		
		curFile = NULL;
		curLine = -1;

		memBlockList.next = NULL;
		memBlockList.prev = NULL;
		memBlockList.file = __FILE__;
		memBlockList.line = __LINE__;
		memBlockList.size = 0;
	}

	size_t	block_id( size_t size );
};

//
//
void*
mem_tracker::Allocate( size_t size, BlockType bt  )
{
	check(size > 0);
	sint32 totalSize = size + sizeof(MEMORY_BLOCK_HEADER);
	void* pMemblock = malloc(totalSize);

	MEMORY_BLOCK_HEADER* pHeader =  (MEMORY_BLOCK_HEADER*)pMemblock;
	pMemblock = (void *)( (unsigned char *)pHeader + sizeof(MEMORY_BLOCK_HEADER) );
	pHeader->file = curFile;
	pHeader->line = curLine;
	pHeader->blockType = bt;
	pHeader->size = size;	

	// use DEADBEEF as a flag that this is actually malloc'd memory
	pHeader->align = 0xDEADBEEF;

	pHeader->prev = &memBlockList;
	pHeader->next = memBlockList.next;
	if( pHeader->next != NULL )
		pHeader->next->prev = pHeader;
	memBlockList.next = pHeader;


	inc_( stats.totals, size );
	inc_( stats.allocs, 1 );
	inc_( stats.block[ block_id(size) ], 1 );
	
	return pMemblock;
}

void
mem_tracker::Free( void* ptr, BlockType bt  )
{
	if (!ptr) return;

	MEMORY_BLOCK_HEADER* pHeader = (MEMORY_BLOCK_HEADER*)((void*)((unsigned char*)ptr - sizeof(MEMORY_BLOCK_HEADER)));

	// verify block type
	if (pHeader->blockType != bt) {
		check("Try to free memory with invalid block type!" == 0);
	}

	// verify that this was actually malloc'd
	if(pHeader->align != 0xDEADBEEF){
		check("call to FREE on non-MALLOC'd or already FREE'd memory!" == 0);
	} else {
		// mark this as already FREE'd
	    pHeader->align = 0;
	}

	pHeader->prev->next = pHeader->next;
	if( pHeader->next != NULL ) pHeader->next->prev = pHeader->prev;

	size_t size = 0;
	dec_( stats.totals, pHeader->size );
	dec_( stats.allocs, 1 );
	dec_( stats.block[ block_id(pHeader->size) ], 1 );
	
	free(pHeader);
}


void 
mem_tracker::SetSourceState(const char* file, int line)
{
	curFile = file;
	curLine = line;
}

const mem_stat&
mem_tracker::info() const
{
	return stats;
}

void
mem_tracker::report() const
{
#ifdef MEM_TRACK
	static WCHAR buf[256];
	wsprintf( buf, L"$$$ bytes : c: %9d / p: %9d / t: %9d\n", stats.totals.current, stats.totals.peak, stats.totals.total );
	OutputDebugString( buf );
	wsprintf( buf, L"$$$ allocs: c: %9d / p: %9d / t: %9d\n", stats.allocs.current, stats.allocs.peak, stats.allocs.total );
	OutputDebugString( buf );
	// etc for all block sizes
	for( int n = MEM_TRACKER_MIN_BLOCKSIZE; n!= MEM_TRACKER_MAX_BLOCKSIZE; ++n ) {
		const mem_sample& smpl = stats.block[n-MEM_TRACKER_MIN_BLOCKSIZE];
		wsprintf( buf, L"$$$ %5d : c: %9d / p: %9d / t: %9d\n", 1 << n, smpl.current, smpl.peak, smpl.total );
		OutputDebugString( buf );
	}

	// Detailed report
#ifndef OS_WINCE
	MEMORY_BLOCK_HEADER *pHeader = memBlockList.next;
	char tbuf[256];
	if (pHeader) {
		while (pHeader) {
			sprintf( tbuf, "Memory leak [%d] in %s at line %d (%d bytes) : \n\t[ ", pHeader->blockType, pHeader->file, pHeader->line, pHeader->size);
			OutputDebugStringA(tbuf);
			unsigned char* pData = ((unsigned char*)pHeader) + sizeof(MEMORY_BLOCK_HEADER);
			for (uint32 xx=0; xx<16 && xx<pHeader->size; ++xx) {
				sprintf( tbuf, "%02X ", *pData++);
				OutputDebugStringA(tbuf);
			}
			OutputDebugStringA("]\n");
			pHeader = pHeader->next;
		}
	} else {
		OutputDebugStringA("- No memory leaks detected! -\n");
	}
#endif //OS_WINCE
#endif // MEM_TRACK
}

size_t
mem_tracker::block_id( size_t size )
{
	if ( size >= MEM_TRACKER_LARGE_SIZE ) {
		return MEM_TRACKER_LARGE_BLOCK;
	} else {
		check( size < MEM_TRACKER_LARGE_SIZE );
		size_t blockSize = 1<<MEM_TRACKER_MIN_BLOCKSIZE;
		size_t blockId = 0;
		while ( size > blockSize ) {
			blockSize <<= 1;
			++blockId;
		}
		check( blockId < MEM_TRACKER_LARGE_BLOCK );
		return blockId;
	}
}

//
//

void SetCurrentSourceState( const char* file, int line )
{
	mem_tracker::inst().SetSourceState( file, line );
}

void* mem_alloc( size_t size, BlockType bt )
{
	return mem_tracker::inst().Allocate( size, bt );
}

void mem_free( void* ptr, BlockType bt )
{
	mem_tracker::inst().Free( ptr, bt );
}

const mem_stat&	mem_info()
{
	return mem_tracker::inst().info();
}

void mem_report()
{
	mem_tracker::inst().report();
}
