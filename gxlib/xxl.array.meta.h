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

#ifndef XXL_ARRAY_META_MAIN_H__
#define XXL_ARRAY_META_MAIN_H__

#include "xxl.array.cfg.h"

namespace meta {

//
// meta bool, true and false types

template< bool Val >
struct bool_
{
	typedef bool_<Val>		type;
	typedef bool			value_type;
	
	enum { value = 			Val };
//	operator bool() const	{ return (bool)(this->value); }
};

typedef bool_<true>		true_;
typedef bool_<false>    false_;

//
// conditional (hack in lack of partial specialization)
template< bool Condition >
struct if_cond_aux
{
	template< typename Then, typename Else >
	struct eval {
		typedef Then type;
	};
};

template<>
struct if_cond_aux<false>
{
	template< typename Then, typename Else >
	struct eval {	
		typedef Else type;
	};
};

template< bool Condition, typename Then, typename Else >
struct if_cond_
{
	typedef typename if_cond_aux<Condition>::template eval<Then,Else>::type type;
};

template< typename B, typename Then, typename Else >
struct if_ : if_cond_< B::value, Then, Else >
{};

//
// logic ops (accepts true_ and false_ types as params)
template< typename T >
struct not_ : true_
{};

template<>
struct not_<true_> : false_
{};

template< typename T1, typename T2 >
struct and_ : false_
{};

template<>
struct and_<true_,true_> : true_
{};

template< typename T1, typename T2 >
struct or_ : true_
{};

template<>
struct or_<false_,false_> : false_
{};

//
// primitive type traits impl

// is ref test (taken from boost)
namespace impl {

	template< typename T >	struct wrap {};
	
	template< typename T >
	T&	(*isr1_litmus_( wrap<T> ))( wrap<T> );
	char isr1_litmus_( ... );

	template< typename T >
	char* isr2_litmus_( T&(*)(wrap<T>) );
	char  isr2_litmus_( ... );

	template< typename T >
	struct is_ref_boost_test {

		enum { result = (sizeof(char) == sizeof( isr2_litmus_( isr1_litmus_(wrap<T>()) ) )) };
		
	};

} // impl

template< typename T >
struct is_ref : bool_< impl::is_ref_boost_test<T>::result >
{};

template<>
struct is_ref<void> : false_
{};

// since we are going for 'bad' compilers - use adobe technique
namespace impl {
	
	template< typename T >
	struct is_ptr_adobe_test {
		static T& 	dummy();

		struct shim {
			shim( const volatile void* );
		};

		static char		isp_litmus_( bool, shim );
		static char*	isp_litmus_( bool, ... );

		enum { result = (sizeof( isp_litmus_(false, dummy()) ) == sizeof(char)) };
	};


} // impl

template< typename T >
struct is_ptr : bool_< impl::is_ptr_adobe_test<T>::result >
//struct is_ptr : and_< typename not_< typename is_ref<T>::type >::type, typename bool_< impl::is_ptr_adobe_test<T>::result >::type >
{};

template<>
struct is_ptr<void> : false_
{};


// beware, hackers technique again
namespace impl {
	
	template< typename Src, typename Dst >
	struct conversion_helper {	
		static char  cvt_litmus_( bool, Dst );
		static char* cvt_litmus_( bool, ... );
		static Src maker();

		enum { result = sizeof( cvt_litmus_(false, maker()) ) == sizeof(char) };
	};

} // impl

template< typename Src, typename Dst >
struct is_convertible : bool_< impl::conversion_helper< Src*, const volatile Dst* >::result >
{};

template< typename Src, typename Dst >
struct is_cv_convertible_ptrs : bool_< impl::conversion_helper< Src, Dst >::result >
{};

template< typename Cond >
struct is_cv_convertible_if_aux {
	template< typename Src, typename Dst >
	struct eval {
		typedef typename is_cv_convertible_ptrs< Src, Dst >::type type;
	};
};

template<>
struct is_cv_convertible_if_aux<false_> {
	template< typename Src, typename Dst >
	struct eval {
		typedef false_ type;
	};
};

//template< typename Cond, typename Src, typename Dst >
//struct is_cv_convertible_if : is_cv_convertible_if_aux<Cond>::template eval<Src,Dst>::type
//{};

namespace impl {

