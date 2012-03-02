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
#include "glxew.h"

/**
 * Generated code calls this function if the test tries to use a GL
 * function that is not supported on the current implementation.
 *
 * This function terminates the test with a SKIP; this saves the
 * piglit test from the burden of having to pre-check whether the
 * implementation supports the functionality being tested.
 */
static void
default_unsupported(const char *name)
{
	printf("Function \"%s\" not supported on this implementation\n", name);
	piglit_report_result(PIGLIT_SKIP);
}

/**
 * Generated code calls this function if a call to GetProcAddress()
 * returns NULL.
 *
 * We don't expect this to ever happen, since we only call
 * GetProcAddress() for functions that the implementation claims to
 * support.  So if it does happen we terminate the test with a FAIL.
 */
static void
default_get_proc_address_failure(const char *function_name)
{
	printf("GetProcAddress failed for \"%s\"\n", function_name);
	piglit_report_result(PIGLIT_FAIL);
}

/**
 * Generated code calls this function to retrieve the address of a
 * function from the implementation.
 *
 * For the moment we only support GLX.  In the future there will be
 * versions of this function for EGL, Windows, and Apple.
 */
static piglit_dispatch_function *
default_get_proc_address(const char *function_name)
{
	/* TODO: support stuff other than GLX */
	return glXGetProcAddressARB((const GLubyte *) function_name);
}

/**
 * Initialize the GL dispatch mechanism to a default configuration.
 *
 * Eventually we will want to replace this with code that initializes
 * the GL dispatch mechanism based on run-time parameters (e.g. to
 * select X vs Wayland, or desktop GL vs GLES).
 *
 * This function is safe to call multiple times--it only has an effect
 * on the first call.
 */
void
piglit_dispatch_default_init()
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
