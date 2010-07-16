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
 *	@brief Generic dynamic buffer class implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_MEMORY_DYNAMIC_BUFFER_H_
#define _IOLIB_HELIUM_MEMORY_DYNAMIC_BUFFER_H_

#define DYNAMIC_BUFFER_INC	512		// increment of the buffer on overflow

#include "he.memory.h"
#include "he.string.h"

/// Basic class for generic dynamic buffer class
class iDynamicPtr
{
public:
	/// Default constructor
	iDynamicPtr()
	: m_Data(NULL)
	, m_Pos(0)
	{
	}

	/// Constructs pointer with specified one
	iDynamicPtr( void* ptr )
	: m_Data(ptr)
	, m_Pos(0)
	{
	}

	/// Writes specified data into buffer
	template< class T >
	bool Write( const T* pdata, uint32 number = 1 )
	{
		uint32 len = sizeof(T) * number;
		if ( m_Data && pdata && len ) {
			CopyBlock8( (uint8*)IncPtr( m_Data, m_Pos), (uint8*)pdata, len );
			m_Pos += len;
			return true;
		}
		return false;
	}

	/// Reads specified ammount of data from the buffer
	template< class T >
	bool Read( T* pdata, uint32 number = 1 ) const
	{
		uint32 len = sizeof(T) * number;
		if ( m_Data && pdata && len ) {
			CopyBlock8( (uint8*)pdata, (uint8*)IncPtr( m_Data, m_Pos), len );
			m_Pos += len;

			return true;
		}
		return false;
	}

	/// Rewinds pointer to begin
	inline void Rewind()
	{
		m_Pos = 0;
	}

protected:

	void*			m_Data;
	mutable uint32	m_Pos;

	/// Increments poiner
	void* IncPtr( void* ptr, uint32 incr ) const
	{
		return (void*)((uint8*)(ptr) + (uint32)(incr));
	}
};

/// Generic dynamic buffer class
class iDynamicBuffer : private iDynamicPtr
{
public:

	/// Default constructor
	iDynamicBuffer( uint32 cap = 0, uint32 incr = DYNAMIC_BUFFER_INC )
	{
		m_Capacity	= cap;
		m_Incr		= incr ? incr : DYNAMIC_BUFFER_INC;
		m_Size		= 0;
		m_ReadOnly	= false;

		if ( m_Capacity ) m_Data = new uint8[m_Capacity];
	}

	/// Copy constructor
	iDynamicBuffer( const iDynamicBuffer& buf )
	{
		m_Pos		= buf.m_Pos;
		m_Capacity	= buf.m_Capacity;
		m_Size		= buf.m_Size;
		m_Incr		= buf.m_Incr;

		if ( !m_ReadOnly && buf.m_Data && m_Capacity ) 
		{
			m_Data = new uint8[m_Capacity];
			CopyBlock8( (uint8*)m_Data, (uint8*)buf.m_Data, m_Capacity);
		} 
		else 
		{
			m_Data = buf.m_Data;
		}
	}

	/// Constructs iDynamicBuffer object with specified data
	iDynamicBuffer( void* data, uint32 len, bool rdonly = true )
	{
		m_Pos		= 0;
		m_Capacity	= len;
		m_Size		= len;
		m_Incr		= DYNAMIC_BUFFER_INC;
		m_ReadOnly	= rdonly;

		if ( !m_ReadOnly && data && m_Capacity ) 
		{
			m_Data = new uint8[m_Capacity];
			CopyBlock8( (uint8*)m_Data, (uint8*)data, m_Capacity );
		} 
		else 
		{
			m_Data = data;
		}
	}

	/// Destructor
	~iDynamicBuffer()
	{
		Clean();
	}

	/// Assignment operator
	iDynamicBuffer& operator = ( const iDynamicBuffer& buf )
	{
		if ( !m_ReadOnly )	delete[] m_Data;

		m_Pos		= buf.m_Pos;
		m_Capacity	= buf.m_Capacity;
		m_Size		= buf.m_Size;
		m_Incr		= buf.m_Incr;
		m_ReadOnly	= buf.m_ReadOnly;

		if ( !m_ReadOnly && buf.m_Data && m_Capacity ) 
		{
			m_Data = new uint8[m_Capacity];
			CopyBlock8( (uint8*)m_Data, (uint8*)buf.m_Data, m_Capacity );
		} 
		else 
		{
			m_Data = buf.m_Data;
		}

		return *this;
	}

