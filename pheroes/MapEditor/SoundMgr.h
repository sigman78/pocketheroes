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

#ifndef _HMM_EDITOR_SOUND_MANAGER_H_
#define _HMM_EDITOR_SOUND_MANAGER_H_

class iSoundMgr
{
public:
	struct iSoundItem {
		iSoundItem(const iStringT& _fileName, const iStringT& _soundId, const iStringT& _constId)
		: fileName(_fileName), soundId(_soundId), constId(_constId) {}
		iStringT	fileName;
		iStringT	soundId;
		iStringT	constId;
	};
public:
	iSoundMgr();
	~iSoundMgr();
	bool Init(const iStringT& fileName);
	bool AddSoundItem(const iStringT& fileName, const iStringT& soundId, const iStringT& constId);
	uint32 Count() const { return m_items.GetSize(); }
	sint32 FindItem(const iStringT& soundId) const;
	const iSoundItem& Item(uint32 idx) const { return m_items[idx]; }

private:
	iStringT					m_resPath;
	iSimpleArray<iSoundItem>	m_items;

};

#endif //_HMM_EDITOR_SOUND_MANAGER_H_
