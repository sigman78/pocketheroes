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
 *	@brief Multithreading synchronization handling wrappers implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_THREADS_H_
#define _IOLIB_HELIUM_THREADS_H_

#include "he.std.h"
// Const
const uint32 MULTILOCK_HANDLES	=	4;

// Thread exec. function decl

typedef INT (CDECL* ThreadFunction)(void*);

/// Base syncronization object class
class iSyncObject
{
public:
	/// Destructor
	virtual ~iSyncObject();

	/// Implicit return handle
	virtual inline operator HANDLE ();

	/// Wait for access for this object. 
	/// Return if successful in timeout specified. 
	/// Zero timeout checks and returns w/o delay Lock and Wait are equivalent.
	virtual	bool Lock( uint32 timeout = INFINITE );
	/// Wait for access for this object. 
	inline	bool Wait( uint32 timeout = INFINITE );

	/// Unlock access to this object.
	virtual bool Unlock();
	/// Unlock access to this object.
	inline	bool Signal();

	/// Unlock access to this object, returns true if successful.
	virtual bool Unlock( uint32 count );

	/// Tests for lock
	virtual bool IsLocked();

protected:
	HANDLE			m_Handle;		// Handle for sync object
	const iCharT*	m_Name;			// Name of the object

	/// diagnostic
	friend class iSingleLock;

	/// Create sync object with it's name
	iSyncObject( const iCharT* aname );
	/// Create nameless sync object
	iSyncObject();
};


///Critical section - faster single process mutex
class iCriticalSection : public iSyncObject
{
public:
	/// Create a new critical section
	iCriticalSection( const iCharT* name = NULL );

	/// Cleanup c/s
	virtual inline ~iCriticalSection();

	/// Aquire lock to the c/s
	virtual bool Lock( uint32 timeout = INFINITE );

	/// Release lock
	virtual bool Unlock();

	/// Release lock
	virtual bool Unlock( uint32 count );

	/// Tests for lock
	virtual bool IsLocked();

	/// Sets the spin count for the c/s
	uint32	SetSpinCount( uint32 spincount );
	
private:
	CRITICAL_SECTION m_CritSec;
};

/// @brief Mutex - only one thread at a time can own the mutex. If the thread trys	to lock mutex that already owned by another thread it will be block	( for the timeout ) until owning thread unlock the mutex.
///	Mutexes are typically used to protect shared resources that can be accessed only by one thread of execution.
class iMutex : public iSyncObject
{
public:
	/// Constructs mutex with optional identifier
	iMutex( bool owned = false, const iCharT* ID = NULL );

	/// Destructor
	virtual inline ~iMutex();

	/// Unlock access to the object
	virtual bool Unlock();

	/// Unlock access to the object
	virtual bool Unlock( uint32 count );
};

/// @brief More than one thread can lock a semaphore class at the same time.
/// If the thread attempts to lock the semaphore and the maximum number	of locks has been reached it will block ( for the timeout ) until one of the owners unlocks the semaphore.
///	Semaphores are typically used to protect shared resources that can be accessed by limited number threads of execution at the same time.
class iSemaphore : public iSyncObject
{
public:
	/// Constructs semaphore with initial and max counts
	iSemaphore( uint32 initial = 0, uint32 max = 1, const iCharT* id = NULL );

	/// Destructor
	virtual inline ~iSemaphore();

	/// Unlock
	virtual bool Unlock();

	/// Unlock
	virtual bool Unlock( uint32 count );
};

/// @brief A WEvent object exists in two states - signaled and non-signaled.
///	When signaled threads attempting to lock this object do not block -	otherwise the threads are blocked until the object changes to the signaled state.
class iWEvent : public iSyncObject
{
public:
	/// Constructor
	iWEvent( bool manual = false, bool initial = false, const iCharT* id = NULL );

	/// Destructor
	virtual inline ~iWEvent();

	/// Unlocks the access to this object
	virtual bool Unlock();

	/// Set the events as signaled
	void Set();

	/// Pulse(signal) the event, releasing waiting threads and then return to the non-signaled state.
	void Pulse();

	/// Reset to the non-signaled state
	void Reset();
};

/// This object allows multithreading execution.
class iThread : public iSyncObject
{
public:

	/// Priority classes enumeration
	enum Priority {
		TIME_CRITICAL	= 15,	
		HIGHEST			= 2,
		ABOVE_NORMAL	= 1,
		NORMAL			= 0,
		BELOW_NORMAL	= -1,
		LOWEST			= -2,
		IDLE			= -15
	};

	/// Constructor 
	iThread( ThreadFunction func, void* arg = NULL );
	/// Destructor
	virtual ~iThread();

	/// Implicit return the handle for this object
	virtual inline operator HANDLE ();

	/// Get the priority
	Priority GetPriority() const;
	/// Set the priority
	void SetPriority( Priority priority );

	/// Returns true if the thread signaled to stop.
	bool IsDone() const;

	/// Returns true if the thread are alive. FALSE if terminated
	bool IsAlive() const;

