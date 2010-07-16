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

#include "stdafx.h"
#include "sprite2.h"
#include "scopeptr.h"

#include "GfxManager.h"

#include "gammatab.h"

//////////////////////////////////////////////////////////////////////////

//
// File structure:
//
// offset:    type:
// 0000			BankHeader			4 ( * uint32)		Catalog
// 0010			Data(Raw&Packed)	hdr.dataLength(b)	Raw image
// xxxx			BankImage			hdr.numSprites		Sprite props
//
// BankImage structure
//
// NOTE! Current limitation of spanned sprite file structure -
// size of pixel chunk should be less than ( sizeof(uint16) * 65535 )
// Refer to the <*1*> marked lines


#pragma pack(push,1)
// 1 + 3*4 = 13 b per sprite
struct BankImage
{
	uint8	imageType;		// type props
	uint32	dataOffset;		// offset of the sprite data from the begining of data block ( in uint16 )
	uint16	imageWidth;
	uint16	imageHeight;
	sint16	originX;
	sint16	originY;
};

struct BankHeader
{
	uint32	magicId;
	uint32	numSprites;
	uint32	dataLength;		// in bytes!!!
	uint32	reserved1_;
};
#pragma pack(pop)

//////////////////////////////////////////////////////////////////////////

/*
Exporter
	iDynamicBuffer	pixels;
	BankImageArray	catalog;

	// IN: iDib sprite, bool spanned, iPoint origin
	/////////////////////////////////////////////////////

	BankImage img;
	img.imageType	= spanned ? 1 : 0;
	img.dataOffset	= pixels.GetCurrentPos() / sizeof(uint16);
	img.imageWidth	= sprite.GetWidth();
	img.imageHeight	= sprite.GetHeight();
	img.originX		= origin.x;
	img.originY		= origin.y;

	catalog.Add( img );

	if ( spanned ) {
		iSimpleArray<uint16>	data;
		MakeSpannedSprite( sprite, data );

		check( data.GetSize() < 65536 );	// Im too lazy to split uint32 ;)

		pixels.Write( data.GetSize() );		// <*1*>
		pixels.Write( data.GetPtr(), data.GetSize() * sizeof(uint16) );
	} else {
		iSimpleArray<uint16>	data;
		MakeSprite( sprite, data );

		pixels.Write( data.GetPtr(), data.GetSize() * sizeof(uint16) );
	}

	// write file
	// iFileI* file

	// header
	file->Write( (uint32)0x1A464253 );
	file->Write( (uint32)catalog.GetSize() );
	file->Write( (uint32)pixels.GetSize() );
	file->Write( (uint32)0xffffffff );

	// pixels
	file->Write( pixels.GetData(), pixels.GetSize() );

	// catalog
	for( uint n = 0; n != catalog.GetSize(); ++n ) {
		// may need to write element-by-element if packing is a problem
		file->Write( &catalog[n], sizeof( BankImage ) );
	}

 */


// SpriteId explanation
// SpriteId is an unique sprite identifier
// It composed of Sprite Bank Identifier + Number of sprite in the bank
// SpriteID = uint32
// Upper 16 bit is a bank number
// Lower 16 bit is a sprite number
// [...BankID : SpriteNum ]

iGfxManager::SpriteId
iGfxManager::MakeSpriteId( BankId cat, uint32 sn )
{
	check( cat < MaxSpriteBanks );
	return sn + (((SpriteId)cat) << 16);
}

iGfxManager::SpriteId 
iGfxManager::GetFlippedSpriteId( SpriteId sid )
{
	return sid | (1UL<<31);
}

bool
iGfxManager::Load( BankId cat, const TCHAR* fileName, LoadMode lmode )
{
	check( cat < MaxSpriteBanks );
	if ( cat >= MaxSpriteBanks ) return false;
	bank_[cat].Unload();
	return bank_[cat].Load( fileName, lmode, gammaLevel );
}

void
iGfxManager::Unload( BankId cat )
{
	check( cat < MaxSpriteBanks );
	if ( cat >= MaxSpriteBanks ) return;
	bank_[cat].Unload();
}

void
iGfxManager::Reload()
{
	for( size_t n = 0; n != MaxSpriteBanks; ++n ) {
		if ( bank_[n].data != 0 ) {
			bank_[n].Reload( gammaLevel );
		}
	}
}

size_t
iGfxManager::NumSprites( BankId cat ) const
{
	check( cat < MaxSpriteBanks );
	if ( cat >= MaxSpriteBanks ) return false;
	
	return bank_[cat].props.GetSize();
}

