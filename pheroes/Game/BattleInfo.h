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

#ifndef _HMM_GAME_BATTLE_INFO_H_
#define _HMM_GAME_BATTLE_INFO_H_

/*
 *	Battle result
 */
enum BATTLE_RESULT {
	BR_NA,
	BR_ASSAULTER_WIN,
	BR_DEFENDER_WIN
};

enum DEFEAT_CAUSE {
	DC_DEFEAT,
	DC_RETREAT,
	DC_SURRENDER
};

class iBattleMember;
struct iBattleResult {
	iBattleResult() : m_result(BR_NA), m_defCause(DC_DEFEAT), m_pDest(NULL), m_pLoser(NULL), m_pWinner(NULL), m_experience(0) {}
	iBattleResult(BATTLE_RESULT result, DEFEAT_CAUSE defCause, iCastle* pDest = NULL, uint32 cost = 0) : m_result(result), m_defCause(defCause), m_pDest(pDest), m_cost(cost), m_pLoser(NULL), m_pWinner(NULL), m_experience(0) {}

	BATTLE_RESULT	m_result;
	DEFEAT_CAUSE	m_defCause;
	iCastle*		m_pDest;
	uint32			m_cost;
	iBattleMember*	m_pLoser;
	iBattleMember*	m_pWinner;
	sint32			m_winCas;
	sint32			m_losCas;
	sint32			m_experience;
};

/*
 *	Battle Side
 */
class iBattleMember : public TypeAware
{
public:
	enum Side {
		Assaulter = 0,
		Defender
	};

	enum Type {
		Undefined = 0,
		Hero,
		Castle,
		OwnCnst,
		VisCnst,
		MapItem,
		MapEvent,
		MapGuard
	};

public:
	IMPL_TYPEAWARE( iBattleMember );
	// c-tor
	iBattleMember(Type type, Side side, const iFurtSkills& furtSkills);
	virtual ~iBattleMember() {}

	// inlines
	inline Side GetSide() const { return m_Side; }
	inline Type MemberType() const { return m_Type; }
	inline const iFurtSkills& FurtSkills() const { return m_furtSkills; }
	inline sint32 GetExperience() const { return m_Experience; }
	inline void AddExperience(sint32 exp) { m_Experience += exp; }
	inline bool CanCastSpell() { return (SpellCaster() && !m_bCastFlag); }
	inline void SetCastFlag(bool bCastFlag = true) { m_bCastFlag = bCastFlag; }

	// virtuals
	virtual inline PLAYER_ID Owner() const { return PID_NEUTRAL; }
	virtual void OnPrepare(iBattleMember* pEnemy) {};
	virtual void OnWin(iBattleResult br) { }
	virtual void OnLose(iBattleResult br) { }

	// pure virtuals
	virtual const iArmy& Army() const  =0;
	virtual iArmy& Army()  =0;
	virtual const iBaseMapObject* BaseMapObject() const  =0;
	virtual iHero* SpellCaster()  =0;

protected:
	bool		m_bCastFlag;
	sint32		m_Experience;
	Type		m_Type;
	Side		m_Side;
	iFurtSkills	m_furtSkills;
};
//////////////////////////////////////////////////////////////////////////
class iBattleMember_Hero : public iBattleMember
{
public:
	IMPL_TYPEAWARE( iBattleMember_Hero );
	iBattleMember_Hero(iHero* pHero, Side side);

	void OnWin(iBattleResult br);
	void OnLose(iBattleResult br);
	inline PLAYER_ID Owner() const { return m_pHero->Owner()->PlayerId(); }
	inline const iArmy& Army() const { return m_pHero->Army(); }
	inline iArmy& Army() { return m_pHero->Army(); }
	inline const iHero* GetHero() const { return m_pHero; }
	inline iHero* GetHero() { return m_pHero; }
	inline const iBaseMapObject* BaseMapObject() const { return m_pHero; }
	inline iHero* SpellCaster() { return m_pHero; }

private:
	iHero*	m_pHero;
};
//////////////////////////////////////////////////////////////////////////
class iBattleMember_Castle : public iBattleMember
{
public:
	IMPL_TYPEAWARE( iBattleMember_Castle );
	iBattleMember_Castle(iCastle* pCastle);

	void OnPrepare(iBattleMember* pEnemy);
	void OnWin(iBattleResult br);
	void OnLose(iBattleResult br);
	inline PLAYER_ID Owner() const { return m_pCastle->Owner(); }
	inline const iArmy& Army() const { return (GetVisitor())?GetVisitor()->Army():m_pCastle->Garrison(); }
	inline iArmy& Army() { return (GetVisitor())?GetVisitor()->Army():m_pCastle->Garrison(); }
	inline const iCastle* GetCastle() const { return m_pCastle; }
	inline iCastle* GetCastle() { return m_pCastle; }
	inline const iHero* GetVisitor() const { return m_pCastle->Visitor(); }
	inline iHero* GetVisitor() { return m_pCastle->Visitor(); }
	inline const iBaseMapObject* BaseMapObject() const { return m_pCastle; }
	inline iHero* SpellCaster() { return m_pCastle->Visitor(); }

private:
	iCastle*	m_pCastle;
};
//////////////////////////////////////////////////////////////////////////
class iBattleMember_OwnCnst : public iBattleMember
{
public:
	IMPL_TYPEAWARE( iBattleMember_OwnCnst );
	iBattleMember_OwnCnst(iOwnCnst* pOwnCnst);

