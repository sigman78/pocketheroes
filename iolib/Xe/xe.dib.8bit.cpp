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
#include "xe.dib.8bit.h"

/*
 *	
 */
void iPaletteDib::CopyToDibXY(iDib& dib, const iPoint& pos, const iPalette& pal) const
{
	if ( (pos.x + (sint32)m_Siz.w) <= 0 || (pos.y + (sint32)m_Siz.w) <= 0) return;
	iRect src_rect(GetSize());
	iSize siz = iSize(dib.GetWidth() - pos.x, dib.GetHeight() - pos.y);
	iRect dst_rect(pos,siz);
	if (!iClipper::iClipRectRect(dst_rect,iRect(0,0,dib.GetWidth(),dib.GetHeight()),src_rect,GetSize())) return;
	const uint8* src_clr = m_Buff+src_rect.y*m_Siz.w+src_rect.x;
	iColor* dst_clr=dib.GetBits()+dst_rect.y*dib.GetWidth()+dst_rect.x;
	for (uint32 yy=0; yy<dst_rect.h; yy++) {
		for (uint32 xx=0; xx<dst_rect.w; ++xx) {
			if (src_clr[xx]) dst_clr[xx] = pal.GetEntry(src_clr[xx]);
		}
		src_clr+=m_Siz.w;
		dst_clr+=dib.GetWidth();
	}
}

void iPaletteDib::CopyRectToDibXY(iDib& dib, const iRect& srect, const iPoint& pos, const iPalette& pal) const
{
	iRect src_rect(srect);
	iSize siz = iSize(dib.GetWidth() - pos.x, dib.GetHeight() - pos.y);
	iRect dst_rect(pos,siz);
	if (!iClipper::iClipRectRect(dst_rect,dib.GetSize(),src_rect,GetSize())) return;
	const uint8* src_clr = m_Buff+src_rect.y*m_Siz.w+src_rect.x;
	iColor* dst_clr=dib.GetBits()+dst_rect.y*dib.GetWidth()+dst_rect.x;
	for (uint32 yy=0; yy<dst_rect.h; yy++) {
		for (uint32 xx=0; xx<dst_rect.w; ++xx) {
			if (src_clr[xx]) dst_clr[xx] = pal.GetEntry(src_clr[xx]);
		}
		src_clr+=m_Siz.w;
		dst_clr+=dib.GetWidth();
	}
}
/*
 *	
 */
bool Load8BitBmp(iPaletteDib& dib, const  iStringT& fname)
{
	if (!iFile::Exists(fname)) return false;
	iSmartPtr<iFileI> pFile(OpenWin32File(fname));
	if (!pFile) return false;

	uint32	nFileLen = (uint32)pFile->GetSize();
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
	dib.ReallocDib(iSize(bmInfo.bmiHeader.biWidth,bmInfo.bmiHeader.biHeight));
	pFile->Seek(bmfHeader.bfOffBits,iFileI::FSEEK_SET);
	// Read data
	iBuff<uint8> linebuff(LineLen);
	for (int yy=0; yy<bmInfo.bmiHeader.biHeight; yy++) {
		pFile->Read(linebuff,LineLen);
		uint8* dst = dib.GetBuffer() + (dib.GetHeight()-yy-1)*dib.GetWidth();
		for (int xx=0; xx<bmInfo.bmiHeader.biWidth; xx++) {
			*dst = linebuff[xx];
			dst++;
		}
	}
	return true;
}

