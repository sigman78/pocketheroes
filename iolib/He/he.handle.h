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
 *	@brief Generic Windows Handle wrapper class implementation
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_HANDLE_H_
#define _IOLIB_HELIUM_HANDLE_H_

/// Generic Windows Handle wrapper class
class iHandle
{
   HANDLE m_Handle;

   /// Default constructor
   iHandle(HANDLE hSrc=INVALID_HANDLE_VALUE)
   {
      m_Handle = hSrc;
   }

   /// Destructor
   ~iHandle()
   {
      Close();
   }

   /// returns HANDLE
   operator HANDLE() const 
   { 
      return m_Handle; 
   }

   /// returns pointer to Handle
   LPHANDLE operator&()
   {
      check(!IsValid());
      return &m_Handle;
   }

   /// Assignment operator
   iHandle& operator=(HANDLE h)
   {
      check(!IsValid());
      m_Handle = h;
      return *this;
   }

   /// Validate handle
   bool IsValid() const 
   { 
      return m_Handle!=INVALID_HANDLE_VALUE; 
   }

   /// Attaches handle
   void Attach(HANDLE h)
   {
      if( IsValid() ) ::CloseHandle(m_Handle);
      m_Handle = h;
   }

   /// Dettaches handle
   HANDLE Detach()
   {
      HANDLE h = m_Handle;
      m_Handle = INVALID_HANDLE_VALUE;
      return h;
   }

   /// Closes handle
   BOOL Close()
   {
      BOOL bRes = FALSE;
      if( m_Handle!=INVALID_HANDLE_VALUE ) 
	  {
         bRes = ::CloseHandle(m_Handle);
         m_Handle = INVALID_HANDLE_VALUE;
      }
      return bRes;
   }

   /// Dublicates handle
   BOOL Duplicate(HANDLE hSource, bool bInherit = false)
   {
      check(!IsValid());
      HANDLE hOurProcess = ::GetCurrentProcess();
      BOOL b = ::DuplicateHandle(hOurProcess, 
         hSource,
         hOurProcess, 
         &m_Handle,
         DUPLICATE_SAME_ACCESS,
         bInherit,
         DUPLICATE_SAME_ACCESS);
      check(b);
      return b;
   }
};

#endif //_IOLIB_HELIUM_HANDLE_H_
/** @} */ // iolib_he
