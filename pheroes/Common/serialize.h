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

#ifndef _HMM_COMMON_SERIALIZE_H_
#define _HMM_COMMON_SERIALIZE_H_

// Point
inline void Serialize(iDynamicBuffer& dbuff, const iPoint& pos)
{
	dbuff.Write((sint16)pos.x);
	dbuff.Write((sint16)pos.y);
}

inline void Unserialize(iDynamicBuffer& dbuff, iPoint& pos)
{
	sint16 x,y;
	dbuff.Read(x); dbuff.Read(y);
	pos.x=x; pos.y=y;
}

inline void Unserialize(iFileI* pFile, iPoint& pos)
{
	sint16 x,y;
	pFile->Read(&x, sizeof(x));
	pFile->Read(&y, sizeof(y));
	pos.x=x; pos.y=y;
}


// Creature group and army
inline void Serialize(iDynamicBuffer& dbuff, const iCreatGroup& cg)
{
	dbuff.Write((sint16)cg.Type());
	dbuff.Write((sint32)cg.Count());
}

inline void Unserialize(iDynamicBuffer& dbuff, iCreatGroup& cg)
{
	sint16 ct;
	sint32 cnt;
	dbuff.Read(ct);
	dbuff.Read(cnt);
	cg.Type() = (CREATURE_TYPE)ct;
	cg.Count() = cnt;
}

inline void Serialize(iDynamicBuffer& dbuff, const iArmy& army)
{
	for (uint32 xx=0; xx<7; ++xx) Serialize(dbuff, army[xx]);
}

inline void Unserialize(iDynamicBuffer& dbuff, iArmy& army)
{
	for (uint32 xx=0; xx<7; ++xx) Unserialize(dbuff, army[xx]);
}

// String of text
inline void Serialize(iDynamicBuffer& dbuff, const iStringT& text)
{
	dbuff.Write(text);
}

inline void Unserialize(iDynamicBuffer& dbuff, iStringT& text)
{
	dbuff.Read(text);
}

inline void Unserialize(iFileI* pFile, iStringT& text)
{
	uint32 len;
	pFile->Read(&len, sizeof(len));
	if ( len ) {
		iCharT* buff = new iCharT[len];
		sint32 bcnt = len*sizeof(iCharT);
		pFile->Read(buff,bcnt);
		text.Set(buff,len);
		delete[] buff;
	} else {
		text = _T("");
	}
}

// Furt skills
inline void Serialize(iDynamicBuffer& dbuff, const iFurtSkills& frSkills)
{
	uint16 cnt = 0;
	uint16 xx;
	for (xx=0; xx<FSK_COUNT; ++xx) if (frSkills.Value((FURTHER_SKILLS)xx)) ++cnt;
	dbuff.Write(cnt);
	for (xx=0; xx<FSK_COUNT; ++xx) {
		FURTHER_SKILLS sk = (FURTHER_SKILLS)xx ;
		if (frSkills.Value(sk)) {
			dbuff.Write(xx);
			dbuff.Write((sint16)frSkills.Value(sk));
		}
	}
}

inline void Unserialize(iDynamicBuffer& dbuff, iFurtSkills& frSkills)
{
	uint16 cnt;
	dbuff.Read(cnt);
	if (!cnt) return;
	frSkills.Reset();
	while (cnt--) {
		uint16 sk;
		sint16 mod;
		dbuff.Read(sk);
		dbuff.Read(mod);
		check(frSkills.Value((FURTHER_SKILLS)sk) == 0);
		frSkills.Value((FURTHER_SKILLS)sk) = mod;
	}
}

// Primary skills
inline void Serialize(iDynamicBuffer& dbuff, const iPrSkills& prSkills)
{
	for (uint32 xx=0; xx<PRSKILL_COUNT; ++xx) dbuff.Write(prSkills.val[xx]);
}

inline void Unserialize(iDynamicBuffer& dbuff, iPrSkills& prSkills)
{
	for (uint32 xx=0; xx<PRSKILL_COUNT; ++xx) {
		dbuff.Read(prSkills.val[xx]);
	}
}

