//
// Two 'K' skewed rice coder. 
// Has decent performance on distributions close to laplacian 
// 2007.12.20 : SiGMan - bs refactored to separate source
// 
#ifndef XAU_RICE_H__
#define XAU_RICE_H__

#include "xau.cfg.h"
#include "xbs.bs.h"

struct rice_param_t
{
	uint32	k;
	uint32	sum;
};

typedef rice_param_t rice_ctx_t[2];

#endif //XAU_RICE_H__

