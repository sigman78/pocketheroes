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

#ifndef _GXLIB_MEMORY_H_
#define _GXLIB_MEMORY_H_


#define DYNAMIC_BUFFER_INC	512		// increment of the buffer on overflow

/*
 *	iBuff
 */
template<class T>
class iBuff
{
public:
	iBuff()
	: m_pBuff(NULL), m_Siz(0) {}

	iBuff(uint32 siz)
	: m_pBuff(NULL), m_Siz(0)
	{ Allocate(siz); }

	iBuff(const T* buff, uint32 siz)
	: m_pBuff(NULL), m_Siz(0)
	{ Allocate(buff, siz); }

	iBuff(const iBuff<T>& buff)
	: m_pBuff(NULL), m_Siz(0)
	{ Allocate(buff); }

	~iBuff()
	{ delete[] m_pBuff; }

	inline bool IsClean() const
	{ return !m_pBuff; }

	void Allocate(uint32 siz)
	{
		Clean();
		m_Siz = siz;
		m_pBuff = new T[m_Siz];
		memset(m_pBuff,0,m_Siz*sizeof(T));
	}

	void Allocate(const T* buff, uint32 siz)
	{
		Clean();
		m_Siz = siz;
		m_pBuff = new T[m_Siz];
		memcpy(m_pBuff,buff,m_Siz*sizeof(T));
	}

	void Allocate(const iBuff<T>& buff)
	{
		Clean();
		m_Siz = buff.GetSize();
		m_pBuff = new T[m_Siz];
		memcpy(m_pBuff,buff,m_Siz*sizeof(T));
	}


	void Clean()
	{
		delete[] m_pBuff;
		m_pBuff = 0;
		m_Siz	= 0;
	}

	inline uint32 GetSize() const
	{ return m_Siz; }

	inline const T* GetPtr() const
	{ return m_pBuff; }

	inline T* GetPtr()
	{ return m_pBuff; }

	inline operator T*()
	{ return m_pBuff; }

	inline operator const T*() const
	{ return m_pBuff; }

private:
	uint32	m_Siz;
	T*		m_pBuff;
};

/*
 *	
 */
class iDynamicPtr
{
public:
	iDynamicPtr()
	: m_Data(NULL) , m_Pos(0){}

	iDynamicPtr( void* ptr )
	: m_Data(ptr), m_Pos(0){}

	template< class T >
	bool Write( const T* pdata, uint32 number = 1 )
	{
		uint32 len = sizeof(T) * number;
		if ( m_Data && pdata && len ) {
			memcpy( IncPtr( m_Data, m_Pos), pdata, len );
			m_Pos += len;
			return true;
		}
		return false;
	}

	template< class T >
	bool Read( T* pdata, uint32 number = 1 ) const
	{
		uint32 len = sizeof(T) * number;
		if ( m_Data && pdata && len ) {
			memcpy( pdata, IncPtr( m_Data, m_Pos), len );
			m_Pos += len;
			return true;
		}
		return false;
	}

	inline void Rewind()
	{ m_Pos = 0; }

protected:
	void*			m_Data;
	mutable uint32	m_Pos;
	void* IncPtr( void* ptr, uint32 incr ) const
	{ return (void*)((uint8*)(ptr) + (uint32)(incr)); }
};

/*
 *	
 */
class iDynamicBuffer : private iDynamicPtr
{
public:
	iDynamicBuffer( uint32 cap = 0, uint32 incr = DYNAMIC_BUFFER_INC )
	{
		m_Capacity	= cap;
		m_Incr		= incr ? incr : DYNAMIC_BUFFER_INC;
		m_Size		= 0;
		m_ReadOnly	= false;
		m_Data		= 0;
		if ( m_Capacity ) m_Data = malloc(m_Capacity);//new uint8[m_Capacity];
	}

	iDynamicBuffer( const iDynamicBuffer& buf )
	{
		m_Pos		= buf.m_Pos;
		m_Capacity	= buf.m_Capacity;
		m_Size		= buf.m_Size;
		m_Incr		= buf.m_Incr;
		m_ReadOnly	= buf.m_ReadOnly;
		if ( !m_ReadOnly && buf.m_Data && m_Capacity ) {
			m_Data = malloc( m_Capacity); //new uint8[m_Capacity];
			memcpy( m_Data, buf.m_Data, m_Capacity);
		} else {
			m_Data = buf.m_Data;
		}
	}

