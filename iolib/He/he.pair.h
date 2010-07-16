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
 *	@brief The template class stores a pair of objects implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_PAIR_H_
#define _IOLIB_HELIUM_PAIR_H_

/// @brief The template class stores a pair of objects, first, of type T, and second, of type U. 
/// The type definition first_type is the same as the template parameter T, while second_type is the same as the template parameter U
template< typename T, typename U >
class iPair
{
public:

	/// Initializes first to T() and second to U(). 
	iPair( const T &f, const U& s )
	: first(f)
	, second(s)
	{}

	/// Default constructor
	iPair()
	{}

	/// Copy constructor
	iPair( const iPair& p )
	: first( p.first )
	, second( p.second )
	{}

	/// Assignment operator
	iPair& operator = ( const iPair& other )
	{
		first	= other.first;
		second	= other.second;
		return *this;
	}

	/// Equality operator
	bool operator == ( const iPair& other ) const
	{
		return ( first == other.first ) && ( second == other.second );
	}

	/// Inequality operator
	bool operator != ( const iPair& other ) const
	{
		return !operator==(other);
	}

	T first;
	U second;
};

template< typename T, typename U >
iPair<T,U> MakePair( const T& first, const U& second ) 
{
	return iPair<T,U>( first, second );
}

#endif //_IOLIB_HELIUM_PAIR_H_
/** @} */ // iolib_he
