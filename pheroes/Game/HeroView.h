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

#ifndef _HMM_GAME_HERO_VIEW_H_
#define _HMM_GAME_HERO_VIEW_H_

//////////////////////////////////////////////////////////////////////////
class iHeroViewChild : public iView
{
public:
	iHeroViewChild(iViewMgr* pViewMgr);
	void SetHero(iHero* pHero);
	virtual void OnCompose();
	virtual void OnActivate() {}
	virtual void OnHeroChanged() {}

protected:
	iHero*	m_pHero;
};

//////////////////////////////////////////////////////////////////////////
enum HTABSWITCHES  {
	HVT_SKILLS = 0,
	HVT_ARTIFACTS,
	HVT_SPELLBOOK,
//	HVT_QUESTLOG,
	HVT_COUNT
};

/*
 *	Primary Skills view
 */
class iPrSkillBtn : public iButton, public  IViewCmdHandler
{
public:
	iPrSkillBtn(iViewMgr* pViewMgr, const iRect& rect, FURTHER_SKILLS skill);
	void OnCompose();
	void SetHero(iHero* pHero);
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

	iHero* 			m_pHero;
	FURTHER_SKILLS	m_skill;
};
//////////////////////////////////////////////////////////////////////////
class iHeroPrSkillView : public iView, public  IViewCmdHandler
{
public:
	iHeroPrSkillView(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect);
	void OnCompose();
	void SetHero(iHero* pHero);

private:
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

private:
	iPrSkillBtn*	m_pBtns[4];
	iHero*			m_pHero;
};

/*
 *	Hero Titlebar
 */
class iHeroTitleBar : public iView
{
public:
	iHeroTitleBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect);
	void OnCompose();
	void SetHero(iHero* pHero);
private:
	iHero*	m_pHero;
};

/*
 *	Hero Toolbar
 */
class iHeroToolBar : public iView
{
public:
	friend class iHeroView;
	iHeroToolBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect);
	void OnCompose();
	void SetHero(iHero* pHero, bool bReadOnly);

protected:
	iIconButton*	m_pDismissBtn;
	iBarTabSwitch*	m_pTabSwitch;
	iHeroPortBtn*	m_pHeroPortBtn;

private:
	iHero*	m_pHero;
};

/*
 *	Hero view
 */
class iHeroView : public iChildGameView
{
public:
	iHeroView(CHILD_VIEW parentView);
	~iHeroView();

	bool OnKeyDown(sint32 key);
	bool Process(fix32 t);
	void OnCompose();
	void SetHero(iHero* pHero);

private:
	void ShowPage(uint32 nIdx);
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);
	void NextTab();
	void PrevTab();

private:
	iHero*				m_pHero;
	sint32				m_curTab;
	IViewCmdHandler*	m_pCmdHandler;
	iHeroPrSkillView*	m_pPrSkillsView;
	iHeroTitleBar*		m_pTitleBar;
	iHeroToolBar*		m_pToolBar;
	iHeroViewChild*		m_pChilds[HVT_COUNT];
};

#endif //_HMM_GAME_HERO_VIEW_H_