bool Save8BitBmp(const iPaletteDib& dib, const  iStringT& fname)
{
	iFile file;
	file.Create( fname );
	if( !file.IsOpen() ) return false;
	uint32 pix_siz = dib.GetWidth()*dib.GetHeight();
	check(pix_siz);


	uint32 bmInfoSiz = sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD);
	BITMAPINFO *pbmInfo = (BITMAPINFO*)new uint8[bmInfoSiz];
	memset(pbmInfo,0,sizeof(BITMAPINFOHEADER));
	// Prepare BITMAPINFOHEADER
	pbmInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmInfo->bmiHeader.biWidth = dib.GetWidth();
	pbmInfo->bmiHeader.biHeight = dib.GetHeight();
	pbmInfo->bmiHeader.biPlanes = 1;
	pbmInfo->bmiHeader.biBitCount = 8;
	pbmInfo->bmiHeader.biCompression = BI_RGB;
	pbmInfo->bmiHeader.biSizeImage = 0;
	pbmInfo->bmiHeader.biXPelsPerMeter = 600;
	pbmInfo->bmiHeader.biYPelsPerMeter = 600;
	pbmInfo->bmiHeader.biClrUsed = 256;
	pbmInfo->bmiHeader.biClrImportant = 0;

	// Convert palette
	for (uint32 cc=0; cc<256; ++cc){
		iColor clr = dib.GetPaletteEntry((uint8)cc);
		pbmInfo->bmiColors[cc].rgbRed=clr.GetR();
		pbmInfo->bmiColors[cc].rgbGreen=clr.GetG();
		pbmInfo->bmiColors[cc].rgbBlue=clr.GetB();
		pbmInfo->bmiColors[cc].rgbReserved = 0;
	}

	// Prepare BITMAPFILEHEADER
	BITMAPFILEHEADER	bmfHeader;
	bmfHeader.bfType = ((WORD) ('M' << 8) | 'B');
	bmfHeader.bfReserved1 = 0;
	bmfHeader.bfReserved2 = 0;
	bmfHeader.bfSize = sizeof(BITMAPFILEHEADER) + bmInfoSiz + (pix_siz);
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + bmInfoSiz;

	iFLOG((_T("%d - %d - %d"),sizeof(BITMAPFILEHEADER),sizeof(BITMAPINFOHEADER),bmInfoSiz));

	file.Write(&bmfHeader,sizeof(BITMAPFILEHEADER));
	file.Write(pbmInfo,bmInfoSiz);

	for (uint32 yy=0; yy<dib.GetHeight(); ++yy) {
		uint32 act_len = dib.GetWidth();
		uint32 pad_len = iALIGN(act_len,4) - act_len;
		file.Write(dib.GetBuffer() + ((dib.GetHeight()-yy-1)*act_len),act_len);
		if (pad_len) file.WriteFill(0,pad_len);
	}

	delete[] pbmInfo;

	file.Close();

	return true;
}

bool Load8BitPalette(iPalette& pal, const iStringT& fname)
{
	if (iFile::GetSize(fname) != 768) return false;
	iFile file;
	file.Open( fname );
	if( !file.IsOpen() ) return false;

	for (uint32 xx=0; xx<256; ++xx){
		uint32 val;
		file.Read(&val,3);
		pal.SetEntry(xx,iColor( uint8(val & 0xFF), uint8((val & 0xFF00)>>8), uint8((val & 0xFF0000)>>16)) );
	}
	file.Close();
	return true;
}

bool Save8BitPalette(const iPalette& pal, const iStringT& fname)
{
	iFile file;
	file.Create( fname );
	if( !file.IsOpen() ) return false;

	for (uint32 xx=0; xx<256; ++xx){
		iColor clr = pal.GetEntry((uint8)xx);
		uint32 val = (((uint32)clr.GetR()) | ((uint32)clr.GetG())<<8 | ((uint32)clr.GetB())<<16);
		file.Write(&val,3);
	}
	file.Close();
	return true;
}

bool Save8BitPaletteSource(const iPalette& pal, const iStringT& fname)
{
	iFile file;
	file.Create( fname );
	if( !file.IsOpen() ) return false;

	file.WriteString("static RGBColorType palette[] = {\n");

	for (uint32 xx=0; xx<256; ++xx){
		iColor clr = pal.GetEntry((uint8)xx);
		file.WriteString(iFormat("\t{0x%02X, 0x%02X, 0x%02X }",clr.GetR(),clr.GetG(),clr.GetB()));
		if (xx == 255) file.WriteString("\n");
		else file.WriteString(",\n");
	}
	file.WriteString("};\n");

	file.Close();

	return true;
}

