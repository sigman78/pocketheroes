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

#include "he.std.h"
#include "he.string.h"
#include "he.localization.h"
#include "he.file.h"


//
// iLocaleMessageManager
//

iLocaleMessageManager::iLocaleMessageManager()
{
}

iLocaleMessageManager::iLocaleMessageManager(const iStringT &lng_fname)
{
	LoadFromFile(lng_fname);
}

bool iLocaleMessageManager::LoadFromFile(const iStringT &lng_fname)
{
	iTextFileT msgFile;
	if (!msgFile.Open(lng_fname)) return false;

	m_MsgHash.RemoveAll();

	iSimpleArray<iStringT> tmp_cnt;

	for (uint32 xx=0; xx<msgFile.GetStringsCount(); ++xx)
	{
		iStringT str(msgFile.GetConstString(xx));

		// means comment line, i.e. ignore it
		if (str.Length() > 2 && str[0] != ';' && str[0] != '/')
		{
			// here we check for "<<<"
			if (str.Left(3).Compare(_T("<<<")) == 0)
			{

				// check data from temporal container
				if (tmp_cnt.GetSize() == 2)
				{
					// we have valid number of strings i.e. "original" and "translated"
					m_MsgHash.Insert(iKeyValue(tmp_cnt[0],tmp_cnt[1]));
				}

				// erase temporal container
				tmp_cnt.RemoveAll();
			}
			else
			{
				if (str[0] == '\"')	str.Trim(_T("\""));
				else str.Trim();
				if (!str.Empty())
				{
					// add string to temporal container
					Text2EscSeq(str);
					tmp_cnt.Add(str);
				}
			}
		}
	}
	
	// check data from temporal container
	if (tmp_cnt.GetSize() == 2)
	{
		// we have valid number of strings i.e. "original" and "translated"
		m_MsgHash.Insert(iKeyValue(tmp_cnt[0],tmp_cnt[1]));
	}

	// erase temporal container
	tmp_cnt.RemoveAll();

	return true;
}
