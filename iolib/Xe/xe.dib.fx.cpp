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

#include "xe.std.h"

/*
__forceinline float Displace(float num, const iSize &dsiz)
{
	float _max = num / (float)(dsiz.w + dsiz.h) * 3.0f * log(num/dsiz.w);
	return (((float)rand())/(double)RAND_MAX - 0.5f) * _max;
}

void DivideGrid(iDib &dib, float x, float y, float width, float height, float c1, float c2, float c3, float c4)
{
	float Edge1, Edge2, Edge3, Edge4, Middle;
	float newWidth = width / 2.0;
	float newHeight = height / 2.0;

	if (width > 2.0f || height > 2.0f){	
		Middle = (c1 + c2 + c3 + c4) / 4.0f + Displace(newWidth + newHeight, dib.GetSize());	//Randomly displace the midpoint!
		Edge1 = (c1 + c2) / 2.0f;	//Calculate the edges by averaging the two corners of each edge.
		Edge2 = (c2 + c3) / 2.0f;
		Edge3 = (c3 + c4) / 2.0f;
		Edge4 = (c4 + c1) / 2.0f;
		
		//Make sure that the midpoint doesn't accidentally "randomly displaced" past the boundaries!
		if (Middle < 0) {
			Middle = 0;
		} else if (Middle > 1.0f) {
			Middle = 1.0f;
		}
		
		//Do the operation over again for each of the four new grids.			
		DivideGrid(dib, x, y, newWidth, newHeight, c1, Edge1, Middle, Edge4);
		DivideGrid(dib, x + newWidth, y, newWidth, newHeight, Edge1, c2, Edge2, Middle);
		DivideGrid(dib, x + newWidth, y + newHeight, newWidth, newHeight, Middle, Edge2, c3, Edge3);
		DivideGrid(dib, x, y + newHeight, newWidth, newHeight, Edge4, Middle, Edge3, c4);
	} else	{
		//This is the "base case," where each grid piece is less than the size of a pixel.
		//The four corners of the grid piece will be averaged and drawn as a single pixel.
		float c = (c1 + c2 + c3 + c4) / 4.0f;
		iColor *clr = dib.GetBits() + dib.GetWidth()*(uint32)y + (uint32)x;
		clr->SetRGBA(c*255.0f,c*255.0f,c*255.0f,255);
	}	
}

void iDibFX::GenerateClouds(iDib &dib)
{
	float c1, c2, c3, c4;
	
	//Assign the four corners of the intial grid random color values
	//These will end up being the colors of the four corners of the applet.		
	c1 = (float)((float)rand())/(float)RAND_MAX;
	c2 = (float)((float)rand())/(float)RAND_MAX;
	c3 = (float)((float)rand())/(float)RAND_MAX;
	c4 = (float)((float)rand())/(float)RAND_MAX;
			
	DivideGrid(dib, 0, 0, dib.GetWidth() , dib.GetHeight() , c1, c2, c3, c4);	
}
*/

__forceinline sint32 Displace(sint32 num, const sint32 siz)
{
	/*
	sint32 _max = num / (siz + siz) * 3;// * log<sint32>(num/siz);
	return ((rand()%256 - 128) * _max) >> 8;*/
	float _max = (float)(num>>8) / (float)(siz+siz) * 3.0f * logf(float(num>>8)/siz);
	return sint32((((float)rand())/(float)RAND_MAX - 0.5f) * _max * 256.0f);
}

void DivideGrid(iDib &dib, sint32 x, sint32 y, sint32 width, sint32 height, sint32 c1, sint32 c2, sint32 c3, sint32 c4)
{
	sint32 Edge1, Edge2, Edge3, Edge4, Middle;
	sint32 newWidth = width >> 1;
	sint32 newHeight = height >> 1;

	if (width >= 512 || height >= 512){
		Middle = ((c1 + c2 + c3 + c4) >> 2) + Displace(newWidth + newHeight, dib.GetWidth());	//Randomly displace the midpoint!
		Edge1 = (c1 + c2) >> 1;	//Calculate the edges by averaging the two corners of each edge.
		Edge2 = (c2 + c3) >> 1;
		Edge3 = (c3 + c4) >> 1;
		Edge4 = (c4 + c1) >> 1;
		
		//Make sure that the midpoint doesn't accidentally "randomly displaced" past the boundaries!
		if (Middle < 0) {
			Middle = 0;
		} else if (Middle > 255) {
			Middle = 255;
		}
		
		//Do the operation over again for each of the four new grids.			
		DivideGrid(dib, x, y, newWidth, newHeight, c1, Edge1, Middle, Edge4);
		DivideGrid(dib, x + newWidth, y, newWidth, newHeight, Edge1, c2, Edge2, Middle);
		DivideGrid(dib, x + newWidth, y + newHeight, newWidth, newHeight, Middle, Edge2, c3, Edge3);
		DivideGrid(dib, x, y + newHeight, newWidth, newHeight, Edge4, Middle, Edge3, c4);
	} else	{
		//This is the "base case," where each grid piece is less than the size of a pixel.
		//The four corners of the grid piece will be averaged and drawn as a single pixel.
		
		uint8 c = uint8((c1 + c2 + c3 + c4) >> 2);
		iColor *clr = dib.GetBits() + dib.GetWidth()*(y>>8) + (x>>8);
		clr->SetRGBA(c,c,c,255);
	}	
}

void iDibFX::GenerateClouds(iDib &dib)
{
	sint32 c1, c2, c3, c4;
	
	//Assign the four corners of the intial grid random color values
	//These will end up being the colors of the four corners of the applet.		
	c1 = (rand()<<8)/RAND_MAX;
	c2 = (rand()<<8)/RAND_MAX;
	c3 = (rand()<<8)/RAND_MAX;
	c4 = (rand()<<8)/RAND_MAX;
			
	DivideGrid(dib, 0, 0, dib.GetWidth()<<8 , dib.GetHeight()<<8 , c1, c2, c3, c4);	
}
