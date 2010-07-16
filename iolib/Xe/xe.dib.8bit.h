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
 *	@brief 8bit images handling classes implementation
 */
/** @addtogroup iolib_xe
 *  @{
 */
#ifndef _IOLIB_XENON_8BIT_DIB_HANDLING_H_
#define _IOLIB_XENON_8BIT_DIB_HANDLING_H_

/// @brief The iPalette class encapsulates a standard 8bit color palette
/// The interface allows the application to take full advantage of the color capabilities.
class iPalette
{
public:
	/// Default constructor
	iPalette()
	{}

	/// Constructs palette from 16bit color set
	iPalette( const uint16* pal)
	{
		Init(pal);
	}

	/// Copy constructor
	iPalette( const iPalette& other)
	{ CopyBlock8( (uint8*)entries, (uint8*)other.entries, sizeof(entries) ); }

	/// Assignment operator
	iPalette& operator = ( const iPalette& other)
	{
		CopyBlock8( (uint8*)entries, (uint8*)other.entries, sizeof(entries) );
		return *this;
	}

	/// Initializes palette from 16bit color set
	void Init(const uint16* pal)
	{
		for (uint32 xx=0; xx != 256; ++xx) {
			entries[xx].SetR( uint8( (pal[xx] & (0x1F<<11)) >> 8) );
			entries[xx].SetG( uint8( (pal[xx] & (0x3F<<5)) >> 3) );
			entries[xx].SetB( uint8( (pal[xx] & 0x1F) << 3) );
			entries[xx].SetA( uint8( 255 ) );
		}
	}

	/// Returns specified palette's element
	inline const iColor& GetEntry( uint8 n ) const
	{ return entries[ n ]; }

	/// Assign specified palette element's value
	inline void SetEntry( uint8 idx, const iColor& clr )
	{ entries[ idx ] = clr; }

	/// Initializes palette with gray scale gradient
	void BWGrad()
	{
		for( uint32 i = 0; i < 256; ++i ) {
			entries[i] = iColor((uint8)i,(uint8)i,(uint8)i,255);
		}
	}

private:
	iColor entries[256];
};

/// iPalette based 8bit Dib
class iPaletteDib
{
public:
	/// Default constructor
	iPaletteDib()
	: m_Buff(NULL)
	{
		ReallocDib(iSize(5,5));
	}

	/// Constructs iPaletteDib with specified size
	iPaletteDib(const iSize &siz)
	: m_Buff(NULL)
	{
		ReallocDib(siz);
	}

	/// Destructor
	~iPaletteDib()
	{
		if (m_Buff) delete m_Buff;
	}

	/// Reallocates object's buffer with specified size
	void ReallocDib(const iSize &siz)
	{
		m_Siz = siz;
		if (m_Buff) delete m_Buff;
		m_Buff = new uint8[siz.w*siz.h];
	}

	/// Makes clone of instance
	void MakeClone(iPaletteDib &dst) const
	{
		dst.ReallocDib(m_Siz);
		CopyBlock8(dst.GetBuffer(),GetBuffer(),m_Siz.w*m_Siz.h);
	}

	/// Returns pointer to the first pixel of buffer
	inline uint8* GetBuffer()
	{
		return m_Buff;
	}

	/// Returns constant pointer to the first pixel of the image
	inline const uint8* GetBuffer() const
	{
		return m_Buff;
	}

	/// Returns pointer to the first pixel of specified line
	inline uint8* GetBuffer(uint32 line)
	{
		return m_Buff + (line*m_Siz.w);
	}

	/// Returns constant pointer to the first pixel of specified line
	inline const uint8* GetBuffer(uint32 line) const
	{
		return m_Buff + (line*m_Siz.w);
	}

	/// Returns pointer to the specified pixel
	inline uint8* GetBuffer(const iPoint &pos)
	{
		if (!IsValidPos(pos)) return NULL;
		return m_Buff + (pos.y*m_Siz.w+pos.x);
	}

	/// Returns constant pointer to the specified pixel
	inline const uint8* GetBuffer(const iPoint &pos) const
	{
		if (!IsValidPos(pos)) return NULL;
		return m_Buff + (pos.y*m_Siz.w+pos.x);
	}

	/// Validates specified position
	inline bool IsValidPos(const iPoint &pos) const
	{
		if (pos.x < 0 || pos.y < 0 || (uint32)pos.x >= m_Siz.w || (uint32)pos.y >= m_Siz.h) return false;
		return true;
	}

