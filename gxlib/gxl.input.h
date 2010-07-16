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

#ifndef _GXLIB_INPUT_H_
#define _GXLIB_INPUT_H_

#include "gxl.window.h"

enum iKbdKey {
	KEY_INVALID = -1,
	KEY_ENTER = 0,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_VKA,
	KEY_VKB,
	KEY_VKC,
	KEY_VKD,
	KEY_VKE,
	KEY_VKF,
	KEY_REC,
	KEY_TALK,
	KEY_END,
	KEY_BACK,
	KEY_HOME,
	KEY_SOFT1,
	KEY_SOFT2,
	KEY_COUNT
};

class iInput : public iInputMsgHnd
{
public:
	struct iEntry {
		enum Type {
			Unknown = 0,
			MouseMove,
			MouseDown,
			MouseUp,
			KeyDown,
			KeyUp
		};

		iEntry() : taskType(Unknown), key(0) {}
		iEntry(Type t, sint16 x, sint16 y) : taskType(t), px(x), py(y) {}
		iEntry(Type t, iKbdKey k) : taskType(t), key(k) {}

		Type	taskType;
		union {
			struct {
				sint16 px,py;
			};
			sint32	key;
		};
	};

private:
#ifdef OS_WINCE
	GXKeyList			m_KeyList;
#endif //OS_WINCE
	bool				m_KeysState[KEY_COUNT];
	uint32				m_Flags;
	sint16				m_mo;
	iSize				m_dimScreen;
	iCircBuff<iEntry>	m_Entries;
	bool				m_bInited;

	struct KeyEntry {
		uint32 vk;	// virtual key code
		uint32 kc;  // key code
	};

	KeyEntry			m_Translate[19];
	uint32				m_TranslateSize;
public:

	iInput();
	~iInput();
	bool Init(const iSize& scr_siz, uint32 flags);
	void Destroy();

	void SetOrientation(bool bLandscape, bool bLeftHander);
	inline uint32 EntryCount() const { return m_Entries.Count(); }
	inline iEntry& Get() { return m_Entries.Get(); }
	inline void Reset() { m_Entries.Reset(); }

	iKbdKey KeyCode2Key(sint16 key_code);
	
	void CvtScrCoor(sint16& px, sint16& py);

	// iInputMsgHnd 
	void msg_OnKeyDown(sint16 key);
	void msg_OnKeyUp(sint16 key);
	void msg_OnMouseMove(sint16 px, sint16 py);
	void msg_OnMouseDown(sint16 px, sint16 py);
	void msg_OnMouseUp(sint16 px, sint16 py);

	void ClearKeyState();
};

#endif //_GXLIB_INPUT_H_

