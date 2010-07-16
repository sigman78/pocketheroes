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
 *	@brief Common dialogs implementation
 */
/** @addtogroup iolib_li
 *  @{
 */
#ifndef _IOLIB_LITHIUM_COMMON_DIALOGS_IMPLEMENTATION_H_
#define _IOLIB_LITHIUM_COMMON_DIALOGS_IMPLEMENTATION_H_

/**********************************************************************************
* Usage Sample
***********************************************************************************

iFolderBrowserDialog folderdlg;
if (folderdlg.DoModal(m_hWnd, _T("C:\\!!!\\")) == IDOK)
{
	MessageBox(folderdlg.m_FolderName.CStr());
}

iOpenFileDialog fdlg(_T("Source Files (*.c, *.cpp, *.h, *.asm, *.inc)\0*.c;*.cpp;*.h;*.asm;*.inc\0"),_T("*.exe"));
if (fdlg.DoModal(m_hWnd,true,folderdlg.m_FolderName.CStr()) == IDOK)
{
	iOpenFileList ofl(fdlg.m_pFileName);
	for (uint32 xx=0; xx<ofl.m_FileList.GetSize(); ++xx)
	{
		iStringT output(_T("Full Name: "));
		output += ofl.m_FileList[xx] + _T("\n");

		iFileName fname(ofl.m_FileList[xx]);

		output += _T("Device: ");
		output += fname.GetDev() + _T("\n");

		output += _T("Path: ");
		output += fname.GetPath() + _T("\n");

		output += _T("FileName: ");
		output += fname.GetName() + _T("\n");

		output += _T("Extension: ");
		output += fname.GetExt() + _T("\n");

		MessageBox(output.CStr());
	}
}
************************************************************************************/

const uint32 MAX_FILENAME_SIZE = 5000;

/// Common Open/Save file dialog class template
template <class T>
class ATL_NO_VTABLE iFileDialogImpl : public CDialogImplBase
{
public:
	OPENFILENAME m_ofn;
	BOOL	m_bOpenFileDialog;			// TRUE for file open, FALSE for file save
	iCharT*	m_pFileTitle;				// contains file title after return
	iCharT*	m_pFileName;				// contains full path name after return

	/// Constructs iFileDialogImpl class with specified params
	iFileDialogImpl(
				BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
				const iCharT* lpszFilter = NULL,
				const iCharT* lpszDefExt = NULL,
				const iCharT* lpszFileName = NULL,
				uint32 dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
				)
	{
		memset(&m_ofn, 0, sizeof(m_ofn)); // initialize structure to 0/NULL


		m_pFileName = new iCharT[MAX_FILENAME_SIZE];
		m_pFileTitle = new iCharT[MAX_FILENAME_SIZE];

		m_pFileName[0] = '\0';
		m_pFileTitle[0] = '\0';

		m_bOpenFileDialog = bOpenFileDialog;

		m_ofn.lStructSize = sizeof(m_ofn);
#if (_WIN32_WINNT >= 0x0500)
		// adjust struct size if running on older version of Windows
		if(AtlIsOldWindows()){
			ATLASSERT(sizeof(m_ofn) > OPENFILENAME_SIZE_VERSION_400);	// must be
			m_ofn.lStructSize = OPENFILENAME_SIZE_VERSION_400;
		}
#endif //(_WIN32_WINNT >= 0x0500)
		m_ofn.lpstrFile = m_pFileName;
		m_ofn.nMaxFile = MAX_FILENAME_SIZE;
		m_ofn.lpstrDefExt = lpszDefExt;
		m_ofn.lpstrFileTitle = (iCharT*)m_pFileTitle;
		m_ofn.nMaxFileTitle = MAX_FILENAME_SIZE;
		m_ofn.Flags |= dwFlags | OFN_EXPLORER | OFN_ENABLESIZING;
		m_ofn.lpstrFilter = lpszFilter;
		m_ofn.hInstance = _Module.GetResourceInstance();
		m_ofn.lpfnHook = (LPOFNHOOKPROC)T::StartDialogProc;
		m_ofn.hwndOwner = 0;

		// setup initial file name
		if(lpszFileName != NULL) lstrcpyn(m_pFileName, lpszFileName, 4096);
	}

