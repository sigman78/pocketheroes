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
 *	@brief Generic text string template class implementation
 */
/** @addtogroup iolib_he
 *  @{
 */

#ifndef _IOLIB_HELIUM_STRING_H_
#define _IOLIB_HELIUM_STRING_H_

#include "he.string.std.h"
#include "he.container.simple.array.h"

#define	STRING_INC			16		// String increment ( power of 2 )
#define STRING_BUFFER		8912	// Buffer size used for printf style methods
#define STRING_NUMBERLEN	33		// Buffer for storing itoa results and such

#pragma warning(push)
#pragma warning(disable : 4200)	// Disable non-standard extension : zero size

/// Generic text string template class 
template <class T>
class iStringTemplate
{
public:
	enum 
	{ NOT_FOUND	= -1 };

	/// Default constructor
	inline iStringTemplate<T>()
	{
		str = null;
		siz = len = 0;
	}

	/// Copy constructor
	inline iStringTemplate<T>( const iStringTemplate<T>& s )
	{
		str = null; 
		siz = len = 0;
		*this = s;	// explicit operator = 
	}

	/// Copy of existing string
	inline iStringTemplate<T>( const T* s )
	{
		str = null; 
		siz = len = 0;
		*this = s;	// explicit operator = 
	}

	/// Copy of existing string
	inline iStringTemplate<T>( const T* s, uint32 nr )
	{
		str = null; 
		siz = len = 0;
		Set( s, nr );
	}

	/// Repeated character
	inline iStringTemplate<T>( uint32 ch, uint32 num )
	{
		str = null; siz = len = 0;
		Set( ch, num );
	}

	/// Destructor
	~iStringTemplate<T>()
	{ SetEmpty(); }

	/// Append C style string
	void Append( const T* src, uint32 srclen )
	{
		check( src != NULL && srclen != 0 );
		uint32 newlength = len + srclen;
		uint32 pos = len;
		if ( srclen > 0 ) {
			// reallocate
			Realloc( newlength );

			// move existing bytes down
			MoveBlock8( (uint8*)(str + pos + srclen), (uint8*)(str + pos),(newlength - pos - srclen) * sizeof(T) );
			// copy substring
			CopyBlock8( (uint8*)(str + pos), (uint8*)src, srclen * sizeof(T) );
			len = newlength;
			str[len] = 0;
		}
	}


	/// Vsprintf type concatenation
	sint32 Add( const T* fmt, va_list argptr )
	{
		iStringTemplate<T> temp;
		temp.Set( fmt, argptr );
		Ins( temp.CStr(), len );
		return len;
	}

	/// Sprintf type concatenation
	sint32 VARARG Addf( const T* fmt, ... )
	{
		// format string to the temp buffer
		va_list argptr;
		va_start( argptr, fmt );
		sint32 r = Add( fmt, argptr );
		va_end( argptr );
		return r;
	}


	/// Vsprintf type impl
	void Set( const T* fmt, va_list argptr )
	{
		T buf[STRING_BUFFER];
		// format string to the temp buffer
		sint32 buflen = IOSTRING_STD::VStrNPrintF( buf, STRING_BUFFER-1, fmt, argptr );
		// add buffer to existing string
		if ( buflen < 0 ) {
			buflen = STRING_BUFFER;
		}
		if ( buflen > 0 ) {
			Realloc( buflen );
			CopyBlock8( (uint8*)str, (uint8*)buf, buflen * sizeof(T) );
			str[ buflen ] = 0;
			len = buflen;
		} else SetEmpty();
	}

	/// Sprintf type impl
	void VARARG Setf( const T* fmt, ... )
	{
		// format string to the temp buffer
		va_list argptr;
		va_start( argptr, fmt );
		Set( fmt, argptr );
		va_end( argptr );
	}

	/// Repeated character initialization
	void Set( uint32 ch, uint32 nr )
	{
		Realloc( nr );
		if ( nr > 0 ) {
			T* p = str;
			len = nr;
			while ( nr-- ) {
				*p++ = (T)ch;
			}
			str[len] = 0;
		} else { 
			len = 0;
		}
	}

