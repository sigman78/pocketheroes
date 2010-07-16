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
 *	@brief Hash template class implementation
 */
/** @addtogroup iolib_he
 *  @{
 */

#ifndef _IOLIB_HELIUM_CONTAINER_HASH_H_
#define _IOLIB_HELIUM_CONTAINER_HASH_H_

#include "he.container.fast.array.h"

/// @brief Class Hash template class
/// Elements in the hash-table are not destroyed when the hash-table is destroyed.
template < class iKEY, class T, class iHASH, uint32 SIZ = 13 >
class iHashTable
{
public:

	class Iterator;
	class ConstIterator;

	/// Creates a new hashtable with number of buckets. A prime number is usually used.
	inline iHashTable( uint32 numBuckets = 97 );

	/// Destroys the hash table
	inline ~iHashTable();

	/// Returns the number of elements in the table.
	inline uint32 Count() const;

	/// Inserts data into the hash table. Returns the iterator to the location where data was inserted.
	inline Iterator Insert( const T& data );

	/// Inserts data into the hash table. Duplicate keys are inserted near each other.
	inline Iterator InsertNear( const T& data );

	/// Remove item from the list
	inline Iterator Remove( const Iterator& it );

	/// Remove all elements
	inline void RemoveAll();

	/// Find an item given it's key value
	inline Iterator Find( iKEY key );
	/// Find an item given it's key value
	inline ConstIterator Find( iKEY key ) const;

	/// Find next matching item 
	inline Iterator FindNext( const Iterator& item );
	/// Find next matching item 
	inline ConstIterator FindNext( const ConstIterator& item ) const;

	/// Return the iterator to the first element
	inline Iterator First();
	/// Return the iterator to the first element
	inline ConstIterator First() const;

	/// Return the iterator to the last element
	inline Iterator Last();
	/// Return the iterator to the last element
	inline ConstIterator Last() const;

	/// Return the of the list ( not the last element )
	inline Iterator End();
	/// Return the of the list ( not the last element )
	inline ConstIterator End() const;


private:
	
	typedef iFastArray<T,SIZ> Bucket;
	typedef typename Bucket::Iterator BucketIterator;
	typedef typename Bucket::ConstIterator BucketConstIterator;

	/// A private iterator which contains the common secions to both the const and non-const iterators.
	class _Iterator
	{
	public:

		_Iterator()
		: hash(NULL)
		, bucket(0)
		{}

		_Iterator( const iHashTable* _hash )
		: hash( _hash )
		, bucket(0)
		{}

		_Iterator( const iHashTable* _hash, uint32 _bucket, const BucketIterator& _node )
		: hash( _hash )
		, bucket( _bucket )
		, node( _node )
		{}

		/// Copy another iterator
		_Iterator& operator = ( const _Iterator& other )
		{
			hash	= other.hash;
			bucket	= other.bucket;
			node	= other.node;
			return *this;
		}

		/// Set to the first
		void First( uint32 start = 0 )
		{
			node = hash->buckets[ bucket = start ].First();

			// if node end of bucket, check next one
			while ( node == hash->buckets[bucket].End() && bucket < hash->size - 1 )
				node = hash->buckets[++bucket].First();
		}

		/// Set iterator to the last node
		void Last()
		{
			node = hash->buckets[ bucket = hash->size-1 ].Last();

			// if node end of bucket, check prev one
			while ( node == hash->buckets[bucket].End() && bucket > 0 )
				node = hash->buckets[--bucket].Last();
		}

		/// Set iterator to the end
		void End()
		{
			node = hash->buckets[hash->size-1].End();
			bucket = 0;
		}

	protected:

		const iHashTable*		hash;
		uint32					bucket;
		BucketIterator		node;

		/// Increment by a number of nodes
		void Increment( sint32 n )
		{
			while ( n-- )
				Increment();
		}

		/// Decrement by a number of nodes
		void Decrement( sint32 n )
		{
			while ( n-- )
				Decrement();
		}

		/// Increment by one
		void Increment()
		{
			// next node
			++node;

			// reached the end of the list ?
			while ( node == hash->buckets[bucket].End() ) 
			{
				// end of table ?
				if (++bucket >= hash->size) 
					break;
				// get the first node from this bucket
				node = hash->buckets[bucket].First();
			}
		}

