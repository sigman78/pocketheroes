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

#ifndef _GXLIB_FILE_H_
#define _GXLIB_FILE_H_

#include "xxl.ref.countable.h"
#include "xxl.ref.ptr.h"

/*
 *	iFileI interface
 */
class iFileI : public ref_countable
{
public:
	enum SeekMode {
		FSEEK_SET			= 0, // From the begin
		FSEEK_CUR			= 1, // From the current position
		FSEEK_END			= 2  // From the end
	};


	//
	virtual uint32 Seek(sint32 pos, SeekMode smode = FSEEK_SET) = 0;
	virtual uint32 GetPosition() const = 0;
	virtual uint32 Read(void *buff, uint32 buffsize) = 0;
	virtual uint32 Write(const void *buff, uint32 buffsize) = 0;
	virtual bool IsOpen() const = 0;
	virtual uint32 GetSize() const = 0;
	virtual void Flush() = 0;

protected:
	// made protected to prohibit delete operator
	virtual ~iFileI() {}
};

// smart ptr
typedef ref_ptr<iFileI> iFilePtr;

/*
 *	FileI helper
 *	NOTE: Not byte-order safe!
 */
class iFileStream
{
	iFilePtr f_;

	iFileStream( const iFileStream& );
	iFileStream& operator=( const iFileStream& );
public:

	iFileStream( const iFilePtr& f ) : f_(f) {}
	inline iFileI* Get() { return f_.get();}
	inline bool IsValid() const { return f_.get() != NULL; }

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
	{ return Write( str.Length() ) && Write( str.CStr(), sizeof(T) * str.Length() ); }

	bool Read( iStringT& str )
	{ 
		uint32 len; 
		Read( len );
		if (len) {
			str.Set((uint32)0, len);
			Read( str.Str(), len * sizeof(iCharT)); 
			str.SetLength(len);
		} else str.SetEmpty();
		return true;
	}
};

/*
 *	Fabric functions	
 */
iFileI* CreateWin32File(const iStringT& fname);
iFileI* OpenWin32File(const iStringT& fname);

#ifndef UNDER_CE
iFileI* CreateWinStgFile(IStream *stream);
iFileI* OpenWinStgFile(IStream *stream);
#endif

iFileI* CreateMemFile();
iFileI*	OpenMemFileReadOnly( iDynamicBuffer& buf );
iFileI*	OpenMemFileReadOnly( const void* ptr, size_t len );

//
// New in-memory file interface
// Uses arena object for memory management
// Thus allocation is government by the arena rules
// i.e. only in the stack order!



//
bool	CopyFile( iFileI* src, iFileI* dst );

/*
 *	iFile static functions
 */
namespace iFile {
	bool Exists(const iStringT& fname);
	bool Delete(const iStringT& fname);
	bool Rename(const iStringT& fname, const iStringT& to_fname);
	bool Copy(const iStringT& fname, const iStringT& to_fname);
	bool Move(const iStringT& fname, const iStringT& to_fname);
	bool DirExists(const iStringT& dname);
	bool DirDelete(const iStringT& dname);
	void DirRename(const iStringT& dname, const iStringT& to_dname);
	bool DirCreate(const iStringT& dname);
	uint32 GetSize(const iStringT& fname);
} // namespace iFile

/*
 *	FileName functions
 */
namespace iFileName {
	void GetAppFullName(iStringT& str);
	void GetAppName(iStringT& str);
	void GetAppPath(iStringT& str);
} //namespace iFileName 

#endif //_GXLIB_FILE_H_

