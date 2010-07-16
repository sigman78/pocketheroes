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

#ifdef _DEEP_DEBUG_
#	define new DEBUG_NEW
#endif 


#include "BattleView.h"
#include "BattleAI.h"
#include "Dlg_BattleResult.h"
#include "Dlg_BattleHero.h"
#include "Dlg_SurrenderCond.h"
#include "Dlg_TownList.h"
#include "Dlg_BattleLog.h"
#include "Dlg_CastSpell.h"
#include "Dlg_Settings.h"
#include "Dlg_ChooseBattleGroup.h"

const fix32 CELL_EVT_DELAY = fix32(2.0f);

enum BATTLE_CONTROLS_IDS {
	BCI_DEFAULT = 1024,
	BCI_ASSAULTER_BTN,
	BCI_DEFENDER_BTN,
	BCI_MODE_SWITCH,
	BCI_WAIT_BTN,
	BCI_DEFEND_BTN,
	BCI_MSGLOG_BTN,
	BCI_AUTOBATTLE_BTN,
	BCI_CASTSPELL_BTN,
	BCI_SETTINGS_BTN
};

const iDib::pixel SKY_GRAD_COLORS[STYPE_COUNT][2] = {
	{RGB16(255, 255, 255), RGB16(64, 192, 255) },	// Water
	{RGB16(255, 255, 255), RGB16(64, 192, 255) },	// Sand
	{RGB16(255, 255, 255), RGB16(64, 192, 255) },	// Dirt
	{RGB16(255, 255, 255), RGB16(64, 192, 255) },	// Grass
	{RGB16(255, 255, 255), RGB16(64, 192, 255) },	// Swamp
	{RGB16(16, 0, 0), RGB16(255, 0, 0) },			// Lava
	{RGB16(255, 255, 255), RGB16(64, 192, 255) },	// Wasteland
	{RGB16(255, 255, 255), RGB16(64, 192, 255) },	// Desert
	{RGB16(255, 255, 255), RGB16(64, 192, 255) },	// Snow
	{RGB16(255, 255, 255), RGB16(64, 192, 255) },	// New Desert
	{RGB16(255, 255, 255), RGB16(64, 192, 255) },	// Pavement
	{RGB16(255, 255, 255), RGB16(64, 192, 255) }	// New Wasteland
};

const iDib::pixel TOOLTIP_COLORS[STYPE_COUNT][2] = {
	{RGB16(32, 32, 96), RGB16(220, 220, 255) },		// Water
	{RGB16(32, 32, 96), RGB16(220, 220, 255) },		// Sand
	{RGB16(32, 32, 96), RGB16(220, 220, 255) },		// Dirt
	{RGB16(32, 32, 96), RGB16(220, 220, 255) },		// Grass
	{RGB16(32, 32, 96), RGB16(220, 220, 255) },		// Swamp
	{RGB16(16, 0, 0), RGB16(255, 128, 128) },		// Lava
	{RGB16(0, 64, 128), RGB16(255, 255, 255)},		// Wasteland
	{RGB16(32, 32, 96), RGB16(220, 220, 255) },		// Desert
	{RGB16(32, 32, 96), RGB16(220, 220, 255) },		// Snow
	{RGB16(32, 32, 96), RGB16(220, 220, 255) },		// New Desert
	{RGB16(32, 32, 96), RGB16(220, 220, 255) },		// Pavement
	{RGB16(0, 64, 128), RGB16(255, 255, 255)}		// New Wasteland
};

struct iCtlDecDesc {
	SpriteId	sid;
	bool		bGround;
};

// Fort decorations [iCastleFort::ElementType][iCastleFort::ElementState]
const iCtlDecDesc FORT_DECS[iCastleFort::TypeCount][iCastleFort::StateCount] = {
	{
		{PDGG_COMBAT_UTOWER, false}, {PDGG_COMBAT_UTURRET, false}, {PDGG_COMBAT_UTOWER + 1, false}, {PDGG_COMBAT_UTOWER + 2, false}
	},{
		{PDGG_COMBAT_UWALL, false}, {PDGG_COMBAT_UWALL, false}, {PDGG_COMBAT_UWALL + 1, false}, {PDGG_COMBAT_UWALL + 2, false}
	},{
		{PDGG_COMBAT_UTOWER, false}, {PDGG_COMBAT_UTOWER , false}, {PDGG_COMBAT_UTOWER + 1, false}, {PDGG_COMBAT_UTOWER + 2, false}
	},{
		{PDGG_COMBAT_UWALL, false}, {PDGG_COMBAT_UWALL, false}, {PDGG_COMBAT_UWALL + 1, false}, {PDGG_COMBAT_UWALL + 2, false}
	},{
		{PDGG_COMBAT_GUTOWER, false}, {PDGG_COMBAT_GUTOWER, false}, {PDGG_COMBAT_GUTOWER, false}, {PDGG_COMBAT_GUTOWER, false}
	},{
		{PDGG_COMBAT_BRIDGE, false}, {PDGG_COMBAT_BRIDGE + 1, true}, {PDGG_COMBAT_BRIDGE, true}, {PDGG_COMBAT_BRIDGE + 2, true}
	},{
		{PDGG_COMBAT_UTOWER, false}, {PDGG_COMBAT_UTURRET, false}, {PDGG_COMBAT_UTOWER + 1, false}, {PDGG_COMBAT_UTOWER + 2, false}
	},{
		{PDGG_COMBAT_GBTOWER, false}, {PDGG_COMBAT_GBTOWER, false}, {PDGG_COMBAT_GBTOWER, false}, {PDGG_COMBAT_GBTOWER, false}
	},{
		{PDGG_COMBAT_BWALL, false}, {PDGG_COMBAT_BWALL, false}, {PDGG_COMBAT_BWALL + 1, false}, {PDGG_COMBAT_BWALL + 2, false}
	},{
		{PDGG_COMBAT_BTOWER, false}, {PDGG_COMBAT_BTOWER, false}, {PDGG_COMBAT_BTOWER + 1, false}, {PDGG_COMBAT_BTOWER + 2,false}
	},{
		{PDGG_COMBAT_BWALL, false}, {PDGG_COMBAT_BWALL, false}, {PDGG_COMBAT_BWALL + 1, false}, {PDGG_COMBAT_BWALL + 2,false}
	},{
		{PDGG_COMBAT_BTOWER, false}, {PDGG_COMBAT_BTURRET, false}, {PDGG_COMBAT_BTOWER + 1, false}, {PDGG_COMBAT_BTOWER + 2, false}
	}
};


/*
 *	Helpers and constants
 *  TODO:: PRE: Hides declaration - need to rename const
 */
const iPoint anchor(13,31);//(35,45);

inline iPoint Map2Screen(const iPoint& pos)
{ return iPoint(anchor.x+pos.x*24 - (pos.y&1)*13 ,anchor.y + pos.y*17); }

inline iPoint Screen2Map(const iPoint& pos)
{ 
	sint32 y = (pos.y - anchor.y)/17;
	sint32 x = (pos.x - anchor.x + (y&1)*13)/24;
	return iPoint(x,y);
}

const fix32 ACT_STEP = fix32(0.4f);
const iRect BATTLE_POPUP_BOUND(2,2,316,216);
/*
 *	Creature information popup view
 */
iCreatInfoPopup::iCreatInfoPopup(iViewMgr* pViewMgr, PLAYER_ID pid, const iBattleGroup* pGroup)
: iFramePopupView(pViewMgr, pid), m_pGroup(pGroup)
{
}

void iCreatInfoPopup::DoCompose(const iRect& clRect)
{
	iRect rc(clRect);
	CREATURE_TYPE ct = m_pGroup->Type();
	sint32 cnt = m_pGroup->Count();


	// title
	rc.y += gTextComposer.TextBoxOut(dlgfc_topic, gApp.Surface(),gTextMgr[ct*3+TRID_CREATURE_PEASANT_F2], iRect(rc.x,rc.y,rc.w,15));

	// icon
	BlitIcon(gApp.Surface(),PDGG_MINIMON+ct,iRect(rc.x,rc.y,rc.w,45));
	rc.y+=45;

	// Perks
	iSimpleArray<uint16>	perks;
	for (uint32 nn=0; nn<CPERKS_COUNT; ++nn) {
		if (CREAT_DESC[ct].perks & (1<<nn)) perks.Add(nn);
	}

	sint32 tw = perks.GetSize() * 15 - 1;
	sint32 ox = rc.x + ((sint32)(rc.w/2) - tw/2);
	for (uint32 nn=0; nn<perks.GetSize(); ++nn) {
		gGfxMgr.Blit(PDGG_CREAT_PERKS + perks[nn], gApp.Surface(), iPoint(ox, rc.y));
		ox += 15;
	}
	rc.y += 15;
	// specs
	const sint32 lh = 10;
	iRect trc(rc.x,rc.y,38,lh);
	iTextComposer::FontConfig fc(iTextComposer::FS_SMALL, RGB16(192,192,220));
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iStringT(gTextMgr[TRID_SKILL_QUANT_S])+_T(" :"),trc,AlignTopRight); trc.y+=lh;
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iStringT(gTextMgr[TRID_SKILL_ATTACK_S])+_T(" :"),trc,AlignTopRight); trc.y+=lh;
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iStringT(gTextMgr[TRID_SKILL_DEFENCE_S])+_T(" :"),trc,AlignTopRight); trc.y+=lh;
	if (m_pGroup->CanShoot()) { gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iStringT(gTextMgr[TRID_SKILL_SHOTS_S])+_T(" :"),trc,AlignTopRight); trc.y+=lh; }
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iStringT(gTextMgr[TRID_SKILL_DAMAGE_S])+_T(" :"),trc,AlignTopRight); trc.y+=lh;
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iStringT(gTextMgr[TRID_SKILL_HEALTH_S])+_T(" :"),trc,AlignTopRight); trc.y+=lh;
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iStringT(gTextMgr[TRID_SKILL_SPEED_S])+_T(" :"),trc,AlignTopRight); trc.y+=lh;
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iStringT(gTextMgr[TRID_SKILL_MORALE_S])+_T(" :"),trc,AlignTopRight); trc.y+=lh;
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iStringT(gTextMgr[TRID_SKILL_LUCK_S])+_T(" :"),trc,AlignTopRight); trc.y+=lh;

	fc.cmpProps.faceColor = RGB16(220,220,160);
	trc = iRect(rc.x+40,rc.y,rc.w-40,lh);
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),FormatNumber(cnt),trc,AlignTopLeft); trc.y+=lh;
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iFormat(_T("%d (%d)"),CREAT_DESC[ct].attack,m_pGroup->AttackSkill()),trc,AlignTopLeft); trc.y+=lh;
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iFormat(_T("%d (%d)"),CREAT_DESC[ct].defence,m_pGroup->DefenceSkill()),trc,AlignTopLeft); trc.y+=lh;
	if (m_pGroup->CanShoot()) { gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iFormat(_T("%d/%d"),m_pGroup->Shots(),CREAT_DESC[ct].shots),trc,AlignTopLeft); trc.y+=lh; }
	if (m_pGroup->MinDamage() == m_pGroup->MaxDamage()) gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),FormatNumber(m_pGroup->MinDamage()),trc,AlignTopLeft);
	else gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iFormat(_T("%d-%d"),m_pGroup->MinDamage(),m_pGroup->MaxDamage()),trc,AlignTopLeft);
	trc.y+=lh;
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iFormat(_T("%d/%d"),m_pGroup->HitPointsLeft(),m_pGroup->HitPoints()),trc,AlignTopLeft); trc.y+=lh;
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),iFormat(_T("%d (%d)"),CREAT_DESC[ct].speed,m_pGroup->Speed()),trc,AlignTopLeft); trc.y+=lh;
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),FormatNumber(m_pGroup->Morale()),trc,AlignTopLeft); trc.y+=lh;
	gTextComposer.TextOut(fc, gApp.Surface(),trc.point(),FormatNumber(m_pGroup->Luck()),trc,AlignTopLeft); trc.y+=lh;

	// spells
	if (m_pGroup->SpellList().GetSize()) {
		fc.cmpProps.faceColor = RGB16(192,192,192);
		rc.y = trc.y + 10;
		uint32 cnt = iMIN<uint32>(5,m_pGroup->SpellList().GetSize());
		for (uint32 xx=0; xx<cnt; ++xx) {
			if (m_pGroup->SpellList()[xx].m_duration == iBattleGroup::iSpellEntry::DurInfinit) gTextComposer.TextOut(fc, gApp.Surface(),rc.point(),iFormat(_T("%s (-)"), gTextMgr[m_pGroup->SpellList()[xx].m_pSpell->SpellLabel()]),rc,AlignTop);
			else gTextComposer.TextOut(fc, gApp.Surface(),rc.point(),iFormat(_T("%s (%d)"), gTextMgr[m_pGroup->SpellList()[xx].m_pSpell->SpellLabel()], m_pGroup->SpellList()[xx].m_duration),rc,AlignTop);
			rc.y+=10;
		}
	}

}