	/// Returns true if the thread started ok
	bool IsStartedOk() const;

	/// Suspend execution of thread
	bool Suspend();

	/// Resumes execution of thread
	bool Resume();

	/// Terminate execution of thread. Returns result
	uint32 Terminate();

	/// Disable termination then destructed
	inline void	DisableTermination();

	/// End this thread with specified result
	void End( uint32 result = 0 );

	/// Returns the id of current thread
	static uint32 GetCurrentId();

	/// Returns the id of the process
	static uint32 GetProcessId();

	/// Pass buffer to hold process name
	static void GetProcessName( iCharT* name );

	/// Pass the PID of the process to test
	static bool IsProcessAlive( uint32 pid );

private:
	iWEvent			m_Done;			// Event to signal completion of thread
	bool			m_Terminate;	// True if thread termination requested
	ThreadFunction	m_Function;		// function to execute
	void*			m_Data;
	bool			m_Started;		// thread started ok

	/// Start the thread
	static UINT	__stdcall Start( void* threadInfo );

	/// An exception wrapped start function that calls the user's thread func
	static INT	__cdecl SafeStart( void* threadInfo );
};

/// @brief Single lock object contains only one sync object so it may be locked by one thread at a time. \
/// If another thread attemps to lock an object that has already been locked by another thread, that requesting thread will block until the owning thread unlocks the object.
///	WARNING: Can't wait for critical sections !
class iSingleLock
{
public:
	/// Constructs the single lock object from synctonization object, that cannot be NULL. 
	/// If initialLock is true the object is locked then it constructed.
	iSingleLock( iSyncObject* syncObject, bool initalLock = false );

	/// Destruct this single lock. If it has a lock on a syncronization object it will be released
	~iSingleLock();

	/// Lock the sync object. Returns false if not successful
	bool Lock( uint32 timeout = INFINITE );

	/// Unlocks the sync object. Retunrs false if not successful
	bool Unlock();

	/// Unlocks the sync object for specified count which must be greather than zero. Returns false if not successful.
	bool Unlock( uint32 count );

	/// Test the syncronization object for the locked state.
	inline bool IsLocked() const;
private:

	iSyncObject*	m_SyncObject;	// Locking object
	bool			m_Acquired;		// true if already locked
};

/// @brief Multi-lock object contains more the one sync object. 
/// When a threads requests a lock, the first available one is returned. If none are available, the task blocks - for the given timeout. Alternatively a thread can requests all locks at once.
///	WARNING: Can't wait for critical sections !
class iMultiLock
{
public:
	/// Result type enumeration
	enum Result {
		FAILED		= 0,
		TIMEOUT,
		OBJECT		= 0x10000000,
		ABANDONED	= 0x20000000
	};

	/// @brief Constructs a multi-lock from the specified syncronizaion objects that cannot be NULL. 
	/// If initialLock is true the object is locked when this is constructed.
	/// NOTE: syncObjects must live for the lifetime of multilock.
	iMultiLock( iSyncObject** syncObjects, uint32 numObjects, bool initialLock = false );

	/// Destruct this multilock. If it has a lock on a sync object it will be released.
	~iMultiLock();

	/// @brief Locks the syncronization object. 
	/// Returns (FAILED) if lock failed. Returns (OBJECT) to (OBJECT+num-1) an index into the array of objects, indicating which object satisfied the wait. ( if more then one - smallest index returned )
	Result Lock( uint32 timeout = INFINITE, bool all = false );

	/// Unlocks the syncronization object. Always return true.
	bool Unlock();

	/// Unlocks the syncronizaion object for specified count which must be greather than zero. Returns false if not successful
	bool Unlock( uint32 count );

	/// Test the sync object at the specified index for the locked state. Returns true if it is locked.
	inline bool IsLocked( uint32 index ) const;

private:

	HANDLE*			m_Handle;			// Internal handle
	bool*			m_Acquired;			// Array of flags to mark which s/o are locked
	uint32			m_NumObjects;
	iSyncObject**	m_Objects;
};



/////////////////////////////////////////////////////////////////////////
// Impl

// SyncObject
inline iSyncObject::operator HANDLE ()
{
	return m_Handle;
}

inline bool iSyncObject::Signal()
{
	return Unlock();
}

inline bool iSyncObject::Wait( uint32 timeout )
{
	return Lock( timeout );
}

// Mutex
inline iMutex::~iMutex()
{
}

// Semaphore
inline iSemaphore::~iSemaphore()
{
}

// WEvent
inline iWEvent::~iWEvent()
{
}

// Thread
inline iThread::operator HANDLE ()
{
	return m_Done;
}

inline void iThread::DisableTermination()
{
	m_Terminate = false;
}

// SingleLock
inline bool iSingleLock::IsLocked() const
{
	return m_Acquired;
}

// MultiLock
inline bool iMultiLock::IsLocked( uint32 index ) const
{
	return m_Acquired[index];	
}


#endif //_IOLIB_HELIUM_THREADS_H_
/** @} */ // iolib_he
