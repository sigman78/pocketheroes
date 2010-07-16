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

#ifndef IOLIB_PA_SAX_PARSER_H
#define IOLIB_PA_SAX_PARSER_H

#include "he/he.std.h"
#include "he/he.string.h"
#include "he/he.container.simple.array.h"
#include "pa/pa.except.h"

#ifndef _LIB
	typedef void xmlParserCtxt;
	typedef unsigned char xmlChar;
#endif

class SaxError : public Exception
{
public:
	virtual const iCharT* What() const
	{
		return _T("Sax conversion error.");
	}
};


namespace Sax {

///
/// SAX parsing events handler base class
class HandlerBase
{
public:

	HandlerBase() {}
	virtual ~HandlerBase() {}

//	virtual xmlEntity* OnGetEntity( const xmlChar* name ) { return xmlGetPredefinedEntity( name ); }

	virtual void OnWarning( const iStringT& text ) {}
	virtual void OnError( const iStringT& text ) {}
	virtual void OnFatalError( const iStringT& text ) {}

	virtual bool OnStartDocument() { return true; }
	virtual bool OnEndDocument() { return true; }

	virtual bool OnStartElement( const xmlChar* name, const xmlChar** attrib ) = 0;
	virtual bool OnEndElement( const xmlChar* name ) = 0;

	virtual bool OnCharacters( const xmlChar* chars, int length ) = 0;

	virtual bool OnComment( const xmlChar* content ) = 0;

	virtual bool OnCDataBlock( const xmlChar* content, int length ) = 0;

	void Stop();

	bool Parse( const iStringT& );
	bool Parse( const char* bug, int size );


private:

	xmlParserCtxt*	context_;
};

///
///
	
} // namespace Sax


#endif //IOLIB_PA_SAX_PARSER_H
