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
 *	@brief NonWindowed flat scroll bar control class implementation
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_NWCONTROL_FLAT_SCROLLBAR_H_
#define _IOLIB_SILICON_NWCONTROL_FLAT_SCROLLBAR_H_

/// NonWindowed flat scroll bar control class 
class iFlatScrollbar : public iNWControl
{
public:
	/// Default constructor
	iFlatScrollbar()
	: m_Siz(100)
	, m_Pos(0)
	, m_PageSiz(30)
	, m_bDragging(false)
	, m_DragAnchor(cInvalidPoint)
	{}

	/// Creates and initializes flat scroll bar control
	bool CreateFlatScrollBar(iNWElementContainerI *parent, const iRect &rect)
	{
		// call iNW control's initialization
		return CreateNWElement(parent,rect);
	}

	/// Returns current scroll bar position
	sint64 GetPos(void) const
	{
		return m_Pos;
	}

	/// Sets current scroll bar position
	void SetPos(sint64 pos)
	{
		if (m_PageSiz >= m_Siz || pos <= 0) m_Pos = 0;
		else if ( (pos + m_PageSiz) > m_Siz ) m_Pos = (m_Siz - m_PageSiz);
		else m_Pos = pos;
	}

	/// Sets total size
	void SetSiz(sint64 siz)
	{
		m_Siz = siz;
		SetPos(m_Pos);
	}

	/// Sets page size
	void SetPgSiz(sint64 pgsiz)
	{
		m_PageSiz = pgsiz;
		SetPos(m_Pos);
	}

	/// Sets total size, page size and current position
	void SetProps(sint64 siz, sint64 pos, sint64 pgsiz)
	{
		m_Siz = siz;
		m_PageSiz = pgsiz;
		SetPos(pos);
	}

	/// Converts absolute value to screen metrics
	inline sint32 Abs2Scr(sint64 abs) const
	{
		sint64 pix_w = GetElementWidth();
		return static_cast<sint32>((abs*pix_w)/m_Siz);
	}

	/// Converts screen metrics value to absolute
	inline sint64 Scr2Abs(sint32 scr) const
	{
		sint64 pix_w = GetElementWidth();
		return static_cast<sint64>(static_cast<sint64>(scr)*m_Siz)/pix_w;
	}

	/// Calculates thumb metrics
	void CalcThumbMetrics(iRect &rc_thumb) const
	{
		sint64 pix_w = GetElementWidth();
		sint64 pix_h = GetElementHeight();

		if (m_PageSiz >= m_Siz) {
			rc_thumb = iRect(0,0,(uint32)pix_w,(uint32)pix_h);
		} else {
			uint32 pix_begin = static_cast<uint32>((m_Pos * pix_w) / m_Siz);
			uint32 pix_end = static_cast<uint32>(((m_Pos+m_PageSiz) * pix_w) / m_Siz);
			check(pix_begin <= pix_end);
			if (pix_begin == pix_end) pix_end += 1;
			rc_thumb = iRect(pix_begin,0,pix_end-pix_begin,static_cast<uint32>(pix_h));
		}
	}

	/// calculates croll bar metrics
	void CalcBarMetrics(iRect &rc_left, iRect &rc_thumb, iRect &rc_right) const
	{
		sint64 pix_w = GetElementWidth();
		sint64 pix_h = GetElementHeight();

		if (m_PageSiz >= m_Siz) {
			rc_thumb = iRect(0,0,(uint32)pix_w,(uint32)pix_h);
			rc_left = iRect(0,0,0,0);
			rc_right = iRect(0,0,0,0);
		} else {
			uint32 pix_begin = static_cast<uint32>((m_Pos * pix_w) / m_Siz);
			uint32 pix_end = static_cast<uint32>(((m_Pos+m_PageSiz) * pix_w) / m_Siz);
			check(pix_begin <= pix_end);
			if (pix_begin == pix_end) pix_end += 1;
			rc_thumb = iRect(pix_begin,0,pix_end-pix_begin,static_cast<uint32>(pix_h));
			rc_left = iRect(0, 0, pix_begin, static_cast<uint32>(pix_h));
			rc_right = iRect(pix_end, 0, static_cast<uint32>(pix_w) - pix_end, static_cast<uint32>(pix_h));
		}
	}

	virtual void OnLButtonDown(const iPoint &pos, uint32 vkey)
	{
		iRect elrect = GetElementRect();

		iRect rc_left, rc_thumb, rc_right;
		CalcBarMetrics(rc_left, rc_thumb, rc_right);
		iPoint spos(pos.x,pos.y);

		if (rc_left.PtInRect(spos)){
			iLOG(_T("--- Left"));
		} else if (rc_right.PtInRect(spos)) {
			iLOG(_T("--- Right"));
		} else if (rc_thumb.PtInRect(spos)) {
			iLOG(_T("--- Thumb"));
			m_AnchorVal = m_Pos;
			m_DragAnchor = pos;
			m_bDragging = true;
		} else {
			iFLOG((_T("--- (%d,%d) - %d,%d,%d,%d"),spos.x,spos.y,rc_thumb.x,rc_thumb.y,rc_thumb.w,rc_thumb.h));
		}

		/*
		uint64 curpos = m_Min + (((uint64)(pos.x) * m_Max) / (uint64)(elrect.w));
		m_Cur = curpos;
		GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_SLDPOS_CHANGED,0,NULL));
		*/
		ForceRepaint();
		iNWControl::OnLButtonDown(pos,vkey);
	}

	virtual void OnLButtonUp(const iPoint &pos, uint32 vkey)
	{
		if (m_bDragging){
			m_bDragging = false;
			m_DragAnchor = cInvalidPoint;
			GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_SCRBAR_CHANGED,0,NULL));
			ForceRepaint();
		}
		iNWControl::OnLButtonUp(pos,vkey);
	}

	virtual void OnMouseMove(const iPoint &pos)
	{
		if (m_bDragging){
			check(m_DragAnchor != cInvalidPoint);

			sint32 dif = pos.x - m_DragAnchor.x;
			sint64 abs_dif = Scr2Abs(dif);
			SetPos(m_AnchorVal + Scr2Abs(dif));
			GetOwner()->ControlMessage(iNWMessage((iNWElementI*)this, NWMSG_SCRBAR_TRACKING,0,NULL));
			ForceRepaint();
		}
		iNWControl::OnMouseMove(pos);
	}

	virtual void DrawElement(iMemoryDC &ddc) const
	{
		iRect el_rc = GetElementRect();
		ddc.m_Dib.FillRect(el_rc,cColorWhite);

		//
		iRect rc_thumb;
		CalcThumbMetrics(rc_thumb);

		rc_thumb.x += el_rc.x;
		rc_thumb.y += el_rc.y;
		if (m_PageSiz >= m_Siz) ddc.m_Dib.FillRect(rc_thumb,iColor(180,180,192));
		else if (m_bDragging) ddc.m_Dib.FillRect(rc_thumb,iColor(0,0,128));
		else ddc.m_Dib.FillRect(rc_thumb,iColor(64,64,192));
	}


private:
	iPoint	m_DragAnchor;
	sint64	m_AnchorVal;
	bool	m_bDragging;

	sint64	m_Siz;
	sint64	m_Pos;
	sint64	m_PageSiz;
};

#endif //_IOLIB_SILICON_NWCONTROL_FLAT_SCROLLBAR_H_
/** @} */ // iolib_si
