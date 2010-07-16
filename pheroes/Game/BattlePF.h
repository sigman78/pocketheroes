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

#ifndef _HMM_GAME_BATTLE_PATH_FINDING_H_
#define _HMM_GAME_BATTLE_PATH_FINDING_H_

// Cell's nebs [even, odd row][right,left direction][id 0-2][x,y offset] (<-even ids(0,2,4), ->odd ids(1,3,5))
extern const sint16 HEX_NEBS[2][2][3][2];
void CalculatePath(iBattleGroup* pGroup, iPoint& dstPoint, uint8& orient);
/*
 *	Path finding
 */
class iBattlePF
{
public:
	iBattlePF(iBattleGroup* pGroup);

	void MakeDistMap(bool bFull);
	bool MakePath(const iPoint& pos, uint8 dir, bool bMove, iBattleActList& actList);
	
private:
	void PropagateDown(sint16 px, sint16 py, uint8 dir, uint32 step, bool bFull);
	void PropagateDown2(sint16 px, sint16 py, uint8 dir, uint32 step, bool bFull);
	void MakePath1(const iPoint& pos, uint8 dir, iBattleActList& actList);
	void MakePath2(const iPoint& pos, uint8 dir, bool bMove, iBattleActList& actList);

private:
	iBattleGroup*	m_pGroup;
};

#endif //_HMM_GAME_BATTLE_PATH_FINDING_H_

