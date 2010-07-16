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
#include "png/lpng125/png.h"

bool iDibSaver::SaveBmp(const iDib &dib, const iStringT &fname)
{
	iFile file;
	file.Create( fname );
	if( !file.IsOpen() ) return false;

	BITMAPFILEHEADER	bmfHeader;

	bmfHeader.bfType = ((WORD) ('M' << 8) | 'B');
	bmfHeader.bfReserved1 = 0;
	bmfHeader.bfReserved2 = 0;
	bmfHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (dib.GetWidth() * dib.GetHeight() * 4);
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	const BITMAPINFO *bmi = dib.GetBitmapInfo();
	BITMAPINFOHEADER bmihdr;

	CopyBlock8((uint8*)&bmihdr,(const uint8*)&bmi->bmiHeader,sizeof(BITMAPINFOHEADER));
	bmihdr.biHeight = -bmihdr.biHeight;
	file.Write(&bmfHeader,sizeof(BITMAPFILEHEADER));
	file.Write(&bmihdr,sizeof(BITMAPINFOHEADER));
	for (uint32 yy=0; yy<dib.GetHeight(); ++yy) {
		file.Write(dib.GetLine(dib.GetHeight()-yy-1),dib.GetWidth()*4);
	}
	file.Close();
	return true;
}

bool iDibSaver::SaveBmp16(const iDib &dib, const iStringT &fname)
{
	iFile file;
	file.Create( fname );
	if( !file.IsOpen() ) return false;
	uint32 pix_siz = dib.GetWidth()*dib.GetHeight();
	check(pix_siz);

	// Prepare BITMAPFILEHEADER
	BITMAPFILEHEADER	bmfHeader;
	bmfHeader.bfType = ((WORD) ('M' << 8) | 'B');
	bmfHeader.bfReserved1 = 0;
	bmfHeader.bfReserved2 = 0;
	bmfHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (dib.GetWidth() * dib.GetHeight() * 2);
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	// Prepare BITMAPINFOHEADER
	BITMAPINFOHEADER bmihdr;
	bmihdr.biSize = sizeof(BITMAPINFOHEADER);
	bmihdr.biWidth = dib.GetWidth();
	bmihdr.biHeight = dib.GetHeight();
	bmihdr.biPlanes = 1;
	bmihdr.biBitCount = 16;
	bmihdr.biCompression = BI_RGB;
	bmihdr.biSizeImage = 0;
	bmihdr.biXPelsPerMeter = 600;
	bmihdr.biYPelsPerMeter = 600;
	bmihdr.biClrUsed = 0;
	bmihdr.biClrImportant = 0;

	// Allocate 16bit buffer
	uint16 *buff =  new uint16[pix_siz];

	// Convert 32bit -> 16bit
	const iColor* src_ptr = dib.GetBits();
	uint16* dst_ptr = buff;

	for (uint32 xx=0; xx<pix_siz; ++xx){
		*dst_ptr = 
				((uint16)src_ptr->GetB()>>3) | 
				((uint16)src_ptr->GetG()>>3)<<5 | 
				((uint16)src_ptr->GetR()>>3)<<10;
		src_ptr++;
		dst_ptr++;
	}
	
	file.Write(&bmfHeader,sizeof(BITMAPFILEHEADER));
	file.Write(&bmihdr,sizeof(BITMAPINFOHEADER));

	for (uint32 yy=0; yy<dib.GetHeight(); ++yy) {
		uint32 act_len = dib.GetWidth()*2;
		uint32 pad_len = iALIGN(act_len,4) - act_len;
		file.Write(buff+((dib.GetHeight()-yy-1)*dib.GetWidth()),act_len);
		if (pad_len) file.WriteFill(0,pad_len);
	}

	delete[] buff;

	file.Close();

	return true;
}

bool iDibSaver::SavePNG(const iDib &dib, const iStringT &fname)
{
	// 1) Check if file is valid PNG
	FILE* file = _tfopen( fname.CStr(), _T("wb") );
	if ( !file ) return false;

	// create write struct
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		fclose( file );
		return false;
	}

	// error handling!
	if (setjmp(png_jmpbuf(png_ptr))){
		png_destroy_write_struct(&png_ptr, NULL);

		fclose( file );
		return false;
	}

	// init io
	png_init_io( png_ptr, file );


	// create info struct & write it
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_write_struct(&png_ptr, NULL);

		fclose( file );
		return false;
	}

	uint32 width  = dib.GetWidth();
	uint32 height = dib.GetHeight();

	int color_format = PNG_COLOR_TYPE_RGB_ALPHA; // png output format
	int color_format_bpp = 4; // png bytes per pixel

	png_set_IHDR(png_ptr, info_ptr,	width, height, 8, color_format,
				PNG_INTERLACE_NONE,	PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	png_write_info( png_ptr, info_ptr );


	png_set_bgr(png_ptr);

	// 5) Allocate array of lines
	uint8** lines = new uint8* [ height ] ;
	if ( lines == NULL ) {
		png_destroy_read_struct( &png_ptr, &info_ptr, (png_infopp)NULL );
	
		fclose( file );
		return false;
	}

	// 6) Setup line pointers
	for( unsigned int i = 0; i < height; i++ ) {
		lines[i] = (uint8*)dib.GetBits() + 4 * width * i;
	}
	png_write_image( png_ptr, (unsigned char **)lines );


	png_destroy_write_struct(&png_ptr, &info_ptr);
	delete lines;
	
	fclose( file );

	return true;
}