	template< typename T >
	char	iss_litmus_( bool, T const volatile*, T const volatile* );
	char*   iss_litmus_( bool, ... );
	
	template< typename T1, typename T2 >
	struct is_same_adobe_test {
		//static T1* dummy1();
		//static T2* dummy2();
		static T1 t1_;
		static T2 t2_;
			
		//enum { result = (sizeof(char) == sizeof( iss_litmus_(false,dummy1(),dummy2()) )) };
		enum { test1 = (sizeof(char) == sizeof( iss_litmus_(false,&t1_,&t2_) )) };
		// those additional tests are from the boost 
		enum { test2 = test1 && (sizeof(T1) == sizeof(T2)) };
		enum { result = test2 && ( (bool)is_ref<T1>::value == (bool)is_ref<T2>::value) };
	};

} // impl


template< typename T1, typename T2 >
struct is_same : bool_< impl::is_same_adobe_test< T1, T2 >::result >
{};

// native tests

template< typename T >
struct is_native : false_
{};

// internal use only
#define XXL_DECLARE_AS_NATIVE( type ) \
	namespace meta { \
	template<> struct is_native< type > : true_ {}; \
	template<> struct is_native< const type > : true_ {}; \
	template<> struct is_native< volatile type > : true_ {}; \
	template<> struct is_native< const volatile type > : true_ {}; \
	} \
	/* */

// POD test

template< typename T >
struct is_pod : or_< typename is_ptr< T >::type, typename is_native< T >::type >
{};

// declaration of the POD types
#define XXL_DECLARE_AS_POD( type ) \
	namespace meta { \
	template<> struct is_pod< type > : true_ {}; \
	template<> struct is_pod< const type > : true_ {}; \
	template<> struct is_pod< const volatile type > : true_ {}; \
	template<> struct is_pod< volatile type > : true_ {}; \
	} \
	/* */

// TRIVIAL tests

template< typename T >
struct has_trivial_assignment : is_pod< T >
{};

template< typename T >
struct has_trivial_ctor : is_pod< T >
{};

template< typename T >
struct has_trivial_copy_ctor : is_pod< T >
{};

template< typename T >
struct has_trivial_dtor : is_pod< T >
{};

// used to designate structs with initializers
#define XXL_DECLARE_AS_STRUCT_W_CTOR( type ) \
	namespace meta { \
	template<> struct has_trivial_assignment< type > : true_ {}; \
	template<> struct has_trivial_copy_ctor< type > : true_ {}; \
	template<> struct has_trivial_dtor< type > : true_ {}; \
	} \
	/* */

//
// deduce optimal parameter type
// usage: 
// template<T> struct A { 
// typedef typename decl_param<T>::type param_type;
template< typename T >
struct decl_param {
	
	typedef typename if_< typename is_ref<T>::type, T, const T& >::type Ref;

	typedef typename is_pod< T >::type Pod;
	//typedef typename meta::if_< typename meta::is_ref<T>::type, meta::true_, typename meta::is_pod< T >::type >::type Pod;
	typedef typename bool_< sizeof(T) <= sizeof(long) >::type Small;
	typedef typename and_< Pod, Small >::type SmallPod;

	typedef typename if_< SmallPod, T, Ref >::type type;
};


//
//
// OR litmus tests for types

template< typename Src, typename Dst >
struct trivial_native_copy
{
	typedef typename is_ptr<Src>::type	Ptr1;
	typedef typename is_ptr<Dst>::type	Ptr2;
	typedef typename and_<Ptr1,Ptr2>::type BothPtrs;

	//typedef typename is_cv_convertible_if<BothPtrs, Src, Dst>::type Convertible;
	typedef typename if_< BothPtrs, typename is_cv_convertible_ptrs< Src, Dst >::type, false_ >::type Convertible;

	typedef typename and_<BothPtrs,Convertible>::type Trivial1;	

	typedef typename bool_< sizeof(Src)==sizeof(Dst) >::type SameSize;

	typedef typename is_native<Src>::type Native1;
	typedef typename is_native<Dst>::type Native2;
	typedef typename and_<Native1,Native2>::type BothNatives;

	typedef typename and_<BothNatives,SameSize>::type Trivial2;

