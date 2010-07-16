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
 *	@brief WTL helper functions and wrappers implementation
 */
/** @addtogroup iolib_li
 *  @{
 */
#ifndef _IOLIB_LITHIUM_WTL_HELPER_METHODS_H_
#define _IOLIB_LITHIUM_WTL_HELPER_METHODS_H_

/// WTL helper functions and wrappers namespace
namespace WTLHLP
{
	/// ATL tool bar descriptor structure
	struct _AtlToolBarData
	{
		WORD wVersion;
		WORD wWidth;
		WORD wHeight;
		WORD wItemCount;
		//WORD aItems[wItemCount]

		WORD* items()
			{ return (WORD*)(this+1); }
	};

	/// Creates simple toolbar control
	static HWND CreateSimpleToolBarCtrl(HWND hWndParent, UINT nResourceID, BOOL bInitialSeparator = FALSE, DWORD dwStyle = ATL_SIMPLE_TOOLBAR_STYLE, UINT nID = ATL_IDW_TOOLBAR)
	{
		HINSTANCE hInst = _Module.GetResourceInstance();
		HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(nResourceID), RT_TOOLBAR);
		if (hRsrc == NULL)
			return NULL;

		HGLOBAL hGlobal = ::LoadResource(hInst, hRsrc);
		if (hGlobal == NULL)
			return NULL;

		_AtlToolBarData* pData = (_AtlToolBarData*)::LockResource(hGlobal);
		if (pData == NULL)
			return NULL;
		ATLASSERT(pData->wVersion == 1);

		WORD* pItems = pData->items();
		int nItems = pData->wItemCount + (bInitialSeparator ? 1 : 0);
		TBBUTTON* pTBBtn = (TBBUTTON*)_alloca(nItems * sizeof(TBBUTTON));

		// set initial separator (half width)
		if(bInitialSeparator)
		{
			pTBBtn[0].iBitmap = 4;
			pTBBtn[0].idCommand = 0;
			pTBBtn[0].fsState = 0;
			pTBBtn[0].fsStyle = TBSTYLE_SEP;
			pTBBtn[0].dwData = 0;
			pTBBtn[0].iString = 0;
		}

		int nBmp = 0;
		for(int i = 0, j = bInitialSeparator ? 1 : 0; i < pData->wItemCount; i++, j++)
		{
			if(pItems[i] != 0)
			{
				pTBBtn[j].iBitmap = nBmp++;
				pTBBtn[j].idCommand = pItems[i];
				pTBBtn[j].fsState = TBSTATE_ENABLED;
				pTBBtn[j].fsStyle = TBSTYLE_BUTTON;
				pTBBtn[j].dwData = 0;
				pTBBtn[j].iString = 0;
			}
			else
			{
				pTBBtn[j].iBitmap = 8;
				pTBBtn[j].idCommand = 0;
				pTBBtn[j].fsState = 0;
				pTBBtn[j].fsStyle = TBSTYLE_SEP;
				pTBBtn[j].dwData = 0;
				pTBBtn[j].iString = 0;
			}
		}

		HWND hWnd = ::CreateWindowEx(0, TOOLBARCLASSNAME, NULL, dwStyle, 0,0,100,100,
				hWndParent, (HMENU)LongToHandle(nID), _Module.GetModuleInstance(), NULL);

		::SendMessage(hWnd, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0L);

		// check if font is taller than our bitmaps
		CFontHandle font = (HFONT)::SendMessage(hWnd, WM_GETFONT, 0, 0L);
		if(font.IsNull())
			font = AtlGetDefaultGuiFont();
		LOGFONT lf;
		font.GetLogFont(lf);
		WORD cyFontHeight = (WORD)abs(lf.lfHeight);

		if(AtlIsAlphaBitmapResource(nResourceID))	// alpha channel bitmap (valid for Windows XP only)
		{
			HIMAGELIST hImageList = ImageList_LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(nResourceID), pData->wWidth, 1, CLR_NONE, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
			ATLASSERT(hImageList != NULL);
			::SendMessage(hWnd, TB_SETIMAGELIST, 0, (LPARAM)hImageList);
		}
		else
		{
			TBADDBITMAP tbab;
			tbab.hInst = hInst;
			tbab.nID = nResourceID;
			::SendMessage(hWnd, TB_ADDBITMAP, nBmp, (LPARAM)&tbab);
		}

		TBADDBITMAP tbab;
		tbab.hInst = hInst;
		tbab.nID = nResourceID;
		::SendMessage(hWnd, TB_ADDBUTTONS, nItems, (LPARAM)pTBBtn);
		::SendMessage(hWnd, TB_SETBITMAPSIZE, 0, MAKELONG(pData->wWidth, max(pData->wHeight, cyFontHeight)));
		::SendMessage(hWnd, TB_SETBUTTONSIZE, 0, MAKELONG(pData->wWidth + 7, max(pData->wHeight, cyFontHeight) + 7));