bool Save8BitPaletteSource16(const iPalette& pal, const iStringT& fname)
{
	iFile file;
	file.Create( fname );
	if( !file.IsOpen() ) return false;

	file.WriteString("static RGBColorType palette[] = {\n");

	for (uint32 xx=0; xx<256; ++xx){
		iColor clr = pal.GetEntry((uint8)xx);
		uint16 r = clr.GetR();
		uint16 g = clr.GetG();
		uint16 b = clr.GetB();
		file.WriteString(iFormat("\t0x%04X", (r>>3)<<11 | (g>>2)<<5 | (b>>3)));
		if (xx == 255) file.WriteString("\n");
		else if (xx%8 == 0 ) file.WriteString(",\n");
		else file.WriteString(",");
	}
	file.WriteString("};\n");

	file.Close();

	return true;
}

/*
 *	
 */

iHisto::iHisto()
{
	reservedColors = 0;
	totalPixels = 0;
	data = new IntData;
}

iHisto::~iHisto()
{
	delete data;
}

void iHisto::Init( uint8 reserv )
{
	assert( reserv >= 0 && reserv < 255 );
	reservedColors = reserv;
	totalPixels = 0;

	for( int j = 0; j < BOX; j++ ) { 
		for( int k = 0; k < BOX; k++ ) {
			for( int l = 0; l < BOX; l++ ) {
				data->wt[j][k][l] = 0;
				data->mr[j][k][l] = 0;
				data->mg[j][k][l] = 0;
				data->mb[j][k][l] = 0;
				data->m2[j][k][l] = 0.0f;
			}
		}
	}
}

void iHisto::Scan( const iDib& dib )
{
	Hist3d(dib, (long *)&data->wt, (long *)&data->mr, (long *)&data->mg, (long *)&data->mb, (float *)&data->m2);
}

void iHisto::Calculate( iPalette& pal )
{
	box	cube[MAXCOLOR];
	float vv[MAXCOLOR];
	int next=0;
	float temp = 0.0f;
	int palSize = 256 - reservedColors;

	Momt3d((long *)&data->wt, (long *)&data->mr, (long *)&data->mg, (long *)&data->mb, (float *)&data->m2);

	cube[0].r0 = 
		cube[0].g0 = 
		cube[0].b0 = 0;
	cube[0].r1 = 
    	cube[0].g1 = 
    	cube[0].b1 = 32;
	//
	for( int i = 1; i < palSize; i++ ) {

		if (Cut(&cube[next], &cube[i])) {
			vv[next] = (cube[next].vol > 1) ? Var(&cube[next]) : 0.0f;
	    	vv[i] = (cube[i].vol > 1) ? Var(&cube[i]) : 0.0f;
		} else {
			vv[next] = 0.0f;
			i--;
		}
		next = 0;
		temp = vv[0];
		for (int k = 1; k <= i; ++k)
			if (vv[k] > temp) {
				temp = vv[k];
				next = k;
			}
		if (temp <= 0.0f) {
			palSize = i + 1;
	    	break;
		}
	}

	unsigned char* tag = new unsigned char [(BOX * BOX * BOX)];

	for (int k = 0; k < palSize; ++k) {
		Mark(&cube[k], k, tag);
		long weight = Vol(&cube[k], data->wt);

		iColor clr;
		if (weight) {
			clr.r = (uint8)(Vol(&cube[k], data->mr) / weight);
			clr.g = (uint8)(Vol(&cube[k], data->mg) / weight);
			clr.b = (uint8)(Vol(&cube[k], data->mb) / weight);
		} else {
			clr.rgba = 0;
		}
		pal.SetEntry( k, clr );
	}
	
	delete[] tag;
}