iSize iCreatInfoPopup::ClientSize()
{
	sint32 w = 80;
	sint32 h = 0;

	// title
	h += gTextComposer.GetTextBoxSize(gTextMgr[m_pGroup->Type()*3+TRID_CREATURE_PEASANT_F2], 80, dlgfc_topic).h;

	// icon
	h += 45;

	// perks
	h += 15;

	// specs
	h += 8 * 10;
	if (m_pGroup->CanShoot()) h += 10;

	// spells
	if (m_pGroup->SpellList().GetSize()) {
		h += 10 + iMIN<sint32>(5, m_pGroup->SpellList().GetSize()) * 10;
	}

	// 
	h += 5;
	return iSize(w,h);
}

/*
 *	Battle composer
 */
struct iBatCmpItem {
	iBatCmpItem(const iPoint& pos, bool bCurrent) : m_pos(pos), m_bCurrent(bCurrent) {}

	virtual void ComposeShadow(iDib& surf, const iPoint& anchor) const  =0;
	virtual void Compose(iDib& surf, const iPoint& anchor, bool bInt) const  =0;

	iPoint	m_pos;
	bool	m_bCurrent;
};
//
struct iBatCmpItem_Creature : public iBatCmpItem {
	iBatCmpItem_Creature(const iBattleGroup* pGroup, bool bCurrent) : iBatCmpItem(pGroup->Pos(), bCurrent), m_pGroup(pGroup) {}

	void ComposeShadow(iDib& surf, const iPoint& anchor) const
	{
		iPoint cop = anchor;
		sint16 ct = m_pGroup->Type();

		if (CREAT_DESC[ct].size == 1) cop.x-=11;
		else if(CREAT_DESC[ct].size == 2) {
			if (m_pGroup->Orient() == iBattleGroup::Right) cop.x-=22;
		}

		if (m_pGroup->IsAlive()) {
			uint16 sid  = (m_pGroup->Orient() == (iBattleGroup::Right)?PDGG_MINIMON:PDGG_RMINIMON) + ct;
			if (m_pGroup->State()==iBattleGroup::Melee || m_pGroup->State()==iBattleGroup::Shooting) sid += 45;
			gGfxMgr.BlitEffect(sid, surf, cop,iGfxManager::EfxShadowIso);
		} else {
			uint16 sid  = (m_pGroup->Orient() == (iBattleGroup::Right)?PDGG_MINIMON:PDGG_RMINIMON) + ct + 135;
			gGfxMgr.Blit(sid, surf, cop);
		}
	}

	void Compose(iDib& surf, const iPoint& anchor, bool bInt) const
	{
		sint16 ct = m_pGroup->Type();
		iPoint cop = anchor;

		if (!m_pGroup->IsAlive()) return;

		if (CREAT_DESC[ct].size == 1) cop.x-=11;
		else if(CREAT_DESC[ct].size == 2) {
			if (m_pGroup->Orient() == iBattleGroup::Right) cop.x-=22;
		}

		// Draw group
		uint16 sid  = (m_pGroup->Orient() == (iBattleGroup::Right)?PDGG_MINIMON:PDGG_RMINIMON) + ct;
		if (m_pGroup->State() == iBattleGroup::Melee || m_pGroup->State()==iBattleGroup::Shooting) sid += 45;
		else if (m_pGroup->State() == iBattleGroup::RecDamage) sid += 90;
		gGfxMgr.Blit(sid, surf, cop);

		// Draw current group edging
		if (bInt && m_bCurrent) {
			uint16 frameSid  = (m_pGroup->Orient() == (iBattleGroup::Right)?PDGG_MINIMONF:PDGG_RMINIMONF) + m_pGroup->Type();
			gGfxMgr.Blit(frameSid, surf, cop);
		}

		// Draw group's quanity
		iPoint p = anchor;
		iRect rc(p.x+1,p.y+9,20,11);
		SPELL_DISPOS sd = m_pGroup->SpellsDispos();
		if (sd == SPD_NEUTRAL) surf.VGradientRect(rc,RGB16(140,140,46),RGB16(58,58,0));
		else if (sd == SPD_POSITIVE) surf.VGradientRect(rc,RGB16(56,160,22),RGB16(32,64,16));
		else if (sd == SPD_NEGATIVE) surf.VGradientRect(rc,RGB16(192,76,32),RGB16(80,32,16));
		else surf.VGradientRect(rc,RGB16(120,76,160),RGB16(44,8,80));
		
		//if (composeList[xx].value->PassMap().GetAt(composeList[xx].value->Pos().x+TAIL_OFFSET[!composeList[xx].value->Orient()], composeList[xx].value->Pos().y) == CT_PASSABLE) rc.x += TAIL_OFFSET[!composeList[xx].value->Orient()] * 20;
		/*if (m_pGroup->InMoat()) surf.VGradientRect(rc,RGB16(192,76,32),RGB16(80,32,16));
		else */
		iTextComposer::FontConfig fc(iTextComposer::FS_MEDIUM, iDibFont::ComposeProps(RGB16(255,255,192), RGB16(48,48,64)));
		surf.FrameRect(rc,RGB16(208,176,28));
		gTextComposer.TextOut(fc, surf, rc, FormatShortNumber(m_pGroup->Count()), rc, AlignCenter);

		// receive damage glyph
		if (m_pGroup->State() == iBattleGroup::RecDamage) gGfxMgr.Blit(PDGG_BTL_RECDMG, surf, p);
		else if (m_pGroup->State() == iBattleGroup::ResistSpell) gGfxMgr.Blit(PDGG_BTL_RESIST_SPELL, surf, p);
	}

	const iBattleGroup*	m_pGroup;
};
//
struct iBatCmpItem_Obstacle : public iBatCmpItem {
	iBatCmpItem_Obstacle(const iBatObstEntry* pObstacle) : iBatCmpItem(pObstacle->cell, false), m_pObstacle(pObstacle) {}

	void ComposeShadow(iDib& surf, const iPoint& anchor) const
	{
		iPoint cop = anchor + gGame.ItemMgr().BatObstMgr().Item(m_pObstacle->obstId).offset;
		SpriteId sid = gGame.ItemMgr().BatObstMgr().Item(m_pObstacle->obstId).sprite;
		gGfxMgr.BlitEffect(sid, surf, cop,iGfxManager::EfxShadowIso);
	}

	void Compose(iDib& surf, const iPoint& anchor, bool bInt) const
	{
		iPoint cop = anchor + gGame.ItemMgr().BatObstMgr().Item(m_pObstacle->obstId).offset;
		SpriteId sid = gGame.ItemMgr().BatObstMgr().Item(m_pObstacle->obstId).sprite;
		gGfxMgr.Blit(sid, surf, cop);
	}

	const iBatObstEntry*	m_pObstacle;
};
//
struct iBatCmpItem_FortElement : public iBatCmpItem {
	iBatCmpItem_FortElement(const iCastleFort::iElement* pElement) : iBatCmpItem(iPoint(iCastleFort::ElCells[pElement->type][0], iCastleFort::ElCells[pElement->type][1]), false), m_pElement(pElement) {}

	void ComposeShadow(iDib& surf, const iPoint& anchor) const
	{
		if (FORT_DECS[m_pElement->type][m_pElement->state].bGround) {
			SpriteId sid = FORT_DECS[m_pElement->type][m_pElement->state].sid;
			gGfxMgr.Blit(sid, surf, anchor);
		}
	}

	void Compose(iDib& surf, const iPoint& anchor, bool bInt) const
	{
		if (!FORT_DECS[m_pElement->type][m_pElement->state].bGround) {
			SpriteId sid = FORT_DECS[m_pElement->type][m_pElement->state].sid;
			gGfxMgr.Blit(sid, surf, anchor);
		}
		if (m_pElement->bRecDmg) gGfxMgr.Blit(PDGG_BTL_RECDMG, surf, anchor);

		/*
		iPoint cop = anchor + m_offset;
		gGfxMgr.Blit(m_sprite, surf, cop);
		*/
	}

