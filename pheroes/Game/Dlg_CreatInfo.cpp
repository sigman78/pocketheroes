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
#include "Dlg_CreatInfo.h"

//////////////////////////////////////////////////////////////////////////
class iPerksInfoDlg : public iBaseGameDlg
{
public:
	iPerksInfoDlg(iViewMgr* pViewMgr, CREATURE_TYPE ct)
	: iBaseGameDlg(pViewMgr, PID_NEUTRAL), m_ct(ct)
	{
		// Init perks list
		for (uint16 nn=0; nn<CPERKS_COUNT; ++nn) {
			if (CREAT_DESC[ct].perks & (1<<nn)) m_perks.Add(nn);
		}
	}
	
	void OnCreateDlg()
	{
		iRect clRect = ClientRect();
		AddChild(new iTextButton(m_pMgr,this,iRect(clRect.x+(clRect.w/2-20),clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_OK, DRC_OK));
	}

protected:
	void DoCompose(const iRect& clRect)
	{
		iRect rc(clRect);

		// title
		gTextComposer.TextOut(dlgfc_hdr, gApp.Surface(), rc.point(), gTextMgr[TRID_CREATURE_PEASANT_F2+m_ct*3], rc, AlignTop);
		rc.y += 20;

		// perks
		for (uint32 nn=0; nn<m_perks.GetSize(); ++nn) {
			sint32 h = gTextComposer.TextBoxOut(dlgfc_plain, gApp.Surface(), gTextMgr[TRID_CPERK_DOUBLESHOT+m_perks[nn]], iRect(rc.x+30,rc.y,120,20));
			BlitIcon(gApp.Surface(), PDGG_CREAT_PERKS+m_perks[nn], iRect(rc.x,rc.y,30,h));
			rc.y += h;
			rc.y += 8;
		}
	}

	iSize ClientSize() const
	{
		sint32 w = 155;
		sint32 h = 0;

		// title 
		h += 20;

		// perks
		for (uint32 nn=0; nn<m_perks.GetSize(); ++nn) {
			h += gTextComposer.GetTextBoxSize(gTextMgr[TRID_CPERK_DOUBLESHOT+m_perks[nn]], 120, dlgfc_plain).h;
			h += 8;
		}

		// ok button
		h += DEF_BTN_HEIGHT;

		return iSize(w,h);
	}

	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
	{
		EndDialog(pView->GetUID());
	}

	iSimpleArray<uint16>	m_perks;
	CREATURE_TYPE			m_ct;
};
//////////////////////////////////////////////////////////////////////////
class iPerksBtn : public iButton, public IViewCmdHandler
{
public:
	iPerksBtn(iViewMgr* pViewMgr, const iRect& rect, uint32 uid, CREATURE_TYPE ct)
	: iButton(pViewMgr, this, rect, uid, Visible | Enabled), m_ct(ct)
	{
		// Init perks list
		for (uint16 nn=0; nn<CPERKS_COUNT; ++nn) {
			if (CREAT_DESC[ct].perks & (1<<nn)) m_perks.Add(nn);
		}
	}

	void OnCompose()
	{
		iRect rc = GetScrRect();
		sint32 tw = m_perks.GetSize() * 15 - 1;
		sint32 ox = rc.x + ((sint32)(rc.w/2) - tw/2);
		for (uint32 nn=0; nn<m_perks.GetSize(); ++nn) {
			gGfxMgr.Blit(PDGG_CREAT_PERKS + m_perks[nn], gApp.Surface(), iPoint(ox, rc.y));
			ox += 15;
		}
	}

	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
	{
		iPerksInfoDlg idlg(m_pMgr, m_ct);
		idlg.DoModal();
	}

