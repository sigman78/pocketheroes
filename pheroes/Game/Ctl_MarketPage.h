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

#ifndef _HMM_GAME_CASTLE_VIEW_MARKETPLACE_PAGE_H_
#define _HMM_GAME_CASTLE_VIEW_MARKETPLACE_PAGE_H_

//////////////////////////////////////////////////////////////////////////
class iMarketView : public iView, public IViewCmdHandler
{
private:
	class iMineralBtn : public iButton
	{
	public:
		iMineralBtn(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid, MINERAL_TYPE mt)
		: iButton(pViewMgr, pCmdHandler, rect, uid, Visible|Enabled), m_mt(mt), m_bSelected(false) {}

		inline MINERAL_TYPE MineralType() const { return m_mt; }

		void SetState(bool bSeleted)
		{
			m_bSelected = bSeleted;
			Invalidate();
		}

		void SetQuant(FractionCoeff nval)
		{
			m_quant = nval;
			Invalidate();
		}

		inline FractionCoeff Quantity() const { return m_quant; }

		void OnCompose()
		{
			iRect rc = GetScrRect();
			ButtonFrame(gApp.Surface(),rc,GetButtonState());
			if (m_bSelected) gApp.Surface().FillRect(rc, RGB16(255,192,0), 64);
			gApp.Surface().FrameRect(iRect(rc.x-1,rc.y-1,rc.w+2,rc.h+2),cColor_Black);

			BlitIcon(gApp.Surface(), PDGG_RES_BIG_ICONS+m_mt, iRect(rc.x,rc.y,rc.w,rc.h-13));
			rc.DeflateRect(0,rc.h-13,0,0);
			
            iTextComposer::FontConfig fontcfg(iTextComposer::FS_MEDIUM,RGB16(192,192,192)) ;
			if (!m_quant.IsValid()) gTextComposer.TextOut( fontcfg, gApp.Surface(),rc.point(), gTextMgr[TRID_NA], rc, AlignCenter);
			else if (m_quant.num == 0 || m_quant.num >= m_quant.denum) gTextComposer.TextOut( fontcfg, gApp.Surface(),rc.point(), FormatNumber(m_quant.num), rc, AlignCenter);
			else gTextComposer.TextOut(fontcfg, gApp.Surface(),rc.point(), iFormat(_T("%d : %d"), m_quant.num, m_quant.denum), rc, AlignCenter);
		}

	private:
		bool			m_bSelected;
		MINERAL_TYPE	m_mt;
		FractionCoeff	m_quant;
	};

public:

	iMarketView(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid, const iSize& cellSiz)
	: iView(pViewMgr, rect, GENERIC_VIEWPORT, uid, Visible|Enabled), m_pCmdHandler(pCmdHandler), m_cellSiz(cellSiz)
	, m_leftSel(-1), m_rightSel(-1), m_marketIdx(1), m_pCastle(NULL)
	{
		sint32 ox = rect.w/4 - cellSiz.w*3/2;
		sint32 oy = 14;
		sint32 bw = cellSiz.w;
		sint32 bh = cellSiz.h;
		sint32 b = 1;

		// Left side minerals
		AddChild(m_leftButtons[2] = new iMineralBtn(pViewMgr, this, iRect(ox+(bw+b)*0,(bh+b)*0+oy,bw,bh), 203, MINERAL_WOOD));
		AddChild(m_leftButtons[3] = new iMineralBtn(pViewMgr, this, iRect(ox+(bw+b)*1,(bh+b)*0+oy,bw,bh), 204, MINERAL_MERCURY));
		AddChild(m_leftButtons[1] = new iMineralBtn(pViewMgr, this, iRect(ox+(bw+b)*2,(bh+b)*0+oy,bw,bh), 202, MINERAL_ORE));
		AddChild(m_leftButtons[6] = new iMineralBtn(pViewMgr, this, iRect(ox+(bw+b)*0,(bh+b)*1+oy,bw,bh), 207, MINERAL_SULFUR));
		AddChild(m_leftButtons[5] = new iMineralBtn(pViewMgr, this, iRect(ox+(bw+b)*1,(bh+b)*1+oy,bw,bh), 206, MINERAL_CRYSTAL));
		AddChild(m_leftButtons[4] = new iMineralBtn(pViewMgr, this, iRect(ox+(bw+b)*2,(bh+b)*1+oy,bw,bh), 205, MINERAL_GEMS));
		AddChild(m_leftButtons[0] = new iMineralBtn(pViewMgr, this, iRect(ox+(bw+b)*0,(bh+b)*2+oy,bw,bh), 201, MINERAL_GOLD));
		ox += rect.w/2;

		// Right side minerals
		AddChild(m_rightButtons[2] = new iMineralBtn(pViewMgr, this, iRect(ox+(bw+b)*0,(bh+b)*0+oy,bw,bh), 213, MINERAL_WOOD));
		AddChild(m_rightButtons[3] = new iMineralBtn(pViewMgr, this, iRect(ox+(bw+b)*1,(bh+b)*0+oy,bw,bh), 214, MINERAL_MERCURY));
		AddChild(m_rightButtons[1] = new iMineralBtn(pViewMgr, this, iRect(ox+(bw+b)*2,(bh+b)*0+oy,bw,bh), 212, MINERAL_ORE));
		AddChild(m_rightButtons[6] = new iMineralBtn(pViewMgr, this, iRect(ox+(bw+b)*0,(bh+b)*1+oy,bw,bh), 217, MINERAL_SULFUR));
		AddChild(m_rightButtons[5] = new iMineralBtn(pViewMgr, this, iRect(ox+(bw+b)*1,(bh+b)*1+oy,bw,bh), 216, MINERAL_CRYSTAL));
		AddChild(m_rightButtons[4] = new iMineralBtn(pViewMgr, this, iRect(ox+(bw+b)*2,(bh+b)*1+oy,bw,bh), 215, MINERAL_GEMS));
		AddChild(m_rightButtons[0] = new iMineralBtn(pViewMgr, this, iRect(ox+(bw+b)*2,(bh+b)*2+oy,bw,bh), 211, MINERAL_GOLD));

		// Slider
		AddChild(m_pSlider = new iPHScrollBar(m_pMgr, this, iRect(rect.w/2-(bw*2), oy + bh * 3,bw*4,15), 100, iScrollBar::Horizontal));

		// Max and Trade buttons
		AddChild(m_pMaxBtn = new iDlgIconButton(m_pMgr, this, iRect(rect.w/2-55, oy + bh * 3 + 20, 50, DEF_BTN_HEIGHT),PDGG_BTN_MAX, 101));
		AddChild(m_pTradeBtn = new iTextButton(m_pMgr, this, iRect(rect.w/2+5, oy + bh * 3 + 20, 50, DEF_BTN_HEIGHT), TRID_TRADE, 102));
	}