	const iCastleFort::iElement*	m_pElement;
};

//
struct iBatCmpItem_Sprite : public iBatCmpItem {
	iBatCmpItem_Sprite(const iPoint& pos, SpriteId sid, const iPoint& offset) : iBatCmpItem(pos, false), m_sprite(sid), m_offset(offset) {}

	void ComposeShadow(iDib& surf, const iPoint& anchor) const
	{
	}

	void Compose(iDib& surf, const iPoint& anchor, bool bInt) const
	{
		iPoint cop = anchor + m_offset;
		gGfxMgr.Blit(m_sprite, surf, cop);
	}

	SpriteId	m_sprite;
	iPoint		m_offset;
};


/*
 *	Battle View
 */
iBattleView::iBattleView()
: iChildGameView(false, CV_OVERLAND)
, m_trackCell(cInvalidPoint), m_trackPos(cInvalidPoint)
, m_pBattle(NULL), m_battleMode(BNM_MELEE), m_bAni(false)
, m_pMeleeTrack(NULL), m_pShootTrack(NULL), m_pAutoBattleToolBar(NULL)
, m_pCreatInfoPopup(NULL), m_pCastSpellToolBar(NULL), m_bForceInfo(false), m_bHumanTurn(false)
{
	AddChild(m_pToolBar = new iBattleToolBar(&gApp.ViewMgr(), this, iRect(0, m_Rect.y2()-20,m_Rect.w, 21)));
}

iBattleView::~iBattleView()
{ }

void iBattleView::BeginBattle(iBattleWrapper* pBattle, SURF_TYPE st)
{
	m_pBattle = pBattle;
	m_surfType = st;
	iBattleMember_Hero* pAssaulter = DynamicCast<iBattleMember_Hero*>(m_pBattle->Engine().GetBattleInfo().m_pAssaulter);
	check(pAssaulter);
	m_pToolBar->m_pBtnAssaulter->SetHero(pAssaulter->GetHero());
	if (iBattleMember_Hero* pHero = DynamicCast<iBattleMember_Hero*>(m_pBattle->Engine().GetBattleInfo().m_pDefender)){
		m_pToolBar->m_pBtnDefender->SetHero(pHero->GetHero());
	} else if (iBattleMember_Castle* pCastle = DynamicCast<iBattleMember_Castle*>(m_pBattle->Engine().GetBattleInfo().m_pDefender)){
		if (pCastle->GetVisitor()) m_pToolBar->m_pBtnDefender->SetHero(pCastle->GetVisitor());
	}
	PrepareSurface();

	// Prepare and distribute obstacles
	if (!pBattle->Engine().CastleFort()) {
		iSimpleArray<uint16> olist;
		gGame.ItemMgr().BatObstMgr().SelectItems(olist, st);
		uint32 max_count = 5;
		while (max_count-- && olist.GetSize()) {
			uint32 rval = gGame.Map().Rand(olist.GetSize());
			uint8 size = gGame.ItemMgr().BatObstMgr().Item(olist[rval]).size;
			iPoint op;
			do {
				op.x = 3+gGame.Map().Rand(7);
				op.y = 1+gGame.Map().Rand(9);
			} while (pBattle->Engine().ObstaclesMap().GetAt(op.x, op.y) == CT_OBSTACLE || (size == 2 && pBattle->Engine().ObstaclesMap().GetAt(op.x+1, op.y) == CT_OBSTACLE));
			m_obstacles.Add(iBatObstEntry(olist[rval], op));
			pBattle->Engine().ObstaclesMap().GetAt(op.x, op.y) = CT_OBSTACLE;
			if (size == 2) pBattle->Engine().ObstaclesMap().GetAt(op.x+1, op.y) = CT_OBSTACLE;
			olist.RemoveAt(rval);
		}
	}
}

void iBattleView::Start()
{
	gSfxMgr.PlaySound(CSND_PREBATTLE);
	OnGroupChanged();
}

void iBattleView::EndBattle(iBattleResult br)
{
	// Prepare battle result
	m_pBattle->Engine().PrepareBattleResult(br);

	// Show result
	iDlg_BattleResult brd(m_pMgr, m_pBattle->Engine(), br);
	brd.DoModal();

	// End battle
	m_pBattle->EndBattle(br);
}

void iBattleView::BeginAutobattle()
{
	check(!m_pAutoBattleToolBar);
	m_pToolBar->SetVisible(false);
	AddChild(m_pAutoBattleToolBar = new iAutoBattleToolBar(m_pMgr, this, iRect(0, m_Rect.y2()-20,m_Rect.w, 21)));
	m_pBattle->Engine().SetAutobattle(true);
	OnGroupChanged();
}

void iBattleView::EndAutobattle()
{
	check(m_pAutoBattleToolBar);
	RemoveChild(m_pAutoBattleToolBar);
	delete m_pAutoBattleToolBar;
	m_pAutoBattleToolBar = NULL;
	m_pToolBar->SetVisible(true);
	m_pBattle->Engine().SetAutobattle(false);
}

bool iBattleView::BeginSpellTrack(iCombatSpell* pSpell)
{
	check(!m_spellTargets.GetSize());
	iHero* pCaster = m_pBattle->Engine().TurnSeq().CurUnit()->Owner()->SpellCaster();
	check(pCaster);
	MAGIC_SCHOOL_LEVEL msl = pSpell->GetSchoolLevel(pCaster);
	if (pSpell->TargetMode(msl) == STM_CREAT_GROUP) {
		m_pBattle->Engine().GetValidSpellTargets(pSpell, msl, m_spellTargets);
		if (!m_spellTargets.GetSize()) return false;
	} else {
		iSimpleArray<iBattleGroup*> targets;
		m_pBattle->Engine().SelectSpellTargets(pSpell, targets);
		if (!targets.GetSize()) return false;
	}

	check(!m_pCastSpellToolBar);
	m_pToolBar->SetVisible(false);
	AddChild(m_pCastSpellToolBar = new iCastSpellToolBar(m_pMgr, this, iRect(0, m_Rect.y2()-20,m_Rect.w, 21), pSpell));
	return true;
}

void iBattleView::EndSpellTrack(const iPoint& cell)
{
	check(m_pCastSpellToolBar);
	if (cell != cInvalidPoint) {
		iHero* pCaster = m_pBattle->Engine().TurnSeq().CurUnit()->Owner()->SpellCaster();
		check(pCaster);
		MAGIC_SCHOOL_LEVEL msl = m_pCastSpellToolBar->Spell()->GetSchoolLevel(pCaster);
		if (m_pCastSpellToolBar->Spell()->TargetMode(msl) != STM_CREAT_GROUP || m_spellTargets.Find(cell) != -1) {

			iBattleMember *targetOwner  = NULL;
			uint16 tt = m_pCastSpellToolBar->Spell()->TargetType(msl);

			iBattleMember::Side ourSide = m_pBattle->Engine().TurnSeq().CurUnit()->Owner()->GetSide();
			iBattleMember::Side enemySide;
			if(ourSide == iBattleMember::Side::Assaulter)
				enemySide = iBattleMember::Side::Defender;
			else
				enemySide = iBattleMember::Side::Assaulter;

			if(((tt & STT_FRIENDLY) == 0) && ((tt & STT_ENEMY) != 0))
				targetOwner =  m_pBattle->Engine().GetBattleInfo().Member(enemySide);
			else if (((tt & STT_FRIENDLY) != 0) && ((tt & STT_ENEMY) == 0))
				targetOwner =  m_pBattle->Engine().GetBattleInfo().Member(ourSide);
			//else leave NULL to select all
			
			iSimpleArray<iBattleGroup*> targets = m_pBattle->Engine().FindAllGroups(cell, targetOwner, m_pCastSpellToolBar->Spell()->AffectsToDead());			
			
			//uncomment to test:
			//targets.Add(m_pBattle->Engine().AArmy().Groups().At(0));
			//targets.Add(m_pBattle->Engine().DArmy().Groups().At(0));			

			iBattleGroup *pTarget = NULL;						
			if(targets.GetSize() > 1) {
				uint32 idx;
				iSimpleArray<iStringT> texts;
				for(uint32 xx=0; xx<targets.GetSize(); xx++)
				{
					if(targets[xx]->IsAlive())
						texts.Add(iFormat<iCharT>(_T("%d"), targets[xx]->Count()));
					else if(m_pCastSpellToolBar->Spell()->Id() == MSP_RESURRECT) {
						sint32 hp =  ((iSpell_Resurrect*)(m_pCastSpellToolBar->Spell()))->EstimateHitPoints(pCaster);
						texts.Add(iFormat<iCharT>(_T("%d/%d"), 
							targets[xx]->CountResurrected(hp),
							targets[xx]->InitialCount()));
					}
					else
						texts.Add(iStringT(_T("N/A")));
						
				}
				iDlg_ChooseBattleGroup dlg(m_pMgr, targets, texts, pCaster->Owner()->PlayerId(), &idx);
				if(dlg.DoModal() == DRC_OK)				
					pTarget = targets.At(idx);					
				else
					pTarget = NULL;
			} else
				pTarget = targets.At(0);					
		
			if (pTarget)
				if(!m_pBattle->Engine().CastSpell(m_pCastSpellToolBar->Spell(), cell, pTarget)) {
					iTextDlg tdlg(m_pMgr, _T(""), gTextMgr[TRID_MSG_SPELL_AFFECT_NO_ONE], pCaster->Owner()->PlayerId());
					tdlg.DoModal();
				} else BeginAni();
		}
	}
	RemoveChild(m_pCastSpellToolBar);
	delete m_pCastSpellToolBar;
	m_pCastSpellToolBar = NULL;
	m_pToolBar->SetVisible(true);
	m_spellTargets.RemoveAll();
}