	/// Destructor
	~iFileDialogImpl()
	{
		delete[] m_pFileName;
		delete[] m_pFileTitle;
	}

	/// Call this function to display the Windows common file dialog box and allow the user to browse files and directories and enter a filename
	INT_PTR DoModal(HWND hWndParent = ::GetActiveWindow(), bool allow_msel = false, const iCharT* defpath=NULL, const iCharT* caption=NULL)
	{
		//check(m_ofn.Flags & OFN_ENABLEHOOK);
		//check(m_ofn.lpfnHook != NULL);	// can still be a user hook

		check(m_ofn.Flags & OFN_EXPLORER);

		if(m_ofn.hwndOwner == NULL)		// set only if not specified before
			m_ofn.hwndOwner = hWndParent;

		check(m_hWnd == NULL);
		_Module.AddCreateWndData(&m_thunk.cd, (CDialogImplBase*)this);

		if (allow_msel) m_ofn.Flags |= OFN_ALLOWMULTISELECT;
		if (caption) m_ofn.lpstrTitle = caption;

		m_ofn.lpstrInitialDir = defpath;

		BOOL bRet;
		if(m_bOpenFileDialog)
			bRet = ::GetOpenFileName(&m_ofn);
		else
			bRet = ::GetSaveFileName(&m_ofn);

		m_hWnd = NULL;

		return bRet ? IDOK : IDCANCEL;
	}

	/*
// Attributes
	CWindow GetFileDialogWindow() const
	{
		check(::IsWindow(m_hWnd));
		return CWindow(GetParent());
	}

	int GetFilePath(LPTSTR lpstrFilePath, int nLength) const
	{
		check(::IsWindow(m_hWnd));
		check(m_ofn.Flags & OFN_EXPLORER);

		return (int)GetFileDialogWindow().SendMessage(CDM_GETFILEPATH, nLength, (LPARAM)lpstrFilePath);
	}

	int GetFolderIDList(void* lpBuff, int nLength) const
	{
		check(::IsWindow(m_hWnd));
		check(m_ofn.Flags & OFN_EXPLORER);

		return (int)GetFileDialogWindow().SendMessage(CDM_GETFOLDERIDLIST, nLength, (LPARAM)lpBuff);
	}

	int GetFolderPath(LPTSTR lpstrFolderPath, int nLength) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(m_ofn.Flags & OFN_EXPLORER);

		return (int)GetFileDialogWindow().SendMessage(CDM_GETFOLDERPATH, nLength, (LPARAM)lpstrFolderPath);
	}

	int GetSpec(LPTSTR lpstrSpec, int nLength) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(m_ofn.Flags & OFN_EXPLORER);

		return (int)GetFileDialogWindow().SendMessage(CDM_GETSPEC, nLength, (LPARAM)lpstrSpec);
	}

	void SetControlText(int nCtrlID, LPCTSTR lpstrText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(m_ofn.Flags & OFN_EXPLORER);

		GetFileDialogWindow().SendMessage(CDM_SETCONTROLTEXT, nCtrlID, (LPARAM)lpstrText);
	}

	void SetDefExt(LPCTSTR lpstrExt)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(m_ofn.Flags & OFN_EXPLORER);

		GetFileDialogWindow().SendMessage(CDM_SETDEFEXT, 0, (LPARAM)lpstrExt);
	}

	BOOL GetReadOnlyPref() const	// return TRUE if readonly checked
	{
		return (m_ofn.Flags & OFN_READONLY) ? TRUE : FALSE;
	}

// Operations
	void HideControl(int nCtrlID)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(m_ofn.Flags & OFN_EXPLORER);

		GetFileDialogWindow().SendMessage(CDM_HIDECONTROL, nCtrlID);
	}

// Special override for common dialogs
	BOOL EndDialog(INT_PTR nRetCode = 0)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		GetFileDialogWindow().SendMessage(WM_COMMAND, MAKEWPARAM(IDCANCEL, 0));
		return TRUE;
	}
*/

// Message map and handlers
	BEGIN_MSG_MAP(CFileDialogImpl)
		NOTIFY_CODE_HANDLER(CDN_FILEOK, _OnFileOK)
		NOTIFY_CODE_HANDLER(CDN_FOLDERCHANGE, _OnFolderChange)
		NOTIFY_CODE_HANDLER(CDN_HELP, _OnHelp)
		NOTIFY_CODE_HANDLER(CDN_INITDONE, _OnInitDone)
		NOTIFY_CODE_HANDLER(CDN_SELCHANGE, _OnSelChange)
		NOTIFY_CODE_HANDLER(CDN_SHAREVIOLATION, _OnShareViolation)
		NOTIFY_CODE_HANDLER(CDN_TYPECHANGE, _OnTypeChange)
	END_MSG_MAP()

