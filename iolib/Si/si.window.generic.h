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
 *	@brief Generic window class implementation
 */
/** @addtogroup iolib_si
 *  @{
 */
#ifndef _IOLIB_SILICON_GENERIC_WINDOW_H_
#define _IOLIB_SILICON_GENERIC_WINDOW_H_

#ifndef WM_MOUSEWHEEL
#	define WM_MOUSEWHEEL WM_MOUSELAST + 1
#endif //WM_MOUSEWHEEL

/// Generic window class
class iGenericWindow : public CWindowImpl<iGenericWindow, CWindow, CWinTraits<0> >
{
public:

	DECLARE_WND_CLASS(NULL)

	BOOL PreTranslateMessage(MSG *pMsg)
	{
		pMsg;
		return FALSE;
	}

	/// Default constructor
	iGenericWindow();

	/// Destructor
	virtual ~iGenericWindow();

	/// Destroys window and its content
	void DestroyContent();

	/// Creates window with given params
	bool CreateGenericWindow( const iRect &rect, HWND parent, uint32 style, uint32 exstyle, bool layered);

	/// Returns client area rect
	virtual iRect GetClRect() const;
	/// Returns window rect
	virtual iRect GetWndRect() const;
	/// Returns window size
	virtual iSize GetWndSize() const;
	/// Returns window position
	virtual iPoint GetWndPos() const;
	/// Setups new window rect
	virtual void SetWndRect(const iRect &rect);
	/// Setups new window size
	virtual void SetWndSize(const iSize &siz);
	/// Setups new window position
	virtual void SetWndPos(const iPoint &pos);
	
	/// Returns current mouse pointer position in client coordinates
	iPoint GetCurPoint() const;
	/// Returns - is current mouse pointer coordinates belong to window or not
	bool IsMouseOnWindow() const;
	/// Converts screen coordinates into client
	iPoint ScreenToClient(const iPoint& pnt) const;
	/// Converts client coordinates into screen
	iPoint ClientToScreen(const iPoint& pnt) const;

	/// Holds redrawing
	void HoldRedraw();
	/// Releases redrawing
	void ReleaseRedraw();
	/// Redraws window
	void RedrawWnd(const iRect &rect = cInvalidRect);

	/// Mouse Enter message wrapper
	virtual void MsgMouseEnter(void){};
	/// Mouse Leave message wrapper
	virtual void MsgMouseLeave(void){};
	/// WM_MOUSEMOVE message wrapper
	virtual void MsgMouseMove(const iPoint &pos){};
	/// WM_WINDOWPOSCHANGED message wrapper
	virtual void MsgWindowPosChanged(LPWINDOWPOS wndpos){};
	/// WM_WINDOWPOSCHANGING message wrapper
	virtual void MsgWindowPosChanging(LPWINDOWPOS wndpos){};
	/// WM_ACTIVATE message wrapper
	virtual void MsgActivate(bool fActive, bool fMinimized){};
	/// WM_SIZE message wrapper
	virtual void MsgSize(const iSize &nsize){};
	/// WM_PAINT message wrapper
	virtual void MsgPaint(iDC &dc, PAINTSTRUCT &ps){};
	/// Layered version of WM_PAINT message wrapper
	virtual void MsgLayeredPaint(const iRect &rect = cInvalidRect){};

	/// WM_DESTROY message wrapper
	virtual void MsgDestroy(void){};
	/// WM_CLOSE message wrapper
	virtual void MsgClose(void){};
	/// WM_TIMER message wrapper
	virtual void MsgTimer(uint32 tid){};

	/// Mouse Left Button Down message wrapper
	virtual void MsgLButtonDown(const iPoint &pos, uint32 vkey){};
	/// Mouse Left Button Up message wrapper
	virtual void MsgLButtonUp(const iPoint &pos, uint32 vkey){};
	/// Mouse Right Button Down message wrapper
	virtual void MsgRButtonDown(const iPoint &pos, uint32 vkey){};
	/// Mouse Right Button Up message wrapper
	virtual void MsgRButtonUp(const iPoint &pos, uint32 vkey){};
	/// Mouse Middle Button Down message wrapper
	virtual void MsgMButtonDown(const iPoint &pos, uint32 vkey){};
	/// Mouse Middle Button Up message wrapper
	virtual void MsgMButtonUp(const iPoint &pos, uint32 vkey){};
	/// Mouse Left Button Click message wrapper
	virtual void MsgLButtonClick(const iPoint &pos){};
	/// Mouse Right Button Click message wrapper
	virtual void MsgRButtonClick(const iPoint &pos){};
	/// Mouse Middle Button Click message wrapper
	virtual void MsgMButtonClick(const iPoint &pos){};
	/// Mouse Wheel message wrapper
	virtual void MsgMouseWheel(const iPoint &pos, sint32 mDelta){}
	/// WM_GETCURSOR message wrapper
	virtual void MsgSetCursor(void){}

	/// Key Down message wrapper
	virtual void MsgKeyDown(uint32 vkeycode){};
	/// Key Up message wrapper
	virtual void MsgKeyUp(uint32 vkeycode){};

	/// Begin Mouse Tracking message wrapper
	virtual void MsgBeginTrack(const iPoint &pos){};
	/// End Mouse Tracking message wrapper
	virtual void MsgEndTrack(const iPoint &pos){};
	/// Mouse Tracking message wrapper
	virtual void MsgTracking(const iPoint &pos){};

	/// Override the window's default minimum or maximum tracking size
	virtual bool MsgGetMinMaxInfo(const iSize &minsize, const iSize &maxsize){return false;}
	/// WM_NCHITTEST message wrapper
	virtual sint32 MsgNcHitTest(const iPoint &mpos){ return HTCLIENT; }

	/// Enables window
	void Enable();
	/// Disables window
	void Disable();
	/// Checks - is window enabled or not
	bool IsEnabled();

protected:
	// Message Handlers
	LRESULT OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);

	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnNcMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnMButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnMButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);

	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);

	BEGIN_MSG_MAP(iGenericWindow)
	//
		MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanged)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_NCMOUSEMOVE, OnNcMouseMove)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
		MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)
		MESSAGE_HANDLER(WM_MBUTTONDOWN, OnMButtonDown)
		MESSAGE_HANDLER(WM_MBUTTONUP, OnMButtonUp)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_KEYUP, OnKeyUp)
		MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
		MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
	END_MSG_MAP()

	bool m_MouseInside;
	bool m_bTracked;
	bool m_Enabled;
	bool m_Activated;
	bool m_IsLayered;
	bool m_HoldRedraw;
};

#endif //_IOLIB_SILICON_GENERIC_WINDOW_H_
/** @} */ // iolib_si
