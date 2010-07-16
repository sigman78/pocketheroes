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
 *	@brief NonWindowed tooltip control class implementation
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_BASE_NWCONTROL_WITH_TOOLTIP_H_
#define _IOLIB_SILICON_BASE_NWCONTROL_WITH_TOOLTIP_H_

#include "si.nwcontrol.base.h"
#include "si.tooltip.disp.h"

/// NonWindowed tooltip control class
class iNWControlTT : public iNWControl
{
public:
	/// Default constructor
	iNWControlTT()
	: m_pttipDisp(NULL)
	{}

	/// Creates and initializes tooltip control
	bool CreateNWElementTT(iNWElementContainerI *parent, const iRect &rect, iToolTipDispatcherI* pDisp=NULL, const iStringT &ttip=_T(""));

	/// Sets tooltip text
	void SetToolTipText(const iStringT& ttip);
	/// Returns tooltip text
	const iStringT& GetToolTipText() const;
	/// Sets tooltip control dispatcher
	void SetToolTipDisp(iToolTipDispatcherI* pDisp);
	/// Returns pointer to the tooltip control dispatcher
	iToolTipDispatcherI* GetToolTipDisp() const;

	virtual void OnMouseEnter();
	virtual void OnMouseLeave();

private:
	iToolTipDispatcherI*	m_pttipDisp;
	iStringT				m_ttipText;
};


#endif //_IOLIB_SILICON_BASE_NWCONTROL_WITH_TOOLTIP_H_
/** @} */ // iolib_si
