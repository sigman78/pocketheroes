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


/*
 *	
 */
void iSprite::Init(const iDib& _dib, const iPoint& _anchor, bool _spanned, bool _crop, uint8 _shadow, const iRect& rc)
{
	id = -1;
	anchor = _anchor;
	spanned = _spanned;
	crop = _crop;
	shadow = _shadow;
	if (rc == cInvalidRect) {
		dib.Create(&_dib);
	} else {
		dib.Create(&_dib,rc);
	}
}

/*
 *	
 */
class iSpriteXmlReader : public Sax::PathHandler
{
public:
	iSpriteXmlReader(iSpriteMgr& spriteMgr)
	: m_SpriteMgr(spriteMgr)
	{}

	bool OnEndDocument()
	{
		return true;
	}

	bool OnEnterElement( const iStringT& path, const Attributes& attr )
	{
		if (path == _T("HMMSpriteSet")) {
		} else if (path == _T("HMMSpriteSet.Group")) {
			m_GroupName = attr[_T("id")];
		} else if (path == _T("HMMSpriteSet.Group.Item")) {
			uint8 shadow = 0;
			if (attr.Exists(_T("shadow"))) shadow = (uint8)attr[_T("shadow")].GetUInt();
			iPoint seq(1,1);
			if (attr.Exists(_T("sequence"))) seq = ParsePnt( attr[_T("sequence")]);
			bool spanned=true;
			if (attr.Exists(_T("spanned")) && !attr[_T("spanned")].GetUInt() ) spanned = false;
			if (!m_SpriteMgr.AddSprite(m_GroupName + _T(".") + attr[_T("id")], attr[_T("chunk")], ParsePnt( attr[_T("anchor")]), spanned, attr[_T("crop")].GetUInt() == 1, shadow, seq.x, seq.y, attr[_T("constId")] )) return false;
		}

		return true;
	}
	
	iStringT		m_GroupName;
	iSpriteMgr&		m_SpriteMgr;
};

bool IsFirstIsMoreRecentThanSecond( const iStringT& first, const iStringT& second )
{
#pragma message("TODO: Write me!")
	return true;	// always rebuild cache for now
}

/*
 *	
 */
