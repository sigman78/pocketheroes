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
 *	@brief Abstraction layer for Operation Systems  specific functions
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_OS_SPECIFIC_METHODS_IMPLEMENTAION_H_
#define _IOLIB_SILICON_OS_SPECIFIC_METHODS_IMPLEMENTAION_H_

/// Abstraction layer for Operation Systems  specific
class OSSPECFNC
{
public:
	/// Constructor
	OSSPECFNC();

	/// Destructor
	~OSSPECFNC();

	/// UpdateLayeredWindow function prototype
	typedef int (WINAPI *PFN_UpdateLayeredWindow)( HWND hwnd, HDC hdcDest, POINT *pptDst, SIZE *psize, HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags);
	/// UpdateLayeredWindow function
	static int UpdateLayeredWindow( HWND hwnd, HDC hdcDest, POINT *pptDst, SIZE *psize, HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags);

private:
	bool Init();
	void UnInit();
	static PFN_UpdateLayeredWindow	m_pfnUpdateLayeredWindow;

	HMODULE		m_hUser32DLL;

};

//extern OSSPECFNC gOsSpecFnc;

#endif //_IOLIB_SILICON_OS_SPECIFIC_METHODS_IMPLEMENTAION_H_
/** @} */ // iolib_si
