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
 *	@brief NonWindowed push button with popup menu control class implementation
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_NWCONTROL_POPUPMENU_BUTTON_H_
#define _IOLIB_SILICON_NWCONTROL_POPUPMENU_BUTTON_H_

/// NonWindowed push button with popup menu control
class iNWPopupButton : public iNWControl, public iPopupNotifySubscriber
{
public:
	virtual void OnPopupNotify(uint32 uMsg, sint32 lParam);


	/// Creates and initializes  push button with popup menu control
	bool CreateNWPopupButton(iNWElementContainerI *parent, const iRect &rect, const iDib &dib, uint32 state_cnt);

	virtual void OnMouseClick(const iPoint &pos);
	virtual void DrawElement(iMemoryDC &ddc) const;
	virtual HCURSOR GetCursor(const iPoint &pos) const;
	virtual void DestroyElement(void);

	/// Sets current (selected) item
	void SetTabState(uint32 nstate = 0);
	/// Returns current (selected) item
	uint32 GetTabState(void) const;


private:
	iPopupMenu	m_PopupMenu;
	uint32		m_CurSel;
};

#endif //_IOLIB_SILICON_NWCONTROL_POPUPMENU_BUTTON_H_
/** @} */ // iolib_si
