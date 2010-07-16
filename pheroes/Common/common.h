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

#ifndef _HMM_COMMON_DEFS_H_
#define _HMM_COMMON_DEFS_H_

/*
 *	Common defines and constants
 */
const uint32 EMAP_FILE_HDR_KEY = 0x76235278;
const uint32 EMAP_FILE_VERSION = 0x19;

const uint32 GMAP_FILE_HDR_KEY = 'G' | ('M'<<8) | ('A'<<16) | ('P'<<24);
const uint16 GMAP_FILE_VERSION = 0x39;

const uint32 GOBJ_FILE_HDR_KEY = 'G' | ('O'<<8) | ('B'<<16) | ('J'<<24);
const uint16 GOBJ_FILE_VERSION = 0x07;

const uint32 GFNT_FILE_HDR_KEY = 'G' | ('F'<<8) | ('N'<<16) | ('T'<<24);
const uint16 GFNT_FILE_VERSION = 0x01;

const sint32 RANDOM_QUANTITY = 0;

/*
 *	Game languages
 */
enum GAME_LANG {
	GLNG_ENGLISH = 0,
	GLNG_RUSSIAN,
	/*GLNG_POLISH,
	GLNG_SLOVAK,
	GLNG_GERMAN,
	GLNG_CZECH,
	GLNG_ITALIAN,
	GLNG_FRENCH,
	GLNG_SPANISH,*/
	GLNG_COUNT
};

/*
 *	Surfaces
 */
enum SURF_TYPE {
	STYPE_WATER = 0,
	STYPE_SAND,
	STYPE_DIRT,
	STYPE_GRASS,
	STYPE_SWAMP,
	STYPE_LAVA,
	STYPE_WASTELAND,
	STYPE_DESERT,
	STYPE_SNOW,
	STYPE_NDESERT,
	STYPE_PAVEMENT,
	STYPE_NWASTELAND,
	STYPE_COUNT
};

extern const uint32 SURF_TYPE_MASK[STYPE_COUNT]; 
extern const uint8 SURF_MOVE_COST[STYPE_COUNT];
extern const uint16 SURF_COLOR[STYPE_COUNT];

///
inline uint32 CalcCellSeq(const iPoint &pnt, uint32 maxv)
{
	uint32 result = pnt.x;
	result += ~(pnt.y << 16);
	result ^= (pnt.x >> 5);
	result += (pnt.y << 3);
	result ^= (pnt.x>>13);
	result += ~(pnt.y << 9);
	result ^= (result>>17);
		
#ifdef _HMM_GAME_
	result = iRandTable[ (result ^ (result>>8) ^ (result>>16)) & 255 ];
#else
	result = iTables::crc32[ (result ^ (result>>8) ^ (result>>16)) & 255 ];
#endif
	return result % maxv;
}

/*
 *	Sprite level
 */
enum SPRITE_LEVEL {
	SPL_GROUND = 0,
	SPL_PLANTS,
	SPL_OBJECT
};

/*
 *	Game types
 */
enum GAME_TYPE {
	GMT_SPLAYER=0,
	GMT_HOTSEAT,
	GMT_NETWORK,
	GMT_COUNT
};

/*
 *	Object types
 */
enum OBJ_TYPE {
	OBJT_UNKNOWN = 0,
	OBJT_HERO,
	OBJT_VIS_CNST,
	OBJT_OWN_CNST,
	OBJT_MAPOBJECT,
	OBJT_DECOR
};

/*
 *	Map
 */
enum MAP_SIZE {
	MSIZ_SMALL = 0,
	MSIZ_MEDIUM,
	MSIZ_LARGE,
	MSIZ_EXTRALARGE,
	MSIZ_COUNT
};

extern const uint32 MAP_SIZ_SIZE[MSIZ_COUNT];

/*
 *	Difficulty
 */
enum DIFFICULTY_LEVEL {
	DFC_UNDEFINED	= -1,
	DFC_EASY		= 0,
	DFC_NORMAL,
	DFC_HARD,
	DFC_EXPERT,
	DFC_IMPOSSIBLE,
	DFC_COUNT
};

