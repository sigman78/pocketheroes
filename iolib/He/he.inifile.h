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
 *	@brief Profile settings handling wrapper class implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_INIFILE_H_
#define _IOLIB_HELIUM_INIFILE_H_

#include "he.std.h"
#include "he.string.h"
#include "he.file.name.h"

/// Profile settings handling wrapper class (base class)
class iIniStorage
{
public:
	/// Default constructor
	iIniStorage()
	{
	}

	/// Destructor
	~iIniStorage()
	{
	}

protected:

	/// Represents bool value as string
	iStringT BoolToString(const bool b)
	{
		iStringT result;
		result.Setf(_T("%s"),(b)?_T("Y"):_T("N"));
		return result;
	}

	/// Returns bool value, represented as string
	bool StringToBool(const iStringT &str)
	{
		bool result;

		switch( str[0] ) 
		{
		case _T('y'): // Yes
		case _T('Y'):
		case _T('t'): // True
		case _T('T'):
		case _T('1'): // 1
			result = true;
			break;
		case _T('n'): // No
		case _T('N'):
		case _T('f'): // False
		case _T('F'):
		case _T('0'): // 0
			result = false;
			break;
		default:
			result = false;
		}

		return result;
	}

	/// Represents point value as string
	iStringT PointToString(const iPoint &point)
	{
		iStringT result;
		result.Setf(_T("iPoint(%d,%d)"),point.x,point.y);
		return result;
	}

	/// Returns point value, represented as string
	iPoint StringToPoint(const iStringT &str)
	{
		sint32 res_x=0;
		sint32 res_y=0;

		_stscanf(str.CStr(),_T("iPoint(%d,%d)"),&res_x,&res_y);

		return iPoint(res_x,res_y);
	}

	/// Represents size value as string
	iStringT SizeToString(const iSize &size)
	{
		iStringT result;
		result.Setf(_T("iSize(%u,%u)"),size.w,size.h);
		return result;
	}

	/// Returns size value, represented as string
	iSize StringToSize(const iStringT &str)
	{
		uint32 res_w=0;
		uint32 res_h=0;

		_stscanf(str.CStr(),_T("iSize(%u,%u)"),&res_w,&res_h);

		return iSize(res_w,res_h);
	}

	/// Represents rect value as string
	iStringT RectToString(const iRect &rect)
	{
		iStringT result;
		result.Setf(_T("iRect(%d,%d,%u,%u)"),rect.x,rect.y,rect.w,rect.h);
		return result;
	}

	/// Returns rect value, represented as string
	iRect StringToRect(const iStringT &str)
	{
		sint32 res_x=0;
		sint32 res_y=0;
		uint32 res_w=0;
		uint32 res_h=0;

		_stscanf(str.CStr(),_T("iRect(%d,%d,%u,%u)"),&res_x,&res_y,&res_w,&res_h);

		return iRect(res_x,res_y,res_w,res_h);
	}

	/// Returns raw buffer, represented as HEX string of text
	iSimpleArray<uint8> StringToBinary(const iStringA &str)
	{
		iSimpleArray<uint8> result;

		for (uint32 xx=0; xx < str.Size()/2; ++xx)
		{
			uint8 val=0;
			iCharA ch = (iCharA)str[xx*2];

			if (ch >= ((unsigned char)'0') && ch <= ((unsigned char)'9')) val += (ch-((unsigned char)'0'))*16;
			else if (ch >= ((unsigned char)'A') && ch <= ((unsigned char)'F')) val += (ch-((unsigned char)'A')+10)*16;

			ch = (unsigned char)str[xx*2+1];
			if (ch >= ((unsigned char)'0') && ch <= ((unsigned char)'9')) val += (ch-((unsigned char)'0'));
			else if (ch >= ((unsigned char)'A') && ch <= ((unsigned char)'F')) val += (ch-((unsigned char)'A')+10);

			result.Add(val);
		}
		return result;
	}

