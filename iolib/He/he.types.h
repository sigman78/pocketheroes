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

/** @file
 *	@brief Basic types defination
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_TYPES_H_
#define _IOLIB_HELIUM_TYPES_H_

typedef __int8				sint8;		///< -128 to 127
typedef __int16				sint16;		///< -32,768 to 32,767
typedef __int32				sint32;		///< -2,147,483,648 to 2,147,483,647
typedef __int64				sint64;		///< -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
typedef int					sint;		///< -2,147,483,648 to 2,147,483,647

typedef unsigned __int8		uint8;		///< 255
typedef unsigned __int16	uint16;		///< 65,535
typedef unsigned __int32	uint32;		///< 4,294,967,295
typedef unsigned __int64	uint64;		///< 18,446,744,073,709,551,616
typedef unsigned int		uint;		///< 4,294,967,295

typedef CHAR	iCharA;					///< generic 8bit char (CHAR)
typedef WCHAR	iCharW;					///< 16bit Unicode char (WCHAR)
typedef TCHAR	iCharT;					///< depended on project defines (TCHAR)


#endif //_IOLIB_HELIUM_TYPES_H_
/** @} */ // iolib_he
