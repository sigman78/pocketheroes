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
#include "gxl.config.file.h"

bool ParseConfigFile(const iStringT& fname, IConfigReaderProc& readerProc)
{
	FILE* file = _tfopen(fname.CStr(),_T("rt"));
	if (!file) return false;

	char buff[1024];
	while (fgets(buff, 1024, file)) {
		iStringA strBuff(buff);

		// Trim spaces
		strBuff.Trim();
		// Check for full line comment
		if (strBuff[0] == '/' && strBuff[1] == '/') continue;
		// split key and value
		sint32 epos = strBuff.Find('=');
		if (epos < 1 || (epos+1) >= (sint32)strBuff.Length()) continue;
		iStringA key = strBuff.Left(epos);
		key.Trim();
		iStringA value = strBuff.Right(strBuff.Length() - (epos+1));
		value.Trim();
		if (key.Empty() || value.Empty()) continue;
		readerProc.OnConfigKey(key, value);
	}

	return true;
}
