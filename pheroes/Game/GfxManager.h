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

#ifndef _HMM_GAME_GFX_MANAGER_H_
#define _HMM_GAME_GFX_MANAGER_H_

//////////////////////////////////////////////////////////////////////////
typedef uint32 SpriteId;
//////////////////////////////////////////////////////////////////////////

#ifdef GAME_EDITOR
class GfxBankExporter
{
public:
	void AddRaw( iDib& sprite, iPoint origin, uint8 flags = 0 );
	void AddSpanned( iDib& sprite, iPoint origin, uint8 flags = 0 );

	void Export( iFileI* dest );
	void Export( const TCHAR* fileName );

	size_t Size() const;
	void Clear();

	size_t EstimateFileSize() const;

	GfxBankExporter();
	~GfxBankExporter();
};
#endif

class iGfxManager
{
public:
	enum { MaxSpriteBanks = 16 };
	enum { MaxDynItems = 32 };

	enum Effects {
		EfxNone			= 0,
		EfxTransparent	= 1,
		EfxShadowIso	= 2,
		EfxShadow2D		= 3,
		EfxWired		= 4
	};

	enum LoadMode {
		LM_Unknown,
		LM_Memory,
		LM_MappedFile
	};

	typedef uint32	BankId;
	typedef uint32  SpriteId;

	iGfxManager()
	: dynPos(0)
	, gammaLevel(0)
	{ memset(dynItems,0, sizeof(dynItems)); }

	~iGfxManager()
	{
		for (sint32 xx=0; xx<MaxDynItems; ++xx) {
			if (dynItems[xx]) {
				delete dynItems[xx];
				dynItems[xx] = NULL;
			}
		}
	}


	// Helper to make SpriteUID from BankId + Sprite number
	static SpriteId MakeSpriteId( BankId cat, uint32 sn );
	static SpriteId GetFlippedSpriteId( SpriteId sid );
	static inline bool IsFlippedSprite( SpriteId sid )
	{ return (sid & (1UL<<31)) == 1; }

    void inline SetGamma( uint32 glevel )
    { gammaLevel = glevel; }

	bool Load( BankId cat, const TCHAR* fileName, LoadMode lmode );
	void Unload( BankId cat );
	void Reload();

	size_t		NumSprites( BankId cat ) const;
	SpriteId	FirstId( BankId cat ) const;
	SpriteId	EndId( BankId cat ) const;

	iSize Dimension( SpriteId sid ) const;
	iPoint Anchor( SpriteId sid ) const;
	uint8 Shadow( SpriteId sid ) const;
	
	void Blit( SpriteId sid, iDib& to, iPoint pos ) const;
	void Blit( SpriteId sid, iDib& to, const iRect& src, iPoint pos ) const;
	
	void BlitAlpha( SpriteId sid, iDib& to, iPoint pos, uint8 a ) const; // alpha - 0-63
	void BlitAlpha( SpriteId sid, iDib& to, const iRect& src, iPoint pos, uint8 a ) const; // alpha - 0-63
	void BlitEffect( SpriteId sid, iDib& to, iPoint pos, Effects efx ) const;

	void BlitTile( SpriteId sid, iDib& to, const iRect& dst ) const;
	void BlitTile( SpriteId sid, iDib& to, const iRect& src, const iRect& dst ) const;
	void BlitTile3( SpriteId sid, iDib& to, const iRect& src, const iRect& dst ) const;

	void BlitMasked( SpriteId sid, SpriteId mid, iDib& to, iPoint pos ) const;

	void MakeDib( SpriteId sid, iDib& dst );

	size_t MemoryOccuped() const;
	

	// 4*4 = 16 b per sprite
	struct Sprite
	{
		uint8			type_;
		uint32			offset_;	// offset from the start of data block ( in uin16 / pixels )
		uint16			sizeW_;
		uint16			sizeH_;
		sint16			originX_;
		sint16			originY_;
	};

	struct SpriteBank {
		size_t			dataSize;	// for statistics
		iBuff<Sprite>	props;

		const Sprite*	GetSprite( uint32 num ) const;
		const uint16*	Data( uint32 offset ) const;
		size_t			MemTotal() const;

		SpriteBank();
		~SpriteBank();

		// 
		uint8*			data;
		HANDLE			hFile;
		HANDLE			hMapping;
		void*			pMap;
		LoadMode		loadMode;
		iStringT		fileName;

		bool Load( const TCHAR* file, LoadMode lmode, uint32 gammaLevel );
		void Unload();
		bool Reload( uint32 gammaLevel );
	};

	struct DynItem {
		DynItem() : data(NULL) {}
		~DynItem() { if (data) { free(data); data=NULL; } }

		SpriteId	sid;
		Sprite		sprite;
		uint8*		data;
		size_t		dataSize;
	};

	uint32		gammaLevel;
	DynItem*	dynItems[ MaxDynItems ];
	uint32		dynPos;
	SpriteBank	bank_[ MaxSpriteBanks ];

private:
	// Non copyable
	iGfxManager& operator=( const iGfxManager& );
	iGfxManager( const iGfxManager& );

	void FlipSprite(SpriteId sid);

	void BlitNormal( const Sprite* spr, const uint16* data, iDib& srf, const iRect& src, iPoint dst ) const;
	
	void BlitAlpha( const iGfxManager::Sprite* sprPtr, const uint16* pixels, iDib& srf, const iRect& src, iPoint pos, uint8 a) const;
	void BlitTransparent( const Sprite* spr, const uint16* data, iDib& srf, iPoint dst) const;
	void BlitShadowIso( const Sprite* spr, const uint16* data, iDib& srf, iPoint dst ) const;
	void BlitShadow2D( const Sprite* spr, const uint16* data, iDib& srf, iPoint dst ) const;
	void BlitWired( const Sprite* spr, const uint16* data, iDib& srf, iPoint dst ) const;
};

#endif //_HMM_GAME_GFX_MANAGER_H_

