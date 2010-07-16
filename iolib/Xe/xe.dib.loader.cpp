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
#include "png.h"
#include "..\\he\\he.file.name.h"

bool iDibLoader::LoadFromResource(iDib &dib, uint32 resid)
{
	HBITMAP hbmp = (HBITMAP)LoadImage(GetModuleHandle(0),MAKEINTRESOURCE(resid),IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);

	if (!hbmp) return false;

	BITMAP bmp;
	if (GetObject(hbmp,sizeof(BITMAP),&bmp) == 0 )
	{
		DeleteObject(hbmp);
		return false;
	}
	
	// resource nust be 24bpp or 32bpp
	if (bmp.bmBitsPixel != 24 && bmp.bmBitsPixel != 32)
	{
		DeleteObject(hbmp);
		return false;
	}

	dib.Resize(iSize(bmp.bmWidth,bmp.bmHeight));
	uint32 buff_siz = bmp.bmWidthBytes * bmp.bmHeight;

	uint8 *buffer = new uint8[buff_siz];
	GetBitmapBits(hbmp,buff_siz,buffer);

	iColor *bits = 	dib.GetBits();

	if (bmp.bmBitsPixel == 24)
	{
		uint8 *tbuffer = buffer;
		for (int yy=0; yy < bmp.bmHeight; ++yy)
		{
			for (int xx=0; xx < bmp.bmWidth; ++xx)
			{
				bits->SetRGBA(tbuffer[2],tbuffer[1],tbuffer[0],255);
				tbuffer += 3;
				bits++;
			}
		}
	}
	else if (bmp.bmBitsPixel == 32)
	{
		uint8 *tbuffer = buffer;
		for (int yy=0; yy < bmp.bmHeight; ++yy)
		{
			for (int xx=0; xx < bmp.bmWidth; ++xx)
			{
				bits->SetRGBA(tbuffer[2],tbuffer[1],tbuffer[0],tbuffer[3]);
				tbuffer += 4;
				bits++;
			}
		}
	}

	delete buffer;

	DeleteObject(hbmp);

	return true;
}

bool iDibLoader::Load(iDib &dib, const iStringT &fname)
{
	iFileName fn(fname);
	iStringT extension(fn.GetExt());

	if (extension.CompareIgnoreCase(_T("bmp"))==0) return LoadBmp(dib,fname);
	else if (extension.CompareIgnoreCase(_T("png"))==0) return LoadPng(dib,fname);
	else if (extension.CompareIgnoreCase(_T("tga"))==0) return LoadTga(dib,fname);

	return false;
}
/*
 *	
 */
bool iDibLoader::LoadBmp(iDib &dib, const iStringT &fname)
{
	iFile file;
	
	file.Open( fname );

	if( !file.IsOpen() ) return false;

	BITMAPFILEHEADER	bmfHeader;
	BITMAPINFO			bmInfo;
	uint32				nFileLen;

	nFileLen = file.GetSize();


	// Read file header
	if (!file.Read((uint8*)&bmfHeader, sizeof(bmfHeader))) return false;

	// File type should be 'BM'
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B')) return false;

	// Read BitmapInfo
	if (!file.Read((uint8*)&bmInfo, sizeof(bmInfo))) return false;

	// Check dimension colordepth and file size
	if (bmInfo.bmiHeader.biBitCount!=24 && bmInfo.bmiHeader.biBitCount!=32 && bmInfo.bmiHeader.biBitCount!=16) return false;

	uint32 LineLen=bmInfo.bmiHeader.biWidth*(bmInfo.bmiHeader.biBitCount/8);
	if (LineLen%4 != 0) LineLen = LineLen + (4-(LineLen%4));


	// Resize Dib 
	dib.Resize(iSize(bmInfo.bmiHeader.biWidth,bmInfo.bmiHeader.biHeight));
	
	file.Seek(bmfHeader.bfOffBits,iFile::FSEEK_SET);
	// Read data
	uint8 linebuff[64000];
	if (bmInfo.bmiHeader.biBitCount==32){
		for (int yy=0; yy<bmInfo.bmiHeader.biHeight; yy++) {
			file.Read(linebuff,LineLen);
			for (int xx=0; xx<bmInfo.bmiHeader.biWidth; xx++) {
				dib.PutPixel(iPoint(xx,bmInfo.bmiHeader.biHeight-yy-1),iColor(linebuff[xx*4+2],linebuff[xx*4+1],linebuff[xx*4],linebuff[xx*4+3]),BLEND_SRCCOPY);
			}
		}
	} else if (bmInfo.bmiHeader.biBitCount==24) {
		for (int yy=0; yy<bmInfo.bmiHeader.biHeight; yy++) {
			file.Read(linebuff,LineLen);
			for (int xx=0; xx<bmInfo.bmiHeader.biWidth; xx++) {
				dib.PutPixel(iPoint(xx,bmInfo.bmiHeader.biHeight-yy-1),iColor(linebuff[xx*3+2],linebuff[xx*3+1],linebuff[xx*3],255),BLEND_SRCCOPY);
			}
		}
	} else if (bmInfo.bmiHeader.biBitCount==16) {
		for (int yy=0; yy<bmInfo.bmiHeader.biHeight; yy++) {
			file.Read(linebuff,LineLen);
			for (int xx=0; xx<bmInfo.bmiHeader.biWidth; xx++) {
				uint16 val = *((uint16*)linebuff+xx);
				iColor clr = iColor( uint8((val&(0x1F<<10))>>7), uint8((val&(0x1F<<6))>>2), uint8((val&0x1F)<<3),255);
				dib.PutPixel(iPoint(xx,bmInfo.bmiHeader.biHeight-yy-1),clr,BLEND_SRCCOPY);
			}
		}
	}

	return true;
}

