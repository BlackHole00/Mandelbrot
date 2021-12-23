#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint8_t byte;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#ifdef _USE_FLOAT16
#include <float16.h>
typedef float16 f16;
#endif

typedef float f32;
typedef double f64;

typedef size_t usize;

#ifdef __ssize_t_defined
typedef ssize_t isize;
#endif

typedef char* rawstr;

#define f16_from_i32(_INT) f16_from_int(_INT)