	CREATURE_TYPE			m_ct;
	iSimpleArray<uint16>	m_perks;
};
//////////////////////////////////////////////////////////////////////////
iDlg_CreatInfo::iDlg_CreatInfo(iViewMgr* pViewMgr, PLAYER_ID pid, iCreatGroup& cGroup, const iFurtSkills& furtSkills, bool bCanDismiss, sint32 moraleMod)
: iBaseGameDlg(pViewMgr,pid), m_cGroup(cGroup), m_furtSkills(furtSkills), m_bCanDismiss(bCanDismiss), m_moraleMod(moraleMod)
{
}

void iDlg_CreatInfo::OnCreateDlg()
{
	iRect clRect = ClientRect();

	// Perks
	if (CREAT_DESC[m_cGroup.Type()].perks != CPERK_NONE) {
		AddChild(new iPerksBtn(m_pMgr, iRect(clRect.x, clRect.y + 60, clRect.w, 15), 500, m_cGroup.Type()));
	}

	// Buttons
	if (m_bCanDismiss) {
		AddChild(new iTextButton(m_pMgr, this, iRect(clRect.x+5, clRect.y2()-DEF_BTN_HEIGHT, 55, DEF_BTN_HEIGHT), TRID_CREAT_DISMISS, 100));
		AddChild(new iTextButton(m_pMgr, this, iRect(clRect.x2()-45, clRect.y2()-DEF_BTN_HEIGHT, 40, DEF_BTN_HEIGHT), TRID_OK, DRC_OK));
	} else {
		AddChild(new iTextButton(m_pMgr, this, iRect(clRect.x + clRect.w/2-20, clRect.y2()-DEF_BTN_HEIGHT, 40, DEF_BTN_HEIGHT), TRID_OK, DRC_OK));
	}
}

void iDlg_CreatInfo::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);

	// title
	gTextComposer.TextOut(dlgfc_hdr, gApp.Surface(),rc.point(),gTextMgr[m_cGroup.Type()*3+TRID_CREATURE_PEASANT_F2], iRect(rc.x,rc.y,rc.w,15),AlignCenter);
	rc.y+=15;

	// icon
	BlitIcon(gApp.Surface(),PDGG_MINIMON+m_cGroup.Type(),iRect(rc.x,rc.y,rc.w,45));
	rc.y+=45;

	// Perks
	if (CREAT_DESC[m_cGroup.Type()].perks != CPERK_NONE) rc.y+=15;

	// props
	iRect trc(rc.x,rc.y,90,12);
	iTextComposer::FontConfig fc(iTextComposer::FS_SMALL, RGB16(192,192,255));
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iStringT(gTextMgr[TRID_SKILL_ATTACK])+_T(" :"),trc,AlignTopRight); trc.y+=10;
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iStringT(gTextMgr[TRID_SKILL_DEFENCE])+_T(" :"),trc,AlignTopRight); trc.y+=10;
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iStringT(gTextMgr[TRID_SKILL_SHOTS])+_T(" :"),trc,AlignTopRight); trc.y+=10;
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iStringT(gTextMgr[TRID_SKILL_DAMAGE])+_T(" :"),trc,AlignTopRight); trc.y+=10;
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iStringT(gTextMgr[TRID_SKILL_HEALTH])+_T(" :"),trc,AlignTopRight); trc.y+=10;
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iStringT(gTextMgr[TRID_SKILL_SPEED])+_T(" :"),trc,AlignTopRight); trc.y+=10;
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iStringT(gTextMgr[TRID_SKILL_MORALE])+_T(" :"),trc,AlignTopRight); trc.y+=10;
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iStringT(gTextMgr[TRID_SKILL_LUCK])+_T(" :"),trc,AlignTopRight); trc.y+=10;

	trc = iRect(rc.x+95,rc.y,rc.w-105,12);
	iTextComposer::FontConfig tfc(iTextComposer::FS_SMALL, RGB16(255,220,192));
	iStringT tout;

	tout.Setf(_T("%d (%d)"),CREAT_DESC[m_cGroup.Type()].attack, CREAT_DESC[m_cGroup.Type()].attack+m_furtSkills.Value(FSK_ATTACK));
	gTextComposer.TextOut(tfc, gApp.Surface(),trc.point(),tout,trc,AlignTopLeft); trc.y+=10;
	tout.Setf(_T("%d (%d)"),CREAT_DESC[m_cGroup.Type()].defence, CREAT_DESC[m_cGroup.Type()].defence+m_furtSkills.Value(FSK_DEFENCE));
	gTextComposer.TextOut(tfc, gApp.Surface(),trc.point(),tout,trc,AlignTopLeft); trc.y+=10;
	if (CREAT_DESC[m_cGroup.Type()].shots) tout.Setf(_T("%d"),CREAT_DESC[m_cGroup.Type()].shots);
	else tout.Setf(_T("-"));
	gTextComposer.TextOut(tfc, gApp.Surface(),trc.point(),tout,trc,AlignTopLeft); trc.y+=10;
	tout.Setf(_T("%d - %d"),CREAT_DESC[m_cGroup.Type()].damage_min,CREAT_DESC[m_cGroup.Type()].damage_max);
	gTextComposer.TextOut(tfc, gApp.Surface(),trc.point(),tout,trc,AlignTopLeft); trc.y+=10;
	tout.Setf(_T("%d (%d)"),CREAT_DESC[m_cGroup.Type()].hits, CREAT_DESC[m_cGroup.Type()].hits+m_furtSkills.Value(FSK_HITS));
	gTextComposer.TextOut(tfc, gApp.Surface(),trc.point(),tout,trc,AlignTopLeft); trc.y+=10;
	tout.Setf(_T("%d (%d)"),CREAT_DESC[m_cGroup.Type()].speed, CREAT_DESC[m_cGroup.Type()].speed+m_furtSkills.Value(FSK_SPEED));
	gTextComposer.TextOut(tfc, gApp.Surface(),trc.point(),tout,trc,AlignTopLeft); trc.y+=10;
	sint32 morale = (CREAT_DESC[m_cGroup.Type()].perks&CPERK_UNDEAD)?0:(m_furtSkills.Value(FSK_MORALE)+m_moraleMod);
	gTextComposer.TextOut(tfc, gApp.Surface(),trc.point(),FormatNumber(morale,true),trc,AlignTopLeft); trc.y+=10;
	gTextComposer.TextOut(tfc, gApp.Surface(),trc.point(),FormatNumber(m_furtSkills.Value(FSK_LUCK),true),trc,AlignTopLeft); trc.y+=10;
}

iSize iDlg_CreatInfo::ClientSize() const
{
	sint32 w = 150;
	sint32 h = 0;

	// title
	h += 15;
	// icon
	h += 45;
	// perks
	if (CREAT_DESC[m_cGroup.Type()].perks != CPERK_NONE) h += 15;
	// characteristics
	h += 8*10+10;
	// buttons
	h += DEF_BTN_HEIGHT;

	return iSize(w,h);
}

void iDlg_CreatInfo::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (uid == DRC_OK) {
		EndDialog(DRC_OK);
	} else if (uid == 100) {
		iQuestDlg dlg(m_pMgr, gTextMgr[m_cGroup.Type()*3+TRID_CREATURE_PEASANT_F2], gTextMgr[TRID_MSG_QDISMISS], m_pid);
		if (dlg.DoModal() == DRC_YES) {
			m_cGroup.Reset();
			EndDialog(DRC_OK);
		}
	}
}
