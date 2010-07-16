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
 *	@brief Windows Device context helper functions
 */
/** @addtogroup iolib_xe
 *  @{
 */
#ifndef _XENNON_DEVICE_CONTEXT_HELPERS_H_
#define _XENNON_DEVICE_CONTEXT_HELPERS_H_

/// Windows Device context helper functions namespace
namespace iDCHLP {

/// Fills a rectangle by using the specified color. 
inline void FillWnd(HDC hdc, const iRect& rc, const iColor& clr)
{
	HBRUSH hBrush = ::CreateSolidBrush(clr);
	::FillRect(hdc,&RECT(rc),hBrush);
	::DeleteObject(hBrush);
}

/// Fills a rectangle by using the specified color and Draws a border around it.
inline void FillFrmWnd(HDC hdc, const iRect& rc, const iColor& clr_fill, const iColor& clr_frm)
{
	iRect trc(rc);
	HBRUSH hBrush_fill = ::CreateSolidBrush(clr_fill);
	HBRUSH hBrush_frame = ::CreateSolidBrush(clr_frm);
	
	::FrameRect(hdc,&RECT(trc),hBrush_frame);
	trc.DeflateRect(1);
	::FillRect(hdc,&RECT(trc),hBrush_fill);

	::DeleteObject(hBrush_fill);
	::DeleteObject(hBrush_frame);
}
}; // namespace iDCHLP

#endif //_XENNON_DEVICE_CONTEXT_HELPERS_H_
/** @} */ // iolib_xe
