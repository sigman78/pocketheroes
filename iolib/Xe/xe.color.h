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
 *	@brief Various color notations classes implementation 
 */
/** @addtogroup iolib_xe
 *  @{
 */

#ifndef _IOLIB_XENON_COLOR_H_
#define _IOLIB_XENON_COLOR_H_

#include <math.h>

/// This macro selects a red, green, blue (RGB) color based on the specified color values and the capabilities of iDIB. 
#define RGBVAL(r,g,b)       (((r) << 16) | ((g) << 8) | (b))
/// This macro selects a red, green, blue, alpha (RGBA) color based on the specified color values and the capabilities of iDib. 
#define RGBAVAL(r,g,b,a)    (((a) << 24) | RGBVAL(r,g,b))
/// This macro retreives red, green, blue values from selected value
#define GETRGBVALS(c,r,g,b) ((b) = (BLUEVAL(c)), (g) = (GREENVAL(c)), (r) = (REDVAL(c)))

/// This macro clips byte's value
#define CLIP_BYTE(x) (x>255)?255:x
/// This macro packs four bytes into LONG
#define BYTES_TO_LONG(b1,b2,b3,b4)  ( ((b1) << 24) | ((b2) << 16) | ((b3) << 8) | (b4) )

/// Color channels enumeration
enum ColorChannel {
	Channel_Alpha	= 3,
	Channel_Red		= 2,
	Channel_Green	= 1,
	Channel_Blue	= 0
};

/// Color channel masks enumeration
enum ChannelMask {
	Mask_Alpha	= 0xFF000000,
	Mask_Red	= 0x00FF0000,
	Mask_Green	= 0x0000FF00,
	Mask_Blue	= 0x000000FF,
	Mask_Color	= 0x00FFFFFF,
	Mask_All	= 0xFFFFFFFF
};

/// Wrapper class for R8G8B8A8 color notation
class iColor
{
public:

	/// Default constructor
	__forceinline iColor ()
    { 
		return;
	}

	/// Constructs iColor object with specified 32 bit color value
    __forceinline iColor (const uint32 C)
    {
        rgba = C;
        return;
    }

	/// Constructs iColor object with specified 24 bits color and 8 bits alpha value
    __forceinline iColor (const uint32 C, uint8 alpha)
    {
        rgba = C;
		a = alpha;
        return;
    }

	/// Constructs iColor object with specified 16 bits color and 8 bits alpha value
    __forceinline iColor (uint16 rgb16, uint8 alpha)
    {
		a = 255;
		r = (uint8)((rgb16 & 0xf800) >> 8);
		g = (uint8)((rgb16 & 0x07e0) >> 3);
		b = (uint8)((rgb16 & 0x001f) << 3);
	}
    
	/// Constructs iColor object with selected red, green and blue components
    __forceinline iColor (uint8 Red, uint8 Green, uint8 Blue)
    { 
        rgba = RGBVAL (Red, Green, Blue);
        return;
	}

	/// Constructs iColor object with selected red, green, blue and alpha components
    __forceinline iColor (uint8 Red, uint8 Green, uint8 Blue, uint8 Alpha)
    { 
        rgba = RGBAVAL (Red, Green, Blue, Alpha);
        return;
	}
	
	/// Returns packet color value
    __forceinline operator uint32 () const
    {
        return (rgba);
    }

	/// returns result of the equality comparison
    __forceinline bool operator == (iColor &rhs)  const
    {
        if (rgba == rhs.rgba)
            return (true);

        return (false);
    }

	/// returns result of the inequality comparison
    __forceinline bool operator!= (iColor &rhs)  const
    {
        if (rgba == rhs.rgba) return (false);
        return (true);
    }

	/// Setup value for red channel
	__forceinline void SetR (uint8 nr)
	{ r=nr; }
	
	/// Setup value for green channel
	__forceinline void SetG (uint8 ng)
	{ g=ng; }

	/// Setup value for blue channel
	__forceinline void SetB (uint8 nb)
	{ b=nb; }

	/// Setup value for alpha channel
	__forceinline void SetA (uint8 na)
	{ a=na; }

	/// Setup value for all channels
	__forceinline void SetRGBA (uint8 nr, uint8 ng, uint8 nb, uint8 na)
	{
		r=nr;
		g=ng;
		b=nb;
		a=na;
	}