	void OnLose(iBattleResult br);
	inline PLAYER_ID Owner() const { return m_pOwnCnst->Owner(); }
	inline const iArmy& Army() const { return m_pOwnCnst->Guard(); }
	inline iArmy& Army() { return m_pOwnCnst->Guard(); }
	inline const iOwnCnst* GetOwnCnst() const { return m_pOwnCnst; }
	inline iOwnCnst* GetOwnCnst() { return m_pOwnCnst; }
	inline const iBaseMapObject* BaseMapObject() const { return m_pOwnCnst; }
	inline iHero* SpellCaster() { return NULL; }

private:
	iOwnCnst*	m_pOwnCnst;
};
//////////////////////////////////////////////////////////////////////////
class iBattleMember_VisCnst : public iBattleMember
{
public:
	IMPL_TYPEAWARE( iBattleMember_VisCnst );
	iBattleMember_VisCnst(iVisCnst* pVisCnst);

	void OnLose(iBattleResult br);
	inline const iArmy& Army() const { return m_pVisCnst->Guard(); }
	inline iArmy& Army() { return m_pVisCnst->Guard(); }
	inline const iVisCnst* GetVisCnst() const { return m_pVisCnst; }
	inline iVisCnst* GetVisCnst() { return m_pVisCnst; }
	inline const iBaseMapObject* BaseMapObject() const { return m_pVisCnst; }
	inline iHero* SpellCaster() { return NULL; }

private:
	iVisCnst*	m_pVisCnst;
};
//////////////////////////////////////////////////////////////////////////
class iBattleMember_MapItem : public iBattleMember
{
public:
	IMPL_TYPEAWARE( iBattleMember_MapItem );
	iBattleMember_MapItem(iMapItem* pMapItem);

	void OnLose(iBattleResult br);
	inline const iArmy& Army() const { return m_pMapItem->Guard(); }
	inline iArmy& Army() { return m_pMapItem->Guard(); }
	inline const iMapItem* GetMapItem() const { return m_pMapItem; }
	inline iMapItem* GetMapItem() { return m_pMapItem; }
	inline const iBaseMapObject* BaseMapObject() const { return m_pMapItem; }
	inline iHero* SpellCaster() { return NULL; }

private:
	iMapItem*	m_pMapItem;
};
//////////////////////////////////////////////////////////////////////////
class iBattleMember_MapEvent : public iBattleMember
{
public:
	IMPL_TYPEAWARE( iBattleMember_MapEvent );
	iBattleMember_MapEvent(iMapEvent* pMapEvent);

	void OnLose(iBattleResult br);
	inline const iArmy& Army() const { return m_pMapEvent->Guard(); }
	inline iArmy& Army() { return m_pMapEvent->Guard(); }
	inline const iMapEvent* GetMapEvent() const { return m_pMapEvent; }
	inline iMapEvent* GetMapEvent() { return m_pMapEvent; }
	inline const iBaseMapObject* BaseMapObject() const { return m_pMapEvent; }
	inline iHero* SpellCaster() { return NULL; }

private:
	iMapEvent*	m_pMapEvent;
};
//////////////////////////////////////////////////////////////////////////
class iBattleMember_MapGuard : public iBattleMember
{
public:
	IMPL_TYPEAWARE( iBattleMember_MapGuard );
	iBattleMember_MapGuard(iMapGuard* pMapGuard);

	void OnPrepare(iBattleMember* pEnemy);
	void OnWin(iBattleResult br);
	void OnLose(iBattleResult br);
	inline const iArmy& Army() const { return m_Army; }
	inline iArmy& Army() { return m_Army; }
	inline const iMapGuard* GetMapGuard() const { return m_pMapGuard; }
	inline iMapGuard* GetMapGuard() { return m_pMapGuard; }
	inline const iBaseMapObject* BaseMapObject() const { return m_pMapGuard; }
	inline iHero* SpellCaster() { return NULL; }

private:
	iArmy		m_Army;
	iMapGuard*	m_pMapGuard;
};

/*
 *	Battle info
 */
struct iBattleInfo {
	// c-tors
	iBattleInfo();
	iBattleInfo(iHero* pAssaulter, iHero* pDefender);
	iBattleInfo(iHero* pAssaulter, iCastle* pDefender);
	iBattleInfo(iHero* pAssaulter, iOwnCnst* pDefender);
	iBattleInfo(iHero* pAssaulter, iVisCnst* pDefender);
	iBattleInfo(iHero* pAssaulter, iMapItem* pDefender);
	iBattleInfo(iHero* pAssaulter, iMapEvent* pDefender);
	iBattleInfo(iHero* pAssaulter, iMapGuard* pDefender);

	// Cleanup
	void Cleanup();

	//
	const iBattleMember* Member(iBattleMember::Side side) const { return (side==iBattleMember::Assaulter)?m_pAssaulter:m_pDefender; }
	iBattleMember* Member(iBattleMember::Side side) { return (side==iBattleMember::Assaulter)?m_pAssaulter:m_pDefender; }
	iBattleMember::Side InteractorSide() const;
	inline PLAYER_ID InteractorPlayer() const
	{ iBattleMember::Side is = InteractorSide(); return (is==iBattleMember::Assaulter)?m_pAssaulter->Owner():m_pDefender->Owner(); }

	inline const iBaseMapObject* Defender() const { return m_pDefender->BaseMapObject(); }

	iHero* HasArtInBattle(ART_TYPE artt) const;

	//
	iBattleMember* m_pAssaulter;
	iBattleMember* m_pDefender;
};

#endif //_HMM_GAME_BATTLE_INFO_H_