	/// Returns raw buffer, represented as HEX string of text
	iSimpleArray<uint8> StringToBinary(const iStringW &str)
	{
		iSimpleArray<uint8> result;

		for (uint32 xx=0; xx < str.Size()/2; ++xx)
		{
			uint8 val=0;
			iCharW ch = (iCharW)str[xx*2];

			if (ch >= ((unsigned short)'0') && ch <= ((unsigned short)'9')) val += (ch-((unsigned short)'0'))*16;
			else if (ch >= ((unsigned short)'A') && ch <= ((unsigned short)'F')) val += (ch-((unsigned short)'A')+10)*16;

			ch = (unsigned short)str[xx*2+1];
			if (ch >= ((unsigned short)'0') && ch <= ((unsigned short)'9')) val += (ch-((unsigned short)'0'));
			else if (ch >= ((unsigned short)'A') && ch <= ((unsigned short)'F')) val += (ch-((unsigned short)'A')+10);

			result.Add(val);
		}
		return result;
	}

	/// Represents raw buffer as HEX string of text
	iStringT BinaryToString(const iSimpleArray<uint8> &bin)
	{
		iStringT result(_T(""));

		for (uint32 xx=0; xx < bin.GetSize(); ++xx)
		{
			result.Addf(_T("%02X"),bin[xx]);
		}

		return result;
	}
};

/// Profile settings handling wrapper class (Windows registry)
class iIniRegistry : public iIniStorage
{
public:
	iStringT	m_RegKeyName;
	HKEY		m_hKey;

	/// Default constructor
	iIniRegistry()
	: m_hKey(NULL)
	{
		m_RegKeyName = _T("iSpy");
	}

	/// Destructor
	~iIniRegistry()
	{
		if (m_hKey)	::RegCloseKey(m_hKey);
	}

