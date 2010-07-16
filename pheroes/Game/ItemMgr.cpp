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
#include "serialize.h"
#include "lzo.h"


/*
 *	Artifacts
 */
iArtT::iArtT(TextResId nameKey, TextResId descKey, ART_TYPE type, ART_LEVEL_TYPE level, ART_ASSIGN_TYPE assignType, uint16 reqLevel, SECONDARY_SKILLS reqSecSkill, SpriteId sid)
: m_NameKey(nameKey), m_DescKey(descKey), m_Type(type), m_Level(level), m_AssignType(assignType), m_reqLevel(reqLevel), m_reqSecSkill(reqSecSkill), m_Sid(sid) {}

bool iArtT::CanAttach(const iHero* pOwner) const
{
	return (!m_reqLevel || pOwner->Level() >= m_reqLevel) && (m_reqSecSkill == SECSK_NONE || pOwner->SecSkills().SkillLevel(m_reqSecSkill) != SSLVL_NONE);
}

//
iArtT_FurtSkill::iArtT_FurtSkill(TextResId nameKey, TextResId descKey, ART_LEVEL_TYPE level, ART_ASSIGN_TYPE assignType, uint16 reqLevel, SECONDARY_SKILLS reqSecSkill, SpriteId sid, const iFurtSkills& furtSkills)
: iArtT(nameKey, descKey, ARTT_FURTSKILL, level, assignType, reqLevel, reqSecSkill, sid), m_furtSkills(furtSkills) {}

void iArtT_FurtSkill::Attach(iHero* pOwner) const
{
	check(CanAttach(pOwner));
	pOwner->VarFurtSkills() += m_furtSkills;
}

void iArtT_FurtSkill::Dettach(iHero* pOwner) const
{
	pOwner->VarFurtSkills() -= m_furtSkills;
}
//
iArtT_NegSphere::iArtT_NegSphere(TextResId nameKey, TextResId descKey, ART_LEVEL_TYPE level, ART_ASSIGN_TYPE assignType, uint16 reqLevel, SECONDARY_SKILLS reqSecSkill, SpriteId sid)
: iArtT(nameKey, descKey, ARTT_NEGSPHERE, level, assignType, reqLevel, reqSecSkill, sid) {}
//
iArtT_ShOfWar::iArtT_ShOfWar(TextResId nameKey, TextResId descKey, ART_LEVEL_TYPE level, ART_ASSIGN_TYPE assignType, uint16 reqLevel, SECONDARY_SKILLS reqSecSkill, SpriteId sid)
: iArtT(nameKey, descKey, ARTT_SHOFWAR, level, assignType, reqLevel, reqSecSkill, sid) {}
//
iArtT_CurSword::iArtT_CurSword(TextResId nameKey, TextResId descKey, ART_LEVEL_TYPE level, ART_ASSIGN_TYPE assignType, uint16 reqLevel, SECONDARY_SKILLS reqSecSkill, SpriteId sid)
: iArtT(nameKey, descKey, ARTT_CURSWORD, level, assignType, reqLevel, reqSecSkill, sid) {}

//
iArtT_Ultimate::iArtT_Ultimate()
: iArtT(TRID_ART_ULTIMATE, TRID_ART_ULTIMATE_DESC, ARTT_ULTIMATE, ART_LEVEL_ULTIMATE, ART_ASSIGN_HEAD, 0, SECSK_NONE, PDGG_ULTIMATE_ART){}

void iArtT_Ultimate::Attach(iHero* pOwner) const
{
	check(CanAttach(pOwner));
	for (uint32 nn=0; nn<7; ++nn) pOwner->VarFurtSkills().Value((FURTHER_SKILLS)ULTART_STDMODIF[pOwner->Proto()->m_hType][nn][0]) += ULTART_STDMODIF[pOwner->Proto()->m_hType][nn][1];
}

void iArtT_Ultimate::Dettach(iHero* pOwner) const
{
	for (uint32 nn=0; nn<7; ++nn) pOwner->VarFurtSkills().Value((FURTHER_SKILLS)ULTART_STDMODIF[pOwner->Proto()->m_hType][nn][0]) -= ULTART_STDMODIF[pOwner->Proto()->m_hType][nn][1];
}

