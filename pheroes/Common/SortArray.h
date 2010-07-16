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

#ifndef _HMM_COMMON_AUTO_SORT_ARRAY_H_
#define _HMM_COMMON_AUTO_SORT_ARRAY_H_

//
// WARN!! Must be refactored according to the SimpleArray changes
// NOTE:: Provide a way to traits-qualify the internal iEntry structure
//		according to member T

//#ifdef XXL_DECLARE_AS_POD_IF_POD
//XXL_DECLARE_AS_POD_IF_POD( )
//#endif 

template <class T>
class iSortArray
{
public:
	struct iEntry {
		iEntry() : idx(0), T(0) {}
		iEntry(sint32 _idx, const T& _value) : idx(_idx), value(_value) {}
		sint32	idx;
		T		value;
	};
public:

	inline iEntry* GetPtr()
	{ return m_Array.GetData(); }

	inline const iEntry* GetPtr() const
	{ return m_Array.GetData(); }

	void Init(const iSortArray<T>& other)
	{
		iSortArray<T>& me = *this;
		me = other;
	}

	inline T Pop()
	{
		return m_Array.Pop().value;
	}

	inline sint32 FirstIdx() const { return m_Array[0].idx; }
	inline sint32 LastIdx() const { return m_Array.GetLast().idx; }

	inline T& Last() { return m_Array.GetLast().value; }
	inline const T& Last() const { return m_Array.GetLast().value; }

	bool RemoveAt(uint32 idx)
	{ return m_Array.RemoveAt(idx); }

	// indexes: from smaller to bigger 
	bool Insert(const T& value, sint32 idx)
	{
		uint32 length = m_Array.GetSize();
		if (!length || m_Array[length-1].idx <= idx) {
			m_Array.Add(iEntry(idx, value));
			return true;
		}

		if (idx < m_Array[0].idx) {
			m_Array.InsertBefore(0,iEntry(idx, value));
			return true;
		}

		uint32 middle, half;
		uint32 first = 0;

		while (length > 0) {
			half = length >> 1;
			middle = first + half;
			if ( m_Array[middle].idx <= idx ) {
				first = middle+1;
				length = length - half - 1;
			} else {
				length = half;
			}
		}
		m_Array.InsertBefore(first,iEntry(idx, value));
		return true;
	}

	void Cleanup() { m_Array.RemoveAll(); }

	inline uint32 Size() const { return m_Array.GetSize(); }

	inline const iEntry& operator[] (uint32 idx) const { return m_Array[idx]; }
	inline const T& Value(uint32 idx) const { return m_Array[idx].value; }
	inline T& Value(uint32 idx) { return m_Array[idx].value; }
	inline sint32 Index(uint32 idx) const { return m_Array[idx].idx; }

	inline bool SelfTest() const
	{
		const uint32 siz = m_Array.GetSize();
		if ( siz < 2 ) return true;
		
		for( uint32 n = 0; n != siz - 1; ++n ) {
			if ( m_Array[n].idx > m_Array[n+1].idx ) return false;
		}

		return true;
	}

protected:
	iSimpleArray<iEntry>	m_Array;
};

#endif //_HMM_COMMON_AUTO_SORT_ARRAY_H_

