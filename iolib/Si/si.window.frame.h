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
 *	@brief Generic Frame window (sizable) class implementation
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_FRAME_WINDOW_H_
#define _IOLIB_SILICON_FRAME_WINDOW_H_

/// Generic Frame window (sizable) class
class iFrameWindow : public iTopmostWindow
{
public:
	/// Creates iFrameWindow object with specified params
	bool CreateFrameWindow(const iRect &rect, bool layered = false, bool sticky=true, bool sizable=true);

	/// Override the window's default minimum or maximum tracking size.
	void SetMinMaxInfo(const iSize &minsiz = cInvalidSize, const iSize &maxsiz = cInvalidSize);
	
	virtual sint32 MsgNcHitTest(const iPoint &mpos);
private:
	bool	m_Sizable;
	iSize	m_MinSize;
	iSize	m_MaxSize;
};

#endif //_IOLIB_SILICON_FRAME_WINDOW_H_
/** @} */ // iolib_si
