#include "stdafx.h"
#include "resource.h"
#include "ExportDlg.h"
#include "SortArray.h"
#include "serialize.h"
#include "MinimapView.h"
#include "lzo.h"
#include "xau/xau.audio.h"
#include "xau/xjp.codec.h"

#define HMM_DEV_EXPORT
//#define HMM_JIM_VERSION

//////////////////////////////////////////////////////////////////////////
LPCTSTR thetext[] = {  
			_T("#FFB0#S2Pocket Heroes"),
            _T("#FDDDv.1.04 beta"),
            _T("#FDDDby iO UPG"),
            _T(""),
            _T(""),
            _T("#FFB0Programmers:"),
            _T("#FDDDRobert Tarasov"),
            _T("#FDDD(Tutankhamen/iO)"),
            _T("#FDDDAnton Stuk"),
            _T("#FDDDSiGMan/iO"),
            _T("#FDDDAndrey Kiryushkin (Hedin)"),
            _T(""),
            _T("#FFB0Artists:"),
            _T("#FDDDEugene Ivanov (Jim)"),
            _T("#FDDDDmitry Zaitsev"),
            _T("#FDDDAlek Revyako"),
			_T("#FDDDMihail Chilikin"),
            _T("#FDDDOleg Kirshev"),
			_T("#FDDDVladimir Gulevskiy (GRAFuS)"),
			_T("#FDDDKevin Brock (tekpir8)"),
			_T("#FDDDStanislav (Mr.Flasher)"),
			_T("#FDDDAlexey Garkushin (GAS13)"),
            _T("#FDDDTutankhamen/iO"),
            _T(""),
            _T("#FFB0Guest artists:"),
            _T("#FDDDSimon Butler"),
            _T("#FDDDDanny Flexner"),
            _T("#FDDDIan Schlaepfer"),
            _T("#FDDDPaul Noble"),
            _T("#FDDDMichael Elwin Setiadi"),
			_T("#FDDDDmitry Koshelev"),
			_T("#FDDDIvan Ivanov"),
            _T(""),
            _T("#FFB0Map designers:"),
			_T("#FDDDRoman Tersky (Mongol)"),
			_T("#FDDDKevin Brock (tekpir8)"),
			_T("#FDDDRuslan Pantyushin (Lain)"),
			_T("#FDDDTsukanov Nikita (keks-n)"),
            _T("#FDDDEugene Ivanov (Jim)"),
            _T("#FDDDYaroslaw Artemiev (Yaros)"),
            _T("#FDDDVictor Chirea (victorash)"),
            _T("#FDDDObi-Van (Stas)"),
            _T("#FDDDArtem Stragnik (Starmi)"),
            _T("#FDDDMihail Loktionov (Matador)"),
			_T("#FDDDDraco Lockhard"),
            _T("#FDDDTutankhamen/iO"),
            _T(""),
            _T("#FFB0WEB master:"),
            _T("#FDDDSergey Indiukov (DukereD)"),
            _T("#FDDDTimur Parfentsev (Basmach)"),
            _T(""),
            _T("#FFB0Manual and Help file:"),
            _T("#FDDDAndy Young (67L48)"),
            _T("#FDDDVictor Chirea (victorash)"),
            _T("#FDDDSergey Indiukov"),
			_T(""),
            _T("#FFB0Testers:"),
            _T("#FDDDYaroslaw Artemiev (Yaros)"),
			_T("#FDDDStanislav (Mr.Flasher)"),
            _T("#FDDDAndrew Busygin"),
            _T("#FDDDSergey Indiukov (DukereD)"),
            _T("#FDDDVictor Chirea (victorash)"),
            _T("#FDDDEugene Ivanov (Jim)"),
            _T("#FDDDObi-Van (Stas)"),
            _T("#FDDDIvan Alekhin (gsvano)"),
            _T("#FDDDBaRaKuDa (Alexander)"),
            _T("#FDDDArtem Stragnik (Starmi)"),
            _T("#FDDDChibis"),
            _T("#FDDDMihail Loktionov (Matador)"),
			_T("#FDDDAlexey Muratov (Till)"),
			_T("#FDDDAlexander Muranov (SAS)"),
			_T("#FDDDTsukanov Nikita (keks-n)"),
            _T(""),
            _T("#FFB0Alpha version supporters:"),
            _T("#FDDDAndrey Glinsky"),
            _T("#FDDDFjodor Dzjuba"),
            _T("#FDDDJustin Jones"),
            _T("#FDDDChibis"),
            _T("#FDDDRalph Sanders"),
            _T("#FDDDChris Bygrave"),
            _T("#FDDDDavid Zuravel"),
            _T("#FDDDArtem Stragnik (Starmi)"),
            _T("#FDDDValdemar"),
            _T("#FDDDRS"),
            _T("#FDDDSergey Indiukov (DukereD)"),
            _T("#FDDDTom Fitzpatrick"),
            _T("#FDDDRoman Klyachkivsky"),
            _T("#FDDDOlivier Binda"),
            _T("#FDDDEmanuel Mayer"),
            _T("#FDDDKonstantin Ilyin"),
            _T("#FDDDPaul Cook"),
            _T("#FDDDAnton Rozen"),
            _T("#FDDDPetr Cermak"),
            _T("#FDDDNorry Messina"),
            _T("#FDDDCary Fleming"),
            _T("#FDDDLjusalfheim (4PDA)"),
            _T("#FDDDMarc Ruder"),
            _T("#FDDDMichael Thomas"),
            _T("#FDDDJean-Paul Lacombe"),
            _T("#FDDDestena.ru"),
            _T("#FDDDMaksim Frolov"),
            _T("#FDDDMiroslav Dvoracek"),
            _T("#FDDDEvgeny Kremer"),
            _T("#FDDDGrulezZ"),
            _T("#FDDDLSDsl"),
            _T("#FDDDSergey Chalykh"),
            _T("#FDDDAnton Stuk"),
            _T("#FDDDDmitry Sinchilin"),
            _T("#FDDDAleksander Larionov"),
            _T(""),
            _T("#FFB0Special thanx to:"),
            _T("#FDDDJon Van Caneghem"),
			_T("#FDDDDmitry Lekhno (Krondor)"),
};

//////////////////////////////////////////////////////////////////////////
void MakeSpanSprite(const iDib& src, iDynamicBuffer &buff);
void MakeSprite(const iDib& src, iDynamicBuffer &buff);
void CropSprite(iDib& dib, iPoint& anchor);
//////////////////////////////////////////////////////////////////////////
const uint32 SPFT_RAW		= 0x0;
const uint32 SPFT_SPANNED	= 0x1;
const uint32 SPF_SHADOW		= 0xE;
const uint32 SPF_SHT_NONE	= 0x0;
const uint32 SPF_SHT_BASIC	= 0x2;
const uint32 SPF_SHT_GUI	= 0x4;
const uint32 SPF_SHT_ALT	= 0x6;

//////////////////////////////////////////////////////////////////////////
class GfxBankExporter
{
public:
	GfxBankExporter(iExportDlg& edlg) : m_ExpDlg(edlg) {}

	void Put( const iDib& sprite, const iStringT& name, bool spanned = true, bool crop = false);
	void Put( const iDib& sprite, const iStringT& name, iRect rc, bool spanned = true, bool crop = false);
	void Put( const iSprite& sprite, const iStringT& name );

	bool Export( iFileI* dest );
	bool Export( const iStringT& fileName );

	size_t Size() const
	{ return catalog.GetSize();	}
	
	void Clear()
	{ catalog.RemoveAll(); pixels.Clean(); }

	//size_t EstimateFileSize() const;

private:

	bool Validate( const iDib& sprite, uint8 type );
	void AddSprite( const iDib& sprite, const iStringT& name, iPoint origin, uint8 type, bool crop );

#pragma pack(push,1)
	struct BankImage
	{
		uint8	imageType;		// type props
		uint32	dataOffset;		// offset of the sprite data from the begining of data block ( in uint16 )
		uint16	imageWidth;
		uint16	imageHeight;
		sint16	originX;
		sint16	originY;
	};
#pragma pack(pop)

