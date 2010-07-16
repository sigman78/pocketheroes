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

#include <windows.h>
#include <stdio.h>

//
int main(int argc, char *argv[])
{
	for (int xx=0; xx<32; ++xx) {	
		//printf("0x%04X,\n", ((xx)<<11)); 							// Player 1 (red)
		//printf("0x%04X,\n", ((xx*8/3)<<5)); 						// Player 2 (green)
		//printf("0x%04X,\n", xx);									// Player 3 (blue)
		int val = xx;
		printf("0x%04X,\n", xx | (xx<<6)  | ((xx)<<11)); 	// Player 4 (yellow)
		// printf("0x%04X,\n", ((xx)<<11) | ((xx*2)<<5) | (xx) ); 	// BW
	}
	return 0;
}