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

#ifndef IOLIB_PA_VARIANT_H
#define IOLIB_PA_VARIANT_H

#define WIN32_LEAN_AND_MEAN
#include <ole2.h>

#include "pa.convert.h"

class LexicalCastError : public Exception
{
public:
	virtual const iCharT* What() const
	{
		return _T("Lexical cast error.");
	}
};

class InternalCastError : public Exception
{
public:
	
	InternalCastError( HRESULT error = E_FAIL ) 
	: hr( error )
	{}

	virtual const iCharT* What() const
	{
		return _T("Internal error.");
	}

	HRESULT hr;
};



//
// Almost same as CComVariant

template< class T >
struct VariantTraits {};

//

#define DECL_VTRAITS( type, id, field ) \
	template<> struct VariantTraits< type > { \
		enum { VT = (id) }; \
		static type VARIANT::* const pField; \
	}; \
	__declspec(selectany) type VARIANT::* const VariantTraits< type >::pField = &VARIANT::field; \
	/**/

DECL_VTRAITS( char,	 VT_I1,					cVal	);
DECL_VTRAITS( char*, (VT_I1 | VT_BYREF),	pcVal	);

DECL_VTRAITS( unsigned char, VT_UI1,		bVal	);
DECL_VTRAITS( unsigned char*, (VT_UI1 | VT_BYREF),	pbVal	);

DECL_VTRAITS( short, VT_I2, iVal );
DECL_VTRAITS( short*, (VT_I2 | VT_BYREF ), piVal );

DECL_VTRAITS( unsigned short, VT_UI2, uiVal );
//DECL_VTRAITS( unsigned short*, (VT_UI2 | VT_BYREF), puiVal );

DECL_VTRAITS( int, VT_I4, intVal	);
DECL_VTRAITS( int*, (VT_I4 | VT_BYREF),	pintVal	);

DECL_VTRAITS( unsigned int, VT_UI4, uintVal );
DECL_VTRAITS( unsigned int*, (VT_UI4 | VT_BYREF), puintVal );

DECL_VTRAITS( long, VT_I4, lVal );
DECL_VTRAITS( long*, (VT_I4 | VT_BYREF), plVal );

DECL_VTRAITS( unsigned long, VT_UI4, ulVal );
DECL_VTRAITS( unsigned long*, (VT_UI4 | VT_BYREF), pulVal );

/*
DECL_VTRAITS( __int64, VT_I8, llVal );
DECL_VTRAITS( __int64*, (VT_I8 | VT_BYREF), pllVal );
*/

DECL_VTRAITS( unsigned __int64, VT_UI8, ullVal );
DECL_VTRAITS( unsigned __int64*, (VT_UI8 | VT_BYREF), pullVal );

DECL_VTRAITS( float, VT_R4, fltVal );
DECL_VTRAITS( float*, (VT_R4 | VT_BYREF), pfltVal );

DECL_VTRAITS( double, VT_R8, dblVal );
DECL_VTRAITS( double*, (VT_R8 | VT_BYREF), pdblVal );

DECL_VTRAITS( BSTR, VT_BSTR, bstrVal );
DECL_VTRAITS( BSTR*, (VT_BSTR | VT_BYREF), pbstrVal );


// --------------------------------

class Variant : public tagVARIANT
{
public:
	Variant()
	{
		::VariantInit( this );
	};
	~Variant()
	{
		Clear();
	}

	Variant( const VARIANT& vsrc )
	{
		vt = VT_EMPTY;
		InternalCopy( &vsrc );
	}

	Variant( const Variant& vsrc )
	{
		vt = VT_EMPTY;
		InternalCopy( &vsrc );
	}

	Variant( const WCHAR* str )
	{
		vt = VT_EMPTY;
		*this = str;
	}

	Variant( const char* str )
	{
		vt = VT_EMPTY;
		*this = str;
	}

	Variant( const iStringW& str )
	{
		vt = VT_EMPTY;
		*this = str.CStr();
	}

