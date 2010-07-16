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

#ifndef _GXLIB_COMMON_TEMPLATES_H_
#define _GXLIB_COMMON_TEMPLATES_H_

#include <math.h>

template< class T >
inline void iSwap( T& a, T& b )
{
	T tmp = a;
	a = b;
	b = tmp;
}

template <class T>
inline T iCLAMP(T mn, T mx, T vl)
{
	check(mn <= mx);
	if (vl >= mn && vl <= mx ) return vl;
	else if (vl < mn) return mn;
	else return mx;
}

template <class T>
inline T iMAX(T x, T y)
{
	if (x > y) return x;
	else return y;
}

template <class T>
inline T iMIN(T x, T y)
{
	if (x < y) return x;
	else return y;
}

template <class T>
inline T iDIF(T x, T y)
{
	return (x>y)?(x-y):(y-x);
}

template <class T>
inline T iABS(T x)
{
	if (x > 0) return x;
	else return -x;
}

template <class T>
inline sint32 iSIGN(T x)
{
	if (x > T((sint32)0)) return 1;
	else if (x < T((sint32)0)) return -1;
	else return 0;
}

inline uint32 iALIGN(uint32 val, uint32 al)
{
	return ((val+(al-1))/al)*al;
}

template<class T>
inline T iWRAP(T val, T minv, T wrap)
{
	check(minv<wrap);
	if (val < minv) return val+wrap-minv;
	if (val >= wrap) return val-wrap+minv;
	return val;
}

inline float RoundFloat(float f)
{
	if (f < 0.0f) return (float)ceil(f-0.5f);
	else return (float)floor(f+0.5f);
}

template< uint32 p >
struct iPowC
{
	template< typename T >
	inline T operator()( T v ) const
	{ return iPowC<p-1>()( v ) * v; }
};

template<>
struct iPowC<0>
{
	template< typename T >
	inline T operator()( T v ) const
	{ return T(1); }
};

template <class T>
inline T iPow(T val, uint32 pow)
{
	if (pow == 0) return T(1);
	while( --pow ) val *= val;
	return val;
}

/*
 *	
 */
template< typename T >
class iSingleton
{
public:
	iSingleton()
	{ 
		ODS(L"---C-tor\n");
		check( p == 0 ); 
		p = static_cast<iSingleton*>(this); 
	}
	~iSingleton()
	{ 
		ODS(L"---D-tor\n");
		check( p != 0 ); 
		p = 0; 
	}

	static T& Inst()
	{ 
		ODS(L"---Inst\n");
		check( p != 0 );
		return *static_cast<T*>( p );	
	}

private:
	static iSingleton* p;
	iSingleton( const iSingleton& );
	void operator=( const iSingleton& );
};

template< typename T >
iSingleton<T>* iSingleton<T>::p = 0;

#endif //_GXLIB_COMMON_TEMPLATES_H_