	/// Cleanups allocated buffer
	void Clean()
	{
		if ( !m_ReadOnly ) 
		{
			delete[] m_Data;
		} 
		m_Data = NULL;

		m_Capacity	= 0;
		m_Size		= 0;
		Rewind();
	}

	/// Rewinds to begin
	void Rewind()
	{
		iDynamicPtr::Rewind();
	}

	/// Resets buffer's content
	void Reset()
	{
		m_Size = 0;
		Rewind();
	}

	/// Reinitializes buffer with specified settings
	void ReInit( uint32 cap = 0, uint32 incr = DYNAMIC_BUFFER_INC )
	{
		Clean();

		m_Capacity = cap;
		if ( m_Capacity ) 
		{
			m_Data = new uint8[m_Capacity];
		}

		m_Incr = incr ? incr : DYNAMIC_BUFFER_INC;
		m_ReadOnly = false;
	}

	/// Increment buffer size
	uint32 IncSize(uint32 offs)
	{
		if ( (m_Size + offs) > m_Capacity ) {
			m_Size = m_Capacity;
			return false;
		} else 	{
			m_Size += offs;
			return true;
		}
	}

	/// Increment current position
	uint32 IncPos(uint32 offs) const
	{
		uint32 oldPos = m_Pos;
		if ( offs + m_Pos > m_Capacity )
		{
			m_Pos = m_Capacity;
		} else {
			m_Pos += offs;
		}
		return m_Pos - oldPos;
	}

	/// Seek to specified position
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


	/// Specify new buffer size
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

	/// Sets new buffer's data
	void SetData( void* data, uint32 length, bool rdonly = true )
	{
		if ( !m_ReadOnly) {
			delete[] data;
		}

		if ( !m_ReadOnly && data && length ) {
			m_Data = new uint8[length];
			CopyBlock8( (uint8*)m_Data, (uint8*)data, length );
		} else 	{
			m_Data = data;
		}

		m_Capacity	= length;
		m_Size		= length;
		m_Incr		= DYNAMIC_BUFFER_INC;
		m_ReadOnly	= rdonly;
		Rewind();
	}

	/// Returns - is it read only buffer or not
	bool IsReadOnly() const
	{
		return m_ReadOnly;
	}

	/// Returns capacity of the buffer
	uint32 GetCapacity() const
	{
		return m_Capacity;
	}

	/// Returns size of the buffer
	uint32 GetSize() const
	{
		return m_Size;
	}

	/// Return default allocator increment value
	uint32 GetIncrement() const
	{
		return m_Incr;
	}

	/// Return pointer to the buffer
	void* GetData() const
	{
		return m_Data;
	}

	/// Return pointer to the current data
	void* GetCurrentData() const
	{
		return IncPtr(m_Data, m_Pos);
	}

	/// Returns current position
	uint32 GetCurrentPos() const
	{
		return m_Pos;
	}

	/// Writes unsigned 8bit value to the buffer
	bool Write( uint8 b )
	{
		return (Write(&b, 1, sizeof(uint8)) > 0);
	}

	/// Writes signed 8bit value to the buffer
	bool Write( sint8 b )
	{
		return (Write(&b, 1, sizeof(sint8)) > 0);
	}

	/// Writes unsigned 16bit value to the buffer
	bool Write( uint16 w )
	{
		return (Write(&w, 1, sizeof(uint16)) > 0);
	}

	/// Writes signed 16bit value to the buffer
	bool Write( sint16 w )
	{
		return (Write(&w, 1, sizeof(sint16)) > 0);
	}

	/// Writes unsigned 32bit value to the buffer
	bool Write( uint32 d )
	{
		return (Write(&d, 1, sizeof(uint32))> 0);
	}

	/// Writes signed 32bit value to the buffer
	bool Write( sint32 d )
	{
		return (Write(&d, 1, sizeof(sint32)) > 0);
	}

	/// Writes unsigned 64bit value to the buffer
	bool Write( uint64 d )
	{
		return (Write(&d, 1, sizeof(uint64))> 0);
	}

	/// Writes signed 64bit value to the buffer
	bool Write( sint64 d )
	{
		return (Write(&d, 1, sizeof(sint64)) > 0);
	}

	/// Writes floating point 32bit value to the buffer
	bool Write( float f )
	{
		return (Write(&f, 1, sizeof(float)) > 0);
	}

