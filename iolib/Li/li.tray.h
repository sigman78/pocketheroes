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
 *	@brief System tray element class implementation
 */
/** @addtogroup iolib_li
 *  @{
 */
#ifndef _IOLIB_LITHIUM_TRAY_ICON_H_
#define _IOLIB_LITHIUM_TRAY_ICON_H_

/// Wrapper class for the Win32 NOTIFYICONDATA structure
class CNotifyIconData : public NOTIFYICONDATA
{
public:	
	/// Default constructor
	CNotifyIconData()
	{
		memset(this, 0, sizeof(NOTIFYICONDATA));
		cbSize = sizeof(NOTIFYICONDATA);
	}
};

/// @brief System tray element class 
/// Template used to support adding an icon to the taskbar. This class will maintain a taskbar icon and associated context menu.
template <class T>
class iTrayIconImpl
{
private:
	UINT WM_TRAYICON;
	CNotifyIconData m_nid;
	bool m_bInstalled;
	UINT m_nDefault;
public:	
	/// Default constructor
	iTrayIconImpl() : m_bInstalled(false), m_nDefault(0)
	{
		WM_TRAYICON = ::RegisterWindowMessage(_T("WM_TRAYICON"));
	}
	
	/// Destructor
	~iTrayIconImpl()
	{
		// Remove the icon
		RemoveIcon();
	}

	/// Installs a taskbar icon
	// 	\param lpszToolTip 	The tooltip to display
	//	\param hIcon The icon to display
	// 	\param nID The resource ID of the context menu
	bool InstallIcon(LPCTSTR lpszToolTip, HICON hIcon, UINT nID)
	{
		T* pT = static_cast<T*>(this);
		// Fill in the data		
		m_nid.hWnd = pT->m_hWnd;
		m_nid.uID = nID;
		m_nid.hIcon = hIcon;
		m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		m_nid.uCallbackMessage = WM_TRAYICON;
		_tcscpy(m_nid.szTip, lpszToolTip);
		// Install
		m_bInstalled = Shell_NotifyIcon(NIM_ADD, &m_nid) ? true : false;
		// Done
		return m_bInstalled;
	}

	/// Removes taskbar icon
	bool RemoveIcon()
	{
		if (!m_bInstalled)
			return false;
		// Remove
		m_nid.uFlags = 0;
		return Shell_NotifyIcon(NIM_DELETE, &m_nid) ? true : false;
	}

	/// Set the icon tooltip text
	bool SetTooltipText(LPCTSTR pszTooltipText)
	{
		if (pszTooltipText == NULL)
			return FALSE;
		// Fill the structure
		m_nid.uFlags = NIF_TIP;
		_tcscpy(m_nid.szTip, pszTooltipText);
		// Set
		return Shell_NotifyIcon(NIM_MODIFY, &m_nid) ? true : false;
	}

	/// Set the default menu item ID
	inline void SetDefaultItem(UINT nID) { m_nDefault = nID; }

	BEGIN_MSG_MAP(iTrayIconImpl)
		MESSAGE_HANDLER(WM_TRAYICON, OnTrayIcon)
	END_MSG_MAP()

	LRESULT OnTrayIcon(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		// Is this the ID we want?
		if (wParam != m_nid.uID)
			return 0;
		T* pT = static_cast<T*>(this);
		// Was the right-button clicked?
		if (LOWORD(lParam) == WM_RBUTTONUP)
		{
			// Load the menu
			CMenu oMenu;
			if (!oMenu.LoadMenu(m_nid.uID))
				return 0;
			// Get the sub-menu
			CMenuHandle oPopup(oMenu.GetSubMenu(0));
			// Prepare
			pT->PrepareMenu(oPopup);
			// Get the menu position
			CPoint pos;
			GetCursorPos(&pos);
			// Make app the foreground
			SetForegroundWindow(pT->m_hWnd);
			// Set the default menu item
			if (m_nDefault == 0)
				oPopup.SetMenuDefaultItem(0, TRUE);
			else
				oPopup.SetMenuDefaultItem(m_nDefault);
			// Track
			oPopup.TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, pT->m_hWnd);
			// BUGFIX: See "PRB: Menus for Notification Icons Don't Work Correctly"
			pT->PostMessage(WM_NULL);
			// Done
			oMenu.DestroyMenu();
		}
		else if (LOWORD(lParam) == WM_LBUTTONDBLCLK)
		{
			// Make app the foreground
			SetForegroundWindow(pT->m_hWnd);
			// Load the menu
			CMenu oMenu;
			if (!oMenu.LoadMenu(m_nid.uID))
				return 0;
			// Get the sub-menu
			CMenuHandle oPopup(oMenu.GetSubMenu(0));			
			// Get the item
			if (m_nDefault)
			{
				// Send
				pT->SendMessage(WM_COMMAND, m_nDefault, 0);
			}
			else
			{
				UINT nItem = oPopup.GetMenuItemID(0);
				// Send
				pT->SendMessage(WM_COMMAND, nItem, 0);
			}
			// Done
			oMenu.DestroyMenu();
		}
		return 0;
	}

	/// Allow the menu items to be enabled/checked/etc.
	virtual void PrepareMenu(HMENU hMenu)
	{
		// Stub
	}
};

#endif //_IOLIB_LITHIUM_TRAY_ICON_H_
/** @} */ // iolib_li
