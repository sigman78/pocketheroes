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

/** @file
 *	@brief Localization engine implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_LOCALIZATION_H_
#define _IOLIB_HELIUM_LOCALIZATION_H_


//
// For using localized messages you need 
// to implement folowing steps:
//
//	* define IOLIB_ENABLE_LOCALIZATION
//  * declare g_LocMgr as iLocaleMessageManager
//	* Load language file, i.e. g_LocMgr.LoadFromFile(...)
//  * use iLM(...)
//

#include "he.container.hash.h"
#include "he.tables.h"

/// Localization manager class
class iLocaleMessageManager
{
public:
	/// Default constructor
	iLocaleMessageManager();

	/// Constructs iLocaleMessageManager object with specified language file
	iLocaleMessageManager(const iStringT &lng_fname);

	/// Loads specified language file
	bool LoadFromFile(const iStringT &lng_fname);

	/// Returns localized version of specified string of text
	inline const iCharT* GetMessage(const iCharT* str)
	{
		KVHash::Iterator it = m_MsgHash.Find(iStringT(str));
		if (it==m_MsgHash.End()) return str;
		return (*it).m_Value.CStr();
	}

	/// Returns information about availability of localized version of specified string of text
	bool CheckMessage(const iCharT* str)
	{
		KVHash::Iterator it = m_MsgHash.Find(iStringT(str));
		if (it==m_MsgHash.End()) return false;
		return true;
	}

	/// Returns ammount of currently available localized entries
	uint32 MessageCount()
	{
		return m_MsgHash.Count();
	}

private:

	struct iKeyValue 
	{
		iKeyValue()
		{
		}

		iStringT	m_Key;
		iStringT	m_Value;

		inline iKeyValue( const iStringT& k, const iStringT& v )
		: m_Key( k )
		, m_Value( v )
		{
		}
	};

	struct iKeyValueHF 
	{
		uint32 operator() ( const iStringT& str )
		{
			return iTables::HashFn( str );
		}

		iStringT operator() ( iKeyValue& pair ) 
		{
			return pair.m_Key;
		}
	};

public:
	typedef iHashTable< const iStringT&, iKeyValue, iKeyValueHF > KVHash;
	iStringT			m_Title;
	KVHash				m_MsgHash;
};

#ifdef IOLIB_ENABLE_LOCALIZATION
	#define iLM(x) g_LocMgr.GetMessage( _T( ## x ) )
#else
	#define iLM(x) _T( ## x )
#endif //IOLIB_ENABLE_LOCALIZATION


#endif //_IOLIB_HELIUM_LOCALIZATION_H_
/** @} */ // iolib_he
