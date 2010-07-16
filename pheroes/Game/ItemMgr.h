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

#ifndef _HMM_GAME_ITEM_MANAGER_H_
#define _HMM_GAME_ITEM_MANAGER_H_

const uint8		CASTLE_DEFAULT_SCOUTING	=	5;

/*
 *	Template prototypes
 */
class iPathElT
{
public:
	iPathElT(TextResId nameKey, PATH_ELEMENT_TYPE type, const SpriteId& spriteBase, SPRITE_LEVEL level, sint32 passMod)
	: m_nameKey(nameKey), m_type(type), m_spriteBase(spriteBase), m_level(level), m_passMod(passMod) {}

	inline TextResId NameKey() const { return m_nameKey; }
	inline PATH_ELEMENT_TYPE Type() const { return m_type; }
	inline SpriteId SpriteConfig(uint8 cfg) const { return m_spriteBase + cfg; }
	inline SPRITE_LEVEL Level() const { return m_level; }
	inline sint32 PassModVal() const { return m_passMod; }

private:
	SpriteId			m_spriteBase;
	TextResId			m_nameKey;
	PATH_ELEMENT_TYPE	m_type;
	SPRITE_LEVEL		m_level;
	sint32				m_passMod;
};

//
class iHero;
class iArtT
{
public:
	iArtT(TextResId nameKey, TextResId descKey, ART_TYPE type, ART_LEVEL_TYPE level, ART_ASSIGN_TYPE assignType, uint16 reqLevel, SECONDARY_SKILLS reqSecSkill, SpriteId sid);
	virtual ~iArtT() {}

	virtual TextResId NameKey(const iHero* pOwner) const { return m_NameKey; }
	virtual TextResId DescKey(const iHero* pOwner) const { return m_DescKey; }
	inline ART_TYPE Type() const { return m_Type; }
	inline ART_LEVEL_TYPE Level() const { return m_Level; }
	inline ART_ASSIGN_TYPE Assign() const { return m_AssignType; }
	inline const SpriteId& Sprite() const { return m_Sid; }
	inline uint16 ReqLevel() const { return m_reqLevel; }
	inline SECONDARY_SKILLS ReqSkill() const { return m_reqSecSkill; }

	// Overrides
	bool CanAttach(const iHero* pOwner) const;
	virtual void Attach(iHero* pOwner) const {}
	virtual void Dettach(iHero* pOwner) const {}
	virtual void NewDay(iHero* pOwner) const {}
	virtual void NewWeek(iHero* pOwner) const {}

private:
	TextResId			m_NameKey;
	TextResId			m_DescKey;
	ART_TYPE			m_Type;
	ART_LEVEL_TYPE		m_Level;
	ART_ASSIGN_TYPE		m_AssignType;
	uint16				m_reqLevel;
	SECONDARY_SKILLS	m_reqSecSkill;
	SpriteId			m_Sid;
};
//
class iArtT_FurtSkill : public iArtT
{
public:
	iArtT_FurtSkill(TextResId nameKey, TextResId descKey, ART_LEVEL_TYPE level, ART_ASSIGN_TYPE assignType, uint16 reqLevel, SECONDARY_SKILLS reqSecSkill, SpriteId sid, const iFurtSkills& furtSkills);

	void Attach(iHero* pOwner) const;
	void Dettach(iHero* pOwner) const;

	inline const iFurtSkills& FurtSkills() const { return m_furtSkills; }

private:
	iFurtSkills m_furtSkills;
};
//
class iArtT_NegSphere : public iArtT
{
public:
	iArtT_NegSphere(TextResId nameKey, TextResId descKey, ART_LEVEL_TYPE level, ART_ASSIGN_TYPE assignType, uint16 reqLevel, SECONDARY_SKILLS reqSecSkill, SpriteId sid);
};
//
class iArtT_ShOfWar : public iArtT
{
public:
	iArtT_ShOfWar(TextResId nameKey, TextResId descKey, ART_LEVEL_TYPE level, ART_ASSIGN_TYPE assignType, uint16 reqLevel, SECONDARY_SKILLS reqSecSkill, SpriteId sid);
};
//
class iArtT_CurSword : public iArtT
{
public:
	iArtT_CurSword(TextResId nameKey, TextResId descKey, ART_LEVEL_TYPE level, ART_ASSIGN_TYPE assignType, uint16 reqLevel, SECONDARY_SKILLS reqSecSkill, SpriteId sid);
};
//
class iArtT_Ultimate : public iArtT
{
public:
	iArtT_Ultimate();
	void Attach(iHero* pOwner) const;
	void Dettach(iHero* pOwner) const;
	TextResId DescKey(const iHero* pOwner) const;
};

