//
// Main configuration file
#ifndef XBS_CFG_H__
#define XBS_CFG_H__

// following configs are suggested for different architectures
// ----------------------------- 
// INTEL-PC
// LOOKUPS 			- fast!
// UNALIGNED READS  - possible
//
// ARM / MIPS / SH
// LOOKUPS			- disabled (shifts are free)
// UNALIGNED READS	- do not works


#ifdef OS_WINCE
#if defined(ARM) || defined(_ARM_) || defined(MIPS) || defined(SH3)
//#	define XBS_NO_UNALIGNED_ACCESS
#	define XBS_NOLOOKUP
#	define XBS_UNALIGNED_BYTE_READ
#endif
#endif

// true for pocketPC and X86 - but better perform a check in startup
#define XBS_CPU_LE

#ifndef always_inline
#ifdef NDEBUG
#	define always_inline __forceinline
#else
#	define always_inline __inline
#endif
#endif

#	define XBS_UNALIGNED_BYTE_READ

#endif //XBS_CFG_H__

