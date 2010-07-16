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
#include ".\iBattleGroupBtn.h"
#include "dlg_choosebattlegroup.h"

iDlg_ChooseBattleGroup::iDlg_ChooseBattleGroup(iViewMgr* pViewMgr, iSimpleArray<iBattleGroup *> list, iSimpleArray<iStringT> texts, PLAYER_ID pid, uint32* idx)
: iBaseGameDlg(pViewMgr, pid), m_list(list), m_idx(idx), m_texts(texts)
{
}

iDlg_ChooseBattleGroup::~iDlg_ChooseBattleGroup(void)
{

}

void iDlg_ChooseBattleGroup::buildBtns() 
{
	iRect clrc = ClientRect();	
	for(uint32 x=0; x<m_list.GetSize(); x++) {
		if(x < m_pSlider->CurPos())
			m_buttons[x]->SetVisible(false);
		else if(x < m_pSlider->CurPos() + 4) {
			uint32 xx = x - m_pSlider->CurPos();
			m_buttons[x]->SetPos(iPoint(clrc.x + (clrc.w/2-65) + xx * 30, clrc.y2()-20-10-15 - 35));
			m_buttons[x]->SetVisible(true);
		} else
			m_buttons[x]->SetVisible(false);		
	}
}

void iDlg_ChooseBattleGroup::OnCreateDlg()
{
	iRect clrc = ClientRect();
	// Slider
	AddChild(m_pSlider = new iPHScrollBar(m_pMgr, this, iRect(clrc.x + (clrc.w/2-65), clrc.y2()-20-10-15,130,15), 101, iScrollBar::Horizontal));	
	m_pSlider->SetMetrics(m_list.GetSize(), 1);
	
	m_pSlider->SetCurPos(0);

	for (uint32 xx=0; xx<m_list.GetSize(); xx++) {	
		iBattleGroupBtn *btn = new iBattleGroupBtn(m_pMgr, this, iPoint(clrc.x + (clrc.w/2-65) + xx * 30, clrc.y2()-20-10-15 - 35), 0, m_list[xx], m_texts[xx]);
		m_buttons.Add(btn);
		AddChild(m_buttons[xx]);
	}	

	buildBtns();

	AddChild(new iTextButton(m_pMgr, this, iRect((clrc.x+clrc.w/2)-40, clrc.y2()-DEF_BTN_HEIGHT, 40, DEF_BTN_HEIGHT), TRID_OK, DRC_OK));	
	AddChild(new iTextButton(m_pMgr, this, iRect((clrc.x+clrc.w/2), clrc.y2()-DEF_BTN_HEIGHT, 40, DEF_BTN_HEIGHT), TRID_CANCEL, DRC_CANCEL));	
}

void iDlg_ChooseBattleGroup::OnMouseClick(const iPoint &pos) 
{
	for(uint32 xx=0; xx<m_buttons.GetSize(); xx++)
		m_buttons[xx]->Deselect();
}

void iDlg_ChooseBattleGroup::DoCompose(const iRect& rect)
{			
}

iSize iDlg_ChooseBattleGroup::ClientSize() const
{
	return iSize(200, 65 + DEF_BTN_HEIGHT); 
}

void iDlg_ChooseBattleGroup::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();
	if (cmd == CCI_BTNCLICK) {
		sint32 selected = -1;
		for(uint32 xx=0; xx<m_buttons.GetSize(); xx++)
			if(m_buttons[xx]->Selected()) {
				selected = xx;
				break;
			}
		if(selected != -1) {
			*m_idx = selected;
			EndDialog(pView->GetUID());
		}
	} else if(uid == 101) {
		buildBtns();
	}
}