/*
 *	
 */

extern "C" 
{
	void pngReadData( png_structp png_ptr, png_bytep data, png_size_t length)
	{
		uint32 readen;
		readen = ((iFileI*)(png_ptr->io_ptr))->Read(data,length);
		if (readen != length) png_error(png_ptr, "Read Error");
	}

	void pngWriteData(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		uint32 writen;
		writen = ((iFileI*)(png_ptr->io_ptr))->Write(data,length);
		if (writen != length) png_error(png_ptr, "Write Error");
	}

	void pngFlushData(png_structp png_ptr)
	{
	}
}

bool iDibLoader::LoadPng(iDib &dib, iFileI *fio)
{
	png_structp pngPtr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );

	pngPtr->io_ptr = fio;
	pngPtr->read_data_fn = pngReadData;

	png_infop infoPtr = png_create_info_struct( pngPtr );

	if ( !infoPtr ) 
	{
		png_destroy_read_struct( &pngPtr, (png_infopp)NULL, (png_infopp)NULL);
		return false;
	}

	if ( setjmp( pngPtr->jmpbuf ) ) 
	{
		png_destroy_read_struct( &pngPtr, &infoPtr, (png_infopp)NULL );
		return false;
	}

	/*
	png_init_io( pngPtr, file );
	png_set_sig_bytes( pngPtr, sizeof( header ) );
	*/

	png_read_info( pngPtr, infoPtr );

	int bitDepth;
	int colorType;
	int interlaceType;
	png_uint_32 imageWidth;
	png_uint_32 imageHeight;

	png_get_IHDR( pngPtr, infoPtr, &imageWidth, &imageHeight, &bitDepth, &colorType, &interlaceType, NULL, NULL );
	
	if ( imageWidth < 1 || imageWidth > 10000 || imageHeight < 1 || imageHeight > 10000 ) 
	{
		png_destroy_read_struct( &pngPtr, &infoPtr, (png_infopp)NULL );
		return false;
	}

	dib.Resize(iSize(imageWidth,imageHeight));

	// 3) Setup translators

	if ( (colorType == PNG_COLOR_TYPE_PALETTE) || (colorType == PNG_COLOR_TYPE_GRAY) ) 
	{
		png_set_expand( pngPtr );
	}
	
	if ( colorType == PNG_COLOR_TYPE_PALETTE ) 
	{
		png_set_palette_to_rgb( pngPtr );
	}

	if ( colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA ) 
	{
		png_set_gray_to_rgb( pngPtr );
	}
	else
	{
		png_set_bgr(pngPtr);
	}

    if (png_get_valid(pngPtr, infoPtr,PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(pngPtr);
	}

	png_set_strip_16( pngPtr );
	png_set_packing( pngPtr );

	if ( png_get_valid( pngPtr, infoPtr, PNG_INFO_tRNS ) ) 
	{
		//png_set_expand( pngPtr );
	}

    if (colorType == PNG_COLOR_TYPE_RGB || PNG_COLOR_TYPE_PALETTE)
        png_set_filler(pngPtr, 0xff, PNG_FILLER_AFTER);

	int iPasses = png_set_interlace_handling( pngPtr );

	// 4) Update info
	png_read_update_info( pngPtr, infoPtr );
	
	// 5) Allocate array of lines
	uint8** lines = new uint8* [ imageHeight ] ;
	if ( lines == NULL ) 
	{
		delete lines ;
		png_destroy_read_struct( &pngPtr, &infoPtr, (png_infopp)NULL );
		return false;
	}

	// 6) Setup line pointers
	for( unsigned int i = 0; i < imageHeight; i++ ) 
	{
		lines[i] = (uint8*)dib.GetBits() + 4 * imageWidth * i;
	}

	// 7) Read!
	png_read_image( pngPtr, (unsigned char **)lines );

	png_destroy_read_struct( &pngPtr, &infoPtr, (png_infopp)NULL );


	delete lines;

	return true;
}

