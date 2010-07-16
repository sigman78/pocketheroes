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
 *	@brief Popup menu class implementation
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_WINDOW_POPUP_MENU_H_
#define _IOLIB_SILICON_WINDOW_POPUP_MENU_H_

#include "si.std.h"
#include "..\\he\\he.container.simple.array.h"

/// Popup menu item class 
class iMenuItem
{
public:

	/// Constructs menu item with specified params
	iMenuItem(iNWSimpleButton *btn, const iStringT &txt, bool enb)
	: m_ItemText(txt)
	, m_Button(btn)
	, m_Enabled(true)
	{
	}

	/// Destructor
	~iMenuItem()
	{
		delete m_Button;
		m_Button = NULL;
	}

	iNWSimpleButton	*m_Button;
	iStringT		m_ItemText;
	bool			m_Enabled;
};

/// Popup menu class 
class iPopupMenu : public iPopupWindow, public iNWSubscriberI
{
public:

	/// Destructor
	~iPopupMenu();

	/// Inserts new menu item with specified params
	void InsertMenuItem(const iDib &dib, const iStringT &txt = iStringT(_T("PopupItem")), bool enb=true);

	virtual bool OnNWMessage( iNWMessage& msg );
	virtual void MsgActivate(bool fActive, bool fMinimized);
	virtual void DrawBackground(iDib &odib, const iRect &rect = cInvalidRect);

	/// Hides popup menu
	void HidePopupMenu();
	/// Shows popup menu at specified position
	void ShowPopupMenu(const iPoint &pos, iAlignment algn);

	/// Initializes popup menu with specified params
	bool InitPopupMenu(iPopupNotifySubscriber *pwns, const iStringT &capt, const iSize &elsiz);

	/// Returns constant pointer to the specified menu item
	const iMenuItem* GetItem(uint32 item) const;

	virtual void MsgDestroy(void)
	{
		DestroyPopupMenuContent();
		iPopupWindow::MsgDestroy();
	}

private:
	/// Cleanups
	void DestroyPopupMenuContent()
	{
		for (uint32 xx=0; xx< m_Items.GetSize(); ++xx) delete m_Items[xx];
		m_Items.RemoveAll();
	}
	
	iStringT	m_Caption;
	iSize		m_ElementSize;


	iSimpleArray<iMenuItem*>	m_Items;
};

#endif //_IOLIB_SILICON_WINDOW_POPUP_MENU_H_
/** @} */ // iolib_si
