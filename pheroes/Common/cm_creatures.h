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

#ifndef _HMM_COMMON_CREATURES_DEFINE_H_
#define _HMM_COMMON_CREATURES_DEFINE_H_

/*
 *	Creatures
 */
enum CREATURE_TYPE {
	CREAT_UNKNOWN = -1,
	/* Knight */
	CREAT_PEASANT = 0,
	CREAT_ARCHER,
	CREAT_PIKEMAN,
	CREAT_MONK,
	CREAT_CAVALRY,
	CREAT_PALADIN,

	/* Barbarian */
	CREAT_GOBLIN,
	CREAT_ORC,
	CREAT_WARG_RIDER,
	CREAT_OGRE,
	CREAT_TROLL,
	CREAT_CYCLOP,

	/* Wizard */
	CREAT_YOUNG_MAGE,
	CREAT_WHITE_WOLF,
	CREAT_LIVING_ARMOR,
	CREAT_PEGASUS,
	CREAT_MAGE,
	CREAT_THOR,
	
	/* Warlock */
	CREAT_CENTAUR,
	CREAT_GARGOYLE,
	CREAT_GRIFFIN,
	CREAT_MINOTAUR,
	CREAT_HYDRA,
	CREAT_RED_DRAGON,
		
	/* Sorcer */
	CREAT_SPRITE,
	CREAT_DWARF,
	CREAT_ELF,
	CREAT_DRUID,
	CREAT_UNICORN,
	CREAT_FIREBIRD,
	
	/* Necromant */
	CREAT_SKELETON,
	CREAT_ZOMBIE,
	CREAT_LICH,
	CREAT_VAMPIRE,
	CREAT_BLACK_KNIGHT,
	CREAT_PLAGUE,

	/* Neutral = 9 */
	CREAT_ROGUE,
	CREAT_NOMAD,
	CREAT_GHOST,
	CREAT_GENIE,
	CREAT_MEDUSA,
	CREAT_EARTH_ELEMENTAL,
	CREAT_AIR_ELEMENTAL,
	CREAT_FIRE_ELEMENTAL,
	CREAT_WATER_ELEMENTAL,

	CREAT_COUNT,

	/* Random values */
	CREAT_RANDOM		= 0x0F00,
	CREAT_RANDOM_L1		= CREAT_RANDOM | 1,
	CREAT_RANDOM_L2		= CREAT_RANDOM | 2,
	CREAT_RANDOM_L3		= CREAT_RANDOM | 3,
	CREAT_RANDOM_L4		= CREAT_RANDOM | 4,
	CREAT_RANDOM_L5		= CREAT_RANDOM | 5,
	CREAT_RANDOM_L6		= CREAT_RANDOM | 6,
};
//////////////////////////////////////////////////////////////////////////
const uint32 CPERKS_COUNT = 21;
enum CREAT_PERK {
	CPERK_NONE				= 0,
	CPERK_DOUBLESHOT		= 0x00000001,	// Shots twice (rangers, grand elves, etc)
	CPERK_DOUBLEATTACK		= 0x00000002,	// Attack the target twice (paladins, wolves, etc)
	CPERK_NOMELEEPENALTY	= 0x00000004,	// scores full damage even at melee
	CPERK_NONRETALATTACK	= 0x00000008,	// Non retaliated attack (vampires, rogues, sprites, hydras, etc)
	CPERK_RETALTOALL		= 0x00000010,	// Retaliates against every attack (griffins)
	CPERK_TWOHEXATTACK		= 0x00000020,	// Two-hex attack (dragons)
	CPERK_ADJACENTATTACK	= 0x00000040,	// Attacks all adjacent enemies (hydras)
	CPERK_LICHSHOOT			= 0x00000080,	// Range attack affects adjacent hexes except undeads (Liches)
	CPERK_UNDEAD			= 0x00000100,	// All necropolis creatures + ghosts
	CPERK_LIFELESS			= 0x00000200,	// Golems, gargoyles, elementals
	CPERK_REGENERATES		= 0x00000400,	// Trolls
	CPERK_JOUSTING			= 0x00000800,	// Cavalry
	CPERK_AIRMAGICIMM		= 0x00001000,	// Air elementals
	CPERK_EARTHMAGICIMM		= 0x00002000,	// Earth elementals
	CPERK_FIREMAGICIMM		= 0x00004000,	// Fire elementals, Phoenix
	CPERK_WATERMAGICIMM		= 0x00008000,	// Water elementals
	CPERK_40PROCRESIST		= 0x00010000,	// 40% magic resistance (dwrves)
	CPERK_QUARTERDMG		= 0x00020000,	// receives only qurter damage from damage spells (golems)
	CPERK_GHOST				= 0x00040000,	// Ghost perk
	CPERK_DOHALF			= 0x00080000,	// Genie perk
	CPERK_DRAINLIFES		= 0x00100000,	// Drains life (vampires)
	CPERK_ALLMAGICIMM		= CPERK_AIRMAGICIMM | CPERK_EARTHMAGICIMM | CPERK_FIREMAGICIMM | CPERK_WATERMAGICIMM	// Black draggons
};

//////////////////////////////////////////////////////////////////////////
enum CREATURE_SPEED {
	SPEED_SUPERSLOW	= 1,
	SPEED_ULTRASLOW = 2,
	SPEED_VERYSLOW = 3,
	SPEED_EXTRASLOW = 4,
	SPEED_SLOW = 5,
	SPEED_SWIFT = 6,
	SPEED_EXTRASWIFT = 7,
	SPEED_VERYSWIFT = 8,  
	SPEED_ULTRASWIFT = 9,	// (2 to 2)
	SPEED_SUPERSWIFT = 10,	// (1 to 2 or 2 to 1)
	SPEED_QUICK = 11,		// (1 to 1)
	SPEED_EXTRAQUICK = 12,
	SPEED_VERYQUICK = 13,
	SPEED_ULTRAQUICK = 14,
	SPEED_SUPERQUICK = 15,
	SPEED_FAST = 16,
	SPEED_EXTRAFAST = 17,
	SPEED_VERYFAST = 18,
	SPEED_ULTRAFAST = 19,
	SPEED_SUPERFAST = 20,
	SPEED_MAX
};
//////////////////////////////////////////////////////////////////////////
enum TRANS_TYPE {
	TRANS_WALK = 0,
	TRANS_FLY
};
//////////////////////////////////////////////////////////////////////////
struct CREAT_DESCRIPTOR {
	uint8	level;		// unit level (1-6)
	uint8	nation;		// unit alignment

	uint8	attack;		// attack skill
	uint8	defence;	// deffence skill
	uint8	speed;		// unit speed (1-20)
	uint8	size;		// unit size (1 or 2 - used in battle)

	uint8	transType;	// transportation method
	uint8	shots;		// 0 - means no range attack
	uint16	hits;		// hit points (health)
	uint16	damage_min;	// minimum damage
	uint16	damage_max;	// maximum damage

	iMineralSet	cost;	// cost per unit
	uint16		growth;	// growth rate
	uint16		pidx;	// power index (used for AI)
	uint32		perks;	// creature perks
};

extern const CREAT_DESCRIPTOR CREAT_DESC[CREAT_COUNT];
extern const sint32 INITIAL_HERO_ARMY[HERO_TYPE_COUNT][2][3];

#endif //_HMM_COMMON_CREATURES_DEFINE_H_

