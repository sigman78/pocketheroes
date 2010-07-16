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
 *	@brief Tree container template class implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_CONTAINER_TREE_H_
#define _IOLIB_HELIUM_CONTAINER_TREE_H_

/// Tree container template class
template< class T >
class iTree
{
public:

	// Forwards
	class Iterator;
	class ConstIterator;

	// Friends
	friend class Iterator;
	friend class ConstIterator;

	/// Constructor
	inline iTree();

	/// Destructor
	inline ~iTree();

	/// Gets the number of all child nodes immediately below this one
	inline uint32 Count() const;

	/// Get the number of all nodes
	inline uint32 CountAll() const;

	/// Set the current parent for the given node
	inline void SetParent( T* parentNode );

	/// Adds a node the the end of the child list of this node
	inline Iterator Add( T* node );

	/// Removes a node from the tail
	inline T* Remove();

	/// Pushes node onto the head
	inline Iterator Push( T* node );

	/// Pops a node
	inline T* Pop();

	/// Inserts a node in to the middle of the child list before
	inline Iterator InsertBefore( Iterator& target, T* node );

	/// Inserts a node into the middle, after 
	inline Iterator InsertAfter( Iterator& target, T* node );

	/// Removes given node
	inline bool Remove( T* node );

	/// Removes given node from the child list
	inline Iterator Remove( Iterator& it );

	/// Removes all nodes from the current child list, does not destroy it
	void RemoveAll();

	/// Removes all nodes w/o destroying
	void RemoveAllChildren();
	
	/// Removes all nodes from the child list and destroy them
	void DeleteAll();

	/// Iterator to the end of a list
	inline Iterator End();
	/// Iterator to the end of a list
	inline ConstIterator End() const;

	/// Iterator to the parent of this node
	inline Iterator Parent();
	/// Iterator to the parent of this node
	inline ConstIterator Parent() const;

	/// Traverse the tree in depth-first order and calls given method
	void Traverse( void (*func)(T *) );
	/// Traverse the tree in depth-first order and calls given method
	void Traverse( void (*func)(const T *) ) const;

	/// Gets the iterator for the next node on this level
	inline Iterator Next();
	/// Gets the iterator for the next node on this level
	inline ConstIterator Next() const;

	/// Gets the iterator for the prev node on this level
	inline Iterator Prev();
	/// Gets the iterator for the prev node on this level
	inline ConstIterator Prev() const;

	/// Get the iterator for the first child node
	inline Iterator First();
	/// Get the iterator for the first child node
	inline ConstIterator First() const;

	/// Get the iterator for the last child node
	inline Iterator Last();
	/// Get the iterator for the last child node
	inline ConstIterator Last() const;

	/// Gets the iterator for the nth node in the child list
	inline Iterator Index( DWORD n );
	/// Gets the iterator for the nth node in the child list
	inline ConstIterator Index( DWORD n ) const;

	/// Determine, if the given iterator is on the list
	bool Exists( const Iterator& it ) const;
	/// Determine, if the given iterator is on the list
	bool Exists( const ConstIterator& it ) const;

	/// Retreive node's depth (0 - root)
	uint32 GetDepth() const
	{
		uint32 rval = 0;
		T* step = _parent;
		while (step) {
			step = step->_parent;
			++rval;
		}
		return rval;
	}

	/// Iterator class
	class Iterator 
	{
		friend class ConstIterator;
	public:
		
		/// Constructs a new iterator from the given node
		Iterator( T* _node = NULL )
		{
			node = _node;
		}

		/// Determines if a given iterator is equivalent to this one
		bool operator == ( const Iterator& other ) const
		{
			return node == other.node;
		}

		/// Determines if a given iterator is equivalent to this one
		bool operator == ( const ConstIterator& other ) const
		{
			return node == other.node;
		}

		/// Determines if a given iterator is not equivalent to this one
		bool operator != ( const Iterator& other ) const
		{
			return ! operator == ( other );
		}

