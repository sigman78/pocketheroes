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
 *	@brief Named pipe wrapper class implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_IPC_NAMED_PIPE_H_
#define _IOLIB_HELIUM_IPC_NAMED_PIPE_H_

#include "he.std.h"
#include "he.string.h"

/// Named pipe wrapper class
class iNamedPipe
{
public:
	/// Default constructor
	iNamedPipe()
	{
		m_hPipe = INVALID_HANDLE_VALUE;
	}

	/// Destructor
	~iNamedPipe()
	{
		Close();
	}

	/// Creates an instance of a named pipe and stores a handle for subsequent pipe operations
	bool Create(const iStringT &name, uint32 dwOpenMode, uint32 dwPipeMode, uint32 dwMaxInstances, uint32 dwOutBufferSize, uint32 dwInBufferSize, uint32 dwDefaultTimeOut, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL)
	{
		check(!IsOpen());

		//the class encapsulates creating the pipe name, all that is required is
		//a simple name for the mailslot e.g. name = PJPIPE will create the pipe
		//name \\.\pipe\PJPIPE
		iStringT pipeName;
		pipeName.Setf(_T("\\\\.\\PIPE\\%s"),name.CStr());

		m_hPipe = ::CreateNamedPipe(pipeName.CStr(), dwOpenMode, dwPipeMode, dwMaxInstances, dwOutBufferSize, dwInBufferSize, dwDefaultTimeOut, lpSecurityAttributes);

		return (m_hPipe != INVALID_HANDLE_VALUE);
	}