iGfxManager::SpriteId
iGfxManager::FirstId( BankId cat ) const
{
	check( cat < MaxSpriteBanks );
	if ( cat >= MaxSpriteBanks ) return false;

	return MakeSpriteId( cat, 0 );
}

iGfxManager::SpriteId
iGfxManager::EndId( BankId cat ) const
{
	check( cat < MaxSpriteBanks );
	if ( cat >= MaxSpriteBanks ) return false;

	return MakeSpriteId( cat, bank_[cat].props.GetSize() );
}

iSize 
iGfxManager::Dimension( SpriteId sid ) const
{
	uint32 catId = sid >> 16;
	uint32 sprId = sid & 0xffff;

	check( catId < MaxSpriteBanks );
	check( sprId < bank_[catId].props.GetSize() );

	const Sprite* sprPtr= bank_[catId].GetSprite( sprId );

	return iSize( sprPtr->sizeW_, sprPtr->sizeH_ );
}

iPoint 
iGfxManager::Anchor( SpriteId sid ) const
{
	uint32 catId = sid >> 16;
	uint32 sprId = sid & 0xffff;

	check( catId < MaxSpriteBanks );
	check( sprId < bank_[catId].props.GetSize() );

	const Sprite* sprPtr= bank_[catId].GetSprite( sprId );

	return iPoint( sprPtr->originX_ , sprPtr->originY_);
}

uint8
iGfxManager::Shadow( SpriteId sid ) const
{
	uint32 catId = sid >> 16;
	uint32 sprId = sid & 0xffff;

	check( catId < MaxSpriteBanks );
	check( sprId < bank_[catId].props.GetSize() );

	const Sprite* sprPtr= bank_[catId].GetSprite( sprId );

	return (sprPtr->type_ & 0xE) >> 1;
}

void
iGfxManager::Blit( SpriteId sid, iDib& srf, iPoint pos ) const
{
	uint32 catId = sid >> 16;
	uint32 sprId = sid & 0xffff;

	check( catId < MaxSpriteBanks );
	check( sprId < bank_[catId].props.GetSize() );

	const Sprite* sprPtr= bank_[catId].GetSprite( sprId );
	const uint16* pixels= bank_[catId].Data( sprPtr->offset_ );
    
	iRect src_rect( 0, 0, sprPtr->sizeW_, sprPtr->sizeH_ );
	BlitNormal( sprPtr, pixels, srf, src_rect, pos );
}

void 
iGfxManager::Blit( SpriteId sid, iDib& srf, const iRect& src, iPoint pos ) const
{
	uint32 catId = sid >> 16;
	uint32 sprId = sid & 0xffff;

	check( catId < MaxSpriteBanks );
	check( sprId < bank_[catId].props.GetSize() );

	const Sprite* sprPtr= bank_[catId].GetSprite( sprId );
	const uint16* pixels= bank_[catId].Data( sprPtr->offset_ );

	BlitNormal( sprPtr, pixels, srf, src, pos );
}

void 
iGfxManager::FlipSprite(SpriteId sid)
{
	uint32 catId = sid >> 16;
	uint32 sprId = sid & 0xffff;

	check( catId < MaxSpriteBanks );
	check( sprId < bank_[catId].props.GetSize() );

	const Sprite* sprPtr= bank_[catId].GetSprite( sprId );
	const uint16* pixels= bank_[catId].Data( sprPtr->offset_ );

	if (dynItems[dynPos]) delete dynItems[dynPos];
	dynItems[dynPos] = new DynItem();
	
	if ( sprPtr->type_ & 1 ) {
		// Spanned sprite

	} else {
		// Raw sprite
		check(0);
	}

	//dynItems[dynPos].data = (uint8*)malloc(sprPtr->)
	dynPos++;
}

