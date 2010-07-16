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

/** @file
 *	@brief Fixed point type implementation
 */
/** @addtogroup iolib_he
 *  @{
 */

#ifndef _IOLIB_FIXED_POINT_TYPE_H_
#define _IOLIB_FIXED_POINT_TYPE_H_

namespace pg {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace impl {

template<int n> struct bits_helper {};
template<> struct bits_helper<4> { typedef sint8 sint; typedef uint8 uint; };
template<> struct bits_helper<3> { typedef sint16 sint; typedef uint16 uint; };
template<> struct bits_helper<2> { typedef sint32 sint; typedef uint32 uint; };
template<> struct bits_helper<1> { typedef sint64 sint; typedef uint64 uint; };

template<> struct bits_helper<0> { typedef sint64 sint; typedef uint64 uint; };

template< size_t N > struct bits
{
	enum { select = 
		(N <= 64) +
		(N <= 32) +
		(N <= 16) +
		(N <= 8)
	};
	typedef typename bits_helper< select >::sint sint;
	typedef typename bits_helper< select >::uint uint;
};

template< size_t A, size_t B > struct min_val { enum { value = (A<B) ? A : B }; };
template< size_t A, size_t B > struct max_val { enum { value = (A>B) ? A : B }; };

template< size_t N, typename T > T low_bits() 
{
	return (~(~(T(0u)) << N));
}

} // impl

#endif // DOXYGEN

/// Fixed Point type template class
template< size_t Bits, size_t Frac >
class fixed
{
public:
	typedef sint32 int_type;
	typedef uint32 uint_type;

	typedef fixed<Bits,Frac> this_type;	
	typedef typename impl::bits< Bits >::sint value_type;
	enum { 
		word_bits = Bits,
		frac_bits = Frac,
		int_bits  = Bits - Frac - 1
	};

	//
	inline fixed() : val(0) {}
	inline fixed( const this_type& rhs ) : val( rhs.val ) {}
	inline explicit fixed( int_type i ) : val( value_type(i) << frac_bits ) {}
	inline explicit fixed( float f ) { set( f ); }

	inline this_type& operator=( const this_type rhs )
	{ val = rhs.val; return *this; }
	inline this_type& operator=( const int_type rhs )
	{ val = value_type(rhs) << frac_bits; return *this; }
	inline this_type& operator=( float f )
	{ set( f ); return *this; }

	inline void set( float f ) { val = (value_type)( f * float(value_type(1) << frac_bits) + 0.5f ); }
	inline float to_float() const { return float(val) / float(value_type(1) << frac_bits); }

	inline this_type operator-() const
	{ this_type tmp(*this); tmp.val = -tmp.val; return tmp; }

	inline this_type& operator+=( const this_type rhs )
	{ val += rhs.val; return *this; }
	inline this_type& operator-=( const this_type rhs )
	{ val -= rhs.val; return *this; }

	inline this_type& operator*=( const this_type rhs )
	{ val = mul_shift( val, rhs.val ); return *this; }
	inline this_type& operator/=( const this_type rhs )
	{ val = val / ( rhs.val >> frac_bits ); return *this; }

	friend inline this_type operator+( const this_type lhs, const this_type rhs )
	{ this_type tmp(lhs); return tmp+=rhs; }
	friend inline this_type operator-( const this_type lhs, const this_type rhs )
	{ this_type tmp(lhs); return tmp-=rhs; }
	friend inline this_type operator*( const this_type lhs, const this_type rhs )
	{ this_type tmp(lhs); return tmp*=rhs; }
	friend inline this_type operator/( const this_type lhs, const this_type rhs )
	{ this_type tmp(lhs); return tmp/=rhs; }


	//
	inline this_type& operator*=( const int_type rhs )
	{ val = val * rhs; return *this; }
	inline this_type& operator/=( const int_type rhs )
	{ val = val / rhs; return *this; }
	friend inline this_type operator*( const this_type lhs, const int_type rhs )
	{ this_type tmp(lhs); tmp.val *= rhs; return tmp; }
	friend inline this_type operator*( const int_type lhs, const this_type rhs )
	{ this_type tmp(rhs); tmp.val *= ;hs; return tmp; }
	friend inline this_type operator/( const this_type lhs, const int_type rhs )
	{ this_type tmp(lhs); tmp.val /= rhs; return tmp; }

