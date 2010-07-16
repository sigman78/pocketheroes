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
 *	@brief Handling of strings that represents either a full or partial path
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_FILENAME_H_
#define _IOLIB_HELIUM_FILENAME_H_

#include "he.std.h"
#include "he.string.h"
#include "he.container.simple.array.h"

const iCharT PATH_SLASH = _T('\\');
const iCharT PATH_DOT = _T('.');
const iCharT PATH_COLLON = _T(':');


/// Filename class
class iFileName
{
public:
	/// Device class
	class iDevice
	{
	public:
		/// Device type enumeration
		enum DEVTYPE {
			LOCAL_DISK,
			REMOTE_DISK
		};

		/// Constructs device with specified name
		iDevice(const iStringT &str)
		{
			SetDevice(str);
		}

		/// Copy constructor
		iDevice(const iDevice &dev)
		{
			SetDevice(dev);
		}

		/// Returns device name
		iStringT GetDevString() const
		{
			return m_Device;
		}

		/// Returns device type
		DEVTYPE GetDevType() const
		{
			return m_DevType;
		}

		/// Sets device name
		void SetDevice(const iStringT &str)
		{
			ParseDevString(str);
		}

		/// Sets device props according to given one
		void SetDevice(const iDevice &dev)
		{
			m_Device = dev.GetDevString();
			m_DevType = dev.GetDevType();
		}


	private:

		/// Parses device string
		void ParseDevString(const iStringT &str)
		{
			if (str[0] == PATH_SLASH && str[1] == PATH_SLASH) {
			} else if (str[1] == PATH_COLLON && str[2] == PATH_SLASH) {
			} else {
				return;
			}
			m_Device = str;
			m_DevType = LOCAL_DISK;
		}

		iStringT	m_Device;
		DEVTYPE		m_DevType;
	};

	/// Path class
	class iPath
	{
	public:
		/// Constructs path with given name
		iPath(const iStringT &str)
		{
			SetPath(str);
		}

		/// Copy constructor
		iPath(const iPath &path)
		{
			SetPath(path);
		}

		/// Gets level of the path
		uint32 GetTotLevels() const
		{
			return m_PathLevels.GetSize();
		}

		/// Sets new path
		void SetPath(const iStringT &str)
		{
			ParsePathString(str);
		}

		/// Sets new path
		void SetPath(const iPath &path)
		{
			m_PathLevels.RemoveAll();
			for (uint32 xx=0; xx < path.GetTotLevels(); xx++)
			{
				m_PathLevels.Add(GetLevel(xx));
			}
		}

		/// Gets current path level
		iStringT GetLevel(uint32 idx) const
		{
			if (idx >= m_PathLevels.GetSize()) return iStringT();
			else return m_PathLevels[idx];
		}

		/// Gets path string
		iStringT GetPathString() const
		{
			iStringT result;
			for (uint32 xx=0; xx<m_PathLevels.GetSize(); xx++)
			{
				iCharT sl[] = {PATH_SLASH,'0'};
				result+=m_PathLevels[xx] + sl;
			}
			return result;
		}

	private:
		
		/// Parses path string
		void ParsePathString(const iStringT &str)
		{
			iCharT trimSpec[] = { _T(' '), 0 };
			iStringT wks(str);
			wks.Trim(trimSpec);

			if(wks.Empty()) return;

			iCharT sl[]={PATH_SLASH,0};

			if (wks.Left(1) == sl)	wks = wks.Right(wks.Length() - 1);
			if (wks.Right(1) != sl)	wks += sl;

			sint32 pos=0;
			sint32 npos=0;

			while ( pos < (sint32)wks.Length() && wks.Find(PATH_SLASH, pos) != iStringT::NOT_FOUND )
			{
				if ((pos - npos) > 0 )
				{
					m_PathLevels.Add(wks.Mid(npos,pos-npos));
				}
				npos = pos+1;
			}

		}

		iSimpleArray<iStringT> m_PathLevels;
	};

	///	FileName and Extension class
	class iNameExt
	{
	public:

		/// Constructs object with given name
		iNameExt(const iStringT &str)
		{
			SetNameExt(str);
		}

		/// Constructs object with given name and extension
		iNameExt(const iStringT &nm, const iStringT &ext)
		{
			SetName(nm);
			SetExt(ext);
		}

		/// Copy constructor
		iNameExt(const iNameExt &nmext)
		{
			SetNameExt(nmext);
		}

		/// Sets new name and extension
		void SetNameExt(const iStringT &str)
		{
			ParseNameExt(str);
		}

		/// Sets new name and extension
		void SetNameExt(const iNameExt &nmext)
		{
			m_Name = nmext.GetName();
			m_Ext = nmext.GetExt();
		}

		/// Sets new name
		void SetName(const iStringT &str)
		{
			m_Name = str;
		}

		/// Sets new extension
		void SetExt(const iStringT &str)
		{
			m_Ext = str;
		}

		/// Returns file name
		iStringT	GetName() const
		{
			return m_Name;
		}

		/// Returns file extension
		iStringT GetExt() const
		{
			return m_Ext;
		}

		/// Returns file name and extension
		iStringT GetNameExt() const
		{
			iStringT result(m_Name);
			if (!m_Ext.Empty()) result += (_T(".") + m_Ext);

			return result;
		}

	private:

		/// Parses file name and extension
		void ParseNameExt(const iStringT &str)
		{
			iStringT wkstr(str);

			if (wkstr.Find(PATH_DOT) == iStringT::NOT_FOUND)
			{
				m_Name = wkstr;
				m_Ext = _T("");
				return;
			}
			else
			{
				sint32 pos = wkstr.ReverseFind(PATH_DOT);

				iStringT nm;
				iStringT ex;

				if (pos == 0)
				{
					nm=wkstr;
					ex=_T("");
				}
				else
				{
					nm=wkstr.Left(pos);
					ex=wkstr.Mid(pos + 1);
				}

				nm.Trim();
				ex.Trim();

				m_Name = nm;
				m_Ext = ex;
			}
		}

		iStringT	m_Name;
		iStringT	m_Ext;

	};



	//
	// iFileName
	//

public:
	// iFileName("//MYPC/E$/folder/file.ext");
	iFileName(const iStringT &fullname);
	// iFileName("file.ext","C:/path/");
	iFileName(const iStringT &name, const iStringT &devpath);
	// iFileName("file.ext","path/","C:/");
	iFileName(const iStringT &name, const iStringT &path, const iStringT dev);


	void SetDevPathNameExt(const iStringT &fullname);
	void SetDevPath(const iStringT &devpath);
	void SetPath(const iStringT &path);
	void SetDev(const iStringT &dev);
	void SetNameExt(const iStringT &nmext);
	void SetName(const iStringT &name);
	void SetExt(const iStringT &ext);

	iStringT GetDevPathNameExt(void);
	iStringT GetDevPathName(void);
	iStringT GetDevPath(void);
	iStringT GetPath(void);
	iStringT GetDev(void);
	iStringT GetNameExt(void);
	iStringT GetName(void);
	iStringT GetExt(void);

private:
	iStringT		m_NameExt;		// "filename" or "filename.ext" or "file.name.ext"
	iStringT		m_Path;			// "path/" or "path/path/"
	iStringT		m_Device;		// "/" or "C:/" or "//MYPC/device/"

private:

	iStringT CutDevice(iStringT &str);
	iStringT CutNameExt(iStringT &str);

	void NormalizePath(iStringT &str);
	void NormalizeDev(iStringT &str);
	void NormalizeDevPath(iStringT &str);
	void NormalizeNameExt(iStringT &str);

	iStringT ExtractName(const iStringT &str);
	iStringT ExtractExt(const iStringT &str);
};

/// Command line parser class
class iCommandLine
{
public:
	/// Constructs iCommandLine object with given command line
	iCommandLine(const iCharT* cmdline)
	{ Parse(cmdline); }

	/// Constructs iCommandLine object with given command line
	iCommandLine(iCharT* argv[], uint32 argc)
	{ Parse(argv,argc); }

	/// Parses given command line
	void Parse(iCharT* argv[], uint32 argc)
	{
		m_ArgArray.RemoveAll();

		for (uint32 xx=1; xx<argc; ++xx)
		{
			iStringT tStr(argv[xx]);
			tStr.Trim( _T(" \t\"") );
			m_ArgArray.Add(tStr);
		}
	}

	/// Parses given command line
	void Parse(const iCharT* cmdLine)
	{
		m_ArgArray.RemoveAll();

		const iCharT* parsePos = cmdLine;
		while ( *parsePos ) 
		{
			// scan for non whitespace
			while ( *parsePos && IsWhitespace( parsePos ) ) parsePos++;
			if (!*parsePos) break;
			// check for quote
			const iCharT* closingPos = parsePos + 1;
			if ( IsQuote( parsePos ) ) 
			{
				// eat to the closing quote
				while ( *closingPos && !IsQuote( closingPos ) ) closingPos++;
			} 
			else 
			{
				// eat till the whitespace
				while( *closingPos && !IsWhitespace( closingPos ) ) closingPos++;
			}
			// check length
			const iCharT* parseEnd = closingPos;
			//if ( *parseEnd == 0 ) parseEnd--;

			if ( parseEnd - parsePos != 0 ) 
			{
				iStringT arg( parsePos, parseEnd - parsePos );
				arg.Trim( _T(" \t\"") );
				m_ArgArray.Add(arg);
			}

			parsePos = closingPos;
			if (*parsePos != 0) parsePos++;
		}
	}

	/// Returns specified argument
	iStringT GetArgument(int idx)
	{
		return m_ArgArray[idx];
	}

	/// Returns number of arguments
	int GetArgNum()
	{
		return m_ArgArray.GetSize();
	}

	inline bool IsWhitespace( const iCharT* pos )
	{
		return (*pos == _T(' ')) || (*pos == _T('\t'));
	}

	inline bool IsQuote( const iCharT* pos )
	{
		return (*pos == _T('"'));
	}

	iSimpleArray<iStringT>	m_ArgArray;
};

/// Returns current (working) directory
inline iStringT GetCurrentPath()
{
	TCHAR buff[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,buff);
	
	return iStringT(buff);
}

/// Returns Windows directory
inline iStringT GetWindowsPath()
{
	TCHAR buff[MAX_PATH];
	GetWindowsDirectory(buff,MAX_PATH);
	
	return iStringT(buff);
}

/// Returns System32 directory
inline iStringT GetSystem32Path()
{
	TCHAR buff[MAX_PATH];
	GetSystemDirectory(buff,MAX_PATH);
	
	return iStringT(buff);
}

/// Returns Module's directory
inline iStringT GetAppPath()
{
	TCHAR buff[MAX_PATH];
	GetModuleFileName(NULL,buff,MAX_PATH);
	
	return iFileName(iStringT(buff)).GetDevPath();
}

/// Returns Module's file name
inline iStringT GetAppFileName()
{
	TCHAR buff[MAX_PATH];
	GetModuleFileName(NULL,buff,MAX_PATH);
	
	return iFileName(iStringT(buff)).GetNameExt();
}

#endif //_IOLIB_HELIUM_FILENAME_H_
/** @} */ // iolib_he