	/// Set string to existing string with max len capped by nr
	void Set( const T* s, uint32 nr )
	{
		check( s != 0 );
		if ( nr > 0 ) {
			Realloc( nr );
			CopyBlock8( (uint8*)str, (uint8*)s, nr * sizeof(T) );
			str[ nr ] = 0;
			len = nr;
		} else SetEmpty();
	}


	/// Assignment
	iStringTemplate<T>&	operator = ( const iStringTemplate<T>& s )
	{
		Set( s.str, s.len );
		return *this;
	}

	/// Assignment
	iStringTemplate<T>&	operator = ( const T* s )
	{
		check( s != NULL );
		Set( s, IOSTRING_STD::StrLen( s ) );
		return *this;
	}


	/// atoi style impl
	inline	sint32		GetInt( sint32 radix = 10 ) const
	{ return (str) ? (sint32)IOSTRING_STD::StrToL( str, NULL, radix ) : 0; }
		
	/// atoi style impl
	inline	uint32		GetUInt( sint32 radix = 10 ) const
	{ return (str) ? (uint32)IOSTRING_STD::StrToUL( str, NULL, radix ) : 0; }

	/// atoi style impl
	inline	float		GetFloat() const
	{ T* ptr; return (str) ? (float)IOSTRING_STD::StrToD( str, &ptr ) : 0; }

	/// atoi style impl
	inline	double		GetDouble() const
	{ T* ptr;  return (str) ? (double)IOSTRING_STD::StrToD( str, &ptr ) : 0; }


	/// Replaces one character with another
	void Replace( const T* look_for, const T* replace_to )
	{
		check( look_for != NULL );
		if ( len > 0 ) {
			T*	start = str;
			uint32	found = IOSTRING_STD::StrCSpn( str, look_for );
			while ( found >= 0 && found != len ) {
				Del( found );
				Ins( replace_to, found );
				if ( str == 0 ) break;
				found = IOSTRING_STD::StrCSpn( str, look_for );
			}
		}
	}

	/// Strip characters form the string
	void Strip(const T* chars)
	{
		check( chars != NULL );
		if ( len > 0 ) {
			T*	start = str;
			uint32	found = IOSTRING_STD::StrCSpn( str, chars );
			while ( found != len ) {
				Del( found );
				if ( str == 0 ) break;
				found = IOSTRING_STD::StrCSpn( str, chars );
			}
		}
	}

	/// Trim string for whitespaces from left and right
	void Trim( const T* wspaces = NULL )
	{
		if ( str != null ) {
			if ( wspaces == NULL ) {
				wspaces = defwsps;
			}
			// trim from the begining
			sint32 frombegin = IOSTRING_STD::StrSpn( str, wspaces );
			if ( frombegin != 0 ) {
				Del( 0, frombegin );
			}
			if ( len == 0 )	return;
			// trim from the end
			T* end = str + len - 1;
			while ( (end > str) && ( IOSTRING_STD::StrChr( wspaces, *end ) != NULL ) ) {
				--end;
			}
			SetLength( end - str + 1 );
		}
	}

	/// Binary operator
	inline bool operator < ( const T* s ) const
	{ return (CompareWithMatchCase(s) == LESS ); }

	/// Binary operator
	inline bool operator > ( const T* s ) const
	{ return (CompareWithMatchCase(s) == GREATER ); }

	/// Binary operator
	inline bool operator ==( const T* s ) const
	{ return (CompareWithMatchCase(s) == EQUAL ); }

	/// Binary operator
	inline bool operator !=( const T* s ) const
	{ return (CompareWithMatchCase(s) != EQUAL ); }

	/// Binary operator
	inline bool operator <=( const T* s ) const
	{ return (CompareWithMatchCase(s) != GREATER ); }

	/// Binary operator
	inline bool operator >=( const T* s ) const
	{ return (CompareWithMatchCase(s) != LESS ); }

	/// Binary operator
	inline bool operator < ( const iStringTemplate<T>& s ) const
	{ return (CompareWithMatchCase(s) == LESS ); }

	/// Binary operator
	inline bool operator > ( const iStringTemplate<T>& s ) const
	{ return (CompareWithMatchCase(s) == GREATER ); }

	/// Binary operator
	inline bool operator ==( const iStringTemplate<T>& s ) const
	{ return (CompareWithMatchCase(s) == EQUAL ); }

