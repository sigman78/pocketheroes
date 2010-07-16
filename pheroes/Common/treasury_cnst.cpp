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
#include "serialize.h"

void iTrVarCtr::AddVariant(uint32 probability)
{
	m_items.Add(Item(probability));
}

void iTrVarCtr::Serialize(iDynamicBuffer& dbuff) const
{
	uint16 quant = (uint16)m_items.GetSize();
	dbuff.Write(quant);
	for (uint16 xx=0; xx<quant; ++xx) {
		dbuff.Write((uint8)m_items[xx].probability);
		::Serialize(dbuff, m_items[xx].guards);
		::Serialize(dbuff, m_items[xx].rewards);
	}
}

void iTrVarCtr::Unserialize(iDynamicBuffer& dbuff)
{
	m_items.RemoveAll();
	uint16 quant;
	dbuff.Read(quant);
	while (quant--) {
		uint8 prob;
		dbuff.Read(prob);
		m_items.Add(Item(prob));
		::Unserialize(dbuff, m_items.GetLast().guards);
		::Unserialize(dbuff, m_items.GetLast().rewards);
	}
}