bool iBattleView::OnGroupChanged()
{
	iBattleUnit* pCurUnit = m_pBattle->Engine().TurnSeq().CurUnit();

	if (iBattleUnit_CreatGroup* pCurCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(pCurUnit)) {
		iBattleGroup* pGroup  = pCurCreatGroup->GetCreatGroup();
		// First check morale
		if (pGroup->RoundMorale() == iBattleGroup::MLM_NEGATIVE) {
			m_pBattle->Engine().BadMorale();
			return true;
		}

		PLAYER_ID pid = pGroup->Owner()->Owner();
		bool bAiPlayer = pid == PID_NEUTRAL || gGame.Map().FindPlayer(pid)->PlayerType() == PT_COMPUTER;
		if (bAiPlayer || IsAutobattle()) {
			m_bHumanTurn = false;
			m_pToolBar->EnableControls(iBattleToolBar::EnemyTurn);

			// Try to cast the spell
			bool bCasted = ProcessSpellCastAI(m_pBattle->Engine());

			// Want retreat ?
			iBattleResult bres;
			if (bAiPlayer && !CheckBattleStateAI(m_pBattle->Engine(), bres)) {
				EndBattle(bres);
				return false;
			}

			// Process battle step
			if (!bCasted) {
				if (ProcessBattleStepAI(m_pBattle->Engine())) BeginAni();
				else if (!OnGroupChanged()) return false;
			}
		} else {
			m_bHumanTurn = true;
			uint32 flags = iBattleToolBar::CanInfo | iBattleToolBar::CanMelee;
			if (pGroup->Shots() && pGroup->ShootListCount()) flags |= iBattleToolBar::CanShoot;
			if (pGroup->Owner()->CanCastSpell()) flags |= iBattleToolBar::CanCast;
			if (m_pBattle->Engine().CanWait()) flags |= iBattleToolBar::CanWait;
			m_pToolBar->EnableControls(flags);
			bool bCanShot = pGroup->Shots() && pGroup->ShootListCount();
			if (m_battleMode != BNM_SHOOT && bCanShot) {
				m_battleMode = BNM_SHOOT;
				m_pToolBar->m_pModeSwitch->SetCurrentTab(BNM_SHOOT);
			} else if (!bCanShot && m_battleMode != BNM_MELEE) {
				m_battleMode = BNM_MELEE;
				m_pToolBar->m_pModeSwitch->SetCurrentTab(BNM_MELEE);
			}
		}

	} else if (iBattleUnit_Catapult* pCatapult = DynamicCast<iBattleUnit_Catapult*>(pCurUnit)) {
		iCastleFort::ElementType target = m_pBattle->Engine().CastleFort()->GetTarget();
		if (target != iCastleFort::InvalidElement) {
			m_pBattle->Engine().CatapultShot(target);
			BeginAni();
		}
	} else if (iBattleUnit_Turret* pTurret = DynamicCast<iBattleUnit_Turret*>(pCurUnit)) {
		iBattleGroup* pTarget = SelectTurretTarget(m_pBattle->Engine());
		check(pTarget);
		m_pBattle->Engine().TurretShot(pTarget);
		BeginAni();
		//m_pToolBar->EnableControls(iBattleToolBar::Acting);
	}
	return true;
}

void iBattleView::BeginAni()
{
	//gLogMgr.Log(iFormat(_T("BeginAni: %d actions\r\n"),m_pBattle->Engine().ActionCount()));
	m_pToolBar->EnableControls(iBattleToolBar::Acting);
	m_bAni = true;
	m_actTimer = fix32::zero;
	Invalidate();
}

void iBattleView::EndAni()
{
	//gLogMgr.Log(_T("EndAni\r\n"));
	m_bAni = false;
}

bool iBattleView::OnKeyDown(sint32 key)
{
	if (!m_bHumanTurn) return false;
	if (key == gSettings.ActionKey(BAT_HELP_MODE)) {
		m_bForceInfo = true;
		m_pToolBar->SetEnabled(false);
	} else {
		return false;
	}
	return true;
}

bool iBattleView::OnKeyUp(sint32 key)
{
	if (!m_bHumanTurn) return false;
	if (key == gSettings.ActionKey(BAT_HELP_MODE)) {
		m_bForceInfo = false;
		m_pToolBar->SetEnabled(true);
	} else {
		return false;
	}
	return true;
}

// Calculates delta distance between two cells
inline uint32 CellsDelta2(sint32 px1, sint32 py1, sint32 px2, sint32 py2) 
{ 
	px1 = px1 + py1/2;
	px2 = px2 + py2/2;
	uint32 dx = iABS(px1 - px2); 
	uint32 dy = iABS(py1 - py2);
	return iMIN(dx,dy) + iDIF(dx,dy); 
}

void iBattleView::OnMouseDown(const iPoint& pos)
{
	if (!m_bHumanTurn || m_bAni) return;
	m_trackPos = pos;
	m_trackCell = Screen2Map(pos);

	if (SpellTracking()) {
		iHero* pCaster = m_pBattle->Engine().TurnSeq().CurUnit()->Owner()->SpellCaster();
		check(pCaster);
		MAGIC_SCHOOL_LEVEL msl = m_pCastSpellToolBar->Spell()->GetSchoolLevel(pCaster);
		if (m_pCastSpellToolBar->Spell()->TargetMode(msl) == STM_CREAT_GROUP) {
			sint32 res = m_spellTargets.Find(m_trackCell);
			if (res != -1) {
				iBattleGroup* pTarget = m_pBattle->Engine().FindGroup(m_trackCell, m_pCastSpellToolBar->Spell()->AffectsToDead());
				check(pTarget);
				m_toolTip.Setf(gTextMgr[TRID_MSG_BAT_CAST_SPELL_TO], gTextMgr[m_pCastSpellToolBar->Spell()->NameKey()], gTextMgr[TRID_CREATURE_PEASANT_F3+pTarget->Type()*3]);
			} else {
				m_toolTip= _T("");
			}
		} else if (m_trackCell != cInvalidPoint) {
			m_toolTip.Setf(gTextMgr[TRID_MSG_BAT_CAST_SPELL], gTextMgr[m_pCastSpellToolBar->Spell()->NameKey()]);
		} else {
			m_toolTip= _T("");
		}
		Invalidate();
	} else if (m_battleMode == BNM_INFO || m_bForceInfo) {
		const iBattleGroup* pGroup = m_pBattle->Engine().FindGroup(m_trackCell);
		if (pGroup) {
			AddChild(m_pCreatInfoPopup = new iCreatInfoPopup(m_pMgr, pGroup->Owner()->Owner(), pGroup));
			if (pGroup->Pos().x > 5) m_pCreatInfoPopup->TrackPopup(iPoint(BATTLE_POPUP_BOUND.x,BATTLE_POPUP_BOUND.y),BATTLE_POPUP_BOUND,AlignLeft);
			else m_pCreatInfoPopup->TrackPopup(iPoint(BATTLE_POPUP_BOUND.x2(),BATTLE_POPUP_BOUND.y),BATTLE_POPUP_BOUND,AlignRight);
		} else {
			iBattleUnit_CreatGroup* pCurCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(m_pBattle->Engine().TurnSeq().CurUnit());
			check(pCurCreatGroup);
			iBattleGroup* pCreatGroup = pCurCreatGroup->GetCreatGroup();
			//OutputDebugString(iFormat(_T("Cells delta (%d,%d) -> (%d,%d) is %d\n"),pCreatGroup->Pos().x,pCreatGroup->Pos().y,m_trackCell.x,m_trackCell.y,CellsDelta2(pCreatGroup->Pos().x,pCreatGroup->Pos().y,m_trackCell.x,m_trackCell.y)).CStr());
		}
	} else if (m_battleMode == BNM_MELEE) {
		iBattleUnit_CreatGroup* pCurCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(m_pBattle->Engine().TurnSeq().CurUnit());
		check(pCurCreatGroup);
		m_pMeleeTrack = pCurCreatGroup->GetCreatGroup()->GetMeleeEntry(m_trackCell);
		m_meleeDir = -1;
		const iBattleGroup* pGroup = m_pBattle->Engine().FindGroup(m_trackCell);
		if (pGroup && m_pMeleeTrack) {
			m_toolTip.Setf(gTextMgr[TRID_MSG_BAT_ATTACK], gTextMgr[TRID_CREATURE_PEASANT_F3+pGroup->Type()*3]);
			sint32 min_dmg = pCurCreatGroup->GetCreatGroup()->CalcDamage(pGroup, pCurCreatGroup->GetCreatGroup()->ActMinDamage(), false, 1, pCurCreatGroup->GetCreatGroup()->CalcJoustingBonus(pGroup));
			sint32 max_dmg = pCurCreatGroup->GetCreatGroup()->CalcDamage(pGroup, pCurCreatGroup->GetCreatGroup()->ActMaxDamage(), false, 1, pCurCreatGroup->GetCreatGroup()->CalcJoustingBonus(pGroup));
			if (max_dmg == min_dmg) m_toolTip.Addf(gTextMgr[TRID_MSG_BAT_DAMAGE1], min_dmg);
			else m_toolTip.Addf(gTextMgr[TRID_MSG_BAT_DAMAGE2], min_dmg, max_dmg);
		}
		Invalidate();
	} else if (m_battleMode == BNM_SHOOT) {
		iBattleUnit_CreatGroup* pCurCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(m_pBattle->Engine().TurnSeq().CurUnit());
		check(pCurCreatGroup);
		m_pShootTrack = pCurCreatGroup->GetCreatGroup()->GetShootEntry(m_trackCell);
		const iBattleGroup* pGroup = m_pBattle->Engine().FindGroup(m_trackCell);
		if (pGroup && m_pShootTrack) {
			m_toolTip.Setf(gTextMgr[TRID_MSG_BAT_SHOOT], gTextMgr[TRID_CREATURE_PEASANT_F3+pGroup->Type()*3]);
			sint32 min_dmg = pCurCreatGroup->GetCreatGroup()->CalcDamage(pGroup, pCurCreatGroup->GetCreatGroup()->ActMinDamage(), true, m_pShootTrack->m_penalty, pCurCreatGroup->GetCreatGroup()->CalcJoustingBonus(pGroup));
			sint32 max_dmg = pCurCreatGroup->GetCreatGroup()->CalcDamage(pGroup, pCurCreatGroup->GetCreatGroup()->ActMaxDamage(), true, m_pShootTrack->m_penalty, pCurCreatGroup->GetCreatGroup()->CalcJoustingBonus(pGroup));
			if (max_dmg == min_dmg) m_toolTip.Addf(gTextMgr[TRID_MSG_BAT_DAMAGE1], min_dmg);
			else m_toolTip.Addf(gTextMgr[TRID_MSG_BAT_DAMAGE2], min_dmg, max_dmg);
		}
		Invalidate();
	}
}

