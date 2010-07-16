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

#include "stdafx.h"
#include "Credits.h"
#include "MenuView.h"
#include "Dlg_ScenList.h"
#include "Dlg_Save.h"
#include "Dlg_ScenProps.h"
#include "Dlg_HallOfFame.h"


/*
 *	Main dialog
 */

const iDib::pixel menuBtnText[15] = {
	RGB16(210,190,115), RGB16(214,192,110), RGB16(216,192,102), RGB16(219,193,96), RGB16(221,193,85),
	RGB16(224,194,76),  RGB16(228,196,67),  RGB16(231,195,59), RGB16(233,196,49), RGB16(236,196,40),
	RGB16(239,198,31), RGB16(242,198,23), RGB16(224,198,16), RGB16(247,199,0), RGB16(248,200,0)
};

class iMainMenuDlg : public iDialog, public  IViewCmdHandler
{
public:
	class iMainMenuBtn : public iButton
	{
	public:
		iMainMenuBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, TextResId textKey, uint32 uid, uint32 state = Visible|Enabled)\
		: iButton(pViewMgr, pCmdHandler, rect, uid, state), m_TextKey(textKey)
		{}

		void OnBtnDown() const
		{
			//gSfxMgr.PlaySound(CSND_BUTTON);
		}

		void OnCompose()
		{
//			gApp.Surface().Darken50Rect(GetScrRect());
			// Compose outer frame
			iRect rect = GetScrRect();
			rect.InflateRect(1);

			iDibFont::ComposeProps props = iDibFont::ComposeProps(iGradient(menuBtnText,15), cColor_Black, iDibFont::DecBorder);
			uint32 state = GetButtonState();
			if ( state & iButton::Disabled ) {
				props = iDibFont::ComposeProps(RGB16(255,160,80), cColor_Black, iDibFont::DecBorder);
			} else if ( state & iButton::Pressed ) {
				props = iDibFont::ComposeProps(RGB16(255,255,255), cColor_Black, iDibFont::DecBorder);
				uint16 cColor_Grey = RGB16(32,32,32);
				gApp.Surface().HLine(iPoint(rect.x+2, rect.y), rect.x+rect.w-3, cColor_Grey);
				gApp.Surface().HLine(iPoint(rect.x+2, rect.y+rect.h-1), rect.x+rect.w-3, cColor_Grey);
				gApp.Surface().VLine(iPoint(rect.x,rect.y+2), rect.y+rect.h-2, cColor_Grey);
				gApp.Surface().VLine(iPoint(rect.x+rect.w-1,rect.y+2), rect.y+rect.h-2, cColor_Grey);
				gApp.Surface().Darken50Rect(GetScrRect());
			}
			iTextComposer::FontConfig fc(iTextComposer::FS_LARGE, props );
			gTextComposer.TextOut(fc, gApp.Surface(), iPoint(), gTextMgr[m_TextKey], GetScrRect(), AlignCenter);
		}

	private:
		TextResId	m_TextKey;
	};
	
public:
	iMainMenuDlg(iViewMgr* pViewMgr) : iDialog(pViewMgr) {}

	enum {
		BTN_DIST = 5
	};

