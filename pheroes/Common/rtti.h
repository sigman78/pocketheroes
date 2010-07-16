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
// Compact RTTI lib
//

#ifndef _COMPACT_RTTI_LIBRARY_H_
#define _COMPACT_RTTI_LIBRARY_H_

class NonCopyable
{
	NonCopyable( const NonCopyable& ) {}
	void operator = ( const NonCopyable& ) {}
public:
	NonCopyable() {}
};

typedef void*	CompileTimeIdType;

template< class T >
struct CompileTimeId
{
	static char Cookie;
	static CompileTimeIdType Value()
	{ return &Cookie; }
};

template< class T >
char CompileTimeId<T>::Cookie;

//

class TypeAware : NonCopyable
{
public:

	virtual void* CastTo( CompileTimeIdType ) 
	{ return 0; }

public:

	TypeAware() {}
	virtual ~TypeAware() {}

};

template< class Ptr >
Ptr DynamicCast( TypeAware* object )
{
	if ( !object ) return 0;
	return static_cast<Ptr>( object->CastTo( CompileTimeId<Ptr>::Value() ));
}

#define IMPL_TYPEAWARE(ClassType) \
public: \
virtual void* CastTo( CompileTimeIdType type ) { \
return (CompileTimeId<ClassType*>::Value() == type ? this : 0); \
} \
public:\
/* */

#endif //_COMPACT_RTTI_LIBRARY_H_
