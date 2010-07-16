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

#ifndef _HMM_EDITOR_MAP_HANDLER_H_
#define _HMM_EDITOR_MAP_HANDLER_H_

#include "Commands.h"

/*
 *	
 */
struct iCell {
	iCell()
	: pDecor(NULL)
	, pPathEl(NULL)
	{}

	const iAvatar*	pDecor;
	iPathElNode*	pPathEl;
};

/*
 *	
 */
class iMapHandler : public iMapT<iCell>
{
public:
	typedef iSimpleArray<iHero*>		iHeroList;
	typedef iSimpleArray<iMapItem*>		iMapItemList;
	typedef iSimpleArray<iMapEvent*>	iMapEventList;
	typedef iSimpleArray<iMapGuard*>	iMapGuardList;
	typedef iSimpleArray<iVisCnst*>		iVisCnstList;
	typedef iSimpleArray<iOwnCnst*>		iOwnCnstList;
	typedef iSimpleArray<iCastle*>		iCastleList;

	typedef iMapT<sint8> PassMap;

	struct iSurfCell {
		uint8 lowestLayer;
		uint8 layers[STYPE_COUNT];
	};

	struct iNode {
		iNode(SURF_TYPE _surf = STYPE_WATER, uint8 _alt = 0)
		: surf((uint8)_surf)
		, alt(_alt){}
		uint8		surf;
		uint8		alt;
	};

	class iSubs {
	public:
		virtual void OnMapContentChanged() = NULL;
	};

	iHeroList		m_HeroList;
	iMapItemList	m_MapItemList;
	iMapEventList	m_MapEventList;
	iMapGuardList	m_MapGuardList;
	iVisCnstList	m_VisCnstList;
	iOwnCnstList	m_OwnCnstList;
	iCastleList		m_CastleList;
	iMapT<iNode>	m_Nodes;
	iStringT		m_FileName;
	iStringT		m_MapAuthor;
	iStringT		m_MapVersion;
	iPlayer*		m_pPlayers[PID_COUNT];
	PLAYER_ID		m_CurPlayerId;
	PassMap			m_PassMap;
	MAP_SIZE		m_Siz;
	iTimeEventMgr	m_timeEventMgr;
	iPoint			m_posUltimateArt;
	uint32			m_radUltimateArt;

	CommandManager	m_cmdMgr;
	
	uint32			m_lngMask;
	iTextMgr		m_ctMgr;

	CommandManager&	CmdMgr()
	{ return m_cmdMgr;	}


	iMapHandler();
	~iMapHandler();

	void ClearMap();
	bool InitMap(MAP_SIZE siz);

	void PreparePassMap(iAvatarMgr& avatarMgr, bool bDynObj);
	bool ValidateMap(iStringT& res, bool bWarnings) const;

	void SetNodeSurf(sint32 x, sint32 y, SURF_TYPE _surf)
	{ if (IsValidNode(x,y)) m_Nodes.GetPtr(x,y)->surf = (uint8)_surf; }
	void SetSurf(sint32 x, sint32 y, sint32 brushIdx, SURF_TYPE _surf);

	void AddPathEl(sint32 x, sint32 y, iPathElNode* pPathEl);
	void AddDecoration(sint32 x, sint32 y, iDecorNode* pDecor);
	void RemoveDecoration(const iPoint& pos);

	void AddMapObject(iBaseMapObject* pObj);
	void RemoveMapObject(iBaseMapObject* pObj);
	iHero* AddHero(iHero* pHero);
	void AddObject(iBaseNode* pObjNode, PLAYER_ID owner, const iPoint& pos);
	
	void AddPlayer(PLAYER_ID pid, PLAYER_TYPE_MASK ptm);
	bool CanDeletePlayer(PLAYER_ID pid) const;
	void DeletePlayer(PLAYER_ID pid);

	void ChangeCastleOwner(iCastle* pCastle, PLAYER_ID npid) const;
	void UpdateMainCastle(PLAYER_ID pid) const;

	//
	void Cmd_AddObject( iBaseNode* pObjNode, PLAYER_ID pid, const iPoint& pos );

	void SetCurPlayerId(PLAYER_ID playerId)
	{ m_CurPlayerId = playerId; }
	inline PLAYER_ID GetCurPlayerId() const
	{ return m_CurPlayerId; }
	
