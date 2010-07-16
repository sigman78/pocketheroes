//
// dct internals

#include "stdafx.h"
//#include "xjp.boot.h"

#include "xjp.dct.h"

namespace xjp {

//////////////////////////////////////////////////////////
//   - Data flow schematics for FDCT -
// Output is scaled by 2.sqrt(2)
// Initial butterflies (in0/in7, etc.) are not fully depicted.
// Note: Rot6 coeffs are multiplied by sqrt(2).
//////////////////////////////////////////////////////////
/*
 <---------Stage1 =even part=----------->

 in3 mm3  +_____.___-___________.____* out6
  x              \ /            |
 in4 mm4          \             |
                 / \            |
 in0 mm0  +_____o___+__.___-___ | ___* out4
  x                     \ /     |
 in7 mm7                 \    (Rot6)
                        / \     |
 in1 mm1  +_____o___+__o___+___ | ___* out0
  x              \ /            |
 in6 mm6          /             |
                 / \            |
 in2 mm2  +_____.___-___________o____* out2
  x
 in5 mm5

 <---------Stage2 =odd part=---------------->
 
 mm7*___._________.___-___[xSqrt2]___* out3
        |          \ /
      (Rot3)        \
        |          / \
 mm5*__ | ___o____o___+___.___-______* out7
        |    |             \ /
        |  (Rot1)           \
        |    |             / \
 mm6*__ |____.____o___+___o___+______* out1
        |          \ /
        |           /
        |          / \
 mm4*___o_________.___-___[xSqrt2]___* out5



    Alternative schematics for stage 2:
    -----------------------------------

 mm7 *___[xSqrt2]____o___+____o_______* out1
                      \ /     |
                       /    (Rot1)
                      / \     |
 mm6 *____o___+______.___-___ | __.___* out5
           \ /                |   |
            /                 |   |
           / \                |   |
 mm5 *____.___-______.___-____.__ | __* out7
                      \ /         |
                       \        (Rot3)
                      / \         |
 mm4 *___[xSqrt2]____o___+________o___* out3

*/

//////////////////////////////////////////////////////////
//   - Data flow schematics for IDCT -
// Output is scaled by 2.sqrt(2)
// Note: Rot6 coeffs are multiplied by sqrt(2).
//////////////////////////////////////////////////////////
/*
 <---------Stage 1 =even part=---------------->

 in3 *___[xSqrt2]____.___-________o____* mm7
                      \ /         |
                       /          |
                      / \         |
 in1 *____o___+______o___+____.__ | ___* mm3
           \ /                |   |
            /               (Rot3)|
           / \                |   |
 in7 *____.___-______.___-____o__ | ___* mm1
                      \ /         |
                       \        (Rot1)
                      / \         |
 in5 *___[xSqrt2]____o___+________.____* mm5


    Alternative schematics for stage 1:
    -----------------------------------
 
 in1 *________.____.___-_____[xSqrt2]__* mm1
              |     \ /
            (Rot1)   \
              |     / \
 in5 *___o___ | ___o___+_____.___-_____* mm5
         |    |               \ /
         |    |                \
         |    |               / \
 in7 *__ | ___o____o___+__ __o___+_____* mm7
         |          \ /
       (Rot3)        /
         |          / \
 in3 *___._________.___-_____[xSqrt2]__* mm3

 <---------Stage2 =odd part=--------->

 in6 *__o___________o___+___* mm6 -> butterfly with mm3 -> out3/out4
 mm6    |            \ /    
        |             /
        |            / \    
 in4 *_ | ___o___+__.___-___* mm4 -> butf. w/ mm1 -> out7/out0
 mm4    |     \ /
      (Rot6)   /
        |     / \
 in0 *_ | ___.___-__.___-___* mm0 -> butf. w/ mm5 -> out6/out1
 mm0    |            \ /
        |             \
        |            / \
 in2 *__.___________o___+___* mm2 -> butf. w/ mm7 -> out2/out5
 mm2

*/
//////////////////////////////////////////////////////////




// macroses to hide non important details
//
#define LOAD_BUTF(m1, m2, a, b, tmp, S) \
  (m1) = (S)[(a)] + (S)[(b)]; \
  (m2) = (S)[(a)] - (S)[(b)]

#define BUTF(a, b, tmp) \
  (tmp) = (a)+(b); \
  (b) = (a)-(b);   \
  (a) = (tmp)

#define ROTATE(m1,m2,c,k1,k2,tmp,Fix,Rnd) \
  (tmp) = ( (m1) + (m2) )*(c); \
  (m1) *= k1; \
  (m2) *= k2; \
  (tmp) += (Rnd); \
  (m1) = ((m1)+(tmp))>>Fix; \
  (m2) = ((m2)+(tmp))>>Fix;

#define ROTATE2(m1,m2,c,k1,k2,tmp) \
  (tmp) = ( (m1) + (m2) )*(c); \
  (m1) *= k1; \
  (m2) *= k2; \
  (m1) = (m1)+(tmp); \
  (m2) = (m2)+(tmp);

#define ROTATE0(m1,m2,c,k1,k2,tmp) \
  (m1) = ( (m2) )*(c); \
  (m2) = (m2)*k2+(m1);

#define SHIFTL(x,n)   ((x)<<(n))
#define SHIFTR(x, n)  ((x)>>(n))
#define HALF(n)       (1<<((n)-1))

#define IPASS 3
#define FPASS 2
#define FIX  16

//
// algorithm constants turned into .16 fixed point
#define ROT6_C     35468
#define ROT6_SmC   50159
#define ROT6_SpC  121095
#define ROT17_C    77062
#define ROT17_SmC  25571
#define ROT17_SpC 128553
#define ROT37_C    58981
#define ROT37_SmC  98391
#define ROT37_SpC  19571
#define ROT13_C   167963
#define ROT13_SmC 134553
#define ROT13_SpC 201373

//
// forward dct transform
void fdct16( dpel_t* pels )
{
	dpel_t* in = pels;
	
	for( size_t i = 0; i != 8; i++ ) {
	    sint32 mm0, mm1, mm2, mm3, mm4, mm5, mm6, mm7, Spill;

	    // odd
	    LOAD_BUTF(mm1,mm6, 1, 6, mm0, in);
	    LOAD_BUTF(mm2,mm5, 2, 5, mm0, in);
	    LOAD_BUTF(mm3,mm4, 3, 4, mm0, in);
	    LOAD_BUTF(mm0,mm7, 0, 7, Spill, in);

	    BUTF(mm1, mm2, Spill);
	    BUTF(mm0, mm3, Spill);

	    ROTATE(mm3, mm2, ROT6_C, ROT6_SmC, -ROT6_SpC, Spill, FIX-FPASS, HALF(FIX-FPASS));
    	in[2] = (dpel_t)mm3;
	    in[6] = (dpel_t)mm2;

	    BUTF(mm0, mm1, Spill);
    	in[0] = (dpel_t)SHIFTL(mm0, FPASS);
	    in[4] = (dpel_t)SHIFTL(mm1, FPASS);

	
		// even
	    mm3 = mm5 + mm7;
	    mm2 = mm4 + mm6;
	    ROTATE(mm2, mm3,  ROT17_C, -ROT17_SpC, -ROT17_SmC, mm0, FIX-FPASS, HALF(FIX-FPASS));
	    ROTATE(mm4, mm7, -ROT37_C,  ROT37_SpC,  ROT37_SmC, mm0, FIX-FPASS, HALF(FIX-FPASS));
	    mm7 += mm3;
	    mm4 += mm2;
	    in[1] = (dpel_t)mm7;
	    in[7] = (dpel_t)mm4;

	    ROTATE(mm5, mm6, -ROT13_C,  ROT13_SmC,  ROT13_SpC, mm0, FIX-FPASS, HALF(FIX-FPASS));
	    mm5 += mm3;
	    mm6 += mm2;
	    in[3] = (dpel_t)mm6;
	    in[5] = (dpel_t)mm5;

    	in  += 8;
	}

	in = pels;
	for( size_t j = 0; j != 8; j++ ) {
	    sint32 mm0, mm1, mm2, mm3, mm4, mm5, mm6, mm7, Spill;

	    // odd
		LOAD_BUTF(mm1,mm6, 1*8, 6*8, mm0, in);
		LOAD_BUTF(mm2,mm5, 2*8, 5*8, mm0, in);
		BUTF(mm1, mm2, mm0);

		LOAD_BUTF(mm3,mm4, 3*8, 4*8, mm0, in);
		LOAD_BUTF(mm0,mm7, 0*8, 7*8, Spill, in);
		BUTF(mm0, mm3, Spill);

		ROTATE(mm3, mm2, ROT6_C, ROT6_SmC, -ROT6_SpC, Spill, 0,  HALF(FIX+FPASS+3));
		in[2*8] = (dpel_t)SHIFTR(mm3,FIX+FPASS+3);
		in[6*8] = (dpel_t)SHIFTR(mm2,FIX+FPASS+3);

		mm0 += HALF(FPASS+3) - 1;
		BUTF(mm0, mm1, Spill);
		in[0*8] = (dpel_t)SHIFTR(mm0, FPASS+3);
	    in[4*8] = (dpel_t)SHIFTR(mm1, FPASS+3);

		// even
		mm3 = mm5 + mm7;
		mm2 = mm4 + mm6;

		ROTATE(mm2, mm3,  ROT17_C, -ROT17_SpC, -ROT17_SmC, mm0, 0, HALF(FIX+FPASS+3));
		ROTATE2(mm4, mm7, -ROT37_C,  ROT37_SpC,  ROT37_SmC, mm0);
		mm7 += mm3;
		mm4 += mm2;
		in[7*8] = (dpel_t)SHIFTR(mm4,FIX+FPASS+3);
		in[1*8] = (dpel_t)SHIFTR(mm7,FIX+FPASS+3);

		ROTATE2(mm5, mm6, -ROT13_C,  ROT13_SmC,  ROT13_SpC, mm0);
		mm5 += mm3;
		mm6 += mm2;
		in[5*8] = (dpel_t)SHIFTR(mm5,FIX+FPASS+3);
		in[3*8] = (dpel_t)SHIFTR(mm6,FIX+FPASS+3);

		in++;
	}
}


//
// inverse dct transform
void idct16( dpel_t* pels )
{
	dpel_t* in = pels;
	
	for( size_t i = 0; i != 8; i++ ) {
	    sint32 mm0, mm1, mm2, mm3, mm4, mm5, mm6, mm7, Spill;

		mm4 = (sint32)in[7];
		mm5 = (sint32)in[5];
		mm6 = (sint32)in[3];
		mm7 = (sint32)in[1];

		mm2 = mm4 + mm6;
		mm3 = mm5 + mm7;
		ROTATE2(mm2, mm3,  ROT17_C, -ROT17_SpC, -ROT17_SmC, mm1);
		ROTATE2(mm4, mm7, -ROT37_C,  ROT37_SpC,  ROT37_SmC, mm1);
		ROTATE2(mm5, mm6, -ROT13_C,  ROT13_SmC,  ROT13_SpC, mm1);

		mm4 += mm2;
		mm5 += mm3;
		mm6 += mm2;
		mm7 += mm3;

		// even
		mm3 = (sint32)in[2];
		mm2 = (sint32)in[6];

		ROTATE2(mm3, mm2, ROT6_C, ROT6_SmC, -ROT6_SpC, mm1);

		LOAD_BUTF(mm0, mm1, 0, 4, Spill, in);
		mm0 = SHIFTL(mm0, FIX) + HALF(FIX-IPASS);
		mm1 = SHIFTL(mm1, FIX) + HALF(FIX-IPASS);
		BUTF(mm0, mm3, Spill);
		BUTF(mm1, mm2, Spill);


		BUTF(mm0, mm7, Spill);
		in[0] = (dpel_t)SHIFTR(mm0, FIX-IPASS);
		in[7] = (dpel_t)SHIFTR(mm7, FIX-IPASS);
		BUTF(mm1, mm6, mm0);
    	in[1] = (dpel_t)SHIFTR(mm1, FIX-IPASS);
		in[6] = (dpel_t)SHIFTR(mm6, FIX-IPASS);
		BUTF(mm2, mm5, mm0);
		in[2] = (dpel_t)SHIFTR(mm2, FIX-IPASS);
		in[5] = (dpel_t)SHIFTR(mm5, FIX-IPASS);
    	BUTF(mm3, mm4, mm0);
		in[3] = (dpel_t)SHIFTR(mm3, FIX-IPASS);
		in[4] = (dpel_t)SHIFTR(mm4, FIX-IPASS);

		in += 8;
	}

	in = pels;
	for( size_t j = 0; j != 8; j++ ) {
	    sint32 mm0, mm1, mm2, mm3, mm4, mm5, mm6, mm7, Spill;

	    // odd
		mm4 = (sint32)in[7*8];
		mm5 = (sint32)in[5*8];
		mm6 = (sint32)in[3*8];
		mm7 = (sint32)in[1*8];

		mm2 = mm4 + mm6;
		mm3 = mm5 + mm7;
		ROTATE2(mm2, mm3,  ROT17_C, -ROT17_SpC, -ROT17_SmC, mm1);
		ROTATE2(mm4, mm7, -ROT37_C,  ROT37_SpC,  ROT37_SmC, mm1);
		ROTATE2(mm5, mm6, -ROT13_C,  ROT13_SmC,  ROT13_SpC, mm1);

		mm4 += mm2;
		mm5 += mm3;
		mm6 += mm2;
		mm7 += mm3;
    
		// even
    	mm3 = (sint32)in[2*8];
    	mm2 = (sint32)in[6*8];

		ROTATE2(mm3, mm2, ROT6_C, ROT6_SmC, -ROT6_SpC, mm1);

		LOAD_BUTF(mm0, mm1, 0*8, 4*8, Spill, in);
		mm0 = SHIFTL(mm0, FIX) + HALF(FIX+IPASS+3);
		mm1 = SHIFTL(mm1, FIX) + HALF(FIX+IPASS+3);
		BUTF(mm0, mm3, Spill);
		BUTF(mm1, mm2, Spill);

		BUTF(mm0, mm7, Spill);
		in[8*0] = (dpel_t) SHIFTR(mm0, FIX+IPASS+3);
		in[8*7] = (dpel_t) SHIFTR(mm7, FIX+IPASS+3);
		BUTF(mm1, mm6, mm0);
		in[8*1] = (dpel_t) SHIFTR(mm1, FIX+IPASS+3);
		in[8*6] = (dpel_t) SHIFTR(mm6, FIX+IPASS+3);
		BUTF(mm2, mm5, mm0);
		in[8*2] = (dpel_t) SHIFTR(mm2, FIX+IPASS+3);
		in[8*5] = (dpel_t) SHIFTR(mm5, FIX+IPASS+3);
		BUTF(mm3, mm4, mm0);
		in[8*3] = (dpel_t) SHIFTR(mm3, FIX+IPASS+3);
		in[8*4] = (dpel_t) SHIFTR(mm4, FIX+IPASS+3);

		in++;
	}

}

} //xjp


