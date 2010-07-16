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

// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__6A751B97_F6AF_405D_B1AE_65CF4279BE62__INCLUDED_)
#define AFX_MAINFRM_H__6A751B97_F6AF_405D_B1AE_65CF4279BE62__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "MinimapView.h"
#include "MainView.h"
#include "ChildDlgs.h"
#include "ExportDlg.h"
#include "NewMapDlg.h"
#include "Commands.h"
#include "PropertyPages.h"
#include "MapValidateDlg.h"
#include "TextResDlg.h"

class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
		public CMessageFilter, public CIdleHandler, public IStatusBar
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	iMapHandler			m_Map;
	CSplitterWindow		m_vSplit;
	CHorSplitterWindow	m_hSplit;
	CCommandBarCtrl		m_CmdBar;

	iMinimapView		m_MinimapView;
	CMapEditorView		m_MainView;
	iItemBrowser		m_ItemBrowser;

	void SetText(const iStringT& text)
	{
		::SetWindowText(m_hWndStatusBar,text.CStr());
	}

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{		
		if(pMsg->message ==	WM_KEYDOWN || pMsg->message ==	WM_KEYUP)
			m_MainView.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);

		return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
	}

	void UpdateUI()
	{
		UIEnable( ID_FILE_SAVE, m_Map.IsInited() );
		UIEnable( ID_FILE_SAVEAS, m_Map.IsInited() );

		UIEnable( ID_EXPORT_DATA, m_Map.IsInited() );
		UIEnable( ID_EDIT_UNDO, m_Map.CmdMgr().CanUndo() );
		UIEnable( ID_EDIT_REDO, m_Map.CmdMgr().CanRedo() );

		UIEnable( ID_TOOLS_MAPSPEC, m_Map.IsInited() );
		UIEnable( ID_TOOLS_VALIDATE, m_Map.IsInited() );
		UIEnable( ID_MAPTEXT, m_Map.IsInited() );

		UIEnable(ID_VIEW_GRID,m_Map.IsInited());
		UISetCheck(ID_VIEW_GRID, m_MainView.m_Composer.IsShowGrid());
		UIEnable(ID_VIEW_SHADOWS,m_Map.IsInited());
		UISetCheck(ID_VIEW_SHADOWS, m_MainView.m_Composer.IsShowShadow());
		UIEnable(ID_VIEW_PASSMAP,m_Map.IsInited());
		UISetCheck(ID_VIEW_PASSMAP, m_MainView.m_Composer.IsShowPassMap());
		
		for (sint32 xx=0; xx<4; ++xx) {
			UISetCheck(ID_TOOL_EDIT+xx, (m_MainView.m_NavMode == xx));
		}

		for (xx=0; xx<5; ++xx) {
			UISetCheck(ID_BRUSH01+xx, ((sint32)m_MainView.m_selBrush == xx));
		}

		for (xx=0; xx<STYPE_COUNT; ++xx) {
			UISetCheck(ID_SURF_WATER+xx, (m_MainView.m_selSurf == xx));
		}

		for (xx=PID_NEUTRAL; xx<PID_COUNT; ++xx) {
			UIEnable(ID_PLAYER1+xx,xx==PID_NEUTRAL || m_Map.m_pPlayers[xx]!=NULL);
			UISetCheck(ID_PLAYER1+xx,m_Map.GetCurPlayerId() == xx);
		}

		UIUpdateToolBar();
	}

	virtual BOOL OnIdle()
	{
		UpdateUI();
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_FILE_SAVE, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_FILE_SAVEAS, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_EXPORT_DATA, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_GRID, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_VIEW_SHADOWS, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_VIEW_PASSMAP, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		
		UPDATE_ELEMENT(ID_TOOL_EDIT, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_TOOL_SURFACE, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_TOOL_CREATE, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_TOOL_ERASE, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_TOOLS_MAPSPEC, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_TOOLS_VALIDATE, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_MAPTEXT, UPDUI_MENUPOPUP)

		UPDATE_ELEMENT(ID_BRUSH01, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_BRUSH02, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_BRUSH03, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_BRUSH04, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_BRUSH05, UPDUI_TOOLBAR)

		UPDATE_ELEMENT(ID_SURF_WATER, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_SURF_SAND, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_SURF_DIRT, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_SURF_GRASS, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_SURF_SWAMP, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_SURF_LAVA, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_SURF_WASTELAND, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_SURF_DESERT, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_SURF_SNOW, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_SURF_NDESERT, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_SURF_COBBLES1, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_SURF_COBBLES2, UPDUI_TOOLBAR)

		UPDATE_ELEMENT(ID_PLAYER_NEUTRAL, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_PLAYER1, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_PLAYER2, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_PLAYER3, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_PLAYER4, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_PLAYER5, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_PLAYER6, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_EDIT_UNDO, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_REDO, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_FILE_OPEN, OnFileOpen)
		COMMAND_ID_HANDLER(ID_FILE_SAVE, OnFileSave)
		COMMAND_ID_HANDLER(ID_FILE_SAVEAS, OnFileSaveAs)
		COMMAND_ID_HANDLER(ID_VIEW_GRID, OnViewGrid)
		COMMAND_ID_HANDLER(ID_VIEW_SHADOWS, OnViewShadows)
		COMMAND_ID_HANDLER(ID_VIEW_PASSMAP, OnViewPassmap)
		COMMAND_ID_HANDLER(ID_EXPORT_DATA, OnExportData)
		COMMAND_ID_HANDLER(ID_BATCH_EXPORT, OnBatchExport)
		COMMAND_RANGE_HANDLER(ID_TOOL_EDIT, ID_TOOL_ERASE, OnNavModeChange)
		COMMAND_ID_HANDLER(ID_TOOLS_MAPSPEC, OnMapSpec)
		COMMAND_ID_HANDLER(ID_TOOLS_VALIDATE, OnMapValidate)
		COMMAND_ID_HANDLER(ID_MAPTEXT, OnMapText)
		COMMAND_RANGE_HANDLER(ID_BRUSH01, ID_BRUSH05, OnBrushChange)
		COMMAND_RANGE_HANDLER(ID_SURF_WATER, ID_SURF_COBBLES2, OnSurfChange)
		COMMAND_ID_HANDLER(ID_SURF_UP, OnSurfUp);
		COMMAND_ID_HANDLER(ID_SURF_DOWN, OnSurfDown);
		COMMAND_RANGE_HANDLER(ID_PLAYER_NEUTRAL, ID_PLAYER6, OnPlayerChange)
		COMMAND_ID_HANDLER(ID_EDIT_UNDO, OnEditUndo)
		COMMAND_ID_HANDLER(ID_EDIT_REDO, OnEditRedo)

		COMMAND_ID_HANDLER(ID_ERASE, OnErase)
		COMMAND_ID_HANDLER(ID_SURFACE, OnSurface)		

		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	void OnToolChanged()
	{
		CReBarCtrl rebar = m_hWndToolBar;
		int nSurfBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 2);
		int nBrushBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 3);
		if (m_MainView.m_NavMode == CMapEditorView::NM_Surface) {
			rebar.ShowBand(nSurfBandIndex, TRUE);
			rebar.ShowBand(nBrushBandIndex, TRUE);
		} else {
			rebar.ShowBand(nSurfBandIndex, FALSE);
			rebar.ShowBand(nBrushBandIndex, FALSE);
		}
		UpdateLayout();
	}

	HWND CreateClient()
	{
		iRect clrect = iWMetrics::GetClRect( m_hWnd);
		RECT trect = {0,0,10,10};

		m_vSplit.Create( m_hWnd, RECT(clrect), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN );
		m_vSplit.m_cxyMin =  200;
		m_vSplit.SetSplitterExtendedStyle(SPLIT_RIGHTALIGNED);
		m_vSplit.SetSplitterPos( clrect.w - 220);
		m_vSplit.SetSplitterPane( 0, m_MainView.CreateMainView(m_vSplit.m_hWnd, this, &m_ItemBrowser, &m_MinimapView) );
		m_vSplit.SetSplitterPane( 1, m_hSplit.Create( m_vSplit.m_hWnd, RECT(clrect),NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN ));
		m_hSplit.m_cxyMin =  150;
		m_hSplit.SetSplitterExtendedStyle(0);
		m_hSplit.SetSplitterPos( trect.top + 160);

		m_MinimapView.CreateMinimapView(m_hSplit, &m_MainView);
		m_hSplit.SetSplitterPane( 0, m_MinimapView);

		m_ItemBrowser.CreateItemBrowser(m_hSplit, &m_MainView);
		m_hSplit.SetSplitterPane( 1, m_ItemBrowser);

		OnToolChanged();
		

		return m_vSplit.m_hWnd;
	}

	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if (m_Map.IsInited()) {
			int ret = MessageBox(_T("The map has changed.\n\nDo you want to save the changes?"), _T("Warning"), MB_YESNOCANCEL);
			if (ret == IDYES) {
				if (Save()){
					bHandled = FALSE;
				}
			} else if (ret == IDNO) {
				bHandled = FALSE;
			} 
		} else bHandled = FALSE;
		return 0;
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// create command bar window
		HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
		// attach menu
		m_CmdBar.AttachMenu(GetMenu());
		// load command bar images
		m_CmdBar.LoadImages(IDR_MAINFRAME);
		// remove old menu
		SetMenu(NULL);
		
		HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
		HWND hWndSurfBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_SURF_TYPE, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
		HWND hWndBrushBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_BRUSH_SIZ, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
		
		HIMAGELIST hImages;
		hImages = ImageList_LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDB_MAINTB24),24,1,RGB(255,0,255) ,IMAGE_BITMAP,LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
		::SendMessage( hWndToolBar, TB_SETIMAGELIST, 0, (LPARAM)hImages );
		hImages = ImageList_LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDB_SURFTB24),24,1,RGB(255,0,255) ,IMAGE_BITMAP,LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
		::SendMessage( hWndSurfBar, TB_SETIMAGELIST, 0, (LPARAM)hImages );

		CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
		AddSimpleReBarBand(hWndCmdBar);
		AddSimpleReBarBand(hWndToolBar, NULL, TRUE);
		AddSimpleReBarBand(hWndSurfBar, NULL, TRUE);
		AddSimpleReBarBand(hWndBrushBar, NULL, FALSE);
		
		CreateSimpleStatusBar();
		UIAddToolBar(hWndToolBar);
		UIAddToolBar(hWndSurfBar);
		UIAddToolBar(hWndBrushBar);

		UpdateUI();
		m_hWndClient = CreateClient();
		
		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		return 0;
	}

	LRESULT OnPlayerChange(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_Map.SetCurPlayerId((PLAYER_ID)(wID-ID_PLAYER1));
		return 0;
	}

	LRESULT OnBrushChange(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_MainView.m_selBrush = (wID-ID_BRUSH01);
		return 0;
	}

	LRESULT OnSurfChange(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_MainView.m_selSurf = (SURF_TYPE)(wID-ID_SURF_WATER);
		return 0;
	}

	LRESULT OnSurfUp(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_MainView.m_selSurf = (SURF_TYPE)((WORD)m_MainView.m_selSurf + 1);
		if(m_MainView.m_selSurf >  (SURF_TYPE)(ID_SURF_COBBLES2 - ID_SURF_WATER))
			m_MainView.m_selSurf = (SURF_TYPE)(0);
		return 0;
	}

	LRESULT OnSurfDown(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_MainView.m_selSurf = (SURF_TYPE)((WORD)m_MainView.m_selSurf - 1);
		if(m_MainView.m_selSurf < (SURF_TYPE)0)
			m_MainView.m_selSurf = (SURF_TYPE)(ID_SURF_COBBLES2 - ID_SURF_WATER);
		return 0;
	}



	LRESULT OnNavModeChange(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_MainView.m_NavMode = (CMapEditorView::NavMode)(wID-ID_TOOL_EDIT);
		OnToolChanged();
		return 0;
	}

	LRESULT OnExportData(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if (!m_Map.m_FileName.Empty()) {
			if (!SaveMapToFile( m_Map, m_Map.m_FileName)){
				MessageBox(_T("Unable to save map file!"));
				return 0;
			}
		} else {
			if (!SaveAs()) return 0;
		}

		iExportDlg edlg(m_Map.m_FileName);
		edlg.DoModal();
		return 0;
	}

	LRESULT OnBatchExport(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		iOpenFileDialog odlg(_T("HMM Map files (*.hmm)\0*.hmm"),_T("*.hmm"), NULL, OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT);
		if (odlg.DoModal(m_hWnd) == IDOK) {
			iOpenFileList ofl(odlg.m_pFileName);
			/*
			iStringT msg = iFormat(_T("%d file(s) selected:\n\n"), ofl.m_FileList.GetSize());
			for (uint32 xx=0; xx<ofl.m_FileList.GetSize(); ++xx) {
				msg.Addf(_T("<%s>\n"), ofl.m_FileList[xx].CStr());
			}
			MessageBox(msg.CStr(), 0, MB_OK);*/
			iExportDlg edlg(ofl.m_FileList);
			edlg.DoModal();
		}

		return 0;
	}

	LRESULT OnMapSpec(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CPropertySheetEx sheet( _T("Map Specifications") );
		iMSPropsPage_General gp(m_Map);
		iMSPropsPage_Players pp(m_Map);
		iMSPropsPage_Languages pl(m_Map);
		iMSPropsPage_TimeEvents ep(m_Map);
		sheet.AddPage(gp);
		sheet.AddPage(pp);
		sheet.AddPage(pl);
		sheet.AddPage(ep);
		sheet.DoModal(m_hWnd);
		return 0;
	}

	LRESULT OnMapValidate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		iStringT res;
		if (!m_Map.ValidateMap(res, false)) {
			iMapValidateDlg mvdlg;
			mvdlg.DoMapValidateModal(res);
		} else {
			MessageBox(_T("This map has no error."));
		}
		return 0;
	}

	LRESULT OnMapText(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		iTextResDlg tdlg(&m_Map);
		tdlg.DoModal();
		return 0;
	}

	LRESULT OnViewGrid(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_MainView.m_Composer.ShowGrid(!m_MainView.m_Composer.IsShowGrid());
		m_MainView.Invalidate();
		return 0;
	}

	LRESULT OnViewShadows(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_MainView.m_Composer.ShowShadow(!m_MainView.m_Composer.IsShowShadow());
		m_MainView.Invalidate();
		return 0;
	}

	LRESULT OnViewPassmap(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_MainView.m_Composer.ShowPassMap(!m_MainView.m_Composer.IsShowPassMap());
		m_MainView.Invalidate();
		return 0;
	}

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: add code to initialize document
		iNewMapDlg ndlg;
		if (ndlg.DoNewMapModal(&m_Map) == IDOK) {
			m_Map.m_FileName = _T("");
			gItemMgr.Reset();
			m_Map.CmdMgr().ClearAll();
			m_MainView.SetMap(&m_Map);
			m_MinimapView.SetMap(&m_Map);
			m_ItemBrowser.Update();
		}

		return 0;
	}

	LRESULT OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		iOpenFileDialog odlg(_T("HMM Map files (*.hmm)\0*.hmm"),_T("*.hmm"));
		if (odlg.DoModal(m_hWnd) == IDOK) {
			gItemMgr.Reset();
			m_Map.m_FileName = _T("");
			if (LoadMapFromFile(m_Map, odlg.m_pFileName)) {
				m_MainView.SetMap(&m_Map);
				m_MinimapView.SetMap(&m_Map);
				m_Map.CmdMgr().ClearAll();
				SetWindowText((iStringT(_T("MapEditor - ")) + m_Map.m_ctMgr[MakeTextEntryName(TET_MAP_NAME, NULL)]).CStr());
			} else {
				MessageBox(_T("Unable to open map file!"));
			}
			m_ItemBrowser.Update();
		}
		return 0;
	}

	bool SaveAs()
	{
		iSaveFileDialog sdlg(_T("HMM Map files (*.hmm)\0*.hmm"),_T("*.hmm"));
		if (sdlg.DoModal(m_hWnd) == IDOK) {
			if (!SaveMapToFile( m_Map, sdlg.m_pFileName)){
				MessageBox(_T("Unable to save map file!"));
				return false;
			}
			m_Map.m_FileName = sdlg.m_pFileName;
		} return false;
		return true;
	}

	bool Save()
	{
		if (!m_Map.m_FileName.Empty()) {
			if (!SaveMapToFile( m_Map, m_Map.m_FileName)){
				MessageBox(_T("Unable to save map file!"));
				return false;
			}
			return true;
		}
		return SaveAs();
	}


	LRESULT OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		Save();
		return 0;
	}

	LRESULT OnFileSaveAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		SaveAs();
		return 0;
	}

	LRESULT OnEditUndo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_Map.CmdMgr().Undo( &m_Map );
		return 0;
	}

	LRESULT OnEditRedo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_Map.CmdMgr().Redo( &m_Map );
		return 0;
	}

	LRESULT OnErase(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{		
		m_MainView.m_NavMode = CMapEditorView::NavMode::NM_Erase;
		OnToolChanged();		
		return 0;
	}
	
	LRESULT OnSurface(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{		
		m_MainView.m_NavMode = CMapEditorView::NavMode::NM_Surface;
		OnToolChanged();		
		return 0;
	}	
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__6A751B97_F6AF_405D_B1AE_65CF4279BE62__INCLUDED_)
