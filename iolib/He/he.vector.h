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
 *	@brief Vector templates implementation
 */
/** @addtogroup iolib_he
 *  @{
 */

#ifndef _IOLIB_HELIUM_VECTOR_H_
#define _IOLIB_HELIUM_VECTOR_H_

/// 2D Vector class
class i2DVector
{
public:
	float x,y;

	/// Default constructor
	i2DVector()
	: x(0.0f)
	, y(0.0f)
	{}

	/// Constructs vector with given initialization values
	i2DVector(float nx, float ny)
	: x(nx)
	, y(ny)
	{}

	/// Equality operator
	bool operator == ( const i2DVector& vector ) const
	{ return ( x == vector.x && y == vector.y ); }

	/// Inequality operator
	bool operator != ( const i2DVector& vector ) const
	{ return ! operator == ( vector ); }

	/// operator +=
	inline i2DVector& operator += ( float scalar )
	{
		x += scalar;
		y += scalar;
		return *this;
	}

	/// operator -=
	inline i2DVector& operator -= ( float scalar )
	{
		x -= scalar;
		y -= scalar;
		return *this;
	}

	/// operator *=
	inline i2DVector& operator *= ( float scalar )
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}
	
	/// operator /=
	inline i2DVector& operator /= ( float scalar )
	{
		check( scalar != 0.0f );
		x /= scalar;
		y /= scalar;
		return *this;
	}

	/// operator +
	inline i2DVector operator + ( float scalar ) const
	{ return i2DVector( *this ) += scalar; }

	/// operator -
	inline i2DVector operator - ( float scalar ) const
	{ return i2DVector( *this ) -= scalar; }
	
	/// operator *
	inline i2DVector operator * ( float scalar ) const
	{ return i2DVector( *this ) *= scalar; }
	
	/// operator /
	inline i2DVector operator / ( float scalar ) const
	{ return i2DVector( *this ) /= scalar; }

	/// friend operator +
	friend i2DVector operator + ( float scalar, const i2DVector& vector )
	{ return vector + scalar; }
	
	/// friend operator -
	friend i2DVector operator - ( float scalar, const i2DVector& vector )
	{ return vector - scalar; }
	
	/// friend operator *
	friend i2DVector operator * ( float scalar, const i2DVector& vector )
	{ return vector * scalar; }


	/// friend operator +
	friend i2DVector operator + ( const i2DVector& vector )
	{ return vector; }

	/// friend operator -
	friend i2DVector operator - ( const i2DVector& vector )
	{ return i2DVector( -vector.x, -vector.y ); }

	// Assignment operator
	inline i2DVector& operator = ( const i2DVector& vector )
	{
		x = vector.x;
		y = vector.y;
		return *this;
	}

	// Assignment operator
	inline i2DVector& operator = ( const iPoint& pos )
	{
		x = (float)pos.x;
		y = (float)pos.y;
		return *this;
	}

	/// operator +=
	inline i2DVector& operator += ( const i2DVector& vector )
	{
		x += vector.x;
		y += vector.y;
		return *this;
	}

	/// operator -=
	inline i2DVector& operator -= ( const i2DVector& vector )
	{
		x -= vector.x;
		y -= vector.y;
		return *this;
	}

	/// operator +
	inline i2DVector operator + ( const i2DVector& vector ) const
	{
		return i2DVector( *this ) += vector;
	}

	/// operator -
	inline i2DVector operator - ( const i2DVector& vector ) const
	{
		return i2DVector( *this ) -= vector;
	}

	/// Resets vector to zero
	inline void Zero()
	{
		x = 0.0f;
		y = 0.0f;
	}

	/// Returns Zero vector
	static inline i2DVector GetZero()
	{ return i2DVector( 0.0f, 0.0f ); }

	/// Returns aproximated vector's magnitude
	float ApproximateMagnitude( float x1, float y1 ) const 
	{ 
		x1 = iABS(x1); 
		y1 = iABS(y1); 
 		if ( x1 < 0.414213f * y1 ) return y1 + 0.483608f * x1 * x1 / y1; 
 		if ( y1 < 0.414213f * x1 ) return x1 + 0.483608f * y1 * y1 / x1; 
 
		// function returns infinite float then zero length used 
		float sum = x1 + y1; 
		if ( sum )  return 1.04907f * sum - 1.36785f * x1 * y1 / sum; 
		else  return 0.0f; 
	} 
 
	/// Returns aproximated vector's magnitude
	float ApproxMagnitude() const 
	{ return ApproximateMagnitude( x, y ); } 
 
	/// Returns aproximated vector's inverted magnitude
	float ApproxInvMagnitude() const 
	{ 
		float mag = ApproximateMagnitude( x, y ); 
 		check( mag != 0.0f ); 
 		return 1.0f / mag; 
	} 

	/// Returns normalized vector's value
	i2DVector GetApproxNormalized() const 
	{ 
		float mag = ApproximateMagnitude( x, y ); 
		check( mag != 0.0f ); 
 		float inv = 1.0f / mag; 
 		return i2DVector( x * inv, y * inv ); 
	} 

};


