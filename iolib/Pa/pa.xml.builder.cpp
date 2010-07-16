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

#include "libxml/tree.h"
#include "libxml/parser.h"
#include "libxml/entities.h"

#include "pa\pa.xml.builder.h"
#include "pa\pa.convert.utf8.h"

XmlBuilder::XmlBuilder()
: doc_(0)
, node_(0)
{
	Clear();
}

XmlBuilder::~XmlBuilder()
{
	Clear();
}

//////////////////////////////////////////////////////////////////////////

void
XmlBuilder::DropDoc()
{
	node_ = 0;
	if ( doc_ ) {
		xmlFreeDoc( doc_ );
		doc_ = 0;
	}
}

void 
XmlBuilder::InitNew( const char* encoding )
{
	doc_ = xmlNewDoc( BAD_CAST "1.0" );
	doc_->encoding = xmlStrdup( BAD_CAST encoding );
	doc_->charset = XML_CHAR_ENCODING_UTF8;
	doc_->standalone = 1;
}


//////////////////////////////////////////////////////////////////////////

void
XmlBuilder::Clear()
{
	DropDoc();
	InitNew( "UTF-8" );
}

bool
XmlBuilder::Save(const iStringT& fileName)
{
	if ( doc_ == 0 ) throw XmlBuilderException();

	xmlKeepBlanksDefault( 0 );
	//xmlIndentTreeOutput = 1;
	
	CvtTA<> fNameA( fileName.CStr() );
	int bytes = xmlSaveFormatFile( fNameA, doc_, 1 );

	return bytes != 0;
}

void
XmlBuilder::StartElement( const iStringT& name )
{
	check( doc_ != 0 );
	
	CvtTA<> nodeNameA( name.CStr() );
	xmlNodePtr newNode = xmlNewNode( 0, (xmlChar*)nodeNameA.str_ );
	if ( !node_ ) {
		// Root element
		xmlDocSetRootElement( doc_, newNode );
	} else {
		xmlAddChild( node_, newNode );
	}
	node_ = newNode;
}

void
XmlBuilder::EndElement()
{
	check( doc_ != 0 );
	if ( node_ ) {
		node_ = node_->parent;
	} else {
		throw XmlBuilderException();
	}
}

void
XmlBuilder::AddAttribute( const iStringT& name, const iStringT& value )
{
	check( doc_ != 0 );

	CvtTA<> attrNameA( name.CStr() );
	CvtTA<> attrValA( value.CStr(), CP_UTF8 );

	if ( !xmlSetProp( node_, (xmlChar*)attrNameA.str_, (xmlChar*)attrValA.str_) ) throw XmlBuilderException();
}

void
XmlBuilder::AddText(const iStringT& content)
{
	check( doc_ != 0 || node_ != 0 );

	CvtTA<> cntA( content.CStr(), CP_UTF8 );

	xmlNodePtr text = xmlNewText( (xmlChar*)cntA.str_ );
	check( text != 0 );
	
	if ( !xmlAddChild( node_, text ) ) throw XmlBuilderException();
}

void
XmlBuilder::AddComment(const iStringT& comment)
{
	check( doc_ != 0 || node_ != 0 );

	CvtTA<> cntA( comment.CStr(), CP_UTF8 );

	xmlNodePtr text = xmlNewComment( (xmlChar*)cntA.str_ );
	check( text != 0 );

	if ( !xmlAddChild( node_, text ) ) throw XmlBuilderException();
}

void
XmlBuilder::AddCData( const iStringT& data )
{
	check( doc_ != 0 || node_ != 0 );

	CvtTA<> cntA( data.CStr(), CP_UTF8 );

	xmlNodePtr cdata = xmlNewCDataBlock( doc_, (xmlChar*)cntA.str_, lstrlenA( cntA.str_ ) );
	check( cdata != 0 );

	if ( !xmlAddChild( node_, cdata ) ) throw XmlBuilderException();
}

void
XmlBuilder::AddCData( const uint8* ptr, uint32 size )
{
	static xmlChar hexN[] = "0123456789ABCDEF";
	check( doc_ != 0 || node_ != 0 );

	if ( size == 0 ) return;

	// convert to hex
	xmlChar* buff = new xmlChar[ size * 2 + 1 ];
	buff[ size*2 ] = 0;

	for( uint32 n = 0; n < size; n++ ) {
		buff[0+2*n] = hexN[ ptr[n] >> 4 ];
		buff[1+2*n] = hexN[ ptr[n] & 0x0f ];
	}

	xmlNodePtr cdata = xmlNewCDataBlock( doc_, buff, size * 2 );
	check( cdata != 0 );
	delete[] buff;

	if ( !xmlAddChild( node_, cdata ) ) throw XmlBuilderException();
}