	iDynamicBuffer( void* data, uint32 len, bool rdonly = true )
	{
		m_Pos		= 0;
		m_Capacity	= len;
		m_Size		= len;
		m_Incr		= DYNAMIC_BUFFER_INC;
		m_ReadOnly	= rdonly;
		if ( !m_ReadOnly && data && m_Capacity ) {
			m_Data = malloc( m_Capacity); //new uint8[m_Capacity];
			memcpy( m_Data, data, m_Capacity );
		} else {
			m_Data = data;
		}
	}

	~iDynamicBuffer()
	{ Clean(); }

	iDynamicBuffer& operator = ( const iDynamicBuffer& buf )
	{
		if ( !m_ReadOnly )	free(m_Data);//delete[] m_Data;
		m_Pos		= buf.m_Pos;
		m_Capacity	= buf.m_Capacity;
		m_Size		= buf.m_Size;
		m_Incr		= buf.m_Incr;
		m_ReadOnly	= buf.m_ReadOnly;
		if ( !m_ReadOnly && buf.m_Data && m_Capacity ) {
			m_Data = malloc( m_Capacity); //new uint8[m_Capacity];
			memcpy( m_Data, buf.m_Data, m_Capacity );
		} else {
			m_Data = buf.m_Data;
		}
		return *this;
	}

	void Clean()
	{
		if ( !m_ReadOnly ) {
			free(m_Data); //delete[] m_Data;
		} 
		m_Data = NULL;
		m_Capacity	= 0;
		m_Size		= 0;
		m_ReadOnly	= 0;
		Rewind();
	}

	void Rewind()
	{ iDynamicPtr::Rewind(); }

	void Reset()
	{
		m_Size = 0;
		Rewind();
	}

	void ReInit( uint32 cap = 0, uint32 incr = DYNAMIC_BUFFER_INC )
	{
		Clean();
		m_Capacity = cap;
		if ( m_Capacity ) {
			m_Data = malloc( m_Capacity);//new uint8[m_Capacity];
		}
		m_Incr = incr ? incr : DYNAMIC_BUFFER_INC;
		m_ReadOnly = false;
	}

	uint32 IncSize(uint32 offs)
	{
		if ( (m_Size + offs) > m_Capacity ) {
			m_Size = m_Capacity;
			return false;
		} else {
			m_Size += offs;
			return true;
		}
	}

	uint32 IncPos(uint32 offs) const
	{
		uint32 oldPos = m_Pos;
		if ( offs + m_Pos > m_Capacity ) {
			m_Pos = m_Capacity;
		} else {
			m_Pos += offs;
		}
		return m_Pos - oldPos;
	}

	bool Seek( uint32 pos )
	{
		if ( pos > m_Capacity ) {
			m_Pos = m_Capacity;
			return false;
		} else {
			m_Pos = pos;
			return true;
		}
	}

	bool SetSize( uint32 size )
	{
		if ( size > m_Capacity ) {
			m_Size = m_Capacity;
			return false;
		} else {
			m_Size = size;
			return true;
		}
	}

	void SetData( void* data, uint32 length, bool rdonly = true )
	{
		if ( !m_ReadOnly) {
			free(data);//delete[] data;
		}
		if ( !m_ReadOnly && data && length ) {
			m_Data = malloc(length);
			memcpy( m_Data, data, length );
		} else  {
			m_Data = data;
		}
		m_Capacity	= length;
		m_Size		= length;
		m_Incr		= DYNAMIC_BUFFER_INC;
		m_ReadOnly	= rdonly;
		Rewind();
	}

	inline bool IsReadOnly() const
	{ return m_ReadOnly; }

	inline uint32 GetCapacity() const
	{ return m_Capacity; }

	inline uint32 GetSize() const
	{ return m_Size; }

	inline uint32 GetIncrement() const
	{ return m_Incr; }

	inline void* GetData() const
	{ return m_Data; }

	inline void* GetCurrentData() const
	{ return IncPtr(m_Data, m_Pos);	}

	inline uint32 GetCurrentPos() const
	{ return m_Pos; }

	inline bool Write( uint8 b )
	{ return (Write(&b, 1, sizeof(uint8)) > 0); }

	inline bool Write( sint8 b )
	{ return (Write(&b, 1, sizeof(sint8)) > 0);	}