	/// Binary operator
	inline bool operator !=( const iStringTemplate<T>& s ) const
	{ return (CompareWithMatchCase(s) != EQUAL ); }

	/// Binary operator
	inline bool operator <=( const iStringTemplate<T>& s ) const
	{ return (CompareWithMatchCase(s) != GREATER ); }

	/// Binary operator
	inline bool operator >=( const iStringTemplate<T>& s ) const
	{ return (CompareWithMatchCase(s) != LESS ); }

	/// Compares this string object with another string using the generic-text function _tcscmp.
	sint32	Compare( const iStringTemplate<T>& s ) const
	{ return IOSTRING_STD::StrCmp( str, s.str ); }

	/// Compares this string object with another string using the generic-text function _tcscmp.
	sint32	Compare( const T* s ) const
	{ return IOSTRING_STD::StrCmp( str, s ); }

	/// Compares this string object with another string (case insensitive) using the generic-text function _tcsicmp.
	sint32	CompareIgnoreCase( const iStringTemplate<T>& s ) const
	{ return IOSTRING_STD::StrICmp( str, s.str ); }

	/// Compares this string object with another string (case insensitive) using the generic-text function _tcsicmp.
	sint32	CompareIgnoreCase( const T* s ) const
	{ return IOSTRING_STD::StrICmp( str, s ); }

	/// Compares this string object with another string (case insensitive) using the generic-text function _tcsicmp.
	inline bool EqualIgnoreCase( const iStringTemplate<T>& s ) const
	{ return (CompareWith(s) == EQUAL);	}

	/// Compares this string object with another string (case insensitive) using the generic-text function _tcsicmp.
	inline bool EqualIgnoreCase( const T* s ) const
	{ return (CompareWith(s) == EQUAL);	}

	// Direct access
	// Perl addressing supported, indices less than zero takes
	// characters from the end e.g.
	// str[2] - second from the start
	// str[-3] - third from the end

	/// Returns the character at a given position — operator substitution for GetAt
	inline uint32 operator[]( uint32 i ) const
	{ return (*(str + i)); }

	/// Returns the character at a given position — operator substitution for GetAt
	inline T& operator[]( uint32 i )
	{ return (*(str + i)); }

	/// Return length of string in characters
	inline uint32 Length() const
	{ return len; }

	/// Returns - is this empty string or not
	inline bool Empty() const
	{ return len == 0; }

	/// Return size of the string in bytes ( with null terminator )
	inline uint32 Size() const
	{ return ( ( len + 1 ) * sizeof(T) ); }


	/// Set the length of string ( trunication possible )
	void	SetLength( uint32 newlen )
	{
		if ( newlen == 0 ) {
			SetEmpty();
		} else if ( newlen <= len ) {
			len = newlen;
			str[len] = 0;
		} else {
			// nothing to do - need to reallocate
			Realloc( newlen );
		}
	}

	/// Reverse Find a character, returns position or NOT_FOUND 
	sint32	ReverseFind( T c) const
	{
		if (Empty()) return NOT_FOUND;
		T* cPtr = IOSTRING_STD::StrRChr( str, c );
		if ( cPtr == NULL ) return NOT_FOUND;
		return ( cPtr - str );
	}

	/// Find a character, returns position or NOT_FOUND 
	sint32	Find( T c, uint32 start =  0 ) const
	{
		if ( start >= len )	return NOT_FOUND;
		T* cPtr = IOSTRING_STD::StrChr( str + start, c );
		if ( cPtr == NULL ) return NOT_FOUND;
		return ( cPtr - str );
	}

	/// Find a string, returns position or NOT_FOUND
	sint32	Find( const iStringTemplate<T>& s, uint32 start =  0 ) const
	{
		if ( Empty() || s.Empty() )	return NOT_FOUND;
		if ( start >= len )	return NOT_FOUND;
		T* sPtr = IOSTRING_STD::StrStr( str + start, s.str );
		if ( sPtr == NULL )	return NOT_FOUND;
		return ( sPtr - str );
	}

	/// Find a character ignoring case
	inline sint32	FindIgnoreCase( T c, uint32 start = 0 ) const
	{ return FindIgnoreCase( CStr(), c, start ); }