/*
 *	Players
 */
enum PLAYER_TYPE {
	PT_UNDEFINED = 0,
	PT_HUMAN,
	PT_COMPUTER,
	PT_COUNT
};

enum PLAYER_TYPE_MASK {
	PTM_HUMAN_ONLY		= 0,
	PTM_COMPUTER_ONLY,
	PTM_HUMAN_OR_COMPUTER,
	PTM_COUNT
};

enum PLAYER_ID {
	PID_NEUTRAL = -1,
	PID_RED = 0,
	PID_GREEN,
	PID_BLUE,
	PID_CYAN,
	PID_MAGENTA,
	PID_YELLOW,
	PID_COUNT
};
extern const uint16 PLAYER_COLORS[PID_COUNT];
extern LPCTSTR const PLAYER_TEXT_COLORS[PID_COUNT];
extern LPCTSTR const PLAYER_WORD_COLORS[PID_COUNT];

enum PLAYER_ID_MASK {
	PIM_NONE	= 0x00,
	PIM_RED		= 0x01,
	PIM_GREEN	= 0x02,
	PIM_BLUE	= 0x04,
	PIM_CYAN	= 0x08,
	PIM_MAGENTA	= 0x10,
	PIM_YELLOW	= 0x20,
	PIM_ALL		= PIM_RED | PIM_GREEN | PIM_BLUE | PIM_CYAN | PIM_MAGENTA | PIM_YELLOW
};

/*
 *	Nations and heroes
 */
enum IDEOLOGY_TYPE {
	IDEOLOGY_NEUTRAL = 0,
	IDEOLOGY_GOOD,
	IDEOLOGY_EVIL,
	IDEOLOGY_COUNT
};

enum NATION_TYPE {
	NATION_NEUTRAL = 0,
	NATION_HIGHMEN,
	NATION_BARBARIANS,
	NATION_WIZARDS,
	NATION_BEASTMEN,
	NATION_ELVES,
	NATION_UNDEADS,
	NATION_COUNT
};

enum NATION_TYPE_MASK {
	NTM_HIGHMEN		= 0x1,
	NTM_BARBARIANS	= 0x2,
	NTM_WIZARDS		= 0x4,
	NTM_BEASTMEN	= 0x8,
	NTM_ELVES		= 0x10,
	NTM_UNDEADS		= 0x20
};

extern const IDEOLOGY_TYPE NATION_TYPE_IDEOLOGY[NATION_COUNT];

enum HERO_TYPE {
	HERO_TYPE_KNIGHT = 0,
	HERO_TYPE_BARBARIAN,
	HERO_TYPE_WIZARD,
	HERO_TYPE_WARLOCK,
	HERO_TYPE_SORCERESS,
	HERO_TYPE_NECROMANCER,
	HERO_TYPE_COUNT
};

enum HERO_TYPE_MASK {
	HTM_KNIGHT		= 0x1,
	HTM_BARBARIAN	= 0x2,
	HTM_WIZARD		= 0x4,
	HTM_WARLOCK		= 0x8,
	HTM_SORCERESS	= 0x10,
	HTM_NECROMANCER	= 0x20,
	HTM_GOOD		= HTM_KNIGHT | HTM_WIZARD | HTM_SORCERESS,
	HTM_EVIL		= HTM_BARBARIAN | HTM_WARLOCK | HTM_NECROMANCER,
	HTM_MIGHT		= HTM_KNIGHT | HTM_BARBARIAN,
	HTM_MISC		= HTM_SORCERESS | HTM_NECROMANCER,
	HTM_MAGIC		= HTM_WIZARD | HTM_WARLOCK,
	HTM_ALL			= HTM_GOOD | HTM_EVIL
};
extern const IDEOLOGY_TYPE HERO_TYPE_IDEOLOGY[HERO_TYPE_COUNT];

const sint32 DEF_HERO_SCOUTING = 4;
const sint32 DEF_HERO_MYSTICISM = 1;
const sint32 DEF_HERO_MOVES = 60;

/*
 *	Skills
 */
