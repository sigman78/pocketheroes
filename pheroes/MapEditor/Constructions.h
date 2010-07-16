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

#ifndef _HMM_MAPEDITOR_CONSTRUCTIONS_H_
#define _HMM_MAPEDITOR_CONSTRUCTIONS_H_

/*
 *	Prototypes (nodes)
 */
class iVisCnst;
class iBaseVisCnstNode : public iDecorNode
{
public:
	IMPL_TYPEAWARE( iBaseVisCnstNode );
	iBaseVisCnstNode(const iStringT& id, const iStringT& nameKey, const iStringT& msgKey, const iStringT& vmsgKey, VIS_CNST_TYPE type, bool bFixedGuard, bool bCustomMessage) 
	: iDecorNode(id, nameKey), m_msgKey(msgKey), m_vmsgKey(vmsgKey), m_Type(type), m_bFixedGuard(bFixedGuard), m_bCustomMessage(bCustomMessage) {}
	virtual ~iBaseVisCnstNode() {}

	iVisCnst* CreateObject(const iPoint& pos);
	virtual void Serialize(iDynamicBuffer& buff);
	inline VIS_CNST_TYPE Type() const { return m_Type; }
	inline const iStringT& MsgKey() const { return m_msgKey; }
	inline const iStringT& VisMsgKey() const { return m_vmsgKey; }
	inline bool FixedGuard() const { return m_bFixedGuard; }
	inline bool CustomizableMessage() const { return m_bCustomMessage; }

protected:
	bool			m_bFixedGuard;
	bool			m_bCustomMessage;
	iStringT		m_msgKey;
	iStringT		m_vmsgKey;
	VIS_CNST_TYPE	m_Type;
};

//
class iVisCnstNode_Stable : public iBaseVisCnstNode
{
public:
	iVisCnstNode_Stable(const iStringT& id, const iStringT& nameKey, const iStringT& msgKey, const iStringT& vmsgKey, sint32 act_pts)
	: iBaseVisCnstNode(id, nameKey, msgKey, vmsgKey, VCNST_STABLE, false, false), m_ActPoints(act_pts) {}
	void Serialize(iDynamicBuffer& buff);

protected:
	sint32	m_ActPoints;
};
//
class iVisCnstNode_Gazebo : public iBaseVisCnstNode
{
public:
	iVisCnstNode_Gazebo(const iStringT& id, const iStringT& nameKey, const iStringT& msgKey, const iStringT& vmsgKey)
	: iBaseVisCnstNode(id, nameKey, msgKey, vmsgKey, VCNST_GAZEBO, false, false){}
};
//
class iVisCnstNode_ManaSource : public iBaseVisCnstNode
{
public:
	iVisCnstNode_ManaSource(const iStringT& id, const iStringT& nameKey, const iStringT& msgKey, const iStringT& vmsgKey, sint32 mana_pts)
	: iBaseVisCnstNode(id, nameKey, msgKey, vmsgKey, VCNST_MANASOURCE, false, false), m_ManaPoints(mana_pts) {}
	void Serialize(iDynamicBuffer& buff);
	
protected:
	sint32	m_ManaPoints;
};
//
class iVisCnstNode_MLModifier : public iBaseVisCnstNode
{
public:
	iVisCnstNode_MLModifier(const iStringT& id, const iStringT& nameKey, const iStringT& msgKey, const iStringT& vmsgKey, sint8 moraleM, sint8 luckM)
	: iBaseVisCnstNode(id, nameKey, msgKey, vmsgKey, VCNST_MLMODIFIER, false, false), m_moraleModifier(moraleM), m_luckModifier(luckM) {}
	void Serialize(iDynamicBuffer& buff);
	
protected:
	sint8	m_moraleModifier;
	sint8	m_luckModifier;
};
//
class iVisCnstNode_PSModifier : public iBaseVisCnstNode
{
public:
	iVisCnstNode_PSModifier(const iStringT& id, const iStringT& nameKey, const iStringT& msgKey, const iStringT& vmsgKey, const iPrSkills& psM)
	: iBaseVisCnstNode(id, nameKey, msgKey, vmsgKey, VCNST_PSMODIFIER, false, false), m_psModifier(psM) {}
	void Serialize(iDynamicBuffer& buff);
	
protected:
	iPrSkills	m_psModifier;
};
//
class iVisCnstNode_Obelisk : public iBaseVisCnstNode
{
public:
	iVisCnstNode_Obelisk(const iStringT& id, const iStringT& nameKey, const iStringT& msgKey, const iStringT& vmsgKey)
	: iBaseVisCnstNode(id, nameKey, msgKey, vmsgKey, VCNST_OBELISK, false, false) {}
};
//
class iVisCnstNode_Sign : public iBaseVisCnstNode
{
public:
	iVisCnstNode_Sign(const iStringT& id, const iStringT& nameKey, const iStringT& msgKey, const iStringT& vmsgKey)
	: iBaseVisCnstNode(id, nameKey, msgKey, vmsgKey, VCNST_SIGN, false, true) {}
};
//
class iVisCnstNode_Dwelling : public iBaseVisCnstNode
{
public:
	iVisCnstNode_Dwelling(const iStringT& id, const iStringT& nameKey, const iStringT& msgKey, const iStringT& vmsgKey, CREATURE_TYPE ctype)
	: iBaseVisCnstNode(id, nameKey, msgKey, vmsgKey, VCNST_DWELLING, false, false), m_creatType(ctype) {}
	void Serialize(iDynamicBuffer& buff);

