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
 *	@brief Generic cache container template class implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_CONTAINER_CACHE_H_
#define _IOLIB_HELIUM_CONTAINER_CACHE_H_

#include "he.container.fast.ilist.h"
#include "he.container.hash.h"

/// Generic cache container template class 
template <class T, class iHASH>
class iCache
{
public:
	/// Heap element
	struct HeapEl : public iIListNode {
		/// Constructs heap element
		HeapEl(T*_value)
		: value(_value)
		{}

		/// Destructor
		~HeapEl()
		{
			delete value;
		}

		T*	value;
	private:
		HeapEl& operator=( const HeapEl& );
		HeapEl( HeapEl& );
	};

	typedef typename iHASH::KeyType		iKEY;
	typedef typename iHashTable<iKEY,T*,iHASH>::Iterator	HashIterator;
	typedef typename iIList<HeapEl>::Iterator	HeapIterator;
	
	/// Default constructor
	iCache(uint32 sizLimit = 0, uint32 numBuckets = 97)
	: m_sizLimit(sizLimit)
	, m_HashTable(numBuckets)
	{
	}

	/// Destructor
	~iCache()
	{
		ResetContent();
	}

	/// Returns count of elements in Cache
	uint32 GetElementsCount() const
	{
		return m_HashTable.Count();
	}

	/// Returns constant pointer to specified element
	const T* GetElement(const iKEY &key) const
	{
		HashIterator hashIt = m_HashTable.Find(key);
		if (hashIt != m_HashTable.End()) {
			T* el = *hashIt;
			m_Heap.Remove(HeapIterator(el));
			m_Heap.Push(el);
			return el;
		}
		return NULL;
	}

	/// Returns pointer to specified element
	T* GetElement(const iKEY &key)
	{
		HashIterator hashIt = m_HashTable.Find(key);
		if (hashIt != m_HashTable.End()) return *hashIt;
		return NULL;
	}

	/// Adds element into cache
	void AddElement(T* newEl)
	{
		verify(EvictCache());
		m_Heap.Push(new HeapEl(newEl));
		m_HashTable.Insert(newEl);
	}

	/// Deletes element from chache
	void DeleteElement(const iKEY& key)
	{
		HashIterator hashIt = m_HashTable.Find(key);
		if (hashIt != m_HashTable.End()) {
			T* el = *hashIt;
			m_HashTable.Remove(hashIt);
			for( HeapIterator hit = m_Heap.First(); hit != m_Heap.End(); ++hit ) {
				if ( hit->value == el ) {
					m_Heap.Delete( hit );
					break;
				}
			}
			//m_Heap.Remove(HeapIterator(el));
			//delete el;
		}
	}

	/// Removes element from cache (not delete!)
	T* RemoveElement(const iKEY& key)
	{
		HashIterator hashIt = m_HashTable.Find(key);
		if (hashIt != m_HashTable.End()) {
			T* el = *hashIt;
			m_HashTable.Remove(hashIt);
			m_Heap.Remove(HeapIterator(el));
			return el;
		}
		return NULL;
	}

private:
	void ResetContent()
	{
		m_Heap.DeleteAll();
		m_HashTable.RemoveAll();
	}

	bool EvictCache()
	{
		if (!m_sizLimit || m_sizLimit > m_HashTable.Count()) return true;
		while (m_sizLimit < m_HashTable.Count()){
			if (!RemoveLastEl()) return false;
		}
		return true;
	}

	bool RemoveLastEl()
	{
		HeapIterator heapIt = m_Heap.Last();
		if (heapIt == m_Heap.End()) return false;

		HashIterator hashIt = m_HashTable.Find( iHASH()( &(*heapIt->value) ));
		if (hashIt == m_HashTable.End()) return false;
		
		m_HashTable.Remove(hashIt);
		m_Heap.Delete(heapIt);

		return true;
	}

private:
	uint32						m_sizLimit;
	iIList<HeapEl>				m_Heap;
	iHashTable<iKEY,T*,iHASH>	m_HashTable;
};

#endif //_IOLIB_HELIUM_CONTAINER_CACHE_H_
/** @} */ // iolib_he
