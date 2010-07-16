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
#include "lzo.h"

iTextManager::iTextManager()
{}

iTextManager::~iTextManager()
{
	Cleanup();
}

void iTextManager::Cleanup()
{
}

bool ProcessString(iStringT& str)
{
	str.Trim();

	if (str.Empty() || str[0] != L'{') return false;
	str = str.Mid(str.ReverseFind(L'{'));
	str.Trim(L"{}\n");

	return true;
}

bool iTextManager::Init()
{

#if defined (TRANSLATOR_VERSION)
	// Load Text resources
	iFilePtr file(OpenWin32File(gRootPath + L"language.dat"));
	if (file) {
		m_bHasLngFile = true;
		const uint32 buff_siz = 2048;
		iCharW buff[buff_siz];
		file->Read(buff, sizeof(iCharW));
		if (buff[0] != 0xFEFF) {
			m_bHasLngFile = false;
		} else {
			uint32 reserve = 0;
			uint32 readen = 0;
			while (readen = file->Read(buff+reserve, (buff_siz-reserve) * sizeof(iCharW))) {
				uint32 bpos = 0;
				for (uint32 nn=bpos; nn<readen; ++nn) {
					if (buff[nn] == L'\n') {
						iStringT str(buff+bpos, nn-bpos);
						if (ProcessString(str)) {
							m_lngData.Add(str);
						}
						bpos = nn+1;
					}
				}

				memcpy(buff, buff+bpos, (buff_siz-bpos) * sizeof(iCharW));
				reserve = buff_siz-bpos;
			}
		}

	} else {
		m_bHasLngFile = false;
	}
#endif

	return true;
}

