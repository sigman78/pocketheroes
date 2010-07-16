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
#include "GfxHlp.h"

// Gradients for common font styles

const iDib::pixel gradBtnText[13] = {
	RGB16(255,160,0), RGB16(255,164,10), RGB16(255,169,21), RGB16(255,175,36), RGB16(255,181,52), RGB16(255,188,68), RGB16(255,195,85), RGB16(255,203,105), RGB16(255,211,123), 
	RGB16(255,218,140), RGB16(255,224,155), RGB16(255,230,170), RGB16(255,235,180)
};

iGradient defGrad(gradBtnText, 13);

// Common font styles
iTextComposer::FontConfig dlgfc_hdr(iTextComposer::FS_LARGE, iDibFont::ComposeProps(RGB16(255,180,64)));
iTextComposer::FontConfig dlgfc_topic(iTextComposer::FS_MEDIUM, iDibFont::ComposeProps(RGB16(255,255,0)));
iTextComposer::FontConfig dlgfc_plain(iTextComposer::FS_MEDIUM, iDibFont::ComposeProps(RGB16(210,210,220)));
iTextComposer::FontConfig dlgfc_stopic(iTextComposer::FS_SMALL, iDibFont::ComposeProps(RGB16(255,255,0)));
iTextComposer::FontConfig dlgfc_splain(iTextComposer::FS_SMALL, iDibFont::ComposeProps(RGB16(210,210,220)));

iTextComposer::FontConfig btnfc_normal(iTextComposer::FS_MEDIUM, iDibFont::ComposeProps(defGrad, cColor_Black, iDibFont::DecBorder ));
iTextComposer::FontConfig btnfc_pressed(iTextComposer::FS_MEDIUM, iDibFont::ComposeProps(defGrad, cColor_Black, iDibFont::DecBorder ));
iTextComposer::FontConfig btnfc_disabled(iTextComposer::FS_MEDIUM, iDibFont::ComposeProps(RGB16(160,96,32), cColor_Black, iDibFont::DecBorder ));


//
const sint32 DLG_SHADOW_OFFSET = 5;

// Dialog background shading

#define iabs(x) ( (x) < 0 ? -(x) : (x) )
#define ipow2(x) ( (x)*(x) )

sint32 dist_a( sint32 dx, sint32 dy )
{
	if ( dx > dy ) {
		return ( 61685 * dx + 26870 * dy ) >> 16;
	} else {
		return ( 61685 * dy + 26870 * dx ) >> 16;
	}
}

sint32 dist_b( sint32 dx, sint32 dy )
{
	return dx + dy - (dx < dy ? dx : dy) / 2;
}

struct grad_shader
{
	sint32	rx;
	sint32	ry;			
	sint32	rw;
	sint32	rh;
	
	enum { Max_Height = 256 };
	enum { Max_Radii  = 256+128 };
	
	uint32	vshade[ Max_Height ];
	uint32	rshade[ Max_Radii  ];
	
	void init( sint32 x, sint32 y, sint32 w, sint32 h )
	{
		rx = x;	ry = y;
		rw = w; rh = h;
	}
	
	void precalc()
	{
		// total presetup cost is:
		// height: sqrt, div
		// 256   : div
		const sint32 half = rh / 2;
		// vshade upper
		vshade[0] = 128;
		sint32 n;
		for( n = 0; n != half; n++ ) {
			sint32 arg = n;
			// base is: 1 - sqrt(x)
			sint32 val = 65535 - 256 * int_sqrt( (arg * 65536) / half );
			// safe 4'th power - faster falloff
			val >>= 8;
			val = val * val;
			val >>= 8;
			val = val * val;
			vshade[n] = val;
		}
		// vshare lower
		for( ; n != rh; n++ ) {
			sint32 arg = half - (n - half);
			// base is: 1 - sqrt(x)
			sint32 val = 65536 - 256 * int_sqrt( (arg * 65536) / half );
			vshade[n] = val >> 2;
		}
		
		// rshade [ 0..255..x ]
		sint32 radii = rh > rw ? rw : rh;
		check( radii <= Max_Radii );
		for( sint32 r = 0; r != Max_Radii; r++ ) {
			sint32 arg = (256 * r) / radii;
			sint32 val = 65536 - ( arg * arg );
			rshade[r] = val > 0 ? val >> 1 : 0;
		}
	}
	
