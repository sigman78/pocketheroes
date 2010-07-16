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
 *	@brief NonWindowed (4 state) push button control class implementation
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_NWCONTROL_BUTTON_H_
#define _IOLIB_SILICON_NWCONTROL_BUTTON_H_

#include "si.nwcontrol.base.ttip.h"

/// NonWindowed (4 state) push button control class [Disabled/Focused/Pressed/Unpressed(Normal)]
class iNWButton : public iNWControlTT
{
public:
	/// Default constructor
	iNWButton();
	/// Creates and initializes push button control
	bool CreateNWButton(iNWElementContainerI *parent, const iRect &rect, const iDib &dib, iToolTipDispatcherI* pDisp=NULL, const iStringT &ttip=_T(""));

	// iNWControl
	virtual void OnMouseEnter(void);
	virtual void OnMouseLeave(void);
	virtual void OnMouseClick(const iPoint &pos);
	virtual void OnLButtonDown(const iPoint &pos, uint32 vkey);
	virtual void OnLButtonUp(const iPoint &pos, uint32 vkey);

	virtual void DrawElement(iMemoryDC &ddc) const;
	virtual HCURSOR GetCursor(const iPoint &pos) const;

private:
	iDib	m_Dibs[4];

	/// Button state enumeration
	enum BtnState {
		BtnDisabled = 0,
		BtnFocused,
		BtnPressed,
		BtnUnpressed
	};

	BtnState	m_BtnState;
};

#endif //_IOLIB_SILICON_NWCONTROL_BUTTON_H_
/** @} */ // iolib_si