	iSimpleArray<BankImage>	catalog;
	iDynamicBuffer			pixels;
	iExportDlg&				m_ExpDlg;
	iDynamicBuffer			m_procData;
};

void
GfxBankExporter::AddSprite( const iDib& _sprite, const iStringT& name, iPoint origin, uint8 type, bool crop )
{
	
	if (!Validate( _sprite, type )) {
		m_ExpDlg.Log(name + _T(" -!- Warning: spanned sprite is bigger than raw!\n"));
	}

	iDib dib(&_sprite);
	if (crop) {
		//iFLOG((_T("Croping sprite %s (%d,%d,%d,%d):"),name.CStr(),origin.x,origin.y,dib.GetWidth(),dib.GetHeight()));
		CropSprite(dib,origin);
		//iFLOG((_T("\t->(%d,%d,%d,%d):"),origin.x,origin.y,dib.GetWidth(),dib.GetHeight()));
	} 

	BankImage img;
	img.imageType	= type;
	img.dataOffset	= pixels.GetCurrentPos() / sizeof(uint16);
	img.imageWidth	= (uint16)dib.GetWidth();
	img.imageHeight	= (uint16)dib.GetHeight();
	img.originX		= (sint16)origin.x;
	img.originY		= (sint16)origin.y;
		
	catalog.Add( img );

	if ( type & 1 ) {
		MakeSpanSprite( dib, m_procData );

		check( m_procData.GetSize() < 65536*2 );	// Im too lazy to split uint32 ;)

		pixels.Write( (uint16)(m_procData.GetSize()/sizeof(uint16)) );		// <*1*>
		pixels.Write( m_procData.GetData(), m_procData.GetSize() );
	} else {
		MakeSprite( dib, m_procData );
		pixels.Write( m_procData.GetData(), m_procData.GetSize() );
	}
}

bool
GfxBankExporter::Export(const iStringT& fileName )
{
	iSmartPtr<iFileI> file( CreateWin32File( fileName ) );
	if ( !file ) return false;
	return Export( file.Get() );
}

bool
GfxBankExporter::Export( iFileI* dest )
{
	check( dest != 0 );

	// header
	uint32	hdrBits[4];
	hdrBits[0]	= 0x1A464253;
	hdrBits[1]	= catalog.GetSize();
	hdrBits[2]	= pixels.GetSize();
	hdrBits[3]	= 0xffffffff;

	dest->Write( hdrBits, sizeof(hdrBits) );
	dest->Write( pixels.GetData(), pixels.GetSize() );
	dest->Write( hdrBits, sizeof(hdrBits) );

	for( uint32 n = 0; n != catalog.GetSize(); ++n ) {
		dest->Write( &catalog[n], sizeof(BankImage) );
	}

	m_ExpDlg.Log(iFormat(_T("- Done: %d bytes in %d sprites (Dsc: %d bytes).\n"),pixels.GetSize(), catalog.GetSize(),catalog.GetSize()*sizeof(BankImage)));

	return true;
}

void 
GfxBankExporter::Put( const iDib& sprite, const iStringT& name, bool spanned, bool crop )
{
	iDib dib( sprite );
	uint8 type = (uint8)(spanned?SPFT_SPANNED:SPFT_RAW);
	AddSprite( dib, name, iPoint(0,0) , type, crop );
}

void 
GfxBankExporter::Put( const iDib& sprite, const iStringT& name, iRect rc, bool spanned, bool crop )
{
	iDib part;
	part.Create( &sprite, rc );
	Put( part, name, spanned, crop );
}

void
GfxBankExporter::Put( const iSprite& sprite, const iStringT& name )
{
	uint8 type = (uint8)(sprite.spanned?SPFT_SPANNED:SPFT_RAW) | (sprite.shadow << 1);
	AddSprite( sprite.dib, name, iPoint( sprite.anchor.x, sprite.anchor.y ), type, sprite.crop );
}

bool 
GfxBankExporter::Validate( const iDib& sprite, uint8 type )
{
	MakeSpanSprite( sprite, m_procData);
	uint32 spannedSize = m_procData.GetSize();
	MakeSprite( sprite, m_procData);
	uint32 rawSize = m_procData.GetSize();

	bool spanRequested = type & 1;

	if ( rawSize < spannedSize && spanRequested ) {
		return false;
	}

	return true;
}

struct PixExporter
{
	// TODO : export as header listing [offset:length] for each element
	// + raw bits : this would make it easy to load selectively
};

/*
 *	
 */
#define RGB32_2_RGB16(bgra) (uint16)(((bgra>>8)&0xF800) | ((bgra>>5)&0x7E0) | ((bgra>>3)&0x1F))

void MakeSpanSprite(const iDib& src, iDynamicBuffer &buff)
{
	buff.Reset();
	for (sint32 yy=0; yy<(sint32)src.GetHeight(); ++yy){
		const iColor* sptr = src.GetLine(yy);
		const iColor* tptr = sptr;

		uint16*  phIdx = NULL;
		sint32	fspace=0;
		sint32	span_begin;
		sint32	span_len;
		sint32	mode=0;

		for (sint32 xx=0; xx<(sint32)src.GetWidth(); ++xx){
			if ( tptr->GetA() != 0 ){
				if (mode == 0){
					span_begin = xx;
					span_len = 0;
					mode = 1;
				}
				span_len++;
			} else {
				if (mode == 1){
					mode = 0;
					phIdx = (uint16*)buff.GetCurrentData();
					buff.Write( (uint16)((fspace<<8) | span_len) );
					for (sint32 zz=0; zz<span_len; ++zz) {
						const iColor* pptr = (sptr+span_begin+zz);
						uint16 val = RGB32_2_RGB16(pptr->rgba);
						buff.Write( val );
					}
					fspace=0;
				} 
				fspace++;
			}
			tptr++;
		}

		//
		if (mode == 1) {
			buff.Write( (uint16)((1<<15) | (fspace<<8) | span_len) );
			for (sint32 zz=0; zz<span_len; ++zz) {
				const iColor* pptr = (sptr+span_begin+zz);
				uint16 val = RGB32_2_RGB16(pptr->rgba);
				buff.Write( val );
			}
		} else  if (!phIdx) {
			buff.Write( (uint16)(1<<15) );
		} else {
			*phIdx |= (1<<15);
		}

	}
}



void MakePix( const iDib& src, iDynamicBuffer& buff, bool withAlpha )
{
	buff.Reset();
	buff.ReInit( src.GetWidth() * src.GetHeight() * 3 );
	memset( buff.GetData(), 0, src.GetWidth() * src.GetHeight() * 3 );
	
	xjp::encoder_t enc;
	uint32 picType = withAlpha ? xjp::PIX_RGBA32 : xjp::PIX_RGB32;
	uint32 compQual= withAlpha ? 0x44 : 0x66;
	xjp::enc_init( &enc, src.GetWidth(), src.GetHeight(), picType , compQual );
	uint32 size = xjp::enc_do( &enc, src.GetLine( 0 ), buff.GetData() );
	xjp::enc_done( &enc );
	check( size < buff.GetCapacity() );
	buff.SetSize( size );
}

void MakeSprite(const iDib& src, iDynamicBuffer &buff)
{
	buff.Reset();
	for (uint32 yy=0; yy<src.GetHeight(); ++yy){
		const iColor* pSrc = src.GetLine(yy);
		for (uint32 xx=0; xx<src.GetWidth(); ++xx){
			uint16 val = RGB32_2_RGB16(pSrc->rgba);
			buff.Write( val );
			pSrc++;
		}
	}
}

