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

//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>

#include "xxl.arena.h"


// IN SYSTEM_INFO its
// dwAllocationGranularity
// dwPageSize
const size_t VASPageSize= 64 * 1024;
const size_t OSPageSize	=  4 * 1024;
const size_t GrowNumPages= 4;          // 16K allocation granularity

#define MM_ASSERT( xx ) check( xx )

template< typename T >
inline bool IsPow2( T x )
{
    return !(x & ~(-x));
}

template< typename T >
inline T AlignUp( T val, size_t alignment )
{
    MM_ASSERT( IsPow2( alignment ) );
    return (T)( ( (UINT_PTR)val + (alignment-1) ) & ~( alignment-1 ));
}

//
// arena implementation
//
bool
arena::is_valid() const
{
	// non-initialized state is counted as invalid
	if ( (begin_ == NULL) || (limit_ == NULL) ) return false;

	// if allocated, limit must be strictly greater than begining
	if ( begin_ >= limit_ ) return false;
	// end_ must lie within [begin_,limit_]
	if ( (end_ < begin_) || (end_ > limit_) ) return false;
	// head_ must be withing [begin_,end_]
	if ( (head_ < begin_) || (head_ > end_) ) return false;

	return true;
}

arena::arena()
: begin_(NULL)
, end_(NULL)
, limit_(NULL)
, head_(NULL)
{}

arena::arena( size_t capacity, size_t initial )
: begin_(NULL)
, end_(NULL)
, limit_(NULL)
, head_(NULL)
{
	bool ok = init( capacity, initial );
	MM_ASSERT( ok );
}

arena::~arena()
{
	done();
}

bool
arena::init( size_t capacity, size_t initial )
{
	// init is already called
	MM_ASSERT( begin_ == NULL && "Arena : Trying to initialize second time!" );

	// initialize region
	size_t bytesReserve = AlignUp( capacity, VASPageSize );
	begin_ 				= (byte*)VirtualAlloc( NULL, bytesReserve, MEM_RESERVE, PAGE_NOACCESS );
	// fails for unknown reason ;)
	MM_ASSERT( begin_ != NULL && "Arena : Reservation failed!" );
	if ( !begin_ ) return false;
	// this is our reservation bound
	limit_ 			= begin_ + bytesReserve;
	end_			= begin_;
	head_			= begin_;

	// preallocate initial region
	return grow_to( begin_ + initial );
}

void
arena::done()
{
	// dispose memory
	// do not need to decommit pages, its done automatically
	if ( NULL != begin_ ) {
		VirtualFree( (PVOID)begin_, (SIZE_T)(limit_ - begin_), MEM_RELEASE );
	}

	// clear vars
	begin_ = limit_ = head_ = end_ = NULL;
}

void
arena::report()
{
	MM_ASSERT( 0 == "Not implemented" );
}

void*	
arena::alloc( size_t bytes, size_t alignment /*= DefaultAlignment*/ )
{
	MM_ASSERT( is_valid() && "Arena : Im sick!" );

	// align head_ ptr according to the 'alignment'
	byte* start = AlignUp( head_, alignment );
	// new head_ ptr is
	byte* new_head = start + bytes;

	if ( (new_head > end_) && !grow_to( new_head ) ) {
		// grow failed
		MM_ASSERT( 0 == "Arena : Grow failed!" );
		return NULL;
	}

	head_ = new_head;
	return (void*)start;
}

bool
arena::expand( size_t bytes )
{
	MM_ASSERT( is_valid() && "Arena : Im sick!" );

	// increase head_
	byte* new_head = head_ + bytes;

	if ( (new_head > end_) && !grow_to( new_head ) ) {
		// grow error!
		MM_ASSERT( 0 == "Arena : Grow failed!" );
		return false;
	}

	head_ = new_head;
	return true;
}

arena::mark_t
arena::push()
{
	MM_ASSERT( is_valid() && "Arena : Im sick!" );
	return (mark_t)head_;
}

void
arena::pop( mark_t mark )
{
	MM_ASSERT( is_valid() && "Arena : Im sick!" );
	head_ = (byte*)mark;
	MM_ASSERT( is_valid() && "Arena : Im sick!" );
}

void
arena::purge( bool all )
{
	if ( begin_ != NULL && begin_ != end_ ) {
		// ensure what the arena is valid
		MM_ASSERT( is_valid() && "Arena : Im sick!" );
		MM_ASSERT( (!all || empty()) && "Arena : Im not empty!" );
		// dispose 
		byte* new_end = all ? begin_ : AlignUp( head_, OSPageSize * GrowNumPages );	
		VirtualFree( (PVOID)new_end, (SIZE_T)(end_ - new_end), MEM_DECOMMIT );
		// reset
		end_ = new_end;
	}
}

bool
arena::grow_to( byte* new_end ) 
{
	byte* aligned_end = end_; //AlignUp( end_, OSPageSize * GrowNumPages );
	//byte* aligned_end = begin_ + AlignUp( (end_ - begin_), OSPageSize * GrowNumPages );
	new_end = AlignUp( new_end, OSPageSize * GrowNumPages );	

	// reached and of reserved region
	if ( new_end > limit_ ) return false;

	if ( new_end > aligned_end ) {
		// commit pages in region or bail out
		if ( NULL == VirtualAlloc( (PVOID)begin_, (SIZE_T)( new_end - begin_), MEM_COMMIT, PAGE_READWRITE ) ) return false;
	}

	//end_ = max( end_, new_end );
	end_ = new_end;
	return true;
}

// self test
/////////////////////////////////

#ifdef SELF_TEST

void status( arena& a )
{
	printf("$ %08x %08x (%d) %08x (%d) %08x (%d)\n", a.begin_, a.head_, a.head_ - a.begin_,a.end_, a.end_ - a.begin_, a.limit_, a.limit_ - a.begin_ );
}

#define stat( x ) status( x )

int func2( arena& a, size_t s )
{
	{
		arena_lock lk( a );
	
		stat( a );
		printf("#2 executing 10 times by %d bytes\n", s );

		for( size_t n = 0; n != 10; ++n ) {
			void* p = a.alloc( s );
			MM_ASSERT( p != NULL );
			stat( a );
		}

		printf("#2 releasing\n");
	}
	stat( a );
	return 0;
}

int func1( arena& a, size_t s1, size_t s2 )
{

	stat( a );
	{
		arena_lock lk( a );
		printf("#1 - allocating %d\n", s1 );
		void* p1 = a.alloc( s1 );
		MM_ASSERT( p1 != NULL );
	
		func2( a, s2 );
		a.purge();

		printf("#1 - allocating %d\n", s1 );
		void* p2 = a.alloc( s1 );
		MM_ASSERT( p2 != NULL );

		printf("#1 releasing\n");
	}
	stat( a );
	return 0;
}

int main()
{
	arena ar;

	printf("init... \n");
	ar.init( 600*1024, 10000 );

	printf("testing...\n");
	func1( ar, 12345, 54321 );
    printf("puring...\n");
	stat( ar );
	ar.purge( true );
	stat( ar );
	printf("testing again...\n");
	func1( ar, 12345, 54321 );
	stat( ar );
	

	printf("done...\n");
	ar.done();

	return 0;
}

#endif // SELF_TEST