	void draw( iDib::pixel* ptr, uint32 stride )
	{	
		// setup circle centre points
		sint32 px = rx + rw / 2;
		sint32 py = ry + rh - rh / 4;
		
		for( sint32 yy = 0; yy != rh; yy++ ) {
			iDib::pixel* pline = (ptr) + (yy+ry) * stride + rx;
			sint32 ady = iabs( py - (ry + yy) );
			// tight pixel loop
			for( sint32 xx = 0; xx != rw; xx++ ) {
				sint32 dx = px - (rx + xx);
				// fast eq.dist approximation
				sint32 dst = dist_a( iabs(dx), ady ); 
				// combined and normalized
				sint32 val = (rshade[ dst ] + vshade[ yy ] ) >> 8;
				// correct overflow (its possible to get rid of 
				// if we normalize beforehead, but wtf? )
				val = val > 255 ? 255 : val;
				// apply gamma and inverse
				val = 255 - (ipow2( val ) >> 8);
				// factor is ready to multiply: [0..255] range
				uint16 src = *pline;
				uint16 sr = val * ((src & (0x1F<<11)) >> 11);
				uint16 sg = val * ((src & (0x3F<<5)) >> 5);
				uint16 sb = val * ((src & (0x1F)));
				*pline++ = (((sr)>>8)<<11 | ((sg)>>8)<<5 | ((sb)>>8));
			}
		}
	}
};

// end of dialog background shading

enum QUANTITY_VAL {
	QVAL_FEW = 0,
	QVAL_SEVERAL,
	QVAL_PACK,
	QVAL_LOTS,
	QVAL_HORDE,
	QVAL_THRONG,
	QVAL_SWARM,
	QVAL_LEGION,
	QVAL_COUNT
};

inline QUANTITY_VAL QuantityVal(sint32 q)
{
	if (q < 5) return QVAL_FEW;
	else if (q < 10) return QVAL_SEVERAL;
	else if (q < 20) return QVAL_PACK;
	else if (q < 50) return QVAL_LOTS;
	else if (q < 100) return QVAL_HORDE;
	else if (q < 250) return QVAL_THRONG;
	else if (q < 500) return QVAL_SWARM;
	else return QVAL_LEGION;
}

iStringT FormatShortNumber(sint32 val)
{
	if (val >= 1000000) {
		return iFormat(_T("%dM"), val/1000000);
	} else if (val >= 1000) {
		return iFormat(_T("%dK"), val/1000);
	} else return iFormat(_T("%d"), val);
}

iStringT FormatNumber(sint32 val, bool bForseSign)
{
	iCharT postfix = 0;
	if (val >= 100000000) {
		val /= 1000000;
		postfix = 'M';
	} else if (val >= 100000) {
		val /= 1000;
		postfix = 'K';
	}

	iStringT result;
	if (bForseSign && val > 0) result += _T("+");
	result.Addf(_T("%d"), val);
	if (postfix)  result.Addf(_T("%c"), postfix);
	return result;
	//
}

iStringT CreatQuant2Text( sint32 q, VISION_LEVEL vl)
{
	if (vl == VL_BASIC) return gTextMgr[TRID_QUANT_FEW+QuantityVal(q)];
	else if (vl == VL_EXPERT) return FormatNumber(q);
	return iStringT(_T("?"));
}

iStringT CreatGroup2Text( CREATURE_TYPE ct, sint32 q, VISION_LEVEL vl)
{
	return CreatQuant2Text(q, vl) + _T(" ")+gTextMgr[TRID_CREATURE_PEASANT_F3+ct*3];
}

iStringT Mineral2Text(MINERAL_TYPE mt, sint32 quant)
{
	return iFormat(_T("#I%04X%d"), PDGG_RES_MINI_ICONS+mt, quant);
}

iStringT MineralSet2Text(const iMineralSet& ms)
{
	iStringT res;
	for (uint32 xx=0; xx<MINERAL_COUNT; ++xx){
		if (ms.quant[xx]) res.Addf(_T("#I%04X%d  "),PDGG_RES_MINI_ICONS+xx, ms.quant[xx]);
	}
	if (res.Empty()) res = _T("-");
	return res;
}

