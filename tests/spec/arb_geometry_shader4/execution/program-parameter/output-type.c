/*
 * Copyright © 2013 The Piglit project
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
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/**
 * \file output-type.c
 *
 * Test required errors for wrong GL_GEOMETRY_OUTPUT_TYPE parameters.
 *
 * From the ARB_geometry_shader4 spec (section Errors):
 * "The error INVALID_VALUE is generated by ProgramParameteriARB if <pname> is
 * GEOMETRY_OUTPUT_TYPE_ARB and <value> is not one of POINTS, LINE_STRIP or
 * TRIANGLE_STRIP."
 */

#include "common.h"


/* Primitive types passed as geometry shader output type and expected error. */
static const struct primitive_geom_info primitives_out[] = {
	{GL_POINTS, GL_NO_ERROR},

	{GL_LINES, GL_INVALID_VALUE},
	{GL_LINE_STRIP, GL_NO_ERROR},
	{GL_LINE_LOOP, GL_INVALID_VALUE},

	{GL_TRIANGLES, GL_INVALID_VALUE},
	{GL_TRIANGLE_STRIP, GL_NO_ERROR},
	{GL_TRIANGLE_FAN, GL_INVALID_VALUE},

	{GL_LINES_ADJACENCY, GL_INVALID_VALUE},
	{GL_LINE_STRIP_ADJACENCY, GL_INVALID_VALUE},

	{GL_TRIANGLES_ADJACENCY, GL_INVALID_VALUE},
	{GL_TRIANGLE_STRIP_ADJACENCY, GL_INVALID_VALUE},

	{GL_QUADS, GL_INVALID_VALUE},
	{GL_QUAD_STRIP, GL_INVALID_VALUE},
	{GL_POLYGON, GL_INVALID_VALUE},
};


PIGLIT_GL_TEST_CONFIG_BEGIN
	config.supports_gl_compat_version = 20;
	config.window_visual = PIGLIT_GL_VISUAL_DOUBLE | PIGLIT_GL_VISUAL_RGBA;
PIGLIT_GL_TEST_CONFIG_END


void
piglit_init(int argc, char **argv)
{
	bool pass = true;
	GLuint prog;
	int i;

	piglit_require_extension("GL_ARB_geometry_shader4");

	/* Create shader. */
	prog = create_shader(vs_text, gs_text, fs_text);

	for (i = 0; i < ARRAY_SIZE(primitives_out); i++) {
		const struct primitive_geom_info p = primitives_out[i];
		printf("Testing %s.\n", piglit_get_prim_name(p.type));
		glProgramParameteri(prog, GL_GEOMETRY_OUTPUT_TYPE_ARB, p.type);
		pass = piglit_check_gl_error(p.error) && pass;
	}

	piglit_report_result(pass ? PIGLIT_PASS : PIGLIT_FAIL);
}


enum piglit_result
piglit_display(void)
{
	/* Should never be reached */
	return PIGLIT_FAIL;
}