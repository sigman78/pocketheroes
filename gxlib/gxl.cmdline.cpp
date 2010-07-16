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
#include "gxl.ctr.array.h"
#include "gxl.cmdline.h"

iCmdLineParser::iCmdLineParser(LPCTSTR lpstrCmdLine)
{
	Parse(lpstrCmdLine);
}

void iCmdLineParser::Parse(LPCTSTR lpstrCmdLine)
{
	const iCharT* begin = lpstrCmdLine;
	const iCharT* endChar = lpstrCmdLine + _tcslen(lpstrCmdLine)+1;


	while (begin < endChar) {
		if (*begin == '\"') {
			++begin;
			if (begin == endChar) continue;
			if (const iCharT* f = _tcschr(begin, '\"')) {
				if (f == begin){
					++begin;
				} else {
					m_argList.Add(iStringT(begin, f-begin));
					begin = f+1;
				}
			} else {
				if ( _tcslen(begin) > 0 ) m_argList.Add(begin);
				begin = endChar;
			}
		} else {
			if (const iCharT* f = _tcschr(begin, ' ')) {
				if (begin == f) {
					++begin;
				} else {
					m_argList.Add(iStringT(begin, f-begin));
					begin = f+1;
				}
			} else {
				if ( _tcslen(begin) > 0 ) m_argList.Add(begin);
				begin = endChar;
			}
		}
	}
}