enum PRSKILL_TYPE {
	PRSKILL_ATTACK = 0,
	PRSKILL_DEFENCE,
	PRSKILL_POWER,
	PRSKILL_KNOWLEDGE,
	PRSKILL_COUNT
};

struct iPrSkills {
	iPrSkills() { memset(val,0,sizeof(val)); }
	iPrSkills(sint32 attack, sint32 defence, sint32 power, sint32 knowledge) { val[0]=attack; val[1]=defence; val[2]=power; val[3]=knowledge; }
	iPrSkills(const iPrSkills& other) { memcpy(val, other.val, sizeof(val));}

	inline void operator += (const iPrSkills& ps)
	{ for (uint32 xx=0; xx<PRSKILL_COUNT; ++xx) val[xx] += ps.val[xx]; }
	inline void operator -= (const iPrSkills& ps)
	{ for (uint32 xx=0; xx<PRSKILL_COUNT; ++xx) val[xx] -= ps.val[xx]; }

	sint32 val[PRSKILL_COUNT];
};

enum SECONDARY_SKILLS {
	SECSK_NONE = -1,	//	Basic		Advanced	Expert		Master		DayStart	OnDemand	OnlyBattle
	SECSK_ESTATES = 0,	//	+100gp		+250gp		+500gp		+1000gp		X
	SECSK_LEADERSHIP,	//	+1 morale	+2 morale	+3 morale	+4 morale							X
	SECSK_LUCK,			//	+1 luck		+2 luck		+3 luck		+4 luck								X
	SECSK_DIPLOMACY,	//	20%			35%			50%			65%						X
	SECSK_AIRMAGIC,		//																X
	SECSK_EARTHMAGIC,	//																X
	SECSK_FIREMAGIC,	//																X
	SECSK_WATERMAGIC,	//																X
	SECSK_WISDOM,		//	2nd			3rd			4th			5th						X
	SECSK_NECROMANCY,	//	10%			20%			30%			40%						X
	SECSK_MYSTICISM,	//	+2mp		+4mp		+6mp		+8mp		X
	SECSK_INTELLIGENCE,	//
	SECSK_RESISTANCE,	//
	SECSK_SORCERY,		//	+5%			+10%		+15%		+20%					X
	SECSK_LEARNING,		//	+5%exp		+10%exp		+15%exp		+20%exp					X
	SECSK_SCOUTING,		//	+1cell		+2cell		+3cell		+4cell					X
	SECSK_LOGISTICS,	//	+15ap		+30ap		+45ap		+60ap		X
	SECSK_PATHFINDING,	//																X
	SECSK_ARCHERY,		//																			X
	SESCK_BALLISTICS,	//																			X
	SECSK_OFFENCE,		//																			X
	SECSK_ARMORER,		//																			X
	SECSK_COUNT
};

enum SECSKILL_LEVEL {
	SSLVL_NONE = -1,
	SSLVL_BASIC = 0,
	SSLVL_ADVANCED,
	SSLVL_EXPERT,
	SSLVL_COUNT,
};

struct iSecSkillEntry
{
	iSecSkillEntry(SECONDARY_SKILLS skill=SECSK_NONE, SECSKILL_LEVEL level=SSLVL_NONE)
	: m_skill(skill), m_level(level) {}

	sint8		m_skill;
	sint8		m_level;
};

typedef iSimpleArray<iSecSkillEntry> iSecSkills;

/*
 *	Minerals
 */
enum MINERAL_TYPE {
	MINERAL_UNKNOWN = -1,
	MINERAL_GOLD = 0,
	MINERAL_ORE,
	MINERAL_WOOD,
	MINERAL_MERCURY,
	MINERAL_GEMS,
	MINERAL_CRYSTAL,
	MINERAL_SULFUR,
	MINERAL_COUNT
};

struct FractionCoeff
{
	sint32	num,denum;
	FractionCoeff() : num(-1), denum(-1) {}
	FractionCoeff( sint32 n, sint32 d ) : num(n), denum(d) {}

	inline bool IsValid() const { return num != -1 && denum != -1; }