	Variant( const iStringA& str )
	{
		vt = VT_EMPTY;
		*this = str.CStr();
	}

	Variant( bool src )
	{
		vt = VT_BOOL;
		boolVal = src ? -1 : 0;
	}

#define DECL_VAR_CTOR( type, id, field ) \
	Variant( type src ) { \
		vt = (id); \
		field = src; \
	} \
	/**/

	DECL_VAR_CTOR( int, VT_I4, intVal );
	DECL_VAR_CTOR( unsigned int, VT_UI4, uintVal );

	DECL_VAR_CTOR( BYTE, VT_UI1, bVal );
	DECL_VAR_CTOR( short, VT_I2, iVal );
	DECL_VAR_CTOR( long, VT_I4, lVal );
	DECL_VAR_CTOR( float, VT_R4, fltVal );
	DECL_VAR_CTOR( double, VT_R8, dblVal );
/*
	DECL_VAR_CTOR( LONGLONG, VT_I8, llVal );
	DECL_VAR_CTOR( ULONGLONG, VT_UI8, ullVal );
*/

	DECL_VAR_CTOR( char, VT_I1, cVal );
	DECL_VAR_CTOR( unsigned short, VT_UI2, uiVal );
	DECL_VAR_CTOR( unsigned long, VT_UI4, ulVal );


	//

	Variant& operator = ( const Variant& other )
	{
		InternalCopy( &other );
		return *this;
	}

	Variant& operator = ( const WCHAR* str )
	{
		Clear();
		vt = VT_BSTR;
		bstrVal = ::SysAllocString( str );

		if ( bstrVal == 0 ) throw InternalCastError( E_OUTOFMEMORY );

		return *this;
	}

	Variant& operator = ( const iStringW& str )
	{
		*this = str.CStr();
		return *this;
	}

	Variant& operator = ( const iStringA& str )
	{
		*this = str.CStr();
		return *this;
	}

	Variant& operator = ( const char* str )
	{
		Clear();
		vt = VT_BSTR;
		CvtAW<> wideStr( str );
		bstrVal = ::SysAllocString( wideStr );

		if ( bstrVal == 0 ) throw InternalCastError( E_OUTOFMEMORY );

		return *this;
	}

	Variant& operator = ( bool src )
	{
		if ( vt != VT_BOOL ) {
			Clear();
			vt = VT_BOOL;
		}
		boolVal = src ? -1 : 0;
		return *this;
	}

#define DECL_VAR_COPY( type, id, field ) \
	Variant& operator = ( type src ) { \
	if ( vt != (id) ) { Clear(); vt = (id); } \
	field = src; return *this; } \
	/**/
		
	DECL_VAR_COPY( int, VT_I4, intVal );
	DECL_VAR_COPY( BYTE, VT_UI1, bVal );
	DECL_VAR_COPY( short, VT_I2, iVal );
	DECL_VAR_COPY( long, VT_I4, lVal );
	DECL_VAR_COPY( float, VT_R4, fltVal );
	DECL_VAR_COPY( double, VT_R8, dblVal );
	DECL_VAR_COPY( char, VT_I1, cVal );
	DECL_VAR_COPY( unsigned short, VT_UI2, uiVal );
	DECL_VAR_COPY( unsigned long, VT_UI4, ulVal );
	DECL_VAR_COPY( unsigned int, VT_UI4, uintVal );
/*
	DECL_VAR_COPY( LONGLONG, VT_I8, llVal );
	DECL_VAR_COPY( ULONGLONG, VT_UI8, ullVal );
*/

