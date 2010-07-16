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

#ifndef _GXLIB_CONTAINER_LIST_H_
#define _GXLIB_CONTAINER_LIST_H_

struct iIListNode {
	iIListNode*	_Next;
	iIListNode*	_Prev;

	inline iIListNode();
};

// Double linked list class
template< class T >
class iIList
{
public:
	
	class Iterator;
	class ConstIterator;

	// Ctor
	inline iIList();

	// Dtor
	inline ~iIList();

	/////////////////////////////////////
	// Insertion / removal

	// Add an item to the end of the list
	// Returns iterator where the item was inserted
	inline Iterator Add( T* item );

	// Add an item ( if it doesn't already exists )
	inline Iterator AddUnique( T* item );

	// Remove an item from the end of the list
	// Returns NULL if there no items to remove
	inline T* Remove();

	// Push an item at the beginning
	// Returns the iterator hwere the item was inserted
	inline Iterator Push( T* item );

	// Push an item ( if it doesn't already exists )
	inline Iterator PushUnique( T* item );

	// Pops an item from the start of the list, does not delete it.
	// Returns NULL if none exists
	inline T* Pop();

	// Insert node in order
	inline Iterator Insert( T* node );

	// Insert node in order if it does't already exists
	inline Iterator InsertUnique( T* node );

	// Insert node before 'beforeHere'
	inline Iterator Insert( const Iterator& beforeHere, T* node );

	// Insert node after 'afterHere'
	inline Iterator InsertAfter( const Iterator& afterHere, T* node );

	// Insert node before 'beforeHere'
	inline Iterator InsertBefore( const Iterator& beforeHere, T* node );

	// Remove an item from the list
	inline Iterator Remove( const Iterator& it );

	// Remove an item from the list
	inline Iterator Remove( uint32 n );

	// Remove all nodes 
	void RemoveAll();

	//////////////////////////////////////
	// Special instrusive only methods

	// Free an item 
	inline Iterator Delete( Iterator& it );

	// Free an item
	inline Iterator Delete( uint32 n );

	// Delete all nodes
	void DeleteAll();

	////////////////////////////////////////
	// Access methods

	// Find an item
	// An iterator of this class returns a poInter to the item so we
	// need to dereference it twice
	inline Iterator Find( T* item );

	// Find an item
	inline ConstIterator Find( const T* item ) const;

	// Return the element at 'index'
	inline T& operator [] ( uint32 index );

	// Return the element
	inline const T& operator [] ( uint32 index ) const;

	/////////////////////////////////
	// Iterator access methods

	// Return an iterator to the first element
	inline Iterator First();

	// Return an const iterator to the first element
	inline ConstIterator First() const;

	// Return an iterator to the last element
	inline Iterator Last();

	// Return an const iterator to the last element
	inline ConstIterator Last() const;

	// Return the end of the list
	inline Iterator End();

	// Return the end of the list
	inline ConstIterator End() const;

	// Return an iterator for the n-th element
	inline Iterator Index( uint32 n );

	// Return an const iterator for the n-th element
	inline ConstIterator Index( uint32 n ) const;

	//////////////////////////////////
	// Information methods

	// Number of nodes
	inline uint32 Count() const;

	// True if nothing in the list
	inline bool IsEmpty() const;

	// Return position of iterator
	inline uint32 Index( Iterator it ) const;

	// 
	uint32 Index( ConstIterator it ) const;

	////////////////////////////////////
	// List only methods

	// Swap two elements
	void Swap( Iterator& n1, Iterator& n2 );

	// Return True if node exists
	bool Exists( const Iterator& node ) const;

	// Reset the list
	inline void Reset();