void Tile3x3Sprite(iDib& dib, const iRect& rc, SpriteId sid)
{
	iRect orc(rc);

	iSize ssiz = gGfxMgr.Dimension(sid);
	check( (ssiz.w % 3) == 0 && (ssiz.h % 3) == 0);
	sint32 elw = ssiz.w / 3;
	sint32 elh = ssiz.h / 3;
	
	sint32 xcnt = ((sint32)rc.w - elw * 2 + elw - 1) / elw;
	sint32 ycnt = ((sint32)rc.h - elh * 2 + elh - 1) / elh;
	sint32 xpos = rc.x + elw;
	sint32 ypos = rc.y + elh;
	sint32 xx,yy;
	
	for (yy=0; yy<ycnt; ++yy) {
		for (xx=0; xx<xcnt; ++xx){
			gGfxMgr.Blit(sid, dib, iRect(elw,elh,elw,elh), iPoint(xpos,ypos));
			xpos += elw;
		}
		ypos += elh;
		xpos = rc.x+elw;
	}
	
	xpos = rc.x+elw;
	for (xx=0; xx < xcnt; ++xx) {
		gGfxMgr.Blit(sid, dib, iRect(elw,0,elw,elh), iPoint(xpos,rc.y));
		gGfxMgr.Blit(sid, dib, iRect(elw,elh*2,elw,elh), iPoint(xpos,rc.y+rc.h-elh));
		xpos += elw;
	}
	
	ypos = rc.y+elh;
	for (yy=0; yy < ycnt; ++yy) {
		gGfxMgr.Blit(sid, dib, iRect(0,elh,elw,elh), iPoint(rc.x,ypos));
		gGfxMgr.Blit(sid, dib, iRect(elw*2,elh,elw,elh), iPoint(rc.x+rc.w-elw,ypos));
		ypos += elh;
	}
	
	gGfxMgr.Blit(sid, dib, iRect(0,0,elw,elh), iPoint(rc.x,rc.y));
	gGfxMgr.Blit(sid, dib, iRect(0,elh*2,elw,elh), iPoint(rc.x,rc.y+rc.h-elh));
	gGfxMgr.Blit(sid, dib, iRect(elw*2,0,elw,elh), iPoint(rc.x+rc.w-elw,rc.y));
	gGfxMgr.Blit(sid, dib, iRect(elw*2,elh*2,elw,elh), iPoint(rc.x+rc.w-elw,rc.y+rc.h-elh));
}


//////////////////////////////////////////////////////////////////////////
void FrameRoundRect(iDib& surf, const iRect& rect, iDib::pixel clr)
{
	surf.HLine(iPoint(rect.x+1, rect.y), rect.x+rect.w-2, clr);
	surf.HLine(iPoint(rect.x+1, rect.y+rect.h-1), rect.x+rect.w-2, clr);
	surf.VLine(iPoint(rect.x,rect.y+1), rect.y+rect.h-1,clr);
	surf.VLine(iPoint(rect.x+rect.w-1,rect.y+1), rect.y+rect.h-1,clr);
	surf.PutPixel(rect.x+1, rect.y+1, clr);
	surf.PutPixel(rect.x+rect.w-2, rect.y+1, clr);
	surf.PutPixel(rect.x+1, rect.y+rect.h-2, clr);
	surf.PutPixel(rect.x+rect.w-2, rect.y+rect.h-2, clr);
}

void DrawRoundRect(iDib& surf, const iRect& rect, iDib::pixel fClr, iDib::pixel bClr)
{
	surf.FillRect(iRect(rect.x+1, rect.y+1, rect.w-2, rect.h-2), bClr);
	FrameRoundRect(surf, rect, fClr);
}


