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

#ifndef _HMM_GAME_CASTLE_VIEW_CONSTRUCTIONS_PAGE_H_
#define _HMM_GAME_CASTLE_VIEW_CONSTRUCTIONS_PAGE_H_

/*
 *	Castle constructions listbox
 */
class iCnstListBox : public iListBox
{
public:
	iCnstListBox(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid)
	: iListBox(pViewMgr, pCmdHandler, rect, uid), m_pCastle(NULL)
	{}

	inline uint32 LBItemHeight() const
	{ return 16; }

	inline uint32 LBItemsCount() const
	{ if (m_pCastle) return m_pCastle->Cnsts().Count(); else return 0; }

	void SetCastle(iCastle* pCastle)
	{
		m_pCastle = pCastle;
		UpdateList();
	}

	void UpdateList()
	{ UpdateListBox(); }

private:
	void ComposeLBBackground(const iRect& rect)
	{
		iRect rc = rect;
		gApp.Surface().Darken25Rect(rc);
	}

	void ComposeLBItem(uint32 iIdx, bool bSel, const iRect& irc)
	{
		iTextComposer::FontConfig fc(dlgfc_plain);
		iRect rc=irc;

		ButtonFrame(gApp.Surface(),rc,iButton::Pressed);
		rc.DeflateRect(1);
		if (bSel) {
			gGfxMgr.BlitTile(PDGG_CTILE, gApp.Surface(),rc);
			ButtonFrame(gApp.Surface(),rc,0);
		}

		if (!m_pCastle) return;
		rc.DeflateRect(1);
		iCtlCnst::State state = m_pCastle->Cnsts()[iIdx]->GetState();
		if (state == iCtlCnst::Built) {
			fc.cmpProps.faceColor = RGB16(255,192,0);
			BlitIcon(gApp.Surface(),PDGG_CNST_STATE_GLYPH+0, iRect(rc.x2()-14,rc.y,14,rc.h));
		} else if (state == iCtlCnst::Available) {
			fc.cmpProps.faceColor = RGB16(128,255,64);
		} else if (state == iCtlCnst::NotEnoughRes) {
			fc.cmpProps.faceColor = RGB16(255,128,64);
			BlitIcon(gApp.Surface(),PDGG_CNST_STATE_GLYPH+2, iRect(rc.x2()-14,rc.y,14,rc.h));
		} else if (state == iCtlCnst::Unavailable) {
			fc.cmpProps.faceColor = RGB16(255,64,64);
			BlitIcon(gApp.Surface(),PDGG_CNST_STATE_GLYPH+1, iRect(rc.x2()-14,rc.y,14,rc.h));
		}

		gTextComposer.TextOut(fc,gApp.Surface(),rc,m_pCastle->Cnsts()[iIdx]->Name(),rc,AlignLeft);
	}

private:
	iCastle*	m_pCastle;
};

//////////////////////////////////////////////////////////////////////////
class iCnstDescView : public iView
{
public:
	iCnstDescView(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect)
	: iView(pViewMgr,rect,GENERIC_VIEWPORT,0,Enabled|Visible), m_pCnsts(NULL), cIdx(-1)
	{
		AddChild(pBuildBtn = new iTextButton(pViewMgr, pCmdHandler, iRect(rect.x2()-60,rect.y2()-DEF_BTN_HEIGHT-5,55,DEF_BTN_HEIGHT), TRID_BTN_BUILD, 113));
		pBuildBtn->SetEnabled(false);
	}

	void SetCnsts(const iCtlCnsts* pCnsts) { m_pCnsts = pCnsts; cIdx = 0; UpdateState(); }
	void SetCnstIdx(sint32 idx)	{ cIdx=idx; UpdateState(); }

	void UpdateState() { pBuildBtn->SetEnabled(m_pCnsts && cIdx!=-1 && m_pCnsts->At(cIdx)->GetState() == iCtlCnst::Available); Invalidate(); }

