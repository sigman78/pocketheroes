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

#ifdef _DEEP_DEBUG_
#	define new DEBUG_NEW
#endif 


#include "BattlePF.h"

const sint16 HEX_NEBS[2][2][3][2] = {
	{
		{{1,-1},{1,0},{1,1}},
		{{0,-1},{-1,0},{0,1}}
	},{
		{{0,-1},{1,0},{0,1}},
		{{-1,-1},{-1,0},{-1,1}}
	}
};

//////////////////////////////////////////////////////////////////////////
inline uint32 CellsDelta(sint32 px1, sint32 py1, sint32 px2, sint32 py2) 
{ 
	uint32 dx = iABS(px1 - px2); 
	uint32 dy = iABS(py1 - py2);
	if ( (dx & 1) == 0 ) ++dy;
	return dx+(dy>>1); 
}

//////////////////////////////////////////////////////////////////////////
void CalculatePath(iBattleGroup* pGroup, iPoint& dstPoint, uint8& orient)
{
	check (dstPoint.x >= 0 && dstPoint.x < 13 && dstPoint.y >= 0 && dstPoint.y < 11);
	sint16 px = (sint16)dstPoint.x;
	sint16 py = (sint16)dstPoint.y;
	const iBattleGroup::iDistMap& dm = pGroup->DistMap();
	uint8 cost = dm.GetAt(px,py).dirs[orient].val;
	sint32 speed = pGroup->Speed();

	if (pGroup->TransType() == TRANS_FLY) {
		sint16 tgtX, tgtY;
		uint32 tgtIdx = 0xFFFF;
		ODS(iFormat(_T("--Processing unit at %d,%d:\n"), pGroup->Pos().x, pGroup->Pos().y).CStr());
		// Locate nearest acceptable cell
		iPoint np;
		for (sint16 yy=0; yy<dm.GetHeight(); ++yy) {
			np.y = yy;
			for (sint16 xx=0; xx<dm.GetWidth(); ++xx) {
				np.x = xx;
				for (uint8 dd = 0; dd<2; ++dd) {
					if (pGroup->Pos() != np && dm.GetAt(xx,yy).dirs[dd].val <= pGroup->Speed() && pGroup->CanMove(xx,yy) && (pGroup->PassMap().GetAt(xx,yy) & 0x7F) != CT_MOAT){
						uint32 nidx = CellsDelta(xx, yy, px, py);
						ODS(iFormat(_T("\tCan move at %d,%d with idx %d: "), xx, yy, nidx).CStr());
						if (nidx < tgtIdx) {
							tgtX = xx;
							tgtY = yy;
							orient = dd;
							tgtIdx = nidx;
							ODS(_T("Ok\n"));
						} else {
							ODS(_T("No\n"));
						}
					}
				}
			}
		}
		ODS(iFormat(_T("--Found cell at %d,%d.\n"), tgtX, tgtY).CStr());
		//check(pGroup->Pos().x != tgtX || pGroup->Pos().y != tgtY);
		px = tgtX;
		py = tgtY;
	} else if (pGroup->TransType() == TRANS_WALK) {
		if (pGroup->Size() == 1) {
			while (cost > speed && pGroup->CanMove(px,py)) {
				sint16 sv = py&1;
				uint8 dd = dm.GetAt(px,py).dirs[orient].dir;
				uint8 d = dd >> 1;
				uint8 o = !(dd & 0x1);
				sint16 npx = px + HEX_NEBS[sv][o][d][0];
				sint16 npy = py + HEX_NEBS[sv][o][d][1];
				if (orient == o) orient = !orient;
				px = npx;
				py = npy;
				cost = dm.GetAt(px,py).dirs[o].val;
			}
		} else {
			while (cost > speed && pGroup->CanMove(px,py)) {
				sint16 sv = py&1;
				uint8 dd = dm.GetAt(px,py).dirs[orient].dir;
				uint8 d = dd >> 1;
				uint8 o = !(dd & 0x1);
				sint16 npx = px + HEX_NEBS[sv][o][d][0];
				sint16 npy = py + HEX_NEBS[sv][o][d][1];
				uint8 ncost = dm.GetAt(npx,npy).dirs[!o].val;

				sint16 rx = px + TAIL_OFFSET[!o];
				sint16 ry = py;
				if (dm.IsValidPos(rx,ry)) {
					uint8 rdd = dm.GetAt(rx,ry).dirs[!orient].dir;
					uint8 rd = rdd >> 1;
					uint8 ro = !(rdd & 0x1);
					sint16 rnpx = px + HEX_NEBS[sv][!o][d][0];
					sint16 rnpy = py + HEX_NEBS[sv][!o][d][1];
					if (dm.IsValidPos(rnpx,rnpy) && dm.GetAt(rnpx,rnpy).dirs[o].val < ncost) {
						npx = rnpx;
						npy = rnpy;
						ncost = dm.GetAt(rnpx,rnpy).dirs[o].val;
						orient = !orient;
					}
				}
				px = npx;
				py = npy;
				cost = ncost;
			}
		}
	} else {
		// unknown transportation type
		check(0);
	}
	dstPoint.x = px;
	dstPoint.y = py;

}

