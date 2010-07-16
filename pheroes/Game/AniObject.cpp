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

#include "stdafx.h"

/*
 *	
 */
iAniObj::iAniObj(IAniCB* pcb, fix32 t, const iPoint pos)
: m_pCB(pcb), m_Duration(t), m_Timer(fix32::zero), m_Pos(pos), m_type(CellAni)
{}

iAniObj::iAniObj(IAniCB* pcb, fix32 t)
: m_pCB(pcb), m_Duration(t), m_Timer(fix32::zero), m_Pos(cInvalidPoint), m_type(FullScreenAni)
{}

void iAniObj::Process(fix32 t)
{
	m_Timer += t;
	if (m_Timer >= m_Duration) {
		m_pCB->AniDone();
	}
}


/*
 *	
 */
iPoint cellOffset[8] = {
	iPoint(0,-20), iPoint(20,-9), iPoint(40,0), iPoint(20,9), 
	iPoint(0,20), iPoint(-20,9), iPoint(-40,0), iPoint(-20,-9)
};

//const float DEF_ANI_SPEED = 3.6f;
//const float HERO_SPEED_VAL[5] = { 2.5f, 3.0f, 3.5f, 4.5f, 5.5f };
const fix32 DEF_ANI_SPEED ( fix32::fp( 0x38000 ) ); // 3.5
const fix32 HERO_SPEED_VAL[5] = { 
	fix32::fp( 0x28000 ), fix32::fp( 0x30000 ), fix32::fp( 0x38000 ), fix32::fp( 0x48000 ), fix32::fp( 0x58000 ) 
	};

iAniObj_MovHero::iAniObj_MovHero(IAniCB* pcb, iHero* pHero)
: m_aniSpeed(HERO_SPEED_VAL[gSettings.GetEntryValue(CET_HEROSPEED)]), iAniObj(pcb,fix32(pHero->Path().GetStepLength() / fix32(HERO_SPEED_VAL[gSettings.GetEntryValue(CET_HEROSPEED)])), pHero->Pos()), m_pHero(pHero), m_tgtPos(pHero->Path().GetStepPos())
{}

iPoint iAniObj_MovHero::PtOffset() const
{ 
	return iPoint(
		(fix32(cellOffset[m_pHero->Angle()].x) * (m_Timer/m_Duration)).ceil(), 
		(fix32(cellOffset[m_pHero->Angle()].y) * (m_Timer/m_Duration)).ceil()); 
}

void iAniObj_MovHero::GetAniInfo(SpriteId& sid, iPoint& offset)
{
	sid = (m_Timer * fix32(15.0f) * m_aniSpeed).ceil() % 8 + 1;
	offset = PtOffset(); 
}

/*
 *	
 */
iAniObj_FadeOut::iAniObj_FadeOut(IAniCB* pcb, SpriteId sid, const iPoint pos)
: iAniObj(pcb,fix32(0.8f)/DEF_ANI_SPEED, pos), m_Sid(sid)
{}

void iAniObj_FadeOut::GetAniInfo(SpriteId& sid, uint8 &a)
{
	sid = m_Sid;
	a = iCLAMP<uint8>(0,63,(uint8)((m_Duration-m_Timer) * fix32(64.0f) / m_Duration).ceil());
}

/*
 *	
 */
iAniObj_Teleport::iAniObj_Teleport(IAniCB* pcb)
: iAniObj(pcb,fix32(0.5f)), m_buff(gApp.Surface())
{
}

void iAniObj_Teleport::Compose(iDib& surf)
{
	uint8 a = iCLAMP<uint8>(0,255,(uint8)((m_Duration-m_Timer) * fix32(255.0f) / m_Duration).ceil());
	m_buff.CopyToDibXY(surf, iPoint(), a);
	//surf.FillRect(surf.GetSize(), cColor_Black, a);
}