TextResId iArtT_Ultimate::DescKey(const iHero* pOwner) const 
{ 
	if (!pOwner) return iArtT::DescKey(pOwner); 
	return TRID_ART_ULTIMATE_DESC_01 + pOwner->Proto()->m_hType; 
}

/*
 *	Visitables
 */
iVisCnst* iVisCnstT::CreateObject(const iPoint& pos)
{
	return new iVisCnst(this, pos, false);
}
//////////////////////////////////////////////////////////////////////////
iVisCnst* iVisCnstT_Stable::CreateObject(const iPoint& pos)
{
	return new iVisCnst_Stable(this, pos);
}
//////////////////////////////////////////////////////////////////////////
iVisCnst* iVisCnstT_Gazebo::CreateObject(const iPoint& pos)
{
	return new iVisCnst_Gazebo(this, pos);
}
//////////////////////////////////////////////////////////////////////////
iVisCnst* iVisCnstT_ManaSource::CreateObject(const iPoint& pos)
{
	return new iVisCnst_ManaSource(this, pos);
}
//////////////////////////////////////////////////////////////////////////
iVisCnst* iVisCnstT_MLModifier::CreateObject(const iPoint& pos)
{
	return new iVisCnst_MLModifier(this, pos);
}
//////////////////////////////////////////////////////////////////////////
iVisCnst* iVisCnstT_PSModifier::CreateObject(const iPoint& pos)
{
	return new iVisCnst_PSModifier(this, pos);
}
//////////////////////////////////////////////////////////////////////////
iVisCnst* iVisCnstT_Obelisk::CreateObject(const iPoint& pos)
{
	return new iVisCnst_Obelisk(this, pos);
}
//////////////////////////////////////////////////////////////////////////
iVisCnst* iVisCnstT_Dwelling::CreateObject(const iPoint& pos)
{
	return new iVisCnst_Dwelling(this, pos);
}
//////////////////////////////////////////////////////////////////////////
iVisCnst* iVisCnstT_Sign::CreateObject(const iPoint& pos)
{
	return new iVisCnst_Sign(this, pos);
}
//////////////////////////////////////////////////////////////////////////
iVisCnst* iVisCnstT_WitchHut::CreateObject(const iPoint& pos)
{
	return new iVisCnst_WitchHut(this, pos);
}
//////////////////////////////////////////////////////////////////////////
iVisCnst* iVisCnstT_Shrine::CreateObject(const iPoint& pos)
{
	return new iVisCnst_Shrine(this, pos);
}
//////////////////////////////////////////////////////////////////////////
iVisCnst* iVisCnstT_Treasury::CreateObject(const iPoint& pos)
{
	return new iVisCnst_Treasury(this, pos);
}
//////////////////////////////////////////////////////////////////////////
iVisCnst* iVisCnstT_Teleport::CreateObject(const iPoint& pos)
{
	return new iVisCnst_Teleport(this, pos);
}
//////////////////////////////////////////////////////////////////////////
iVisCnst* iVisCnstT_Keymaster::CreateObject(const iPoint& pos)
{
	return new iVisCnst_Keymaster(this, pos);
}
//////////////////////////////////////////////////////////////////////////
iVisCnst* iVisCnstT_TreeOfKnowledge::CreateObject(const iPoint& pos)
{
	return new iVisCnst_TreeOfKnowledge(this, pos);
}
//////////////////////////////////////////////////////////////////////////
iVisCnst* iVisCnstT_Windmill::CreateObject(const iPoint& pos)
{
	return new iVisCnst_Windmill(this, pos);
}
//////////////////////////////////////////////////////////////////////////
iVisCnst* iVisCnstT_WeeklyMin::CreateObject(const iPoint& pos)
{
	return new iVisCnst_WeeklyMin(this, pos);
}
/*
 *	Ownerables
 */
iOwnCnst* iOwnCnstT::CreateObject(const iPoint& pos)
{
	return new iOwnCnst(this, pos);
}

//////////////////////////////////////////////////////////////////////////
iOwnCnst* iOwnCnstT_FurtSkill::CreateObject(const iPoint& pos)
{
	return new iOwnCnst_FurtSkill(this, pos);
}