void ComposeDlgBkgnd(iDib& surf, const iRect& rect, PLAYER_ID pid, bool bDecs)
{
	uint32 nval = (pid+1);
	uint32 toffs = (bDecs)?12:10;
	if (rect.IsEmpty()) return;

	// shadow
	surf.Darken50Rect(iRect(rect.x2()+1, rect.y+DLG_SHADOW_OFFSET,DLG_SHADOW_OFFSET,rect.h));
	surf.Darken50Rect(iRect(rect.x+DLG_SHADOW_OFFSET, rect.y2()+1,rect.w-DLG_SHADOW_OFFSET,DLG_SHADOW_OFFSET));
	
	// tile background
	iRect bkRect(rect);
	bkRect.DeflateRect(10);
	gGfxMgr.BlitTile(bDecs?PDGG_BKTILE2:PDGG_BKTILE, surf,bkRect);

	// Shade
	if (bDecs) {
		grad_shader shader;
		shader.init( bkRect.x, bkRect.y, bkRect.w, bkRect.h);
		shader.precalc();
		shader.draw( surf.GetPtr(), surf.GetWidth() );
	}


	// top/bottom tiles
	gGfxMgr.BlitTile(PDGG_DLG_HTILES+nval, surf, iRect(rect.x+toffs,rect.y,rect.w-toffs*2,10));
	gGfxMgr.BlitTile(PDGG_DLG_HTILES+nval, surf, iRect(rect.x+toffs,rect.y2()-9,rect.w-toffs*2,10));

	iDib hgr(iSize(rect.w-(toffs*2),1), iDib::RGB);
	hgr.HGradientRect(iRect(0,0,hgr.GetWidth()/2,1), RGB16(64,0,0), RGB16(255,192,64));
	hgr.HGradientRect(iRect(hgr.GetWidth()/2,0,hgr.GetWidth()/2,1), RGB16(255,192,64), RGB16(64,0,0));
	hgr.CopyToDibXY(surf, iPoint(rect.x+toffs, rect.y+1));
	hgr.CopyToDibXY(surf, iPoint(rect.x+toffs, rect.y+8));
	hgr.CopyToDibXY(surf, iPoint(rect.x+toffs, rect.y2()-8));
	hgr.CopyToDibXY(surf, iPoint(rect.x+toffs, rect.y2()-1));

	// left/right tiles
	gGfxMgr.BlitTile(PDGG_DLG_VTILES+nval, surf, iRect(rect.x,rect.y+toffs,10,rect.h-toffs*2));
	gGfxMgr.BlitTile(PDGG_DLG_VTILES+nval, surf, iRect(rect.x2()-9,rect.y+toffs,10,rect.h-toffs*2));
	
	iDib vgr(iSize(1,rect.h-(toffs*2)), iDib::RGB);
	vgr.VGradientRect(iRect(0,0,1,vgr.GetHeight()/2), RGB16(64,0,0), RGB16(255,192,64));
	vgr.VGradientRect(iRect(0,vgr.GetHeight()/2,1,vgr.GetHeight()/2), RGB16(255,192,64), RGB16(64,0,0));
	vgr.CopyToDibXY(surf, iPoint(rect.x+1, rect.y+toffs));
	vgr.CopyToDibXY(surf, iPoint(rect.x+8, rect.y+toffs));
	vgr.CopyToDibXY(surf, iPoint(rect.x2()-8, rect.y+toffs));
	vgr.CopyToDibXY(surf, iPoint(rect.x2()-1, rect.y+toffs));

	// corners
	if (bDecs){
		gGfxMgr.Blit(PDGG_DLG_CORNERS+0, surf, iPoint(rect.x-2,rect.y-2));
		gGfxMgr.Blit(PDGG_DLG_CORNERS+1, surf, iPoint(rect.x2()-27,rect.y-2));
		gGfxMgr.Blit(PDGG_DLG_CORNERS+2, surf, iPoint(rect.x-2,rect.y2()-27));
		gGfxMgr.Blit(PDGG_DLG_CORNERS+3, surf, iPoint(rect.x2()-27,rect.y2()-27));
	} else {
		gGfxMgr.Blit(PDGG_DLG_CORNERS_SMALL+nval, surf, iPoint(rect.x,rect.y));
		gGfxMgr.Blit(PDGG_DLG_CORNERS_SMALL+nval, surf, iPoint(rect.x2()-9,rect.y));
		gGfxMgr.Blit(PDGG_DLG_CORNERS_SMALL+nval, surf, iPoint(rect.x,rect.y2()-9));
		gGfxMgr.Blit(PDGG_DLG_CORNERS_SMALL+nval, surf, iPoint(rect.x2()-9,rect.y2()-9));
	}
}

void BlitIcon(iDib& dib, SpriteId sid, const iRect& orc, uint8 a, const iPoint& offset)
{
	iSize ssiz = gGfxMgr.Dimension(sid);
	iPoint offs = gGfxMgr.Anchor(sid);
	iPoint op(orc.x+((sint32)(orc.w/2)-(sint32)(ssiz.w/2))-offs.x+offset.x, orc.y+((sint32)(orc.h/2)-(sint32)(ssiz.h/2))-offs.y+offset.y);
	if (a==63) gGfxMgr.Blit( sid, dib, op);
	else gGfxMgr.BlitAlpha( sid, dib, op,a);
}

void ButtonFrame(iDib& dib, const iRect& rc, uint32 state)
{
	iDib::pixel ltClr = (state&iButton::Pressed)?cColor_Black:cColor_White;
	iDib::pixel rbClr = (state&iButton::Pressed)?cColor_White:cColor_Black;
	dib.HLine(iPoint(rc.x+1,rc.y),rc.x2()-1,ltClr,64);
	dib.HLine(iPoint(rc.x+1,rc.y2()),rc.x2()-1,rbClr,64);
	dib.VLine(iPoint(rc.x,rc.y+1),rc.y2(),ltClr,64);
	dib.VLine(iPoint(rc.x2(),rc.y+1),rc.y2(),rbClr,64);
	//if (state&iButton::Disabled) dib.Darken25Rect(rc);
}

