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
	printf("Bitfont compiler 0.1.a.2\n");
	printf("by Tutankhamen/iO 2004                    \n");
	printf("-----------------------------------------------\n");
}

LPCTSTR FontList[3] = {
	_T("fnt_sml.png"),
	_T("fnt_med.png"),
	_T("fnt_big.png")
};

uint8 Pack8PixelsToByte(const iColor* pSrc, uint32 cnt)
{
	uint8 res = 0;
	for (uint32 nn=0; nn<cnt; ++nn) {
		if (pSrc->GetR() > 0) res |= (1 << nn);
		pSrc++;
	}
	return res;
}

struct CharDesc {
	uint16 	posX;
	uint8	width;
	uint8	reserved;
};

uint32 ExportBitFont(iDib& dib, iDynamicBuffer& buff)
{
	// preread metrics
    CharDesc	fntW[1024];
	uint32 		lastAnch = 0;
	uint32 		curChar = 0;

	// x-metrics
	for (uint32 xx=0; xx<dib.GetWidth(); ++xx) {
		if (dib.GetPixel(iPoint(xx,0)) == iColor(0xFFFF0000)) {
			uint32 w = xx-lastAnch-1;
			fntW[curChar].posX = lastAnch;
			fntW[curChar].width = w;
			fntW[curChar].reserved = 0;
			curChar++;
			lastAnch=xx;
		}
	}
	uint32 w = xx-lastAnch-1;
	fntW[curChar].posX = lastAnch;
	fntW[curChar].width = w;
	fntW[curChar].reserved = 0;
	curChar++;

	// items count
    buff.Write(curChar);
	// font height
	buff.Write(uint8(dib.GetHeight()-1));
	// font widths
	for (uint32 zz=0; zz<curChar; ++zz) {
		buff.Write(fntW[zz].posX);
		buff.Write(fntW[zz].width);
		buff.Write(fntW[zz].reserved);
	}

	uint32 lineBuffSiz = (dib.GetWidth()+7)/8;
	uint8* lineBuff = new uint8[lineBuffSiz];
	buff.Write((uint16)(lineBuffSiz * (dib.GetHeight()-1)));
	for (uint32 yy=1; yy<dib.GetHeight(); ++yy){
		const iColor* pSrc = dib.GetLine(yy);
		memset(lineBuff, 0, lineBuffSiz);
		uint32 pos = 0;
		for (uint32 xx=0; xx<lineBuffSiz; ++xx){
			uint32 cnt = 8;
			if (pos + 8 >= dib.GetWidth()) cnt = dib.GetWidth() - pos;
			lineBuff[xx] = Pack8PixelsToByte(pSrc, cnt);
			pSrc += 8;
			pos += 8;
		}
		buff.Write(lineBuff, lineBuffSiz);
	}
	delete[] lineBuff;

	return curChar;
}

int _tmain(int argc, WCHAR* argv[])
{
	ShowHeap();

	iDynamicBuffer obuff;
	iDib fntDib;

	if (!iDibLoader::LoadPng(fntDib, argv[1])) {
		_tprintf(_T("Unable to open '%s'!\n"), argv[1]);
		return -1;
	}

	uint32 res = ExportBitFont(fntDib, obuff);

	// Store
	iSmartPtr<iFileI> opf(CreateWin32File(_T("font.lzo")));
		
	opf->Write(obuff.GetData(),obuff.GetSize());


	return 0;
}