	/// Find a string ignoring case
	inline sint32	FindIgnoreCase( const iStringTemplate<T>& s, uint32 start = 0 ) const
	{ return FindIgnoreCase( CStr(), len, s.CStr(), start ); }

	/// Extract left 'num' characters discarding right side
	iStringTemplate<T>	Left( uint32 num ) const
	{
		if ( Empty() ) 	return iStringTemplate<T>();
		num = ( num > len ? len : num );
		return iStringTemplate<T>( str, num );
	}

	/// Extracts right 'num' characters discarding left side
	iStringTemplate<T>	Right( uint32 num ) const
	{
		if ( Empty() ) return iStringTemplate<T>();
		num = ( num > len ? len : num );
		return iStringTemplate<T>( str + len - num, num );
	}

	/// Extracts part of string ( default to the end )
	iStringTemplate<T>	Mid( uint32 start, uint32 length = 0 ) const
	{
		if ( Empty() || (start >= len) ) return iStringTemplate<T>();
		if ( length == 0 ) {
			length = len - start;	
		}
		return iStringTemplate<T>( str + start, length );
	}

	/// Return size of string allocation
	inline uint32 AllocSize() const
	{ return siz * sizeof(T); }

	/// Extracts next token from this string 
	iStringTemplate<T>	Token( const T* delim )
	{
		check( delim != NULL );
		if ( Empty() ) 	return iStringTemplate<T>();
		// skip leading delimiters
		const T* ptr = str;
		while( *ptr != '\0' && (IOSTRING_STD::StrChr( delim, *ptr ) != NULL)) ptr++;
		// if string empty, return it
		if ( *ptr == '\0' ) {
			SetEmpty();
			return iStringTemplate<T>();
		}
		// find next delimiter occurence
		const T* next = ptr;
		while( *next != '\0' && (IOSTRING_STD::StrChr( delim, *next ) == NULL)) next++;
		// get text between next & ptr into substring, cuts fragment from *this
		check( (next - ptr) > 0 );
		iStringTemplate<T> token( ptr, next - ptr );
		Del( 0, ( next - str ) );
		return token;
	}

	/// Inserts a string 's' at given 'pos'
	void	Ins( const T* s, uint32 pos = 0 )
	{
		check( s != NULL );
		uint32 insertlen = IOSTRING_STD::StrLen( s );
		uint32 newlength = len + insertlen;
		if ( insertlen > 0 ) {
			if ( pos > len ) pos = len;
			// reallocate
			Realloc( newlength );
			// move existing bytes down
			MoveBlock8( (uint8*)(str + pos + insertlen), (uint8*)(str + pos),(newlength - pos - insertlen) * sizeof(T) );
			// copy substring
			CopyBlock8( (uint8*)(str + pos), (uint8*)s, insertlen * sizeof(T) );
			len = newlength;
			str[len] = 0;
		}
	}

	/// Inserts character 'c' at given 'pos' - 'nr' times
	void	Ins( uint32 ch, uint32 pos = 0, uint32 nr = 1	)
	{
		iStringTemplate<T> temp( ch, nr );
		Ins( temp.CStr(), pos );
	}

	/// Delete a character at given 'pos' for 'nr' times
	void	Del( uint32 pos = 0, uint32 nr = 1 )
	{
		if ( nr > 0 && pos < len ) {
			nr = ((pos + nr) > len ? (len - pos) : nr);
			sint32 bcopy = len - ( pos + nr ) + 1;
			CopyBlock8( (uint8*)(str + pos),(uint8*)(str + pos + nr), bcopy * sizeof(T) );
			len = len - nr;
		}
	}

	/// Returns constant character pointer to the string’s data
	inline const T* CStr() const
	{ return static_cast<const T*>(str); }

	/// Returns character pointer to the string’s data
	inline T* Str()
	{ return str; }

	/// Concatenation
	inline friend iStringTemplate<T> operator + ( const iStringTemplate<T>& left, const T* right )
	{
		iStringTemplate<T> tmp( left );
		return tmp += right;
	}

	/// Concatenation
	inline friend iStringTemplate<T> operator + ( const iStringTemplate<T>& left, const iStringTemplate<T>& right )
	{
		iStringTemplate<T> tmp( left );
		return tmp += right;
	}