	/// Initializes storage
	bool InitStorage()
	{
		LONG lResult = ERROR_SUCCESS;
		SECURITY_ATTRIBUTES sa = {0};
		if (m_hKey)	{
			::RegCloseKey(m_hKey);
			m_hKey = NULL;
		}
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		lResult = ::RegCreateKeyEx(HKEY_CURRENT_USER, _T("Software\\iSpy"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, &sa, &m_hKey, NULL);
		if ((ERROR_SUCCESS == lResult) && (NULL != m_hKey))	return true;
		return false;
	}

	/// Puts bool value into profile
	void PutBool(const iStringT &vname, bool value)
	{
		PutUInt32(vname, value);
	}

	/// Puts uint32 value into profile
	void PutUInt32(const iStringT &vname, uint32 value)
	{
		::RegSetValueEx(m_hKey,vname.CStr(),NULL,REG_DWORD,(BYTE*)&value,sizeof(uint32));
	}

	/// Puts sint32 value into profile
	void PutSInt32(const iStringT &vname, sint32 value)
	{
		::RegSetValueEx(m_hKey,vname.CStr(),NULL,REG_DWORD,(BYTE*)&value,sizeof(sint32));
	}

	/// Puts point value into profile
	void PutPoint(const iStringT &vname, const iPoint &value)
	{
		::RegSetValueEx(m_hKey,vname.CStr(),NULL,REG_BINARY,(BYTE*)&value,sizeof(iPoint));
	}

	/// Puts size value into profile
	void PutSize(const iStringT &vname, const iSize &value)
	{
		::RegSetValueEx(m_hKey,vname.CStr(),NULL,REG_BINARY,(BYTE*)&value,sizeof(iSize));
	}

	/// Puts rect value into profile
	void PutRect(const iStringT &vname, const iRect &value)
	{
		::RegSetValueEx(m_hKey,vname.CStr(),NULL,REG_BINARY,(BYTE*)&value,sizeof(iRect));
	}

	/// Gets bool value from profile
	bool GetBool(const iStringT &vname, bool defValue)
	{
		return GetUInt32(vname, defValue)?true:false;
	}

	/// Gets uint32 value from profile
	uint32 GetUInt32(const iStringT &vname, uint32 defValue)
	{
		DWORD dwType;
		uint32 dwData;
		DWORD dwSiz = sizeof(uint32);

		if (ERROR_SUCCESS == ::RegQueryValueEx(m_hKey,vname.CStr(),NULL,&dwType,(BYTE*)&dwData,&dwSiz) && 
			dwType == REG_DWORD)
		{
			return dwData;
		}

		return defValue;
	}

	/// Gets sint32 value from profile
	sint32 GetSInt32(const iStringT &vname, sint32 defValue)
	{
		DWORD dwType;
		sint32 dwData;
		DWORD dwSiz = sizeof(sint32);

		if (ERROR_SUCCESS == ::RegQueryValueEx(m_hKey,vname.CStr(),NULL,&dwType,(BYTE*)&dwData,&dwSiz) && 
			dwType == REG_DWORD)
		{
			return dwData;
		}

		return defValue;
	}

	/// Gets point value from profile
	iPoint GetPoint(const iStringT &vname, const iPoint &defValue)
	{
		DWORD dwType;
		iPoint ptData;
		DWORD ptSiz = sizeof(iPoint);

		if (ERROR_SUCCESS == ::RegQueryValueEx(m_hKey,vname.CStr(),NULL,&dwType,(BYTE*)&ptData,&ptSiz) && 
			dwType == REG_BINARY)
		{
			return ptData;
		}

		return defValue;
	}

	/// Gets size value from profile
	iSize GetSize(const iStringT &vname, const iSize &defValue)
	{
		DWORD dwType;
		iSize szData;
		DWORD szSiz = sizeof(iSize);

		if (ERROR_SUCCESS == ::RegQueryValueEx(m_hKey,vname.CStr(),NULL,&dwType,(BYTE*)&szData,&szSiz) && 
			dwType == REG_BINARY)
		{
			return szData;
		}

		return defValue;
	}

	/// Gets rect value from profile
	iRect GetRect(const iStringT &vname, const iRect &defValue)
	{
		DWORD dwType;
		iRect rcData;
		DWORD rcSiz = sizeof(iRect);

		if (ERROR_SUCCESS == ::RegQueryValueEx(m_hKey,vname.CStr(),NULL,&dwType,(BYTE*)&rcData,&rcSiz) && 
			dwType == REG_BINARY)
		{
			return rcData;
		}

		return defValue;
	}

};

/// Profile settings handling wrapper class (.ini file)
class iIniFile : public iIniStorage
{
public:
	iStringT	m_FileName;

	/// Default constructor
	iIniFile()
	{
		iCharT str[512];
		::GetModuleFileName(NULL,str,512);

		m_FileName = iFileName(iStringT(str)).GetName();
	}

	/// Constructs iIniFile object with given file name
	iIniFile(const iStringT &fname)
	: m_FileName(fname)
	{
	}

	/// Sets file name
	void SetFilename(const iStringT &fname)
	{
		m_FileName = fname;
	}

	/// Gets string of text value from profile
	iStringT GetString(const iStringT &section, const iStringT &key, const iStringT &defValue)
	{
		iCharT buffer[1024];
		iStringT result;

		sint32 ret = ::GetPrivateProfileString(section.CStr(), key.CStr(), defValue.CStr(), buffer, 1024, m_FileName.CStr());

		if (ret) result = iStringT(buffer);
		if (result == defValue) PutString(section,key,result);

		return result;
	}

	/// Gets integer value from profile
	int GetInt(const iStringT &section, const iStringT &key, sint32 defValue)
	{
		sint32 result = ::GetPrivateProfileInt(section.CStr(), key.CStr(), defValue, m_FileName.CStr());

		if (result == defValue) PutInt(section,key,result);

		return result;
	}

