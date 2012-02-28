/* Copyright 2012 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "piglit-util.h"

static piglit_get_proc_address_function *__get_core_proc_address = NULL;
static piglit_get_proc_address_function *__get_ext_proc_address = NULL;
static piglit_error_function *__unsupported = NULL;
static piglit_error_function *__get_proc_address_failure = NULL;
static int __gl_version = 0;
static const char *__gl_extensions = NULL;
static bool __is_initialized = false;

static void
__check_initialized()
{
	if (__is_initialized)
		return;

	printf("piglit_dispatch_init() must be called before GL functions\n");
	assert(false);
	exit(1);
}

static piglit_dispatch_function *
__get_core_proc(const char *name)
{
	void *function_pointer = __get_core_proc_address(name);
	if (function_pointer == NULL)
		__get_proc_address_failure(name);
	return function_pointer;
}

static piglit_dispatch_function *
__get_ext_proc(const char *name)
{
	void *function_pointer = __get_ext_proc_address(name);
	if (function_pointer == NULL)
		__get_proc_address_failure(name);
	return function_pointer;
}

static inline bool
__check_version(int required_version)
{
	return __gl_version >= required_version;
}

static inline bool
__check_extension(const char *name)
{
	return piglit_is_extension_in_string(__gl_extensions, name);
}

#include "generated_dispatch.c"

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

	initialize_dispatch_pointers();

	__is_initialized = true;

	/* Store the GL version and extension string for use by
	 * __check_version() and __check_extension().  Note: the
	 * following two calls are safe because the only GL function
	 * they call is glGetString(), and the stub function for
	 * glGetString does not need to call __check_version() or
	 * __check_extension().
	 */
	__gl_version = piglit_get_gl_version();
	__gl_extensions = (const char *) glGetString(GL_EXTENSIONS);
}