	CREATURE_TYPE m_creatType;
};
//
class iVisCnstNode_WitchHut : public iBaseVisCnstNode
{
public:
	iVisCnstNode_WitchHut(const iStringT& id, const iStringT& nameKey, const iStringT& msgKey, const iStringT& vmsgKey)
	: iBaseVisCnstNode(id, nameKey, msgKey, vmsgKey, VCNST_WITCHHUT, false, false) {}
};
//
class iVisCnstNode_Shrine : public iBaseVisCnstNode
{
public:
	iVisCnstNode_Shrine(const iStringT& id, const iStringT& nameKey, const iStringT& msgKey, const iStringT& vmsgKey, SPELL_LEVEL level)
	: iBaseVisCnstNode(id, nameKey, msgKey, vmsgKey, VCNST_SHRINE, false, false), m_level(level) {}
	void Serialize(iDynamicBuffer& buff);

	SPELL_LEVEL m_level;
};
//
class iVisCnstNode_Treasury : public iBaseVisCnstNode
{
public:
	iVisCnstNode_Treasury(const iStringT& id, const iStringT& nameKey, const iStringT& msgKey, const iStringT& vmsgKey)
	: iBaseVisCnstNode(id, nameKey, msgKey, vmsgKey, VCNST_TREASURY, true, false){}
	void Serialize(iDynamicBuffer& buff);