	void OnCompose()
	{
		iRect rc = GetScrRect();
		gTextComposer.TextOut(dlgfc_plain, gApp.Surface(), rc.point(), gTextMgr[TRID_KINGDOM_RESOURCES], iRect(rc.x+rc.w/4-m_cellSiz.w*3/2,rc.y,m_cellSiz.w*3,16), AlignTop);
		gTextComposer.TextOut(dlgfc_plain, gApp.Surface(), rc.point(), gTextMgr[TRID_AVAILABLE_FOR_TRADE], iRect(rc.x+rc.w/2 + rc.w/4-m_cellSiz.w*3/2,rc.y,m_cellSiz.w*3,16), AlignTop);

		iRect trc(rc.x+rc.w/2-m_cellSiz.w*2, rc.y + 20 + m_cellSiz.h*2, m_cellSiz.w*4, m_cellSiz.h/2+12);
		if (m_pTradeBtn->IsEnabled()) {
			MINERAL_TYPE mfrom = m_leftButtons[m_leftSel]->MineralType();
			MINERAL_TYPE mto = m_rightButtons[m_rightSel]->MineralType();
			FractionCoeff nval = MineralExchRate(mfrom, mto, m_marketIdx).GetNormalized();
			trc.DeflateRect(0, gTextComposer.TextBoxOut(dlgfc_splain, gApp.Surface(), gTextMgr[TRID_MSG_MARKET_OFFER], trc),0,0);
			gTextComposer.TextOut(dlgfc_topic, gApp.Surface(), trc.point(), iFormat(gTextMgr[TRID_MSG_MARKET_SFORS], Mineral2Text(mto, nval.num * m_pSlider->CurPos()).CStr(), Mineral2Text(mfrom, nval.denum * m_pSlider->CurPos()).CStr()), trc, AlignCenter);
		} else gTextComposer.TextBoxOut(dlgfc_splain, gApp.Surface(), gTextMgr[TRID_MSG_MARKET_DEF], trc);
		//gApp.Surface().FillRect(iRect(rc.x+rc.w/2-m_cellSiz.w*2, rc.y + 26 + m_cellSiz.h*2, m_cellSiz.w*4, m_cellSiz.h/2+6 ), cColor_Black);
		//gTextComposer.TextOut(dlgfc_plain, gApp.Surface(), rc.point(), gTextMgr[TRID_KINGDOM_RESOURCES], iRect(rc.x+rc.w/4-m_cellSiz.w*3/2,rc.y,m_cellSiz.w*3,16), AlignTop);
	}

	void SetCastle(iCastle* pCastle)
	{
		m_pCastle = pCastle;
		m_marketIdx = iMIN<sint32>(9,gGame.Map().FindPlayer(pCastle->Owner())->GetMarketIdx());
		if (m_leftSel != -1) m_leftButtons[m_leftSel]->SetState(false);
		if (m_rightSel != -1) m_rightButtons[m_rightSel]->SetState(false);
		m_leftSel = m_rightSel = -1;
		UpdateLeftSide();
		UpdateControls();
	}

