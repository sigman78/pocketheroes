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
 *	@brief NonWindowed slider bar control class implementation
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_NWCONTROL_SLIDER_BAR_H_
#define _IOLIB_SILICON_NWCONTROL_SLIDER_BAR_H_

/// NonWindowed slider bar control class 
class iNWSliderBar : public iNWControl
{
public:
	/// Default constructor
	iNWSliderBar();

	/// Creates and initializes  slider bar control
	bool CreateNWSliderBar(iNWElementContainerI *parent, const iRect &rect, const iDib &back_dib, const iDib &bar_dib, const iDib &handle_dib);

	virtual void DrawElement(iMemoryDC &ddc) const;

	virtual void OnLButtonDown(const iPoint& pos, uint32 vkey);
	virtual void OnLButtonUp(const iPoint& pos, uint32 vkey);
	virtual void OnMouseMove(const iPoint& pos);

	/// Sets current slider position
	void SetCur(sint32 cur, bool refMsg = true);
	/// Sets minimum and maximum values
	void SetMinMax(sint32 min, sint32 max);
	/// Sets current, minimum and maximum values
	void SetAll(sint32 cur, sint32 min, sint32 max);

	/// Returns current slider position
	inline sint32 GetCur()
	{ return m_Cur; }

	/// Returns minimum value
	inline sint32 GetMin()
	{ return m_Min; }
	
	/// Returns maximum value
	inline sint32 GetMax()
	{ return m_Max; }

private:
	sint32 Pix2Pos(sint32 pix_pos);


	bool	m_bTracking;
	sint32	m_TrackPos;

	sint32	m_Cur;
	sint32	m_Min;
	sint32	m_Max;

	iDib	m_dibBack;
	iDib	m_dibBar;
	iDib	m_dibHandle;
};

#endif //_IOLIB_SILICON_NWCONTROL_SLIDER_BAR_H_
/** @} */ // iolib_si