void iBattleView::OnMouseUp(const iPoint& pos)
{
	if (!m_bHumanTurn || m_bAni) return;

	if (SpellTracking()) {
		EndSpellTrack(m_trackCell);
		Invalidate();
	} else if (m_pCreatInfoPopup || m_battleMode == BNM_INFO || m_bForceInfo) {
		if (m_pCreatInfoPopup) {
			RemoveChild(m_pCreatInfoPopup);
			m_pCreatInfoPopup->HidePopup();
			delete m_pCreatInfoPopup;
			m_pCreatInfoPopup = NULL;
		}
	} else if (m_battleMode == BNM_MELEE && m_pMeleeTrack) {
		if ( m_meleeDir != -1) {
			check (m_meleeDir < MDIR_COUNT);
			m_pBattle->Engine().Melee(m_pMeleeTrack->m_pos, (uint16)m_meleeDir);
			BeginAni();
			m_meleeDir = -1;
		}
		m_pMeleeTrack = NULL;
	} else if (m_battleMode == BNM_SHOOT) {
		if (m_pShootTrack) {
			m_pBattle->Engine().Shot(m_pShootTrack->m_pos, m_pShootTrack->m_penalty);
			BeginAni();
			m_pShootTrack = NULL;
		}
	} else if (iBattleUnit_CreatGroup* pCurCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(m_pBattle->Engine().TurnSeq().CurUnit())) {
		iPoint nCell = Screen2Map(pos);
		if (nCell == m_trackCell && pCurCreatGroup->GetCreatGroup()->CanMove(nCell.x,nCell.y) && !pCurCreatGroup->GetCreatGroup()->IsGroupCell(nCell)) {
			m_pBattle->Engine().Move(nCell, pCurCreatGroup->GetCreatGroup()->Orient());
			BeginAni();
		}
	}
	m_trackCell = cInvalidPoint;
	m_trackPos = cInvalidPoint;
	if (!m_toolTip.Empty()) m_toolTip = _T("");
	Invalidate();
}

void iBattleView::OnMouseTrack(const iPoint& pos)
{
	if (!m_bHumanTurn || m_bAni || m_pCreatInfoPopup) return;
	m_trackPos = pos;

	if (SpellTracking()) {
		m_trackCell = Screen2Map(pos);
		iHero* pCaster = m_pBattle->Engine().TurnSeq().CurUnit()->Owner()->SpellCaster();
		check(pCaster);
		MAGIC_SCHOOL_LEVEL msl = m_pCastSpellToolBar->Spell()->GetSchoolLevel(pCaster);
		if (m_pCastSpellToolBar->Spell()->TargetMode(msl) == STM_CREAT_GROUP) {
			sint32 res = m_spellTargets.Find(m_trackCell);
			if (res != -1) {
				iBattleGroup* pTarget = m_pBattle->Engine().FindGroup(m_trackCell, m_pCastSpellToolBar->Spell()->AffectsToDead());
				check(pTarget);
				m_toolTip.Setf(gTextMgr[TRID_MSG_BAT_CAST_SPELL_TO], gTextMgr[m_pCastSpellToolBar->Spell()->NameKey()], gTextMgr[TRID_CREATURE_PEASANT_F3+pTarget->Type()*3]);
			} else {
				m_toolTip= _T("");
			}
		} else if (m_trackCell != cInvalidPoint) {
			m_toolTip.Setf(gTextMgr[TRID_MSG_BAT_CAST_SPELL], gTextMgr[m_pCastSpellToolBar->Spell()->NameKey()]);
		} else {
			m_toolTip= _T("");
		}
		Invalidate();
	} else if (m_battleMode == BNM_MELEE && m_pMeleeTrack) {
		iPoint center = Map2Screen(m_pMeleeTrack->m_pos);
		center.Move(10,9);
		sint32 xdiff = pos.x - center.x;
		sint32 ydiff = pos.y - center.y;
		if ( iABS(xdiff) > 30 || iABS(ydiff) > 30) {
			m_meleeDir = -1;
		} else {
			sint32 ndir = -1;

			if ( ((m_pMeleeTrack->m_cfg & (1<<MDIR_LSPTOP)) || (m_pMeleeTrack->m_cfg & (1<<MDIR_LSPBOTTOM))) && xdiff<=0 && iABS(xdiff) > iABS(ydiff)) {
				if (ydiff<0) ndir = MDIR_LSPTOP;
				else ndir = MDIR_LSPBOTTOM;
			} else if (((m_pMeleeTrack->m_cfg & (1<<MDIR_RSPTOP)) || (m_pMeleeTrack->m_cfg & (1<<MDIR_RSPBOTTOM))) && xdiff>0 && iABS(xdiff) > iABS(ydiff)) {
				if (ydiff<0) ndir = MDIR_RSPTOP;
				else ndir = MDIR_RSPBOTTOM;
			} else if ( (m_pMeleeTrack->m_cfg & (1<<MDIR_NORTH)) && ydiff <= 0 && iABS(ydiff) > (iABS(xdiff)*3)) {
				ndir = MDIR_NORTH;
			} else if (xdiff <= 0 && ydiff <= 0 && iABS(ydiff) > iABS(xdiff)) {
				ndir = MDIR_NORTHWEST;
			} else if (xdiff > 0 && ydiff <= 0 && iABS(ydiff) > iABS(xdiff)) {
				ndir = MDIR_NORTHEAST;
			} else if ((m_pMeleeTrack->m_cfg & (1<<MDIR_SOUTH)) && ydiff > 0 && iABS(ydiff) > (iABS(xdiff)*3)) {
				ndir = MDIR_SOUTH;
			} else if (xdiff <= 0 && ydiff > 0 && iABS(ydiff) > iABS(xdiff)) {
				ndir = MDIR_SOUTHWEST;
			} else if (xdiff > 0 && ydiff > 0 && iABS(ydiff) > iABS(xdiff)) {
				ndir = MDIR_SOUTHEAST;
			} else if (xdiff <= 0  && iABS(ydiff) <= iABS(xdiff)) {
				ndir = MDIR_WEST;
			} else if (xdiff > 0  && iABS(ydiff) <= iABS(xdiff)) {
				ndir = MDIR_EAST;
			}
			
			if (m_pMeleeTrack->m_cfg & (1<<ndir)) m_meleeDir = ndir;
			else m_meleeDir = -1;
		}
		Invalidate();
	} else if (m_battleMode == BNM_SHOOT) {
		m_trackCell = Screen2Map(pos);
		iBattleUnit_CreatGroup* pCurCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(m_pBattle->Engine().TurnSeq().CurUnit());
		check(pCurCreatGroup);
		iPoint ncell = Screen2Map(pos);
		if (!m_pShootTrack || ncell != m_pShootTrack->m_pos) m_pShootTrack = pCurCreatGroup->GetCreatGroup()->GetShootEntry(ncell);

		const iBattleGroup* pGroup = m_pBattle->Engine().FindGroup(ncell);
		if (m_pShootTrack && pGroup) {
			m_toolTip.Setf(gTextMgr[TRID_MSG_BAT_SHOOT], gTextMgr[TRID_CREATURE_PEASANT_F3+pGroup->Type()*3]);
			sint32 min_dmg = pCurCreatGroup->GetCreatGroup()->CalcDamage(pGroup, pCurCreatGroup->GetCreatGroup()->ActMinDamage(), true, m_pShootTrack->m_penalty, pCurCreatGroup->GetCreatGroup()->CalcJoustingBonus(pGroup));
			sint32 max_dmg = pCurCreatGroup->GetCreatGroup()->CalcDamage(pGroup, pCurCreatGroup->GetCreatGroup()->ActMaxDamage(), true, m_pShootTrack->m_penalty, pCurCreatGroup->GetCreatGroup()->CalcJoustingBonus(pGroup));
			if (max_dmg == min_dmg) m_toolTip.Addf(gTextMgr[TRID_MSG_BAT_DAMAGE1], min_dmg);
			else m_toolTip.Addf(gTextMgr[TRID_MSG_BAT_DAMAGE2], min_dmg, max_dmg);
		} else {
			m_toolTip = _T("");
		}

		Invalidate();
	}
}

void iBattleView::PrepareSurface()
{
	m_dibSurf.Init(m_Rect.size(),iDib::RGB);

	iIsoMetric im(5);

	sint32 step_x = im.GetCellStepX()*2;
	sint32 step_y = im.GetCellStepY();
	sint32 cnt_x=10;
	sint32 cnt_y=20;
	// Draw background surface
	for (sint32 yy=0; yy<cnt_y; ++yy) {
		for (sint32 xx=0; xx<cnt_x; ++xx){
			sint32 xpos = xx*step_x;
			sint32 ypos = yy*step_y;
			if (yy%2!=0) xpos -= step_x >> 1;
			iPoint op(xpos-step_x,ypos+20);
			uint32 cseq = CalcCellSeq(iPoint(xx,yy),4);
			// Surf
			gGfxMgr.BlitMasked(PDGG_SURF_TILES + m_surfType*4 + cseq, PDGG_TRANS_TILES+14,m_dibSurf,op);
		}
	}

	// Draw moat
	if (const iCastleFort* pFort = m_pBattle->Engine().CastleFort()) {
		if (pFort->HasMoat()) {
			gGfxMgr.Blit(PDGG_COMBAT_MOAT, m_dibSurf, Map2Screen(iPoint(8,0)));
		}
	}


	// Draw Sky
	m_dibSurf.VGradientRect(iRect(m_Rect.x,m_Rect.y,m_Rect.w,30), SKY_GRAD_COLORS[m_surfType][0], SKY_GRAD_COLORS[m_surfType][1]);
	m_dibSurf.HLine(iPoint(m_Rect.x,m_Rect.y+30), m_Rect.x2(), SKY_GRAD_COLORS[m_surfType][1], 192);
	m_dibSurf.HLine(iPoint(m_Rect.x,m_Rect.y+31), m_Rect.x2(), SKY_GRAD_COLORS[m_surfType][1], 128);
	m_dibSurf.HLine(iPoint(m_Rect.x,m_Rect.y+32), m_Rect.x2(), SKY_GRAD_COLORS[m_surfType][1], 64);

	//m_dibSurf.FillRect(iRect(0,0,m_Rect.w,30),cColor_Blue);
}