const iTextComposer::FontConfig& GetButtonFont(uint32 state)
{
	if (state & iButton::Disabled) return btnfc_disabled;
	else if (state & iButton::Pressed) return btnfc_pressed;
	else return btnfc_normal;
}

void ComposeDlgButton(iDib& dib, const iRect& rc, uint32 state)
{
	iRect orc(rc);
	SpriteId btnSid; 
	if (state&iButton::Disabled) btnSid = PDGG_DLG_BTN;
	else if (state&iButton::Pressed) btnSid = PDGG_DLG_BTN + 1;
	else btnSid = PDGG_DLG_BTN + 2;

	// Compose outer frame
	iRect rect = rc;
	rect.InflateRect(1);
	dib.HLine(iPoint(rect.x+2, rect.y), rect.x+rect.w-3, 0);
	dib.HLine(iPoint(rect.x+2, rect.y+rect.h-1), rect.x+rect.w-3, 0);
	dib.VLine(iPoint(rect.x,rect.y+2), rect.y+rect.h-2, 0);
	dib.VLine(iPoint(rect.x+rect.w-1,rect.y+2), rect.y+rect.h-2, 0);

	// Compose button background
	Tile3x3Sprite(dib, rc, btnSid);
}

void ComposeDlgTextButton(iDib& dib, const iRect& rc, uint32 state, TextResId textKey)
{
	ComposeDlgButton(dib, rc, state);

	// Compose text
	if (textKey != TRID_VOID) {
		iTextComposer::FontConfig fc (GetButtonFont(state));
		gTextComposer.TextOut(fc,dib,rc,gTextMgr[textKey],rc,AlignCenter, (state & iButton::Pressed)?iPoint(1,1):iPoint());
	}
}

void ComposeDlgIconButton(iDib& dib, const iRect& rc, uint32 state, SpriteId sid)
{
	ComposeDlgButton(dib, rc, state);

	// Compose Icon
	iSize ssiz = gGfxMgr.Dimension(sid);
	iPoint op(rc.x+((sint32)(rc.w/2)-(sint32)(ssiz.w/2)), rc.y+((sint32)(rc.h/2)-(sint32)(ssiz.h/2)));
	BlitIcon( dib, sid, rc, (state&iButton::Disabled)?32:63, (state & iButton::Pressed)?iPoint(1,1):iPoint());
}

void ComposeDlgTab(iDib& dib, const iRect& rc, uint32 state, SpriteId sid)
{
	if (state&iButton::Selected) {
		gGfxMgr.BlitTile(PDGG_BKTILE,dib,rc);
	}
	iDib::pixel ltClr = (state&iButton::Pressed)?cColor_Black:cColor_White;
	iDib::pixel rbClr = (state&iButton::Pressed)?cColor_White:cColor_Black;
	uint8 a = (state&iButton::Selected)?64:32;
	dib.HLine(iPoint(rc.x+1,rc.y2()),rc.x2()-1,rbClr,a);
	dib.VLine(iPoint(rc.x,rc.y+1),rc.y2(),ltClr,a);
	dib.VLine(iPoint(rc.x2(),rc.y+1),rc.y2(),rbClr,a);

	if (state&iButton::Selected) BlitIcon( dib, sid, rc);
	else BlitIcon( dib, sid, rc,32);
}

void ComposeIconButton(iDib& dib, const iRect& rc, uint32 state, SpriteId sid)
{
	gGfxMgr.BlitTile(PDGG_BKTILE,dib,rc);
	ButtonFrame(dib,rc,state);
	
	if (state&iButton::Selected) dib.FillRect(rc, RGB16(255,192,0), 96);

	iSize ssiz = gGfxMgr.Dimension(sid);
	iPoint op(rc.x+((sint32)(rc.w/2)-(sint32)(ssiz.w/2)), rc.y+((sint32)(rc.h/2)-(sint32)(ssiz.h/2)));
	if (state&iButton::Disabled) dib.Darken25Rect(rc);
	BlitIcon( dib, sid, rc, (state&iButton::Disabled)?32:63);
}

