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

#ifndef XXL_ARRAY_UTILS_MAIN_H__
#define XXL_ARRAY_UTILS_MAIN_H__

#if UNDER_CE > 400
#	include <cstring>
#endif //MIPS
#include "xxl.array.cfg.h"
#include "xxl.array.meta.h"

// MEMDBG workaround (?)
#ifdef new
#define new_previously_defined new
#endif
#undef new

namespace util {

//
// construct & destruct element or range
namespace impl {

	template< typename T >
	inline void destruct_aux( T* ptr, const meta::false_ )
	{ ptr->~T(); }

	template< typename T >
	inline void destruct_aux( T* ptr, const meta::true_ )
	{}

	template< typename It >
	inline void destruct_range_aux( It first, It last, const meta::false_ )
	{
		for( ; first != last; ++first ) destruct_aux( &*first, meta::false_() );
	}

	template< typename It >
	inline void destruct_range_aux( It first, It last, const meta::true_ )
	{}

} //impl

template< typename T >
inline void destruct( T* ptr )
{ 
	//ptr; 
	typedef typename meta::has_trivial_dtor<T>::type t_dtor; 
	impl::destruct_aux( ptr, t_dtor() ); 
}

template< typename T >
inline void destruct( T* first, T* last )
{
	typedef typename meta::has_trivial_dtor<T>::type t_dtor; 
	impl::destruct_range_aux( first, last, t_dtor() ); 
}

template< typename T >
inline void construct( T* p )
{ new( p ) T(); }

template< typename T, typename U >
inline void construct( T* p, const U& v )
{ new( p ) T(v); }

//
// uninitialized copy
namespace impl {

	template< typename InT, typename OutT >
	inline OutT ucopy_impl( InT first, InT last, OutT res )
	{
		OutT cur = res;
		XXL_TRY {
			for ( size_t n = last - first; n != 0; --n ) {
				construct( &*cur, *first );
				++first;
				++cur;
			}
			return cur;
		} XXL_UNWIND_RET( destruct( res, cur ), cur )
	}


	inline void* 
	ucopy_impl_trivial( const void* first, const void* last, void* res )
	{
		const size_t bytes = ((const char*)last - (const char*)first);
		return bytes ? (char*)memcpy( res, first, bytes) + bytes : res;
	}

	// trivial type
	template< typename InT, typename OutT >
	inline OutT ucopy_ptrs_aux( InT first, InT last, OutT res, const meta::true_ )
	{
		return (OutT)ucopy_impl_trivial( first, last, res );
	}

	// trivial type
	template< typename InT, typename OutT >
	inline OutT ucopy_ptrs_aux( InT first, InT last, OutT res, const meta::false_ )
	{
		return ucopy_impl( first, last, res );
	}

} // impl

template< typename InT, typename OutT >
inline OutT* ucopy_ptrs( InT* first, InT* last, OutT* res )
{
	// XXL_COMPILE_CHECK( is_pointer(InT) && is_pointer(OutT) )
	return impl::ucopy_ptrs_aux( first, last, res, 
				meta::use_trivial_ucopy( first, last )
			);
}

namespace impl {

	template< typename P, typename T >
	inline void ufill_aux( P first, P last, const T& val, const meta::false_ )
	{
		P cur = first;
		XXL_TRY {
			for( size_t n = last - first; n != 0; --n, ++cur ) {
				construct( cur, val );
			}
		} XXL_UNWIND( destruct( first, cur ) );
	}

	template< typename P, typename T >
	inline void ufill_aux( P first, P last, const T& val, const meta::true_ )
	{
		// actually is should be guaranteed not to throw
		size_t num = last - first;
		for( size_t n = 0; n != num; ++n ) {
			first[n] = val;
		}
	}

	template< typename P, typename T >
	inline P ufilln_aux( P first, size_t n, const T& val, const meta::false_ )
	{	
		P cur = first;
		XXL_TRY {
			for( ; n != 0; --n, ++cur ) {
				construct( cur, val );
			}
		} XXL_UNWIND_RET( destruct( first, cur ), cur );
		return cur;
	}

