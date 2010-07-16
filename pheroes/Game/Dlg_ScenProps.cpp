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
#include "Dlg_ScenProps.h"


//////////////////////////////////////////////////////////////////////////
class iPlayerBtn : public iButton
{
public:
	iPlayerBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, PLAYER_ID pid, PLAYER_TYPE pt, uint32 uid, uint32 state = Visible|Enabled)
	: iButton(pViewMgr, pCmdHandler, rect, uid, state), m_pid(pid), m_pt(pt) {}

	void OnCompose()
	{
		iRect rc = GetScrRect();
		gApp.Surface().FrameRect(rc, cColor_Black);
		rc.DeflateRect(1);
		gApp.Surface().FillRect(rc, PLAYER_COLORS[m_pid],96);
		ButtonFrame(gApp.Surface(), rc, m_state);		

		SpriteId sid = (m_pt == PT_HUMAN)?PDGG_ICN_PLT_HUMAN:PDGG_ICN_PLT_AI;
		iPoint op(rc.x + (sint32)(rc.w/2) - (sint32)(gGfxMgr.Dimension(sid).w/2), rc.y + (sint32)(rc.h/2) - (sint32)(gGfxMgr.Dimension(sid).h/2));
		gGfxMgr.Blit(sid, gApp.Surface(), op);
		/*
		if (m_pt == PT_HUMAN) gTextComposer.TextOut(dlgfc_stopic, gApp.Surface(), iPoint(), _T("Human"), rc, AlignCenter);
		else if (m_pt == PT_COMPUTER) gTextComposer.TextOut(dlgfc_splain, gApp.Surface(), iPoint(), _T("CPU"), rc, AlignCenter);
		*/

		if (!IsEnabled()) gApp.Surface().FillRect(rc, cColor_Gray64,128);
	}

	inline PLAYER_TYPE PlayerType() const { return m_pt; }
	inline PLAYER_TYPE& PlayerType() { return m_pt; }
	inline PLAYER_TYPE TogglePlayerType() { return (m_pt==PT_HUMAN)?m_pt=PT_COMPUTER:m_pt=PT_HUMAN; }

private:
	PLAYER_ID	m_pid;
	PLAYER_TYPE	m_pt;
};
//////////////////////////////////////////////////////////////////////////
class iNationBtn : public iButton, public IViewCmdHandler
{
public:
	iNationBtn(iViewMgr* pViewMgr, const iRect& rect, PLAYER_ID pid, CTL_TYPE nt, uint32 uid, bool bFixed)
		: iButton(pViewMgr, this, rect, uid, (bFixed)?(Visible):(Visible|Enabled)), m_pid(pid), m_nt(nt), m_bFixed(bFixed) {}

	inline CTL_TYPE PlayerNation() const { return m_nt; }

private:
	void OnCompose()
	{
		iRect rc = GetScrRect();
		gApp.Surface().FrameRect(rc, cColor_Black);
		rc.DeflateRect(1);

		SpriteId icn = PDGG_CTL_SICONS+CTLT_COUNT*2+m_nt;
		//PDGG_CTL_SICONS + (NATION_COUNT-NATION_HIGHMEN)*2 + (m_nt-NATION_HIGHMEN)
		BlitIcon(gApp.Surface(), icn, rc);
		ButtonFrame(gApp.Surface(), rc, m_state);

		if (!IsEnabled()) gApp.Surface().FillRect(rc, cColor_Gray64,128);
	}
	
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
	{
		if (!m_bFixed && cmd == CCI_BTNCLICK || cmd == CCI_BTNDBLCLICK) {
			if (m_nt == CTLT_RANDOM) m_nt = CTLT_CITADEL;
			else m_nt = (CTL_TYPE)(m_nt+1);
		}
	}


private:
	bool		m_bFixed;
	PLAYER_ID	m_pid;
	CTL_TYPE	m_nt;
};

//////////////////////////////////////////////////////////////////////////
class iDifLvlTab : public iTabbedSwitch
{
public:
	iDifLvlTab(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid, uint32 state)
	: iTabbedSwitch(pViewMgr, pCmdHandler, rect, DFC_COUNT, uid, state) {}

	void ComposeTabItem(uint32 idx, uint32 itemState, const iRect& rect)
	{
		iRect orc = rect;
		orc.DeflateRect(1);
		SpriteId icn = PDGG_ICN_DIF_LEVEL+idx;
		BlitIcon(gApp.Surface(), icn, orc);
		if (itemState & iButton::Selected) gApp.Surface().FrameRect(orc, cColor_Red);
		if (!IsEnabled()) gApp.Surface().FillRect(orc, cColor_Gray64,128);
	}

};

//////////////////////////////////////////////////////////////////////////
iScenPropsDlg::iScenPropsDlg(iViewMgr* pViewMgr, iMapInfo& scProps, bool bReadOnly)
: iBaseGameDlg(pViewMgr,PID_NEUTRAL), m_scProps(scProps), m_bReadOnly(bReadOnly)
{}

