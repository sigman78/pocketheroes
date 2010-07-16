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


#ifndef _GXLIB_FIXED_POINT_TYPE_DEF_H_
#define _GXLIB_FIXED_POINT_TYPE_DEF_H_

//
// TODO:: Correct division code ( now can lead to overflow / zero division in some cases )
// TODO:: Add diagnostics
// TODO:: Provide non-templated fixed32 class ( using optimized math from GPP )


namespace pg {

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

//template< size_t N > struct bitmask { enum { value = (1<<(N-1)) | bitmask<N-1>::value }; };
//template<> struct bitmask<0> { enum { value = 0 }; };
template< size_t N, typename T > T low_bits() 
{
	return (~(~(T(0u)) << N));
}

} // impl

//
//
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

	static const this_type zero;

	struct fp {
		const value_type toset_;
		fp( value_type val ) : toset_(val) {}
	};

	//
	inline fixed() : val(0) {}
	inline fixed( const this_type& rhs ) : val( rhs.val ) {}
	inline fixed( int_type i ) : val( value_type(i) << frac_bits ) {}
	inline explicit fixed( float f ) { set( f ); }
	inline fixed( const fp v ) : val( v.toset_ ) {}

//	template< size_t B, size_t F >
//	fixed( const fixed<B,F>& rhs ) : val( convert( rhs.val, B, F ) ) {}

	inline this_type& operator=( const this_type rhs )
	{ val = rhs.val; return *this; }
	inline this_type& operator=( const int_type rhs )
	{ val = value_type(rhs) << frac_bits; return *this; }
	inline this_type& operator=( float f )
	{ set( f ); return *this; }
	inline this_type& operator=( const fp v )
	{ val = v.toset_; return *this; }

//	template< size_t B, size_t F >
//	inline this_type& operator=( const fixed<B,F> fx )
//	{ val = convert( fx.val, B, F ); return *this; }

	inline void set( int_type i ) { val = (value_type(i) << frac_bits); }
	inline void set( float f ) { val = (value_type)( f * float(value_type(1) << frac_bits) + 0.5f ); }
	inline float to_float() const { return float(val) / float(value_type(1) << frac_bits); }

	inline this_type operator-() const
	{ this_type tmp(*this); tmp.val = -tmp.val; return tmp; }

	inline this_type& operator+=( const this_type rhs )	{ val += rhs.val; return *this; }
	inline this_type& operator-=( const this_type rhs )	{ val -= rhs.val; return *this; }
	inline this_type& operator*=( const this_type rhs )	{ val = mul_shift( val, rhs.val ); return *this; }
	inline this_type& operator/=( const this_type rhs )	{ val = shift_div( val, rhs.val ); return *this; }
	//{ val = val / ( rhs.val >> frac_bits ); return *this; }

	inline this_type& operator+=( const int_type rhs )	{ return operator+=( this_type(rhs)); }
	inline this_type& operator-=( const int_type rhs )	{ return operator-=( this_type(rhs)); }
	inline this_type& operator*=( const int_type rhs )	{ val *= rhs; return *this; }
	inline this_type& operator/=( const int_type rhs )	{ val /= rhs; return *this; }

	friend inline this_type operator+( const this_type lhs, const this_type rhs )	{ this_type tmp(lhs); return tmp+=rhs; }
	friend inline this_type operator-( const this_type lhs, const this_type rhs )	{ this_type tmp(lhs); return tmp-=rhs; }
	friend inline this_type operator*( const this_type lhs, const this_type rhs )	{ this_type tmp(lhs); return tmp*=rhs; }
	friend inline this_type operator/( const this_type lhs, const this_type rhs )	{ this_type tmp(lhs); return tmp/=rhs; }


	//
	friend inline this_type operator*( const this_type lhs, const int_type rhs )
	{ this_type tmp(lhs); tmp.val *= rhs; return tmp; }
	friend inline this_type operator*( const int_type lhs, const this_type rhs )
	{ this_type tmp(rhs); tmp.val *= lhs; return tmp; }
	friend inline this_type operator/( const this_type lhs, const int_type rhs )
	{ this_type tmp(lhs); tmp.val /= rhs; return tmp; }
	friend inline this_type operator/( const int_type lhs, const this_type rhs )
	{ this_type tmp(lhs); tmp.val /= rhs; return tmp; }

	//
	inline int_type floor() const
	{ return (val >> frac_bits); }
	inline int_type ceil() const
	{ return (val & (uint_type)impl::low_bits<frac_bits,value_type>()) ? ((val>>frac_bits)+1) : (val>>frac_bits);}

	value_type val;

