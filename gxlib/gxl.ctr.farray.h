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

#ifndef _GXLIB_CONTAINER_FAST_ARRAY_H_
#define _GXLIB_CONTAINER_FAST_ARRAY_H_

#define TEMPLATE		template <class T, uint32 SIZ>
#define QUAL			iFastArray<T,SIZ>

#define ASSERTF( a, b )

// The FastArray template implements a generic dynamic array container
// It is equivalent in implementation to the DynamicArray except that it
// does not properly handles storage of complex data types.
// Constructors and destructors NOT called to ensure data integrity at the expence of time
// It is used for data types which does not rely on a destructor/copy constructor.

template <class T, uint32 SIZ=13>
class iFastArray
{
	// Forwards
private:
	struct Chunk;
	enum { ChunkSizeConst = SIZ };

public:
	// Forwards
	class Iterator;
	class ConstIterator;

	// Constructor/destructor
	inline iFastArray();
	inline ~iFastArray();

	// Take a copy of another list
	inline iFastArray( const iFastArray& rvalue );

	// Append
	inline iFastArray& operator += ( const iFastArray& rvalue );

	// Assigment
	inline iFastArray& operator = ( const iFastArray& rvalue );

	///////////////////////////
	// Insertion / removal

	// Add item to the end of the list
	inline void Add( T item );

	// Add an item ( if it isn't already exists )
	inline void AddUnique( T item );

	// Remove item from the end.
	// Returns sentinal if there are no items to remove
	inline T Remove( T sentinal );

	// Push an item at the begining
	inline void Push( T item );

	// Push an item ( if it isn't already exists )
	inline void PushUnique( T item );

	// Pops an item from the start of the list, does not delete it.
	// Returns sentinal if none exists
	inline T Pop( T sentinal );

	// Insert node in order ( requires less than op )
	inline Iterator Insert( T node );

	// Insert node in order if it doesn't already exists
	inline Iterator InsertUnique( T node );

	// Insert node before 'beforeHere'
	inline Iterator Insert( const Iterator& beforeHere, T node );

	// Insert node after 'afterHere'
	inline Iterator InsertAfter( const Iterator& afterHere, T node );

	// Insert node before 'beforeHere'
	inline Iterator InsertBefore( const Iterator& beforeHere, T node );

	// Remove item from the list
	inline Iterator Remove( const Iterator& it );

	// Remove items fit->lit from the list
	inline void Remove( const Iterator& fit,  const Iterator& lit);

	// Remove item from the list
	inline Iterator Remove( uint32 n );

	// Remove last n items from the list
	inline void RemoveLast( uint32 n );

	// Remove item by iterator
	inline Iterator RemovePack( const Iterator& it );

	// Remove all
	inline void RemoveAll();

	/////////////////////////////
	// Access methods

	// Returns true if there are any valid nodes in the list
	inline operator bool() const;

	// Find an item, returns an iterator for it's location
	template <class K>
	Iterator Find( const K& item ) 
	{
		for( Iterator it(First()); it != End(); ++it ) 
		{
			if ( *it == item ) 	return it;
		}
		return End();
	}

	// Find an item, returns an iterator for it's location
	template <class K>
	ConstIterator Find( const K& item ) const 
	{
		for( ConstIterator it(First()); it != End(); ++it ) 
		{
			if ( *it == item )	return it;
		}
		return End();
	}

	// Find an item and remove it if exists, returns true if found.
	template <class K>
	bool FindRemove( const K& item ) 
	{
		Iterator found( Find( item ) );

		if ( found != End() ) 
		{
			Remove( found );
			return true;
		}
		return false;
	}

	// Return the element at 'index', asserts if out of bounds
	inline T& operator[] ( uint32 index );

	// Return the constant element at 'index'
	inline const T& operator[] ( uint32 index ) const;

	////////////////////////////
	// Iterator access methods

	// Returns an iterator for the first element.
	inline Iterator First();

	// Returns an const iterator for the first element.
	inline ConstIterator First() const;

	// Returns an iterator for the last element.
	inline Iterator Last();

	// Returns an const iterator for the last element.
	inline ConstIterator Last() const;

	// Returns an iterator for the end of the list ( not the last element )
	inline Iterator End();

