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

#ifndef _HMM_GAME_CASTLE_CONSTRUCTIONS_H_
#define _HMM_GAME_CASTLE_CONSTRUCTIONS_H_

/*
 *	
 */
struct iDepItem
{
	iDepItem(const CTL_CONSTR d1, const CTL_CONSTR d2) { dep[0] = d1; dep[1] = d2; }
	iDepItem(const CTL_CONSTR d[2]) { dep[0] = d[0]; dep[1] = d[1]; }

	inline bool Empty() const { return dep[0] == CTLCNST_INVALID && dep[1] == CTLCNST_INVALID; }

	CTL_CONSTR dep[2];
};

/*
 *	Castle constructions
 */
class iCastle;
class iCtlCnst : public TypeAware
{
public:
	IMPL_TYPEAWARE( iCtlCnst );
	enum State {
		Built,
		Available,
		NotEnoughRes,
		Unavailable
	};
	
public:
	iCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst);
	virtual ~iCtlCnst() {}

public:
	static iStringT Name(CTL_CONSTR _cnst);
	iStringT Name() const { return Name(cnst); }
	virtual iStringT Desc() const;

	inline iMineralSet Price() const { return CTLCNSTS_DESC[cnst].price; }
	inline iCastle* Owner() { return pCastle; }
	inline iDepItem Depends() const { return iDepItem(CTLCNSTS_DESC[cnst].depend); }
	inline CTL_CONSTR Type() const { return cnst; }

	void Init();
	void Build();
	inline bool IsBuilt() const { return state == Built; }
	inline State GetState() const { return state; }
	inline void SetState(State nState) { state=nState; }
	inline bool IsInited() const { return bInited; }

	// virtuals
	virtual void Serialize(iDynamicBuffer& buff);
	virtual void Unserialize(iDynamicBuffer& buff);
	virtual void OnInit() { }
	virtual void OnBuild() { }
	virtual void NewDay() {}
	virtual void NewWeek(const iWeekDesc& weekDesk) {}
	virtual void OnVisitorEnter(iHero* pHero) {}
	virtual void OnVisitorLeave(iHero* pHero) {}
	virtual void OnOwnerChanged(PLAYER_ID newOwner, PLAYER_ID oldOwner) {}
	virtual bool Visited(const iHero* pHero) const { return true; }
	virtual bool GetActivityText(iStringT& text) const { return false; }

protected:
	bool		bInited;
	iCastle*	pCastle;
	State		state;
	CTL_CONSTR	cnst;
};
//////////////////////////////////////////////////////////////////////////
class iDwellCtlCnst : public iCtlCnst
{
public:
	IMPL_TYPEAWARE( iDwellCtlCnst );
	iDwellCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst, CREATURE_TYPE _crType);
	iStringT Desc() const;
	void OnBuild();
	void NewWeek(const iWeekDesc& weekDesk);
	void Serialize(iDynamicBuffer& buff);
	void Unserialize(iDynamicBuffer& buff);

	uint32 Growth();
	inline CREATURE_TYPE CrType() const { return crType; }
	inline uint32 CrCount() const { return crCount; }
	inline uint32& CrCount() { return crCount; }

private:
	CREATURE_TYPE	crType;
	uint32			crCount;
};
//////////////////////////////////////////////////////////////////////////
class iTavernCtlCnst : public iCtlCnst
{
public:
	IMPL_TYPEAWARE( iTavernCtlCnst );
	iTavernCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst);
	void OnInit();
	void NewWeek(const iWeekDesc& weekDesk);
	void Serialize(iDynamicBuffer& buff);
	void Unserialize(iDynamicBuffer& buff);
	void OnVisitorEnter(iHero* pHero);
	void OnVisitorLeave(iHero* pHero);

	const iHero* Visitor() const ;
	iHero* RecruitVisitor();
	bool CanRecruit() const;
	inline uint32 CurRumor() const { return m_curRumor; }