void 
iGfxManager::BlitNormal( const iGfxManager::Sprite* sprPtr, const uint16* pixels, iDib& srf, const iRect& src, iPoint pos ) const
{
	check( sprPtr != 0 && pixels != 0 );

	iPoint origin( sprPtr->originX_, sprPtr->originY_ );
	iSize  size( sprPtr->sizeW_, sprPtr->sizeH_ );

	// correct sprite position according to anchor
	pos += origin;

	// check bounds & clip
	// __NOT_NECESSARY__ if ( (pos.x + (sint32)size.w) <= 0 || (pos.y + (sint32)size.h) <= 0) return;
	iRect src_rect(src);
	//iSize siz( srf.GetWidth() - pos.x, srf.GetHeight() - pos.y);
	//iRect dst_rect(pos,siz);  
	//__SAME_AS__
	iRect dst_rect( pos, size );
	if (!iClipper::iClipRectRect(dst_rect,srf.GetSize(),src_rect,size)) return;

	// calc dest ptr
	uint16* dst_clr = srf.GetPtr(dst_rect);
	const size_t dstStride = srf.GetWidth();

	// blit
	if ( sprPtr->type_ & 1 ) {
		// Spanned sprite
		typedef Blitter<CopyOp> Blt;
		CopyOp op;
		// ptr, dst_clr, dstStrude, src_rect(.x .y .w, .h )

		// <*1*>
		const uint16* eptr= pixels + (size_t)*pixels + 1;	
		const uint16* ptr = pixels + 1;

		if (src_rect.size() == size){
			// sprite non clipped - use lighting fast blit
			while (ptr != eptr) {
				ptr = Blt::SpanFast( op, ptr,dst_clr );
				dst_clr += dstStride;
			}
		} else {
			// sprite clipped - full blown blit
			dst_clr -= src_rect.x;

			sint32 toSkip = src_rect.y;
			while (toSkip--) ptr = Blt::SpanSkip( ptr );

			const uint16* clipIn = dst_clr + src_rect.x;
			for (sint32 yy=0; yy<(sint32)src_rect.h; ++yy){
				ptr = Blt::Span(op, ptr, dst_clr, clipIn, clipIn + src_rect.w );
				dst_clr	+= dstStride;
				clipIn	+= dstStride;
			}
		}
	} else {
		// Raw sprite
		const uint16* src_clr = pixels + src_rect.y*size.w + src_rect.x;

		for (uint32 yy=0; yy != dst_rect.h; ++yy) {
			//BlitDibBlock_RGB(dst_clr,src_clr,dst_rect.w);
			memcpy( dst_clr, src_clr, dst_rect.w * sizeof(uint16) );
			src_clr += size.w;
			dst_clr += dstStride;
		}
	}
}

void 
iGfxManager::BlitEffect( SpriteId sid, iDib& to, iPoint pos, Effects efx) const
{
	// All other effects works only for span-sprites
	uint32 catId = sid >> 16;
	uint32 sprId = sid & 0xffff;

	check( catId < MaxSpriteBanks );
	check( sprId < bank_[catId].props.GetSize() );

	const Sprite* sprPtr= bank_[catId].GetSprite( sprId );
	const uint16* pixels= bank_[catId].Data( sprPtr->offset_ );

	switch( efx ) {

	case EfxTransparent:
		BlitTransparent( sprPtr, pixels, to, pos);
		break;

	case EfxShadowIso:
		BlitShadowIso( sprPtr, pixels, to, pos );
		break;

	case EfxShadow2D:
		BlitShadow2D( sprPtr, pixels, to, pos );
		break;

	case EfxWired:
		BlitWired( sprPtr, pixels, to, pos );
		break;

	default:
		// Just in case
		check(0);
		Blit( sid, to, pos );
	}
}

void 
iGfxManager::BlitAlpha( SpriteId sid, iDib& to, iPoint pos, uint8 a ) const
{
	// All other effects works only for span-sprites
	uint32 catId = sid >> 16;
	uint32 sprId = sid & 0xffff;

	check( catId < MaxSpriteBanks );
	check( sprId < bank_[catId].props.GetSize() );

	const Sprite* sprPtr= bank_[catId].GetSprite( sprId );
	const uint16* pixels= bank_[catId].Data( sprPtr->offset_ );
	iRect src_rect( 0, 0, sprPtr->sizeW_, sprPtr->sizeH_ );

	if (a == 63) BlitNormal(sprPtr, pixels, to, src_rect, pos);
	else BlitAlpha(sprPtr, pixels, to, src_rect, pos, a);
}

void 
iGfxManager::BlitAlpha( SpriteId sid, iDib& to, const iRect& src, iPoint pos, uint8 a ) const
{
	// All other effects works only for span-sprites
	uint32 catId = sid >> 16;
	uint32 sprId = sid & 0xffff;
	
	check( catId < MaxSpriteBanks );
	check( sprId < bank_[catId].props.GetSize() );
	
	const Sprite* sprPtr= bank_[catId].GetSprite( sprId );
	const uint16* pixels= bank_[catId].Data( sprPtr->offset_ );

	if (a == 63) BlitNormal(sprPtr, pixels, to, src, pos);
	else BlitAlpha(sprPtr, pixels, to, src, pos, a);
}

