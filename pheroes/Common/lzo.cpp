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
#include "lzo/minilzo.h"
#include "lzo.h"

namespace LZO {

const uint16 LZO_BLOCK_HDR = 'L' | ('Z'<<8);
const uint32 LZO_MAX_BUF_LEN = 4 * 1024 * 1024;

bool Init()
{
	return (lzo_init() == LZO_E_OK);
}


#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]
static HEAP_ALLOC(wrkmem,LZO1X_1_MEM_COMPRESS);

uint32 Compress(const unsigned char* rawBuff, uint32 rawBuffLen, iDynamicBuffer& lzoBuff)
{
	check(rawBuffLen > 0 && rawBuffLen <= LZO_MAX_BUF_LEN);
	unsigned long out_len = (rawBuffLen + rawBuffLen / 16 + 64 + 3);
	unsigned char* buff = new unsigned char[out_len];

	// compress from `in' to `out' with LZO1X-1
    int res = lzo1x_1_compress(rawBuff,rawBuffLen,buff,&out_len,wrkmem);
    if (res != LZO_E_OK) {
        // this should NEVER happen
		delete[] buff;
		check(0);
        return 0;
    }

	lzoBuff.Write(LZO_BLOCK_HDR);
	lzoBuff.Write(rawBuffLen);
	lzoBuff.Write(buff, out_len);
	delete[] buff;
	return lzoBuff.GetSize();
}


uint32 Decompress(const unsigned char* lzoBuff, uint32 lzoBuffLen, iDynamicBuffer& rawBuff)
{
	uint16 hdr;
	memcpy(&hdr, lzoBuff, sizeof(uint16));
	lzoBuffLen -=  sizeof(uint16);
	lzoBuff += sizeof(uint16);
	if (hdr != LZO_BLOCK_HDR) {
		check(0);
		return 0;
	}

	unsigned long raw_buf_len;
	memcpy(&raw_buf_len, lzoBuff, sizeof(unsigned long));
	lzoBuffLen -=  sizeof(uint32);
	lzoBuff += sizeof(uint32);
	if (raw_buf_len <= 0 || raw_buf_len > LZO_MAX_BUF_LEN) {
		check(0);
		return 0;
	}

	rawBuff.ReInit(raw_buf_len);
	rawBuff.IncSize( raw_buf_len );
    int res = lzo1x_decompress(lzoBuff,lzoBuffLen,(unsigned char*)rawBuff.GetData(),&raw_buf_len,NULL);
	if (res != LZO_E_OK) {
		check(0);
		return 0;
	}
	return raw_buf_len;
}


};
