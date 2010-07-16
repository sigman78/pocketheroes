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

#ifndef _HMM_COMMON_MAGIC_SPELLS_DEFINE_H_
#define _HMM_COMMON_MAGIC_SPELLS_DEFINE_H_

// Spell type
enum SPELL_TYPE {
	SPT_OVERLAND = 0,
	SPT_COMBAT,
	SPT_COUNT
};

enum SPELL_TYPE_MASK {
	SPT_MASK_OVERLAND	=	0x01,
	SPT_MASK_COMBAT		=	0x02,
	SPT_MASK_ALL		=	SPT_MASK_OVERLAND | SPT_MASK_COMBAT
};

// Spell level
enum SPELL_LEVEL {
	SPL_FIRST = 0,
	SPL_SECOND,
	SPL_THIRD,
	SPL_FOURTH,
	SPL_FIFTH,
	SPL_COUNT
};

enum SPELL_LEVEL_MASK {
	SPL_MASK_NONE	=	0x00,
	SPL_MASK_FIRST	=	0x01,
	SPL_MASK_SECOND	=	0x02,
	SPL_MASK_THIRD	=	0x04,
	SPL_MASK_FOURTH	=	0x08,
	SPL_MASK_FIFTH	=	0x10,
	SPL_MASK_ALL	=	SPL_MASK_FIRST | SPL_MASK_SECOND | SPL_MASK_THIRD | SPL_MASK_FOURTH | SPL_MASK_FIFTH
};

extern const uint32 SPELL_LEVEL_MASKS[SPL_COUNT];

// Magic school type
enum MAGIC_SCHOOL {
	MST_AIR = 0,
	MST_EARTH,
	MST_FIRE,
	MST_WATER,
	MST_COUNT
};

// Magic school secondary skills (Air, Earth, Fire, Water)
extern const SECONDARY_SKILLS MSCH_SECSKILLS[MST_COUNT];

enum MAGIC_SCHOOL_MASK {
	MST_MASK_AIR	=	0x01,
	MST_MASK_EARTH	=	0x02,
	MST_MASK_FIRE	=	0x04,
	MST_MASK_WATER	=	0x08,
	MST_MASK_GOOD	=	MST_MASK_AIR | MST_MASK_EARTH | MST_MASK_WATER,
	MST_MASK_EVIL	=	MST_MASK_AIR | MST_MASK_EARTH | MST_MASK_FIRE,
	MST_MASK_ALL	=	MST_MASK_AIR | MST_MASK_EARTH | MST_MASK_WATER | MST_MASK_FIRE,
};


// Spell classes
enum SPELL_CLASS {
	SPC_DAMAGE,			// fparam = damage
	SPC_FURTSKILL,		// fparam = skill, sparam = modifier
	SPC_DISRAY,			// fparam = defence modifier
	SPC_RESURRECT,		// fparam = hit points, sparam = true/not
	SPC_SUMMON,			// fparam = creature type, sparam = quantity
	SPC_BLESS,			// fparam = modifier (+1, 0, -1), sparam = anti spell
	SPC_BLIND,			// fparam = retail power (%), sparam = not used
	SPC_EARTHQUAKE,		// fparam = walls ammount
	SPC_DISPEL,			// - none -
	SPC_CURE,			// fparam = hit points per spell power
	SPC_TOWNPORTAL,		// fparam = (_?_)
	SPC_ANTIMAGIC,		// fparam = maxLevel
	SPC_PRAYER,			// fparam = value
	SPC_VISION,			// fparam = radius
	SPC_COUNT
};

// Target type mask (used for combat spells)
enum SPELL_TARGET_TYPE_MASK {
	STT_NONE		= 0x00,
	STT_FRIENDLY	= 0x01,
	STT_ENEMY		= 0x02,
	STT_UNDEAD		= 0x04,
	STT_LIFELESS	= 0x08,
	STT_NORMAL		= 0x10,
	STT_TROOPS		= 0x20,
	STT_SHOOTERS	= 0x40,
	STT_FRUNDEADS	= STT_FRIENDLY | STT_UNDEAD | STT_TROOPS | STT_SHOOTERS ,
	STT_FRNUNDEADS	= STT_FRIENDLY | STT_LIFELESS | STT_NORMAL | STT_TROOPS | STT_SHOOTERS ,
	STT_ENNUNDEADS	= STT_ENEMY | STT_LIFELESS | STT_NORMAL | STT_TROOPS | STT_SHOOTERS ,
	STT_ENNORMALS	= STT_ENEMY | STT_NORMAL | STT_TROOPS | STT_SHOOTERS ,
	STT_ALLUNDEADS	= STT_FRIENDLY | STT_ENEMY | STT_UNDEAD | STT_TROOPS | STT_SHOOTERS ,
	STT_ALLNUNDEADS	= STT_FRIENDLY | STT_ENEMY | STT_NORMAL | STT_LIFELESS | STT_TROOPS | STT_SHOOTERS ,
	STT_ALLNORMALS	= STT_FRIENDLY | STT_ENEMY | STT_NORMAL | STT_TROOPS | STT_SHOOTERS ,
	STT_FRNORMALS	= STT_FRIENDLY | STT_NORMAL | STT_TROOPS | STT_SHOOTERS ,
	STT_FRTROOPS	= STT_FRIENDLY | STT_UNDEAD | STT_LIFELESS | STT_NORMAL | STT_TROOPS,
	STT_FRSHOOTERS	= STT_FRIENDLY | STT_UNDEAD | STT_LIFELESS | STT_NORMAL | STT_SHOOTERS,
	STT_ALLFRIENDLY	= STT_FRIENDLY | STT_UNDEAD | STT_LIFELESS | STT_NORMAL | STT_TROOPS | STT_SHOOTERS,
	STT_ALLENEMY	= STT_ENEMY | STT_UNDEAD | STT_LIFELESS | STT_NORMAL | STT_TROOPS | STT_SHOOTERS,
	STT_ALL			= STT_FRIENDLY | STT_ENEMY | STT_UNDEAD | STT_LIFELESS | STT_NORMAL | STT_TROOPS | STT_SHOOTERS 
};