void CropSprite(iDib& dib, iPoint& anchor)
{
	iRect nrc = iDibHelpers::FindSolidArea(dib);
	if (nrc == dib.GetSize()) return;

	iDib odib(&dib);
	dib.Create(&odib, nrc);
	anchor += nrc.point();
}


/*
 *	
 */
class iHdrDesigner
{
public:
	struct DefEntry {
		DefEntry(const iStringA _defName, sint16 _val)
		: defName(_defName), val(_val) {}
		iStringA	defName;
		sint16		val;
	};
	
	typedef iSimpleArray<DefEntry>	DefList;


	iHdrDesigner(const iStringT& fname, const iStringA& title, const iStringA& prefix)
	: m_FileName(fname), m_Title(title), m_Prefix(prefix) {}

	void AddString(const iStringA& str)
	{
		m_Strings.Add(str);
	}

	void AddDefination(const iStringA& defName, sint16 defVal)
	{
		m_DefList.Add(DefEntry(defName,defVal));
	}

	void Flush()
	{
		FILE *file = _tfopen(m_FileName.CStr(),_T("wt"));
		fprintf(file,"#ifndef _%s_H_\n",m_Title.CStr());
		fprintf(file,"#define _%s_H_\n\n",m_Title.CStr());
		fprintf(file,"// This file is automatically generated by Pocket Heroes resource compiler.\n");
		fprintf(file,"// Do not modify this file -- YOUR CHANGES WILL BE ERASED!\n\n");

		for (uint32 ss=0; ss<m_Strings.GetSize(); ++ss) {
			fprintf(file, m_Strings[ss].CStr());
			fprintf(file, "\n\n");
		}
		for (uint32 xx=0; xx<m_DefList.GetSize(); ++xx){
			fprintf(file,"const sint16 %32s = %d;\n",(m_Prefix+m_DefList[xx].defName).CStr(), m_DefList[xx].val );
		}
		fprintf(file,"\n#endif //_%s_H_\n",m_Title.CStr());
		fclose(file);
	}


private:
	iSimpleArray<iStringA>	m_Strings;
	DefList		m_DefList;
	iStringT	m_FileName;
	iStringA	m_Title;
	iStringA	m_Prefix;
};

/*
 *	Fonts
 */

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
			fntW[curChar].posX = (uint16)lastAnch;
			fntW[curChar].width = (uint8)w;
			fntW[curChar].reserved = 0;
			curChar++;
			lastAnch=xx;
		}
	}
	uint32 w = xx-lastAnch-1;
	fntW[curChar].posX = (uint16)lastAnch;
	fntW[curChar].width = (uint8)w;
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

/*
 *	Export dialog
 */
iExportDlg::iExportDlg(const iStringT& mapFile)
{
	m_mapFileList.Add(mapFile);
}

iExportDlg::iExportDlg(const iSimpleArray<iStringT>& mapFileList)
{
	m_mapFileList = mapFileList;
}


void iExportDlg::Log(const iStringT& msg)
{
	CEdit edit(GetDlgItem(IDC_LOG));
	edit.AppendText(msg.CStr());
}

