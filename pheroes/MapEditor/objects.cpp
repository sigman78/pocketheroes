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

//////////////////////////////////////////////////////////////////////////
iMapItem::iMapItem(const iPoint& pos,  MAP_ITEM_TYPE mit, const SpriteId& sid)
: iBaseMapObject(pos), m_mapItemType(mit), m_spriteId(sid) {}

inline iStringT MineralSprite(MINERAL_TYPE mt)
{
	iStringT sm(MINERAL_ID[mt]);
	sm.Lower();
	return iFormat(_T("resources.%s"),sm.CStr());
}

//////////////////////////////////////////////////////////////////////////
iMapItem_Mineral::iMapItem_Mineral(const iPoint& pos, MINERAL_TYPE mt, sint32 count)
: iMapItem(pos,MAPITEM_MINERAL, (mt==MINERAL_UNKNOWN)?_T("resources.rand_res_icn"):iFormat(_T("resources.icons_%04d"),mt)), m_mineralType(mt), m_count(count) {}
//////////////////////////////////////////////////////////////////////////
iMapItem_ManaCrystal::iMapItem_ManaCrystal(const iPoint& pos, sint32 count)
: iMapItem(pos,MAPITEM_MANACRYSTAL, _T("resources.mcryst")), m_count(count) {}
//////////////////////////////////////////////////////////////////////////
iMapItem_CampFire::iMapItem_CampFire(const iPoint& pos)
: iMapItem(pos, MAPITEM_CAMPFIRE, _T("resources.campfire")) {}
//////////////////////////////////////////////////////////////////////////
iMapItem_Chest::iMapItem_Chest(const iPoint& pos)
: iMapItem(pos, MAPITEM_CHEST, _T("resources.chest")) {}
//////////////////////////////////////////////////////////////////////////
iMapItem_SpellScroll::iMapItem_SpellScroll(const iPoint& pos, uint16 spellId)
: iMapItem(pos, MAPITEM_SPELLSCROLL, _T("resources.spell_scroll")), m_spellId(spellId) {}
//////////////////////////////////////////////////////////////////////////
iMapItem_Lamp::iMapItem_Lamp(const iPoint& pos)
: iMapItem(pos, MAPITEM_LAMP, _T("resources.lamp")) {}
//////////////////////////////////////////////////////////////////////////
iMapItem_KeyGuard::iMapItem_KeyGuard(const iPoint& pos, uint8 key)
: iMapItem(pos, MAPITEM_KEYGUARD, iFormat(_T("resources.key_guards_%04d"),key)), m_key(key) {}

//////////////////////////////////////////////////////////////////////////
iMapGuard::iMapGuard(const iPoint& pos, CREATURE_TYPE ct, sint32 count, GUARD_DISPOS disp, bool bNotGrow)
: iBaseMapObject(pos), m_creat(ct, count), m_disp(disp), m_bNotGrow(bNotGrow) 
{
	if (ct < CREAT_RANDOM) m_spriteId = iFormat(_T("creatures.minimon_%04d"),ct);
	else m_spriteId = iFormat(_T("creatures.rand_guards_%04d"),ct-CREAT_RANDOM);
}

const iStringT& iMapGuard::Title() const
{
	return gItemMgr.m_TextMgr[(m_creat.Type() < CREAT_RANDOM)?CREAT_NAMEKEY[m_creat.Type()]:RAND_GUARDS_NAMEKEY[m_creat.Type()-CREAT_RANDOM]];
}
//////////////////////////////////////////////////////////////////////////
iMapEvent::iMapEvent(const iPoint& pos)
: iBaseMapObject(pos), m_playerMask(PIM_ALL)
{
}

//////////////////////////////////////////////////////////////////////////
iHero::iHero(const iPoint& pos, iHeroNode* proto, PLAYER_ID owner)
: iBaseMapObject(pos), m_pProto(proto), m_Owner(owner), m_Experience(0), m_Level(0), m_bCustomSpells(false) {}
