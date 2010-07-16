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

#ifndef _HMM_GAME_BATTLE_FORT_H_
#define _HMM_GAME_BATTLE_FORT_H_

//////////////////////////////////////////////////////////////////////////
typedef iMapT<uint8> iBattleMap;

enum CELL_TYPE {
	CT_OBSTACLE = 0,
	CT_MOAT,
	CT_PASSABLE,
	CT_BRIDGE,
	CT_FRIENDLYC,
	CT_ENEMYC
};

extern const sint32 CATAPULT_PROPS[5][3];

/*
 *	CastleFort
 */
class iBattleGroup;
class iCastleFort
{
public:
	enum ElementType {
		InvalidElement = -1,
		RightTower3,
		RightWall2,
		RightTower2,
		RightWall1,
		RightTower1,
		Bridge,
		MainTower,
		LeftTower1,
		LeftWall1,
		LeftTower2,
		LeftWall2,
		LeftTower3,
		TypeCount
	};

	enum ElementState {
		Normal,
		Special,
		Damaged,
		Destroyed,
		StateCount
	};

	static inline sint32 ElementsCount() { return TypeCount; }
	static const sint32 ElCells[TypeCount][2];
	static const sint32 MoatCells[11][2];

public:
	struct iElement {
		iElement(ElementType _type, ElementState _state, sint32 _hits) : type(_type), state(_state), hits(_hits), bRecDmg(false) {}
		virtual void ProcessPassMap(iBattleMap& passMap, bool bOwn) const {}
		virtual bool Passable() const { return false; }
		virtual void Attack(sint32 damage) { hits -= damage; if (hits > 0) state = Damaged; else state = Destroyed; }
		sint32			hits;
		ElementType		type;
		ElementState	state;
		bool			bRecDmg;
	};

	struct iElement_Wall : public iElement {
		iElement_Wall(ElementType _type, sint32 _hits) : iElement(_type, Normal, _hits) {}
		void ProcessPassMap(iBattleMap& passMap, bool bOwn) const
		{ 
			if (state != Destroyed) {
				passMap.GetAt(ElCells[type][0],ElCells[type][1]) = CT_OBSTACLE; 
			}
		}
		bool Passable() const { return state == Destroyed; }
	};

	struct iElement_Tower : public iElement {
		iElement_Tower(ElementType _type, bool _bTurret, sint32 _hits) : iElement(_type, ((_bTurret)?Special:Normal), _hits), bTurret(_bTurret) {}
		void ProcessPassMap(iBattleMap& passMap, bool bOwn) const
		{ if (type != MainTower) passMap.GetAt(ElCells[type][0],ElCells[type][1]) = CT_OBSTACLE; }
		bool bTurret;
	};

	struct iElement_Bridge : public iElement {
		iElement_Bridge(sint32 _hits) : iElement(Bridge, Normal, _hits) {}
		void ProcessPassMap(iBattleMap& passMap, bool bOwn) const
		{ 
			if (!bOwn && (state == Normal || state == Damaged)) {
				passMap.GetAt(ElCells[type][0]-1,ElCells[type][1]) = CT_OBSTACLE; 
				passMap.GetAt(ElCells[type][0],ElCells[type][1]) = CT_OBSTACLE; 
				passMap.GetAt(ElCells[type][0]+1,ElCells[type][1]) = CT_OBSTACLE; 
			} else {
				passMap.GetAt(ElCells[type][0]-1,ElCells[type][1]) = CT_BRIDGE; 
				passMap.GetAt(ElCells[type][0],ElCells[type][1]) = CT_BRIDGE; 
				passMap.GetAt(ElCells[type][0]+1,ElCells[type][1]) = CT_BRIDGE; 
			}
		}
		bool Passable() const { return state == Destroyed || state == Special; }

		void Enter(iBattleGroup* pActor) 
		{ 
			check(state != Destroyed);
			check(visitors.Find(pActor) == -1);
			visitors.Add(pActor);
			if (state != Special) state = Special;
		}

		void Leave(iBattleGroup* pActor) 
		{ 
			check(state != Destroyed);
			check(visitors.Find(pActor) != -1);
			check(state == Special);
			visitors.Remove(pActor);
			if (!visitors.GetSize()) state = Normal;
		}

		iSimpleArray<iBattleGroup*> visitors;
	};

public:
	iCastleFort(iCastle* pCastle);
	~iCastleFort();

	void ProcessPassMap(iBattleMap& passMap, bool bOwn) const;
	ElementType GetTarget(ElementType target = InvalidElement, sint32 prob = 100) const; 
	bool HasWalls() const;
	bool HasTarget() const;
	bool HasTurret() const;

	inline bool HasMoat() const { return m_bHasMoat; }
	inline const iElement* GetElement(uint32 idx) const { check(idx<TypeCount); return m_pElements[idx];}
	inline iElement* GetElement(uint32 idx) { check(idx<TypeCount); return m_pElements[idx];}

private:
	iElement*	m_pElements[TypeCount];
	bool		m_bHasMoat;
};

#endif // _HMM_GAME_BATTLE_FORT_H_

