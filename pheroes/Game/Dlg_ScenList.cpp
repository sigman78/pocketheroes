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

#include "stdafx.h"
#include "ScopePtr.h"
#include "Dlg_ScenList.h"

/*
 *	
 */
void EnumScenarios(iScenList& scList)
{
	iStringT strText = gMapsPath + _T("*.phm");
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile(strText.CStr(), &wfd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 ) {
				ScopedPtr<iMapInfo> mapInfo( new iMapInfo() );
				mapInfo->m_bNewGame = true;
				mapInfo->m_FileName = gMapsPath + wfd.cFileName;
				iFilePtr pFile(OpenWin32File(mapInfo->m_FileName));
				if ( !pFile ) continue;
				check(pFile);
				uint32 fourcc; pFile->Read(&fourcc,sizeof(fourcc));
				if (fourcc == GMAP_FILE_HDR_KEY && mapInfo->ReadMapInfo(pFile.get())) {
					scList.Add(mapInfo.Giveup());
				}
			}
		} while (FindNextFile(hFind, &wfd));
		FindClose(hFind);
	}
}

void CleanupScenarioList(iScenList& scList)
{
	for (uint32 xx=0; xx<scList.GetSize(); ++xx) {
		delete scList[xx];
	}
	scList.RemoveAll();

}

/*
 *	Scenario list box
 */
class iScenListBox : public iListBox
{
public:
	iScenListBox(iViewMgr* pViewMgr, IViewCmdHandler* pCmdHandler, const iRect& rect, uint32 uid, const iScenList& scList)
	: iListBox(pViewMgr, pCmdHandler, rect, uid), m_scList(scList)
	{}

	inline uint32 LBItemHeight() const { return 15; }
	inline uint32 LBItemsCount() const { return m_scList.GetSize(); }

private:
	void ComposeLBBackground(const iRect& rect)
	{
		iRect rc = rect;
		gApp.Surface().Darken25Rect(rc);
	}

	void ComposeLBItem(uint32 iIdx, bool bSel, const iRect& irc)
	{
		iTextComposer::FontConfig fc(dlgfc_plain);
		iRect rc=irc;

		ButtonFrame(gApp.Surface(),rc,iButton::Pressed);
		rc.DeflateRect(1);
		if (bSel) {
			gGfxMgr.BlitTile(PDGG_CTILE, gApp.Surface(),rc);
			ButtonFrame(gApp.Surface(),rc,0);
		}

		rc.DeflateRect(1);

		if (!m_scList[iIdx]->Supported()) fc.cmpProps.faceColor = RGB16(192,160,160);

		// Map Size
		gTextComposer.TextOut(fc,gApp.Surface(),rc,gTextMgr[TRID_SHORT_MAPSIZ_SMALL+m_scList[iIdx]->m_Size],iRect(rc.x,rc.y,20,rc.h),AlignCenter);
		rc.DeflateRect(23,0,0,0);

		// Players count
		gTextComposer.TextOut(fc,gApp.Surface(),rc,iFormat(_T("%d/%d"),m_scList[iIdx]->HumanPlayers(),m_scList[iIdx]->TotalPlayers()),iRect(rc.x,rc.y,25,rc.h),AlignCenter);
		rc.DeflateRect(25,0,0,0);

		// Map name
		gTextComposer.TextOut(fc,gApp.Surface(),rc,m_scList[iIdx]->m_Name,rc,AlignLeft);
	}

private:
	const iScenList&	m_scList;
};

/*
 *	Scenario List dialog
 */
iScenListDlg::iScenListDlg(iViewMgr* pViewMgr) 
: iBaseGameDlg(pViewMgr,PID_NEUTRAL), m_selScen(-1)
{
}

iScenListDlg::~iScenListDlg()
{
	// store settings ??

	CleanupScenarioList(m_scList);
}

void iScenListDlg::SortScenarios(SortBy sort_by)
{
	iSortArray<iMapInfo*> sort;
	sint32 idx;
	for (uint32 xx=0; xx<m_scList.GetSize(); ++xx) {
		if (sort_by == Size) idx = m_scList[xx]->m_Size;
		else if (sort_by == Players) idx = m_scList[xx]->m_Players.GetSize();
		else if (sort_by == Name) idx = (m_scList[xx]->m_Name[0] << 16) | m_scList[xx]->m_Name[1];
		if (!m_scList[xx]->Supported()) idx += 0x10000000;
		sort.Insert(m_scList[xx], idx);
	}
	for (uint32 xx=0; xx<m_scList.GetSize(); ++xx) m_scList[xx] = sort.Value(xx);
}