	inline FractionCoeff GetNormalized() const
	{
		if (num > denum) return FractionCoeff(num/denum, 1);
		else if (denum > num) return FractionCoeff(1, (denum+num-1)/num);
		return FractionCoeff(1,1);
	}

	sint32 operator *= ( sint32 );
};

inline FractionCoeff MineralExchRate(MINERAL_TYPE from, MINERAL_TYPE to, sint32 mlvl)
{
	static const sint32 MINERAL_EXCH_RATE[MINERAL_COUNT] = { 1, 250, 250, 500, 500, 500, 500 };
	return FractionCoeff (MINERAL_EXCH_RATE[from] * (mlvl+1) , 10 * 2 * MINERAL_EXCH_RATE[to]);
/*	static float MINERAL_EXCH_RATE[MINERAL_COUNT] = { 0.002f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f };
	float mval = 10.0f * 2 / (mlvl+1);
	float v1 = MINERAL_EXCH_RATE[from] / mval;
	float v2 = v1 / MINERAL_EXCH_RATE[to];
	return  v2;*/
}

struct iMineralSet {
	sint32 quant[MINERAL_COUNT];
	inline void Reset() { memset(quant,0,sizeof(quant)); }
	
	inline sint32 Has(const iMineralSet& ms) const 
	{
		sint32 cnt = 0;
		for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) {
			if (quant[xx]<ms.quant[xx]) return 0; 
			else if (ms.quant[xx]) cnt = (cnt)?iMIN(cnt,quant[xx] / ms.quant[xx]):quant[xx] / ms.quant[xx];
		}
		return cnt;
	}

	inline iMineralSet operator * (sint32 val) const
	{ 
		iMineralSet result; result.Reset();
		for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) result.quant[xx] = quant[xx] * val;
		return result;
	}

	inline bool Empty() const
	{
		for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) if (quant[xx] != 0) return false; 
		return true;
	}

	inline iMineralSet DeficientAmount(const iMineralSet& other) const
	{
		iMineralSet result;	result.Reset();
		for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) if (other.quant[xx] > quant[xx]) result.quant[xx] = other.quant[xx] - quant[xx];
		return result;
	}

	inline iMineralSet Intersect(const iMineralSet& other) const
	{
		iMineralSet result;
		for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) result.quant[xx] = iMIN(quant[xx], other.quant[xx]);
		return result;
	}

	inline void Normalize() { for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) if (quant[xx] < 0)  quant[xx] = 0; }

	inline bool operator == (const iMineralSet& ms) const { for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) if (quant[xx] != ms.quant[xx]) return false; return true; }
	inline bool operator != (const iMineralSet& ms) const { return !operator==(ms); }
	inline iMineralSet& operator = (const iMineralSet& ms) { for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) quant[xx] = ms.quant[xx]; return *this; }
	inline void operator += (const iMineralSet& ms) { for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) quant[xx] += ms.quant[xx]; }
	inline void operator -= (const iMineralSet& ms) { for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) quant[xx] -= ms.quant[xx]; }
};

extern const sint32 MINERALS_DIVIDER[MINERAL_COUNT];

/*
 *	Artifacts
 */
enum ART_LEVEL_TYPE {
	ART_LEVEL_NONE,
	ART_LEVEL_TREASURE,
	ART_LEVEL_MINOR,
	ART_LEVEL_MAJOR,
	ART_LEVEL_RELICT,
	ART_LEVEL_ULTIMATE,
	ART_LEVEL_COUNT
};

enum RANDOM_ARTIFACTS {
	ARTT_RAND		= 0xFF00,
	ARTT_RAND_L1	= 0xFF01,
	ARTT_RAND_L2	= 0xFF02,
	ARTT_RAND_L3	= 0xFF03,
	ARTT_RAND_L4	= 0xFF04
};

enum ART_TYPE {
	ARTT_BASIC = 0,
	ARTT_FURTSKILL,
	ARTT_NEGSPHERE,
	ARTT_SHOFWAR,
	ARTT_CURSWORD,
	ARTT_COUNT,
	ARTT_ULTIMATE	= 0x0F00
};