//////////////////////////////////////////////////////////////////////////
class iCnstT : public iAvatarsHolder, public TypeAware
{
public:
	IMPL_TYPEAWARE( iCnstT );
	iCnstT(TextResId nameKey, uint16 protoId) : m_NameKey(nameKey), m_protoId(protoId) {}

	inline TextResId NameKey() const { return m_NameKey; }
	inline uint16 ProtoId() const { return m_protoId; }

private:
	TextResId	m_NameKey;
	uint16		m_protoId;
};
//////////////////////////////////////////////////////////////////////////
class iVisCnst;
class iVisCnstT : public iCnstT
{
public:
	IMPL_TYPEAWARE( iVisCnstT );
	iVisCnstT(TextResId nameKey, uint16 protoId, TextResId msgKey, TextResId vmsgKey, VIS_CNST_TYPE type, bool bFixedGuard) 
	: iCnstT(nameKey, protoId), m_Type(type), m_msgKey(msgKey), m_vmsgKey(vmsgKey), m_bFixedGuard(bFixedGuard){}

	virtual ~iVisCnstT(){}
	inline VIS_CNST_TYPE Type() const { return m_Type; }
	inline TextResId Message() const { return m_msgKey; }
	inline TextResId VisMessage() const { return m_vmsgKey; }
	virtual iVisCnst* CreateObject(const iPoint& pos);
	inline bool FixedGuard() const { return m_bFixedGuard; }

private:
	bool			m_bFixedGuard;
	TextResId		m_msgKey;
	TextResId		m_vmsgKey;
	VIS_CNST_TYPE	m_Type;
};
//
class iVisCnstT_Stable : public iVisCnstT
{
public:
	IMPL_TYPEAWARE( iVisCnstT_Stable );
	iVisCnstT_Stable(TextResId nameKey, uint16 protoId, TextResId msgKey, TextResId vmsgKey, sint32 act_pts) 
	: iVisCnstT(nameKey, protoId, msgKey, vmsgKey, VCNST_STABLE, false), m_ActPoints(act_pts) {}
	inline sint32 ActPts() const { return m_ActPoints; }
	iVisCnst* CreateObject(const iPoint& pos);

private:
	sint32	m_ActPoints;
};
//
class iVisCnstT_Gazebo : public iVisCnstT
{
public:
	IMPL_TYPEAWARE( iVisCnstT_Gazebo );
	iVisCnstT_Gazebo(TextResId nameKey, uint16 protoId, TextResId msgKey, TextResId vmsgKey) 
	: iVisCnstT(nameKey, protoId, msgKey, vmsgKey, VCNST_GAZEBO, false) {}
	iVisCnst* CreateObject(const iPoint& pos);
};
//
class iVisCnstT_ManaSource : public iVisCnstT
{
public:
	IMPL_TYPEAWARE( iVisCnstT_ManaSource );
	iVisCnstT_ManaSource(TextResId nameKey, uint16 protoId, TextResId msgKey, TextResId vmsgKey, sint32 mana_pts) 
	: iVisCnstT(nameKey, protoId, msgKey, vmsgKey, VCNST_MANASOURCE, false), m_ManaPoints(mana_pts) {}
	inline sint32 ManaPts() const { return m_ManaPoints; }
	iVisCnst* CreateObject(const iPoint& pos);

private:
	sint32	m_ManaPoints;
};
//
class iVisCnstT_MLModifier : public iVisCnstT
{
public:
	IMPL_TYPEAWARE( iVisCnstT_MLModifier );
	iVisCnstT_MLModifier(TextResId nameKey, uint16 protoId, TextResId msgKey, TextResId vmsgKey, sint8 moraleM, sint8 luckM) 
	: iVisCnstT(nameKey, protoId, msgKey, vmsgKey, VCNST_MLMODIFIER, false), m_moraleModifier(moraleM), m_luckModifier(luckM) {}
	inline sint8 MoraleModifier() const { return m_moraleModifier; }
	inline sint8 LuckModifier() const { return m_luckModifier; }
	iVisCnst* CreateObject(const iPoint& pos);

private:
	sint8	m_moraleModifier;
	sint8	m_luckModifier;
};
//
class iVisCnstT_PSModifier : public iVisCnstT
{
public:
	IMPL_TYPEAWARE( iVisCnstT_PSModifier );
	iVisCnstT_PSModifier(TextResId nameKey, uint16 protoId, TextResId msgKey, TextResId vmsgKey, const iPrSkills& psM) 
	: iVisCnstT(nameKey, protoId, msgKey, vmsgKey, VCNST_PSMODIFIER, false), m_psModifier(psM) {}
	inline const iPrSkills& PSModifier() const { return m_psModifier; }
	iVisCnst* CreateObject(const iPoint& pos);

private:
	iPrSkills	m_psModifier;
};
//
class iVisCnstT_Obelisk : public iVisCnstT
{
public:
	IMPL_TYPEAWARE( iVisCnstT_Obelisk );
	iVisCnstT_Obelisk(TextResId nameKey, uint16 protoId, TextResId msgKey, TextResId vmsgKey) 
	: iVisCnstT(nameKey, protoId, msgKey, vmsgKey, VCNST_OBELISK, false) {}
	iVisCnst* CreateObject(const iPoint& pos);
};
//
class iVisCnstT_Sign : public iVisCnstT
{
public:
	IMPL_TYPEAWARE( iVisCnstT_Sign );
	iVisCnstT_Sign(TextResId nameKey, uint16 protoId, TextResId msgKey, TextResId vmsgKey) 
	: iVisCnstT(nameKey, protoId, msgKey, vmsgKey, VCNST_SIGN, false) {}
	iVisCnst* CreateObject(const iPoint& pos);
};
//
class iVisCnstT_Dwelling : public iVisCnstT
{
public:
	IMPL_TYPEAWARE( iVisCnstT_Dwelling );
	iVisCnstT_Dwelling(TextResId nameKey, uint16 protoId, TextResId msgKey, TextResId vmsgKey, CREATURE_TYPE ctype) 
	: iVisCnstT(nameKey, protoId, msgKey, vmsgKey, VCNST_DWELLING, false), m_creatType(ctype) {}
	iVisCnst* CreateObject(const iPoint& pos);

