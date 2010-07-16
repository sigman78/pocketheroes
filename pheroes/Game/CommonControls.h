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

#ifndef _HMM_GAME_COMMON_CONTROLS_H_
#define _HMM_GAME_COMMON_CONTROLS_H_

/*
 *	Popup view
 */
class iFramePopupView : public iPopupView
{
public:
	iFramePopupView(iViewMgr* pViewMgr, PLAYER_ID pid);

	// Pure virtuals
	virtual void DoCompose(const iRect& clRect)  =0;
	virtual iSize ClientSize()  =0;

private:
	void OnCompose();
	iSize PopupViewSize();
	iSize PopupViewMinSize();

private:
	PLAYER_ID	m_pid;
};

/*
 *	Common controls
 */

// Label
class iPHLabel : public iBaseCtrl
{
public:
	iPHLabel(iViewMgr* pViewMgr, const iRect& rect, const iStringT& text, Alignment align, iTextComposer::FontConfig fc);
	void SetText(const iStringT& text) { m_text = text; Invalidate(); }
	void OnCompose();

private:
	iStringT	m_text;
	Alignment	m_align;
	iTextComposer::FontConfig	m_fc;
};

// Text Box
class iPHTextBox : public iBaseCtrl
{
public:
	iPHTextBox(iViewMgr* pViewMgr, const iRect& rect, const iStringT& text, iTextComposer::FontConfig fc);
	void SetText(const iStringT& text) { m_text = text; Invalidate(); }
	void OnCompose();

private:
	iStringT	m_text;
	iTextComposer::FontConfig	m_fc;
};

// ToolbarSplitter
class iTBSplitter : public iBaseCtrl
{
public:
	iTBSplitter(iViewMgr* pViewMgr, const iRect& rect);
	void OnCompose();
};

// Text button
class iTextButton : public iButton
{
public:
	iTextButton(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, TextResId textKey, uint32 uid, uint32 state = Visible|Enabled);
	void OnBtnDown() const;
	void OnCompose();
	void SetCaption(TextResId textKey) { m_TextKey = textKey; Invalidate(); }

private:
	TextResId	m_TextKey;
};

// Icon button
class iIconButton : public iButton
{
public:
	iIconButton(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, SpriteId sid, uint32 uid, uint32 state = Visible|Enabled);
	void OnBtnDown() const;
	void OnCompose();

private:
	SpriteId	m_spriteId;
};

// Dialog Icon button
class iDlgIconButton : public iButton
{
public:
	iDlgIconButton(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, SpriteId sid, uint32 uid, uint32 state = Visible|Enabled);
	void OnBtnDown() const;
	void OnCompose();

private:
	SpriteId	m_spriteId;
};

// Icon button
class iCheckButton : public iButton, public IViewCmdHandler
{
public:
	iCheckButton(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, SpriteId sid, uint32 uid, uint32 state = Visible|Enabled, bool bChecked = false);
	void SetChecked(bool bChecked, bool bNotify = true);
	inline bool IsChecked() const { return m_bChecked; }

private:
	void OnCompose();
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

private:
	IViewCmdHandler*	m_pOwnCmdHandler;
	bool				m_bChecked;
	SpriteId			m_spriteId;
};

// Check box
class iCheckBox : public iButton, public IViewCmdHandler
{
public:
	iCheckBox(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, const iStringT& text, uint32 uid, uint32 state = Visible|Enabled, bool bChecked = false);
	void SetChecked(bool bChecked, bool bNotify = true);
	inline bool IsChecked() const { return m_bChecked; }

private:
	void OnCompose();
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

private:
	IViewCmdHandler*	m_pOwnCmdHandler;
	bool				m_bChecked;
	iStringT			m_text;
};

// Scroll bar
class iPHScrollBar : public iScrollBar
{
public:
	iPHScrollBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid, uint32 flags=0);
	void ComposeSBElement(Element el, const iRect& rc, uint32 flags);
};

