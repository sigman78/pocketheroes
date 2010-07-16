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

#ifndef XXL_ARRAY_UTILS_MOVE_H__
#define XXL_ARRAY_UTILS_MOVE_H__

#include "xxl.array.mem.h"

namespace util {

// partial move framework
template< typename T >
class move_source {
	explicit move_source( T& src ) 
	: data_( src )
	{}

	T& get() const
	{ return data_; }

private:
	T& data_;

	typedef move_source<T> SelfType__;
	SelfType__& operator=( const SelfType__& );
};

// traits class
template< typename T >
struct move_traits {
	// tells wether the special move constructor or copy constructor should be called
	typedef typename meta::is_internal<T>::type moveable;
	// tells if the move is complete or partial (do we need to destroy the source)
	typedef typename meta::has_trivial_dtor<T>::type complete;
};

// move source traits
template< typename T >
struct move_source_traits {
	typedef typename move_traits<T>::moveable moveable;

	typedef typename meta::if_<
			moveable, 
			move_source<T>, 
			const T& >::type type;
};

// wrapper
template< typename T >
inline typename move_source_traits<T>::type
as_move_source( T& src )
{
	typedef typename move_source_traits<T>::type SrcT;
	return SrcT(src);
}

//
// algorithm support
namespace impl {

	template< typename T1, typename T2 >
	inline void move_construct_aux( T1* p, T2& val, const meta::false_ /*ispod*/ )
	{ new( p ) T1( as_move_source(val) ); }

	template< typename T1, typename T2 >
	inline void move_construct_aux( T1* p, T2& val, const meta::true_ /*ispod*/ )
	{ new( p ) T1( val ); }

} // impl

template< typename Dst, typename Src >
void move_construct( Dst* p, Src& val )
{
	typedef typename meta::is_pod<Dst>::type isPOD;
	impl::move_construct_aux( p, val, isPOD() );
}

template< typename T >
inline void destruct_moved( T* ptr ) 
{
	typedef typename move_traits<T>::complete trivialDtor;
	impl::destruct_aux( ptr, trivialDtor() );
}

template< typename Ptr >
inline void destruct_moved( T* first, T* last )
{
	typedef typename move_traits<T>::complete trivialDtor;
	impl::destruct_range_aux( first, last, trivialDtor() );
}

//
//
namespace impl {

	template< typename InT, typename OutT >
	inline OutT umove_ptrs_aux( InT first, InT last, OutT res, const meta::false_ /*moveable*/ )
	{
		// fallback to copy
		return ucopy_ptrs( first, last, res );
	}

	template< typename InT, typename OutT >
	inline OutT umove_ptrs_aux( InT first, InT last, OutT res, const meta::true_ /*moveable*/ )
	{
		// move ctor does not throw
		size_t num = (size_t)(last - first);
		for( size_t n = 0; n != num; ++n ) {
			move_construct( &*res, *first );
			++first;
			++res;
		}	
		return res;
	}

} // util

template< typename S, typename D >
inline D* umove_ptrs( S* first, S* last, D* res )
{
	typedef typename move_traits<S>::moveable MoveAble;
	impl::umove_ptrs_aux( first, last, res, MoveAble() );
}

#endif //XXL_ARRAY_UTILS_MOVE_H__