		/// Determines if a given iterator is not equivalent to this one
		bool operator != ( const ConstIterator& other ) const
		{
			return ! operator == ( other );
		}

		/// Determines if given node is equivalent to the one refered by the iterator
		bool operator == ( const T* otherNode ) const
		{
			return node == otherNode;
		}

		/// Determines if given node is not equivalent to the one refered by the iterator
		bool operator != ( const T* otherNode ) const
		{
			return ! operator == ( otherNode );
		}

		/// Gets a pointer to the contents of the node that iterator points to
		T* operator * () const { return node; }
		/// References a node that this iterator points to
		T* operator -> () const { return node; }

		/// Moves this iterator along to the next child node
		Iterator& operator ++ ()	{ node = node->_next; return *this; }
		/// Moves this iterator along to the next child node
		Iterator  operator ++ (sint)	{ Iterator i(node); node = node->_next; return i; }
		/// Moves the iterator forward n nodes
		Iterator operator + ( sint32 n ) { Iterator i(node); while ( n-- ) ++i; return i; }
		
		/// Moves this iterator back to previous child node
		Iterator& operator -- ()	{ node = node->_prev; return *this; }
		/// Moves this iterator back to previous child node
		Iterator  operator -- (sint)	{ Iterator i(node); node = node->_prev; return i; }
		/// Moves the iterator backward n nodes
		Iterator operator - ( sint32 n ) { Iterator i(node); while ( n-- ) --i; return i; }

		/// Moves iterator up to the parent node
		Iterator& Up()			{ node = node->_parent; return *this; }
		/// Moves iterator down to the first child node
		Iterator& DownFirst()	{ node = node->_head; return *this; }
		/// Moves iterator down to the last child node
		Iterator& DownLast()	{ node = node->_tail; return *this; }

		/// Gets the next node in the tree using depth-first traversal
		Iterator& WalkNext( const T* endNode = NULL )
		{
			check( node ); //, (TEXT("WalkNext on null iterator")) );
			if ( node->Count() )
				*this = node->First();
			else if ( node->_next )
				*this = node->Next();
			else {
				T* newNode = node->_parent;
				while ( newNode ) {
					if ( endNode && ( newNode == endNode || newNode->_next == endNode ) ) {
						newNode = NULL;
					} else {
						if ( newNode->_next ) {
							*this = Iterator( newNode->_next );
							return *this;
						}
						newNode = newNode->_parent;
					}
				}
				*this = Iterator( NULL );
			}
			if ( endNode && *this == endNode )
				*this = Iterator( NULL );
			return *this;
		}

		/// Gets the previous node in the tree using a depth-first traversal
		Iterator& WalkPrev( const T* endNode = NULL )
		{
			check( node ); //, (TEXT("WalkPrev on null iterator")) );
			if ( node->Count() )
				*this = node->Last();
			else if ( node->_prev )
				*this = node->Prev();
			else {
				T* newNode = node->_parent;
				while ( newNode ) {
					if ( endNode && ( newNode == endNode || newNode->_prev == endNode )) {
						newNode = NULL;
					} else {
						if ( newNode->_prev ) {
							*this = Iterator( newNode->_prev );
							return *this;
						}
						newNode = newNode->_parent;	
					}
				}
				*this = Iterator( NULL );
			}
			if ( endNode && *this == endNode ) 
				*this = Iterator( NULL );
			return *this;
		}
	private:
		T*		node;
	};

	/// Constant iterator class
	class ConstIterator 
	{
		friend class Iterator;

	public:
		
		/// Constructs a new iterator from the given node
		ConstIterator( T* _node = NULL )
		{
			node = _node;
		}

		/// Constructs a new iterator from the given node
		ConstIterator( const Iterator& other )
		{
			node = other.node;
		}

		/// Determines if a given iterator is equivalent to this one
		bool operator == ( const Iterator& other ) const
		{
			return node == other.node;
		}

		/// Determines if a given iterator is equivalent to this one
		bool operator == ( const ConstIterator& other ) const
		{
			return node == other.node;
		}