	/// Copies itself to specified iDib object according to given parameters
	void CopyToDibXY(iDib& dib, const iPoint& pos, const iPalette& pal) const;

	/// Copies rectangle of itself to specified iDib object according to given parameters
	void CopyRectToDibXY(iDib& dib, const iRect& srect, const iPoint& pos, const iPalette& pal) const;

	/// Returns width of allocated image
	inline uint32 GetWidth() const
	{ return m_Siz.w; }

	/// Returns height of allocated image
	inline uint32 GetHeight() const
	{ return m_Siz.h; }

	/// Returns size of allocated image
	inline iSize GetSize() const
	{ return  m_Siz; }

	/// Assigns specified palette
	inline void AssignPalette(const iPalette& npal)
	{ m_Palette = npal; }

	/// Assign specified palette element's value
	inline void SetPaletteEntry(uint8 pidx, const iColor& clr)
	{ m_Palette.SetEntry(pidx,clr); }

	/// Returns specified palette's element
	inline const iColor& GetPaletteEntry(uint8 pidx) const
	{ return m_Palette.GetEntry(pidx); }

private:
	/// Copy constructor inhibit
	iPaletteDib(const iPaletteDib&)
	{}

	/// Assignment operator inhibit
	iPaletteDib& operator = (const iPaletteDib& dib)
	{
		return *this;
	}

	iSize	m_Siz;
	uint8	*m_Buff;	
	iPalette	m_Palette;
};

/*
 *	
 */
bool Save8BitBmp(const iPaletteDib& dib, const  iStringT& fname);
bool Load8BitBmp(iPaletteDib& dib, const  iStringT& fname);
bool Save8BitPalette(const iPalette& pal, const iStringT& fname);
bool Load8BitPalette(iPalette& pal, const iStringT& fname);
bool Save8BitPaletteSource(const iPalette& pal, const iStringT& fname);
bool Save8BitPaletteSource16(const iPalette& pal, const iStringT& fname);
/*
 *	
 */

struct box {
    int r0;			 /* min value, exclusive */
    int r1;			 /* max value, inclusive */
    int g0;  
    int g1;  
    int b0;  
    int b1;
    int vol;
};

enum {
	RED = 0,
	GREEN = 1,
	BLUE = 2,
	BOX	= 33,
	MAXCOLOR = 256,
};

class iHisto
{
public:
	iHisto();
	~iHisto();

	void Init( uint8 reserv = 0 );
	void Scan( const iDib& dib );
	void Calculate( iPalette& pal );

private:

	int	reservedColors;
	int totalPixels;

	struct IntData {
		long	wt[BOX][BOX][BOX];
		long	mr[BOX][BOX][BOX];
		long	mg[BOX][BOX][BOX];
		long	mb[BOX][BOX][BOX];

		float	m2[BOX][BOX][BOX];

		int		ImageSize;           /* image size */
		int		PalSize;             /* color look-up table size */

		unsigned short  *Qadd;          // *must* be unsigned?
		unsigned char 	*TrueColorPic;
	};

	IntData* data;
	
	void Hist3d( const iDib& dib, long *vwt, long *vmr, long *vmg, long *vmb, float *m_2);
	void Momt3d(long *vwt, long *vmr, long *vmg, long *vmb, float *m_2);
	long Vol(struct box * cube, long mmt[BOX][BOX][BOX]);
	long Bottom(struct box * cube, unsigned char dir, long mmt[BOX][BOX][BOX]);
	long Top(struct box * cube, unsigned char dir, int pos, long mmt[BOX][BOX][BOX]);
	float Var(struct box * cube);
	float Maximize(struct box* cube, unsigned char dir, int first, int last, int* cut, long whole_r, long whole_g, long whole_b, long whole_w);
	void Mark(struct box * cube, int label, unsigned char *tag);
	int Cut(struct box * set1, struct box * set2);
};

/*
 *	
 */
class iRemapper 
{
public:
	iRemapper()
	{}
	
	void Init( iPalette& remapTo )
	{ map = remapTo; }

	void Process( const iDib& src, iPaletteDib& dst);
	void Process( const iDib& src, iDib& dst);

private:
	unsigned int FindBestColor( const iColor &clr );

	iPalette map;
};

#endif //_IOLIB_XENON_8BIT_DIB_HANDLING_H_
/** @} */ // iolib_xe
