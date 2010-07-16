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

#ifndef _HMM_GAME_ANIMATION_OBJECT_H_
#define _HMM_GAME_ANIMATION_OBJECT_H_

/*
 *	
 */
struct IAniCB
{
	virtual void AniDone() =0;
};

/*
 *	
 */
class iAniObj : public TypeAware
{
public:
	enum AniType {
		CellAni,
		FullScreenAni
	};

	IMPL_TYPEAWARE( iAniObj );

	iAniObj( IAniCB* pcb, fix32 t, const iPoint pos);
	iAniObj( IAniCB* pcb, fix32 t);

	virtual ~iAniObj()
	{
		int a= 0;
	}

	void Process(fix32 t);

	inline iPoint Pos() const { return m_Pos; }
	inline AniType Type() const { return m_type; }

protected:
	AniType	m_type;
	iPoint	m_Pos;
	IAniCB*	m_pCB;
	fix32	m_Timer;
	fix32	m_Duration;
};

/*
 *	
 */
class iAniObj_MovHero : public iAniObj
{
public:
	IMPL_TYPEAWARE( iAniObj_MovHero );
	iAniObj_MovHero(IAniCB* pcb, iHero* pHero);
	iPoint PtOffset() const;
	void GetAniInfo(SpriteId& sid, iPoint& offset);
	inline iHero* Hero() { return m_pHero; }
	inline const iPoint& TargetPos() const { return m_tgtPos; }

private:
	iPoint	m_tgtPos;
	iHero*	m_pHero;
	fix32	m_aniSpeed;
};

/*
 *	
 */
class iAniObj_FadeOut : public iAniObj
{
public:
	IMPL_TYPEAWARE( iAniObj_FadeOut );
	iAniObj_FadeOut(IAniCB* pcb, SpriteId sid, const iPoint pos);
	void GetAniInfo(SpriteId& sid, uint8 &a);

private:
	SpriteId	m_Sid;
};

/*
 *	
 */
class iAniObj_Teleport : public iAniObj
{
public:
	IMPL_TYPEAWARE( iAniObj_Teleport );
	iAniObj_Teleport(IAniCB* pcb);
	void Compose(iDib& surf);

private:
	iDib	m_buff;
};


#endif //_HMM_GAME_ANIMATION_OBJECT_H_