		/// Determines if a given iterator is not equivalent to this one
		bool operator != ( const Iterator& other ) const
		{
			return ! operator == ( other );
		}

		/// Determines if a given iterator is not equivalent to this one
		bool operator != ( const ConstIterator& other ) const
		{
			return ! operator == ( other );
		}

		/// Determines if given node is equivalent to the one refered by the iterator
		bool operator == ( const T* otherNode ) const
		{
			return node == otherNode;
		}

		/// Determines if given node is not equivalent to the one refered by the iterator
		bool operator != ( const T* otherNode ) const
		{
			return ! operator == ( otherNode );
		}

		/// Gets a pointer to the contents of the node that iterator points to
		const T* operator * () const { return node; }
		/// References a node that this iterator points to
		const T* operator -> () const { return node; }

		/// Moves this iterator along to the next child node
		ConstIterator& operator ++ ()	{ node = node->_next; return *this; }
		/// Moves this iterator along to the next child node
		ConstIterator  operator ++ (sint)	{ ConstIterator i(node); node = node->_next; return i; }
		/// Moves the iterator forward n nodes
		ConstIterator operator + ( sint32 n ) { ConstIterator i(node); while ( n-- ) ++i; return i; }
		
		/// Moves this iterator back to previous child node
		ConstIterator& operator -- ()		{ node = node->_prev; return *this; }
		/// Moves this iterator back to previous child node
		ConstIterator  operator -- (sint)	{ Iterator i(node); node = node->_prev; return i; }
		/// Moves the iterator backward n nodes
		ConstIterator operator - ( sint32 n ) { ConstIterator i(node); while ( n-- ) --i; return i; }

		/// Moves iterator up to the parent node
		ConstIterator& Up()			{ node = node->_parent; return *this; }
		/// Moves iterator down to the first child node
		ConstIterator& DownFirst()	{ node = node->_head; return *this; }
		/// Moves iterator down to the last child node
		ConstIterator& DownLast()	{ node = node->_tail; return *this; }

		/// Gets the next node in the tree using depth-first traversal
		ConstIterator& WalkNext( const T* endNode = NULL )
		{
			check( node ); //, (TEXT("WalkNext on null iterator")) );
			if ( node->Count() )
				*this = node->First();
			else if ( node->_next )
				*this = node->Next();
			else {
				T* newNode = node->_parent;
				while ( newNode ) {
					if ( endNode && ( newNode == endNode || newNode->_next == endNode ) ) {
						newNode = NULL;
					} else {
						if ( newNode->_next ) {
							*this = ConstIterator( newNode->_next );
							return *this;
						}
						newNode = newNode->_parent;
					}
				}
				*this = ConstIterator( NULL );
			}
			if ( endNode && *this == endNode )
				*this = ConstIterator( NULL );
			return *this;
		}

		/// Gets the previous node in the tree using a depth-first traversal
		ConstIterator& WalkPrev( const T* endNode = NULL )
		{
			check( node ); //, (TEXT("WalkPrev on null iterator")) );
			if ( node->Count() )
				*this = node->Last();
			else if ( node->_prev )
				*this = node->Prev();
			else {
				T* newNode = node->_parent;
				while ( newNode ) {
					if ( endNode && ( newNode == endNode || newNode->_prev == endNode )) {
						newNode = NULL;
					} else {
						if ( newNode->_prev ) {
							*this = ConstIterator( newNode->_prev );
							return *this;
						}
						newNode = newNode->_parent;	
					}
				}
				*this = ConstIterator( NULL );
			}
			if ( endNode && *this == endNode ) 
				*this = ConstIterator( NULL );
			return *this;
		}
	private:
		T*		node;
	};

	inline const T* GetNextPtr() const{
		return _next;
	}

	inline const T* GetPrevPtr() const{
		return _prev;
	}
	
	////
protected:
	uint32	_count;
	T*		_parent;
	T*		_head;
	T*		_tail;
	T*		_next;
	T*		_prev;
private:
	inline void IRemove( Iterator target );
	inline void IDelete( Iterator target );
};

