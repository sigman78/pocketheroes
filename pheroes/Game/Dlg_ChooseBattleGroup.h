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

#include ".\iBattleGroupBtn.h"

class iDlg_ChooseBattleGroup:	public iBaseGameDlg
{
public:
	iDlg_ChooseBattleGroup::iDlg_ChooseBattleGroup(iViewMgr* pViewMgr, iSimpleArray<iBattleGroup *> list, iSimpleArray<iStringT> texts, PLAYER_ID pid, uint32* idx);
	~iDlg_ChooseBattleGroup(void);		

private:
	void DoCompose(const iRect& rect);
	void iDlg_ChooseBattleGroup::OnMouseClick(const iPoint &pos) ;
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);
	void buildBtns();

	iSize ClientSize() const;
	
	void OnCreateDlg();

	iSimpleArray<iBattleGroup *> m_list;
	iSimpleArray<iStringT> m_texts;
	iPHScrollBar* m_pSlider;
	uint32 *m_idx;	

	
	iSimpleArray<iBattleGroupBtn*> m_buttons;
};
