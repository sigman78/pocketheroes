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

#ifndef _HMM_MINIMAP_VIEW_H_
#define _HMM_MINIMAP_VIEW_H_

const sint32 ZOOM_MIN = 1;
const sint32 ZOOM_MAX = 4;

class iMinimapView : public iChildGameView
{
public:
	iMinimapView();

	void OnMouseDown(const iPoint& pos);
	void OnMouseUp(const iPoint& pos);
	void OnMouseTrack(const iPoint& pos);
	
	void PrepareSurface();

	void SetCenterCell(const iPoint& pos);
	iPoint GetCenterCell() const { return m_CenterCell; }

	void OnZoomChanged();
	sint32 GetScale() const	{ return m_Scale; }
	inline void SetZoom(sint32 nScale) { m_Scale = iCLAMP<sint32>(ZOOM_MIN, ZOOM_MAX, nScale); }
	inline void ZoomIn() { m_Scale = iMIN<sint32>(m_Scale+1,ZOOM_MAX); OnZoomChanged(); }
	inline void ZoomOut() { m_Scale = iMAX<sint32>(m_Scale-1,ZOOM_MIN); OnZoomChanged(); }

	inline iPoint Screen2Map(const iPoint& pos) const
	{
		sint32 mx = ((pos.x-(sint32)(m_Rect.w/2))/m_Scale+pos.y/m_Scale)/2;
		sint32 my = pos.y/m_Scale-mx;
		return iPoint(mx,my);
	}

	inline iPoint Map2Screen(const iPoint& pos) const
	{ return iPoint((-pos.y+pos.x)*m_Scale+m_Rect.w/2,(pos.x+pos.y)*m_Scale); }

	inline iPoint GetOffset() const
	{ return iPoint(m_Rect.w/2,m_Rect.h/2) - Map2Screen(m_CenterCell); }

	void OnCompose();

private:
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

private:
	sint32			m_Scale;
	iBuff<uint16>	m_Surf;
	iPoint			m_CenterCell;
	iPoint			m_TrackCell;
};

/*
 *	Minimap Toolbar 
 */
class iMinimapToolBar : public iView
{
public:
	iMinimapToolBar(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect);
	void OnCompose();
};

#endif //_HMM_MINIMAP_VIEW_H_

