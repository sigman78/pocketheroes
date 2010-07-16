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
 *	@brief Console helper functions implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_CONSOLE_HELPERS_H_
#define _IOLIB_HELIUM_CONSOLE_HELPERS_H_

/// Console helper functions namespace
namespace iConsole
{
	/// Returns dimensions of current console
	inline iSize GetWindowSize()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if(GetConsoleScreenBufferInfo (	GetStdHandle (STD_OUTPUT_HANDLE), &csbi)){
			return iSize(csbi.dwSize.X,csbi.dwSize.Y);
		} else {
			return iSize();
		}
	}

	/// Returns cursor position in current console
	inline iPoint GetCursorPos()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if(GetConsoleScreenBufferInfo (	GetStdHandle (STD_OUTPUT_HANDLE), &csbi)){
			return iPoint(csbi.dwCursorPosition.X,csbi.dwCursorPosition.Y);
		} else {
			return iPoint();
		}
	}

}//namespace iConsole

#endif //_IOLIB_HELIUM_CONSOLE_HELPERS_H_
/** @} */ // iolib_he