bool iBattleView::Process(fix32 t)
{
	// actions
	if (m_pBattle->Engine().ActionCount()) {
		m_actTimer += t;
		while (m_pBattle->Engine().ActionCount() && m_actTimer >= m_pBattle->Engine().CurAction()->m_delay) {
			iBattleAct* pAct = m_pBattle->Engine().StepAction();
			check(pAct);
			Invalidate();
			m_actTimer -= pAct->m_delay;
			delete pAct;

			// No more actions for current group
			if (!m_pBattle->Engine().ActionCount()) {
				EndAni();
				// Check victory conditions
				BATTLE_RESULT br = m_pBattle->Engine().CheckBattleState();
				if (br != BR_NA) {
					EndBattle(iBattleResult(br,DC_DEFEAT));
					return true;
				}
				m_pBattle->Engine().NextGroup();
				if (!OnGroupChanged()) return true;
				Invalidate();
			}
		}
	}

	// events
	iEvtIt it=m_cellEvents.First();
	while(it != m_cellEvents.End()) {
		Invalidate();
		(*it)->time -= t;
		if ( (*it)->time < fix32::zero){
			it = m_cellEvents.Delete(it);
			continue;
		}
		(*it)->dstate = (uint8)iCLAMP<sint32>(0,255,((*it)->time * fix32(255.0f) / CELL_EVT_DELAY).ceil());
		++it;
	}

	return true;
}

void iBattleView::AddLogEvent(const iStringT& msg)
{
	m_log.Add(msg);
}

void iBattleView::AddCellEvent(const iStringT& msg, const iPoint& pos)
{
	m_cellEvents.Add(new iGameEvent(fix32(2.0f), msg, pos));
}

void iBattleView::OnCompose()
{
	// Compose background
	m_dibSurf.CopyToDibXY(gApp.Surface(),iPoint(0,0));
	if (!m_pBattle) return;

	// Show turn sequence (for current round)
	iSize glyphSize(30,26);
	if (m_dibTurnGlyph.GetSize() != glyphSize) m_dibTurnGlyph.Resize(glyphSize);
	const iTurnSeq& turnSeq = m_pBattle->Engine().TurnSeq();
	iRect tseqRect(1,1,glyphSize.w,glyphSize.h);
	for (uint32 xx=0; xx<10; ++xx) {
		gApp.Surface().CopyRectToDibXY(m_dibTurnGlyph, tseqRect, iPoint());
		if (xx < turnSeq.GetTurnSeqSize()) {
			if (iBattleUnit_CreatGroup* pCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(turnSeq.GetTurnSeqItem(xx))) {
				ComposeCreatureIcon(m_dibTurnGlyph, iRect(1,1,glyphSize.w-2,glyphSize.h-2), pCreatGroup->GetCreatGroup()->Type(), pCreatGroup->Owner()->GetSide() == iBattleMember::Defender);
			} else if (iBattleUnit_Catapult* pCatapult = DynamicCast<iBattleUnit_Catapult*>(turnSeq.GetTurnSeqItem(xx))) {
				gGfxMgr.Blit(PDGG_ICN_CATAPULT, m_dibTurnGlyph, iPoint(1,1));
			} else if (iBattleUnit_Turret* pTurret = DynamicCast<iBattleUnit_Turret*>(turnSeq.GetTurnSeqItem(xx))) {
				gGfxMgr.Blit(PDGG_ICN_TURRET, m_dibTurnGlyph, iPoint(1,1));
			}
		} else {
			//gGfxMgr.BlitTile(PDGG_BKTILE, m_dibTurnGlyph ,glyphSize);
			//ButtonFrame(m_dibTurnGlyph, glyphSize, 0);
		}
		m_dibTurnGlyph.CopyToDibXY(gApp.Surface(), tseqRect, (uint8)((15-xx)*10));
		tseqRect.x += 32;
	}

	iBattleUnit* pCurUnit  = m_pBattle->Engine().TurnSeq().CurUnit();
	PLAYER_ID pid = pCurUnit->Owner()->Owner();
	bool bInt = !(pid == PID_NEUTRAL || gGame.Map().FindPlayer(pid)->PlayerType() == PT_COMPUTER) && !m_pBattle->Engine().ActionCount() && (m_pBattle->Engine().CheckBattleState() == BR_NA);

	iPoint op;
	// Draw Grid
	if (gSettings.GetEntryValue(CET_COMBATGRID)) {
		op = anchor;
		for (sint32 yy=0; yy<11; ++yy) {
			for (uint32 xx=0; xx<13; ++xx) {
				gGfxMgr.BlitEffect(PDGG_GRID_HEX, gApp.Surface(), op,iGfxManager::EfxTransparent);
				// Draw cover map (only for interactive side)
				if (bInt && !SpellTracking()){
					if (iBattleUnit_CreatGroup* pCurCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(m_pBattle->Engine().TurnSeq().CurUnit())){
						if (pCurCreatGroup->GetCreatGroup()->CanMove(xx,yy)) {
							gGfxMgr.BlitEffect( PDGG_GRID_HEX, gApp.Surface(), op, iGfxManager::EfxTransparent);
						}
					}
				}
				op.x+=24;
			}
			op.y+=17;
			op.x=anchor.x;
			if (yy%2 == 0) op.x-=13;
		}
	}

	// Show path (for debug)
	//if (!m_Actions.GetSize()) for (uint32 pp=0; pp<m_Path.GetSize(); ++pp) gfxMgr.BlitEffect( PDGG_HEXCELLS, dib, Map2Screen(m_Path[pp]), iGfxManager::EfxTransparent);

	// Draw current troop and targets highlighters (only for interactive side)
	if (bInt) {
		if (SpellTracking()) {
			iHero* pCaster = m_pBattle->Engine().TurnSeq().CurUnit()->Owner()->SpellCaster();
			check(pCaster);
			MAGIC_SCHOOL_LEVEL msl = m_pCastSpellToolBar->Spell()->GetSchoolLevel(pCaster);
			if (m_pCastSpellToolBar->Spell()->TargetMode(msl) == STM_CREAT_GROUP) {
				check(m_spellTargets.GetSize());
				for (uint32 xx=0; xx<m_spellTargets.GetSize(); ++xx) {
					iPoint op = Map2Screen(m_spellTargets[xx]);
					gGfxMgr.Blit( PDGG_HEX_TARGET_CELL, gApp.Surface(), op);
					if (m_spellTargets[xx] != m_trackCell) gGfxMgr.BlitEffect( PDGG_HEX_TARGET_CELL, gApp.Surface(), op, iGfxManager::EfxTransparent);
				}
			} else {
				check(m_pCastSpellToolBar->Spell()->HasCoverArea(msl));
				iSimpleArray<iPoint> spellCovers;
				m_pBattle->Engine().GetSpellCovers(m_pCastSpellToolBar->Spell(), msl, m_trackCell, spellCovers);
				iBattleUnit_Hero caster(m_pBattle->Engine().TurnSeq().CurUnit()->Owner());
				for (uint32 xx=0; xx<spellCovers.GetSize(); ++xx) {
					iPoint op = Map2Screen(spellCovers[xx]);
					if (iBattleGroup* pTarget = m_pBattle->Engine().FindGroup(spellCovers[xx]))	{
						if (m_pCastSpellToolBar->Spell()->CanCast(&caster, pTarget)) {
							gGfxMgr.Blit( PDGG_HEX_TARGET_CELL, gApp.Surface(), op);
						} else {
							gGfxMgr.Blit( PDGG_HEX_CELL, gApp.Surface(), op);
						}
					} else gGfxMgr.Blit( PDGG_HEX_CELL, gApp.Surface(), op);
					//gGfxMgr.BlitEffect( PDGG_HEX_TARGET_CELL, gApp.Surface(), op, iGfxManager::EfxTransparent);
				}
			}
		} else {
			if (iBattleUnit_CreatGroup* pCurCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(pCurUnit)) {
				if (m_battleMode == BNM_MELEE) {
					for (uint32 xx=0; xx<pCurCreatGroup->GetCreatGroup()->MeleeListCount(); ++xx) {
						iPoint op = Map2Screen(pCurCreatGroup->GetCreatGroup()->GetMeleeEntry(xx)->m_pos);
						gGfxMgr.Blit( PDGG_HEX_TARGET_CELL, gApp.Surface(), op);
						gGfxMgr.BlitEffect( PDGG_HEX_TARGET_CELL, gApp.Surface(), op, iGfxManager::EfxTransparent);
					}
				} else if (m_battleMode == BNM_SHOOT) {
					for (uint32 xx=0; xx<pCurCreatGroup->GetCreatGroup()->ShootListCount(); ++xx) {
						iPoint op = Map2Screen(pCurCreatGroup->GetCreatGroup()->GetShootEntry(xx)->m_pos);
						gGfxMgr.Blit( PDGG_HEX_TARGET_CELL, gApp.Surface(), op);
						if (pCurCreatGroup->GetCreatGroup()->GetShootEntry(xx)->m_pos != m_trackCell) gGfxMgr.BlitEffect( PDGG_HEX_TARGET_CELL, gApp.Surface(), op, iGfxManager::EfxTransparent);
					}
				}
			}
			if (m_pMeleeTrack) gGfxMgr.Blit(PDGG_HEX_TARGET_CELL, gApp.Surface(), Map2Screen(m_trackCell));
		}
	}

	//
	iSortArray<iBatCmpItem*> cmpList;

	// Add castle's fort elements
	if (const iCastleFort* pFort = m_pBattle->Engine().CastleFort()) {
		for (uint32 xx=0; xx<pFort->ElementsCount(); ++xx) {
			cmpList.Insert(new iBatCmpItem_FortElement(pFort->GetElement(xx)), (FORT_DECS[pFort->GetElement(xx)->type][pFort->GetElement(xx)->state].bGround)?0:(iCastleFort::ElCells[xx][1]*13+iCastleFort::ElCells[xx][0]));
		}
	}

	// Add units
	op = anchor;
	iBattleGroup* pGroup = NULL;
	if (iBattleUnit_CreatGroup* pCurCreatGroup = DynamicCast<iBattleUnit_CreatGroup*>(pCurUnit)) pGroup = pCurCreatGroup->GetCreatGroup();
	for (uint32 xx=0; xx<m_pBattle->Engine().AArmy().Count(); ++xx) { 
		uint32 val = (m_pBattle->Engine().AArmy()[xx]->Pos().y*13+m_pBattle->Engine().AArmy()[xx]->Pos().x);
		if (m_pBattle->Engine().AArmy()[xx]->State() == iBattleGroup::Melee || m_pBattle->Engine().AArmy()[xx]->State() == iBattleGroup::Shooting) val += 2;
		cmpList.Insert(new iBatCmpItem_Creature(m_pBattle->Engine().AArmy()[xx], m_pBattle->Engine().AArmy()[xx] == pGroup && !SpellTracking()), val);
	}
	for (uint32 xx=0; xx<m_pBattle->Engine().DArmy().Count(); ++xx) {
		uint32 val = (m_pBattle->Engine().DArmy()[xx]->Pos().y*13+m_pBattle->Engine().DArmy()[xx]->Pos().x);
		if (m_pBattle->Engine().DArmy()[xx]->State() == iBattleGroup::Melee || m_pBattle->Engine().DArmy()[xx]->State() == iBattleGroup::Shooting) val += 2;
		cmpList.Insert(new iBatCmpItem_Creature(m_pBattle->Engine().DArmy()[xx], m_pBattle->Engine().DArmy()[xx] == pGroup && !SpellTracking()), val);
	}

	// Add obstacles
	for (uint32 xx=0; xx<m_obstacles.GetSize(); ++xx) { 
		uint32 val = (m_obstacles[xx].cell.y*13+m_obstacles[xx].cell.x);
		cmpList.Insert(new iBatCmpItem_Obstacle(&m_obstacles[xx]), val);
	}

	// Draw shadow
	for (uint32 xx=0; xx<cmpList.Size(); ++xx) {
		cmpList[xx].value->ComposeShadow(gApp.Surface(),Map2Screen(cmpList[xx].value->m_pos));
	}

	// Draw sprites
	for (uint32 xx=0; xx<cmpList.Size(); ++xx) {
		cmpList[xx].value->Compose(gApp.Surface(),Map2Screen(cmpList[xx].value->m_pos), bInt);
	}

	for (uint32 xx=0; xx<cmpList.Size(); ++xx) delete cmpList[xx].value;
	cmpList.Cleanup();

#if 0 // Debug dist map
	op = anchor;
	iTextComposer::FontConfig fc(iTextComposer::FS_SMALL);
	for (yy=0; yy<11; ++yy) {
		for (uint32 xx=0; xx<13; ++xx) {
			if (m_pBattle->Engine().CurGroup()->CanMove(xx,yy)) gGfxMgr.BlitAlpha(PDGG_GRID_HEX, gApp.Surface(), op, 16);
			if (m_pBattle->Engine().CurGroup()->DistMap().GetAt(xx,yy).dirs[0] != 0xFF) gTextComposer.TextOut(fc, gApp.Surface(), iPoint(0,0), iFormat(_T("%d"),m_pBattle->Engine().CurGroup()->DistMap().GetAt(xx,yy).dirs[0]), iRect(op.x,op.y,20,19),AlignRight);
			if (m_pBattle->Engine().CurGroup()->DistMap().GetAt(xx,yy).dirs[1] != 0xFF) gTextComposer.TextOut(fc, gApp.Surface(), iPoint(0,0), iFormat(_T("%d"),m_pBattle->Engine().CurGroup()->DistMap().GetAt(xx,yy).dirs[1]), iRect(op.x,op.y,20,19),AlignLeft);
			op.x+=20;
		}
		op.y+=15;
		op.x=anchor.x;
		if (yy%2 == 0) op.x-=10;
	}
#endif

	// Compose cell events
	for (iEvtCIt it=m_cellEvents.First(); it != m_cellEvents.End(); ++it) {
		iPoint pos = Map2Screen((*it)->pos);
		pos.x += 10;
		pos.y -= 20 + (255 - (*it)->dstate) / 7;
		uint8 a = (*it)->dstate;
		if (a < 128) a *= 2; else a = 255;
		
		iTextComposer::FontConfig fontcfg(iTextComposer::FS_MEDIUM, iDibFont::ComposeProps(defGrad, cColor_Black, iDibFont::DecBorder ));
		gTextComposer.TextOut( fontcfg, gApp.Surface(), pos, iFormat(_T("#A%02X"),a)+(*it)->text, iRect(pos,iSize()), AlignTop);
	}

	// Show toolTip
	if (!m_toolTip.Empty()) {
		iTextComposer::FontConfig fontcfg(iTextComposer::FS_MEDIUM, iDibFont::ComposeProps(TOOLTIP_COLORS[m_surfType][1], TOOLTIP_COLORS[m_surfType][0],iDibFont::DecBorder));
		gTextComposer.TextOut( fontcfg, gApp.Surface(), iPoint(), m_toolTip, iRect(0,0,320,30), AlignCenter);
	}

	// Draw  cursor and action info
	if (SpellTracking()) {
		iHero* pCaster = m_pBattle->Engine().TurnSeq().CurUnit()->Owner()->SpellCaster();
		check(pCaster);
		MAGIC_SCHOOL_LEVEL msl = m_pCastSpellToolBar->Spell()->GetSchoolLevel(pCaster);
		if (m_pCastSpellToolBar->Spell()->TargetMode(msl) == STM_CREAT_GROUP) {
			if (m_trackCell != cInvalidPoint && m_spellTargets.Find(m_trackCell) != -1) {
				iPoint cop(m_trackPos.x - (sint32)(gGfxMgr.Dimension(PDGG_SPELL_CURSOR).w/2), m_trackPos.y - (sint32)(gGfxMgr.Dimension(PDGG_SPELL_CURSOR).h/2));
				gGfxMgr.BlitEffect( PDGG_SPELL_CURSOR, gApp.Surface(), cop,iGfxManager::EfxShadow2D);
				gGfxMgr.Blit( PDGG_SPELL_CURSOR, gApp.Surface(), cop);
			}
		} else {
		}
	} else if (m_battleMode == BNM_MELEE) {
		if (m_pMeleeTrack && m_meleeDir != -1) {
			iPoint cop = Map2Screen(m_pMeleeTrack->m_pos);
			gGfxMgr.BlitEffect( PDGG_MELEE_CURSOR+m_meleeDir, gApp.Surface(), cop,iGfxManager::EfxShadow2D);
			gGfxMgr.Blit( PDGG_MELEE_CURSOR+m_meleeDir, gApp.Surface(), cop);
		}
	} else if (m_battleMode == BNM_SHOOT) {
		if (m_pShootTrack) {
			SpriteId sid = PDGG_SHOOT_CURSOR;
			if (m_pShootTrack->m_penalty > 1) sid += m_pShootTrack->m_penalty/2;
			iPoint cop(m_trackPos.x - (sint32)(gGfxMgr.Dimension(sid).w/2), m_trackPos.y - (sint32)(gGfxMgr.Dimension(sid).h/2));
			gGfxMgr.BlitEffect( sid, gApp.Surface(), cop,iGfxManager::EfxShadow2D);
			gGfxMgr.Blit( sid, gApp.Surface(), cop);
		}
	}
}