/*
 *	Path finding
 */
iBattlePF::iBattlePF(iBattleGroup* pGroup)
: m_pGroup(pGroup)
{ }

static int counter = 0;
static int tcounter = 0;


struct iOpenCell {
	iOpenCell(sint16 _px, sint16 _py, uint8 _dir, uint32 _step)
	: px(_px), py(_py), dir(_dir), step(_step) {}
	sint16 px, py;
	uint32 step;
	uint8 dir;
};

void iBattlePF::PropagateDown(sint16 px, sint16 py, uint8 dir, uint32 step, bool bFull)
{
	iSimpleArray<iOpenCell> openCells;
	uint32 curCell = 0;

	sint32 speed = m_pGroup->Speed();
	bool bFlying = m_pGroup->TransType() == TRANS_FLY;
	
	// Add origin cell
	openCells.Add(iOpenCell(px, py, dir, step));

	while (curCell < openCells.GetSize()) {
		iOpenCell cell = openCells[curCell++];

		// process current cell
		uint8 qdir = (cell.dir & 0x1);
		// 1
		if ( (!bFull && cell.step > speed) ) continue; 
		// 2
		uint8& pmc = m_pGroup->PassMap().GetAt(cell.px,cell.py);
		uint8 tt = (pmc&0x7F);
		// 3
		if ( cell.step && (tt != CT_PASSABLE && tt != CT_BRIDGE && tt != CT_MOAT && !bFlying)) continue;
		// 4
		if ( cell.step && (!bFlying || (tt == CT_PASSABLE || tt == CT_BRIDGE || tt == CT_MOAT)) && (cell.step <= speed || bFull) ) pmc |= 0x80;

		sint16 sv = cell.py&1;
		iBattleGroup::iDistCell& dc = m_pGroup->DistMap().GetAt(cell.px,cell.py);

		// add nebs to open list
		for (uint32 yy=0; yy<2; ++yy) {
			if (dc.dirs[qdir].val > cell.step) {
				dc.dirs[qdir].val = cell.step;
				dc.dirs[qdir].dir = cell.dir;
				uint32 nstep = cell.step+1;
				bool bhpass = (cell.step && tt == CT_MOAT && bFull && !bFlying);
				if (bhpass) nstep = cell.step + 20;
				if (nstep < 0xFF && (!cell.step || tt != CT_MOAT || bFlying || bhpass)) {
					for (uint32 xx=0; xx<3; ++xx) {
						sint16 npx = cell.px+HEX_NEBS[sv][qdir][xx][0];
						sint16 npy = cell.py+HEX_NEBS[sv][qdir][xx][1];
						if (m_pGroup->PassMap().IsValidPos(npx,npy)) openCells.Add(iOpenCell(npx, npy, ((2-xx)<<1) | qdir, nstep));
					}
				}
			}
			qdir = !qdir;
		}
	}

	/*
	iters_cur++;
	uint8 qdir = (dir & 0x1);
	bool bFlying = m_pGroup->TransType() == TRANS_FLY;
	counter++;
	// 1
	if ( (!bFull && step > m_pGroup->Speed()) ) return; 
	// 2
	uint8& pmc = m_pGroup->PassMap().GetAt(px,py);
	uint8 tt = (pmc&0x7F);
	// 3
	if ( step && (tt != CT_PASSABLE && tt != CT_BRIDGE && tt != CT_MOAT && !bFlying)) return;
	// 4
	if (step && (!bFlying || (tt == CT_PASSABLE || tt == CT_BRIDGE || tt == CT_MOAT)) && (step <= m_pGroup->Speed() || bFull)) pmc |= 0x80;

	sint16 sv = py&1;

	iBattleGroup::iDistCell& dc = m_pGroup->DistMap().GetAt(px,py);

	for (uint32 yy=0; yy<2; ++yy) {
		if (dc.dirs[qdir].val > step) {
			dc.dirs[qdir].val = step;
			dc.dirs[qdir].dir = dir;
			uint32 nstep = step+1;
			bool bhpass = (step && tt == CT_MOAT && bFull && !bFlying);
			if (bhpass) nstep = step + 20;
			if (nstep < 0xFF && (!step || tt != CT_MOAT || bFlying || bhpass)) {
				for (uint32 xx=0; xx<3; ++xx) {
					sint16 npx = px+HEX_NEBS[sv][qdir][xx][0];
					sint16 npy = py+HEX_NEBS[sv][qdir][xx][1];
					if (m_pGroup->PassMap().IsValidPos(npx,npy)) PropagateDown(npx,npy, ((2-xx)<<1) | qdir,nstep, bFull);
				}
			}
		}
		qdir = !qdir;
	}*/
}

