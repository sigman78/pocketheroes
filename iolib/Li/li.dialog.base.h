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
 *	@brief Generic dialog class implementation
 */
/** @addtogroup iolib_li
 *  @{
 */
#ifndef _IOLIB_LITHIUM_DIALOG_BASE_H_
#define _IOLIB_LITHIUM_DIALOG_BASE_H_

// Decoration constants
const uint32 DLGDEC_FILL_SOLID		= 0x00000001;

const uint32 DLGDEC_FRAME_RAISEN	= 0x00000010;
const uint32 DLGDEC_FRAME_SUNKEN	= 0x00000020;
const uint32 DLGDEC_FRAME_EDGED		= 0x00000030;


// Dilog Style constants

const uint32 DLGSTL_APPWINDOW		= 0x00000001;
const uint32 DLGSTL_TOOLWINDOW		= 0x00000002;
const uint32 DLGSTL_TOPMOST			= 0x00000004;


/// Generic Dialog class
class iGenericDialog : public CWindowImpl<iGenericDialog>
{
public:

	/// Destructor
	~iGenericDialog()
	{
		if (::IsWindow(m_hWnd)) EndDialog();
	}

	BEGIN_MSG_MAP(iBaseChildDialog)
		MESSAGE_HANDLER( WM_SIZE, OnSize);
		MESSAGE_HANDLER( WM_PAINT, OnPaint );
		MESSAGE_HANDLER( WM_ERASEBKGND, OnEraseBackground );
	END_MSG_MAP()

	/// Creates generic dialog with specified props
	bool CreateGenericDlg(HWND parent, const iRect &rect, const iStringT &caption, uint32 styles, uint32 exstyles, uint32 decoration)
	{
		m_DlgDecoration = decoration;

		if (!Create( parent,RECT(rect),caption.CStr(),styles,exstyles)) return false;

		return true;
	}

	/// Overrideable OnSize function
	virtual void MsgSize(iSize &nsize)
	{
	}

	/// Overrideable OnPaint function
	virtual void MsgPaint(HDC hdc, PAINTSTRUCT &ps)
	{
		HBRUSH l_brush = ::GetSysColorBrush(COLOR_3DHIGHLIGHT);
		HBRUSH d_brush = ::GetSysColorBrush(COLOR_3DSHADOW);
		HBRUSH m_brush = ::GetSysColorBrush(COLOR_3DFACE);

		RECT clRect;
		GetClientRect(&clRect);

		if (m_DlgDecoration & DLGDEC_FRAME_EDGED)
		{
			RECT rc1 = {clRect.left,clRect.top,clRect.right-1,clRect.bottom-1};
			RECT rc2 = {clRect.left+1,clRect.top+1,clRect.right,clRect.bottom};
			FrameRect(hdc,&rc1,d_brush);
			FrameRect(hdc,&rc2,l_brush);

			clRect.left+=2;
			clRect.right-=2;
			clRect.top+=2;
			clRect.bottom-=2;
		}
		else if (m_DlgDecoration & DLGDEC_FRAME_RAISEN)
		{
		}
		else if (m_DlgDecoration & DLGDEC_FRAME_SUNKEN)
		{
		}

		if (m_DlgDecoration & DLGDEC_FILL_SOLID)
		{
			::FillRect(hdc,&clRect, m_brush);
		}
	}

	LRESULT OnSize( UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
	{
		MsgSize(iSize(LOWORD(lParam),HIWORD(lParam)));
		return 0;
	}

	LRESULT OnEraseBackground( UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnPaint( UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;
		PAINTSTRUCT ps;
		HDC hdc=BeginPaint(&ps);

		MsgPaint(hdc, ps);

		EndPaint(&ps);
		return 0;
	}

	/// Shows dialog
	void ShowDialog()
	{
		ShowWindow(SW_SHOW);
		UpdateWindow();
	}

	/// Hides dialog
	void HideDialog()
	{
		ShowWindow(SW_HIDE);
	}

	/// Returns - is dialog visible or not
	bool IsVisible()
	{
		return (IsWindowVisible()==TRUE)?true:false;
	}

	/// Destroys dialog
	void EndDialog()
	{
		HideDialog();
		DestroyWindow();
	}

	/// Sets dialog's caption text
	void SetCaptionText(const iStringT &ncapt)
	{
		SetWindowText(ncapt.CStr());
	}

	/// Returns dialog's caption text
	iStringT GetCaptionText()
	{
		iCharT buff[1024];
		GetWindowText(buff,1024);
		return iStringT(buff);
	}

	uint32 m_DlgDecoration;
};

/// Base child dialog class
class iBaseChildDialog : public iGenericDialog
{
public:
	/// Creates base child dialog with specified props
	bool CreateBaseChildDlg(HWND parent, const iRect &rect, uint32 decoration = DLGDEC_FILL_SOLID | DLGDEC_FRAME_EDGED, bool visible = true)
	{
		uint32 styles = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		if (visible) styles |= WS_VISIBLE;

		uint32 exstyles = 0;

		return CreateGenericDlg(parent,rect,iStringT(_T("Child Dialog")),styles,0,decoration);
	}
};

/// Base modalless dialog class
class iBaseDialog : public iGenericDialog
{
public:
	/// Creates base modalless dialog with specified props
	bool CreateBaseDialog(const iRect &rect, const iStringT &caption, uint32 dlgstyle, bool visible)
	{
		uint32 styles = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		if (visible) styles |= WS_VISIBLE;

		uint32 exstyles = 0;
		if (dlgstyle & DLGSTL_TOOLWINDOW) exstyles |= WS_EX_TOOLWINDOW;
		if (dlgstyle & DLGSTL_APPWINDOW) exstyles |= WS_EX_APPWINDOW;
		if (dlgstyle & DLGSTL_TOPMOST) exstyles |= WS_EX_TOPMOST;

		return CreateGenericDlg(0,rect,caption,styles,exstyles,DLGDEC_FILL_SOLID);
	}

	/// Creates base modalless dialog with specified props
	bool CreateBaseDialog(const iSize &size, const iStringT &caption, uint32 dlgstyle, bool visible)
	{
		iRect rect(
			GetSystemMetrics(SM_CXSCREEN) / 2 - size.w/2,
			GetSystemMetrics(SM_CYSCREEN) / 2 - size.h/2,
			size.w,
			size.h
			);

		return CreateBaseDialog(rect,caption,dlgstyle, visible);
	}

};

#endif //_IOLIB_LITHIUM_DIALOG_BASE_H_
/** @} */ // iolib_li
