//
// Polyphase filter 

#ifndef XAU_PPF_H__
#define XAU_PPF_H__

#include "xau.cfg.h"

#define XAU_FILTER_MAX_ORDER 16

struct ppfilter_t {
	sint32	shift;
	sint32	round;
	sint32	error;
	sint32	mode;

	sint32	qm[XAU_FILTER_MAX_ORDER];
	sint32	dx[XAU_FILTER_MAX_ORDER];
	sint32	dl[XAU_FILTER_MAX_ORDER];
};

#endif //XAU_PPF_H__

