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

#ifndef _GXLIB_CONTAINER_MAP_H_
#define _GXLIB_CONTAINER_MAP_H_

template <class T>
class iMapT
{
public:
	iMapT()
	: m_Width(0)
	, m_Height(0)
	, m_Buff(NULL)
	{}

	iMapT(sint32 w, uint32 h)
	: m_Width(0)
	, m_Height(0)
	, m_Buff(NULL)
	{
		Init(w,h);
	}

	virtual ~iMapT()
	{
		Cleanup();
	}

	void Cleanup()
	{
		if (m_Buff) {
			delete[] m_Buff;
			m_Buff = NULL;
			m_Width = m_Height = 0;
		}
	}

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

	bool Init(const iMapT<T>& other)
	{
		if (!Init(other.GetWidth(), other.GetHeight())) return false;
		memcpy(m_Buff,other.GetPtr(),m_Width*m_Height*sizeof(T));
		return true;
	}

	void CopyFrom(const iMapT<T>& other)
	{
		check(other.GetWidth() == m_Width && other.GetHeight() == m_Height);
		memcpy(m_Buff,other.GetPtr(),m_Width*m_Height*sizeof(T));
	}

	void FillMem(const T& val)
	{
		T* ptr = m_Buff;
		for (sint32 xx=0; xx<m_Height*m_Width; ++xx){
			*ptr = val;
			ptr++;
		}
	}

	void FillRect( const iRect& rc, const T& val )
	{
		check(m_Buff);
		for (uint32 yy = 0; yy<rc.h; ++yy) {
			T* ptr = m_Buff + (rc.y+yy) * m_Width + rc.x;
			for (uint32 xx=0; xx<rc.w; ++xx, ++ptr) {
				*ptr = val;
			}
		}
	}
	
	void ZeroMem(uint8 val = 0)
	{
		check(m_Buff);
		if (m_Buff) {
			memset(m_Buff,val,m_Width*m_Height*sizeof(T));
		}
	}

	// inlines
	inline sint32 GetWidth() const
	{ return m_Width; }

	inline sint32 GetHeight() const
	{ return m_Height; }

	inline bool IsInited() const
	{ return m_Buff!=NULL; }

	inline const T* GetPtr() const
	{ return m_Buff; }

	inline T* GetPtr()
	{ return m_Buff; }

	// optimized frequently used check
	inline bool IsValidPos(sint32 x, sint32 y) const
	{ return ( (uint32)x<(uint32)m_Width && (uint32)y<(uint32)m_Height); }

	inline const T& GetAt(sint32 x, sint32 y) const
	{ check(IsValidPos(x,y)); return m_Buff[y*m_Width+x]; }

	inline T& GetAt(sint32 x, sint32 y)
	{ check(IsValidPos(x,y)); return m_Buff[y*m_Width+x]; }

	inline void SetAt(sint32 x, sint32 y, T& val)
	{ check(IsValidPos(x,y)); m_Buff[y*m_Width+x] = val; }

protected:
	sint32	m_Width;
	sint32	m_Height;
	T*		m_Buff;

};

#endif //_GXLIB_CONTAINER_MAP_H_

