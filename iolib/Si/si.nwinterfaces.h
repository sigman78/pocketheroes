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
 *	@brief Common [iOLiB::Si] definations
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_INTERFACE_DECLARATIONS_FOR_NWELEMENTS_H_
#define _IOLIB_SILICON_INTERFACE_DECLARATIONS_FOR_NWELEMENTS_H_

class iNWElementContainerI;
class iNWElementI;

/// NonWindowed control's message container
class iNWMessage
{
public:

	/// Default constructor
    iNWMessage () 
    { }

	/// Constructs iNWMessage object with given params
    iNWMessage (iNWElementI *el, uint32 msg, uint32 lParam, void *pParam)
	: m_Element(el)
	, m_Message(msg)
	, m_lParam(lParam)
	, m_pParam(pParam)
    { }

	iNWElementI*	m_Element;
    uint32			m_Message;
	uint32			m_lParam;
	void*			m_pParam;
};

/// NonWindowed control's message subscriber interface
class iNWSubscriberI
{
public:
	/// Message handling method
	virtual bool OnNWMessage( iNWMessage& msg ) = 0;
};

/// Common NonWindowed control interface
class iNWElementI
{
public:
	/// Forces element's redrawing
	virtual void ForceRepaint(void) const = NULL;

	/// Draws element
	virtual void DrawElement(iMemoryDC &ddc) const = NULL;

	/// Returns pointer to the element's owner
	virtual iNWElementContainerI* GetOwner(void) const = NULL;

	/// Begins tracking
	virtual void BeginTrack(const iPoint& pos) = NULL;
	/// Ends tracking
	virtual void EndTrack(void) = NULL;
	/// Returns tracking state
	virtual bool IsTrack(void) const = NULL;

	/// Returns tracking anchor point
	virtual const iPoint& GetTrackAnchor() const = NULL;

	/// Returns cursor handle
	virtual HCURSOR GetCursor(const iPoint &pos) const = NULL;

	/// Returns element's width
	virtual uint32 GetElementWidth() const = NULL;
	/// Returns element's height
	virtual uint32 GetElementHeight() const = NULL;
	/// Returns element's rect
	virtual const iRect& GetElementRect() const = NULL;
	/// Returns element's rect
	virtual iRect GetElementRect() = NULL;
	/// Setups new element's rect
	virtual void SetElementRect(const iRect &rect) = NULL;
	/// Returns - is specified point belong to element's rect or not
	virtual bool IsMyPoint(const iPoint &pnt) const = NULL;

	/// Mouse Enter message handling
	virtual void OnMouseEnter(void) = NULL;
	/// Mouse Leave message handling
	virtual void OnMouseLeave(void) = NULL;
	/// Mouse Move message handling
	virtual void OnMouseMove(const iPoint &pos) = NULL;
	/// Mouse Click message handling
	virtual void OnMouseClick(const iPoint &pos) = NULL;
	/// Mouse Double Click message handling
	virtual void OnDblClick(const iPoint &pos) = NULL;
	/// Mouse Left Button Down message handling
	virtual void OnLButtonDown(const iPoint &pos, uint32 vkey) = NULL;
	/// Mouse Left Button Up message handling
	virtual void OnLButtonUp(const iPoint &pos, uint32 vkey) = NULL;
	/// Mouse Right Button Down message handling
	virtual void OnRButtonDown(const iPoint &pos, uint32 vkey) = NULL;
	/// Mouse Right Button Up message handling
	virtual void OnRButtonUp(const iPoint &pos, uint32 vkey) = NULL;
	/// Mouse Middle Button Down message handling
	virtual void OnMButtonDown(const iPoint &pos, uint32 vkey) = NULL;
	/// Mouse Middle Button Up message handling
	virtual void OnMButtonUp(const iPoint &pos, uint32 vkey) = NULL;
	/// LowLevel mouse proc
	virtual void OnLLMouseProc(const iPoint &pos, int flags) = NULL;
	/// Key Down message handling
	virtual void OnKeyDown(uint32 vkcode, const iPoint& mpos) = NULL;
	/// Key Up message handling
	virtual void OnKeyUp(uint32 vkcode, const iPoint& mpos) = NULL;