bool iSpriteMgr::Init(const iStringT& fname)
{
	try {

		/*iStringT cachePath = _T("c:\\__spritecache__");

		if ( iFile::Exists(cachePath) && !IsFirstIsMoreRecentThanSecond( fname, cachePath ) ) {
			iSmartPtr<iFileI> df2( OpenWin32File( cachePath ) );
			if ( df2 ) return InitFromCache( df2.Get() );
		}*/

		iFileName fn(fname);
		m_ResPath = fn.GetDevPath();
		// Load sprites
		iSpriteXmlReader reader(*this);
		if (!Sax::Parse( fname, &reader )) throw 0;
		
		// Normalize indeces
		uint32 constCount=0;
		for (uint32 xx=0; xx<m_ConstDefList.GetSize(); ++xx){
			if (m_ConstDefList[xx].m_SeqCnt == 1){
				operator[](m_ConstDefList[xx].m_SpriteName).id=constCount;
				constCount++;
			} else {
				for (uint32 zz=0; zz<m_ConstDefList[xx].m_SeqCnt; ++zz){
					iStringT nname = iFormat(_T("%s_%04d"),m_ConstDefList[xx].m_SpriteName.CStr(),zz);
					operator[](nname).id=constCount;
					constCount++;
				}
			}
		}

		uint32 idCounter = constCount;
		for (SpriteIt sit = m_SpriteHash.First(); sit != m_SpriteHash.End(); ++sit){
			if ( (*sit).m_Value.id == -1 ) {
				(*sit).m_Value.id = idCounter;
				idCounter++;
			}
		}

		OutputDebugString(iFormat(_T("Normilized %d sprites (%d const entries)\n"),idCounter,constCount).CStr());

		//iSmartPtr<iFileI> df( CreateWin32File( cachePath ) );
		//SaveCache( df.Get() );

	} catch ( int ) {
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
//
//

struct SerializeError {};

template< typename Except >
bool Throw()
{ throw Except(); return true; }

#define DECL_SERIALIZABLE( type ) \
void Serialize( iFileI* to, type v ) { \
	file.Write( to, v ) || Throw<SerializeError>(); \
} \
/* */

template< typename T >
void Serialize( iFileI* to, const T& v )
{
	iFileHandle file( to );
	file.Write( v ) || Throw<SerializeError>();
}

template< typename T >
void Deserialize( iFileI* to, T& v )
{
	iFileHandle file( to );
	file.Read( v ) || Throw<SerializeError>();
}

template< typename T >
void Serialize( iFileI* to, const T* v, uint32 num )
{
	iFileHandle file( to );
	// ?
	file.Write( v, sizeof(T) * num ) || Throw<SerializeError>();
}

template< typename T >
void Deserialize( iFileI* to, T* v, uint32 num )
{
	iFileHandle file( to );
	// ?
	file.Read( v, sizeof(T) * num ) || Throw<SerializeError>();
}

void SerializeBin( iFileI* to, const void* v, uint32 num )
{
	iFileHandle file( to );
	file.Write( v, num ) || Throw<SerializeError>();
}

void DeserializeBin( iFileI* to, void* v, uint32 num )
{
	iFileHandle file( to );
	file.Read( v, num ) || Throw<SerializeError>();
}


void Serialize( iFileI* to, const iPoint& pt )
{
	Serialize( to, pt.x );
	Serialize( to, pt.y );
}

void Deserialize( iFileI* to, iPoint& pt )
{
	Deserialize( to, pt.x );
	Deserialize( to, pt.y );
}

void Serialize( iFileI* to, const iSize sz )
{
	Serialize( to, sz.w );
	Serialize( to, sz.h );
}

void Deserialize( iFileI* to, iSize& sz )
{
	Deserialize( to, sz.w );
	Deserialize( to, sz.h );
}

void Serialize( iFileI* to, const iRect& rc )
{
	Serialize( to, (const iPoint&)rc );
	Serialize( to, (const iSize&)rc );
}

void Deserialize( iFileI* to, iRect& rc )
{
	Deserialize( to, (iPoint&)rc );
	Deserialize( to, (iSize&)rc );
}

void Serialize( iFileI* to, const iDib& dib )
{
	Serialize( to, dib.GetSize() );
	if ( ( dib.GetWidth() | dib.GetHeight() ) ) 
		SerializeBin(  to, dib.GetBits(), dib.GetByteSize() );
}

void Deserialize( iFileI* to, iDib& dib )
{
	iSize size;
	Deserialize( to, size );
	
	if ( !(size.w && size.h) ) {
		dib.Destroy();
		return;
	} 

	dib.Create( size ) || Throw<SerializeError>();
	DeserializeBin(  to, dib.GetBits(), dib.GetByteSize() );
}

template< typename HK, typename HV, typename HF  >
void Serialize( iFileI* to, const iHashTable<HK,HV,HF>& ht )
{
	typedef iHashTable<HK,HV,HF> HT;

	uint32 count = ht.Count();
	Serialize( to, count );
	for( HT::ConstIterator it = ht.First(); it != ht.End(); ++it ) {
		Serialize( to, *it );
	}		
}

template< typename HK, typename HV, typename HF  >
void Deserialize( iFileI* to, iHashTable<HK,HV,HF>& ht )
{
	typedef iHashTable<HK,HV,HF> HT;

	uint32 count;
	Deserialize( to, count );
	for( uint32 n = 0; n != count; ++n ) {
		HV val;
		Deserialize( to, val );
		ht.Insert( val );
	}
}

template< typename T >
void Serialize( iFileI* to, const iSimpleArray<T>& arr )
{
	Serialize( to, arr.GetSize() );
	for( uint32 n = 0; n != arr.GetSize(); ++n )
		Serialize( to, arr[n] );
}

template< typename T >
void Deserialize( iFileI* to, iSimpleArray<T>& arr )
{
	uint32 size;
	Deserialize( to, size );
	for( uint32 n = 0; n != size; ++n ) {
		T val;
		Deserialize( to, val );
		arr.Add( val );
	}
}

void Serialize( iFileI* to, const iSprite& sp )
{
	Serialize( to, sp.id );
	Serialize( to, sp.shadow );
	Serialize( to, sp.anchor );
	Serialize( to, sp.dib );
}

void Deserialize( iFileI* to, iSprite& sp )
{
	Deserialize( to, sp.id );
	Deserialize( to, sp.shadow );
	Deserialize( to, sp.anchor );
	Deserialize( to, sp.dib );
}

void Serialize( iFileI* to, const iConstDef& def )
{
	Serialize( to, def.m_SpriteName );
	Serialize( to, def.m_ConstName );
	Serialize( to, def.m_SeqCnt );
}

void Deserialize( iFileI* to, iConstDef& def )
{
	Deserialize( to, def.m_SpriteName );
	Deserialize( to, def.m_ConstName );
	Deserialize( to, def.m_SeqCnt );
}

void Serialize( iFileI* to, const iSpriteMgr::KeyVal & ci )
{
	Serialize( to, ci.m_Key );
	Serialize( to, ci.m_Value );
}

void Deserialize( iFileI* to, iSpriteMgr::KeyVal & ci )
{
	Deserialize( to, ci.m_Key );
	Deserialize( to, ci.m_Value );
}

bool
iSpriteMgr::InitFromCache(iFileI* to )
{
	try {
/*
		Deserialize( to, m_CellFactor );
		Deserialize( to, m_ResPath );
		Deserialize( to, m_InvalidSprite );

		Deserialize( to, m_pdggTiles );
		Deserialize( to, m_pdggTransWS );
		Deserialize( to, m_pdggTransMask );
		Deserialize( to, m_pdggGrid );
		Deserialize( to, m_pdggPathEl );
		Deserialize( to, m_pdggHeroes );
		Deserialize( to, m_pdggFlags );

		Deserialize( to, m_SpriteHash );
		Deserialize( to, m_ConstDefList );
*/
	} catch (SerializeError&) {
		return false;
	}
	return true;
}

bool
iSpriteMgr::SaveCache( iFileI* to )
{
	try {
/*
		Serialize( to, m_CellFactor );
		Serialize( to, m_ResPath );
		Serialize( to, m_InvalidSprite );

		Serialize( to, m_pdggTiles );
		Serialize( to, m_pdggTransWS );
		Serialize( to, m_pdggTransMask );
		Serialize( to, m_pdggGrid );
		Serialize( to, m_pdggPathEl );
		Serialize( to, m_pdggHeroes );
		Serialize( to, m_pdggFlags );

		Serialize( to, m_SpriteHash );
		Serialize( to, m_ConstDefList );
*/
	} catch (SerializeError&) {
		return false;
	}

	return true;
}
