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

#ifndef XXL_GUARD_H__
#define XXL_GUARD_H__

namespace impl {
	
class scope_guard_impl_base
{
public:
	scope_guard_impl_base()
	: dropped_( false )
	{}

	void drop() const
	{ dropped_ = true; }
protected:
	~scope_guard_impl_base()
	{}

	scope_guard_impl_base( const scope_guard_impl_base& other )
	: dropped_( other.dropped_ )
	{ other.drop(); }

	template< typename Z >
	static void safe_execute( Z& z )
	{
		if ( !z.dropped_ ) 
			z.execute();
	}

	mutable bool dropped_;
private:
	scope_guard_impl_base& operator=( const scope_guard_impl_base& );
};

} // impl

typedef const impl::scope_guard_impl_base& scope_guard;

namespace impl {

template< typename F >
class scope_guard_impl0 : public scope_guard_impl_base
{
public:
	static scope_guard_impl0<F> make( F func )
	{ return scope_guard_impl0<F>( func ); }

	~scope_guard_impl0()
	{ safe_execute( *this ); }

	void execute()
	{ func_(); }
protected:
	scope_guard_impl0( F func )
	: func_( func )
	{}
	
	F func_;
};

template< typename F, typename P1 >
class scope_guard_impl1 : public scope_guard_impl_base
{
public:
	static scope_guard_impl1<F,P1> make( F func, P1 param1 )
	{ return scope_guard_impl1<F,P1>( func, param1 ); }

	~scope_guard_impl1()
	{ safe_execute( *this ); }

	void execute()
	{ func_( param1_ ); }
protected:
	scope_guard_impl1( F func, P1 param1 )
	: func_( func )
	, param1_( param1 )
	{}
	
	F func_;
	const P1 param1_;
};

} // impl

template< typename F >
inline impl::scope_guard_impl0<F> make_guard( F func )
{
	return impl::scope_guard_impl0<F>::make( func );
}

template< typename F, typename P1 >
inline impl::scope_guard_impl1<F,P1> make_guard( F func, P1 param1 )
{
	return impl::scope_guard_impl1<F,P1>::make( func, param1 );
}

#endif //XXL_GUARD_H__