/////////////////////////////////////////////////////////////////////////
// Implementation
template< class T >
inline iTree<T>::iTree<T>()
{
	_next = _prev = _head = _tail = _parent = NULL;
	_count = 0;
}

template< class T >
inline iTree<T>::~iTree<T>()
{
	DeleteAll();
}

template< class T >
inline uint32 iTree<T>::Count() const
{
	return _count;
}

template< class T >
inline uint32 iTree<T>::CountAll() const
{
	uint32 total = Count();
	ConstIterator it = First();
	while ( it != End() ) {
		total += it->Count();
		it.WalkNext( (const T*)this );
	}
	return total;
}

template< class T >
inline void iTree<T>::SetParent( T* parentNode )
{
	if ( _parent != parentNode ) {
		if ( _parent ) {
			_parent->Remove( (T*)this );
		}
		if ( parentNode ) {
			parentNode->Add( (T*)this );
		}
	}
}

template< class T >
inline typename iTree<T>::Iterator iTree<T>::Add( T* node )
{
	check( node ); //, (TEXT("Inserting NULL?")) );
	check( (!node->_next || !node->_prev) ); //, (TEXT("Node already on the tree?")) );
	node->_parent = (T*)this;
	node->_next = NULL;
	node->_prev = _tail;
	if ( !Count() )
		_head = node;
	else 
		_tail->_next = node;

	_tail = node;
	_count++;
	return Iterator( _tail );
}

template< class T >
inline T* iTree<T>::Remove()
{
	if ( !Count() )
		return NULL;
	Iterator last = Last();
	IRemove( last );
	return *last;
}

template< class T >
inline typename iTree<T>::Iterator iTree<T>::Push( T* node )
{
	check( node ); //, (TEXT("Inserting NULL?")) );
	check( (!node->_next || !node->_prev) ); //, (TEXT("Node already on the tree?")) );
	node->_parent = (T*)this;
	node->_next = _head;
	node->_prev = NULL;
	if ( !Count() )
		_tail = node;
	else
		_head->_prev = node;

	_head = node;
	_count++;
	return Iterator( _head );
}

template< class T >
inline T* iTree<T>::Pop()
{
	if ( !Count() )
		return NULL;
	Iterator first = First();
	IRemove( first );
	return *first;
}

template< class T >
inline typename iTree<T>::Iterator iTree<T>::InsertBefore( Iterator& target, T* node )
{
	check( node ); //, (TEXT("Inserting NULL?")) );
	check( (!node->_next || !node->_prev) ); //, (TEXT("Node already on the tree?")) );
	check( *target ); //, (TEXT("Inserting before Iterator(NULL)?")) );
	if ( _head == *target )
		_head = node;
	node->_parent = (T*)this;
	node->_next = *target;
	node->_prev = target->_prev;
	if ( target->_prev )
		target->_prev->_next = node;
	target->_prev = node;
	_count++;
	return Iterator( node );
}

template< class T >
inline typename iTree<T>::Iterator iTree<T>::InsertAfter( Iterator& target, T* node )
{
	check( node ); //, (TEXT("Inserting NULL?")) );
	check( (!node->_next || !node->_prev) ); //, (TEXT("Node already on the tree?")) );
	check( *target ); //, (TEXT("Inserting aftre Iterator(NULL)?")) );
	if ( _tail == *target )
		_tail = node;
	node->_parent = (T*)this;
	node->_prev = *target;
	node->_next = target->_next;
	if ( target->_next )
		target->_next->_prev = node;
	target->_next = node;
	_count++;
	return Iterator( node );
}

template< class T >
inline bool iTree<T>::Remove( T* node )
{
	check( node ); //, (TEXT("Node is null")) );
	if ( node->_parent != this )
		return false;
	Iterator it( node );
	Remove( it );
	return true;
}

template< class T >
inline typename iTree<T>::Iterator iTree<T>::Remove( Iterator& target )
{
	check( Exists( target ) ); //, (TEXT("Node is not on the list!")) );
	check( target != End() ); //, (TEXT("Removing non-existant node")) );
	Iterator next( target );
	++next;
	IRemove( target );
	return next;
}

