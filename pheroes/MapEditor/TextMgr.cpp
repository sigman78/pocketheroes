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

void iTextMgr::Reset()
{
	m_TextHash.RemoveAll();
	m_idxCounter = 0;
}

struct iLangFileEntry {
	iStringT	key;
	iStringT	orig;
	iStringT	trans;
};

bool ParseLangLine(const iStringT& line, iLangFileEntry& entry)
{
	if (line[0] == _T('{')) {
		sint32 bidx = 0;
		sint32 len = 0;
		sint32 eidx = line.Find(_T('}'),bidx+1);
		if (eidx == -1) return false;
		len = eidx-bidx-1;
		check(len);
		entry.key = len?line.Mid(bidx+1,len):_T("");
		bidx = line.Find(_T('{'),eidx);
		if (bidx == -1) return false;
		eidx = line.Find(_T('}'),bidx);
		if (eidx == -1) return false;
		len = eidx-bidx-1;
		entry.orig = len?line.Mid(bidx+1,len):_T("");
		bidx = line.Find(_T('{'),eidx);
		if (bidx == -1) return false;
		eidx = line.Find(_T('}'),bidx);
		if (eidx == -1) return false;
		len = eidx-bidx-1;
		entry.trans = len?line.Mid(bidx+1,len):_T("");
		
		return true;
	}
	return false;
}

bool AddLanguageFile(const iStringT& fileName, GAME_LANG lang, iTextMgr& tmgr, bool bPrimary)
{
	iTextFileW langFile;
	if (!langFile.Open(fileName)) {
		MessageBox(NULL, iFormat(_T("Unable to open language file <%s>!"), fileName.CStr()).CStr(), NULL, MB_OK);
		return false;
	}

	if (bPrimary) {
		check(tmgr.GetHash().Count() == 0);
		tmgr.Reset();
	} else {
		check(tmgr.GetHash().Count() != 0);
	}

	uint32 cnt = langFile.GetStringsCount();
	iLangFileEntry fentry;
	for (uint32 xx=0; xx<cnt; ++xx) {
		if (ParseLangLine(langFile.GetConstString(xx), fentry)) {
			if (bPrimary) {
				iTextMgr::iTextEntry& newEntry = tmgr.AddTextEntry(fentry.key,fentry.orig);
				newEntry.text[lang] = fentry.trans;
			} else {
				iTextMgr::iTextEntry* pEntry = tmgr.GetUnsafeEntryPtr(fentry.key);
				if (pEntry) {
					if (pEntry->text[GLNG_ENGLISH].Compare(fentry.orig) == 0) {
						pEntry->text[lang] = fentry.trans;
					} else {
						MessageBox(0, iFormat(L"<%s> langauage file has different original text for key <%s>!\n<%s>\n<%s>", fileName.CStr(), fentry.key.CStr(), pEntry->text[GLNG_ENGLISH].CStr(), fentry.orig.CStr()).CStr(), 0, MB_OK);
					}
				} else {
					MessageBox(0, iFormat(L"Invalid key <%s> in <%s> translation <%s>!", fentry.key.CStr(), fileName.CStr()).CStr(), 0, MB_OK);
				}
			}
		}
	}

	// Validate language entries
	if (!bPrimary) {
		for (iTextMgr::iKVHash::ConstIterator it = tmgr.GetHash().First(); it != tmgr.GetHash().End(); ++it) {
			const iTextMgr::iTextEntry& entry = *it;
			if (!entry.text[GLNG_ENGLISH].Empty()) {
				if (entry.text[lang].Empty()) {
					MessageBox(0, iFormat(L"No translation for key <%s> in <%s>!", entry.key.CStr(), fileName.CStr()).CStr(), 0, MB_OK);
				}
			}
		}
	}

	return true;

}

void Reset()
{

}