void iHisto::Hist3d( const iDib& dib, long *vwt, long *vmr, long *vmg, long *vmb, float *m_2)
{
	int		ind,r,g,b;
	int		inr,ing,inb,table[256];
	long	x,y,i;

	for (i = 0; i < 256; ++i)
	table[i] = i * i;

	for( y = 0; y < (sint32)dib.GetHeight(); ++y ) {
		for( x = 0; x < (sint32)dib.GetWidth(); ++x ) {
			iColor clr = dib.GetPixel( iPoint(x, y) );
			r = clr.r;
			g = clr.g;
			b = clr.b;

			inr = (r >> 3) + 1;
			ing = (g >> 3) + 1;
			inb = (b >> 3) + 1;
///			data->Qadd[i] = 
			ind = (inr << 10) + (inr << 6) + inr + (ing << 5) + ing + inb;
			/* [inr][ing][inb] */
			++vwt[ind];
			vmr[ind] += r;
			vmg[ind] += g;
			vmb[ind] += b;
			m_2[ind] += (float) (table[r] + table[g] + table[b]);
		}
	}
}

void iHisto::Momt3d(long *vwt, long *vmr, long *vmg, long *vmb, float *m_2)
{
	unsigned short ind1,
			ind2;
	unsigned char i,
			r,
			g,
			b;
	long            line,
			line_r,
			line_g,
			line_b,
			area[BOX],
			area_r[BOX],
			area_g[BOX],
			area_b[BOX];
	float           line2,
			area2[BOX];

	for (r = 1; r <= 32; ++r) {
		for (i = 0; i <= 32; ++i) { area2[i] = 0;area[i] = area_r[i] = area_g[i] = area_b[i] = 0; }
		for (g = 1; g <= 32; ++g) {
			line2 = 0;
			line = line_r = line_g = line_b = 0;
			for (b = 1; b <= 32; ++b) {
				ind1 = (r << 10) + (r << 6) + r + (g << 5) + g + b;
				/* [r][g][b] */
				line += vwt[ind1];
				line_r += vmr[ind1];
				line_g += vmg[ind1];
				line_b += vmb[ind1];
				line2 += m_2[ind1];
				area[b] += line;
				area_r[b] += line_r;
				area_g[b] += line_g;
				area_b[b] += line_b;
				area2[b] += line2;
				ind2 = ind1 - 1089; /* [r-1][g][b] */
				vwt[ind1] = vwt[ind2] + area[b];
				vmr[ind1] = vmr[ind2] + area_r[b];
				vmg[ind1] = vmg[ind2] + area_g[b];
				vmb[ind1] = vmb[ind2] + area_b[b];
				m_2[ind1] = m_2[ind2] + area2[b];
			}
		}
	}
}

long iHisto::Vol(struct box * cube, long mmt[BOX][BOX][BOX])
{
	return (mmt[cube->r1][cube->g1][cube->b1]
		- mmt[cube->r1][cube->g1][cube->b0]
		- mmt[cube->r1][cube->g0][cube->b1]
		+ mmt[cube->r1][cube->g0][cube->b0]
		- mmt[cube->r0][cube->g1][cube->b1]
		+ mmt[cube->r0][cube->g1][cube->b0]
		+ mmt[cube->r0][cube->g0][cube->b1]
		- mmt[cube->r0][cube->g0][cube->b0]);
}

long iHisto::Bottom(struct box * cube, unsigned char dir, long mmt[BOX][BOX][BOX])
{
	switch (dir) {
	case RED:
	return (-mmt[cube->r0][cube->g1][cube->b1]
		+ mmt[cube->r0][cube->g1][cube->b0]
		+ mmt[cube->r0][cube->g0][cube->b1]
		- mmt[cube->r0][cube->g0][cube->b0]);
	case GREEN:
	return (-mmt[cube->r1][cube->g0][cube->b1]
		+ mmt[cube->r1][cube->g0][cube->b0]
		+ mmt[cube->r0][cube->g0][cube->b1]
		- mmt[cube->r0][cube->g0][cube->b0]);
	case BLUE:
	return (-mmt[cube->r1][cube->g1][cube->b0]
		+ mmt[cube->r1][cube->g0][cube->b0]
		+ mmt[cube->r0][cube->g1][cube->b0]
		- mmt[cube->r0][cube->g0][cube->b0]);
	}
	return 0;
}

