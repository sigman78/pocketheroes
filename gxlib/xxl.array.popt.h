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

#ifndef XXL_ARRAY_PTRS_MAIN_H__
#define XXL_ARRAY_PTRS_MAIN_H__

#include "xxl.array.meta.h"
#include "xxl.array.h"
#include "xxl.array.stg.h"

namespace util {

namespace impl {

	struct cvp_shim{ cvp_shim( void* ); };
	struct cvp_const_shim{ cvp_const_shim( const void* ); };
	struct cvp_volatile_shim{ cvp_volatile_shim( volatile void* ); };
	struct cvp_const_volatile_shim{ cvp_const_volatile_shim( const volatile void* ); };

	template< typename T > 
	char  use_void_ptr( const meta::false_, const T& );
	char  use_void_ptr( const meta::true_, ... );
	char* use_void_ptr( const meta::true_, cvp_shim );

	template< typename T >
	char  use_const_void_ptr( const meta::false_, const T& );
	char  use_const_void_ptr( const meta::true_, ... );
	char* use_const_void_ptr( const meta::true_, cvp_const_shim );

	template< typename T >
	char  use_volatile_void_ptr( const meta::false_, const T& );
	char  use_volatile_void_ptr( const meta::true_, ... );
	char* use_volatile_void_ptr( const meta::true_, cvp_volatile_shim );

	template< typename T >
	char  use_const_volatile_void_ptr( const meta::false_, const T& );
	char  use_const_volatile_void_ptr( const meta::true_, ... );
	char* use_const_volatile_void_ptr( const meta::true_, cvp_const_volatile_shim );

} // impl

	template< typename T >
	struct ptr_storage_type {
		typedef typename meta::is_pod<T>::type PODType;

		static T tester();

		enum { use_void_ptr =
				(sizeof(char*) == sizeof(impl::use_void_ptr( PODType(), tester()) ) ) };
		enum { use_const_void_ptr =
				(sizeof(char*) == sizeof(impl::use_const_void_ptr( PODType(), tester()) ) ) };
		enum { use_volatile_void_ptr =
				(sizeof(char*) == sizeof(impl::use_volatile_void_ptr( PODType(), tester()) ) ) };
		enum { use_const_volatile_void_ptr =
				(sizeof(char*) == sizeof(impl::use_const_volatile_void_ptr( PODType(), tester()) ) ) };

		typedef typename meta::if_cond_< !use_const_volatile_void_ptr,
					T,
					typename meta::if_cond_< use_void_ptr,
						void*,
						typename meta::if_cond_< use_const_void_ptr,
							const void*,
							typename meta::if_cond_< use_volatile_void_ptr,
								volatile void*,
								const volatile void*>::type >::type >::type >::type QualifiedType;
		
		typedef QualifiedType	type;
	}; // storage type

	template< typename StorageT, typename ValueT >
	struct ptr_cast_traits {
		typedef StorageT		storage_type;
		typedef ValueT			value_type;

		static value_type* to_value_type_ptr( storage_type* ptr )
		{ return reinterpret_cast<value_type*>(ptr); }

		static value_type const * to_value_type_cptr( storage_type const * ptr )
		{ return reinterpret_cast<value_type const *>(ptr); }

		static value_type** to_value_type_pptr( storage_type** ptr )
		{ return reinterpret_cast<value_type const *>(ptr); }

		static value_type& to_value_type_ref( storage_type& ref )
		{ return reinterpret_cast<value_type&>(ref); }

		static value_type const& to_value_type_cref( storage_type const& ref )
		{ return reinterpret_cast<value_type const&>(ref); }

		//  -----------
		static storage_type* to_storage_type_ptr( value_type* ptr )
		{ return reinterpret_cast<storage_type*>(ptr); }

		static storage_type const * to_storage_type_cptr( value_type const * ptr )
		{ return reinterpret_cast<storage_type const*>(ptr); }

		static storage_type ** to_storage_type_pptr( value_type** ptr )
		{ return reinterpret_cast<storage_type**>(ptr); }

		static storage_type& to_storage_type_ref( value_type& ptr )
		{ return reinterpret_cast<storage_type&>(ptr); }

		static storage_type const& to_storage_type_cref( value_type const& ptr )
		//{ return (storage_type const&)ptr; }
		{ return reinterpret_cast<storage_type const&>(ptr); }

		// --------------
		template< typename T >
		static T const& to_storage_type_crefT( T const& ref ) 
		{ return ref; }
	};

} // util

template< typename Val > 
class array_popt : public meta::internal_class< array_popt< Val > >
{
	typedef typename util::ptr_storage_type< Val >::QualifiedType	StoredType;
	//typedef typename alloc_traits< StoredType, Std > 				StoredTypeAlloc;
	typedef dynamic_storage<StoredType>								StgType;
	typedef array_base< StoredType, StgType >						BaseType;
	typedef array_popt< Val >										this_type;

	typedef util::ptr_cast_traits< StoredType, Val >				cast_traits;


public:

	typedef Val					value_type;
	typedef value_type*	    	pointer;
	typedef const value_type*   const_pointer;
	typedef value_type*			iterator;
	typedef const value_type*	const_iterator;
	typedef value_type&			reference;
	typedef const value_type&	const_reference;
	
	typedef typename meta::decl_param<Val>::type	param_type;
	//typedef const Val&			param_type;

	////////////////////////////////////////////////////////////////////

	inline iterator first()
	{ return cast_traits::to_value_type_ptr( impl_.first() ); } 

	inline const_iterator first() const
	{ return cast_traits::to_value_type_cptr( impl_.first() ); }

	inline iterator last()
	{ return cast_traits::to_value_type_ptr( impl_.last() );  }

	inline const_iterator last() const
	{ return cast_traits::to_value_type_cptr( impl_.last() );  }

	inline iterator end()
	{ return cast_traits::to_value_type_ptr( impl_.end() );  }

	inline const_iterator end() const
	{ return cast_traits::to_value_type_cptr( impl_.end() );  }

	////////////////////////////////////////////////////////////////////
	
	inline bool empty() const
	{ return impl_.empty(); }

	inline size_t size() const
	{ return impl_.size(); }
		
	inline size_t capacity() const
	{ return impl_.capacity(); }

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
	{ return cast_traits::to_value_type_ref( impl_[index] ); }

	inline const_reference operator[]( size_t index ) const
	{ return cast_traits::to_value_type_cref( impl_[index] ); }

	inline reference at( size_t index )
	{ return cast_traits::to_value_type_ref( impl_.at(index) ); }

	inline const_reference at( size_t index ) const
	{ return cast_traits::to_value_type_cref( impl_.at(index) ); }

	////////////////////////////////////////////////////////////////////

	inline array_popt()
	: impl_()
	{}

	inline array_popt( const this_type& other )
	: impl_( other.impl_ )
	{}

	inline explicit array_popt( size_t reservation )
	: impl_( reservation )
	{}

	inline explicit array_popt( size_t reservation, param_type val )
	: impl_( reservation, cast_traits::to_storage_type_cref(val) )
	{}

	inline array_popt( const_iterator ifirst, const_iterator ilast )
	: impl_( ifirst, ilast )
	{}

	inline this_type& operator=( const this_type& other )
	{ impl_ = other.impl_; return *this; }

	////////////////////////////////////////////////////////////////////

	inline void resize( size_t newSize, param_type val )
	{ impl_.resize( newSize, cast_traits::to_storage_type_cref(val) ); }

	inline void resize( size_t newSize )
	{ impl_.resize( newSize); }

	inline void reserve( size_t reservation )
	{ impl_.reserve( reservation ); }

	void clear()
	{ impl_.clear(); }

	////////////////////////////////////////////////////////////////////

	inline void assign( size_t num, param_type val )
	{ impl_.assign( num, cast_traits::to_storage_type_cref(val) ); }

	inline void assign( const_iterator ifirst, const_iterator ilast )
	{ impl_.assign( cast_traits::to_storage_type_cptr(ifirst), cast_traits::to_storage_type_cptr(ilast) ); }

	////////////////////////////////////////////////////////////////////

	inline void push_back( param_type val )
	{ impl_.push_back( cast_traits::to_storage_type_cref( val ) ); }

	inline value_type pop_back()
	{ /*return cast_traits::to_value_type_*/
		return (value_type)impl_.pop_back();
	}

	////////////////////////////////////////////////////////////////////

	iterator insert( iterator pos, param_type val )
	{ return cast_traits::to_value_type_ptr( impl_.insert( cast_traits::to_storage_type_ptr(pos), cast_traits::to_storage_type_cref(val) ) );
	}

	void insert( iterator pos, size_t num, param_type val )
	{ impl_.insert( cast_traits::to_storage_type_ptr(pos), num, cast_traits::to_storage_type_cref(val) ); }

	void insert( iterator pos, const_iterator ifirst, const_iterator ilast )
	{ impl_.insert( cast_traits::to_storage_type_ptr(pos), cast_traits::to_storage_type_cptr(ifirst), cast_traits::to_storage_type_cptr(ilast) ); }

	////////////////////////////////////////////////////////////////////

	iterator erase( iterator pos )
	{ return cast_traits::to_value_type_ptr( impl_.erase( cast_traits::to_storage_type_ptr( pos ) ) ); }

	iterator erase( iterator ifirst, iterator ilast )
	{ return cast_traits::to_value_type_ptr( impl_.erase( cast_traits::to_storage_type_ptr( ifirst ), cast_traits::to_storage_type_ptr( ilast ) ) ); }

	////////////////////////////////////////////////////////////////////

	void swap( this_type& other )
	{ impl_.swap( other.impl_ ); }

	////////////////////////////////////////////////////////////////////

private:	

	BaseType impl_;
};

#endif //XXL_ARRAY_PTRS_MAIN_H__

