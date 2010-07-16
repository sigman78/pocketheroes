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

#ifndef _HMM_GAME_CASTLE_VIEW_TAVERN_PAGE_H_
#define _HMM_GAME_CASTLE_VIEW_TAVERN_PAGE_H_

//////////////////////////////////////////////////////////////////////////
enum THGUILD_STATS {
	TGS_NUMTOWNS = 0,
	TGS_NUMHEROES,
	TGS_RESOURCES,
	TGS_OBELISKS,
	TGS_ARTIFACTS,
	TGS_ARMYSTRENGTH,
	TGS_INCOME,
	TGS_COUNT
};

struct ThGuildStat {
	void AddEntry(THGUILD_STATS stat, PLAYER_ID pid, sint32 val)
	{
		entries[stat].Insert(pid, -val);
	}

	void Reset()
	{
		for (uint32 nn=0; nn<TGS_COUNT; ++nn) entries[nn].Cleanup();
	}

	iSortArray<uint32>	entries[TGS_COUNT];
};

const sint32 THFLAG_WIDTH[PID_COUNT] = { 7, 10, 13, 16, 19, 22 };

//////////////////////////////////////////////////////////////////////////
class iThievesGuildView : public iView
{
public:
	iThievesGuildView(iViewMgr* pViewMgr, const iRect& rect, uint32 uid)
	: iView(pViewMgr, rect, GENERIC_VIEWPORT, uid, Visible|Enabled), m_pCastle(NULL) {}

	void SetCastle(iCastle* pCastle)
	{
		m_pCastle = pCastle;
		// Prepare statistics
		m_stat.Reset();
		for (iGameWorld::iPLIt plit = gGame.Map().m_Players.First(); plit != gGame.Map().m_Players.End(); ++plit) {
			iPlayer* pPlayer = *plit;
			PLAYER_ID pid = pPlayer->PlayerId();

			// Number of Towns
			m_stat.AddEntry(TGS_NUMTOWNS, pid, pPlayer->CastleCount());
			// Number of Heroes
			m_stat.AddEntry(TGS_NUMHEROES, pid, pPlayer->HeroCount());
			// Resources
			{
				sint32 val = 0;
				for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) val += pPlayer->Mineral((MINERAL_TYPE)xx)/ MINERALS_DIVIDER[xx];
				m_stat.AddEntry(TGS_RESOURCES, pid, val);
			}
			// Obelisks Found
			m_stat.AddEntry(TGS_OBELISKS, pid, pPlayer->OpenedPuzzles());
			// Artifacts
			{
				sint32 val = 0;
				for (iPlayer::iCHLIt hit = pPlayer->HeroFirst(); hit != pPlayer->HeroEnd(); ++hit) val += (*hit)->Artifacts().TotalCount();
				m_stat.AddEntry(TGS_ARTIFACTS, pid, val);
			}
			// Army Strength
			{
				sint32 val = 0;
				for (iPlayer::iCHLIt hit = pPlayer->HeroFirst(); hit != pPlayer->HeroEnd(); ++hit) val += (*hit)->Army().ArmyPower();
				for (iPlayer::iCCtLIt cit = pPlayer->CastleFirst(); cit != pPlayer->CastleEnd(); ++cit) val += (*cit)->Garrison().ArmyPower();
				m_stat.AddEntry(TGS_ARMYSTRENGTH, pid, val);
			}
			// Income
			{
				sint32 val = 0;
				val += pPlayer->FurtSkills().Value(FSK_MIN_GOLD);
				for (iPlayer::iCHLIt hit = pPlayer->HeroFirst(); hit != pPlayer->HeroEnd(); ++hit) val += (*hit)->ConstFurtSkills().Value(FSK_MIN_GOLD) + (*hit)->VarFurtSkills().Value(FSK_MIN_GOLD);
				m_stat.AddEntry(TGS_INCOME, pid, val);
			}
		}
	}

	void DrawCells(const iRect& rc, THGUILD_STATS stat, bool bShowStats)
	{
		sint32 plcnt = gGame.Map().m_Players.Count();
		sint32 cellW = rc.w/plcnt;
		iRect cellRc(rc.x, rc.y, cellW, rc.h);

		uint32 w = cellW-1;
		for (sint32 nn=0; nn<plcnt; ++nn) {
			if (nn == (plcnt -1)) w += rc.w - (cellW*plcnt);
			gApp.Surface().Darken25Rect(iRect(rc.x+nn*cellW, rc.y, w, rc.h));
		}

		if (!bShowStats) return;
		for (sint32 cc=0; cc<plcnt;) {
			iSimpleArray<uint8> pids;
			sint32 cval = m_stat.entries[stat].Index(cc);
			pids.Add(m_stat.entries[stat].Value(cc++));
			for (uint32 pp=cc; pp<m_stat.entries[stat].Size(); ++pp) {
				if ( m_stat.entries[stat].Index(pp) > cval ) break;
				pids.Add(m_stat.entries[stat].Value(cc++));
			}
	
			sint32 xpos = cellRc.x + ((sint32)(cellRc.w/2) - THFLAG_WIDTH[pids.GetSize()-1]/2);
			for (uint32 ff=0; ff<pids.GetSize(); ++ff) {
				gGfxMgr.Blit(PDGG_THFLAGS + pids[ff], gApp.Surface(), iPoint(xpos, rc.y+1));
				xpos += 3;
			}
			cellRc.x += cellW;
		}

	}

	void OnCompose()
	{
		iRect rc = GetScrRect();
		if (!m_pCastle) return;

		iPlayer* pPlayer = gGame.Map().FindPlayer(m_pCastle->Owner());
		check(pPlayer);
		uint32 detLvl = 0;
		for (iPlayer::iCtLIt cit = pPlayer->CastleFirst(); cit != pPlayer->CastleEnd(); ++cit) if ( (*cit)->FindCnst(CTLCNST_TAVERN)->IsBuilt() ) detLvl++;

		iRect enRc(rc.x, rc.y,rc.w,13);
		for (uint32 nn=0; nn<TGS_COUNT; ++nn) {
			gApp.Surface().Darken50Rect(enRc);
			// Title
			gTextComposer.TextOut((detLvl > nn)?dlgfc_topic:dlgfc_plain, gApp.Surface(), rc, gTextMgr[TRID_THGUILD_NUMOFTOWNS+nn], enRc, AlignCenter);
			// Values
			DrawCells(iRect(enRc.x, enRc.y+14, enRc.w, 11), (THGUILD_STATS)nn, detLvl > nn);
			enRc.y += 26;
		}
	}