	/// Inserts left string into right side
	inline friend iStringTemplate<T> operator + ( const T* left, const iStringTemplate<T>& right )
	{
		iStringTemplate<T> tmp( right );
		tmp.Ins( left );
		return tmp;
	}

	/// Inserts left char into right side
	inline friend iStringTemplate<T> operator + ( const T left, const iStringTemplate<T>& right )
	{
		iStringTemplate<T> tmp( right );
		tmp.Ins( left );
		return tmp;
	}

	/// Append
	inline iStringTemplate<T>&	operator += ( const T* right )
	{
		Ins( right, len );
		return *this;
	}

	/// Append
	inline iStringTemplate<T>&	operator += ( const iStringTemplate<T>& right )
	{
		Ins( right.CStr(), len );
		return *this;
	}

	/// Case conversion
	inline void Lower()
	{ if ( str ) IOSTRING_STD::StrLwr( str ); }

	/// Case conversion
	inline void Upper()
	{ if ( str ) IOSTRING_STD::StrUpr( str ); }

	/// Finds substring in current string ignoring case
	static sint32 FindIgnoreCase( const T* str, T c, uint32 start )
	{
		check( str != NULL );
		check( start != 0 );
		const T* ptr = str + start;
		c = IOSTRING_STD::ToUpper(c);
		while ( *ptr != '\0') {
			if ( IOSTRING_STD::ToUpper(*ptr) == c )	return (sint32)(ptr - str);
			ptr++;
		}
		return NOT_FOUND;
	}

	/// Finds substring in current string ignoring case
	static sint32 FindIgnoreCase( const T* str, uint32 len, const T* s, uint32 start )
	{
		check( str != NULL );
		check( s != NULL );
		check( start != 0 );

		const T* ptr = str + start;
		while( *ptr != '\0' ) {
			const T* sptr = ptr;
			const T* pptr = s;
			while ( IOSTRING_STD::ToUpper(*sptr) == IOSTRING_STD::ToUpper(*pptr) ) {
				if ( *pptr == '\0' ) return (sint32)(ptr - str);
				if ( *sptr == '\0' ) return NOT_FOUND;
				sptr++;
				pptr++;
			}
			if ( *pptr == '\0' ) return (sint32)(ptr - str);
			ptr++;
		}
		return NOT_FOUND;
	}

private:
	T*		str;			// the data
	uint32	siz, len;		// allocated and used sizes
	static T null[];		// shared null string
	static T defwsps[];

	enum CompareResult {
		LESS	= 0,
		EQUAL,
		GREATER
	};

	// Returns the number of bytes required for string storage, passed
	// is real length of string ( not including null )
	inline	uint32	NewSize( uint32 needed ) const
	{ return (((needed+STRING_INC+1)/STRING_INC)*STRING_INC); }

	// Reallocate to following size
	// zero size empty string
	void	Realloc( uint32 size )
	{
		uint32 nsiz = NewSize( size );
		// early out
		if ( nsiz == siz ) return;
		siz = nsiz;
		T* tmp = str;
		check( tmp != NULL );
		if ( size != 0 ) {
			str = new T[ siz ];
			if ( tmp != null ) {
				len = ( len >= size ? size : len );
				CopyBlock8( (uint8*)str, (uint8*)tmp, len * sizeof( T ) );
			} else {
				len = 0;
			}
			str[len] = 0;
		} else {
			str = null;
			len = 0;
			siz = 0;
		}
		if ( tmp != null )	delete[] tmp;
	}

	// Set the string to empty ( free if nessesary )
	inline void	SetEmpty()
	{
		if ( str != null ) {
			delete[] str;
			str = null;
		}
		siz = len = 0;
	}

	// Compare functions
	CompareResult	CompareWith( const iStringTemplate<T>& s ) const
	{ return CompareWith( s.str ); }

	CompareResult	CompareWith( const T* s ) const
	{
		if ( s == NULL ) {
			return GREATER;
		} else {
			sint32 res = IOSTRING_STD::StrICmp( str, s );
			if ( res < 0 ) return LESS;
			else if ( res > 0 )	return GREATER;
		}
		return EQUAL;
	}