	DECL_VAR_COPY( BYTE*, VT_UI1 | VT_BYREF, pbVal );
	DECL_VAR_COPY( short*, VT_I2 | VT_BYREF, piVal );
	DECL_VAR_COPY( unsigned short*, VT_UI2 | VT_BYREF, puiVal );
	DECL_VAR_COPY( int*, VT_I4 | VT_BYREF, pintVal );
	DECL_VAR_COPY( unsigned int*, VT_UI4 | VT_BYREF, puintVal );
	DECL_VAR_COPY( long*, VT_I4 | VT_BYREF, plVal );
	DECL_VAR_COPY( unsigned long*, VT_UI4 | VT_BYREF, pulVal );
/*
	DECL_VAR_COPY( LONGLONG*, VT_I8 | VT_BYREF, pllVal );
	DECL_VAR_COPY( ULONGLONG*, VT_UI8 | VT_BYREF, pullVal );
*/

	DECL_VAR_COPY( float*, VT_R4 | VT_BYREF, pfltVal );
	DECL_VAR_COPY( double*, VT_R8 | VT_BYREF, pdblVal );

	///

	void Clear() { ::VariantClear( this ); }

	HRESULT Copy( const VARIANT* src ) 
	{
		return ::VariantCopy( this, const_cast<VARIANT*>( src ) );
	}

	void CopyTo( BSTR* strDst )
	{
		//ASSERT( strDst != 0 && vt == VT_BSTR );
		if ( strDst != 0 && vt == VT_BSTR ) {
			*strDst = ::SysAllocStringByteLen( (char*)bstrVal, ::SysStringByteLen( bstrVal ) );
			if ( *strDst == 0 ) throw InternalCastError( E_OUTOFMEMORY );
		} else if ( vt != VT_BSTR ) throw InternalCastError( E_OUTOFMEMORY );
	}

	bool ChangeType( VARTYPE newOne, const VARIANT* src = 0 )
	{
		VARIANT* pVar = const_cast<VARIANT*>( src );
		if ( pVar == 0 )
			pVar = this;
		return SUCCEEDED( ::VariantChangeType( this, pVar, 0, newOne ) );
	}

	void InternalCopy( const VARIANT* src )
	{
		Copy( src );
	}

	ULONG GetSize()
	{
		ULONG size = sizeof(VARTYPE);

		switch( vt ) {
		case VT_UI1:
		case VT_I1:
			size += sizeof( BYTE );
			break;
		case VT_UI2:
		case VT_I2:
		case VT_BOOL:
			size += sizeof( short );
			break;
		case VT_R4:
		case VT_I4:
		case VT_UI4:
		case VT_INT:
		case VT_UINT:
		case VT_ERROR:
			size += sizeof( long );
			break;
/*
		case VT_I8:
		case VT_UI8:
			size += sizeof( LONGLONG );
			break;
*/
		case VT_R8:
			size += sizeof( double );
			break;
		}

		if ( size == sizeof(VARTYPE) ) {
			BSTR bstr = 0;
			Variant varBSTR;
			if ( vt != VT_BSTR ) {
				if ( SUCCEEDED( ::VariantChangeType( &varBSTR, const_cast<VARIANT*>((const VARIANT*)this), VARIANT_NOVALUEPROP, VT_BSTR ) ) ) {
					bstr = varBSTR.bstrVal;
				}
			} else {
				bstr = bstrVal;
			}

			if ( bstr != 0 )
				size += sizeof( ULONG ) + SysStringByteLen( bstr ) + sizeof( WCHAR );
		}

		return size;
	}

};

// -----------------------------

template< class T >
inline T VariantCast( const Variant& src, const T& = T() )
{
	typedef VariantTraits<T> Traits;

	Variant dst;
	dst.ChangeType( Traits::VT, &src );

	return dst.*Traits::pField;
}

// ------------------------------

template< typename Target, typename Source >
Target LexicalCast( Source arg )
{
	typedef VariantTraits<Target> Traits;

	try {
		Variant var( arg );
		var.ChangeType( Traits::VT );	

		return var.*Traits::pField;
	} catch ( InternalCastError& ) {
		throw LexicalCastError();
	}
}


#endif //IOLIB_PA_VARIANT_H
