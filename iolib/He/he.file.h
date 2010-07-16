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
 *	@brief Generic file stream handling implementation
 */
/** @addtogroup iolib_he
 *  @{
 */

#ifndef _IOLIB_HELIUM_FILE_H_
#define _IOLIB_HELIUM_FILE_H_

#include "he.string.h"
#include "he.memory.dynbuff.h"
#include "he.handle.h"
#include "he.container.simple.array.h"
#include <ole2.h>

/// iFileI common file stream interface
class iFileI
{
public:
	/// Seeking mode enumeration
	enum SeekMode {
		FSEEK_SET			= 0, ///< From the begin
		FSEEK_CUR			= 1, ///< From the current position
		FSEEK_END			= 2  ///< From the end
	};

	/// Default constructor
	iFileI() {}
	/// Destructor
	virtual ~iFileI() {}

	/// Changes the seek pointer to a new location relative to the current seek pointer or the beginning or end of the stream.
	virtual void Seek(sint64 pos, SeekMode smode = FSEEK_SET) = NULL;
	/// Returns current pointer position
	virtual uint64 GetPosition() const = NULL;
	/// Reads a specified number of bytes from the stream object into memory starting at the current seek pointer.
	virtual uint32 Read(void *buff, uint32 buffsize) = NULL;
	/// Writes a specified number of bytes into the stream object starting at the current seek pointer.
	virtual uint32 Write(const void *buff, uint32 buffsize) = NULL;
	/// Returns stream initialization state
	virtual bool IsOpen() const = NULL;
	/// Returns size of stream object
	virtual uint64 GetSize() const = NULL;
};

/*
 *	FileI helper
 *	NOTE: Not byte-order safe!
 */
template< bool bManaged = false >
class iFilePtrT
{
	iFileI*	f_;

	iFilePtrT( const iFilePtrT& );
	iFilePtrT& operator=( const iFilePtrT& );
public:

	iFilePtrT( iFileI* f ) : f_(f) {}
	~iFilePtrT() { if ( bManaged) delete f_;	}
	iFileI* Giveup() { iFileI* f = f_; f_ = 0; return f; }

	void Seek( sint32 pos, iFileI::SeekMode mode = iFileI::FSEEK_SET )	{ f_->Seek( pos, mode);	}
	uint32 Position() const { return f_->GetPosition(); }
	uint32 Size() const { return f_->GetSize();	}

	inline bool Write( uint8 d )	{ return sizeof(uint8) == f_->Write( &d, sizeof(uint8) );}
	inline bool Write( sint8 d )	{ return sizeof(sint8) == f_->Write( &d, sizeof(sint8) );}
	inline bool Write( uint16 d )	{ return sizeof(uint16) == f_->Write( &d, sizeof(uint16) );}
	inline bool Write( sint16 d )	{ return sizeof(sint16) == f_->Write( &d, sizeof(sint16) );}
	inline bool Write( uint32 d )	{ return sizeof(uint32) == f_->Write( &d, sizeof(uint32) );}
	inline bool Write( sint32 d )	{ return sizeof(sint32) == f_->Write( &d, sizeof(sint32) );}
	inline bool Write( float d )	{ return sizeof(float) == f_->Write( &d, sizeof(float) );}
	

	inline bool Read( uint8& d )	{ return sizeof(uint8) == f_->Read( &d, sizeof(uint8) ); }
	inline bool Read( sint8& d )	{ return sizeof(sint8) == f_->Read( &d, sizeof(sint8) ); }
	inline bool Read( uint16& d )	{ return sizeof(uint16) == f_->Read( &d, sizeof(uint16) ); }
	inline bool Read( sint16& d )	{ return sizeof(sint16) == f_->Read( &d, sizeof(sint16) ); }
	inline bool Read( uint32& d )	{ return sizeof(uint32) == f_->Read( &d, sizeof(uint32) ); }
	inline bool Read( sint32& d )	{ return sizeof(sint32) == f_->Read( &d, sizeof(sint32) ); }
	inline bool Read( float& d )	{ return sizeof(float) == f_->Read( &d, sizeof(float) ); }

