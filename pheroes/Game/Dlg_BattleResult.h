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

#ifndef _HMM_GAME_BATTLE_RESULT_DIALOG_H_
#define _HMM_GAME_BATTLE_RESULT_DIALOG_H_

class iDlg_BattleResult : public iBaseGameDlg
{
public:
	iDlg_BattleResult(iViewMgr* pViewMgr, iBattleEngine& engine, iBattleResult br);

private:
	void OnCreateDlg();
	void DoCompose(const iRect& rect);
	iSize ClientSize() const;
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

	class iCasualtList 
	{
	public:
		void Add(CREATURE_TYPE ct, sint32 cnt)
		{
			for (uint32 xx=0; xx<casualties.GetSize(); ++xx) {
				if (casualties[xx].Type() == ct) {
					casualties[xx].Count() += cnt;
					return;
				}
			}
			casualties.Add(iCreatGroup(ct,cnt));
		}

		inline const iCreatGroup& operator[](uint32 idx) const { return casualties[idx]; }
		inline uint32 Count() const { return casualties.GetSize(); }

	private:
		iSimpleArray<iCreatGroup>	casualties;
	};

private:
	iCasualtList			m_aCas;
	iCasualtList			m_dCas;
	iStringT				m_Title;
	iStringT				m_Text;
	const iBattleEngine&	m_engine;
	iBattleResult			m_br;
};

#endif //_HMM_GAME_BATTLE_RESULT_DIALOG_H_