void 
iGfxManager::BlitAlpha( const iGfxManager::Sprite* sprPtr, const uint16* pixels, iDib& srf, const iRect& src, iPoint pos, uint8 a ) const
{
	check( sprPtr != 0 && pixels != 0 );

	// return if fully transparent
	if ( a == 0 ) return;

	iPoint origin( sprPtr->originX_, sprPtr->originY_ );
	iSize  size( sprPtr->sizeW_, sprPtr->sizeH_ );

	// correct sprite position according to anchor
	pos += origin;

	// check bounds & clip
	// __NOT_NECESSARY__ if ( (pos.x + (sint32)size.w) <= 0 || (pos.y + (sint32)size.h) <= 0) return;
	iRect src_rect(src);
	//iSize siz( srf.GetWidth() - pos.x, srf.GetHeight() - pos.y);
	//iRect dst_rect(pos,siz);  
	//__SAME_AS__
	iRect dst_rect( pos, size );
	if (!iClipper::iClipRectRect(dst_rect,srf.GetSize(),src_rect,size)) return;

	// calc dest ptr
	uint16* dst_clr = srf.GetPtr(dst_rect);
	const size_t dstStride = srf.GetWidth();

	// blit
	if ( sprPtr->type_ & 1 ) {
		// Spanned sprite
		typedef Blitter<ConstAlphaBlendOp> Blt;
		ConstAlphaBlendOp op(a);
		// ptr, dst_clr, dstStrude, src_rect(.x .y .w, .h )

		// <*1*>
		const uint16* eptr= pixels + (size_t)*pixels + 1;	
		const uint16* ptr = pixels + 1;

		if (src_rect.size() == size){
			// sprite non clipped - use lighting fast blit
			while (ptr != eptr) {
				ptr = Blt::SpanFast( op, ptr,dst_clr );
				dst_clr += dstStride;
			}
		} else {
			// sprite clipped - full blown blit
			dst_clr -= src_rect.x;

			sint32 toSkip = src_rect.y;
			while (toSkip--) ptr = Blt::SpanSkip( ptr );

			const uint16* clipIn = dst_clr + src_rect.x;
			for (sint32 yy=0; yy<(sint32)src_rect.h; ++yy){
				ptr = Blt::Span(op, ptr, dst_clr, clipIn, clipIn + src_rect.w );
				dst_clr	+= dstStride;
				clipIn	+= dstStride;
			}
		}
	} else {
		// Raw sprite
		const uint16* src_clr = pixels + src_rect.y*size.w + src_rect.x;

		for (uint32 yy=0; yy != dst_rect.h; ++yy) {
			//BlitDibBlock_RGB(dst_clr,src_clr,dst_rect.w);
			memcpy( dst_clr, src_clr, dst_rect.w * sizeof(uint16) );
			src_clr += size.w;
			dst_clr += dstStride;
		}
	}
}

void 
iGfxManager::BlitTransparent( const Sprite* sprPtr, const uint16* pixels, iDib& srf, iPoint pos) const
{
	check( sprPtr != 0 && pixels != 0 );

	if ( 0 ==(sprPtr->type_ & 1) ) {
		check(0); return;
	}

	iPoint origin( sprPtr->originX_, sprPtr->originY_ );
	iSize  size( sprPtr->sizeW_, sprPtr->sizeH_ );

	// correct sprite position according to anchor
	pos += origin;

	iRect src_rect(size);
	iRect dst_rect( pos, size );
	if (!iClipper::iClipRectRect(dst_rect,srf.GetSize(),src_rect,size)) return;

	// calc dest ptr
	uint16* dst_clr = srf.GetPtr(dst_rect);
	const size_t dstStride = srf.GetWidth();

	// blit
	// Spanned sprite
	typedef Blitter<Shadow25Op> Blt;
	Shadow25Op op;

	// <*1*>
	const uint16* eptr= pixels + (size_t)*pixels + 1;	
	const uint16* ptr = pixels + 1;

	if (src_rect.size() == size){
		// sprite non clipped - use lighting fast blit
		while (ptr != eptr) {
			ptr = Blt::SpanFast( op, ptr,dst_clr );
			dst_clr += dstStride;
		}
	} else {
		// sprite clipped - full blown blit
		dst_clr -= src_rect.x;

		sint32 toSkip = src_rect.y;
		while (toSkip--) ptr = Blt::SpanSkip( ptr );

		const uint16* clipIn = dst_clr + src_rect.x;
		for (sint32 yy=0; yy<(sint32)src_rect.h; ++yy){
			ptr = Blt::Span( op, ptr, dst_clr, clipIn, clipIn + src_rect.w );
			dst_clr	+= dstStride;
			clipIn	+= dstStride;
		}
	}
}