void ComposeProgressBar(bool bVertical, iDib& dib, const iRect& rc, iDib::pixel clr, sint32 cval, sint32 mval)
{
	iDib::pixel bclr = (iDib::pixel)Darken50( clr );
	dib.FillRect(rc,bclr);
	iRect trc;
	if (bVertical) {
		sint32 mg = iCLAMP<sint32>(0,rc.h,(cval * rc.h) / mval);
		trc = iRect(rc.x,rc.y+rc.h-mg,rc.w,mg);
	} else {
		sint32 mg = iCLAMP<sint32>(0,rc.w,(cval * rc.w) / mval);
		trc = iRect(rc.x,rc.y,mg,rc.h);
	}
	dib.FillRect(trc,clr);

	dib.HLine(iPoint(rc.x,rc.y),rc.x+rc.w-2,cColor_White,48);
	dib.VLine(iPoint(rc.x,rc.y+1),rc.y+rc.h-1,cColor_White,48);
	dib.HLine(iPoint(rc.x+1,rc.y+rc.h-1),rc.x+rc.w-1,cColor_Black,48);
	dib.VLine(iPoint(rc.x+rc.w-1,rc.y+1),rc.y+rc.h-1,cColor_Black,48);
}

void ComposeCreatureIcon(iDib& surf, const iRect& rc, CREATURE_TYPE ct, bool bFlipped)
{
	SpriteId sid = ((bFlipped)?PDGG_RMINIMON:PDGG_MINIMON) + ct;
	
	sint32 sw = gGfxMgr.Dimension(sid).w;
	sint32 sh = gGfxMgr.Dimension(sid).h;
	iPoint sa = gGfxMgr.Anchor(sid);

	sint32 dw = (sint32)(rc.w/2) - sw/2;
	sint32 dh = sh-(sint32)(rc.h/2) - sh/2;
	
	sint32 ox,oy,sx,sy;

	if (dw > 0) {
		sx = 0;
		ox = rc.x+dw-sa.x;
	} else {
		if (!bFlipped) sx = sw - (sint32)(rc.w);
		else sx = 0;
		sw = rc.w;
		ox = rc.x-sa.x;
	}

	oy = rc.y-sa.y;
	sy = 0;
	sh = iMIN<sint32>(sh,rc.h);

	gGfxMgr.Blit(sid, surf, iRect(sx,sy,sw,sh), iPoint(ox,oy));
}

void ComposeCreatureButton(iDib& surf, const iRect& rc, CREATURE_TYPE ct, sint32 cnt, VISION_LEVEL vl, bool bPressed, bool bDrag, bool bDrop)
{
	if (ct != CREAT_UNKNOWN) surf.FillRect(rc,cColor_Gray128,32);
	else surf.Darken25Rect(rc);
	ButtonFrame(surf, rc, bPressed);
	if (ct != CREAT_UNKNOWN && !bDrag) {
		ComposeCreatureIcon(surf,iRect(rc.x+2,rc.y+2,rc.w-4,rc.h-4),ct);
		surf.Darken50Rect(iRect(rc.x,rc.y2()-10,rc.w,11));
		iTextComposer::FontConfig fc((vl == VL_BASIC)?iTextComposer::FS_SMALL:iTextComposer::FS_MEDIUM, RGB16(192,192,192));
		gTextComposer.TextOut(fc, surf, iPoint(rc.x,rc.y2()-10), CreatQuant2Text(cnt,vl),iRect(rc.x,rc.y2()-10,rc.w,11),AlignCenter);
	}
	
	if (bDrag) surf.FrameRect(rc,cColor_Blue,64);
	if (bDrop) surf.FrameRect(rc,cColor_Yello,64);
}

void ComposeCreatureButtonWithText(iDib& surf, const iRect& rc, CREATURE_TYPE ct, iStringT text, VISION_LEVEL vl, bool bPressed, bool bDrag, bool bDrop)
{
	if (ct != CREAT_UNKNOWN) surf.FillRect(rc,cColor_Gray128,32);
	else surf.Darken25Rect(rc);
	ButtonFrame(surf, rc, bPressed);
	if (ct != CREAT_UNKNOWN && !bDrag) {
		ComposeCreatureIcon(surf,iRect(rc.x+2,rc.y+2,rc.w-4,rc.h-4),ct);
		surf.Darken50Rect(iRect(rc.x,rc.y2()-10,rc.w,11));
		iTextComposer::FontConfig fc((vl == VL_BASIC)?iTextComposer::FS_SMALL:iTextComposer::FS_MEDIUM, RGB16(192,192,192));
		gTextComposer.TextOut(fc, surf, iPoint(rc.x,rc.y2()-10), text,iRect(rc.x,rc.y2()-10,rc.w,11),AlignCenter);
	}
	
	if (bDrag) surf.FrameRect(rc,cColor_Blue,64);
	if (bDrop) surf.FrameRect(rc,cColor_Yello,64);
}