	// Returns an const iterator for the end of the list ( not the last element )
	inline ConstIterator End() const;

	// Returns an iterator for the n-th element 
	inline Iterator Index( uint32 n );

	// Returns an const iterator for the n-th element 
	inline ConstIterator Index( uint32 n ) const;

	////////////////////////////
	// Information methods:

	// Return count of active nodes.
	inline uint32 GetSize() const;

	// Return true if list empty
	inline bool IsEmpty() const;

	// Return the position of this iterator
	inline uint32 Index( Iterator it ) const;

	// Return the position of this iterator
	inline uint32 Index( ConstIterator it ) const;

	/////////////////////////////
	// Misc list only methods:

	// Swap two elem
	inline void Swap( Iterator& it1, Iterator& it2 );

	// Returns true if node exists in the list
	inline bool Exists( const Iterator& it ) const;

	// Reset the list. This method removes all nodes from the list but
	// does __NOT__ destroy them !!!
	inline void Reset();

	// Sort the list in the increasing order.
	// This function requires the elements to have the
	// "less than" operator defined.
	// Selection sort.
	inline void Sort();

	//////////////////////////
	// Non constant iterator

	class Iterator 
	{
	public:
		// Def constructor
		Iterator() 
		{
			head = chunk = NULL;
			node = NULL;
		}
		// Construct iterator from the node
		Iterator( Chunk* aHead, Chunk* aChunk ) 
		{
			head = aHead;
			chunk= aChunk;
			if ( chunk ) node = &chunk->data[0];
			else node = NULL;
		}
		// Assignment 
		Iterator& operator = ( const Iterator &other )
		{
			head	= other.head;
			chunk	= other.chunk;
			node	= other.node;
			return *this;
		}
		// Compare iterator
		bool operator == ( const Iterator& other ) const
		{
			return node == other.node;
		}
		// Compare
		bool operator != ( const Iterator& other ) const
		{
			return !operator == ( other );
		}
		// Compare iterator
		bool operator == ( const ConstIterator& other ) const
		{
			return node == other.node;
		}
		// Compare
		bool operator != ( const ConstIterator& other ) const
		{
			return !operator == ( other );
		}
		// Compare with another node
		bool operator == ( const T& otherNode ) const
		{
			return (*node) == otherNode;
		}
		// Compare
		bool operator != ( const T& otherNode ) const
		{
			return !operator == ( otherNode );
		}
		// Return contents of iterator
		T& operator * () const
		{
			check( node != NULL );//, (TEXT("Can't reference to non-existant node")));
			return *node;
		}
		// Return contents of iterator
		T& operator -> () const
		{
			check( node != NULL );//, (TEXT("Can't reference to non-existant node")));
			return *node;
		}
		// Increment by number of nodes
		Iterator& operator += ( sint32 n )
		{
			while ( n-- ) operator++();
			return *this;
		}
		// Decrement by number of nodes
		Iterator& operator -= ( sint32 n )
		{
			while ( n-- ) operator--();
			return *this;
		}
		// Prefx increment to next
		Iterator& operator ++ ()
		{
			if ( node && ++node >= ( &chunk->data[chunk->count] ) ) 
			{
				chunk = chunk->next;
				if ( chunk != head ) node = &chunk->data[0];
				else node = NULL;
			}
			return *this;
		}
		// Postfx increment to next
		Iterator operator ++ ( int )
		{
			Iterator it(*this);
			operator++();
			return it;
		}
		// Increment n-th nodes forward
		Iterator operator + ( sint32 n )
		{
			Iterator it(*this);
			return it += n;
		}
		// Prefx decrement to prev
		Iterator& operator -- ()
		{
			if ( node && --node < ( &chunk->data[0] ) ) 
			{
				if ( chunk == head ) 
				{
					node = NULL;
				} 
				else 
				{
					chunk = chunk->prev;
					node = &chunk->data[chunk->count-1];	
				}
			}
			return *this;
		}
		// Postfx decrement ro prev
		Iterator operator -- (int)
		{
			Iterator it(*this);
			operator--();
			return it;
		}
		// Decrement n-th node backwards
		Iterator operator - ( sint32 n )
		{
			Iterator it(*this);
			while ( n-- ) --it;
			return it;
		}
	private:
		// Friends
		friend class iFastArray<T,SIZ>;
		friend class ConstIterator;