		/// Decrement by one
		void Decrement()
		{
			// prev node
			--node;

			// reached the end of the list ?
			while ( node == hash[buckets].End() ) 
			{
				// end of table ?
				if ( --bucket == 0 )
					break;
				// get the first node
				node = hash->buckets[bucket].First();
			}
		}
	};

	friend class _Iterator;

public:

	/// A non-const iterator
	class Iterator : public _Iterator
	{
	public:

		Iterator() {}

		/// Copy
		Iterator& operator = ( const Iterator& other )
		{
			_Iterator::operator = (other);
			return *this;
		}

		/// Compare two iterators
		bool operator == ( const Iterator& other ) const
		{
			return node == other.node;
		}

		/// Compare for difference
		bool operator != ( const Iterator& other ) const
		{
			return !operator == ( other );
		}

		/// Return contents
		T& operator * () const
		{
			return *node;
		}

		T& operator -> () const
		{
			return *node;
		}

		/// Increment 
		Iterator& operator += ( sint32 n )
		{
			Increment( n );
			return *this;
		}

		/// Decrement
		Iterator& operator -= ( sint32 n )
		{
			Decrement( n );
			return *this;
		}

		/// Prefix increment
		Iterator& operator ++ ()
		{
			Increment();
			return *this;
		}

		/// Prefix decrement
		Iterator& operator -- ()
		{
			Decrement();
			return *this;
		}

		/// Postfix increment
		Iterator operator ++ ( int )
		{
			Iterator it( *this );
			Increment();
			return it;
		}

		/// Postfix decrement
		Iterator operator -- ( int )
		{
			Iterator it ( *this );
			Decrement();
			return it;
		}

		/// Increments n nodes forward
		Iterator operator + ( sint32 n ) const
		{
			Iterator it (*this);
			return it += n;
		}

		/// Decrements n nodes backward
		Iterator operator - ( sint32 n ) const
		{
			Iterator it (*this);
			return it -= n;
		}

		/// Return node for debugging
		const T* _Node() const
		{
			return &(*node);
		}

	private:

		friend class iHashTable<iKEY,T,iHASH,SIZ>;

		Iterator( const iHashTable* hash, uint32 bucket, const BucketIterator& node )
		: _Iterator( hash, bucket, node )
		{}

		Iterator( const iHashTable* hash )
		: _Iterator( hash )
		{}
	};

	/// A constant iterator
	class ConstIterator : public _Iterator
	{
	public:

		ConstIterator() {}

		/// copy
		ConstIterator& operator = ( const ConstIterator& other )
		{
			_Iterator::operator = ( other );
			return *this;
		}

		/// Compare
		bool operator == ( const ConstIterator& other ) const
		{
			return node == other.node;
		}

		bool operator != ( const ConstIterator& other ) const
		{
			return ! operator == ( other );
		}

		/// Return contents 
		const T& operator * () const
		{
			return *node;
		}

		/// Return contents
		const T& operator -> () const
		{
			return *node;
		}

		/// Increment by number of nodes
		ConstIterator& operator += ( sint32 n )
		{
			Increment( n );
			return *this;
		}

		/// Decrement by number of nodes
		ConstIterator& operator -= ( sint32 n )
		{
			Decrement( n );
			return *this;
		}

		/// Prefix increment
		ConstIterator& operator ++ ()
		{
			Increment();
			return *this;
		}

		/// Prefix decrement
		ConstIterator& operator -- ()
		{
			Decrement();
			return *this;
		}

		/// Postfix increment
		ConstIterator operator ++ ( int )
		{
			ConstIterator it( *this );
			Increment();
			return it;
		}

		/// Postfix decrement
		ConstIterator operator -- ( int )
		{
			ConstIterator it ( *this );
			Decrement();
			return it;
		}

		/// Increments n nodes forward
		ConstIterator operator + ( sint32 n ) const
		{
			ConstIterator it (*this);
			return it += n;
		}

		/// Decrements n nodes backward
		ConstIterator operator - ( sint32 n ) const
		{
			ConstIterator it (*this);
			return it -= n;
		}

