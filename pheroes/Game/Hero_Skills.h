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

#ifndef _HMM_GAME_HERO_VIEW_SKILLS_PAGE_H_
#define _HMM_GAME_HERO_VIEW_SKILLS_PAGE_H_

#include "Dlg_SkillInfo.h"
#include "Dlg_MLInfo.h"

//
class iSkillsTab : public iHeroViewChild, public  IViewCmdHandler
{
private:
	class iSkillsTabBtn : public iButton
	{
	public:
		iSkillsTabBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid)
		: iButton(pViewMgr, pCmdHandler, rect, uid, Visible|Enabled), m_pHero(NULL) {}
		inline void SetHero(const iHero* pHero)	{ m_pHero = pHero; Invalidate(); }
		inline const iHero* Hero() const { return m_pHero; }
	protected:
		const iHero* m_pHero;
	};


	class iSecSkillBtn : public iButton
	{
	public:
		iSecSkillBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid)
		: iButton(pViewMgr, pCmdHandler, rect, uid, Visible|Enabled), m_pSkill(NULL) {}

		inline void SetSkill(const iSecSkillEntry* pSkill) { m_pSkill = pSkill; Invalidate(); }
		inline const iSecSkillEntry* Skill() const { return m_pSkill; }

		void OnCompose()
		{
			iRect rc = GetScrRect();
			ButtonFrame(gApp.Surface(),rc,GetButtonState());
			if (m_pSkill){
				BlitIcon(gApp.Surface(), PDGG_SEC_SKILLS+m_pSkill->m_skill, iRect(rc.x+2,rc.y+3,32,24));
				rc.DeflateRect(35,0,0,0);
				gTextComposer.TextOut(iTextComposer::FontConfig(iTextComposer::FS_MEDIUM,RGB16(255,220,64)), gApp.Surface(),iPoint(rc.x,rc.y+3),gTextMgr[TRID_HSKILL_ESTATES+m_pSkill->m_skill]);
				gTextComposer.TextOut(iTextComposer::FontConfig(iTextComposer::FS_SMALL,RGB16(192,192,192)), gApp.Surface(),iPoint(rc.x,rc.y+13),gTextMgr[TRID_HSKILL_LEVEL_BASIC+m_pSkill->m_level]);
			} else {
				gApp.Surface().FillRect(rc,0,32);
			}
		}

	private:
		const iSecSkillEntry* m_pSkill;
	};

	class iExpBtn : public iSkillsTabBtn
	{
	public:
		iExpBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid) : iSkillsTabBtn(pViewMgr, pCmdHandler, rect, uid) {}

		void OnCompose()
		{
			iRect rc = GetScrRect();
			ButtonFrame(gApp.Surface(),rc,GetButtonState());
			if (m_pHero) {
				iSize siz = gGfxMgr.Dimension(PDGG_DGLYPH_EXP);
				sint32 opx = rc.x + (sint32)(rc.w/2) - (sint32)(siz.w/2) - gGfxMgr.Anchor(PDGG_DGLYPH_EXP).x;
				sint32 opy = rc.y + (sint32)(rc.h/2) - (sint32)(siz.h/2) - gGfxMgr.Anchor(PDGG_DGLYPH_EXP).x - 2;
				gGfxMgr.Blit(PDGG_DGLYPH_EXP, gApp.Surface(), iPoint(opx,opy));
				iTextComposer::FontConfig fc(iTextComposer::FS_MEDIUM, RGB16(255,255,0));
				iRect trc(rc.x+(sint32)(rc.w/2)-20,rc.y+rc.h-12, 40,11);
				DrawRoundRect(gApp.Surface(), trc, RGB16(192,128,32), cColor_Black);
				gTextComposer.TextOut(fc, gApp.Surface(), iPoint(), FormatNumber(m_pHero->Level()), trc, AlignCenter);
			}
		}
	};

	class iManaBtn : public iSkillsTabBtn
	{
	public:
		iManaBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid) : iSkillsTabBtn(pViewMgr, pCmdHandler, rect, uid) {}

		void OnCompose()
		{
			iRect rc = GetScrRect();
			ButtonFrame(gApp.Surface(),rc,GetButtonState());
			if (m_pHero) {
				iSize siz = gGfxMgr.Dimension(PDGG_DGLYPH_MANA);
				sint32 opx = rc.x + (sint32)(rc.w/2) - (sint32)(siz.w/2) - gGfxMgr.Anchor(PDGG_DGLYPH_MANA).x;
				sint32 opy = rc.y + (sint32)(rc.h/2) - (sint32)(siz.h/2) - gGfxMgr.Anchor(PDGG_DGLYPH_MANA).x - 5;
				gGfxMgr.Blit(PDGG_DGLYPH_MANA, gApp.Surface(), iPoint(opx,opy));
				iTextComposer::FontConfig fc(iTextComposer::FS_MEDIUM, RGB16(255,255,0));
				iRect trc(rc.x+(sint32)(rc.w/2)-20,rc.y+rc.h-12, 40,11);
				DrawRoundRect(gApp.Surface(), trc, RGB16(192,128,32), cColor_Black);
				gTextComposer.TextOut(fc, gApp.Surface(), iPoint(), iFormat(_T("%d/%d"),m_pHero->ManaPts(),m_pHero->MaxManaPts()), trc, AlignCenter);
			}
		}
	};

	class iActPtsBtn : public iSkillsTabBtn
	{
	public:
		iActPtsBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid) : iSkillsTabBtn(pViewMgr, pCmdHandler, rect, uid) {}

		void OnCompose()
		{
			iRect rc = GetScrRect();
			ButtonFrame(gApp.Surface(),rc,GetButtonState());
			if (m_pHero) {
				iSize siz = gGfxMgr.Dimension(PDGG_DGLYPH_MOVES);
				sint32 opx = rc.x + (sint32)(rc.w/2) - (sint32)(siz.w/2) - gGfxMgr.Anchor(PDGG_DGLYPH_MOVES).x;
				sint32 opy = rc.y + (sint32)(rc.h/2) - (sint32)(siz.h/2) - gGfxMgr.Anchor(PDGG_DGLYPH_MOVES).y - 1;
				gGfxMgr.Blit(PDGG_DGLYPH_MOVES, gApp.Surface(), iPoint(opx,opy));
				iTextComposer::FontConfig fc(iTextComposer::FS_MEDIUM, RGB16(255,255,0));
				iRect trc(rc.x+(sint32)(rc.w/2)-20,rc.y+rc.h-12, 40,11);
				DrawRoundRect(gApp.Surface(), trc, RGB16(192,128,32), cColor_Black);
				gTextComposer.TextOut(fc, gApp.Surface(), iPoint(), iFormat(_T("%d/%d"),m_pHero->Moves(),m_pHero->TotalMoves()), trc, AlignCenter);
			}
		}
	};
	
	class iMoraleBtn : public iSkillsTabBtn
	{
	public:
		iMoraleBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid) : iSkillsTabBtn(pViewMgr, pCmdHandler, rect, uid){}

		void OnCompose()
		{
			iRect rc = GetScrRect();
			ButtonFrame(gApp.Surface(),rc,GetButtonState());
			if (m_pHero) {
				ComposeMoraleGlyph(gApp.Surface(), rc, (sint8)m_pHero->FurtSkill(FSK_MORALE));
			}
		}
	};
	
	class iLuckBtn : public iSkillsTabBtn
	{
	public:
		iLuckBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid) : iSkillsTabBtn(pViewMgr, pCmdHandler, rect, uid){}

		void OnCompose()
		{
			iRect rc = GetScrRect();
			ButtonFrame(gApp.Surface(),rc,GetButtonState());
			if (m_pHero) {
				ComposeLuckGlyph(gApp.Surface(), rc, (sint8)m_pHero->FurtSkill(FSK_LUCK));
			}
		}
	};