		// Private membrs

		T*		node;			// Pointer to the current node
		Chunk*	head;			
		Chunk*	chunk;			// Pointer to the current chunk

		// Private membrs
		Iterator( Chunk* aHead, Chunk* aChunk, T* aNode )
		{
			head	= aHead;
			chunk	= aChunk;
			node	= aNode;
		}
		Iterator( Chunk* aHead, Chunk* aChunk, uint32 n )
		{
			head	= aHead;
			chunk	= aChunk;
			node	= &chunk->data[n];
		}
	};
	
	////////////////////////
	// A constant iterator
	class ConstIterator 
	{
	public:
		// Def constructor
		ConstIterator()
		{
			head = chunk = NULL;
			node = NULL;
		}

		// Construct from a node
		ConstIterator( Chunk* aHead, Chunk* aChunk )
		{
			head	= aHead;
			chunk	= aChunk;
			if ( chunk ) node = &chunk->data[0];
			else node = NULL;
		}
		// Construct ConstIterator from Iterator
		ConstIterator( const Iterator& it )
		{
			head	= it.head;
			chunk	= it.chunk;
			node	= it.node;
		}
		// Assignment
		ConstIterator& operator = ( const Iterator& other )
		{
			head	= other.head;
			chunk	= other.chunk;
			node	= other.node;
			return *this;
		}
		// Assignment
		ConstIterator&	operator = ( const ConstIterator& other )
		{
			head	= other.head;
			chunk	= other.chunk;
			node	= other.node;
			return *this;
		}
		// Compare existing iterator
		bool	operator == ( const Iterator& other ) const
		{
			return node == other.node;
		}
		// Compare
		bool	operator != ( const Iterator& other ) const
		{
			return ! (operator == ( other ));
		}
		// Compare
		bool	operator == ( const ConstIterator& other ) const
		{
			return node == other.node;
		}
		// Compare
		bool	operator != ( const ConstIterator& other ) const
		{
			return ! (operator == ( other ));
		}
		// Compare with another node
		bool	operator == ( const T& otherNode ) const
		{
			return (*node) == otherNode;
		}
		// Compare
		bool	operator != ( const T& otherNode ) const
		{
			return !( operator == ( otherNode ));
		}
		// Return content
		const T& operator * () const
		{
			check( node != NULL );//, (TEXT("Can't reference non-existant node")));
			return *node;
		}
		// Return content
		const T& operator -> () const
		{
			check( node != NULL );//, (TEXT("Can't reference non-existant node")));
			return *node;
		}
		// Increment by number nodes
		ConstIterator& operator += ( sint32 n )
		{
			while (n--)	operator++();
			return *this;
		}
		// Decrement by number nodes
		ConstIterator& operator -= ( sint32 n )
		{
			while (n--)	operator--();
			return *this;
		}
		// Prefx increment to next
		ConstIterator& operator ++ ()
		{
			if ( node && ++node >= ( &chunk->data[chunk->count] )) 
			{
				chunk = chunk->next;
				if ( chunk != head ) node = &chunk->data[0];
				else node = NULL;
			}
			return *this;
		}
		// Postfix increment to next
		ConstIterator operator ++ ( int )
		{
			ConstIterator it(*this);
			operator++();
			return it;
		}
		// Increment n nodes forwards
		ConstIterator operator + ( sint32 n )
		{
			ConstIterator it(*this);
			return it += n;
		}
		// Prefx decrement to prev
		ConstIterator& operator -- ()
		{
			if ( node && --node < ( &chunk->data[0] ) ) 
			{
				if ( chunk == head ) 
				{
					node = NULL;
				} 
				else 
				{
					chunk = chunk->prev;
					node = &chunk->data[chunk->count-1];
				}
			}
			return *this;
		}
		// Postfx decrement to prev
		ConstIterator operator -- (int)
		{
			ConstIterator it(*this);
			operator--();
			return it;
		}
		// Decrement n node backwards
		ConstIterator operator - ( sint32 n )
		{
			ConstIterator it(*this);
			while( n-- ) --it;
			return it;
		}
	private:
		// Friends
		friend class iFastArray<T,SIZ>;
		friend class Iterator;

		// Private membrs
		const T*	node;
		Chunk*		head;
		Chunk*		chunk;

