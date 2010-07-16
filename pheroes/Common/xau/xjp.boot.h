#ifndef BOBOGOFJOSDFADADNDSDNKNDQW2323
#define BOBOGOFJOSDFADADNDSDNKNDQW2323

typedef unsigned int uint32;
typedef signed   int sint32;

typedef unsigned short uint16;
typedef signed   short sint16;

typedef unsigned char uint8;
typedef signed char sint8;

#include <assert.h>
#include <string.h>

#ifndef check
#define check assert
#endif

#if defined(__GNUC__)
#	define always_inline __attribute__((always_inline)) inline
#	define long64 long long
#elif ( _MSC_VER+0 > 1 )
#	define always_inline __forceinline
#	define long64 __int64
#	define for if(0) {} else for
#else
#	define always_inline __attribute__((always_inline)) inline
#	define long64 long long
#endif


#endif //BOBOGOFJOSDFADADNDSDNKNDQW2323
