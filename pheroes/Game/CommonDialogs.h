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

#ifndef _HMM_GAME_COMMON_DIALOGS_H_
#define _HMM_GAME_COMMON_DIALOGS_H_

/*
 *	Base game dialog
 */
class iBaseGameDlg : public iDialog, public  IViewCmdHandler
{
public:
	iBaseGameDlg(iViewMgr* pViewMgr, PLAYER_ID pid);
	virtual ~iBaseGameDlg() {}

	iRect ClientRect() const;
	iSize GetMaxClientSize() const;

	// pure virtuals
	virtual void DoCompose(const iRect& clRect)  =0;
	virtual iSize ClientSize() const  =0;

private:
	void OnCompose();
	iSize GetDlgMetrics() const;

protected:
	PLAYER_ID	m_pid;
};

/*
 *	Base text dialog MB_OK
 */
class iTextDlg : public iBaseGameDlg
{
public:
	iTextDlg(iViewMgr* pViewMgr, const iStringT& title, const iStringT& text, PLAYER_ID pid, iTextComposer::FontConfig fc_title = dlgfc_hdr, iTextComposer::FontConfig fc_text = dlgfc_plain);
	virtual ~iTextDlg() {}
	virtual void OnCreateDlg();
	void SetText(const iStringT& text);

protected:
	virtual void DoCompose(const iRect& clRect);
	virtual iSize ClientSize() const;

private:
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

protected:
	iTextComposer::FontConfig m_fcTitle; 
	iTextComposer::FontConfig m_fcText;
	iStringT	m_title;
	iStringT	m_text;
};

/*
 *	Extended text dialog MB_OK
 */
class iExtTextDlg : public iBaseGameDlg
{
public:
	iExtTextDlg(iViewMgr* pViewMgr, const iStringT& title, const iStringT& text, const iStringT& sign, PLAYER_ID pid);
	virtual ~iExtTextDlg() {}
	virtual void OnCreateDlg();

protected:
	virtual void DoCompose(const iRect& clRect);
	virtual iSize ClientSize() const;

private:
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

protected:
	iStringT	m_title;
	iStringT	m_text;
	iStringT	m_sign;
};

/*
 *	Base text with icon dialog MB_OK
 */
class iIconDlg : public iTextDlg
{
public:
	iIconDlg(iViewMgr* pViewMgr, const iStringT& title, const iStringT& text, SpriteId sid, PLAYER_ID pid);
	virtual ~iIconDlg() {}

protected:
	virtual void DoCompose(const iRect& clRect);
	virtual iSize ClientSize() const;

protected:
	SpriteId	m_sid;
};

/*
 *	Question dialog MB_YESNO
 */
class iQuestDlg : public iTextDlg
{
public:
	iQuestDlg(iViewMgr* pViewMgr, const iStringT& title, const iStringT& text, PLAYER_ID pid);
	virtual ~iQuestDlg() {}

private:
	void OnCreateDlg();
};

#endif //_HMM_GAME_COMMON_DIALOGS_H_