enum ART_ASSIGN_TYPE {
	ART_ASSIGN_HEAD = 0,
	ART_ASSIGN_NECK,
	ART_ASSIGN_TORSO,
	ART_ASSIGN_SHOULDERS,
	ART_ASSIGN_HANDS,
	ART_ASSIGN_FINGERS,
	ART_ASSIGN_LEGS,
	ART_ASSIGN_FEET,
	ART_ASSIGN_MISC,
	ART_ASSIGN_COUNT
};

enum HERO_ART_CELL {
	AC_UNDEFINED = -1,
	AC_HEAD,
	AC_NECK,
	AC_TORSO,
	AC_LHAND,
	AC_RHAND,
	AC_LFINGERS,
	AC_RFINGERS,
	AC_SHOULDERS,
	AC_LEGS,
	AC_FEET,
	AC_MISC1,
	AC_MISC2,
	AC_MISC3,
	AC_MISC4,
	AC_COUNT
};

enum SPEC_HERO_FLAGS {
	SHF_INVALID = -1,
	SHF_NORANGEPENALTY,	// No range attack penalty
	SHF_SUMRESBOUNS,	// 50% bonus for resurrection and summon spells
	SHF_MANARESTORE,	// Mana restores each day
	SHF_DMGSPBONUS,		// Effect from all damage spells increased by 50%
	SHF_NECRBONUS		// Necromancy skill restores mummies instead of skeletons
};

//
extern const sint32 ULTART_STDMODIF[HERO_TYPE_COUNT][7][2];
extern const SPEC_HERO_FLAGS ULTART_SPECFLAGS[HERO_TYPE_COUNT];
extern const ART_ASSIGN_TYPE HERO_ART_CELL_ASSIGN[AC_COUNT];

class iArtifactList {
public:
	struct iItem {
		iItem(uint16 _id, HERO_ART_CELL _assign) : id(_id), assign(_assign) {}
		uint16			id;
		HERO_ART_CELL	assign;
	};

	inline void Add(uint16 _id, HERO_ART_CELL _assign) { m_Items.Add(iItem(_id, _assign)); }
	inline uint32 Count() const { return m_Items.GetSize(); }
	inline const iItem& At(uint32 idx) const { return m_Items[idx]; }
	inline iItem& At(uint32 idx) { return m_Items[idx]; }

private:
	iSimpleArray<iItem>	m_Items;
};

/*
 *	Path elements
 */
enum PATH_ELEMENT_TYPE {
	PET_WALL = 0,
	PET_ROAD,
	PET_RIVER,
	PET_COUNT
};

/*
 *	Map items
 */
enum MAP_ITEM_TYPE {
	MAPITEM_MINERAL = 0,
	MAPITEM_MANACRYSTAL,
	MAPITEM_CAMPFIRE,
	MAPITEM_CHEST,
	MAPITEM_ARTIFACT,
	MAPITEM_SPELLSCROLL,
	MAPITEM_LAMP,
	MAPITEM_KEYGUARD,
	MAPITEM_COUNT
};

/*
 *	Guards
 */
enum GUARD_DISPOS {
	GDISP_COMPLIANT = 0,
	GDISP_AGGRESSIVE,
	GDISP_SAVAGE,
	GDISP_COUNT
};

/*
 *	Vision level (information level)
 */
enum VISION_LEVEL {
	VL_NONE = 0,
	VL_BASIC,
	VL_EXPERT
};

/*
 *	
 */
extern const uint8 HERO_PRIM_SKILL[HERO_TYPE_COUNT][PRSKILL_COUNT];
extern const uint8 HERO_SEC_SKILL[HERO_TYPE_COUNT][SECSK_COUNT];

/*
 *	Further skills (don't forget to edit defines.cpp)
 */
enum FURTHER_SKILLS {
	FSK_INVALID = -1,
	
	FSK_ATTACK,			// [All]		Increases Attack skill by 'n'
	FSK_DEFENCE,		// [All]		Increases Defence skill by 'n'
	FSK_POWER,			// [All]		Increases Spell power skill by 'n'
	FSK_KNOWLEDGE,		// [All]		Increases Knowledge skill by 'n'