	iTrVarCtr	m_vars;
};
//
class iVisCnstNode_Teleport : public iBaseVisCnstNode
{
public:
	iVisCnstNode_Teleport(const iStringT& id, const iStringT& nameKey, const iStringT& msgKey, const iStringT& vmsgKey, uint8 mode, uint32 index)
	: iBaseVisCnstNode(id, nameKey, msgKey, vmsgKey, VCNST_TELEPORT, true, false), m_mode(mode), m_index(index) {}
	void Serialize(iDynamicBuffer& buff);
	inline uint32 GetIndex() {return m_index;}
protected:
	uint8	m_mode;
	uint32	m_index;
};
//
class iVisCnstNode_Keymaster : public iBaseVisCnstNode
{
public:
	iVisCnstNode_Keymaster(const iStringT& id, const iStringT& nameKey, const iStringT& msgKey, const iStringT& vmsgKey, uint8 key)
	: iBaseVisCnstNode(id, nameKey, msgKey, vmsgKey, VCNST_KEYMASTER, true, false), m_key(key) {}
	void Serialize(iDynamicBuffer& buff);

protected:
	uint8	m_key;
};
//
class iVisCnstNode_TreeOfKnowledge : public iBaseVisCnstNode
{
public:
	iVisCnstNode_TreeOfKnowledge(const iStringT& id, const iStringT& nameKey, const iStringT& msgKey, const iStringT& vmsgKey)
	: iBaseVisCnstNode(id, nameKey, msgKey, vmsgKey, VCNST_KNWLTREE, false, false) {}
};
//
class iVisCnstNode_Windmill : public iBaseVisCnstNode
{
public:
	iVisCnstNode_Windmill(const iStringT& id, const iStringT& nameKey, const iStringT& msgKey, const iStringT& vmsgKey)
	: iBaseVisCnstNode(id, nameKey, msgKey, vmsgKey, VCNST_WINDMILL, false, false) {}
};
//
class iVisCnstNode_WeeklyMin : public iBaseVisCnstNode
{
public:
	iVisCnstNode_WeeklyMin(const iStringT& id, const iStringT& nameKey, const iStringT& msgKey, const iStringT& vmsgKey)
	: iBaseVisCnstNode(id, nameKey, msgKey, vmsgKey, VCNST_WEEKLYMIN, false, false) { m_minerals.Reset(); }
	void Serialize(iDynamicBuffer& buff);
	iMineralSet& Minerals() { return m_minerals; }

private:
	iMineralSet	m_minerals;
};
//////////////////////////////////////////////////////////////////////////
class iOwnCnst;
class iBaseOwnCnstNode : public iDecorNode
{
public:
	IMPL_TYPEAWARE( iBaseOwnCnstNode );
	iBaseOwnCnstNode(const iStringT& id, const iStringT& nameKey, const iStringT& msgKey, const SpriteId& msgIcn, OWN_CNST_TYPE type, uint8 scouting) 
	: iDecorNode(id, nameKey), m_msgKey(msgKey), m_msgIcn(msgIcn), m_Type(type), m_Scouting(scouting) {}
	virtual ~iBaseOwnCnstNode(){}

	iOwnCnst* CreateObject(const iPoint& pos, PLAYER_ID owner);
	virtual void Serialize(iDynamicBuffer& buff);
	inline OWN_CNST_TYPE Type() const { return m_Type; }
	inline uint8 Scouting() const { return m_Scouting; }
	inline const iStringT& MsgKey() const { return m_msgKey; }

protected:
	iStringT		m_msgKey;
	SpriteId		m_msgIcn;
	uint8			m_Scouting;
	OWN_CNST_TYPE	m_Type;
};
//
class iOwnCnstNode_FurtSkill : public iBaseOwnCnstNode
{
public:
	iOwnCnstNode_FurtSkill(const iStringT& id, const iStringT& nameKey, const iStringT& msgKey, const SpriteId& msgIcn, const iFurtSkills& furtSkills, uint8 scouting)
	: iBaseOwnCnstNode(id, nameKey, msgKey, msgIcn, OCNST_FURTSKILL, scouting), m_furtSkills(furtSkills) {}
	void Serialize(iDynamicBuffer& buff);

protected:
	iFurtSkills		m_furtSkills;
};

//////////////////////////////////////////////////////////////////////////
class iCastle;
class iCastleNode : public iDecorNode
{
public:
	IMPL_TYPEAWARE( iCastleNode );
	iCastleNode(const iStringT& id, CTL_TYPE type, CTL_SIZE size, CTL_ORIENT orient, uint16 cfg) 
	: iDecorNode(id, CASTLE_NAMEKEY[type]), m_Type(type), m_Size(size), m_Orient(orient), m_Cfg(cfg) {}
	virtual ~iCastleNode() {}

	iCastle* CreateObject(const iPoint& pos, PLAYER_ID owner);
	inline CTL_TYPE Type() const { return m_Type; }
	inline CTL_SIZE Size() const { return m_Size; }
	inline CTL_ORIENT Orient() const { return m_Orient; }
	inline uint16 Config() const { return m_Cfg; }

protected:
	CTL_TYPE	m_Type;
	CTL_SIZE	m_Size;
	CTL_ORIENT	m_Orient;
	uint16		m_Cfg;
};

