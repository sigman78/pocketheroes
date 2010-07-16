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

/*
 *	
 */
ART_ASSIGN_TYPE iArtCell::AssignType()
{
	if (artId != 0xFFFF) return gGame.ItemMgr().m_artMgr[artId].Assign();
	check(cell != AC_UNDEFINED);
	return HERO_ART_CELL_ASSIGN[cell];
}

/*
 *	
 */
iArtCtr::iArtCtr(iHero* pOwner)
: m_pOwner(pOwner) 
{
	for (uint32 xx=0; xx<AC_COUNT; ++xx) m_Dressed.Add(iArtCell((HERO_ART_CELL)xx));
}

uint16 iArtCtr::Pop()
{
	check(BackPackCount());
	return m_BackPack.Pop();
}

bool iArtCtr::HasArtType(ART_TYPE artt) const
{
	for (uint32 xx=0; xx<m_Dressed.GetSize(); ++xx) {
		if (!m_Dressed[xx].Empty() && gGame.ItemMgr().m_artMgr[m_Dressed[xx].artId].Type() == artt) return true;
	}
	return false;
}

bool iArtCtr::CanAttach(uint16 artId) const
{
	return gGame.ItemMgr().m_artMgr[artId].CanAttach(m_pOwner);
}

void iArtCtr::Push(uint16 artId)
{
	// try to dress
	if (CanAttach(artId)) {
		for (uint32 xx=0; xx<m_Dressed.GetSize(); ++xx){
			if (HERO_ART_CELL_ASSIGN[m_Dressed[xx].cell] == gGame.ItemMgr().m_artMgr[artId].Assign() && m_Dressed[xx].Empty()) {
				PushDressed(artId, (HERO_ART_CELL)xx);
				return;
			}
		}
	}

	// push to back pack
	PushToBackPack(artId);
}

bool iArtCtr::CanSmartPush(uint16 artId) const
{
	iArtifactMgr& artMgr = gGame.ItemMgr().m_artMgr;
	// find cell with most weak weared artifact
	HERO_ART_CELL cell = AC_UNDEFINED;
	ART_LEVEL_TYPE level = artMgr[artId].Level();
	if (CanAttach(artId) && m_pOwner->NeedArtifact(artId)) {
		for (uint32 xx=0; xx<m_Dressed.GetSize(); ++xx){
			if (HERO_ART_CELL_ASSIGN[m_Dressed[xx].cell] == artMgr[artId].Assign()) {
				if (m_Dressed[xx].Empty()) {
					cell = (HERO_ART_CELL)xx;
					level = ART_LEVEL_NONE;
					break;
				} else if (artMgr[m_Dressed[xx].artId].Level() < level) {
					cell = (HERO_ART_CELL)xx;
					level = artMgr[m_Dressed[xx].artId].Level();
				}
			}
		}
	}

	return (cell != AC_UNDEFINED);
}

void iArtCtr::SmartPush(uint16 artId)
{
	iArtifactMgr& artMgr = gGame.ItemMgr().m_artMgr;
	// find cell with most weak weared artifact
	HERO_ART_CELL cell = AC_UNDEFINED;
	ART_LEVEL_TYPE level = artMgr[artId].Level();
	if (CanAttach(artId) && m_pOwner->NeedArtifact(artId)) {
		for (uint32 xx=0; xx<m_Dressed.GetSize(); ++xx){
			if (HERO_ART_CELL_ASSIGN[m_Dressed[xx].cell] == artMgr[artId].Assign()) {
				if (m_Dressed[xx].Empty()) {
					cell = (HERO_ART_CELL)xx;
					level = ART_LEVEL_NONE;
					break;
				} else if (artMgr[m_Dressed[xx].artId].Level() < level) {
					cell = (HERO_ART_CELL)xx;
					level = artMgr[m_Dressed[xx].artId].Level();
				}
			}
		}
	}

	if (cell == AC_UNDEFINED) {
		// push to back pack
		PushToBackPack(artId);
	} else {
		if (!m_Dressed[cell].Empty()) {
			// detach
			uint16 oldArt = Pop(true, cell);
			// move to backpack
			PushToBackPack(oldArt);
		}
		// wear
		PushDressed(artId, cell);
	}
}

void iArtCtr::PushDressed(uint16 artId, HERO_ART_CELL cell)
{
	check(m_Dressed[cell].Empty() && CanAttach(artId));
	m_Dressed[cell].artId = artId;
	gGame.ItemMgr().m_artMgr[artId].Attach(m_pOwner);
}

void iArtCtr::PushToBackPack(uint16 artId)
{
	m_BackPack.Add(artId);
}

uint16 iArtCtr::Pop(bool bDressed, uint32 idx)
{
	if (bDressed) {
		check(idx<m_Dressed.GetSize() && !m_Dressed[idx].Empty());
		uint16 result = m_Dressed[idx].artId;
		m_Dressed[idx].Reset();
		gGame.ItemMgr().m_artMgr[result].Dettach(m_pOwner);
		return result;
	} else {
		check(idx<m_BackPack.GetSize());
		uint16 result = m_BackPack[idx];
		m_BackPack.RemoveAt(idx);
		return result;
	}
}

void iArtCtr::PopBackPack(iArtList& artList)
{
	artList += m_BackPack;
	m_BackPack.RemoveAll();
}

void iArtCtr::PopAll(iArtList& artList)
{
	// Dressed artifacts
	for (uint32 xx=0; xx<m_Dressed.GetSize(); ++xx) {
		if (m_Dressed[xx].artId != 0xFFFF) {
			artList.Add(m_Dressed[xx].artId);
			gGame.ItemMgr().m_artMgr[m_Dressed[xx].artId].Dettach(m_pOwner);
			m_Dressed[xx].artId = 0xFFFF;
		}
	}

	// BackPack artifacts
	PopBackPack(artList);
}

void iArtCtr::GetAll(iArtList& artList) const
{
	// Dressed artifacts
	for (uint32 xx=0; xx<m_Dressed.GetSize(); ++xx) {
		if (m_Dressed[xx].artId != 0xFFFF) {
			artList.Add(m_Dressed[xx].artId);
		}
	}
	// BackPack artifacts
	artList += m_BackPack;
}

uint32 iArtCtr::TotalCount() const
{
	uint32 res = 0;
	// Dressed artifacts
	for (uint32 xx=0; xx<m_Dressed.GetSize(); ++xx) if (m_Dressed[xx].artId != 0xFFFF) res++;
	// BackPack artifacts
	res += m_BackPack.GetSize();
	return res;
}