	CREATURE_TYPE CreatType() const { return m_creatType; }

private:
	CREATURE_TYPE m_creatType;
};

//
class iVisCnstT_WitchHut : public iVisCnstT
{
public:
	IMPL_TYPEAWARE( iVisCnstT_WitchHut );
	iVisCnstT_WitchHut(TextResId nameKey, uint16 protoId, TextResId msgKey, TextResId vmsgKey) 
	: iVisCnstT(nameKey, protoId, msgKey, vmsgKey, VCNST_WITCHHUT, false) {}
	iVisCnst* CreateObject(const iPoint& pos);
};
//
class iVisCnstT_Shrine : public iVisCnstT
{
public:
	IMPL_TYPEAWARE( iVisCnstT_Shrine );
	iVisCnstT_Shrine(TextResId nameKey, uint16 protoId, TextResId msgKey, TextResId vmsgKey, SPELL_LEVEL level) 
	: iVisCnstT(nameKey, protoId, msgKey, vmsgKey, VCNST_SHRINE, false), m_level(level) {}
	iVisCnst* CreateObject(const iPoint& pos);
	SPELL_LEVEL SpellLevel() const { return m_level; } 

private:
	SPELL_LEVEL m_level;
};
//
class iVisCnstT_Treasury : public iVisCnstT
{
public:
	IMPL_TYPEAWARE( iVisCnstT_Treasury );
	iVisCnstT_Treasury(TextResId nameKey, uint16 protoId, TextResId msgKey, TextResId vmsgKey) 
	: iVisCnstT(nameKey, protoId, msgKey, vmsgKey, VCNST_TREASURY, false){}
	iVisCnst* CreateObject(const iPoint& pos);
	iTrVarCtr& GetVarCtr() { return m_vars; }
	const iTrVarCtr& GetVarCtr() const { return m_vars; }

private:
	SPELL_LEVEL m_level;
	iTrVarCtr	m_vars;
};

