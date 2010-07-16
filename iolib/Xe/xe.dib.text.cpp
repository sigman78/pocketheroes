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

iDibText::iDibText()
{
	m_TextColor = cColorBlack;
	m_BackColor = cColorTransparent;
}

void iDibText::SetTextColor(const iColor &clr)
{
	m_TextColor = clr;
}

void iDibText::SetBackColor(const iColor &clr)
{
	m_BackColor = clr;
}

void iDibText::SetTextAlignment(iAlignment algn)
{
	m_Alignment = algn;
}

void iDibText::SetFont(iFont& fnt)
{
	m_Font.Attach(fnt.Detach());
}

void iDibText::BlitText_TransparentBackground(iDib &dib)
{
	// Create gray dib and retreive text alpha
	iGrayDib gdib(dib.GetSize());
	iDibGrayDibHandl::DibChannel2GrayDib(dib,gdib,Channel_Red);

	dib.Fill(m_TextColor,BLEND_SRCCOPY);

	iDibGrayDibHandl::GrayDib2DibChannel(gdib,dib,Channel_Alpha);
}

void iDibText::BlitText_SolidBackground(iDib &dib)
{
	// Create gray dib and retreive text alpha
	iGrayDib gdib(dib.GetSize());
	iDibGrayDibHandl::DibChannel2GrayDib(dib,gdib,Channel_Red);

	dib.Fill(m_BackColor,BLEND_SRCCOPY);

	const uint8* a_buff = gdib.GetBuffer();
	iColor *dst_buff = dib.GetBits();

	// Using Transparent Alpha / Solid Alpha / Normal Alpha

	for (uint32 zz=0; zz < gdib.GetWidth()*gdib.GetHeight(); ++zz)
	{
		if ( *a_buff == 255 ) *dst_buff = m_TextColor;
		else if ( *a_buff != 0 )
		{
			iColor oclr(m_BackColor);
			*dst_buff = oclr.CombineWithColor(m_TextColor,*a_buff);
		}
		++a_buff;
		++dst_buff;
	}
}

void iDibText::BlitText_NormalBackground(iDib &dib)
{
	// Create gray dib and retreive text alpha
	iGrayDib gdib(dib.GetSize());
	iDibGrayDibHandl::DibChannel2GrayDib(dib,gdib,Channel_Red);

	const uint8* a_buff = gdib.GetBuffer();
	iColor *dst_buff = dib.GetBits();

	// Using Transparent Alpha / Solid Alpha / Normal Alpha

	for (uint32 zz=0; zz < gdib.GetWidth()*gdib.GetHeight(); ++zz)
	{
		if ( *a_buff == 255 ) *dst_buff = m_TextColor;
		else if ( *a_buff == 0 ) *dst_buff = m_BackColor;
		else
		{
			iColor oclr(m_BackColor);
			*dst_buff = oclr.CombineWithColor(m_TextColor,*a_buff);
		}

		++a_buff;
		++dst_buff;
	}
}

iSize iDibText::GetTextSize(const iStringT &text)
{
	if (text.Empty()) return iSize(0,0);

	// Create DC and setup its props
	iDC dc;
	dc.CreateCompatibleDC();

	// create temp color dib and output text
	HFONT old_fnt = dc.SelectFont(m_Font);
	iSize tsiz = dc.GetTextSize(text);
	dc.SelectFont(old_fnt);
	return tsiz;
}

void iDibText::TextOut(iDib &dib, const iStringT &text, const iRect &rect, const iPoint &offset)
{
	if (text.Empty()) return;
	// Create DC and setup its props
	iDC dc;
	dc.CreateCompatibleDC();
	dc.SetTextColor(cColorWhite);
	dc.SetBkColor(cColorBlack);
	dc.SetBkMode(OPAQUE);

	// Calculate output rect
	iRect out_rect(rect);
	if (out_rect==cInvalidRect) out_rect = dib.GetSize();

	// create temp color dib and output text
	HFONT old_fnt = dc.SelectFont(m_Font);
	iSize tsiz = dc.GetTextSize(text);
	if (tsiz.w<1 || tsiz.h<1)
	{
		dc.SelectFont(old_fnt);
		return;
	}
	iDib tdib(tsiz);

	HBITMAP old_bmp = dc.SelectBitmap(tdib.GetBitmap());

	dc.TextOutRect(iPoint(0,0),tdib.GetSize(),text);

	dc.SelectBitmap(old_bmp);
	dc.SelectFont(old_fnt);

	if (m_BackColor.IsSolid()) BlitText_SolidBackground(tdib);
	else if (m_BackColor.IsTransparent()) BlitText_TransparentBackground(tdib);
	else BlitText_NormalBackground(tdib);

	iPoint out_pos(0,0);

	if (m_Alignment == Align_LeftTop) out_pos=iPoint(0,0);
	else if (m_Alignment == Align_Top) out_pos=iPoint(out_rect.w/2 - tsiz.w/2,0);
	else if (m_Alignment == Align_RightTop) out_pos=iPoint(out_rect.w - tsiz.w,0);
	else if (m_Alignment == Align_Right) out_pos=iPoint(out_rect.w - tsiz.w,out_rect.h/2 - tsiz.h/2);
	else if (m_Alignment == Align_RightBottom) out_pos=iPoint(out_rect.w - tsiz.w,out_rect.h - tsiz.h);
	else if (m_Alignment == Align_Bottom) out_pos=iPoint(out_rect.w/2 - tsiz.w/2,out_rect.h - tsiz.h);
	else if (m_Alignment == Align_LeftBottom) out_pos=iPoint(0,out_rect.h - tsiz.h);
	else if (m_Alignment == Align_Left) out_pos=iPoint(0,out_rect.h/2 - tsiz.h/2);
	else if (m_Alignment == Align_Center) out_pos=iPoint(out_rect.w/2 - tsiz.w/2,out_rect.h/2 - tsiz.h/2);

	if (offset != cInvalidPoint) out_pos += offset;

	tdib.CopyRectToDibXY(&dib,iRect(0,0,out_rect.w,out_rect.h),out_pos,BLEND_ALPHABLEND);

	dc.SelectFont(old_fnt);
	dc.SelectBitmap(old_bmp);
}