	void UpdateLeftSide()
	{
		const iMineralSet& ms = gGame.Map().FindPlayer(m_pCastle->Owner())->Minerals();
		for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) {
			m_leftButtons[xx]->SetQuant( FractionCoeff(ms.quant[xx],1) );
		}
	}

	void UpdateRightSide()
	{
		if (m_leftSel == -1) {
			for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) m_rightButtons[xx]->SetQuant(FractionCoeff());
		} else {
			MINERAL_TYPE from = m_leftButtons[m_leftSel]->MineralType();
			for (uint32 xx=0; xx<MINERAL_COUNT; ++xx) {
				MINERAL_TYPE to = m_rightButtons[xx]->MineralType();
				if (from != to)	{
					FractionCoeff nval = MineralExchRate(from, to, m_marketIdx).GetNormalized();
					m_rightButtons[xx]->SetQuant(nval);
				} else {
					m_rightButtons[xx]->SetQuant(FractionCoeff());
				}
			}
		}
	}

	void UpdateControls()
	{
		UpdateRightSide();
		bool dall = m_leftSel == -1 || m_rightSel == -1 || m_leftButtons[m_leftSel]->MineralType() == m_rightButtons[m_rightSel]->MineralType();

		if (!dall) {
			sint32 lnum = m_leftButtons[m_leftSel]->Quantity().num;
			sint32 rnum = m_rightButtons[m_rightSel]->Quantity().denum;
			m_pSlider->SetMetrics(lnum/rnum+1,1);
			if (lnum < rnum) dall = true;
		} else {
			m_pSlider->SetMetrics(0,0);
		}

		m_pSlider->SetCurPos(0);
		m_pMaxBtn->SetEnabled(!dall);
		m_pTradeBtn->SetEnabled(false);
		m_pSlider->SetEnabled(!dall);
	}

	void SliderPosChanged()
	{
		check(m_pSlider->IsEnabled());
		m_pTradeBtn->SetEnabled(m_pSlider->CurPos() > 0);
	}

	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
	{
		uint32 uid = pView->GetUID();
		if ( uid == 100 ) {
			// Slider
			SliderPosChanged();
		} else if ( uid == 101 ) {
			// Max button
			m_pSlider->SetCurPos(m_pSlider->MaxPos());
			SliderPosChanged();
		} else if ( uid == 102 ) {
			// Trade button
			check(m_leftSel != -1 && m_rightSel != -1);
			iPlayer* pOwner = gGame.Map().FindPlayer(m_pCastle->Owner());
			check(pOwner);
			MINERAL_TYPE mfrom = m_leftButtons[m_leftSel]->MineralType();
			MINERAL_TYPE mto = m_rightButtons[m_rightSel]->MineralType();
			FractionCoeff nval = MineralExchRate(mfrom, mto, m_marketIdx).GetNormalized();
			pOwner->Minerals().quant[mfrom] -= nval.denum * m_pSlider->CurPos();
			pOwner->Minerals().quant[mto] += nval.num * m_pSlider->CurPos();
			SetCastle(m_pCastle);
		} else if ( uid >= 201 && uid <= 207 ) {
			// Left minerals
			if (m_leftSel != -1) m_leftButtons[m_leftSel]->SetState(false);
			m_leftSel = (uid-201);
			m_leftButtons[m_leftSel]->SetState(true);
			UpdateControls();
		} else if (uid >= 211 && uid <= 217) {
			// Right minerals
			if (m_rightSel != -1) m_rightButtons[m_rightSel]->SetState(false);
			m_rightSel = (uid-211);
			m_rightButtons[m_rightSel]->SetState(true);
			UpdateControls();
		}
	}

private:
	iCastle*			m_pCastle;
	sint32				m_marketIdx;
	iDlgIconButton*		m_pMaxBtn;
	iTextButton*		m_pTradeBtn;
	iPHScrollBar*		m_pSlider;
	sint32				m_leftSel;
	sint32				m_rightSel;
	iMineralBtn*		m_leftButtons[MINERAL_COUNT];
	iMineralBtn*		m_rightButtons[MINERAL_COUNT];
	iSize				m_cellSiz;
	IViewCmdHandler*	m_pCmdHandler;
};


//////////////////////////////////////////////////////////////////////////
class iMarketTab : public iCtlViewChild, public  IViewCmdHandler
{
public:
	iMarketTab(iViewMgr* pViewMgr)
	: iCtlViewChild(pViewMgr), m_pCastle(NULL)
	{
		AddChild( m_pMarketView = new iMarketView(pViewMgr, this, iRect(5,5,310,200), 100, iSize(44,40)) );
	}

	void OnActivate() 
	{
		SetCastle(m_pCastle);
	}

	void OnCompose()
	{
		iCtlViewChild::OnCompose();
		iRect rc = GetScrRect();
		rc.DeflateRect(5);
	}

	void SetCastle(iCastle* pCastle)
	{ 
		m_pCastle = pCastle;
		m_pMarketView->SetCastle(pCastle);
		Invalidate(); 
	}

private:
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
	{ 
		uint32 uid = pView->GetUID();
		if (uid == 110) {
			// SetCastle(m_pCastle);
		}
	}
	
private:
	iMarketView*	m_pMarketView;
	iCastle*		m_pCastle;
};

#endif //_HMM_GAME_CASTLE_VIEW_MARKETPLACE_PAGE_H_