/// 4D Vector class
class i4DVector
{
public:
	float w,x,y,z;

	/// Default constructor
	i4DVector()
	: w(0.0f)
	, x(0.0f)
	, y(0.0f)
	, z(0.0f)
	{}

	/// Constructs vector with given initialization values
	i4DVector(float nw, float nx, float ny, float nz)
	: w(nw)
	, x(nx)
	, y(ny)
	, z(nz)
	{}

	/// Equality operator
	bool operator == ( const i4DVector& vector ) const
	{ return ( w == vector.w && x == vector.x && y == vector.y && z == vector.z ); }

	/// Inequality operator
	bool operator != ( const i4DVector& vector ) const
	{ return ! operator == ( vector ); }

	/// operator +=
	inline i4DVector& operator += ( float scalar )
	{
		w += scalar;
		x += scalar;
		y += scalar;
		z += scalar;
		return *this;
	}

	/// operator -=
	inline i4DVector& operator -= ( float scalar )
	{
		w -= scalar;
		x -= scalar;
		y -= scalar;
		z -= scalar;
		return *this;
	}

	/// operator *=
	inline i4DVector& operator *= ( float scalar )
	{
		w *= scalar;
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	
	/// operator /=
	inline i4DVector& operator /= ( float scalar )
	{
		check( scalar != 0.0f );
		w /= scalar;
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	/// operator +
	inline i4DVector operator + ( float scalar ) const
	{ return i4DVector( *this ) += scalar; }

	/// operator -
	inline i4DVector operator - ( float scalar ) const
	{ return i4DVector( *this ) -= scalar; }
	
	/// operator *
	inline i4DVector operator * ( float scalar ) const
	{ return i4DVector( *this ) *= scalar; }
	
	/// operator /
	inline i4DVector operator / ( float scalar ) const
	{ return i4DVector( *this ) /= scalar; }

	/// friend operator +
	friend i4DVector operator + ( float scalar, const i4DVector& vector )
	{ return vector + scalar; }
	
	/// friend operator -
	friend i4DVector operator - ( float scalar, const i4DVector& vector )
	{ return vector - scalar; }
	
	/// friend operator *
	friend i4DVector operator * ( float scalar, const i4DVector& vector )
	{ return vector * scalar; }


	/// friend operator +
	friend i4DVector operator + ( const i4DVector& vector )
	{ return vector; }

	/// friend operator -
	friend i4DVector operator - ( const i4DVector& vector )
	{ return i4DVector( -vector.w, -vector.x, -vector.y, -vector.z); }

	/// Assignment operator
	inline i4DVector& operator = ( const i4DVector& vector )
	{
		w = vector.w;
		x = vector.x;
		y = vector.y;
		z = vector.z;
		return *this;
	}

	/// operator +=
	inline i4DVector& operator += ( const i4DVector& vector )
	{
		w += vector.w;
		x += vector.x;
		y += vector.y;
		z += vector.z;
		return *this;
	}

	/// operator -=
	inline i4DVector& operator -= ( const i4DVector& vector )
	{
		w -= vector.w;
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		return *this;
	}

	/// operator +
	inline i4DVector operator + ( const i4DVector& vector ) const
	{ return i4DVector( *this ) += vector; }

	/// operator -
	inline i4DVector operator - ( const i4DVector& vector ) const
	{ return i4DVector( *this ) -= vector; }

	/// Resets vector to zero
	inline void Zero()
	{
		w = 0.0f;
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	/// Returns Zero vector
	static inline i4DVector GetZero()
	{ return i4DVector( 0.0f, 0.0f, 0.0f, 0.0f ); }
};

#endif //_IOLIB_HELIUM_VECTOR_H_
/** @} */ // iolib_he
