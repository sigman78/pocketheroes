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

#ifndef _HMM_GAME_CONSTRUCTIONS_H_
#define _HMM_GAME_CONSTRUCTIONS_H_

/*
 *	Common construction type
 */
class iMapCnst : public iIListNode, public iBaseMapObject
{
public:
	IMPL_TYPEAWARE( iMapCnst );
	iMapCnst(const iPoint& pos) : iBaseMapObject(pos, false) {}
	virtual ~iMapCnst() {}
	iHero* Visitor() const;
	virtual bool Activate(iHero* pVisitor, bool bActive) { return true; }
};

/*
 *	Visitables
 */
class iHero;
class iVisCnst : public iMapCnst
{
public:
	typedef iSimpleArray<uint16> iVisitors;

public:
	IMPL_TYPEAWARE( iVisCnst );
	iVisCnst(iVisCnstT* pProto, const iPoint& pos, bool bCustomGUI) : iMapCnst(pos), m_pProto(pProto), m_bCustomGUI(bCustomGUI) {}
	virtual ~iVisCnst() {}

	inline const iVisCnstT* Proto() const { return m_pProto; }
	inline iArmy& Guard() { return m_Guard; }
	inline const iArmy& Guard() const { return m_Guard; }
	inline const iStringT& CustomMessage() const { return m_customMsg; }
	inline iStringT& CustomMessage() { return m_customMsg; }

	virtual void NewDay();
	virtual void NewWeek(const iWeekDesc& weekDesk);
	bool Activate(iHero* pVisitor, bool bActive);
	virtual void OnActivate(iHero* pVisitor, bool bActive);
	virtual bool Visited(const iHero* pVisitor) const;
	virtual void Serialize(iDynamicBuffer& dbuff);
	virtual uint8 Unserialize(iDynamicBuffer& dbuff);
	virtual bool CustomPopupText(const iHero* pVisitor, iStringT& ctext) const { return false; }
	virtual bool ShowVisitedLabel() const { return true; }