	/// Writes narrow string of text to the buffer
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

	/// Writes unicode string of text to the buffer
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

	/// Writes specified scope of raw data to the buffer
	uint32 Write( const void* buffer, uint32 n, uint32 size = 1 )
	{
		uint32 len = n * size;
		if ( buffer && len && !m_ReadOnly ) {
			AddLength( len );

			if ( iDynamicPtr::Write( (uint8*)buffer, len )) 
			{
				if ( m_Pos > m_Size ) m_Size = m_Pos;
				return len / size;
			} 
			else 
			{
				return 0;
			}
		} 
		else 
		{
			return 0;
		}
	}

	/// Reads unsigned 8bit value from the buffer
	bool Read( uint8& b ) const
	{
		return (Read(&b, 1, sizeof(uint8)) > 0);
	}

	/// Reads signed 8bit value from the buffer
	bool Read( sint8& b ) const
	{
		return (Read(&b, 1, sizeof(sint8)) > 0);
	}

	/// Reads unsigned 16bit value from the buffer
	bool Read( uint16& w ) const
	{
		return (Read(&w, 1, sizeof(uint16)) > 0);
	}

	/// Reads signed 16bit value from the buffer
	bool Read( sint16& w ) const
	{
		return (Read(&w, 1, sizeof(sint16)) > 0);
	}

	/// Reads unsigned 32bit value from the buffer
	bool Read( uint32& d ) const
	{
		return (Read(&d, 1, sizeof(uint32)) > 0);
	}

	/// Reads signed 32bit value from the buffer
	bool Read( sint32& d ) const
	{
		return (Read(&d, 1, sizeof(sint32)) > 0);
	}

	/// Reads unsigned 64bit value from the buffer
	bool Read( uint64& d ) const
	{
		return (Read(&d, 1, sizeof(uint64)) > 0);
	}

	/// Reads signed 64bit value from the buffer
	bool Read( sint64& d ) const
	{
		return (Read(&d, 1, sizeof(sint64)) > 0);
	}

	/// Reads floating point 32bit value from the buffer
	bool Read( float& f ) const
	{
		return (Read(&f, 1, sizeof(float)) > 0);
	}

	/// Reads narrow string of text from the buffer
	bool Read( iStringA& s ) const	// with NULL term
	{
		uint32 len;

		if ( !Read( len ) || len > m_Capacity - m_Pos) return false;
		if ( len ) 
		{
			s = iStringA( (const iCharA*)IncPtr( m_Data, m_Pos ), len );
			m_Pos += len*sizeof(iCharA);
		} 
		else 
		{
			s = iStringA("");
		}

		return true;
	}

	/// Reads unicode string of text from the buffer
	bool Read( iStringW& s ) const	// with NULL term
	{
		uint32 len;

		if ( !Read( len ) || len > m_Capacity - m_Pos) return false;
		if ( len ) 
		{
			s = iStringW( (const iCharW*)IncPtr( m_Data, m_Pos ), len );
			m_Pos += len*sizeof(iCharW);
		} 
		else 
		{
			s = iStringW(L"");
		}

		return true;
	}

	/// Reads specified ammount of raw data from the buffer
	uint32 Read( void* buffer, uint32 n, uint32 size = 1 ) const
	{
		uint32 len = n * size;
		return ( (buffer && m_Pos < m_Capacity && (m_Capacity - m_Pos) >= len && iDynamicPtr::Read( (uint8*)buffer, len )) ? n :  0 );
	}

	void AddLength( uint32 ns )
	{
		if ( !m_ReadOnly && ( ns > (m_Capacity - m_Pos))) 
		{
			uint32 newlength = ((m_Pos + ns + m_Incr - 1) / m_Incr) * m_Incr;

			void* lp = new uint8[ newlength ];
			if ( m_Data ) 
			{
				CopyBlock8( (uint8*)lp, (uint8*)m_Data, m_Capacity );
				delete[] m_Data;
			}

			m_Data = lp;
			m_Capacity = newlength;
		}
	}

	/// Returns - is current position meets end of buffer or not
	bool IsEnd() const
	{
		return (m_Pos >= m_Size);
	}

private:

	uint32	m_Capacity;
	uint32	m_Size;
	uint32	m_Incr;
	bool	m_ReadOnly;
};

#endif //_IOLIB_HELIUM_MEMORY_DYNAMIC_BUFFER_H_
/** @} */ // iolib_he
