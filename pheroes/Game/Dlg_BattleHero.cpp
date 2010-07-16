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
#include "Dlg_BattleHero.h"

iDlg_BattleHero::iDlg_BattleHero(iViewMgr* pViewMgr, iHero& hero, uint32 flags)
: iBaseGameDlg(pViewMgr, hero.Owner()->PlayerId()), m_hero(hero), m_flags(flags)
{}

void iDlg_BattleHero::OnCreateDlg()
{
	iRect clRect = ClientRect();

	// Button size 52x15 (3*52+10)
	iRect rc(clRect.x + (clRect.w/2-83), clRect.y2()-DEF_BTN_HEIGHT, 52, DEF_BTN_HEIGHT);
	AddChild(new iTextButton(m_pMgr,this,rc,TRID_SURRENDER, Surrender, (m_flags&CanSurrender)?(Visible|Enabled):(Visible) ));
	rc.x+=57;
	AddChild(new iTextButton(m_pMgr,this,rc,TRID_RETREAT, Retreat, (m_flags&CanRetreat)?(Visible|Enabled):(Visible)));
	rc.x+=57;
	AddChild(new iTextButton(m_pMgr,this,rc,TRID_CLOSE, Close));
}

void iDlg_BattleHero::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);

	// Name
	iStringT txt;
	txt.Addf(_T("#S0#FEEE%s#S2#FAFF %s #S0#FEEE%s %d"),gTextMgr[TRID_HERO_TYPE_KNIGHT+m_hero.Type()],m_hero.Name().CStr(),gTextMgr[TRID_LEVEL],m_hero.Level());
	gTextComposer.TextOut(gApp.Surface(),rc,txt,rc,AlignTop);
	rc.y += 15;

	rc.DeflateRect(5);

	// Portrait
	ButtonFrame(gApp.Surface(),iRect(rc.x,rc.y,50,50),iButton::Pressed);
	gGfxMgr.Blit(m_hero.Proto()->m_icn48,gApp.Surface(),iPoint(rc.x+1,rc.y+1));


	iRect trc(rc.x+57,rc.y,rc.w-55,50);
	//gApp.Surface().FillRect(trc,0);
	iTextComposer::FontConfig fch(iTextComposer::FS_SMALL, RGB16(255,192,128));
	iTextComposer::FontConfig fcv(iTextComposer::FS_SMALL, RGB16(255,255,0));

	iRect ttrc(trc.x,trc.y,trc.w/2,10);
	gTextComposer.TextOut(fch, gApp.Surface(),iPoint(),iStringT(gTextMgr[TRID_SKILL_ATTACK_S+0])+_T(" :"),iRect(ttrc.x,ttrc.y,ttrc.w-15,ttrc.h),AlignTopRight);
	gTextComposer.TextOut(fcv, gApp.Surface(),iPoint(),FormatNumber(m_hero.FurtSkill(FSK_ATTACK)),iRect(ttrc.x+ttrc.w-15,ttrc.y,15,ttrc.h),AlignTopLeft);
	ttrc.x+=ttrc.w;
	gTextComposer.TextOut(fch, gApp.Surface(),iPoint(),iStringT(gTextMgr[TRID_SKILL_ATTACK_S+1])+_T(" :"),iRect(ttrc.x,ttrc.y,ttrc.w-15,ttrc.h),AlignTopRight);
	gTextComposer.TextOut(fcv, gApp.Surface(),iPoint(),FormatNumber(m_hero.FurtSkill(FSK_DEFENCE)),iRect(ttrc.x+ttrc.w-15,ttrc.y,15,ttrc.h),AlignTopLeft);
	ttrc.x-=ttrc.w;
	ttrc.y+=ttrc.h;
	gTextComposer.TextOut(fch, gApp.Surface(),iPoint(),iStringT(gTextMgr[TRID_SKILL_ATTACK_S+2])+_T(" :"),iRect(ttrc.x,ttrc.y,ttrc.w-15,ttrc.h),AlignTopRight);
	gTextComposer.TextOut(fcv, gApp.Surface(),iPoint(),FormatNumber(m_hero.FurtSkill(FSK_POWER)),iRect(ttrc.x+ttrc.w-15,ttrc.y,15,ttrc.h),AlignTopLeft);
	ttrc.x+=ttrc.w;
	gTextComposer.TextOut(fch, gApp.Surface(),iPoint(),iStringT(gTextMgr[TRID_SKILL_ATTACK_S+3])+_T(" :"),iRect(ttrc.x,ttrc.y,ttrc.w-15,ttrc.h),AlignTopRight);
	gTextComposer.TextOut(fcv, gApp.Surface(),iPoint(),FormatNumber(m_hero.FurtSkill(FSK_KNOWLEDGE)),iRect(ttrc.x+ttrc.w-15,ttrc.y,15,ttrc.h),AlignTopLeft);
	ttrc.x-=ttrc.w;
	ttrc.y += 10;

	fch.cmpProps.faceColor = RGB16(192,255,128);
	ttrc.y += 5;
	gTextComposer.TextOut(fch, gApp.Surface(),iPoint(),iStringT(gTextMgr[TRID_SKILL_MORALE_S])+_T(" :"),iRect(ttrc.x,ttrc.y,ttrc.w-15,ttrc.h),AlignTopRight);
	gTextComposer.TextOut(fcv, gApp.Surface(),iPoint(),FormatNumber(m_hero.FurtSkill(FSK_MORALE),true),iRect(ttrc.x+ttrc.w-15,ttrc.y,15,ttrc.h),AlignTopLeft);
	ttrc.x+=ttrc.w;
	gTextComposer.TextOut(fch, gApp.Surface(),iPoint(),iStringT(gTextMgr[TRID_SKILL_LUCK_S])+_T(" :"),iRect(ttrc.x,ttrc.y,ttrc.w-15,ttrc.h),AlignTopRight);
	gTextComposer.TextOut(fcv, gApp.Surface(),iPoint(),FormatNumber(m_hero.FurtSkill(FSK_LUCK),true),iRect(ttrc.x+ttrc.w-15,ttrc.y,15,ttrc.h),AlignTopLeft);
	ttrc.x-=ttrc.w;
	ttrc.y += 10;

	fch.cmpProps.faceColor = RGB16(255,128,128);
	ttrc.y += 5;
	ttrc.w = trc.w;
	gTextComposer.TextOut(fch, gApp.Surface(),iPoint(),gTextMgr[TRID_MANA]+iFormat(_T(" : %d / %d"),m_hero.ManaPts(),m_hero.MaxManaPts()),iRect(ttrc.x,ttrc.y,ttrc.w,ttrc.h),AlignTop);

	rc.y += 55;
}

iSize iDlg_BattleHero::ClientSize() const
{
	sint32 w = 180;
	sint32 h = 0;

	// Name
	h += 20;

	// portrait and skills
	h += 60;

	// buttons
	h += DEF_BTN_HEIGHT + 5;
	return iSize(w,h);
}

void iDlg_BattleHero::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (cmd == CCI_BTNCLICK) {
		EndDialog(pView->GetUID());
	}
}

