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
#include "serialize.h"

/*
 *	Prototypes (nodes)
 */
iVisCnst* iBaseVisCnstNode::CreateObject(const iPoint& pos)
{ return new iVisCnst(pos, this); }
//
void iBaseVisCnstNode::Serialize(iDynamicBuffer& buff)
{
	buff.Write((uint16)m_Type);
	buff.Write(gItemMgr.m_TextMgr.GetEntry(m_NameKey).idx);
	buff.Write(gItemMgr.m_TextMgr.GetEntry(m_msgKey).idx);
	buff.Write(gItemMgr.m_TextMgr.GetEntry(m_vmsgKey).idx);
}
//
void iVisCnstNode_Stable::Serialize(iDynamicBuffer& buff)
{
	iBaseVisCnstNode::Serialize(buff);
	buff.Write((sint16)m_ActPoints);
}
//
void iVisCnstNode_ManaSource::Serialize(iDynamicBuffer& buff)
{
	iBaseVisCnstNode::Serialize(buff);
	buff.Write((sint16)m_ManaPoints);
}
//
void iVisCnstNode_MLModifier::Serialize(iDynamicBuffer& buff)
{
	iBaseVisCnstNode::Serialize(buff);
	buff.Write(m_moraleModifier);
	buff.Write(m_luckModifier);
}
//
void iVisCnstNode_PSModifier::Serialize(iDynamicBuffer& buff)
{
	iBaseVisCnstNode::Serialize(buff);
	::Serialize(buff,m_psModifier);
}
//
void iVisCnstNode_Dwelling::Serialize(iDynamicBuffer& buff)
{
	iBaseVisCnstNode::Serialize(buff);
	buff.Write((uint16)m_creatType);
}
//
void iVisCnstNode_Shrine::Serialize(iDynamicBuffer& buff)
{
	iBaseVisCnstNode::Serialize(buff);
	buff.Write((uint8)m_level);
}
//
void iVisCnstNode_Treasury::Serialize(iDynamicBuffer& buff)
{
	iBaseVisCnstNode::Serialize(buff);
	m_vars.Serialize(buff);
}
//
void iVisCnstNode_Teleport::Serialize(iDynamicBuffer& buff)
{
	iBaseVisCnstNode::Serialize(buff);
	buff.Write((uint8)m_mode);
	buff.Write((uint8)m_index);
}
//
void iVisCnstNode_Keymaster::Serialize(iDynamicBuffer& buff)
{
	iBaseVisCnstNode::Serialize(buff);
	buff.Write((uint8)m_key);
}
//
void iVisCnstNode_WeeklyMin::Serialize(iDynamicBuffer& buff)
{
	iBaseVisCnstNode::Serialize(buff);
	::Serialize(buff, m_minerals);
}

//////////////////////////////////////////////////////////////////////////
iOwnCnst* iBaseOwnCnstNode::CreateObject(const iPoint& pos, PLAYER_ID owner)
{ return new iOwnCnst(pos,owner,this); }
//
void iBaseOwnCnstNode::Serialize(iDynamicBuffer& buff)
{
	buff.Write((uint16)m_Type);
	buff.Write((uint8)m_Scouting);
	buff.Write(gItemMgr.m_TextMgr.GetEntry(m_NameKey).idx);
	buff.Write(gItemMgr.m_TextMgr.GetEntry(m_msgKey).idx);
	if (m_msgIcn.Empty()) buff.Write((uint16)0xFFFF);
	else {
		const iSpriteMgr& spMgr = gItemMgr.m_SpriteMgr;
		buff.Write((uint16)spMgr[m_msgIcn].id);
	}
}
//
void iOwnCnstNode_FurtSkill::Serialize(iDynamicBuffer& buff)
{
	iBaseOwnCnstNode::Serialize(buff);
	::Serialize(buff, m_furtSkills);
}
//////////////////////////////////////////////////////////////////////////
void iCastle::iCtlCnsts::Init(iCastle* pCastle)
{
	for (uint32 xx=0; xx<CTLCNST_COUNT; ++xx){
		if (CTLCNSTS_DESC[xx].caps.Support(pCastle->Proto()->Type(),pCastle->Proto()->Size())) {
			m_cnsts.Add(iCnstEntry((CTL_CONSTR)xx));
		}
	}
}

void iCastle::iCtlCnsts::SetState(CTL_CONSTR type, uint32 state)
{
	for (uint32 xx=0; xx<m_cnsts.GetSize(); ++xx) {
		if (m_cnsts[xx].type == type) {
			m_cnsts[xx].state = state;
			return;
		}
	}
}

const iStringT& iCastle::Name() const
{ 
	/*if (!m_CustomName.Empty()) return m_CustomName; 
	else */return gItemMgr.m_TextMgr[m_pProto->NameKey()]; 
}

iCastle* iCastleNode::CreateObject(const iPoint& pos, PLAYER_ID owner)
{ return new iCastle(pos, owner, this); }

/*
 *	Objects
 */
