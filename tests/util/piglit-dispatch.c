#include "piglit-util.h"
#include "glxew.h"

static piglit_get_proc_address_function *__get_core_proc_address = NULL;
static piglit_get_proc_address_function *__get_ext_proc_address = NULL;
static piglit_error_function *__unsupported = NULL;
static piglit_error_function *__get_proc_address_failure = NULL;

static piglit_dispatch_function *
get_core_proc_address(const char *name)
{
	void *function_pointer = __get_core_proc_address(name);
	if (function_pointer == NULL)
		__get_proc_address_failure(name);
	return function_pointer;
}

static piglit_dispatch_function *
get_ext_proc_address(const char *name)
{
	void *function_pointer = __get_ext_proc_address(name);
	if (function_pointer == NULL)
		__get_proc_address_failure(name);
	return function_pointer;
}

#include "generated_dispatch.c"

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
	return glXGetProcAddressARB(function_name);
}

void
piglit_dispatch_init(piglit_dispatch_api api,
		     piglit_get_proc_address_function *get_core_proc,
		     piglit_get_proc_address_function *get_ext_proc,
		     piglit_error_function *unsupported_proc,
		     piglit_error_function *failure_proc)
{
	/* Note: at the moment the only supported API is
	 * PIGLIT_DISPATCH_GL.
	 */
	(void) api;

	__get_core_proc_address = get_core_proc;
	__get_ext_proc_address = get_ext_proc;
	__unsupported = unsupported_proc;
	__get_proc_address_failure = failure_proc;
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