	CompareResult	CompareWithMatchCase( const iStringTemplate<T>& s ) const
	{ return CompareWithMatchCase( s.str ); }

	CompareResult	CompareWithMatchCase( const T* s ) const
	{
		if ( s == NULL ) {
			return GREATER;
		} else {
			sint32 res = IOSTRING_STD::StrCmp( str, s );
			if ( res < 0 ) return LESS;
			else if ( res > 0 ) return GREATER;
		}
		return EQUAL;
	}

	// Str validate
	static bool IsValidStr( const iStringTemplate<T>& s )
	{
		if ( s.str == null ) return false;
		return true;
	}

	static bool IsValidStr( const T* s )
	{
		if ( s == NULL ) return false;
		return true;
	}

};

template <class T>
T iStringTemplate<T>::null[] = {0};

template <class T>
T iStringTemplate<T>::defwsps[] = { '\t', '\r', '\n', ' ', 0 };

//
//
//
typedef iStringTemplate<iCharW> iStringW;
typedef iStringTemplate<iCharA> iStringA;
typedef iStringTemplate<iCharT> iStringT;

//
//
//
template< class Ch >
iStringTemplate<Ch> VARARG iFormat( const Ch* fmt, ... )
{
	va_list argptr;
	va_start( argptr, fmt );
	iStringTemplate<Ch>	res;
	res.Set( fmt, argptr );
	va_end( argptr );
	return res;
}


//
//
//
namespace Impl {

template< typename TCh >
struct iStringSetter
{
	operator TCh*() { return buf; }

	~iStringSetter() 
	{
		if ( owns ) {
			str = buf;
			delete []buf;
		}
	}

	iStringSetter( const iStringSetter& rhs ) : str( rhs.str ), buf( rhs.buf )
	{
		rhs.owns = false;
		owns = true;
	}

	iStringSetter( iStringTemplate<TCh>& nStr, size_t size ) : str( nStr ), buf( new TCh[ size + 1 ] )
	{
		#ifndef NDEBUG
			memset( buf, 0, sizeof(TCh) * (size + 1) );
		#endif
		*buf = 0;
		owns = true;
	}

protected:
	//iStringSetter& operator=( const iStringSetter& ); 
	mutable bool owns;
	iStringTemplate<TCh>& str;
	TCh* buf;

};

} // namespace Details

template< typename TCh >
inline Impl::iStringSetter<TCh> iStringSet( iStringTemplate<TCh>& str, size_t size )
{ return Impl::iStringSetter<TCh>( str, size ); }

/// Basic class with forbidden copy constructor and assignment operator
class NoCopy
{
public:
	NoCopy() {};
private:
	NoCopy( const NoCopy& );
	NoCopy& operator = ( const NoCopy& );
};


/// Templete class for string conversion
template< int BufferLength = 128 >
class CvtW2W : private NoCopy
{
public:
	CvtW2W( const WCHAR* in, unsigned int codePage = CP_ACP )
	: str_( in )
	{}

	~CvtW2W()
	{}

	operator const WCHAR* () const
	{ return str_; }

	WCHAR* Unsafe() const
	{ return const_cast<WCHAR*>( str_ ); }

	const WCHAR* str_;
};

/// Templete class for string conversion
template< int BufferLength = 128 >
class CvtA2A : private NoCopy
{
public:
	CvtA2A( const char* in, unsigned int codePage = CP_ACP )
	: str_( in )
	{}

	~CvtA2A()
	{}

	operator const char* () const
	{ return str_; }

	char* Unsafe() const
	{ return const_cast<char*>( str_ ); }

	const char* str_;
};

/// Templete class for string conversion
template< int BufferLength = 128 >
class CvtW2A : private NoCopy
{
public:

	CvtW2A( const WCHAR* in, unsigned int codePage = CP_ACP )
	: str_( buffer_ )
	{
		if ( in == 0 ) {
			str_ = 0;
		} else {
			int lenW = lstrlenW( in ) + 1;
			int lenA = ::WideCharToMultiByte( codePage, 0, in, lenW, 0, 0, 0, 0 );

			if ( lenA > BufferLength ) {
				str_ = static_cast< char * >( malloc( lenA * sizeof( char ) ) );
				check(str_ != 0);
			}

			if ( ::WideCharToMultiByte( codePage, 0, in, lenW, str_, lenA, 0, 0 ) == 0 )
			{
				DWORD dwres = GetLastError();
				check(0);
			}
		}
	}

