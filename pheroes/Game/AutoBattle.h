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

#ifndef _HMM_GAME_AUTO_BATTLE_H_
#define _HMM_GAME_AUTO_BATTLE_H_

class iAutoBattle : public iBattleWrapper
{
public:
	iAutoBattle(bool bShowResult = false);
	void OnBeginBattle();
	void OnStart();
	void OnEndBattle();
	void AddLogEvent(const iStringT& msg);
	void AddCellEvent(const iStringT& msg, const iPoint& pos);

private:
	bool	m_bShowResult;
};

#endif //_HMM_GAME_AUTO_BATTLE_H_

