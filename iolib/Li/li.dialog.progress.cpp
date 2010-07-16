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

#include "li.std.h"
#include "li.dialog.progress.h"

//
// iProgressDialog
//

bool iProgressDialog::CreateProgressDlg(const iStringT &caption, const iStringT &lbl_text, bool cancel_btn, bool visible)
{
	m_CanCanceled = cancel_btn;
	m_IsCanceled = false;
	
	iSize clsize(300,50);

	iSize wsize(clsize.w + GetSystemMetrics(SM_CXDLGFRAME)*2,clsize.h+GetSystemMetrics(SM_CYSMCAPTION)+GetSystemMetrics(SM_CYDLGFRAME)*2);

	if (m_CanCanceled) wsize.h += (25+5);

	if (!CreateBaseDialog(wsize, caption,DLGSTL_TOOLWINDOW | DLGSTL_TOPMOST, visible)) return false;

	HFONT fnt = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	RECT lblrect = { 5,5,295,25};
	if (!m_TextLabel.Create(m_hWnd,lblrect,lbl_text.CStr(),SS_CENTER | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS)) return false;
	m_TextLabel.SetFont(fnt);

	RECT prgrect = { 5,25,295,45 };
	if (!m_ProgressCtrl.Create(m_hWnd,prgrect,0,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS)) return false;

	if (m_CanCanceled)
	{
		RECT btnrect = { 100,50,200,75 };
		if (!m_CancelBtn.Create(m_hWnd,btnrect, _T("Cancel"),WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,0,1024)) return false;
		m_CancelBtn.SetFont(fnt);
	}

	return true;
}


void iProgressDialog::SetProgressRange(sint32 rmin, sint32 rmax)
{
	m_ProgressCtrl.SetRange32(rmin,rmax);
}

sint32 iProgressDialog::GetRangeMin()
{
	PBRANGE pbr;
	m_ProgressCtrl.GetRange(&pbr);
	return pbr.iLow;
}

sint32 iProgressDialog::GetRangeMax()
{
	PBRANGE pbr;
	m_ProgressCtrl.GetRange(&pbr);
	return pbr.iHigh;
}

void iProgressDialog::SetProgressPos(sint32 npos)
{
	m_ProgressCtrl.SetPos(npos);
	UpdateWindow();
}

sint32 iProgressDialog::GetProgressPos()
{
	return m_ProgressCtrl.GetPos();
}

void iProgressDialog::SetLabelText(const iStringT &nltxt)
{
	m_TextLabel.SetWindowText(nltxt.CStr());
	UpdateWindow();
}

void iProgressDialog::SetCancelBtnText(const iStringT &btntxt)
{
	if (m_CanCanceled) m_CancelBtn.SetWindowText(btntxt.CStr());
}

void iProgressDialog::SetConfirmation( const iStringT& text, const iStringT& title )
{
	m_ConfirmationText = text;
	m_ConfirmationTitle= title;
}

bool iProgressDialog::ConfirmCancel()
{
	if ( !m_ConfirmationText.Empty() ) {
		return IDYES == MessageBox( m_ConfirmationText.CStr(), m_ConfirmationTitle.CStr(), MB_YESNO | MB_ICONWARNING );
	}
	return true;
}