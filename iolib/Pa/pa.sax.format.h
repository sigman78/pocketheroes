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

*****************************************************************************/

#ifndef IOLIB_PA_SAX_FORMAT_H
#define IOLIB_PA_SAX_FORMAT_H

#include "pa/pa.sax.handler.h"
#include "pa/pa.variant.h"
#include "pa/pa.serialize.h"

namespace Sax {

	class XmlFormat : public GenericFormat< const char* >
	{
		iStringT docName;
		int indent;
		FILE* output;

	public:
		XmlFormat( const iCharT* document ) : docName( document ) {}

		void StartInput()
		{}
		void StartCompositeInput( IdType id )
		{}
		void EndCompositeInput( IdType id )
		{}
		void EndInput()
		{}

		void InputSimple( iStringT& value, IdType id )
		{
			ReadString( value, id );
		}

		template< class T >
		void InputSimple( T& value, IdType id )
		{
			iStringT buf;
			if ( ReadString( buf, id ) ) value = LexicalCast<T>( buf.CStr() );
		}

		//

		void StartOutput()
		{
			output = _tfopen( docName.CStr(), _T("wt") );
			if ( !output ) throw SerialzeError();
			fputs( "\357\273\277<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\" ?>\n", output );
			indent = 0;
		}
		void StartCompositeOutput( IdType id )
		{
			MakeIndent( indent++ );
			fprintf( output, "<%s>\n", id );
		}
		void EndCompositeOutput( IdType id )
		{
			MakeIndent( --indent );
			fprintf( output, "</%s>\n", id );
		}
		void EndOutput()
		{
			fclose( output );
		}

/*
		void OutputSimple( const iStringA& value, IdType id )
		{
			if ( !value.Empty() ) {
				MakeIndent( indent );
				
				fprintf( output, "<%s>%s</%s>\n", id, value.CStr(), id );
			}
		}
*/
		template<class T>
		void OutputSimple( const T& value, IdType id )
		{
			MakeIndent( indent );

			Variant var( value );
			var.ChangeType( VT_BSTR );
			CvtWA<> abuf( var.bstrVal, CP_UTF8 );
			fprintf( output, "<%s>%s</%s>\n", id, (const char*)abuf, id );
		}

		void OutputSimple( const iStringT& value, IdType id )
		{
			if ( !value.Empty() ) {
				MakeIndent( indent );
				
				CvtTA<> abuf( value.CStr(), CP_UTF8 );
				fprintf( output, "<%s>%s</%s>\n", id, (const char*)abuf, id );
			}
		}

	private:

		bool ReadString( const iStringT& str, IdType id )
		{
			return false;
		}

		void MakeIndent( int n )
		{
			iStringA tmp( ' ', n * 2 );
			fputs( tmp.CStr(), output );
		}

	};

} // namespace Sax

#endif //IOLIB_PA_SAX_FORMAT_H