void ComposeCreatureCell(iDib& surf, const iRect& rc, CREATURE_TYPE ct, sint32 cnt, VISION_LEVEL vl)
{
	if (ct != CREAT_UNKNOWN) surf.FillRect(rc,cColor_Gray128,32);
	else surf.FillRect(rc,cColor_Black,32);
	ButtonFrame(surf, rc, 0);
	if (ct != CREAT_UNKNOWN) {
		ComposeCreatureIcon(surf,iRect(rc.x+2,rc.y+2,rc.w-4,rc.h-4),ct);
		surf.Darken50Rect(iRect(rc.x,rc.y2()-10,rc.w,11));
		iTextComposer::FontConfig fc((vl == VL_BASIC)?iTextComposer::FS_SMALL:iTextComposer::FS_MEDIUM, RGB16(192,192,192));
		gTextComposer.TextOut(fc, surf, iPoint(rc.x,rc.y2()-10), CreatQuant2Text(cnt,vl),iRect(rc.x,rc.y2()-10,rc.w,11),AlignCenter);
	}
}

iSize CalcArmyCellsSize(const iSize& cellSize)
{
	return iSize(cellSize.w*4+3, cellSize.h*2+1);
}

void ComposeArmyCells(iDib& surf, const iPoint& pos, const iSize& cellSize, const iArmy& army, VISION_LEVEL vl)
{
	iPoint p(pos);
	for (uint32 xx=0; xx<3; ++xx) {
		ComposeCreatureCell(surf, iRect(p.x+15,p.y,cellSize.w,cellSize.h), army[xx].Type(), army[xx].Count(), vl);
		p.x += 31;
	}
	
	p.y+=cellSize.h+1;
	p.x = pos.x;
	for (uint32 xx=3; xx<7; ++xx) {
		ComposeCreatureCell(surf, iRect(p.x,p.y,cellSize.w,cellSize.h), army[xx].Type(), army[xx].Count(), vl);
		p.x += 31;
	}
}

void ComposeSpellLevel(iDib& surf, const iRect& rect, MAGIC_SPELL spell, const iHero* pCaster)
{
	sint32 spLvl = SPELL_DESCRIPTORS[spell].level + 1;
	sint32 msLvl = pCaster->FurtSkill((FURTHER_SKILLS)(FSK_MAG_AIR+SPELL_DESCRIPTORS[spell].school));
	sint32 p = rect.x + rect.w/2-18;
	for (sint32 xx=1; xx<=3; ++xx) {
		SpriteId sid = PDGG_MSCHICN_S+SPELL_DESCRIPTORS[spell].school;
		if (msLvl<xx) sid += 4;
		gGfxMgr.Blit(sid, surf, iPoint(p, rect.y));
		p += 12;
	}
}

void ComposeMoraleGlyph(iDib& surf, const iRect& orc, sint8 val)
{
	if (!val) {
		SpriteId sid = PDGG_DGLYPH_MORALE+1;
		iSize siz = gGfxMgr.Dimension(sid);
		sint32 opx = orc.x + (sint32)(orc.w/2) - (sint32)(siz.w/2) - gGfxMgr.Anchor(sid).x;
		sint32 opy = orc.y + (sint32)(orc.h/2) - (sint32)(siz.h/2) - gGfxMgr.Anchor(sid).x;
		gGfxMgr.Blit(sid, gApp.Surface(), iPoint(opx,opy));
	} else {
		SpriteId sid = (val>0)?PDGG_DGLYPH_MORALE:(PDGG_DGLYPH_MORALE+2);
		sint32 cval = iMIN<sint8>(iABS(val), 3);
		iSize siz = gGfxMgr.Dimension(sid);
		siz.w += (cval-1) * 6;
		sint32 opx = orc.x + (sint32)(orc.w/2) - (sint32)(siz.w/2) - gGfxMgr.Anchor(sid).x;
		sint32 opy = orc.y + (sint32)(orc.h/2) - (sint32)(siz.h/2) - gGfxMgr.Anchor(sid).x;
		for (sint32 xx=0; xx<cval; ++xx) {
			gGfxMgr.Blit(sid, gApp.Surface(), iPoint(opx,opy));
			opx += 6;
		}
	}
}