	/// Obtain red channel's value
    __forceinline uint8 GetR () const
	{ return r; }

	/// Obtain green channel's value
    __forceinline uint8 GetG () const
	{ return g;	}

	/// Obtain blue channel's value
    __forceinline uint8 GetB () const
	{ return b; }

	/// Obtain alpha channel's value
    __forceinline uint8 GetA () const
	{ return a; }

	/// Obtain specified channel's value
    __forceinline uint8 GetChannel (ColorChannel ch) const
	{ return (uint8)(rgba >> (ch*8)); }

	/// Combine with specified color according to blend level
	__forceinline iColor CombineWithColor(const iColor &cw, uint8 lvl)
	{
		uint16 sr = (uint16)r*(255-lvl) + (uint16)cw.r*lvl;
		uint16 sg = (uint16)g*(255-lvl) + (uint16)cw.g*lvl;
		uint16 sb = (uint16)b*(255-lvl) + (uint16)cw.b*lvl;
		uint16 sa = (uint16)a*(255-lvl) + (uint16)cw.a*lvl;
		return iColor(sr/256,sg/256,sb/256,sa/256);
	}

	/// Alpha blend with specified color
    __forceinline void AlphaBlend (const iColor& clr)
	{
		if (clr.a == 255) {
			rgba = clr.rgba;
		} else if (clr.a != 0) {
			sint32 mixA = clr.a;
			sint32 mixIA= 255 - clr.a;
			sint32 mmix = (a*mixIA)/256;
			r=sint32((clr.r*mixA) + sint32((r*mmix)))/256;
			g=sint32((clr.g*mixA) + sint32((g*mmix)))/256;
			b=sint32((clr.b*mixA) + sint32((b*mmix)))/256;
			a=sint32((255*mixA) + sint32(a*mixIA))/256;
		}
	}

	/// Alpha blend with specified color (background alpha always stay equal to 255 i.e. opaque)
    __forceinline void ConstAlphaBlend (const iColor& clr, uint8 alpha )
	{
		if (alpha == 255) {
			r = clr.r;
			g = clr.g;
			b = clr.b;
			a = 255;
		} else if (alpha != 0) {
			sint32 mixA = alpha;
			sint32 mixIA= 255 - alpha;
			r=sint32((clr.r*mixA) + sint32((r*mixIA)))/256;
			g=sint32((clr.g*mixA) + sint32((g*mixIA)))/256;
			b=sint32((clr.b*mixA) + sint32((b*mixIA)))/256;
			a=255;
		}

	}

	/// Obtain inverted value of color
	inline iColor GetInverted() const
	{
		return iColor(255 - r, 255 - g, 255 - b, a);
	}

	union {
		uint32 rgba;
		struct 	{
			uint8 b;
			uint8 g;
			uint8 r;
			uint8 a;
		};
	};

	/// returns result of the equality comparison
	bool operator== (const iColor &clr) const
	{ return ((r==clr.r) && (g==clr.g) && (b==clr.b)); }

	/// returns result of the inequality comparison
	bool operator!= (const iColor &clr) const
	{ return ! operator == (clr); }

	/// COLORREF value assignment
	inline void Assign(COLORREF clr)
	{
		r = GetRValue(clr);
		g = GetGValue(clr);
		b = GetBValue(clr);
	}

	/// Returns color's value in COLORREF
	operator COLORREF() const
	{ return RGB(r,g,b); }

	/// Is this color opaque or not (i.e. a==255)
	__forceinline bool IsSolid() const
	{ return (a==255); }

	/// Is this color absolutelly transparent or not (i.e. a==0)
	__forceinline bool IsTransparent() const
	{ return (a==0); }
};

// Stock ColorZ


/// Predefined transparent color
extern const iColor cColorTransparent;

/// Predefined opaque black color
extern const iColor cColorBlack;
/// Predefined opaque white color
extern const iColor cColorWhite;
/// Predefined opaque dark red color
extern const iColor cColorRed64;
/// Predefined opaque dark green color
extern const iColor cColorGreen64;
/// Predefined opaque dark blue color
extern const iColor cColorBlue64;
/// Predefined opaque red color
extern const iColor cColorRed128;
/// Predefined opaque green color
extern const iColor cColorGreen128;
/// Predefined opaque blue color
extern const iColor cColorBlue128;
/// Predefined opaque light red color
extern const iColor cColorRed255;
/// Predefined opaque light green color
extern const iColor cColorGreen255;
/// Predefined opaque light blue color
extern const iColor cColorBlue255;

