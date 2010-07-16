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

#ifndef _HMM_GAME_ARTIFACT_INFO_DIALOG_H_
#define _HMM_GAME_ARTIFACT_INFO_DIALOG_H_

class iDlg_ArtInfo : public iBaseGameDlg
{
public:
	iDlg_ArtInfo(iViewMgr* pViewMgr, uint16 artId, const iHero* pOwner);

private:
	void OnCreateDlg();
	void DoCompose(const iRect& rect);
	iSize ClientSize() const;
	void iCMDH_ControlCommand(iView* pView, CTRL_CMD_ID cmd, sint32 param);

private:
	uint16			m_artId;
	const iHero*	m_pOwner;
	iStringT		m_reqMsg;
	iTextComposer::FontConfig m_fcReqMsg;
};

#endif //_HMM_GAME_ARTIFACT_INFO_DIALOG_H_

