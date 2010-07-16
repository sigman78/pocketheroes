//
// bitstream interface (only objects to lower coupling pressure)

#ifndef XBS_BS_H__
#define XBS_BS_H__

#include "xbs.cfg.h"

#ifdef XBS_CPU_LE
//#	define XBS_READER_LE
#endif

struct put_bits_t
{
    uint8 *buf;
    uint8 *end;
    uint32 index;
};

struct get_bits_t
{
    const uint8 *buf;
    const uint8 *end;
	uint32 index;
    uint32 bits;
};


#endif //XBS_BS_H__

