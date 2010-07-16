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
 *	@brief Memory buffer wrapper class template implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_BUFFER_H_
#define _IOLIB_HELIUM_BUFFER_H_

/// This macro releases specified interface
#define SAFE_RELEASE(x) if (x) { (x)->Release(); (x)=NULL; }
/// This macro deletes specified object 
#define SAFE_DELETE(x) if (x) { delete (x); (x)=NULL; }
/// This macro deletes specified array
#define SAFE_ARRAY_DELETE(x) if (x) { delete[] (x); (x)=NULL; }

/// Memory buffer wrapper class template
template<class T>
class iBuff
{
public:
	/// Default constructor
	iBuff()
	: m_pBuff(NULL)
	, m_Siz(0)
	{}

	/// Constructs iBuff object with specified size
	iBuff(uint32 siz)
	: m_pBuff(NULL)
	, m_Siz(0)
	{
		Allocate(siz);
	}

	/// Constructs iBuff object with specified buffer
	iBuff(const T* buff, uint32 siz)
	: m_pBuff(NULL)
	, m_Siz(0)
	{
		Allocate(buff, siz);
	}

	/// Copy constructor
	iBuff(const iBuff<T>& buff)
	: m_pBuff(NULL)
	, m_Siz(0)
	{
		Allocate(buff);
	}

	/// Destructor
	~iBuff()
	{
		Clean();
	}

	/// Validates buffer state
	bool IsClean() const
	{
		return !m_pBuff;
	}

	/// Allocates iBuff object with specified size
	void Allocate(uint32 siz)
	{
		Clean();
		m_Siz = siz;
		m_pBuff = new T[m_Siz];
		memset(m_pBuff,0,m_Siz*sizeof(T));
	}

	/// Allocates iBuff object with specified buffer
	void Allocate(const T* buff, uint32 siz)
	{
		Clean();
		m_Siz = siz;
		m_pBuff = new T[m_Siz];
		memcpy(m_pBuff,buff,m_Siz*sizeof(T));
	}

	/// Allocates iBuff object with another iBuff object
	void Allocate(const iBuff<T>& buff)
	{
		Clean();
		m_Siz = buff.GetSize();
		m_pBuff = new T[m_Siz];
		memcpy(m_pBuff,buff,m_Siz*sizeof(T));
	}


	/// Cleanups buffer
	void Clean()
	{
		SAFE_ARRAY_DELETE(m_pBuff);
		m_Siz = 0;
	}

	/// Returns buffer size
	inline uint32 GetSize() const
	{
		return m_Siz;
	}

	/// Returns pointer to the first element
	operator T*()
	{
		return m_pBuff;
	}

	/// Returns constant pointer to the first element
	operator const T*() const
	{
		return m_pBuff;
	}

private:
	uint32	m_Siz;
	T*		m_pBuff;
};


#endif //_IOLIB_HELIUM_BUFFER_H_
/** @} */ // iolib_he
