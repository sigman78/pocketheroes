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
#include "gxl.topmostview.h"
#include "gxl.viewmgr.h"
#include "gxl.application.h"

iTopmostView::iTopmostView(iViewMgr* pViewMgr)
: iView(pViewMgr, pViewMgr->Metrics(), GENERIC_VIEWPORT, 0, Visible|Enabled) {}

// Message handler
bool iTopmostView::ProcessMessage(const iInput::iEntry& msg)
{
	switch(msg.taskType) {
		case iInput::iEntry::MouseMove: MouseTrack(iPoint(msg.px,msg.py)); break;
		case iInput::iEntry::MouseDown: MouseDown(iPoint(msg.px,msg.py)); break;
		case iInput::iEntry::MouseUp: MouseUp(iPoint(msg.px,msg.py)); break;
		case iInput::iEntry::KeyDown: KeyDown(msg.key); break;
		case iInput::iEntry::KeyUp: KeyUp(msg.key); break;
	}
	return true;
}

bool iTopmostView::KeyDown(sint32 key)
{
	return OnKeyDown(key);
}

bool iTopmostView::KeyUp(sint32 key)
{
	return OnKeyUp(key);
}

