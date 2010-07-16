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

#include "stdafx.h"
#include "lzo.h"

//////////////////////////////////////////////////////////////////////////
static const iCharW CCODE = '#';
static const iStringT dots(_T("..."));

inline uint8 HexChar2Int(iCharW c)
{
	if (c >= '0' && c <= '9') return c - '0';
	else if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	else {
		check(0);
		return 0;
	}
}

bool ProcessChar(const iCharW* &text, SpriteId &sid, iTextComposer::FontConfig& cfg)
{
	if (*text == CCODE) {
		++text;
		uint16 r,g,b,a;
		switch (*text){
			case 'I': 
				++text;
				r = HexChar2Int(*text++);
				g = HexChar2Int(*text++);
				b = HexChar2Int(*text++);
				a = HexChar2Int(*text++);
				sid = (r<<12) | (g<<8) | (b<<4) | a;
				break;
			case 'A':
				++text;
				r = HexChar2Int(*text++);
				g = HexChar2Int(*text++);
				cfg.cmpProps.alpha = r<<4 | g;
				break;
			case 'S':
				++text;
				r = HexChar2Int(*text++);
				check(r>=0 && r <iTextComposer::FS_COUNT);
				cfg.fontSize = (iTextComposer::FontSize)r;
				break;
			case 'F': 
				++text;
				r = HexChar2Int(*text++);
				g = HexChar2Int(*text++);
				b = HexChar2Int(*text++);
				cfg.cmpProps.faceColor = r<<12|g<<7|b<<1;
				break;
			case 'B': 
				++text;
				r = HexChar2Int(*text++);
				g = HexChar2Int(*text++);
				b = HexChar2Int(*text++);
				cfg.cmpProps.borderColor = r<<12|g<<7|b<<1;
				break;
		}
		return false;
	} else if (*text == '\r') {
		text++;
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
iTextComposer::iTextComposer()
: m_fontConfig(FS_MEDIUM, RGB16(255,255,255)){}

bool iTextComposer::Init()
{
	// Init fonts
	iFilePtr pFile = OpenWin32File(gDataPath+_T("fonts.dat"));
	if (!pFile) return false;

	iDynamicBuffer fileBuff(pFile->GetSize());
	fileBuff.IncSize(pFile->GetSize() );
	pFile->Read(fileBuff.GetData(),pFile->GetSize());
	pFile.reset();

	uint32 hdr; fileBuff.Read(hdr);
	uint16 ver; fileBuff.Read(ver);
	if (hdr != GFNT_FILE_HDR_KEY || ver != GFNT_FILE_VERSION) return false;

	
	// Decompress
	iDynamicBuffer dbuff;
	LZO::Decompress((const unsigned char*)fileBuff.GetCurrentData(), fileBuff.GetSize() - fileBuff.GetCurrentPos(), dbuff);
	fileBuff.Clean();

	for (uint32 nn=0; nn<FS_COUNT; ++nn) {
		if (!m_Fonts[nn].Init(dbuff, (nn==2)?2:0)) return false;
	}
	
	return true;
}

void iTextComposer::SetFontSize(FontSize fs)
{
	m_fontConfig.fontSize = fs;
}

iStringT iTextComposer::CropString(const iStringT& text, FontConfig& fc, uint32 width, bool bDot)
{
	uint32 txtWidth = GetTextSize(text, fc).w;
	if (text.Empty() || txtWidth <= width) return text;

	uint32 dotsWidth = GetTextSize(dots, fc).w;
	uint32 strLen = text.Length();
	do {
		--strLen;
		txtWidth -= m_Fonts[fc.fontSize].GetCharWidth(text[strLen]);
	} while (strLen && (txtWidth+dotsWidth) > width);

	if (!strLen) return iStringT();
	return text.Left(strLen) + dots;
}

iSize iTextComposer::GetTextSize(const iStringT& text, FontConfig cfg)
{
	iSize res(2,m_Fonts[cfg.fontSize].GetFontHeight());

	
	const iCharW* ptr = text.CStr();
	const iCharW* lastChar = ptr+text.Length();

	while (ptr != lastChar) {
		SpriteId sid = -1;
		if (ProcessChar(ptr, sid, cfg)) {
			sint32 nw = m_Fonts[cfg.fontSize].GetCharWidth(*ptr);
			res.w += nw;
			++ptr;
		} else if (sid != -1) res.w += gGfxMgr.Dimension(sid).w;
	}

	return res;
}

iSize iTextComposer::GetTextBoxSize(const iStringT& text, uint32 width, FontConfig fc)
{
	if (text.Empty()) return iSize();

	sint32 height = 0;
	const iCharT* pTB = text.CStr();
	const iCharT* pCur = pTB;
	const iCharT* pLB = pTB;
	const iCharT* pEnd = pTB + text.Length();
	const iCharT* pLS = 0;
	SpriteId sid;
	FontConfig ffc = fc;

	while (pEnd > pCur) {
		if (!ProcessChar(pCur, sid, ffc)) continue;
		if (*pCur == '\n') {
			height += 11;
			++pCur;
			pLB = pCur;
			pLS = 0;
		} else {
			if (*pCur == ' ') {
				pLS = pCur;
			}
			iSize ts = GetTextSize(text.Mid(pLB-pTB, pCur-pLB+1), ffc);
			if (ts.w > width) {
				if (pLS) {
					pCur = pLS;
				} else pCur--;
				height += 11;
				++pCur;
				pLB = pCur;
				pLS = 0;
			} else {
				++pCur;
			}
		}
	}

	if (pCur != pLB) {
		height += 11;
	}

	return iSize(width, height);
}

void iTextComposer::TextOut(const FontConfig &fcc, iDib& dib, const iPoint& pos, const iStringT& text, const iRect& rect, Alignment al, const iPoint& offset)
{
	iPoint op(pos);
	FontConfig fc(fcc);

	if (rect != cInvalidRect) {
		iRect orc = AlignRect(GetTextSize(text,fc),rect,al);
		op = orc.point();
	}

	op += offset;

	// move baseline
	sint32 foy = op.y;
	
	const iCharW* ptr = text.CStr();
	const iCharW* lastChar = ptr+text.Length();


	while (ptr != lastChar) {
		SpriteId sid = -1;
		if (ProcessChar(ptr, sid, fc)) {
			// Shadow or border
			if (fc.cmpProps.decor != iDibFont::DecNone) m_Fonts[fc.fontSize].ComposeChar(*ptr,dib,iPoint(op.x,foy),fc.cmpProps, fc.cmpProps.decor);
			// Text face
			sint32 nw = m_Fonts[fc.fontSize].ComposeChar(*ptr,dib,iPoint(op.x,foy),fc.cmpProps, iDibFont::DecNone);
			op.x += nw;
			++ptr;
		} else if (sid != -1) {
			if (fc.cmpProps.alpha == 255) gGfxMgr.Blit( sid, dib, iPoint(op.x+1,op.y+2));
			else gGfxMgr.BlitAlpha( sid, dib, iPoint(op.x+1,op.y+2), fc.cmpProps.alpha >> 2);
			op.x += gGfxMgr.Dimension(sid).w;
		} else {
		}
	}

}

void iTextComposer::TextOut(iDib& dib, const iPoint& pos, const iStringT& text, const iRect& rect, Alignment al, const iPoint& offset)
{
	FontConfig fc(m_fontConfig);
	TextOut(fc,dib,pos,text,rect,al, offset);
}

sint32 iTextComposer::TextBoxOut(const FontConfig &fc, iDib& dib, const iStringT& text, const iRect& orc)
{
	if (text.Empty()) return 0;

	sint32 height = 0;
	iRect rect(orc);
	rect.h = 11;
	const iCharT* pTB = text.CStr();
	const iCharT* pCur = pTB;
	const iCharT* pLB = pTB;
	const iCharT* pEnd = pTB + text.Length();
	const iCharT* pLS = 0;
	SpriteId sid;
	FontConfig ffc = fc;

	while (pEnd > pCur) {
		if (!ProcessChar(pCur, sid, ffc)) continue;
		if (*pCur == '\n') {
			if (pLB != pCur) {
				TextOut(ffc, dib, rect.point(), text.Mid(pLB - pTB, pCur - pLB), rect, AlignCenter);
			}
			height += 11;
			rect.y += 11;
			++pCur;
			pLB = pCur;
			pLS = 0;
		} else {
			if (*pCur == ' ') {
				pLS = pCur;
			}
			iSize ts = GetTextSize(text.Mid(pLB-pTB, pCur-pLB+1), ffc);
			if (ts.w > rect.w) {
				if (pLS) {
					pCur = pLS;
				} else pCur--;
				if (pLB != pCur) {
					TextOut(ffc, dib, rect.point(), text.Mid(pLB - pTB, pCur - pLB + 1), rect, AlignCenter);
				}
				height += 11;
				rect.y += 11;
				++pCur;
				pLB = pCur;
				pLS = 0;
			} else {
				++pCur;
			}
		}
	}

	if (pCur != pLB) {
		TextOut(ffc, dib, rect.point(), text.Mid(pLB - pTB, pCur - pLB), rect, AlignCenter);
		height += 11;
	}

	/*
	sint32 lineBegin=0;
	sint32 linePos = 0;
	sint32 height = 0;
	iRect rect(orc);
	rect.h=10;

	sint32 spos = text.Find(_T(' '),linePos);
	while (1) {
		iStringT tTxt = text.Mid(lineBegin, (spos ==-1)?0:spos-lineBegin);
		iSize sSiz = GetTextSize(tTxt, fc );
		if ( sSiz.w > orc.w && linePos>lineBegin) {
			iStringT oText = text.Mid(lineBegin, linePos-lineBegin-1);
			TextOut(fc,dib,rect.point(),oText,rect,AlignCenter);
			height += 11;
			rect.y += 11;
			lineBegin = linePos;
		}
		if (spos == -1) break;
		linePos = spos+1;
		spos = text.Find(_T(' '),linePos);
	}
	TextOut(fc,dib,rect.point(),text.Mid(lineBegin),rect,AlignCenter);
	height += 11;
	*/

	return height;
}

sint32 iTextComposer::TextBoxOut(iDib& dib, const iStringT& text, const iRect& orc)
{
	FontConfig fc(m_fontConfig);
	return TextBoxOut(fc, dib, text, orc);
}


