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
 *	@brief Common GUI helper functions implementation
 */
/** @addtogroup iolib_li
 *  @{
 */
#ifndef _IOLIB_LITHIUM_HELPER_METHODS_H_
#define _IOLIB_LITHIUM_HELPER_METHODS_H_

const uint32 DLGTEXT_BUFF_SIZE	= 1024;

/// Common GUI helper functions workspace
namespace LI
{
	/// Returns caption text of specified window
	inline iStringT GetWndText(HWND hwnd)
	{
		iCharT buff[DLGTEXT_BUFF_SIZE] = {0};
		::GetWindowText(hwnd,buff,DLGTEXT_BUFF_SIZE);
		return iStringT(buff);
	}

	/// Sets caption text for specified dialog item
	inline void SetDlgItemText(HWND dlgHwnd, iDlgItem dlgItemId, const iStringT &text)
	{
		::SetDlgItemText(dlgHwnd,dlgItemId,text.CStr());
	}

	/// Returns caption text of specified dialog item
	inline void GetDlgItemText(HWND dlgHwnd, iDlgItem dlgItemId, iStringT &text)
	{
		iCharT buff[DLGTEXT_BUFF_SIZE] = {0};
		::GetDlgItemText(dlgHwnd,dlgItemId,buff,DLGTEXT_BUFF_SIZE);
		text = iStringT(buff);
	}

	/// Returns caption text of specified dialog item
	inline iStringT GetDlgItemText(HWND dlgHwnd, iDlgItem dlgItemId)
	{
		iStringT result;
		GetDlgItemText(dlgHwnd,dlgItemId,result);
		return result;
	}

	/// Sets integer value for specified dialog item
	inline void SetDlgItemInt(HWND dlgHwnd, iDlgItem dlgItemId, sint32 value)
	{
		uint32 uval = iABS(value);
		::SetDlgItemInt(dlgHwnd, dlgItemId,uval,(value < 0));
	}

	/// Returns integer value of specified dialog item
	inline sint32 GetDlgItemInt(HWND dlgHwnd, iDlgItem dlgItemId)
	{
		BOOL bRes = TRUE;
		sint32 result = (sint32)::GetDlgItemInt(dlgHwnd, dlgItemId,&bRes,TRUE);

		if (!bRes) return 0;

		return result;
	}

	/// Returns position of specified dialog item
	inline iPoint GetDlgItemPos(HWND dlgHwnd, iDlgItem dlgItemId)
	{
		RECT rc;
		GetWindowRect(GetDlgItem(dlgHwnd,dlgItemId),&rc);
		POINT pt={rc.left,rc.top};
		ScreenToClient(dlgHwnd, &pt);
		return iPoint(pt);
	}	
	
	/// Center specified window relative to parent
	inline void CenterWindow(HWND hwnd, HWND parent)
	{
		check(hwnd!=NULL);
		check(hwnd!=parent);

		RECT wnd_rc, par_rc;

		::GetWindowRect(hwnd,&wnd_rc);
		iSize wnd_siz(wnd_rc.right - wnd_rc.left,wnd_rc.bottom - wnd_rc.top);

		if (parent) ::GetWindowRect(parent,&par_rc);
		else {
			par_rc.left = 0;
			par_rc.top = 0;
			par_rc.right = GetSystemMetrics(SM_CXSCREEN);
			par_rc.bottom = GetSystemMetrics(SM_CYSCREEN);
		}
		iSize par_siz(par_rc.right - par_rc.left,par_rc.bottom - par_rc.top);

		iRect out_rc( (sint32)(par_siz.w/2) - (sint32)(wnd_siz.w/2), (sint32)(par_siz.h/2) - (sint32)(wnd_siz.h/2), wnd_siz.w, wnd_siz.h);

		::MoveWindow(hwnd, out_rc.x, out_rc.y, out_rc.w, out_rc.h,FALSE);
	}

	/// Adds item into list box control
	inline void AddListBoxItem(HWND dlgHwnd, iDlgItem dlgItemId, const iStringT &text)
	{
		CListBox(::GetDlgItem(dlgHwnd,dlgItemId)).AddString(text.CStr());
	}

	/// Returns index of currently selected list box control item
	inline sint32 GetListBoxCurSel(HWND dlgHwnd, iDlgItem dlgItemId)
	{
		return CListBox(::GetDlgItem(dlgHwnd,dlgItemId)).GetCurSel();
	}

} //namespace LI

#endif //_IOLIB_LITHIUM_HELPER_METHODS_H_
/** @} */ // iolib_li
