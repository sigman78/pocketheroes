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

#ifndef _HMM_GAME_ARTIFACTS_H_
#define _HMM_GAME_ARTIFACTS_H_

/*
 *	Artifacts container
 */
struct iArtCell {
	iArtCell(HERO_ART_CELL _cell, uint16 _artId = 0xFFFF) : cell(_cell), artId(_artId) {}
	inline bool Empty() const { return artId == 0xFFFF;}
	inline void Reset() { artId = 0xFFFF; }
	ART_ASSIGN_TYPE AssignType();

	HERO_ART_CELL	cell;
	uint16			artId;
};

typedef iSimpleArray<uint16>	iArtList;
typedef iSimpleArray<iArtCell>	iArtCellList;
//
class iArtCtr
{
public:
	iArtCtr(iHero* pOwner);

	//
	uint16 Pop();

	// 
	bool HasArtType(ART_TYPE artt) const;
	bool CanAttach(uint16 artId) const;
	void Push(uint16 artId);
	bool CanSmartPush(uint16 artId) const;
	void SmartPush(uint16 artId);
	void PushDressed(uint16 artId, HERO_ART_CELL cell);
	void PushToBackPack(uint16 artId);
	uint16 Pop(bool bDressed, uint32 idx);
	void PopBackPack(iArtList& artList);
	void PopAll(iArtList& artList);
	void GetAll(iArtList& artList) const;

	// Backpack
	inline uint32 BackPackCount() const { return m_BackPack.GetSize(); }
	inline uint16 BackPackItem(uint32 idx) const { check(idx<m_BackPack.GetSize()); return m_BackPack[idx]; }
	inline const iArtCell& DressedItem(HERO_ART_CELL cell) const { return m_Dressed[cell]; }
	inline iArtCell& DressedItem(HERO_ART_CELL cell) { return m_Dressed[cell]; }
	inline iArtList& BackPack() { return m_BackPack; }

	uint32 TotalCount() const;

private:
	iArtCellList	m_Dressed;
	iArtList		m_BackPack;
	iHero*			m_pOwner;
};

#endif //_HMM_GAME_ARTIFACTS_H_