/*
 *	Unique objects repositories
 */
iCastleRepos::iCastleRepos()
{
	// init castle
	InitCastles();
}

void iCastleRepos::InitCastles()
{
	for (uint16 ctlType=0; ctlType<CTLT_COUNT; ++ctlType) {
		m_items[ctlType].RemoveAll();
		for (uint16 xx=0; xx<16; ++xx){
			m_items[ctlType].Add(TRID_CASTLE_NAME_01+(ctlType*16)+xx);
		}
	}
}

TextResId iCastleRepos::GetCastleName(CTL_TYPE ctlType)
{
	check(m_items[ctlType].GetSize());
	uint32 idx = gGame.Map().Rand(m_items[ctlType].GetSize());
	TextResId res = m_items[ctlType][idx];
	m_items[ctlType].RemoveAt(idx);
	return res;
}

/*
 *	Spell manager
 */
iSpellMgr::iSpellMgr()
{
}

iSpellMgr::~iSpellMgr()
{
	Cleanup();
}

iBaseSpell* CreateSpell(MAGIC_SPELL spellId)
{
	iBaseSpell* pRes = NULL;

	switch(SPELL_DESCRIPTORS[spellId].spClass) {
	case SPC_DAMAGE:
		pRes = new iSpell_Damage(spellId);
		break;
	case SPC_FURTSKILL:
		pRes = new iSpell_FurtSkill(spellId);
		break;
	case SPC_DISRAY:
		pRes = new iSpell_DisRay(spellId);
		break;
	case SPC_RESURRECT:
		pRes = new iSpell_Resurrect(spellId);
		break;
	case SPC_SUMMON:
		pRes = new iSpell_Summon(spellId);
		break;
	case SPC_BLESS:
		pRes = new iSpell_Bless(spellId);
		break;
	case SPC_BLIND:
		pRes = new iSpell_Blind(spellId);
		break;
	case SPC_EARTHQUAKE:
		pRes = new iSpell_Earthquake(spellId);
		break;
	case SPC_DISPEL:
		pRes = new iSpell_Dispel(spellId);
		break;
	case SPC_CURE:
		pRes = new iSpell_Cure(spellId);
		break;
	case SPC_TOWNPORTAL:
		pRes = new iSpell_TownPortal(spellId);
		break;
	case SPC_ANTIMAGIC:
		pRes = new iSpell_Antimagic(spellId);
		break;
	case SPC_PRAYER:
		pRes = new iSpell_Prayer(spellId);
		break;
	case SPC_VISION:
		pRes = new iSpell_Vision(spellId);
		break;
	default:
		check("!!! WARNING !!! - Unknown spell class!" == 0);
	};

	check(pRes);
	return pRes;
}

void iSpellMgr::InitSpells()
{
	for (uint32 xx=0; xx<MSP_COUNT; ++xx) {
		m_spells.Add(CreateSpell((MAGIC_SPELL)xx));
	}
}

void iSpellMgr::Cleanup()
{
	for (uint32 xx=0; xx<m_spells.GetSize(); xx++) {
		delete m_spells[xx];
	}
	m_spells.RemoveAll();
}

/*
 *	Heroes Manager
 */

// c-tor
iHeroesMgr::iHeroesMgr()
{
}

// d-tor
iHeroesMgr::~iHeroesMgr()
{
	Cleanup();
}

// Add hero prototype
void iHeroesMgr::AddProto(iHeroT* pProto)
{
	// add prototype
	m_protos.Add(pProto);
}

// Cleanup manager content
void iHeroesMgr::Cleanup()
{
	uint32 cnt;
	// delete all prototypes
	cnt = m_protos.GetSize();
	while (cnt--) {
		delete m_protos[cnt];
	}
	m_protos.RemoveAll();

	CleanupObjects();
}

// Cleanup objects
void iHeroesMgr::CleanupObjects()
{
	uint32 cnt;
	// delete all objects
	cnt = m_heroes.GetSize();
	while (cnt--) {
		delete m_heroes[cnt];
	}
	m_heroes.RemoveAll();
}

