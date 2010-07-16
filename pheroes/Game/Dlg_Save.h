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

#ifndef _HMM_GAME_SAVE_DIALOG_H_
#define _HMM_GAME_SAVE_DIALOG_H_

const uint32 SAVE_GAME_SLOTS = 8;

typedef iSimpleArray<iMapInfo*> iSaveSlots;
//////////////////////////////////////////////////////////////////////////
class iSaveDlg : public iBaseGameDlg
{
public:
	iSaveDlg(iViewMgr* pViewMgr, bool bSave);
	~iSaveDlg();
	static void GetSaveFileName(iStringT& fname, uint32 slot);
	void SelFile(iStringT& fname) const;
	const iMapInfo& SelScenario() const;

private:
	void OnCreateDlg();
	void DoCompose(const iRect& rect);
	iSize ClientSize() const;
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);
	inline bool CanSelect() const { return m_selSlot != -1 && (m_bSave || m_saveSlots[m_selSlot] != NULL); }
	bool Confirmed() const;

private:
	bool		m_bSave;
	sint32		m_selSlot;
	iSaveSlots	m_saveSlots;
};

#endif //_HMM_GAME_SAVE_DIALOG_H_