#ifdef DO_TEST

#include <cstdio>


	short v[64] =
	{ 	0,  1,  2,  3,  4,  5,  6,  7,
		1,  2,  3,  4,  5,  6,  7,  8,
		2,  3,  4,  5,  6,  7,  8,  9,
		3,  4,  5,  6,  7,  8,  9,  10,
		4,  5,  6,  7,  8,   9,  10, 11,
		5,  6,  7,  8,  9,  10, 11, 12,
		6,  7,  8,  9,  10, 11, 12, 255,
		7,  8,  9, 10,  11, 12, 255, -255 };

template< typename T >
void dumpRow( T v[8] )
{
	printf("{%d,%d,%d,%d,%d,%d,%d,%d}\n",v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7] );
}

template< typename T >
void dumpMat( T v[64] )
{
	for( size_t n = 0; n != 8; ++n ) {
		dumpRow( v + n*8 );
	}
}

int main()
{
	short v2[64];

	memcpy( v2, v, 2 * 64 ); 
	xjp::fdct( v2 );
	dumpMat( v2 );
	xjp::idct( v2 );
	dumpMat( v2 );

	for( size_t n = 0; n !=64; n++ ) {
		if ( v2[n] != v[n] ) {
			printf("differs at #%d (%d) = (%d)\n", n, v2[n], v[n] );
			//break;
		}
	}


	printf("done...\n");
	return 0;
}

#endif //DO_TEST



