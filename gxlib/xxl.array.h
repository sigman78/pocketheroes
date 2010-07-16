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

#ifndef XXL_ARRAY_BASE_MAIN_H__
#define XXL_ARRAY_BASE_MAIN_H__

#include "xxl.array.cfg.h"
#include "xxl.array.stg.h"

template< typename Tp, typename Storage >
class array_base  
	: public meta::internal_class< array_base< Tp, Storage > >
	, private Storage
{
public:

		typedef Storage					base_type;
		typedef array_base<Tp,Storage>  this_type;

		typedef size_t					size_type;

		typedef Tp						value_type;
		typedef Tp&						reference;
		typedef const Tp&				const_reference;
		typedef Tp*						pointer;
		typedef const Tp*				const_pointer;

		typedef typename base_type::iterator		iterator;
		typedef typename base_type::const_iterator  const_iterator;

		typedef typename meta::decl_param<Tp>::type	param_type;
		//typedef const value_type& param_type;

private:

		typedef typename base_type::buffer_ptrs	buffer_ptrs;

		// helpers
		inline void grow_commit_destruct( buffer_ptrs newBuf, size_t newSize )
		{
			util::destruct( this_type::first(), this_type::end() );
			grow_commit_no_destruct( newBuf, newSize );
		}

		inline void grow_commit_no_destruct( buffer_ptrs newBuf, size_t newSize )
		{
			base_type::grow_commit( newBuf );
			finish_ = first() + newSize;
		}

		inline void grow_destruct_abort( buffer_ptrs newBuf, iterator curFin )
		{
			util::destruct( newBuf.first, curFin );
			base_type::grow_abort( newBuf );
		}

protected:

		inline bool chkIndex( size_t index ) const
		{ return index < size(); }

public:

		////////////////////////////////////////////////////////////////////

		inline iterator first()
		{ return base_type::first(); }

		inline const_iterator first() const
		{ return base_type::first(); }

		inline iterator last()
		{ check( finish_ != base_type::first() ); return (finish_-1); }

		inline const_iterator last() const
		{ check( finish_ != base_type::first() ); return (finish_-1); }

		inline iterator end()
		{ return finish_; }

		inline const_iterator end() const
		{ return finish_; }

		////////////////////////////////////////////////////////////////////

		inline bool empty() const
		{ return finish_ == this_type::first(); }

		inline size_t size() const
		{ return (size_t)(finish_ - this_type::first()); }
		
		inline size_t capacity() const
		{ return base_type::capacity(); }

		////////////////////////////////////////////////////////////////////

		inline reference front()
		{ return at(0); }

		inline const_reference front() const
		{ return at(0); }

		inline reference back() 
		{ return at( size() - 1 ); }

		inline const_reference back() const
		{ return at( size() - 1 ); }

		////////////////////////////////////////////////////////////////////

		inline reference operator[]( size_t index )
		{ check( index < size() ); return *(this_type::first() + index); }

		inline const_reference operator[]( size_t index ) const
		{ check( index < size() ); return *(this_type::first() + index); }

		inline reference at( size_t index )
		{ return (*this)[index]; }

		inline const_reference at( size_t index ) const
		{ return (*this)[index]; }

		////////////////////////////////////////////////////////////////////

		inline array_base()
		: base_type()
		, finish_( base_type::first() )
		{}

		inline ~array_base()
		{ util::destruct( first(), finish_ ); }

		array_base( const this_type& other )
		: base_type( other.size() )
		{ finish_ = util::ucopy_ptrs( other.first(), other.end(), this_type::first() ); }

		inline explicit array_base( size_t reservation )
		: base_type( reservation )
		{ 
			finish_ = util::ufilln_ptrs( base_type::first(), reservation, value_type() ); 
		}

		inline explicit array_base( size_t reservation, param_type val )
		: base_type( reservation )
		{ finish_ = util::ufilln_ptrs( base_type::first(), reservation, val );	}

		array_base( const_iterator ifirst, const_iterator ilast )
		: base_type( (size_t)(ilast-ifirst) )
		{ finish_ = util::ucopy_ptrs( ifirst, ilast, this_type::first() ); }

		this_type& operator=( const this_type& );

		////////////////////////////////////////////////////////////////////

		void resize( size_t newSize, param_type val )
		{
			if ( newSize < size() ) {
				erase( this_type::first() + newSize, this_type::end() );
			} else if ( newSize > size() ) {
				insert_fill( this_type::end(), newSize - size(), val );
			}
		}

		void resize( size_t newSize )
		{ resize( newSize, Tp() ); }

		void reserve( size_t reservation )
		{
			if ( base_type::need_grow( reservation ) ) {
				size_t oldSize = size();
				buffer_ptrs newBuf = base_type::grow_begin( reservation );
				XXL_TRY {
					//move!
					util::ucopy_ptrs( this_type::first(), this_type::end(), newBuf.first );
					grow_commit_destruct( newBuf, oldSize );
				} XXL_UNWIND( base_type::grow_abort( newBuf ) );
			}
		}

		void clear()
		{ util::destruct( first(), end() ); finish_ = first(); }

		////////////////////////////////////////////////////////////////////

		void assign( size_t num, param_type val )
		{ assign_fill( num, val ); }

		void assign( const_iterator first, const_iterator last )
		{ assign_range( first, last ); }

		////////////////////////////////////////////////////////////////////

		inline void push_back( param_type val )
		{
			if ( !base_type::need_grow( size() + 1 ) ) {
				util::construct( finish_, val );
				++finish_;
			} else {
				insert_fill( finish_, 1, val );
			}
		}	

		inline value_type pop_back()
		{
			value_type dup( back() );
			util::destruct( --finish_ );
			return dup;
		}

		////////////////////////////////////////////////////////////////////

		iterator insert( iterator pos, param_type val )
		{
			check( pos >= first() && pos <= end() );
			size_t index = (size_t)( pos - this_type::first() );

			if ( !base_type::need_grow( size() + 1 ) && pos == finish_ ) {
				util::construct( finish_, val );
				++finish_;	
			} else {
				insert_fill( pos, 1, val );
			}
			return first() + index;
		}

		void insert( iterator pos, size_t num, param_type val )
		{
			insert_fill( pos, num, val );
		}

		void insert( iterator pos, const_iterator ifirst, const_iterator ilast )
		{
			insert_range( pos, ifirst, ilast );
		}

		////////////////////////////////////////////////////////////////////

		iterator erase( iterator pos )
		{
			erase_range( pos, pos + 1 );
			return pos;
		}

		iterator erase( iterator ifirst, iterator ilast )
		{
			erase_range( ifirst, ilast );
			return ifirst;
		}

		////////////////////////////////////////////////////////////////////

		void swap( this_type& other )
		{
			base_type::swap( other );
			::swap( finish_, other.finish_ );
		}

		////////////////////////////////////////////////////////////////////

private:

		void assign_fill( size_t num, param_type val );
    	void assign_range( const_iterator first, const_iterator last );
    	void insert_fill( iterator pos, size_t num, param_type val );
		void insert_range( iterator pos, const_iterator first, const_iterator last );
		void erase_range( iterator first, iterator last );

		iterator finish_;
};

