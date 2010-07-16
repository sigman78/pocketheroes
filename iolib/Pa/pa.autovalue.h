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

/*****************************************************************************

 [Pa]lladium XML library

 Uses: Gnome libxml2 parser
 
*****************************************************************************/

#ifndef IOLIB_PA_AUTOVALUE_H
#define IOLIB_PA_AUTOVALUE_H

//
//
template< class T, T init = T() >
class AutoVal
{
	T t_;

public:

	typedef T			DataType;
	typedef AutoVal&	SelfType;

	/// Def ctor
	inline AutoVal() : t_( init ) {}

	/// Copy ctor
	template< class V >
	inline AutoVal( V val ) : t_( val ) {}

//	inline const T& Data() const { return t_; }
	inline T Data() const { return t_; }

	///
	inline operator T () const { return t_; }
	inline operator T& () { return t_; }

	///
	template< class V > inline SelfType operator =   ( V val ) { t_ = val; return *this; }
	template< class V > inline SelfType operator +=  ( V val ) { t_ += val; return *this; }
	template< class V > inline SelfType operator -=  ( V val ) { t_ -= val; return *this; }
	template< class V > inline SelfType operator *=  ( V val ) { t_ *= val; return *this; }
	template< class V > inline SelfType operator /=  ( V val ) { t_ /= val; return *this; }
	template< class V > inline SelfType operator %=  ( V val ) { t_ %= val; return *this; }
	template< class V > inline SelfType operator &=  ( V val ) { t_ &= val; return *this; }
	template< class V > inline SelfType operator |=  ( V val ) { t_ |= val; return *this; }
	template< class V > inline SelfType operator ^=  ( V val ) { t_ ^= val; return *this; }
	template< class V > inline SelfType operator <<= ( V val ) { t_ <<= val; return *this; }
	template< class V > inline SelfType operator >>= ( V val ) { t_ >>= val; return *this; }

	inline SelfType operator ++ ()	{ ++t_; return *this; }
	inline DataType operator ++ ( int )  { return t_++; }

	inline SelfType operator -- ()	{ --t_; return *this; }
	inline DataType operator -- ( int )  { return t_--; }
};

#endif //IOLIB_PA_AUTOVALUE_H