		return hWnd;
	}

	/// Creates simple rebar control
	static HWND CreateSimpleReBarCtrl(HWND hWndParent, DWORD dwStyle = ATL_SIMPLE_REBAR_STYLE, UINT nID = ATL_IDW_TOOLBAR)
	{
		// Ensure style combinations for proper rebar painting
		if(dwStyle & CCS_NODIVIDER && dwStyle & WS_BORDER)	dwStyle &= ~WS_BORDER;
		else if(!(dwStyle & WS_BORDER) && !(dwStyle & CCS_NODIVIDER)) dwStyle |= CCS_NODIVIDER;

		// Create rebar window
		HWND hWndReBar = ::CreateWindowEx(0, REBARCLASSNAME, NULL, dwStyle, 0, 0, 100, 100, hWndParent, (HMENU)LongToHandle(nID), _Module.GetModuleInstance(), NULL);
		if(hWndReBar == NULL)
		{
			ATLTRACE2(atlTraceUI, 0, _T("Failed to create rebar.\n"));
			return NULL;
		}

		// Initialize and send the REBARINFO structure
		REBARINFO rbi;
		rbi.cbSize = sizeof(REBARINFO);
		rbi.fMask  = 0;
		if(!::SendMessage(hWndReBar, RB_SETBARINFO, 0, (LPARAM)&rbi))
		{
			ATLTRACE2(atlTraceUI, 0, _T("Failed to initialize rebar.\n"));
			::DestroyWindow(hWndReBar);
			return NULL;
		}

		return hWndReBar;
	}

	/// Adds simple rebar band control
	static BOOL AddSimpleReBarBandCtrl(HWND hWndReBar, HWND hWndBand, int nID = 0, LPTSTR lpstrTitle = NULL, BOOL bNewRow = FALSE, int cxWidth = 0, BOOL bFullWidthAlways = FALSE)
	{
		ATLASSERT(::IsWindow(hWndReBar));	// must be already created
#ifdef _DEBUG
		// block - check if this is really a rebar
		{
			TCHAR lpszClassName[sizeof(REBARCLASSNAME)];
			::GetClassName(hWndReBar, lpszClassName, sizeof(REBARCLASSNAME));
			ATLASSERT(lstrcmp(lpszClassName, REBARCLASSNAME) == 0);
		}
#endif //_DEBUG
		ATLASSERT(::IsWindow(hWndBand));	// must be already created

		// Get number of buttons on the toolbar
		int nBtnCount = (int)::SendMessage(hWndBand, TB_BUTTONCOUNT, 0, 0L);

		// Set band info structure
		REBARBANDINFO rbBand;
		rbBand.cbSize = sizeof(REBARBANDINFO);
#if (_WIN32_IE >= 0x0400)
		rbBand.fMask = RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_ID | RBBIM_SIZE | RBBIM_IDEALSIZE;
#else
		rbBand.fMask = RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_ID | RBBIM_SIZE;
#endif //!(_WIN32_IE >= 0x0400)
		if(lpstrTitle != NULL)
			rbBand.fMask |= RBBIM_TEXT;
		rbBand.fStyle = RBBS_CHILDEDGE;
#if (_WIN32_IE >= 0x0500)
		if(nBtnCount > 0)	// add chevron style for toolbar with buttons
			rbBand.fStyle |= RBBS_USECHEVRON;
#endif //(_WIN32_IE >= 0x0500)
		if(bNewRow)
			rbBand.fStyle |= RBBS_BREAK;

		rbBand.lpText = lpstrTitle;
		rbBand.hwndChild = hWndBand;
		if(nID == 0)	// calc band ID
			nID = ATL_IDW_BAND_FIRST + (int)::SendMessage(hWndReBar, RB_GETBANDCOUNT, 0, 0L);
		rbBand.wID = nID;

		// Calculate the size of the band
		BOOL bRet;
		RECT rcTmp;
		if(nBtnCount > 0)
		{
			bRet = (BOOL)::SendMessage(hWndBand, TB_GETITEMRECT, nBtnCount - 1, (LPARAM)&rcTmp);
			ATLASSERT(bRet);
			rbBand.cx = (cxWidth != 0) ? cxWidth : rcTmp.right;
			rbBand.cyMinChild = rcTmp.bottom - rcTmp.top;
			if(bFullWidthAlways)
			{
				rbBand.cxMinChild = rbBand.cx;
			}
			else if(lpstrTitle == 0)
			{
				bRet = (BOOL)::SendMessage(hWndBand, TB_GETITEMRECT, 0, (LPARAM)&rcTmp);
				ATLASSERT(bRet);
				rbBand.cxMinChild = rcTmp.right;
			}
			else
			{
				rbBand.cxMinChild = 0;
			}
		}
		else	// no buttons, either not a toolbar or really has no buttons
		{
			bRet = ::GetWindowRect(hWndBand, &rcTmp);
			ATLASSERT(bRet);
			rbBand.cx = (cxWidth != 0) ? cxWidth : (rcTmp.right - rcTmp.left);
			rbBand.cxMinChild = bFullWidthAlways ? rbBand.cx : 0;
			rbBand.cyMinChild = rcTmp.bottom - rcTmp.top;
		}

#if (_WIN32_IE >= 0x0400)
		rbBand.cxIdeal = rbBand.cx;
#endif //(_WIN32_IE >= 0x0400)

		// Add the band
		LRESULT lRes = ::SendMessage(hWndReBar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbBand);
		if(lRes == 0)
		{
			ATLTRACE2(atlTraceUI, 0, _T("Failed to add a band to the rebar.\n"));
			return FALSE;
		}

#if (_WIN32_IE >= 0x0501)
		DWORD dwExStyle = (DWORD)::SendMessage(hWndBand, TB_GETEXTENDEDSTYLE, 0, 0L);
		::SendMessage(hWndBand, TB_SETEXTENDEDSTYLE, 0, dwExStyle | TBSTYLE_EX_HIDECLIPPEDBUTTONS);
#endif //(_WIN32_IE >= 0x0501)
		return TRUE;
	}

}; // namespace WTLHLP


#endif //_IOLIB_LITHIUM_WTL_HELPER_METHODS_H_
/** @} */ // iolib_li
