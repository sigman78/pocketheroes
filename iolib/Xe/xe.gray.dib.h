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
 *	@brief Gray scale dib class implementation
 */
/** @addtogroup iolib_xe
 *  @{
 */
#ifndef _IOLIB_XENON_GRAY_DIB_H_
#define _IOLIB_XENON_GRAY_DIB_H_

/// 8bit gray scale image
class iGrayDib
{
public:
	/// Default constructor
	iGrayDib()
	: m_Buff(NULL)
	{
		ReallocDib(iSize(5,5));
	}

	/// Constructs iGrayDib object with specified size
	iGrayDib(const iSize &siz)
	: m_Buff(NULL)
	{
		ReallocDib(siz);
	}

	/// Destructor
	~iGrayDib()
	{
		if (m_Buff) delete m_Buff;
	}

	/// Reallocates iGaryDib image metrics with new size
	void ReallocDib(const iSize &siz)
	{
		m_Size = siz;
		if (m_Buff) delete m_Buff;
		m_Buff = new uint8[siz.w*siz.h];
	}

	/// Makes clone of itself
	void MakeClone(iGrayDib &dst) const
	{
		dst.ReallocDib(m_Size);
		CopyBlock8(dst.GetBuffer(),GetBuffer(),m_Size.w*m_Size.h);
	}

	/// Returns pointer to the first pixel of the image
	uint8* GetBuffer()
	{
		return m_Buff;
	}

	/// Returns constant pointer to the first pixel of the image
	const uint8* GetBuffer() const
	{
		return m_Buff;
	}

	/// Returns pointer to the first pixel of specified line
	uint8* GetBuffer(uint32 line)
	{
		return m_Buff + (line*m_Size.w);
	}

	/// Returns constant pointer to the first pixel of specified line
	const uint8* GetBuffer(uint32 line) const
	{
		return m_Buff + (line*m_Size.w);
	}

	/// Returns pointer to the specified pixel
	uint8* GetBuffer(const iPoint &pos)
	{
		if (!IsValidPos(pos)) return NULL;
		return m_Buff + (pos.y*m_Size.w+pos.x);
	}

	/// Returns constant pointer to the specified pixel
	const uint8* GetBuffer(const iPoint &pos) const
	{
		if (!IsValidPos(pos)) return NULL;
		return m_Buff + (pos.y*m_Size.w+pos.x);
	}

	/// Validates position
	bool IsValidPos(const iPoint &pos) const
	{
		if (pos.x < 0 || pos.y < 0 || (uint32)pos.x >= m_Size.w || (uint32)pos.y >= m_Size.h) return false;
		return true;
	}

	/// Returns width of allocated image
	uint32 GetWidth() const
	{
		return m_Size.w;
	}

	/// Returns height of allocated image
	uint32 GetHeight() const
	{
		return m_Size.h;
	}

	/// Returns size of allocated image
	iSize GetSize() const
	{
		return  m_Size;
	}

	/// Fills image buffer with specified value
	void FillBuffer(uint8 v)
	{
		FillBlock8(m_Buff,v, m_Size.w*m_Size.h);
	}

	/// Fills rectangle area of the image buffer with specified value
	void FillRect(const iRect &rect, uint8 v)
	{
		iRect dst_rect(rect);
		if (!iClipper::ClipRect(dst_rect,GetSize())) return;

		for (uint32 yy=0; yy<dst_rect.h; ++yy) {
			uint8* dst_buff = GetBuffer(iPoint(dst_rect.x,dst_rect.y+yy));
			FillBlock8(dst_buff,v,dst_rect.w);
		}

	}

	/// Setup brightness level for whole image
	void SetGlobalLevel(uint8 v)
	{
		for (uint32 xx=0; xx<m_Size.w*m_Size.h; ++xx)
		{
			m_Buff[xx] = (uint8) ((uint32)m_Buff[xx] * v / 255);
		}
	}

	/// Blends specified iGrayDib image to itself
	void BlendGrayDib(const iGrayDib &src, const iPoint &pos)
	{
		iRect drect = iRect(pos.x,pos.y,src.GetWidth(),src.GetHeight());

		if ( drect.x < 0){
			drect.w += drect.x;
			drect.x = 0;
		}

		if ( drect.y < 0){
			drect.h += drect.y;
			drect.y = 0;
		}

		if ( (drect.x + drect.w) > GetWidth() ) drect.w = GetWidth() - drect.x;
		if ( (drect.y + drect.h) > GetHeight() ) drect.h = GetHeight() - drect.y;

		iRect srect(drect.x-pos.x, drect.y-pos.y,drect.w,drect.h);

		for (uint32 yy = 0; yy < drect.h; ++yy) {
			uint8* dstptr = GetBuffer( iPoint(drect.x, drect.y + yy) );
			const uint8* srcptr = src.GetBuffer( iPoint( srect.x, srect.y + yy) );
			for( uint32 n = 0; n < drect.w; n++ ) {
				*dstptr++ = iMAX( *dstptr, *srcptr++ );
			}
		}
	}

	/// Blends line using itself as brush pattern
	void DrawBrushLine(const iGrayDib &brush, const iPoint &pos1, const iPoint &pos2)
	{
		iPoint p( pos1 );
		iPoint d( iABS( pos2.x - pos1.x ), iABS( pos2.y - pos1.y ) );
		iPoint s( iSIGN( pos2.x - pos1.x ), iSIGN( pos2.y - pos1.y ) );

		sint32 d_, d1, d2;

		uint32 brw = brush.GetWidth()/2;
		uint32 brh = brush.GetHeight()/2;

		if ( d.y <= d.x ) {
			d_	= d.y * 2 - d.x;
			d1	= d.y * 2;
			d2	= ( d.y - d.x ) * 2;

			BlendGrayDib(brush,iPoint(p.x-brw,p.y-brh));
			//ComposeGrayDib2Dib(src,dst,iPoint(p.x-brw,p.y-brh),clr);

			p = iPoint( pos1.x + s.x, pos1.y );
			for( sint32 i = 0; i < d.x; i++ ) {
				if ( d_ > 0 ) 
				{
					d_	+= d2;
					p.y	+= s.y;
				} else {
					d_	+= d1;
				}

				BlendGrayDib(brush,iPoint(p.x-brw,p.y-brh));
				//ComposeGrayDib2Dib(src,dst,iPoint(p.x-brw,p.y-brh),clr);
				p.x += s.x;

			}
		} else {
			d_	= d.x * 2 - d.y;
			d1	= d.x * 2;
			d2	= ( d.x - d.y ) * 2;

			BlendGrayDib(brush,iPoint(p.x-brw,p.y-brh));
			//ComposeGrayDib2Dib(src,dst,iPoint(p.x-brw,p.y-brh),clr);

			p	= iPoint( pos1.x, pos1.y + s.y );

			for( sint32 i = 0; i < d.y; i++ ) {
				if ( d_ > 0 ) {
					d_	+= d2;
					p.x	+= s.x;
				} else {
					d_	+= d1;
				}

				BlendGrayDib(brush,iPoint(p.x-brw,p.y-brh));
				//ComposeGrayDib2Dib(src,dst,iPoint(p.x-brw,p.y-brh),clr);

				p.y += s.y;
			}
		}

	}

private:

	iGrayDib(const iGrayDib&)
	{}

	iGrayDib& operator = (const iGrayDib& dib)
	{ return *this; }

	iSize	m_Size;
	uint8	*m_Buff;
};

#endif //_IOLIB_XENON_GRAY_DIB_H_
/** @} */ // iolib_xe
