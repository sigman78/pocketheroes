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
 *	@brief Progress bar dialog class implementation
 */
/** @addtogroup iolib_li
 *  @{
 */
#ifndef _IOLIB_LITHIUM_DIALOG_PROGRESS_H_
#define _IOLIB_LITHIUM_DIALOG_PROGRESS_H_

#include "li.dialog.base.h"

/// Progress bar dialog class 
class iProgressDialog : public iBaseDialog
{
public:
	/// Creates Progress bar dialog with specified props
	bool CreateProgressDlg(const iStringT &caption, const iStringT &lbl_text, bool cancel_btn, bool visible = false);

	/// Sets minimum and maximum value of progress bar
	void SetProgressRange(sint32 rmin=0, sint32 rmax=100);
	/// Sets minimum value of progress bar
	sint32 GetRangeMin();
	/// Sets maximum value of progress bar
	sint32 GetRangeMax();
	/// Sets current value (position) of progress bar
	void SetProgressPos(sint32 npos);
	/// Returns current value (position) of progress bar
	sint32 GetProgressPos();
	/// Sets label's text
	void SetLabelText(const iStringT &nltxt);
	/// Sets cancel button's text
	void SetCancelBtnText(const iStringT &btntxt);
	/// Enable confirmaion mode
	void SetConfirmation( const iStringT& text, const iStringT& title );

	/// Returns cancelation state
	inline bool IsCanceled()
	{
		return m_IsCanceled;
	}

	BEGIN_MSG_MAP(iProgressDialog)
		COMMAND_ID_HANDLER(1024,OnCancelButton)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		CHAIN_MSG_MAP(iBaseDialog)
	END_MSG_MAP()

	LRESULT OnCancelButton(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if ( m_CanCanceled && ConfirmCancel() ) {
			m_IsCanceled = true;
		}
		return 0;
	}

	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler)
	{
		if ( m_CanCanceled && wParam == VK_ESCAPE && ConfirmCancel() )
		{
			m_IsCanceled = true;
		}
		return 0;
	}


private:

	bool ConfirmCancel();

	iStringT			m_ConfirmationText;
	iStringT			m_ConfirmationTitle;

	CProgressBarCtrl	m_ProgressCtrl;
	CStatic				m_TextLabel;

	bool				m_CanCanceled;
	CButton				m_CancelBtn;
	bool				m_IsCanceled;
};

#endif //_IOLIB_LITHIUM_DIALOG_PROGRESS_H_
/** @} */ // iolib_li
