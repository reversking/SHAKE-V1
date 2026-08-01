










#ifndef BROTLI_COMMON_TYPES_H_
#define BROTLI_COMMON_TYPES_H_

#include <stddef.h>

#if defined(_MSC_VER) && (_MSC_VER < 1600)
typedef __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
typedef __int64 int64_t;
#else
#include <stdint.h>
#endif




















#define BROTLI_BOOL int

#define BROTLI_TRUE 1

#define BROTLI_FALSE 0

#define TO_BROTLI_BOOL(X) (!!(X) ? BROTLI_TRUE : BROTLI_FALSE)

#define BROTLI_MAKE_UINT64_T(high, low) ((((uint64_t)(high)) << 32) | low)

#define BROTLI_UINT32_MAX (~((uint32_t)0))
#define BROTLI_SIZE_MAX (~((size_t)0))










typedef void* (*brotli_alloc_func)(void* opaque, size_t size);









typedef void (*brotli_free_func)(void* opaque, void* address);

#endif
