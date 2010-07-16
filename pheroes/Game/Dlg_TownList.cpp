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
#include "Dlg_TownList.h"

//////////////////////////////////////////////////////////////////////////
class iTownListBox : public iListBox
{
public:
	enum State {
		Normal,
		Occupied,
	};

	iTownListBox(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid, iPlayer* pOwner)
	: iListBox(pViewMgr, pCmdHandler, rect, uid), m_pOwner(pOwner)
	{}

	State GetItemState(sint32 idx) {
		check(idx >=0 && idx < (sint32)m_pOwner->CastleCount());
		iCastle* pCastle = *(m_pOwner->CastleFirst()+idx);
		return (pCastle->Visitor())?Occupied:Normal;
	}

	inline uint32 LBItemHeight() const { return 26; }
	inline uint32 LBItemsCount() const { return m_pOwner->CastleCount(); }

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
		iTextComposer::FontConfig fc(dlgfc_plain);
		iRect rc=irc;

		if (bSel) {
			gGfxMgr.BlitTile(PDGG_CTILE, gApp.Surface(),rc);
			ButtonFrame(gApp.Surface(),rc,0);
		}

		rc.DeflateRect(2);
		iCastle* pCastle = *(m_pOwner->CastleFirst()+iIdx);

		// Glyph
		gApp.Surface().FillRect(iRect(rc.x,rc.y,34,22),cColor_Black);
		SpriteId sid = PDGG_CTL_SICONS + (pCastle->Proto()->Size()-1)*CTLT_COUNT + pCastle->Proto()->Type();
		gGfxMgr.Blit(sid,gApp.Surface(), iPoint(rc.x+1,rc.y+1));

		// Name
		rc.DeflateRect(36,0,0,0);
		rc.h = 10;
		gTextComposer.TextOut(dlgfc_topic,gApp.Surface(),rc,pCastle->Name(),rc,AlignTop);
		rc.y+=10;
		State state = GetItemState(iIdx);
		if (state == Occupied) {
			iTextComposer::FontConfig fc(iTextComposer::FS_SMALL,RGB16(255,128,128));
			gTextComposer.TextOut(fc,gApp.Surface(),rc,pCastle->Visitor()->Name(),rc,AlignTop);
		} else if (state == Normal) {
			iTextComposer::FontConfig fc(iTextComposer::FS_SMALL,RGB16(128,255,128));
			gTextComposer.TextOut(fc,gApp.Surface(),rc,gTextMgr[TRID_UNOCCUPIED],rc,AlignTop);
		} else {
			check(0);
		}
	}

private:
	iPlayer*	m_pOwner;
};

//////////////////////////////////////////////////////////////////////////
iDlg_TownList::iDlg_TownList(iViewMgr* pViewMgr, iPlayer* pOwner, const iStringT& title, TextResId okBtnText)
: iBaseGameDlg(pViewMgr,pOwner->PlayerId()), m_pOwner(pOwner), m_title(title), m_okBtnText(okBtnText), m_pDestination(NULL)
{}

void iDlg_TownList::OnCreateDlg()
{
	iRect clRect = ClientRect();

	AddChild(m_pTownList = new iTownListBox(m_pMgr, this, iRect(clRect.x+5, clRect.y+20, clRect.w-25-1, 130), 101, m_pOwner));
	AddChild(m_pScrollBar = new iPHScrollBar(m_pMgr, this, iRect(clRect.x+clRect.w-20,clRect.y+20,15,130), 102));
	m_pTownList->SetScrollBar(m_pScrollBar);

	sint32 npos = clRect.x + (clRect.w/2-65);
	AddChild(m_pbtnOk =  new iTextButton(m_pMgr,this,iRect(npos,clRect.y2()-DEF_BTN_HEIGHT,60,DEF_BTN_HEIGHT),m_okBtnText, DRC_OK, Visible));
	AddChild(new iTextButton(m_pMgr,this,iRect(npos+70,clRect.y2()-DEF_BTN_HEIGHT,60,DEF_BTN_HEIGHT),TRID_CANCEL, DRC_CANCEL));
}

void iDlg_TownList::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);
	// title
	gTextComposer.TextOut(dlgfc_hdr, gApp.Surface(),rc.point(), m_title, iRect(rc.x,rc.y,rc.w,15),AlignTop);
	rc.y+=15;
}

iSize iDlg_TownList::ClientSize() const
{
	sint32 w = 200;
	sint32 h = 0;

	// title
	h += 20;
	// town list
	h += 5+130+5;
	// buttons
	h += DEF_BTN_HEIGHT;

	return iSize(w,h);
}

void iDlg_TownList::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (uid == DRC_CANCEL) {
		EndDialog(uid);
	} else if (uid == DRC_OK) {
		sint32 sel = m_pTownList->SelItem();
		check(m_pTownList->GetItemState(sel) == iTownListBox::Normal);
		m_pDestination = *(m_pOwner->CastleFirst() + sel);
		EndDialog(uid);
	} else if (uid == 101) {
		// Listbox
		if (cmd == CCI_LBSELCHANGED) {
			m_pbtnOk->SetEnabled(param != -1 && m_pTownList->GetItemState(param) == iTownListBox::Normal);
		} else if (cmd == CCI_LBSELDBLCLICK) {
			check(param != -1 && param < (sint32)m_pOwner->CastleCount());
			if (m_pTownList->GetItemState(param) == iTownListBox::Normal) {
				m_pDestination = *(m_pOwner->CastleFirst() + param);
				EndDialog(DRC_OK);
			}
		}
	}
}


