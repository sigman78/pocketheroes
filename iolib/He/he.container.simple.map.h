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
 *	@brief Associative map container template class implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_CONTAINER_SIMPLE_MAP_H_
#define _IOLIB_HELIUM_CONTAINER_SIMPLE_MAP_H_

#include "he\he.std.h"
#include "he\he.container.simple.array.h"

/// Associative map Key-Value element template class
template< class K, class V >
struct iMapping
{
	K key;
	V val;

	/// Default constructor
	iMapping()
	: key()
	, val()
	{}

	/// Construcotr
	iMapping( K k, V v )
	: key( k )
	, val( v )
	{}

	bool operator < ( const K& other ) const
	{
		return key < other;
	}

	bool operator == ( const K& other ) const
	{
		return key == other;
	}
};

/// Associative map container template class
template< class K, class V >
class iAssocMap : protected iSimpleArray< iMapping< K, V > >
{
	typedef K							KeyType;
	typedef V							ValueType;

	typedef iMapping< K, V >			DataType;
	typedef iSimpleArray< DataType >	BaseType;
	typedef iAssocMap< K, V >			SelfType;

public:

	/// Default constructor
	iAssocMap()
	: sorted_( false )
	{}

	/// Returns reference to the item with specified key
	V& operator [] ( const K& key )
	{
		int ndx = FindIndex( key );
		if ( ndx == -1 ) {
			BaseType::Add( DataType( K(key), V() ) ); // !!!
			SortKeys();
			ndx = FindIndex( key );
		}
		return BaseType::operator[]( ndx ).val;
	}

	/// Returns item with specified key
	V operator [] ( const K& key ) const
	{
		int ndx = FindIndex( key );
		if ( ndx == -1 ) return V();
		return BaseType::operator[]( ndx ).val;
	}

	/// Returns availability of the item with specified key
	bool Exists( const K& key) const
	{
		return (FindIndex( key ) != -1);
	}

	/// Returns number of elements in container
	int Size() const
	{
		return BaseType::GetSize();
	}

	/// Returns reference to the item with specified index
	const DataType&  At( int i ) const
	{
		return BaseType::operator[]( i );
	}

	/// Removes all items from the container
	void RemoveAll()
	{
		BaseType::RemoveAll();
	}
	
private:

	bool sorted_;

	void SortKeys()
	{
		int n = GetSize();
		for( int i = 0; i < n; i++ ) {
			int mine = i;
			for( int j = i + 1; j < n; j++ ) {
				if ( BaseType::operator[]( j ).key < BaseType::operator[]( mine ).key ) mine = j;
			}
			if ( i != mine ) {
				DataType temp( BaseType::operator[]( i ) );
				BaseType::operator[]( i ) = BaseType::operator[]( mine );
				BaseType::operator[]( mine ) = temp;
			}
		}
	}

	int FindIndex( const K& key ) const
	{
		int ndx = LowerBound( key );
		return (ndx != (int)BaseType::GetSize() && !(key < At(ndx).key )) ? ndx : -1;
	}

	int LowerBound( const K& key ) const
	{
		int len = (int)BaseType::GetSize();
		int first = 0;
		int half, middle;

		while ( len > 0 ) {
			half = len >> 1;
			middle = first + half;
			if ( BaseType::operator[]( middle ).key < key ) {
				first = middle + 1;
				len = len - half - 1;
			} else {
				len = half;
			}
		}

		return first;
	}

};


#endif //_IOLIB_HELIUM_CONTAINER_SIMPLE_MAP_H_
/** @} */ // iolib_he