template< typename Tp, typename Storage >
array_base<Tp,Storage>&
array_base<Tp,Storage>::operator=( const array_base<Tp,Storage>& other )
{
	if ( &other != this ) {
		const size_t otherLen = other.size();
		if ( base_type::need_grow( otherLen ) ) {
			// gr/c/b	
			buffer_ptrs newBuf = base_type::grow_begin( otherLen );
			XXL_TRY {
				util::ucopy_ptrs( other.first(), other.end(), newBuf.first );
				grow_commit_destruct( newBuf, otherLen );
			} XXL_UNWIND( base_type::grow_abort( newBuf ) );
		} else if ( size() >= otherLen ) {
			iterator tail = util::copy_ptrs( other.first(), other.end(), first() );
			util::destruct( tail, end() );
			finish_ = first() + otherLen;
		} else {
			util::copy_ptrs( other.first(), other.first() + size(), first() );
			finish_ = util::ucopy_ptrs( other.first() + size(), other.end(), finish_ );
		}
	}
	return *this;
}

template< typename Tp, typename Storage >
void array_base<Tp,Storage>::assign_fill( size_t num, param_type val )
{
	if ( base_type::need_grow( num ) ) {
		// tricky, but works
		this_type tmp( num, val );
		tmp.swap( *this );
	} else if ( num > size() ) {
		util::fill_ptrs( this_type::first(), this_type::end(), val );
		finish_ = util::ufilln_ptrs( finish_, num - size(), val );
	} else {
		iterator tail = util::fill_ptrs( this_type::first(), this_type::first() + num, val );
		util::destruct( tail, finish_ );
		finish_ = tail;
	}
}