void iBattleView::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param)
{
	uint32 uid = pView->GetUID();

	if (m_pCastSpellToolBar && uid == DRC_CANCEL) {
		EndSpellTrack(cInvalidPoint);
	} else if (m_pAutoBattleToolBar && uid == DRC_CANCEL) {
		EndAutobattle();
	} else if (uid == BCI_ASSAULTER_BTN || uid == BCI_DEFENDER_BTN) {
		iBattleMember* pActMember = (uid == BCI_ASSAULTER_BTN)?m_pBattle->Engine().GetBattleInfo().m_pAssaulter:m_pBattle->Engine().GetBattleInfo().m_pDefender;
		iBattleMember* pFriendlyMember = m_pBattle->Engine().TurnSeq().CurUnit()->Owner();
		iBattleMember* pEnemyMember = (pFriendlyMember->GetSide() == iBattleMember::Defender)?m_pBattle->Engine().GetBattleInfo().m_pAssaulter:m_pBattle->Engine().GetBattleInfo().m_pDefender;

		iHero* pHero = NULL;
		if (iBattleMember_Hero* pHeroActor = DynamicCast<iBattleMember_Hero*>(pActMember)) {
			pHero = pHeroActor->GetHero();
		} else if (iBattleMember_Castle* pCastleActor = DynamicCast<iBattleMember_Castle*>(pActMember)) {
			pHero = pCastleActor->GetVisitor();
		} 
		check(pHero);


		BATTLE_RESULT br = (pFriendlyMember->GetSide() == iBattleMember::Defender)?BR_ASSAULTER_WIN:BR_DEFENDER_WIN;


		uint32 flags = 0;
		if (pActMember->GetSide() == pFriendlyMember->GetSide() && !DynamicCast<iCastle*>(pHero->GetLocation())) {
			flags |= iDlg_BattleHero::CanRetreat;
			if (DynamicCast<iBattleMember_Hero*>(pEnemyMember)) flags |= iDlg_BattleHero::CanSurrender;
		}

		iDlg_BattleHero bhDlg(m_pMgr, *pHero, flags);
		sint32 res = bhDlg.DoModal();

		if (res == iDlg_BattleHero::Retreat) {
			// Check for Shackles of War
			if (iHero* pArtOwner = m_pBattle->Engine().GetBattleInfo().HasArtInBattle(ARTT_SHOFWAR)) {
				iTextDlg dlg(m_pMgr, _T(""), iFormat(gTextMgr[TRID_MSG_BAT_SHOFWAR], pArtOwner->Name().CStr()), pHero->Owner()->PlayerId());
				dlg.DoModal();
			} else {
				iDlg_TownList tldlg(m_pMgr, pHero->Owner(),gTextMgr[TRID_SELECT_DESTINATION],TRID_RETREAT);
				if (tldlg.DoModal() == DRC_OK) {
					check(tldlg.Destination());
					EndBattle(iBattleResult(br,DC_RETREAT,tldlg.Destination()));
				}

			}

		} else if (res == iDlg_BattleHero::Surrender) {
			// Check for Shackles of War
			if (iHero* pArtOwner = m_pBattle->Engine().GetBattleInfo().HasArtInBattle(ARTT_SHOFWAR)) {
				iTextDlg dlg(m_pMgr, _T(""), iFormat(gTextMgr[TRID_MSG_BAT_SHOFWAR], pArtOwner->Name().CStr()), pHero->Owner()->PlayerId());
				dlg.DoModal();
			} else {
				iBattleMember_Hero* pEnemyHero = DynamicCast<iBattleMember_Hero*>(pEnemyMember);
				check(pEnemyHero);
				uint32 cost = m_pBattle->Engine().GetSurrenderCost(DynamicCast<iBattleMember_Hero*>(pFriendlyMember));
				iDlg_SurrenderCond scdlg(m_pMgr, pEnemyHero->GetHero(), cost);
				if (scdlg.DoModal() == DRC_OK) {
					if (pHero->Owner()->Mineral(MINERAL_GOLD) < cost) {
						iTextDlg nerdlg(m_pMgr, _T(""), gTextMgr[TRID_MSG_NOTENOUGHGOLD], pHero->Owner()->PlayerId());
						nerdlg.DoModal();
					} else {
						iDlg_TownList tldlg(m_pMgr, pHero->Owner(),gTextMgr[TRID_SELECT_DESTINATION],TRID_SURRENDER);
						if (tldlg.DoModal() == DRC_OK) {
							check(tldlg.Destination());
							EndBattle(iBattleResult(br,DC_SURRENDER,tldlg.Destination(), cost));
						}
					}
				}
			}
		}
	} else if (uid == BCI_WAIT_BTN) {
		m_pBattle->Engine().Wait();
		OnGroupChanged();
	} else if (uid == BCI_DEFEND_BTN) {
		m_pBattle->Engine().Defend();
		OnGroupChanged();
	} else if (uid == BCI_CASTSPELL_BTN) {
		iHero* pHero = m_pBattle->Engine().TurnSeq().CurUnit()->Owner()->SpellCaster();
		check(pHero);
		// Check for Spere of Negation
		if (iHero* pArtOwner = m_pBattle->Engine().GetBattleInfo().HasArtInBattle(ARTT_NEGSPHERE)) {
			iTextDlg dlg(m_pMgr, _T(""), iFormat(gTextMgr[TRID_MSG_BAT_NEGSPHERE], pArtOwner->Name().CStr()), pHero->Owner()->PlayerId());
			dlg.DoModal();
			return;
		}

		// Show spell list dialog
		iDlg_CastSpell csDlg(m_pMgr, pHero, SPT_MASK_COMBAT);
		if (csDlg.DoModal() == DRC_YES) {
			iBaseSpell* pSpell = gGame.ItemMgr().m_spellMgr.Spell(csDlg.SelSpell());
			check(pSpell && pSpell->Type() == SPT_COMBAT);
			iCombatSpell* pCombatSpell = DynamicCast<iCombatSpell*>(pSpell);
			check(pCombatSpell);
			MAGIC_SCHOOL_LEVEL msl = pSpell->GetSchoolLevel(pHero);
			if (pCombatSpell->NeedTarget(msl)) {
				if (!BeginSpellTrack(pCombatSpell)) {
					iTextDlg tdlg(m_pMgr, _T(""), gTextMgr[TRID_MSG_SPELL_AFFECT_NO_ONE], pHero->Owner()->PlayerId());
					tdlg.DoModal();
				}
			} else {
				if (!m_pBattle->Engine().CastSpell(pCombatSpell, cInvalidPoint, NULL)) {
					iTextDlg tdlg(m_pMgr, _T(""), gTextMgr[TRID_MSG_SPELL_AFFECT_NO_ONE], pHero->Owner()->PlayerId());
					tdlg.DoModal();
				} else BeginAni();
			}
		}
	} else if (uid == BCI_AUTOBATTLE_BTN) {
		//iTextDlg tdlg(m_pMgr, _T(""), gTextMgr[TRID_MSG_NOTIMPL], m_pBattle->Engine().TurnSeq().CurUnit()->Owner()->Owner());
		//tdlg.DoModal();
		BeginAutobattle();
	} else if (uid == BCI_MSGLOG_BTN) {
		iDlg_BattleLog bldlg(m_pMgr, m_pBattle->Engine().TurnSeq().CurUnit()->Owner()->Owner(), m_log);
		bldlg.DoModal();
	} else if (uid == BCI_MODE_SWITCH) {
		if (cmd == CCI_TABCHANGED) {
			m_battleMode = (BattleNavMode)param;
		}
	} else if (uid == BCI_SETTINGS_BTN) {
		iSettingsDlg sdlg(&gApp.ViewMgr());
		sdlg.DoModal();
	}
}

