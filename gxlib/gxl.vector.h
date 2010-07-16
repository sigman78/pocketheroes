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

#ifndef _GXLIB_VECTOR_H_
#define _GXLIB_VECTOR_H_

//
// iVector2D
//
class i2DVector
{
public:
	fix32 x,y;

	i2DVector()
	: x(fix32::zero)
	, y(fix32::zero)
	{}

	i2DVector(sint32 nx, sint32 ny)
	: x(nx)
	, y(ny)
	{}

	i2DVector(fix32 nx, fix32 ny)
	: x(nx)
	, y(ny)
	{}

	bool operator == ( const i2DVector& vector ) const
	{
		return ( x == vector.x && y == vector.y );
	}

	bool operator != ( const i2DVector& vector ) const
	{
		return ! operator == ( vector );
	}


	inline i2DVector& operator += ( fix32 scalar )
	{
		x += scalar;
		y += scalar;
		return *this;
	}

	inline i2DVector& operator -= ( fix32 scalar )
	{
		x -= scalar;
		y -= scalar;
		return *this;
	}

	inline i2DVector& operator *= ( fix32 scalar )
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}
	
	inline i2DVector& operator /= ( fix32 scalar )
	{
		check( scalar != fix32::zero );
		x /= scalar;
		y /= scalar;
		return *this;
	}

	inline i2DVector operator + ( fix32 scalar ) const
	{
		return i2DVector( *this ) += scalar;
	}

	inline i2DVector operator - ( fix32 scalar ) const
	{
		return i2DVector( *this ) -= scalar;
	}
	
	inline i2DVector operator * ( fix32 scalar ) const
	{
		return i2DVector( *this ) *= scalar;
	}
	
	inline i2DVector operator / ( fix32 scalar ) const
	{
		return i2DVector( *this ) /= scalar;
	}

	friend i2DVector operator + ( fix32 scalar, const i2DVector& vector )
	{
		return vector + scalar;
	}
	
	friend i2DVector operator - ( fix32 scalar, const i2DVector& vector )
	{
		return vector - scalar;
	}
	
	friend i2DVector operator * ( fix32 scalar, const i2DVector& vector )
	{
		return vector * scalar;
	}


	// Unary
	friend i2DVector operator + ( const i2DVector& vector )
	{
		return vector;
	}

	friend i2DVector operator - ( const i2DVector& vector )
	{
		return i2DVector( -vector.x, -vector.y );
	}

	// Infix
	inline i2DVector& operator = ( const i2DVector& vector )
	{
		x = vector.x;
		y = vector.y;
		return *this;
	}

	inline i2DVector& operator += ( const i2DVector& vector )
	{
		x += vector.x;
		y += vector.y;
		return *this;
	}

	inline i2DVector& operator -= ( const i2DVector& vector )
	{
		x -= vector.x;
		y -= vector.y;
		return *this;
	}

	inline i2DVector operator + ( const i2DVector& vector ) const
	{
		return i2DVector( *this ) += vector;
	}

	inline i2DVector operator - ( const i2DVector& vector ) const
	{
		return i2DVector( *this ) -= vector;
	}

	// Reset
	inline void Zero()
	{
		x = fix32::zero;
		y = fix32::zero;
	}

	static inline i2DVector GetZero()
	{
		return i2DVector( fix32::zero, fix32::zero );
	}

	/*
	float Norma() const
	{
		return (float)sqrt(pow(x,2.0f)+pow(y,2.0f));
	}*/
};

#endif //_GXLIB_VECTOR_H_