template< typename Tp, typename Storage >
void array_base<Tp,Storage>::assign_range( const_iterator ifirst, const_iterator ilast )
{
	size_t num = (size_t)(ilast - ifirst);
	if ( base_type::need_grow( num ) ) {
		// gr/c/b	
		buffer_ptrs newBuf = base_type::grow_begin( num );
		XXL_TRY {
			util::ucopy_ptrs( ifirst, ilast, newBuf.first );
			grow_commit_destruct( newBuf, num );
		} XXL_UNWIND( base_type::grow_abort( newBuf ) );
	} else if ( num > size() ) {
		const_iterator mid = ifirst + size();
		util::copy_ptrs( ifirst, mid, this_type::first() );
		finish_ = util::ucopy_ptrs( mid, ilast, finish_ );
	} else {
		iterator tail = util::copy_ptrs( ifirst, ilast, this_type::first() );
		util::destruct( tail, finish_ );
		finish_ = tail;
	}
}

template< typename Tp, typename Storage >
void array_base<Tp,Storage>::insert_fill( iterator pos, size_t num, param_type val )
{
	check( pos >= this_type::first() && pos <= this_type::end() );
	const size_t newSize = size() + num;
	if ( base_type::need_grow( newSize ) ) {
		// overflow case
		buffer_ptrs newBuf = base_type::grow_begin( newSize );
		iterator tail = newBuf.first;
		XXL_TRY {
			// move
			tail = util::ucopy_ptrs( first(), pos, tail );
			tail = util::ufilln_ptrs( tail, num, val );
			// move
			tail = util::ucopy_ptrs( pos, end(), tail );
			grow_commit_destruct( newBuf, (size_t)(tail - newBuf.first ) );
		} XXL_UNWIND( grow_destruct_abort( newBuf, tail ) );
	} else {
		const size_t numTail = (size_t)( finish_ - pos );
		iterator oldFinish = finish_;
		// WARN - self-inserting issue
		if ( numTail > num ) {
			// move
			finish_ = util::ucopy_ptrs( finish_ - num, finish_, finish_ );
			//finish_ += num;
			// move
			util::copy_backward_ptrs( pos, oldFinish - num, oldFinish );
			util::filln_ptrs( pos, num, val );
		} else {
			finish_ = util::ufilln_ptrs( finish_, num - numTail, val );
			// move
			finish_ = util::ucopy_ptrs( pos, oldFinish, finish_ );
			//finish_ += numTail;
			util::filln_ptrs( pos, numTail, val );
		}
	}
}

template< typename Tp, typename Storage >
void array_base<Tp,Storage>::insert_range( iterator pos, const_iterator ifirst, const_iterator ilast )
{
	check( pos >= this_type::first() && pos <= this_type::end() );
	const size_t num = (size_t)(ilast - ifirst);
	const size_t newSize = size() + num;
	if ( base_type::need_grow( newSize ) ) {
		buffer_ptrs newBuf = base_type::grow_begin( newSize );
		iterator tail = newBuf.first;
		XXL_TRY {
			// move
			tail = util::ucopy_ptrs( this_type::first(), pos, tail );
			tail = util::ucopy_ptrs( ifirst, ilast, tail );
			// move
			tail = util::ucopy_ptrs( pos, this_type::end(), tail );
			grow_commit_destruct( newBuf, (size_t)(tail - newBuf.first ) );
		} XXL_UNWIND( grow_destruct_abort( newBuf, tail ) );
	} else {
		const size_t numTail = (size_t)( finish_ - pos );
		iterator oldFinish = finish_;
		if ( numTail > num ) {
			// move
			finish_ = util::ucopy_ptrs( finish_ - num, finish_, finish_ );
			// move
			util::copy_backward_ptrs( pos, oldFinish - num, oldFinish );
			util::copy_ptrs( ifirst, ilast, pos );
		} else {
			const_iterator mid = ifirst + numTail;

			finish_ = util::ucopy_ptrs( mid, ilast, finish_ );
			// move
			finish_ = util::ucopy_ptrs( pos, oldFinish, finish_ );
			util::copy_ptrs( ifirst, mid, pos );
		}
	}
}

template< typename Tp, typename Storage >
void array_base<Tp,Storage>::erase_range( iterator ifirst, iterator ilast )
{
	check( ifirst >= this_type::first() && ifirst <= end() );
	check( ilast >= this_type::first() && ilast <= end() );
	check( ifirst <= ilast );
	// move
	iterator tail = util::copy_ptrs( ilast, finish_, ifirst );
	util::destruct( tail, finish_ );
	finish_ = tail;
}

////////////////////////////////////

/*
namespace util {

template< typename T, typename A >
struct move_traits< array_base< T, A > >
{
	typedef meta::true_							moveable;
	typedef typename move_traits< A >::complete complete;
};

} // util
*/

#endif //XXL_ARRAY_BASE_MAIN_H__

