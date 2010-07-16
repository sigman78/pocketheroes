//
// basic dct / idct transfroms
// credits goes to Pascal Massimino
// based on "Fast and precise" LLM implementation of FDCT/IDCT paper
// 2007-12-07 : SiGMan - initial revision

#ifndef XJP_DCT_H__
#define XJP_DCT_H__

#include "xbs.cfg.h"

namespace xjp {

typedef sint16 dpel_t;

// inplace fdct transformation
void fdct16( dpel_t* val );
// inplace idct transformation
void idct16( dpel_t* val );


} //xjp

#endif //XJP_DCT_H__

