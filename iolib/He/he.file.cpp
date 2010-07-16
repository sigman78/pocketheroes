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

/*************************************************************************
 *
 * Name: He.Memory.h
 * Desc: File routine class implementation
 * Auth: Tutankhamen (Robert Tarasov)
 * Date: 2002/11/07
 *
 *************************************************************************/
#include "he.std.h"
#include "he.file.h"
#include "he.file.std.h"
#include "he.debug.log.h"

//
//
//
class iFileWin32 : public iFileI
{
public:

	HANDLE m_hFile;

	iFileWin32()
	: m_hFile(INVALID_HANDLE_VALUE)
	{
	}

	~iFileWin32()
	{
		Close();
	}

	bool Open(const iStringT &fname)
	{
		m_hFile = ::CreateFile(fname.CStr(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		return (m_hFile != INVALID_HANDLE_VALUE);
	}

	bool Create(const iStringT &fname)
	{
		m_hFile = ::CreateFile(fname.CStr(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		return (m_hFile != INVALID_HANDLE_VALUE);
	}

	void Close()
	{
		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(m_hFile);
			m_hFile = INVALID_HANDLE_VALUE;
		}
	}

	// iFileI interface implementation
	void Seek(sint64 pos, SeekMode smode = FSEEK_SET)
	{
		check(IsOpen());
		uint32 win32SeekMode;
		LARGE_INTEGER offset;
		offset.QuadPart = pos;

		switch (smode)
		{
			case FSEEK_SET:		win32SeekMode = FILE_BEGIN; break;
			case FSEEK_CUR:		win32SeekMode = FILE_CURRENT; break;
			case FSEEK_END:		win32SeekMode = FILE_END; break;
			default:			win32SeekMode = FILE_BEGIN; break;
		}

		offset.LowPart = ::SetFilePointer(m_hFile, offset.LowPart, &offset.HighPart, win32SeekMode);
	}

	uint64 GetPosition() const
	{
		check(IsOpen());
		LARGE_INTEGER offset;
		offset.QuadPart = 0;
		offset.LowPart = ::SetFilePointer(m_hFile, 0, &offset.HighPart, FILE_CURRENT);

		return offset.QuadPart;
	}

	uint32 Read(void *buff, uint32 buffsize)
	{
		check(IsOpen());
		DWORD readed;
		::ReadFile(m_hFile, buff, buffsize,&readed,NULL);
		return readed;
	}

	uint32 Write(const void *buff, uint32 buffsize)
	{
		check(IsOpen());
		DWORD writen;
		::WriteFile(m_hFile, buff, buffsize,&writen,NULL);
		return writen;
	}

	bool IsOpen() const
	{
		return (m_hFile != INVALID_HANDLE_VALUE);
	}

	uint64 GetSize() const
	{
		ULARGE_INTEGER size;
		size.QuadPart = 0;
		size.LowPart = ::GetFileSize(m_hFile,&size.HighPart);
		return size.QuadPart;
	}
};

//
//
//
class iFileWinStg : public iFileI
{
public:

	IStream* m_pStream;

	iFileWinStg(IStream *pstream)
	: m_pStream(pstream)
	{
	}

	~iFileWinStg()
	{
		m_pStream->Release();
		m_pStream = NULL;
	}

	// iFileI interface implementation
	void Seek(sint64 pos, SeekMode smode = FSEEK_SET)
	{
		check(IsOpen());
		LARGE_INTEGER lgint;
		lgint.QuadPart = pos;
		HRESULT hr = m_pStream->Seek(lgint,smode,NULL);
		check(hr == S_OK);
	}

	uint64 GetPosition() const
	{
		check(IsOpen());
		ULARGE_INTEGER ulgint;
		LARGE_INTEGER lgint;
		lgint.QuadPart = 0;
		HRESULT hr = m_pStream->Seek(lgint,STREAM_SEEK_CUR,&ulgint);
		check(hr == S_OK);
		return ulgint.QuadPart;
	}

	uint32 Read(void *buff, uint32 buffsize)
	{
		check(IsOpen());
		DWORD readed;
		HRESULT hr = m_pStream->Read(buff,buffsize,&readed);
		check(hr == S_OK);
		return readed;
	}

	uint32 Write(const void *buff, uint32 buffsize)
	{
		check(IsOpen());
		DWORD writen;
		HRESULT hr = m_pStream->Write(buff,buffsize,&writen);
		check(hr == S_OK);
		return writen;
	}

	bool IsOpen() const
	{
		return (m_pStream != NULL);
	}

	uint64 GetSize() const
	{
		STATSTG pst;
		HRESULT hr = m_pStream->Stat(&pst,STATFLAG_NONAME);
		check(hr == S_OK && &pst != NULL);
		return pst.cbSize.QuadPart;
	}
};

// 
iFileI* CreateWin32File(const iStringT& fname)
{
	iFileWin32* result = new iFileWin32();
	if (!result->Create(fname))
	{
		delete result;
		return NULL;
	}
	return result;
}

iFileI* OpenWin32File(const iStringT& fname)
{
	iFileWin32* result = new iFileWin32();
	if (!result->Open(fname))
	{
		delete result;
		return NULL;
	}
	return result;
}

iFileI* CreateWinStgFile(IStream *stream)
{
	if (!stream) return NULL;
	return new iFileWinStg(stream);
}

iFileI* OpenWinStgFile(IStream *stream)
{
	if (!stream) return NULL;
	return new iFileWinStg(stream);
}

//
//
//
iFile::OpenTraits iFile::defaultReadTraits(iFile::ACCESS_READ, iFile::OPEN_OPENEXISTING, iFile::SHARE_READ);
iFile::OpenTraits iFile::defaultCreateTraits(iFile::ACCESS_WRITE, iFile::OPEN_CREATEALWAYS, iFile::SHARE_DENY);
iFile::OpenTraits iFile::defaultReadWriteTraits(iFile::ACCESS_READWRITE, iFile::OPEN_OPENALWAYS, iFile::SHARE_DENY);

iFile::Handle InvalidHandle = INVALID_HANDLE_VALUE;



iFile::iFile()
{
	m_hFile=InvalidHandle;
}

iFile::iFile(const iStringT &fname, const OpenTraits &traits)
{
	m_hFile=InvalidHandle;
	Open(fname,traits);
}

iFile::~iFile()
{
	Close();
}

bool iFile::Open(const iStringT &fname, const OpenTraits &traits)
{
	uint32 win32Access;
	uint32 win32Open;
	uint32 win32Share;

	switch (traits.GetAccessMode())
	{
		case ACCESS_CONFIG:		win32Access = 0; break;
		case ACCESS_READ:		win32Access = GENERIC_READ;  break;
		case ACCESS_WRITE:		win32Access = GENERIC_WRITE;  break;
		case ACCESS_READWRITE:	win32Access = (GENERIC_READ | GENERIC_WRITE);  break;
		case ACCESS_APPEND:		win32Access = GENERIC_WRITE;  break;
		default:				win32Access = GENERIC_READ;  break;
	}

	switch (traits.GetOpenMode())
	{
		case OPEN_CREATENEW:	win32Open	= CREATE_NEW; break;
		case OPEN_CREATEALWAYS:	win32Open	= CREATE_ALWAYS; break;
		case OPEN_OPENEXISTING:	win32Open	= OPEN_EXISTING; break;
		case OPEN_OPENALWAYS:	win32Open	= OPEN_ALWAYS; break;
		default:				win32Open	= OPEN_EXISTING; break;
	}

	switch (traits.GetShareMode())
	{
		case SHARE_DENY:		win32Share	= 0; break;
		case SHARE_READ:		win32Share	= FILE_SHARE_READ; break;
		case SHARE_WRITE:		win32Share	= FILE_SHARE_WRITE; break;
		case SHARE_READWRITE:	win32Share	= (FILE_SHARE_READ | FILE_SHARE_WRITE); break;
		default:				win32Share	= 0; break;
	}

	if (m_hFile!=INVALID_HANDLE_VALUE) Close();

	// Open File
	Handle handle  = ::CreateFile(fname.CStr(), win32Access, win32Share, NULL, win32Open, FILE_ATTRIBUTE_NORMAL, NULL);

	if (handle == InvalidHandle) return false;

	m_hFile = handle;

	// If Access mode is Append then seek to end of file
	if (traits.GetAccessMode() == ACCESS_APPEND)
	{
		Seek(0,FSEEK_END);
	}

	return true;
}

bool iFile::Create(const iStringT &fname, const OpenTraits &traits)
{
	return Open(fname,traits);
}

void iFile::Close()
{
	if (m_hFile == InvalidHandle) return;
	::CloseHandle(m_hFile);
	m_hFile = InvalidHandle;
}

bool iFile::IsOpen() const
{
	return (m_hFile != InvalidHandle);
}


void iFile::SeekHuge(sint64 pos, SeekMode smode)
{
	check(m_hFile != InvalidHandle);

	uint32 win32SeekMode;
	LARGE_INTEGER offset;
	offset.QuadPart = pos;

	switch (smode)
	{
		case FSEEK_SET:		win32SeekMode = FILE_BEGIN; break;
		case FSEEK_CUR:		win32SeekMode = FILE_CURRENT; break;
		case FSEEK_END:		win32SeekMode = FILE_END; break;
		default:			win32SeekMode = FILE_BEGIN; break;
	}

	offset.LowPart = ::SetFilePointer(m_hFile, offset.LowPart, &offset.HighPart, win32SeekMode);

	//if (offset.LowPart == 0xFFFFFFFF && GetLastError() != NO_ERROR) THROWF(FileOpenError, ("Unable to Set File Pointer position !") );
}

void iFile::Seek(sint32 pos, SeekMode smode)
{
	check(m_hFile != InvalidHandle);

	uint32 win32SeekMode;
	sint32 offset = pos;
	uint32 dwPtr;

	switch (smode)
	{
		case FSEEK_SET:		win32SeekMode = FILE_BEGIN; break;
		case FSEEK_CUR:		win32SeekMode = FILE_CURRENT; break;
		case FSEEK_END:		win32SeekMode = FILE_END; break;
		default:			win32SeekMode = FILE_BEGIN; break;
	}

	dwPtr = ::SetFilePointer(m_hFile, offset, NULL, win32SeekMode);

	//if (dwPtr == 0xFFFFFFFF && GetLastError() != NO_ERROR) THROWF(FileOpenError, ("Unable to Set File Pointer position!") );
}

uint64 iFile::GetPositionHuge()
{
	check(m_hFile != InvalidHandle);
	
	LARGE_INTEGER offset;
	offset.QuadPart = 0;

	offset.LowPart = ::SetFilePointer(m_hFile, 0, &offset.HighPart, FILE_CURRENT);
	if (offset.LowPart == 0xFFFFFFFF && GetLastError() != NO_ERROR)
	{
		iLOG(_T("!ERROR in iFile::GetPositionHuge"));
	}
	return offset.QuadPart;
}

uint32 iFile::GetPosition()
{
	check(m_hFile != InvalidHandle);
	
	uint32 offset;

	offset = ::SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT);
	if (offset == 0xFFFFFFFF)
	{
		iLOG(_T("!ERROR in iFile::GetPosition"));
	}
	return offset;
}

uint32 iFile::Read(void *buff, uint32 buffsize)
{
	check(m_hFile != InvalidHandle);
	DWORD readed;
	if (!::ReadFile(m_hFile, buff, buffsize,&readed,NULL)) 
	{
		iLOG(_T("!ERROR in iFile::Read"));
	}
	return  static_cast<uint32>(readed);
}

uint32 iFile::Write(const void *buff, uint32 buffsize)
{
	check(m_hFile != InvalidHandle);
	DWORD writen;
	verify(::WriteFile(m_hFile, buff, buffsize,&writen,NULL));
	return  static_cast<uint32>(writen);
}

uint32 iFile::WriteFill(uint8 val, uint32 count)
{
	check(m_hFile != InvalidHandle);
	DWORD writen;
	uint8 *buff = new uint8[count];
	memset(buff,val,count);
	verify(::WriteFile(m_hFile, buff, count,&writen,NULL));
	return  static_cast<uint32>(writen);
}

uint32 iFile::WriteString(const iStringA &str)
{
	return Write(str.CStr(),str.Length() * sizeof(iCharA));
}

uint32 iFile::WriteString(const iStringW &str)
{
	return Write(str.CStr(),str.Length() * sizeof(iCharW));
}

void iFile::SetEOF()
{
	check(m_hFile != InvalidHandle);
	if (!::SetEndOfFile(m_hFile))
	{
		iLOG(_T("!ERROR in iFile::SetEOF"));
	}
}

void iFile::Flush()
{
	check(m_hFile != InvalidHandle);
	if (!::FlushFileBuffers(m_hFile))
	{
		iLOG(_T("!ERROR in iFile::Flush"));
	}
}

uint64 iFile::GetSizeHuge() const 
{
	check(m_hFile != InvalidHandle);
	
	ULARGE_INTEGER size;
	size.QuadPart = 0;
	
	size.LowPart = ::GetFileSize(m_hFile,&size.HighPart);
	
	if (size.LowPart == 0xFFFFFFFF && GetLastError() != NO_ERROR )
	{
		iLOG(_T("!ERROR in iFile::GetSizeHuge"));
	}
	return size.QuadPart;
}

uint32 iFile::GetSize() const
{
	check(m_hFile != InvalidHandle);
	uint32 size = ::GetFileSize(m_hFile,NULL);
	if (size == 0xFFFFFFFF)
	{
		iLOG(_T("!ERROR in iFile::GetSize"));
		return 0;
	}
	return size;
}

//
// static members - ANSI
//
bool iFile::Exists(const iStringA &fname)
{
	DWORD res = ::GetFileAttributesA(fname.CStr());
	if ( (res != INVALID_FILE_ATTRIBUTES) && (0 == (res & FILE_ATTRIBUTE_DIRECTORY))) return true;
	else return false;
}

bool iFile::Delete(const iStringA &fname)
{
	if ( !Exists(fname) ) return true;
	return  ::DeleteFileA(fname.CStr())  ? true : false;
}

void iFile::Rename(const iStringA &fname, const iStringA &to_fname)
{
	if (!Exists(fname) || !::MoveFileA(fname.CStr(), to_fname.CStr()))
	{
	}
}

void iFile::Move(const iStringA &fname, const iStringA &to_fname)
{
	Rename(fname,to_fname);
}

void iFile::Copy(const iStringA &fname, const iStringA &to_fname)
{
	if (!Exists(fname) || !::CopyFileA(fname.CStr(), to_fname.CStr(), FALSE))
	{
	}
}

bool iFile::DirExists(const iStringA &dname)
{
	DWORD res = ::GetFileAttributesA(dname.CStr());
	if ( (res != INVALID_FILE_ATTRIBUTES) && (0 != (res & FILE_ATTRIBUTE_DIRECTORY))) return true;
	else return false;
}

bool iFile::DirDelete(const iStringA &dname)
{
	if ( !Exists(dname) ) return true;
	return ::RemoveDirectoryA(dname.CStr()) ? true : false;
}

void iFile::DirRename(const iStringA &dname, const iStringA &to_dname)
{
	Rename(dname,to_dname);
}

bool iFile::DirCreate(const iStringA &dname)
{
	return ::CreateDirectoryA( dname.CStr(), 0 ) ? true : false;
}

uint32 iFile::GetSize(const iStringA &fname)
{
	HANDLE hFile = ::CreateFileA(fname.CStr(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return 0;
	uint32 size = ::GetFileSize(hFile,NULL);
	if (size == 0xFFFFFFFF) return 0;
	return size;
}


//
// static members - UNICODE
//
bool iFile::Exists(const iStringW &fname)
{
	DWORD res = ::GetFileAttributesW(fname.CStr());
	if ( (res != INVALID_FILE_ATTRIBUTES) && (0 == (res & FILE_ATTRIBUTE_DIRECTORY))) return true;
	else return false;
}

bool iFile::Delete(const iStringW &fname)
{
	if ( !Exists( fname) ) return true;
	return  ::DeleteFileW(fname.CStr()) ? true : false;
}

bool iFile::Rename(const iStringW &fname, const iStringW &to_fname)
{
	if (!Exists(fname) || !::MoveFileW(fname.CStr(), to_fname.CStr())) {
		return false;
	}
	return true;
}

bool iFile::Move(const iStringW &fname, const iStringW &to_fname)
{
	return Rename(fname,to_fname);
}

bool iFile::Copy(const iStringW &fname, const iStringW &to_fname)
{
	if (!Exists(fname) || !::CopyFileW(fname.CStr(), to_fname.CStr(), FALSE)) {
		return false;
	}
	return true;
}

bool iFile::DirExists(const iStringW &dname)
{
	DWORD res = ::GetFileAttributesW(dname.CStr());
	if ( (res != INVALID_FILE_ATTRIBUTES) && (0 != (res & FILE_ATTRIBUTE_DIRECTORY))) return true;
	else return false;
}

bool iFile::DirDelete(const iStringW &dname)
{
	if ( !DirExists( dname ) ) return true;
	return ::RemoveDirectoryW(dname.CStr()) ? true : false;
}

void iFile::DirRename(const iStringW &dname, const iStringW &to_dname)
{
	if (!DirExists(dname)){
		return;
	}
	Rename(dname,to_dname);
}

bool iFile::DirCreate(const iStringW &dname)
{
	return ::CreateDirectoryW( dname.CStr(), 0 ) ? true : false;
}

uint32 iFile::GetSize(const iStringW &fname)
{
	HANDLE hFile = ::CreateFileW(fname.CStr(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return 0;
	uint32 size = ::GetFileSize(hFile,NULL);
	if (size == 0xFFFFFFFF) return 0;
	return size;
}
