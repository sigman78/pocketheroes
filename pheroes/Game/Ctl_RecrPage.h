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

#ifndef _HMM_GAME_CASTLE_VIEW_RECRUIT_PAGE_H_
#define _HMM_GAME_CASTLE_VIEW_RECRUIT_PAGE_H_

#include "Dlg_Recruit.h"

/*
 *	RecrButton
 */
class iRecrButton : public iButton
{
public:
	iRecrButton(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid)
	: iButton(pViewMgr, pCmdHandler, rect, uid, Enabled|Visible), m_pDwel(NULL) 
	{
	}

	void SetDwelling(iDwellCtlCnst* pDwel)
	{
		m_pDwel = pDwel;
		Invalidate();
	}

	inline iDwellCtlCnst* Dwelling() { return m_pDwel; }

	void OnCompose()
	{
		iRect rc = GetScrRect();
		if (!m_pDwel) gApp.Surface().Darken25Rect(rc);
		ButtonFrame(gApp.Surface(), rc, GetButtonState());

		iRect icRect(rc.x+1,rc.y+11,40,38);
		if (m_pDwel) {
			// Creature icon
			SpriteId sid = PDGG_MINIMON + m_pDwel->CrType();
			icRect = AlignRect(gGfxMgr.Dimension(sid), icRect, AlignCenter);
			gGfxMgr.Blit(sid,gApp.Surface(),icRect.point()-gGfxMgr.Anchor(sid));

			// Name
			gTextComposer.TextOut(gApp.Surface(), rc.point(), iStringT(_T("#FFF6")) + gTextMgr[m_pDwel->CrType()*3+TRID_CREATURE_PEASANT_F2],iRect(rc.x+1,rc.y+1,102,12),AlignCenter);

			iRect orc(rc.x+41, rc.y+13, rc.w-41, 24);

			/*
			gTextComposer.TextOut(gApp.Surface(), orc.point(), iFormat(_T("#S2%s%d#S1#FCA6 / %d"), ((m_pDwel->CrCount())?_T("#FFF0"):_T("#FCA6")),m_pDwel->CrCount(),m_pDwel->Growth()),orc,AlignCenter);
			orc.y+=24;
			*/

			// Available
			gTextComposer.TextOut(gApp.Surface(), orc.point(), iFormat(_T("#S0#FB95%s: #S1%s%d"), gTextMgr[TRID_AVAILABLE], ((m_pDwel->CrCount())?_T("#FFF0"):_T("#FCA6")),m_pDwel->CrCount()),orc,AlignTop);
			orc.y += 11;
			// Growth
			gTextComposer.TextOut(gApp.Surface(), orc.point(), iFormat(_T("#S0#FB95%s: #S1#FCA6%d"), gTextMgr[TRID_GROWTH], m_pDwel->Growth()),orc,AlignTop);
			orc.y += 13;

			// Cost
			gTextComposer.TextOut(gApp.Surface(), orc.point(), iStringT(_T("#S1#FFED")) + MineralSet2Text(CREAT_DESC[m_pDwel->CrType()].cost),orc,AlignTop);

			// Skills
			/*
			iTextComposer::FontConfig fc(iTextComposer::FS_MEDIUM, RGB16(192,192,128));
			iPoint op = rc.point() + iPoint(41,16);
			gGfxMgr.Blit(PDGG_ICN_PSKILLS,gApp.Surface(),op + iPoint(0,0) - gGfxMgr.Anchor(PDGG_ICN_PSKILLS));
			gTextComposer.TextOut(fc, gApp.Surface(), op + iPoint(0,0) + iPoint(12,-2), iFormat(_T("%d"),CREAT_DESC[m_pDwel->CrType()].attack));
			gGfxMgr.Blit(PDGG_ICN_PSKILLS+1,gApp.Surface(),op + iPoint(0,12) - gGfxMgr.Anchor(PDGG_ICN_PSKILLS+1));
			gTextComposer.TextOut(fc, gApp.Surface(), op + iPoint(0,12) + iPoint(12,-2), iFormat(_T("%d"),CREAT_DESC[m_pDwel->CrType()].defence));
			gGfxMgr.Blit(PDGG_ICN_SPEED,gApp.Surface(),op + iPoint(0,24) - gGfxMgr.Anchor(PDGG_ICN_SPEED));
			gTextComposer.TextOut(fc, gApp.Surface(), op + iPoint(0,24) + iPoint(12,-2), iFormat(_T("%d"),CREAT_DESC[m_pDwel->CrType()].speed));
			gGfxMgr.Blit(PDGG_ICN_DAMAGE,gApp.Surface(),op + iPoint(26,0) - gGfxMgr.Anchor(PDGG_ICN_DAMAGE));
			gTextComposer.TextOut(fc, gApp.Surface(), op + iPoint(26,0) + iPoint(12,-2), iFormat(_T("%d-%d"),CREAT_DESC[m_pDwel->CrType()].damage_min,CREAT_DESC[m_pDwel->CrType()].damage_max));
			gGfxMgr.Blit(PDGG_ICN_LIFES,gApp.Surface(),op + iPoint(26,12) - gGfxMgr.Anchor(PDGG_ICN_LIFES));
			gTextComposer.TextOut(fc, gApp.Surface(), op + iPoint(26,12) + iPoint(12,-2), iFormat(_T("%d"),CREAT_DESC[m_pDwel->CrType()].hits));
			gGfxMgr.Blit(PDGG_ICN_GROWTH,gApp.Surface(),op + iPoint(26,24) - gGfxMgr.Anchor(PDGG_ICN_GROWTH));
			gTextComposer.TextOut(fc, gApp.Surface(), op + iPoint(26,24) + iPoint(12,-2), iFormat(_T("%d"),m_pDwel->Growth()));
			*/
		}
		rc.InflateRect(1);
		gApp.Surface().FrameRect(rc, cColor_Black);
	}

private:
	iDwellCtlCnst*	m_pDwel;
};
//////////////////////////////////////////////////////////////////////////
const sint32 RECR_BTNSX = 106;
const sint32 RECR_BTNSY = 53;
class iRecrSet : public iView, public IViewCmdHandler
{
public:
	iRecrSet(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, uint32 uid)
	: iView(pViewMgr, iRect(0,0,RECR_BTNSX*3+2,RECR_BTNSY*2+1),GENERIC_VIEWPORT,uid,Enabled|Visible), m_pCmdHandler(pCmdHandler), m_pCastle(NULL) 
	{
		for (uint32 yy=0; yy<2; ++yy) 
			for (uint32 xx=0; xx<3; ++xx)
				AddChild(m_pBtns[yy*3+xx] = new iRecrButton(pViewMgr, this, iRect(xx*(RECR_BTNSX+1),yy*(RECR_BTNSY+1),RECR_BTNSX,RECR_BTNSY), 50 + (yy*3+xx)));
	}

