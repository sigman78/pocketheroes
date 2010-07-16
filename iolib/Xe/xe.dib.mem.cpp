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

#include "xe.std.h"
#include "../he/he.memory.compress.h"

bool iMemoryDib::Bit2Byte(const iDynamicBuffer& in_buff, iDynamicBuffer& out_buff)
{
	check(in_buff.GetSize());
	uint32 nsiz = in_buff.GetSize()*8;
	out_buff.ReInit(nsiz);
	for (uint32 xx=0; xx<in_buff.GetSize(); ++xx){
		uint8 data;
		in_buff.Read(data);
		for (uint32 zz=0; zz<8; ++zz){
			uint8 bd = (uint8)(((data >> zz)&1)?255:0);
			out_buff.Write(bd);
		}
	}
	return true;
}

bool iMemoryDib::Byte2Bit(const iDynamicBuffer& in_buff, iDynamicBuffer& out_buff)
{
	check(in_buff.GetSize());
	uint32 nsiz = in_buff.GetSize()/8 + ((in_buff.GetSize()%8==0)?0:1);
	out_buff.ReInit(nsiz);
	for (uint32 xx=0; xx<nsiz; ++xx){
		uint32 cnt=0;
		uint8 data=0;
		uint8 odata=0;
		while (cnt<8 && in_buff.Read(data)) {
			odata |= (data?1<<cnt:0);
			cnt++;
		}
		out_buff.Write(odata);
	}
	out_buff.Seek(0);
	return true;
}

bool iMemoryDib::Dib2BitRLE(const iGrayDib& in_dib, iDynamicBuffer& out_buff)
{
	check(0);
	return true;
}

bool iMemoryDib::Dib2ByteRLE(const iGrayDib& in_dib, iDynamicBuffer& out_buff)
{
	check(0);
	return true;
}

bool iMemoryDib::BitRLE2Dib(const iDynamicBuffer& in_buff, const iSize& siz, iGrayDib &out_dib)
{
	iDynamicBuffer raw_bit_buff,raw_byte_buff;
	iRLE::Decompress(in_buff,raw_bit_buff);
	iMemoryDib::Bit2Byte(raw_bit_buff,raw_byte_buff);
	check( siz.w*siz.h == raw_byte_buff.GetSize() );
	out_dib.ReallocDib(siz);
	CopyBlock8(out_dib.GetBuffer(),(uint8*)raw_byte_buff.GetData(),raw_byte_buff.GetSize());

	return true;
}

bool iMemoryDib::BitRLE2Dib(const iDynamicBuffer& in_buff, const iSize& siz, iDib &out_dib)
{
	iDynamicBuffer raw_bit_buff,raw_byte_buff;
	iRLE::Decompress(in_buff,raw_bit_buff);
	raw_bit_buff.Seek(0);
	iMemoryDib::Bit2Byte(raw_bit_buff,raw_byte_buff);


	check( siz.w*siz.h == raw_byte_buff.GetSize() );
	out_dib.Resize(siz);

	uint8* sptr = (uint8*)raw_byte_buff.GetData();
	iColor* dptr = out_dib.GetBits();

	for (uint32 xx=0; xx<raw_byte_buff.GetSize(); ++xx){
		dptr->SetRGBA(*sptr,*sptr,*sptr,255);
		sptr++;
		dptr++;
	}
	return true;
}

bool iMemoryDib::ByteRLE2Dib(const iDynamicBuffer& in_buff, const iSize& siz, iGrayDib &out_dib)
{
	check(0);
	return true;
}
