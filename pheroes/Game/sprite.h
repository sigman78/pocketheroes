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

#ifndef _POCKET_HEROES_GAME_SPRITE_H_
#define _POCKET_HEROES_GAME_SPRITE_H_


/*
 *	Abstract Sprite Class
 */
class iSpriteI
{
public:
	virtual ~iSpriteI(){}

	virtual bool Init(iDynamicBuffer &buff) = NULL;
	virtual void Cleanup() = NULL;
	virtual inline iPoint GetAnchor() const = NULL;
	virtual inline iSize GetSize() const = NULL;
	virtual inline bool IsInited() const = NULL;
	virtual void Compose(iDib& srf, const iPoint& pos) const = NULL;
	virtual void ComposeRect(iDib& srf, const iRect& srect, const iPoint& pos) const = NULL;
	virtual void ComposeTranslucent(iDib& srf, const iPoint& pos) const = NULL;
	virtual void TileRect(iDib& srf, const iRect& src_rect, const iRect& dst_rect) const = NULL;
	virtual void H3TileRect(iDib& srf, const iRect& src_rect, const iRect& dst_rect) const = NULL;
	virtual bool HasShadow() const = NULL;
	virtual void SetShadow(bool bVal) = NULL;

	virtual const uint16* GetPtr() const = NULL;
	virtual inline uint32 GetBuffSiz() const = NULL;
};

/*
 *	16bit Dib Sprite Class
 */
class iSprite16 : public iSpriteI
{
public:
	iSprite16()
	: m_bInited(false){}

	bool Init(iDynamicBuffer &buff);
	void Cleanup();
	virtual void Compose(iDib& srf, const iPoint& pos) const;
	virtual void ComposeRect(iDib& srf, const iRect& srect, const iPoint& pos) const;
	virtual void ComposeTranslucent(iDib& srf, const iPoint& pos) const;
	virtual void TileRect(iDib& srf, const iRect& src_rect, const iRect& dst_rect) const;
	virtual void H3TileRect(iDib& srf, const iRect& src_rect, const iRect& dst_rect) const;
	
	inline iSize GetSize() const
	{ return m_Dib.GetSize(); }
	inline iPoint GetAnchor() const
	{ return m_Anchor; }
	inline bool IsInited() const
	{ return m_bInited; }

	inline uint32 GetBuffSiz() const
	{ return m_Dib.GetWidth()*m_Dib.GetHeight()*sizeof(uint16); }
	const uint16* GetPtr() const
	{ return m_Dib.GetPtr(); }
	void SetShadow(bool bVal)
	{ m_bHasShadow = bVal;}
	bool HasShadow() const
	{ return m_bHasShadow; }

private:
	bool	m_bHasShadow;
	iPoint	m_Anchor;
	bool	m_bInited;
	iDib	m_Dib;
};

/*
 *	16bit SpanSprite
 */
class iSpanSprite16 : public iSpriteI
{
public:
	iSpanSprite16()
	: m_bInited(false){}

	bool Init(iDynamicBuffer &buff);
	void Cleanup();
	void Compose(iDib& srf, const iPoint& pos) const;
	void ComposeRect(iDib& srf, const iRect& srect, const iPoint& pos) const;
	void ComposeTranslucent(iDib& srf, const iPoint& pos) const;
	void TileRect(iDib& srf, const iRect& src_rect, const iRect& dst_rect) const;
	void H3TileRect(iDib& srf, const iRect& src_rect, const iRect& dst_rect) const;
	
	inline iSize GetSize() const
	{ return m_Size; }
	inline iPoint GetAnchor() const
	{ return m_Anchor; }
	inline bool IsInited() const
	{ return m_bInited; }
	inline uint32 GetBuffSiz() const
	{ return m_Buff.GetSize()*sizeof(uint16); }
	inline const uint16* GetPtr() const
	{ return m_Buff.GetPtr(); }
	inline const uint16* GetEPtr() const
	{ return m_Buff.GetPtr() + m_Buff.GetSize(); }
	void SetShadow(bool bVal)
	{ m_bHasShadow = bVal;}
	bool HasShadow() const
	{ return m_bHasShadow; }

private:
	bool			m_bHasShadow;
	iPoint			m_Anchor;
	iSize			m_Size;
	iBuff<uint16>	m_Buff;
	bool			m_bInited;
};

/*
 *	Sprite Manager
 */
class iSpriteMgr
{
public:
	iSpriteMgr();
	~iSpriteMgr();
	
	bool InitSpriteMgr();
	uint32 AddSprite(iDynamicBuffer &buff);
	void Cleanup();

	inline iSize GetSpriteSize(uint32 spr_id)
	{
		check(spr_id < m_Sprites.GetSize());
		return m_Sprites[spr_id]->GetSize();
	}

	inline const iDib& GetSpriteDib(uint32 spr_id) const
	{
	}

	void ComposeSprite(uint32 spr_id, iDib& srf, const iPoint& pos) const;
	void ComposeSpriteRect(uint32 spr_id, iDib& srf, const iRect& srect, const iPoint& pos) const;
	
	void ComposeMaskedSprite(uint32 spr_id, uint32 mask_id, iDib& srf, const iPoint& pos) const;
	void ComposeShadow(uint32 spr_id, iDib& srf, const iPoint& pos) const;
	void ComposeTranslucent(uint32 spr_id, iDib& srf, const iPoint& pos) const;

	void TileSpriteRect(uint32 spr_id, iDib& srf, const iRect& src_rect, const iRect& dst_rect) const;
	void H3TileSpriteRect(uint32 spr_id, iDib& srf, const iRect& src_rect, const iRect& dst_rect) const;
	void H3TileSprite(uint32 spr_id, iDib& srf, const iRect& dst_rect) const;

	inline bool HasShadow(uint32 spr_id)
	{ return m_Sprites[spr_id]->HasShadow(); }

private:
	iSimpleArray<iSpriteI*>	m_Sprites;
};

#endif //_POCKET_HEROES_GAME_SPRITE_H_

