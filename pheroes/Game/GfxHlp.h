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

#ifndef _HMM_GAME_GRAPHIX_HELPERS_H_
#define _HMM_GAME_GRAPHIX_HELPERS_H_


//
const sint32 DLG_FRAME_SIZE  = 16;
const sint32 DEF_BTN_HEIGHT = 19;

//
extern iTextComposer::FontConfig dlgfc_hdr;
extern iTextComposer::FontConfig dlgfc_topic;
extern iTextComposer::FontConfig dlgfc_plain;
extern iTextComposer::FontConfig dlgfc_stopic;
extern iTextComposer::FontConfig dlgfc_splain;

extern iTextComposer::FontConfig btnfc_normal;
extern iTextComposer::FontConfig btnfc_pressed;
extern iTextComposer::FontConfig btnfc_disabled;

extern iGradient defGrad;


//
iStringT FormatShortNumber(sint32 val);
iStringT FormatNumber(sint32 val, bool bForseSign = false);

iStringT Mineral2Text(MINERAL_TYPE mt, sint32 quant);
iStringT MineralSet2Text(const iMineralSet& ms);
iStringT CreatQuant2Text( sint32 q, VISION_LEVEL vl);
iStringT CreatGroup2Text( CREATURE_TYPE ct, sint32 q, VISION_LEVEL vl);

void Tile3x3Sprite(iDib& dib, const iRect& rc, SpriteId sid);

void FrameRoundRect(iDib& surf, const iRect& rect, iDib::pixel clr);
void DrawRoundRect(iDib& surf, const iRect& rect, iDib::pixel fClr, iDib::pixel bClr);
void ComposeDlgBkgnd(iDib& surf, const iRect& rc, PLAYER_ID pid, bool bDecs);

void BlitIcon(iDib& dib, SpriteId sid, const iRect& orc, uint8 a=63, const iPoint& offset = iPoint()); // alpha - 0-63
void ButtonFrame(iDib& dib, const iRect& rc, uint32 state);
void ComposeIconButton(iDib& dib, const iRect& rc, uint32 state, SpriteId sid);
void ComposeProgressBar(bool bVertical, iDib& dib, const iRect& rc, iDib::pixel clr, sint32 cval, sint32 mval);

const iTextComposer::FontConfig& GetButtonFont(uint32 state);
void ComposeDlgButton(iDib& dib, const iRect& rc, uint32 state);
void ComposeDlgTextButton(iDib& dib, const iRect& rc, uint32 state, TextResId textKey);
void ComposeDlgIconButton(iDib& dib, const iRect& rc, uint32 state, SpriteId sid);
void ComposeDlgTab(iDib& dib, const iRect& rc, uint32 state, SpriteId sid);

void ComposeCreatureIcon(iDib& surf, const iRect& rc, CREATURE_TYPE ct, bool bFlipped = false);
void ComposeCreatureButton(iDib& surf, const iRect& rc, CREATURE_TYPE ct, sint32 cnt, VISION_LEVEL vl, bool bPressed, bool bDrag, bool bDrop);
void ComposeCreatureButtonWithText(iDib& surf, const iRect& rc, CREATURE_TYPE ct, iStringT text, VISION_LEVEL vl, bool bPressed, bool bDrag, bool bDrop);
void ComposeCreatureCell(iDib& surf, const iRect& rc, CREATURE_TYPE ct, sint32 cnt, VISION_LEVEL vl);
iSize CalcArmyCellsSize(const iSize& cellSize);
void ComposeArmyCells(iDib& surf, const iPoint& pos, const iSize& cellSize, const iArmy& army, VISION_LEVEL vl);
void ComposeSpellLevel(iDib& surf, const iRect& rect, MAGIC_SPELL spell, const iHero* pCaster);

void ComposeMoraleGlyph(iDib& surf, const iRect& orc, sint8 val);
void ComposeLuckGlyph(iDib& surf, const iRect& orc, sint8 val);

void FillStaredRect(iDib& surf, const iRect& rect, const iPoint& anchor);

#ifdef _DEBUG
void DbgDumpFurtSkills(const iStringT& hdr, const iFurtSkills& fsk);
#endif


#endif //_HMM_GAME_GRAPHIX_HELPERS_H_