/*
 *	Objects
 */
class iVisCnst : public iBaseMapObject
{
public:
	IMPL_TYPEAWARE( iVisCnst );
	iVisCnst(const iPoint& pos, iBaseVisCnstNode* pProto) 
	: iBaseMapObject(pos), m_pProto(pProto) {}

	inline const iStringT& CustomMessage() const { return m_customMsg; }
	inline iStringT& CustomMessage() { return m_customMsg; }
	inline const iBaseVisCnstNode* Proto() const { return m_pProto; }
	inline const iArmy& Guard() const { return m_guard; }
	inline iArmy& Guard() { return m_guard; }

protected:
	iStringT			m_customMsg;
	iArmy				m_guard;
	iBaseVisCnstNode*	m_pProto;
};
//////////////////////////////////////////////////////////////////////////
class iOwnCnst : public iBaseMapObject
{
public:
	IMPL_TYPEAWARE( iOwnCnst );
	iOwnCnst(const iPoint& pos, PLAYER_ID owner, iBaseOwnCnstNode* pProto) 
	: iBaseMapObject(pos), m_Owner(owner), m_pProto(pProto) {}

	inline const iBaseOwnCnstNode* Proto() const { return m_pProto; }
	inline PLAYER_ID Owner() const { return m_Owner; }
	void SetOwner(PLAYER_ID newOwner) { m_Owner = newOwner; }
	inline const iArmy& Guard() const { return m_guard; }
	inline iArmy& Guard() { return m_guard; }
	
protected:
	iArmy				m_guard;
	PLAYER_ID			m_Owner;
	iBaseOwnCnstNode*	m_pProto;
};
//////////////////////////////////////////////////////////////////////////
class iCastle : public iBaseMapObject
{
public:
	class iCtlCnsts 
	{
	public:
		struct iCnstEntry {
			iCnstEntry(CTL_CONSTR _type, uint32 _state = 0) : type(_type), state(_state) {}
			inline bool IsBuilt() const { return state == 1; }
			CTL_CONSTR	type;
			uint32		state;
		};

	public:
		iCtlCnsts() : m_bCustomized(false) {}
		void Init(iCastle* pCastle);
		void SetState(CTL_CONSTR type, uint32 state);
		inline bool IsCustomized() const { return m_bCustomized; } 
		inline void SetCustomized(bool bCustomized = true) { m_bCustomized = bCustomized; }
		inline uint32 Count() const { return m_cnsts.GetSize(); }
		inline const iCnstEntry& Entry(uint32 idx) const { return m_cnsts[idx]; }
		inline iCnstEntry& Entry(uint32 idx) { return m_cnsts[idx]; }

	private:
		typedef iSimpleArray<iCnstEntry> iCnstList;
		iCnstList		m_cnsts;
		bool			m_bCustomized;
	};
	
public:
	IMPL_TYPEAWARE( iCastle );
	iCastle(const iPoint& pos, PLAYER_ID owner, iCastleNode* pProto) 
	: iBaseMapObject(pos), m_Owner(owner), m_pProto(pProto) { m_cnsts.Init(this); }

	inline const iCastleNode* Proto() const { return m_pProto; }
	inline PLAYER_ID Owner() const { return m_Owner; }
	void SetOwner(PLAYER_ID newOwner) { m_Owner = newOwner; }
	inline const iStringT& CustomName() const { return m_CustomName; }
	inline iStringT& CustomName() { return m_CustomName; }
	inline const iArmy& Garrison() const { return m_garrison; }
	inline iArmy& Garrison() { return m_garrison; }
	inline iCtlCnsts& CtlCnsts() { return m_cnsts; }
	const iStringT& Name() const;

protected:
	iCtlCnsts		m_cnsts;
	iArmy			m_garrison;
	iStringT		m_CustomName;
	PLAYER_ID		m_Owner;
	iCastleNode*	m_pProto;
};


#endif //_HMM_MAPEDITOR_CONSTRUCTIONS_H_