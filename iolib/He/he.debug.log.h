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
 *	@brief Debug logging routine implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_DEBUG_LOG_H_
#define _IOLIB_HELIUM_DEBUG_LOG_H_

#include "he.string.h"
#include "he.file.h"
#include "he.file.name.h"

//
// for logging you must define IOLIB_DEBUG or IOLIB_LOGGING
//

/// Debug logging level enumeration
enum LogEntryLevel {
	lelError = 0,
	lelWarning,
	lelInformation
};

/// Generic logging class
class iLog 
{
public:

	/// Dumps specified log text string
	static void Dump( const iCharT* str )
	{
		iStringT txt( _T(' '), m_Indent * 4 );
		txt.Addf( _T("%s\r\n"), str );
		
#ifdef FORCE_FILE_LOGGING
		if ( EnsureOpen() )
		{
			m_File->Write(CvtT2A<>(txt.CStr()), txt.Length());
		}
#endif //FORCE_FILE_LOGGING

		OutputDebugString( txt.CStr() );
	}

	/// Dumps specified log text string
	static void Dump( const iStringT &str )
	{
		Dump(str.CStr());
	}

	/// Dumps specified formated text string
	static void VARARG FormDump( const iCharT* str, ... )
	{
		iStringT out;
		va_list args;
		va_start( args, str );
		out.Set( str, args );
		va_end( args );
		Dump( out.CStr() );
	}

	/// Increment indent level
	static void IncIndent()
	{ 
		++m_Indent; 
	}
	
	/// Decrement indent level
	static void DecIndent()
	{ 
		--m_Indent; 
	}

	/// Dump guard helper class
	class Guard 
	{
	public:
		/// Construct safe dump element
		Guard( const iCharT* label )
		: m_Label( label )
		{
			Dump( _T("+") );
			iLog::IncIndent();
		}

		/// Destructor
		~Guard()
		{
			iLog::DecIndent();
			Dump( _T("-") );
		}

		/// Dumps specified log text string
		void Dump( const iCharT* str )
		{
			iLog::Dump( (iStringT(str) + m_Label).CStr() );
		}

	private:
		iStringT	m_Label;
	};
	
private:
	static bool EnsureOpen();
	static void Close();
	static iFile	*m_File;


	static sint32	m_Indent;
	static bool		m_Failed;
};

#if defined( _DEBUG ) || defined( FORCE_LOGGING )

#	define iLOGUARD( label ) iLog::Guard guard_obj__(label)
#	define iLOG( label ) (iLog::Dump( label ) )
#	define iFLOG( label ) iLog::FormDump label

#else

#	define iLOGUARD( label )
#	define iLOG( label )
#	define iFLOG( label )

#endif


#endif //_IOLIB_HELIUM_DEBUG_LOG_H_
/** @} */ // iolib_he