private:
	bool	m_bRecruited;
	uint32	m_curRumor;
};
//////////////////////////////////////////////////////////////////////////
class iMagicNodeCtlCnst : public iCtlCnst
{
public:
	IMPL_TYPEAWARE( iMagicNodeCtlCnst );
	iMagicNodeCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst);
	void OnBuild();
	void OnOwnerChanged(PLAYER_ID newOwner, PLAYER_ID oldOwner);
};
//////////////////////////////////////////////////////////////////////////
class iMineCtlCnst : public iCtlCnst
{
public:
	IMPL_TYPEAWARE( iMineCtlCnst );
	iMineCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst, MINERAL_TYPE mt, sint32 income);
	void OnBuild();
	void OnOwnerChanged(PLAYER_ID newOwner, PLAYER_ID oldOwner);

public:
	MINERAL_TYPE	m_mt;
	sint32			m_income;
};
//////////////////////////////////////////////////////////////////////////
class iMGuildCtlCnst : public iCtlCnst
{
public:
	IMPL_TYPEAWARE( iMGuildCtlCnst );
	iMGuildCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst, uint8 _level);
	iStringT Desc() const;
	void OnInit();
	void NewDay();
	void Serialize(iDynamicBuffer& buff);
	void Unserialize(iDynamicBuffer& buff);
	void OnVisitorEnter(iHero* pHero);
	bool Visited(const iHero* pHero) const;

	inline uint32 SpellsCount() const { return spells.GetSize(); }
	inline MAGIC_SPELL Spell(uint32 idx) const { check(idx<spells.GetSize()); return (MAGIC_SPELL)spells[idx]; }
	inline const iSpellList& SpellList() const { return spells; }
	
	friend class iLibraryCtlCnst;

protected:
	void InitSpells();
	void RefreshSpells();

private:
	iSimpleArray<uint16>	visitors;
	iSpellList				spells;
	uint8					level;
};
//////////////////////////////////////////////////////////////////////////
class iPermFSKModCtlCnst : public iCtlCnst
{
public:
	IMPL_TYPEAWARE( iPermFSKModCtlCnst );
	iPermFSKModCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst, FURTHER_SKILLS _skill, sint32 _mod);
	void OnVisitorEnter(iHero* pHero);
	bool Visited(const iHero* pHero) const;
	void Serialize(iDynamicBuffer& buff);
	void Unserialize(iDynamicBuffer& buff);

public:
	FURTHER_SKILLS			skill;
	sint32					mod;
	iSimpleArray<uint16>	visitors;
};
//////////////////////////////////////////////////////////////////////////
class iDwellEncCtlCnst : public iCtlCnst
{
public:
	IMPL_TYPEAWARE( iDwellEncCtlCnst );
	iDwellEncCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst, CTL_CONSTR _dwel, sint32 _mod);

	iStringT Desc() const;
	inline CTL_CONSTR DwellingType() const { return dwel; }
	inline sint32 GrowthMod() const { return mod; }

private:
	CTL_CONSTR	dwel;
	sint32		mod;
};
//////////////////////////////////////////////////////////////////////////
class iObsPostCtlCnst : public iCtlCnst
{
public:
	IMPL_TYPEAWARE( iObsPostCtlCnst );
	iObsPostCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst);
	void OnBuild();
	void OnOwnerChanged(PLAYER_ID newOwner, PLAYER_ID oldOwner);
};
//////////////////////////////////////////////////////////////////////////
class iLibraryCtlCnst : public iCtlCnst
{
public:
	IMPL_TYPEAWARE( iLibraryCtlCnst );
	iLibraryCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst);
	void NewWeek(const iWeekDesc& weekDesk);
};
//////////////////////////////////////////////////////////////////////////
class iManaVtxCtlCnst : public iCtlCnst
{
public:
	IMPL_TYPEAWARE( iManaVtxCtlCnst );
	iManaVtxCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst);
	void NewWeek(const iWeekDesc& weekDesk);
	void Serialize(iDynamicBuffer& buff);
	void Unserialize(iDynamicBuffer& buff);
	void OnVisitorEnter(iHero* pHero);
	bool Visited(const iHero* pHero) const;
