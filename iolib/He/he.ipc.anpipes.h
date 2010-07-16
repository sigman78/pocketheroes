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
 *	@brief Anonymous pipe wrapper class implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_IPC_ANONYMOUS_PIPE_H_
#define _IOLIB_HELIUM_IPC_ANONYMOUS_PIPE_H_

/// Anonymous pipe wrapper class
class iAnonymousPipe
{
public:
	/// Default constructor
	iAnonymousPipe()
	{
		m_hWrite = INVALID_HANDLE_VALUE;
		m_hRead = INVALID_HANDLE_VALUE;
	}

	/// Destructor
	~iAnonymousPipe()
	{
		Close();
	}

	/// Creates an anonymous pipe, and stores handles to the read and write ends of the pipe
	bool Create(LPSECURITY_ATTRIBUTES lpPipeAttributes = NULL, uint32 dwSize = 0)
	{
		check(!IsOpen());
		return ::CreatePipe(&m_hRead, &m_hWrite, lpPipeAttributes, dwSize) ? true : false;
	}


	/// Writes data to the pipe
	bool Write(const void *pBuffer, uint32 dwNumberOfBytesToWrite, uint32& dwNumberOfBytesWritten)
	{
		check(m_hWrite != INVALID_HANDLE_VALUE);
		return ::WriteFile(m_hWrite, pBuffer, dwNumberOfBytesToWrite, (unsigned long*)&dwNumberOfBytesWritten, NULL) ? true : false;
	}

	/// Reads data from the pipe
	bool Read(void *pBuffer, uint32 dwNumberOfBytesToRead, uint32& dwNumberOfBytesRead)
	{
		check(m_hRead != INVALID_HANDLE_VALUE);
		return ::ReadFile(m_hRead, pBuffer, dwNumberOfBytesToRead, (unsigned long*)&dwNumberOfBytesRead, NULL) ? true : false;
	}

	/// Copies data from anonymous pipe into a buffer without removing it from the pipe
	bool Peek(void *pBuffer, uint32 dwBufferSize, uint32& dwBytesRead, uint32& dwTotalBytesAvail, uint32& dwBytesLeftThisMessage)
	{
		check(m_hRead != INVALID_HANDLE_VALUE);
		return ::PeekNamedPipe(m_hRead, pBuffer, dwBufferSize, (unsigned long*)&dwBytesRead, (unsigned long*)&dwTotalBytesAvail, (unsigned long*)&dwBytesLeftThisMessage) ? true : false;
	}

	/// Closes handles and cleanups
	bool Close()
	{
		if (IsOpen()) {
			bool result1 = ::CloseHandle(m_hWrite) ? true : false;
			bool result2 = ::CloseHandle(m_hRead) ? true : false;

			return result1 && result2;
		}
		else return true;
	}

	/// Validates pipe's status
	bool IsOpen() const
	{ 
		return (m_hWrite != INVALID_HANDLE_VALUE); 
	};


private:
	HANDLE m_hWrite;
	HANDLE m_hRead;

};

#endif //_IOLIB_HELIUM_IPC_ANONYMOUS_PIPES_H_
/** @} */ // iolib_he
