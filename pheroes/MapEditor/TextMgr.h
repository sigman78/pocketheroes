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

#ifndef _HMM_MAP_EDITOR_TEXT_MANAGER_H_
#define _HMM_MAP_EDITOR_TEXT_MANAGER_H_

//
class iTextMgr
{
public:
	//
	struct iTextEntry {
		iTextEntry() {}

		iTextEntry(sint16 _idx, const iStringT& _key, TextEntryType _tet)
		: idx(_idx), key(_key), tet(_tet) {}

		sint16		idx;
		sint16		tet;
		iStringT	key;
		iStringT	text[GLNG_COUNT];
	};
	//
	struct KeyValHF {
		uint32 operator() ( const iStringT& str )
		{ return iTables::HashFn( str ); }
		iStringT operator() ( iTextEntry& entry ) 
		{ return entry.key; }
	};

	typedef iHashTable< const iStringT&, iTextEntry, KeyValHF > iKVHash;
	typedef iKVHash::Iterator TextIt;
	typedef iKVHash::ConstIterator ConstTextIt;

private:
	iKVHash			m_TextHash;
	sint16			m_idxCounter;


public:

	inline iTextEntry* GetUnsafeEntryPtr(const iStringT& key)
	{
		iKVHash::Iterator it = m_TextHash.Find(key);
		if (it==m_TextHash.End()) return NULL;
		return &(*it);
	}

	void Reset();

	iTextEntry& AddTextEntry(const iStringT& _key, TextEntryType _tet = TET_DEFAULT)
	{
		check(m_TextHash.Find(_key) == m_TextHash.End());
		iTextEntry& nentry = *m_TextHash.Insert(iTextEntry(m_idxCounter++, _key, _tet));
		return nentry;
	}

	iTextEntry& AddTextEntry(const iStringT& _key, const iStringT& _orig, TextEntryType _tet = TET_DEFAULT)
	{
		//check(m_TextHash.Find(_key) == m_TextHash.End());
		if ( m_TextHash.Find(_key) != m_TextHash.End()) {
			MessageBox(0, _key.CStr(), _T("Redefined text"), MB_OK );
		}
		iTextEntry& nentry = *m_TextHash.Insert(iTextEntry(m_idxCounter++, _key, _tet));
		nentry.text[GLNG_ENGLISH] = _orig;
		return nentry;
	}

	inline const iTextEntry& GetEntry(const iStringT& key) const
	{
		iKVHash::ConstIterator it = m_TextHash.Find(key);
		check (it!=m_TextHash.End());
		return (*it);
	}

	inline const iStringT& operator[] (const iStringT& key) const
	{
		return GetEntry(key).text[GLNG_ENGLISH];
	}

	inline bool HasEntry(const iStringT& key) const { return m_TextHash.Find(key) != m_TextHash.End(); }

	inline const iKVHash& GetHash() const { return m_TextHash; }
	inline iKVHash& GetUnsafeHash() { return m_TextHash; }
	
	inline void RemoveEntry(const iStringT& key)
	{
		iKVHash::Iterator it = m_TextHash.Find(key);
		check (it!=m_TextHash.End());
		m_TextHash.Remove(it);
	}

	inline iStringT& GetText(const iStringT& key, GAME_LANG lng) 
	{
		iKVHash::Iterator it = m_TextHash.Find(key);
		check (it!=m_TextHash.End());
		return (*it).text[lng];
	}

};
//
bool AddLanguageFile(const iStringT& fileName, GAME_LANG lang, iTextMgr& tmgr, bool bPrimary);

#endif //_HMM_MAP_EDITOR_TEXT_MANAGER_H_