	/// Gets boolean value from profile
	bool GetBool(const iStringT &section, const iStringT &key, bool defValue)
	{
		iCharT buffer[4] = { 0 };
	
		::GetPrivateProfileString(section.CStr(), key.CStr(), (defValue)?_T("Y"):_T("N"), buffer, 4, m_FileName.CStr());

		bool result=StringToBool(iStringT(buffer));

		if (result == defValue) PutBool(section,key,result);

		return result;
	}

	/// Gets point value from profile
	iPoint GetPoint(const iStringT &section, const iStringT &key, const iPoint &defValue)
	{
		iPoint result;
		iStringT buffer = GetString(section,key,PointToString(defValue));

		result = StringToPoint(buffer);

		if (result == defValue) PutPoint(section,key,defValue);

		return result;
	}

	/// Gets size value from profile
	iSize GetSize(const iStringT &section, const iStringT &key, const iSize &defValue)
	{
		iSize result;
		iStringT buffer = GetString(section,key,SizeToString(defValue));

		result = StringToSize(buffer);

		if (result == defValue) PutSize(section,key,defValue);

		return result;
	}

	/// Gets rect value from profile
	iRect GetRect(const iStringT &section, const iStringT &key, const iRect &defValue)
	{
		iRect result;
		iStringT buffer = GetString(section,key,RectToString(defValue));

		result = StringToRect(buffer);

		if (result == defValue) PutRect(section,key,defValue);

		return result;
	}

	/// Gets raw binary data from profile
	iSimpleArray<uint8>	GetBinary(const iStringT &section, const iStringT &key, const iSimpleArray<uint8> &defValue)
	{
		iStringT buffer = GetString(section,key,BinaryToString(defValue));

		iSimpleArray<uint8> result = StringToBinary(buffer);

		if (result == defValue) PutBinary(section,key,defValue);

		return result;
	}

	/// Puts string of text value to profile
	void PutString(const iStringT &section, const iStringT &key, const iStringT &sValue)
	{
		::WritePrivateProfileString(section.CStr(), key.CStr(), sValue.CStr(), m_FileName.CStr());
	}

	/// Puts integer value to profile
	void PutInt(const iStringT &section, const iStringT &key, sint32 iValue)
	{
		iStringT buffer;
		buffer.Setf(_T("%ld"),iValue);
		PutString(section,key,buffer);
	}

	/// Puts boolean value to profile
	void PutBool(const iStringT &section, const iStringT &key, bool bValue)
	{
		iStringT buffer = BoolToString(bValue);
		PutString(section, key, buffer);
	}

	/// Puts point value to profile
	void PutPoint(const iStringT &section, const iStringT &key, const iPoint &pValue)
	{
		PutString(section,key,PointToString(pValue));
	}
	
	/// Puts size value to profile
	void PutSize(const iStringT &section, const iStringT &key, const iSize &sValue)
	{
		PutString(section,key,SizeToString(sValue));
	}

	/// Puts rect value to profile
	void PutRect(const iStringT &section, const iStringT &key, const iRect &rValue)
	{
		PutString(section,key,RectToString(rValue));
	}

	/// Puts raw binary data to profile
	void PutBinary(const iStringT &section, const iStringT &key, const iSimpleArray<uint8> &bValue)
	{
		PutString(section,key,BinaryToString(bValue));
	}

	/// Deletes specified key
	void DeleteKey(const iStringT &section, const iStringT &key)
	{
		::WritePrivateProfileString(section.CStr(), key.CStr(), NULL, m_FileName.CStr());
	}

	/// Deletes specified section
	void DeleteSection(const iStringT &section)
	{
		::WritePrivateProfileString(section.CStr(), NULL, NULL, m_FileName.CStr());
	}

	/// Flushes data
	void Flush()
	{
		::WritePrivateProfileString(NULL, NULL, NULL, m_FileName.CStr());
	}
};


#endif //_IOLIB_HELIUM_INIFILE_H_
/** @} */ // iolib_he
