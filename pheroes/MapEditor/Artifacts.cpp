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

//////////////////////////////////////////////////////////////////////////
iArtifactNode::iArtifactNode(const iStringT& id, const iStringT& nameKey, const iStringT& descKey, ART_TYPE type, ART_LEVEL_TYPE level, ART_ASSIGN_TYPE assign, uint32 reqLevel, SECONDARY_SKILLS reqSecSkill, const SpriteId& spriteId)
: iBaseNode(nameKey)
, iIdxItem(id), m_descKey(descKey), m_Type(type), m_Level(level), m_assign(assign), m_reqLevel(reqLevel), m_reqSecSkill(reqSecSkill), m_SpriteId(spriteId) {}

iArtifactNode::iArtifactNode(RANDOM_ARTIFACTS type,const iStringT& id,  const iStringT& nameKey, const SpriteId& spriteId)
: iBaseNode(nameKey)
, iIdxItem(id, type), m_descKey(_T("VOID")), m_SpriteId(spriteId) {}

iMapItem_Artifact* iArtifactNode::CreateObject(const iPoint& pos)
{ return new iMapItem_Artifact(pos, this); }

void iArtifactNode::Serialize(iDynamicBuffer& buff)
{
	buff.Write((uint16)m_Type);
	buff.Write((uint8)m_Level);
	buff.Write((uint8)m_assign);
	buff.Write((uint16)m_reqLevel);
	buff.Write((sint8)m_reqSecSkill);
}

//
iArtifactNode_FurtSkill::iArtifactNode_FurtSkill(const iStringT& id, const iStringT& nameKey, const iStringT& descKey, ART_LEVEL_TYPE level, ART_ASSIGN_TYPE assign, uint32 reqLevel, SECONDARY_SKILLS reqSecSkill, const SpriteId& spriteId, const iFurtSkills& furtSkills)
: iArtifactNode(id, nameKey, descKey, ARTT_FURTSKILL, level, assign, reqLevel, reqSecSkill, spriteId), m_furtSkills(furtSkills) {}

void iArtifactNode_FurtSkill::Serialize(iDynamicBuffer& buff)
{
	iArtifactNode::Serialize(buff);
	::Serialize(buff, m_furtSkills);
}

//////////////////////////////////////////////////////////////////////////
iMapItem_Artifact::iMapItem_Artifact(const iPoint& pos, iArtifactNode* pProto)
: iMapItem(pos, MAPITEM_ARTIFACT, pProto->Sprite()), m_pProto(pProto) {}
