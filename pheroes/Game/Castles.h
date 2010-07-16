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

#ifndef _HMM_GAME_CASTLES_H_
#define _HMM_GAME_CASTLES_H_

/*
 *	Castle
 */
class iCastle : public iMapCnst
{
public:
	IMPL_TYPEAWARE( iCastle );
	iCastle(iCastleT* pProto, const iPoint& pos, const iStringT& cname, bool bCreateHero);

	inline const iCastleT* Proto() const { return m_pProto; }
	inline PLAYER_ID Owner() const { return m_Owner; }
	void SetOwner(PLAYER_ID newOwner);
	inline const iStringT& Name() const { return m_Name; }
	inline iStringT& Name() { return m_Name; }

	void Init();
	void OnSelect();
	void OnVisitorEnter(iHero* pHero);
	void OnVisitorLeave(iHero* pHero);

	void NewDay();
	void NewWeek(const iWeekDesc& weekDesk);

	uint32 GetConstrCaps();

	inline iCtlCnst* FindCnst(CTL_CONSTR cnst) { return m_Cnsts.GetCnst(cnst); }
	inline iCtlCnst* Cnst(uint32 idx) { return m_Cnsts[idx]; }
	inline const iCtlCnsts& Cnsts() { return m_Cnsts; }
	iTavernCtlCnst* GetTavern(bool bOnlyBuilt);
	inline iArmy& Garrison() { return m_Garrison; }
	inline const iArmy& Garrison() const { return m_Garrison; }
	uint32 GarrisonPower(bool bOnlyFort = false) const;
	
	sint32 Scouting() const;
	inline sint32 ScoutMod() const { return m_scoutMod; }
	inline sint32& ScoutMod() { return m_scoutMod; }

	inline bool CanBuild() const { return m_bCanBuild; }
	inline void SetBuilt(bool val)  { m_bCanBuild = val; }
	void Build(iCtlCnst* pCnst);
	void Build(uint32 cnstIdx);
	void UpdateCnsts();

	friend class iGameWorld;

private:
	void GenerateGuards();

protected:
	sint32		m_scoutMod;
	iArmy		m_Garrison;
	iStringT	m_Name;
	iCtlCnsts	m_Cnsts;
	bool		m_bCanBuild;
	iCastleT*	m_pProto;
	PLAYER_ID	m_Owner;
	bool		m_bCreateHero;
};


#endif //_HMM_GAME_CASTLES_H_