	inline bool Write( uint16 w )
	{ return (Write(&w, 1, sizeof(uint16)) > 0); }

	inline bool Write( sint16 w )
	{ return (Write(&w, 1, sizeof(sint16)) > 0); }

	inline bool Write( uint32 d )
	{ return (Write(&d, 1, sizeof(uint32))> 0); }

	inline bool Write( sint32 d )
	{ return (Write(&d, 1, sizeof(sint32)) > 0); }

	inline bool Write( float f )
	{ return (Write(&f, 1, sizeof(float)) > 0); }

	bool Write( const iStringA& s )	// use NULL terminator
	{
		bool res = false;
		if ( !s.Empty() ) {
			uint32 len = s.Length();
			res = Write( len );
			if ( len )	res &= (Write( s.CStr(), len) > 0 );
		} else {
			uint32 len = 0;
			res = Write( len );
		}
		return res;
	}

	bool Write( const iStringW& s )	// use NULL terminator
	{
		bool res = false;
		if ( !s.Empty() ) {
			uint32 len = s.Length();
			res = Write( len );
			if ( len )	res &= (Write( s.CStr(), len*sizeof(iCharW)) > 0 );
		} else {
			uint32 len = 0;
			res = Write( len );
		}
		return res;
	}

	uint32 Write( const void* buffer, uint32 n, uint32 size = 1 )
	{
		uint32 len = n * size;
		if ( buffer && len && !m_ReadOnly ) {
			AddLength( len );
			if ( iDynamicPtr::Write( (uint8*)buffer, len )) {
				if ( m_Pos > m_Size ) m_Size = m_Pos;
				return len / size;
			} else {
				return 0;
			}
		} else {
			return 0;
		}
	}

	inline bool Read( uint8& b ) const
	{ return (Read(&b, 1, sizeof(uint8)) > 0); }

	inline bool Read( sint8& b ) const
	{ return (Read(&b, 1, sizeof(sint8)) > 0); }

	inline bool Read( uint16& w ) const
	{ return (Read(&w, 1, sizeof(uint16)) > 0); }

	inline bool Read( sint16& w ) const
	{ return (Read(&w, 1, sizeof(sint16)) > 0); }

	inline bool Read( uint32& d ) const
	{ return (Read(&d, 1, sizeof(uint32)) > 0); }

	inline bool Read( sint32& d ) const
	{ return (Read(&d, 1, sizeof(sint32)) > 0); }

	inline bool Read( float& f ) const
	{ return (Read(&f, 1, sizeof(float)) > 0); }

	bool Read( iStringA& s ) const	// with NULL term
	{
		uint32 len;
		if ( !Read( len ) || len > m_Capacity - m_Pos) return false;
		if ( len ) {
			s = iStringA( (const iCharA*)IncPtr( m_Data, m_Pos ), len );
			m_Pos += len*sizeof(iCharA);
		} else {
			s.SetEmpty();
		}

		return true;
	}

	bool Read( iStringW& s ) const	// with NULL term
	{
		uint32 len;
		if ( !Read( len ) || len > m_Capacity - m_Pos) return false;
		if ( len ) {
			s = iStringW( (const iCharW*)IncPtr( m_Data, m_Pos ), len );
			m_Pos += len*sizeof(iCharW);
		} else {
			s.SetEmpty();
		}
		return true;
	}

	inline uint32 Read( void* buffer, uint32 n, uint32 size = 1 ) const
	{
		uint32 len = n * size;
		return ( (buffer && m_Pos < m_Capacity && (m_Capacity - m_Pos) >= len && iDynamicPtr::Read( (uint8*)buffer, len )) ? n :  0 );
	}

	void AddLength( uint32 ns )
	{
		if ( !m_ReadOnly && ( ns > (m_Capacity - m_Pos)))  {
			uint32 newlength = ((m_Pos + ns + m_Incr - 1) / m_Incr) * m_Incr;
			void* lp = malloc( newlength );
			if ( m_Data ) {
				memcpy( lp, m_Data, m_Capacity );
				free(m_Data);//delete[] m_Data;
			}
			m_Data = lp;
			m_Capacity = newlength;
		}
	}

	inline bool IsEnd() const
	{ return (m_Pos >= m_Size); }

private:
	uint32	m_Capacity;
	uint32	m_Size;
	uint32	m_Incr;
	bool	m_ReadOnly;
};

#endif //_GXLIB_MEMORY_H_
