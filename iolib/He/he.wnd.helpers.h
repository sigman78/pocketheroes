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
 *	@brief Common windows helper functions implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_COMMON_WINDOWS_HELPERS_H_
#define _IOLIB_HELIUM_COMMON_WINDOWS_HELPERS_H_

/// Common windows helper functions namespace
namespace iWMetrics
{
	/// Returns current cursor position in screen coordinates
	inline iPoint GetCursorPos(HWND hwnd = NULL)
	{
		POINT pnt;
		::GetCursorPos(&pnt);
		if (hwnd!=NULL) ::ScreenToClient(hwnd,&pnt);
		return iPoint(pnt);
	}

	/// Returns client area's rect of specified window
	inline iRect GetClRect(HWND hwnd)
	{
		check( ::IsWindow( hwnd ) );
		RECT clrect;
		::GetClientRect(hwnd, &clrect);
		return iRect(clrect);
	}

	/// Returns client area's size of specified window
	inline iSize GetClSize(HWND hwnd)
	{
		check( ::IsWindow( hwnd ) );

		RECT clrect;
		::GetClientRect(hwnd, &clrect);
		return iSize(clrect.right-clrect.left,clrect.bottom-clrect.top);
	}

	/// Returns rect of specified window
	inline iRect GetWndRect(HWND hwnd)
	{
		check( ::IsWindow( hwnd ) );
		RECT rect;
		::GetWindowRect(hwnd,&rect);
		return rect;
	}

	/// Returns position of specified window
	inline iPoint GetWndPos(HWND hwnd)
	{
		check( ::IsWindow( hwnd ) );
		RECT rect;
		::GetWindowRect(hwnd,&rect);
		return iPoint(rect.left,rect.top);
	}

	/// Returns size of specified window
	inline iSize GetWndSize(HWND hwnd)
	{
		check( ::IsWindow( hwnd ) );
		RECT rect;
		::GetWindowRect(hwnd,&rect);
		return iSize(rect.right - rect.left, rect.bottom - rect.top);
	}

	/// Sets position of specified window
	inline void SetWndPos(HWND hwnd, const iPoint &pos)
	{
		check( ::IsWindow( hwnd ) );
		::SetWindowPos(hwnd,0,pos.x,pos.y,0,0,SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	}

	/// Sets rect of specified window
	inline void SetWndRect(HWND hwnd, const iRect &rect)
	{
		check( ::IsWindow( hwnd ) );
		::SetWindowPos(hwnd,0,rect.x,rect.y,rect.w,rect.h,SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	}

	/// Sets size of specified window
	inline void SetWndSize(HWND hwnd, const iSize &siz)
	{
		check( ::IsWindow( hwnd ) );
		::SetWindowPos(hwnd,0,0,0,siz.w,siz.h,SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOMOVE);
	}

	/// Checks mouse position relative to specified window's metrics
	inline bool IsMouseOnWindow(HWND hwnd)
	{
		return GetWndRect(hwnd).PtInRect(GetCursorPos());
	}

	/// Checks given position relative to specified window's metrics
	inline bool IsMouseOnWindow(HWND hwnd, const iPoint &pos)
	{
		return GetWndRect(hwnd).PtInRect(pos);
	}

	/// Converts the screen coordinates of a specified point on the screen to client coordinates
	inline iPoint Scr2Cl(HWND hwnd, const iPoint &pnt)
	{
		return pnt - GetWndPos(hwnd);
	}

	/// Converts the client coordinates of a specified point on the screen to screen coordinates
	inline iPoint Cl2Scr(HWND hwnd, const iPoint &pnt)
	{
		return pnt + GetWndPos(hwnd);
	}

	/// Retrieves the position value from LPARAM
	inline iPoint LParam2Pos(LPARAM lParam)
	{
		return iPoint(LOWORD(lParam),HIWORD(lParam));
	}

	/// Retrieves the size value from LPARAM
	inline iSize LParam2Size(LPARAM lParam)
	{
		return iSize(LOWORD(lParam),HIWORD(lParam));
	}

	/// Returns with of screen
	inline uint32 GetScreenWidth()
	{ return GetSystemMetrics(SM_CXSCREEN); }

	/// Returns height of screen
	inline uint32 GetScreenHeight()
	{ return GetSystemMetrics(SM_CYSCREEN); }

	/// Returns size of screen
	inline iSize GetScreenSize()
	{ return iSize(GetScreenWidth(),GetScreenHeight()); }

	/// Returns rectangle of screen
	inline iRect GetScreenRect()
	{
		return GetScreenSize();
	}

	/// Returns rectangle of work area
	inline iRect GetWorkAreaRect()
	{
		RECT rc;
		SystemParametersInfo(SPI_GETWORKAREA, 0, (LPVOID)&rc, 0);
		return iRect(rc);
	}


} // namespace iWMetrics


#endif //_IOLIB_HELIUM_COMMON_WINDOWS_HELPERS_H_
/** @} */ // iolib_he
