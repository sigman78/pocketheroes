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
 *	@brief Text input with button control class implementation
 */
/** @addtogroup iolib_li
 *  @{
 */
#ifndef _IOLIB_LITHIUM_CONTROL_EDIT_WITH_BUTTON_H_
#define _IOLIB_LITHIUM_CONTROL_EDIT_WITH_BUTTON_H_

/// Text input with button control class 
class iCtrlEditBtn : public CWindowImpl<iCtrlEditBtn, CComboBox>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CComboBox::GetWndClassName())


	BEGIN_MSG_MAP(iCtrlEditBtn)
		NOTIFY_CODE_HANDLER(CBN_DROPDOWN,OnDropDown)
	END_MSG_MAP()

	/// On button clicked notifycation
	LRESULT OnDropDown(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
	{
		MessageBox(_T("DropDown!!!"));
		return 0;
	}
	

private:
};

#endif //_IOLIB_LITHIUM_CONTROL_EDIT_WITH_BUTTON_H_
/** @} */ // iolib_li
