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

#include "gxl.inc.h"
#include "gxl.memory.h"
#include "gxl.file.h"
#include "gxl.dib.h"
#include "gxl.dibfont.h"

//
iDibFont::iDibFont()
{}

iDibFont::~iDibFont()
{}

bool iDibFont::Init(iDynamicBuffer& buff, sint32 baseLine)
{
	m_baseLine = baseLine;
	memset(m_idxList, 0xFFFF, sizeof(m_idxList));
	uint16 cidx = 0;
	uint32 cc;
	for (cc=0x20; cc<=0x7E; ++cc) m_idxList[cc] = cidx++;
	for (cc=0xC0; cc<=0x17E; ++cc) m_idxList[cc] = cidx++;
	for (cc=0x401; cc<=0x45F; ++cc) m_idxList[cc] = cidx++;

	uint32 el_cnt; buff.Read(el_cnt);
	m_elDesc.Allocate(el_cnt);
	uint8 fh; buff.Read(fh); m_fntHeight = fh;

	for (uint32 xx=0; xx<el_cnt; ++xx) {
		buff.Read(m_elDesc[xx].posX);
		buff.Read(m_elDesc[xx].width);
		buff.Read(m_elDesc[xx].reserved);
	}

	uint16 rawBufSiz; buff.Read(rawBufSiz);
	m_rawFont.Allocate(rawBufSiz);
	buff.Read(m_rawFont.GetPtr(), rawBufSiz);

	return true;
}

sint32 iDibFont::ComposeChar(iCharW c, iDib& surf, const iPoint& pos, const ComposeProps& cmpProps, Decor layer) const
{
	if (c == 0x20) return m_elDesc[m_idxList[c]].width + 1;
	uint32 stride = m_rawFont.GetSize()/m_fntHeight;
	uint32 bitval = 8 - (m_elDesc[m_idxList[c]].posX & 7);
	const uint8* ptr = m_rawFont.GetPtr() + (m_elDesc[m_idxList[c]].posX >> 3);

	for (uint32 yy=0; yy<m_fntHeight; ++yy){
		iDib::pixel lineClr =  (cmpProps.fillType == Gradient)?cmpProps.gradient.GradValue(yy):cmpProps.faceColor;
		const uint8* tptr = ptr;
		uint32 tbitval = bitval;
		for (uint32 xx=0; xx<=m_elDesc[m_idxList[c]].width; ++xx) {
			if (!tbitval) {
				tptr++;
				tbitval = 8;
			} 
			if ( (*tptr) & (1<<(8-tbitval)) ) {
				if (layer == DecNone) {
					sint32 ox = pos.x + xx;
					sint32 oy = pos.y + yy;
					if (surf.IsValidPos(ox, oy)) {
						iDib::pixel pix = (cmpProps.alpha == 255)?lineClr:iDib::BlendPixels(surf.GetPixel(ox, oy), lineClr,cmpProps.alpha);
						surf.PutPixel(ox, oy, pix);
					}
				} else if (layer == DecShadow) {
					sint32 ox = pos.x + xx + 1;
					sint32 oy = pos.y + yy + 1;
					if (surf.IsValidPos(ox, oy)) {
						iDib::pixel pix = (cmpProps.alpha == 255)?cmpProps.borderColor:iDib::BlendPixels(surf.GetPixel(ox, oy), cmpProps.borderColor,cmpProps.alpha);
						surf.PutPixel(ox, oy, pix);
					}
				} else if (layer == DecBorder) {
					surf.FillRect(iRect(pos.x + xx - 1, pos.y + yy-1,3,3), cmpProps.borderColor, cmpProps.alpha);
				}
			}
			tbitval--;
		}
		ptr += stride;
	}

	return m_elDesc[m_idxList[c]].width + 1;
}

void iDibFont::ComposeText(iDib& dib, const iPoint& pos, const ComposeProps& cmpProps, const iStringT& text, const iRect& orc, Alignment al, const iPoint& offset)
{
	iPoint op(pos);
	//FontConfig fc(fcc);
	
	if (orc != cInvalidRect) {
		//iRect orc = AlignRect(GetTextSize(text,fc),rect,al);
		//op = orc.point();
	}
	
	op += offset;
	
	// move baseline
	sint32 foy = op.y;
	
	const iCharW* ptr = text.CStr();
	const iCharW* lastChar = ptr+text.Length();
	
	while (ptr != lastChar) {
		// Shadow or border
		// if (fc.cmpProps.decor != iDibFont::DecNone) m_Fonts[fc.fontSize].ComposeChar(*ptr,dib,iPoint(op.x,foy),fc.cmpProps, fc.cmpProps.decor);
		// Text face
		op.x += ComposeChar(*ptr,dib,iPoint(op.x,foy),cmpProps, iDibFont::DecNone);
		++ptr;
	}
}