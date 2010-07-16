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

const uint32 SURF_TYPE_MASK[STYPE_COUNT] = {
	1 << STYPE_WATER,
	1 << STYPE_SAND,
	1 << STYPE_DIRT,
	1 << STYPE_GRASS,
	1 << STYPE_SWAMP,
	1 << STYPE_LAVA,
	1 << STYPE_WASTELAND,
	1 << STYPE_DESERT,
	1 << STYPE_SNOW,
	1 << STYPE_NDESERT,
	1 << STYPE_PAVEMENT,
	1 << STYPE_NWASTELAND,
};

const uint8 SURF_MOVE_COST[STYPE_COUNT] = {
	255,	// Water
	12,		// Sand
	6,		// Dirt
	6,		// Grass
	14,		// Swamp
	10,		// Lava
	8,		// Wasteland
	12,		// Desert
	10,		// Snow
	12,		// new desert
	4,		// Pavement
	9,		// new wasteland
};


const uint16 SURF_COLOR[STYPE_COUNT] = {
	RGB16(8, 28, 128),	// Water
	RGB16(214,182,148),	// Sand
	RGB16(99,48,8),		// Dirt
	RGB16(24,97,16),	// Grass
	RGB16(0,44,0),		// Swamp
	RGB16(48,48,48),	// Lava
	RGB16(165,85,16),	// Wasteland
	RGB16(181,138,24),	// Desert
	RGB16(220,220,220),	// Snow
	RGB16(192,160,0),	// new Desert
	RGB16(160,160,160),	// Pavement
	RGB16(192,192,160),	// new wasteland
};

const uint32 MAP_SIZ_SIZE[MSIZ_COUNT] = {
	32,
	64,
	128,
	256
};

//////////////////////////////////////////////////////////////////////////
const iMineralSet INITIAL_RESOURCES[2][DFC_COUNT] = {
	{
		// For Human
		//	Gold,	Ore,	Wood,	Merc,	Gems,	Cryst,	Sulfur
		{	15000,	30,		30,		15,		15,		15,		15		},	// Easy
		{	10000,	20,		20,		10,		10,		10,		10		},	// Normal
		{	5000,	10,		10,		5,		5,		5,		5		},	// Hard
		{	2500,	5,		5,		2,		2,		2,		2		},	// Expert
		{	0,		0,		0,		0,		0,		0,		0		}	// Impossible
	},{
		// For Computer
		//	Gold,	Ore,	Wood,	Merc,	Gems,	Cryst,	Sulfur
		{	2500,	5,		5,		2,		2,		2,		2		},	// Easy
		{	5000,	10,		10,		5,		5,		5,		5		},	// Normal
		{	15000,	30,		30,		15,		15,		15,		15		},	// Hard
		{	25000,	50,		50,		25,		25,		25,		25		},	// Expert
		{	50000,	80,		80,		40,		40,		40,		40		}	// Impossible
	}
};

//////////////////////////////////////////////////////////////////////////
const uint16 PLAYER_COLORS[PID_COUNT] = {
	0x1F<<11,			// Red
	0x30<<5,			// Green
	0x1F,				// Blue
	0x30<<5 | 0x1F,		// Cyan
	0x1D<<11 | 0x1F,	// Magenta
	0x1D<<11 | 0x38<<5	// Yellow
};

LPCTSTR const PLAYER_TEXT_COLORS[PID_COUNT] = {
	_T("#FF99"),
	_T("#F8E8"),
	_T("#F99F"),
	_T("#F7EE"),
	_T("#FE7E"),
	_T("#FEE7")
};

LPCTSTR const PLAYER_WORD_COLORS[PID_COUNT] = {
	_T("Red"),
	_T("Green"),
	_T("Blue"),
	_T("Cyan"),
	_T("Magenta"),
	_T("Yellow")
};

const IDEOLOGY_TYPE NATION_TYPE_IDEOLOGY[NATION_COUNT] = {
	IDEOLOGY_NEUTRAL,
	IDEOLOGY_GOOD,
	IDEOLOGY_EVIL,
	IDEOLOGY_GOOD,
	IDEOLOGY_EVIL,
	IDEOLOGY_GOOD,
	IDEOLOGY_EVIL
};

