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
 *	@brief Window Manager class implementation
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_MAIN_WINDOWS_MANAGER_H_
#define _IOLIB_SILICON_MAIN_WINDOWS_MANAGER_H_

/// Window Manager class 
class iWindowManager : public CWindowImpl<iWindowManager, CWindow, CWinTraits<0> >
{
public:
	DECLARE_WND_CLASS(NULL)

	/// Default constructor
	iWindowManager()
	{
	}


	/// Creates and initializes Window manager
	bool CreateWindowManager()
	{
		RECT rect = {0,0,0,0};
		if (Create(NULL,rect,_T(""),0)==NULL) return false;
		else {
			return true;
		}
	}

	/// Destroys Window manager
	void DestroyWindowManager()
	{
		DestroyWindow();
	}

	// Rect routine
public:

	/// Returns work area rectangle
	static iRect GetWorkAreaRect()
	{
		RECT rect;
		SystemParametersInfo(SPI_GETWORKAREA, 0, (LPVOID)&rect, 0);
		return iRect(rect);
	}

	/// Returns work area size
	static iSize GetWorkAreaSize()
	{
		return iSize(GetWorkAreaRect());
	}

	/// Returns screen rectangle
	static iRect GetScreenRect()
	{
		return iRect(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	}

	/// Returns screen size
	static iSize GetScreenSize()
	{
		return iSize(GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	}

	/// Returns screen width
	static uint32 GetScreenWidth()
	{
		return GetSystemMetrics(SM_CXSCREEN);
	}

	/// Returns screen height
	static uint32 GetScreenHeight()
	{
		return GetSystemMetrics(SM_CYSCREEN);
	}

private:

private:
	BEGIN_MSG_MAP(iWindowManager)
		//
	END_MSG_MAP()

};


#endif //_IOLIB_SILICON_MAIN_WINDOWS_MANAGER_H_
/** @} */ // iolib_si
