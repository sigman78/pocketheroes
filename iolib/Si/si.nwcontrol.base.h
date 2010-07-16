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
 *	@brief Basic NonWindowed control class implementation
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_BASE_NWCONTROL_H_
#define _IOLIB_SILICON_BASE_NWCONTROL_H_

#include "si.nwinterfaces.h"

/// Basic NonWindowed control class 
class iNWControl: public iNWElementI
{
public:
	/// Default constructor
	iNWControl(void);
	
	/// Creates and initializes control
	bool CreateNWElement(iNWElementContainerI *parent, const iRect &rect);

	virtual void ForceRepaint(void) const;
	virtual void DrawElement(iMemoryDC &ddc) const;

	virtual iNWElementContainerI* GetOwner(void) const;

	virtual uint32 GetElementWidth() const;
	virtual uint32 GetElementHeight() const;
	virtual const iRect& GetElementRect() const;
	virtual iRect GetElementRect();
	virtual bool IsMyPoint(const iPoint &pnt) const;

	virtual void SetElementRect(const iRect &rect);
	virtual void SetElementPos(const iPoint &pnt);
	virtual void SetElementSize(const iSize &siz);

	virtual HCURSOR GetCursor(const iPoint &pos) const;

	virtual void BeginTrack(const iPoint& pos);
	virtual void EndTrack(void);
	virtual bool IsTrack(void) const;
	virtual const iPoint& GetTrackAnchor() const;

	virtual void OnMouseEnter(void);
	virtual void OnMouseLeave(void);
	virtual void OnMouseMove(const iPoint &pos);
	virtual void OnMouseClick(const iPoint &pos);
	virtual void OnDblClick(const iPoint &pos);
	virtual void OnLButtonDown(const iPoint &pos, uint32 vkey);
	virtual void OnLButtonUp(const iPoint &pos, uint32 vkey);
	virtual void OnRButtonDown(const iPoint &pos, uint32 vkey);
	virtual void OnRButtonUp(const iPoint &pos, uint32 vkey);
	virtual void OnMButtonDown(const iPoint &pos, uint32 vkey);
	virtual void OnMButtonUp(const iPoint &pos, uint32 vkey);
	virtual void OnLLMouseProc(const iPoint &pos, int flags);

	virtual void OnKeyDown(uint32 vkcode, const iPoint& mpos);
	virtual void OnKeyUp(uint32 vkcode, const iPoint& mpos);

	virtual void DestroyElement(void);

	virtual void ShowElement(void);
	virtual void HideElement(void);
	virtual bool IsElementVisible(void) const;

	virtual void SetElementState(bool bEn);
	virtual void EnableElement(void);
	virtual void DisableElement(void);
	virtual bool IsElementEnabled(void) const;
	virtual bool IsElementCreated(void) const;


protected:
	bool					m_IsVisible;
	bool					m_IsEnabled;
	bool					m_IsCreated;

	bool					m_Tracked;
	iPoint					m_TrackAnchor;
	iNWElementContainerI*	m_Parent;
	iRect					m_ElementRect;
	
};

#endif //_IOLIB_SILICON_BASE_NWCONTROL_H_
/** @} */ // iolib_si