		// Private membrs
		ConstIterator( Chunk* aHead, Chunk* aChunk, const T* aNode )
		{
			node	= aNode;
			head	= aHead;
			chunk	= aChunk;
		}
		ConstIterator( Chunk* aHead, Chunk* aChunk, uint32 n )
		{
			head	= aHead;
			chunk	= aChunk;
			node	= &chunk->data[n];
		}
	};

private:
	// Header of chunk
	struct Chunk 
	{
		Chunk*	next;					// Next chunk
		Chunk*	prev;					// Previous chunk
		uint32	count;					// Number of elements in chunk
		T		data[SIZ];				// Array of data

		Chunk()	{ count = 0; }
	};

	///////////////////////
	// Private membrs

	Chunk* head;
};


/////////////////////////////////////////////////////////////////////
// Inlines


// DynamicArray
// Constructors
TEMPLATE QUAL::iFastArray()
{
	Reset();
}	

// ~DynamicArray
// Dtor
TEMPLATE QUAL::~iFastArray()
{
	RemoveAll();
}

// DynamicArray
// Take a copy
TEMPLATE QUAL::iFastArray( const iFastArray& rvalue )
{
	Reset();
	operator += ( rvalue );
}

// operator =:
// Assignment
TEMPLATE QUAL& QUAL::operator = ( const iFastArray& rvalue )
{
	if ( this ==&rvalue ) {
		return *this;
	}
	RemoveAll();
	operator += ( rvalue );
	return *this;
}

// operator +=
// Append
TEMPLATE QUAL& QUAL::operator += ( const iFastArray& rvalue )
{
	// nothing to copy ?
	if ( !rvalue.head )
		return *this;
	
	Chunk* last;
	Chunk* curr = rvalue.head;

	if ( head ) 
	{
		last = head->prev;
	} 
	else 
	{
		// make first chunk for this list
		last = head = new Chunk();
		head->prev = head;
		head->next = head;

		// Copy elements
		memcpy( &head->count, &curr->count, curr->count*sizeof(T)+sizeof(uint32));

		// next node
		curr = curr->next;

		if ( curr == rvalue.head ) return *this;
	}

	do 
	{
		Chunk* newChunk = new Chunk();

		// Copy elements
		memcpy( &newChunk->count, &curr->count, curr->count*sizeof(T)+sizeof(uint32));		

		// link it
		newChunk->next = head;
		newChunk->prev = last;

		head->prev = newChunk;
		head->next = newChunk;

		last = newChunk;

		curr = curr->next;
	} 
	while ( curr != rvalue.head );

	return *this;
}

/////////////////////////////////
// Insertion / removal

TEMPLATE void QUAL::Add( T item )
{
	//	pointer to last node
	Chunk* tail;

	// if no nodes, add a new chunk to the end
	if ( !head ) 
	{
		// create a first chunk
		tail = head = new Chunk();
		head->prev = head;
		tail->next = tail;
	} 
	else 
	{
		// end node
		tail = head->prev;

		if ( tail->count == SIZ ) 
		{
			// add a new chunk after tail
			Chunk* newEnd = new Chunk();

			newEnd->next = head;
			newEnd->prev = tail;

			head->prev = newEnd;
			tail->next = newEnd;

			tail = newEnd;
		}
	}

	// put the data in the chunk
	tail->data[tail->count++] = item;
}

// AddUnique
//

TEMPLATE void QUAL::AddUnique( T item )
{
	if ( Find( item ) == End() ) Add( item );
}

// Remove
// Remove an item from the end
TEMPLATE T QUAL::Remove( T sentinal )
{
	Chunk* c;

	// abort if no elems
	if ( !head )
		return sentinal;

	// last chunk
	c = head->prev;

	// take a copy of last elem
	T v = c->data[ --c->count ];

	if ( !c->count ) 
	{
		if ( c == head ) 
		{
			delete c;
			head = NULL;
		} 
		else 
		{
			Chunk* p = c->prev;
			p->next = head;
			head->prev = p;

			delete c;
		}
	}
	
	return v;
}

