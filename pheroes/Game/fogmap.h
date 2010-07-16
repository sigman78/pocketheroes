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

#ifndef _HMM_GAME_FOG_MAP_H_
#define _HMM_GAME_FOG_MAP_H_

/*
 *	Fog Map
 */
class iFogMap : private iMapT<uint8>
{
public:
	bool InitFogMap(uint32 w, uint32 h, bool bFogMap);
	void ResetFog();
	void DiscoverMap(sint32 x, sint32 y, sint32 radius, bool bHide = false);
	bool GetFogCfg(const iPoint& pos, uint8& dcfg, uint8& fcfg) const;
	bool IsInvisible(const iPoint& pos) const;
	bool IsHidden(const iPoint& pos) const;

	inline bool IsVisBound(const iPoint& pos) const
	{ return IsVisBound(pos.x, pos.y); }

	inline bool IsVisBound(sint32 x, sint32 y) const
	{
		return 
			((IsValidPos(x+16,y+16) && GetAt(x+16,y+16) == 0) || (IsValidPos(x+16+1,y+16) && GetAt(x+16+1,y+16) == 0) ||
			(IsValidPos(x+16,y+16+1) && GetAt(x+16,y+16+1) == 0) || (IsValidPos(x+16+1,y+16+1) && GetAt(x+16+1,y+16+1) == 0));
	}

	inline uint8* GetFogMapPtr() { return m_Buff; }
	inline uint32 GetFogMapDataSize() { return m_Width * m_Height; }

	inline bool IsValidCell(const iPoint& pos) const
	{ return IsValidPos(pos.x+16,pos.y+16); }

private:
	bool	m_bFogMap;
};

#endif //_HMM_GAME_FOG_MAP_H_