//
bool iExportDlg::Export()
{
	BOOL bExpRes = IsDlgButtonChecked(IDC_EXPRES);
	UpdateControls(true);
	UpdateWindow();

	// Init
	if (!LZO::Init()) {
		Log(_T("ERROR: Unable to init miniLZO!\n"));
		return false;
	}	

	//
	// Create folders
	//
	if (bExpRes) iFile::DirCreate(GetAppPath()+_T("Game\\Data"));
	iFile::DirCreate(GetAppPath()+_T("Game\\Maps"));
	iFile::DirCreate(GetAppPath()+_T("Game\\Save"));

	const iSpriteMgr &spMgr = gItemMgr.m_SpriteMgr;
	const iSoundMgr &sndMgr = gItemMgr.m_SoundMgr;
	const iAvatarMgr &avMgr = gItemMgr.m_AvatarMgr;
	const iTextMgr &txtMgr = gItemMgr.m_TextMgr;

	// 
	// Export startup images
	//
	if (bExpRes) {
		iDynamicBuffer dbuff;
		iDynamicBuffer cmpbuff;
		iDib iologo;

		iStringT pixPath = GetAppPath() + _T("Resources\\hmm\\GFX\\Pix\\");

		iSmartPtr<iFileI> opf(CreateWin32File(GetAppPath()+_T("Game\\Data\\game.pix")));
/*
		iDibLoader::LoadPng(iologo,pixPath+_T("iologo.png"));
		MakePix(iologo, cmpbuff);

		uint32 len = cmpbuff.GetSize();
		opf->Write(&len, sizeof(len));
		opf->Write(cmpbuff.GetData(),len);


		dbuff.Reset();
		cmpbuff.Reset();
		iDibLoader::LoadPng(iologo,pixPath+_T("intro.png"));
		MakePix(iologo, cmpbuff);

		len = cmpbuff.GetSize();
		opf->Write(&len, sizeof(len));
		opf->Write(cmpbuff.GetData(),len);
*/
		dbuff.Reset();
		cmpbuff.Reset();
		iDibLoader::LoadPng(iologo,pixPath+_T("menuback.png"));
		MakePix(iologo, cmpbuff, false );

		uint32 len = cmpbuff.GetSize();
		opf->Write(&len, sizeof(len));
		opf->Write(cmpbuff.GetData(),len);

		cmpbuff.Reset();
		iDibLoader::LoadPng(iologo,pixPath+_T("title_hmm.png"));
		MakePix(iologo, cmpbuff, true );

		len = cmpbuff.GetSize();
		opf->Write(&len, sizeof(len));
		opf->Write(cmpbuff.GetData(),len);


/*
		// check decompressibilty
		xjp::decoder_t dec;
		xjp::dec_init( &dec, NULL, cmpbuff.GetData() );
		check( dec.width == 320 );
		check( dec.height== 720 );

		void* dbuf = malloc( 320*720*3 );
		xjp::dec_do( &dec, dbuf, 3 );
		xjp::dec_done( &dec );

		FILE* f = fopen("output.pbm", "wb");
		fprintf( f, "P6\n%d %d\n255\n", dec.width, dec.height );
		fwrite( dbuf, dec.width*dec.height*3, 1, f );
		fclose(f);
		free(dbuf);
*/

	}

	// 
	// Export Fonts
	// 
	Log(_T("+ Export fonts: "));
	if (bExpRes) {
		Log(_T("\n"));
		iDynamicBuffer obuff;
		for (uint32 xx=0; xx<3; ++xx) {
			iDib fntDib;
			Log(iFormat(_T("- Export <%s>: "), FontList[xx]));
			if (!iDibLoader::LoadPng(fntDib, GetAppPath()+FontList[xx])) {
				Log(_T("Unable to open source file!\n"));
				continue;
			}

			uint32 res = ExportBitFont(fntDib, obuff);
			Log(iFormat(_T("Done! %d characters exported.\n"), res));
		}
		// Compress
		iDynamicBuffer cmpDataBuff;
		LZO::Compress((const unsigned char*)obuff.GetData(), obuff.GetSize(), cmpDataBuff);
		
	
		// Store
		iSmartPtr<iFileI> opf(CreateWin32File(GetAppPath()+_T("Game\\Data\\fonts.dat")));
		
		opf->Write(&GFNT_FILE_HDR_KEY, sizeof(GFNT_FILE_HDR_KEY));
		opf->Write(&GFNT_FILE_VERSION, sizeof(GFNT_FILE_VERSION));
		
		opf->Write(cmpDataBuff.GetData(),cmpDataBuff.GetSize());
		Log(iFormat(_T("- Done: %d bytes\n"),cmpDataBuff.GetSize()));
	} else Log(_T("Skipped\n"));

	//
	// Export text resources
	//
	Log(_T("+ Dumping text resources:"));
	if (bExpRes) {
		iSortArray<iStringT> textKeyList;
		for (iTextMgr::ConstTextIt tit=txtMgr.GetHash().First(); tit != txtMgr.GetHash().End(); ++tit) {
			textKeyList.Insert((*tit).key,(*tit).idx);
		}

		// Prepare text constants header and two text files
		iHdrDesigner hdrText(GetAppPath()+_T("..\\Common\\cnst_text.h"),"CONSTANT_TEXT_DEFINITIONS","TRID_");
		hdrText.AddString(iFormat<iCharA>("extern LPCWSTR LANG_DATA[%d];",textKeyList.Size()));
		
		iStringT flng_eng_name(GetAppPath() + _T("..\\Common\\lang_eng.cpp"));
		FILE* flng_eng = _wfopen(flng_eng_name.CStr(),L"wt");
		fprintf(flng_eng,"#include <stdafx.h>\n#include \"cnst_text.h\" \n\n");
		fprintf(flng_eng,"LPCWSTR LANG_DATA[%d] = {\n", textKeyList.Size());
		
		iStringT flng_rus_name(GetAppPath() + _T("..\\Common\\lang_rus.cpp"));
		FILE* flng_rus = _wfopen(flng_rus_name.CStr(),L"wt");
		fprintf(flng_rus,"#include <stdafx.h>\n#include \"cnst_text.h\" \n\n");
		fprintf(flng_rus,"LPCWSTR LANG_DATA[%d] = {\n", textKeyList.Size());

		/*iStringT flng_pol_name(GetAppPath() + _T("..\\Common\\lang_pol.cpp"));
		FILE* flng_pol = _wfopen(flng_pol_name.CStr(),L"wt");
		fprintf(flng_pol,"#include <stdafx.h>\n#include \"cnst_text.h\" \n\n");
		fprintf(flng_pol,"LPCWSTR LANG_DATA[%d] = {\n", textKeyList.Size());*/

		for (uint32 tid=0; tid<textKeyList.Size(); ++tid) {
			const iTextMgr::iTextEntry& entry = txtMgr.GetEntry(textKeyList[tid].value);
			iStringT orig = entry.text[GLNG_ENGLISH]; orig.Replace(_T("\""),_T("@")); orig.Replace(_T("@"),_T("\\\"")); 
			iStringT t_rus = entry.text[GLNG_RUSSIAN]; t_rus.Replace(_T("\""),_T("@")); t_rus.Replace(_T("@"),_T("\\\"")); 
			//iStringT t_pol = entry.text[GLNG_POLISH]; t_pol.Replace(_T("\""),_T("@")); t_pol.Replace(_T("@"),_T("\\\"")); 
			fprintf(flng_eng,"\tL\"%s\"", CvtT2A<>(orig.CStr()));
			fprintf(flng_rus,"\tL\"%s\"", CvtT2A<>(t_rus.CStr()));
			//fprintf(flng_pol,"\tL\"%s\"", CvtT2A<>(t_pol.CStr()));
			if (tid != textKeyList.Size() - 1) {
				fprintf(flng_eng,",");
				fprintf(flng_rus,",");
				//fprintf(flng_pol,",");
			}
			fprintf(flng_eng,"\n");
			fprintf(flng_rus,"\n");
			//fprintf(flng_pol,"\n");

			//txtBuf_eng.Write(entry.orig.CStr());
			//txtBuf_rus.Write(entry.trans.CStr());
			hdrText.AddDefination(iStringA(CvtT2A<>(entry.key.CStr())),entry.idx);
		}
		hdrText.Flush();

		fprintf(flng_eng,"};\n");
		fclose(flng_eng);

		fprintf(flng_rus,"};\n");
		fclose(flng_rus);

		//fprintf(flng_pol,"};\n");
		//fclose(flng_pol);

		Log(_T("Done"));
	} else Log(_T("Skipped"));
	Log(_T("\n"));

	//
	// Make constant sprites header file
	//
	Log(_T("+ Making constant sprites header file:"));
	if (bExpRes) {
		iHdrDesigner hdrSprites(GetAppPath()+_T("..\\Common\\cnst_gfx.h"),"CONSTANT_GRAPHICS_DEFINITIONS","PDGG_");
		for (uint32 zz=0; zz<spMgr.m_ConstDefList.GetSize(); ++zz) {
			const iSprite* pSprite = NULL;
			if (spMgr.m_ConstDefList[zz].m_SeqCnt == 1) {
				pSprite = spMgr.GetSpritePtr(spMgr.m_ConstDefList[zz].m_SpriteName);
			} else {
				pSprite = spMgr.GetSpritePtr(spMgr.m_ConstDefList[zz].m_SpriteName+_T("_0000"));
			}
			check(pSprite);
			hdrSprites.AddDefination(iStringA(CvtT2A<>(spMgr.m_ConstDefList[zz].m_ConstName.CStr())),(sint16)pSprite->id);
		}
		hdrSprites.Flush();
		Log(_T("Done"));
	} else Log(_T("Skipped"));
	Log(_T("\n"));

	//
	// Dump sprites
	//
	Log(_T("+ Dumping game graphics:\n"));
	DWORD dwStart, dwEnd;
	dwStart = GetTickCount();
	if (bExpRes) {
		iSortArray<SpriteId> spriteList;
		for (iSpriteMgr::ConstSpriteIt sit=spMgr.m_SpriteHash.First(); sit != spMgr.m_SpriteHash.End(); ++sit) {
			spriteList.Insert((*sit).m_Key,(*sit).m_Value.id);
		}

		GfxBankExporter gbank(*this);
		for (uint32 sid=0; sid<spriteList.Size(); ++sid){
			uint32 idx = spriteList[sid].idx;
			SpriteId ssid = spriteList[sid].value;

			check(sid == spriteList[sid].idx);
			gbank.Put(spMgr[spriteList[sid].value],spriteList[sid].value);
		}
		gbank.Export(GetAppPath()+_T("Game\\Data\\game.gfx"));
	} else Log(_T("Skipped"));
	dwEnd = GetTickCount();
	Log(iFormat(_T("%.2f sec"), (dwEnd-dwStart)/1000.0f));
	Log(_T("\n"));

	//
	// Make constant sprites header file
	//
	Log(_T("+ Making constant sprites header file:"));
	if (bExpRes) {
		iHdrDesigner hdrSounds(GetAppPath()+_T("..\\Common\\cnst_sfx.h"),"CONSTANT_SOUNDS_DEFINITIONS","CSND_");
		for (uint32 zz=0; zz<sndMgr.Count(); ++zz) {
			hdrSounds.AddDefination(iStringA(CvtT2A<>(sndMgr.Item(zz).constId.CStr())),(sint16)zz);
		}
		hdrSounds.Flush();
		Log(_T("Done"));
	} else Log(_T("Skipped"));
	Log(_T("\n"));

	//
	// Dump sounds
	//
	Log(_T("+ Dumping game sounds:\n"));
	if (bExpRes) {
		iSmartPtr<iFileI> sfxData(CreateWin32File(GetAppPath()+_T("Game\\Data\\game.sfx")));
		uint32 count = sndMgr.Count();
		sfxData->Write(&count, sizeof(uint32));
		for (uint32 zz=0; zz<count; ++zz) {
			iSmartPtr<iFileI> pcmFile(OpenWin32File(sndMgr.Item(zz).fileName));
			if (pcmFile) {
				// read pcm data
				uint32 pcmLen = pcmFile->GetSize();
				sint8* pcmBuff = (sint8*)malloc(pcmLen);
				pcmFile->Read(pcmBuff, pcmLen);
				// encode
				void* xauBuff = malloc(pcmLen);
				size_t xauLen = xau_pcm8m_encode( pcmBuff, pcmLen, xauBuff, pcmLen);
				// write pcm data length
				sfxData->Write(&pcmLen, sizeof(pcmLen));
				// write xau data length
				sfxData->Write(&xauLen, sizeof(xauLen));
				// write xau buffer
				sfxData->Write(xauBuff, xauLen);
			} else {
				Log(_T("ERROR!!!"));
				break;
			}
		}
	} else Log(_T("Skipped"));
	Log(_T("\n"));

	//
	// Dump avatars and object prototypes
	//
	Log(_T("+ Dumping avatars and object prototypes:\n"));
	if (bExpRes) {
		iDynamicBuffer obuff;
		// Header and version
		// sorry, no hmac on this file

		// Prepare avatar list
		iSortArray<AvatarId> avatarList;
		for (iAvatarMgr::ConstAvatarIt ait=avMgr.m_AvatarHash.First(); ait != avMgr.m_AvatarHash.End(); ++ait) {
			avatarList.Insert((*ait).m_Key,(*ait).m_Value.Idx());
		}
		// Write avatar list
		obuff.Write((uint16)avatarList.Size());
		for (uint32 aid=0; aid<avatarList.Size(); ++aid){
			check(aid == avatarList[aid].idx);
			const iAvatar& avt = avMgr[avatarList[aid].value];

			// Name
			sint16 nkey=-1;
			if (avt.HasName()) nkey = txtMgr.GetEntry(avt.NameKey()).idx;
			obuff.Write(nkey);
			// Sound
			if (avt.GetSoundId().Empty()) {
				obuff.Write((uint16)0xFFFF);
			} else {
				sint32 sndId = sndMgr.FindItem(avt.GetSoundId());
				check(sndId != -1);
				obuff.Write((uint16)sndId);
			}
			// Sprites
			obuff.Write((uint8)avt.SpritesCount());
			for (uint32 asid=0; asid<avt.SpritesCount(); ++asid) {
				obuff.Write((uint16)spMgr[avt.SpriteEntry(asid).spriteId].id);
				obuff.Write((uint8)avt.SpriteEntry(asid).level);
				Serialize(obuff,avt.SpriteEntry(asid).anchor);
			}
			// Pass mods
			obuff.Write((uint8)avt.PassModsCount());
			for (uint32 apid=0; apid<avt.PassModsCount(); ++apid) {
				obuff.Write((sint8)avt.PassModEntry(apid).mod);
				Serialize(obuff,avt.PassModEntry(apid).offset);
			}
			// Flags
			obuff.Write((uint8)avt.FlagsCount());
			for (uint32 fid=0; fid<avt.FlagsCount(); ++fid) {
				Serialize(obuff,avt.FlagEntry(fid));
			}

		}

		// Heroes
		obuff.Write((uint16)gItemMgr.m_pHeroesNode->Count());
		for (iObjGroupNode::ConstIterator hit=gItemMgr.m_pHeroesNode->First(); hit !=gItemMgr.m_pHeroesNode->End(); ++hit) {
			iHeroNode *pHeroNode = (iHeroNode*)(*hit);
			// Type/Nation
			obuff.Write((uint8)pHeroNode->Type());
			// Icons
			obuff.Write((uint16)spMgr[pHeroNode->SmallIcon()].id);
			obuff.Write((uint16)spMgr[pHeroNode->BigIcon()].id);
			// Name Key
			obuff.Write(txtMgr.GetEntry(pHeroNode->NameKey()).idx);
			// Initial FurtSkills
			Serialize(obuff, pHeroNode->FurtSkills());
			// Initial Secondary skills
			Serialize(obuff, pHeroNode->SecSkills());
			// Initial spells
			Serialize(obuff, pHeroNode->Spells());
			// Cost
			Serialize(obuff, pHeroNode->Cost());
		}

		// Path elements
		obuff.Write((uint16)gItemMgr.m_pPathElNode->Count());
		for (iObjGroupNode::ConstIterator peit=gItemMgr.m_pPathElNode->First(); peit !=gItemMgr.m_pPathElNode->End(); ++peit) {
			iPathElNode* pPathElNode = (iPathElNode*)(*peit);
			obuff.Write((uint8)pPathElNode->Type());
			obuff.Write(txtMgr.GetEntry(pPathElNode->NameKey()).idx);
			obuff.Write((uint16)spMgr[pPathElNode->SpriteConfig(0)].id);
			obuff.Write((uint8)pPathElNode->Level());
			obuff.Write((sint8)pPathElNode->PassModVal());
		}
		
		// Visitables
		obuff.Write((uint16)gItemMgr.m_pVisCnstNode->Count());
		for (iObjGroupNode::ConstIterator vcit=gItemMgr.m_pVisCnstNode->First(); vcit !=gItemMgr.m_pVisCnstNode->End(); ++vcit) {
			iBaseVisCnstNode *pVisCnstNode = (iBaseVisCnstNode*)(*vcit);
			pVisCnstNode->Serialize(obuff);
			ExportCovers(obuff,*pVisCnstNode);
		}

		// Ownerables
		obuff.Write((uint16)gItemMgr.m_pOwnCnstNode->Count());
		for (iObjGroupNode::ConstIterator ocit=gItemMgr.m_pOwnCnstNode->First(); ocit !=gItemMgr.m_pOwnCnstNode->End(); ++ocit) {
			iBaseOwnCnstNode *pOwnCnstNode = (iBaseOwnCnstNode*)(*ocit);
			pOwnCnstNode->Serialize(obuff);
			ExportCovers(obuff,*pOwnCnstNode);
		}

		// Castles
		uint16 cpCnt=0;
		iDynamicBuffer cpbuff;
		iObjGroupNode::ConstIterator cit=gItemMgr.m_pCastleNode->First();
		while( cit !=gItemMgr.m_pCastleNode->End()) {
			iCastleNode *pCastleNode = DynamicCast<iCastleNode*>((iBaseNode*)(*cit));
			if (pCastleNode) {
				cpCnt++;
				cpbuff.Write((uint8)pCastleNode->Type());
				cpbuff.Write((uint8)pCastleNode->Size());
				cpbuff.Write((uint8)pCastleNode->Orient());
				cpbuff.Write(pCastleNode->Config());
				ExportCovers(cpbuff,*pCastleNode);
			}
			cit = cit.WalkNext();
		}
		obuff.Write(cpCnt);
		obuff.Write(cpbuff.GetData(),cpbuff.GetSize());

		// Artifacts
		obuff.Write((uint16)(gItemMgr.m_pArtifactNode->Count()-5));
		for (iObjGroupNode::ConstIterator arit=gItemMgr.m_pArtifactNode->First(); arit !=gItemMgr.m_pArtifactNode->End(); ++arit) {
			iArtifactNode *pArtifactNode = (iArtifactNode*)(*arit);
			if (pArtifactNode->IsRandom()) continue;
			obuff.Write(txtMgr.GetEntry(pArtifactNode->NameKey()).idx);
			obuff.Write(txtMgr.GetEntry(pArtifactNode->DescKey()).idx);
			obuff.Write((uint16)spMgr[pArtifactNode->Sprite()].id);
			pArtifactNode->Serialize(obuff);
		}

		// Combat obstacles
		obuff.Write((uint16)gItemMgr.m_battleObstacles.GetSize());
		for (uint32 obstId=0; obstId<gItemMgr.m_battleObstacles.GetSize(); ++obstId) {
			obuff.Write(gItemMgr.m_battleObstacles[obstId].m_surfMask);
			obuff.Write((uint16)spMgr[gItemMgr.m_battleObstacles[obstId].m_sprite].id);
			Serialize(obuff, gItemMgr.m_battleObstacles[obstId].m_offset);
			obuff.Write(gItemMgr.m_battleObstacles[obstId].m_size);
			obuff.Write(gItemMgr.m_battleObstacles[obstId].m_level);
		}

		// Write Credits
		uint16 crCnt = sizeof(thetext) / sizeof(LPCTSTR);
		obuff.Write(crCnt);
		for (uint32 nn=0; nn<crCnt; ++nn) {
			obuff.Write(iStringT(thetext[nn]));
		}

		// Compress
		iDynamicBuffer cmpDataBuff;
		LZO::Compress((const unsigned char*)obuff.GetData(), obuff.GetSize(), cmpDataBuff);

		// Store
		iSmartPtr<iFileI> opf(CreateWin32File(GetAppPath()+_T("Game\\Data\\objects.dat")));

		opf->Write(&GOBJ_FILE_HDR_KEY, sizeof(GOBJ_FILE_HDR_KEY));
		opf->Write(&GOBJ_FILE_VERSION, sizeof(GOBJ_FILE_VERSION));

		opf->Write(cmpDataBuff.GetData(),cmpDataBuff.GetSize());
		Log(iFormat(_T("- Done: %d bytes\n"),cmpDataBuff.GetSize()));

	} else Log(_T("Skipped"));
	Log(_T("\n"));


	//
	// Dump map(s)
	//
	for (uint32 mapId = 0; mapId < m_mapFileList.GetSize(); ++mapId) {
		gItemMgr.Reset();
		iMapHandler map;
		if (!LoadMapFromFile(map, m_mapFileList[mapId])) {
			Log(iFormat(_T("! Unable to open %s!\n"),m_mapFileList[mapId]));
			continue;
		}
		
		iStringT res;
		if(!map.ValidateMap(res, false)) {		
			Log(iFormat(_T("ERROR: Map %s invalid! Fix the following first:\n"), map.m_FileName));
			Log(res);
			continue;
		}


		// Supported languages
		iSimpleArray<uint8> lngs;
		for (uint32 nn=0; nn<GLNG_COUNT; ++nn) {
			if ( (map.m_lngMask & (1<<nn)) ) lngs.Add((uint8)nn);
		}

		// Dump loaded map with specified language
		for (uint32 lng_id=0; lng_id<lngs.GetSize(); ++lng_id) {

			Log(iFormat(_T("+ Dumping map %s (%s):\n"),MakeTextEntryName(TET_MAP_NAME,NULL).CStr(),map.m_FileName));
			iDynamicBuffer mapInfoBuff, mapDataBuf;

			GAME_LANG lng = (GAME_LANG)lngs[lng_id];

			// Fourcc (4 bytes)
			mapInfoBuff.Write(GMAP_FILE_HDR_KEY);

			// Map version (2 bytes)
			mapInfoBuff.Write(GMAP_FILE_VERSION);

			// Save timestamp (0 for new game)
			mapInfoBuff.Write((uint32)0);

			// Random seed (4 bytes) - set to zero
			mapInfoBuff.Write((uint32)0);

			// Map size (1 byte)
			mapInfoBuff.Write((uint8)map.m_Siz);

			// Map name and description (string of text)
			Serialize(mapInfoBuff, map.m_ctMgr.GetText(MakeTextEntryName(TET_MAP_NAME,NULL), lng));
			Serialize(mapInfoBuff, map.m_ctMgr.GetText(MakeTextEntryName(TET_MAP_DESC,NULL), lng));

			// Map version and author
			mapInfoBuff.Write(map.m_MapVersion);
			mapInfoBuff.Write(map.m_MapAuthor);

			// Current date
			mapInfoBuff.Write((uint32)1);

			// Game mode 
			mapInfoBuff.Write((uint16)0);

			// Difficulty
			mapInfoBuff.Write((sint8)DFC_UNDEFINED);

			// Player configuration
			iSimpleArray<iPlayer*> pPlayers;
			for (uint32 pid=PID_RED; pid<PID_COUNT; ++pid) {
				if (map.m_pPlayers[pid]) {
					// Add player
					pPlayers.Add(map.m_pPlayers[pid]);
				}
			}
			mapInfoBuff.Write((uint16)pPlayers.GetSize());
			for (pid = 0; pid<pPlayers.GetSize(); ++pid){
				// PlayerId
				mapInfoBuff.Write((sint8)pPlayers[pid]->m_PlayerId);
				// Nation type and main castle
				if (pPlayers[pid]->m_pMainCastle) {
					// nation of main castle
					mapInfoBuff.Write((uint8)pPlayers[pid]->m_pMainCastle->Proto()->Type());
				} else {
					iHero* pMainHero = NULL;
					for (uint32 hid = 0; hid<map.m_HeroList.GetSize(); ++hid){
						if (map.m_HeroList[hid]->Owner() == pPlayers[pid]->m_PlayerId) {
							pMainHero = map.m_HeroList[hid];
							break;
						}
					}
					// player has no castle and no hero ?
					check(pMainHero);
					// nation depends on main hero
					mapInfoBuff.Write((uint8)(pMainHero->Proto()->Type()));
				}


				// Player Type Mask (not used for saved games)
				mapInfoBuff.Write((sint8)pPlayers[pid]->m_PlayerTypeMask);
				// Player Type (PT_UNDEFINED for new game)
				mapInfoBuff.Write((sint8)PT_UNDEFINED);
				// Player resources (undefined for new game)
				iMineralSet ms; ms.Reset();
				Serialize(mapInfoBuff, ms);
				// Current Hero idx (0xFFFF = undefined for new game)
				mapInfoBuff.Write((uint16)0xFFFF);
				// Current Castle idx (0xFFFF = undefined for new game)
				mapInfoBuff.Write((uint16)0xFFFF);
				// Keys
				mapInfoBuff.Write((uint8)0);
			}

			// Current player Id (0xFFFF = undefined for new game)
			mapInfoBuff.Write((uint16)0xFFFF);

			// Map metrics
			mapInfoBuff.Write((uint16)map.GetWidth());
			mapInfoBuff.Write((uint16)map.GetHeight());

			// Time events
			uint16 cecnt = map.m_timeEventMgr.EventsCount();
			mapDataBuf.Write(cecnt);
			for (uint32 xx=0; xx<cecnt; ++xx){
				const iTimeEvent& evt = map.m_timeEventMgr.Event(xx);
				if (!evt.m_text.Empty()) {
					Serialize(mapDataBuf, map.m_ctMgr.GetText(evt.m_text, lng));
				} else Serialize(mapDataBuf, iStringT());
				mapDataBuf.Write(evt.m_playerMask);
				mapDataBuf.Write(evt.m_fTime);
				mapDataBuf.Write((sint32)evt.m_repTime);
				::Serialize(mapDataBuf, evt.m_minerals);
			}

			// Surface and avatars
			ExportMap(mapDataBuf, map);

			// Fog maps and puzlle cards
			for (pid = 0; pid<pPlayers.GetSize(); ++pid) {
				// Puzzle cards
				mapDataBuf.Write((uint16)0xFFFF);
				// Fog map
				mapDataBuf.Write((uint32)0);
			}
			// Visitables
			mapDataBuf.Write((uint16)map.m_VisCnstList.GetSize());
			uint16 obelisksCount = 0;
			for (uint32 vcid=0; vcid<map.m_VisCnstList.GetSize(); ++vcid){
				// If it obelisk, then increase the counter's value
				if (map.m_VisCnstList[vcid]->Proto()->Type() == VCNST_OBELISK) obelisksCount++;
				// Serialize
				mapDataBuf.Write((uint16)map.m_VisCnstList[vcid]->Proto()->Idx());
				Serialize(mapDataBuf,map.m_VisCnstList[vcid]->Pos());
				Serialize(mapDataBuf,map.m_VisCnstList[vcid]->Guard());
				if (!map.m_VisCnstList[vcid]->CustomMessage().Empty()) {
					Serialize(mapDataBuf, map.m_ctMgr.GetText(map.m_VisCnstList[vcid]->CustomMessage(), lng));
				} else Serialize(mapDataBuf, iStringT());
				mapDataBuf.Write((uint16)0);	// visitors
				mapDataBuf.Write((uint8)0);		// ext data flag
			}
			// Ownerables
			mapDataBuf.Write((uint16)map.m_OwnCnstList.GetSize());
			for (uint32 ocid=0; ocid<map.m_OwnCnstList.GetSize(); ++ocid){
				mapDataBuf.Write((uint16)map.m_OwnCnstList[ocid]->Proto()->Idx());
				mapDataBuf.Write((sint8)map.m_OwnCnstList[ocid]->Owner());
				Serialize(mapDataBuf,map.m_OwnCnstList[ocid]->Pos());
				Serialize(mapDataBuf,map.m_OwnCnstList[ocid]->Guard());
			}
			// Castles
			mapDataBuf.Write((uint16)map.m_CastleList.GetSize());
			for (uint32 cid=0; cid<map.m_CastleList.GetSize(); ++cid){
				iCastle* pCastle = map.m_CastleList[cid];
				// Proto Id
				mapDataBuf.Write((uint16)pCastle->Proto()->Idx());
				// Custom name
				if (!pCastle->CustomName().Empty()) {
					Serialize(mapDataBuf, map.m_ctMgr.GetText(pCastle->CustomName(), lng));
				} else Serialize(mapDataBuf, iStringT());
				// Owner Id
				mapDataBuf.Write((sint8)pCastle->Owner());
				// Position
				Serialize(mapDataBuf,pCastle->Pos());
				// Main castle and create hero flags
				uint8 flags = 0;
				if (pCastle->Owner() != PID_NEUTRAL && map.m_pPlayers[pCastle->Owner()]->m_pMainCastle == pCastle) {
					flags = 0x1;
					if (map.m_pPlayers[pCastle->Owner()]->m_bCreateHero) flags |= 0x2;
				}
				mapDataBuf.Write(flags);
				// Garrison
				Serialize(mapDataBuf,pCastle->Garrison());
				// Can build
				mapDataBuf.Write((uint8)1);
				// Constructions
				uint8 flag = pCastle->CtlCnsts().IsCustomized()?1:0;
				mapDataBuf.Write(flag); // constructions initialization flag (0 - default, 1 - customized, 2 - customized with serialized data)
				if (flag == 1) {
					uint16 cnt = 0;
					for (uint32 xx=0; xx<pCastle->CtlCnsts().Count(); ++xx) if (pCastle->CtlCnsts().Entry(xx).state != 0) ++cnt;
					mapDataBuf.Write(cnt);
					for (uint32 ctlCnstId = 0; ctlCnstId < pCastle->CtlCnsts().Count(); ++ctlCnstId) {
						if (pCastle->CtlCnsts().Entry(ctlCnstId).state != 0) {
							mapDataBuf.Write((uint16)ctlCnstId);
							mapDataBuf.Write((uint8)pCastle->CtlCnsts().Entry(ctlCnstId).state);
						}
					}
				}
			}

			// Ammount of obelisks (0) means no Ultimate artifact ?
			mapDataBuf.Write(obelisksCount);

			// Ultimate artifact position and radius
			::Serialize(mapDataBuf, map.m_posUltimateArt);
			mapDataBuf.Write((uint8)map.m_radUltimateArt);
			mapDataBuf.Write((uint8)0);

			// Map Items
			mapDataBuf.Write((uint16)map.m_MapItemList.GetSize());
			for (uint32 mid=0; mid<map.m_MapItemList.GetSize(); ++mid) {
				iMapItem* pItem = map.m_MapItemList[mid];
				mapDataBuf.Write((uint8)pItem->Type());
				Serialize(mapDataBuf, pItem->Pos());
				Serialize(mapDataBuf, pItem->Guard());
				if (!pItem->Message().Empty()) {
					Serialize(mapDataBuf, map.m_ctMgr.GetText(pItem->Message(), lng));
				} else Serialize(mapDataBuf, iStringT());

				if (pItem->Type() == MAPITEM_MINERAL) {
					iMapItem_Mineral* pMineral = (iMapItem_Mineral*)pItem;
					mapDataBuf.Write((sint8)pMineral->MineralType());
					mapDataBuf.Write((sint32)pMineral->MineralCount());
				} else if (pItem->Type() == MAPITEM_MANACRYSTAL) {
					iMapItem_ManaCrystal* pManaCrystal = (iMapItem_ManaCrystal*) pItem;
					mapDataBuf.Write((sint32)pManaCrystal->Count());
				} else if (pItem->Type() == MAPITEM_CAMPFIRE) {
					mapDataBuf.Write((sint32)RAND_VAL);
					mapDataBuf.Write((sint8)MINERAL_UNKNOWN);
					mapDataBuf.Write((sint32)RAND_VAL);
				} else if (pItem->Type() == MAPITEM_CHEST) {
					mapDataBuf.Write((sint32)RAND_VAL);
				} else if (pItem->Type() == MAPITEM_LAMP) {
					mapDataBuf.Write((sint32)RAND_VAL);
				} else if (pItem->Type() == MAPITEM_ARTIFACT) {
					iMapItem_Artifact* pArtifact = (iMapItem_Artifact*)pItem;
					mapDataBuf.Write(pArtifact->Proto()->Idx());
				} else if (pItem->Type() == MAPITEM_KEYGUARD) {
					iMapItem_KeyGuard* pKeyGuard = (iMapItem_KeyGuard*)pItem;
					mapDataBuf.Write(pKeyGuard->GetKeyType());
				} else if (pItem->Type() == MAPITEM_SPELLSCROLL) {
					iMapItem_SpellScroll* pSpellScroll = (iMapItem_SpellScroll*)pItem;
					mapDataBuf.Write((uint16)pSpellScroll->SpellId());
				}
			}

			// Map events
			mapDataBuf.Write((uint16)map.m_MapEventList.GetSize());
			for (uint32 meid=0; meid<map.m_MapEventList.GetSize(); ++meid) {
				iMapEvent* pEvent = map.m_MapEventList[meid];
				Serialize(mapDataBuf,pEvent->Pos());
				Serialize(mapDataBuf,pEvent->Guard());
				if (!pEvent->RewardMessage().Empty()) {
					Serialize(mapDataBuf, map.m_ctMgr.GetText(pEvent->RewardMessage(), lng));
				} else Serialize(mapDataBuf, iStringT());
				if (!pEvent->AttackMessage().Empty()) {
					Serialize(mapDataBuf, map.m_ctMgr.GetText(pEvent->AttackMessage(), lng));
				} else Serialize(mapDataBuf, iStringT());
				mapDataBuf.Write(pEvent->PlayerMask());
				Serialize(mapDataBuf,pEvent->Rewards());
			}

			// Map guards
			mapDataBuf.Write((uint16)map.m_MapGuardList.GetSize());
			for (uint32 gid=0; gid<map.m_MapGuardList.GetSize(); ++gid) {
				iMapGuard* pGuard = map.m_MapGuardList[gid];
				Serialize(mapDataBuf, pGuard->Creatures());
				mapDataBuf.Write((sint8)RAND_VAL); // join value
				mapDataBuf.Write((uint8)pGuard->Disposition());
				mapDataBuf.Write((uint8)pGuard->NotGrow());
				Serialize(mapDataBuf,pGuard->Pos());
				if (!pGuard->Message().Empty()) {
					Serialize(mapDataBuf, map.m_ctMgr.GetText(pGuard->Message(), lng));
				} else Serialize(mapDataBuf, iStringT());
			}
			// Heroes
			mapDataBuf.Write((uint16)map.m_HeroList.GetSize());
			for (uint32 hid = 0; hid<map.m_HeroList.GetSize(); ++hid){
				iHero* pHero = map.m_HeroList[hid];
				// Proto Id
				mapDataBuf.Write((uint16)pHero->Proto()->Idx());
				// Owner Id
				mapDataBuf.Write((sint8)pHero->Owner());
				// Position
				Serialize(mapDataBuf,pHero->Pos());
				// Angle
				mapDataBuf.Write((sint8)-1);
				// Target
				Serialize(mapDataBuf,pHero->Pos());
				// Custom name
				if (!pHero->CustomName().Empty()) {
					Serialize(mapDataBuf, map.m_ctMgr.GetText(pHero->CustomName(), lng));
				} else Serialize(mapDataBuf, iStringT());
				// Level
				mapDataBuf.Write(pHero->Level());
				// Experience
				mapDataBuf.Write(pHero->Experience());
				// Secondary skills
				Serialize(mapDataBuf, pHero->SecSkills());
				// Artifacts list
				Serialize(mapDataBuf, pHero->Artifacts());
				// Enchancement list
				mapDataBuf.Write((uint16)0);
				// FurtSkills
				Serialize(mapDataBuf, pHero->FurtSkills());
				// Army
				Serialize(mapDataBuf,pHero->Army());
				// Spellbook
				if (pHero->IsCustomSpells()) {
					mapDataBuf.Write((uint8)1);
					Serialize(mapDataBuf, pHero->Spells());
				} else mapDataBuf.Write((uint8)0);
				// Favorite spells
				mapDataBuf.Write((uint8)0);
				// Mana points
				mapDataBuf.Write((sint32)-1);
				// Action points
				mapDataBuf.Write((sint32)-1);
				// CanDig flag
				mapDataBuf.Write((uint8)1);
			}

			// Hero manager (empty for new map)
			mapDataBuf.Write((uint16)0);

			// Store
			iStringT map_fname;
			if (m_mapFileList.GetSize() == 1) {
				map_fname = LI::GetDlgItemText(m_hWnd, IDC_MAP_FILENAME);
			} else {
				sint32 extp = m_mapFileList[mapId].ReverseFind('.');
				sint32 slp = m_mapFileList[mapId].ReverseFind('\\') + 1;
				check(extp > slp);
				map_fname = m_mapFileList[mapId].Mid(slp, extp - slp);
			}
			// Create file
			// WARN:: Since the current gx lib is largely incompatible with the iolib
			// we should dos some work manually
			iStringT mapFileName = GetAppPath()+_T("Game\\Maps\\")+map_fname+_T("_")+LANG_NAME[lng]+_T(".phm");
			iSmartPtr<iFileI> mlf(CreateWin32File( mapFileName ));
			// Write header
			mlf->Write(mapInfoBuff.GetData(),mapInfoBuff.GetSize());
			// Compress data
			iDynamicBuffer cmpDataBuff;
			LZO::Compress((const unsigned char*)mapDataBuf.GetData(), mapDataBuf.GetSize(), cmpDataBuff);
			// Write data
			mlf->Write(cmpDataBuff.GetData(),cmpDataBuff.GetSize());

			Log(iFormat(_T("- Done: %d bytes (compressed to %d bytes)\n"),mapInfoBuff.GetSize()+mapDataBuf.GetSize(), mapInfoBuff.GetSize()+cmpDataBuff.GetSize()));
			Log(_T("\n"));
		}


		/*
		// Map schema
		iDib dibSchema;
		iMinimapView::PrepareMap(m_pMap, dibSchema, true);
		if (dibSchema.GetSize().w < 256) {
			uint32 factor = 256 / dibSchema.GetSize().w;
			check(factor>1);
			iDibTransformer::MultiplySize(dibSchema, factor);
		} else if (dibSchema.GetSize().w > 256) {
			uint32 factor = dibSchema.GetSize().w / 256;
			check(factor>1);
			iDib src(dibSchema);
			dibSchema.Resize(iSize(256,256));
			iDibTransformer::Stretch(src, dibSchema, iDibTransformer::Pyramidal);
		}

		iDibSaver::SavePNG(dibSchema, GetAppPath()+_T("Game\\Maps\\")+map_fname+_T(".png"));
		iDib thumb(iSize(64,64));
		iDibTransformer::Stretch(dibSchema, thumb, iDibTransformer::Pyramidal);
		iDibSaver::SavePNG(thumb, GetAppPath()+_T("Game\\Maps\\")+map_fname+_T("_th.png"));
		*/
	}

	gItemMgr.Reset();
	return true;
}

