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

#ifndef _HMM_LANGUAGE_MANAGER_H_
#define _HMM_LANGUAGE_MANAGER_H_

class iLngMgr : public iTreeNode
{
public:
	struct iBaseLngNode : public iTreeNode {
		enum NodeType {
			GroupNode,
			EntryNode
		};

		iBaseLngNode(NodeType nodeType_) : nodeType(nodeType_){}

		NodeType	nodeType;
	};

	struct iLngEntryGroup : public iBaseLngNode {
		iLngEntryGroup(const iStringT& groupTitle_, const iStringT& groupDesc_) : iBaseLngNode(GroupNode), title(groupTitle_), desc(groupDesc_) {}
		iStringT	title;
		iStringT	desc;
	};

	struct iLngEntry  : public iBaseLngNode {
		iLngEntry(const iStringT& entryId_, const iStringT& origText_) : iBaseLngNode(EntryNode), entryId(entryId_), origText(origText_) {}
		iStringT	entryId;
		iStringT	origText;
	};

public:
	iLngMgr()
	{
	}

	bool ReadOldLngFile(const iStringT& fileName, bool initOriginals)
	{
		iTextFileW langFile;
		if (!langFile.Open(fileName)) return false;
		iTreeNode* pCurGroup = this;

		uint32 cnt = langFile.GetStringsCount();
		for (uint32 xx=0; xx<cnt; ++xx) {
			const iStringT& str = langFile.GetConstString(xx);

			if (str[0] == _T('{')) {
				sint32 bidx = 0;
				sint32 len = 0;
				sint32 eidx = str.Find(_T('}'),bidx+1);
				if (eidx == -1) return false;
				len = eidx-bidx-1;
				check(len);
				iStringT key = len?str.Mid(bidx+1,len):_T("");
				bidx = str.Find(_T('{'),eidx);
				if (bidx == -1) return false;
				eidx = str.Find(_T('}'),bidx);
				if (eidx == -1) return false;
				len = eidx-bidx-1;
				iStringT orig = len?str.Mid(bidx+1,len):_T("");
				bidx = str.Find(_T('{'),eidx);
				if (bidx == -1) return false;
				eidx = str.Find(_T('}'),bidx);
				if (eidx == -1) return false;
				len = eidx-bidx-1;
				iStringT trans = len?str.Mid(bidx+1,len):_T("");
				iBaseLngNode* pNode = new iLngEntry(key, orig);
				if (!orig.Empty()) pCurGroup->Add( pNode );
			}
		}
		return true;
	}

	~iLngMgr()
	{
		Cleanup();
	}

	void Cleanup()
	{
		DeleteAll();
	}
};

#endif //_HMM_LANGUAGE_MANAGER_H_