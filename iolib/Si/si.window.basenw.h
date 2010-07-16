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
 *	@brief Base NonWindowed controls container class implementation
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_BASE_WINDOW_WITH_NWCONTROLS_SUPPORT_H_
#define _IOLIB_SILICON_BASE_WINDOW_WITH_NWCONTROLS_SUPPORT_H_

#include "si.nwinterfaces.h"

/// Base NonWindowed controls container class
class iBaseWindowNW : public iGenericWindow, public iNWElementContainerI
{
public:
	/// Default constructor
	iBaseWindowNW();
	/// Initializes  iBaseWindowNW object and creates window
	bool CreateBaseWindowNW(const iRect &rect, HWND parent, uint32 style, uint32 exstyle, bool layered);

public:
	// Inherited Container Interface
	virtual void RepaintElement(const iNWElementI *el, const iRect &rect);
	virtual void DrawBackground(iMemoryDC &ddc, const iRect &rect = cInvalidRect);
	virtual void RegisterElement(iNWElementI *el);
	virtual void UnRegisterElement(iNWElementI *el);
	virtual void UnRegisterAllElements(void);

	virtual void AddMessageSubscriber(iNWSubscriberI* subs);
	virtual void RemoveMessageSubscriber(iNWSubscriberI* subs);
	virtual void RemoveAllMessageSubscribers(void);
	virtual void ControlMessage(iNWMessage& msg);

	virtual iNWElementI* GetElementByPoint(const iPoint &pnt);

	virtual iPoint NWClientToScreen(const iPoint &pnt) const;
	virtual iRect NWClientToScreen(const iRect &rect) const;


protected:
	virtual void MsgPaint(iDC &dc, PAINTSTRUCT &ps);
	virtual void MsgLayeredPaint(const iRect &rect = cInvalidRect);
	virtual void MsgSize(const iSize &size);
	virtual void MsgDestroy(void);
	virtual void MsgActivate(bool fActive, bool fMinimized);

	virtual void MsgTracking(const iPoint &pos);
	virtual void MsgMouseMove(const iPoint &pos);
	virtual void MsgMouseLeave(void);
	virtual void MsgLButtonDown(const iPoint &pos, uint32 vkey);
	virtual void MsgLButtonUp(const iPoint &pos, uint32 vkey);
	virtual void MsgRButtonDown(const iPoint &pos, uint32 vkey);
	virtual void MsgRButtonUp(const iPoint &pos, uint32 vkey);
	virtual void MsgMButtonDown(const iPoint &pos, uint32 vkey);
	virtual void MsgMButtonUp(const iPoint &pos, uint32 vkey);

	virtual void MsgKeyDown(uint32 vkeycode);
	virtual void MsgKeyUp(uint32 vkeycode);

private:

	/// Resizes buffer dib with new size
	void ResizeComposingDib(const iSize &size);

	iMemoryDC					m_outDC;
	bool						m_Destroying;

	iFastArray<iNWElementI*>	m_Elements;
	iFastArray<iNWSubscriberI*>	m_Subscribers;
	iNWElementI*				m_LastFocusedElement;
	iNWElementI*				m_TrackElement;

	HDC							m_ScreenDC;
	
	uint32						m_dclTime;
	iNWElementI*				m_dclElement;
	iPoint						m_dclPoint;
};

#endif //_IOLIB_SILICON_BASE_WINDOW_WITH_NWCONTROLS_SUPPORT_H_
/** @} */ // iolib_si
