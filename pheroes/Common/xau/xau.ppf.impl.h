//
// 2006-01-09 : SiGMan - Loops unrolled using array indexing, cleaned up

#ifndef XAU_PPF_IMPL_H__
#define XAU_PPF_IMPL_H__

#include "xau.ppf.h"

#define CATCAT(x,y) x ## y
#define CONCAT(x,y) CATCAT(x,y)

#define MSSEQ_1(m)    m
#define MSSEQ_2(m) MSSEQ_1(m); m
#define MSSEQ_3(m) MSSEQ_2(m); m
#define MSSEQ_4(m) MSSEQ_3(m); m
#define MSSEQ_5(m) MSSEQ_4(m); m
#define MSSEQ_6(m) MSSEQ_5(m); m
#define MSSEQ_7(m) MSSEQ_6(m); m
#define MSSEQ_8(m) MSSEQ_7(m); m
#define MSSEQ_9(m) MSSEQ_8(m);  m
#define MSSEQ_10(m) MSSEQ_9(m);  m
#define MSSEQ_11(m) MSSEQ_10(m); m
#define MSSEQ_12(m) MSSEQ_11(m); m
#define MSSEQ_13(m) MSSEQ_12(m); m
#define MSSEQ_14(m) MSSEQ_13(m); m
#define MSSEQ_15(m) MSSEQ_14(m); m
#define MSSEQ_16(m) MSSEQ_15(m); m

#define MSSEQ_N( n, m ) CONCAT( MSSEQ_, n )( m )

#define UNROLL( expr, n ) \
	MSSEQ_N( n, expr ) \
	/* */

///////////////////////////////////////////////////////////////////////

static 
void ppfilter_init( ppfilter_t* c, sint32 shift )
{
	memset( c, 0, sizeof(ppfilter_t) );
	c->shift = shift;
	c->round = 1 << (shift-1);
}

static always_inline 
void ppfilter_m8( register sint32* a, register const sint32* b ) 
{
//	memcpy( a, b, sizeof(sint32)*ord );
	//for( int n = 0; n != ord; n++ ) *a++ = *b++;
	UNROLL( (*a++ = *b++), 8 );
}

static always_inline 
sint32 ppfilter_encode( ppfilter_t* c, sint32 in )
{
	register sint32* qm = c->qm;
	register sint32* dx = c->dx;
	register sint32* dl = c->dl;
	register sint32  sum= c->round;

    if (!c->error) {
//		for( int n = 0; n != ord; ++n ) sum += *dl++ * *qm, qm++;
//		UNROLL( (sum += *dl++ * *qm, qm++), 8 );
		sum += dl[0] * qm[0]; 
		sum += dl[1] * qm[1]; 
		sum += dl[2] * qm[2]; 
		sum += dl[3] * qm[3]; 
		sum += dl[4] * qm[4]; 
		sum += dl[5] * qm[5]; 
		sum += dl[6] * qm[6]; 
		sum += dl[7] * qm[7]; 
    } else if(c->error < 0) {
//		for( int n = 0; n != ord; ++n ) sum += *dl++ * (*qm -= *dx++), qm++;
//		UNROLL( (sum += *dl++ * (*qm -= *dx++), qm++), 8);
		sum += dl[0] * (qm[0] -= dx[0]);
		sum += dl[1] * (qm[1] -= dx[1]);
		sum += dl[2] * (qm[2] -= dx[2]);
		sum += dl[3] * (qm[3] -= dx[3]);
		sum += dl[4] * (qm[4] -= dx[4]);
		sum += dl[5] * (qm[5] -= dx[5]);
		sum += dl[6] * (qm[6] -= dx[6]);
		sum += dl[7] * (qm[7] -= dx[7]);
    } else {
		//for( int n = 0; n != ord; ++n ) sum += *dl++ * (*qm += *dx++), qm++;
		//UNROLL( (sum += *dl++ * (*qm += *dx++), qm++), 8);
		sum += dl[0] * (qm[0] += dx[0]);
		sum += dl[1] * (qm[1] += dx[1]);
		sum += dl[2] * (qm[2] += dx[2]);
		sum += dl[3] * (qm[3] += dx[3]);
		sum += dl[4] * (qm[4] += dx[4]);
		sum += dl[5] * (qm[5] += dx[5]);
		sum += dl[6] * (qm[6] += dx[6]);
		sum += dl[7] * (qm[7] += dx[7]);
    }
	dl += 8; dx += 8; qm += 8;

    *(dx-0) = ((*(dl-1) >> 30) | 1) << 2;
    *(dx-1) = ((*(dl-2) >> 30) | 1) << 1;
    *(dx-2) = ((*(dl-3) >> 30) | 1) << 1;
    *(dx-3) = ((*(dl-4) >> 30) | 1);

    *dl = in;
    in -= (sum >> c->shift);
    c->error = in;

    *(dl-1) = *dl - *(dl-1);
    *(dl-2) = *(dl-1) - *(dl-2);
    *(dl-3) = *(dl-2) - *(dl-3);

    ppfilter_m8(c->dl, c->dl + 1 );
    ppfilter_m8(c->dx, c->dx + 1 );

	return in;
}

static always_inline 
sint32 ppfilter_decode( ppfilter_t* c, sint32 in )
{
	register sint32* qm = c->qm;
	register sint32* dx = c->dx;
	register sint32* dl = c->dl;
	register sint32  sum= c->round;

    if (!c->error) {
		//for( int n = 0; n != ord; ++n ) sum += *dl++ * *qm, qm++;
		UNROLL( (sum += *dl++ * *qm, qm++), 8 );
        dx += 8;
    } else if(c->error < 0) {
		//for( int n = 0; n != ord; ++n ) sum += *dl++ * (*qm -= *dx++), qm++;
		UNROLL( (sum += *dl++ * (*qm -= *dx++), qm++), 8);
    } else {
		//for( int n = 0; n != ord; ++n ) sum += *dl++ * (*qm += *dx++), qm++;
		UNROLL( (sum += *dl++ * (*qm += *dx++), qm++), 8);
    }

    *(dx-0) = ((*(dl-1) >> 30) | 1) << 2;
    *(dx-1) = ((*(dl-2) >> 30) | 1) << 1;
    *(dx-2) = ((*(dl-3) >> 30) | 1) << 1;
    *(dx-3) = ((*(dl-4) >> 30) | 1);

    c->error = in;
    in += (sum >> c->shift);
    *dl = in;

    *(dl-1) = *dl - *(dl-1);
    *(dl-2) = *(dl-1) - *(dl-2);
    *(dl-3) = *(dl-2) - *(dl-3);

    ppfilter_m8(c->dl, c->dl + 1 );
    ppfilter_m8(c->dx, c->dx + 1 );

	return in;
}

#endif //XAU_PPF_IMPL_H__