	/// Shows element
	virtual void ShowElement(void) = NULL;
	/// Hides element
	virtual void HideElement(void) = NULL;
	/// Check element's visibility
	virtual bool IsElementVisible(void) const = NULL;

	/// Sets element's state (Enabled/Disabled)
	virtual void SetElementState(bool bEn) = NULL;
	/// Enables element
	virtual void EnableElement(void) = NULL;
	/// Disables element
	virtual void DisableElement(void) = NULL;
	/// Returns - is element enabled or not
	virtual bool IsElementEnabled(void) const = NULL;

	/// Destroys element
	virtual void DestroyElement(void) = NULL;
};

/// Common NonWindowed controls container interface
class iNWElementContainerI
{
public:
	/// Repaints specified element
	virtual void RepaintElement(const iNWElementI *el, const iRect &rect) = NULL;
	/// Draws background
	virtual void DrawBackground(iMemoryDC &ddc, const iRect &rect = cInvalidRect) = NULL;

	/// Registers new element
	virtual void RegisterElement(iNWElementI *el) = NULL;
	/// Unregisters specified element
	virtual void UnRegisterElement(iNWElementI *el) = NULL;

	/// Adds new message subscriber
	virtual void AddMessageSubscriber(iNWSubscriberI* subs) = NULL;
	/// Handle message
	virtual void ControlMessage(iNWMessage& msg) = NULL;

	/// Returns element under specified point
	virtual iNWElementI* GetElementByPoint(const iPoint &pnt) = NULL;

	/// Converts client coordinate into screen
	virtual iPoint NWClientToScreen(const iPoint &pnt) const = NULL;
	/// Converts client coordinate into screen
	virtual iRect NWClientToScreen(const iRect &rect) const = NULL ;
};

//
// NonWindowed Control Messages
//

// Common control messages			= 0x000000xx
const uint32 NWMSG_MOUSE_ENTER		= 0x00000001;
const uint32 NWMSG_MOUSE_LEAVE		= 0x00000002;
const uint32 NWMSG_MOUSE_CLICK		= 0x00000003;
const uint32 NWMSG_DOUBLE_CLICK		= 0x00000004;
const uint32 NWMSG_MOUSE_LDOWN		= 0x00000005;
const uint32 NWMSG_MOUSE_LUP		= 0x00000006;
const uint32 NWMSG_MOUSE_RDOWN		= 0x00000007;
const uint32 NWMSG_MOUSE_RUP		= 0x00000008;
const uint32 NWMSG_MOUSE_MDOWN		= 0x00000009;
const uint32 NWMSG_MOUSE_MUP		= 0x00000010;

const uint32 NWMSG_VIS_CHANGED		= 0x00000021; //lParam = (1 - Visible / 0 - Hidden)
const uint32 NWMSG_ENB_CHANGED		= 0x00000022; //lParam = (1 - Enabled / 0 - Disabled)

// Button messages					= 0x0000010x
const uint32 NWMSG_BUTTON_CLICK		= 0x00000100;

// Switch Control Messages			= 0x0000011x
const uint32 NWMSG_SWITCH_CHANGED	= 0x00000110; //lParam = new state of switch

// Tab Control Messages				= 0x0000012x
const uint32 NWMSG_TAB_CHANGED		= 0x00000120; //lParam = new selected tap
// Slider Control Messages			= 0x0000013x
const uint32 NWMSG_SLDPOS_TRACKING	= 0x00000130; //pParam = pointer to tracking value
const uint32 NWMSG_SLDPOS_CHANGED	= 0x00000131; //pParam = pointer to new pos
// Scroll Bar						- 0x0000014x
const uint32 NWMSG_SCRBAR_TRACKING	= 0x00000140;
const uint32 NWMSG_SCRBAR_CHANGED	= 0x00000141;

//
// Popup Window Notification Messages
//

const uint32 PWNM_NULL				= 0x00000000;
const uint32 PWNM_DEACTIVATED		= 0x00000010; // Popup window lost focuse
const uint32 PWNM_STATUSCHANGED		= 0x00000020; // Status changed lPararam = new status
#endif //_IOLIB_SILICON_INTERFACE_DECLARATIONS_FOR_NWELEMENTS_H_
/** @} */ // iolib_si
