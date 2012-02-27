#include "piglit-util.h"

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

#include "generated_dispatch.c"
