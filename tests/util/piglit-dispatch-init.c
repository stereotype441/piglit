#include "piglit-util.h"
#include "glxew.h"

static void
default_unsupported(const char *name)
{
	printf("Function \"%s\" not supported on this implementation\n", name);
	piglit_report_result(PIGLIT_SKIP);
}

static void
default_get_proc_address_failure(const char *function_name)
{
	printf("GetProcAddress failed for \"%s\"\n", function_name);
	piglit_report_result(PIGLIT_FAIL);
}

static piglit_dispatch_function *
default_get_proc_address(const char *function_name)
{
	/* TODO: support stuff other than GLX */
	return glXGetProcAddressARB((const GLubyte *) function_name);
}

void
glewInit()
{
	static bool already_initialized = false;

	if (already_initialized)
		return;

	piglit_dispatch_init(PIGLIT_DISPATCH_GL,
			     default_get_proc_address,
			     default_get_proc_address,
			     default_unsupported,
			     default_get_proc_address_failure);

	already_initialized = true;
}