void ComposeLuckGlyph(iDib& surf, const iRect& orc, sint8 val)
{
	if (val <= 0) {
		SpriteId sid = (!val)?PDGG_DGLYPH_LUCK+1:PDGG_DGLYPH_LUCK+2;
		iSize siz = gGfxMgr.Dimension(sid);
		sint32 opx = orc.x + (sint32)(orc.w/2) - (sint32)(siz.w/2) - gGfxMgr.Anchor(sid).x;
		sint32 opy = orc.y + (sint32)(orc.h/2) - (sint32)(siz.h/2) - gGfxMgr.Anchor(sid).x;
		gGfxMgr.Blit(sid, gApp.Surface(), iPoint(opx,opy));
	} else {
		SpriteId sid = PDGG_DGLYPH_LUCK;
		sint32 cval = iMIN<sint8>(iABS(val), 3);
		iSize siz = gGfxMgr.Dimension(sid);
		siz.w += (cval-1) * 6;
		sint32 opx = orc.x + (sint32)(orc.w/2) - (sint32)(siz.w/2) - gGfxMgr.Anchor(sid).x;
		sint32 opy = orc.y + (sint32)(orc.h/2) - (sint32)(siz.h/2) - gGfxMgr.Anchor(sid).x;
		for (sint32 xx=0; xx<cval; ++xx) {
			gGfxMgr.Blit(sid, gApp.Surface(), iPoint(opx,opy));
			opx += 6;
		}
	}
}

const uint16 stars[4] = { RGB16(220,220,220), RGB16(192,192,192), RGB16(160,160,160), RGB16(128,128,128)};

void FillStaredRect(iDib& surf, const iRect& rect, const iPoint& anchor)
{
	iRandomizer rand;
	surf.FillRect(rect, cColor_Black);
	for (uint32 nn=0; nn<2048; ++nn) {
		sint32 px = (rand.Rand() - anchor.x) % 320;
		sint32 py = (rand.Rand() - anchor.y) % 240;
		if (rect.PtInRect(px, py)) surf.PutPixel(px,py,stars[nn&3]);
	}
}

/*
 *	
 */
#ifdef _DEBUG
LPCTSTR FURT_SKILLS_NAMES[FSK_COUNT] = {
	_T("Attack"), _T("Defence"), _T("Spell power"), _T("Knowledge"),
	_T("ActPoints"), _T("Logistics"), _T("Pathfinding"), _T("Scouting"), _T("Vision"),
	_T("Ballistics"), _T("Learning"), _T("Diplomacy"), _T("Necromancy"),
	_T("Sorcery"), _T("ManaPoints"), _T("Intelligence"), _T("Wisdom"),
	_T("AirMagic"), _T("EarthMagic"), _T("FireMagic"), _T("WaterMagic"),
	_T("AirResistance"), _T("EarthResistance"), _T("FireResistance"), _T("WaterResistance"),
	_T("Archery"), _T("Offence"), _T("AirShield"), _T("Shield"), _T("Armorer"), _T("RangeAttack"), _T("MeleeAttack"), _T("Resistance"),
	_T("CreatHits"), _T("CreatSpeed"), _T("Morale"), _T("Luck"), _T("Counterstrike"),
	_T("Gold"), _T("Ore"), _T("Wood"), _T("Mercury"), _T("Gems"), _T("Crystal"), _T("Sulfur")
};

void DbgDumpFurtSkills(const iStringT& hdr, const iFurtSkills& fsk)
{
	OutputDebugString(_T("*********************************\n"));
	OutputDebugString(hdr.CStr());
	OutputDebugString(_T(":\n"));
	OutputDebugString(_T("*********************************\n"));
	for (uint32 xx=0; xx<FSK_COUNT; ++xx){
		if (fsk.Value((FURTHER_SKILLS)xx) != 0) {
			OutputDebugString(iFormat(_T("%s: %d\n"),FURT_SKILLS_NAMES[xx],fsk.Value((FURTHER_SKILLS)xx)).CStr());
		}
	}
	OutputDebugString(_T("------------------------------\n"));
}
#endif

void ShowLoadingBox()
{
	iDisplay& display = gApp.Display();
	iRect rcScreen = display.SurfMetrics();
	iPoint ldrOrg  = rcScreen.Center();
	//ldrOrg.y += rcScreen.h / 3;
	uint32 pbarWidth = (rcScreen.w * 5) / 6;
	iRect rcPbar( ldrOrg.x - pbarWidth/2, ldrOrg.y-22, pbarWidth, 44 );

	iDib& surf = display.GetSurface();
	surf.Darken50Rect(rcPbar);
	surf.Darken50Rect(rcPbar);
	static const wchar_t loadingTxt[] = _T("Loading...");
	gTextComposer.TextOut( dlgfc_hdr, surf, rcPbar.TopLeft(), loadingTxt, rcPbar, AlignCenter  );
	//gTextComposer.TextOut(surf,iPoint(rc.x+34,rc.y),iFormat(_T("#FFF5%s"),gTextMgr[TRID_HSKILL_ESTATES+ss]),iRect(rc.x+34,rc.y,rc.w-30,18),AlignLeft);
	display.DoPaint( display.SurfMetrics() );
}


