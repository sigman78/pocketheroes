//
// 2007-01-09 : SiGMan - ARM profiling, few local speedups

#ifndef XBS_BS_IMPL_H__
#define XBS_BS_IMPL_H__

#include "xbs.bswap.h"
#include "xbs.bs.h"

//
/// put bits ////////////////////////////////////////////

static always_inline 
void init_put_bits(put_bits_t *s, uint8 *buffer, size_t bufferSize)
{
    s->buf = buffer;
    s->end = s->buf + bufferSize;
    s->index=0;
    ((uint32*)(s->buf))[0]=0;
}

static always_inline 
uint32 put_bits_count(put_bits_t *s)
{
    return s->index;
}

static void align_put_bits(put_bits_t *s);

static always_inline
void flush_put_bits(put_bits_t *s)
{
    align_put_bits(s);
}

static always_inline 
void put_bits(put_bits_t *s, uint32 n, uint32 value)
{
    register int index= s->index;
    uint32 *ptr= (uint32*)(((uint8*)s->buf)+(index>>3));

    ptr[0] |= be2me_32(value<<(32-n-(index&7) ));
    ptr[1] = 0;
    index+= n;
    s->index= index;
}

static always_inline 
void skip_put_bits(put_bits_t *s, uint32 n){
    s->index += n;
}

static always_inline 
void set_put_bits_buffer_size(put_bits_t *s, size_t size){
    s->end= s->buf + size;
}

static inline 
void align_put_bits(put_bits_t *s)
{
    put_bits(s, (uint32)(- (sint32)s->index) & 7,0);
}

//
/// get bits ////////////////////////////////////////////////

#define MIN_CACHE_BITS 25

#   define OPEN_READER(name, gb)\
        uint32 name##_index= (gb)->index;\
        uint32 name##_cache= 0;\
        /* */

#   define CLOSE_READER(name, gb)\
        (gb)->index= name##_index;\
		/* */

#ifdef XBS_READER_LE
#   define UPDATE_CACHE(name, gb)\
        name##_cache= unaligned32_le( ((const uint8*)(gb)->buf)+(name##_index>>3) ) >> (name##_index&0x07);\
        /* */

#   define SKIP_CACHE(name, gb, num)\
        name##_cache >>= (num); \
        /* */
#else //XBS_READER_LE
#   define UPDATE_CACHE(name, gb)\
        name##_cache= unaligned32_be( ((const uint8*)(gb)->buf)+(name##_index>>3) ) << (name##_index&0x07);\
        /* */

#   define SKIP_CACHE(name, gb, num)\
        name##_cache <<= (num); \
        /* */
#endif //XBS_READER_LE

///////////////////////////////////////////////////////

#   define SKIP_COUNTER(name, gb, num)\
        name##_index += (num);\
        /* */

#   define SKIP_BITS(name, gb, num)\
        {\
            SKIP_CACHE(name, gb, num)\
            SKIP_COUNTER(name, gb, num)\
        }\
        /* */

#   define LAST_SKIP_BITS(name, gb, num) SKIP_COUNTER(name, gb, num)
#   define LAST_SKIP_CACHE(name, gb, num) ;

#ifdef XBS_READER_LE
#   define SHOW_UBITS(name, gb, num)\
        ((name##_cache) & (NEG_USR32(0xffffffff,num))) \
        /* */

#   define SHOW_SBITS(name, gb, num)\
        NEG_SSR32((name##_cache)<<(32-(num)), num) \
        /* */
#else //XBS_READER_LE
#   define SHOW_UBITS(name, gb, num)\
        NEG_USR32(name##_cache, num)\
        /* */

#   define SHOW_SBITS(name, gb, num)\
        NEG_SSR32(name##_cache, num)\
        /* */
#endif //XBS_READER_LE

#   define GET_CACHE(name, gb)\
        ((uint32)name##_cache)\
        /* */


static always_inline 
uint32 get_bits_count(get_bits_t *s){
    return s->index;
}

static always_inline 
void skip_bits_long(get_bits_t *s, uint32 n){
    s->index += n;
}

static always_inline 
uint32 get_bits(get_bits_t *s, uint32 n){
    uint32 tmp;
    OPEN_READER(re, s)
    UPDATE_CACHE(re, s)
    tmp= SHOW_UBITS(re, s, n);
    LAST_SKIP_BITS(re, s, n)
    CLOSE_READER(re, s)
    return tmp;
}

static always_inline 
uint32 show_bits(get_bits_t *s, uint32 n){
    uint32 tmp;
    OPEN_READER(re, s)
    UPDATE_CACHE(re, s)
    tmp= SHOW_UBITS(re, s, n);
//    CLOSE_READER(re, s)
    return tmp;
}

static always_inline 
void skip_bits(get_bits_t *s, uint32 n){
    OPEN_READER(re, s)
    UPDATE_CACHE(re, s)
    LAST_SKIP_BITS(re, s, n)
    CLOSE_READER(re, s)
}

static always_inline 
uint32 get_bits1(get_bits_t *s){
    uint32 index= s->index;
    uint8 result= s->buf[ index>>3 ];
#ifdef XBS_READER_LE
    result>>= (index&0x07);
    result&= 1;
#else
    result<<= (index&0x07);
    result>>= 8 - 1;
#endif
    index++;
    s->index= index;

    return result;
}

static always_inline 
uint32 show_bits1(get_bits_t *s){
    return show_bits(s, 1);
}

static always_inline 
void skip_bits1(get_bits_t *s){
    skip_bits(s, 1);
}

static inline
void init_get_bits(get_bits_t *s,
                   const uint8 *buffer, size_t bit_size)
{
    size_t buffer_size= (bit_size+7)>>3;

    if(buffer_size == 0 || bit_size == 0) {
        buffer_size = bit_size = 0;
        buffer = NULL;
    }

    s->buf	= buffer;
    s->bits	= bit_size;
    s->end	= buffer + buffer_size;
    s->index=0;
}

static inline 
void align_get_bits(get_bits_t *s)
{
    uint32 n= (uint32)(-(sint32)get_bits_count(s)) & 7;
    if(n) skip_bits(s, n);
}


#endif //XBS_BS_IMPL_H__


