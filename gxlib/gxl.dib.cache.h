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

#ifndef _GXLIB_FIB_CACHE_H_
#define _GXLIB_FIB_CACHE_H_

/*
 *	
 */
struct DIB_CACHE_REF {
	uint32		uid;
	LPCWSTR		fname;
	uint32		type;
};

/*
 *	
 */
namespace iDibHlp{
	inline bool CombineWithAlpha(iDib& dst, const iDib& src)
	{
		if (src.GetType() != iDib::RGB) return false;
		iSize img_siz(src.GetWidth(),src.GetHeight()/2);
		uint32 pix_siz = img_siz.w*img_siz.h;
		dst.Init(img_siz,iDib::RGBA);
		const iDib::pixel* src_rgb = src.GetPtr();
		const iDib::pixel* src_a = src.GetPtr() + pix_siz;
		iDib::pixel* dst_ptr = dst.GetPtr();
		for (uint32 xx=0; xx<pix_siz; ++xx){
			*dst_ptr = (*src_rgb & 0xF<<12) | (*src_rgb & 0xF<<7)<<1 | (*src_rgb & 0xF<<1)<<3 | (*src_a & 0xF800) >> 12;
			src_a++;
			src_rgb++;
			dst_ptr++;
		}
		return true;
	}

	inline bool CreateAsCK(iDib& dst, const iDib& src, iDib::pixel clr)
	{
		if (src.GetType() != iDib::RGB) return false;
		iSize img_siz(src.GetWidth(),src.GetHeight());
		uint32 pix_siz = img_siz.w*img_siz.h;
		dst.Init(img_siz,iDib::RGBCK);
		const iDib::pixel* src_rgb = src.GetPtr();
		iDib::pixel* dst_ptr = dst.GetPtr();
		for (uint32 xx=0; xx<pix_siz; ++xx){
			*dst_ptr = (*src_rgb & 0x1F<<11) | (*src_rgb & 0x1F<<6) | (*src_rgb & 0x1F)<<1 | (*src_rgb != clr);
			src_rgb++;
			dst_ptr++;
		}
		return true;
	}
}
/*
 *	
 */
struct iDibCaheEl {
	iDibCaheEl()
	: pDib(NULL)
	, refCnt(0)
	{}

	iDib*	pDib;
	uint32	refCnt;
};

/*
 *	
 */
class iDibCache
{
public:
	iDibCache()
	{
	}

	~iDibCache()
	{
		Destroy();
	}

	bool Init(DIB_CACHE_REF* pRef, uint32 refcnt, const iStringT& rootPath, HWND hwnd)
	{
		m_pRefs = pRef;
		m_RefsCount = refcnt;
		m_rootPath = rootPath;
		m_dibCache.SetSize(refcnt);
		for (uint32 xx=0; xx<refcnt; ++xx) {
			m_dibCache[xx] = iDibCaheEl();
		}
		if (!m_DibReader.Init()) return false;
		return true;
	}

	void Destroy()
	{
		Cleanup();
		for (uint32 xx=0; xx<m_dibCache.GetSize(); ++xx){
			uint32 qq = m_dibCache[xx].refCnt;
			check(m_dibCache[xx].refCnt == 0);
		}
	}

	const iDib*	Acquire(uint32 uid)
	{
		check(uid < m_dibCache.GetSize());
		uint32 vvv = m_dibCache[uid].refCnt;
		if (!m_dibCache[uid].refCnt){
			check (m_dibCache[uid].pDib == NULL);
			m_dibCache[uid].pDib = new iDib();
			if (m_pRefs[uid].type == 0){
				if (!m_DibReader.FromFile(*m_dibCache[uid].pDib,m_rootPath+m_pRefs[uid].fname)) {
					check(0);
				} 
			} else if (m_pRefs[uid].type == 1) {
				iDib tdib;
				if (!m_DibReader.FromFile(tdib,m_rootPath+m_pRefs[uid].fname) || !iDibHlp::CombineWithAlpha(*m_dibCache[uid].pDib,tdib)) {
					check(0);
				}
			} else if (m_pRefs[uid].type == 2) {
				iDib tdib;
				if (!m_DibReader.FromFile(tdib,m_rootPath+m_pRefs[uid].fname) || !iDibHlp::CreateAsCK(*m_dibCache[uid].pDib,tdib,0)) {
					check(0);
				}
			} 
		}
		m_dibCache[uid].refCnt++;
		return m_dibCache[uid].pDib;
	}

	uint32 Find(const iDib* pDib)
	{
		for (uint32 xx=0; xx<m_dibCache.GetSize(); ++xx){
			if (m_dibCache[xx].pDib == pDib) return xx;
		}
		return -1;
	}

	uint32 Release(uint32 uid)
	{
		check(uid < m_dibCache.GetSize());
		uint32 aa= m_dibCache[uid].refCnt;
		check(m_dibCache[uid].refCnt);
		return --m_dibCache[uid].refCnt;
	}

	uint32 Release(const iDib* pDib)
	{
		check(pDib);
		uint32 idx = Find(pDib);
		check(idx!=-1);
		return Release(idx);
	}

	void Cleanup()
	{
		for (uint32 xx=0; xx<m_dibCache.GetSize(); ++xx){
			if (m_dibCache[xx].refCnt == 0) SAFE_DELETE(m_dibCache[xx].pDib);
		}
	}

	iDibReader& DibReader()
	{ return m_DibReader; }

private:
	iSimpleArray<iDibCaheEl>	m_dibCache;
	iStringT					m_rootPath;
	iDibReader					m_DibReader;

	DIB_CACHE_REF*				m_pRefs;
	uint32						m_RefsCount;
};

#endif //_GXLIB_FIB_CACHE_H_