// Init objects
void iHeroesMgr::InitObjects()
{
	for (uint32 xx=0; xx<m_protos.GetSize(); ++xx) {
		m_heroes.Add(new iHero(m_protos[xx], 0));
	}
}

// select random hero according to specified hero type mask
uint16 iHeroesMgr::Select(uint32 heroTypeMask)
{
	iSimpleArray<uint16> idxs;
	for (uint16 xx=0; xx<(uint16)m_heroes.GetSize(); ++xx) if ((heroTypeMask & (1<<m_heroes[xx]->Proto()->m_hType))) idxs.Add(m_heroes[xx]->Proto()->m_protoId);
	if (!idxs.GetSize()) return INVALID_HERO_ID;
	return idxs[gGame.Map().Rand(idxs.GetSize())];
}

// recruit specified hero according to its prototype id
iHero* iHeroesMgr::Get(uint16 protoId)
{
	check(protoId != INVALID_HERO_ID);
	iHero* pResult = NULL;
	for (uint32 xx=0; xx<m_heroes.GetSize(); ++xx) {
		uint16 hpid = m_heroes[xx]->Proto()->m_protoId;
		if (hpid == protoId) {
			pResult = m_heroes[xx];
			m_heroes.RemoveAt(xx);
			break;
		}
	}
	check(pResult);

	return pResult;
}

// Add given hero to the list
void iHeroesMgr::Put(iHero* pHero)
{
	//OutputDebugString(iFormat(_T("- Put %s\n"),pHero->Name().CStr()).CStr());
	m_heroes.Add(pHero);
}

/*
 *
 */
iArtifactMgr::iArtifactMgr()
{
}

iArtifactMgr::~iArtifactMgr()
{
	Cleanup();
}

void iArtifactMgr::Cleanup()
{
	uint32 cnt = m_artProts.GetSize();
	while (cnt--) delete m_artProts[cnt];
	m_artProts.RemoveAll();
}

uint16 iArtifactMgr::SelectRandomArtifact(ART_LEVEL_TYPE level)
{
	if (level == ART_LEVEL_NONE) return gGame.Map().Rand(m_artProts.GetSize());
	iSimpleArray<uint16> alist;
	for (uint16 xx=0; xx<m_artProts.GetSize(); ++xx) {
		if (m_artProts[xx]->Level() == level) alist.Add(xx);
	}
	check(alist.GetSize() > 0);
	return alist[gGame.Map().Rand(alist.GetSize())];
}
/*
 *	
 */
iItemMgr::iItemMgr()
: m_gameStarted(false)
{
	m_spellMgr.InitSpells();
}

iItemMgr::~iItemMgr() 
{ 
	Cleanup(); 
}

void iItemMgr::Cleanup()
{
	uint32 cnt;

	m_heroesMgr.Cleanup();
	m_artMgr.Cleanup();

	cnt = m_VisProts.GetSize();
	while (cnt--) delete m_VisProts[cnt];
	m_VisProts.RemoveAll();

	cnt = m_OwnProts.GetSize();
	while (cnt--) delete m_OwnProts[cnt];
	m_OwnProts.RemoveAll();

	cnt = m_CastleProts.GetSize();
	while (cnt--) delete m_CastleProts[cnt];
	m_CastleProts.RemoveAll();

	cnt = m_PathElProts.GetSize();
	while (cnt--) delete m_PathElProts[cnt];
	m_PathElProts.RemoveAll();
	
	m_spellMgr.Cleanup();
}

void iItemMgr::OnGameStart()
{
	check(!m_gameStarted);
	m_heroesMgr.InitObjects();
	m_CastleRepos.InitCastles();
	m_gameStarted = true;
}

void iItemMgr::OnGameEnd()
{
	check(m_gameStarted);
	m_heroesMgr.CleanupObjects();
	m_gameStarted = false;
}

uint16 iItemMgr::AlterCtlNation(uint16 proto, CTL_TYPE nation) const 
{
	uint16 cfg = m_CastleProts[proto]->Config();
	for (uint32 xx=0; xx< m_CastleProts.GetSize(); ++xx) {
		if (cfg == m_CastleProts[xx]->Config() && nation == m_CastleProts[xx]->Type()) return xx;
	}
	check(0);
	return proto; 
}