//
//
//
bool iDibLoader::LoadPng(iDib &dib, const iStringT &fname)
{
	// 1) Check if file is valid PNG
	FILE* file = _tfopen( fname.CStr(), _T("rb") );
	if ( !file ) return false;

	uint8 header[8];
	fread( header, 1, sizeof( header ), file );

	bool isPng = !png_sig_cmp( (unsigned char*)header, 0, sizeof( header ) );

	if ( !isPng ) 
	{
		fclose( file );
		return false;
	}

	// 2) Allocate structures to read PNG
	png_structp pngPtr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );

	if ( !pngPtr ) 
	{
		fclose( file );
		return false;
	}

	png_infop infoPtr = png_create_info_struct( pngPtr );

	if ( !infoPtr ) 
	{
		png_destroy_read_struct( &pngPtr, (png_infopp)NULL, (png_infopp)NULL);
		fclose( file );
		return false;
	}

	if ( setjmp( pngPtr->jmpbuf ) ) 
	{
		png_destroy_read_struct( &pngPtr, &infoPtr, (png_infopp)NULL );
		return false;
	}

	png_init_io( pngPtr, file );

	png_set_sig_bytes( pngPtr, sizeof( header ) );

	png_read_info( pngPtr, infoPtr );

	int bitDepth;
	int colorType;
	int interlaceType;
	png_uint_32 imageWidth;
	png_uint_32 imageHeight;

	png_get_IHDR( pngPtr, infoPtr, &imageWidth, &imageHeight, &bitDepth, &colorType, &interlaceType, NULL, NULL );
	
	if ( imageWidth < 1 || imageWidth > 10000 || imageHeight < 1 || imageHeight > 10000 ) 
	{
		png_destroy_read_struct( &pngPtr, &infoPtr, (png_infopp)NULL );
		return false;
	}

	dib.Resize(iSize(imageWidth,imageHeight));

	// 3) Setup translators

	if ( (colorType == PNG_COLOR_TYPE_PALETTE) || (colorType == PNG_COLOR_TYPE_GRAY) ) 
	{
		png_set_expand( pngPtr );
	}
	
	if ( colorType == PNG_COLOR_TYPE_PALETTE ) 
	{
		png_set_palette_to_rgb( pngPtr );
	}

	if ( colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA ) 
	{
		png_set_gray_to_rgb( pngPtr );
	}
	else
	{
		png_set_bgr(pngPtr);
	}

    if (png_get_valid(pngPtr, infoPtr,PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(pngPtr);
	}

	png_set_strip_16( pngPtr );
	png_set_packing( pngPtr );

	if ( png_get_valid( pngPtr, infoPtr, PNG_INFO_tRNS ) ) 
	{
		//png_set_expand( pngPtr );
	}

    if (colorType == PNG_COLOR_TYPE_RGB || PNG_COLOR_TYPE_PALETTE)
        png_set_filler(pngPtr, 0xff, PNG_FILLER_AFTER);

	int iPasses = png_set_interlace_handling( pngPtr );

	// 4) Update info
	png_read_update_info( pngPtr, infoPtr );
	
	// 5) Allocate array of lines
	uint8** lines = new uint8* [ imageHeight ] ;
	if ( lines == NULL ) 
	{
		delete lines ;
		png_destroy_read_struct( &pngPtr, &infoPtr, (png_infopp)NULL );
		return 0;
	}

	// 6) Setup line pointers
	for( unsigned int i = 0; i < imageHeight; i++ ) 
	{
		lines[i] = (uint8*)dib.GetBits() + 4 * imageWidth * i;
	}

	// 7) Read!
	png_read_image( pngPtr, (unsigned char **)lines );

	png_destroy_read_struct( &pngPtr, &infoPtr, (png_infopp)NULL );

	fclose( file );

	delete lines;

	return true;
}