	inline iVisitors& Visitors() { return m_Visitors; }

protected:
	bool		m_bCustomGUI;
	iArmy		m_Guard;
	iStringT	m_customMsg;
	iVisitors	m_Visitors;
	iVisCnstT*	m_pProto;
};
//////////////////////////////////////////////////////////////////////////
class iVisCnst_Stable : public iVisCnst
{
public:
	iVisCnst_Stable(iVisCnstT_Stable* pProto, const iPoint& pos) : iVisCnst(pProto, pos, false) {}
	void NewWeek(const iWeekDesc& weekDesk);
	void OnActivate(iHero* pVisitor, bool bActive);
	bool Visited(const iHero* pVisitor) const;
};
//////////////////////////////////////////////////////////////////////////
class iVisCnst_Gazebo : public iVisCnst
{
public:
	iVisCnst_Gazebo(iVisCnstT_Gazebo* pProto, const iPoint& pos) : iVisCnst(pProto, pos, true) {}
	void OnActivate(iHero* pVisitor, bool bActive);
};
//////////////////////////////////////////////////////////////////////////
class iVisCnst_ManaSource : public iVisCnst
{
public:
	iVisCnst_ManaSource(iVisCnstT_ManaSource* pProto, const iPoint& pos) : iVisCnst(pProto, pos, false) {}
	void NewWeek(const iWeekDesc& weekDesk);
	void OnActivate(iHero* pVisitor, bool bActive);
	bool Visited(const iHero* pVisitor) const;
	sint32 ManaPts() const;
};
//////////////////////////////////////////////////////////////////////////
class iVisCnst_MLModifier : public iVisCnst
{
public:
	iVisCnst_MLModifier(iVisCnstT_MLModifier* pProto, const iPoint& pos) : iVisCnst(pProto, pos, true) {}
	void NewWeek(const iWeekDesc& weekDesk);
	void OnActivate(iHero* pVisitor, bool bActive);
	bool Visited(const iHero* pVisitor) const;
};
//////////////////////////////////////////////////////////////////////////
class iVisCnst_PSModifier : public iVisCnst
{
public:
	iVisCnst_PSModifier(iVisCnstT_PSModifier* pProto, const iPoint& pos) : iVisCnst(pProto, pos, true) {}
	void OnActivate(iHero* pVisitor, bool bActive);
};
//////////////////////////////////////////////////////////////////////////
class iVisCnst_Obelisk : public iVisCnst
{
public:
	iVisCnst_Obelisk(iVisCnstT_Obelisk* pProto, const iPoint& pos) : iVisCnst(pProto, pos, false) {}
	void OnActivate(iHero* pVisitor, bool bActive);
	bool Visited(const iHero* pVisitor) const;
};
//////////////////////////////////////////////////////////////////////////
class iVisCnst_Sign : public iVisCnst
{
public:
	iVisCnst_Sign(iVisCnstT_Sign* pProto, const iPoint& pos) : iVisCnst(pProto, pos, false) {}
	void OnActivate(iHero* pVisitor, bool bActive);
};
//////////////////////////////////////////////////////////////////////////
class iVisCnst_Dwelling : public iVisCnst
{
public:
	iVisCnst_Dwelling(iVisCnstT_Dwelling* pProto, const iPoint& pos) : iVisCnst(pProto, pos, true), m_creatType(pProto->CreatType()), m_growth(CREAT_DESC[pProto->CreatType()].growth), m_creatCount(0) {}
	void NewWeek(const iWeekDesc& weekDesk);
	void OnActivate(iHero* pVisitor, bool bActive);
	bool Visited(const iHero* pVisitor) const;
	void Serialize(iDynamicBuffer& dbuff);
	uint8 Unserialize(iDynamicBuffer& dbuff);
	inline CREATURE_TYPE CreatType() const { return m_creatType; }
	inline uint32 CreatCount() const { return m_creatCount; }
	inline bool JoinForFree() const { return CREAT_DESC[m_creatType].level < 3; }

private:
	uint32				m_growth;
	CREATURE_TYPE		m_creatType;
	uint32				m_creatCount;
};
//////////////////////////////////////////////////////////////////////////
class iVisCnst_WitchHut : public iVisCnst
{
public:
	iVisCnst_WitchHut(iVisCnstT_WitchHut* pProto, const iPoint& pos) : iVisCnst(pProto, pos, true), m_skill(SECSK_NONE) {}
	void OnActivate(iHero* pVisitor, bool bActive);
	void Serialize(iDynamicBuffer& dbuff);
	uint8 Unserialize(iDynamicBuffer& dbuff);
	bool CustomPopupText(const iHero* pVisitor, iStringT& ctext) const;

private:
	SECONDARY_SKILLS			m_skill;
	iSimpleArray<PLAYER_ID>		m_visPlayers;
};
//////////////////////////////////////////////////////////////////////////
class iVisCnst_Shrine : public iVisCnst
{
public:
	iVisCnst_Shrine(iVisCnstT_Shrine* pProto, const iPoint& pos) : iVisCnst(pProto, pos, true), m_spell(MSP_INVALID) {}
	void OnActivate(iHero* pVisitor, bool bActive);
	void Serialize(iDynamicBuffer& dbuff);
	uint8 Unserialize(iDynamicBuffer& dbuff);
	bool CustomPopupText(const iHero* pVisitor, iStringT& ctext) const;
	MAGIC_SPELL Spell() const { return m_spell; }

private:
	MAGIC_SPELL m_spell;
	iSimpleArray<PLAYER_ID>		m_visPlayers;
};
//////////////////////////////////////////////////////////////////////////
class iVisCnst_Treasury : public iVisCnst
{
public:
	iVisCnst_Treasury(iVisCnstT_Treasury* pProto, const iPoint& pos) : iVisCnst(pProto, pos, true), m_bVisited(false) {}
	void OnActivate(iHero* pVisitor, bool bActive);
	void Serialize(iDynamicBuffer& dbuff);
	uint8 Unserialize(iDynamicBuffer& dbuff);
	bool Visited(const iHero* pVisitor) const { return m_bVisited; }

private:
	bool		m_bVisited;
	iRewardsCtr	m_rewards;
};
//////////////////////////////////////////////////////////////////////////
class iVisCnst_Teleport : public iVisCnst
{
public:
	iVisCnst_Teleport(iVisCnstT_Teleport* pProto, const iPoint& pos) : iVisCnst(pProto, pos, true), m_tgtIdx(-1) {}
	void OnActivate(iHero* pVisitor, bool bActive);
	sint32 GetTarget() const { return (IsTwoWay() || IsEntrance())?m_tgtIdx:-1; }
	sint32 GetOrigin() const { return (IsTwoWay() || !IsEntrance())?m_tgtIdx:-1; }
	uint8 Index() const;
	bool IsEntrance() const;
	bool IsTwoWay() const;
	void SetTarget(sint32 tgtIdx) { m_tgtIdx = tgtIdx; }
	bool CustomPopupText(const iHero* pVisitor, iStringT& ctext) const;
	bool ShowVisitedLabel() const { return false; }

private:
	sint32	m_tgtIdx;
};
//////////////////////////////////////////////////////////////////////////
class iVisCnst_Keymaster : public iVisCnst
{
public:
	iVisCnst_Keymaster(iVisCnstT_Keymaster* pProto, const iPoint& pos) : iVisCnst(pProto, pos, false) {}
	uint8 GetKeyType() const;
	void OnActivate(iHero* pVisitor, bool bActive);
	bool Visited(const iHero* pVisitor) const;
};

