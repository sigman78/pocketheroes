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

#ifndef _HMM_GAME_PATH_FINDING_H_
#define _HMM_GAME_PATH_FINDING_H_

const uint8 CNST_PASS_MASK = 0x38;
const uint8 CSTL_PASS_MASK = 0x28;
extern const sint16 nOffs[8][2];


/*  
 *	
 */
class iPath
{
public:
	enum Action {
		None = 0,	// Invalid action
		MoveTo,		// Move to specified point
		Attack,		// Attack guard at specified point
		Touch,		// touch item at specified point
		Enter,		// Enter into construction
		Meet		// Hero meets friendly heroe
	};

	struct Target {
		Target() : action(MoveTo), epmask(0xFF) {}
		sint32	action;
		uint32	epmask;
	}; // 4 + 4  = 8 bytes

	struct iStep {
		iStep()
		: m_Cost(0) {}
		iStep(const iPoint& pos, sint32 cost, Action act)
		: m_Pos(pos), m_Cost(cost), m_action(act) {}
		iPoint	m_Pos;
		sint32	m_Cost;
		sint32	m_action;
	}; //  8 + 4 + 4 = 16 bytes

	typedef iSimpleArray<iStep>			iStepArray;

	// c-tor, d-tor
	iPath(const iPoint& pos) { SetEmpty(pos); }
	~iPath() {}

	void SetEmpty(const iPoint& pos)
	{
		m_Start = pos;
		m_Path.RemoveAll();
	}

	inline bool IsEmpty() const
	{ return m_Path.GetSize() == 0; }

	inline void SetTargetAction(Action act)
	{ m_Path.GetLast().m_action = act; }

	inline sint32 TargetAction() const
	{ return m_Path.GetLast().m_action; }

	inline const iPoint& DestPoint() const
	{ return m_Path.GetLast().m_Pos; }

	inline bool IsDestPoint(const iPoint& pos)
	{ return m_Path.GetSize() && m_Path.GetLast().m_Pos == pos; }
	
	inline bool BelongToPath(const iPoint& pos)
	{
		for (uint32 xx=0; xx<m_Path.GetSize(); ++xx) if (m_Path[xx].m_Pos == pos) return true;
		return false;
	}

	inline sint32 PathCost() const
	{
		sint32 result=0;
		for (uint32 xx=0; xx<m_Path.GetSize(); ++xx)  result += m_Path[xx].m_Cost;
		return result;
	}

	inline iStep GetStep() const
	{ check(!IsEmpty()); return m_Path[0]; }

	inline iPoint GetStepPos() const
	{ check(!IsEmpty()); return m_Path[0].m_Pos; }

	inline sint32 GetStepCost() const
	{ check(!IsEmpty()); return m_Path[0].m_Cost; }

	inline fix32 GetStepLength() const
	{
		if (IsEmpty()) return fix32();
		iPoint p = m_Path[0].m_Pos;
		sint32 x_diff = p.x - m_Start.x;
		sint32 y_diff = p.y - m_Start.y;
		if (x_diff == 0 || y_diff ==0) return fix32(0.5f);
		else return fix32(0.75f);
	}

	inline iStep Step()
	{
		check(m_Path.GetSize());
		iStep res = m_Path.PopFirst();
		m_Start = res.m_Pos;
		return res;
	}

	uint8 GetAngle() const
	{
		if (IsEmpty()) return 0;
		iPoint p = m_Path[0].m_Pos;
		// dumb ass lookup		
		sint32 idx = 4 + iSIGN( p.x - m_Start.x )*3 + iSIGN( p.y - m_Start.y );
		check( idx >= 0 );
		static const uint8 dirs[9] = { 0, 7, 6, 1, 0, 5, 2, 3, 4 };
		return dirs[idx];
/*
		sint32 x_diff = p.x - m_Start.x;
		sint32 y_diff = p.y - m_Start.y;
		if (x_diff<0 && y_diff<0) return 0;
		else if (x_diff==0 && y_diff<0) return 1;
		else if (x_diff>0 && y_diff<0) return 2;
		else if (x_diff>0 && y_diff==0) return 3;
		else if (x_diff>0 && y_diff>0) return 4;
		else if (x_diff==0 && y_diff>0) return 5;
		else if (x_diff<0 && y_diff>0) return 6;
		else if (x_diff<0 && y_diff==0) return 7;
		else return 0;
*/
	}

	// used solely by the path finder
	inline void AddMoveToPoint( const iPoint& pt, sint32 cost )
	{
		if (!m_Path.GetSize()) m_Path.Add(iStep( pt, cost, MoveTo ) );
		else m_Path.InsertBefore(0, iStep( pt, cost, MoveTo ) );
	}

#if 1
	void DumpPath() const;
#endif

	iPoint		m_Start;
	iStepArray	m_Path;
};

//////////////////////////////////////////////////////////////////////////

/*
 *	Since the new pathfinder uses statically allocated data
 *	It couldn't be called recursively.
 *	Additionally that statically allocated data could be purged
 *	to free up memory. That could be done in high-level functions
 *	after successive calls to the pathfinder.
 */
class iPathFinder 
{
private:
	// disable copy construction & assignment
	iPathFinder( const iPathFinder& );
	iPathFinder& operator=( const iPathFinder& );

public:

	enum {
		SUnknown	= 0,
		SOpen		= 1,
		SClosed		= 2,
		SImpassable	= 3,
	};

	typedef uint16 PYX;
		
	struct Mark {
		uint8	state:4;// cached cell state /unknown/closed/open/impassable/
		uint8	dir:4;	// stored direction (valid only for closed cells)
		uint8	pass;	// edge passability flags
		uint16	g;
	}; // 4 bytes * mW * mH

	struct Node {
		PYX		yx;		// packed position
		uint16	g;		// sum of g
	}; // 4 bytes * mW * mH

	// NB: 'nodes' and 'heap' data structures use element 0 as a sentinel, it doesnt belong to the actual data

	// heap structure - made static
	typedef uint32 HeapVal;
	//	HeapVal*	heap;	
	//	size_t		lastHeapNode;

	// world map 
	Mark*	world;		// mW * mH

	// tracked nodes - made static
	//	Node*	nodes;		
	//	size_t	maxNodesNum;	

	size_t	mapW,mapH;

	const sint8* passMap;

	typedef iMapT<sint8>		iPassMap;

public:
	iPathFinder(const iPassMap& pmap);
	~iPathFinder();

	// returns path length or -1 if failed
	sint32 FindPath(const iPoint& srcPos, const iPoint& dstPos, iPath& path, uint8 spMask, uint8 epMask);

	// Free up internal static structures
	static void Purge();

private:

	static	size_t		entryCount;
	static	size_t		maxNodesNum;
	static	Node*		nodes;
	static	HeapVal*	heap;
	static 	size_t		lastHeapNode;

	// internal heap functions
	void	HeapRemoveRoot();
	size_t	HeapInsert( HeapVal n );
	size_t	HeapRebuild( size_t index );

	void	SetupEntryExitFlags( uint16 sp, uint16 ep, uint8 exitMask, uint8 entryMask );
	void	PrepareEmptyWorld();
};



#endif //_HMM_GAME_PATH_FINDING_H_

