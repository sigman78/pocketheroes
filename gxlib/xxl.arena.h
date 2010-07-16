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

#ifndef XXL_MEMORY_ARENA_MAIN_H__
#define XXL_MEMORY_ARENA_MAIN_H__

// Memory Arena Manager
//
// Blazingly-fast allocates hunks of memory for temporal usages
//
// Its same to the system stack but with ability to dynamically grow 
// and contract on demand. Proceed with same care as to 'alloca'
//
// Memory from arena must be used only within the function local scope,
// never try to pass it somewhere outside.
//
// void fn() {
// 	arena& a = arena::instance(); 
// 	arena_lock lk( a );

// 	void* a1 = a.alloc( 12345 );
// 	void* a2 = a.alloc( 54321 );
// 	etc
// }

class arena 
{
public:

	struct perf_t {
		size_t	total;
		size_t	current;
		size_t	peak;
	};

	typedef void* mark_t;

	enum { DefaultAlignment = 8 };

	arena();
	arena( size_t capacity, size_t initial = 0 );
	~arena();
	
	bool	init( size_t capacity, size_t initial );
	void	done();
	void	report();

	void*	top();
	void*	alloc( size_t bytes, size_t alignment = DefaultAlignment );
	bool	expand( size_t size );

	//void*	dbg_alloc( size_t bytes, size_t alignment = DefaultAlignment, size_t line = 0, const char* file = UnknownFile );
	//void* dbg_expand( size_t bytes, size_t line = 0, const char* file = UnknownFile );

	mark_t	push();
	void	pop( mark_t );

	void	purge( bool all = false );

	size_t	capacity() const;
	size_t	size() const;
	bool	empty() const;
	
	bool	is_valid() const;
//private:
	typedef unsigned char* byte;
	
	bool	grow_to( byte* to );

	byte*	begin_;
	byte*	end_;
	byte*	limit_;

	byte*	head_;

	perf_t	palloc_;
	perf_t	pcommit_;

	arena( const arena& );
	arena& operator=( const arena& );
};

//

inline void* arena::top()
{ return (void*)head_; }

inline size_t arena::capacity() const
{ return (size_t)(limit_ - begin_); }

inline size_t arena::size() const
{ return (size_t)(begin_ - head_); }

inline bool arena::empty() const
{ return head_ == begin_; }

//
// scoped lock for arena allocations
struct arena_lock
{
public:
	arena_lock( arena& a ) 
	: arena_(a)
	, mark_( arena_.push() )
	{}

	~arena_lock()
	{ arena_.pop(mark_); }

private:
    arena&			arena_;
    arena::mark_t   mark_;

	arena_lock( const arena_lock& );
	arena_lock& operator=( const arena_lock& );	
};


#endif //XXL_MEMORY_ARENA_MAIN_H__

