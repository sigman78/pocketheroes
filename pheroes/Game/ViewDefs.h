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

#ifndef _HMM_GAME_VIEWPORT_DEFINES_H_
#define _HMM_GAME_VIEWPORT_DEFINES_H_

/*
 *	
 */
const uint32 VPUID_ENG				= 65536;

const uint32 VPUID_BASE				= VPUID_ENG + 1;
const uint32 VPUID_MAINVIEW			= VPUID_ENG + 10;
const uint32 VPUID_MINIMAPVIEW		= VPUID_ENG + 11;
const uint32 VPUID_CASTLEVIEW		= VPUID_ENG + 12;
const uint32 VPUID_HEROVIEW			= VPUID_ENG + 13;
const uint32 VPUID_MAINTOOLBARVIEW	= VPUID_ENG + 20;
const uint32 VPUID_MMAPTOOLBARVIEW	= VPUID_ENG + 21;
const uint32 VPUID_INFOBARVIEW		= VPUID_ENG + 30;
const uint32 VPUID_CTLVIEWTOOLBAR	= VPUID_ENG + 40;
const uint32 VPUID_CTLVIEWTITLELBAR	= VPUID_ENG + 41;
const uint32 VPUID_HEROVIEWTOOLBAR	= VPUID_ENG + 50;
const uint32 VPUID_HEROVIEWTITLELBAR= VPUID_ENG + 51;

const uint32 VPUID_MODE_SWITCH		= VPUID_ENG + 100;
const uint32 VPUID_BTN_MINIMAP		= VPUID_ENG + 101;
const uint32 VPUID_BTN_STATE		= VPUID_ENG + 102;
const uint32 VPUID_BTN_NTURN		= VPUID_ENG + 103;


const uint32 VPUID_BTN_OBJICN		= VPUID_ENG + 200;
const uint32 VPUID_PRG_OBJACT		= VPUID_ENG + 201;
const uint32 VPUID_BTN_OBJPROPS		= VPUID_ENG + 203;
const uint32 VPUID_BTN_HMOVE		= VPUID_ENG + 204;
const uint32 VPUID_BTN_LIST			= VPUID_ENG + 205;
const uint32 VPUID_BTN_OPENCITY		= VPUID_ENG + 206;

const uint32 VPUID_BTN_ZOOMIN		= VPUID_ENG + 301;
const uint32 VPUID_BTN_ZOOMOUT		= VPUID_ENG + 302;
const uint32 VPUID_BTN_CLOSEMMAP	= VPUID_ENG + 303;
const uint32 VPUID_BTN_ENDBATTLE	= VPUID_ENG + 304;
const uint32 VPUID_BTN_CLOSECITY	= VPUID_ENG + 305;
const uint32 VPUID_BTN_CLOSEHERO	= VPUID_ENG + 306;

#endif //_HMM_GAME_VIEWPORT_DEFINES_H_