/*
 *	
 */
struct  iMapCell {
	iMapCell(uint16 _surf=0, uint16 _avatar = 0xFFFF) 
	: surf(_surf), avatar(_avatar) {}
	uint16 surf;
	uint16 avatar;
};
typedef iMapT<iMapCell> iGameMap;
bool iExportDlg::ExportMap(iDynamicBuffer& buff, const iMapHandler& map)
{
	iGameMap gmap(map.GetWidth(),map.GetHeight());

	// Surface and decorations
	for (sint32 yy=0; yy<map.GetHeight(); ++yy) {
		for (sint32 xx=0; xx<map.GetWidth(); ++xx) {
			uint8 n1 = map.m_Nodes.GetCell(xx,yy).surf;
			uint8 n2 = map.m_Nodes.GetCell(xx+1,yy).surf;
			uint8 n3 = map.m_Nodes.GetCell(xx+1,yy+1).surf;
			uint8 n4 = map.m_Nodes.GetCell(xx,yy+1).surf;
			gmap.At(xx,yy).surf =  (n1<<12) | (n2<<8) | (n3<<4) | (n4);

			if (map.At(xx,yy).pDecor) {
				//if (gmap.At(xx,yy).avatar != 0xFFFF) Log(iFormat(_T("- Warning: cell %d,%d already obtained!\n"),xx,yy));
				gmap.At(xx,yy).avatar = (uint16)map.At(xx,yy).pDecor->Idx();
			} else if (map.At(xx,yy).pPathEl) {
				gmap.At(xx,yy).avatar = (uint16)(0xFF00 | map.At(xx,yy).pPathEl->Idx());
			}
		}
	}

	buff.Write(gmap.GetPtr(),gmap.GetWidth()*gmap.GetHeight()*sizeof(iMapCell));

	return true;
}

