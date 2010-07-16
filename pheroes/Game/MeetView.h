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

#ifndef _HMM_GAME_HEROES_MEET_VIEW_H_
#define _HMM_GAME_HEROES_MEET_VIEW_H_

#include "ArtifactList.h"

/*
 *	Hero Child
 */
class iHeroBigPortBtn;
class iHeroChild : public iView, public IViewCmdHandler
{
public:
	iHeroChild(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, iSimpleArray<iArtDragDropItem*>& competitors, const iPoint& pos);
	void OnCompose();
	void SetHero(iHero* pHero);
	inline void SetFriend(iHeroChild* pFriend) { m_pFriend = pFriend; }
	inline iHero* Hero() { return m_pHero; }
	inline iArmyListEx* ArmyList() { return m_pArmyList; }

private:
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

protected:
	iCheckButton*		m_pSplitBtn;

private:
	iHeroBigPortBtn*	m_pPortrait;
	iArmyListEx*		m_pArmyList;
	iArtBackPackCtrl*	m_pArtBackPackCtrl;
	iHero*				m_pHero;
	iHeroChild*			m_pFriend;
};

/*
 *	MeetToolbar
 */
class iMeetToolBar : public iView
{
public:
	iMeetToolBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect);
	void OnCompose();
};

/*
 *	MeetView
 */
class iMeetView : public iChildGameView
{
public:
	iMeetView();
	~iMeetView();

	bool Process(fix32 t);
	void OnCompose();
	void SetHeroes(iHero* pHero1, iHero* pHero2);

private:
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

private:
	iSimpleArray<iArtDragDropItem*>	m_Competitors;
	iHeroChild*						m_pChilds[2];
	iMeetToolBar*					m_pToolBar;
	IViewCmdHandler*				m_pCmdHandler;
};

#endif //_HMM_GAME_HEROES_MEET_VIEW_H_

