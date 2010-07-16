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

#ifndef _HMM_EDITOR_COMPOSER_H_
#define _HMM_EDITOR_COMPOSER_H_

class iComposer
{
public:
	iComposer() 
	: m_oldAnchor(cInvalidPoint) 
	, m_bShadow(true), m_bGrid(false), m_bPassMap(false), m_pMap(NULL)
	{}
	
	~iComposer()
	{ Cleanup(); }
	
	bool InitComposer(iMapHandler* pmap);
	void Cleanup();
	bool ResizeComposeDib(const iSize &new_siz);
	void ComposeAvatar(const iAvatar& avt, const iPoint& pos, bool bShadow, PLAYER_ID pid = PID_NEUTRAL);
	void Compose(HDC hdc, const iSize& siz, const iPoint &offset, const iPoint& snode, iBaseNode* pSelItem);

	inline uint32 GetMapPixelWidth()
	{ return m_pMap->GetWidth() * 40; }

	inline uint32 GetMapPixelHeight()
	{ return m_pMap->GetHeight() * 20; }

	inline bool IsShowShadow() const
	{ return m_bShadow; }
	inline bool IsShowGrid() const
	{ return m_bGrid; }
	inline bool IsShowPassMap() const
	{ return m_bPassMap; }

	inline void ShowShadow(bool bVal)
	{ m_bShadow = bVal; m_bNeedRedraw = true; }
	inline void ShowGrid(bool bVal)
	{ m_bGrid = bVal; m_bNeedRedraw = true; }
	inline void ShowPassMap(bool bVal)
	{ m_bPassMap = bVal; m_bNeedRedraw = true; }

private:
	bool	m_bShadow;
	bool	m_bGrid;
	bool	m_bPassMap;


	iMapHandler*	m_pMap;

	iDib			m_dibGrid;
	iDib			m_dibSelCell;
	iDib			m_dibSelNode;
	iDib			m_dibRedCell;
	iDib			m_dibYelCell;


	bool			m_bNeedRedraw;
	iPoint			m_oldAnchor;
	iDib			m_dibSurface;

	iMemoryDC		m_ComposeMemDC;
};


#endif //_HMM_EDITOR_COMPOSER_H_