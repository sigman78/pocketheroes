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
#include "ChildDlgs.h"
#include "resource.h"

void ComposeVisualObject(iVisualObject& obj, iDib& surf, const iRect& rect)
{
	// Calculate object metrics
	iRect rc;
	iSpriteMgr& smgr = gItemMgr.m_SpriteMgr;
	for (uint32 aid=0; aid<obj.AvatarsCount(); ++aid) {
		const iAvatar& avatar = gItemMgr.m_AvatarMgr[obj.Avatar(aid).id];
		iIsoMetric im(4);
		iPoint avOffset = im.Map2Screen(obj.Avatar(aid).offset) - im.Map2Screen(iPoint());
		for (uint32 sid=0; sid<avatar.SpritesCount(); ++sid) {
			const iSprite* pSprite = smgr.GetSpritePtr(avatar.SpriteEntry(sid).spriteId);
			rc += iRect(pSprite->anchor + avatar.SpriteEntry(sid).anchor + avOffset, pSprite->dib.GetSize());
			//pSprite->dib.CopyToDibXY(&m_ComposeMemDC.m_Dib,pos + pSprite->anchor + avt.SpriteEntry(sid).anchor,BLEND_ALPHABLEND);
		}
	}

	check(!rc.IsEmpty());

	// Compose object to temporal dib
	iDib cmpDib(rc.size());
	for (aid=0; aid<obj.AvatarsCount(); ++aid) {
		const iAvatar& avatar = gItemMgr.m_AvatarMgr[obj.Avatar(aid).id];
		iIsoMetric im(4);
		iPoint avOffset = im.Map2Screen(obj.Avatar(aid).offset) - im.Map2Screen(iPoint());
		for (uint32 sid=0; sid<avatar.SpritesCount(); ++sid) {
			const iSprite* pSprite = smgr.GetSpritePtr(avatar.SpriteEntry(sid).spriteId);
			pSprite->dib.CopyToDibXY(&cmpDib, pSprite->anchor + avatar.SpriteEntry(sid).anchor + avOffset - rc.point(),BLEND_ALPHABLEND);
		}
	}

	// Scale dib if necessary
	if (rect.w < cmpDib.GetWidth() || rect.h < cmpDib.GetHeight()) {
		iDib strDib(cmpDib);
		iRect orc;
		ScaleRect2Rect(cmpDib.GetSize(), rect, orc);
		cmpDib.Resize(orc.size());
		iDibTransformer::Stretch(strDib, cmpDib, iDibTransformer::Fast);
		// Compose dib to surface
		cmpDib.CopyRectToDibXY(&surf, iRect(0,0,iMIN(rect.w, cmpDib.GetWidth()), iMIN(rect.h, cmpDib.GetHeight())), rect.point() + orc.point(), BLEND_ALPHABLEND);
	} else {
		// Compose dib to surface
		iPoint op(rect.x + (rect.w/2 - cmpDib.GetWidth()/2), rect.y + (rect.h/2 - cmpDib.GetHeight()/2));
		cmpDib.CopyToDibXY(&surf, op, BLEND_ALPHABLEND);
	}
}

void iItemBrowser::MakeObjectScheme(iVisualObject* pObj, iDib& dib)
{
	iIsoMetric im(1);
	iRect orc;
	iSize cs(im.GetCellWidth(), im.GetCellHeight());
	// Calculate size
	for (uint32 xx=0; xx<pObj->AvatarsCount(); ++xx) {
		orc += iRect(im.Map2Screen(pObj->Avatar(xx).offset),cs);
	}
	dib.Resize(orc.size());

	for (xx=0; xx<pObj->AvatarsCount(); ++xx) {
		if (pObj->Avatar(xx).offset.x == 0 && pObj->Avatar(xx).offset.y == 0) 
			m_dibCtlCell_y.CopyToDibXY(&dib, im.Map2Screen(pObj->Avatar(xx).offset) - orc.point(), BLEND_ALPHABLEND);
		else
			m_dibCtlCell_r.CopyToDibXY(&dib, im.Map2Screen(pObj->Avatar(xx).offset) - orc.point(), BLEND_ALPHABLEND);
	}
}

