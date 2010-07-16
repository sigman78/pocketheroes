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

#ifndef _HMM_GAME_CASTLE_VIEW_H_
#define _HMM_GAME_CASTLE_VIEW_H_

//////////////////////////////////////////////////////////////////////////
class iCtlViewChild : public iView
{
public:
	iCtlViewChild(iViewMgr* pViewMgr);
	void OnCompose();
	virtual void SetCastle(iCastle* pCastle) {}
	virtual void OnActivate() {}
};

//////////////////////////////////////////////////////////////////////////
enum CASTLE_VIEW_TABS {
	CVT_CONSTRUCT = 0,
	CVT_RECRUIT,
	CVT_SPELLS,
	CVT_MARKET,
	CVT_TAVERN,
	CVT_COUNT
};

/*
 *	Castle view
 */
class iCastleToolBar;
class iCastleView : public iChildGameView
{
public:

	iCastleView();
	~iCastleView();

	bool OnKeyDown(sint32 key);
	bool Process(fix32 t);
	void SetCastle(iCastle* pCastle);
	void OnBuildCnst();

private:
	void OnCompose();
	void ShowPage(uint32 nIdx);
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

	void NextTab();
	void PrevTab();

	void UpdateTabs();

private:
	iCastleToolBar*		m_pToolBar;
	iCastle*			m_pCastle;
	sint32				m_curTab;
	iCtlViewChild*		m_pChilds[CVT_COUNT];
};

/*
 *	Castle Titlebar
 */
class iCastleTitleBar : public iView
{
public:
	iCastleTitleBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect);
	void SetCastle(iCastle* pCastle);

private:
	void OnCompose();

private:
	iCastle*	m_pCastle;
};

/*
 *	Castle Toolbar
 */
class iCastleToolBar : public iView
{
public:
	friend class iCastleView;
	iCastleToolBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect);
	void SetCastle(iCastle* pCastle);

private:
	void OnCompose();

public://hack!
	iCastleGlyphBtn*	m_pCastleGlyphBtn;
	iBarTabSwitch*		m_pTabSwitch;
	iCastle*			m_pCastle;
};

#endif //_HMM_GAME_CASTLE_VIEW_H_