// Target mode (used for combat spells)
enum SPELL_TARGET_MODE {
	STM_NONE = 0,
	STM_SUMMON,
	STM_EARTHQUAKE,
	STM_CREAT_GROUP,
	STM_BALL_SET,
	STM_BLAST_SET,
	STM_RING_SET,
	STM_ALL
};

// Spell list
enum MAGIC_SPELL {
	MSP_INVALID = -1,
	// Air magic 15-3 = 12
	MSP_MAGICARROW,
	MSP_PROTEARTH,
	MSP_HASTE,
	MSP_SHIELD,
	MSP_DISRAY,
	MSP_LIGHTNINGBOLT,
	MSP_PRECISION,
	MSP_AIRSHIELD,
	MSP_HOLYWORD,
	MSP_COUNTERSTRIKE,
	MSP_RESURRECT,
	MSP_AIRELEMENTAL,

	// Earth magic 13-3 = 10
	MSP_PROTAIR,
	MSP_SLOW,
	MSP_STONESKIN,
	MSP_VISIONS,
	MSP_EARTHQUAKE,
	MSP_SORROW,
	MSP_METEORSHOWER,
	MSP_TOWNPORTAL,
	MSP_IMPLOSION,
	MSP_EARTHELEMENTAL,

	// Fire magic 15-3 = 12
	MSP_BLOODLUST,
	MSP_PROTWATER,
	MSP_CURSE,
	MSP_BLIND,
	MSP_WEAKNESS,
	MSP_DEATHRIPPLE,
	MSP_FIREBALL,
	MSP_MISFORTUNE,
	MSP_ANIMATEDEAD,
	MSP_FIREBLAST,
	MSP_ARMAGEDDON,
	MSP_FIREELEMENTAL,

	// Water magic 15-4 = 11
	MSP_BLESS,
	MSP_PROTFIRE,
	MSP_DISPEL,
	MSP_CURE,
	MSP_COLDRAY,
	MSP_FORTUNE,
	MSP_MIRTH,
	MSP_COLDRING,
	MSP_ANTIMAGIC,
	MSP_PRAYER,
	MSP_WATERELEMENTAL,

	// New spells
	MSP_SUMMONSPRITES,

	//
	MSP_COUNT
};

// Spell disposition
enum SPELL_DISPOS {
	SPD_NONE = -1,
	SPD_NEUTRAL = 0,
	SPD_POSITIVE,
	SPD_NEGATIVE,
	SPD_COUNT
};

// Spell Labels
enum SPELL_LABEL {
	SLB_NONE = 0xFF,
	SLB_PROTEARTH = 0,
	SLB_HASTE,
	SLB_SHIELD,
	SLB_PRECISION,
	SLB_AIRSHIELD,
	SLB_COUNTERSTRIKE,
	SLB_PROTAIR,
	SLB_SLOW,
	SLB_STONESKIN,
	SLB_SORROW,
	SLB_BLOODLUST,
	SLB_PROTWATER,
	SLB_CURSE,
	SLB_BLIND,
	SLB_WEAKNESS,
	SLB_MISFORTUNE,
	SLB_BLESS,
	SLB_PROTFIRE,
	SLB_FORTUNE,
	SLB_MIRTH,
	SLB_ANTIMAGIC,
	SLB_PRAYER,
	SLB_DISRAY,
	SLB_COUNT,
};

// Enum spell level
enum MAGIC_SCHOOL_LEVEL {
	MSL_NONE = 0,
	MSL_BASIC,
	MSL_ADVANCED,
	MSL_EXPERT,
	MSL_COUNT
};

// spell effect descriptor (4 x 2 = 8 bytes)
struct SPELL_EFFECT_DESC {
	uint16		tgtType;
	uint16		tgtMode;

	sint16		fparam;
	sint16		sparam;
};

// spell descriptor ( 8 + 8 x 4 = 40 bytes)
struct SPELL_DESC_STRUCT {
	uint8		type;
	uint8		level;
	uint8		school;
	uint8		spClass;
	uint8		bcost;
	uint8		label;
	uint16		reserved;

	SPELL_EFFECT_DESC	eff[MSL_COUNT];
};

extern const SECONDARY_SKILLS MSCH_SECSKILLS[MST_COUNT];
extern const SPELL_DESC_STRUCT SPELL_DESCRIPTORS[MSP_COUNT];

// Spell filter
struct iSpellFilter {
	uint8	typeMask;
	uint8	levelMask;
	uint8	schoolMask;
	uint8	reserved;
};
inline iSpellFilter SpellFilter(uint8 _typeMask, uint8 _levelMask, uint8 _schoolMask)
{ iSpellFilter result; result.typeMask = _typeMask; result.levelMask=_levelMask; result.schoolMask=_schoolMask; return result; }

typedef iSimpleArray<uint16> iSpellList;

#endif //_HMM_COMMON_MAGIC_SPELLS_DEFINE_H_
