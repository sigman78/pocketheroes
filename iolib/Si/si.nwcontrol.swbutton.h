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
 *	@brief NonWindowed switch button control class implementation
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_NWCONTROL_SWITCH_BUTTON_H_
#define _IOLIB_SILICON_NWCONTROL_SWITCH_BUTTON_H_

#include "si.nwcontrol.base.ttip.h"

/// NonWindowed switch button control 
class iNWSwitchButton : public iNWControlTT
{
public:
	/// Default constructor
	iNWSwitchButton();
	/// Creates and initializes  switch button control
	bool CreateNWSwitchButton(iNWElementContainerI *parent, const iRect &rect, const iDib &dib, uint32 state_cnt, iToolTipDispatcherI* pDisp=NULL, const iStringT &ttip=_T(""));

	virtual void OnMouseClick(const iPoint &pos);
	virtual void OnLButtonDown(const iPoint &pos, uint32 vkey);
	virtual void OnLButtonUp(const iPoint &pos, uint32 vkey);

	virtual void DrawElement(iMemoryDC &ddc) const;
	virtual HCURSOR GetCursor(const iPoint &pos) const;

	/// Sets new switch state
	void SetSwitchState(uint32 nstate=0);
	/// Returns current switch state
	uint32 GetSwitchState(void) const;

private:

	iDib	m_Dib;
	uint32	m_StateCnt;
	uint32	m_CurrentState;
};

#endif //_IOLIB_SILICON_NWCONTROL_SWITCH_BUTTON_H_
/** @} */ // iolib_si
