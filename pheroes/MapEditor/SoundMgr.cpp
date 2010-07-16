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
#include "SoundMgr.h"

//////////////////////////////////////////////////////////////////////////
class iSoundXmlReader : public Sax::PathHandler
{
public:
	iSoundXmlReader(iSoundMgr& soundMgr)
	: m_soundMgr(soundMgr)
	{}

	bool OnEndDocument()
	{
		return true;
	}

	bool OnEnterElement( const iStringT& path, const Attributes& attr )
	{
		if (path == _T("HMMSoundSet")) {
		} else if (path == _T("HMMSoundSet.Group")) {
		} else if (path == _T("HMMSoundSet.Group.Item")) {
			if (!m_soundMgr.AddSoundItem(attr[_T("chunk")], attr[_T("id")], attr[_T("constId")] )) return false;
		}

		return true;
	}
	
	iSoundMgr&		m_soundMgr;
};
//////////////////////////////////////////////////////////////////////////
iSoundMgr::iSoundMgr()
{
}

iSoundMgr::~iSoundMgr()
{
}

bool iSoundMgr::Init(const iStringT& fileName)
{
	try {
		iFileName fn(fileName);
		m_resPath = fn.GetDevPath();
		// Load sprites
		iSoundXmlReader reader(*this);
		if (!Sax::Parse( fileName, &reader )) throw 0;
	} catch(int) {
		return false;
	}
	return true;
}

sint32 iSoundMgr::FindItem(const iStringT& soundId) const
{
	for (uint32 xx=0; xx<m_items.GetSize(); ++xx) {
		if (m_items[xx].soundId == soundId) return xx;
	}
	return -1;
}

bool iSoundMgr::AddSoundItem(const iStringT& fileName, const iStringT& soundId, const iStringT& constId)
{
	iStringT fname = m_resPath + fileName;
	iSmartPtr<iFileI> wavFile(OpenWin32File(fname));
	if ( !wavFile ) {
		MessageBox(NULL,iFormat(_T("Unable to open wave file %s !"),fname.CStr()).CStr(),_T("Error"),MB_OK);
		return false;
	}

	OutputDebugString(iFormat(_T("Loading sound file '%s'\n"), fname.CStr()).CStr());
	m_items.Add(iSoundItem(fname, soundId, constId));
	return true;
}