	void SetCastle(iCastle* pCastle)
	{
		m_pCastle = pCastle;
		sint32 c=0;
		for (uint32 xx=0; xx<pCastle->Cnsts().Count(); ++xx) {
			if (iDwellCtlCnst* pDwel = DynamicCast<iDwellCtlCnst*>(pCastle->Cnst(xx))) {
				if (pDwel->IsBuilt()) {
					check(c<6);
					m_pBtns[c]->SetEnabled(true);
					m_pBtns[c]->SetDwelling(pDwel);
					c++;
				}
			}
		}

		for (uint32 xx=c; xx<6; ++xx) {
			m_pBtns[xx]->SetEnabled(false);
			m_pBtns[xx]->SetDwelling(NULL);
		}
	}
	
private:
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
	{
		check(m_pCastle);
		check(m_pBtns[pView->GetUID()-50]->Dwelling());
		iDwellCtlCnst* pDwel = m_pBtns[pView->GetUID()-50]->Dwelling();
		iCreatGroup cg(pDwel->CrType(), pDwel->CrCount());
		iDlg_Recruit dlg(m_pMgr, cg, m_pCastle->Garrison(),pDwel->Owner()->Owner());
		if (dlg.DoModal() == DRC_OK) {
			pDwel->CrCount() = cg.Count();
		}
	}

private:
	iCastle*			m_pCastle;
	iRecrButton*		m_pBtns[6];
	IViewCmdHandler*	m_pCmdHandler;
};