	inline bool Write( const void* p, size_t sz )	{ return sz == f_->Write( p, sz ); }
	inline bool Read( void* d, size_t sz )			{ return sz == f_->Read( d, sz ); }

	template<typename T>
	inline bool Write( const iStringTemplate<T>& str ) 
	{ return Write( str.Length() ) && Write( str.CStr(), sizeof(T) * ( str.Length() ) ); }

	template<typename T>
	inline bool Read( iStringTemplate<T>& str )
	{ uint32 len; return Read( len ) && Read( iStringSet(str, len ), sizeof(T) * len );	}
};

typedef iFilePtrT<false>	iFileHandle;
typedef iFilePtrT<true>		iFilePtr;

/// Creates new file and creates new stream object within this storage object and returns its iFileI interface
iFileI* CreateWin32File(const iStringT& fname);
/// Opens existing file and creates new stream object within this storage object and returns its iFileI interface
iFileI* OpenWin32File(const iStringT& fname);

/// Creates new file and creates new stream object within this storage object and returns its iFileI interface
iFileI* CreateWinStgFile(IStream *stream);
/// Opens existing file and creates new stream object within this storage object and returns its iFileI interface
iFileI* OpenWinStgFile(IStream *stream);


/// Basic file handling wrapper class
class iFile
{
public:

	typedef void* Handle;

	/// Seeking mode enumeration
	enum SeekMode {
		FSEEK_SET			= 0,
		FSEEK_CUR			= 1,
		FSEEK_END			= 2
	};

	/// Access mode enumeration
	enum AccessMode {
		ACCESS_CONFIG		= 0,	///< Open File only for changing attributes of file (read/write not allowed)
		ACCESS_READ			= 1,	///< Open File for Reading
		ACCESS_WRITE		= 2,	///< Open File for Writing
		ACCESS_READWRITE	= 3,	///< Open File for Reading and Writing
		ACCESS_APPEND		= 4		///< Open File for Appending to end of file
	};

	/// Open mode enumeration
	enum OpenMode {
		OPEN_CREATENEW		= 0,	///< Create New File, if file already exists, then throw exception
		OPEN_CREATEALWAYS	= 1,	///< Create New File, if file already exists, then rewrite it
		OPEN_OPENEXISTING	= 2,	///< Open existing file, if file not exists, then throw exception
		OPEN_OPENALWAYS		= 3		///< Open existing file, if file not exists, then create new

	};

	/// Share mode enumeration
	enum ShareMode {
		SHARE_DENY			= 0,
		SHARE_READ			= 1,
		SHARE_WRITE			= 2,
		SHARE_READWRITE		= 3
	};

	/// Open Traits class
	class OpenTraits
	{
	public:
		/// Constructs OpenTraits with specified settings
		OpenTraits(AccessMode access=ACCESS_READ, OpenMode open=OPEN_OPENEXISTING, ShareMode share=SHARE_DENY) 
		: m_Access(access)
		, m_Open(open)
		, m_Share(share)
		{ }

		/// Sets access mode
		void SetAccessMode(AccessMode access) { m_Access = access; }
		/// Sets open mode
		void SetOpenMode(OpenMode open) { m_Open = open; }
		/// Sets share mode
		void SetShareMode(ShareMode share) { m_Share = share; }

		/// Gets access mode
		AccessMode GetAccessMode(void) const { return m_Access; }
		/// Gets open mode
		OpenMode GetOpenMode(void) const { return m_Open; }
		/// Gets share mode
		ShareMode GetShareMode(void) const { return m_Share; }

	private:
		AccessMode	m_Access;
		OpenMode	m_Open;
		ShareMode	m_Share;

	};

