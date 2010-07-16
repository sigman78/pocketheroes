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

#include "he.std.h"
#include "he.threads.h"

/****
	TODO: Should we throw exception if sync object construction in
	os layer is failed ???
	WARN: By the meaning, thread event must be in manual reset state,
	such behavior lets us to use this object in multiwait
	WARN: Thread::IsDone and ::IsAlive is suspective, need to rewrite
	using sync objects. I think.
****/


/*
#define __DEBUGALLOC_MODULE 0x10000000
#define __INCLUDE_WINDOWS
#include "Std.h"
#include "Thread.h"
#include "ExceptionHandler.h"
*/


// SyncObject
//

iSyncObject::~iSyncObject()
{
	if ( m_Handle != NULL ) 
	{
		::CloseHandle( m_Handle );
		m_Handle = NULL;
	}
}

iSyncObject::iSyncObject()
{
	m_Handle	= NULL;
	m_Name		= NULL;
}

iSyncObject::iSyncObject( const iCharT* aname )
{
	m_Handle = NULL;
#ifdef DEBUG
	m_Name = aname;
#endif
}

bool iSyncObject::Lock( uint32 timeout )
{
	if ( ::WaitForSingleObject( m_Handle, timeout ) == WAIT_OBJECT_0 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool iSyncObject::Unlock()
{
	return true;
}

bool iSyncObject::Unlock( uint32 count )
{
	return true;
}

bool iSyncObject::IsLocked()
{
	return false;
}

// CriticalSection
//

iCriticalSection::iCriticalSection( const iCharT* aname ) : iSyncObject( NULL )
{
	FillBlock8((uint8*)&m_CritSec,0,sizeof(m_CritSec));
	::InitializeCriticalSection( &m_CritSec );
}

iCriticalSection::~iCriticalSection()
{
	::DeleteCriticalSection( &m_CritSec );
}

bool iCriticalSection::Lock( uint32 timeout )
{
	::EnterCriticalSection( &m_CritSec );
	return true;
}

bool iCriticalSection::Unlock()
{
	::LeaveCriticalSection( &m_CritSec );
	return true;
}

bool iCriticalSection::Unlock( uint32 count )
{
	return Unlock();
}

bool iCriticalSection::IsLocked()
{
	return false;//::TryEnterCriticalSection( &crit );
}

uint32 iCriticalSection::SetSpinCount( uint32 spincount )
{
	return 0;//::SetCriticalSectionSpinCount( &crit, spincount );
}

// Mutex
//

iMutex::iMutex( bool owned, const iCharT* ID ) : iSyncObject( ID )
{
	m_Handle = ::CreateMutex( NULL, owned, ID );
	// TODO: throw exception ??
}

bool iMutex::Unlock()
{
	return ::ReleaseMutex( m_Handle ) ? true:false;
}

bool iMutex::Unlock( uint32 count )
{
	return Unlock();
}

// Semaphore
//

iSemaphore::iSemaphore( uint32 initial, uint32 max, const iCharT* id ) : iSyncObject( id )
{
	check( max > 0 );
	check( initial <= max );
	m_Handle = ::CreateSemaphore( NULL, initial, max, id );
	// throw exception if handle == NULL ???
}

bool iSemaphore::Unlock()
{
	return Unlock( 1 );
}

bool iSemaphore::Unlock( uint32 count )
{
	return ::ReleaseSemaphore( m_Handle, count, NULL ) ? true :false;
}

// WEvent
//

iWEvent::iWEvent( bool manual, bool initial, const iCharT* id ) : iSyncObject( id )
{
	m_Handle = ::CreateEvent( NULL, manual, initial, id );
	// TODO: throw if handle == NULL
}

bool iWEvent::Unlock()
{
	return ::ResetEvent( m_Handle ) ? true : false;
}

void iWEvent::Set()
{
	::SetEvent( m_Handle );
}

void iWEvent::Pulse()
{
	::PulseEvent( m_Handle );
}

void iWEvent::Reset()
{
	::ResetEvent( m_Handle );
}

// Thread
//
iThread::iThread( ThreadFunction func, void* arg )
{
	check( func != NULL );
	m_Started	= false;
	m_Data		= arg;
	m_Function	= func;
	m_Terminate	= true;
	// lets start the thread
	SafeStart( (void*)this );
}

iThread::~iThread()
{
	if ( IsAlive() && m_Terminate )	Terminate();
}

iThread::Priority iThread::GetPriority() const
{
	if ( IsAlive() ) return (Priority)::GetThreadPriority( m_Handle );
	return (Priority)0;
}

void iThread::SetPriority( Priority priority )
{
	if ( IsAlive() )::SetThreadPriority( m_Handle, (INT)priority );
}

bool iThread::IsDone() const
{
	uint32 dwExitCode;
	return ::GetExitCodeThread( m_Handle, (unsigned long*)&dwExitCode ) && (dwExitCode != STILL_ACTIVE);
}

bool iThread::IsAlive() const
{
	uint32 dwExitCode;
	return ::GetExitCodeThread( m_Handle, (unsigned long*)&dwExitCode ) && (dwExitCode == STILL_ACTIVE);
}

bool iThread::IsStartedOk() const
{
	return m_Started;
}

bool iThread::Suspend()
{
	return ::SuspendThread( m_Handle ) != 0xffffffff;
}

bool iThread::Resume()
{
	return ::ResumeThread( m_Handle ) != 0xffffffff;
}

uint32 iThread::Terminate()
{
	if (!::TerminateThread( m_Handle, 0 )) 
	{
		uint32 eCode;
		::GetExitCodeThread( m_Handle, (unsigned long*)&eCode );
		return eCode;
	}
	return 0;
}

void iThread::End( uint32 result )
{
	// signal thread done
	// <> done.Set(); - cause Access vio.
	::_endthreadex( result );
}

// statics

uint32 iThread::GetCurrentId()
{
	return ::GetCurrentThreadId();
}

uint32 iThread::GetProcessId()
{
	return ::GetCurrentProcessId();
}

void iThread::GetProcessName( iCharT* name )
{
	::GetModuleFileName( NULL, name, 256 );
}

bool iThread::IsProcessAlive( uint32 pid )
{
	uint32 eCode = 0xffaa;
	HANDLE ph = ::OpenProcess( PROCESS_QUERY_INFORMATION, FALSE, pid );
	if ( ph != NULL ) 
	{
		::GetExitCodeProcess( ph, (unsigned long*)&eCode );
		::CloseHandle( ph );
	}
	return eCode == STILL_ACTIVE;
}

UINT __stdcall iThread::Start( void* threadInfo )
{
	// TODO: SEH handling __try, __except
	// assert all incoming data
	iThread* thread = (iThread*)threadInfo;
	check( thread != NULL );
	check( thread->m_Function != NULL );
	// signal started state
	thread->m_Started = true;
	thread->m_Done.Pulse();
	// is it mt-safe ? : check( thread->handle != NULL );
	UINT result = 0;

	result = (*thread->m_Function)( thread->m_Data );
	//result = ExceptionHandler::Main( thread->function, thread->data );

	// assert valid( thread )
	thread->End( result );
	return 0; // not reached
}

INT __cdecl iThread::SafeStart( void* threadInfo )
{
	check( threadInfo != NULL );

	UINT tid;
	iThread* ti = (iThread*)threadInfo;
	if ( ti->m_Handle = (HANDLE)::_beginthreadex( NULL, 0, Start, threadInfo, 0, &tid ) ) 
	{
		// success, wait for event
		//verify( ti->m_Done.Wait() );
		// if error duiring startup, shut things down
		/*
		if ( !ti->m_Started) 
		{
			check(0);
			//verify( ti->Wait() );
			::CloseHandle( ti->m_Handle );
			ti->m_Handle = NULL;
			return -1;
		}
		*/
	} 
	else return -1;
	// we started , no need to reset event
	return 0;
}

//
// SingleLock

iSingleLock::iSingleLock( iSyncObject* syncObject, bool initialLock )
{
	check( syncObject );
	m_SyncObject = syncObject;
	m_Acquired = false;
	if ( initialLock )	Lock();
}

iSingleLock::~iSingleLock()
{
	Unlock();
}

bool iSingleLock::Lock( uint32 timeout )
{
	check( m_SyncObject != NULL );
	m_Acquired =  m_SyncObject->Lock( timeout );
	return m_Acquired;
}

bool iSingleLock::Unlock()
{
	check( m_SyncObject != NULL );
	if ( m_Acquired ) m_Acquired = ! m_SyncObject->Unlock();
	// success means it isn't acquired
	return !m_Acquired;
}

bool iSingleLock::Unlock( uint32 count )
{
	check( m_SyncObject != NULL );
	if ( m_Acquired ) m_Acquired = ! m_SyncObject->Unlock( count );
	return !m_Acquired;
}

//
// MultiLock

iMultiLock::iMultiLock( iSyncObject** syncObjects, uint32 numObjects, bool initialLock )
{
	check( numObjects > 0 && numObjects <= MULTILOCK_HANDLES );
	check( syncObjects != NULL );
	m_Objects		= syncObjects;
	m_Handle		= new HANDLE[numObjects];
	m_Acquired		= new bool[numObjects];
	m_NumObjects	= numObjects;

	for( uint32 i =0; i < numObjects; i++ ) 
	{
		check( m_Objects[i] );
		m_Handle[i] = m_Objects[i]->operator HANDLE();
		m_Acquired[i] = FALSE;
	}
	// acquire all sync
	if ( initialLock ) 	Lock();
}

iMultiLock::~iMultiLock()
{
	Unlock();
	delete[] m_Handle;
	delete[] m_Acquired;
}

iMultiLock::Result iMultiLock::Lock( uint32 timeout, bool all )
{
	Result	retval;
	uint32	retobj;
	uint32 res = ::WaitForMultipleObjects( m_NumObjects, m_Handle, all, timeout );
	if ( res == WAIT_TIMEOUT ) 
	{
		return TIMEOUT;
	} 
	else if ( /*res >= WAIT_OBJECT_0 &&*/ res < (WAIT_OBJECT_0 + m_NumObjects) ) 
	{
		retval = OBJECT;
		retobj = (res - WAIT_OBJECT_0);
	} 
	else if ( res >= WAIT_ABANDONED_0 && res < (WAIT_ABANDONED_0 + m_NumObjects) ) 
	{
		retval = ABANDONED;
		retobj = (res - WAIT_ABANDONED_0);
	} 
	else return FAILED;
	
	if ( retval == OBJECT ) 
	{
		if ( all ) 
		{
			for( DWORD i = 0; i < m_NumObjects; i++ ) 
				m_Acquired[i] = true;
		} 
		else 
		{
			check( retobj <= m_NumObjects );
			m_Acquired[ retobj ] = true;
		}
	}
	return (Result)((uint32)retobj + retval);
}

bool iMultiLock::Unlock()
{
	for( DWORD i = 0; i < m_NumObjects; i++ ) 
	{
		if ( m_Acquired[i] ) 
		{
			m_Acquired[i] = !m_Objects[i]->Unlock();
		}
	}
	return true;
}

bool iMultiLock::Unlock( uint32 count )
{
	bool gotOne = false;
	for( uint32 i = 0; i < m_NumObjects; i++ ) 
	{
		if ( m_Acquired[i] ) 
		{
			iSemaphore* semaphore = (iSemaphore*)m_Objects[i];
			if ( semaphore != NULL ) 
			{
				gotOne = true;
				m_Acquired[i] = !semaphore->Unlock( count );
			}
		}
	}
	return gotOne;
}
