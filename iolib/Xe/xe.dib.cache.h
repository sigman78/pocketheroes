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
 *	@brief iDibCache class implementation
 */
/** @addtogroup iolib_xe
 *  @{
 */
#ifndef _IOLIB_XENON_DIB_CHACHE_H_
#define _IOLIB_XENON_DIB_CHACHE_H_

#include "xe.dib.h"
#include "../he/he.tables.h"
#include "../he/he.container.cache.h"

/// Key-Value pair class template for iDibCache
template <class iKEY>
struct iDibCacheKV {
	/// Constructs iDibCacheKV with given key and dib
	iDibCacheKV(const iKEY& _key, const iDib& _dib)
	: key(_key)
	, dib(_dib)
	{}

	iKEY	key;
	iDib	dib;

	struct Hasher {
		typedef iKEY KeyType;
		const iKEY& operator()( iDibCacheKV* p ) const
		{ return p->key; }
		const uint32 operator()( const iKEY& k ) const
		{ return iTables::HashFn( k ); }
	};
};

/// Dib cache class template
template <class iKEY>
class iDibCache : private iCache<iDibCacheKV<iKEY>, typename iDibCacheKV<iKEY>::Hasher >
{
public:
	/// Default constructor
	iDibCache(uint32 sizLimit = 0)
	: iCache<iDibCacheKV<iKEY>,iDibCacheKV<iKEY>::Hasher>(sizLimit)
	{ }


	/// Destructor
	~iDibCache()
	{ }

	/// Adds dib to the cache and returns its key
	void AddDib(const iDib& dib, const iKEY& key)
	{
		iDibCacheKV<iKEY> *pkv = new iDibCacheKV<iKEY>(key,dib);
		AddElement(pkv);
	}

	/// Returns constant pointer to the specified element
	const iDib* GetDibPtr(const iKEY& key) const
	{
		iDibCacheKV<iKEY>* pEl = GetElement(key);
		if (pEl) return &pEl->dib;
		return NULL;
	}

	/// Returns pointer to the specified element
	iDib* GetDibPtr(const iKEY& key)
	{
		iDibCacheKV<iKEY>* pEl = GetElement(key);
		if (pEl) return &pEl->dib;
		return NULL;
	}

	/// Returns ammount of elements in cache
	uint32 GetDibsCount() const
	{
		return GetElementsCount();
	}

};

/// Define iDibCache with integer-based keys
typedef iDibCache<uint32>	iDibCacheU32;

/// Define iDibCache with string-based keys
typedef iDibCache<iStringT>	iDibCacheStr;

#endif //_IOLIB_XENON_DIB_CHACHE_H_
/** @} */ // iolib_xe