long iHisto::Top(struct box * cube, unsigned char dir, int pos, long mmt[BOX][BOX][BOX])
{
	switch (dir) {
	case RED:
	return (mmt[pos][cube->g1][cube->b1]
		- mmt[pos][cube->g1][cube->b0]
		- mmt[pos][cube->g0][cube->b1]
		+ mmt[pos][cube->g0][cube->b0]);
	case GREEN:
	return (mmt[cube->r1][pos][cube->b1]
		- mmt[cube->r1][pos][cube->b0]
		- mmt[cube->r0][pos][cube->b1]
		+ mmt[cube->r0][pos][cube->b0]);
	case BLUE:
	return (mmt[cube->r1][cube->g1][pos]
		- mmt[cube->r1][cube->g0][pos]
		- mmt[cube->r0][cube->g1][pos]
		+ mmt[cube->r0][cube->g0][pos]);
	}
	return 0;
}

float iHisto::Var(struct box * cube)
{
	float	dr,dg,db,xx;

	dr = (float)Vol(cube, data->mr);
	dg = (float)Vol(cube, data->mg);
	db = (float)Vol(cube, data->mb);
	xx = data->m2[cube->r1][cube->g1][cube->b1]
	- data->m2[cube->r1][cube->g1][cube->b0]
	- data->m2[cube->r1][cube->g0][cube->b1]
	+ data->m2[cube->r1][cube->g0][cube->b0]
	- data->m2[cube->r0][cube->g1][cube->b1]
	+ data->m2[cube->r0][cube->g1][cube->b0]
	+ data->m2[cube->r0][cube->g0][cube->b1]
	- data->m2[cube->r0][cube->g0][cube->b0];

	return (xx - (dr * dr + dg * dg + db * db) / (float) Vol(cube, data->wt));
}

float iHisto::Maximize(struct box* cube, unsigned char dir, int first, int last, int* cut, long whole_r, long whole_g, long whole_b, long whole_w)
{
	long   half_r,
					half_g,
					half_b,
					half_w;
	long            base_r,
					base_g,
					base_b,
			base_w;
	int    i;
	float  temp,    max;

	base_r = Bottom(cube, dir, data->mr);
	base_g = Bottom(cube, dir, data->mg);
	base_b = Bottom(cube, dir, data->mb);
	base_w = Bottom(cube, dir, data->wt);
	max = 0.0f;
	*cut = -1;
	for (i = first; i < last; ++i) {
	half_r = base_r + Top(cube, dir, i, data->mr);
		half_g = base_g + Top(cube, dir, i, data->mg);
		half_b = base_b + Top(cube, dir, i, data->mb);
		half_w = base_w + Top(cube, dir, i, data->wt);
		/* now half_x is sum over lower half of box, if split at i */
		if (half_w == 0) {      /* subbox could be empty of pixels! */
			continue;           /* never split into an empty box */
		} else
			temp = ((float) half_r * half_r + (float) half_g * half_g +
			(float) half_b * half_b) / half_w;

	half_r = whole_r - half_r;
		half_g = whole_g - half_g;
	half_b = whole_b - half_b;
		half_w = whole_w - half_w;
		if (half_w == 0) {      /* subbox could be empty of pixels! */
		continue;           /* never split into an empty box */
		} else
			temp += ((float) half_r * half_r + (float) half_g * half_g +
			 (float) half_b * half_b) / half_w;

		if (temp > max) {
			max = temp;
		*cut = i;
	}
	}
	return (max);
}

