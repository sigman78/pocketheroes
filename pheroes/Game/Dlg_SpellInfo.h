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

#ifndef HMM_GAME_SPELL_INFO_DIALOG_H_
#define HMM_GAME_SPELL_INFO_DIALOG_H_

class iSpellInfoDlg : public iTextDlg
{
public:
	iSpellInfoDlg(iViewMgr* pViewMgr, MAGIC_SPELL spell, MAGIC_SCHOOL_LEVEL level, PLAYER_ID pid);

protected:
	void DoCompose(const iRect& clRect);
	iSize ClientSize() const;

protected:
	MAGIC_SPELL			m_spell;
};

#endif //HMM_GAME_SPELL_INFO_DIALOG_H_