	/// Opens instance of named pipe
	bool Open(const iStringT &server, const iStringT &name, uint32 dwDesiredAccess, uint32 dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, uint32 dwFlagsAndAttributes = 0)
	{
		check(!IsOpen());

		iStringT pipeName;
		pipeName.Setf(_T("\\\\%s\\PIPE\\%s"),server.CStr(),name.CStr());

		m_hPipe = CreateFile(pipeName.CStr(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, OPEN_EXISTING, dwFlagsAndAttributes, NULL);

		return (m_hPipe != INVALID_HANDLE_VALUE);
	}
	
	/// Returns handle to the named pipe
	operator HANDLE() const 
	{ 
		return m_hPipe; 
	}

	/// Closes handles and cleanups
	bool Close()
	{
		if (IsOpen())
		{
			return ::CloseHandle(m_hPipe) ? true : false;
		}
		else return true;
	}

	/// Attaches new pipe handle
	bool Attach(HANDLE hPipe)
	{
		if (m_hPipe != hPipe) Close();
		m_hPipe = hPipe;
		return true;
	}

	/// Dettaches current pipe handle
	HANDLE Detach()
	{
		HANDLE hReturn = m_hPipe;
		m_hPipe = INVALID_HANDLE_VALUE;
		return hReturn;
	}

	/// Enables a named pipe server process to wait for a client process to connect to an instance of a named pipe
	bool ConnectClient(LPOVERLAPPED lpOverlapped = NULL)
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);
		return ::ConnectNamedPipe(m_hPipe, lpOverlapped) ? true : false;
	}

	/// Disconnects the server end of a named pipe instance from a client process
	bool DisconnectClient()
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);
		return ::DisconnectNamedPipe(m_hPipe) ? true : false;
	}

	/// clears the buffers for the pipe and causes all buffered data to be written to the pipe
	bool Flush()
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);
		return ::FlushFileBuffers(m_hPipe) ? true : false;
	}

	/// Writes data to the pipe
	bool Write(const void *pBuffer, uint32 dwNumberOfBytesToWrite, uint32 &dwNumberOfBytesWritten, LPOVERLAPPED lpOverlapped = NULL)
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);
		return ::WriteFile(m_hPipe, pBuffer, dwNumberOfBytesToWrite, (unsigned long*)&dwNumberOfBytesWritten, lpOverlapped) ? true : false;
	}

	/// Writes data to the pipe
	bool Write(const void *pBuffer, uint32 dwNumberOfBytesToWrite, LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);
		return ::WriteFileEx(m_hPipe, pBuffer, dwNumberOfBytesToWrite, lpOverlapped, lpCompletionRoutine) ? true : false;
	}

	/// Reads data from the pipe
	bool Read(void *pBuffer, uint32 dwNumberOfBytesToRead, uint32& dwNumberOfBytesRead, LPOVERLAPPED lpOverlapped = NULL)
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);
		return ::ReadFile(m_hPipe, pBuffer, dwNumberOfBytesToRead, (unsigned long*)&dwNumberOfBytesRead, lpOverlapped) ? true : false;
	}

	/// Reads data from the pipe
	bool Read(void *pBuffer, uint32 dwNumberOfBytesToRead, LPOVERLAPPED lpOverlapped,  LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);
		return ::ReadFileEx(m_hPipe, pBuffer, dwNumberOfBytesToRead, lpOverlapped, lpCompletionRoutine) ? true : false;
	}

	/// Copies data from named pipe into a buffer without removing it from the pipe
	bool Peek(void *pBuffer, uint32 dwBufferSize, uint32& dwBytesRead, uint32& dwTotalBytesAvail, uint32& dwBytesLeftThisMessage)
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);
		return ::PeekNamedPipe(m_hPipe, pBuffer, dwBufferSize, (unsigned long*)&dwBytesRead, (unsigned long*)&dwTotalBytesAvail, (unsigned long*)&dwBytesLeftThisMessage) ? true : false;
	}

	/// Combines the functions that write a message to and read a message from the specified named pipe into a single network operation
	bool Transact(void *pInBuffer, uint32 dwInBufferSize, void *pOutBuffer,	uint32 dwOutBufferSize, DWORD& dwBytesRead, LPOVERLAPPED lpOverlapped = NULL)
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);
		return ::TransactNamedPipe(m_hPipe, pInBuffer, dwInBufferSize, pOutBuffer, dwOutBufferSize, (unsigned long*)&dwBytesRead, lpOverlapped) ? true : false;
	}

	/// Validates pipe's status
	bool IsOpen() const 
	{ 
		return (m_hPipe != INVALID_HANDLE_VALUE); 
	}

	/// Returns - is this blocking pipe or not
	bool IsBlockingPipe(bool& bIsBlocking) const
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);

		uint32 dwState;
		if (::GetNamedPipeHandleState(m_hPipe, (unsigned long*)&dwState, NULL, NULL, NULL, NULL, 0))
		{
		    bIsBlocking = ((dwState & PIPE_NOWAIT) == 0);
			return true;
		}
		else return false;
	}

	/// Returns - is this client pipe or not
	bool IsClientPipe(bool& bClientPipe) const
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);

		uint32 dwFlags;
		if (::GetNamedPipeInfo(m_hPipe, (unsigned long*)&dwFlags, NULL, NULL, NULL))
		{
		    bClientPipe = ((dwFlags & PIPE_CLIENT_END) != 0);
			return true;
		}
		else return false;
	}

	/// Returns - is this server pipe or not
	bool IsServerPipe(bool& bServerPipe) const
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);

		uint32 dwFlags;
		if (::GetNamedPipeInfo(m_hPipe, (unsigned long*)&dwFlags, NULL, NULL, NULL))
		{
		    bServerPipe = ((dwFlags & PIPE_SERVER_END) != 0);
			return true;
		}
		else return false;
	}

	/// Returns - is this message pipe or not
	bool IsMessagePipe(bool& bMessagePipe) const
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);

		uint32 dwState;
		if (::GetNamedPipeHandleState(m_hPipe, (unsigned long*)&dwState, NULL, NULL, NULL, NULL, 0))
		{
		    bMessagePipe = ((dwState & PIPE_READMODE_MESSAGE) != 0);
			return true;
		}
		else return false;
	}

	/// Returns number of current pipe instances
	uint32 GetCurrentInstances() const
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);

		uint32 dwCurInstances = 0;
		::GetNamedPipeHandleState(m_hPipe, NULL, (unsigned long*)&dwCurInstances, NULL, NULL, NULL, 0);

		return dwCurInstances;
	}

	/// Returns the maximum number of bytes to be collected on the client's computer before transmission to the server
	uint32 GetMaxCollectionCount() const
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);

		uint32 dwMaxCollectionCount = 0;
		::GetNamedPipeHandleState(m_hPipe, NULL, NULL, (unsigned long*)&dwMaxCollectionCount, NULL, NULL, 0);

		return dwMaxCollectionCount;
	}

	/// Returns  the maximum time, in milliseconds, that can pass before a remote named pipe transfers information over the network
	uint32 GetCollectionTimeout() const
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);

		uint32 dwCollectDataTimeout = 0;
		::GetNamedPipeHandleState(m_hPipe, NULL, NULL, NULL, (unsigned long*)&dwCollectDataTimeout, NULL, 0);

		return dwCollectDataTimeout;
	}

	/// Returns the size, in bytes, of the buffer for outgoing data. If the buffer size is zero, the buffer is allocated as needed
	uint32 GetOutboundBufferSize() const
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);

		uint32 dwOutBufferSize = 0;
		::GetNamedPipeInfo(m_hPipe, NULL, (unsigned long*)&dwOutBufferSize, NULL, NULL);

		return dwOutBufferSize;
	}

	/// Returns the size, in bytes, of the buffer for incoming data. 
	/// If the buffer size is zero, the buffer is allocated as needed
	uint32 GetInboundBufferSize() const
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);

		uint32 dwInBufferSize = 0;
		::GetNamedPipeInfo(m_hPipe, NULL, NULL, (unsigned long*)&dwInBufferSize, NULL);

		return dwInBufferSize;
	}

	/// Returns the user name string associated with the client application. 
	/// The server can only retrieve this information if the client opened the pipe with SECURITY_IMPERSONATION access
	iStringT GetClientUserName() const
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);

		iCharT buffer[255]={0};
		::GetNamedPipeHandleState(m_hPipe, NULL, NULL, NULL, NULL, buffer, 255);

		return iStringT(buffer);
	}

	/// Returns the maximum number of pipe instances that can be created. 
	/// If the variable is set to PIPE_UNLIMITED_INSTANCES, the number of pipe instances that can be created is limited only by the availability of system resources
	uint32 GetMaxInstances() const
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);

		uint32 dwMaxInstances = 0;
		::GetNamedPipeInfo(m_hPipe, NULL, NULL, NULL, (unsigned long*)&dwMaxInstances);

		return dwMaxInstances;
	}

	/// Sets the read mode and the blocking mode of the specified named pipe. 
	/// If the specified handle is to the client end of a named pipe and if the named pipe server process is on a remote computer, the function can also be used to control local buffering
	bool SetMode(bool bByteMode, bool bBlockingMode)
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);

		uint32 dwMode;
		if (bByteMode) {
			if (bBlockingMode) dwMode = PIPE_READMODE_BYTE | PIPE_WAIT;
			else dwMode = PIPE_READMODE_BYTE | PIPE_NOWAIT;
		} else {
			if (bBlockingMode) dwMode = PIPE_READMODE_MESSAGE | PIPE_WAIT;
			else dwMode = PIPE_READMODE_MESSAGE | PIPE_NOWAIT;
		}
		return ::SetNamedPipeHandleState(m_hPipe, (unsigned long*)&dwMode, NULL, NULL) ? true : false;
	}

	/// Sets the maximum number of bytes collected on the client computer before transmission to the server
	bool SetMaxCollectionCount(uint32 dwCollectionCount)
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);
		return ::SetNamedPipeHandleState(m_hPipe, NULL, (unsigned long*)&dwCollectionCount, NULL) ? true : false;
	}

	/// Sets the maximum time, in milliseconds, that can pass before a remote named pipe transfers information over the network
	bool SetCollectionTimeout(uint32 dwDataTimeout)
	{
		check(m_hPipe != INVALID_HANDLE_VALUE);
		return ::SetNamedPipeHandleState(m_hPipe, NULL, NULL, (unsigned long*)&dwDataTimeout) ? true : false;
	}

	/// Connects to a message-type pipe (and waits if an instance of the pipe is not available), writes to and reads from the pipe, and then closes the pipe
	static bool Call(const iStringT &server, const iStringT &name, void *pInBuffer, uint32 dwInBufferSize, void *pOutBuffer, uint32 dwOutBufferSize, uint32& dwBytesRead, uint32 dwTimeOut)
	{
		iStringT pipeName;
		pipeName.Setf(_T("\\\\%s\\PIPE\\%s"), server.CStr(), name.CStr());
		return ::CallNamedPipe(pipeName.CStr(), pInBuffer, dwInBufferSize, pOutBuffer, dwOutBufferSize, (unsigned long*)&dwBytesRead, dwTimeOut) ? true : false;
	}

	/// Waits until either a time-out interval elapses or an instance of the specified named pipe is available for connection
	static bool ServerAvailable(const iStringT &server, const iStringT &name, uint32 dwTimeOut)
	{
		iStringT pipeName;
		pipeName.Setf(_T("\\\\%s\\PIPE\\%s"), server.CStr(), name.CStr());

		return ::WaitNamedPipe(pipeName.CStr(), dwTimeOut) ? true : false;
	}

private:
	HANDLE m_hPipe;
};

#endif //_IOLIB_HELIUM_IPC_NAMED_PIPE_H_
/** @} */ // iolib_he
