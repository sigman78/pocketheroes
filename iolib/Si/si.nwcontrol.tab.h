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

/** @file
 *	@brief NonWindowed tab control class implementation
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_NWCONTROL_TAB_H_
#define _IOLIB_SILICON_NWCONTROL_TAB_H_

#include "si.tooltip.disp.h"

/// NonWindowed tab control class 
class iNWTabControl : public iNWControl
{
public:
	/// Tab control orientation enumeration
	enum Orientation {
		Vertical,
		Horizontal
	};

	/// Default constructor
	iNWTabControl();
	/// Creates and initializes  tab control
	bool CreateNWTabControl(iNWElementContainerI *parent, 
							const iRect &rect, 
							const iDib &dib, 
							uint32 state_cnt, 
							Orientation ornt,
							bool ex_mode = false);

	virtual void OnMouseClick(const iPoint &pos);
	virtual void OnLButtonDown(const iPoint &pos, uint32 vkey);
	virtual void OnLButtonUp(const iPoint &pos, uint32 vkey);

	virtual void DrawElement(iMemoryDC &ddc) const;
	virtual HCURSOR GetCursor(const iPoint &pos) const;

	/// Sets new tab state
	void SetTabState(uint32 nstate = 0);
	/// Returns current tab state
	uint32 GetTabState(void) const;

	virtual void OnMouseMove(const iPoint& pos);
	virtual void OnMouseLeave();

	/// Sets tab control tooltip information
	void SetToolTipInfo(iToolTipDispatcherI* pDisp, const iStringList& ttip);

private:
	uint32 GetIdByPos(const iPoint &pos);

	uint32			m_StateCnt;
	Orientation		m_Orientation;
	uint32			m_CurrentState;
	iDib			m_Dib;
	uint32			m_ElementWidth;
	uint32			m_ElementHeight;

	bool			m_ExMode;
	uint32			m_TrackId;

	iToolTipDispatcherI*	m_pttipDisp;
	iStringList				m_ttipStrings;
	uint32					m_ttipId;
};


#endif //_IOLIB_SILICON_NWCONTROL_TAB_H_
/** @} */ // iolib_si
