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

#ifndef _HMM_GAME_OVERLAND_VIEW_H_
#define _HMM_GAME_OVERLAND_VIEW_H_

/*
 *	Overland (Main) View
 */
class iMainToolBar;
class iOpTurnBar;
class iInfoBar;
class iMainView : public iChildGameView
{
public:
	iMainView();
	~iMainView();

	void OnTimer(uint32 tid);
	void OnSizeChanged(const iSize& nsiz);
	void OnMouseDown(const iPoint& pos);
	void OnMouseUp(const iPoint& pos);
	void OnMouseClick(const iPoint& pos);
	void OnMouseTrack(const iPoint& pos);
	bool OnKeyDown(sint32 key);
	bool OnKeyUp(sint32 key);
	void OnCompose();

	//
	void EnableToolBar(bool bEnable);

	// Process
	bool Process(fix32 t);
	void OnActivate(bool bActivate);
	
	// Game notifications
	void OnPlayerChanged(iPlayer* pPlayer, bool bAct);

	// Events
	void AddMsg(const iStringT& text);
	void AddCellMsg(const iStringT& msg, const iPoint& pos);

	iBaseMapObject*	ObjectHitTest(const iPoint& scr_pos);

	inline void SetForceTrack(bool bForceTrack)	{ m_bForceTrack = bForceTrack; if (bForceTrack) m_OldAnchor = m_OLComposer.GetAnchor();} 
	inline bool IsForceTrack() const { return m_bForceTrack; }
	
	void SetSurveyMode(bool bSurveyMode);
	inline void SetForceHelp(bool bForceHelp) { m_bForceHelp = bForceHelp;} 
	inline bool IsForceHelp() const { return m_bForceHelp; }
	inline uint8 GetMouseTrackMask() const { return m_MouseTrackMask; }
	inline uint8 GetKeyTrackMask() const { return m_KeyTrackMask; }
	inline iOLComposer& Composer() { return m_OLComposer; }

	void ShowHelpPopup(const iPoint& pos);
	void UpdateMetrics();

	
	// 
	void OnCastleListChanged();
	void OnHeroListChanged();

private:
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);
	void ComposeEventList();

private:
	iOLComposer		m_OLComposer;
	iEventList		m_EventList;
	iPopupView*		m_pPopupView;
	iInfoBar*		m_pInfoBar;
	iMainToolBar*	m_pToolBar;
	iOpTurnBar*		m_pOpTurnBar;

	bool		m_bForceTrack;
	iPoint		m_TrackAnchor;
	iPoint		m_OldAnchor;

	bool		m_bForceHelp;

	uint8		m_MouseTrackMask;	// 4 bits Up,Down,Left,Right
	uint8		m_KeyTrackMask;

	bool		m_bSurveyMode;
	iDib		m_dibSurveyBuff;
	sint32		m_holdTimer;
	iPoint		m_holdPos;

	
	iRect		rcScrLeft;
	iRect		rcScrRight;
	iRect		rcScrTop;
	iRect		rcScrBottom;
};

/*
 *	Overland view Toolbars
 */
class iHeroTB : public iView, public IViewCmdHandler
{
private:
	class iPortBtn : public iButton {
	public:
		iPortBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid);
		void OnCompose();
	};
	class iMoveGaugeBtn : public iButton {
	public:
		iMoveGaugeBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid);
		void OnCompose();
	};
	class iManaGaugeBtn : public iButton {
	public:
		iManaGaugeBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid);
		void OnCompose();
	};
	
public:
	iHeroTB(iViewMgr* pViewMgr, const iRect& rect);
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);
	void OnCompose();
};
//////////////////////////////////////////////////////////////////////////
class iCnstTB : public iView, public IViewCmdHandler
{
private:
	//
	class iPortBtn : public iButton {
	public:
		iPortBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid);
		void OnCompose();
	};

public:
	iCnstTB(iViewMgr* pViewMgr, const iRect& rect);
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);
	void OnCompose();
};
//////////////////////////////////////////////////////////////////////////
class iMainToolBar : public iView, public IViewCmdHandler
{
public:
	iMainToolBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect);
	void OnCompose();
	inline bool InfoMode() const { return false; }

	void EnableHeroBar(bool bEnable);
	void EnableCnstBar(bool bEnable);

private:
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

private:
	
	iHeroTB*	m_pHeroTB;
	iCnstTB*	m_pCnstTB;
};

/*
 *	Info bar
 */
class iInfoBar : public iView
{
public:
	enum Mode {
		Minerals,
		Army,
		System,
	};

public:
	iInfoBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect);
	void OnCompose();
	void OnMouseDown(const iPoint& pos);

private:
	Mode	m_mode;
};

/*
 *	Opponent turn information bar
 */
class iOpTurnBar : public iView
{
public:
	iOpTurnBar(iViewMgr* pViewMgr, const iRect& rect);
	void OnCompose();
};

#endif //_HMM_GAME_OVERLAND_VIEW_H_

