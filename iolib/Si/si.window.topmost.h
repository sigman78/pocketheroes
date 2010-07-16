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
 *	@brief Topmost window class implementation
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_TOPMOST_WINDOW_H_
#define _IOLIB_SILICON_TOPMOST_WINDOW_H_

/// Topmost window class
class iTopmostWindow : public iBaseWindowNW
{
public:
	/// Creates topmost window with specified params
	bool CreateTopmostWindow(const iRect &rect, bool layered = false, bool sticky=true, bool appwnd=true);
	virtual void MsgWindowPosChanging(LPWINDOWPOS wndpos);

private:
	bool	m_Sticky;
	bool	m_AppWindow;
};

#endif//_IOLIB_SILICON_TOPMOST_WINDOW_H_
/** @} */ // iolib_si
