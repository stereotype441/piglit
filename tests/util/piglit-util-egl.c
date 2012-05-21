/*
 * Copyright (c) Intel 2011
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * on the rights to use, copy, modify, merge, publish, distribute, sub
 * license, and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
 * VA LINUX SYSTEM, IBM AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "piglit-util-egl.h"

#include <EGL/egl.h>
#include <stdio.h>

#ifdef USE_WAFFLE
#include <waffle/waffle.h>
#endif

const char* piglit_get_egl_error_name(EGLint error) {
#define CASE(x) case x: return #x;
    switch (error) {
    CASE(EGL_SUCCESS)

    CASE(EGL_BAD_ACCESS)
    CASE(EGL_BAD_ALLOC)
    CASE(EGL_BAD_ATTRIBUTE)
    CASE(EGL_BAD_CONFIG)
    CASE(EGL_BAD_CONTEXT)
    CASE(EGL_BAD_CURRENT_SURFACE)
    CASE(EGL_BAD_DISPLAY)
    CASE(EGL_BAD_MATCH)
    CASE(EGL_BAD_NATIVE_PIXMAP)
    CASE(EGL_BAD_NATIVE_WINDOW)
    CASE(EGL_BAD_PARAMETER)
    CASE(EGL_BAD_SURFACE)
    CASE(EGL_CONTEXT_LOST)
    CASE(EGL_NOT_INITIALIZED)
    default:
        return "(unrecognized error)";
    }
#undef CASE
}

void piglit_expect_egl_error(EGLint expected_error, enum piglit_result result)
{
	EGLint actual_error;

	actual_error = eglGetError();
	if (actual_error == expected_error) {
		return;
	}

	/*
	 * If the lookup of the error's name is successful, then print
	 *     Unexpected EGL error: NAME 0xHEX
	 * Else, print
	 *     Unexpected EGL error: 0xHEX
	 */
	printf("Unexpected EGL error: %s 0x%x\n",
               piglit_get_egl_error_name(actual_error), actual_error);

	/* Print the expected error, but only if an error was really expected. */
	if (expected_error != EGL_SUCCESS) {
		printf("Expected EGL error: %s 0x%x\n",
		piglit_get_gl_error_name(expected_error), expected_error);
        }

	piglit_report_result(result);
}

bool
piglit_is_egl_extension_supported(const char *name)
{
	const char * egl_extension_list;
	EGLDisplay dpy;

	dpy = eglGetCurrentDisplay();

	egl_extension_list = eglQueryString(dpy, EGL_EXTENSIONS);

	return piglit_is_extension_in_string(egl_extension_list, name);
}

void
piglit_require_egl_extension(const char *name)
{
#ifdef USE_WAFFLE
	if (glutGetPlatform() != WAFFLE_PLATFORM_X11_EGL &&
		glutGetPlatform() != WAFFLE_PLATFORM_ANDROID) {
		fprintf(stderr, "EGL test running in platform without EGL\n");
		piglit_report_result(PIGLIT_SKIP);
	}
#endif

	if (!piglit_is_egl_extension_supported(name)) {
		fprintf(stderr, "Test requires %s\n", name);
		piglit_report_result(PIGLIT_SKIP);
	}
}


bool
piglit_check_egl_version(int major, int minor)
{
	int eglMajor;
	int eglMinor;
	const char *egl_version;
	EGLDisplay dpy;

#ifdef USE_WAFFLE
	if (glutGetPlatform() != WAFFLE_PLATFORM_X11_EGL &&
		glutGetPlatform() != WAFFLE_PLATFORM_ANDROID) {
		fprintf(stderr, "EGL test running in platform without EGL\n");
		piglit_report_result(PIGLIT_SKIP);
	}
#endif

	dpy = eglGetCurrentDisplay();
	egl_version = eglQueryString(dpy, EGL_VERSION);

	if (sscanf(egl_version, "%d.%d", &eglMajor, &eglMinor) != 2) {
		fprintf(stderr, "Failed to parse EGL version\n");
		piglit_report_result(PIGLIT_FAIL);
	}

	if (eglMajor != major || eglMinor < minor) {
		fprintf(stderr, "Test requires EGL %d.%d.  Got %d.%d.\n",
			major, minor, eglMajor, eglMinor);
		return false;
	}
	return true;
}

void
piglit_require_egl_version(int major, int minor)
{
	if (!piglit_check_egl_version(major, minor))
		piglit_report_result(PIGLIT_SKIP);
}
