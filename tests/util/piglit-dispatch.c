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

/* Global state maintained by the Piglit dispatch mechanism: */

/**
 * Which function to call to get the address of a core function.
 */
static piglit_get_proc_address_function *__get_core_proc_address = NULL;

/**
 * Which function to call to get the address of a function defined in
 * an extension.
 */
static piglit_get_proc_address_function *__get_ext_proc_address = NULL;

/**
 * Which function to call if the test attempts to call a function that
 * is not supported by the implementation.
 */
static piglit_error_function *__unsupported = NULL;

/**
 * Which function to call if __get_core_proc_address or
 * __get_ext_proc_address returns NULL.
 */
static piglit_error_function *__get_proc_address_failure = NULL;

/**
 * The GL version extracted from glGetString(GL_VERSION), times 10.
 * For example, if the GL version is 2.1, the value 21 is stored here.
 *
 * We cache this here because calling glGetString is prohibited
 * between glBegin and glEnd, and to avoid the inefficiency of
 * redundant glGetString queries.
 */
static int __gl_version = 0;

/**
 * The GL extension string returned by glGetString(GL_EXTENSIONS).
 *
 * We cache this here because calling glGetString is prohibited
 * between glBegin and glEnd, and to avoid the inefficiency of
 * redundant glGetString queries.
 */
static const char *__gl_extensions = NULL;

/**
 * True if piglit_dispatch_init has been called.
 */
static bool __is_initialized = false;

/**
 * Generated code calls this function to verify that the dispatch
 * mechanism has been properly initialized.
 */
static void
__check_initialized()
{
	if (__is_initialized)
		return;

	printf("piglit_dispatch_init() must be called before GL functions\n");
	assert(false);
	exit(1);
}

/**
 * Generated code calls this function to retrieve the address of a
 * core function.
 */
static piglit_dispatch_function *
__get_core_proc(const char *name)
{
	void *function_pointer = __get_core_proc_address(name);
	if (function_pointer == NULL)
		__get_proc_address_failure(name);
	return function_pointer;
}

/**
 * Generated code calls this function to retrieve the address of a
 * function defined in an extension.
 */
static piglit_dispatch_function *
__get_ext_proc(const char *name)
{
	void *function_pointer = __get_ext_proc_address(name);
	if (function_pointer == NULL)
		__get_proc_address_failure(name);
	return function_pointer;
}

/**
 * Generated code calls this function to determine whether a given GL
 * version is supported.
 */
static inline bool
__check_version(int required_version)
{
	return __gl_version >= required_version;
}

/**
 * Generated code calls this function to determine whether a given
 * extension is supported.
 */
static inline bool
__check_extension(const char *name)
{
	return piglit_is_extension_in_string(__gl_extensions, name);
}

#include "generated_dispatch.c"

/**
 * Initialize the dispatch mechanism.
 *
 * - api is the API under test.  This determines whether deprecated
 *   functionality is supported (since deprecated functions cannot be
 *   used in forward compatible contexts).  It also affects which GL
 *   version is queried for (since, for example a function might be
 *   supported in GLES as of version 2.0, but in OpenGL only as of
 *   version 2.1).  Not yet implemented.
 *
 * - get_core_proc and get_ext_proc are the functions to call to
 *   retrieve the address of a core GL function or an extension
 *   function.  Note that for OpenGL, these can both map to the same
 *   function (e.g. glXGetProcAddressARB).  However, in GLES, core
 *   functions are not allowed to be queried using the GetProcAddress
 *   mechanism, so get_core_proc will need to be implemented by
 *   looking up a symbol in a shared library (e.g. using dlsym()).
 *
 * - unsupported_proc is the function to call if a test attempts to
 *   use unsupported GL functionality.  It is passed the name of the
 *   function that the test attempted to use.
 *
 * - failure_proc is the function to call if a call to get_core_proc()
 *   or get_ext_proc() unexpectedly returned NULL.  It is passed the
 *   name of the function that was passed to get_core_proc() or
 *   get_ext_proc().
 */
void
piglit_dispatch_init(piglit_dispatch_api api,
		     piglit_get_proc_address_function *get_core_proc,
		     piglit_get_proc_address_function *get_ext_proc,
		     piglit_error_function *unsupported_proc,
		     piglit_error_function *failure_proc)
{
	(void) api; /* Not yet implemented--assume GL. */

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
