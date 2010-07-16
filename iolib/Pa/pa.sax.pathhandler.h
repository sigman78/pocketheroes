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

#ifndef IOLIB_PA_SAX_PATHHANDLER_H
#define IOLIB_PA_SAX_PATHHANDLER_H

#include "pa.sax.handler.h"

namespace Sax {

class PathHandler : public Handler
{
public:

	virtual bool OnEnterElement( const iStringT& path, const Attributes& attr ) { return true; }
	virtual bool OnLeaveElement( const iStringT& path ) { return true; }

	bool SameElement( const iStringT& path ) const
	{
		return workPath_ == path;
	}

	virtual bool OnStartElement( const iStringT& name, const Attributes& attr )
	{
		PushLevel( name );
		return OnEnterElement( workPath_, attr );
	}

	virtual bool OnEndElement( const iStringT& name )
	{
		bool result = OnLeaveElement( workPath_ );
		PopLevel();
		return result;
	}

private:
	iStringT workPath_;
	
	bool IsHierarchyRoot() const
	{
		return workPath_.Empty();
	}

	iStringT PopLevel()
	{
		iStringT result;
		if ( !IsHierarchyRoot() ) {
			sint32 pos = workPath_.ReverseFind( _T('.') );
			if ( pos >= 0 ) {
				result = workPath_.Mid( pos + 1 );
				workPath_ = workPath_.Left( pos );
			} else {
				result = workPath_;
				workPath_.SetLength( 0 );
			}
		}
		return result;
	}

	void PushLevel( const iStringT& level )
	{
		if ( IsHierarchyRoot() ) {
			workPath_ = level;
		} else {
			workPath_ += _T(".");
			workPath_ += level;
		}
	}

};

} // namespace Sax

#endif //IOLIB_PA_SAX_PATHHANDLER_H