	/// Default read traits
	static OpenTraits defaultReadTraits;
	/// Default write traits
	static OpenTraits defaultReadWriteTraits;
	/// Default create traits
	static OpenTraits defaultCreateTraits;

	/// Constructor
	iFile(const iStringT &fname, const OpenTraits &traits = defaultReadTraits);
	/// Constructor
	iFile();
	/// Destructor
	~iFile();

	/// Returns file's handle
	operator Handle() const	{ return m_hFile; }

	/// Opens specified file
	virtual bool Open(const iStringT &fname, const OpenTraits &traits = defaultReadTraits);
	/// Creates file with specified name
	virtual bool Create(const iStringT &fname, const OpenTraits &traits = defaultCreateTraits);
	/// Closes opened file
	void Close();
	/// Returns - is file opened or not
	bool IsOpen() const;

	/// Seeks to specified position
	void SeekHuge(sint64 pos, SeekMode smode = FSEEK_SET);
	/// Seeks to specified position
	void Seek(sint32 pos, SeekMode smode = FSEEK_SET);

	/// Returns current position
	uint64 GetPositionHuge();
	/// Returns current position
	uint32 GetPosition();

	/// Reads specified ammount of data
	uint32 Read(void *buff, uint32 buffsize);
	/// Writes specified ammount of data
	uint32 Write(const void *buff, uint32 buffsize);
	/// Fills specified ammount of data with given value
	uint32 WriteFill(uint8 val, uint32 count);

	/// Writes narrow string
	uint32 WriteString(const iStringA &str);
	/// Writes unicode string
	uint32 WriteString(const iStringW &str);

	/// Sets end of file
	void SetEOF();
	/// Flushes data
	void Flush();

	/// Returns file size
	uint64 GetSizeHuge() const;
	/// Returns file size
	uint32 GetSize() const;

	/// Checks the existence of a file
	static bool Exists(const iStringA &fname);
	/// Deletes specified file
	static bool Delete(const iStringA &fname);
	/// Renames specified file
	static void Rename(const iStringA &fname, const iStringA &to_fname);
	/// Copies specified file
	static void Copy(const iStringA &fname, const iStringA &to_fname);
	/// Moves specified file
	static void Move(const iStringA &fname, const iStringA &to_fname);

	/// Checks the existence of a directory
	static bool DirExists(const iStringA &dname);
	/// Deletes specified directory
	static bool DirDelete(const iStringA &dname);
	/// Renames specified directory
	static void DirRename(const iStringA &dname, const iStringA &to_dname);
	/// Creates specified directory
	static bool DirCreate(const iStringA &dname);

	/// Returns size of specified file
	static uint32 GetSize(const iStringA &fname);

	/// Checks the existence of a file
	static bool Exists(const iStringW &fname);
	/// Deletes specified file
	static bool Delete(const iStringW &fname);
	/// Renames specified file
	static bool Rename(const iStringW &fname, const iStringW &to_fname);
	/// Copies specified file
	static bool Copy(const iStringW &fname, const iStringW &to_fname);
	/// Moves specified file
	static bool Move(const iStringW &fname, const iStringW &to_fname);

	/// Checks the existence of a directory
	static bool DirExists(const iStringW &dname);
	/// Deletes specified directory
	static bool DirDelete(const iStringW &dname);
	/// Renames specified directory
	static void DirRename(const iStringW &dname, const iStringW &to_dname);
	/// Creates specified directory
	static bool DirCreate(const iStringW &dname);

	/// Returns size of specified file
	static uint32 GetSize(const iStringW &fname);

	/// FileTime class
	struct Time
	{
		// time in seconds since Jan, 1, 1970
		sint32	stamp;

		/// Default constructor
		inline Time()
		{ stamp = 0; }

		void SetSeconds( sint32 seconds );

