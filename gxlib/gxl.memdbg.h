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

#ifndef GXLIB_MEMDBG_H_
#define GXLIB_MEMDBG_H_

//
// we are tracking number of the blocks

//
// global defines

//#define calloc			error __do_not_use_calloc__
//#define realloc			error __do_not_use_realloc__
//#define VirtualAlloc	error __do_not_use_VirtalAlloc__

//
// tunables ( sizes are in pow of 2 )
#define MEM_TRACKER_MIN_BLOCKSIZE 3  // 2^3  = 8 
#define MEM_TRACKER_MAX_BLOCKSIZE 10 // 2^10 = 1024
#define MEM_TRACKER_NUM_BLOCKS	(MEM_TRACKER_MAX_BLOCKSIZE - MEM_TRACKER_MIN_BLOCKSIZE + 2)
#define MEM_TRACKER_LARGE_BLOCK	(MEM_TRACKER_MAX_BLOCKSIZE - MEM_TRACKER_MIN_BLOCKSIZE + 1)
#define MEM_TRACKER_LARGE_SIZE	(1UL<<MEM_TRACKER_MAX_BLOCKSIZE)

//
enum BlockType {
	BlockkType_Invalid = -1,
	BlockType_Malloc,
	BlockType_New,
	BlockType_NewArr
};

//
struct mem_sample {
	size_t	total;
	size_t	current;
	size_t	peak;
};

struct mem_stat {
	mem_sample	totals;
	mem_sample	allocs;
	mem_sample	block[ MEM_TRACKER_NUM_BLOCKS ];
};

//
//
void*			mem_alloc( size_t size, BlockType bt );
void			mem_free( void* ptr, BlockType bt );
const mem_stat&	mem_info();
void			mem_report();

void			SetCurrentSourceState( const char* file, int line );

#ifndef __PLACEMENT_NEW_INLINE
#define __PLACEMENT_NEW_INLINE
inline void* operator new( size_t, void* P )
{ return P; }

inline void operator delete( void*, void* )
{ return; }
#endif //__PLACEMENT_NEW_INLINE

template< typename T >
void PlacementNew( T* p, const T& t )
{
	new( p ) T(t);
}

//
//

#ifdef MEM_TRACK

inline void* operator new( size_t size) { return mem_alloc( size, BlockType_New ); }
inline void* operator new[]( size_t size) { return mem_alloc( size, BlockType_NewArr ); }
inline void	operator delete( void* addr) {  mem_free( addr, BlockType_New ); }
inline void	operator delete[]( void* addr) { mem_free( addr, BlockType_NewArr ); }

#define new    (SetCurrentSourceState(__FILE__,__LINE__),false) ? NULL : new
#define delete (SetCurrentSourceState(__FILE__,__LINE__),false) ? SetCurrentSourceState("",0) : delete
#define malloc(size) ((SetCurrentSourceState(__FILE__, __LINE__), false) ? NULL : mem_alloc(size, BlockType_Malloc))
#define	free(ptr) (SetCurrentSourceState(__FILE__, __LINE__), false) ? SetCurrentSourceState("",0) : mem_free(ptr, BlockType_Malloc)

#endif


#endif //GXLIB_MEMDBG_H_


