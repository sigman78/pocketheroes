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

//
//

#include <cstdarg>

#include "libxml/parser.h"
#include "libxml/parserInternals.h"
#include "libxml/entities.h"

#include "pa.sax.parser.h"
#include "pa.convert.h"


namespace Sax {


/// --------------------------------------------------------
extern "C" {

/*
	inline xmlEntity* zGetEntity( void* X, const xmlChar* name )
	{
		HandlerBase* handler = static_cast<HandlerBase*>(X);
		return handler->OnGetEntity( name );
	}
*/

	inline void zStartDocument( void* X )
	{
		HandlerBase* handler = static_cast<HandlerBase*>(X);
		if ( !handler->OnStartDocument() ) handler->Stop();
	}

	inline void zEndDocument( void* X )
	{
		HandlerBase* handler = static_cast<HandlerBase*>(X);
		if ( !handler->OnEndDocument() ) handler->Stop();
	}

	inline void zStartElement( void* X, const xmlChar* name, const xmlChar** attrs )
	{
		HandlerBase* handler = static_cast<HandlerBase*>(X);
		if ( !handler->OnStartElement(name, attrs) ) handler->Stop();
	}

	inline void zEndElement( void* X, const xmlChar* name )
	{
		HandlerBase* handler = static_cast<HandlerBase*>(X);
		if ( !handler->OnEndElement( name ) ) handler->Stop();
	}

	inline void zCharacters( void* X, const xmlChar* chars, int len )
	{
		HandlerBase* handler = static_cast<HandlerBase*>(X);
		if ( !handler->OnCharacters( chars, len ) ) handler->Stop();
	}

	inline void zComment( void* X, const xmlChar* content )
	{
		HandlerBase* handler = static_cast<HandlerBase*>(X);
		if ( !handler->OnComment( content) ) handler->Stop();
	}

	inline void zCDATABlock( void* X, const xmlChar* chars, int len )
	{
		HandlerBase* handler = static_cast<HandlerBase*>(X);
		if ( !handler->OnCDataBlock( chars, len ) ) handler->Stop();
	}

	inline void zWarning( void* X, const char* fmt, ... )
	{
		va_list arg;
		va_start( arg, fmt );

		//iStringT buf = VPrintf( fmt, arg );
		char buffer[512];
		_vsnprintf( buffer, sizeof( buffer ) / sizeof( char ), fmt, arg );
		va_end( arg );

		HandlerBase* handler = static_cast<HandlerBase*>(X);
		handler->OnWarning( iStringT( CvtAT<>( buffer ) ) );
	}

	inline void zzError( void* X, const char* fmt, ... )
	{
		va_list arg;
		va_start( arg, fmt );

		//	iStringT buf = VPrintf( fmt, arg );
		char buffer[512];
		_vsnprintf( buffer, sizeof( buffer ) / sizeof( char ), fmt, arg );
		va_end( arg );

		HandlerBase* handler = static_cast<HandlerBase*>(X);
		handler->OnError( iStringT( CvtAT<>( buffer ) ) );
	}

	inline void zFatalError( void* X, const char* fmt, ... )
	{
		va_list arg;
		va_start( arg, fmt );

		//	iStringT buf = VPrintf( fmt, arg );
		char buffer[512];
		_vsnprintf( buffer, sizeof( buffer ) / sizeof( char ), fmt, arg );
		va_end( arg );

		HandlerBase* handler = static_cast<HandlerBase*>(X);
		handler->OnFatalError( iStringT( CvtAT<>( buffer ) ) );
	}

}
/// --------------------------------------------------------

bool HandlerBase::Parse( const iStringT& fname )
{
	// FName converted to the ascii
	CvtTA<> fnameAnsi( fname.CStr() );

	context_ = xmlCreateFileParserCtxt( fnameAnsi );
	if ( !context_ ) throw SaxError();

	xmlSAXHandler sh = 
	{
		0,0,0,0,0,
		0, //zGetEntity,
		0,0,0,0,0,0,
		zStartDocument,
		zEndDocument,
		zStartElement,
		zEndElement,
		0,
		zCharacters,
		0,0,
		zComment,
		zWarning,
		zzError,
		zFatalError,
		0,
		zCDATABlock,
		0
	};

	context_->sax = &sh;
	context_->userData = this;
	xmlSubstituteEntitiesDefault( 1 );
	xmlKeepBlanksDefault( 0 );
	xmlParseDocument( context_ );
	bool wellFormed = context_->wellFormed ? true : false;
	context_->sax = 0;
	xmlFreeParserCtxt( context_ );

	return wellFormed;
}

bool HandlerBase::Parse( const char* buf, int size)
{
	context_ = xmlCreateMemoryParserCtxt( buf, size );
	if ( !context_ ) throw SaxError();

	xmlSAXHandler sh = 
	{
		0,0,0,0,0,
		0, //zGetEntity,
		0,0,0,0,0,0,
		zStartDocument,
		zEndDocument,
		zStartElement,
		zEndElement,
		0,
		zCharacters,
		0,0,
		zComment,
		zWarning,
		zzError,
		zFatalError,
		0,
		zCDATABlock,
		0
	};

	context_->sax = &sh;
	context_->userData = this;
	xmlSubstituteEntitiesDefault( 1 );
	xmlKeepBlanksDefault( 0 );
	xmlParseDocument( context_ );
	bool wellFormed = context_->wellFormed ? true : false;
	context_->sax = 0;
	xmlFreeParserCtxt( context_ );

	return wellFormed;
}


void
HandlerBase::Stop()
{ 
	xmlStopParser( context_ ); 
}

} // namespace