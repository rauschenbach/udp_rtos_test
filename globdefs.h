#ifndef _GLOBDEFS_H
#define _GLOBDEFS_H

#include <signal.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <float.h>


typedef uint64_t u64;
typedef int64_t s64;

typedef uint32_t u32;
typedef int32_t s32;


typedef uint16_t u16;
typedef int16_t s16;

typedef uint8_t u8;
typedef int8_t s8;
typedef char   c8;



/* long double */
#ifndef f32
#define f32 float
#endif

#ifndef bool
#define bool uint8_t
#endif


#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif



#ifndef IDEF
#define IDEF static inline
#endif

#define IAR_LOG_PRINTF
//#undef IAR_LOG_PRINTF


 /* �����������-����� � ������� IAR */
#if defined 	IAR_LOG_PRINTF
	#define 	log_printf  printf
#endif


/* ��������������� malloc � free � printf */
#define		MALLOC(x)	pvPortMalloc(x)
#define		FREE(x)	        vPortFree(x)

#endif				/* globdefs.h */