	// A non-const iterator
	class Iterator 
	{
	public:
		// Ctor
		Iterator( T* aNode = NULL )
		{ 
			node = aNode; 
		}
		// Compare existing
		bool operator == ( const ConstIterator& other ) const 
		{ 
			return node == other.node; 
		}
		// Compare existing
		bool operator != ( const ConstIterator& other ) const
		{ 
			return ! operator == ( other ); 
		}
		// Compare existing
		bool operator == ( const Iterator& other ) const
		{ 
			return node == other.node; 
		}
		// Compare existing
		bool operator != ( const Iterator& other ) const
		{ 
			return ! operator == ( other ); 
		}
		// Compare with another node
		bool operator == ( T* otherNode ) const
		{ 
			return node == otherNode; 
		}
		// Compare with another node
		bool operator != ( T* otherNode ) const
		{ 
			return ! operator == ( otherNode ); 
		}
		// Return contents of iterator. 
		// NOTE: this does not return a reference coz the node is already
		// a poInter to the node and we want to be able to use -> operator
		// on the returned value in the same way a dynamic array functions
		T*	operator * () const
		{ 
			return node; 
		}
		// Return contents of iterator. 
		// NOTE: this does not return a reference coz the node is already
		// a poInter to the node and we want to be able to use -> operator
		// on the returned value in the same way a dynamic array functions
		T* operator -> () const
		{ 
			return node; 
		}
		// Prefx increment
		Iterator& operator ++ ()
		{ 
			node = (T*)node->_Next; return *this; 
		}
		// Postfx increment
		Iterator operator ++ ( int )
		{ 
			Iterator it( node ); node = (T*)node->_Next; return it; 
		}
		// Increment n nodes forward
		Iterator operator + ( sint32 n )
		{ 
			Iterator it( node ); while ( n-- ) ++it; return it; 
		}
		// Prefx decrement
		Iterator& operator -- ()
		{ 
			node = (T*)node->_Prev; return *this; 
		}
		// Postfx decrement
		Iterator operator -- ( int )
		{ 
			Iterator it( node ); node = (T*)node->_Prev; return it; 
		}
		// Decrement n nodes backwards
		Iterator operator - ( sint32 n )
		{ 
			Iterator it( node ); while ( n-- ) --it; return it; 
		}

	private:
		friend class ConstIterator;	
		T*		node;
	};

	// A const iterator
	class ConstIterator
	{
	public:
		// Ctor
		ConstIterator( const T* aNode = NULL )
		{ 
			node = aNode; 
		}
		// Ctor
		ConstIterator( const Iterator& other )
		{ 
			node = other.node; 
		}
		// Compare existing
		bool operator == ( const Iterator& other ) const 
		{ 
			return node == other.node; 
		}
		// Compare existing
		bool operator != ( const Iterator& other ) const
		{ 
			return ! operator == ( other ); 
		}
		// Compare existing
		bool operator == ( const ConstIterator& other ) const
		{ 
			return node == other.node; 
		}
		// Compare existing
		bool operator != ( const ConstIterator& other ) const
		{ 
			return ! operator == ( other ); 
		}
		// Compare with another node
		bool operator == ( T* otherNode ) const
		{ 
			return node == otherNode; 
		}
		// Compare with another node
		bool operator != ( T* otherNode ) const
		{ 
			return ! operator == ( otherNode ) ; 
		}
		// Return contents of iterator. 
		// NOTE: this does not return a reference coz the node is already
		// a poInter to the node and we want to be able to use -> operator
		// on the returned value in the same way a dynamic array functions
		const T* operator * () const
		{ 
			return node; 
		}
		// Return contents of iterator. 
		// NOTE: this does not return a reference coz the node is already
		// a poInter to the node and we want to be able to use -> operator
		// on the returned value in the same way a dynamic array functions
		const T* operator -> () const
		{ 
			return node; 
		}
		// Prefx increment
		ConstIterator& operator ++ () 
		{ 
			node = (T*)node->_Next; 
			return *this; 
		}
		// Postfx increment
		ConstIterator operator ++ ( int )
		{ 
			ConstIterator it( node ); 
			node = (T*)node->_Next; 
			return it; 
		}
		// Increment by n
		ConstIterator operator + ( sint32 n )
		{ 
			ConstIterator it( node ); 
			while ( n-- ) ++it; 
			return it; 
		}
		// Prefx decrement
		ConstIterator& operator -- ()
		{ 
			node = (T*)node->_Prev; 
			return *this; 
		}
		// Postfx decrement
		ConstIterator operator -- ( int )
		{ 
			ConstIterator it( node ); 
			node = (T*)node->_Prev; 
			return it; 
		}
		// Decrement by n
		ConstIterator operator - ( sint32 n )
		{ 
			ConstIterator it( node ); 
			while ( n-- ) --it; 
			return it; 
		}
	private:
		friend class Iterator;
		const T*		node;
	};

	protected:

		iIListNode		sentinel;		// Head & tail
		uint32			active;			// Number of nodes

	private:

		// Internal
		inline void IRemove( const Iterator& it );
		inline void IDelete( const Iterator& it );
};

///////////////////////////////////////////////////////////////
// Implementation of IListNode

// Ctor
inline iIListNode::iIListNode()
{
	_Next = _Prev = NULL;
}

///////////////////////////////////////////////////////////////
// Implementation of IList

// operator =:
template< class T>
inline bool operator == ( T* node, const typename iIList<T>::Iterator& other )
{
	return other == node;
}

template< class T >
inline bool operator != ( T* node, const typename iIList<T>::Iterator& other )
{
	return other != node;
}

// IList<T>
template< class T >
iIList<T>::iIList()
{
	Reset();
}

// ~IList<T>
template< class T >
iIList<T>::~iIList()
{
	iIList<T>::DeleteAll();
}

// Add
template< class T >
typename iIList<T>::Iterator iIList<T>::Add( T* item )
{
	return Insert( End(), item );
}

// AddUnique
template< class T >
typename iIList<T>::Iterator iIList<T>::AddUnique( T* item )
{
	if ( Find(item) == End() ) return Add( item );
	return End();
}

// Push
template< class T >
typename iIList<T>::Iterator iIList<T>::Push( T* item )
{
	return Insert( First(), item );
}

// PushUnique
template< class T >
typename iIList<T>::Iterator iIList<T>::PushUnique( T* item )
{
	if ( Find(item) == End() )	return Push( item );
	return End();
}

// RemoveAll
template< class T >
inline void iIList<T>::RemoveAll()
{
	while ( Count() ) IRemove( First() );
}

// DeleteAll
template< class T >
void iIList<T>::DeleteAll()
{
	while ( Count() ) IDelete( First() );
}

// Pop
template< class T >
T* iIList<T>::Pop()
{
	if ( !Count() )	return NULL;

	Iterator first = First();

	iIList<T>::IRemove( First() );

	return *first;
}

// Remove
template< class T >
T* iIList<T>::Remove() 
{
	if ( !Count() )	return NULL;

	Iterator last = Last();

	iIList<T>::IRemove( last );

	return *last;
}

// Delete
template< class T >
typename iIList<T>::Iterator iIList<T>::Delete( Iterator& node )
{
	check( node != End() );
	check( Exists(node) );

	Iterator next( node );

	// inc
	++next;

	iIList<T>::IDelete( node );

	return next;
}

// Delete
template< class T >
typename iIList<T>::Iterator iIList<T>::Delete( uint32 n )
{
	Iterator it = Index( n );
	return Delete( it );
}

// IDelete
template< class T >
void iIList<T>::IDelete( const Iterator& it )
{
	iIList<T>::IRemove( it );
	delete *it;
}

// Remove
template< class T >
typename iIList<T>::Iterator iIList<T>::Remove( const Iterator& node )
{
	check( Exists( node ) );
	check( node != End() );

	// keep node
	Iterator next( node );
	++next;

	// unlink
	iIList<T>::IRemove( node );
	
	return next;
}

// Remove
template< class T >
typename iIList<T>::Iterator iIList<T>::Remove( uint32 n )
{
	return Remove( Index( n ) );
}

// IRemove
template< class T >
void iIList<T>::IRemove( const Iterator& it )
{
	it->_Prev->_Next = it->_Next;
	it->_Next->_Prev = it->_Prev;
	it->_Next = it->_Prev = NULL;
	active--;
}

// InsertAfter
template< class T >
inline typename iIList<T>::Iterator iIList<T>::InsertAfter( const Iterator& afterHere, T* node )
{
	check( node );
	check( (!node->_Next || !node->_Prev) );
	check( First() != node );
	check( *afterHere );

	// insert new node
	node->_Prev = *afterHere;
	node->_Next = afterHere->_Next;
	afterHere->_Next->_Prev = node;
	afterHere->_Next = node;
	active++;

	return Iterator( node );
}

// InsertUnique
template< class T >
typename iIList<T>::Iterator iIList<T>::InsertUnique( T* node )
{
	if ( Find( node ) == End() ) return Insert( node );
	return End();
}

// insert before
template< class T>
inline typename iIList<T>::Iterator iIList<T>::Insert( const Iterator& beforeHere, T* node )
{
	return InsertBefore( beforeHere, node );
}