void 
iGfxManager::BlitShadowIso( const Sprite* sprPtr, const uint16* pixels, iDib& srf, iPoint pos ) const
{
	check( sprPtr != 0 && pixels != 0 );
	check( (sprPtr->type_ & 1) == 1 );	// sprite must be spanned

	iPoint origin( sprPtr->originX_, sprPtr->originY_ );
	iPoint opos = origin + pos;

	// sprite rect -> sprite shadow rect
	iSize sprSz( sprPtr->sizeW_, sprPtr->sizeH_ );
	uint32 hhgt = (sprPtr->sizeH_ + 1) / 2;
	check( hhgt > 1 );
	opos.x -= hhgt ;
	opos.y += sprSz.h - hhgt - 1;
	sprSz.w += hhgt - 1;
	sprSz.h = hhgt;

	if ( (opos.x + (sint32)sprSz.w) <= 0 || (opos.y + (sint32)sprSz.h) <= 0) return;
	iRect src_rect(sprSz);
	iSize siz = iSize(srf.GetWidth() - pos.x, srf.GetHeight() - pos.y);
	iRect dst_rect(opos,siz);
	if (!iClipper::iClipRectRect(dst_rect,srf.GetSize(),src_rect,sprSz)) return;

	uint16* dst_clr = srf.GetPtr(dst_rect);

	const uint16* ptr = pixels + 1;
	const uint16* eptr = pixels + (size_t)*pixels + 1;;
	const size_t dstStride = srf.GetWidth();

	typedef Blitter<Shadow50Op> Blt;
	Shadow50Op op;

	if ( src_rect.size() == sprSz  ){
		while (ptr != eptr) {
			//ptr = ProcessShadowSpanLine(ptr, dst_clr);
			ptr = Blt::SpanFast(op, ptr, dst_clr);
			++dst_clr;											// shadow skew
			dst_clr += dstStride;
			//if ( ptr != eptr ) ptr = SkipSpanLine(ptr);			// shadow skip
			if ( ptr != eptr ) ptr = Blt::SpanSkip(ptr);			// shadow skip
		}
	} else {
		dst_clr -= src_rect.x;

		sint32 toSkip = src_rect.y;
		while (toSkip--) { 
			//ptr = SkipSpanLine(ptr); 
			ptr = Blt::SpanSkip(ptr); 
			check( ptr != eptr );								// skip by factor of two
			//ptr = SkipSpanLine(ptr); 
			ptr = Blt::SpanSkip(ptr); 
		}

		const uint16* clipIn = dst_clr + src_rect.x;
		dst_clr += src_rect.y;									// pre-skew ( after y-clipping )
		for (sint32 yy=0; yy<(sint32)src_rect.h; ++yy) {
			//ptr = ProcessShadowClipSpanLine(ptr, dst_clr, clipIn, clipIn + src_rect.w );
			ptr = Blt::Span(op, ptr, dst_clr, clipIn, clipIn + src_rect.w );
			clipIn += dstStride;
			dst_clr += dstStride;
			++dst_clr;
			if ( ptr != eptr ) ptr = Blt::SpanSkip(ptr); 
			else break;
			//ptr = SkipSpanLine(ptr);							// shadow skip
		}
	}
}

