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
}