// Mini slider (used as slider)
class iPHMiniSlider : public iScrollBar
{
public:
	iPHMiniSlider(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid);
	void ComposeSBElement(Element el, const iRect& rc, uint32 flags);
};

/*
 *	Specialized controls
 */

// Tool bar tabbed switch
class iBarTabSwitch : public iTabbedSwitch
{
public:
	iBarTabSwitch(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 tabcnt, uint32 uid);
	void SetTabIcon(SpriteId sid);
	void ComposeTabItem(uint32 idx, uint32 itemState, const iRect& rect);
	
private:
	iSimpleArray<SpriteId>	m_Tabs;
};

// Hero portrait button
class iHeroPortBtn : public iButton 
{
public:
	iHeroPortBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid);
	void OnCompose();

	void SetHero(iHero* pHero);
	inline const iHero* Hero() const { return m_pHero; }
	inline iHero* Hero() { return m_pHero; }

private:
	iHero*	m_pHero;
};

// Castle glyph button
class iCastleGlyphBtn : public iButton 
{
public:
	iCastleGlyphBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid);
	void OnCompose();

	void SetCastle(iCastle* pCastle);
	inline const iCastle* Castle() const { return m_pCastle; }
	inline iCastle* Castle() { return m_pCastle; }

private:
	iCastle*	m_pCastle;
};


// Spell button
class iSpellBtn : public iButton 
{
public:
	iSpellBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid);
	void OnCompose();
	inline void SetSpell(const iBaseSpell* pSpell) { m_pSpell = pSpell; SetEnabled(m_pSpell!=NULL); }
	inline const iBaseSpell* GetSpell() const { return m_pSpell; }

private:
	const iBaseSpell*	m_pSpell;
};

// Popup list box
class iPopupList : public iPopupView, public IViewCmdHandler
{
public:
	typedef void(PopupListHandler)(sint32);
public:
	// c-tor
	iPopupList(iViewMgr* pViewMgr, PopupListHandler* pHandler, const iSize& itemMetrics, sint32 itemsCount, sint32 selItem, sint32 pageSize);

	// iPopupView overrides
	iSize PopupViewSize();
	iSize PopupViewMinSize();
	void OnTrackPopup(const iRect& clRect);
	void OnHidePopup();

	// iView overrides
	void OnMouseDown(const iPoint& pos);
	void OnMouseClick(const iPoint& pos);
	void OnCompose();

	// Scrollbar command handler
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

	// virtuals
	virtual void DrawItem(sint32 itemId, iDib& surf, const iRect& rc, uint32 flags)  =0;

private:
	PopupListHandler*	m_pHandler;
	iPHScrollBar*		m_pScrollBar;
	sint32				m_scrPos;
	sint32				m_sidx;
	sint32				m_itemsCount;
	sint32				m_pageSize;
	sint32				m_selItem;
	iSize				m_itemMetrics;
};

// Hero list popup
class iHeroListPopup : public iPopupList
{
public:
	// c-tor
	iHeroListPopup(iViewMgr* pViewMgr);

	// iPopupList overrides
	void DrawItem(sint32 itemId, iDib& surf, const iRect& rc, uint32 flags);
};


// Castle list popup
class iCastleListPopup : public iPopupList
{
public:
	// c-tor
	iCastleListPopup(iViewMgr* pViewMgr);

	// iPopupList overrides
	void DrawItem(sint32 itemId, iDib& surf, const iRect& rc, uint32 flags);
};

// Game popup menu
class iGamePopupMenu : public iPopupList
{
public:
	// c-tor
	iGamePopupMenu(iViewMgr* pViewMgr);

	// iPopupList overrides
	void DrawItem(sint32 itemId, iDib& surf, const iRect& rc, uint32 flags);
};

#endif //_HMM_GAME_COMMON_CONTROLS_H_

