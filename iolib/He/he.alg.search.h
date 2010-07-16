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
 *	@brief Binary search algorithms implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_ALG_SEARCH_H_
#define _IOLIB_HELIUM_ALG_SEARCH_H_

/// Returns distance between two given pointers
template< typename T >
INT_PTR Distance( T first, T last )
{
	return (INT_PTR)(last - first);
}

/// Returns first found iterator
template< class FwdIter, class Type >
inline FwdIter LowerBound( FwdIter first, FwdIter last, const Type& val )
{
	typedef INT_PTR DistType;

	DistType length = Distance( first, last );
	DistType halfway;
	FwdIter middle;

	while( length > 0 ) {
		halfway	= length >> 1;
		middle	= first;
		//Advance( middle, halfway );
		middle += halfway;
		if ( *middle < val ) {
			first = middle;
			++first;
			length = length - halfway - 1;
		} else {
			length = halfway;
		}
	}
	return first;
}

/// Returns first found iterator
template< typename FwdIter, class Type, class Comp >
inline FwdIter LowerBound( FwdIter first, FwdIter last, const Type& val, Comp comp )
{
	typedef INT_PTR DistType;

	DistType length = Distance( first, last );
	DistType halfway;
	FwdIter middle;

	while( length > 0 ) {
		halfway	= length >> 1;
		middle	= first;
		//Advance( middle, halfway );
		middle += halfway;
		if ( comp( *middle, val ) ) {
			first = middle;
			++first;
			length = length - halfway - 1;
		} else {
			length = halfway;
		}
	}
	return first;
}

/// Returns last found iterator
template< class FwdIter, class Type >
inline FwdIter UpperBound( FwdIter first, FwdIter last, const Type& val )
{
	typedef INT_PTR DistType;

	DistType length = Distance( first, last );
	DistType halfway;
	FwdIter middle;

	while( length > 0 ) {
		halfway	= length >> 1;
		middle	= first;
		//Advance( middle, halfway );
		middle += halfway;
		if ( val < *middle ) {
			length = halfway;
		} else {
			first = middle;
			++first;
			length = length - halfway - 1;
		}
	}
	return first;
}

/// Returns last found iterator
template< class FwdIter, class Type, class Comp >
inline FwdIter UpperBound( FwdIter first, FwdIter last, const Type& val, Comp comp )
{
	typedef INT_PTR DistType;

	DistType length = Distance( first, last );
	DistType halfway;
	FwdIter middle;

	while( length > 0 ) {
		halfway	= length >> 1;
		middle	= first;
		//Advance( middle, halfway );
		middle += halfway;
		if ( comp( val, *middle ) ) {
			length = halfway;
		} else {
			first = middle;
			++first;
			length = length - halfway - 1;
		}
	}
	return first;
}


template< class FwdIter, class Type >
inline FwdIter BinarySearch( FwdIter first, FwdIter last, const Type& val )
{
	FwdIter pos = LowerBound(first,last,val);
	if ( pos != last && !((val < (*pos)) || ((*pos) < val)) ) return pos;
	return last;
}

template< class T1, class T2, class Comp >
inline bool NotLessAndNotGreater( const T1& a, const T2& b, Comp comp )
{
	return !(comp(a,b) || comp(b,a));
}

template< class FwdIter, class Type, class Comp >
inline FwdIter BinarySearch( FwdIter first, FwdIter last, const Type& val, Comp comp )
{
	FwdIter pos = LowerBound(first,last,val,comp);
	if ( pos != last && NotLessAndNotGreater(val, *pos, comp) ) return pos;
	return last;
}

#endif //_IOLIB_HELIUM_ALG_SEARCH_H_
/** @} */ // iolib_he
