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

#include "he.file.name.h"
#include "he.debug.log.h"

// iFileName("//MYPC/E$/folder/file.ext");
iFileName::iFileName(const iStringT &fullname)
{
	iStringT str(fullname);

	m_Device = CutDevice(str);
	m_NameExt = CutNameExt(str);
	m_Path = str;

	NormalizeDev(m_Device);
	NormalizeNameExt(m_NameExt);
	NormalizePath(m_Path);
}

// iFileName("file.ext","C:/path/");
iFileName::iFileName(const iStringT &name, const iStringT &devpath)
{
	iStringT str(devpath);

	m_Device = CutDevice(str);
	m_NameExt = name;
	m_NameExt = CutNameExt(str);
	m_Path = str;

	NormalizeDev(m_Device);
	NormalizeNameExt(m_NameExt);
	NormalizePath(m_Path);
}

// iFileName("file.ext","path/","C:/");
iFileName::iFileName(const iStringT &name, const iStringT &path, const iStringT dev)
{
	m_NameExt = name;
	m_Path = path;
	m_Device = dev;

	NormalizeDev(m_Device);
	NormalizeNameExt(m_NameExt);
	NormalizePath(m_Path);
}

void iFileName::SetDevPathNameExt(const iStringT &fullname)
{
}

void iFileName::SetDevPath(const iStringT &devpath)
{
	NormalizeDevPath(m_Device);
}

void iFileName::SetPath(const iStringT &path)
{
	NormalizePath(m_Path);
}

void iFileName::SetDev(const iStringT &dev)
{
	NormalizeDev(m_Device);
}

void iFileName::SetNameExt(const iStringT &nmext)
{
	NormalizeNameExt(m_NameExt);
}

void iFileName::SetName(const iStringT &name)
{
	NormalizeNameExt(m_NameExt);
}

void iFileName::SetExt(const iStringT &ext)
{
	NormalizeNameExt(m_NameExt);
}

iStringT iFileName::GetDevPathNameExt(void)
{
	return m_Device + m_Path + m_NameExt;
}

iStringT iFileName::GetDevPathName(void)
{
	return m_Device + m_Path + ExtractName(m_NameExt);
}

iStringT iFileName::GetDevPath(void)
{
	return m_Device + m_Path;
}

iStringT iFileName::GetPath(void)
{
	return m_Path;
}

iStringT iFileName::GetDev(void)
{
	return m_Device;
}

iStringT iFileName::GetNameExt(void)
{
	return m_NameExt;
}

iStringT iFileName::GetName(void)
{
	return ExtractName(m_NameExt);
}

iStringT iFileName::GetExt(void)
{
	return ExtractExt(m_NameExt);
}

//
// Private
//

iStringT iFileName::CutDevice(iStringT &str)
{
	if (str.Length() < 3) return iStringT();

	// //COMPUTERNAME/Resource/Path/Path/...
	if (str.operator[](0) == PATH_SLASH && str[1] == PATH_SLASH)
	{
		iStringT cname;
		iStringT rname;
		sint32 dwpos = str.Find(PATH_SLASH,2);
		if (dwpos == iStringT::NOT_FOUND) return iStringT();
		else  cname = str.Mid(0,dwpos+1);

		sint32 n_dwpos = str.Find(PATH_SLASH, dwpos+1);
		if (n_dwpos == iStringT::NOT_FOUND) return iStringT();
		else  rname = str.Mid(dwpos+1,n_dwpos - dwpos);

		iStringT test_c = cname;
		iStringT test_r = rname;

		iCharT trimSpec[] = { _T(' '), PATH_SLASH, 0 };
		test_c.Trim(trimSpec);
		test_r.Trim(trimSpec);

		if (test_c.Empty() || test_r.Empty()) return iStringT();

		str = str.Right(str.Length() - n_dwpos - 1);

		return cname + rname;
	}
	// C:/Path/Path/...
	else if (str[1] == PATH_COLLON && str[2] == PATH_SLASH)
	{
		iStringT result=str.Left(3);
		str = str.Right(str.Length() - 3);
		return result;
	}
	// Unknown
	else
	{
		return iStringT();
	}

}

iStringT iFileName::CutNameExt(iStringT &str)
{
	sint32 dwpos = str.ReverseFind(PATH_SLASH);

	if (dwpos == iStringT::NOT_FOUND)
	{
		iStringT res = str;
		str=_T("");
		return res;
	}

	iStringT result = str.Right(str.Length() - dwpos - 1);

	str = str.Left(dwpos+1);

	return result;
}

void iFileName::NormalizePath(iStringT &str)
{
	iCharT trimSpec[] = { _T(' '), 0 };
	str.Trim(trimSpec);

	if(str.Empty()) return;

	iCharT sl[]={PATH_SLASH,0};

	if (str.Left(1) == sl)
	{
		str = str.Right(str.Length() - 1);
	}

	if (str.Right(1) != sl)
	{
		str += sl;
	}
}

void iFileName::NormalizeDev(iStringT &str)
{
	iCharT trimSpec[] = { _T(' '), 0 };
	str.Trim(trimSpec);

	if(str.Empty()) return;

	iCharT sl[]={PATH_SLASH,0};

	if (str.Right(1) != sl)
	{
		str += sl;
	}
}

void iFileName::NormalizeDevPath(iStringT &str)
{
	iCharT trimSpec[] = { _T(' '), 0 };
	str.Trim(trimSpec);

	if(str.Empty()) return;

	iCharT sl[]={PATH_SLASH,0};

	if (str.Right(1) != sl)
	{
		str += sl;
	}
}

void iFileName::NormalizeNameExt(iStringT &str)
{
	iCharT trimSpec[] = { _T(' '), PATH_SLASH, 0 };
	str.Trim(trimSpec);
}


iStringT iFileName::ExtractName(const iStringT &str)
{
	if (str.Empty()) return str;

	sint32 pos = m_NameExt.ReverseFind(PATH_DOT);

	if (pos == iStringT::NOT_FOUND) return str;
	else if(pos == (sint32)str.Length()-1) return str.Left(str.Length()-1);
	else if(pos == 0) return iStringT();
	else return m_NameExt.Left(pos);
}

iStringT iFileName::ExtractExt(const iStringT &str)
{
	if (str.Empty()) return str;

	sint32 pos = m_NameExt.ReverseFind(PATH_DOT);

	if (pos == iStringT::NOT_FOUND) return str;
	else if(pos == (sint32)str.Length()-1) return iStringT();
	else if(pos == 0) return str;
	else return m_NameExt.Right(str.Length() - pos - 1);
}