void iBattlePF::PropagateDown2(sint16 px, sint16 py, uint8 dir, uint32 step, bool bFull)
{
	iSimpleArray<iOpenCell> openCells;
	uint32 curCell = 0;

	sint32 speed = m_pGroup->Speed();
	bool bFlying = m_pGroup->TransType() == TRANS_FLY;
	
	// Add origin cell
	openCells.Add(iOpenCell(px, py, dir, step));

	while (curCell < openCells.GetSize()) {
		iOpenCell cell = openCells[curCell++];
		// process current cell
		uint8 qdir = (cell.dir & 0x1);
		sint16 tpx = cell.px+TAIL_OFFSET[qdir];
		sint16 tpy = cell.py;
		if (!m_pGroup->PassMap().IsValidPos(cell.px,cell.py) || !m_pGroup->PassMap().IsValidPos(tpx,tpy) || (!bFull && cell.step > speed)) continue; 
		// 2
		uint8 c = (m_pGroup->PassMap().GetAt(cell.px,cell.py)&0x7F);
		uint8 tc = (m_pGroup->PassMap().GetAt(tpx,tpy)&0x7F);
		// 3
		if ( cell.step && (( (c != CT_PASSABLE && c != CT_BRIDGE && c != CT_MOAT) || (tc != CT_PASSABLE && tc != CT_BRIDGE && tc != CT_MOAT)) && !bFlying) ) continue;
		// 5
		sint16 sv = cell.py&1;
		// add nebs to open list
		for (uint32 yy=0; yy<2; ++yy) {
			iBattleGroup::iDistCell& dc = m_pGroup->DistMap().GetAt(cell.px,cell.py);
			if (dc.dirs[qdir].val > cell.step) {
				dc.dirs[qdir].val = cell.step;
				dc.dirs[qdir].dir = cell.dir;
				if ((cell.step <= speed || bFull) && (!bFlying || ((c == CT_PASSABLE || c == CT_BRIDGE || c == CT_MOAT) && (tc == CT_PASSABLE || tc == CT_BRIDGE || tc == CT_MOAT)))) m_pGroup->PassMap().GetAt(cell.px,cell.py) |= 0x80;
				if (!cell.step || c != CT_MOAT || bFlying) {
					for (uint32 xx=0; xx<3; ++xx) {
						sint16 npx = cell.px+HEX_NEBS[sv][qdir][xx][0];
						sint16 npy = cell.py+HEX_NEBS[sv][qdir][xx][1];
						if (m_pGroup->PassMap().IsValidPos(npx,npy)) openCells.Add(iOpenCell(npx, npy, ((2-xx)<<1) | qdir, cell.step+1));
					}
				}
			}
			cell.px = tpx;
			qdir = !qdir;
		}
	}

	/*
	iters_cur++;
	// 1
	uint8 qdir = (dir & 0x1);
	sint16 tpx = px+TAIL_OFFSET[qdir];
	sint16 tpy = py;
	bool bFlying = m_pGroup->TransType() == TRANS_FLY;

	if (!m_pGroup->PassMap().IsValidPos(px,py) || !m_pGroup->PassMap().IsValidPos(tpx,tpy) || (!bFull && step > m_pGroup->Speed())) return; 
	// 2
	uint8 c = (m_pGroup->PassMap().GetAt(px,py)&0x7F);
	uint8 tc = (m_pGroup->PassMap().GetAt(tpx,tpy)&0x7F);
	// 3
	if ( step && (( (c != CT_PASSABLE && c != CT_BRIDGE && c != CT_MOAT) || (tc != CT_PASSABLE && tc != CT_BRIDGE && tc != CT_MOAT)) && !bFlying) ) return;

	// 5
	sint16 sv = py&1;

	for (uint32 yy=0; yy<2; ++yy) {
		iBattleGroup::iDistCell& dc = m_pGroup->DistMap().GetAt(px,py);
		if (dc.dirs[qdir].val > step) {
			dc.dirs[qdir].val = step;
			dc.dirs[qdir].dir = dir;
			if ((step <= m_pGroup->Speed() || bFull) && (!bFlying || ((c == CT_PASSABLE || c == CT_BRIDGE || c == CT_MOAT) && (tc == CT_PASSABLE || tc == CT_BRIDGE || tc == CT_MOAT)))) m_pGroup->PassMap().GetAt(px,py) |= 0x80;
			if (!step || c != CT_MOAT || bFlying) {
				for (uint32 xx=0; xx<3; ++xx) {
					sint16 npx = px+HEX_NEBS[sv][qdir][xx][0];
					sint16 npy = py+HEX_NEBS[sv][qdir][xx][1];
					if (m_pGroup->PassMap().IsValidPos(npx,npy)) PropagateDown2(npx,npy, ((2-xx)<<1) | qdir,step+1, bFull);
				}
			}
		}
		px = tpx;
		qdir = !qdir;
	}*/
}