private:
	void OnCreateDlg()
	{
		iRect rc = GetDlgMetrics(); rc.h = DEF_BTN_HEIGHT+2;
		AddChild(new iMainMenuBtn(m_pMgr, this, rc, TRID_MENU_NEWGAME, 100, Visible|Enabled)); rc.y+=DEF_BTN_HEIGHT+BTN_DIST;
		AddChild(new iMainMenuBtn(m_pMgr, this, rc, TRID_MENU_LOADGAME, 101, Visible|Enabled)); rc.y+=DEF_BTN_HEIGHT+BTN_DIST;
		AddChild(new iMainMenuBtn(m_pMgr, this, rc, TRID_MENU_HIGHSCORE, 102, Visible|Enabled)); rc.y+=DEF_BTN_HEIGHT+BTN_DIST;
		AddChild(new iMainMenuBtn(m_pMgr, this, rc, TRID_MENU_CREDITS, 103, Visible|Enabled)); rc.y+=DEF_BTN_HEIGHT+BTN_DIST;
		AddChild(new iMainMenuBtn(m_pMgr, this, rc, TRID_MENU_EXITGAME, 104, Visible|Enabled));
	}

	void OnPlace(iRect& rect)
	{
		rect.y += 40;
	}

	void OnCompose()
	{
	}

	iSize GetDlgMetrics() const
	{ return iSize(150,5*(DEF_BTN_HEIGHT+2) + 12); }

	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param) { EndDialog(pView->GetUID()); }
};

/*
 *	
 */

iMenuView::iMenuView()
: iChildGameView(false, CV_UNDEFINED)
{
	m_crComposer.Init();
}

void iMenuView::Start()
{
	while (1) {
		iMainMenuDlg mdlg(&gApp.ViewMgr());
		sint32 res = mdlg.DoModal();

		if (res == 100) {
			// Start new game
			iScenListDlg sldlg(&gApp.ViewMgr());
			res = sldlg.DoModal();
			if (res == DRC_OK) {
				iMapInfo scenProps = sldlg.SelScen();
				iScenPropsDlg spdlg(&gApp.ViewMgr(), scenProps, false);
				if (spdlg.DoModal() == DRC_OK)	{
					scenProps.ReorderPlayers();
					gGame.StartNewGame(scenProps, true);
					break;
				} 
			} else {
				continue;
			}
		} else if (res == 101) {
			// Load saved game
			iSaveDlg saveDlg(&gApp.ViewMgr(), false);
			res = saveDlg.DoModal();
			if (res == DRC_OK) {
				iMapInfo scenProps = saveDlg.SelScenario();
				iScenPropsDlg spdlg(&gApp.ViewMgr(), scenProps, true);
				if (spdlg.DoModal() == DRC_OK)	{
					scenProps.ReorderPlayers();
					gGame.StartNewGame(scenProps, false);
					break;
				} 
			} else {
				continue;
			}
		} else if (res == 102) {
			iDlg_HallOfFame dlg(&gApp.ViewMgr(), gRootPath + _T("PalmHeroes.hsc"));
			dlg.DoModal();
		} else if (res == 103) {
			StartCredits();
			break;
		} else if (res == 104) {
			// Quit to WM200x
			gGame.Quit();
			break;
		}
	}
}

void iMenuView::StartCredits()
{
	m_crComposer.StartCredits();
}

void iMenuView::StopCredits()
{
	m_crComposer.StopCredits();
	Start();
}

bool iMenuView::Process(fix32 t)
{
	if (m_crComposer.IsCreaditsStarted() && m_crComposer.IsCreaditsEnd()) StopCredits();
	Invalidate();
	return true;
}

void iMenuView::OnCompose()
{
	m_crComposer.Compose(gApp.Surface(),iPoint(0,0));

//	gGfxMgr.Blit(PDGG_LOGO, gApp.Surface(), iPoint(44,2));
//	gGfxMgr.Blit(PDGG_LOGO2, gApp.Surface(), iPoint(174,3));

#if 0
	gTextComposer.TextOut(
			iTextComposer::FontConfig(iTextComposer::FS_MEDIUM, iDibFont::ComposeProps(cColor_White, cColor_Black, iDibFont::DecBorder ) ),
			gApp.Surface(), iPoint(), _T("Ёксклюзивна€ верси€ дл€ читателей журнала Mobi (www.mobi.ru)"), 
			iRect(0,m_Rect.y2()-15,m_Rect.w, 15), AlignCenter);
#endif
}

void iMenuView::OnMouseClick(const iPoint& pos)
{
	StopCredits();
}

void iMenuView::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
}

