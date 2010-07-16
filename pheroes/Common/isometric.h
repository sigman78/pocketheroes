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

#ifndef _ISOMETRIC_PROJECTION_HANDLING_H_
#define _ISOMETRIC_PROJECTION_HANDLING_H_

//
// cf - Cell factor of isometric cell (cf = CellWidth/ATOM_WIDTH)
//
const uint32 ATOM_WIDTH = 8;
const uint32 ATOM_HEIGHT = 3;

class iIsoMetric
{
public:

	iIsoMetric(uint32 cf)
	: m_CellFactor(cf)
	{}

	static inline uint32 GetCellWidth(uint32 cf)
	{
		return cf*ATOM_WIDTH;
	}

	static inline uint32 GetCellHeight(uint32 cf)
	{
		return (GetCellWidth(cf)-1)/2;
	}

	inline uint32 GetCellWidth()
	{
		return GetCellWidth(m_CellFactor);
	}

	inline uint32 GetCellHeight()
	{
		return GetCellHeight(m_CellFactor);
	}

	inline iSize GetCellSize()
	{
		return iSize(GetCellWidth(),GetCellHeight());
	}

	inline uint32 GetCellStepX()
	{
		return GetCellWidth()/2;
	}

	inline uint32 GetCellStepY()
	{
		return (GetCellHeight()+1)/2;
	}

	inline iPoint GetScreenOffset(const iPoint& cellOffset)
	{
		return iPoint((cellOffset.x-cellOffset.y)*GetCellStepX(), (cellOffset.x+cellOffset.y)*GetCellStepY());
	}

	inline iPoint Map2Screen(const iPoint &pos)
	{
		sint32 n = (sint32)m_CellFactor*2;
		return iPoint(
			(-2*n*pos.y) + (2*n*pos.x),
			(n*pos.y) + (n*pos.x));
	}

	inline iPoint Screen2Map(const iPoint &pos)
	{
		sint32 n = 2 * m_CellFactor;
		sint32 coef = 4 * n;

		sint32 n2 = n*2;
		sint32 n4 = n*4;

		sint32 px = coef * pos.x;
		sint32 py = coef * ( pos.y - (GetCellHeight()/2) );

		sint32 px2 = px + ( (px/2) <= py ? n4 : 0 );

		sint32 nx = (px/n4 + py/n2);
		sint32 ny = (-px2/n4 + py/n2);

		nx += (nx>0) ? n2 : -n2;
		ny += (ny>0) ? n2 : -n2;

		return iPoint(nx / coef, ny  / coef);
	}

private:
	uint32 m_CellFactor;
};


#endif //_ISOMETRIC_PROJECTION_HANDLING_H_