	typedef typename or_<Trivial1,Trivial2>::type type;
	
};

template< typename Src, typename Dst >
struct trivial_copy
{
	// either its natively copyable
	typedef typename trivial_native_copy<Src,Dst>::type	Native;

	// or hasTrivialAssignment && sameUnCVtype
	typedef typename has_trivial_assignment<Src>::type	TrivialAssignment;
	typedef typename is_same<Src,Dst>::type				SameUnCV;
	typedef typename and_<TrivialAssignment,SameUnCV>::type	Trivial1;

	typedef typename or_<Native,Trivial1>::type type;

	static type test() { return type(); }
};

//template< typename Src, typename Dst >
//trivial_copy<Src,Dst> use_trivial_copy( Src*, Dst* )
//{ return trivial_copy<Src,Dst>(); }

template< typename Src, typename Dst >
typename trivial_copy<Src,Dst>::type
use_trivial_copy( Src*, Dst* )
{ typedef typename trivial_copy<Src,Dst>::type res; return res(); }

template< typename Src, typename Dst >
struct trivial_ucopy
{
	// either its natively copyable
	typedef typename trivial_native_copy<Src,Dst>::type	Native;

	// or hasTrivialAssignment && sameUnCVtype
	typedef typename has_trivial_copy_ctor<Src>::type	TrivialCopyCtor;
	typedef typename is_same<Src,Dst>::type				SameUnCV;
	typedef typename and_<TrivialCopyCtor,SameUnCV>::type	Trivial1;

	typedef typename or_<Native,Trivial1>::type type;

	static type test() { return type(); }
};

//template< typename Src, typename Dst >
//trivial_ucopy<Src,Dst> use_trivial_ucopy( Src*, Dst* )
//{ return trivial_ucopy<Src,Dst>(); }

template< typename Src, typename Dst >
typename trivial_ucopy<Src,Dst>::type
use_trivial_ucopy( Src*, Dst* )
{ typedef typename trivial_ucopy<Src,Dst>::type res; return res(); }

template< typename T >
struct zero_init
{
	typedef typename is_native<T>::type	Native;
	typedef typename is_ptr<T>::type	Ptr;

	typedef typename or_<Native,Ptr>::type	type;

	static type test() { return type(); }
};

//template< typename T >
//zero_init<T> use_zero_init( T* )
//{ return zero_init<T>(); }

template< typename T >
typename zero_init<T>::type
use_zero_init( T* )
{ typedef typename zero_init<T>::type res; return res(); }

template< typename T >
struct trivial_init
{
	typedef typename has_trivial_ctor<T>::type	TrivialCtor;

	typedef typename zero_init<T>::type			ZeroInit;
	typedef typename not_<ZeroInit>::type		NotZeroInit;

	typedef typename and_<TrivialCtor,NotZeroInit>::type type;

	static type test() { return type(); }
};

//template< typename T >
//trivial_init<T> use_trivial_init( T* )
//{ return trivial_init<T>(); }

template< typename T >
typename trivial_init<T>::type
use_trivial_init( T* )
{ typedef typename trivial_init<T>::type res; return res(); }

//
// built-in classes discrimination
// ( as in stlport )
template< typename T >
struct internal_class
{ typedef T type; };

// checks whether the class in built-in or not
template< typename T >
struct is_internal : is_convertible< T, internal_class<T> >
{};

//
// swap specializations in case of hacky compilers
template< typename T >
struct is_swappable : is_internal< T >
{};


} // meta

// here we declare the basic types
// NOTE: its should be done at the global scope!!!

XXL_DECLARE_AS_NATIVE( bool );
XXL_DECLARE_AS_NATIVE( char );
XXL_DECLARE_AS_NATIVE( unsigned char );
XXL_DECLARE_AS_NATIVE( signed char );
XXL_DECLARE_AS_NATIVE( short );
XXL_DECLARE_AS_NATIVE( unsigned short );
XXL_DECLARE_AS_NATIVE( int );
XXL_DECLARE_AS_NATIVE( unsigned int );
XXL_DECLARE_AS_NATIVE( long );
XXL_DECLARE_AS_NATIVE( unsigned long );

// here the the POD types

#include "xxl.array.user.h"

#endif //XXL_ARRAY_META_MAIN_H__