// Push:
//
TEMPLATE void QUAL::Push( T item )
{
	// if empty, add a new chunk to the end
	if ( !head ) 
	{
		head = new Chunk();
		head->next = head;
		head->prev = head;
	} 
	else 
	{
		// check for size
		if ( head->count == SIZ ) 
		{
			// add a new chunk
			Chunk* newFirst = new Chunk();

			newFirst->next = head;
			newFirst->prev = head->prev;
			
			head->prev->next = newFirst;
			head->prev = newFirst;

			head = newFirst;
		} 
		else 
		{
			// make a room
			// note: there will always be at least one node
			memmove( &head->data[1], &head->data[0], head->count*sizeof(T) );
		}
	}

	// increment number of nodes in the chunk
	head->count++;
	// call copy ctor
	head->data[0] = item;
}

// PushUnique:
//
TEMPLATE void QUAL::PushUnique( T item )
{
	if ( Find( item ) == End() ) Push( item );
}

// Pop:
//
TEMPLATE T QUAL::Pop( T sentinal )
{
	Iterator first( First() );
	if ( first == End() ) return sentinal;

	T data = (*first);

	Remove( first );

	return data;
}

// Insert:
// 
TEMPLATE typename QUAL::Iterator QUAL::Insert( T node )
{
	if ( !head ) 
	{
		Add( node );
		return First();
	}

	Iterator it( First() );

	while ( it != End() && *it < node )	++it;

	if ( it == End() ) 
	{
		Add( node );
		return Last();
	}

	return InsertBefore( it, node );
}

// InsertUnique:
//
TEMPLATE typename QUAL::Iterator QUAL::InsertUnique( T node )
{
	if ( !head ) 
	{
		Add( node );
		return First();
	}

	Iterator it( First() );

	while ( it != End() && *it < node )	++it;

	if ( it == End() ) 
	{
		Add( node );
		return Last();
	} 
	else if ( *it > node ) return InsertBefore( it, node );

	return End();
}

// Insert:
//
TEMPLATE typename QUAL::Iterator QUAL::Insert( const Iterator& beforeHere, T item )
{
	return InsertBefore( beforeHere, item );
}

// InsertAfter:
//
TEMPLATE typename QUAL::Iterator QUAL::InsertAfter( const Iterator& afterHere, T item )
{
	Iterator temp( afterHere );
	return InsertBefore( ++temp, item );
}

// InsertBefore:
//
TEMPLATE typename QUAL::Iterator QUAL::InsertBefore( const Iterator& beforeHere, T item )
{
	// if inserting before End() it means same as add
	if ( beforeHere == End() ) 
	{
		Add( item );
		return Last();
	}
	// if inserting before First, it sames as push
	if ( beforeHere == First() ) 
	{
		Push( item );
		return First();
	}

	// copy chunk ptr
	Chunk* c = beforeHere.chunk;

	// figure out which index we need to insert before
	uint32 index = beforeHere.node - (&c->data[0]);
	// inserting before the start of chunk ?
	if ( index == 0 && c->count == SIZ ) 
	{
		// insert at the end of the previous chunk
		// or insert a new chunk between this and previous for
		// a new node..
		Chunk* prev = c->prev;

		// is prev full ?
		if ( prev->count == SIZ || prev == head ) 
		{
			// create a new chunk and insert it
			Chunk* newChunk = new Chunk();

			// insert new chunk before 'c'
			newChunk->next = c;
			newChunk->prev = c->prev;

			c->prev->next = newChunk;
			c->prev = newChunk;

			prev = newChunk;
		}

		// add the new node at the end of the prev chunk
		// use a copy ctor to insert elem
		prev->data[prev->count++] = item;

		// return iterator to newly inserted node
		return Iterator( head, prev, &prev->data[ prev->count-1 ] );
	}

	// will it fit in this chunk ?
	if ( c->count == SIZ ) {
		// handle a full chunk lets split it into two chunks
		Chunk* nextChunk = new Chunk();

		// insert new chunk after 'c'
		nextChunk->prev	= c;
		nextChunk->next = c->next;

		c->next->prev	= nextChunk;
		c->next			= nextChunk;

		// move last half into first half of next chunk
		memmove(&nextChunk->data[0], &c->data[SIZ/2], (SIZ-SIZ/2)*sizeof(T) );
		
		// adjust counts
		c->count = SIZ/2;
		nextChunk->count = (SIZ-SIZ/2);

		// move next chunk to next block if index in 2nd half
		if ( index > SIZ/2 ) {
			c = nextChunk;
			index -= SIZ / 2;
		}
	}

	// there is room in this chunk
	memmove( &c->data[index+1], &c->data[index], (c->count-index)*sizeof(T) );
	// add the new node
	c->data[index] = item;
	c->count++;

	// return iterator
	return Iterator( head, c, &c->data[index] );
}