//////////////////////////////////////////////////////////////////////////
class iVisCnst_TreeOfKnowledge : public iVisCnst
{
public:
	iVisCnst_TreeOfKnowledge(iVisCnstT_TreeOfKnowledge* pProto, const iPoint& pos) : iVisCnst(pProto, pos, true) {}
	void OnActivate(iHero* pVisitor, bool bActive);
};

//////////////////////////////////////////////////////////////////////////
class iVisCnst_Windmill : public iVisCnst
{
public:
	iVisCnst_Windmill(iVisCnstT_Windmill* pProto, const iPoint& pos) : iVisCnst(pProto, pos, true), m_bVisited(false) {}
	void NewWeek(const iWeekDesc& weekDesk);
	void OnActivate(iHero* pVisitor, bool bActive);
	bool Visited(const iHero* pVisitor) const;
	void Serialize(iDynamicBuffer& dbuff);
	uint8 Unserialize(iDynamicBuffer& dbuff);

private:
	void UpdateMinerals();

private:
	MINERAL_TYPE	m_mtype;
	sint32			m_mcnt;
	bool			m_bVisited;
};
//////////////////////////////////////////////////////////////////////////
class iVisCnst_WeeklyMin : public iVisCnst
{
public:
	iVisCnst_WeeklyMin(iVisCnstT_WeeklyMin* pProto, const iPoint& pos) : iVisCnst(pProto, pos, true), m_bVisited(false) {}
	void NewWeek(const iWeekDesc& weekDesk);
	void OnActivate(iHero* pVisitor, bool bActive);
	bool Visited(const iHero* pVisitor) const;
	void Serialize(iDynamicBuffer& dbuff);
	uint8 Unserialize(iDynamicBuffer& dbuff);

private:
	bool			m_bVisited;
};

/*
 *	Ownerables
 */
class iOwnCnst : public iMapCnst
{
public:
	IMPL_TYPEAWARE( iOwnCnst );
	iOwnCnst(iOwnCnstT* pProto, const iPoint& pos);
	virtual ~iOwnCnst() {}

	inline const iOwnCnstT* Proto() const { return m_pProto; }
	inline iOwnCnstT* Proto() { return m_pProto; }
	inline PLAYER_ID Owner() const { return m_Owner; }
	inline iArmy& Guard() { return m_Guard; }
	inline const iArmy& Guard() const { return m_Guard; }
	void SetOwner(PLAYER_ID newOwner);

	virtual void OnOwnerChanged(PLAYER_ID newOwner, PLAYER_ID oldOwner);
	virtual void NewDay();
	virtual void NewWeek(const iWeekDesc& weekDesk);
	bool Activate(iHero* pVisitor, bool bActive);
	virtual void OnActivate(iHero* pVisitor, bool bActive);

protected:
	iArmy		m_Guard;
	iOwnCnstT*	m_pProto;
	PLAYER_ID	m_Owner;
	
};

//////////////////////////////////////////////////////////////////////////
class iOwnCnst_FurtSkill : public iOwnCnst
{
public:
	iOwnCnst_FurtSkill(iOwnCnstT_FurtSkill* pProto, const iPoint& pos) : iOwnCnst(pProto, pos) {}
	void OnOwnerChanged(PLAYER_ID newOwner, PLAYER_ID oldOwner);
};

#endif //_HMM_GAME_CONSTRUCTIONS_H_