/*
 *	
 */
void ReadCovers(iDynamicBuffer& buff, iCnstT* pCnst)
{
	uint16 cCount; buff.Read(cCount);
	uint16 aid;
	sint16 ox,oy;
	while (cCount--) {
		buff.Read(aid);
		buff.Read(ox);
		buff.Read(oy);
		pCnst->AddAvatar(aid,ox,oy);
	}
}
//////////////////////////////////////////////////////////////////////////
iVisCnstT* ReadVisitables(uint16 pidx, iDynamicBuffer& buff)
{
	iVisCnstT* pRes = NULL;
	uint16 type; buff.Read(type);
	sint16 nameKey; buff.Read(nameKey);
	sint16 msgKey; buff.Read(msgKey);
	sint16 vmsgKey; buff.Read(vmsgKey);

	if (type == VCNST_BASIC) {
		pRes = new iVisCnstT(nameKey, pidx, msgKey, vmsgKey, VCNST_BASIC, false);
	} else if (type == VCNST_STABLE) {
		sint16 actpts; buff.Read(actpts);
		pRes = new iVisCnstT_Stable(nameKey, pidx, msgKey, vmsgKey, actpts);
	} else if (type == VCNST_GAZEBO) {
		pRes = new iVisCnstT_Gazebo(nameKey, pidx, msgKey, vmsgKey);
	} else if (type == VCNST_MANASOURCE) {
		sint16 manapts; buff.Read(manapts);
		pRes = new iVisCnstT_ManaSource(nameKey, pidx, msgKey, vmsgKey, manapts);
	} else if (type == VCNST_MLMODIFIER) {
		sint8  moraleM; buff.Read(moraleM);
		sint8  luckM; buff.Read(luckM);
		pRes = new iVisCnstT_MLModifier(nameKey, pidx, msgKey, vmsgKey, moraleM, luckM);
	} else if (type == VCNST_PSMODIFIER) {
		iPrSkills ps; Unserialize(buff, ps);
		pRes = new iVisCnstT_PSModifier(nameKey, pidx, msgKey, vmsgKey, ps);
	} else if (type == VCNST_OBELISK) {
		pRes = new iVisCnstT_Obelisk(nameKey, pidx, msgKey, vmsgKey);
	} else if (type == VCNST_SIGN) {
		pRes = new iVisCnstT_Sign(nameKey, pidx, msgKey, vmsgKey);
	} else if (type == VCNST_DWELLING) {
		uint16 ctype; buff.Read(ctype);
		pRes = new iVisCnstT_Dwelling(nameKey, pidx, msgKey, vmsgKey, (CREATURE_TYPE)ctype);
	} else if (type == VCNST_WITCHHUT) {
		pRes = new iVisCnstT_WitchHut(nameKey, pidx, msgKey, vmsgKey);
	} else if (type == VCNST_SHRINE) {
		uint8 level; buff.Read(level);
		pRes = new iVisCnstT_Shrine(nameKey, pidx, msgKey, vmsgKey,(SPELL_LEVEL)level);
	} else if (type == VCNST_TREASURY) {
		iVisCnstT_Treasury* pTreasury = new iVisCnstT_Treasury(nameKey, pidx, msgKey, vmsgKey);
		pTreasury->GetVarCtr().Unserialize(buff);
		pRes = pTreasury;
	} else if (type == VCNST_TELEPORT) {
		uint8 mode; buff.Read(mode);
		uint8 index; buff.Read(index);
		pRes = new iVisCnstT_Teleport(nameKey, pidx, msgKey, vmsgKey, mode, index);
	} else if (type == VCNST_KEYMASTER) {
		uint8 key; buff.Read(key);
		pRes = new iVisCnstT_Keymaster(nameKey, pidx, msgKey, vmsgKey, key);
	} else if (type == VCNST_KNWLTREE) {
		pRes = new iVisCnstT_TreeOfKnowledge(nameKey, pidx,  msgKey, vmsgKey);
	} else if (type == VCNST_WINDMILL) {
		pRes = new iVisCnstT_Windmill(nameKey, pidx, msgKey, vmsgKey);
	} else if (type == VCNST_WEEKLYMIN) {
		pRes = new iVisCnstT_WeeklyMin(nameKey, pidx, msgKey, vmsgKey);
		::Unserialize(buff, ((iVisCnstT_WeeklyMin*)pRes)->Minerals());
	} else {
		// Unknown visitable construction
		check(pRes);
	}

	ReadCovers(buff, pRes);

	return pRes;
}
//////////////////////////////////////////////////////////////////////////
iOwnCnstT* ReadOwnerables(uint16 pidx, iDynamicBuffer& buff)
{
	iOwnCnstT* pRes = NULL;
	uint16 type; buff.Read(type);
	uint8 scouting; buff.Read(scouting);
	sint16 nameKey; buff.Read(nameKey);
	sint16 msgKey; buff.Read(msgKey);
	uint16 msgIcn; buff.Read(msgIcn);

	if (type == OCNST_BASIC) {
		pRes = new iOwnCnstT(nameKey, pidx, msgKey, msgIcn, OCNST_BASIC, scouting);	
	} else if (type == OCNST_FURTSKILL) {
		iFurtSkills fs;
		::Unserialize(buff, fs);
		pRes = new iOwnCnstT_FurtSkill(nameKey, pidx, msgKey, msgIcn, scouting,fs);
	} else {
		// Unknown ownerable construction
		check(pRes);
	}

	ReadCovers(buff, pRes);

	return pRes;
}

