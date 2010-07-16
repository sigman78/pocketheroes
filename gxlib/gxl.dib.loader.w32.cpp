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

#ifdef OS_WIN32

bool Read8BitBMP(iPaletteDib& dib, iFileI* pFile)
{
	uint32	nFileLen = pFile->GetSize();
	// Read file header
	BITMAPFILEHEADER	bmfHeader;
	if (!pFile->Read(&bmfHeader, sizeof(bmfHeader))) return false;
	// File type should be 'BM'
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B')) return false;
	// Read BitmapInfo
	BITMAPINFO			bmInfo;
	if (!pFile->Read(&bmInfo, sizeof(bmInfo))) return false;
	// Check dimension colordepth and file size
	if (bmInfo.bmiHeader.biBitCount!=8) return false;
	uint32 LineLen=bmInfo.bmiHeader.biWidth;
	if (LineLen%4 != 0) LineLen = LineLen + (4-(LineLen%4));
	// Resize Dib 
	dib.Init(iSize(bmInfo.bmiHeader.biWidth,bmInfo.bmiHeader.biHeight));
	pFile->Seek(bmfHeader.bfOffBits,iFileI::FSEEK_SET);
	// Read data
	iBuff<uint8> linebuff(LineLen);
	for (int yy=0; yy<bmInfo.bmiHeader.biHeight; yy++) {
		pFile->Read(linebuff,LineLen);
		iPaletteDib::pixel* dst = dib.GetPtr() + (dib.GetHeight()-yy-1)*dib.GetWidth();
		for (int xx=0; xx<bmInfo.bmiHeader.biWidth; xx++) {
			*dst = linebuff[xx];
			dst++;
			//dib.PutPixel(iPoint(xx,bmInfo.bmiHeader.biHeight-yy-1),iColor(linebuff[xx*4+2],linebuff[xx*4+1],linebuff[xx*4],linebuff[xx*4+3]),BLEND_SRCCOPY);
		}
	}
	return true;
}

bool ReadBMP(iDib& dib, iFileI* pFile)
{
	uint32	nFileLen = pFile->GetSize();
	// Read file header
	BITMAPFILEHEADER	bmfHeader;
	if (!pFile->Read(&bmfHeader, sizeof(bmfHeader))) return false;
	// File type should be 'BM'
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B')) return false;
	// Read BitmapInfo
	BITMAPINFO			bmInfo;
	if (!pFile->Read(&bmInfo, sizeof(bmInfo))) return false;
	// Check dimension colordepth and file size
	if (bmInfo.bmiHeader.biBitCount!=16 && bmInfo.bmiHeader.biBitCount!=24 && bmInfo.bmiHeader.biBitCount!=32) return false;
	uint32 LineLen=bmInfo.bmiHeader.biWidth*(bmInfo.bmiHeader.biBitCount/8);
	if (LineLen%4 != 0) LineLen = LineLen + (4-(LineLen%4));
	// Resize Dib 
	dib.Init(iSize(bmInfo.bmiHeader.biWidth,bmInfo.bmiHeader.biHeight),iDib::RGB);
	pFile->Seek(bmfHeader.bfOffBits,iFileI::FSEEK_SET);
	// Read data
	iBuff<uint8> linebuff(LineLen);
	iDib::pixel* dst = dib.GetPtr();
	if (bmInfo.bmiHeader.biBitCount==32) {
		// 32 BIT ( X8 R8 G8 B8)
		for (int yy=0; yy<bmInfo.bmiHeader.biHeight; yy++) {
			pFile->Read(linebuff,LineLen);
			for (int xx=0; xx<bmInfo.bmiHeader.biWidth; xx++) {
				*dst = 0xFF;
				dst++;
				//dib.PutPixel(iPoint(xx,bmInfo.bmiHeader.biHeight-yy-1),iColor(linebuff[xx*4+2],linebuff[xx*4+1],linebuff[xx*4],linebuff[xx*4+3]),BLEND_SRCCOPY);
			}
		}
	} else if (bmInfo.bmiHeader.biBitCount==24) {
		// 24 BIT ( R8 G8 B8 )
		for (int yy=0; yy<bmInfo.bmiHeader.biHeight; yy++) {
			pFile->Read(linebuff,LineLen);
			for (int xx=0; xx<bmInfo.bmiHeader.biWidth; xx++) {
				dib.PutPixel(xx,bmInfo.bmiHeader.biHeight-yy-1,((uint16)(linebuff[xx*3+2]>>3) << 11 | (uint16)(linebuff[xx*3+1]>>2) << 5 | ((uint16)(linebuff[xx*3]>>3))));
			}
		}
	} else if (bmInfo.bmiHeader.biBitCount==16) {
		// 16 BIT ( R5 G6 B5 )
		for (int yy=0; yy<bmInfo.bmiHeader.biHeight; yy++) {
			pFile->Read(linebuff,LineLen);
			for (int xx=0; xx<bmInfo.bmiHeader.biWidth; xx++) {
				uint16 val = *((uint16*)linebuff.GetPtr()+xx);
				dib.PutPixel(xx,bmInfo.bmiHeader.biHeight-yy-1,(val&(0x1F<<10))<<1 | (val&(0x1F<<5))<<1 | (val&(0x1F)));
			}
		}
	}
	return true;
}

/*
 *	
 */
class iDibReaderImpl
{
public:
	iDibReaderImpl()
	{ }

	bool Init()
	{
		return true;
	}

	void Cleanup()
	{
	}

	bool FromStream(iPaletteDib& dib, iFileI* pFile, iDibReader::IMG_TYPE it) const
	{
		if (it == iDibReader::IT_BMP) return Read8BitBMP(dib,pFile);
		return false;
	}

	bool FromStream(iDib& dib, iFileI* pFile, iDibReader::IMG_TYPE it) const
	{
		if (it == iDibReader::IT_BMP) return ReadBMP(dib,pFile);
		return false;
	}
};

#endif
