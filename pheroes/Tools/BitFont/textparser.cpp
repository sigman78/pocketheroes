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

#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <stdio.h>

#include "he/he.std.h"
#include "he/he.memory.dynbuff.h"
#include "he/he.container.simple.array.h"


struct iSequence {
	iSequence(uint16 val) : sval(val), eval(val) {}

	void Reset(uint16 val)
	{
		sval = eval = val;
	}

	bool PushVal(uint16 val)
	{
		if (val < eval) {
			wprintf(L"ERROR!!!\n");
			exit(-1);
		}
		if (val <= (eval + 1)) {
			eval = val;
			return true;
		}
		return false;
	}

	uint16 	sval;
	uint16 	eval;
};

int wmain(int argc, WCHAR* argv[])
{
	if (argc < 2) {
		wprintf(L"Usage: textparser.exe <text_file>\n");
		return -1;
	}

	FILE* pSrc = _wfopen(argv[1],L"rb");
	if (!pSrc) {
		wprintf(L"Unable to open <%s>!\n", argv[1]);
		return -1;
	}

	uint16 buff[8192];
	uint32 cnt = 0;
	fseek(pSrc, 2, SEEK_SET);

	// read
	uint16 ch;
	while( fread(&ch, sizeof(ch), 1, pSrc) ) {
		if (ch != '\n' && ch != '\r' && ch != ' ' && ch != '\t') buff[cnt++] = ch;
	}

	// sort
	for (uint32 i=0; i<cnt; ++i) {
		for (uint32 j=0; j<i; ++j) {
			if (buff[i] < buff[j]) iSwap(buff[i], buff[j]);
		}
	}

	// Prepare sequences
	iSimpleArray<iSequence> seq;
	iSequence cseq(buff[0]);
	for (uint32 nn=1; nn<cnt; ++nn) {
		if (!cseq.PushVal(buff[nn])) {
			seq.Add(cseq);
			cseq.Reset(buff[nn]);
		}
	}
	seq.Add(cseq);

	uint32 total = 0;
	for (uint32 ss=0; ss<seq.GetSize(); ++ss) {
		if (seq[ss].sval == seq[ss].eval) {
			wprintf(L"%02d: %04X\n", ss, seq[ss].sval);
			total ++;
		} else {
			wprintf(L"%02d: %04X - %04X (%d)\n", ss, seq[ss].sval, seq[ss].eval, seq[ss].eval - seq[ss].sval + 1);
			total += (seq[ss].eval - seq[ss].sval + 1);
		}
	}
	wprintf(L"-------------\nTOTAL: %d characters.\n", total);

	
	
	fclose(pSrc);
	return 0;
}
