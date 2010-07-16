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

#ifndef _HMM_GAME_CHEST_DIALOG_H_
#define _HMM_GAME_CHEST_DIALOG_H_

/*
 *	
 */
class iDlg_Chest : public iBaseGameDlg
{
public:
	enum Selection {
		Gold = 100,
		Experience
	};
public:
	iDlg_Chest(iViewMgr* pViewMgr, iHero* pHero, iMapItem_Chest* pChest);

	inline iHero* Hero() { return m_pHero; }
	inline sint32 GoldCount() const	{ return m_cntGold; }
	inline sint32 ExpCount() const { return m_cntExp; }

private:
	void OnCreateDlg();
	void DoCompose(const iRect& rect);
	iSize ClientSize() const;
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

private:
	iHero*			m_pHero;
	sint32			m_cntExp;
	sint32			m_cntGold;
};


#endif //_HMM_GAME_CHEST_DIALOG_H_

