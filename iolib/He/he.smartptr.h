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

/** @file
 *	@brief Smart pointer template class implementation
 */
/** @addtogroup iolib_he
 *  @{
 */

#ifndef _IOLIB_HELIUM_SMART_POINTER_H_
#define _IOLIB_HELIUM_SMART_POINTER_H_

#pragma warning( push )
#pragma warning( disable: 4284 )

/// Smart pointer template class 
template< typename _Tp >
class iSmartPtr
{
private:
	_Tp*	_ptr;				

	iSmartPtr( iSmartPtr const& );
	iSmartPtr& operator = ( iSmartPtr const& );

	typedef iSmartPtr<_Tp> ThisType;

public:
	typedef _Tp		ElementType;

	/// Explicit default constructor
	explicit iSmartPtr( _Tp* p = 0 ) : _ptr( p ) {}
	~iSmartPtr() { delete _ptr; }

	/// Resets poiner
	void Reset( _Tp* p = 0 )
	{
		if ( _ptr != p ) {
			ThisType(p).Swap( *this );
		}
	}

	/// Dereference operator
	_Tp& operator * () const
	{
		check( _ptr != 0 );
		return *_ptr;
	}

	/// Pointer to member operator
	_Tp* operator -> () const
	{
		check( _ptr != 0 );
		return _ptr;
	}

	/// Returns pointer
	_Tp* Get() const
	{ return _ptr; }

	/// Equality to zero
	operator bool () const
	{ return _ptr == 0 ? false : true; }

	/// Inequality to zero
	bool operator ! () const
	{ return _ptr == 0; }

	/// Swaps to iSmartPtr's values
	void Swap( iSmartPtr& s )
	{
		_Tp* tmp = s._ptr;
		s._ptr = _ptr;
		_ptr = tmp;
	}

	/// Dettaches poiner
	_Tp* Giveup()
	{ _Tp* val = _ptr; _ptr = 0; return val; }
};

#pragma warning( pop )


#endif //_IOLIB_HELIUM_SMART_POINTER_H_
/** @} */ // iolib_he