	template< typename P, typename T >
	inline P ufilln_aux( P first, size_t num, const T& val, const meta::true_ )
	{
		for( size_t n = 0 ; n != num; ++n ) {
			first[n] = val;
		}
		return first + num;
	}

} // impl

//
template< typename P, typename T >
inline void ufill_ptrs( P* first, P* last, const T& val )
{
	// ??? trivial init ???
	impl::ufill_aux( first, last, val, meta::use_trivial_init( first ) );
}

template< typename P, typename T >
inline P* ufilln_ptrs( P* first, size_t n, const T& val )
{
	return impl::ufilln_aux( first, n, val, meta::use_trivial_init( first ) );	
}

namespace impl {

	template< typename P, typename T >
	inline P* uinit_aux2( P* first, size_t n, const T& val, const meta::false_ )
	{ return ufilln_ptrs( first, n, val ); }

	template< typename P, typename T >
	inline P* uinit_aux2( P* first, size_t n, const T& , const meta::true_ )
	{ 
		memset( (unsigned char*)first, 0, n * sizeof(P) );  
		return first + n;
	}

	template< typename P, typename T >
	inline P* uinit_aux( P* first, size_t n, const T& , const meta::true_ )
	{ return first + n;	}

	template< typename P, typename T >
	inline P* uinit_aux( P* first, size_t n, const T& val, const meta::false_ )
	{ return uinit_aux2( first, n, val, meta::use_zero_init( first ) );	}

} // impl

template< typename P, typename T >
inline P* uinit_ptrs( P* first, size_t n, const T& val )
{
	return impl::uinit_aux( first, n, val, meta::use_trivial_init( first ) );
}

namespace impl {

	inline void* 
	copy_trivial( const void* first, const void* last, void* dest )
	{
		const size_t bytes = ((const char*)last - (const char*)first);
		return bytes ? (char*)memmove( dest, first, bytes) + bytes : dest;
	}

	template< typename InT, typename OutT >
	OutT copy_aux( InT first, InT last, OutT dest, const meta::true_ )
	{
		return (OutT)copy_trivial( first, last, dest );
	}

	template< typename InT, typename OutT >
	OutT copy_aux( InT first, InT last, OutT dest, const meta::false_ )
	{
		for( size_t n = last - first; n != 0; --n ) {
			*dest = *first;
			++first;
			++dest;
		}
		return dest;
	}

	inline void*
	copy_backward_trivial( const void* first, const void* last, void* dest )
	{
		const size_t bytes = ((const char*)last - (const char*)first);
		return bytes ? (char*)memmove( (char*)dest - bytes, first, bytes) : dest;
	}

	template< typename InT, typename OutT >
	OutT copy_backward_aux( InT first, InT last, OutT dest, const meta::true_ )
	{
		return (OutT)copy_backward_trivial( first, last, dest );
	}

	template< typename InT, typename OutT >
	OutT copy_backward_aux( InT first, InT last, OutT dest, const meta::false_ )
	{
		for( size_t n = last - first; n != 0; --n ) {
			*--dest = *--last;
		}
		return dest;
	}

} // impl

template< typename S, typename D >
inline D* copy_ptrs( S* first, S* last, D* dest )
{
	return impl::copy_aux( first, last, dest, meta::use_trivial_copy( first, dest ) );
}

template< typename S, typename D >
inline D* copy_backward_ptrs( S* first, S* last, D* dest )
{
	return impl::copy_backward_aux( first, last, dest, meta::use_trivial_copy( first, dest ) );
}

template< typename P, typename T >
inline P* fill_ptrs( P* first, P* last, const T& val )
{
	const size_t num = last - first;
	for( size_t n = 0; n != num; ++n ) {
		first[n] = val;
	}	
	return first + num;
}

template< typename P, typename T >
inline P* filln_ptrs( P* first, size_t num, const T& val )
{
	for( size_t n = 0; n != num; ++n ) {
		first[n] = val;
	}	
	return first + num;
}

inline char* fill_ptrs( char* first, char* last, char val )
{
	memset( first, val, last - first );
	return first + (last-first);
}

namespace impl {

//	template< typename InT, typename OutT >
//	OutT

} // impl

} // util

#ifdef new_previously_defined
#define new new_previously_defined
#undef new_previously_defined
#endif

#endif //XXL_ARRAY_UTILS_MAIN_H__


