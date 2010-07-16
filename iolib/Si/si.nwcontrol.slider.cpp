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

#include "si.common.h"
#include "si.nwcontrol.base.h"
#include "si.nwcontrol.slider.h"

iNWSliderBar::iNWSliderBar()
: m_Cur(0)
, m_Min(0)
, m_Max(1000)
, m_bTracking(false)
, m_TrackPos(0)
{
}

bool iNWSliderBar::CreateNWSliderBar(iNWElementContainerI *parent, const iRect &rect, const iDib &back_dib, const iDib &bar_dib, const iDib &handle_dib)
{
	m_dibBack = back_dib;
	m_dibBar = bar_dib;
	m_dibHandle = handle_dib;

	return CreateNWElement(parent,rect);
}

void iNWSliderBar::DrawElement(iMemoryDC &ddc) const
{
	iRect elrc = GetElementRect();
	iSize bsiz(m_dibBack.GetWidth()/3,m_dibBack.GetHeight()/2);

	// Draw Background
	sint32 ypos = (m_IsEnabled?0:bsiz.h);
	m_dibBack.CopyRectToDibXY(&ddc.m_Dib, iRect(0,ypos,bsiz.w,bsiz.h),iPoint(elrc.x,elrc.y),BLEND_ALPHABLEND);
	m_dibBack.TileRectToDibRect(iRect(bsiz.w,ypos,bsiz.w,bsiz.h),ddc.m_Dib,iRect(elrc.x+bsiz.w,elrc.y,elrc.w-bsiz.w*2,bsiz.h),BLEND_ALPHABLEND);
	m_dibBack.CopyRectToDibXY(&ddc.m_Dib, iRect(bsiz.w*2,ypos,bsiz.w,bsiz.h),iPoint(elrc.x+elrc.w-bsiz.w,elrc.y),BLEND_ALPHABLEND);

	// Create Bar (fake impl)
	iSize belsiz(m_dibBar.GetWidth()/3,m_dibBar.GetHeight()/2);
	iSize barsiz(elrc.w - bsiz.w*2,belsiz.h);

	iDib eBar(barsiz); iDib fBar(barsiz);
	m_dibBar.CopyRectToDibXY(&eBar, iRect(0,0,belsiz.w,belsiz.h),iPoint(0,0),BLEND_SRCCOPY);
	m_dibBar.TileRectToDibRect(iRect(belsiz.w,0,belsiz.w,belsiz.h),eBar,iRect(belsiz.w,0,barsiz.w-belsiz.w*2,barsiz.h),BLEND_SRCCOPY);
	m_dibBar.CopyRectToDibXY(&eBar, iRect(belsiz.w*2,0,belsiz.w,belsiz.h),iPoint(barsiz.w-belsiz.w,0),BLEND_SRCCOPY);

	m_dibBar.CopyRectToDibXY(&fBar, iRect(0,belsiz.h,belsiz.w,belsiz.h),iPoint(0,0),BLEND_SRCCOPY);
	m_dibBar.TileRectToDibRect(iRect(belsiz.w,belsiz.h,belsiz.w,belsiz.h),fBar,iRect(belsiz.w,0,barsiz.w-belsiz.w*2,barsiz.h),BLEND_SRCCOPY);
	m_dibBar.CopyRectToDibXY(&fBar, iRect(belsiz.w*2,belsiz.h,belsiz.w,belsiz.h),iPoint(barsiz.w-belsiz.w,0),BLEND_SRCCOPY);

	// Compose
	iPoint bar_pos(elrc.x+bsiz.w,elrc.y+(bsiz.h/2 - barsiz.h/2));
	sint32 hpos;
	sint32 cpos = m_bTracking?m_TrackPos:m_Cur;
	if (!m_IsEnabled || cpos == m_Min || m_Min == m_Max){
		hpos = 0;
		eBar.CopyToDibXY(&ddc.m_Dib,bar_pos,BLEND_ALPHABLEND);
	} else if (cpos == m_Max) {
		hpos = barsiz.w;
		fBar.CopyToDibXY(&ddc.m_Dib,bar_pos,BLEND_ALPHABLEND);
	} else {
		hpos = iCLAMP<sint32>((sint32)0,(sint32)barsiz.w,(sint32)barsiz.w * (cpos-m_Min) / (m_Max-m_Min));
		fBar.CopyRectToDibXY(&ddc.m_Dib,iRect(0,0,hpos,barsiz.h),iPoint(bar_pos.x,bar_pos.y),BLEND_ALPHABLEND);
		eBar.CopyRectToDibXY(&ddc.m_Dib,iRect(hpos,0,barsiz.w-hpos,barsiz.h),iPoint(bar_pos.x+hpos,bar_pos.y),BLEND_ALPHABLEND);
	}

	// Draw Handle
	if (m_IsEnabled) {
		iSize hndsiz(m_dibHandle.GetWidth()/2,m_dibHandle.GetHeight());
		m_dibHandle.CopyRectToDibXY(&ddc.m_Dib, iRect(m_bTracking?hndsiz.w:0,0,hndsiz.w,hndsiz.h),iPoint(bar_pos.x+hpos-(sint32)(hndsiz.w/2),bar_pos.y+barsiz.h/2-hndsiz.h/2),BLEND_ALPHABLEND);
	}
}

void iNWSliderBar::SetCur(sint32 cur, bool refMsg )
{
	m_Cur = iCLAMP(m_Min,m_Max,cur);
	if (refMsg) GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_SLDPOS_CHANGED,0,&m_Cur));
	ForceRepaint();
}

void iNWSliderBar::SetMinMax(sint32 min, sint32 max)
{
	m_Min = iMIN(min,max);
	m_Max = iMAX(min,max);
	m_Cur = iCLAMP(m_Min,m_Max,m_Cur);
	ForceRepaint();
}

void iNWSliderBar::SetAll(sint32 cur, sint32 min, sint32 max)
{
	m_Min = iMIN(min,max);
	m_Max = iMAX(min,max);
	m_Cur = iCLAMP(m_Min,m_Max,cur);
	ForceRepaint();
}

sint32 iNWSliderBar::Pix2Pos(sint32 pix_pos)
{
	sint32 boff = m_dibBack.GetWidth()/3;
	sint32 bw = GetElementWidth() - boff*2;
	return iCLAMP<sint32>(m_Min,m_Max,m_Min+(pix_pos-boff)*(m_Max-m_Min)/bw);
}

void iNWSliderBar::OnLButtonDown(const iPoint& pos, uint32 vkey)
{
	m_bTracking = true;
	m_TrackPos = Pix2Pos(pos.x);
	GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_SLDPOS_TRACKING,0,&m_TrackPos));

	iNWControl::OnLButtonDown(pos,vkey);
}

void iNWSliderBar::OnLButtonUp(const iPoint& pos, uint32 vkey)
{
	if (m_bTracking) {
		m_bTracking = false;
		SetCur(m_TrackPos);
	}
	iNWControl::OnLButtonUp(pos,vkey);
}

void iNWSliderBar::OnMouseMove(const iPoint& pos)
{
	if (m_bTracking){
		m_TrackPos = Pix2Pos(pos.x);
		GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_SLDPOS_TRACKING,0,&m_TrackPos));
		ForceRepaint();
	}
	iNWControl::OnMouseMove(pos);
}