private:
	ThGuildStat	m_stat;
	iCastle*	m_pCastle;
};

//////////////////////////////////////////////////////////////////////////
class iTavernTab : public iCtlViewChild, public  IViewCmdHandler
{
public:
	iTavernTab(iViewMgr* pViewMgr)
	: iCtlViewChild(pViewMgr), m_pCastle(NULL), m_pTavern(NULL)
	{
		AddChild(m_pRecrBtn = new iTextButton(pViewMgr, this, iRect(100, 80-DEF_BTN_HEIGHT, 55,DEF_BTN_HEIGHT), TRID_BTN_RECRUIT, 110));
		m_pRecrBtn->SetEnabled(false);
		AddChild(m_pThievesGuild = new iThievesGuildView(pViewMgr, iRect(161,0,m_Rect.w-161,m_Rect.h),200));
	}

	void OnActivate() 
	{
		SetCastle(m_pCastle);
	}

	void OnCompose()
	{
		iCtlViewChild::OnCompose();
		iRect rc = GetScrRect();
		check(m_pTavern && m_pTavern->IsBuilt());

		gApp.Surface().VLine(iPoint(rc.x+160,rc.y), rc.y+rc.h, cColor_Black);
		gApp.Surface().HLine(iPoint(rc.x,rc.y+85), rc.x+160, cColor_Black);

		/*
		if (!m_pTavern || !m_pTavern->IsBuilt()) {
			gTextComposer.TextOut(dlgfc_plain,gApp.Surface(),rc.point(), _T("Tavern not available!"), rc, AlignCenter);
			return;
		}*/

		// Visitor
		if (const iHero* pVisitor = m_pTavern->Visitor()) {
			ButtonFrame(gApp.Surface(),iRect(rc.x+5,rc.y+5,50,50),iButton::Pressed);
			gGfxMgr.Blit(pVisitor->Proto()->m_icn48, gApp.Surface(), iPoint(rc.x+6,rc.y+6));
			iRect trc(rc.x+55, rc.y+7, 105, 50);
			iTextComposer::FontConfig sfc(iTextComposer::FS_SMALL, RGB16(0xE0,0xE0,0xE0));
			iTextComposer::FontConfig nfc(iTextComposer::FS_LARGE, RGB16(0xA0,0xF0,0xF0));
			gTextComposer.TextOut(nfc,gApp.Surface(), trc.point(), pVisitor->Name(), trc, AlignTop);
			trc.DeflateRect(0,15,0,0);
			gTextComposer.TextOut(sfc,gApp.Surface(), trc.point(), iFormat(_T("%s, %s %d"),gTextMgr[TRID_HERO_TYPE_KNIGHT+pVisitor->Type()], gTextMgr[TRID_LEVEL],pVisitor->Level()), trc, AlignTop);
			trc.DeflateRect(0,17,0,0);
			iStringT psText;
			for (sint32 psid=0; psid<PRSKILL_COUNT; ++psid) {
				psText.Addf(_T("#I%04X #FCCC%d "), PDGG_ICN_PSKILLS+psid, pVisitor->FurtSkill((FURTHER_SKILLS)(FSK_ATTACK+psid)));
			}
			gTextComposer.TextOut(gApp.Surface(), trc.point(), psText, trc, AlignTop);

			iRect crc(rc.x, rc.y + 80 - DEF_BTN_HEIGHT, 100, DEF_BTN_HEIGHT);
			gTextComposer.TextOut(dlgfc_topic,gApp.Surface(), crc.point(), MineralSet2Text(pVisitor->Proto()->m_Cost), crc, AlignCenter);
		} else {
			iTextComposer::FontConfig fc(iTextComposer::FS_MEDIUM, RGB16(0xE0,0xE0,0xE0));
			gTextComposer.TextBoxOut(fc, gApp.Surface(), gTextMgr[TRID_MSG_NO_HERO_TO_RECRUIT], iRect(rc.x, rc.y+20, 160, 85));
		}

		// Rumor
		iRect rrc(rc.x+5, rc.y+90, 150, rc.h - 95);
		sint32 h = gTextComposer.TextBoxOut(dlgfc_stopic,gApp.Surface(), gTextMgr[TRID_TAVERN_RUMOR_FOREWORD], rrc);
		sint32 tbh = gTextComposer.GetTextBoxSize(gTextMgr[TRID_TAVERN_RUMOR_01+m_pTavern->CurRumor()], rrc.w, dlgfc_splain).h;
		rrc.DeflateRect(0, h,0,0);
		rrc.DeflateRect(0, (sint32)(rrc.h/2)-tbh/2, 0, 0);
		gTextComposer.TextBoxOut(dlgfc_splain,gApp.Surface(), gTextMgr[TRID_TAVERN_RUMOR_01+m_pTavern->CurRumor()], rrc);

		// Thieve's Guild

	}

	void SetCastle(iCastle* pCastle)
	{ 
		m_pCastle = pCastle;
		if (pCastle) {
			m_pTavern = m_pCastle->GetTavern(true);
		} else {
			m_pTavern = NULL;
		}
		m_pThievesGuild->SetCastle(m_pCastle);
		m_pRecrBtn->SetEnabled(m_pTavern && m_pTavern->CanRecruit());
		Invalidate(); 
	}

private:
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
	{ 
		uint32 uid = pView->GetUID();
		if (uid == 110) {
			iHero* pNewHero = m_pTavern->RecruitVisitor();
			SetCastle(m_pCastle);
		}
	}
	
private:
	iThievesGuildView*	m_pThievesGuild;
	iTextButton*		m_pRecrBtn;
	iCastle*			m_pCastle;
	iTavernCtlCnst*		m_pTavern;
};

#endif //_HMM_GAME_CASTLE_VIEW_TAVERN_PAGE_H_
