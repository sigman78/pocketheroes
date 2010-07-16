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
 *	@brief The template to define a range of values defined by minimum and maximum values of class parameter.
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef __HE_RANGE_H
#define __HE_RANGE_H


/// iRange template is used to define a range of values defined by minimum and maximum values of class parameter. Interface of parameter class must support arithmetic, comparision and assigment operators.
template< class T >
struct iRange
{
	// Public
	T min, max;

	/// Default constructor
	inline iRange();
	/// Constructs object with specified min and max values
	inline iRange( const T minVal, const T maxVal );
	/// Copy constructor
	inline iRange( const iRange& range );

	/// Assignment operator
	inline iRange<T>& operator = ( const iRange<T>& other );

	/// Access min value
	inline T Min() const;
	/// Access max value
	inline T Max() const;
	/// Access mid value
	inline T Mid() const;
	/// Access size value
	inline T Size() const;

	/// Test for inclusive range
	inline bool IsWithin( const T val ) const;
	/// Test for inclusive range
	inline bool IsWithin( const	iRange<T> val ) const;

	/// Test for overlapping
	inline bool IsOverlap( const iRange<T> val ) const;

	/// Clamp value
	inline T Clamp( const T val ) const;

	/// Insert a value, stretching extents to accomodate
	inline void Insert( const T val );

	/// Insert a range into range
	inline void Insert( const iRange<T>& range );

	/// Return normalized 0.0f to 1.0f value, representing 'val's position in this range
	inline float Normalize( const T val ) const;

	/// Map normalized value to the range
	inline T Map( float normal ) const;

	/// Zeroise
	inline void Zero();

	/// Normalize range, set min to 0 and max to 1
	inline void Normalize();

	// Shifting range
	inline iRange	operator + ( const T shift );
	inline iRange	operator - ( const T shift );
	inline iRange&	operator += ( const T shift );
	inline iRange&	operator -= ( const T shift );

	// Scale range
	inline iRange	operator * ( const T scale );
	inline iRange	operator / ( const T scale );
	inline iRange&	operator *= ( const T scale );
	inline iRange&	operator /= ( const T scale );
};

//////////////////////////////////////////////////////////////////
// Implementation

template< class T > iRange<T>::iRange()
{}

template< class T > iRange<T>::iRange( const T minVal, const T maxVal )
{
	min = minVal;
	max = maxVal;
}

template< class T > iRange<T>::iRange( const iRange& range )
{
	min	= (T)range.min;
	max = (T)range.max;
}

template< class T > iRange<T>& iRange<T>::operator = ( const iRange<T>& other )
{
	min	= (T)other.min;
	max = (T)other.max;
	return *this;
}

//

template< class T > T iRange<T>::Min() const
{
	return min;
}

template< class T > T iRange<T>::Max() const
{
	return max;
}

template< class T > T iRange<T>::Mid() const
{
	return (min + max) / (T)2;
}

template< class T > T iRange<T>::Size() const
{
	return (max - min);
}

//

template< class T > bool iRange<T>::IsWithin( const T val ) const
{
	return min <= val && val <= max;
}

template< class T > bool iRange<T>::IsWithin( const iRange<T> val ) const
{
	return val.min >= min && val.max <= max;
}

// 

template< class T > bool iRange<T>::IsOverlap( const iRange<T> val ) const
{
	return IsWithin( val.min ) || IsWithin( val.max );
}

//

template< class T > T iRange<T>::Clamp( const T val ) const
{
	return (T < min) ? min : ((T > max) ? max : val);
}
//

template< class T > void iRange<T>::Insert( const T val )
{
	if ( val < min ) min = val;
	if ( val > max ) max = val;
}

template< class T > void iRange<T>::Insert( const iRange<T>& range )
{
	Insert( range.min );
	Insert( range.max );
}

// 

template< class T > float iRange<T>::Normalize( const T val ) const
{
	return (max - min != (T)0) ? (Clamp(val) - min) / (max - min) : 0.0f;
}

template< class T > T iRange<T>::Map( float val ) const
{
	return min + ( max - min ) * val;
}

//

template< class T > void iRange<T>::Zero()
{
	min = max = (T)0;
}

template< class T > void iRange<T>::Normalize()
{
	min = (T)0;
	max = (T)1;
}

//

template< class T > iRange<T> iRange<T>::operator + ( const T shift )
{
	return iRange( min + shift, max + shift );
}

template< class T > iRange<T> iRange<T>::operator - ( const T shift )
{
	return iRange( min - shift, max - shift );
}

template< class T > iRange<T>& iRange<T>::operator += ( const T shift )
{
	min += shift; max += shift; return *this;
}

template< class T > iRange<T>& iRange<T>::operator -= ( const T shift )
{
	min -= shift; max -= shift; return *this;
}

//

template< class T > iRange<T> iRange<T>::operator * ( const T scale )
{
	return iRange( min * scale, max * scale );
}

template< class T > iRange<T> iRange<T>::operator / ( const T scale )
{
	return iRange( min / scale, max / scale );
}

template< class T > iRange<T>& iRange<T>::operator *= ( const T scale )
{
	min *= scale; max *= scale; return *this;
}

template< class T > iRange<T>& iRange<T>::operator /= ( const T scale )
{
	min /= scale; max /= scale; return *this;
}

#endif //__HE_RANGE_H
/** @} */ // iolib_he