// Targa header
typedef struct _TgaHeader
{
    uint8   IdLength;            // Image ID Field Length
    uint8   CmapType;            // Color Map Type
    uint8   ImageType;           // Image Type

    uint16   CmapIndex;           // First Entry Index
    uint16   CmapLength;          // Color Map Length
    uint8   CmapEntrySize;       // Color Map Entry Size

    uint16   X_Origin;            // X-origin of Image
    uint16   Y_Origin;            // Y-origin of Image
    uint16   ImageWidth;          // Image Width
    uint16   ImageHeight;         // Image Height
    uint8   PixelDepth;          // Pixel Depth
    uint8   ImagDesc;            // Image Descriptor
} TGAHEADER;

// Targa image types
#define TGA_Null		0
#define TGA_Map			1
#define TGA_RGB			2
#define TGA_Mono		3
#define TGA_RLEMap		9
#define TGA_RLERGB		10
#define TGA_RLEMono		11
#define TGA_CompMap		32
#define TGA_CompMap4	33

bool iDibLoader::LoadTga(iDib &dib, const iStringT &fname)
{
	// Open targa file
	FILE *targaFile;

	targaFile=_tfopen(fname.CStr(),_T("rb"));
		
	if ( targaFile==NULL) return false;

	// Read targa header
	TGAHEADER targaHeader;

	ZeroMemory ( &targaHeader, sizeof ( TGAHEADER ) );

	fread(&targaHeader.IdLength, 1, 1, targaFile );
	fread(&targaHeader.CmapType, 1, 1, targaFile );
	fread(&targaHeader.ImageType, 1, 1, targaFile );
	fread(&targaHeader.CmapIndex, 2, 1, targaFile );
	fread(&targaHeader.CmapLength, 2, 1, targaFile );
	fread(&targaHeader.CmapEntrySize, 1, 1, targaFile );
	fread(&targaHeader.X_Origin, 2, 1, targaFile );
	fread(&targaHeader.Y_Origin, 2, 1, targaFile );
	fread(&targaHeader.ImageWidth, 2, 1, targaFile );
	fread(&targaHeader.ImageHeight, 2, 1, targaFile );
	fread(&targaHeader.PixelDepth, 1, 1, targaFile );
	fread(&targaHeader.ImagDesc, 1,1,targaFile  );
		
	if ( targaHeader.ImageType != TGA_RGB )	return false;

	bool isFlipped = ((targaHeader.ImagDesc & 0x20) == 0);
			
	// Read targa bits (32 bit)
	if ( targaHeader.PixelDepth == 32 )
	{
		// Create dib
		uint32 w = targaHeader.ImageWidth, h = targaHeader.ImageHeight;
				
		dib.Resize( iSize(w,h) );
		uint8 *src = new uint8[w * h * 4];

		// Read bits
		fread(src, w * h * 4,1,targaFile );
		for (uint32 yy=0; yy<h; yy++)
		{
			if (!isFlipped) CopyBlock32( (uint32*)(dib.GetBits()+yy*w) ,(uint32*)(src+yy*w*4),w);
			else CopyBlock32((uint32*)(dib.GetBits()+(h-(yy+1))*(w)),(uint32*)(src+yy*w*4),w);
		}
		delete [] src;
	}
	// Read targa bits (24 bit)
	else if ( targaHeader.PixelDepth == 24 )
	{
		// Create dib
		uint32 w = targaHeader.ImageWidth, h = targaHeader.ImageHeight;

		dib.Resize( iSize(w, h) );

		// Read bits
		uint8 *src = new uint8[w * h * 3];
				
		if ( src == NULL )
		{
			return false;
		}

		fread( src, w * h * 3, 1, targaFile );

		// Copy bits
		for ( uint32 yy = 0; yy < h; yy++ ) {
			uint32 *sdst;
			if (!isFlipped)	sdst = (uint32*) dib.GetLine(yy);
			else sdst = (uint32*) dib.GetLine(h - yy - 1);
			uint8 *ssrc = (uint8*) src + (yy*w*3);
			for (uint32 xx = 0; xx < w; xx++) {
				*sdst = RGB(ssrc[0],ssrc[1],ssrc[2]);
				ssrc+=3; 
				*sdst++ |= 0xff000000;
			}
		}

		delete [] src;
	} else {
		fclose(targaFile);
		return false;
	}

	fclose(targaFile);
	return true;
}

/*
 *	
 */
bool iDibLoader::LoadGif(iDib &dib, const iStringT &fname)
{
	check(0);
	return true;
}