TEMPLATE void QUAL::Remove( const Iterator& fit,  const Iterator& lit)
{
	int dist = Index( lit ) - Index( fit );
	Iterator it = fit;
	while( dist-- ) {
		it = Remove( it );
	}
}

// Remove:
//
TEMPLATE typename QUAL::Iterator QUAL::Remove( const Iterator& it )
{
	if ( !head || it == End() )	return it;

	Chunk* c	= it.chunk;
	T* node		= it.node;

	// if not removing the last
	T* last = &c->data[c->count - 1];

	if ( node < last ) {
		// fill da gap
		memmove(node,node+1,(last - node) * sizeof(T));
	}

	// last one ?
	if ( c->count == 1 ) 
	{
		if ( c == head ) 
		{
			if ( c->next == head ) 
			{
				// only chunk left
				delete c;
				head = NULL;
				node = NULL;
				c = NULL;
			} 
			else 
			{
				// remove chunk
				c->prev->next = c->next;
				c->next->prev = c->prev;
				Chunk* t = c;
				c = c->next;

				delete t;
				node = &c->data[0];
				head = c;
			}
		} 
		else 
		{
			// remove the chunk
			c->prev->next = c->next;
			c->next->prev = c->prev;
			Chunk* t = c;
			c = c->next;

			delete t;
			node = ( c == head ) ? NULL : &c->data[0];
		}
	} 
	else 
	{
		c->count --;
		if ( node == last ) 
		{
			c = c->next;
			node = ( c == head ) ? NULL : &c->data[0];
		}
	}

	return Iterator( head, c, node );
}

// Remove:
// 
TEMPLATE typename QUAL::Iterator QUAL::Remove( uint32 n )
{
	Iterator it = First();
	it += n;
	return Remove( it );
}

// RemoveLast:
// remove last 'n' items
TEMPLATE void QUAL::RemoveLast( uint32 n )
{
	if ( head ) 
	{
		Chunk* last = head->prev;

		while ( last != head && last->count <= n ) 
		{
			// count off
			n -= last->count;
			// walk
			last = last->prev;
		}

		if ( n > SIZ ) 
		{
			RemoveAll();
			return;
		} 
		else if ( n ) 
		{
			// trim
			last->count -= n;
		}

		// remove all chunks following this one
		Chunk* curr = last->next;

		while ( curr != head ) 
		{
			Chunk* next = curr->next;

			delete curr;
			curr = next;
		}

		last->next = head;
		head->prev = last;
	}
}


// RemovePack:
// Remove the item specified by iterator and fill it's position with
// random node elsewhere in the list
TEMPLATE typename QUAL::Iterator QUAL::RemovePack( const Iterator& it )
{
	Chunk* c = it.chunk;

	if ( !head || it == End() )	return it;

	if ( c->count == 1 ) 
	{
		if ( head == c ) 
		{
			if ( c->next == head ) 
			{
				delete head;
				head = NULL;
				return End();
			} 
			else 
			{
				Iterator n( head, c->next );

				// unlink
				head = c->next;
				c->prev->next = c->next;
				c->next->prev = c->prev;
				delete c;

				return n;
			}
		}

		Iterator n( head, c->next );

		// unlink
		c->prev->next = c->next;
		c->next->prev = c->prev;
		delete c;

		return n;
	} 
	else 
	{
		T* node = it.node;
		
		// decr number of nodes
		c->count --;

		// if node not the last in the chunk
		if ( node != &c->data[ c->count ] ) 
		{
			// move last element to old node
			*node = &c->data[ c->count ];
			// Return the same iterator  ( new data )
			return it;
		} 
		else 
		{
			// return first element
			return Iterator( head, c->next );
		}
	}
}

