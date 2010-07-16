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

#ifndef _HMM_GAME_MORALE_AND_LUCK_INFO_DIALOGS_H_
#define _HMM_GAME_MORALE_AND_LUCK_INFO_DIALOGS_H_

class iMLInfoDlg : public iBaseGameDlg
{
public:
	iMLInfoDlg(iViewMgr* pViewMgr, bool bLuck, sint8 val, PLAYER_ID pid);
	void OnCreateDlg();

private:
	void DoCompose(const iRect& clRect);
	iSize ClientSize() const;
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

private:
	iStringT	m_title;
	iStringT	m_text;
	bool		m_bLuck;
	sint8		m_val;
};

#endif //_HMM_GAME_MORALE_AND_LUCK_INFO_DIALOGS_H_