//#define FIXED_UNSAFE
#ifdef FIXED_UNSAFE
	inline explicit fixed( uint_type i ) : val( value_type(i) << frac_bits ) {}
	inline this_type& operator=( const uint_type rhs ) { val = value_type(rhs) << frac_bits; return *this; }
	inline void set( uint_type i ) { val = (value_type(i) << frac_bits); }

	inline this_type& operator+=( const uint_type rhs )
	{ return operator+=( this_type(rhs)); }
	inline this_type& operator-=( const uint_type rhs )
	{ return operator-=( this_type(rhs)); }

	inline this_type& operator*=( const uint_type rhs )
	{ val *= rhs; return *this; }
	inline this_type& operator/=( const uint_type rhs )
	{ val /= rhs; return *this; }

	friend inline this_type operator*( const this_type lhs, const uint_type rhs )
	{ this_type tmp(lhs); tmp.val *= rhs; return tmp; }
	friend inline this_type operator*( const uint_type lhs, const this_type rhs )
	{ this_type tmp(rhs); tmp.val *= lhs; return tmp; }
	friend inline this_type operator/( const this_type lhs, const uint_type rhs )
	{ this_type tmp(lhs); tmp.val /= rhs; return tmp; }
	friend inline this_type operator/( const uint_type lhs, const this_type rhs )
	{ this_type tmp(lhs); tmp.val /= rhs; return tmp; }
#endif //FIXED_UNSAFE

private:
	inline value_type mul_shift( const value_type a, const value_type b ) const
	{
		typedef typename impl::bits<word_bits*2>::sint big_type;
		big_type v = ((big_type)a * b) >> frac_bits;
		return (value_type)v;
	}

	inline value_type shift_div( const value_type a, const value_type b ) const
	{
		typedef typename impl::bits<word_bits*2>::sint big_type;
		big_type v = (((big_type)a) << frac_bits) / b;
		return (value_type)v;
	}

	/*	
	inline int64 mul_shift( int64 a, int64 b ) const
	{
		int64 res = (a >> frac_bits) * b;
		res += ((a & impl::low_bits<frac_bits,uint64>()) * b) >> frac_bits;
		return res;
	}*/

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

template< size_t Bits, size_t Frac >
const fixed<Bits,Frac> fixed<Bits,Frac>::zero;

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

/////
template< size_t B, size_t F >
bool operator==( typename fixed<B,F>::uint_type a, fixed<B,F> b )
{ return a == b.ceil(); }

template< size_t B, size_t F >
bool operator!=( typename fixed<B,F>::uint_type a, fixed<B,F> b )
{ return a != b.ceil(); }

template< size_t B, size_t F >
bool operator>( typename fixed<B,F>::uint_type a, fixed<B,F> b )
{ return a > b.ceil(); }

template< size_t B, size_t F >
bool operator<( typename fixed<B,F>::uint_type a, fixed<B,F> b )
{ return a < b.ceil(); }

template< size_t B, size_t F >
bool operator>=( typename fixed<B,F>::uint_type a, fixed<B,F> b )
{ return !(a < b); }

template< size_t B, size_t F >
bool operator<=( typename fixed<B,F>::uint_type a, fixed<B,F> b )
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
typedef pg::fixed<64,32> fix64;

#endif //_GXLIB_FIXED_POINT_TYPE_DEF_H_