//////////////////////////////////////////////////////////////////////////
iArtT* ReadArtifacts(iDynamicBuffer& buff)
{
	iArtT* pRes = NULL;

	sint16 nameKey; buff.Read(nameKey);
	sint16 descKey; buff.Read(descKey);
	uint16 icon; buff.Read(icon);
	uint16 type; buff.Read(type);
	uint8 level; buff.Read(level);
	uint8 assign; buff.Read(assign);
	uint16 reqLevel; buff.Read(reqLevel);
	sint8 reqSecSkill; buff.Read(reqSecSkill);
	if (type == ARTT_FURTSKILL) {
		iFurtSkills fs;
		Unserialize(buff, fs);
		pRes = new iArtT_FurtSkill(nameKey, descKey, (ART_LEVEL_TYPE)level, (ART_ASSIGN_TYPE)assign ,reqLevel, (SECONDARY_SKILLS)reqSecSkill,icon, fs);
	} else if (type == ARTT_NEGSPHERE) {
		pRes = new iArtT_NegSphere(nameKey, descKey, (ART_LEVEL_TYPE)level, (ART_ASSIGN_TYPE)assign ,reqLevel, (SECONDARY_SKILLS)reqSecSkill,icon);
	} else if (type == ARTT_SHOFWAR) {
		pRes = new iArtT_ShOfWar(nameKey, descKey, (ART_LEVEL_TYPE)level, (ART_ASSIGN_TYPE)assign ,reqLevel, (SECONDARY_SKILLS)reqSecSkill,icon);
	} else if (type == ARTT_CURSWORD) {
		pRes = new iArtT_CurSword(nameKey, descKey, (ART_LEVEL_TYPE)level, (ART_ASSIGN_TYPE)assign ,reqLevel, (SECONDARY_SKILLS)reqSecSkill,icon);
	} else {
		// Unknown type of artifact
		check(pRes);
	}

	return pRes;
}

