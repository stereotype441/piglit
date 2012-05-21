
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)
#include <windows.h>
#endif

#if defined(_MSC_VER)
typedef signed char int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#ifndef __cplusplus
#define bool BOOL
#define true 1
#define false 0
#endif
#define log2(x) (log(x) / log(2))
#else
#include <stdint.h>
#include <stdbool.h>
#endif

enum piglit_result {
	PIGLIT_PASS,
	PIGLIT_FAIL,
	PIGLIT_SKIP,
	PIGLIT_WARN
};

void piglit_report_result(enum piglit_result result);

#ifndef _WIN32

/* APIENTRY and GLAPIENTRY are not used on Linux or Mac. */
#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef GLAPIENTRY
#define GLAPIENTRY
#endif

#else

#ifndef APIENTRY
#define APIENTRY __stdcall
#endif

#ifndef GLAPIENTRY
#define GLAPIENTRY APIENTRY
#endif

#ifndef GLAPI
#define GLAPI extern
#endif

#endif

typedef enum {
	PIGLIT_DISPATCH_GL,
	PIGLIT_DISPATCH_GL_FWD,
	PIGLIT_DISPATCH_ES1,
	PIGLIT_DISPATCH_ES2
} piglit_dispatch_api;

typedef void (APIENTRY *piglit_dispatch_function_ptr)(void);

typedef piglit_dispatch_function_ptr (*piglit_get_core_proc_address_function_ptr)(const char *, int);

typedef piglit_dispatch_function_ptr (*piglit_get_ext_proc_address_function_ptr)(const char *);

typedef piglit_dispatch_function_ptr (*piglit_dispatch_resolver_ptr)();

typedef void (*piglit_error_function_ptr)(const char *);

void piglit_dispatch_init(piglit_dispatch_api api,
			  piglit_get_core_proc_address_function_ptr get_core_proc,
			  piglit_get_ext_proc_address_function_ptr get_ext_proc,
			  piglit_error_function_ptr unsupported_proc,
			  piglit_error_function_ptr failure_proc);

#ifdef __cplusplus
}
#endif
