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

#include "generated_dispatch.c"

