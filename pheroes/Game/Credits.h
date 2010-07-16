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

#ifndef _HMM_GAME_CREDITS_COMPOSER_H_
#define _HMM_GAME_CREDITS_COMPOSER_H_

// light info
struct LIGHT {
	sint32 x;		// x displacement
	sint32 y;		// y displacement
};

// cluster info
struct CLUSTER {
	sint32 x;		// current x position
	sint32 y;		// current y position
	sint32 cx;		// central x location
	sint32 cy;		// central y location
	sint32 width;	// width of ellipse
	sint32 height;	// height of ellipse
	LIGHT r;		// red light info
	LIGHT g;		// green light info
	LIGHT b;		// blue light info
};

// fast light info
struct FASTLIGHT {
	sint32 x;		// x displacement
	sint32 y;		// y displacement
};


// fast cluster info
struct FASTCLUSTER {
	sint32 x;		// current x position
	sint32 y;		// current y position
	FASTLIGHT r;	// red light info
	FASTLIGHT g;	// green light info
	FASTLIGHT b;	// blue light info
};

//////////////////////////////////////////////////////////////////////////
class iCreditsComposer 
{
public:
	iCreditsComposer() : m_bShowCredits(false) {}
	~iCreditsComposer() { Cleanup(); }

	void Init();
	void Cleanup();
	void Compose(iDib& surface, const iPoint& pos);

	void StartCredits();
	void StopCredits();
	inline bool IsCreaditsStarted() const { return m_bShowCredits; }
	inline bool IsCreaditsEnd() const { return m_bEnd; }

private:
	void InitLight();
	void InitBump();
	void InitMotionBlur();

private:
	iDib			m_back;
	uint32			m_pos;
	uint16*			m_logo;
	iSize			m_logoSize;


	bool			m_bEnd;
	sint32			m_scrPos;
	bool			m_bShowCredits;
	iMapT<uint8>	m_image;
	uint8			bump_buffer[320*240*2];
	uint8			light[256*256];
	uint8			combine[256][256];
	uint8			clip[256*8];
	uint8			clip256[256][256];
	CLUSTER			cluster[1];
	iSize			m_siz;
	float			t;
	float			dt;
};


#endif //_HMM_GAME_CREDITS_VIEW_H_

