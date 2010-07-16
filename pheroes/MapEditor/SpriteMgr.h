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

#ifndef _HMM_EDITOR_SPRITE_MANAGER_H_
#define _HMM_EDITOR_SPRITE_MANAGER_H_

/*
 *	
 */
struct iSprite
{
	void Init(const iDib& _dib, const iPoint& _anchor, bool _spanned, bool _crop, uint8 _shadow, const iRect& rc=cInvalidRect);

	uint32	id;
	iPoint	anchor;
	bool	spanned;
	bool	crop;
	uint8	shadow;
	iDib	dib;
};

/*
 *	
 */
struct iConstDef
{
	iConstDef() {}
	iConstDef(const SpriteId& spriteName, const iStringT& constName, uint32 seqCnt) 
	: m_SpriteName(spriteName), m_ConstName(constName), m_SeqCnt(seqCnt) {}

	SpriteId	m_SpriteName;
	iStringT	m_ConstName;
	uint32		m_SeqCnt;
};

typedef iSimpleArray<iConstDef>	iConstDefList;

/*
 *	
 */
class iSpriteMgr
{
public:
	//
	struct KeyVal {
		KeyVal(){}
		iStringT	m_Key;
		iSprite		m_Value;
		inline KeyVal( const iStringT& k) : m_Key( k ){}
	};
private:
	//
	struct KeyValHF {
		uint32 operator() ( const iStringT& str )
		{ return iTables::HashFn( str ); }
		iStringT operator() ( KeyVal& pair ) 
		{ return pair.m_Key; }
	};

	uint8		m_CellFactor;
	iStringT	m_ResPath;
	iSprite		m_InvalidSprite;

public:
	// Predefined sprites
	iDib		m_pdggTiles;
	iDib		m_pdggTransWS;
	iDib		m_pdggTransMask;
	iDib		m_pdggFog;
	iDib		m_pdggGrid;
	iDib		m_pdggPathEl;
	iDib		m_pdggHeroes;
	iDib		m_pdggFlags;

	// Predefined interface elements

	iSpriteMgr()
	{}

	typedef iHashTable< const iStringT&, KeyVal, KeyValHF > iKVHash;
	typedef iKVHash::Iterator SpriteIt;
	typedef iKVHash::ConstIterator ConstSpriteIt;
	iKVHash			m_SpriteHash;
	iConstDefList	m_ConstDefList;

	inline uint8 GetCellFactor() const
	{ return m_CellFactor;}

	inline const iStringT& GetResPath() const
	{ return m_ResPath; }

	bool AddSprite(const iStringT& name, const iStringT& chunk, const iPoint& anchor, bool spanned, bool crop, uint8 shadow, uint32 seqX, uint32 seqY, const iStringT& constName)
	{
		iDib tdib;
		if (!seqX || !seqY || !iDibLoader::Load(tdib, m_ResPath + chunk)) {
			MessageBox(NULL,iFormat(_T("Unable to open sprite %s !"),(m_ResPath + chunk).CStr()).CStr(),_T("Error"),MB_OK);
			return false;
		}
		//OutputDebugString(iFormat(_T("Sprite [%s] loaded.\n"),(m_ResPath + chunk).CStr()).CStr());
		
		if (seqX == 1 && seqY == 1){
			iKVHash::Iterator it = m_SpriteHash.Insert(name);
			(*it).m_Value.Init(tdib, anchor, spanned, crop, shadow);
		} else {
			uint32 iw = tdib.GetWidth() / seqX;
			uint32 ih = tdib.GetHeight() / seqY;
			iRect rc(0,0,iw,ih);
			for (uint32 yy=0; yy<seqY; ++yy) {
				for (uint32 xx=0; xx<seqX; ++xx) {
					iStringT nname = iFormat(_T("%s_%04d"),name.CStr(),yy*seqX+xx);
					iKVHash::Iterator it = m_SpriteHash.Insert(nname);
					(*it).m_Value.Init(tdib, anchor, spanned, crop, shadow, rc);
					rc.x+=iw;
				}
				rc.y+=ih;
				rc.x=0;
			}
		}

		if (!constName.Empty()) {
			m_ConstDefList.Add(iConstDef(name,constName,seqX*seqY));
		}

		return true;
	}

	const iSprite* GetSpritePtr(const iStringT& name) const
	{
		iKVHash::ConstIterator it = m_SpriteHash.Find(name);
		if (it==m_SpriteHash.End()) return &m_InvalidSprite;
		const iSprite* result = &(*it).m_Value;
		if (result) return result;
		return &m_InvalidSprite;
	}

	bool Init(const iStringT& fname);

	bool InitFromCache(iFileI* cache );
	bool SaveCache( iFileI* cache );

	const iSprite& operator[] (const iStringT& name) const
	{
		iKVHash::ConstIterator it = m_SpriteHash.Find(name);
		check (it!=m_SpriteHash.End());
		return (*it).m_Value;
	}

private:

	iSprite& operator[] (const iStringT& name)
	{
		iKVHash::Iterator it = m_SpriteHash.Find(name);
		check (it!=m_SpriteHash.End());
		return (*it).m_Value;
	}
};

#endif //_HMM_EDITOR_SPRITE_MANAGER_H_