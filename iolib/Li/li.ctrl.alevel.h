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
 *	@brief Audio level visualization control class implementation
 */
/** @addtogroup iolib_li
 *  @{
 */
#ifndef _IOLIB_LITHIUM_CONTROL_AUDIO_LEVEL_METER_H_
#define _IOLIB_LITHIUM_CONTROL_AUDIO_LEVEL_METER_H_

/// Audio level visualization control class
class iCtrlALevel : public CWindowImpl<iCtrlALevel, CStatic>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CStatic::GetWndClassName())
	
	BEGIN_MSG_MAP(iCtrlALevel)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	iCtrlALevel() : m_Min(0), m_Max(100), m_Pos(0){}

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
	{
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(&ps);

		iSize clsiz = iWMetrics::GetClSize(m_hWnd);
		if (m_dib[0].GetSize() != clsiz) RecreateDibs(clsiz);
		iDibDC::DibToDCXY(m_dib[0],dc,iPoint(0,0));
		iDibDC::DibRectToDCXY(m_dib[1],iRect(0,0,clsiz.w * (m_Pos-m_Min) / (m_Max-m_Min),clsiz.h),dc,iPoint(0,0));

		EndPaint(&ps);
		bHandler=TRUE;
		return 0;
	}

	void RecreateDibs(const iSize& clsiz)
	{
		m_dib[0].Create(iSize(clsiz),iColor(128,128,128,255));
		m_dib[1].Create(iSize(clsiz));

		uint32 gs = clsiz.w/3*2;
		m_dib[1].FillRect(iRect(0,0,gs,clsiz.h),iColor(0,220,0,255),BLEND_SRCCOPY);

		uint32 yrs = clsiz.w - gs;
		uint32 ys  = yrs / 2;
		m_dib[1].FillRect(iRect(gs,0,ys,clsiz.h),iColor(220,220,0,255),BLEND_SRCCOPY);
		gs += ys;
		uint32 rs = clsiz.w - gs;
		m_dib[1].FillRect(iRect(gs,0,rs,clsiz.h),iColor(255,0,0,255),BLEND_SRCCOPY);
	}

	/// Setup audio level params
	void SetParams(sint32 _min, sint32 _max, sint32 _pos)
	{
		check(_min < _max);
		m_Min = _min;
		m_Max = _max;
		m_Pos = iCLAMP<sint32>(_min,_max,_pos);
		Invalidate();
	}


private:
	iDib	m_dib[2];
	sint32	m_Min, m_Max, m_Pos;
};


#endif //_IOLIB_LITHIUM_CONTROL_AUDIO_LEVEL_METER_H_
/** @} */ // iolib_li
