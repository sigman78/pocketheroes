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

#ifndef _HMM_MAP_EDITOR_ARTIFACTS_H_
#define _HMM_MAP_EDITOR_ARTIFACTS_H_

//////////////////////////////////////////////////////////////////////////
class iMapItem_Artifact;
class iArtifactNode : public iBaseNode, public iIdxItem
{
public:
	IMPL_TYPEAWARE( iArtifactNode );
	iArtifactNode(const iStringT& id, const iStringT& nameKey, const iStringT& descKey, ART_TYPE type, ART_LEVEL_TYPE level, ART_ASSIGN_TYPE assign, uint32 reqLevel, SECONDARY_SKILLS reqSecSkill, const SpriteId& spriteId);
	iArtifactNode(RANDOM_ARTIFACTS type, const iStringT& id, const iStringT& nameKey, const SpriteId& spriteId);
	virtual ~iArtifactNode() {}

	iMapItem_Artifact* CreateObject(const iPoint& pos);
	virtual void Serialize(iDynamicBuffer& buff);

	inline const iStringT& DescKey() const { return m_descKey; }
	inline ART_TYPE Type() const { return m_Type; }
	inline ART_LEVEL_TYPE Level() const { return m_Level; }
	inline ART_ASSIGN_TYPE Assign() const { return m_assign; }
	inline const SpriteId& Sprite() const { return m_SpriteId; }
	inline bool IsRandom() const { return Idx() >= ARTT_RAND; }

private:
	iStringT			m_descKey;
	ART_TYPE			m_Type;
	ART_LEVEL_TYPE		m_Level;
	ART_ASSIGN_TYPE		m_assign;
	SpriteId			m_SpriteId;
	uint32				m_reqLevel;
	SECONDARY_SKILLS	m_reqSecSkill;
};

//
class iArtifactNode_FurtSkill : public iArtifactNode
{
public:
	iArtifactNode_FurtSkill(const iStringT& id, const iStringT& nameKey, const iStringT& descKey, ART_LEVEL_TYPE level, ART_ASSIGN_TYPE assign, uint32 reqLevel, SECONDARY_SKILLS reqSecSkill, const SpriteId& spriteId, const iFurtSkills& furtSkills);

	void Serialize(iDynamicBuffer& buff);

	inline const iFurtSkills& FurtSkills() const { return m_furtSkills; }

private:
	iFurtSkills m_furtSkills;
};

//////////////////////////////////////////////////////////////////////////
class iMapItem_Artifact : public iMapItem
{
public:
	iMapItem_Artifact(const iPoint& pos, iArtifactNode* pProto);
	inline iArtifactNode* Proto() { return m_pProto; }

private:
	iArtifactNode*	m_pProto;
};

#endif //_HMM_MAP_EDITOR_ARTIFACTS_H_