/*
 *	Battle Toolbars
 */
iBattleToolBar::iBattleToolBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect)
: iView(pViewMgr, rect, GENERIC_VIEWPORT, 0, Enabled|Visible)
{
	sint32 xpos = 0;

	// Assaulter
	AddChild(m_pBtnAssaulter = new iHeroPortBtn(pViewMgr, pCmdHandler, iRect(xpos,1,32,20),BCI_ASSAULTER_BTN));
	xpos += 32;

	// splitter
	xpos += 1;
	AddChild(new iTBSplitter(pViewMgr, iRect(xpos,1,5,20)));
	xpos += 5+1;

	// Current creature mode tab
	m_pModeSwitch = new iBarTabSwitch(pViewMgr, pCmdHandler, iRect(xpos,1,BNM_COUNT*25,20), BNM_COUNT, BCI_MODE_SWITCH);
	m_pModeSwitch->SetTabIcon(PDGG_BTN_ATTACK);
	m_pModeSwitch->SetTabIcon(PDGG_BTN_SHOOT);
	m_pModeSwitch->SetTabIcon(PDGG_BTN_INFO);
	m_pModeSwitch->SetCurrentTab(BNM_MELEE);
	AddChild(m_pModeSwitch);
	xpos += BNM_COUNT * 25;

	// splitter
	xpos += 1;
	AddChild(new iTBSplitter(pViewMgr, iRect(xpos,1,5,20)));
	xpos += 5+1;

	// Wait and Defend buttons
	AddChild(m_pBtnWait = new iIconButton(pViewMgr,pCmdHandler, iRect(xpos,1,25,20),PDGG_BTN_ENDTURN,BCI_WAIT_BTN));
	xpos += 25;
	AddChild(m_pBtnDefend = new iIconButton(pViewMgr,pCmdHandler, iRect(xpos,1,25,20),PDGG_BTN_DEFEND,BCI_DEFEND_BTN));
	xpos += 25;
	
	// splitter
	xpos += 1;
	AddChild(new iTBSplitter(pViewMgr, iRect(xpos,1,5,20)));
	xpos += 5+1;

	// Cast spell button
	AddChild(m_pBtnCastSpell = new iIconButton(pViewMgr,pCmdHandler, iRect(xpos,1,24,20),PDGG_TAB_SPELLBOOK,BCI_CASTSPELL_BTN));
	xpos += 24;

	// Autobattle button
	AddChild(m_pBtnAutoBattle = new iIconButton(pViewMgr,pCmdHandler, iRect(xpos,1,24,20),PDGG_BTN_AUTOBATTLE,BCI_AUTOBATTLE_BTN));
	xpos += 24;

	// splitter
	xpos += 1;
	AddChild(new iTBSplitter(pViewMgr, iRect(xpos,1,5,20)));
	xpos += 5+1;

	// Battle message log
	AddChild(m_pBtnMsgLog = new iIconButton(pViewMgr,pCmdHandler, iRect(xpos,1,24,20),PDGG_TAB_QUESTLOG,BCI_MSGLOG_BTN));
	xpos += 24;

	// Settings button
	AddChild(m_pBtnSettings = new iIconButton(pViewMgr,pCmdHandler, iRect(xpos,1,24,20),PDGG_BTN_MAINMENU,BCI_SETTINGS_BTN));
	xpos += 24;

	// splitter
	xpos += 1;
	AddChild(new iTBSplitter(pViewMgr, iRect(xpos,1,5,20)));
	xpos += 5+1;

	// Defender
	AddChild(m_pBtnDefender = new iHeroPortBtn(pViewMgr, pCmdHandler, iRect(xpos,1,32,20),BCI_DEFENDER_BTN));
}

void iBattleToolBar::EnableControls(uint32 flags)
{
	bool bCommon = (flags&EnemyTurn) == 0 && (flags&Acting) == 0;

	m_pBtnAssaulter->SetEnabled(bCommon);
	m_pModeSwitch->SetEnabled(bCommon);
	m_pBtnWait->SetEnabled(bCommon && (flags&CanWait) != 0);
	m_pBtnDefend->SetEnabled(bCommon);
	m_pBtnCastSpell->SetEnabled(bCommon && (flags&CanCast) != 0);
	m_pBtnAutoBattle->SetEnabled(bCommon);
	m_pBtnMsgLog->SetEnabled(bCommon);
	m_pBtnSettings->SetEnabled(bCommon);
	if (m_pBtnDefender->Hero()) m_pBtnDefender->SetEnabled(bCommon);

	m_pModeSwitch->EnableTab(BNM_MELEE, (flags&CanMelee) != 0);
	m_pModeSwitch->EnableTab(BNM_SHOOT, (flags&CanShoot) != 0);
	m_pModeSwitch->EnableTab(BNM_INFO, (flags&CanInfo) != 0);
}

void iBattleToolBar::OnCompose()
{
	iRect rect = GetScrRect();
	gApp.Surface().FillRect(rect,cColor_Black);
}


/*
 * Spell toolbar
 */
iCastSpellToolBar::iCastSpellToolBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, iCombatSpell* pSpell)
: iView(pViewMgr, rect, GENERIC_VIEWPORT, 0, Enabled|Visible), m_pSpell(pSpell)
{
	// Cast spell button
	AddChild(new iTextButton(pViewMgr,pCmdHandler, iRect(rect.w-50,1,50,20), TRID_CANCEL, DRC_CANCEL));
}

void iCastSpellToolBar::OnCompose()
{
	iRect rect = GetScrRect();
	gApp.Surface().FillRect(rect,cColor_Black);

	iRect rc(rect.x, rect.y+1, rect.w-51, rect.h-1);
	gGfxMgr.BlitTile(PDGG_BKTILE, gApp.Surface(), rc);
	ButtonFrame(gApp.Surface(), rc, 0);
	gTextComposer.TextOut(dlgfc_hdr, gApp.Surface(), rc.point(), gTextMgr[TRID_SPNAME_MAGICARROW+m_pSpell->Id()], rc, AlignCenter);
}

/*
 * Spell toolbar
 */
iAutoBattleToolBar::iAutoBattleToolBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect)
: iView(pViewMgr, rect, GENERIC_VIEWPORT, 0, Enabled|Visible)
{
	// Cast spell button
	AddChild(new iTextButton(pViewMgr,pCmdHandler, iRect(rect.w-50,1,50,20), TRID_CANCEL, DRC_CANCEL));
}

void iAutoBattleToolBar::OnCompose()
{
	iRect rect = GetScrRect();
	gApp.Surface().FillRect(rect,cColor_Black);

	iRect rc(rect.x, rect.y+1, rect.w-51, rect.h-1);
	gGfxMgr.BlitTile(PDGG_BKTILE, gApp.Surface(), rc);
	ButtonFrame(gApp.Surface(), rc, 0);
	gTextComposer.TextOut(dlgfc_hdr, gApp.Surface(), rc.point(), gTextMgr[TRID_AUTO_COMBAT], rc, AlignCenter);
}

