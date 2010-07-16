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
 *	@brief Low level image manipulation handling
 */
/** @addtogroup iolib_xe
 *  @{
 */
#ifndef _IOLIB_XENON_DIB_LOWLEVELPROC_H_
#define _IOLIB_XENON_DIB_LOWLEVELPROC_H_

/// Fill opaque block function prototype
extern void (*FillBlockOpaque)  (uint32 *dst, const uint32 src, uint32 size, void *lpParam);
/// Fill opaque block function (standard implementation)
extern void FillBlockOpaqueStd  (uint32 *dst, const uint32 src, uint32 size, void *lpParam);
/// Fill opaque block function (initialization)
extern void FillBlockOpaqueInit  (uint32 *dst, const uint32 src, uint32 size, void *lpParam);

/// Fill alpha block function prototype
extern void (*FillBlockAlpha)  (uint32 *dst, const uint32 src, uint32 size, void *lpParam);
/// Fill alpha block function (standard implementation)
extern void FillBlockAlphaStd  (uint32 *dst, const uint32 src, uint32 size, void *lpParam);
/// Fill alpha block function (with MMX optimizization)
extern void FillBlockAlphaMMX  (uint32 *dst, const uint32 src, uint32 size, void *lpParam);
/// Fill alpha block function (with SSE optimizization)
extern void FillBlockAlphaSSE  (uint32 *dst, const uint32 src, uint32 size, void *lpParam);
/// Fill alpha block function (initialization)
extern void FillBlockAlphaInit  (uint32 *dst, const uint32 src, uint32 size, void *lpParam);

/// Copy opaque block function prototype
extern void (*CopyBlockOpaque)  (uint32 *dst, const uint32 *src, uint32 size, void *lpParam);
/// Copy opaque block function (standard implementation)
extern void CopyBlockOpaqueStd  (uint32 *dst, const uint32 *src, uint32 size, void *lpParam);
/// Copy opaque block function (initialization)
extern void CopyBlockOpaqueInit (uint32 *dst, const uint32 *src, uint32 size, void *lpParam);

/// Blit alpha block function prototype
extern void (*BlitBlockAlpha)  ( uint32* dst, const uint32* src, uint32 size, void *lpParam);
/// Blit alpha block function (standard implementation)
extern void BlitBlockAlphaStd  (uint32* dst, const uint32* src, uint32 size, void *lpParam);
/// Blit alpha block function (with MMX optimizization)
extern void BlitBlockAlphaMMX  (uint32* dst, const uint32* src, uint32 size, void *lpParam);
/// Blit alpha block function (with SSE optimizization)
extern void BlitBlockAlphaSSE  (uint32* dst, const uint32* src, uint32 size, void *lpParam);
/// Blit alpha block function (initialization)
extern void BlitBlockAlphaInit (uint32* dst, const uint32* src, uint32 size, void *lpParam);

/// Blit alpha-key block function prototype
extern void (*BlitBlockAlphaKey)  ( uint32* dst, const uint32* src, uint32 size, void *lpParam);
/// Blit alpha-key block function (standard implementation)
extern void BlitBlockAlphaKeyStd  (uint32* dst, const uint32* src, uint32 size, void *lpParam);
/// Blit alpha-key block function (initialization)
extern void BlitBlockAlphaKeyInit (uint32* dst, const uint32* src, uint32 size, void *lpParam);

/// Blit color-key block function prototype
extern void (*BlitBlockCK)  ( uint32* dst, const uint32* src, uint32 size, void *lpParam);
/// Blit color-key block function (standard implementation)
extern void BlitBlockCKStd ( uint32 *dst, const uint32 *src, uint32 size, void *lpParam);
/// Blit color-key block function (initialization)
extern void BlitBlockCKInit ( uint32 *dst, const uint32 *src, uint32 size, void *lpParam);


#endif //_IOLIB_XENON_DIB_LOWLEVELPROC_H_
/** @} */ // iolib_xe