//
class iVisCnstT_Teleport : public iVisCnstT
{
public:
	IMPL_TYPEAWARE( iVisCnstT_Teleport );
	iVisCnstT_Teleport(TextResId nameKey, uint16 protoId, TextResId msgKey, TextResId vmsgKey, uint8 mode, uint8 index) 
	: iVisCnstT(nameKey, protoId, msgKey, vmsgKey, VCNST_TELEPORT, false), m_mode(mode), m_index(index) {}
	iVisCnst* CreateObject(const iPoint& pos);

	inline uint8 Index() const { return m_index; }
	inline uint8 IsEntrance() const { return (m_mode & 1) > 0; }
	inline uint8 IsTwoWay() const { return (m_mode & 2) > 0; }

private:
	uint8	m_mode;
	uint8	m_index;
};
//
class iVisCnstT_Keymaster : public iVisCnstT
{
public:
	IMPL_TYPEAWARE( iVisCnstT_Keymaster );
	iVisCnstT_Keymaster(TextResId nameKey, uint16 protoId, TextResId msgKey, TextResId vmsgKey, uint8 key) 
	: iVisCnstT(nameKey, protoId, msgKey, vmsgKey, VCNST_KEYMASTER, false), m_key(key) {}
	iVisCnst* CreateObject(const iPoint& pos);

	inline uint8 GetKeyType() const { return m_key; }

private:
	uint8	m_key;
};
//
class iVisCnstT_TreeOfKnowledge : public iVisCnstT
{
public:
	IMPL_TYPEAWARE( iVisCnstT_TreeOfKnowledge );
	iVisCnstT_TreeOfKnowledge(TextResId nameKey, uint16 protoId, TextResId msgKey, TextResId vmsgKey) 
	: iVisCnstT(nameKey, protoId, msgKey, vmsgKey, VCNST_KNWLTREE, false) {}
	iVisCnst* CreateObject(const iPoint& pos);
};
//
class iVisCnstT_Windmill : public iVisCnstT
{
public:
	IMPL_TYPEAWARE( iVisCnstT_Windmill );
	iVisCnstT_Windmill(TextResId nameKey, uint16 protoId, TextResId msgKey, TextResId vmsgKey) 
	: iVisCnstT(nameKey, protoId, msgKey, vmsgKey, VCNST_WINDMILL, false) {}
	iVisCnst* CreateObject(const iPoint& pos);
};

