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

#ifndef _HMM_RESOURCE_MANAGER_H_
#define _HMM_RESOURCE_MANAGER_H_

/*
 *	
 */
inline sint32 FindStr(LPCTSTR *pStrList, uint32 count, LPCTSTR pSubStr)
{
	for (uint32 xx=0; xx<count; ++xx){
		if (_tcsicmp(pStrList[xx],pSubStr) == 0) return xx;
	}
	return -1;
}

/*
 *	
 */
class iObjGroupNode : public iGroupNode
{
public:
	iObjGroupNode(const iStringT& name)
	: iGroupNode(name)
	{}

	iArtifactNode* GetArtifactNode(const iStringT& id)
	{
		Iterator it = First();
		while (*it) {
			if (iArtifactNode* pArtifact = DynamicCast<iArtifactNode*>((iBaseNode*)*it)) {
				if (pArtifact->Id() == id) return pArtifact;
			}
			it = it.WalkNext();
		}
		return NULL;
	}

	iArtifactNode* GetArtifactNode(uint16 idx)
	{
		Iterator it = First();
		while (*it) {
			if (iArtifactNode* pArtifact = DynamicCast<iArtifactNode*>((iBaseNode*)*it)) {
				if (pArtifact->Idx() == idx) return pArtifact;
			}
			it = it.WalkNext();
		}
		return NULL;
	}
	
	iDecorNode* GetDecorNode(const iStringT& id)
	{
		Iterator it = First();
		while (*it) {
			if (iDecorNode* pDecor = DynamicCast<iDecorNode*>((iBaseNode*)*it)) {
				if (pDecor->Id() == id) return pDecor;
			}
			it = it.WalkNext();
		}
		return NULL;
	}

	iPathElNode* GetPathElNode(const iStringT& id)
	{
		Iterator it = First();
		while (*it) {
			if (iPathElNode* pPathEl = DynamicCast<iPathElNode*>((iBaseNode*)*it)) {
				if (pPathEl->Id() == id) return pPathEl;
			}
			it = it.WalkNext();
		}
		return NULL;
	}

	iBaseOwnCnstNode* GetOwnCnstNode(const iStringT& id)
	{
		Iterator it = First();
		while (*it) {
			if (iBaseOwnCnstNode* pOwnCnst = DynamicCast<iBaseOwnCnstNode*>((iBaseNode*)*it)) {
				if (pOwnCnst->Id() == id) return pOwnCnst;
			}
			it = it.WalkNext();
		}
		return NULL;
	}

	iCastleNode* GetCastleNode(const iStringT& id, CTL_TYPE type)
	{
		Iterator it = First()+type;
		for (Iterator cit = (*it)->First(); cit != (*it)->End(); ++cit) {
			iCastleNode* pCastle = DynamicCast<iCastleNode*>((iBaseNode*)*cit);
			check(pCastle);
			if (pCastle->Id() == id) return pCastle;
		}
		/*
		while (*it) {
			if (iCastleNode* pCastle = DynamicCast<iCastleNode*>((iBaseNode*)*it)) {
				if (pCastle->Id() == id && pCastle->Type() == type) return pCastle;
			}
			it = it.WalkNext();
		}
		*/
		return NULL;
	}	

	iBaseVisCnstNode* GetVisCnstNode(const iStringT& id)
	{
		Iterator it = First();
		while (*it) {
			if (iBaseVisCnstNode* pVisCnst = DynamicCast<iBaseVisCnstNode*>((iBaseNode*)*it)) {
				if (pVisCnst->Id() == id) return pVisCnst;
			}
			it = it.WalkNext();
		}
		return NULL;
	}

	iHeroNode* GetHeroNode(const iStringT& id)
	{
		Iterator it = First();
		while (*it) {
			if (iHeroNode* pHeroNode = DynamicCast<iHeroNode*>((iBaseNode*)*it)) {
				if (pHeroNode->Id() == id) return pHeroNode;
			}
			it = it.WalkNext();
		}
		return NULL;
	}
};

/*
 *	
 */
class iItemMgr : public iTreeNode
{
public:
	iItemMgr()
	: m_bInited(false) 
	{}

	~iItemMgr() 
	{}

	bool Init(const iStringT& fname);
	void Reset();

	iObjGroupNode*			m_pResNode;
	iObjGroupNode*			m_pArtifactNode;
	iObjGroupNode*			m_pDecorNode;
	iObjGroupNode*			m_pPathElNode;
	iObjGroupNode*			m_pVisCnstNode;
	iObjGroupNode*			m_pOwnCnstNode;
	iObjGroupNode*			m_pCastleNode;
	iObjGroupNode*			m_pHeroesNode;
	iObjGroupNode*			m_pGuardsNode;
	iStringT				m_SetName;
	iTextMgr				m_TextMgr;
	iSpriteMgr				m_SpriteMgr;
	iSoundMgr				m_SoundMgr;
	iAvatarMgr				m_AvatarMgr;

	iSimpleArray<iBattleObstacle>	m_battleObstacles;

private:
	bool			m_bInited;
};



#endif //_HMM_RESOURCE_MANAGER_H_