void iBattlePF::MakeDistMap(bool bFull)
{
	// move and melee
	m_pGroup->DistMap().FillMem(iBattleGroup::iDistCell());

	tcounter++;
	if (m_pGroup->Size() == 1) {
		PropagateDown((sint16)m_pGroup->Pos().x,(sint16)m_pGroup->Pos().y,m_pGroup->Orient(), 0, bFull);
	} else {
		PropagateDown2((sint16)m_pGroup->Pos().x,(sint16)m_pGroup->Pos().y,m_pGroup->Orient(), 0, bFull);
	}
}

void iBattlePF::MakePath1(const iPoint& pos, uint8 dir, iBattleActList& actList)
{
	sint16 px = (sint16)pos.x;
	sint16 py = (sint16)pos.y;
	const iBattleGroup::iDistMap& dm = m_pGroup->DistMap();
	uint8 cost = dm.GetAt(px,py).dirs[dir].val;

	while (cost) {
		sint16 sv = py&1;
		uint8 dd = dm.GetAt(px,py).dirs[dir].dir;
		uint8 d = dd >> 1;
		uint8 o = !(dd & 0x1);
		sint16 npx = px + HEX_NEBS[sv][o][d][0];
		sint16 npy = py + HEX_NEBS[sv][o][d][1];

		if (dir == o) {
			actList.PushAction(new iBattleAct_Rotate(m_pGroup));
			dir = !dir;
		}

		actList.PushAction(new iBattleAct_Move(m_pGroup, iPoint(px,py)));
		px = npx;
		py = npy;
		cost = dm.GetAt(px,py).dirs[0].val;
	}
	// Initial (start point) rotate
	if (dir != m_pGroup->Orient()) actList.PushAction(new iBattleAct_Rotate(m_pGroup));
}

void iBattlePF::MakePath2(const iPoint& pos, uint8 dir, bool bMove, iBattleActList& actList)
{
	sint16 px = (sint16)pos.x;
	sint16 py = (sint16)pos.y;
	const iBattleGroup::iDistMap& dm = m_pGroup->DistMap();
	uint8 cost = dm.GetAt(px,py).dirs[dir].val;
	
	if ( cost == 0xFF ) {
		dir = !dir;
		if (bMove) {
			actList.PushAction(new iBattleAct_Rotate(m_pGroup));
			cost = dm.GetAt(px,py).dirs[dir].val;
		}
	}
	check(cost != 0xFF);
	
	while (cost) {
		sint16 sv = py&1;

		uint8 dd = dm.GetAt(px,py).dirs[dir].dir;
		uint8 d = dd >> 1;
		uint8 o = !(dd & 0x1);

		sint16 npx, npy;
		if ( dir == o ){
			npx = px + HEX_NEBS[sv][!o][d][0];
			npy = py + HEX_NEBS[sv][!o][d][1];
			px += TAIL_OFFSET[o];
			dir = !dir;
			actList.PushAction(new iBattleAct_Rotate(m_pGroup));
		} else {
			npx = px + HEX_NEBS[sv][o][d][0];
			npy = py + HEX_NEBS[sv][o][d][1];
		}

		actList.PushAction(new iBattleAct_Move(m_pGroup, iPoint(px,py)));
		px = npx;
		py = npy;
		cost = dm.GetAt(px,py).dirs[!o].val;
	}
	// Initial (start point) rotate
	if (dir != m_pGroup->Orient()) actList.PushAction(new iBattleAct_Rotate(m_pGroup));
}

bool iBattlePF::MakePath(const iPoint& pos, uint8 dir, bool bMove, iBattleActList& actList)
{
	if (m_pGroup->DistMap().GetAt(pos.x,pos.y).data == 0xFFFF) return false;
	if (m_pGroup->Size() == 1) MakePath1(pos, dir, actList);
	else MakePath2(pos, dir, bMove, actList);

	return true;
}