//
class iVisCnstT_WeeklyMin : public iVisCnstT
{
public:
	IMPL_TYPEAWARE( iVisCnstT_WeeklyMin );
	iVisCnstT_WeeklyMin(TextResId nameKey, uint16 protoId, TextResId msgKey, TextResId vmsgKey) 
	: iVisCnstT(nameKey, protoId, msgKey, vmsgKey, VCNST_WEEKLYMIN, false) {}
	iVisCnst* CreateObject(const iPoint& pos);
	iMineralSet& Minerals() { return m_minerals; }

private:
	iMineralSet	m_minerals;
};
//////////////////////////////////////////////////////////////////////////
class iOwnCnst;
class iOwnCnstT : public iCnstT
{
public:
	IMPL_TYPEAWARE( iOwnCnstT );
	iOwnCnstT(TextResId nameKey, uint16 protoId, TextResId msgKey, SpriteId msgIcn, OWN_CNST_TYPE type, uint8 scout) 
	: iCnstT(nameKey, protoId), m_msgKey(msgKey), m_msgIcn(msgIcn), m_Type(type), m_Scouting(scout) {}
	inline OWN_CNST_TYPE Type() const { return m_Type; }
	inline uint8 Scouting() const { return m_Scouting; }
	inline TextResId Message() const { return m_msgKey; }
	inline SpriteId MessageIcon() const { return m_msgIcn; }
	virtual iOwnCnst* CreateObject(const iPoint& pos);

private:
	TextResId		m_msgKey;
	SpriteId		m_msgIcn;
	OWN_CNST_TYPE	m_Type;
	uint8			m_Scouting;
};
//
class iOwnCnstT_FurtSkill : public iOwnCnstT
{
public:
	IMPL_TYPEAWARE( iOwnCnstT_FurtSkill );
	iOwnCnstT_FurtSkill(TextResId nameKey, uint16 protoId, TextResId msgKey, SpriteId msgIcn, uint8 scout, const iFurtSkills& furtSkills) 
	: iOwnCnstT(nameKey, protoId, msgKey, msgIcn, OCNST_FURTSKILL, scout), m_furtSkills(furtSkills) {}
	inline const iFurtSkills& FurtSkills() const { return m_furtSkills; }
	iOwnCnst* CreateObject(const iPoint& pos);

private:
	iFurtSkills		m_furtSkills;
};
//////////////////////////////////////////////////////////////////////////
class iCastleT : public iCnstT
{
public:
	IMPL_TYPEAWARE( iCastleT );
	iCastleT(uint16 protoId, CTL_TYPE type, CTL_SIZE size, CTL_ORIENT orient, uint16 cfg)
	: iCnstT(TRID_CASTLE_TYPE_CITADEL+type, protoId), m_Type(type), m_Size(size), m_Orient(orient), m_Cfg(cfg) {}
	inline CTL_TYPE Type() const { return m_Type; }
	inline CTL_SIZE Size() const { return m_Size; }
	inline CTL_ORIENT Orient() const { return m_Orient; }
	inline uint8 Scouting() const { return CASTLE_DEFAULT_SCOUTING + m_Size*3; }
	inline uint16 Config() const { return m_Cfg; }

private:
	uint16		m_Cfg;
	CTL_TYPE	m_Type;
	CTL_SIZE	m_Size;
	CTL_ORIENT	m_Orient;
};

/*
 *	Unique objects repositories
 */
class iCastleRepos
{
public:
	iCastleRepos();
	TextResId GetCastleName(CTL_TYPE ctlType);
	void InitCastles();

private:
	iSimpleArray<TextResId>	m_items[CTLT_COUNT];
};

/*
 *	Spell manager
 */
class iSpellMgr
{
public:
	iSpellMgr();
	~iSpellMgr();
	void InitSpells();
	void Cleanup();

public:
	inline const iBaseSpell* Spell(MAGIC_SPELL spellId) const { return m_spells[spellId]; }
	inline iBaseSpell* Spell(MAGIC_SPELL spellId) { return m_spells[spellId]; }
	inline uint32 SpellsCount() const { return m_spells.GetSize(); }

private:
	iSimpleArray<iBaseSpell*>	m_spells;
};

/*
 *	Heroes Manager
 */
class iHeroT
{
public:
	iHeroT(uint16 protoId, TextResId nameKey, HERO_TYPE htype, SpriteId icn32, SpriteId icn48) 
	: m_protoId(protoId), m_NameKey(nameKey), m_hType(htype), m_icn32(icn32), m_icn48(icn48) { }

public:
	uint16		m_protoId;
	TextResId	m_NameKey;
	HERO_TYPE	m_hType;
	SpriteId	m_icn32;
	SpriteId	m_icn48;
	iFurtSkills	m_furtSkills;
	iSecSkills	m_secSkills;
	iSpellList	m_spells;
	iMineralSet	m_Cost;
};
//////////////////////////////////////////////////////////////////////////
class iHeroesMgr
{
public:
	enum HeroVarIndex {
		Low		= 1,
		Normal	= 5,
		High	= 10
	};

	enum {
		INVALID_HERO_ID = 0xFFFF
	};

	typedef iSimpleArray<iHeroT*>	iHeroProtoList;
	typedef iSimpleArray<iHero*>	iHeroItemList;
	friend class iGameWorld;

public:
	// c-tor
	iHeroesMgr();
	// d-tor
	~iHeroesMgr();

