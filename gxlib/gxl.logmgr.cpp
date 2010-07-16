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

#include "gxl.inc.h"
#include "gxl.logmgr.h"

static LPCTSTR NEW_LINE = _T("\r\n");

iLogMgr::iLogMgr()
: m_pLogFile(NULL)
{
}

iLogMgr::~iLogMgr()
{}

bool iLogMgr::Init(const iStringT& fname)
{
	m_pLogFile = CreateWin32File(fname);
	if (!m_pLogFile) return false;
	uint16 hdr = 0xFEFF;
	m_pLogFile->Write(&hdr, sizeof(hdr));

	return true;
}

void iLogMgr::Log(const iStringT& entry)
{
	if (!m_pLogFile) return;
	m_pLogFile->Write(entry.CStr(), entry.Length()*sizeof(iCharT));
	m_pLogFile->Flush();
}

