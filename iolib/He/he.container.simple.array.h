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
 *	@brief Simple array container template class implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_CONTAINER_SIMPLE_ARRAY_H_
#define _IOLIB_HELIUM_CONTAINER_SIMPLE_ARRAY_H_

#include "he.std.h"

#pragma warning( push )
#pragma warning( disable : 4291 )

/// Simple array container template class 
template <class T>
class iSimpleArray
{
public:
	enum 
	{
		NOT_FOUND	= -1
	};

	T*		m_aT;
	uint32	m_nSize;
	uint32	m_nAllocSize;

	/// Default constructor
	iSimpleArray() : m_aT(NULL), m_nSize(0), m_nAllocSize(0)
	{ }

	/// Copy constructor
	iSimpleArray(const iSimpleArray<T> &src)
	: m_aT(NULL)
	, m_nSize(src.m_nSize), m_nAllocSize(src.m_nSize)
	{ 
		if ( src.m_aT != 0 ) {
			m_aT = (T*) malloc( m_nSize * sizeof(T)  );
			for( uint32 n = 0; n < m_nSize; ++n )
				new(&m_aT[n]) iWrapper(src[n]);
		}
	}

	/// Destructor
	~iSimpleArray()
	{
		RemoveAll();
	}

	/// Preallocates container's buffer
	bool Preallocate(uint32 siz)
	{
		T* aT;
		aT = (T*)realloc(m_aT, siz * sizeof(T));
		if(aT == NULL)	return false;
		m_nAllocSize = siz;
		m_aT = aT;
		return true;
	}

	/// Reallocates container's buffer
	bool SetSize(uint32 nsize)
	{
		if (nsize == m_nSize) return true;

		T* aT = (T*)realloc(m_aT, nsize * sizeof(T));
		if(aT == NULL)	return false;
		m_nAllocSize = nsize;
		m_nSize = nsize;
		m_aT = aT;

		return true;
	}

	/// Returns number of elements in container
	inline uint32 GetSize() const
	{
		return m_nSize;
	}

	/// Returns - is this container empty or not
	inline bool IsEmpty() const
	{
		return (m_nSize == 0);
	}

	/// Adds new item to container
	bool Add(const T& t)
	{
		if(m_nSize == m_nAllocSize)
		{
			T* aT;
			int nNewAllocSize = (m_nAllocSize == 0) ? 1 : (m_nSize * 2);
			aT = (T*)realloc(m_aT, nNewAllocSize * sizeof(T));
			if(aT == NULL)	return false;
			m_nAllocSize = nNewAllocSize;
			m_aT = aT;
		}
		m_nSize++;
		SetAtIndex(m_nSize - 1, t);
		return true;
	}

	/// Adds unique item to container
	bool AddUnique(const T& t)
	{
		if (Find(t) == NOT_FOUND) return Add(t);
		return false;
	}

	/// Adds all items from another container
	bool Add(const iSimpleArray<T> &ta)
	{
		for (uint32 xx=0; xx<ta.GetSize(); ++xx){
			Add(ta[xx]);
		}
		return true;
	}

	/// Pops item from the end
	T Pop()
	{
		check( GetSize() != 0 );
		uint32 lastN = GetSize() - 1;
		T val = operator []( lastN );
		RemoveAt( lastN );
		return val;
	}

	/// Removes specified item
	bool Remove(const T& t)
	{
		sint32 nIndex = Find(t);

		if(nIndex == NOT_FOUND)	return false;

		return RemoveAt(nIndex);
	}
	
	/// Removes item with specified index
	bool RemoveAt(uint32 nIndex)
	{
		if(nIndex != (m_nSize - 1)) {
			m_aT[nIndex].~T();
			memmove((void*)&m_aT[nIndex], (void*)&m_aT[nIndex + 1], (m_nSize - (nIndex + 1)) * sizeof(T));
		}
		m_nSize--;
		return true;
	}

