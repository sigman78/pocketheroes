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

#include "common.h"


void ShowHeap()
{
	printf("\n");
	printf("-----------------------------------------------\n");
	printf("Bitfont generator 0.1.a.2\n");
	printf("by Tutankhamen/iO 2004                    \n");
	printf("-----------------------------------------------\n");
}

void ShowUsage()
{
	printf("Usage: b2c.exe <src> <dst> <aname> <vpl>\n");
	printf("-----------------------------------------------\n");
}


void ComposeCharacter(iMemoryDC& memDC, TCHAR ich, sint32 px, uint32 w)
{
	iDibPrimitives::DrawHLine(memDC.m_Dib, iPoint(px,0), px+w, clrWidth, BLEND_SRCCOPY);
	memDC.m_Dib.PutPixel(iPoint(px,0), clrAnchor);
	
	/*
	// Frame
	memDC.SetTextColor(clrWFrame);
	TextOut(memDC,px,1,&ich,1);
	TextOut(memDC,px+1,1,&ich,1);
	TextOut(memDC,px+2,1,&ich,1);
	TextOut(memDC,px,2,&ich,1);
	TextOut(memDC,px+2,2,&ich,1);
	TextOut(memDC,px,3,&ich,1);
	TextOut(memDC,px+1,3,&ich,1);
	// Shadow
	memDC.SetTextColor(clrWShadow);
	TextOut(memDC,px+2,3,&ich,1);
	*/

	// Face
	memDC.SetTextColor(clrWFace);
	TextOut(memDC,px+1,2,&ich,1);
}

enum CharSet {
	CHS_BASICLATIN,
	CHS_LATINSUPL,
	CHS_LATINEX1,
	CHS_LATINEX2,
	CHS_CYRILLIC,
	CHS_COUNT
};

const sint32 CHARSET_TABLES[CHS_COUNT][2] = {
	{ 0x0020, 0x007E },
	{ 0x00C0, 0x00FF },
	{ 0x0100, 0x017F },
	{ 0x0180, 0x024F },
	{ 0x0400, 0x04FF }
};

bool MakeBitFont(const iStringT& face_name, sint32 siz, LONG style, const iStringT& fname)
{
	iStringT pngName = fname+_T(".png");
	printf("%s (%s) : ",face_name.CStr(),fname.CStr());
	/*
	if (iFile::Exists(pngName)){
		printf("Already exists. Skiped...\n");
		return false;
	}*/


	HFONT hFont = CreateFont(siz,0,0,0, style, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_RASTER_PRECIS, CLIP_STROKE_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH, face_name.CStr());

	iMemoryDC memDC(iSize(4096,128));
	memDC.SetFont(hFont);

	// calculate widths for whole table
    int *fntW = new int[0xFFFF];
	if (!GetCharWidth32(memDC,0,0xFFFF,fntW)) {
		printf("Error: 0x%X\n",GetLastError());
		return false;
	}

	// calculate total width of all required characters
	int tw=0;
	for (uint32 cc=0; cc<CHS_COUNT; ++cc) {
		for (uint32 xx=CHARSET_TABLES[cc][0]; xx<=CHARSET_TABLES[cc][1]; ++xx) tw += (fntW[xx]);
	}

	// Prepare text metrics and dib
	TEXTMETRIC tm;
	GetTextMetrics(memDC,&tm);
	memDC.Resize(iSize(tw,tm.tmHeight+3));
	memDC.m_Dib.Fill(cColorBlack, BLEND_SRCCOPY);
	memDC.SetFont(hFont);
	memDC.SetTextBkMode(TRANSPARENT);

	int px=0;

	for (cc=0; cc<CHS_COUNT; ++cc) {
		for (uint32 xx=CHARSET_TABLES[cc][0]; xx<=CHARSET_TABLES[cc][1]; ++xx) {
			ComposeCharacter(memDC, xx, px, fntW[xx]);
			px += (fntW[xx]);
		}
	}

	iDibColorChannel::FillChannel(memDC.m_Dib, Channel_Alpha, 0xFF);

	if (!iDibSaver::SavePNG(memDC.m_Dib,pngName)){
		printf("Error! Unable to save png file!\n");
		return false;
	}

	printf("Done!\n");
	return true;
}

int main(int argc, char* argv[])
{
	ShowHeap();

    MakeBitFont(L"Small fonts",-9,FW_NORMAL,L"std");

	return 0;
}