/*
 *	Recruit page
 */
class iRecrTab : public iCtlViewChild, public  IViewCmdHandler
{
public:
	iRecrTab(iViewMgr* pViewMgr)
	: iCtlViewChild(pViewMgr), m_pCastle(NULL)
	{
		AddChild(pRecrSet = new iRecrSet(pViewMgr,this, 101));

		AddChild(m_pCastleGlyphBtn = new iCastleGlyphBtn(pViewMgr, this, iRect(0,108,32,20),110));
		AddChild(m_pSplitBtn = new iCheckButton(pViewMgr, this, iRect(0,108+20+1,32,32), PDGG_BTN_SPLIT, 111));
		AddChild(m_pHeroPortBtn = new iHeroPortBtn(pViewMgr, this, iRect(0,162,32,20),112));
		AddChild(pCastleArmyList = new iArmyListEx(pViewMgr, this, iPoint(33,108), m_pSplitBtn, iSize(40,37), 102));
		AddChild(pHeroArmyList = new iArmyListEx(pViewMgr, this, iPoint(33,146), m_pSplitBtn, iSize(40,36), 103));

		pCastleArmyList->AddCompetitor(pHeroArmyList);
		pHeroArmyList->AddCompetitor(pCastleArmyList);
	}

	void OnActivate() 
	{
		SetCastle(m_pCastle);
	}

	void OnCompose()
	{
		iCtlViewChild::OnCompose();
		//iRect rc = GetScrRect();
		//ButtonFrame(gApp.Surface(), iRect(rc.x+2,rc.y+105,rc.w-4,2), 0);
	}

	void SetCastle(iCastle* pCastle)
	{ 
		m_pCastle = pCastle;
		pRecrSet->SetCastle(pCastle);
		m_pCastleGlyphBtn->SetCastle(pCastle);
		if (pCastle) {
			pRecrSet->SetCastle(m_pCastle);
			pCastleArmyList->SetArmy(&m_pCastle->Garrison(), NULL, true);
			m_pHeroPortBtn->SetHero(m_pCastle->Visitor());
			if (pCastle->Visitor()) {
				pHeroArmyList->SetArmy(&m_pCastle->Visitor()->Army(), m_pCastle->Visitor(), false);
			} else {
				pHeroArmyList->SetArmy(NULL, NULL, false);
			}
		} else {
			pRecrSet->SetCastle(NULL);
			pCastleArmyList->SetArmy(NULL, NULL, true);
			pHeroArmyList->SetArmy(NULL, NULL, false);
			m_pHeroPortBtn->SetHero(NULL);
		}
		Invalidate(); 
	}

private:
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
	{
		uint32 uid = pView->GetUID();
		if (uid == 112) {
			check(m_pCastle->Visitor());
			if (gGame.Map().CurHero() != m_pCastle->Visitor()) m_pCastle->Visitor()->Owner()->SetCurHero(m_pCastle->Visitor());
			gGame.ShowView(iChildGameView::CV_HERO);
		}
	}
	
private:
	iCastle*		m_pCastle;
	iArmyListEx*	pCastleArmyList;
	iArmyListEx*	pHeroArmyList;
	iRecrSet*		pRecrSet;

	iCheckButton*		m_pSplitBtn;
	iCastleGlyphBtn*	m_pCastleGlyphBtn;
	iHeroPortBtn*		m_pHeroPortBtn;
};

#endif // _HMM_GAME_CASTLE_VIEW_RECRUIT_PAGE_H_