template< class T >
inline void iTree<T>::RemoveAll()
{
	while( Count() )
		IRemove( First() );
}

template< class T >
inline void iTree<T>::RemoveAllChildren()
{
	Iterator nextNode = First();
	while ( nextNode != End() ) {
		nextNode->RemoveAllChildren();
		nextNode = Remove( nextNode );
	}
}

template< class T >
inline void iTree<T>::DeleteAll()
{
	while ( Count() )
		IDelete( First() );
}

template< class T >
inline typename iTree<T>::Iterator iTree<T>::End()
{
	return Iterator( NULL );
}

template< class T >
inline typename iTree<T>::ConstIterator iTree<T>::End() const
{
	return ConstIterator( NULL );
}

template< class T >
inline typename iTree<T>::Iterator iTree<T>::Parent()
{
	return Iterator( _parent );
}

template< class T >
inline typename iTree<T>::ConstIterator iTree<T>::Parent() const
{
	return ConstIterator( _parent );
}

template< class T >
inline typename iTree<T>::Iterator iTree<T>::Next()
{
	return Iterator( _next );
}

template< class T >
inline typename iTree<T>::ConstIterator iTree<T>::Next() const
{
	return ConstIterator( _next );
}

template< class T >
inline typename iTree<T>::Iterator iTree<T>::Prev()
{
	return Iterator( _prev );
}

template< class T >
inline typename iTree<T>::ConstIterator iTree<T>::Prev() const
{
	return ConstIterator( _prev );
}

template< class T >
inline typename iTree<T>::Iterator iTree<T>::First()
{
	return Iterator( _head );
}

template< class T >
inline typename iTree<T>::ConstIterator iTree<T>::First() const
{
	return ConstIterator( _head );
}

template< class T >
inline typename iTree<T>::Iterator iTree<T>::Last()
{
	return Iterator( _tail );
}

template< class T >
inline typename iTree<T>::ConstIterator iTree<T>::Last() const
{
	return ConstIterator( _tail );
}

template< class T >
inline void iTree<T>::Traverse( void(*func)(T*) )
{
	Iterator it( (T*)this );
	while ( it != End() ) {
		func( *it );
		it.WalkNext( (const T*)this );
	}
}

template< class T >
inline void iTree<T>::Traverse( void(*func)(const T*) ) const
{
	ConstIterator it( (T*)this );
	while ( it != End() ) {
		func( *it );
		it.WalkNext( (const T*)this );
	}
}

template< class T >
inline typename iTree<T>::Iterator iTree<T>::Index( DWORD n )
{
	return Iterator( First() + n );
}

template< class T >
inline typename iTree<T>::ConstIterator iTree<T>::Index( DWORD n ) const
{
	return ConstIterator( First() + n );
}

template< class T >
bool iTree<T>::Exists( const Iterator& target ) const
{
	for ( ConstIterator it = First(); it != End(); ++it )
		if ( it == target )
			return true;
	return false;
}

template< class T >
bool iTree<T>::Exists( const ConstIterator& target ) const
{
	for ( ConstIterator it = First(); it != End(); ++it )
		if ( it == target )
			return true;
	return false;
}

template< class T >
inline void iTree<T>::IRemove( Iterator target )
{
	if ( _head == *target ) {
		_head = target->_next;
		if ( _head )
			_head->_prev = NULL;
	} else {
		target->_prev->_next = target->_next;
	}

	if ( _tail == *target ) {
		_tail = target->_prev;
		if ( _tail )
			_tail->_next = NULL;
	} else {
		target->_next->_prev = target->_prev;
	}
	target->_parent = target->_next = target->_prev = NULL;
	_count--;
}

template< class T >
inline void iTree<T>::IDelete( Iterator target )
{
	target->DeleteAll();
	IRemove( target );
	delete *target;
}

#endif //_IOLIB_HELIUM_CONTAINER_TREE_H_
/** @} */ // iolib_he