	~CvtW2A()
	{ if ( str_ != buffer_ ) free( str_ ); }

	operator const char* () const
	{ return str_; }

	char* Unsafe() const
	{ return const_cast<char*>( str_ ); }

	char* str_;
	char buffer_[ BufferLength ];

};

/// Templete class for string conversion
template< int BufferLength = 128 >
class CvtA2W : private NoCopy
{
public:

	CvtA2W( const char* in, unsigned int codePage = CP_ACP )
	: str_( buffer_ )
	{
		if ( in == 0 ) {
			str_ = 0;
		} else {
			int lenA = lstrlenA( in )  + 1;
			int lenW = lenA;
			if ( lenW > BufferLength ) {
				str_ = static_cast< WCHAR* >( malloc( lenW * sizeof( WCHAR ) ) );
				check(str_ != 0);
			}
			if ( ::MultiByteToWideChar( codePage, 0, in, lenA, str_, lenW ) == 0 ) {
				check(0);
			}
		}
	}

	~CvtA2W()
	{ if ( str_ != buffer_ ) free( str_ ); }

	operator const WCHAR* () const
	{ return str_; }

	WCHAR* Unsafe() const
	{ return const_cast<WCHAR*>( str_ ); }

	WCHAR* str_;
	WCHAR buffer_[ BufferLength ];
};

#ifdef _UNICODE

#define CvtW2T	CvtW2W
#define CvtA2T	CvtA2W
#define	CvtT2W	CvtW2W
#define CvtT2A	CvtW2A

#else

#define CvtW2T	CvtW2A
#define CvtA2T	CvtA2A
#define CvtT2W	CvtA2W
#define CvtT2A	CvtA2A

#endif

/// Parses and replaces ESC sequences
inline void Text2EscSeq(iStringW &str)
{
	iStringW result;
	if (str.Size() == 0) return;
	sint32 spos=0;
	sint32 fpos=0;
	while (iStringW::NOT_FOUND != (fpos = str.Find('\\',spos))){
		if (fpos-spos) result += str.Mid(spos,fpos-spos);
		switch (str[fpos+1])
		{
		case 'n': result += L"\n"; break;
		case 'r': result += L"\r"; break;
		case 't': result += L"\t"; break;
		case '\\': result += L"\\"; break;
		};
		spos = fpos+2;
	}
	result += str.Mid(spos);
	str = result;
}

/// Parses and replaces ESC sequences
inline void Text2EscSeq(iStringA &str)
{
	iStringA result;
	if (str.Size() == 0) return;
	sint32 spos=0;
	sint32 fpos=0;
	while (iStringA::NOT_FOUND != (fpos = str.Find('\\',spos))){
		if (fpos-spos) result += str.Mid(spos,fpos-spos);
		switch (str[fpos+1]) {
		case 'n': result += "\n"; break;
		case 'r': result += "\r"; break;
		case 't': result += "\t"; break;
		case '\\': result += "\\"; break;
		};
		spos = fpos+2;
	}
	result += str.Mid(spos);
	str = result;
}

/// Parses and replaces ESC sequences
inline void EscSeq2Text(iStringW &str)
{
	iStringW result;
	if (str.Size() == 0) return;
	uint32 pos=0;
	while (pos < str.Length()){
		switch (str[pos]){
		case '\n': result += L"\\n"; break;
		case '\r': result += L"\\r"; break;
		case '\t': result += L"\\t"; break;
		case '\\': result += L"\\\\"; break;
		default: result += str.Mid(pos,1);
		};
		pos++;
	}
	str = result;
}

/// Converts HEX string into integer value
inline uint32 HexStringToInt(const iStringA &str)
{
	check(0);
	return 0;
}

/// Converts HEX string into integer value
inline uint32 HexStringToInt(const iStringW &str)
{
	iCharW *schar;
	uint32 val = wcstoul(str.CStr(),&schar,16);
	return val;
}

typedef iSimpleArray< iStringT >  iStringList;

#endif //_IOLIB_HELIUM_STRING_H_
/** @} */ // iolib_he