/// Wrapper class for HSV (hue-saturation-value) color notation
class iHSVColor
{
public:
	/// hue - the "color". dominant frequency
	float h;
	/// saturation - purity of color
	float s;
	/// value - brightness or intensity
	float v;

	/// Default constructor
	inline iHSVColor()
	{ }
	
	/// Constructs iHSVColor object with specified h,s,v values
	inline iHSVColor( float hue, float sat, float val )
	{
		h = hue;
		s = sat;
		v = val;
	}

	/// Copy constructor
	inline iHSVColor( const iHSVColor& clr )
	{
		h = clr.h;
		s = clr.s;
		v = clr.v;
	}

	/// Constructs iHSVColor object from specified iColor object
	inline iHSVColor( const iColor& clr )
	{ Convert( clr ); }

	/// Returns packed HSV value
	inline uint32 GetIntValue()
	{
		check((h <= 1.0f && h >= 0.0f) || h==-1.0f);
		check(s <= 1.0f && s >= 0.0f);
		check(v <= 1.0f && v >= 0.0f);
		return ((iCLAMP<sint32>(0,255,(sint32)((h<0.0f)?0.0f:h*255.0f))) | (iCLAMP<sint32>(0,255,(sint32)(s*255.0f))<<8) | (iCLAMP<sint32>(0,255,(sint32)(v*255.0f))<<16));
	}

	/// Set packet HSV value
	inline void SetIntValue(uint32 ival)
	{
		h = (float)((uint8)(ival)) / 255.0f;
		s = (float)((uint8)(ival>>8)) / 255.0f;
		v = (float)((uint8)(ival>>16)) / 255.0f;
	}

	/// Converts iColor into iHSVColor (RGB->HSV)
	iHSVColor& Convert( const iColor& rgb )
	{
		float min, max, delta;
		float r = rgb.r / 255.0f;
		float g = rgb.g / 255.0f;
		float b = rgb.b / 255.0f;

		max = iMAX( r, iMAX( g, b ) );
		min = iMIN( r, iMIN( g, b ) );

		delta = max - min;
		v = max;

		if ( max !=  0.0f )	s = delta / max;
		else s = 0.0f;

		if ( s == 0.0f ){
			h = -1.0f;
		} else {
			if ( r == max )	h = (g - b) / delta;
			else if ( g == max ) h = 2.0f + ( b - r ) / delta;
			else if ( b == max ) h = 4.0f + ( r - g ) / delta;

			h = h / 6.0f;
			h = iCLAMP(0.0f,1.0f,h);
		}
		return *this;
	}

	/// returns color value in an RGB notation
	iColor GetColor() const
	{
		float f, x, y, z, h_;
		int i;
		float r, g, b;
		if ( s == 0.0 ) {
			r = g = b = s;
		} else {
			h_ = h;
			while ( h_ < 0.0 ) h_ += 1.0f;
			h_ = 6.0f * (float)fmodf( h_, 1.0f );
			i = (int)h_;
			f = h_ - i;
			x = v * ( 1.0f - s );
			y = v * ( 1.0f - ( s * f ));
			z = v * ( 1.0f - ( s * ( 1.0f - f )));
			switch ( i ) {
			case 0 :
				r = v; g = z; b = x; 
				break;
			case 1 :
				r = y; g = v; b = x; 
				break;
			case 2 :
				r = x; g = v; b = z; 
				break;
			case 3 :
				r = x; g = y; b = v; 
				break;
			case 4 :
				r = z; g = x; b = v; 
				break;
			case 5 :
				r = v; g = x; b = y; 
				break;
			}
		}
		
		return iColor( (uint8)(r * 255.0f), (uint8)(g * 255.0f), (uint8)(b * 255.0f) );
	}
};

/// Converts COLORREF value into iColor object
inline iColor COLORREF2ICOLOR(COLORREF cref)
{
	return iColor(GetRValue(cref),GetGValue(cref),GetBValue(cref),255);
}


#endif //_IOLIB_XENON_COLOR_H_
/** @} */ // iolib_xe