void iHisto::Mark(struct box * cube, int label, unsigned char *tag)
{
	int    r, g, b;
	for (r = cube->r0 + 1; r <= cube->r1; ++r)
	for (g = cube->g0 + 1; g <= cube->g1; ++g)
		for (b = cube->b0 + 1; b <= cube->b1; ++b)
		tag[(r << 10) + (r << 6) + r + (g << 5) + g + b] = label;
}

int iHisto::Cut(struct box * set1, struct box * set2)
{
	unsigned char   dir;
	int             cutr,
					cutg,
					cutb;
	float           maxr,
			maxg,
					maxb;
	long            whole_r,
			whole_g,
			whole_b,
					whole_w;

	whole_r = Vol(set1, data->mr);
	whole_g = Vol(set1, data->mg);
	whole_b = Vol(set1, data->mb);
	whole_w = Vol(set1, data->wt);

	maxr = Maximize(set1, RED, set1->r0 + 1, set1->r1, &cutr,
			whole_r, whole_g, whole_b, whole_w);
	maxg = Maximize(set1, GREEN, set1->g0 + 1, set1->g1, &cutg,
			whole_r, whole_g, whole_b, whole_w);
	maxb = Maximize(set1, BLUE, set1->b0 + 1, set1->b1, &cutb,
					whole_r, whole_g, whole_b, whole_w);

	if ((maxr >= maxg) && (maxr >= maxb)) {
	dir = RED;
		if (cutr < 0)
			return 0;           /* can't split the box */
	} else if ((maxg >= maxr) && (maxg >= maxb))
	dir = GREEN;
	else
		dir = BLUE;

	set2->r1 = set1->r1;
	set2->g1 = set1->g1;
	set2->b1 = set1->b1;

	switch (dir) {
	case RED:
		set2->r0 = set1->r1 = cutr;
	set2->g0 = set1->g0;
		set2->b0 = set1->b0;
		break;
	case GREEN:
		set2->g0 = set1->g1 = cutg;
	set2->r0 = set1->r0;
		set2->b0 = set1->b0;
		break;
	case BLUE:
	set2->b0 = set1->b1 = cutb;
		set2->r0 = set1->r0;
		set2->g0 = set1->g0;
	break;
	}
	set1->vol = (set1->r1 - set1->r0) * (set1->g1 - set1->g0) * (set1->b1 - set1->b0);
	set2->vol = (set2->r1 - set2->r0) * (set2->g1 - set2->g0) * (set2->b1 - set2->b0);
	return 1;
}

/*
 *	
 */
void iRemapper::Process( const iDib& src, iPaletteDib& dst)
{
	dst.ReallocDib(src.GetSize());

	uint32 pix_siz = src.GetWidth()*src.GetHeight();
	const iColor* src_ptr = src.GetBits();
	uint8* dst_ptr = dst.GetBuffer();

	for( uint32 pp = 0; pp < pix_siz; ++pp ) {
		*dst_ptr++ = FindBestColor(*src_ptr++);
	}

	dst.AssignPalette(map);
}

void iRemapper::Process( const iDib& src, iDib& dst)
{
	dst.Resize(src.GetSize());

	uint32 pix_siz = src.GetWidth()*src.GetHeight();
	const iColor* src_ptr = src.GetBits();
	iColor* dst_ptr = dst.GetBits();

	for( uint32 pp = 0; pp < pix_siz; ++pp ) {
		*dst_ptr++ = map.GetEntry(FindBestColor(*src_ptr++));
	}
}

unsigned int iRemapper::FindBestColor( const iColor &clr )
{
	int mindiff = 0x7fffffff;
	int index = 0;

	for( int i = 0; i < 256; ++i ) {
		iColor pel = map.GetEntry( i );
		int dr = (int)clr.r - pel.r;
		if ( dr < 0 ) dr = -dr;
		int dg = (int)clr.g - pel.g;
		if ( dg < 0 ) dg = -dg;
		int db = (int)clr.b - pel.b;
		if ( db < 0 ) db = -db;
		int dif = dr + dg + db;
		if ( dif < mindiff ) {
			mindiff = dif;
			index = i;
		}
	}
	return index;
}