	LRESULT _OnFileOK(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		T* pT = static_cast<T*>(this);
		return !pT->OnFileOK((LPOFNOTIFY)pnmh);
	}
	LRESULT _OnFolderChange(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		T* pT = static_cast<T*>(this);
		pT->OnFolderChange((LPOFNOTIFY)pnmh);
		return 0;
	}
	LRESULT _OnHelp(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		T* pT = static_cast<T*>(this);
		pT->OnHelp((LPOFNOTIFY)pnmh);
		return 0;
	}
	LRESULT _OnInitDone(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		T* pT = static_cast<T*>(this);
		pT->OnInitDone((LPOFNOTIFY)pnmh);
		return 0;
	}
	LRESULT _OnSelChange(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		T* pT = static_cast<T*>(this);
		pT->OnSelChange((LPOFNOTIFY)pnmh);
		return 0;
	}
	LRESULT _OnShareViolation(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		T* pT = static_cast<T*>(this);
		return pT->OnShareViolation((LPOFNOTIFY)pnmh);
	}
	LRESULT _OnTypeChange(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		T* pT = static_cast<T*>(this);
		pT->OnTypeChange((LPOFNOTIFY)pnmh);
		return 0;
	}

	/// On OK button pressed overrideable function
	BOOL OnFileOK(LPOFNOTIFY /*lpon*/)
	{
		return TRUE;
	}

	/// On folder changed overrideable function
	void OnFolderChange(LPOFNOTIFY /*lpon*/)
	{
	}

	/// On help button pressed overrideable function
	void OnHelp(LPOFNOTIFY /*lpon*/)
	{
	}

	/// On end of initialization overrideable function
	void OnInitDone(LPOFNOTIFY /*lpon*/)
	{
	}

	/// On selection changed overrideable function
	void OnSelChange(LPOFNOTIFY /*lpon*/)
	{
	}

	/// On share violation overrideable function
	int OnShareViolation(LPOFNOTIFY /*lpon*/)
	{
		return 0;
	}

	/// On type changed overrideable function
	void OnTypeChange(LPOFNOTIFY /*lpon*/)
	{
	}
};

/// Common Open file dialog class
class iOpenFileDialog : public iFileDialogImpl<iOpenFileDialog>
{
public:
	iOpenFileDialog(
			const iCharT* lpszFilter = NULL,
			const iCharT* lpszDefExt = NULL,
			const iCharT* lpszFileName = NULL,
			uint32 dwFlags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST
			)
			: iFileDialogImpl<iOpenFileDialog>(TRUE, lpszFilter, lpszDefExt, lpszFileName, dwFlags)
	{ }

	// override base class map and references to handlers
	DECLARE_EMPTY_MSG_MAP()
};

