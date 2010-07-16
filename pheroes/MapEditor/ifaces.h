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

#ifndef _HMM_EDITOR_COMMON_INTERFACES_H_
#define _HMM_EDITOR_COMMON_INTERFACES_H_

//////////////////////////////////////////////////////////////////////////
inline iPoint ParsePnt( const iStringT& str )
{
	sint32 commaPos = str.Find( _T(',') );
	if ( commaPos < 1 ) return iPoint(0,0);
	return iPoint( 
		str.Left(commaPos).GetInt(), 
		str.Mid( commaPos+1 ).GetInt() 
		);
}

//////////////////////////////////////////////////////////////////////////
interface IStatusBar
{
	virtual ~IStatusBar() {}
	virtual void SetText(const iStringT& text) = NULL;
};

#endif //_HMM_EDITOR_COMMON_INTERFACES_H_