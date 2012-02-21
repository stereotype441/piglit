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

static void *
get_proc_address(char *name)
{
	/* TODO: support stuff other than GLX */
	void *function_pointer = (*glXGetProcAddressARB)((const GLubyte *) name);
	if (function_pointer == NULL) {
		printf("GetProcAddress failed for \"%s\"\n", name);
		piglit_report_result(PIGLIT_FAIL);
	}
	return function_pointer;
}

static void
unsupported(char *name)
{
	printf("Function \"%s\" not supported on this implementation\n", name);
	piglit_report_result(PIGLIT_SKIP);
}

#include "wrappers.c"

