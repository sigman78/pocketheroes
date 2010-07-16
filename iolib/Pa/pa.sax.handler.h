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

#ifndef IOLIB_PA_SAX_HANLDER_H
#define IOLIB_PA_SAX_HANLDER_H

#include "he\he.std.h"
#include "he\he.string.h"
#include "he\he.container.simple.array.h"

#include "pa.convert.h"
#include "pa.map.h"
#include "pa.sax.parser.h"

namespace Sax {

// Forwards
class Handler;
bool Parse( const iStringT&, Handler* );
bool Parse( const char*, int , Handler* );

/// 
/// Base handler
class Handler : private HandlerBase
{
//	template< class s, class c > friend void Parse( const iStringT&, Handler<s,c> * );
	friend bool Parse( const iStringT& , Handler* );
	friend bool Parse( const char*, int , Handler* );

public:

	typedef AssocMap< iStringT, iStringT > Attributes;
	
	Handler() {}
	virtual ~Handler() {}

	virtual bool OnStartDocument() { return true; }
	virtual bool OnEndDocument() { return true; }

	virtual bool OnStartElement( const iStringT& name, const Attributes& attr ) {return true;}
	virtual bool OnEndElement( const iStringT& name ) {return true;}

	virtual bool OnCharacters( const iStringT& chars ) {return true;}
	virtual bool OnComment( const iStringT& content ) {return true;}
	virtual bool OnCDataBlock( const iStringT& content ) {return true;}

private:

	//TODO:: Compiles only in _UNICODE builds
	iStringT Convert( const xmlChar* str );
	iStringT Convert( const xmlChar* str, int length );

	virtual bool OnStartElement( const xmlChar* name, const xmlChar** attrib );
	virtual bool OnEndElement( const xmlChar* name );
	virtual bool OnCharacters( const xmlChar* chars, int length );
	virtual bool OnComment( const xmlChar* content );
	virtual bool OnCDataBlock( const xmlChar* content, int length );

};

// -----------------------------------------------

inline bool
Handler::OnStartElement( const xmlChar* name, const xmlChar** attrib )
{
	Attributes amap;
	if ( attrib ) {
		for ( const xmlChar** cur = attrib; cur && *cur; cur += 2 ) {
			amap[ Convert( *cur ) ] = Convert( *(cur + 1) );
		}
	}
	return OnStartElement( Convert( name ), amap );
}

inline bool
Handler::OnEndElement( const xmlChar* name )
{
	return OnEndElement( Convert( name ) );
}

inline bool
Handler::OnCharacters( const xmlChar* chars, int length )
{
	return OnCharacters( Convert( chars, length ) );
}

inline bool
Handler::OnComment( const xmlChar* content )
{
	return OnComment( Convert( content ) );
}

inline bool
Handler::OnCDataBlock( const xmlChar* content, int length )
{
	return OnCDataBlock( Convert( content, length ) );
}

inline bool
Parse( const iStringT& fname, Handler* handler )
{
	return handler->Parse( fname );
}

inline bool
Parse( const char* buf, int size, Handler* handler )
{
	return handler->Parse( buf, size );
}


// --------------------------

inline iStringT
Handler::Convert( const xmlChar* str )
{
	return CvtWT<>( Ansi2Unicode( (char*)str, -1, CP_UTF8 ).CStr() );
}

inline iStringT
Handler::Convert( const xmlChar* str, int length )
{
	return CvtWT<>( Ansi2Unicode( (char*)str, length, CP_UTF8 ).CStr() );
}

} // namespace Sax

#endif //IOLIB_PA_SAX_HANLDER_H