	//
	inline int_type floor() const
	{ return (val >> frac_bits); }
	inline int_type ceil() const
	{ return (val & (uint_type)impl::low_bits<frac_bits,value_type>()) ? ((val>>frac_bits)+1) : (val>>frac_bits);}

	value_type val;

private:
	inline value_type mul_shift( value_type a, value_type b ) const
	{
		typedef typename impl::bits<word_bits*2>::sint big_type;
		big_type v = ((big_type)a * b) >> frac_bits;
		return (value_type)v;
	}
	
	inline value_type convert( value_type val, size_t bits, size_t frac ) const
	{
		if ( frac > frac_bits ) {
			return val >> (frac-frac_bits);
		} else if ( frac < frac_bits ) {
			return val << (frac_bits-frac);
		} else {
			return val;
		}
	}
};

//////////////////////////////////////////
template< size_t B, size_t F >
bool operator==( fixed<B,F> a, fixed<B,F> b )
{ return a.val == b.val; }

template< size_t B, size_t F >
bool operator!=( fixed<B,F> a, fixed<B,F> b )
{ return a.val != b.val; }

template< size_t B, size_t F >
bool operator>( fixed<B,F> a, fixed<B,F> b )
{ return a.val > b.val; }

template< size_t B, size_t F >
bool operator<( fixed<B,F> a, fixed<B,F> b )
{ return a.val < b.val; }

template< size_t B, size_t F >
bool operator>=( fixed<B,F> a, fixed<B,F> b )
{ return !(a < b); }

template< size_t B, size_t F >
bool operator<=( fixed<B,F> a, fixed<B,F> b )
{ return !(a > b); }

/////

template< size_t B, size_t F >
bool operator==( fixed<B,F> a, typename fixed<B,F>::int_type b )
{ return a.ceil() == b; }

template< size_t B, size_t F >
bool operator!=( fixed<B,F> a, typename fixed<B,F>::int_type b )
{ return a.ceil() != b; }

template< size_t B, size_t F >
bool operator>( fixed<B,F> a, typename fixed<B,F>::int_type b )
{ return a.ceil() > b; }

template< size_t B, size_t F >
bool operator<( fixed<B,F> a, typename fixed<B,F>::int_type b )
{ return a.ceil() < b; }

template< size_t B, size_t F >
bool operator>=( fixed<B,F> a, typename fixed<B,F>::int_type b )
{ return !(a < b); }

template< size_t B, size_t F >
bool operator<=( fixed<B,F> a, typename fixed<B,F>::int_type b )
{ return !(a > b); }

//////

template< size_t B, size_t F >
bool operator==( typename fixed<B,F>::int_type a, fixed<B,F> b )
{ return a == b.ceil(); }

template< size_t B, size_t F >
bool operator!=( typename fixed<B,F>::int_type a, fixed<B,F> b )
{ return a != b.ceil(); }

template< size_t B, size_t F >
bool operator>( typename fixed<B,F>::int_type a, fixed<B,F> b )
{ return a > b.ceil(); }

template< size_t B, size_t F >
bool operator<( typename fixed<B,F>::int_type a, fixed<B,F> b )
{ return a < b.ceil(); }

template< size_t B, size_t F >
bool operator>=( typename fixed<B,F>::int_type a, fixed<B,F> b )
{ return !(a < b); }

template< size_t B, size_t F >
bool operator<=( typename fixed<B,F>::int_type a, fixed<B,F> b )
{ return !(a > b); }

//
template< size_t B, size_t F >
fixed<B,F> sin_1( const fixed<B,F> x )
{	
	typedef fixed<B,F> ftype;
	ftype x3 = x * x * x;
	ftype x5 = x3 * x * x;
	ftype x7 = x5 * x * x;
	static const ftype k1( 2796203.f / 16777216.f );
	static const ftype k2( 0.00833282412f );
	static const ftype k3( 0.000195878418f );
	return x - k1 * x3 + k2 * x5 - k3 * x7;
}

} // pg

typedef pg::fixed<32,16> fix32;

#endif //_IOLIB_FIXED_POINT_TYPE_H_
/** @} */ // iolib_he
