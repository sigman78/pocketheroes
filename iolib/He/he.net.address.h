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
 *	@brief Internet Protocol Adress wrapper class implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_NETWORK_ADDRESS_H_
#define _IOLIB_HELIUM_NETWORK_ADDRESS_H_

#include "he.std.h"
#include "he.string.h"

/// Internet Protocol Adress wrapper class
class iIPAddress : public IN_ADDR
{
public:
	static iIPAddress GetLocalIPAddress()
	{ }

	/// Default constructor
	iIPAddress()
	{
		s_addr = 0;
	}

	/// Constructs iIPAddress object with given DWORD value
	iIPAddress(uint32 dw)
	{
		s_addr = htonl(dw);
	}

	/// Constructs iIPAddress object with given text string value
	iIPAddress(const iStringT &ipaddr)
	{
		s_addr = inet_addr(ipaddr.CStr());
	}

	/// Copy constructor
	iIPAddress(const iIPAddress &ipaddr)
	{
		s_addr = ipaddr.s_addr;
	}

	/// Constructs iIPAddress object with given IN_ADDR struct
	iIPAddress(const IN_ADDR &inaddr)
	{
		memcpy(this,&inaddr,sizeof(IN_ADDR));
	}
	
	/// Returns current IP address as string of text (ex.: 127.0.0.1)
	iStringT GetAsString() const
	{
		return iStringT(inet_ntoa(*this));
	}

	/// Assignment operator
	const iIPAddress& operator=(const IN_ADDR &inaddr)
	{
		memcpy(this,&inaddr,sizeof(IN_ADDR));
	}

	/// Returns pointer to the IN_ADDR struct
	operator LPIN_ADDR()
	{
		return this;
	}

	/// Returns IN_ADDR struct
	operator IN_ADDR()
	{
		return (*this);
	}
};

/// Socket Adress wrapper class
class iSocketAddress : public SOCKADDR_IN
{
public:
	/// Default constructor
	iSocketAddress()
	{
		sin_family = AF_INET;
		sin_addr = iIPAddress();
		sin_port = 0;
	}


	/// Constructs iIPAddress object with given IP adress and port number
	iSocketAddress(const iIPAddress &addr, uint16 port )
	{
		sin_family = AF_INET;
		sin_addr = addr;
		sin_port = htons(port);
	}

	/// Constructs iIPAddress object with given IP adress and port number
	iSocketAddress(const iStringT &addr, uint16 port )
	{
		sin_family = AF_INET;
		sin_addr = iIPAddress(addr);
		sin_port = htons(port);
	}

	/// Constructs iIPAddress object with given SOCKADDR struct
	iSocketAddress(const SOCKADDR &sa )
	{
		memcpy(this,&sa,sizeof(SOCKADDR));
	}

	/// Constructs iIPAddress object with given SOCKADDR_IN struct
	iSocketAddress(const SOCKADDR_IN &san )
	{
		memcpy(this,&san,sizeof(SOCKADDR_IN));
	}

	/// Destructor
	~iSocketAddress()
	{
	}

	/// Returns current IP address as string of text (ex.: 127.0.0.1:80)
	iStringT GetAsString() const
	{
		return iStringT(inet_ntoa(sin_addr));
	}
	
	/// Returns port number
	uint16 GetPort()
	{
		return ntohs(sin_port);
	}

	/// Returns IP address
	iIPAddress GetIPAddress()
	{
		return iIPAddress(this->sin_addr);
	}

	/// Returns pointer to the SOCKADDR struct
	operator const LPSOCKADDR() const
	{
		return ((SOCKADDR*)this);
	}

	/// Returns pointer to the SOCKADDR_IN struct
	operator const LPSOCKADDR_IN() const
	{
		return ((SOCKADDR_IN*)this);
	}

	/// Returns the SOCKADDR struct
	operator SOCKADDR() const
	{
		return *((SOCKADDR*)this);
	}

	/// Returns the SOCKADDR_IN struct
	operator SOCKADDR_IN() const
	{
		return *((SOCKADDR_IN*)this);
	}
};

/// Local host information wrapper class
class iLocalHostInfo
{
public:

	/// Returns local host name
	static iStringT GetLocalHostName()
	{
		WORD wVersionRequested;
		WSADATA wsaData;
		char name[255];
		PHOSTENT hostinfo;
		wVersionRequested = MAKEWORD( 2, 0 );

		if ( WSAStartup( wVersionRequested, &wsaData ) == 0 )
		{

			if( gethostname ( name, sizeof(name)) == 0)
			{
				  if((hostinfo = gethostbyname(name)) != NULL)
				  {
				  }
			}
    
			WSACleanup( );
		} 

		return iStringT(name);
	}
};


#endif //_IOLIB_HELIUM_NETWORK_ADDRESS_H_
/** @} */ // iolib_he