	/// Insert new item before item with specified index
	bool InsertBefore(uint32 nIndex, const T& t)
	{
		if (nIndex >= m_nSize) nIndex = m_nSize - 1;

		if(m_nSize == m_nAllocSize) {
			T* aT;
			int nNewAllocSize = (m_nAllocSize == 0) ? 1 : (m_nSize * 2);
			aT = (T*)realloc(m_aT, nNewAllocSize * sizeof(T));
			if(aT == NULL)	return false;
			m_nAllocSize = nNewAllocSize;
			m_aT = aT;
		}

		//m_aT[nIndex]
		m_nSize++;
		memmove((void*)&m_aT[nIndex+1], (void*)&m_aT[nIndex], (m_nSize - (nIndex + 1)) * sizeof(T));

		SetAtIndex(nIndex,t);
		return true;
	}

	/// Insert new item after item with specified index
	bool InsertAfter(uint32 nIndex, const T& t)
	{
		if (nIndex >= m_nSize) nIndex = m_nSize - 1;

		if(m_nSize == m_nAllocSize)	{
			T* aT;
			int nNewAllocSize = (m_nAllocSize == 0) ? 1 : (m_nSize * 2);
			aT = (T*)realloc(m_aT, nNewAllocSize * sizeof(T));
			if(aT == NULL)	return false;
			m_nAllocSize = nNewAllocSize;
			m_aT = aT;
		}

		//m_aT[nIndex]
		m_nSize++;
		memmove((void*)&m_aT[nIndex+2], (void*)&m_aT[nIndex + 1], (m_nSize - (nIndex + 2)) * sizeof(T));

		SetAtIndex(nIndex+1,t);
		return true;
	}

	/// Remove all items
	void RemoveAll()
	{
		if(m_aT != NULL) {
			for(uint32 i = 0; i < m_nSize; i++) m_aT[i].~T();
			free(m_aT);
			m_aT = NULL;
		}
		m_nSize = 0;
		m_nAllocSize = 0;
	}

	/// returns reference to the item with specified index
	T& operator[] (uint32 nIndex) const
	{
		check(nIndex >= 0 && nIndex < m_nSize);
		return m_aT[nIndex];
	}

	/// returns reference to the last item
	T& GetLast() const
	{
		check(m_nSize != 0);
		return m_aT[m_nSize-1];
	}

	/// Assignment operator
	iSimpleArray<T>& operator = ( const iSimpleArray<T>& other )
	{
		RemoveAll();
		m_nSize = m_nAllocSize = other.m_nSize;
		if ( other.m_aT ) {
			m_aT = (T*) malloc( sizeof(T) * m_nSize );
			for( uint32 n = 0; n < m_nSize; ++n )
				new(&m_aT[n]) iWrapper(other[n]);
		} else {
			m_aT = 0;
		}

		return *this;
	}

	/// Equality operator
	bool operator == (const iSimpleArray<T> &eq)
	{
		if (GetSize() != eq.GetSize()) return false;

		for (uint32 xx=0; xx < GetSize(); ++xx)
		{
			if (m_aT[xx] != eq[xx]) return false;
		}

		return true;
	}

	/// Returns pointer to the first element
	T* GetData() const
	{
		return m_aT;
	}

	/// Wrapper
	class iWrapper
	{
	public:
		iWrapper(const T& _t) : t(_t)
		{
		}
		template <class _Ty>
		void *operator new(size_t, _Ty* p)
		{
			return p;
		}
		T t;
	};

	/// Set item to specified index
	void SetAtIndex(uint32 nIndex, const T& t)
	{
		check(nIndex >= 0 && nIndex < m_nSize);
		new(&m_aT[nIndex]) iWrapper(t);
	}


	/// Find item
	template <class K>
	sint32 Find(const K& t) const
	{
		for(uint i = 0; i < m_nSize; i++) {
			if(m_aT[i] == t) return i;
		}
		return NOT_FOUND;  // not found
	}
};

#pragma warning(pop)

#endif //_IOLIB_HELIUM_CONTAINER_SIMPLE_ARRAY_H_
/** @} */ // iolib_he