void 
iGfxManager::BlitShadow2D( const Sprite* sprPtr, const uint16* pixels, iDib& srf, iPoint pos ) const
{
	check( sprPtr != 0 && pixels != 0 );

	if ( 0 ==(sprPtr->type_ & 1) ) {
		check(0); return;
	}

	iPoint origin( sprPtr->originX_, sprPtr->originY_ );
	iSize  size( sprPtr->sizeW_, sprPtr->sizeH_ );

	// correct sprite position according to anchor
	pos += origin;
	pos += iPoint( -1, 2 );	// shadow displacement

	iRect src_rect(size);
	iRect dst_rect( pos, size );
	if (!iClipper::iClipRectRect(dst_rect,srf.GetSize(),src_rect,size)) return;

	// calc dest ptr
	uint16* dst_clr = srf.GetPtr(dst_rect);
	const size_t dstStride = srf.GetWidth();

	// blit
	// Spanned sprite
	typedef Blitter<Shadow50Op> Blt;
	Shadow50Op op;

	// <*1*>
	const uint16* eptr= pixels + (size_t)*pixels + 1;	
	const uint16* ptr = pixels + 1;

	if (src_rect.size() == size){
		// sprite non clipped - use lighting fast blit
		while (ptr != eptr) {
			ptr = Blt::SpanFast( op, ptr,dst_clr );
			dst_clr += dstStride;
		}
	} else {
		// sprite clipped - full blown blit
		dst_clr -= src_rect.x;

		sint32 toSkip = src_rect.y;
		while (toSkip--) ptr = Blt::SpanSkip( ptr );

		const uint16* clipIn = dst_clr + src_rect.x;
		for (sint32 yy=0; yy<(sint32)src_rect.h; ++yy){
			ptr = Blt::Span(op, ptr, dst_clr, clipIn, clipIn + src_rect.w );
			dst_clr	+= dstStride;
			clipIn	+= dstStride;
		}
	}
}

void 
iGfxManager::BlitWired( const Sprite* sprPtr, const uint16* pixels, iDib& srf, iPoint pos) const
{
	check( sprPtr != 0 && pixels != 0 );

	iPoint origin( sprPtr->originX_, sprPtr->originY_ );
	iSize  size( sprPtr->sizeW_, sprPtr->sizeH_ );

	BlitNormal(sprPtr, pixels, srf, iRect(0,0,size.w,size.h), pos);
}


void 
iGfxManager::BlitMasked( SpriteId sid, SpriteId mid, iDib& srf, iPoint pos ) const
{
	uint32 catSId = sid >> 16;
	uint32 sprSId = sid & 0xffff;
	check( catSId < MaxSpriteBanks );
	check( sprSId < bank_[catSId].props.GetSize() );

	uint32 catMId = mid >> 16;
	uint32 sprMId = mid & 0xffff;
	check( catMId < MaxSpriteBanks );
	check( sprMId < bank_[catMId].props.GetSize() );

	const Sprite* sprPtr= bank_[catSId].GetSprite( sprSId );
	const uint16* sprPix= bank_[catSId].Data( sprPtr->offset_ );

	const Sprite* mskPtr= bank_[catMId].GetSprite( sprMId );
	const uint16* mskPix= bank_[catMId].Data( mskPtr->offset_ );

	check( (sprPtr->type_ & 1) == 0 );	// sprite should be RAW
	check( (mskPtr->type_ & 1) == 1 );	// while mask is spanned

	//-------------------------------------------------------

	iPoint sprOrigin( sprPtr->originX_, sprPtr->originY_ );
	iSize  sprSize( sprPtr->sizeW_, sprPtr->sizeH_ );

	// correct sprite position according to anchor
	pos += sprOrigin;

	iRect src_rect(sprSize);
	iRect dst_rect( pos, sprSize ); // ? was 'siz' = srf.Size - pos
	if (!iClipper::iClipRectRect(dst_rect,srf.GetSize(),src_rect,sprSize)) return;

	// calc dest ptr
	uint16* dptr = srf.GetPtr(dst_rect);
	const uint16* sptr = sprPix;
	// <*1*>
	const uint16* mptr = mskPix + 1;
	const uint16* eptr = mskPix + (size_t)*mskPix + 1;
	const size_t dstStride = srf.GetWidth();
	const size_t srcStride = sprSize.w;

	// Spanned sprite
	typedef Blitter<CopyOp> Blt;
	CopyOp op;

	if (src_rect.size() == sprSize){
		// sprite non clipped - use lighting fast blit
		while (mptr != eptr) {
			mptr = Blt::MaskedSpanFast( op, mptr, sptr,dptr );
			dptr += dstStride;
			sptr += srcStride;
		}
	} else {
		// sprite clipped - full blown blit
		dptr -= src_rect.x;

		sint32 toSkip = src_rect.y;
		sptr += sprSize.w * toSkip;
		while (toSkip--) mptr = Blt::SpanSkip( mptr );

		const uint16* clipIn = dptr + src_rect.x;
		for (sint32 yy=0; yy<(sint32)src_rect.h; ++yy){
			mptr = Blt::MaskedSpan( op, mptr, sptr, dptr, clipIn, clipIn + src_rect.w );
			dptr	+= dstStride;
			clipIn	+= dstStride;
			sptr	+= srcStride;
		}
	}
}

