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

#ifndef XXL_ARRAY_META_USER_H__
#define XXL_ARRAY_META_USER_H__

#include "gxl.vector.h"
#include "gxl.metrics.h"
#include "gxl.fixpoint.h"

// this file contains all library type-traits declarations
// which propgates additional speedup of array inner loops
// following decls are available
// 

// this not quite true and probably should not be declared this way
// however it does: 
// a) given type is 'native' type similar to pointers or integers
// b) it should be zero-initialized
// c) has trivial ctor,c-ctor,assignment and dtor
XXL_DECLARE_AS_NATIVE( fix32 	);
XXL_DECLARE_AS_NATIVE( fix64	);

// this is used to discriminate POD (or Plain Old Data) types
// i.e. structures with no ctors or d-tors and with simple
// data members inside (or other PODs otherwise)
struct this_is_real_POD { int a; bool b; };
XXL_DECLARE_AS_POD( this_is_real_POD );

// use to declare 'simple' classes as those which have only 
// one special-purpose constructor and has no complex inner members
XXL_DECLARE_AS_STRUCT_W_CTOR( i2DVector );
XXL_DECLARE_AS_STRUCT_W_CTOR( iPoint	);
XXL_DECLARE_AS_STRUCT_W_CTOR( iSize		);
XXL_DECLARE_AS_STRUCT_W_CTOR( iRect		);




#endif //XXL_ARRAY_META_USER_H__

