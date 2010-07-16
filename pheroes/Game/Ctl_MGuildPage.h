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

#ifndef _HMM_GAME_CASTLE_VIEW_MAGE_GUILD_PAGE_H_
#define _HMM_GAME_CASTLE_VIEW_MAGE_GUILD_PAGE_H_

class iMGuildTab : public iCtlViewChild, public  IViewCmdHandler
{
public:
	iMGuildTab(iViewMgr* pViewMgr)
	: iCtlViewChild(pViewMgr), m_pCastle(NULL)
	{
		sint32 cellW = 61;
		sint32 cellH = 36;

		uint32 xx=0;
		for (uint32 lvl = 0; lvl<5; ++lvl){
			uint32 scnt = 5-lvl;
			iPoint pos(160 - (cellW*scnt+3*(scnt-1))/2, 1 + (5-lvl-1)*cellH);
			for (uint32 spid=0; spid<scnt; ++spid) {
				AddChild(m_pSpellBtn[xx] = new iSpellBtn(pViewMgr,this,iRect(pos.x,pos.y,cellW,cellH),110 + xx));
				++xx;
				pos.x += cellW+3;
			}
		}
	}

	void OnActivate() 
	{
		SetCastle(m_pCastle);
	}

	void OnCompose()
	{
		iCtlViewChild::OnCompose();
	}

	void SetCastle(iCastle* pCastle)
	{ 
		m_pCastle = pCastle;
		if (m_pCastle) {
			uint32 xx=0;
			for (uint32 lvl=0; lvl<5; ++lvl) {
				iCtlCnst* pCnst = m_pCastle->FindCnst((CTL_CONSTR)(CTLCNST_MAGEGUILD_L1+lvl));
				uint32 tcnt = (5-lvl);
				if (pCnst && pCnst->IsBuilt()) {
					iMGuildCtlCnst* pMGuild = DynamicCast<iMGuildCtlCnst*>(pCnst);
					check(pMGuild);
					uint32 spid;
					for (spid=0; spid < pMGuild->SpellsCount(); ++spid, ++xx) m_pSpellBtn[xx]->SetSpell(gGame.ItemMgr().m_spellMgr.Spell(pMGuild->Spell(spid)));
					for (uint32 fsp=spid; fsp<tcnt; ++fsp, ++xx) m_pSpellBtn[xx]->SetSpell(NULL);
				} else {
					for (uint32 fsp=0; fsp<tcnt; ++fsp, ++xx) m_pSpellBtn[xx]->SetSpell(NULL);
				}
			}
		} else {
			for (uint32 xx=0; xx<15; ++xx) m_pSpellBtn[xx]->SetSpell(NULL);
		}
		
		Invalidate(); 
	}

private:
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
	{
		uint32 uid = pView->GetUID();
		if (uid >= 110 && uid < 125) {
			iIconDlg dlg(&gApp.ViewMgr(), gTextMgr[m_pSpellBtn[uid-110]->GetSpell()->NameKey()], gTextMgr[m_pSpellBtn[uid-110]->GetSpell()->DescKey(MSL_NONE)], m_pSpellBtn[uid-110]->GetSpell()->Icon(),m_pCastle->Owner());
			dlg.DoModal();
		}
	}
	
private:
	iCastle*	m_pCastle;
	iSpellBtn*	m_pSpellBtn[15];
};

#endif //_HMM_GAME_CASTLE_VIEW_MAGE_GUILD_PAGE_H_
