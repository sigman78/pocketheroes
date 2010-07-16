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

#ifndef _HMM_GAME_HELPER_FUNCTIONS_H_
#define _HMM_GAME_HELPER_FUNCTIONS_H_


sint32 InitialTroopQuantity(CREATURE_TYPE ct);
void InitCreatGroup(iCreatGroup& cg);
void InitArmy(iArmy& army);
uint32 SelectSpells(iSpellList& spIds, iSpellFilter filter, uint32 count, MAGIC_SPELL reqSpell);
sint32 CalcRandValue(const uint8 pVals[], sint32 siz);
sint32 InitialExpPts();
iStringT FormatDate(uint32 timestamp, bool bShowTime);

// helper to save game to a file
bool SaveGameFile( const iStringT& fname, iGameWorld& gmap );

#endif //_HMM_GAME_HELPER_FUNCTIONS_H_