const sint32 yoffs = 18;
void iScenListDlg::OnCreateDlg()
{
	iRect clRect = ClientRect();

	EnumScenarios(m_scList);
	SortScenarios(Name);

	// Listbox header
	AddChild(new iDlgIconButton(m_pMgr, this, iRect(clRect.x,clRect.y + yoffs, 24,DEF_BTN_HEIGHT), PDGG_BTN_MAPSIZE, 501));
	AddChild(new iDlgIconButton(m_pMgr, this, iRect(clRect.x+25,clRect.y + yoffs, 24,DEF_BTN_HEIGHT), PDGG_BTN_PLAYERS_COUNT, 502));
	AddChild(new iTextButton(m_pMgr, this, iRect(clRect.x+50,clRect.y + yoffs, 280-16-50,DEF_BTN_HEIGHT), TRID_MAP_NAME, 503));

	// Listbox
	iScenListBox* pLB = new iScenListBox(m_pMgr, this, iRect(clRect.x,clRect.y + yoffs+DEF_BTN_HEIGHT+1,280-16,120), 100, m_scList);
	AddChild(pLB);
	// Scroll bar
	iPHScrollBar* pScrollBar = new iPHScrollBar(m_pMgr, this, iRect(clRect.x+clRect.w-15, clRect.y + yoffs, 15, 120+DEF_BTN_HEIGHT+1), 300);
	AddChild(pScrollBar);
	pLB->SetScrollBar(pScrollBar);

	// Buttons
	sint32 npos = clRect.x + (clRect.w/2-80);
	AddChild(new iTextButton(m_pMgr,this,iRect(npos,clRect.y2()-DEF_BTN_HEIGHT,50,DEF_BTN_HEIGHT),TRID_OK, DRC_OK,iButton::Visible));
	AddChild(new iTextButton(m_pMgr,this,iRect(npos+55,clRect.y2()-DEF_BTN_HEIGHT,50,DEF_BTN_HEIGHT),TRID_INFO, 301,iButton::Visible));
	AddChild(new iTextButton(m_pMgr,this,iRect(npos+110,clRect.y2()-DEF_BTN_HEIGHT,50,DEF_BTN_HEIGHT),TRID_CANCEL, DRC_CANCEL));

	// Init list
	SortScenarios((SortBy)gSettings.GetEntryValue(CET_NGDSORT));
	if (m_scList.GetSize()) {
		sint32 selScen = iCLAMP<sint32>(0, m_scList.GetSize()-1, gSettings.GetEntryValue(CET_NGDPOS));
		pLB->SetCurSel(selScen, true);
	}
}

void iScenListDlg::DoCompose(const iRect& rect) 
{
	gTextComposer.TextBoxOut(dlgfc_hdr, gApp.Surface(),gTextMgr[TRID_SELECT_SCENARIO_DLG_HDR], iRect(rect.x,rect.y,rect.w,24));
	gApp.Surface().FrameRect(iRect(rect.x-1,rect.y-1+yoffs+DEF_BTN_HEIGHT+1,282-16,120+2),cColor_Black);
}

iSize iScenListDlg::ClientSize() const
{ 
	return iSize(280,150 + DEF_BTN_HEIGHT + DEF_BTN_HEIGHT); 
}

void iScenListDlg::iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param) 
{ 
	uint32 uid = pView->GetUID();
	if (uid == DRC_OK || uid == DRC_CANCEL){
		EndDialog(uid); 
	} else if (uid == 301) {
		iStringT title = m_scList[m_selScen]->m_Name;
		//if (!m_scList[m_selScen]->m_Version.Empty()) title.Addf(_T(" v.%s"), m_scList[m_selScen]->m_Version.CStr());
		iStringT desc = m_scList[m_selScen]->m_Description;
		if (!m_scList[m_selScen]->m_Author.Empty()) desc.Addf(_T("\n\n%s: %s"), gTextMgr[TRID_MAP_AUTHOR], m_scList[m_selScen]->m_Author.CStr());
		iTextDlg tdlg(m_pMgr, title, desc, PID_NEUTRAL, dlgfc_topic, dlgfc_splain);
		tdlg.DoModal();
	} else if (uid >= 501 && uid <= 503) {
		sint32 nval = uid - 501;
		gSettings.SetEntryValue(CET_NGDSORT, nval);
		SortScenarios((SortBy)nval);
		Invalidate();
	} else if (uid == 100) {
		if (cmd == CCI_LBSELCHANGED) {
			m_selScen = param;
			gSettings.SetEntryValue(CET_NGDPOS, param);
			GetChildById(DRC_OK)->SetEnabled(m_selScen != -1 && m_scList[m_selScen]->Supported() && m_scList[m_selScen]->HumanPlayers() > 0);
			GetChildById(301)->SetEnabled(m_selScen != -1 && m_scList[m_selScen]->HumanPlayers() > 0);
		} else if (cmd == CCI_LBSELDBLCLICK) {
			if (m_selScen != -1 && m_scList[m_selScen]->Supported()) EndDialog(DRC_OK); 
		} 
	}
}

