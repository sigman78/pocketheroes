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

#ifndef _HMM_GAME_POPUP_DIALOGS_H_
#define _HMM_GAME_POPUP_DIALOGS_H_

//////////////////////////////////////////////////////////////////////////
class iTextPopupDlg : public iFramePopupView
{
public:
	iTextPopupDlg(iViewMgr* pViewMgr, const iStringT& text, PLAYER_ID pid);
	void DoCompose(const iRect& clRect);
	iSize ClientSize();

private:
	iStringT	m_Text;
};

//////////////////////////////////////////////////////////////////////////
class iPopupDlg_Guard : public iFramePopupView
{
public:
	iPopupDlg_Guard(iViewMgr* pViewMgr, iMapGuard* pGuard, VISION_LEVEL vl, iHero* pHero);
	void DoCompose(const iRect& clRect);
	iSize ClientSize();

private:
	iMapGuard*		m_pGuard;
	iStringT		m_text1;
	iStringT		m_text2;
};

//////////////////////////////////////////////////////////////////////////
class iPopupDlg_VisCnst : public iFramePopupView
{
public:
	iPopupDlg_VisCnst(iViewMgr* pViewMgr, iVisCnst* pVisCnst, VISION_LEVEL vl, const iHero* pVisitor);
	void DoCompose(const iRect& clRect);
	iSize ClientSize();

private:
	iVisCnst*		m_pVisCnst;
	VISION_LEVEL	m_vLevel;
	const iHero*	m_pVisitor;
};
//////////////////////////////////////////////////////////////////////////
class iPopupDlg_OwnCnst : public iFramePopupView
{
public:
	iPopupDlg_OwnCnst(iViewMgr* pViewMgr, iOwnCnst* pOwnCnst, VISION_LEVEL vl);
	void DoCompose(const iRect& clRect);
	iSize ClientSize();

private:
	iOwnCnst* m_pOwnCnst;
	VISION_LEVEL	m_vLevel;
};
//////////////////////////////////////////////////////////////////////////
class iPopupDlg_Castle: public iFramePopupView
{
public:
	iPopupDlg_Castle(iViewMgr* pViewMgr, iCastle* pCastle, VISION_LEVEL vl);
	void DoCompose(const iRect& clRect);
	iSize ClientSize();

private:
	iCastle* m_pCastle;
	VISION_LEVEL	m_vLevel;
};
//////////////////////////////////////////////////////////////////////////
class iPopupDlg_Hero : public iFramePopupView
{
public:
	iPopupDlg_Hero(iViewMgr* pViewMgr, iHero* pHero, VISION_LEVEL vl);
	void DoCompose(const iRect& clRect);
	iSize ClientSize();

private:
	iHero*	m_pHero;
	VISION_LEVEL	m_vLevel;
};

#endif //_HMM_GAME_POPUP_DIALOGS_H_

