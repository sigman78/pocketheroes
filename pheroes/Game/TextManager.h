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

#ifndef _HMM_GAME_TEXT_MANAGER_H_
#define _HMM_GAME_TEXT_MANAGER_H_

//////////////////////////////////////////////////////////////////////////
typedef uint16 TextResId;

//////////////////////////////////////////////////////////////////////////
class iTextManager
{
public:
	iTextManager();
	~iTextManager();

	bool Init();
	void Cleanup();

#if !defined (TRANSLATOR_VERSION)
	inline const iCharW* operator[] (TextResId resId) const
	{ check(resId < sizeof(LANG_DATA)/sizeof(iCharW)); return LANG_DATA[resId]; }
#else
	inline const iCharW* operator[] (TextResId resId) const
	{ 
		if (m_bHasLngFile){
			check(resId < m_lngData.GetSize()); 
			return m_lngData[resId].CStr();
		} else {
			check(resId < sizeof(LANG_DATA)/sizeof(iCharW)); 
			return LANG_DATA[resId]; 
		}
	}

private:
	bool					m_bHasLngFile;
	iSimpleArray<iStringT>	m_lngData;
#endif
};

#endif //_HMM_GAME_TEXT_MANAGER_H_