void iScenPropsDlg::OnCreateDlg()
{
	iRect clRect = ClientRect();

	sint32 yp = clRect.y;

	AddChild(new iPHLabel(m_pMgr, iRect(clRect.x, yp, clRect.w, 15), m_scProps.m_Name, AlignTop, dlgfc_hdr) );
	yp += 20;

	AddChild(m_pDfcLabel = new iPHLabel(m_pMgr, iRect(clRect.x, yp, clRect.w, 15), GetDfcString(m_scProps.m_Difficulty), AlignTop, dlgfc_topic) );
	yp += 15;

	AddChild(m_difLevel = new iDifLvlTab(m_pMgr, this, iRect(clRect.x+(clRect.w/2-95),yp,190,38), 301, m_bReadOnly?(Visible):(Visible|Enabled)));
	if (m_scProps.m_Difficulty == DFC_UNDEFINED) m_scProps.m_Difficulty = DFC_NORMAL;
	m_difLevel->SetCurrentTab(m_scProps.m_Difficulty);
	m_pDfcLabel->SetText(GetDfcString(m_scProps.m_Difficulty));
	yp += 45;

	uint32 cnt = m_scProps.TotalPlayers();	
	sint32 btnsw = cnt * 34 + (cnt-1)*3;
	sint32 sy = yp;
	sint32 sx = clRect.x + (clRect.w/2 - btnsw/2);
	for (uint32 xx=0; xx<cnt; ++xx) {
		iNationBtn* pNatBtn = new iNationBtn(m_pMgr, iRect(sx,sy,34,22), m_scProps.m_Players[xx].m_Id, m_scProps.m_Players[xx].m_Nation, 150+xx, m_scProps.m_Players[xx].m_Nation != CTLT_RANDOM);
		AddChild(pNatBtn);
		m_btnNations.Add(pNatBtn);
		PLAYER_TYPE tp;
		switch(m_scProps.m_Players[xx].m_TypeMask) {
			case PTM_HUMAN_ONLY:
				tp = PT_HUMAN;
				break;
			case PTM_COMPUTER_ONLY:
				tp = PT_COMPUTER;
				break;
			default:	
				tp = xx == 0 ? PT_HUMAN : PT_COMPUTER;
				break;
		}
		iPlayerBtn* pPlBtn = new iPlayerBtn(m_pMgr, this, iRect(sx,sy+25,34,34), m_scProps.m_Players[xx].m_Id, m_bReadOnly ? m_scProps.m_Players[xx].m_Type : tp, 200+xx, (!m_bReadOnly && (m_scProps.m_Players[xx].m_TypeMask == PTM_HUMAN_OR_COMPUTER))?(Visible|Enabled):(Visible));
		AddChild(pPlBtn);
		m_btnPlayers.Add(pPlBtn);
		sx += 37;
	}

	// Buttons
	sint32 npos = clRect.x + (clRect.w/2-80);
	AddChild(m_pOkBtn = new iTextButton(m_pMgr,this,iRect(npos,clRect.y2()-DEF_BTN_HEIGHT,50,DEF_BTN_HEIGHT),TRID_OK, DRC_OK,iButton::Visible));
	AddChild(new iTextButton(m_pMgr,this,iRect(npos+55,clRect.y2()-DEF_BTN_HEIGHT,50,DEF_BTN_HEIGHT),TRID_INFO, 401));
	AddChild(new iTextButton(m_pMgr,this,iRect(npos+110,clRect.y2()-DEF_BTN_HEIGHT,50,DEF_BTN_HEIGHT),TRID_CANCEL, DRC_CANCEL));

	UpdateControls();
}

void iScenPropsDlg::DoCompose(const iRect& rect)
{
	iRect rc(rect);
}

iSize iScenPropsDlg::ClientSize() const
{
	return iSize(270,150 + DEF_BTN_HEIGHT); 
}

iStringT iScenPropsDlg::GetDfcString(DIFFICULTY_LEVEL dl)
{
	return iFormat(_T("#FCCC%s: #FFF0%s"), gTextMgr[TRID_DIFFICULTY_LEVEL], gTextMgr[TRID_DIFF_EASY+dl]);
}

void iScenPropsDlg::UpdateControls()
{
	bool bHasHuman = false;
	for (uint32 xx=0; xx<m_scProps.m_Players.GetSize(); ++xx) {
		if (m_btnPlayers[xx]->PlayerType() == PT_HUMAN) {
			bHasHuman = true;
			break;
		}
	}
	m_pOkBtn->SetEnabled(bHasHuman);
}

void iScenPropsDlg::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if ( (uid == DRC_OK || uid == DRC_CANCEL) && cmd == CCI_BTNCLICK) {
		// Setup difficulty
		m_scProps.m_Difficulty = (DIFFICULTY_LEVEL)m_difLevel->GetCurrentTab();
		// Setup players
		for (uint32 xx=0; xx<m_scProps.m_Players.GetSize(); ++xx) {
			m_scProps.m_Players[xx].m_Type = m_btnPlayers[xx]->PlayerType();
			m_scProps.m_Players[xx].m_Nation = m_btnNations[xx]->PlayerNation();
		}
		// End dialog
		EndDialog(uid); 
	} else if (uid == 301) {
		m_pDfcLabel->SetText(GetDfcString((DIFFICULTY_LEVEL)m_difLevel->GetCurrentTab()));
	} else if (uid == 401) {
		iStringT title = m_scProps.m_Name;
		//if (!m_scProps.m_Version.Empty()) title.Addf(_T(" v.%s"), m_scProps.m_Version.CStr());
		iStringT desc = m_scProps.m_Description;
		if (!m_scProps.m_Author.Empty()) desc.Addf(_T("\n\n%s: %s"), gTextMgr[TRID_MAP_AUTHOR], m_scProps.m_Author.CStr());
		iTextDlg tdlg(m_pMgr, title, desc, PID_NEUTRAL, dlgfc_topic, dlgfc_splain);
		tdlg.DoModal();
	} else if (uid >= 200 && uid < (200+m_scProps.m_Players.GetSize()) && cmd == CCI_BTNCLICK) {
		uint32 val = uid - 200;
		m_btnPlayers[val]->TogglePlayerType();
		UpdateControls();
	}
}


