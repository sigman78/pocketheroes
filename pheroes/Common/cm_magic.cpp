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

// Magic school secondary skills (Air, Earth, Fire, Water)
const SECONDARY_SKILLS MSCH_SECSKILLS[MST_COUNT] = {
	SECSK_AIRMAGIC, SECSK_EARTHMAGIC, SECSK_FIREMAGIC, SECSK_WATERMAGIC
};

const uint32 SPELL_LEVEL_MASKS[SPL_COUNT] = {
	SPL_MASK_FIRST, SPL_MASK_SECOND, SPL_MASK_THIRD, SPL_MASK_FOURTH, SPL_MASK_FIFTH
};

const SPELL_DESC_STRUCT SPELL_DESCRIPTORS[MSP_COUNT] = {
	{
		//////////////////////////////////////////////////////////////////////////
		// Air magic
		//////////////////////////////////////////////////////////////////////////

		// Magic arrow
		SPT_COMBAT, SPL_FIRST, MST_AIR, SPC_DAMAGE, 5, SLB_NONE, 0,
		{
			{
				// None (pow*10)
				STT_ALLENEMY, STM_CREAT_GROUP, 10
			},{
				// Basic (pow*10)
				STT_ALLENEMY, STM_CREAT_GROUP, 10
			},{
				// Advanced (pow*12)
				STT_ALLENEMY, STM_CREAT_GROUP, 12
			},{
				// Expert (pow*15)
				STT_ALLENEMY, STM_CREAT_GROUP, 15
			}
		}
	},{
		// Protection from earth
		SPT_COMBAT, SPL_FIRST, MST_AIR, SPC_FURTSKILL, 5, SLB_PROTEARTH, 0,
		{
			{
				// none (30%)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_RES_EARTH, 30
			},{
				// basic (30%)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_RES_EARTH, 30
			},{
				// advanced (50%)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_RES_EARTH, 50
			},{
				// expert (50% + all)
				STT_ALLFRIENDLY, STM_ALL, FSK_RES_EARTH, 50
			}
		}
	},{
		// Haste
		SPT_COMBAT, SPL_FIRST, MST_AIR, SPC_FURTSKILL, 6, SLB_HASTE, 0,
		{
			{
				// none (3 hexes)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_SPEED, 3
			},{
				// basic (3 hexes)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_SPEED, 3
			},{
				// advanced (5 hexes)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_SPEED, 5
			},{
				// expert (5 hexes + all)
				STT_ALLFRIENDLY, STM_ALL, FSK_SPEED, 5
			}
		}
	},{
		// Shield 
		SPT_COMBAT, SPL_FIRST, MST_AIR, SPC_FURTSKILL, 5, SLB_SHIELD, 0,
		{
			{
				// none (15%)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_SHIELD, 15
			},{
				// basic (15%)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_SHIELD, 15
			},{
				// advanced (30%)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_SHIELD, 30
			},{
				// expert (30% + all)
				STT_ALLFRIENDLY, STM_ALL, FSK_SHIELD, 30
			}
		}
	},{
		// Disrupting ray
		SPT_COMBAT, SPL_SECOND, MST_AIR, SPC_DISRAY, 10, SLB_DISRAY, 0,
		{
			{
				// none (-3)
				STT_ALLENEMY, STM_CREAT_GROUP, 3
			},{
				// none (-3)
				STT_ALLENEMY, STM_CREAT_GROUP, 3
			},{
				// none (-4)
				STT_ALLENEMY, STM_CREAT_GROUP, 4
			},{
				// none (-5)
				STT_ALLENEMY, STM_CREAT_GROUP, 5
			}
		}
	},{
		// Lightning bolt
		SPT_COMBAT, SPL_SECOND, MST_AIR, SPC_DAMAGE, 10, SLB_NONE, 0,
		{
			{
				// none (pow*25)
				STT_ALLENEMY, STM_CREAT_GROUP, 25
			},{
				// basic (pow*25)
				STT_ALLENEMY, STM_CREAT_GROUP, 25
			},{
				// advanced (pow*27)
				STT_ALLENEMY, STM_CREAT_GROUP, 27
			},{
				// expert (pow*30)
				STT_ALLENEMY, STM_CREAT_GROUP, 30
			}
		}
	},{
		// Precision
		SPT_COMBAT, SPL_SECOND, MST_AIR, SPC_FURTSKILL, 8, SLB_PRECISION, 0,
		{
			{
				// none (+3)
				STT_FRSHOOTERS, STM_CREAT_GROUP, FSK_RANGEATTACK, 3
			},{
				// basic (+3)
				STT_FRSHOOTERS, STM_CREAT_GROUP, FSK_RANGEATTACK, 3
			},{
				// advanced (+5)
				STT_FRSHOOTERS, STM_CREAT_GROUP, FSK_RANGEATTACK, 5
			},{
				// expert (+5 + all)
				STT_FRSHOOTERS, STM_ALL, FSK_RANGEATTACK, 5
			}
		}
	},{
		// Air shield 
		SPT_COMBAT, SPL_SECOND, MST_AIR, SPC_FURTSKILL, 12, SLB_AIRSHIELD, 0,
		{
			{
				// none (25%)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_AIRSHIELD, 25
			},{
				// basic (25%)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_AIRSHIELD, 25
			},{
				// advanced (50%)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_AIRSHIELD, 50
			},{
				// expert (50% + all)
				STT_ALLFRIENDLY, STM_ALL, FSK_AIRSHIELD, 50
			}
		}
	},{
		// Holy word
		SPT_COMBAT, SPL_THIRD, MST_AIR, SPC_DAMAGE, 15, SLB_NONE, 0,
		{
			{
				// none (pow*10)
				STT_ALLUNDEADS, STM_ALL, 10
			},{
				// none (pow*10)
				STT_ALLUNDEADS, STM_ALL, 10
			},{
				// none (pow*12)
				STT_ALLUNDEADS, STM_ALL, 12
			},{
				// none (pow*15)
				STT_ALLUNDEADS, STM_ALL, 15
			}
		}
	},{
		// Counterstrike
		SPT_COMBAT, SPL_FOURTH, MST_AIR, SPC_FURTSKILL, 20, SLB_COUNTERSTRIKE, 0,
		{
			{
				// none (1 counterstrike)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_COUNTERSTRIKE, 1
			},{
				// basic (1 counterstrike)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_COUNTERSTRIKE, 1
			},{
				// advanced (2 counterstrike)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_COUNTERSTRIKE, 2
			},{
				// expert (3 counterstrike)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_COUNTERSTRIKE, 3
			}
		}
	},{
		// Resurrect 
		SPT_COMBAT, SPL_FIFTH, MST_AIR, SPC_RESURRECT, 25, SLB_NONE, 0,
		{
			{
				// none (pow*40)
				STT_FRNORMALS, STM_CREAT_GROUP, 40, 1
			},{
				// basic (pow*40)
				STT_FRNORMALS, STM_CREAT_GROUP, 40, 1
			},{
				// advanced (pow*50)
				STT_FRNORMALS, STM_CREAT_GROUP, 45, 1
			},{
				// expert (pow*60)
				STT_FRNORMALS, STM_CREAT_GROUP, 50, 1
			}
		}
	},{
		// Summon air elemental
		SPT_COMBAT, SPL_FIFTH, MST_AIR, SPC_SUMMON, 25, SLB_NONE, 0,
		{
			{
				// none (pow*3)
				STT_ALL, STM_SUMMON, CREAT_AIR_ELEMENTAL, 3
			},{
				// basic (pow*3)
				STT_ALL, STM_SUMMON, CREAT_AIR_ELEMENTAL, 3
			},{
				// advanced (pow*4)
				STT_ALL, STM_SUMMON, CREAT_AIR_ELEMENTAL, 4
			},{
				// expert (pow*5)
				STT_ALL, STM_SUMMON, CREAT_AIR_ELEMENTAL, 5
			}
		}
	},{
		//////////////////////////////////////////////////////////////////////////
		// Earth magic
		//////////////////////////////////////////////////////////////////////////

		// Protection from air 
		SPT_COMBAT, SPL_FIRST, MST_EARTH, SPC_FURTSKILL, 5, SLB_PROTAIR, 0,
		{
			{
				// none (30%)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_RES_AIR, 30
			},{
				// basic (30%)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_RES_AIR, 30
			},{
				// advanced (50%)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_RES_AIR, 50
			},{
				// expert (50% + all)
				STT_ALLFRIENDLY, STM_ALL, FSK_RES_AIR, 50
			}
		}
	},{
		// Slow 
		SPT_COMBAT, SPL_FIRST, MST_EARTH, SPC_FURTSKILL, 6, SLB_SLOW, 0,
		{
			{
				// none (-3)
				STT_ALLENEMY, STM_CREAT_GROUP, FSK_SPEED, -3
			},{
				// basic (-3)
				STT_ALLENEMY, STM_CREAT_GROUP, FSK_SPEED, -3
			},{
				// advanced (-6)
				STT_ALLENEMY, STM_CREAT_GROUP, FSK_SPEED, -6
			},{
				// expert (-6 + all)
				STT_ALLENEMY, STM_ALL, FSK_SPEED, -6
			}
		}
	},{
		// Stone skin 
		SPT_COMBAT, SPL_FIRST, MST_EARTH, SPC_FURTSKILL, 5, SLB_STONESKIN, 0,
		{
			{
				// none (+3)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_DEFENCE, 3
			},{
				// basic (+3)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_DEFENCE, 3
			},{
				// advanced (+6)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_DEFENCE, 6
			},{
				// expert (+6 + all)
				STT_ALLFRIENDLY, STM_ALL, FSK_DEFENCE, 6
			}
		}
	},{
		// Visions
		SPT_OVERLAND, SPL_SECOND, MST_EARTH, SPC_VISION, 10, SLB_NONE, 0,
		{
			{
				// none (5 cells)
				STT_NONE, STM_NONE, 5
			},{
				// none (5 cells)
				STT_NONE, STM_NONE, 5
			},{
				// none (10 cells)
				STT_NONE, STM_NONE, 10
			},{
				// none (15 cells)
				STT_NONE, STM_NONE, 15
			}
		}
	},{
		// Earthquake
		SPT_COMBAT, SPL_THIRD, MST_EARTH, SPC_EARTHQUAKE, 15, SLB_NONE, 0,
		{
			{
				// none (2 walls)
				STT_ALL, STM_EARTHQUAKE, 2
			},{
				// basic (2 walls)
				STT_ALL, STM_EARTHQUAKE, 2
			},{
				// advanced (3 walls)
				STT_ALL, STM_EARTHQUAKE, 3
			},{
				// expert (4 walls)
				STT_ALL, STM_EARTHQUAKE, 4
			}
		}
	},{
		// Sorrow 
		SPT_COMBAT, SPL_FOURTH, MST_EARTH, SPC_FURTSKILL, 16, SLB_SORROW, 0,
		{
			{
				// none (-1)
				STT_ENNUNDEADS, STM_CREAT_GROUP, FSK_MORALE, -1
			},{
				// basic (-1)
				STT_ENNUNDEADS, STM_CREAT_GROUP, FSK_MORALE, -1
			},{
				// advanced (-2)
				STT_ENNUNDEADS, STM_CREAT_GROUP, FSK_MORALE, -2
			},{
				// expert (-2 + all)
				STT_ENNUNDEADS, STM_ALL, FSK_MORALE, -2
			}
		}
	},{
		// Meteor shower
		SPT_COMBAT, SPL_FOURTH, MST_EARTH, SPC_DAMAGE, 18, SLB_NONE, 0,
		{
			{
				// none (pow*25)
				STT_ALL, STM_BALL_SET, 25
			},{
				// basic (pow*25)
				STT_ALL, STM_BALL_SET, 25
			},{
				// advanced (pow*27)
				STT_ALL, STM_BALL_SET, 27
			},{
				// expert (pow*30)
				STT_ALL, STM_BALL_SET, 30
			}
		}
	},{
		// Town portal
		SPT_OVERLAND, SPL_FIFTH, MST_EARTH, SPC_TOWNPORTAL, 25, SLB_NONE, 0,
		{
			{
				// none (?)
				STT_NONE, STM_NONE, 0,
			},{
				// basic (?)
				STT_NONE, STM_NONE, 0,
			},{
				// advanced (?)
				STT_NONE, STM_NONE, 1,
			},{
				// expert (?)
				STT_NONE, STM_NONE, 1,
			}
		}
	},{
		// Implosion
		SPT_COMBAT, SPL_FIFTH, MST_EARTH, SPC_DAMAGE, 25, SLB_NONE, 0,
		{
			{
				// none (pow*60)
				STT_ALLENEMY, STM_CREAT_GROUP, 60
			},{
				// basic (pow*60)
				STT_ALLENEMY, STM_CREAT_GROUP, 60
			},{
				// advanced (pow*70)
				STT_ALLENEMY, STM_CREAT_GROUP, 70
			},{
				// expert (pow*80)
				STT_ALLENEMY, STM_CREAT_GROUP, 80
			}
		}
	},{
		// Summon earth elemental
		SPT_COMBAT, SPL_FIFTH, MST_EARTH, SPC_SUMMON, 25, SLB_NONE, 0,
		{
			{
				// none (pow*3)
				STT_NONE, STM_SUMMON, CREAT_EARTH_ELEMENTAL, 3
			},{
				// basic (pow*3)
				STT_NONE, STM_SUMMON, CREAT_EARTH_ELEMENTAL, 3
			},{
				// advanced (pow*4)
				STT_NONE, STM_SUMMON, CREAT_EARTH_ELEMENTAL, 4
			},{
				// expert (pow*5)
				STT_NONE, STM_SUMMON, CREAT_EARTH_ELEMENTAL, 5
			}
		}
	},{
		//////////////////////////////////////////////////////////////////////////
		// Fire magic
		//////////////////////////////////////////////////////////////////////////

		// Bloodlust
		SPT_COMBAT, SPL_FIRST, MST_FIRE, SPC_FURTSKILL, 5, SLB_BLOODLUST, 0,
		{
			{
				// none (+3)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_MELEEATTACK, 3
			},{
				// basic (+3)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_MELEEATTACK, 3
			},{
				// advanced (+6)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_MELEEATTACK, 6
			},{
				// expert (+6 + all)
				STT_ALLFRIENDLY, STM_ALL, FSK_MELEEATTACK, 6
			}
		}
	},{
		// Protection from water
		SPT_COMBAT, SPL_FIRST, MST_FIRE, SPC_FURTSKILL, 5, SLB_PROTWATER, 0,
		{
			{
				// none (30%)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_RES_WATER, 30
			},{
				// basic (30%)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_RES_WATER, 30
			},{
				// advanced (50%)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_RES_WATER, 50
			},{
				// expert (50% + all)
				STT_ALLFRIENDLY, STM_ALL, FSK_RES_WATER, 50
			}
		}
	},{
		// Curse
		SPT_COMBAT, SPL_FIRST, MST_FIRE, SPC_BLESS, 6, SLB_CURSE, 0,
		{
			{
				// none (?)
				STT_ENNUNDEADS, STM_CREAT_GROUP, -1, MSP_BLESS
			},{
				// basic (?)
				STT_ENNUNDEADS, STM_CREAT_GROUP, -1, MSP_BLESS
			},{
				// advanced (?)
				STT_ENNUNDEADS, STM_CREAT_GROUP, -1, MSP_BLESS
			},{
				// expert (? + all)
				STT_ENNUNDEADS, STM_ALL, -1, MSP_BLESS
			}
		}
	},{
		// Blind
		SPT_COMBAT, SPL_SECOND, MST_FIRE, SPC_BLIND, 10, SLB_BLIND, 0,
		{
			{
				// none (50% retail)
				STT_ENNORMALS, STM_CREAT_GROUP, 2
			},{
				// basic (50% retail)
				STT_ENNORMALS, STM_CREAT_GROUP, 2
			},{
				// advanced (25% retail)
				STT_ENNORMALS, STM_CREAT_GROUP, 4
			},{
				// expert (no retail)
				STT_ENNORMALS, STM_CREAT_GROUP, 0
			}
		}
	},{
		// Weakness 
		SPT_COMBAT, SPL_SECOND, MST_FIRE, SPC_FURTSKILL, 8, SLB_WEAKNESS, 0,
		{
			{
				// none (-3)
				STT_ALLENEMY, STM_CREAT_GROUP, FSK_ATTACK, -3
			},{
				// basic (-3)
				STT_ALLENEMY, STM_CREAT_GROUP, FSK_ATTACK, -3
			},{
				// advanced (-6)
				STT_ALLENEMY, STM_CREAT_GROUP, FSK_ATTACK, -6
			},{
				// expert (-6 + all)
				STT_ALLENEMY, STM_ALL, FSK_ATTACK, -6
			}
		}
	},{
		// Death ripple
		SPT_COMBAT, SPL_SECOND, MST_FIRE, SPC_DAMAGE, 10, SLB_NONE, 0,
		{
			{
				// none (pow*5)
				STT_ALLNORMALS, STM_ALL, 5
			},{
				// basic (pow*5)
				STT_ALLNORMALS, STM_ALL, 5
			},{
				// advanced (pow*7)
				STT_ALLNORMALS, STM_ALL, 7
			},{
				// expert (pow*10)
				STT_ALLNORMALS, STM_ALL, 10
			}
		}
	},{
		// Fireball
		SPT_COMBAT, SPL_THIRD, MST_FIRE, SPC_DAMAGE, 15, SLB_NONE, 0,
		{
			{
				// none (pow*15)
				STT_ALL, STM_BALL_SET, 15
			},{
				// basic (pow*15)
				STT_ALL, STM_BALL_SET, 15
			},{
				// advanced (pow*17)
				STT_ALL, STM_BALL_SET, 17
			},{
				// expert (pow*20)
				STT_ALL, STM_BALL_SET, 20
			}
		}
	},{
		// Misfortune 
		SPT_COMBAT, SPL_THIRD, MST_FIRE, SPC_FURTSKILL, 12, SLB_MISFORTUNE, 0,
		{
			{
				// none (-1)
				STT_ALLENEMY, STM_CREAT_GROUP, FSK_LUCK, -1
			},{
				// basic (-1)
				STT_ALLENEMY, STM_CREAT_GROUP, FSK_LUCK, -1
			},{
				// advanced (-2)
				STT_ALLENEMY, STM_CREAT_GROUP, FSK_LUCK, -2
			},{
				// expert (-2 + all)
				STT_ALLENEMY, STM_ALL, FSK_LUCK, -2
			}
		}
	},{
		// Animate dead
		SPT_COMBAT, SPL_THIRD, MST_FIRE, SPC_RESURRECT, 15, SLB_NONE, 0,
		{
			{
				// none (pow*40)
				STT_FRUNDEADS, STM_CREAT_GROUP, 40, 1
			},{
				// basic (pow*40)
				STT_FRUNDEADS, STM_CREAT_GROUP, 40, 1
			},{
				// advanced (pow*45)
				STT_FRUNDEADS, STM_CREAT_GROUP, 45, 1
			},{
				// expert (pow*50)
				STT_FRUNDEADS, STM_CREAT_GROUP, 50, 1
			}
		}
	},{
		// Fireblast 
		SPT_COMBAT, SPL_FOURTH, MST_FIRE, SPC_DAMAGE, 16, SLB_NONE, 0,
		{
			{
				// none (pow*15)
				STT_ALL, STM_BLAST_SET, 15
			},{
				// basic (pow*15)
				STT_ALL, STM_BLAST_SET, 15
			},{
				// advanced (pow*17)
				STT_ALL, STM_BLAST_SET, 17
			},{
				// expert (pow*20)
				STT_ALL, STM_BLAST_SET, 20
			}
		}
	},{
		// Armageddon
		SPT_COMBAT, SPL_FIFTH, MST_FIRE, SPC_DAMAGE, 30, SLB_NONE, 0,
		{
			{
				// none (pow*40)
				STT_ALL, STM_ALL, 40
			},{
				// basic (pow*40)
				STT_ALL, STM_ALL, 40
			},{
				// advanced (pow*45)
				STT_ALL, STM_ALL, 45
			},{
				// expert (pow*50)
				STT_ALL, STM_ALL, 50
			}
		}
	},{
		// Summon fire elemental 
		SPT_COMBAT, SPL_FIFTH, MST_FIRE, SPC_SUMMON, 25, SLB_NONE, 0,
		{
			{
				// none (pow*3)
				STT_ALL, STM_SUMMON, CREAT_FIRE_ELEMENTAL, 3
			},{
				// basic (pow*3)
				STT_ALL, STM_SUMMON, CREAT_FIRE_ELEMENTAL, 3
			},{
				// advanced (pow*4)
				STT_ALL, STM_SUMMON, CREAT_FIRE_ELEMENTAL, 4
			},{
				// expert (pow*5)
				STT_ALL, STM_SUMMON, CREAT_FIRE_ELEMENTAL, 5
			}
		}
	},{
		//////////////////////////////////////////////////////////////////////////
		// Water magic
		//////////////////////////////////////////////////////////////////////////

		// Bless
		SPT_COMBAT, SPL_FIRST, MST_WATER, SPC_BLESS, 5, SLB_BLESS, 0,
		{
			{
				// none (?)
				STT_FRNUNDEADS, STM_CREAT_GROUP, 1, MSP_CURSE
			},{
				// basic (?)
				STT_FRNUNDEADS, STM_CREAT_GROUP, 1, MSP_CURSE
			},{
				// advanced (?)
				STT_FRNUNDEADS, STM_CREAT_GROUP, 1, MSP_CURSE
			},{
				// expert (? + all)
				STT_FRNUNDEADS, STM_ALL, 1, MSP_CURSE
			}
		}
	},{
		// Protection from fire
		SPT_COMBAT, SPL_FIRST, MST_WATER, SPC_FURTSKILL, 5, SLB_PROTFIRE, 0,
		{
			{
				// none (30%)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_RES_FIRE, 30
			},{
				// basic (30%)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_RES_FIRE, 30
			},{
				// advanced (50%)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_RES_FIRE, 50
			},{
				// expert (50% + all)
				STT_ALLFRIENDLY, STM_ALL, FSK_RES_FIRE, 50
			}
		}
	},{
		// Dispel
		SPT_COMBAT, SPL_FIRST, MST_WATER, SPC_DISPEL, 5, SLB_NONE, 0,
		{
			{
				// none (friendly)
				STT_ALLFRIENDLY, STM_CREAT_GROUP
			},{
				// basic (friendly)
				STT_ALLFRIENDLY, STM_CREAT_GROUP
			},{
				// advanced (any)
				STT_ALL, STM_CREAT_GROUP
			},{
				// expert (all)
				STT_ALL, STM_ALL
			}
		}
	},{
		// Cure
		SPT_COMBAT, SPL_FIRST, MST_WATER, SPC_CURE, 6, SLB_NONE, 0,
		{
			{
				// none (pow*5)
				STT_FRNORMALS, STM_CREAT_GROUP, 5
			},{
				// basic (pow*5)
				STT_FRNORMALS, STM_CREAT_GROUP, 5
			},{
				// advanced (pow*7)
				STT_FRNORMALS, STM_CREAT_GROUP, 7
			},{
				// expert (pow*7 + all)
				STT_FRNORMALS, STM_ALL, 7
			}
		}
	},{
		// Cold ray
		SPT_COMBAT, SPL_SECOND, MST_WATER, SPC_DAMAGE, 8, SLB_NONE, 0,
		{
			{
				// none (pow*20)
				STT_ALLENEMY, STM_CREAT_GROUP, 20
			},{
				// basic (pow*20)
				STT_ALLENEMY, STM_CREAT_GROUP, 20
			},{
				// advanced (pow*22)
				STT_ALLENEMY, STM_CREAT_GROUP, 22
			},{
				// expert (pow*25)
				STT_ALLENEMY, STM_CREAT_GROUP, 25
			}
		}
	},{
		// Fortune
		SPT_COMBAT, SPL_SECOND, MST_WATER, SPC_FURTSKILL, 7, SLB_FORTUNE, 0,
		{
			{
				// none (+1)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_LUCK, 1
			},{
				// basic (+1)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_LUCK, 1
			},{
				// advanced (+2)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, FSK_LUCK, 2
			},{
				// expert (+2 + all)
				STT_ALLFRIENDLY, STM_ALL, FSK_LUCK, 2
			}
		}
	},{
		// Mirth
		SPT_COMBAT, SPL_THIRD, MST_WATER, SPC_FURTSKILL, 12, SLB_MIRTH, 0,
		{
			{
				// none (+1)
				STT_FRNUNDEADS, STM_CREAT_GROUP, FSK_MORALE, 1
			},{
				// basic (+1)
				STT_FRNUNDEADS, STM_CREAT_GROUP, FSK_MORALE, 1
			},{
				// advanced (+2)
				STT_FRNUNDEADS, STM_CREAT_GROUP, FSK_MORALE, 2
			},{
				// expert (+2 + all)
				STT_FRNUNDEADS, STM_ALL, FSK_MORALE, 2
			}
		}
	},{
		// Cold ring
		SPT_COMBAT, SPL_THIRD, MST_WATER, SPC_DAMAGE, 12, SLB_NONE, 0,
		{
			{
				// none (pow*10)
				STT_ALLENEMY, STM_RING_SET, 10
			},{
				// basic (pow*10)
				STT_ALLENEMY, STM_RING_SET, 10
			},{
				// advanced (pow*12)
				STT_ALLENEMY, STM_RING_SET, 12
			},{
				// expert (pow*15)
				STT_ALLENEMY, STM_RING_SET, 15
			}
		}
	},{
		// Anti-magic
		SPT_COMBAT, SPL_THIRD, MST_WATER, SPC_ANTIMAGIC, 15, SLB_ANTIMAGIC, 0,
		{
			{
				// none (1-3 level)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, SPL_MASK_FIRST | SPL_MASK_SECOND | SPL_MASK_THIRD
			},{
				// basic (1-3 level)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, SPL_MASK_FIRST | SPL_MASK_SECOND | SPL_MASK_THIRD
			},{
				// advanced (1-4 level)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, SPL_MASK_FIRST | SPL_MASK_SECOND | SPL_MASK_THIRD | SPL_MASK_FOURTH
			},{
				// expert (all spells)
				STT_ALLFRIENDLY, STM_CREAT_GROUP, SPL_MASK_ALL
			}
		}
	},{
		// Prayer 
		SPT_COMBAT, SPL_FOURTH, MST_WATER, SPC_PRAYER, 18, SLB_PRAYER, 0,
		{
			{
				// none (+2)
				STT_FRNUNDEADS, STM_CREAT_GROUP, 2
			},{
				// basic (+2)
				STT_FRNUNDEADS, STM_CREAT_GROUP, 2
			},{
				// advanced (+4)
				STT_FRNUNDEADS, STM_CREAT_GROUP, 4
			},{
				// expert (+4 + all)
				STT_FRNUNDEADS, STM_ALL, 4
			}
		}
	},{
		// Summon water elemental
		SPT_COMBAT, SPL_FIFTH, MST_WATER, SPC_SUMMON, 25, SLB_NONE, 0,
		{
			{
				// none (pow*3)
				STT_NONE, STM_SUMMON, CREAT_WATER_ELEMENTAL, 3
			},{
				// basic (pow*3)
				STT_NONE, STM_SUMMON, CREAT_WATER_ELEMENTAL, 3
			},{
				// advanced (pow*4)
				STT_NONE, STM_SUMMON, CREAT_WATER_ELEMENTAL, 4
			},{
				// expert (pow*5)
				STT_NONE, STM_SUMMON, CREAT_WATER_ELEMENTAL, 5
			}
		}
	},{
		//////////////////////////////////////////////////////////////////////////
		// New spells
		//////////////////////////////////////////////////////////////////////////

		// Summon Sprites
		SPT_COMBAT, SPL_SECOND, MST_WATER, SPC_SUMMON, 8, SLB_NONE, 0,
		{
			{
				// none (pow*5)
				STT_NONE, STM_SUMMON, CREAT_SPRITE, 5
			},{
				// basic (pow*5)
				STT_NONE, STM_SUMMON, CREAT_SPRITE, 5
			},{
				// advanced (pow*7)
				STT_NONE, STM_SUMMON, CREAT_SPRITE, 7
			},{
				// expert (pow*10)
				STT_NONE, STM_SUMMON, CREAT_SPRITE, 10
			}
		}
	}
};


