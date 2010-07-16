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

#ifndef _HMM_GAME_CREATURE_INFORMATION_DIALOG_H_
#define _HMM_GAME_CREATURE_INFORMATION_DIALOG_H_

//////////////////////////////////////////////////////////////////////////
class iHighScore
{
public:
	struct iEntry {
		iEntry() {}
		iEntry(const iStringT& land_, uint32 date_, uint32 days_, uint32 score_) : land(land_), date(date_), days(days_), score(score_){}
		iStringT	land;
		uint32		date;
		uint32		days;
		uint32		score;
	};
	
public:
	void Reset();
	void Load(const iStringT& fname);
	void Save(const iStringT& fname) const;
	sint32 AddEntry(const iHighScore::iEntry& entry);

	inline uint32 Count() const { return m_entries.GetSize(); }
	inline const iEntry& Entry(uint32 idx) const { return m_entries[idx]; }

private:
	iSimpleArray<iEntry>	m_entries;
};
//////////////////////////////////////////////////////////////////////////
class iDlg_HallOfFame : public iBaseGameDlg
{
public:
	iDlg_HallOfFame(iViewMgr* pViewMgr, const iStringT& fname);
	iDlg_HallOfFame(iViewMgr* pViewMgr, const iStringT& fname, const iHighScore::iEntry& entry);

private:
	void OnCreateDlg();
	void DoCompose(const iRect& rect);
	iSize ClientSize() const;
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

private:
	sint32		m_curScore;
	iStringT	m_fname;
	iHighScore	m_hScore;
};

#endif //_HMM_GAME_CREATURE_INFORMATION_DIALOG_H_

