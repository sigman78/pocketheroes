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

/// @file
///	@brief Non moveable scoped smart pointers
//|-----------------------------------------------------------
/// @addtogroup mm_common
/// @{

#ifndef __SCOPEPTR_H
#define __SCOPEPTR_H

#pragma warning( push )
#pragma warning( disable: 4284 )

/// Smart scoped pointer class
///	Mimics a built-in pointer except that it guarantees deletion
///	of the object pointed to, either on destruction of the self
///	or via an explicit method call. ScopedPtr is the simple 
///	solution for the simple needs; Use SharedPtr in other cases.
///	\note
///		Cannot be used with Array or other container, which 
///		requires copy constructor or assignment operator
///		being defined. Instead use SharedPtr.
template< typename T >
class ScopedPtr 
{
private:

	/// contained object
	T*	ptr_;				

	/// Disabled copy ctor
	ScopedPtr( ScopedPtr const& );
	/// Disabled assignment
	ScopedPtr& operator=( ScopedPtr const& );

	typedef ScopedPtr<T> ThisType;

public:

	/// Type of the contained object
	typedef T		ElementType;


	/// Constructs new ScopedPtr
	///	Takes as argument pointer to the object which becomes managed.
	///	\note Does not throw.
	explicit ScopedPtr( T* p = 0 ) : ptr_( p ) // throw()
	{}

	/// Destroys ScopedPtr
	///	If there still managed object present - destoys it as well.
	///	\note Does not throw.
	~ScopedPtr() // throw()
	{ delete ptr_; }

	/// Resets ScopedPtr
	///	Release managed object and destoy it.
	///	\note Does not throw.
	void Reset( T* p = 0 ) // throw()
	{
		if ( ptr_ != p ) {
			ThisType(p).Swap( *this );
		}
	}

	/// Takes reference to the managed pointer.
	///	\note Does not throw.
	T& operator*() const // throw()
	{
		assert( ptr_ != 0 );
		return *ptr_;
	}

	/// Dereference managed pointer
	///	\note Does not throw.
	T* operator->() const // throw()
	{
		assert( ptr_ != 0 );
		return ptr_;
	}

	/// Takes out managed pointer from ScopedPtr.
	///	\note Does not throw.
	T* Get() const // throw()
	{ return ptr_; }

	/// Checks if there managed pointer inside.
	///	\return True if managed object present
	///	\note Does not throw.
	operator bool() const // throw()
	{ return ptr_ == 0 ? false : true; }

	/// Checks if there no managed pointer inside.
	///	\return True if not managed object present.
	///	\note Does not throw.
	bool operator!() const // throw()
	{ return ptr_ == 0; }

	/// Swaps two ScopedPtrs.
	///	\note Does not throw.
	void Swap( ScopedPtr& s ) // throw()
	{
		T* tmp = s.ptr_;
		s.ptr_ = ptr_;
		ptr_ = tmp;
	}

	T* Giveup()
	{ T* val = ptr_; ptr_ = 0; return val; }
};

/// Smart scoped pointer as array class 
///	Mimics a built-in pointer to the array, except that it guarantees deletion
///	of the array pointed to, either on destruction of the self
///	or via an explicit method call. ScopedArray is the simple 
///	solution for the simple needs; Use SharedArray in other cases.
///	\note
///		Cannot be used with Array or other container, which 
///		requires copy constructor or assignment operator
///		being defined. Instead use SharedArray.
template< typename T >
class ScopedArray
{
private:

	/// contained array
	T*	ptr_;				

	/// Disabled copy ctor
	ScopedArray( ScopedArray const& );
	/// Disabled assignment
	ScopedArray& operator=( ScopedArray const& );

	typedef ScopedArray<T> ThisType;

public:

	/// Type of the contained object
	typedef T		ElementType;

	/// Constructs new ScopedArray
	///	Takes as argument pointer to the object which becomes managed.
	///	\note Does not throw.
	explicit ScopedArray( T* p = 0 ) : ptr_( p ) // throw()
	{}

	/// Destroys ScopedPtr
	///	If there still managed object present - destoys it as well.
	///	\note Does not throw.
	~ScopedArray() // throw()
	{ delete[] ptr_; }

	/// Resets ScopedPtr
	///	Release managed object and destoy it.
	///	\note Does not throw.
	void Reset( T* p = 0 ) // throw()
	{
		if ( ptr_ != p ) {
			delete[] ptr_;
			ptr_ = p;
		}
	}

	/// Implements [] operator for the contained array
	///	\note Does not throw.
	T& operator[]( size_t i ) const // throw()
	{
		assert( ptr_ != 0 );
		assert( i >= 0 );
		return ptr_[ i ];
	}

	/// Takes out managed pointer from ScopedArray.
	///	\note Does not throw.
	T* Get() const // throw()
	{ return ptr_; }

	/// Checks if there managed pointer inside.
	///	\return True if managed object present
	///	\note Does not throw.
	operator bool() const // throw()
	{ return ptr_ == 0 ? false : true; }

	/// Checks if there no managed pointer inside.
	///	\return True if not managed object present.
	///	\note Does not throw.
	bool operator!() const // throw()
	{ return ptr_ == 0; }

	/// Swaps two ScopedArrays.
	///	\note Does not throw.
	void Swap( ScopedArray& s ) // throw()
	{
		T* tmp = s.ptr_;
		s.ptr_ = ptr_;
		ptr_ = tmp;
	}

	T* Giveup()
	{ T* val = ptr_; ptr_ = 0; return val; }
};

#pragma warning( pop )

#endif //__SCOPEPTR_H


//---------------------------------------------------------
/// @}