const IDEOLOGY_TYPE HERO_TYPE_IDEOLOGY[HERO_TYPE_COUNT] = {
	IDEOLOGY_GOOD,
	IDEOLOGY_EVIL,
	IDEOLOGY_GOOD,
	IDEOLOGY_EVIL,
	IDEOLOGY_GOOD,
	IDEOLOGY_EVIL
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
const ART_ASSIGN_TYPE HERO_ART_CELL_ASSIGN[AC_COUNT] = {
	ART_ASSIGN_HEAD,
	ART_ASSIGN_NECK,
	ART_ASSIGN_TORSO,
	ART_ASSIGN_HANDS,
	ART_ASSIGN_HANDS,
	ART_ASSIGN_FINGERS,
	ART_ASSIGN_FINGERS,
	ART_ASSIGN_SHOULDERS,
	ART_ASSIGN_LEGS,
	ART_ASSIGN_FEET,
	ART_ASSIGN_MISC,
	ART_ASSIGN_MISC,
	ART_ASSIGN_MISC,
	ART_ASSIGN_MISC
};

//
const sint32 ULTART_STDMODIF[HERO_TYPE_COUNT][7][2] = {
	{
		// Knight
		{FSK_MIN_GOLD, 5000}, {FSK_ATTACK, 4}, {FSK_DEFENCE, 4}, {FSK_POWER, 4}, {FSK_KNOWLEDGE, 4}, {FSK_LOGISTICS, 20}, {FSK_SPEED, 2}
	},{
		// Barbarian
		{FSK_MIN_GOLD, 5000}, {FSK_ATTACK, 4}, {FSK_DEFENCE, 4}, {FSK_POWER, 4}, {FSK_KNOWLEDGE, 4}, {FSK_RESIST, 30}, {/**/FSK_OFFENCE, 0}
	},{
		// Wizard
		{FSK_MIN_GOLD, 5000}, {FSK_ATTACK, 4}, {FSK_DEFENCE, 4}, {FSK_POWER, 6}, {FSK_KNOWLEDGE, 6}, {/**/FSK_RESIST, 0}, {/**/FSK_OFFENCE, 0}
	},{
		// Warlock
		{FSK_MIN_GOLD, 5000}, {FSK_ATTACK, 4}, {FSK_DEFENCE, 4}, {FSK_POWER, 6}, {FSK_KNOWLEDGE, 6}, {/**/FSK_RESIST, 0}, {/**/FSK_OFFENCE, 0}
	},{
		// Sorceress
		{FSK_MIN_GOLD, 5000}, {FSK_ATTACK, 4}, {FSK_DEFENCE, 4}, {FSK_POWER, 6}, {FSK_KNOWLEDGE, 6}, {/**/FSK_RESIST, 0}, {/**/FSK_OFFENCE, 0}
	},{
		// Necromancer
		{FSK_MIN_GOLD, 5000}, {FSK_ATTACK, 4}, {FSK_DEFENCE, 4}, {FSK_POWER, 4}, {FSK_KNOWLEDGE, 4}, {FSK_NECROMANCY, 15}, {/**/FSK_OFFENCE, 0}
	}
};
//
const SPEC_HERO_FLAGS ULTART_SPECFLAGS[HERO_TYPE_COUNT] = {
	SHF_INVALID,		// Knight
	SHF_NORANGEPENALTY,	// Barbarian
	SHF_SUMRESBOUNS,	// Wizard
	SHF_MANARESTORE,	// Warlock
	SHF_DMGSPBONUS,		// Sorceress
	SHF_NECRBONUS		// Necromancer
};
//
const uint8 HERO_PRIM_SKILL[HERO_TYPE_COUNT][PRSKILL_COUNT] = {
	{25,50,5,20},	// Knight
	{60,25,5,10},	// Barbarian
	{10,10,40,40},	// Wizard
	{15,10,45,30},	// Warlock
	{15,15,30,40},	// Sorceress
	{15,15,40,30}	// Necromancer
};

/*
 *	SecSkills
 */
const uint8 HERO_SEC_SKILL[HERO_TYPE_COUNT][SECSK_COUNT] = {
	{ 
		// Knight
		2,6,6,4,		// Estates, Leadership, Luck, Diplomacy
		1,1,0,2,		// Air, Earth, Fire, Water
		1,0,0,1,2,0,4,	// Wisdom, Necromancy, Mysticism, Intelligence, Resistance, Sorcery, Learning
		3,4,3,			// Scouting, Logistics, Pathfinding,
		3,3,2,4			// Archery, Ballistics, Offence, Armorer
	},{ 
		// Barbarian
		2,4,4,3,		// Estates, Leadership, Luck, Diplomacy
		1,1,2,0,		// Air, Earth, Fire, Water
		1,0,0,1,1,0,2,	// Wisdom, Necromancy, Mysticism, Intelligence, Resistance, Sorcery, Learning
		3,3,5,			// Scouting, Logistics, Pathfinding,
		4,3,5,3			// Archery, Ballistics, Offence, Armorer
	},{ 
		// Wizard
		2,2,2,3,		// Estates, Leadership, Luck, Diplomacy
		3,3,0,5,		// Air, Earth, Fire, Water
		6,0,4,2,2,3,5,	// Wisdom, Necromancy, Mysticism, Intelligence, Resistance, Sorcery, Learning
		3,2,2,			// Scouting, Logistics, Pathfinding,
		0,0,0,0			// Archery, Ballistics, Offence, Armorer
	},{ 
		// Warlock
		2,0,2,3,		// Estates, Leadership, Luck, Diplomacy
		3,3,5,0,		// Air, Earth, Fire, Water
		4,1,4,3,2,5,4,	// Wisdom, Necromancy, Mysticism, Intelligence, Resistance, Sorcery, Learning
		4,2,3,			// Scouting, Logistics, Pathfinding, 
		0,0,0,0			// Archery, Ballistics, Offence, Armorer
	},{ 
		// Sorceress
		2,2,2,3,		// Estates, Leadership, Luck, Diplomacy
		3,3,0,5,		// Air, Earth, Fire, Water
		6,0,4,2,2,3,5,	// Wisdom, Necromancy, Mysticism, Intelligence, Resistance, Sorcery, Learning
		3,2,2,			// Scouting, Logistics, Pathfinding,
		1,1,0,0			// Archery, Ballistics, Offence, Armorer
	},{ 
		// Necromancer
		2,0,2,3,		// Estates, Leadership, Luck, Diplomacy
		3,3,5,0,		// Air, Earth, Fire, Water
		4,5,4,3,2,5,4,	// Wisdom, Necromancy, Mysticism, Intelligence, Resistance, Sorcery, Learning
		4,2,3,			// Scouting, Logistics, Pathfinding, 
		1,1,0,0			// Archery, Ballistics, Offence, Armorer
	}
};

const sint32 SEKSKILL_DESC[SECSK_COUNT][SSLVL_COUNT][2] = {
	{
		// Estates
		{FSK_MIN_GOLD, 250},{FSK_MIN_GOLD, 500},{FSK_MIN_GOLD, 1000},
	},{
		// Leadership
		{FSK_MORALE, 1},{FSK_MORALE, 2},{FSK_MORALE, 3}
	},{
		// Luck
		{FSK_LUCK, 1},{FSK_LUCK, 2},{FSK_LUCK, 3}
	},{
		// Diplomacy
		{FSK_DIPLOMACY, 20},{FSK_DIPLOMACY, 40},{FSK_DIPLOMACY, 60}
	},{
		// Air
		{FSK_MAG_AIR, 1},{FSK_MAG_AIR, 2},{FSK_MAG_AIR, 3}
	},{
		// Earth
		{FSK_MAG_EARTH, 1},{FSK_MAG_EARTH, 2},{FSK_MAG_EARTH, 3}
	},{
		// Fire
		{FSK_MAG_FIRE, 1},{FSK_MAG_FIRE, 2},{FSK_MAG_FIRE, 3}
	},{
		// Water
		{FSK_MAG_WATER, 1},{FSK_MAG_WATER, 2},{FSK_MAG_WATER, 3}
	},{
		// Wisdom
		{FSK_WISDOM, 1},{FSK_WISDOM, 2},{FSK_WISDOM, 3}
	},{
		// Necromancy
		{FSK_NECROMANCY, 10},{FSK_NECROMANCY, 20},{FSK_NECROMANCY, 30}
	},{
		// Mysticism
		{FSK_MANAPTS, 2},{FSK_MANAPTS, 4},{FSK_MANAPTS, 6}
	},{
		// Intelligence
		{FSK_INTELLIGENCE, 25},{FSK_INTELLIGENCE, 50},{FSK_INTELLIGENCE, 100}
	},{
		// Resistance
		{FSK_RESIST, 5},{FSK_RESIST, 10},{FSK_RESIST, 20}
	},{
		// Sorcery
		{FSK_SORCERY, 5},{FSK_SORCERY, 10},{FSK_SORCERY, 15}
	},{
		// Learning
		{FSK_LEARNING, 5},{FSK_LEARNING, 10},{FSK_LEARNING, 15}
	},{
		// Scouting
		{FSK_SCOUTING, 2},{FSK_SCOUTING, 4},{FSK_SCOUTING, 6}
	},{
		// Logistics
		{FSK_LOGISTICS, 10},{FSK_LOGISTICS, 20},{FSK_LOGISTICS, 30}
	},{
		// Pathfinding
		{FSK_PATHFINDING, 25},{FSK_PATHFINDING, 50},{FSK_PATHFINDING, 75}
	},{
		// Archery
		{FSK_ARCHERY, 10},{FSK_ARCHERY, 25},{FSK_ARCHERY, 50}
	},{
		// Ballistics
		{FSK_BALLISTICS, 1},{FSK_BALLISTICS, 2},{FSK_BALLISTICS, 3}
	},{
		// Offence
		{FSK_OFFENCE, 10},{FSK_OFFENCE, 20},{FSK_OFFENCE, 30}
	},{
		// Armorer
		{FSK_ARMORER, 5},{FSK_ARMORER, 10},{FSK_ARMORER, 15}
	}
};

//////////////////////////////////////////////////////////////////////////
const iPoint HERO_FLAG_ANCHOR[72] = {
	iPoint(4,7), iPoint(4,5), iPoint(4,4), iPoint(4,5), iPoint(4,7),
	iPoint(4,6), iPoint(4,4), iPoint(4,5), iPoint(4,6),

	iPoint(8,7), iPoint(9,7), iPoint(8,7), iPoint(9,8), iPoint(10,8),
	iPoint(10,7), iPoint(9,7), iPoint(8,7), iPoint(7,7), 
	
	iPoint(11,8), iPoint(12,8), iPoint(11,8), iPoint(10,8), iPoint(10,9),
	iPoint(9,9), iPoint(10,8), iPoint(11,8), iPoint(10,8), 
	
	iPoint(13,7), iPoint(14,7), iPoint(13,8), iPoint(11,9), iPoint(13,9),
	iPoint(14,10), iPoint(14,9), iPoint(14,8), iPoint(12,7),
	
	//
	iPoint(32,8), iPoint(32,9), iPoint(32,10), iPoint(32,8), iPoint(32,9),
	iPoint(32,10), iPoint(32,11), iPoint(32,8), iPoint(32,9),
	
	//
	iPoint(20,7), iPoint(19,7), iPoint(20,8), iPoint(19,9), iPoint(20,9),
	iPoint(19,10), iPoint(19,9), iPoint(19,8), iPoint(22,7),
	
	iPoint(22,8), iPoint(21,8), iPoint(22,8), iPoint(23,8), iPoint(23,9),
	iPoint(24,9), iPoint(23,8), iPoint(22,8), iPoint(23,8),

	iPoint(25,7), iPoint(24,7), iPoint(25,7), iPoint(24,8), iPoint(23,8),
	iPoint(23,7), iPoint(24,7), iPoint(25,7), iPoint(26,7),
};


const sint32 MINERALS_DIVIDER[MINERAL_COUNT] = {
	1000, 2, 2, 1, 1, 1, 1
};
