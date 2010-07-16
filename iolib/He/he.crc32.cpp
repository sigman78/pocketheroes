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
#include "he.file.h"
#include "he.tables.h"
#include "he.crc32.h"

uint32 iCrc32::CalcFile(const iStringT &fname)
{
	iFile file;
	uint32 rescrc=0xffffffff;
	if (!file.Open(fname)) return false;

	uint32 readen=0;
	uint8 buff[4096];

	while ((readen=file.Read(buff,4096)) != 0) CalcBlock(buff,readen,rescrc);

	rescrc = ~rescrc;

	return rescrc;
}
