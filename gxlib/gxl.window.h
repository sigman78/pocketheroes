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

#ifndef _GXLIB_WINDOW_H_
#define _GXLIB_WINDOW_H_

/*
 *	
 */
struct iDispMsgHnd
{
	virtual void msg_Suspend() = 0;
	virtual void msg_Resume() = 0;
	virtual void msg_OnPaint(HDC hdc) = 0;
};

struct iInputMsgHnd
{
	virtual void msg_OnKeyDown(sint16 key_code) = 0;
	virtual void msg_OnKeyUp(sint16 key_code) = 0;
	virtual void msg_OnMouseMove(sint16 px, sint16 py) = 0;
	virtual void msg_OnMouseDown(sint16 px, sint16 py) = 0;
	virtual void msg_OnMouseUp(sint16 px, sint16 py) = 0;
};

/*
 *	
 */
class iGXApp;
class iWindow
{
public:
	iWindow();
	~iWindow();
	bool Init(HINSTANCE hInst, LPCWSTR wndName, const iSize& siz, iGXApp* pApp );
	void Destroy();

	LRESULT OnMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void SetOwner(iGXApp* pApp);
	void SetSize(const iSize& siz);

	inline iSize GetSize() const
	{ return m_Size; }

	inline HWND GetHWND()
	{ return m_hWnd; }

	inline operator HWND()
	{ return m_hWnd; }

private:
	iSize			m_Size;
	bool			m_bTrack;
	bool			m_bActive;
	HWND			m_hWnd;
	iGXApp*			m_pOwner;
};

#endif //_GXLIB_WINDOW_H_

