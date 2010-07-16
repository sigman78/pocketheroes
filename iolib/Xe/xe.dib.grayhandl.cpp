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

#include "xe.std.h"


void iDibGrayDibHandl::DibChannel2GrayDib(const iDib &src, iGrayDib &dst, const ColorChannel ch)
{
	DibChannelRect2GrayDib(src,iRect(0,0,src.GetWidth(),src.GetHeight()),dst,ch);
}

void iDibGrayDibHandl::DibChannelRect2GrayDib(const iDib &src, const iRect &src_rect, iGrayDib &dst, const ColorChannel ch)
{
	dst.ReallocDib(src_rect.size());
	uint8 *dst_buff = dst.GetBuffer();

	for (uint32 yy=0; yy < src_rect.h; ++yy)
	{
		const uint8 *src_buff = reinterpret_cast< const uint8* >(src.GetLine(yy + src_rect.y) + src_rect.x) + ch;
		for (uint32 xx=0; xx < src_rect.w; ++xx)
		{
			*dst_buff++ = *src_buff;
			src_buff+=sizeof(iColor);
		}

	}
}

void iDibGrayDibHandl::GrayDib2DibChannel(const iGrayDib &src, iDib &dst, const ColorChannel ch)
{
	check(src.GetSize() == dst.GetSize() );

	const uint8* src_buff = src.GetBuffer();
	uint8* dst_buff = reinterpret_cast< uint8* > (dst.GetBits()) + ch;

	for (uint32 xx=0; xx < src.GetWidth()*src.GetHeight(); ++xx)
	{
		*dst_buff = *src_buff++;
		dst_buff += sizeof(iColor);
	}
}

void iDibGrayDibHandl::GrayDibRect2DibChannel(const iGrayDib &src, const iRect &srect, iDib &dst, const iPoint &pos, const ColorChannel ch)
{
	iRect srcRect(srect);
	iRect dstRect(dst.GetSize());

	iClipper::iClipRectRect(dstRect,iRect(pos,srect.size()),srcRect,iRect(src.GetSize()));

	for (uint32 yy=0; yy<srcRect.h; ++yy)
	{
		const uint8 *src_buff = src.GetBuffer(iPoint(srcRect.x,srcRect.y+yy));
		uint8 *dst_buff = reinterpret_cast< uint8* >(dst.GetLine(yy + dstRect.y) + dstRect.x) + ch;

		for (uint32 xx=0; xx<srcRect.w; ++xx)
		{
			*dst_buff = *src_buff++;
			dst_buff+=sizeof(iColor);
		}
	}
}

void iDibGrayDibHandl::ComposeGrayDib2Dib(const iGrayDib &src, iDib &dst, const iPoint &pos, const iColor &clr)
{
	iColor color(clr);

	iRect drect = iRect(pos.x,pos.y,src.GetWidth(),src.GetHeight());

	if ( drect.x >= dst.GetWidth() || drect.y >= dst.GetHeight() ) return;

	if ( drect.x < 0)
	{
		drect.w += drect.x;
		drect.x = 0;
	}

	if ( drect.y < 0)
	{
		drect.h += drect.y;
		drect.y = 0;
	}

	if ( (drect.x + drect.w) > dst.GetWidth() ) drect.w = dst.GetWidth() - drect.x;
	if ( (drect.y + drect.h) > dst.GetHeight() ) drect.h = dst.GetHeight() - drect.y;

	for(uint32 yy=0; yy < drect.h; ++yy)
	{
		const uint8* src_buff = src.GetBuffer(iPoint(drect.x-pos.x, drect.y-pos.y+yy));
		iColor* dst_buff = dst.GetBits() + (drect.y+yy)*dst.GetWidth() + drect.x;
		for (uint32 xx=0; xx < drect.w; ++xx)
		{
			dst_buff->ConstAlphaBlend(clr,src_buff[xx]);
			dst_buff++;
		}
	}
}

void iDibGrayDibHandl::ComposeGrayDibRect2Dib(const iGrayDib &src, iDib &dst, const iRect &src_rect, const iPoint &pos, const iColor &clr)
{
	iColor color(clr);

	iRect srect(src_rect);

	if ( srect.x < 0)
	{
		srect.w += srect.x;
		srect.x = 0;
	}

	if ( srect.y < 0)
	{
		srect.h += srect.y;
		srect.y = 0;
	}

	if ( (srect.x + srect.w) > src.GetWidth() ) srect.w = src.GetWidth() - srect.x;
	if ( (srect.y + srect.h) > src.GetHeight() ) srect.h = src.GetHeight() - srect.y;

	iRect drect = iRect(pos.x,pos.y,srect.w,srect.h);

	if ( drect.x < 0)
	{
		drect.w += drect.x;
		drect.x = 0;
	}
                                                                                                                                                                                                
	if ( drect.y < 0)
	{
		drect.h += drect.y;
		drect.y = 0;
	}

	if ( (drect.x + drect.w) > dst.GetWidth() ) drect.w = dst.GetWidth() - drect.x;
	if ( (drect.y + drect.h) > dst.GetHeight() ) drect.h = dst.GetHeight() - drect.y;

	for(uint32 yy=0; yy < drect.h; ++yy)
	{
		const uint8* src_buff = src.GetBuffer(iPoint(srect.x, srect.y+yy));
		iColor* dst_buff = dst.GetBits() + (drect.y+yy)*dst.GetWidth() + drect.x;
		for (uint32 xx=0; xx < drect.w; ++xx)
		{
			dst_buff->ConstAlphaBlend(clr,src_buff[xx]);
			dst_buff++;
		}
	}
}

