#if defined(_WIN32)
#include <windows.h>
#endif

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "config.h"
#if defined(HAVE_SYS_TIME_H) && defined(HAVE_SYS_RESOURCE_H) && defined(HAVE_SETRLIMIT)
#include <sys/time.h>
#include <sys/resource.h>
#define USE_SETRLIMIT
#endif

#include "piglit-util.h"
#include "glxew.h"

// TODO: it seems bad to call this "glGetProcAddress".
#if defined(_WIN32)
#  define glGetProcAddress(name) wglGetProcAddress((LPCSTR)name)
#else
#  if defined(__APPLE__)
#    define glGetProcAddress(name) NSGLGetProcAddress(name)
#  else
#    if defined(__sgi) || defined(__sun)
#      define glGetProcAddress(name) dlGetProcAddress(name)
#    else /* __linux */
#      define glGetProcAddress(name) (*glXGetProcAddressARB)(name)
#    endif
#  endif
#endif

#include "wrappers.c"

