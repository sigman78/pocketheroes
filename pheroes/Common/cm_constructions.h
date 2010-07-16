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

#ifndef _HMM_COMMON_CONSTRUCTIONS_H_
#define _HMM_COMMON_CONSTRUCTIONS_H_

/*
 *	Predefined construction types
 */

// Visitables
enum VIS_CNST_TYPE {
	VCNST_BASIC = 0,	// Generic Visitable construction
	VCNST_STABLE,		// Adds 'n' action points (end of week to each visitor)
	VCNST_GAZEBO,		// Adds 'n' exp points (once to each visitor)
	VCNST_MANASOURCE,	// Adds 'n' mana points (end of week to each visitor)
	VCNST_MLMODIFIER,	// Modifies morale and luck
	VCNST_PSMODIFIER,	// Modifies set of primary skills by mask
	VCNST_OBELISK,		// Opens one element of puzzle map
	VCNST_SIGN,			// Shows message
	VCNST_DWELLING,		// Dwelling
	VCNST_WITCHHUT,		// Witch Hut (learn random or specified secondary skill)
	VCNST_SHRINE,		// Magic shrine (learn random spell of specified (1-3) level)
	VCNST_TREASURY,		// Resource treasury
	VCNST_TELEPORT,		// Teleports hero
	VCNST_KEYMASTER,	// Gives specified key
	VCNST_KNWLTREE,		// Tree of Knowledge
	VCNST_WINDMILL,		// Windmill (gives random ammount of random mineral one time per week)
	VCNST_WEEKLYMIN,	// Gives fixed ammount of specified mineral(s) one time per week
	VCNST_COUNT
						// Windmill (2-5 of random resource (except gold) one time per week)
						// Water wheel (+1000gp one time per week)
						// University (lern one or more from 4 secondray skills for 2.000)
						// Dragon utopia
						// Altar of sacrifice (creatures and artifacts to experience)
						// Market
						// Black market
						// Treasures ('n' units of 'x' creature type guards 'm' mineral set)
						// Den of thieves (shows detailed world information)
						// Shrine of magic (learns random 1-3 level spell)
						// Keymaster's Tent
						// Seer's hut
						// Crypt/Graveyard
						// Corps/Skeleton
};

// Ownerables
enum OWN_CNST_TYPE {
	OCNST_BASIC = 0,	// Generic Ownerable construction
	OCNST_FURTSKILL,	// Modifies one or several owner's further skills
	OCNST_COUNT
};

#endif //_HMM_COMMON_CONSTRUCTIONS_H_

