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
#include "he.debug.log.h"

iFile	*iLog::m_File = NULL;
sint32	iLog::m_Indent = 0;
bool	iLog::m_Failed = false;
	
bool iLog::EnsureOpen()
{
	if ( !m_File && !m_Failed ) 
	{
		m_File = new iFile();
		if (!m_File) 
		{
			m_Failed = true;
			return false;
		}

		iStringT fname(GetAppPath() + GetAppFileName() + _T(".iolog"));
		if (!m_File->Create( fname ))
		{
			delete m_File;
			m_File = NULL;
			m_Failed = true;
			return false;
		}
		if ( m_File ) atexit( iLog::Close );
	}
	if ( m_File ) return true;
	return false;
}

void iLog::Close()
{
	if (m_File) delete m_File;
}
