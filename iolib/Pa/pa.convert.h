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

/*****************************************************************************

 [Pa]lladium XML library

 Uses: Gnome libxml2 parser
 
*****************************************************************************/

#ifndef IOLIB_PA_CONVERT_H
#define IOLIB_PA_CONVERT_H

#include "he\he.std.h"
#include "he\he.string.h"
#include "pa\pa.except.h"

class ConversionError : public Exception
{
public:
	virtual const iCharT* What() const
	{
		return _T("Text conversion error.");
	}
};

inline iStringW Ansi2Unicode( const char* str, int len = -1, unsigned int codePage = CP_ACP )
{
	if ( str == 0 || len == 0 ) return iStringW();

	int wlen = ::MultiByteToWideChar( codePage, 0, str, len, 0, 0 );

//	ASSERT( wlen > 0 );
	
	if ( wlen == 0 || ( wlen == 1 && len == -1 ) ) {
		return iStringW();
	}

	if ( len != -1 ) wlen++;

//	ASSERT( wlen > 1 );

	iStringW result( (unsigned int)0, (wlen-1) );

	::MultiByteToWideChar( codePage, 0, str, len, const_cast<WCHAR*>(result.CStr()), wlen );

	return result;
}

inline iStringA Unicode2Ansi( const WCHAR* str, int len = -1, unsigned int codePage = CP_ACP )
{
	if ( str == 0 || len == 0 ) return iStringA();

	int alen = ::WideCharToMultiByte( codePage, 0, str, len, 0, 0, 0, 0 );
//	ASSERT( alen >= 0 );

	if ( alen == 0 || ( alen == 1 && len == -1 ) ) {
		return iStringA();
	}

	if ( len != -1 ) alen++;

//	ASSERT( alen > 1 );

	iStringA result( (unsigned int)0, (alen-1) );

	::WideCharToMultiByte( codePage, 0, str, len, const_cast<char*>(result.CStr()), alen, 0, 0 );

	return result;
}

// ------------------------------------------------------

template< int BufferLength = 128 >
class CvtWW : private NoCopy
{
public:
	CvtWW( const WCHAR* in, unsigned int codePage = CP_ACP )
	: str_( in )
	{}

	~CvtWW()
	{}

	operator const WCHAR* () const
	{ return str_; }

	const WCHAR* str_;
};

template< int BufferLength = 128 >
class CvtAA : private NoCopy
{
public:
	CvtAA( const char* in, unsigned int codePage = CP_ACP )
	: str_( in )
	{}

	~CvtAA()
	{}

	operator const char* () const
	{ return str_; }

	const char* str_;
};

template< int BufferLength = 128 >
class CvtWA : private NoCopy
{
public:

	CvtWA( const WCHAR* in, unsigned int codePage = CP_ACP )
	: str_( buffer_ )
	{
		if ( in == 0 ) {
			str_ = 0;
		} else {
			int lenW = lstrlenW( in ) + 1;
			int lenA = ::WideCharToMultiByte( codePage, 0, in, lenW, 0, 0, 0, 0 );

			if ( lenA > BufferLength ) {
				str_ = static_cast< char * >( malloc( lenA * sizeof( char ) ) );
				if ( str_ == 0 ) throw ConversionError();
			}

			if ( ::WideCharToMultiByte( codePage, 0, in, lenW, str_, lenA, 0, 0 ) == 0 )
			{
				DWORD dwres = GetLastError();
				throw ConversionError();
			}
		}
	}

	~CvtWA()
	{
		if ( str_ != buffer_ ) free( str_ );
	}

	operator const char* () const
	{ return str_; }

	char* str_;
	char buffer_[ BufferLength ];

};

template< int BufferLength = 128 >
class CvtAW : private NoCopy
{
public:

	CvtAW( const char* in, unsigned int codePage = CP_ACP )
	: str_( buffer_ )
	{
		if ( in == 0 ) {
			str_ = 0;
		} else {
			int lenA = lstrlenA( in )  + 1;
			int lenW = lenA;

			if ( lenW > BufferLength ) {
				str_ = static_cast< WCHAR* >( malloc( lenW * sizeof( WCHAR ) ) );
				if ( str_ == 0 ) throw ConversionError();
			}

			if ( ::MultiByteToWideChar( codePage, 0, in, lenA, str_, lenW ) == 0 )
				throw ConversionError();
		}
	}

	~CvtAW()
	{ 
		if ( str_ != buffer_ ) free( str_ );
	}

	operator const WCHAR* () const
	{ return str_; }

	WCHAR* str_;
	WCHAR buffer_[ BufferLength ];
};

#ifdef _UNICODE

#define CvtWT	CvtWW
#define CvtAT	CvtAW
#define	CvtTW	CvtWW
#define CvtTA	CvtWA

#else

#define CvtWT	CvtWA
#define CvtAT	CvtAA
#define CvtTW	CvtAW
#define CvtTA	CvtAA

#endif


#endif //IOLIB_PA_CONVERT_H