HWND iItemBrowser::CreateItemBrowser(HWND parent, ISelHandler* pSelHandler)
{
	m_pSelHandler = pSelHandler;
	iDibLoader::LoadFromResource(m_dibClp,IDB_TREE_CLP);
	iDibLoader::LoadFromResource(m_dibExp,IDB_TREE_EXP);
	iDibLoader::LoadFromResource(m_dibFolder,IDB_TREE_FOLDER);
	iDibLoader::LoadPng(m_dibCtlCell_r,GetAppPath()+_T("ctl_cellr.png"));
	iDibLoader::LoadPng(m_dibCtlCell_y,GetAppPath()+_T("ctl_celly.png"));
	if (!CreateBaseChildDlg(parent, iRect(0,0,1,1))) return NULL;
	m_TreeCtrl.CreateTreeCtrl( m_hWnd, RECT(iRect(0,0,1,1)), 7777,0, this,this,this);

	//
	m_TreeCtrl.SetRootNode(&gItemMgr);
	return m_hWnd;
}

void iItemBrowser::iTree_ComposeItem( iTreeCtrl* sender, iMemoryDC &memdc, const iRect& irect, iTreeNode* node, bool bSel )
{
	if (bSel){
		memdc.m_Dib.FillRect(irect, iColor(192,220,255,255));
		iDibPrimitives::DrawFrame(memdc.m_Dib,irect,iColor(128,160,255,255),BLEND_SRCCOPY);
	} else 	{
		memdc.m_Dib.FillRect(irect, cColorWhite);
	}

	iBaseNode* pNode = (iBaseNode*)node;
	const iSpriteMgr &spriteMgr = gItemMgr.m_SpriteMgr;

	if (iGroupNode* objGroupNode = DynamicCast<iGroupNode*>(pNode)) {
		if (objGroupNode->Count()){
			if (objGroupNode->IsOpen()) m_dibExp.CopyToDibXY(&memdc.m_Dib,iPoint(irect.x+2,irect.y+2),BLEND_ALPHABLEND);
			else m_dibClp.CopyToDibXY(&memdc.m_Dib,iPoint(irect.x+2,irect.y+2),BLEND_ALPHABLEND);
		}
		m_dibFolder.CopyToDibXY(&memdc.m_Dib,iPoint(irect.x+12,irect.y+2),BLEND_ALPHABLEND);
		iRect txt_rc(irect.x+54,irect.y+2,irect.w-56,15);
		memdc.SetTextColor(RGB(0,0,0));
		memdc.DrawTextRect(objGroupNode->NameKey(),txt_rc,true);
		txt_rc.y+=15;
		memdc.SetTextColor(RGB(128,128,128));
		memdc.DrawTextRect(iFormat(_T("%d item(s)"),objGroupNode->Count()),txt_rc,true);
	} else {
		iStringT text;
		iRect text_rc(irect.x+40,irect.y+5,irect.w-56,15);
		iColor text_color(cColorBlack);
		
		if (DynamicCast<iDecorNode*>(pNode) || DynamicCast<iBaseVisCnstNode*>(pNode) || DynamicCast<iBaseOwnCnstNode*>(pNode)) {
			iDecorNode* decorNode = (iDecorNode*)pNode;
			ComposeVisualObject(*decorNode, memdc.m_Dib, iRect(irect.x+1, irect.y+1, 36, irect.h-2));
			if (!decorNode->NameKey().Empty()) text = gItemMgr.m_TextMgr[decorNode->NameKey()];
			else {
				for (uint32 xx=0; xx<decorNode->AvatarsCount(); ++xx) {
					if (xx) text += _T(", ");
					const iAvatar& avt = gItemMgr.m_AvatarMgr[decorNode->Avatar(xx).id];
					text += gItemMgr.m_TextMgr[avt.NameKey()];
				}
			}
			memdc.SetTextColor(text_color);
			memdc.DrawTextRect(text,text_rc,true);
		} else if (iCastleNode* pCastle = DynamicCast<iCastleNode*>(pNode)) {
			iDib scheme;
			MakeObjectScheme(pCastle,scheme);
			scheme.CopyToDibXY(&memdc.m_Dib, iPoint(irect.x + 32 - (sint32)(scheme.GetWidth()/2), irect.y + (sint32)(irect.h/2) - (sint32)(scheme.GetHeight()/2) ), BLEND_ALPHABLEND);
			memdc.SetTextColor(text_color);
			memdc.DrawTextRect(text,text_rc,true);
		} else if (iPathElNode* pathNode = DynamicCast<iPathElNode*>(pNode)) {
			text = gItemMgr.m_TextMgr[pathNode->NameKey()];
			const iDib& dib = spriteMgr[pathNode->SpriteConfig(5)].dib;
			iRect orc = iDibHelpers::FindSolidArea(dib);
			dib.CopyToDibXY(&memdc.m_Dib,irect.point()-orc.point(),BLEND_ALPHABLEND);
			memdc.SetTextColor(text_color);
			memdc.DrawTextRect(text,text_rc,true);
		} else if (iHeroNode* heroNode = DynamicCast<iHeroNode*>(pNode)) {
			const iDib& port = spriteMgr.GetSpritePtr(heroNode->SmallIcon())->dib;
			port.CopyToDibXY(&memdc.m_Dib, iPoint(irect.x+2, irect.y+2), BLEND_SRCCOPY);
			if (!heroNode->IsAvailable()) text_color = 0x80a080;
			text.Setf(_T("%s (%s)"),gItemMgr.m_TextMgr[heroNode->NameKey()].CStr(),gItemMgr.m_TextMgr[HERO_TYPE_NAMEKEY[heroNode->Type()]]);
			memdc.SetTextColor(text_color);
			memdc.DrawTextRect(text,text_rc,true);
		} else if (iArtifactNode* pArtNode =  DynamicCast<iArtifactNode*>(pNode)) {
			const iDib& dib = spriteMgr[pArtNode->Sprite()].dib;
			dib.CopyToDibXY(&memdc.m_Dib,iPoint(irect.x + irect.h/2 - dib.GetWidth()/2, irect.y + irect.h/2 - dib.GetHeight()/2),BLEND_ALPHABLEND);
			memdc.SetTextColor(text_color);
			text_rc.y -= 3;
			memdc.DrawTextRect(gItemMgr.m_TextMgr[pArtNode->NameKey()],text_rc,true);
			memdc.SetTextColor(RGB(128,128,128));
			text_rc.y += 15;
			memdc.DrawTextRect(gItemMgr.m_TextMgr[pArtNode->DescKey()],text_rc,true);
		} else if (iGuardNode* pGuardNode = DynamicCast<iGuardNode*>(pNode)) {
			const iDib& dib = spriteMgr[pGuardNode->Sprite()].dib;
			iRect orc = iDibHelpers::FindSolidArea(dib);
			dib.CopyToDibXY(&memdc.m_Dib,irect.point()-orc.point(),BLEND_ALPHABLEND);
			text = gItemMgr.m_TextMgr[pGuardNode->NameKey()];
			memdc.SetTextColor(text_color);
			memdc.DrawTextRect(text,text_rc,true);
		} else if (iMineralNode* pMineralNode = DynamicCast<iMineralNode*>(pNode)) {
			const iDib& dib = spriteMgr[(pMineralNode->MineralType()==MINERAL_UNKNOWN)?_T("resources.rand_res_icn"):iFormat(_T("resources.icons_%04d"),pMineralNode->MineralType())].dib;
			dib.CopyToDibXY(&memdc.m_Dib,iPoint(irect.x + irect.h/2 - dib.GetWidth()/2, irect.y + irect.h/2 - dib.GetHeight()/2),BLEND_ALPHABLEND);
			memdc.SetTextColor(text_color);
			memdc.DrawTextRect(gItemMgr.m_TextMgr[pMineralNode->NameKey()],text_rc,true);
		} else if (DynamicCast<iManaCrystalNode*>(pNode)) {
			const iDib& dib = spriteMgr[_T("resources.mcryst")].dib;
			dib.CopyToDibXY(&memdc.m_Dib,iPoint(irect.x + irect.h/2 - dib.GetWidth()/2, irect.y + irect.h/2 - dib.GetHeight()/2),BLEND_ALPHABLEND);
			memdc.SetTextColor(text_color);
			memdc.DrawTextRect(gItemMgr.m_TextMgr[pNode->NameKey()],text_rc,true);
		} else if (DynamicCast<iCampFireNode*>(pNode)) {
			const iDib& dib = spriteMgr[_T("resources.campfire")].dib;
			dib.CopyToDibXY(&memdc.m_Dib,iPoint(irect.x + irect.h/2 - dib.GetWidth()/2, irect.y + irect.h/2 - dib.GetHeight()/2),BLEND_ALPHABLEND);
			memdc.SetTextColor(text_color);
			memdc.DrawTextRect(gItemMgr.m_TextMgr[pNode->NameKey()],text_rc,true);
		} else if (DynamicCast<iChestNode*>(pNode)) {
			const iDib& dib = spriteMgr[_T("resources.chest")].dib;
			dib.CopyToDibXY(&memdc.m_Dib,iPoint(irect.x + irect.h/2 - dib.GetWidth()/2, irect.y + irect.h/2 - dib.GetHeight()/2),BLEND_ALPHABLEND);
			memdc.SetTextColor(text_color);
			memdc.DrawTextRect(gItemMgr.m_TextMgr[pNode->NameKey()],text_rc,true);
		} else if (DynamicCast<iSpellScrollNode*>(pNode)) {
			const iDib& dib = spriteMgr[_T("resources.spell_scroll")].dib;
			dib.CopyToDibXY(&memdc.m_Dib,iPoint(irect.x + irect.h/2 - dib.GetWidth()/2, irect.y + irect.h/2 - dib.GetHeight()/2),BLEND_ALPHABLEND);
			memdc.SetTextColor(text_color);
			memdc.DrawTextRect(gItemMgr.m_TextMgr[pNode->NameKey()],text_rc,true);
		} else if (DynamicCast<iLampNode*>(pNode)) {
			const iDib& dib = spriteMgr[_T("resources.lamp")].dib;
			dib.CopyToDibXY(&memdc.m_Dib,iPoint(irect.x + irect.h/2 - dib.GetWidth()/2, irect.y + irect.h/2 - dib.GetHeight()/2),BLEND_ALPHABLEND);
			memdc.SetTextColor(text_color);
			memdc.DrawTextRect(gItemMgr.m_TextMgr[pNode->NameKey()],text_rc,true);
		} else if (iKeyGuardNode* pKeyGuardNode = DynamicCast<iKeyGuardNode*>(pNode)) {
			const iDib& dib = spriteMgr[iFormat(_T("resources.key_guards_%04d"),pKeyGuardNode->m_key)].dib;
			dib.CopyToDibXY(&memdc.m_Dib,iPoint(irect.x + irect.h/2 - dib.GetWidth()/2, irect.y + irect.h/2 - dib.GetHeight()/2),BLEND_ALPHABLEND);
			memdc.SetTextColor(text_color);
			memdc.DrawTextRect(gItemMgr.m_TextMgr[pNode->NameKey()],text_rc,true);
		} else if (DynamicCast<iMapEventNode*>(pNode)) {
			const iDib& dib = spriteMgr[_T("common.event")].dib;
			dib.CopyToDibXY(&memdc.m_Dib,iPoint(irect.x + irect.h/2 - dib.GetWidth()/2, irect.y + irect.h/2 - dib.GetHeight()/2),BLEND_ALPHABLEND);
			memdc.SetTextColor(text_color);
			memdc.DrawTextRect(gItemMgr.m_TextMgr[pNode->NameKey()],text_rc,true);
		} else if (DynamicCast<iUltimateArtNode*>(pNode)) {
			const iDib& dib = spriteMgr[_T("common.ultimate")].dib;
			dib.CopyToDibXY(&memdc.m_Dib,iPoint(irect.x + irect.h/2 - dib.GetWidth()/2, irect.y + irect.h/2 - dib.GetHeight()/2),BLEND_ALPHABLEND);
			memdc.SetTextColor(text_color);
			memdc.DrawTextRect(gItemMgr.m_TextMgr[pNode->NameKey()],text_rc,true);
		} else {
			text = gItemMgr.m_TextMgr[pNode->NameKey()];
			memdc.SetTextColor(text_color);
			memdc.DrawTextRect(text,text_rc,true);
		}

	}

}
