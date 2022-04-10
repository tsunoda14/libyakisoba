#ifndef BCAS_GLOBALS_H
#define BCAS_GLOBALS_H

#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;

typedef uint64_t u64;
typedef int64_t  s64;

typedef u8 CRC32_t[4];
typedef u8 MAC_t[4];

#define FALSE 0
#define TRUE 1

#endif