private:
	void DoubleManaPoints(iHero* pHero);
private:
	bool	m_bVisited;
};
//////////////////////////////////////////////////////////////////////////
class iTreasuryCtlCnst : public iCtlCnst
{
public:
	IMPL_TYPEAWARE( iTreasuryCtlCnst );
	iTreasuryCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst);
	void NewWeek(const iWeekDesc& weekDesk);
};
//////////////////////////////////////////////////////////////////////////
class iMystPoundCtlCnst : public iCtlCnst
{
public:
	IMPL_TYPEAWARE( iMystPoundCtlCnst );
	iMystPoundCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst);
	void NewWeek(const iWeekDesc& weekDesk);
	void Serialize(iDynamicBuffer& buff);
	void Unserialize(iDynamicBuffer& buff);
	bool GetActivityText(iStringT& text) const;

private:
	MINERAL_TYPE	m_mt;
	sint32			m_cnt;
};
//////////////////////////////////////////////////////////////////////////
class iNecrAmpCtlCnst : public iCtlCnst
{
public:
	IMPL_TYPEAWARE( iNecrAmpCtlCnst );
	iNecrAmpCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst);
	void OnBuild();
	void OnOwnerChanged(PLAYER_ID newOwner, PLAYER_ID oldOwner);
};
//////////////////////////////////////////////////////////////////////////
class iCovOfDarkCtlCnst : public iCtlCnst
{
public:
	IMPL_TYPEAWARE( iCovOfDarkCtlCnst );
	iCovOfDarkCtlCnst(iCastle* _pCastle, CTL_CONSTR _cnst);
	void OnBuild();
	void NewDay();

private:
	void HideCastle();
};

//////////////////////////////////////////////////////////////////////////
class iCtlCnsts
{
public:

	iCtlCnsts(iCastle* pCastle) : m_pCastle(pCastle) {}
	~iCtlCnsts() { Cleanup();}
	void Init();

	iCtlCnst* Add(iCtlCnst* pCnst)
	{ m_items.Add(pCnst); return pCnst; }

	inline sint32 Find(const iCtlCnst* pCnst) { return m_items.Find(pCnst); }

	void NewDay();
	void NewWeek(const iWeekDesc& weekDesk);
	void OnVisitorEnter(iHero* pHero);
	void OnVisitorLeave(iHero* pHero);
	void OnOwnerChanged(PLAYER_ID newOwner, PLAYER_ID oldOwner);
	void Serialize(iDynamicBuffer& buff);
	void Unserialize(iDynamicBuffer& buff);

	void DependsList(const iCtlCnst* pCnst, iSimpleArray<uint16>& depList) const;
	void UpdateState(bool bCanBuild);
	inline uint32 Count() const { return m_items.GetSize(); }
	inline iCtlCnst* operator [] (uint32 idx) { return m_items[idx]; }
	inline const iCtlCnst* operator [] (uint32 idx) const { return m_items[idx]; }
	inline iCtlCnst* At(uint32 idx) { return m_items[idx]; }
	inline const iCtlCnst* At(uint32 idx) const { return m_items[idx]; }
	inline const iCtlCnst* GetCnst(CTL_CONSTR cnst) const
	{ for (uint32 xx=0; xx<m_items.GetSize(); ++xx) if(m_items[xx]->Type() == cnst) return m_items[xx]; return NULL; }
	inline iCtlCnst* GetCnst(CTL_CONSTR cnst)
	{ for (uint32 xx=0; xx<m_items.GetSize(); ++xx) if(m_items[xx]->Type() == cnst) return m_items[xx]; return NULL; }

	const iCastle* Owner() const { return m_pCastle; }

private:
	void Cleanup()
	{
		for (uint32 xx=0; xx<m_items.GetSize(); ++xx) delete m_items[xx];
		m_items.RemoveAll();
	}

	iCastle*				m_pCastle;
	iSimpleArray<iCtlCnst*>	m_items;
};

#endif //_HMM_GAME_CASTLE_CONSTRUCTIONS_H_