//////////////////////////////////////////////////////////////////////////
bool LoadObjectTemplates(iDynamicBuffer& buff, iItemMgr& imgr)
{
	// Heroes
	uint16 hCount;
	buff.Read(hCount);
	uint16 pidx=0;
	while (hCount--) {
		// Type/Nation
		uint8 hType; buff.Read(hType);
		// Icons
		uint16 icn32,icn48; buff.Read(icn32); buff.Read(icn48);
		// Name key
		sint16 nameKey; buff.Read(nameKey);
		// Create object prototype
		iHeroT* pHero = new iHeroT(pidx++, nameKey, (HERO_TYPE)hType, icn32, icn48);
		// FurtSkills
		Unserialize(buff, pHero->m_furtSkills);
		// Secondary skills
		Unserialize(buff, pHero->m_secSkills);
		// Initial spells
		Unserialize(buff, pHero->m_spells);
		// Cost
		Unserialize(buff, pHero->m_Cost);
		// Add prototype to Heroe's manager
		imgr.HeroesMgr().AddProto(pHero);
	}


	// Path Elements
	uint16 pecount;
	buff.Read(pecount);
	while(pecount--) {
		uint8 type; buff.Read(type);
		sint16 nameKey; buff.Read(nameKey);
		uint16 spriteId; buff.Read(spriteId);
		uint8 level; buff.Read(level);
		sint8 passMod; buff.Read(passMod);
		imgr.m_PathElProts.Add(new iPathElT(nameKey, (PATH_ELEMENT_TYPE)type, spriteId, (SPRITE_LEVEL)level, passMod));
	}

	// Visitables
	uint16 vCount;
	buff.Read(vCount);
	pidx = 0;
	while (vCount--) {
		imgr.m_VisProts.Add(ReadVisitables(pidx++, buff));
	}

	// Ownerables
	uint16 oCount;
	buff.Read(oCount);
	pidx = 0;
	while (oCount--) {
		imgr.m_OwnProts.Add(ReadOwnerables(pidx++, buff));
	}

	// Castles
	uint16 cCount;
	buff.Read(cCount);
	pidx = 0;
	while (cCount--) {
		uint8 type, size, orient;
		uint16 cfg;
		buff.Read(type);
		buff.Read(size);
		buff.Read(orient);
		buff.Read(cfg);
		iCastleT* pCastle = new iCastleT(pidx++, (CTL_TYPE)type,(CTL_SIZE)size,(CTL_ORIENT)orient, cfg);
		imgr.m_CastleProts.Add(pCastle);
		ReadCovers(buff, pCastle);
	}

	// Artifacts
	uint16 aCnt;
	buff.Read(aCnt);
	while (aCnt--) {
		imgr.m_artMgr.AddArtifact(ReadArtifacts(buff));
	}

	// Battle obstacles
	uint16 boCnt;
	buff.Read(boCnt);
	while (boCnt--) {
		uint16 surfMask; buff.Read(surfMask);
		uint16 sprite; buff.Read(sprite);
		iPoint offset; Unserialize(buff, offset);
		uint8 size; buff.Read(size);
		uint8 level; buff.Read(level);
		imgr.m_batObstMgr.Add(surfMask, sprite, offset, size, level);
	}

	// read credits
	uint16 cnt;
	buff.Read(cnt);
	while(cnt--) {
		iStringT text;
		buff.Read(text);
		imgr.m_credits.Add(text);
	}

	return true;
}

bool LoadResources(iItemMgr& imgr)
{
	// Open and validate the object file
	iFilePtr pFile = OpenWin32File(gDataPath+_T("objects.dat"));
	if (!pFile) {
		MessageBox(NULL, _T("Unable to open game objects file!"), NULL, MB_OK);
		return false;
	}
	iDynamicBuffer fileBuff(pFile->GetSize());
	fileBuff.IncSize(pFile->GetSize() );
	pFile->Read(fileBuff.GetData(),pFile->GetSize());
	pFile.reset();
	uint32 hdr; fileBuff.Read(hdr);
	uint16 ver; fileBuff.Read(ver);
	if (hdr != GOBJ_FILE_HDR_KEY || ver != GOBJ_FILE_VERSION) {
		MessageBox(NULL, _T("Game objects file is corrupted!"), NULL, MB_OK);
		return false;
	}

	// Decompress
	iDynamicBuffer dbuff;
	LZO::Decompress((const unsigned char*)fileBuff.GetCurrentData(), fileBuff.GetSize() - fileBuff.GetCurrentPos(), dbuff);
	fileBuff.Clean();


	// Loading avatars
	if (!LoadAvatars(dbuff, imgr.m_AvatarMgr)) {
		MessageBox(NULL, _T("Unable to load game avatars!"), NULL, MB_OK);
		return false;
	}
	
	// Loading object templates
	if (!LoadObjectTemplates(dbuff, imgr)) {
		MessageBox(NULL, _T("Unable to load game object templates!"), NULL, MB_OK);
		return false;
	}
	
	return true;
}

