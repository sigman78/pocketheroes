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

#ifndef _GXLIB_CONTAINER_ARRAY_H_
#define _GXLIB_CONTAINER_ARRAY_H_

#pragma warning(push)
#pragma warning(disable:4291)

#include "xxl.array.cfg.h"
#include "xxl.array.popt.h"

// enables common ptr base optimization
//
#if 1
template <class T>
class iSimpleArray : private array_popt< T >
{
public:
	
	typedef iSimpleArray<T>							ThisType;
	typedef array_popt< T >							BaseType;
	typedef T 										ValueType;
	typedef typename BaseType::param_type			ParamType;
#else
template <class T>
class iSimpleArray : private array_base< T, dynamic_storage<T> >
{
public:
	
	typedef iSimpleArray<T>							ThisType;
	typedef array_base< T, dynamic_storage<T> >		BaseType;
	typedef T 										ValueType;
	typedef typename BaseType::param_type			ParamType;
#endif

	enum 
	{
		NOT_FOUND	= -1
	};

	inline iSimpleArray() 
	: BaseType()
	{}

	iSimpleArray(uint32 paSiz) 
	: BaseType()
	{ 
		BaseType::reserve(paSiz);
	}

	inline iSimpleArray(const ThisType &src)
	: BaseType( src )
	{}

	inline ~iSimpleArray()
	{}

	bool Preallocate(uint32 siz)
	{
		BaseType::reserve( siz );
		return BaseType::capacity() >= siz;
	}

	bool SetSize(uint32 nsize)
	{
		BaseType::resize( nsize );
		return BaseType::size() == nsize;
	}

	inline bool ValidIdx(sint32 idx) const
	{ return idx>=0 && idx<(sint32)BaseType::size(); }

	inline uint32 GetSize() const
	{
		return BaseType::size();
	}

	inline bool IsEmpty() const
	{
		return BaseType::empty();
	}

	inline bool Add( ParamType t)
	{
		BaseType::push_back( t );
		return true;
	}

	bool AddUnique( ParamType t )
	{
		if (Find(t) == NOT_FOUND) return Add(t);
		return false;
	}

	inline bool Add(const ThisType &ta)
	{
		BaseType::insert( BaseType::end(), ta.first(), ta.end() );
		return true;
	}

	inline bool Push( ParamType t)
	{
		BaseType::push_back( t );
		return true;
	}
	
	inline T Pop() 
	{
		return BaseType::pop_back();
	}

	inline T PopFirst()
	{
		check( GetSize() != 0 );
		T val = BaseType::front();
		BaseType::erase( BaseType::first() );
		return val;
	}

	bool Remove( ParamType t)
	{
		sint32 nIndex = Find(t);

		if(nIndex == NOT_FOUND)	return false;

		return RemoveAt(nIndex);
	}

	bool RemoveAt(uint32 nIndex)
	{
		BaseType::erase( BaseType::first() + nIndex );
		return true;
	}

	bool InsertBefore(uint32 nIndex, ParamType t )
	{
//		if ( nIndex >= BaseType::size() ) nIndex = BaseType::size() - 1;
		BaseType::insert( BaseType::first() + nIndex, t );
		return true;
	}

	bool InsertAfter(uint32 nIndex, ParamType t )
	{
//		if ( nIndex >= BaseType::size() ) nIndex = BaseType::size() - 1;
		/////
		BaseType::insert( BaseType::first() + nIndex + 1, t );
		return true;
	}

	void RemoveAll()
	{
		BaseType::clear();
	}

	inline T& At(uint32 nIndex)
	{
		return BaseType::at(nIndex);
	}

	inline const T& At(uint32 nIndex) const
	{
		return BaseType::at(nIndex);
	}

	inline T& operator[] (uint32 nIndex)
	{
		return BaseType::at(nIndex);
	}

	inline const T& operator[] (uint32 nIndex) const
	{
		return BaseType::at(nIndex);
	}

	inline T& GetLast()
	{
		return BaseType::back();
	}

	inline const T& GetLast() const
	{
		return BaseType::back();
	}

	inline void operator += (const ThisType& other)
	{
		BaseType::insert( BaseType::end(), other.first(), other.end() );
	}

	bool operator == (const ThisType &eq)
	{
		if (BaseType::size() != eq.size()) return false;

		for (uint32 xx=0; xx != BaseType::size(); ++xx)
		{
			if ( !(BaseType::at(xx) == eq.at(xx) )) return false;
		}

		return true;
	}

	inline T* GetData() 
	{
		return BaseType::first();
	}

	inline void SetAtIndex(uint32 nIndex, ParamType t)
	{
		BaseType::at( nIndex ) = t;
	}


	template <class K>
	sint32 Find(const K& t) const
	{
		for(uint i = 0; i < BaseType::size(); i++) {
			if( BaseType::at(i) == t)  return i;
		}
		return NOT_FOUND;  // not found
	}

	void Swap( ThisType& other )
	{
		BaseType::swap( other );
	}

};

#pragma warning(pop)


#endif //_GXLIB_CONTAINER_ARRAY_H_