// RemoveAll:
//
TEMPLATE void QUAL::RemoveAll()
{
	if ( head ) 
	{
		Chunk* first = head->next;

		while ( first != head ) {
			Chunk* next = first->next;
			delete first;
			first = next;
		}

		delete first;
		head = NULL;
	}
}

/////////////////////
// Access

// operator Bool()
//
TEMPLATE QUAL::operator bool() const
{
	return head ? true : false;
}

// Index:
// Return the iterator at the index'th item ( of End() )
TEMPLATE typename QUAL::Iterator QUAL::Index( uint32 index )
{
	if ( index >= Count() ) return End();

	return First() + index;
}

// Index:
//
TEMPLATE typename QUAL::ConstIterator QUAL::Index( uint32 index ) const
{
	if ( index >= Count() )	return End();

	return First() + index;
}

// operator []
// return the elem at index, asserts if out of bounds
TEMPLATE T& QUAL::operator[] ( uint32 index )
{
	check( index < GetSize() ); //, ("Out of bounds, index=%u", index));
	return *(First() + index);
}

// operator[]
//
TEMPLATE const T& QUAL::operator[] ( uint32 index ) const
{
	check( index < GetSize() );//, ("Out of bounds, index=%u", index));
	const T& ret = *(First() + index);
	return ret;
}

// First:
// return iterator to the first element
TEMPLATE typename QUAL::Iterator QUAL::First()
{
	return Iterator( head, head );
}

// First:
//
TEMPLATE typename QUAL::ConstIterator QUAL::First() const
{
	return ConstIterator( head, head );
}

// Last:
// return iterator to the last element
TEMPLATE typename QUAL::Iterator QUAL::Last()
{
	if ( head )	return Iterator( head, head->prev, &(head->prev->data[head->prev->count - 1 ]));
	return End();
}

// Last:
//
TEMPLATE typename QUAL::ConstIterator QUAL::Last() const
{
	if ( head )	return ConstIterator( head, head->prev, &(head->prev->data[head->prev->count - 1 ]));
	return End();
}

// End:
// return the end of the list ( not the last element )
TEMPLATE typename QUAL::Iterator QUAL::End()
{
	return Iterator( NULL, NULL );
}

// End:
//
TEMPLATE typename QUAL::ConstIterator QUAL::End() const
{
	return ConstIterator( NULL, NULL );
}

/////////////////////////////
// Information methods

// Count:
// count of active nodes
TEMPLATE uint32 QUAL::GetSize() const
{
	uint32 count = 0;
	Chunk* c = head;

	if ( c ) 
	{
		do 
		{
			count += c->count;
			c = c->next;
		} 
		while ( c != head );
	}
	return count;
}

// IsEmpty
//
TEMPLATE bool QUAL::IsEmpty() const
{
	return head == NULL;
}

// Index
// Returns the position of iterator
TEMPLATE uint32 QUAL::Index( Iterator it ) const
{
	return Index( ConstIterator( it ) );
}

// Index
//
TEMPLATE uint32 QUAL::Index( ConstIterator it ) const
{
	uint32 count = 0;
	for( ConstIterator j( First() ); j != it; ++j, ++count );
	return count;
}

/////////////////////////
// Misc list only mtds

// Exists
// Returns true if node exists
TEMPLATE bool QUAL::Exists( const Iterator& node ) const
{
	check( false )//, ("Exists(iterator) not implemented"));
	return false;
}

// Reset
//
TEMPLATE void QUAL::Reset()
{
	head = NULL;
}

// Swap
// Swap two elements
TEMPLATE void QUAL::Swap( Iterator& it1, Iterator& it2 )
{
	check( false );//, ("Swap( Iterator, Iterator) not implemented"));
}

/////////////////////////////
// Utility methods

// Sort:
//
TEMPLATE void QUAL::Sort()
{
	sint32 n = (sint32)Count();
	for( sint32 i = 0; i < n; i++ ) 
	{
		// Find smallest index
		sint32 min = i;
		for( uint32 j = i + 1; j < n; j++ )
			if ( operator[](j) < operator[](min) ) min = j;
		// Swap
		if ( i != min ) 
		{
			T tmp			= operator [](i);
			operator [](i)	= operator [](min);
			operator [](min)= tmp;
		}
	}
}

#undef QUAL
#undef TEMPLATE

#endif //_GXLIB_CONTAINER_FAST_ARRAY_H_