	// Add hero prototype
	void AddProto(iHeroT* pProto);
	
	// Cleanup manager content
	void Cleanup();

	// Cleanup objects
	void CleanupObjects();

	// Init objects
	void InitObjects();

	// select random hero according to specified hero type mask
	uint16 Select(uint32 heroTypeMask);

	// recruit specified hero according to its prototype id
	iHero* Get(uint16 protoId);

	// Add given hero to the list
	void Put(iHero* pHero);

protected:
	iHeroProtoList	m_protos;
	iHeroItemList	m_heroes;
};

//////////////////////////////////////////////////////////////////////////
class iBatObstMgr 
{
public:
	struct iItem {
		iItem(uint16 _surfMask, SpriteId _sprite, const iPoint& _offset, uint8 _size, uint8 _level)
		: surfMask(_surfMask), sprite(_sprite), offset(_offset), size(_size), level(_level) {}
		uint16		surfMask;	// surface mask
		SpriteId	sprite;		// sprite id
		iPoint		offset;		// offset
		uint8		size;		// obstacle size (1 or 2)
		uint8		level;		// level (0 - ground, 1 - normal)
	};

public:
	void Add(uint16 surfMask, SpriteId sprite, const iPoint& offset, uint8 size, uint8 level)
	{ 
		m_items.Add(iItem(surfMask, sprite, offset, size, level)); 
	}

	void SelectItems(iSimpleArray<uint16>& items, SURF_TYPE st) const
	{
		for (uint16 xx=0; xx<m_items.GetSize(); ++xx)
			if (m_items[xx].surfMask & SURF_TYPE_MASK[st])
				items.Add(xx);
	}

	inline const iItem& Item(uint32 idx) const { return m_items[idx]; }

		
private:
	iSimpleArray<iItem>	m_items;
};

//////////////////////////////////////////////////////////////////////////
class iArtifactMgr
{
public:
	iArtifactMgr();
	~iArtifactMgr();
	void Cleanup();

	const iArtT& operator[] (uint32 idx) { check(idx<0xFF00); if (idx == ARTT_ULTIMATE) return m_ultimateArt; else return *m_artProts[idx]; }
	uint16 SelectRandomArtifact(ART_LEVEL_TYPE level);
	void AddArtifact(iArtT* pArt) { m_artProts.Add(pArt); }
	
protected:
	iSimpleArray<iArtT*>	m_artProts;
	iArtT_Ultimate			m_ultimateArt;
};


/*
 *	Item/Prototype manager
 */
class iItemMgr
{
public:
	typedef iSimpleArray<iPathElT*>		iPathElList;
	typedef iSimpleArray<iVisCnstT*>	iVisList;
	typedef iSimpleArray<iOwnCnstT*>	iOwnList;
	typedef iSimpleArray<iCastleT*>		iCastleList;

public:
	// c-tor, d-tor
	iItemMgr();
	~iItemMgr();
	void Cleanup();

	inline iHeroesMgr& HeroesMgr() { return m_heroesMgr; }
	inline const iBatObstMgr& BatObstMgr() const { return m_batObstMgr; }

	void OnGameStart();
	void OnGameEnd();

	friend bool LoadObjectTemplates(iDynamicBuffer& buff, iItemMgr& imgr);

	inline bool IsRandomCtl(uint16 proto) const { return m_CastleProts[proto]->Type() == CTLT_RANDOM; }
	uint16 AlterCtlNation(uint16 proto, CTL_TYPE nation) const;

public:
	iCastleRepos	m_CastleRepos;
	iPathElList		m_PathElProts;
	iArtifactMgr	m_artMgr;
	iVisList		m_VisProts;
	iOwnList		m_OwnProts;
	iCastleList		m_CastleProts;
	iAvatarMgr		m_AvatarMgr;
	iSpellMgr		m_spellMgr;
	iSimpleArray<iStringT>	m_credits;

protected:
	iHeroesMgr		m_heroesMgr;
	bool			m_gameStarted;
	iBatObstMgr		m_batObstMgr;
};

bool LoadResources(iItemMgr& imgr);

#endif //_HMM_GAME_ITEM_MANAGER_H_