// Mineral Set
// NOTE: Minerals now must be encrypted to:
// a) prevent modification 
// b) checking real values to detect protection
inline void Serialize(iDynamicBuffer& dbuff, const iMineralSet& minSet)
{
	for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) dbuff.Write(minSet.quant[xx]);
}

inline void Unserialize(iDynamicBuffer& dbuff, iMineralSet& minSet)
{
	for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) {
		dbuff.Read(minSet.quant[xx]);
	}
}


inline void Unserialize(iFileI* pFile, iMineralSet& minSet)
{
	for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) {
		pFile->Read(&minSet.quant[xx], sizeof(minSet.quant[xx]));
	}
}

// Secondary Skills
inline void Serialize(iDynamicBuffer& dbuff, const iSecSkills& secSkills)
{
	dbuff.Write((uint8)secSkills.GetSize());
	for (uint32 ssid=0; ssid<secSkills.GetSize(); ++ssid) {
		dbuff.Write(secSkills[ssid].m_skill);
		dbuff.Write(secSkills[ssid].m_level);
	}
}

inline void Unserialize(iDynamicBuffer& dbuff, iSecSkills& secSkills)
{
	uint8 sscnt;
	sint8 ssType, ssLevel;
	dbuff.Read(sscnt);
	if (!sscnt) return;
	secSkills.RemoveAll();
	for (uint8 ssid=0; ssid<sscnt; ++ssid) {
		dbuff.Read(ssType);
		dbuff.Read(ssLevel);
		secSkills.Add(iSecSkillEntry((SECONDARY_SKILLS)ssType, (SECSKILL_LEVEL)ssLevel));
	}
}


// Artifact list
inline void Serialize(iDynamicBuffer& dbuff, const iArtifactList& artifacts)
{
	dbuff.Write((uint16)artifacts.Count());
	for (uint32 artIdx=0; artIdx<artifacts.Count(); ++artIdx) {
		dbuff.Write((uint16)artifacts.At(artIdx).id);
		dbuff.Write((sint16)artifacts.At(artIdx).assign);
	}
}

inline void Unserialize(iDynamicBuffer& dbuff, iArtifactList& artifacts)
{
	uint16 artCnt; 
	dbuff.Read(artCnt);
	while (artCnt--) {
		uint16 artId;
		sint16 assign;
		dbuff.Read(artId);
		dbuff.Read(assign);
		artifacts.Add(artId, (HERO_ART_CELL)assign);
	}
}

// Spells
inline void Serialize(iDynamicBuffer& dbuff, const iSpellList& spells)
{
	dbuff.Write((uint16)spells.GetSize());
	for (uint32 spIdx=0; spIdx<spells.GetSize(); ++spIdx) dbuff.Write(spells[spIdx]);
}

inline void Unserialize(iDynamicBuffer& dbuff, iSpellList& spells)
{
	uint16 spCnt; 
	dbuff.Read(spCnt);
	if (!spCnt) return;
	spells.RemoveAll();
	while (spCnt--) {
		uint16 spId;
		dbuff.Read(spId);
		if (spId < MSP_COUNT) spells.Add(spId);
	}
}

// Rewards
inline void Serialize(iDynamicBuffer& dbuff, const iRewardsCtr& rewards)
{
	uint16 quant = (uint16)rewards.GetSize();
	dbuff.Write(quant);
	for (uint16 xx=0; xx<quant; ++xx) {
		dbuff.Write((uint8)rewards[xx].m_type);
		dbuff.Write(rewards[xx].m_fParam);
		dbuff.Write(rewards[xx].m_sParam);
	}
}

inline void Unserialize(iDynamicBuffer& dbuff, iRewardsCtr& rewards)
{
	rewards.RemoveAll();
	uint16 quant;
	dbuff.Read(quant);
	while (quant--) {
		uint8 rtype;
		dbuff.Read(rtype);
		sint32 fparam, sparam;
		dbuff.Read(fparam);
		dbuff.Read(sparam);
		rewards.Add(iRewardItem((REWARD_ITEM_TYPE)rtype,fparam,sparam));
	}
}

#endif //_HMM_COMMON_SERIALIZE_H_

