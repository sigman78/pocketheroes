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

/*****************************************************************************

[Pa]lladium XML library

*****************************************************************************/

#ifndef IOLIB_PA_CONVERT_UTF8_H
#define IOLIB_PA_CONVERT_UTF8_H

#include "he\he.std.h"
#include "he\he.string.h"
#include "pa\pa.except.h"

namespace Charsets {

typedef unsigned long               UCS4;
typedef unsigned short              UCS2;
typedef unsigned char               UTF8;

//consts
const UCS4 byteMask              = 0xBF;
const UCS4 byteMark              = 0x80;
const int  halfShift             = 10;
const UCS4 halfBase              = 0x0010000UL;
const UCS4 halfMask              = 0x3FFUL;
const UCS4 kSurrogateHighStart   = 0xD800UL;
const UCS4 kSurrogateHighEnd     = 0xDBFFUL;
const UCS4 kSurrogateLowStart    = 0xDC00UL;
const UCS4 kSurrogateLowEnd      = 0xDFFFUL;
const UCS4 kMaximumUCS2          = 0x0000FFFFUL;
const UCS4 kMaximumUCS4          = 0x7FFFFFFFUL;
const UCS4 kReplacementCharacter = 0x0000FFFDUL;

struct t_utf8_ucs_map
{
	UTF8   cmask;  //mask of first byte mark
	UTF8   cval;   //first byte mark
	int   shift;
	UCS4  lmask;  //mask of ucs value
	UCS4  lval;   //min value of ucs
};//t_utf8_ucs_map

const UINT sz_utf8_ucs_map=7;

extern const t_utf8_ucs_map g_utf8_ucs_map[sz_utf8_ucs_map];
////////////////////////////////////////////////////////////////////////////////
//single ucs2<-->utf8 convertor

template<class TForwardIterator>
TForwardIterator SingleUcs2ToUtf8Helper(TForwardIterator   source_beg,
									   TForwardIterator   source_end,
									   UTF8*              utf8,
									   /*in-out*/UINT&    sz_utf8,
									   /*in-out*/bool&    stop_on_error);

template<class TForwardIterator>
TForwardIterator SingleUtf8ToUcs2Helper(TForwardIterator   source_beg,
										TForwardIterator   source_end,
										UCS2*              ucs2,
										/*in-out*/UINT&    sz_ucs2,
										/*in-out*/bool&    stop_on_error);

template<class TForwardIterator>
inline TForwardIterator SingleUcs2ToUtf8(TForwardIterator   source_beg,
											TForwardIterator   source_end,
											char*              utf8,
											/*in-out*/UINT&    sz_utf8,
											/*in-out*/bool&    stop_on_error)
{
	return SingleUcs2ToUtf8Helper(source_beg,source_end,reinterpret_cast<UTF8*>(utf8),sz_utf8,stop_on_error);
}

template<class TForwardIterator>
inline TForwardIterator SingleUtf8ToUcs2(TForwardIterator   source_beg,
											TForwardIterator   source_end,
											wchar_t*           ucs2,
											/*in-out*/UINT&    sz_ucs2,
											/*in-out*/bool&    stop_on_error)
{
	return SingleUtf8ToUcs2Helper(source_beg,source_end,reinterpret_cast<UCS2*>(ucs2),
		sz_ucs2,stop_on_error);
}

////////////////////////////////////////////////////////////////////////////////
//char sequential ucs2<-->utf8 converter

inline iStringA CvtToUtf8( const iStringW& src, bool stopOnError = false );
inline iStringW CvtFromUtf8( const iStringA& src, bool stopOnError = false );

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define _DIM_( x ) (sizeof(x) / sizeof(x[0]))
#define _END_( x ) (x + (sizeof(x) / sizeof(x[0])))

////////////////////////////////////////////////////////////////////////////////
//single ucs to utf convertor

template<class TForwardIterator>
TForwardIterator SingleUcs2ToUtf8Helper(TForwardIterator   source_beg,
									   TForwardIterator   source_end,
									   UTF8*              utf8,
									   UINT&              sz_utf8,
									   bool&              stop_on_error)
{
	check(utf8 || sz_utf8==0);

	const UINT sz_ucsValueStep  =sz_utf8_ucs_map;
	const UINT original_sz_utf8 =sz_utf8;

	sz_utf8=0;

	if(source_beg==source_end)
	{
		stop_on_error=false;
		return source_beg;
	}

	UCS4 wc=*source_beg;

	//detect size of utf8 sequential
	for(sz_utf8=1;sz_utf8!=sz_ucsValueStep && wc>g_utf8_ucs_map[sz_utf8].lmask;++sz_utf8);

	if(utf8==NULL)
	{
		stop_on_error=false;
		return source_beg;
	}

	if(sz_utf8>original_sz_utf8)
	{
		stop_on_error=true;
		return source_beg;
	}

	utf8+=sz_utf8;

	for(UINT i=1;i!=sz_utf8;++i)
	{
		*(--utf8)=UTF8( (wc | byteMark) & byteMask );

		wc>>=6;
	}//while

	*(--utf8)=UTF8( wc | g_utf8_ucs_map[sz_utf8].cval );

	stop_on_error=false;

	return ++source_beg;
}//single_ucs2_to_utf8

//------------------------------------------------------------------------
template<class TForwardIterator>
TForwardIterator SingleUtf8ToUcs2Helper(TForwardIterator   source_beg,
									   TForwardIterator   source_end,
									   UCS2*              ucs2,
									   /*in-out*/UINT&    sz_ucs2,
									   /*in-out*/bool&    stop_on_error)
{
	check(ucs2 || sz_ucs2==0);

	UCS4 l;
	UCS4 c0,c;

	const t_utf8_ucs_map* end_utf8_ucs_map =_END_(g_utf8_ucs_map);

	const UINT original_sz_ucs2 =sz_ucs2;

	sz_ucs2=0;

	if(source_beg==source_end) //нам не передали utf8 последовательность
	{
		stop_on_error=false;

		return source_beg;
	}

	stop_on_error=true;

	l=((c0=*source_beg)&0xFF);//запоминаем первый байт utf8 последовательности

	for(const t_utf8_ucs_map* i_map=g_utf8_ucs_map+1;i_map!=end_utf8_ucs_map; ++i_map)
	{
		if(static_cast<UTF8>(c0&i_map->cmask)==i_map->cval) //последовательно сравниваем префикс первого байта
		{
			l&=i_map->lmask; //обрезаем лишние старшие биты

			//полученное число не должно быть меньше (проверяем корректность упаковки)
			//или буфер получателя меньше допустимого
			if(l<i_map->lval || original_sz_ucs2<1)
				break;//return source_beg;

			sz_ucs2=1;

			*ucs2=static_cast<UCS2>(l); //возвращаем сформированное UCS значение

			stop_on_error=false;

			return ++source_beg; //пропускаем последний обработанный UTF символ
		}

		//проверка длины UTF8 последовательности
		if(++source_beg==source_end)
			break;//return source_beg;

		//вытаскиваем префикс байта из последовательности
		if((c=(*source_beg^0x80)&0xFF)&0xC0)
			break;//return source_beg; //нарушен префикс дополнительного байта UTF8-цепочки 

		l=(l<<6)+c;
	}//for t

	return source_beg;
}//single_utf8_to_ucs2

////////////////////////////////////////////////////////////////////////////////
//char sequential ucs2<-->utf8 converter

inline iStringA CvtToUtf8( const iStringW& src, bool stopOnError )
{
	UTF8 utf8_buffer[32];
	UINT sz_utf8_buffer;
	iStringA result;

	const bool origStopOnError = stopOnError;
	stopOnError = false;

	const iCharW* srcStart = src.CStr();
	const iCharW* srcEnd = src.CStr() + src.Length();

	while( srcStart != srcEnd ) {
		srcStart = SingleUcs2ToUtf8Helper( srcStart, srcEnd, 
			utf8_buffer, sz_utf8_buffer = _DIM_(utf8_buffer),
			stopOnError = origStopOnError );

		if ( stopOnError ) break;

		//for( int iBuf = 0; iBuf != sz_utf8_buffer; ++iBuf ) 	result += iCharA( utf8_buffer[iBuf] );
		result.Append( (iCharA*)utf8_buffer, sz_utf8_buffer );
		
	}

	return result;
}

inline iStringW CvtFromUtf8( const iStringA& src, bool stopOnError )
{
	UCS2 ucs2_buffer[4];
	UINT sz_ucs2_buffer;
	iStringW result;

	const bool origStopOnError = stopOnError;
	stopOnError = false;

	const iCharA* srcStart = src.CStr();
	const iCharA* srcEnd = src.CStr() + src.Length();

	while( srcStart != srcEnd ) {
		srcStart = SingleUtf8ToUcs2Helper( srcStart, srcEnd,
			ucs2_buffer, sz_ucs2_buffer = _DIM_( ucs2_buffer ),
			stopOnError = origStopOnError );
		
		if ( stopOnError ) break;

		//for( int iBuf = 0; iBuf != sz_ucs2_buffer; ++iBuf ) result += iCharW( ucs2_buffer[iBuf] );
		result.Append( (iCharW*)ucs2_buffer, sz_ucs2_buffer );
	}

	return result;
}


} // namespace Charsets

#undef _END_
#undef _DIM_

#endif //IOLIB_PA_CONVERT_UTF8_H
