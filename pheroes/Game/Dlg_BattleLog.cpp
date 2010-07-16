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
#include "Dlg_BattleLog.h"

//////////////////////////////////////////////////////////////////////////
class iTextListBox : public iListBox
{
public:
	iTextListBox(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid, const iSimpleArray<iStringT>& strList)
	: iListBox(pViewMgr, pCmdHandler, rect, uid), m_strList(strList)
	{}

	inline uint32 LBItemHeight() const { return 15; }
	inline uint32 LBItemsCount() const { return m_strList.GetSize(); }

private:
	void ComposeLBBackground(const iRect& rect)
	{
		iRect rc = rect;
		gApp.Surface().Darken25Rect(rc);
		rc.InflateRect(1);
		gApp.Surface().FrameRect(rc, cColor_Black);
	}

	void ComposeLBItem(uint32 iIdx, bool bSel, const iRect& irc)
	{
		iRect rc=irc;

		if (bSel) {
			gGfxMgr.BlitTile(PDGG_CTILE, gApp.Surface(),rc);
			ButtonFrame(gApp.Surface(),rc,0);
		}

		rc.DeflateRect(2);

		gTextComposer.TextOut(gApp.Surface(), rc, gTextComposer.CropString(m_strList[iIdx], dlgfc_splain, rc.w, true), rc, AlignLeft);
	}

private:
	const iSimpleArray<iStringT>&	m_strList;
};

//////////////////////////////////////////////////////////////////////////
iDlg_BattleLog::iDlg_BattleLog(iViewMgr* pViewMgr, PLAYER_ID pid, const iSimpleArray<iStringT>& log)
: iBaseGameDlg(pViewMgr, pid), m_log(log)
{
}

void iDlg_BattleLog::OnCreateDlg()
{
	iRect clRect = ClientRect();
	AddChild(m_pLogList = new iTextListBox(m_pMgr, this, iRect(clRect.x+5,clRect.y+20,clRect.w-26,135), 101, m_log));
	AddChild(m_pScrollBar = new iPHScrollBar(m_pMgr, this, iRect(clRect.x+clRect.w-20,clRect.y+20,15,135), 102));
	m_pLogList->SetScrollBar(m_pScrollBar);
	if (m_pLogList->LBItemsCount() > m_pLogList->PageSize()) {
		// scroll list down
		sint32 npos = m_pLogList->LBItemsCount() - m_pLogList->PageSize();
		m_pScrollBar->SetCurPos(npos);
		m_pLogList->SetCurPos(npos);
	}
	AddChild(new iTextButton(m_pMgr,this,iRect(clRect.x+(clRect.w/2-20),clRect.y2()-DEF_BTN_HEIGHT,40,DEF_BTN_HEIGHT),TRID_OK, DRC_OK));
}

void iDlg_BattleLog::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);

	// title
	gTextComposer.TextOut(dlgfc_hdr, gApp.Surface(), rc.point(), gTextMgr[TRID_COMBAT_LOG], iRect(rc.x,rc.y,rc.w,15),AlignCenter);
}

iSize iDlg_BattleLog::ClientSize() const
{
	return iSize(260,165 + DEF_BTN_HEIGHT);
}

void iDlg_BattleLog::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (uid == DRC_OK && cmd == CCI_BTNCLICK) {
		EndDialog(pView->GetUID());
	} else if (uid == 101) {
		// Listbox
		if (cmd == CCI_LBSELDBLCLICK) {
			if (param != -1) {
				iTextDlg tdlg(m_pMgr, gTextMgr[TRID_COMBAT_LOG], m_log[param], m_pid);
				tdlg.DoModal();
			}
		} 
	} 
}

