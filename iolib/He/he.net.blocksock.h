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
 *	@brief Network Socket wrapper class implementation (blocking)
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_NETWORK_BLOCKING_SOCKETS_H_
#define _IOLIB_HELIUM_NETWORK_BLOCKING_SOCKETS_H_

#include "he.net.address.h"

/// Network Socket wrapper class (blocking)
class iBlockingSocket
{
public:
	/// Default constructor
	iBlockingSocket()
	: m_hSocket(0)
	{
	}

	/// Destructor
	~iBlockingSocket()
	{
	}

	/// Closes socket and cleanups
	void Cleanup()
	{
		if(m_hSocket == 0) return;
		verify(closesocket(m_hSocket) != SOCKET_ERROR);
		m_hSocket = 0;
	}

	/// Creates socket of specified type
	bool Create(int nType = SOCK_STREAM)
	{
		check(m_hSocket == 0);
		if((m_hSocket = socket(AF_INET, nType, 0)) == INVALID_SOCKET) 
		{
			return false;
		}
		return true;
	}

	/// Closes socket
	bool Close()
	{
		if (m_hSocket == 0) return true;

		if(closesocket(m_hSocket) == SOCKET_ERROR) 
		{
			return false;
		}
		m_hSocket = 0;
		return true;
	}

	/// Associates a local address with a socket
	bool Bind(const iSocketAddress &saddr)
	{
		check(m_hSocket != 0);
		if(bind(m_hSocket, saddr, sizeof(SOCKADDR)) == SOCKET_ERROR) 
		{
			return false;
		}
		return true;
	}

	/// Prepares a socket to listen for incoming connections
	bool Listen()
	{
		check(m_hSocket != 0);
		if(listen(m_hSocket, 5) == SOCKET_ERROR) 
		{
			return false;
		}
		return true;
	}

	/// 
	bool Connect(const iSocketAddress &saddr)
	{
		check(m_hSocket != 0);

		if(connect(m_hSocket, saddr, sizeof(SOCKADDR)) == SOCKET_ERROR) 
		{
			return false;
		}
		return true;
	}

	/// Establishes a connection to a peer
	bool Accept(iBlockingSocket& s, iSocketAddress &saddr)
	{
		check(m_hSocket != 0);
		check(s.m_hSocket == NULL);

		sint32 nLengthAddr = sizeof(SOCKADDR);
		s.m_hSocket = WSAAccept(m_hSocket, saddr, (int*)&nLengthAddr,NULL,0);

		if(s == INVALID_SOCKET) 
		{
			return false;
		}
		return true;
	}

	/// Sends data on a connected socket
	sint32 Send(const uint8 *buff, uint32 buffsiz, const int nSecs)
	{
		check(m_hSocket != 0);
		// returned value will be less than nSize if client cancels the reading
		FD_SET fd = {1, m_hSocket};
		TIMEVAL tv = {nSecs, 0};
		if(select(0, NULL, &fd, NULL, &tv) == 0) 
		{
			return SOCKET_ERROR;
		}
		int nBytesSent;

		if((nBytesSent = send(m_hSocket, buff, buffsiz, 0)) == SOCKET_ERROR) 
		{
			return SOCKET_ERROR;
		}

		return nBytesSent;
	}

	/// Sends outgoing data to a connected stream or datagram socket
	uint32 Write(const uint8 *buff, uint32 buffsiz, const int nSecs)
	{
		sint32 nBytesThisTime;
		const uint8 *nbuff = buff;
		sint32 nBytesLeft = buffsiz;

		do 
		{
			nBytesThisTime = Send(nbuff, nBytesLeft, nSecs);
			if (nBytesThisTime == SOCKET_ERROR) return SOCKET_ERROR;

			nBytesLeft-=nBytesThisTime;
			nbuff+=nBytesThisTime;
		} 
		while(nBytesLeft > 0);

		return buffsiz - nBytesLeft;
	}

	/// receives data from a connected or bound socket
	int Receive(uint8 *buff, uint32 buffsiz, const int nSecs)
	{
		check(m_hSocket != 0);

//		FD_SET fd = {1, m_hSocket};
//		TIMEVAL tv = {nSecs, 0};

		int nBytesReceived;
		if((nBytesReceived = recv(m_hSocket, buff, buffsiz, 0)) == SOCKET_ERROR) 
		{
			return SOCKET_ERROR;
		}
		return nBytesReceived;
	}

	/// Returns the amount of data pending in the network's input buffer that can be read from socket
	uint32 GetInBufferBytes()
	{
		check(m_hSocket != 0);
		unsigned long bavl = 0;
		if( ioctlsocket(m_hSocket,FIONREAD,&bavl) == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		return bavl;
	}

	/// sends datagram to a specific destination
	int SendDatagram(const uint8 *buff, uint32 buffsiz, const iSocketAddress &saddr,	const sint32 nSecs)
	{
		check(m_hSocket != 0);

		FD_SET fd = {1, m_hSocket};
		TIMEVAL tv = {nSecs, 0};

		if(select(0, NULL, &fd, NULL, &tv) == 0) {
			// timeout
			return SOCKET_ERROR;
		}

		int nBytesSent = sendto(m_hSocket, buff, buffsiz, 0, saddr, sizeof(SOCKADDR));
		if(nBytesSent == SOCKET_ERROR) 
		{
			return SOCKET_ERROR;
		}
		return nBytesSent;
	}

	/// Receives a datagram and stores the source address
	int ReceiveDatagram(uint8 *buff, uint32 buffsiz, iSocketAddress &saddr, const int nSecs)
	{
		check(m_hSocket != 0);

		FD_SET fd = {1, m_hSocket};
		TIMEVAL tv = {nSecs, 0};

		if(select(0, &fd, NULL, NULL, &tv) == 0) 
		{
			// Timeout
			return SOCKET_ERROR;
		}

		// input buffer should be big enough for the entire datagram
		int nFromSize = sizeof(SOCKADDR);
		int nBytesReceived = recvfrom(m_hSocket, buff, buffsiz, 0, saddr, &nFromSize);
		if(nBytesReceived == SOCKET_ERROR) 
		{
			return SOCKET_ERROR;
		}
		return nBytesReceived;
	}

	/// Gets the peer address to which a socket is connected
	bool GetPeerAddr(iSocketAddress &saddr)
	{
		check(m_hSocket != 0);
		int nLengthAddr = sizeof(SOCKADDR);
		if(getpeername(m_hSocket, saddr, &nLengthAddr) == SOCKET_ERROR) 
		{
			return false;
		}
		return true;
	}

	/// Gets the local address for a socket
	bool GetSockAddr(iSocketAddress &saddr)
	{
		check(m_hSocket != 0);
		int nLengthAddr = sizeof(SOCKADDR);
		if(getsockname(m_hSocket, saddr, &nLengthAddr) == SOCKET_ERROR) 
		{
			return false;
		}
		return true;
	}

	/// Returns handle to Socket
	operator SOCKET()
	{ 
		return m_hSocket; 
	}


private:
	SOCKET	m_hSocket;
};

///  Initiates use of Ws2_32.dll by a process
inline bool InitWinSockets()
{
	uint16 wVersionRequested;
	WSADATA wsaData;
	int err;
 
	wVersionRequested = MAKEWORD( 2, 2 );
 
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return false;
	}

	if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) {
		WSACleanup( );
		return false;
	}
	return true;
}

/// Terminates use of the Ws2_32.dll
inline void ReleaseWinSockets()
{
	WSACleanup();
}

#endif //_IOLIB_HELIUM_NETWORK_BLOCKING_SOCKETS_H_
/** @} */ // iolib_he
