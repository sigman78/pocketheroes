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

#include "he.std.h"
#include "he.memory.compress.h"

bool iRLE::Compress(const iDynamicBuffer& src, iDynamicBuffer& dst)
{
	uint32	len = src.GetSize();
	uint8	*src_buff = (uint8*)src.GetData();
    uint8	byte1;
    uint8	byte2;
    uint8	frame_size;
    uint8	array[129];

    while (len){
		byte1 = *src_buff;
		src_buff++;
		len--;
		frame_size = 1;

		if (len){
			byte2 = *src_buff;
			src_buff++;
			len--;
            frame_size = 2;
            do {
				if (byte1 == byte2) { 
					while (len && (byte1 == byte2) && (frame_size < 129)) { 
						byte2 = *src_buff;
						src_buff++;
						len--;
                        frame_size++;
					}

					if (byte1 == byte2) { 
						dst.Write((uint8)(frame_size+126));
						dst.Write(byte1);
						if (len) {
							byte1=*src_buff;
							src_buff++;
							len--;
							frame_size = 1;
						} else {
							frame_size = 0;
						}
					} else { 
						dst.Write((uint8)(frame_size+125));
						dst.Write(byte1);
						byte1 = byte2;
						frame_size = 1;
					}

					if (len) { 
						byte2 = *src_buff;
						src_buff++;
						len--;
						frame_size = 2;
					}
				} else {
					//Prepare the array of comparisons
					//where will be stored all the identical bytes */
				    *array = byte1;
				    array[1] = byte2;
					while (len && (array[frame_size-2] != array[frame_size-1]) && (frame_size  < 128)) { 
						array[frame_size] = *src_buff;
						src_buff++;
						len--;
						frame_size++;
					}

					/* Do we meet a sequence of all different bytes followed by identical byte? */
                    if (array[frame_size-2] == array[frame_size-1]) { 
						/* Yes, then don't count the two last bytes */
						dst.Write((uint8)(frame_size-3));
						for (sint32 i=0; i<frame_size-2; i++) {
							dst.Write((uint8)(array[i]));
						}

						byte1 = array[frame_size-2];
						byte2 = byte1;
						frame_size = 2;
					} else {
						dst.Write((uint8)(frame_size-1));
						for (sint32 i=0; i<frame_size; i++) {
							dst.Write((uint8)(array[i]));
					    }
						if (!len) {
							frame_size = 0;
						} else {
							byte1 = *src_buff;
							src_buff++;
							len--;
							if (!len) {
								frame_size = 1;
							} else {
								byte2 = *src_buff;
								src_buff++;
								len--;
								frame_size = 2;
							}
						}
					}
				}
			} while (len || (frame_size >= 2));

		    if (frame_size == 1){
				dst.Write((uint8)0);
				dst.Write(byte1);
			}
	    }
	}
    return true;
}

bool iRLE::Decompress(const iDynamicBuffer& src, iDynamicBuffer& dst)
{
	uint32	len = src.GetSize();
	uint8*	src_buff = (uint8*)src.GetData();
   	uint8	header;

    while (len){
		header = *src_buff;
		src_buff++;
		len--;

		if (!(header & 128)){
			// There are header+1 different bytes.
			for (uint32 i=0; i<=header; i++) {
				dst.Write(src_buff,1);
				src_buff++;
				len--;
			}
		} else {
			const unsigned int n = (header & 127) + 2;
			for (uint32 i=0; i<n; i++) {
				dst.Write(src_buff,1);
			}
			src_buff++;
			len--;
		}
	}
	return true;
}