		/// Return node for debugging
		const T* _Node() const
		{
			return &(*node);
		}
		
	private:

		friend class iHashTable<iKEY,T,iHASH,SIZ>;

		ConstIterator( const iHashTable* hash, uint32 bucket, const BucketIterator& node )
		: _Iterator( hash, bucket, node )
		{}

		ConstIterator( const iHashTable* hash )
		: _Iterator( hash )
		{}
	};

private:

	Bucket*		buckets;
	uint32		count;
	uint32		size;

	/// Return the bucket number given the key.
	inline uint32 GetHash( iKEY key ) const;

	/// Return the bucket number given the data
	inline uint32 GetHash( T data ) const;
};

///////////////////////////////////////////////////////////////////
// Implementation

template<class iKEY, class T, class iHASH, UINT SIZ> 
iHashTable<iKEY,T,iHASH,SIZ>::iHashTable( uint32 _size )
:size( _size ), count( 0 )
{
	// allocate an array of buckets
	buckets = new Bucket[size];
}

template<class iKEY, class T, class iHASH, UINT SIZ> 
iHashTable<iKEY,T,iHASH,SIZ>::~iHashTable()
{
	// delete all buckets
	delete[] buckets;
}

template<class iKEY, class T, class iHASH, UINT SIZ> 
uint32 iHashTable<iKEY,T,iHASH,SIZ>::Count() const
{
	return count;
}

template<class iKEY, class T, class iHASH, UINT SIZ> 
typename iHashTable<iKEY,T,iHASH,SIZ>::Iterator iHashTable<iKEY,T,iHASH,SIZ>::Insert( const T& data )
{
	uint32 hash = GetHash( data );
	++count;
	buckets[hash].Add( data );
	return Iterator( this, hash, buckets[hash].Last() );
}

template<class iKEY, class T, class iHASH, UINT SIZ> 
typename iHashTable<iKEY,T,iHASH,SIZ>::Iterator iHashTable<iKEY,T,iHASH,SIZ>::InsertNear( const T& data )
{
	++count;
	uint32 hash = GetHash( data );

	Bucket* bucket =buckets + hash;
	BucketIterator node;
	iHASH hashFunc;
	for ( node = bucket->First(); node != bucket->End(); ++node ) 
	{
		if ( hashFunc( *node ) == hashFunc( data ) ) 
		{
			return Iterator( this, hash, bucket->InsertBefore( node, data ) );
		}
	}
	bucket->Add( data );
	return Iterator( this, hash, bucket->Last() );
}

template<class iKEY, class T, class iHASH, UINT SIZ> 
typename iHashTable<iKEY,T,iHASH,SIZ>::Iterator iHashTable<iKEY,T,iHASH,SIZ>::Remove( const Iterator& it )
{
	--count;

	// remove it and get next one
	BucketIterator bnext = buckets[it.bucket].Remove( it.node );

	// if next node not the end of bucket, return it
	if ( bnext != buckets[ it.bucket ].End() )
		return Iterator( this, it.bucket, bnext );

	// if more buckets remain, return node from the first one with node
	if ( it.bucket < size - 1 ) 
	{
		Iterator next( this );
		next.First( 1 + it.bucket );
		return next;
	}

	// None left
	return End();
}

template<class iKEY, class T, class iHASH, UINT SIZ> 
void iHashTable<iKEY,T,iHASH,SIZ>::RemoveAll()
{
	for ( uint32 i = 0; i < size; i++ ){
		buckets[i].RemoveAll();
	}

	count = 0;
}

template<class iKEY, class T, class iHASH, UINT SIZ> 
typename iHashTable<iKEY,T,iHASH,SIZ>::Iterator iHashTable<iKEY,T,iHASH,SIZ>::Find( iKEY key )
{
	uint32 hash = GetHash( key );
	Bucket* bucket = buckets + hash;
	BucketIterator node;
	iHASH hashFunc;

	for ( node = bucket->First(); node != bucket->End(); ++node )
		if ( hashFunc(*node) == key )
			return Iterator( this, hash, node );

	return End();
}

