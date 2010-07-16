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

#include "gxl.inc.h"
#ifdef OS_WINCE
#include <gx.h>
#endif //OS_WINCE
#include "gxl.ctr.cbuff.h"
#include "gxl.ctr.array.h"
#include "gxl.window.h"
#include "gxl.input.h"
#include "gxl.timer.h"
#include "gxl.display.h"
#include "gxl.view.h"
#include "gxl.dialog.h"
#include "gxl.popupview.h"
#include "gxl.viewmgr.h"
#include "gxl.application.h"

iPopupView::iPopupView(iViewMgr* pViewMgr)
: iView(pViewMgr, iRect(0,0,0,0), GENERIC_VIEWPORT, 0, Enabled)
{
}

void iPopupView::TrackPopup(const iPoint& pos, const iRect& bound, Alignment al)
{
	iSize nsiz = PopupViewSize();
	iSize msiz = PopupViewMinSize();
	nsiz.w = iMAX<uint32>(nsiz.w,msiz.w);
	nsiz.h = iMAX<uint32>(nsiz.h,msiz.h);

	iRect orc = bound;
	
	if (al & AlignRight) {
		orc.w = pos.x - bound.x;
	} else if (al & AlignLeft) {
		orc.x = pos.x;
	}

	if (al & AlignBottom) orc.h = pos.y - bound.y;
	else if (al & AlignTop) orc.y = pos.y;

	m_Rect = AlignRect(nsiz, orc, al);


	if (m_Rect.x < bound.x) m_Rect.x = bound.x;
	else if (m_Rect.x + m_Rect.w > bound.x + bound.w) m_Rect.x = bound.x + bound.w - m_Rect.w;
	if (m_Rect.y < bound.y) m_Rect.y = bound.y;
	else if (m_Rect.y + m_Rect.h > bound.y + bound.h) m_Rect.y = bound.y + bound.h - m_Rect.h;

	OnTrackPopup(m_Rect);
	SetVisible(true);
}

void iPopupView::HidePopup()
{
	OnHidePopup();
	SetVisible(false);
}