//////////////////////////////////////////////////////////////////////////
void iExportDlg::ExportCovers(iDynamicBuffer& buff, const iVisualObject& vol)
{
	// store avatars instead of cover list
	const iAvatarMgr &avMgr = gItemMgr.m_AvatarMgr;
	buff.Write((uint16)vol.AvatarsCount());
	for (uint32 xx=0; xx<vol.AvatarsCount(); ++xx) {
		buff.Write((uint16)avMgr[vol.Avatar(xx).id].Idx());
		buff.Write((sint16)vol.Avatar(xx).offset.x);
		buff.Write((sint16)vol.Avatar(xx).offset.y);
	}
}

LRESULT iExportDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CheckDlgButton(IDC_EXPRES, FALSE);

#if !defined (HMM_DEV_EXPORT)
	::EnableWindow(GetDlgItem(IDC_EXPRES), FALSE);
#endif

	check (m_mapFileList.GetSize());
	if (m_mapFileList.GetSize() == 1) {
		// single file mode
		sint32 extp = m_mapFileList[0].ReverseFind('.');
		sint32 slp = m_mapFileList[0].ReverseFind('\\') + 1;
		check(extp > slp);
		iStringT fname = m_mapFileList[0].Mid(slp, extp - slp);
		LI::SetDlgItemText(m_hWnd, IDC_MAP_FILENAME, fname);
	} else {
		// multiple files mode
		LI::SetDlgItemText(m_hWnd, IDC_MAP_FILENAME, _T("n/a"));
		::EnableWindow(GetDlgItem(IDC_MAP_FILENAME), FALSE);
	}

	UpdateControls(false);
	return TRUE;
}

LRESULT iExportDlg::OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	Export();
	CButton(GetDlgItem(IDOK)).EnableWindow(FALSE);
	return TRUE;
}

LRESULT iExportDlg::OnExpResCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UpdateControls(false);
	return TRUE;
}

void iExportDlg::UpdateControls(bool bRun)
{
#if defined (HMM_DEV_EXPORT) || defined (HMM_JIM_VERSION)
	::EnableWindow(GetDlgItem(IDC_ENCRYPTION), !bRun );
#	if defined (HMM_DEV_EXPORT)
	::EnableWindow(GetDlgItem(IDC_EXPRES), !bRun );
#	endif
#endif
}
