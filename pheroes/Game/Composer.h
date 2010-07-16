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

#ifndef _HMM_GAME_OVERLAND_COMPOSER_H_
#define _HMM_GAME_OVERLAND_COMPOSER_H_

/*
 *	
 */
struct ComposeItem {
	inline ComposeItem(SpriteId sid, const iPoint& pos, uint8 shadow, SPRITE_LEVEL level, uint8 a)
	: m_sid(sid), m_pos(pos), m_shadow(shadow), m_level(level), m_alpha(a) {}

	inline bool Solid() const
	{ return m_alpha == 255; }

	SpriteId	m_sid;
	iPoint		m_pos;
	uint8		m_shadow;
	uint8		m_level;
	uint8		m_alpha;
};

inline SpriteId GetHeroSprite(iHero *pHero)
{ return PDGG_HEROES+pHero->Angle()*9 + pHero->Proto()->m_hType*72; }


/*
 *	
 */
class iComposeList : public iSortArray<ComposeItem>
{
public:
	iComposeList() {}

	void AddItem(SpriteId sid, const iPoint& pos, uint8 shadow, SPRITE_LEVEL level, uint32 snum, uint8 a=255)
	{
		check(m_ComposeRect != cInvalidRect);
		iRect srect(pos+gGfxMgr.Anchor(sid),gGfxMgr.Dimension(sid));
		if (iClipper::IsIntersectRect(srect,m_ComposeRect)) {
			Insert(ComposeItem(sid,pos,shadow,level, a),snum);
		}
	}

	void Cleanup()
	{
		m_Array.RemoveAll();
		m_ComposeRect = cInvalidRect;
	}

	void SetComposeRect(const iRect& rect)
	{
		m_ComposeRect = rect;
	}

private:
	iRect				m_ComposeRect;
};

/*
 *	
 */
class iOLComposer: public IAniCB
{
public:
	iOLComposer(const iRect& rc, iView* pOwner);
	~iOLComposer();

	bool Init();
	void Cleanup();

	// Metrics
	inline sint32 MapPixW() const { return gGame.Map().GetWidth() * 40; }
	inline sint32 MapPixH() const { return gGame.Map().GetHeight() * 20; }

	// Anchor
	void SetAnchor(const iPoint& pos);
	inline const iPoint& GetAnchor() const { return m_Anchor; }
	inline iPoint ActAnchor(const iPoint& pos) const { return iPoint(m_Anchor.x-MapPixW()/2-40+pos.x,m_Anchor.y-40+pos.y-15); }
	iPoint GetCenterCell(const iPoint& anchor) const;
	iPoint GetCenterViewAnchor(const iPoint& cell, const iPoint& offset = iPoint());

	iPoint GetCenterCell() const;
	void CenterView(const iPoint& cell, const iPoint& offset = iPoint());
	
	// Animation CB
	void AniDone();
	bool StepAni(fix32 t);
	void SetAniObj(iAniObj* pObj);
	iAniObj* RemoveAniObj();
	inline iAniObj* AniObj() { return m_pAniObj; }
	void AddCellMsg(const iStringT& msg, const iPoint& pos);

	// Target
	inline const iPoint& GetTargetPoint() const { return m_ptTarget; }
	inline SpriteId GetTargetCursor() const { return m_crTarget; }
	inline void SetTarget(const iPoint& ptTarget, SpriteId crTarget) { m_ptTarget = ptTarget; m_crTarget = crTarget; }

	void Invalidate() { m_lastCompAnchor = cInvalidPoint; }

	void Compose();
	static void Compose(iDib& outDib, const iPoint& anchor, bool bPuzzleMap = false, const iPoint& tgtCell = cInvalidPoint);

private:
	iView*			m_pOwner;
	iRect			m_Rect;
	iPoint			m_Anchor;
	iPoint			m_lastCompAnchor;
	iDib			m_dibSurfLayer;
	iDib			m_dibSurveyBuff;
	iComposeList	m_ComposeList;
	bool			m_bSurveyMode;

	iPoint			m_ptTarget;
	SpriteId		m_crTarget;

	iAniObj*		m_pAniObj;
	iEventList		m_cellEvents;
};

#endif //_HMM_GAME_OVERLAND_COMPOSER_H_