	FSK_ACTPTS,			// [Overland]	Hero’s land movement range is increased on 'n' pts
	FSK_LOGISTICS,		// [Overland]	Hero’s land movement range is increased by 'n'%
	FSK_PATHFINDING,	// [Overland]	Reduce the effects of difficult terrain by 'n'%
	FSK_SCOUTING,		// [Overland]	Increases Scouting level by 'n' cells
	FSK_VISION,			// [Overland]	Increases Vision level by 'n'

	FSK_BALLISTICS,		// [Combat]		???
	FSK_LEARNING,		// [Overland]	Earned experience is increased by 'n'%
	FSK_DIPLOMACY,		// [All]		'n'% of creatures normally fleeing from your army offer to join. Cost of surrendering is reduced by 'n/2'%
	FSK_NECROMANCY,		// [Combat]		'n'% of enemy creatures killed are resurrected as skeletons and added to the hero’s army
	
	FSK_SORCERY,		// [Combat]		Increases Magic spells power by 'n'%
	FSK_MANAPTS,		// [Overland]	Restores 'n' mana points each day
	FSK_INTELLIGENCE,	// [Overland]	Maximum spell points is increased by 'n'%
	FSK_WISDOM,			// [Overland]	Allows the hero to learn 'n'th level spells and below
	
	FSK_MAG_AIR,		// [All]		Allows the hero to cast 'n'th level spells of Air magic and below
	FSK_MAG_EARTH,		// [All]		Allows the hero to cast 'n'th level spells of Earth magic and below
	FSK_MAG_FIRE,		// [All]		Allows the hero to cast 'n'th level spells of Fire magic and below
	FSK_MAG_WATER,		// [All]		Allows the hero to cast 'n'th level spells of Water magic and below

	FSK_RES_AIR,		// [Combat]		Damage from Air magic spells is reduced by 'n'% for target
	FSK_RES_EARTH,		// [Combat]		Damage from Earth magic spells is reduced by 'n'% for target
	FSK_RES_FIRE,		// [Combat]		Damage from Fire magic spells is reduced by 'n'% for target
	FSK_RES_WATER,		// [Combat]		Damage from Water magic spells is reduced by 'n'% for target

	FSK_ARCHERY,		// [Combat]		Increases Archery skill by 'n'% (increase range attack damage)
	FSK_OFFENCE,		// [Combat]		Increases Offence skill by 'n'% (increase melee damage)
	FSK_AIRSHIELD,		// [Combat]		Decrease range attack damage by 'n'%
	FSK_SHIELD,			// [Combat]		Decrease melee damage by 'n'%
	FSK_ARMORER,		// [Combat]		Increases Armorer skill by 'n'% (decrease damage)
	FSK_RANGEATTACK,	// [Combat]		Increase attack skill for range attack by 'n'
	FSK_MELEEATTACK,	// [Combat]		Increase attack skill for melee attack by 'n'
	FSK_RESIST,			// [Combat]		Increases Magic resistance skill by 'n'%
	
	FSK_HITS,			// [Combat]		Creatures hit-points is increased by 'n'
	FSK_SPEED,			// [Combat]		Creatures speed is increased by 'n'
	FSK_MORALE,			// [Combat]		Morale is increased by 'n'
	FSK_LUCK,			// [Combat]		Luck is increased by 'n'
	FSK_COUNTERSTRIKE,	// [Combat]		Counterstrike

	FSK_MIN_GOLD,		// [Overland]	Adds 'n' units of gold each day
	FSK_MIN_ORE,		// [Overland]	Adds 'n' units of ore each day
	FSK_MIN_WOOD,		// [Overland]	Adds 'n' units of wood each day
	FSK_MIN_MERCURY,	// [Overland]	Adds 'n' units of mercury each day
	FSK_MIN_GEMS,		// [Overland]	Adds 'n' units of gems each day
	FSK_MIN_CRYSTAL,	// [Overland]	Adds 'n' units of crystal each day
	FSK_MIN_SULFUR,		// [Overland]	Adds 'n' units of sulfur each day
	FSK_COUNT
};
//////////////////////////////////////////////////////////////////////////
class iFurtSkills
{
public:
	// c-tors
	iFurtSkills() { Reset(); }
	iFurtSkills(const iFurtSkills& other) { memcpy(values, other.values, sizeof(values)); }