		// Compare
		inline bool operator < ( const Time& otherTime ) const;
		inline bool operator > ( const Time& otherTime ) const;
		inline bool operator <= ( const Time& otherTime ) const;
		inline bool operator >= ( const Time& otherTime ) const;
		inline bool operator == ( const Time& otherTime ) const;

	};


	/// FileInfo structure
	struct Entry
	{
		iStringT	path;
		uint64		size;
		Time		timeCreate;
		Time		timeAccess;
		Time		timeModify;
		uint16		attr;

		/// Constructor
		inline Entry()
		{
			size = 0;
			attr = 0;
		}

		inline bool IsDir();
	};
	

private:

	Handle m_hFile;
};

/// Text file handling wrapper template class
template <class T>
class iTextFileTemplate
{
public:

	/// File encoding enumeration
	enum TextFileType {
		TFT_ANSI = 0,
		TFT_UNICODE_LE,
		TFT_UNICODE_BE
	};

	/// Opens specified file
	bool Open(const iStringT &fname)
	{
		return ParseFile(fname);
	}

	/// Returns number of text strings in the file
	uint32 GetStringsCount()
	{
		return m_Strings.GetSize();
	}

	/// Returns constant reference to the specified string
	const iStringTemplate<T>& GetConstString(uint32 idx) const
	{
		return m_Strings[idx];
	}

	/// Returns reference to the specified string
	iStringTemplate<T> GetString(uint32 idx)
	{
		return m_Strings[idx];
	}

	/// Adds the string
	void AddString(const iStringTemplate<T> &str)
	{
	}

	/// Inserts the string
	void InsertString(uint32 idx, const iStringTemplate<T> &str)
	{
	}

private:
	/// Parses already opened file
	bool ParseFile(const iStringT &fname)
	{
		iFile file;
		if (!file.Open(fname,iFile::defaultReadTraits)) return false;

		uint32 buffsiz = (file.GetSize() / sizeof(T)) * sizeof(T);
		if (!buffsiz) return false;

		TextFileType tft;

		if (sizeof(T) == sizeof(iCharW))
		{
			// In case of unicode we need to check first two bytes
			uint16 hdr;
			file.Read(&hdr,sizeof(uint16));
			if (hdr == 0xFFFE)
			{
				// Big endian Unicode
				tft = TFT_UNICODE_BE;
				buffsiz -= sizeof(uint16);
			}
			else if (hdr == 0xFEFF)
			{
				// Little endian Unicode
				tft = TFT_UNICODE_LE;
				buffsiz -= sizeof(uint16);
			}
			else
			{
				// Set Default as Little endian Unicode
				tft = TFT_UNICODE_LE;
				file.Seek(0);
			}
		}
		else tft = TFT_ANSI;

		if (!buffsiz) return false;

		uint8 *buff = new uint8[buffsiz];
		file.Read(buff,buffsiz);

		// in case of big endian - we need to reverse bytes
		if (tft == TFT_UNICODE_BE)
		{
			for (uint32 xx=0; xx < buffsiz / sizeof(T); ++xx)
			{
				iSwap(buff[xx*sizeof(T)],buff[xx*sizeof(T)+1]);
			}
		}

		T *tptr = (T*)buff;
		T *last = (T*)(buff+buffsiz - sizeof(T));


		for (;;)
		{
			uint32 slen=0;
			while(tptr != last && *tptr != T('\n'))
			{
				slen++;
				tptr++;
			}

			iStringTemplate<T> str(tptr-slen,slen+1);

			str.Trim();

			m_Strings.Add(str);

			if (tptr != last) tptr++;
			if (tptr == last) break;
		}


		delete[] buff;

		return true;
	}


private:
	iSimpleArray< iStringTemplate<T> >	m_Strings;
};

typedef iTextFileTemplate<iCharA>	iTextFileA;
typedef iTextFileTemplate<iCharW>	iTextFileW;
typedef iTextFileTemplate<iCharT>	iTextFileT;

#endif //_IOLIB_HELIUM_FILE_H_
/** @} */ // iolib_he