template<class iKEY, class T, class iHASH, UINT SIZ> 
typename iHashTable<iKEY,T,iHASH,SIZ>::ConstIterator iHashTable<iKEY,T,iHASH,SIZ>::Find( iKEY key ) const
{
	uint32 hash = GetHash( key );
	Bucket* bucket = buckets + hash;
	BucketIterator node;
	iHASH hashFunc;

	for ( node = bucket->First(); node != bucket->End(); ++node )
		if ( hashFunc(*node) == key )
			return ConstIterator( this, hash, node );

	return End();
}

template<class iKEY, class T, class iHASH, UINT SIZ> 
typename iHashTable<iKEY,T,iHASH,SIZ>::Iterator iHashTable<iKEY,T,iHASH,SIZ>::FindNext( const typename iHashTable<iKEY,T,iHASH,SIZ>::Iterator& item )
{
	ASSERT( item != End() );

	Bucket* bucket	= buckets + item.bucket;
	BucketIterator node = item.node;
	iHASH hashFunc;

	// walk to the next node
	for( ++node; node != bucket->End(); ++node )
		if ( hashFunc( *node ) == hashFunc( *item ) )
			return Iterator( this, item.bucket, node );
	
	return End();
}

template<class iKEY, class T, class iHASH, UINT SIZ> 
typename iHashTable<iKEY,T,iHASH,SIZ>::ConstIterator iHashTable<iKEY,T,iHASH,SIZ>::FindNext( const typename iHashTable<iKEY,T,iHASH,SIZ>::ConstIterator& item ) const
{
	ASSERT( item != End() );

	Bucket* bucket	= buckets + item.bucket;
	BucketIterator node = item.node;
	iHASH hashFunc;

	// walk to the next node
	for( ++node; node != bucket->End(); ++node )
		if ( hashFunc( *node ) == hashFunc( *item ) )
			return ConstIterator( this, item.bucket, node );
	
	return End();
}

//	inline Iterator FindNext( const Iterator& item );
//	inline ConstIterator FindNext( const ConstIterator& item ) const;

template<class iKEY, class T, class iHASH, UINT SIZ> 
typename iHashTable<iKEY,T,iHASH,SIZ>::Iterator iHashTable<iKEY,T,iHASH,SIZ>::First()
{
	Iterator it( this );
	it.First();
	return it;
}

template<class iKEY, class T, class iHASH, UINT SIZ> 
typename iHashTable<iKEY,T,iHASH,SIZ>::Iterator iHashTable<iKEY,T,iHASH,SIZ>::Last()
{
	Iterator it( this );
	it.Last();
	return it;
}

template<class iKEY, class T, class iHASH, UINT SIZ> 
typename iHashTable<iKEY,T,iHASH,SIZ>::Iterator iHashTable<iKEY,T,iHASH,SIZ>::End()
{
	Iterator it( this );
	it.End();
	return it;
}

template<class iKEY, class T, class iHASH, UINT SIZ> 
typename iHashTable<iKEY,T,iHASH,SIZ>::ConstIterator iHashTable<iKEY,T,iHASH,SIZ>::First() const
{
	ConstIterator it( this );
	it.First();
	return it;
}

template<class iKEY, class T, class iHASH, UINT SIZ> 
typename iHashTable<iKEY,T,iHASH,SIZ>::ConstIterator iHashTable<iKEY,T,iHASH,SIZ>::Last() const
{
	ConstIterator it( this );
	it.Last();
	return it;
}

template<class iKEY, class T, class iHASH, UINT SIZ>
typename iHashTable<iKEY,T,iHASH,SIZ>::ConstIterator iHashTable<iKEY,T,iHASH,SIZ>::End() const
{
	ConstIterator it( this );
	it.End();
	return it;
}

template<class iKEY, class T, class iHASH, UINT SIZ>
uint32 iHashTable<iKEY,T,iHASH,SIZ>::GetHash( iKEY key ) const
{
	return iHASH()(key) % size;
}

template<class iKEY, class T, class iHASH, UINT SIZ> 
uint32 iHashTable<iKEY,T,iHASH,SIZ>::GetHash( T data ) const
{
	return GetHash( iHASH()(data) );
}

#endif //_IOLIB_HELIUM_CONTAINER_HASH_H_
/** @} */ // iolib_he