	void OnCompose()
	{
		iTextComposer::FontConfig fc(dlgfc_plain);
		iRect rc = GetScrRect();

		gApp.Surface().Darken25Rect(rc);
		ButtonFrame(gApp.Surface(),rc,iButton::Pressed);

		if (!m_pCnsts || cIdx == -1) return;

		iRect orc(rc.x+5, rc.y,rc.w-10,20);

		// Title
		fc.fontSize = iTextComposer::FS_LARGE;
		fc.cmpProps.faceColor = RGB16(255,192,0);
		gTextComposer.TextOut(fc,gApp.Surface(),orc,m_pCnsts->At(cIdx)->Name(),orc,AlignCenter);
		orc.y += 16;

		// Description
		fc.fontSize = iTextComposer::FS_SMALL;
		fc.cmpProps.faceColor = RGB16(160,160,160);
		orc.y += gTextComposer.TextBoxOut(fc,gApp.Surface(),m_pCnsts->At(cIdx)->Desc(),orc);

		// Requirement list
		if (m_pCnsts->At(cIdx)->GetState() == iCtlCnst::Unavailable) {
			fc.fontSize = iTextComposer::FS_MEDIUM;
			fc.cmpProps.faceColor = RGB16(255,128,0);
			gTextComposer.TextOut(fc,gApp.Surface(),orc,gTextMgr[TRID_REQUIRES],orc,AlignCenter);
			orc.y += 15;
			iSimpleArray<uint16> dList;
			m_pCnsts->DependsList(m_pCnsts->At(cIdx), dList);
			fc.fontSize = iTextComposer::FS_SMALL;
			fc.cmpProps.faceColor = RGB16(192,160,160);
			if (dList.GetSize() <= 4){
				for (uint32 xx=0; xx<dList.GetSize(); ++xx){
					gTextComposer.TextOut(fc,gApp.Surface(),orc,iCtlCnst::Name((CTL_CONSTR)(dList[xx])),orc,AlignCenter);
					orc.y += 12;
				}
			} else {
				for (uint32 xx=0; xx<3; ++xx){
					gTextComposer.TextOut(fc,gApp.Surface(),orc,iCtlCnst::Name((CTL_CONSTR)(dList[xx])),orc,AlignCenter);
					orc.y += 12;
				}
				gTextComposer.TextOut(fc,gApp.Surface(),orc,_T("(...)"),orc,AlignCenter);
				orc.y += 12;
			}
			orc.y += 8;
		} else if (m_pCnsts->At(cIdx)->GetState() == iCtlCnst::Built) {
			orc.y += 8;
			fc.fontSize = iTextComposer::FS_MEDIUM;
			fc.cmpProps.faceColor = RGB16(255,192,0);
			orc.y += gTextComposer.TextBoxOut(fc,gApp.Surface(),gTextMgr[TRID_MSG_CNSTALREADYBUILT],orc);
			iStringT actText;
			if (m_pCnsts->At(cIdx)->GetActivityText(actText)) {
				fc.fontSize = iTextComposer::FS_SMALL;
				fc.cmpProps.faceColor = RGB16(220,220,220);
				orc.y += gTextComposer.TextBoxOut(fc,gApp.Surface(),actText,orc);
			}
		} else {
			fc.fontSize = iTextComposer::FS_SMALL;
			fc.cmpProps.faceColor = RGB16(160,255,160);
			orc.y += gTextComposer.TextBoxOut(fc,gApp.Surface(),gTextMgr[TRID_MSG_BLDNODEPS],orc);
		}

		// Cost
		iMineralSet ms = m_pCnsts->At(cIdx)->Price();
		fc.fontSize = iTextComposer::FS_MEDIUM;
		fc.cmpProps.faceColor = RGB16(255,192,0);
		gTextComposer.TextOut(fc,gApp.Surface(),orc,gTextMgr[TRID_COST],orc,AlignCenter);
		orc.y += 15;
		iStringT costTxt;
		uint32 cnt=0;
		for (uint32 xx=0; xx<MINERAL_COUNT; ++xx){
			if (ms.quant[xx]) {
				bool bDif = ms.quant[xx] > gGame.Map().FindPlayer(m_pCnsts->Owner()->Owner())->Mineral((MINERAL_TYPE)xx);
				costTxt.Addf(_T("  #I%04X%s%d  "),PDGG_RES_MINI_ICONS+xx, (bDif)?_T("#FF44"):_T("#FCCC"), ms.quant[xx]);
				++cnt;
			}
			if (cnt == 4) {
				gTextComposer.TextOut(fc,gApp.Surface(),orc,costTxt,orc,AlignCenter);
				costTxt = _T("");
				orc.y+=15;
				cnt = 0;
			}
		}
		gTextComposer.TextOut(gApp.Surface(),orc,costTxt,orc,AlignCenter);

	}

	const iCtlCnsts*	m_pCnsts;
	sint32				cIdx;
	iTextButton*		pBuildBtn;
};

//////////////////////////////////////////////////////////////////////////
class iCnstTab : public iCtlViewChild, public IViewCmdHandler
{
public:
	iCnstTab(iViewMgr* pViewMgr, iCastleView* pCastleView)
	: iCtlViewChild(pViewMgr), m_pCastleView(pCastleView), m_pCastle(NULL)
	{
		AddChild(pDescView = new iCnstDescView(pViewMgr, this, iRect(3,3,174,176)));
		AddChild(pListBox = new iCnstListBox(pViewMgr, this, iRect(181,3,120,176), 111));
		AddChild(pScrBar = new iPHScrollBar(pViewMgr, NULL, iRect(302,4,15,174), 112));
		pListBox->SetScrollBar(pScrBar);
	}

	void SetCastle(iCastle* pCastle)
	{
		m_pCastle = pCastle;
		pListBox->SetCastle(pCastle);
		pDescView->SetCnsts(&m_pCastle->Cnsts());
	}

	void OnCompose()
	{
		iCtlViewChild::OnCompose();
		iRect rc(301,3,17,176);
		rc += GetScrRect().point();
		ButtonFrame(gApp.Surface(), rc, iButton::Pressed);
		/*
		iRect rc = GetScrRect();
		gGame.MainView()->Composer().Compose();
		gGfxMgr.BlitTile(PDGG_BKTILE, gApp.Surface(),rc);
		gApp.Surface().Darken25Rect(rc);
		*/
	}

	void OnActivate()
	{
		m_pCastle->UpdateCnsts();
		pDescView->UpdateState();
	}

	inline iCnstListBox* CnstList() { return pListBox; }

private:
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
	{
		uint32 uid = pView->GetUID();
		if (uid == 111) {
			if (cmd == CCI_LBSELCHANGED) {
				if (!m_pCastle ) pDescView->SetCnsts(NULL);
				else if (param == -1) pDescView->SetCnstIdx(-1);
				else pDescView->SetCnstIdx(param);
			}
		} else if (uid == 113) {
			// Build button
			check(pDescView->cIdx != -1);
			m_pCastle->Build(pDescView->cIdx);
			pDescView->UpdateState();
			m_pCastleView->OnBuildCnst();
		}
	}
	
private:
	iCastleView*	m_pCastleView;
	iCastle*		m_pCastle;
	iCnstDescView*	pDescView;
	iCnstListBox*	pListBox;
	iPHScrollBar*	pScrBar;
};

#endif //_HMM_GAME_CASTLE_VIEW_CONSTRUCTIONS_PAGE_H_

