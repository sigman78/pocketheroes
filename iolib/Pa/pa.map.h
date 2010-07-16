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

#ifndef IOLIB_PA_MAP_H
#define IOLIB_PA_MAP_H

#include "he\he.std.h"
#include "he\he.container.simple.array.h"

template< class K, class V >
struct Mapping
{
	K key;
	V val;

	Mapping()
	: key()
	, val()
	{}

	Mapping( K k, V v )
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

template< class K, class V >
class AssocMap : protected iSimpleArray< Mapping< K, V > >
{
	typedef K							KeyType;
	typedef V							ValueType;

	typedef Mapping< K, V >				DataType;
	typedef iSimpleArray< DataType >	BaseType;
	typedef AssocMap< K, V >			SelfType;

public:

	AssocMap()
	: sorted_( false )
	{}

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

	V operator [] ( const K& key ) const
	{
		int ndx = FindIndex( key );
		if ( ndx == -1 ) return V();
		return BaseType::operator[]( ndx ).val;
	}

	bool Exists( const K& key) const
	{
		return (FindIndex( key ) != -1);
	}

	int Size() const
	{
		return BaseType::GetSize();
	}

	const DataType&  At( int i ) const
	{
		return BaseType::operator[]( i );
	}

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

#endif //IOLIB_PA_MAP_H
