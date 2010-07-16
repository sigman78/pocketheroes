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
#include "Dlg_SpellInfo.h"

iSpellInfoDlg::iSpellInfoDlg(iViewMgr* pViewMgr, MAGIC_SPELL spell, MAGIC_SCHOOL_LEVEL level, PLAYER_ID pid)
: iTextDlg(pViewMgr, gTextMgr[gGame.ItemMgr().m_spellMgr.Spell(spell)->NameKey()], gTextMgr[gGame.ItemMgr().m_spellMgr.Spell(spell)->DescKey(level)], pid), m_spell(spell)
{}

void iSpellInfoDlg::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);
	iTextDlg::DoCompose(clRect);
	iPoint op(rc.x + (rc.w/2 - 30), rc.y2() - 15 - 10 - 36);
	gGfxMgr.Blit(PDGG_SPELLSCROLL_UNROLLED,gApp.Surface(),op);
	op.Move(15,7);
	gGfxMgr.Blit(gGame.ItemMgr().m_spellMgr.Spell(m_spell)->Icon(),gApp.Surface(),op);
}

iSize iSpellInfoDlg::ClientSize() const
{
	iSize res = iTextDlg::ClientSize();
	res.h += 46;
	return res;
}


