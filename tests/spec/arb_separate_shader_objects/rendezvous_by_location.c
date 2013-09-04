/*
 * Copyright © 2013 Intel Corporation
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

#include "piglit-util-gl-common.h"

PIGLIT_GL_TEST_CONFIG_BEGIN

	config.supports_gl_compat_version = 10;
	config.window_visual = PIGLIT_GL_VISUAL_RGB | PIGLIT_GL_VISUAL_DOUBLE;

PIGLIT_GL_TEST_CONFIG_END

static const char *vs_code =
	"#version 110\n"
	"#extension GL_ARB_separate_shader_objects: require\n"
	"layout(location = 2) out vec3 a;\n"
	"layout(location = 3) out vec3 b;\n"
	"void main()\n"
	"{\n"
	"    gl_Position = gl_Vertex;\n"
	"    a = vec3(0, 0, 1);\n"
	"    b = vec3(1, 0, 0);\n"
	"}\n"
	;

static const char *fs_code =
	"#version 110\n"
	"#extension GL_ARB_separate_shader_objects: require\n"
	"layout(location = 3) in vec3 a;\n"
	"layout(location = 2) in vec3 b;\n"
	"void main()\n"
	"{\n"
	"    gl_FragColor = vec4(cross(a, b), 1);\n"
	"}\n"
	;

enum piglit_result
piglit_display(void)
{
	static const float expected[] = {
		0.0f, 1.0f, 0.0f, 1.0f
	};
	bool pass;

	piglit_draw_rect(-1, -1, 2, 2);
	pass = piglit_probe_rect_rgba(0, 0, piglit_width, piglit_height,
				      expected);

	piglit_present_results();
	return pass ? PIGLIT_PASS : PIGLIT_FAIL;
}

void piglit_init(int argc, char **argv)
{
	GLuint vs_prog;
	GLuint fs_prog;
	GLuint pipeline;

	piglit_require_vertex_shader();
	piglit_require_extension("GL_ARB_separate_shader_objects");

	vs_prog = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vs_code);
	piglit_link_check_status(vs_prog);

	fs_prog = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fs_code);
	piglit_link_check_status(fs_prog);

	glGenProgramPipelines(1, &pipeline);
	glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vs_prog);
	glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fs_prog);
	piglit_program_pipeline_check_status(pipeline);

	if (!piglit_check_gl_error(0))
		piglit_report_result(PIGLIT_FAIL);

	glBindProgramPipeline(pipeline);
}
