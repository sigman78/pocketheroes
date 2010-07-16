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

#ifndef _HMM_EDITOR_AVATAR_MANAGER_H_
#define _HMM_EDITOR_AVATAR_MANAGER_H_

/*
 *	
 */
class iAvatarMgr
{
	//
	struct KeyVal {
		KeyVal() : m_Value(_T(""),_T(""),0) {}
		inline KeyVal( const iStringT& k) : m_Key( k ), m_Value(k , _T(""),0) {}

		iStringT	m_Key;
		iAvatar		m_Value;
	};

	//
	struct KeyValHF {
		uint32 operator() ( const iStringT& str )
		{ return iTables::HashFn( str ); }
		iStringT operator() ( KeyVal& pair ) 
		{ return pair.m_Key; }
	};

	uint32	m_Counter;

public:
	iAvatarMgr() : m_Counter(0) {}

	typedef iHashTable< const iStringT&, KeyVal, KeyValHF > iKVHash;
	typedef iKVHash::Iterator		AvatarIt;
	typedef iKVHash::ConstIterator	ConstAvatarIt;
	iKVHash		m_AvatarHash;

	iAvatar* AddAvatar(const iStringT& id, const iStringT& nameKey)
	{
		AvatarIt it = m_AvatarHash.Insert(id);
		(*it).m_Value.SetIdx(m_Counter++);
		(*it).m_Value.SetId(id);
		(*it).m_Value.SetNameKey(nameKey);
		return &(*it).m_Value;
	}

	const iAvatar&  operator[](const iStringT& id) const
	{
		ConstAvatarIt it = m_AvatarHash.Find(id);
		if (it==m_AvatarHash.End()) {
			check(0);
		}
		const iAvatar& result = (*it).m_Value;
		return result;
	}
};

#endif //_HMM_EDITOR_AVATAR_MANAGER_H_