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

#ifndef _GXLIB_SMAT_POINTER_H_
#define _GXLIB_SMAT_POINTER_H_

#pragma warning( push )
#pragma warning( disable: 4284 )

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

	explicit iSmartPtr( _Tp* p = 0 ) : _ptr( p ) {}
	~iSmartPtr() { delete _ptr; }

	void Reset( _Tp* p = 0 )
	{
		if ( _ptr != p ) {
			ThisType(p).Swap( *this );
		}
	}

	_Tp& operator * () const
	{
		check( _ptr != 0 );
		return *_ptr;
	}

	_Tp* operator -> () const
	{
		check( _ptr != 0 );
		return _ptr;
	}

	_Tp* Get() const
	{ return _ptr; }

	operator bool () const
	{ return _ptr == 0 ? false : true; }

	bool operator ! () const
	{ return _ptr == 0; }

	void Swap( iSmartPtr& s )
	{
		_Tp* tmp = s._ptr;
		s._ptr = _ptr;
		_ptr = tmp;
	}

	_Tp* Giveup()
	{ _Tp* val = _ptr; _ptr = 0; return val; }
};

#pragma warning( pop )

#endif //_GXLIB_SMAT_POINTER_H_

