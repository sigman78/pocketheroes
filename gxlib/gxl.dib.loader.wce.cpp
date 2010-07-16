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

#ifdef OS_WINCE

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
#define IMGDECOMP_E_NOIMAGE             0x800b0100
typedef void (CALLBACK *PROGRESSFUNC)(void *pRender, BOOL bComplete, LPARAM lParam);
typedef DWORD (CALLBACK *GETDATAFUNC)(LPSTR szBuffer, DWORD dwBufferMax, LPARAM lParam);
typedef struct tagDecompressImageInfo {
	DWORD                   dwSize;                 // Size of this structure
	LPBYTE                  pbBuffer;               // Pointer to the buffer to use for data
	DWORD                   dwBufferMax;            // Size of the buffer
	DWORD                   dwBufferCurrent;        // The amount of data which is current in the buffer
	HBITMAP *               phBM;                   // Pointer to the bitmap returned (can be NULL)
	void **					ppImageRender;			// Pointer to an IImageRender object (can be NULL)
	int						iBitDepth;              // Bit depth of the output image
	LPARAM                  lParam;                 // User parameter for callback functions
	HDC                     hdc;                    // HDC to use for retrieving palettes
	int                     iScale;                 // Scale factor (1 - 100)
	int                     iMaxWidth;              // Maximum width of the output image
	int                     iMaxHeight;             // Maxumum height of the output image
	GETDATAFUNC             pfnGetData;             // Callback function to get more data
	PROGRESSFUNC			pfnImageProgress;       // Callback function to notify caller of progress decoding the image
	COLORREF                crTransparentOverride;	// If this color is not (UINT)-1, it will override the
													// transparent color in the image with this color. (GIF ONLY)
} DecompressImageInfo;

//  GetImageData
static DWORD CALLBACK GetImageData( LPSTR szBuffer, DWORD dwBufferMax, LPARAM lParam )
{
	if ( (iFileI*)lParam){
		//graphics file
		iFileI* pFile = (iFileI*)lParam;
		DWORD readen = pFile->Read(szBuffer,dwBufferMax);
		return readen;
	}
	return 0;
}
//  ImageProgress
static void CALLBACK ImageProgress( void *pRender, BOOL bComplete, LPARAM lParam )
{}

/*
 *	
 */
class iDibReaderImpl
{
	typedef HRESULT (CALLBACK *DECIMGINDFUNC)(DecompressImageInfo *pParams);
	HMODULE			m_hDll;
	DECIMGINDFUNC	DecompressImageIndirect;
	uint8			m_Buff[1024];

public:
	iDibReaderImpl()
	: m_hDll(NULL)
	{ }

	bool Init()
	{
		check(!m_hDll);
		m_hDll = LoadLibrary(L"IMGDECMP.DLL");
		if (!m_hDll) return false;
		DecompressImageIndirect = (DECIMGINDFUNC)GetProcAddress(m_hDll,L"DecompressImageIndirect");
		if (!DecompressImageIndirect) {
			Cleanup();
			return false;
		}
		return true;
	}

	void Cleanup()
	{
		if (m_hDll){
			FreeLibrary(m_hDll);
			m_hDll = NULL;
		}
	}

	bool FromStream(iPaletteDib& dib, iFileI* pFile, iDibReader::IMG_TYPE it) const
	{
		//if (it == iDibReader::IT_BMP) return Read8BitBMP(dib, pFile);
		check(0);
		return false;
	}

	bool FromStream(iDib& dib, iFileI* pFile, iDibReader::IMG_TYPE it) const
	{
		if (it == iDibReader::IT_BMP) return ReadBMP(dib, pFile);
		HBITMAP oBmp = NULL;
		HDC hdc = GetDC(0);

		bool bOk = false;
		do {
			//
			DecompressImageInfo dii = {
				sizeof(DecompressImageInfo),
				(BYTE*)m_Buff, 1024, 0,
				&oBmp, NULL, 24,
				(LPARAM)pFile, hdc,
				100, 10000, 10000,
				GetImageData, ImageProgress, (UINT)-1
			};
			HRESULT hr = DecompressImageIndirect( &dii );
			if (hr != S_OK) break;

			BITMAP bitmap;
			GetObject(oBmp,sizeof(bitmap),&bitmap);
			dib.Init(iSize(bitmap.bmWidth,bitmap.bmHeight),iDib::RGB);

			for (uint32 yy=0; yy<dib.GetHeight(); ++yy){
				uint8* src_ptr = (uint8*)bitmap.bmBits + yy*bitmap.bmWidthBytes;
				uint16* dst_ptr = (uint16*)dib.GetPtr() + (dib.GetHeight()-yy-1)*dib.GetWidth();
				for (uint32 xx=0; xx<dib.GetWidth(); ++xx){
					*dst_ptr =  src_ptr[0]>>3 | (uint16)(src_ptr[1]>>2)<<5 | (uint16)(src_ptr[2]>>3)<<11;
					dst_ptr++;
					src_ptr+=3;
				}
			}
		} while(0);
		
		if (oBmp) DeleteObject(oBmp);
		ReleaseDC(0,hdc);
		return bOk;
	}

};

#endif