//
void 
iGfxManager::BlitTile( SpriteId sid, iDib& srf, const iRect& dst ) const
{
	BlitTile(sid,srf,Dimension(sid),dst);
}

void 
iGfxManager::BlitTile( SpriteId sid, iDib& srf, const iRect& src, const iRect& dst ) const
{
	if (!src.w || !src.h) return;

	// Tile Dib
	uint32 numx = dst.w / src.w;
	uint32 numy = dst.h / src.h;
	if ((dst.w % src.w) != 0) numx ++;
	if ((dst.h % src.h) != 0) numy ++;

	for (uint32 yy=0; yy<numy; ++yy) {
		for (uint32 xx=0; xx<numx; ++xx) {
			sint32 w = iMIN(src.w,dst.w - (xx*src.w)); 
			sint32 h = iMIN(src.h,dst.h - (yy*src.h)); 
			Blit( sid, srf, iRect(src.x,src.y,w,h), iPoint( dst.x+xx*src.w, dst.y+yy*src.h ) );
		}
	}
}

void 
iGfxManager::BlitTile3( SpriteId sid, iDib& srf, const iRect& src, const iRect& dst ) const
{
	uint32 sH = src.h;
	uint32 sW = 0;

	if (!sH || src.w%3 != 0 || !(sW=src.w/3)){
		check(0); return;
	}

	Blit( sid, srf, iRect(src.x,src.y,sW,sH), dst.point() );
	Blit( sid, srf, iRect(src.x+sW*2, src.y,sW,sH), iPoint(dst.x+dst.w-sW, dst.y) );
	BlitTile( sid, srf, iRect(src.x+sW,src.y,sW,sH), iRect(dst.x+sW,dst.y,dst.w-sW*2,sH) );
}

void
iGfxManager::MakeDib( SpriteId sid, iDib& dst )
{
	// reside dib
	// same as blit
}

size_t
iGfxManager::MemoryOccuped() const
{
	size_t total = 0;
	for( uint32 n = 0; n != MaxSpriteBanks; ++n ) {
		total += bank_[n].MemTotal();
	}
	return total;
}

// ....

iGfxManager::SpriteBank::SpriteBank()
{
	data = 0;
	dataSize = 0;
	hFile = INVALID_HANDLE_VALUE;
	hMapping = INVALID_HANDLE_VALUE;
	pMap = NULL;
	loadMode = LM_Unknown;
}

iGfxManager::SpriteBank::~SpriteBank()
{
	Unload();
}


///////////////////////////////////////////////////////////////////////////////
struct ApplyGammaMethod
{
	ScopedArray<uint16> trans_;
	ApplyGammaMethod( const uint8* table )
	{
		uint16* mapping = new uint16[65536];
		trans_.Reset( mapping );
		for (uint32 rr=0; rr<0x20; ++rr) {
			for (uint32 gg=0; gg<0x40; ++gg) {
				for (uint32 bb=0; bb<0x20; ++bb) {
					*mapping++ = (((uint32)table[rr])<<11) | (((uint32)table[32+gg])<<5) | ((uint32)table[bb]);
				}
			}
		}
	}

	struct Operator
	{
		const uint16* tab_;
		always_inline Operator( const uint16* t ) : tab_(t) {}
		always_inline void operator()( Pixel* ptr ) const
		{
			*ptr = tab_[*ptr];
		}
	};

	always_inline Operator makeop() const
	{ return Operator(trans_.Get()); }

};


//////////////////////////////////////////////////////////////////////////////

