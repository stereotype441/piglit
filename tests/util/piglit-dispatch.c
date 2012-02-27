#include "piglit-util.h"
#include "glxew.h"

typedef void piglit_dispatch_function(void);

static void
get_proc_address_failure(const char *function_name)
{
	printf("GetProcAddress failed for \"%s\"\n", function_name);
	piglit_report_result(PIGLIT_FAIL);
}

static piglit_dispatch_function *
get_core_proc_address(const char *name)
{
	/* TODO: support stuff other than GLX */
	void *function_pointer = (*glXGetProcAddressARB)((const GLubyte *) name);
	if (function_pointer == NULL)
		get_proc_address_failure(name);
	return function_pointer;
}

static piglit_dispatch_function *
get_ext_proc_address(const char *name)
{
	/* TODO: support stuff other than GLX */
	void *function_pointer = (*glXGetProcAddressARB)((const GLubyte *) name);
	if (function_pointer == NULL)
		get_proc_address_failure(name);
	return function_pointer;
}

static void
unsupported(const char *name)
{
	printf("Function \"%s\" not supported on this implementation\n", name);
	piglit_report_result(PIGLIT_SKIP);
}

#include "generated_dispatch.c"

