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
#include "gxl.viewmgr.h"
#include "gxl.application.h"

iDialog::iDialog(iViewMgr* pViewMgr)
: iView(pViewMgr, iRect(0, 0, 0, 0), MODAL_DIALOG, 0, Enabled), m_retCode(DRC_UNDEFINED) { }

sint32 iDialog::DoModal()
{
	Center();
	OnCreateDlg();
	SetVisible();
	m_pMgr->PushModalDlg(this);
	while (m_pMgr->App()->Cycle() && m_retCode == DRC_UNDEFINED) {}
	iDialog* pDlg = m_pMgr->PopModalDlg();
	check(pDlg == this);

	return m_retCode;
}

void iDialog::Center()
{
	iRect rect = AlignRect(GetDlgMetrics(), m_pMgr->Metrics(), AlignCenter);
	OnPlace(rect);
	SetRect(rect);
}

bool iDialog::EndDialog(sint32 retCode)
{
	if (m_retCode != DRC_UNDEFINED)	return false;
	m_retCode = retCode;
	return true;
}

void iDialog::Invalidate()
{
	m_bNeedRedraw = true;
	if (m_pMgr->CurView())  m_pMgr->CurView()->Invalidate();
}