	inline uint8 GetPathCfg(const iPoint& pos, iPathElNode* pPathEl)
	{
		uint8 cfg=0;
		if (IsValidPos(pos.x,pos.y-1) && GetCell(pos.x,pos.y-1).pPathEl == pPathEl) cfg |= 1;
		if (IsValidPos(pos.x+1,pos.y) && GetCell(pos.x+1,pos.y).pPathEl == pPathEl) cfg |= 2;
		if (IsValidPos(pos.x,pos.y+1) && GetCell(pos.x,pos.y+1).pPathEl == pPathEl) cfg |= 4;
		if (IsValidPos(pos.x-1,pos.y) && GetCell(pos.x-1,pos.y).pPathEl == pPathEl) cfg |= 8;
		return cfg;
	}

	inline bool GetCellSurf(const iPoint& pos, iSurfCell& cell) const
	{
		uint8 s[4];
		s[0] = m_Nodes.GetCell(pos.x,pos.y).surf;
		s[1] = m_Nodes.GetCell(pos.x+1,pos.y).surf;
		s[2] = m_Nodes.GetCell(pos.x+1,pos.y+1).surf;
		s[3] = m_Nodes.GetCell(pos.x,pos.y+1).surf;
		memset(&cell, 0, sizeof(iSurfCell));


		// calculate underlying surface type
		cell.lowestLayer = iMIN(s[0],iMIN(s[1],iMIN(s[2],s[3])));
		// setup config masks for all type of surfaces
		for (uint32 xx=0; xx<4; ++xx) {
			if (s[xx] != cell.lowestLayer) cell.layers[s[xx]] |= (1 << xx);
		}

		return s[0] == s[1] && s[1] == s[2] && s[2] == s[3];
		/*
		//  1 bit	: solid surface  flag
		//	3 bits	: underlying surface type
		//  28 bits	: 4 bits per surface transition config 4*7=28 bits

		uint8 s[4];
		ccfg = 0;

		s[0] = m_Nodes.GetCell(pos.x,pos.y).surf;
		s[1] = m_Nodes.GetCell(pos.x+1,pos.y).surf;
		s[2] = m_Nodes.GetCell(pos.x+1,pos.y+1).surf;
		s[3] = m_Nodes.GetCell(pos.x,pos.y+1).surf;

		if ( s[0] == s[1] && s[1] == s[2] && s[2] == s[3] ) {
			// setup solid flag and surface type
			ccfg |= (1<<31) | ( s[0] << 28);
			return;
		}

		// calculate underlying surface type
		uint8 uls = iMIN(s[0],iMIN(s[1],iMIN(s[2],s[3])));
		ccfg |= (uls<<28);

		// setup config masks for all surfaces
		for (uint32 xx=0; xx<4; ++xx){
			if (s[xx] != uls) ccfg |= (1 << (s[xx]*4+xx));
		}*/
	}

	inline void GetCellAltitude(sint32 x, sint32 y, uint8& ccfg, uint8& alt) const
	{
		uint8 aa[4] = {
			m_Nodes.GetCell(x,y).alt,
			m_Nodes.GetCell(x+1,y).alt,
			m_Nodes.GetCell(x+1,y+1).alt,
			m_Nodes.GetCell(x,y+1).alt
		};

		alt = iMIN(iMIN(iMIN(aa[0],aa[1]),aa[2]),aa[3]);
		ccfg = ((aa[0]-alt)<<6) | ((aa[1]-alt)<<4) | ((aa[2]-alt)<<2) | (aa[3]-alt);
	}

	iBaseMapObject* GetCellObject(const iPoint& pos);

	inline bool IsValidNode(sint32 x, sint32 y)
	{ return m_Nodes.IsValidPos(x,y); }

	void IncAltitude(sint32 x, sint32 y)
	{ prvSetAltitude(x,y, m_Nodes.GetCell(x,y).alt+1); }

	void DecAltitude(sint32 x, sint32 y)
	{ if (m_Nodes.GetCell(x,y).alt == 0) return;
	  prvSetAltitude(x,y, m_Nodes.GetCell(x,y).alt-1); }

	void SetAltitude(sint32 x, sint32 y, uint8 na)
	{ prvSetAltitude(x,y,na); }

	bool prvSetAltitude(sint32 x, sint32 y, uint8 na);

};

/*
 *	
 */
bool LoadMapFromFile(iMapHandler& map, const iStringT& fname);
bool SaveMapToFile(iMapHandler& map, const iStringT& fname);

#endif //_HMM_EDITOR_MAP_HANDLER_H_