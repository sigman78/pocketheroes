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
 *	@brief Map container template class implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_CONTAINER_MAP_H_
#define _IOLIB_HELIUM_CONTAINER_MAP_H_

/// Map container template class 
template <class T>
class iMapT
{
public:
	/// Default constructor
	iMapT()
	: m_Width(0)
	, m_Height(0)
	, m_Buff(NULL)
	{}

	/// Constructs Map container with given width and height
	iMapT(sint32 w, uint32 h)
	: m_Width(0)
	, m_Height(0)
	, m_Buff(NULL)
	{
		Init(w,h);
	}

	/// Cleanups
	void Cleanup()
	{
		if (m_Buff) {
			delete[] m_Buff;
			m_Buff = NULL;
			m_Width = m_Height = 0;
		}
	}

	/// Initializes Map container with given width and height
	bool Init(sint32 w, sint32 h)
	{
		Cleanup();
		check(w > 0);
		check(h > 0);
		m_Width = w;
		m_Height = h;
		m_Buff = new T[w*h];
		return (m_Buff!=NULL);
	}

	/// Reinitialize all elements with given value
	void FillMem(const T& val)
	{
		T* ptr = m_Buff;
		for (sint32 xx=0; xx<m_Height*m_Width; ++xx){
			*ptr = val;
			ptr++;
		}
	}

	/// Zero allocated memory
	void ZeroMem()
	{
		if (m_Buff) {
			memset(m_Buff,0,m_Width*m_Height*sizeof(T));
		}
	}

	/// Returns the width of map
	inline sint32 GetWidth() const
	{ return m_Width; }

	/// Returns the height of map
	inline sint32 GetHeight() const
	{ return m_Height; }

	/// Check container state
	inline bool IsInited() const
	{ return m_Buff!=NULL; }

	/// Returns constant pointer to the first element
	inline const T* GetPtr() const
	{ return m_Buff; }

	/// Returns pointer to the first element
	inline T* GetPtr()
	{ return m_Buff; }

	/// Validate given position
	inline bool IsValidPos(sint32 x, sint32 y) const
	{ return (x>=0 && x<m_Width && y>=0 && y<m_Height); }

	// Returns constant pointer to the specified element
	inline const T* GetPtr(sint32 x, sint32 y) const
	{ check(IsValidPos(x,y)); return m_Buff+y*m_Width+x; }

	// Returns pointer to the specified element
	inline T* GetPtr(sint32 x, sint32 y)
	{ check(IsValidPos(x,y)); return m_Buff+y*m_Width+x; }

	// Returns specified element
	inline T GetCell(sint32 x, sint32 y) const
	{ check(IsValidPos(x,y)); return m_Buff[y*m_Width+x]; }

	// Set new value for specified element
	inline void SetCell(sint32 x, sint32 y, const T& val)
	{ check(IsValidPos(x,y)); m_Buff[y*m_Width+x] = val; }

	// Access by reference
	inline T& At(sint32 x, sint32 y)
	{ check(IsValidPos(x,y)); return m_Buff[y*m_Width+x]; }
	inline const T& At(sint32 x, sint32 y) const
	{ check(IsValidPos(x,y)); return m_Buff[y*m_Width+x]; }

protected:
	sint32	m_Width;
	sint32	m_Height;
	T*		m_Buff;

};

#endif //_IOLIB_HELIUM_CONTAINER_MAP_H_
/** @} */ // iolib_he