// Insert node 'beforeHere'
template< class T >
inline typename iIList<T>::Iterator iIList<T>::InsertBefore( const Iterator& beforeHere, T* node )
{
	check( node );
	check( (!node->_Next || !node->_Prev) );
	check( First() != node );
	check( *beforeHere );

	// insert
	node->_Next = *beforeHere;
	node->_Prev = beforeHere->_Prev;
	beforeHere->_Prev->_Next = node;
	beforeHere->_Prev = node;
	active++;

	return Iterator( node );
}


// Exists
template< class T >
bool iIList<T>::Exists( const Iterator& it ) const
{
	for ( ConstIterator i = First(); i != End(); ++i )
	{
		if ( it == i )	return true;
	}
	return false;
}

// Insert
template< class T >
typename iIList<T>::Iterator iIList<T>::Insert( T* node )
{
	Iterator it = First();
	for( ; it != End() && (**it) < (*node); ) ++it;
	return InsertBefore( it, node );
}

// Swap
template< class T >
void iIList<T>::Swap( Iterator& it1, Iterator& it2 )
{
	T* swap;

	swap = (T*)(it1->_Prev);
	it1->_Prev = it2->_Prev;
	it2->_Prev = swap;
	swap = (T*)(it1->_Next);
	it1->_Next = it2->_Next;
	it2->_Next = swap;
	swap = *it1;
	it1 = Iterator( it2 );
	it2 = Iterator( swap );
}

// Index
template< class T >
typename iIList<T>::Iterator iIList<T>::Index( uint32 n ) 
{
	return Iterator( First() + n );
}


// Index
template< class T >
typename iIList<T>::ConstIterator iIList<T>::Index( uint32 n ) const
{
	return ConstIterator( First() + n );
}

// Index
template< class T >
uint32 iIList<T>::Index( ConstIterator it ) const
{
	uint32 count = 0;

	while ( it != End() ) 
	{
		++count;
		--it;
	}
	return count;
}

// Find
template< class T >
typename iIList<T>::Iterator iIList<T>::Find( T* item )
{
	Iterator item_it( item );
	for( Iterator it(First()); it != End(); ++it )
	{
		if ( it == item_it ) return it;
	}
	return End();
}

// Find
template< class T >
typename iIList<T>::ConstIterator iIList<T>::Find( const T* item ) const
{
	ConstIterator item_it( item );
	for( ConstIterator it(First()); it != End(); ++it )
	{
		if ( it == item_it ) return it;
	}
	return End();
}

// operator[]
template< class T >
T& iIList<T>::operator [] ( uint32 index )
{
	check( index < Count() );
	return **(First() + index);
}

// operator[]
template< class T >
const T& iIList<T>::operator [] ( uint32 index ) const
{
	ASSERT( index < Count() );
	return **(First() + index);
}

// First
template< class T >
typename iIList<T>::Iterator iIList<T>::First()
{
	return Iterator( (T*)sentinel._Next );
}

// First
template< class T >
typename iIList<T>::ConstIterator iIList<T>::First() const
{
	return ConstIterator( (T*)sentinel._Next );
}

// Last
template< class T >
typename iIList<T>::Iterator iIList<T>::Last()
{
	return Iterator( (T*)sentinel._Prev );
}

// Last
template< class T >
typename iIList<T>::ConstIterator iIList<T>::Last() const
{
	return ConstIterator( (T*)sentinel._Prev );
}

// End
template< class T >
typename iIList<T>::Iterator iIList<T>::End() 
{
	return Iterator( (T*)&sentinel );
}

// End
template< class T >
typename iIList<T>::ConstIterator iIList<T>::End() const
{
	return ConstIterator( (T*)&sentinel );
}

// Count
template< class T >
uint32 iIList<T>::Count() const
{
	return active;
}

// IsEmpty
template< class T >
bool iIList<T>::IsEmpty() const
{
	return Count() == 0;
}

// Index
template< class T >
uint32 iIList<T>::Index( Iterator it ) const
{
	return Index( ConstIterator( it ) );
}

// Reset
template< class T >
void iIList<T>::Reset()
{
	sentinel._Next = sentinel._Prev = &sentinel;
	active = 0;
}

#endif //_GXLIB_CONTAINER_LIST_H_

