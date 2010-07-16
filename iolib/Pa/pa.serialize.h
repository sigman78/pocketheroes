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

#ifndef IOLIB_PA_SERIALIZE_H
#define IOLIB_PA_SERIALIZE_H

#include "pa/pa.except.h"

class SerialzeError : public Exception
{
public:
	virtual const iCharT* What() const
	{
		return _T("Serialization error.");
	}
};


//
//
template< class ID >
class GenericFormat
{
public:

	typedef ID IdType;

	void StartInput();
	void StartCompositeInput( IdType id );
	void EndCompositeInput( IdType id );
	void EndInput();

	void StartOutput();
	void StartCompositeOutput( IdType id );
	void EndCompositeOutput( IdType id );
	void EndOutput();

	template< class T >
	void InputSimple( T& value, IdType id );

	template< class T >
	void OutputSimple( T& value, IdType id );
};

template< class Format >
class ObjectInput
{
protected:
	Format&	format_;
public:

	typedef typename Format::IdType IdType;

	ObjectInput( Format& fmt ) : format_( fmt )
	{
		format_.StartInput();
	}

	~ObjectInput()
	{
		format_.EndInput();
	}

	template< class T >
	ObjectInput& Simple( T& value, IdType id )
	{
		format_.StartCompositeInput( id );
		value.Composite( *this );
		format_.EndCompositeInput( id );
		return *this;
	}

	ObjectInput& Simple( bool& val, IdType id ) { format_.InputSimple( val, id ); return *this; }
	ObjectInput& Simple( char& val, IdType id ) { format_.InputSimple( val, id ); return *this; }
	ObjectInput& Simple( unsigned char& val, IdType id ) { format_.InputSimple( val, id ); return *this; }
	ObjectInput& Simple( short& val, IdType id ) { format_.InputSimple( val, id ); return *this; }
	ObjectInput& Simple( unsigned short& val, IdType id ) { format_.InputSimple( val, id ); return *this; }
	ObjectInput& Simple( int& val, IdType id ) { format_.InputSimple( val, id ); return *this; }
	ObjectInput& Simple( unsigned int& val, IdType id ) { format_.InputSimple( val, id ); return *this; }
	ObjectInput& Simple( long& val, IdType id ) { format_.InputSimple( val, id ); return *this; }
	ObjectInput& Simple( unsigned long& val, IdType id ) { format_.InputSimple( val, id ); return *this; }
	ObjectInput& Simple( float& val, IdType id ) { format_.InputSimple( val, id ); return *this; }
	ObjectInput& Simple( double& val, IdType id ) { format_.InputSimple( val, id ); return *this; }
	ObjectInput& Simple( iStringT& val, IdType id ) { format_.InputSimple( val, id ); return *this; }
//	ObjectInput& Simple( iStringW& val, IdType id ) { format_.InputSimple( val, id ); return *this; }
};

template< class Format >
class ObjectOutput
{
protected:
	Format&	format_;
public:

	typedef typename Format::IdType	IdType;

	ObjectOutput( Format& fmt ) : format_( fmt ) 
	{
		format_.StartOutput();
	}

	~ObjectOutput()
	{
		format_.EndOutput();
	}

	template< class T >
	ObjectOutput& Simple( T& value, IdType id )
	{
		format_.StartCompositeOutput(id);
		const_cast<T&>( value ).Composite( *this ); // Ugly but works
		format_.EndCompositeOutput(id);
		return *this;
	}

	ObjectOutput& Simple( bool& val, IdType id ) { format_.OutputSimple( val, id ); return *this; }
	ObjectOutput& Simple( char& val, IdType id ) { format_.OutputSimple( val, id ); return *this; }
	ObjectOutput& Simple( unsigned char& val, IdType id ) { format_.OutputSimple( val, id ); return *this; }
	ObjectOutput& Simple( short& val, IdType id ) { format_.OutputSimple( val, id ); return *this; }
	ObjectOutput& Simple( unsigned short& val, IdType id ) { format_.OutputSimple( val, id ); return *this; }
	ObjectOutput& Simple( int& val, IdType id ) { format_.OutputSimple( val, id ); return *this; }
	ObjectOutput& Simple( unsigned int& val, IdType id ) { format_.OutputSimple( val, id ); return *this; }
	ObjectOutput& Simple( long& val, IdType id ) { format_.OutputSimple( val, id ); return *this; }
	ObjectOutput& Simple( unsigned long& val, IdType id ) { format_.OutputSimple( val, id ); return *this; }
	ObjectOutput& Simple( float& val, IdType id ) { format_.OutputSimple( val, id ); return *this; }
	ObjectOutput& Simple( double& val, IdType id ) { format_.OutputSimple( val, id ); return *this; }
	ObjectOutput& Simple( iStringT& val, IdType id ) { format_.OutputSimple( val, id ); return *this; }
//	ObjectOutput& Simple( iStringW& val, IdType id ) { format_.OutputSimple( val, id ); return *this; }
};

#endif //IOLIB_PA_SERIALIZE_H