/// Common Save file dialog class
class iSaveFileDialog : public iFileDialogImpl<iSaveFileDialog>
{
public:
	iSaveFileDialog(
			const iCharT* lpszFilter = NULL,
			const iCharT* lpszDefExt = NULL,
			const iCharT* lpszFileName = NULL,
			uint32 dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
			)
			: iFileDialogImpl<iSaveFileDialog>(FALSE, lpszFilter, lpszDefExt, lpszFileName, dwFlags)
	{ }

	// override base class map and references to handlers
	DECLARE_EMPTY_MSG_MAP()
};

/// File list container
class iOpenFileList
{
public:
	/// Constructs iOpenFileList object with specified list
	iOpenFileList(const iCharT* flist)
	{
		ParseList(flist);
	}

	/// Parses null terminated list of strings
	void ParseList(const iCharT* flist)
	{
		iSimpleArray<iStringT>	tarr;

		for(;;)
		{
			sint32 sl = _tcslen(flist);
			if (sl > 0)
			{
				tarr.Add(iStringT(flist));
				flist += sl+1;
			}
			else break;
		}

		if (tarr.GetSize() > 1)
		{
			for (uint32 xx=1; xx<tarr.GetSize(); ++xx)
			{
				m_FileList.Add(tarr[0] + _T("\\") + tarr[xx]);
			}
		}
		else if (tarr.GetSize() == 1)
		{
			m_FileList.Add(tarr[0]);
		}
	}

	iSimpleArray<iStringT>	m_FileList;
};

/// Common folder browser dialog class
class iFolderBrowserDialog
{
public:
	
	/// Default constructor
	iFolderBrowserDialog()
	{
	}

	/// Call this function to display the Windows common floder list dialog box and allow the user to browse directories
	sint32 DoModal(HWND parent = ::GetActiveWindow(), const iCharT* defpath=NULL, const iCharT* caption=_T("Select a folder:"), const iCharT* root = NULL)
	{
		iCharT path[_MAX_PATH];
		IMalloc* pMalloc;
		IShellFolder* pDesktop;
		LPITEMIDLIST pidlRoot, pidl;
		sint32 bRet = IDCANCEL;

		if (SHGetMalloc(&pMalloc) != NOERROR) return IDCANCEL;
		
		BROWSEINFO bi;

		/* gets the ITEMIDLIST for the desktop folder. */
		if (SHGetDesktopFolder(&pDesktop) == NOERROR) {
			ULONG cchEaten=0;
			ULONG dwAttr=0;

			/* translates the root path into ITEMIDLIST. */
			if (pDesktop->ParseDisplayName( NULL, NULL, CvtT2W<>(root).Unsafe(), &cchEaten, &pidlRoot, &dwAttr) != NOERROR) pidlRoot = NULL;
			pDesktop->Release();
		}


		/* fills the structure. */
		bi.hwndOwner = parent;
		bi.pidlRoot = pidlRoot;
		bi.pszDisplayName = path;
		bi.lpszTitle = _T("Choose a folder.");
		bi.ulFlags = 0;
		bi.lpfn = (BFFCALLBACK)BrowseCallbackProc;
		bi.lParam = (LPARAM)defpath;

		/* invokes the dialog. */
		if ((pidl = SHBrowseForFolder(&bi)) != NULL) 
		{
			bRet = SHGetPathFromIDList(pidl, path);
			m_FolderName = iStringT(path);
			pMalloc->Free(pidl);
		}

		/* clean up. */
		if (pidlRoot)
			pMalloc->Free(pidlRoot);
		pMalloc->Release();

		return bRet;
	}

	static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
	{
		switch (uMsg) 
		{
			case BFFM_INITIALIZED:
				/* change the selected folder. */
				SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
				break;
			case BFFM_SELCHANGED:
				break;
			default:
				break;
		}
		return(0);
	}


	iStringT	m_FolderName;
};

#endif //_IOLIB_LITHIUM_COMMON_DIALOGS_IMPLEMENTATION_H_
/** @} */ // iolib_li