bool
iGfxManager::SpriteBank::Load( const TCHAR* filename, LoadMode lmode, uint32 gammaLevel )
{
	fileName = filename;
	if (lmode == LM_Memory) {
		iFilePtr file = OpenWin32File(filename);
		if (!file) return false;

		// NOTE! Reading POD structures is safe but beware _PACKING_

		// read header
		BankHeader header;
		file->Read( &header, sizeof(BankHeader) );

		// read pixels grand totale
		data = (uint8*)malloc( header.dataLength );
		file->Read( data, header.dataLength );
		dataSize = header.dataLength;

		file->Seek(16,iFileI::FSEEK_CUR);
		uint32 fpos=file->GetPosition();

		props.Allocate( header.numSprites );
		iGfxManager::Sprite* spr = props.GetPtr();
		// read sprite records
		for( uint32 n = 0; n != header.numSprites; ++n ) {
			BankImage img;
			file->Read( &img, sizeof(BankImage) );

			spr[n].offset_	= img.dataOffset;
			spr[n].sizeW_	= img.imageWidth;
			spr[n].sizeH_	= img.imageHeight;
			spr[n].originX_	= img.originX;
			spr[n].originY_ = img.originY;
			spr[n].type_	= img.imageType;
		}
		// postprocess sprites and apply gamma
		if ( gammaLevel != 0 ) {
			typedef Blitter<ApplyGammaMethod::Operator> Blt;
			check( gammaLevel < gammaTabNum );
			ApplyGammaMethod method( gammaTabs[gammaLevel] );
			for( uint32 n = 0; n != header.numSprites; ++n ) {
				// hack!! hack!!
				if ( spr[n].type_ & 1 ) {
					uint16* pixels= (uint16*)Data( spr[n].offset_ );
					uint16* ptr = pixels + 1;
					uint16* eptr = pixels + (size_t)*pixels + 1;;
					while ( ptr != eptr ) {
						ptr = Blt::ProcessSpan( method.makeop(), ptr );
					}
				} else {
					uint16* pixels= (uint16*)Data( spr[n].offset_ );
					uint16* ptr = pixels;
					uint16* eptr = pixels + spr[n].sizeH_ * spr[n].sizeW_;
					ApplyGammaMethod::Operator op = method.makeop();
					while ( ptr != eptr ) {
						op( ptr++ );
					}	
				}
			}
		}
	} else if (lmode == LM_MappedFile) {
#ifndef OS_WINCE
		hFile = CreateFile( filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
#else
		hFile = CreateFileForMapping( filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
#endif
		if ( INVALID_HANDLE_VALUE == hFile ) {
			DWORD err = GetLastError();
			return false;
		}
		hMapping = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, 0, NULL );
		if ( 0 == hMapping ) {
			DWORD err = GetLastError();
			return false;
		}
		pMap = MapViewOfFile( hMapping, FILE_MAP_READ, 0, 0, 0 );
		if ( NULL == pMap ) {
			DWORD err = GetLastError();
			return false;
		}
		// NOTE! Reading POD structures is safe but beware _PACKING_

		// read header
		BankHeader header = *(BankHeader*)pMap;

		// read pixels grand totale
		data = (uint8*)pMap + sizeof(BankHeader);
		dataSize = header.dataLength;

	//	file->Seek(16,iFileI::FSEEK_CUR);
	//	uint32 fpos=file->GetPosition();

		props.Allocate( header.numSprites );
		iGfxManager::Sprite* spr = props.GetPtr();
		// read sprite records
		BankImage* recPtr = (BankImage*)((uint8*)pMap + sizeof(BankHeader) + header.dataLength + 16);
		for( uint32 n = 0; n != header.numSprites; ++n ) {
			BankImage img = *recPtr++;

			spr[n].offset_	= img.dataOffset;
			spr[n].sizeW_	= img.imageWidth;
			spr[n].sizeH_	= img.imageHeight;
			spr[n].originX_	= img.originX;
			spr[n].originY_ = img.originY;
			spr[n].type_	= img.imageType;
		}
	} else {
		check("Unknown load mode!" == 0);
		return false;
	}

	loadMode = lmode;

	return true;
}

void
iGfxManager::SpriteBank::Unload()
{
	props.Clean();
	if (loadMode == LM_Memory) {
		free( data );
	} else if (loadMode == LM_MappedFile) {
		if ( pMap ) {
			UnmapViewOfFile( pMap );
			pMap = 0;
		}
		if ( hMapping ) {
			CloseHandle( hMapping );
			hMapping = INVALID_HANDLE_VALUE;
		}
		if ( hFile ) {
			CloseHandle( hFile );
			hFile = INVALID_HANDLE_VALUE;
		}
	}
}

bool
iGfxManager::SpriteBank::Reload( uint32 gammaLevel )
{
	if ( loadMode != LM_MappedFile ) {
		Unload();
		return Load( fileName.CStr(), loadMode, gammaLevel );
	}
	return true;
}

const iGfxManager::Sprite*
iGfxManager::SpriteBank::GetSprite( uint32 num ) const
{
	check( num < 65536 );		// just in case someone pass full sprite id
	check( num < props.GetSize() );
	return props.GetPtr() + num;
}

const uint16*
iGfxManager::SpriteBank::Data( uint32 offset ) const
{
	return (const uint16*)(data) + offset;
}

size_t
iGfxManager::SpriteBank::MemTotal() const
{
	return dataSize + props.GetSize() * sizeof(iGfxManager::Sprite);
}

