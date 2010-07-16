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

#ifndef _HMM_GAME_SPELL_LIST_VIEW_H_
#define _HMM_GAME_SPELL_LIST_VIEW_H_

class iSpellListBox;
class iSpellListView : public iView, public IViewCmdHandler
{
public:
	iSpellListView(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid, iHero* pOwner, bool bShowFavorites);

	void SetOwner(iHero* pOwner);
	inline iHero* Owner() { return m_pOwner; }
	inline MAGIC_SPELL SelSpell() const { return m_curSel; }
	inline bool CanCastSelSpell() const	{ return m_curSel != MSP_INVALID && m_pOwner->ManaPts() >= iBaseSpell::GetSpellCost(m_curSel, m_pOwner); }
	inline void SetMask(uint32 typeMask) { m_typeMask = typeMask; UpdateSpellList(); }
	void SelNext();
	void SelPrev();

private:
	void UpdateSpellList();
	void OnCompose();
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

private:
	iSpellListBox*		m_pSpellListBox;
	iSpellBtn*			m_pSpellButton;
	iBarTabSwitch*		m_pSchoolSwitch;
	uint32				m_typeMask;
	MAGIC_SPELL			m_curSel;
	iHero*				m_pOwner;
	IViewCmdHandler*	m_pCmdHandler;
};

#endif //_HMM_GAME_SPELL_LIST_VIEW_H_