	//
	inline void Reset() { memset(values, 0, sizeof(values)); }
	inline sint32 Value(FURTHER_SKILLS type) const { check(type >= FSK_INVALID && type < FSK_COUNT); return values[type]; }
	inline sint32& Value(FURTHER_SKILLS type) { check(type >= FSK_INVALID && type < FSK_COUNT); return values[type]; }

	inline bool Empty() const { for (uint32 xx=0; xx<FSK_COUNT; ++xx) if (values[xx] != 0) return false; return true; }

	// operator +=
	inline const iFurtSkills& operator += (const iFurtSkills& fs)
	{ for (uint32 xx=0; xx<FSK_COUNT; ++xx) values[xx] += fs.values[xx]; return *this; }
	// operator -=
	inline const iFurtSkills& operator -= (const iFurtSkills& fs)
	{ for (uint32 xx=0; xx<FSK_COUNT; ++xx) values[xx] -= fs.values[xx]; return *this; }
	// operatotr +
	inline iFurtSkills operator + (const iFurtSkills& other) const
	{ return iFurtSkills(*this) += other; }
	// operatotr -
	inline iFurtSkills operator - (const iFurtSkills& other) const
	{ return iFurtSkills(*this) -= other; }

	// Primary skills
	inline void operator += (const iPrSkills& ps)
	{ for (uint32 xx=0; xx<PRSKILL_COUNT; ++xx) values[xx+FSK_ATTACK] += ps.val[xx]; }
	inline void operator -= (const iPrSkills& ps)
	{ for (uint32 xx=0; xx<PRSKILL_COUNT; ++xx) values[xx+FSK_ATTACK] -= ps.val[xx]; }

	
private:
	sint32	values[FSK_COUNT];
};

/*
 *	Rewards
 */
enum REWARD_ITEM_TYPE {
	RIT_INVALID = -1,
	RIT_MINERAL = 0,	// Type, Quantity
	RIT_EXPERIENCE,		// None, Quantity
	RIT_MANAPTS,		// None, Quantity
	RIT_TRAVELPTS,		// None, Quantity
	RIT_MORALE,			// None, Modifier
	RIT_LUCK,			// None, Modifier
	RIT_FURTSKILL,		// Type, Modifier (Primary skill only)
	RIT_ARTIFACT,		// Artifact level or idx, None (can be defined as random)
	RIT_MAGICSPELL,		// Spell level or idx, None (can be defined as random)
	RIT_CREATGROUP,		// Type, Quantity
	RIT_COUNT
};

// reward item
struct iRewardItem 
{
	iRewardItem(REWARD_ITEM_TYPE type, sint32 fparam, sint32 sparam)
	: m_type(type), m_fParam(fparam), m_sParam(sparam) {}

	REWARD_ITEM_TYPE	m_type;
	sint32				m_fParam;
	sint32				m_sParam;
};

// Rewards container
typedef iSimpleArray<iRewardItem> iRewardsCtr;

// Short point
typedef uint32 shPoint;
#define MAKE_SHPOINT(x, y) (uint32)(((x&0xFFFF)<<16) | (y&0xFFFF))
#define SHPOINT_X(pt) (sint16)(pt>>16)
#define SHPOINT_Y(pt) (sint16)(pt&0xFFFF)
typedef iSimpleArray<shPoint> iShPointList;

/*
 *	
 */
extern const iPoint HERO_FLAG_ANCHOR[72];
extern const iMineralSet INITIAL_RESOURCES[2][DFC_COUNT]; // Human and computer
extern const sint32 SEKSKILL_DESC[SECSK_COUNT][SSLVL_COUNT][2];


// consts
const sint32 RAND_VAL = -1;

#endif //_HMM_COMMON_DEFS_H_