public:
	iArmyListEx*	m_pArmyList;
	iCheckButton*	m_pSplitBtn;
	iSecSkillBtn*	m_pBtnSecSkill[8];

	iSkillsTabBtn*	m_pSkillTabBtns[5];
 
	iSkillsTab(iViewMgr* pViewMgr) : iHeroViewChild(pViewMgr)
	{
		AddChild(m_pSkillTabBtns[0] = new iExpBtn(pViewMgr, this, iRect(0,0,53,36), 120));
		AddChild(m_pSkillTabBtns[1] = new iManaBtn(pViewMgr, this, iRect(53,0,53,36), 121));
		AddChild(m_pSkillTabBtns[2] = new iActPtsBtn(pViewMgr, this, iRect(106,0,53,36), 122));
		AddChild(m_pSkillTabBtns[3] = new iMoraleBtn(pViewMgr, this, iRect(159,0,52,36), 123));
		AddChild(m_pSkillTabBtns[4] = new iLuckBtn(pViewMgr, this, iRect(211,0,52,36), 124));

		iPoint pos(0,37);
		sint32 cellW = 131;
		sint32 cellH = 30;
		for (uint32 xx=0; xx<8; ++xx) {
			AddChild(m_pBtnSecSkill[xx] = new iSecSkillBtn(pViewMgr,this,iRect(pos.x+((xx&1)?cellW+1:0),pos.y+((xx>>1)*cellH),cellW,cellH),110 + xx));
		}

		AddChild(m_pSplitBtn = new iCheckButton(pViewMgr, this, iRect(m_Rect.w-18,m_Rect.h-39,18,39), PDGG_BTN_SPLIT, 102));
		AddChild(m_pArmyList = new iArmyListEx(pViewMgr, this, iPoint(0,m_Rect.h-39), m_pSplitBtn, iSize(34,39), 101));
	}

	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
	{
		uint32 uid = pView->GetUID();
		if (uid == 120) {
			// Experience button
			iIconDlg dlg(m_pMgr, iFormat(_T("%s %d"),gTextMgr[TRID_LEVEL], m_pHero->Level()), iStringT(gTextMgr[TRID_INFO_EXPERIENCE]) + _T("\n\n#FEE8") + iFormat(gTextMgr[TRID_MSG_EXP_TO_NEXT_LEVEL], m_pHero->LevelBound(m_pHero->Level()+1) - m_pHero->Experience()), PDGG_DGLYPH_EXP, m_pHero->Owner()->PlayerId());
			dlg.DoModal();
		} else if (uid == 121) {
			// Mana points button
			iIconDlg dlg(m_pMgr, gTextMgr[TRID_MANA], gTextMgr[TRID_INFO_MANA], PDGG_DGLYPH_MANA, m_pHero->Owner()->PlayerId());
			dlg.DoModal();
		} else if (uid == 122) {
			// Travel points button
			iIconDlg dlg(m_pMgr, gTextMgr[TRID_TRAVELPTS], gTextMgr[TRID_INFO_TRAVELPTS], PDGG_DGLYPH_MOVES, m_pHero->Owner()->PlayerId());
			dlg.DoModal();
		} else if (uid == 123) {
			// Morale button
			iMLInfoDlg dlg(m_pMgr, false, m_pHero->FurtSkill(FSK_MORALE), m_pHero->Owner()->PlayerId());
			dlg.DoModal();
		} else if (uid == 124) {
			// Luck button
			iMLInfoDlg dlg(m_pMgr, true, m_pHero->FurtSkill(FSK_LUCK), m_pHero->Owner()->PlayerId());
			dlg.DoModal();
		} else if (uid >= 110 && uid < 118) {
			if (const iSecSkillEntry* pSkill = m_pBtnSecSkill[uid - 110]->Skill()) {
				iSecSkillInfoDlg dlg(m_pMgr, (SECONDARY_SKILLS)pSkill->m_skill, (SECSKILL_LEVEL)pSkill->m_level, m_pHero->Owner()->PlayerId());
				dlg.DoModal();
			}
		}
	}

	void OnHeroChanged() 
	{
		if (m_pHero) {
			uint32 xx;
			for (xx=0; xx<m_pHero->SecSkills().Count(); ++xx) m_pBtnSecSkill[xx]->SetSkill(&m_pHero->SecSkills().At(xx));
			for (uint32 zz=xx; zz<8; ++zz) m_pBtnSecSkill[zz]->SetSkill(NULL);
			m_pArmyList->SetArmy(&m_pHero->Army(), m_pHero, false);
			for (xx=0; xx<5; ++xx) m_pSkillTabBtns[xx]->SetHero(m_pHero);
		} else for (uint32 zz=0; zz<8; ++zz) m_pBtnSecSkill[zz]->SetSkill(NULL);
		Invalidate();
	}

	void OnCompose()
	{
		iHeroViewChild::OnCompose();
		iRect rc = GetScrRect();
		gApp.Surface().HLine(iPoint(rc.x,rc.y+36), rc.x+rc.w,cColor_Black);
		gApp.Surface().VLine(iPoint(rc.x+131,rc.y+36), rc.y+37+30*4,cColor_Black);
	}
};

#endif //_HMM_GAME_HERO_VIEW_SKILLS_PAGE_H_

