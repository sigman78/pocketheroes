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


#include "gxl.inc.h"
#include "gxl.memory.h"
#include "gxl.file.h"

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES 0xffffffff
#endif

class iFileWin32 : public ref_countable_impl<iFileI>
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

	bool Open(const iStringT& fname)
	{
		m_hFile = ::CreateFile(fname.CStr(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		return (m_hFile != INVALID_HANDLE_VALUE);
	}

	bool Create(const iStringT& fname)
	{
		m_hFile = ::CreateFile(fname.CStr(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		return (m_hFile != INVALID_HANDLE_VALUE);
	}

	void Close()
	{
		if (m_hFile != INVALID_HANDLE_VALUE) {
			::CloseHandle(m_hFile);
			m_hFile = INVALID_HANDLE_VALUE;
		}
	}

	// iFileI interface implementation
	uint32 Seek(sint32 pos, SeekMode smode = FSEEK_SET)
	{
		check(IsOpen());
		uint32 win32SeekMode;
		LARGE_INTEGER offset;
		offset.QuadPart = pos;

		switch (smode) {
			case FSEEK_SET:		win32SeekMode = FILE_BEGIN; break;
			case FSEEK_CUR:		win32SeekMode = FILE_CURRENT; break;
			case FSEEK_END:		win32SeekMode = FILE_END; break;
			default:			win32SeekMode = FILE_BEGIN; break;
		}

		offset.LowPart = ::SetFilePointer(m_hFile, offset.LowPart, &offset.HighPart, win32SeekMode);
		return offset.LowPart;
	}

	uint32 GetPosition() const
	{
		check(IsOpen());
		return ::SetFilePointer(m_hFile, 0, 0, FILE_CURRENT);
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

	uint32 GetSize() const
	{
		return ::GetFileSize(m_hFile,0);
	}

	void Flush()
	{
		check(IsOpen());
		::FlushFileBuffers(m_hFile);
	}
};

//
//
//
#ifndef UNDER_CE
class iFileWinStg : public ref_countable_impl<iFileI>
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
	uint32 Seek(sint32 pos, SeekMode smode = FSEEK_SET)
	{
		check(IsOpen());
		LARGE_INTEGER lgint;
		ULARGE_INTEGER newPos;
		lgint.QuadPart = pos;
		HRESULT hr = m_pStream->Seek(lgint,smode,&newPos);
		check(hr == S_OK);
		if ( hr == S_OK ) return newPos.LowPart;
		else return ~0UL;	
	}

	uint32 GetPosition() const
	{
		check(IsOpen());
		ULARGE_INTEGER ulgint;
		LARGE_INTEGER lgint;
		lgint.QuadPart = 0;
		HRESULT hr = m_pStream->Seek(lgint,STREAM_SEEK_CUR,&ulgint);
		check(hr == S_OK);
		return (uint32)ulgint.QuadPart;
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

	uint32 GetSize() const
	{
		STATSTG pst;
		HRESULT hr = m_pStream->Stat(&pst,STATFLAG_NONAME);
		check(hr == S_OK && &pst != NULL);
		return (uint32)pst.cbSize.QuadPart;
	}

	void Flush()
	{
	}
};
#endif

// 
iFileI* CreateWin32File(const iStringT& fname)
{
	iFileWin32* result = new iFileWin32();
	if (!result->Create(fname)) {
		delete result;
		return NULL;
	}
	return result;
}

iFileI* OpenWin32File(const iStringT& fname)
{
	iFileWin32* result = new iFileWin32();
	if (!result->Open(fname)) {
		delete result;
		return NULL;
	}
	return result;
}

#ifndef UNDER_CE
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
#endif


/*
 *	iFile static functions
 */
namespace iFile {

bool Exists(const iStringT& fname)
{
	DWORD res = ::GetFileAttributesW(fname.CStr());
	if ( res == INVALID_FILE_ATTRIBUTES ) return false;
	else return (res & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

bool Delete(const iStringT& fname)
{
	if ( !Exists(fname) ) return true;
	return  ::DeleteFileW(fname.CStr())  ? true : false;
}

bool Rename(const iStringT& fname, const iStringT& to_fname)
{
	if (!Exists(fname) || !::MoveFileW(fname.CStr(), to_fname.CStr())) {
		return false;
	}
	return true;
}

bool Copy(const iStringT& fname, const iStringT& to_fname)
{
	if (!Exists(fname) || !::CopyFileW(fname.CStr(), to_fname.CStr(), FALSE)) {
		return false;
	}
	return true;
}

bool Move(const iStringT& fname, const iStringT& to_fname)
{
	return Rename(fname,to_fname);
}

bool DirExists(const iStringT& dname)
{
	DWORD res = ::GetFileAttributesW(dname.CStr());
	if ( res == INVALID_FILE_ATTRIBUTES ) return false;
	else return (res & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool DirDelete(const iStringT& dname)
{
	if ( !DirExists( dname ) ) return true;
	return ::RemoveDirectoryW(dname.CStr()) ? true : false;
}

void DirRename(const iStringT& dname, const iStringT& to_dname)
{
	if (!DirExists(dname)){
		return;
	}
	Rename(dname,to_dname);
}

bool DirCreate(const iStringT& dname)
{
	return ::CreateDirectoryW( dname.CStr(), 0 ) ? true : false;
}

uint32 GetSize(const iStringT& fname)
{
	HANDLE hFile = ::CreateFileW(fname.CStr(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return 0;
	uint32 size = ::GetFileSize(hFile,NULL);
	if (size == 0xFFFFFFFF) return 0;
	return size;
}

} // namespace iFile

/*
 *	FileName functions
 */
namespace iFileName {

void GetAppFullName(iStringT& str)
{
	iCharW buff[MAX_PATH];
	GetModuleFileName(0,buff,MAX_PATH);
	str = buff;
}

void GetAppName(iStringT& str)
{
	iStringT fname;
	GetAppFullName(fname);
	sint32 pos = fname.ReverseFind('\\');
	if (pos != -1){
		str = fname.Mid(pos+1);
	}
}

void GetAppPath(iStringT& str)
{
	iStringT fname;
	GetAppFullName(fname);
	sint32 pos = fname.ReverseFind('\\');
	if (pos != -1){
		str = fname.Left(pos+1);
	}
}

} //namespace iFileName 
