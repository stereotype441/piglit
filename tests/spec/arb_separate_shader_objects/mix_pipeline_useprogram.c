/*
 * Copyright © 2013 Gregory Hainaut <gregory.hainaut@gmail.com>
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

/**
 * \file mix_pipeline_useprogram.
 * Test mixing separable and non-separable programs in various ways.
 *
 * Section 2.11.3 (Program Objects) of the OpenGL 4.1 spec says:
 *
 *     "The executable code for an individual shader stage is taken from the
 *     current program for that stage. If there is a current program object
 *     established by UseProgram, that program is considered current for all
 *     stages. Otherwise, if there is a bound program pipeline object (see
 *     section 2.11.4), the program bound to the appropriate stage of the
 *     pipeline object is considered current. If there is no current program
 *     object or bound program pipeline object, no program is current for any
 *     stage."

 * Section 2.11.4 (Program Pipeline Objects) of the OpenGL 4.1 spec says:
 *
 *     "If no current program object has been established by UseProgram, the
 *     program objects used for each shader stage and for uniform updates are
 *     taken from the bound program pipeline object, if any. If there is a
 *     current program object established by UseProgram, the bound program
 *     pipeline object has no effect on rendering or uniform updates. When a
 *     bound program pipeline object is used for rendering, individual shader
 *     executables are taken from its program objects as described in the
 *     discussion of UseProgram in section 2.11.3)."
 *
 * Section 2.11.7 (Uniform Variables) of the OpenGL 4.1 spec says:
 *
 *     "If a non-zero program object is bound by UseProgram, it is the active
 *     program object whose uniforms are updated by these commands. If no
 *     program object is bound using UseProgram, the active program object of
 *     the current program pipeline object set by ActiveShaderProgram is the
 *     active program object. If the current program pipeline object has no
 *     active program or there is no current program pipeline object, then
 *     there is no active program."
 *
 * Note that with these rules, it's not possible to mix program objects bound
 * to the context with program objects bound to a program pipeline object; if
 * any program is bound to the context, the current pipeline object is
 * ignored.
 */
#include "piglit-util-gl-common.h"

PIGLIT_GL_TEST_CONFIG_BEGIN

	config.supports_gl_compat_version = 10;

	config.window_width = 32;
	config.window_height = 32;

PIGLIT_GL_TEST_CONFIG_END

static GLuint prog;
static GLuint pipe;
static bool pass = true;
static bool subtest = true;

static bool
check(GLenum pname, GLint expected)
{
	GLint value = 0;
	glGetIntegerv(pname, &value);

	pass = piglit_check_gl_error(GL_NO_ERROR) && pass;
	if (value != expected) {
		fprintf(stderr, "Failed to get %s expected %d got %d\n",
			piglit_get_gl_enum_name(pname), expected, value);
		return false;
	} else {
		return true;
	}
}

static void
bind_program(bool enable)
{
	if (enable) {
		glUseProgram(prog);
		if (!piglit_automatic)
			printf("Enable monolithic program\n");
		subtest = check(GL_CURRENT_PROGRAM, prog) && subtest;
	} else {
		glUseProgram(0);
		if (!piglit_automatic)
			printf("Disable monolithic program\n");
		subtest = check(GL_CURRENT_PROGRAM, 0) && subtest;
	}
}

static void
bind_pipeline(bool enable)
{
	if (enable) {
		glBindProgramPipeline(pipe);
		if (!piglit_automatic)
			printf("Bind pipeline\n");
	} else {
		glBindProgramPipeline(0);
		if (!piglit_automatic)
			printf("Unbind pipeline\n");
	}
}

static bool
draw(float expected[4])
{
	piglit_draw_rect(-1, -1, 2, 2);
	return piglit_probe_rect_rgba(0, 0, piglit_width, piglit_height,
				      expected);
}

static bool
set_and_check_uniform(GLint program, float expected)
{
	float value;
	glUniform1f(0, expected);
	glGetUniformfv(program, 0, &value);
	if (value != expected) {
		fprintf(stderr,
			"Failed to get uniform value of %d, expected %f, "
			"got %f\n",
			program, expected, value);
		return false;
	}
	return true;
}

static void
report_subtest(const char* msg)
{
	piglit_report_subtest_result(subtest ? PIGLIT_PASS : PIGLIT_FAIL, msg);

	pass = subtest && pass;
	subtest = true;
}

enum piglit_result
piglit_display(void)
{
	GLint active_shader_pipe;
	GLint uniform_loc_pipe;
	GLint uniform_loc_prog;
	float red[4] = {1.0, 0.0, 0.0, 1.0};
	float green[4] = {0.0, 1.0, 0.0, 1.0};
	float blue[4] = {0.0, 0.0, 1.0, 1.0};
	float gb[4] = {0.0, 1.0, 1.0, 1.0};

	glGetProgramPipelineiv(pipe, GL_ACTIVE_PROGRAM, &active_shader_pipe);
	uniform_loc_pipe = glGetUniformLocation(active_shader_pipe, "blue");
	uniform_loc_prog = glGetUniformLocation(prog, "blue");
	/* otherwise it is difficult which program is really updated */
	assert(uniform_loc_prog == 0);
	assert(uniform_loc_pipe == 0);

	/* wrong setup: stop here */
	if (!pass)
		return PIGLIT_FAIL;

	/* Set up fixed function to draw blue if we lose our shader. */
	glColor4f(0.0, 0.0, 1.0, 1.0);

	/* TEST 1: BindPipeline after UseProgram */
	if (!piglit_automatic)
		printf("glUseProgram, then glBindPipeline...\n");
	bind_program(true);
	subtest = set_and_check_uniform(prog, 1.0) && subtest;
	bind_pipeline(true);
	/* It must ignore the pipeline */
	subtest = set_and_check_uniform(prog, 0.0) && subtest;
	/* But the pipeline is attached to the binding point */
	subtest = check(GL_PROGRAM_PIPELINE_BINDING, pipe) && subtest;

	/* UseProgram rendering */
	subtest = draw(red) && subtest;

	report_subtest("glUseProgram, then glBindPipeline");

	/* TEST 2: BindPipeline without UseProgram */
	if (!piglit_automatic)
		printf("glBindPipeline without glUseProgram...\n");
	bind_program(false);
	bind_pipeline(true);
	subtest = set_and_check_uniform(active_shader_pipe, 1.0) && subtest;
	subtest = check(GL_PROGRAM_PIPELINE_BINDING, pipe) && subtest;

	/* Pipeline rendering  */
	subtest = draw(green) && subtest;

	report_subtest("glBindPipeline without glUseProgram");

	/* TEST 3: UseProgram after BindPipeline */
	if (!piglit_automatic)
		printf("glBindPipeline, then glUseProgram...\n");
	bind_program(true);
	subtest = check(GL_PROGRAM_PIPELINE_BINDING, pipe) && subtest;

	report_subtest("glBindPipeline, then glUseProgram");

	/* UseProgram rendering */
	subtest = draw(red) && subtest;

	/* TEST 4: UseProgram(0) after BindPipeline */
	if (!piglit_automatic)
		printf("glBindPipeline, then glUseProgram(0)...\n");
	bind_program(false);
	bind_pipeline(true);
	subtest = check(GL_PROGRAM_PIPELINE_BINDING, pipe) && subtest;

	/* Sanity check */
	/* Pipeline rendering  */
	subtest = draw(green) && subtest;

	bind_program(false);

	/* Pipeline rendering  */
	subtest = draw(green) && subtest;

	bind_pipeline(true);
	subtest = check(GL_PROGRAM_PIPELINE_BINDING, pipe) && subtest;
	subtest = draw(green) && subtest;

	report_subtest("glBindPipeline, then glUseProgram(0)");

	/* TEST 5: like previous test but use a real program before
	 * UseProgram(0)
	 */
	if (!piglit_automatic)
		printf("glUseProgram, then glBindPipeline, "
		       "then glUseProgram(0)...\n");
	bind_program(false);
	bind_pipeline(true);

	/* Sanity check */
	/* Pipeline rendering  */
	subtest = draw(green) && subtest;

	/* Set wrong uniform value */
	subtest = set_and_check_uniform(active_shader_pipe, 0.0) && subtest;

	bind_program(true);
	bind_program(false);

	/* Pipeline rendering: with bad uniform  */
	subtest = draw(gb) && subtest;

	/* pipeline program must be still active */
	subtest = set_and_check_uniform(active_shader_pipe, 1.0) && subtest;

	bind_pipeline(true);
	/* Pipeline rendering  */
	subtest = draw(green) && subtest;

	report_subtest("glUseProgram, then glBindPipeline, "
		       "then glUseProgram(0)");

	/* TEST 6: Sanity check */
	if (!piglit_automatic)
		printf("Final sanity test...\n");
	bind_program(false);
	bind_pipeline(false);
	/* Fixed function rendering */
	subtest = draw(blue) && subtest;

	report_subtest("Final sanity test");

	piglit_present_results();

	return pass ? PIGLIT_PASS : PIGLIT_FAIL;
}


void
piglit_init(int argc, char **argv)
{
	GLint vs_prog;
	GLint fs_prog;
	GLint vs, fs;
	const char *vs_source =
		"#version 110\n"
		"void main()\n"
		"{\n"
		"	gl_Position = gl_Vertex;\n"
		"}\n";
	const char *fs_source_r =
		"#version 110\n"
		"uniform float blue;\n"
		"void main()\n"
		"{\n"
		"	gl_FragColor = vec4(1.0, 0.0, blue, 1.0);\n"
		"}\n";
	const char *fs_source_g =
		"#version 110\n"
		"uniform float blue;\n"
		"void main()\n"
		"{\n"
		"	gl_FragColor = vec4(0.0, 1.0, 1.0 - blue, 1.0);\n"
		"}\n";

	pass = true;

	piglit_require_gl_version(20);
	piglit_require_extension("GL_ARB_separate_shader_objects");

	/* Standard program (ie not separate) */
	prog = piglit_build_simple_program(vs_source, fs_source_r);

	/* Now create program for the pipeline program */
	vs = piglit_compile_shader_text(GL_VERTEX_SHADER, vs_source);
	fs = piglit_compile_shader_text(GL_FRAGMENT_SHADER, fs_source_g);

	vs_prog = glCreateProgram();
	glProgramParameteri(vs_prog, GL_PROGRAM_SEPARABLE, GL_TRUE);
	glAttachShader(vs_prog, vs);
	glLinkProgram(vs_prog);
	pass = piglit_link_check_status(vs_prog) && pass;
	glDeleteShader(vs);

	fs_prog = glCreateProgram();
	glProgramParameteri(fs_prog, GL_PROGRAM_SEPARABLE, GL_TRUE);
	glAttachShader(fs_prog, fs);
	glLinkProgram(fs_prog);
	pass = piglit_link_check_status(fs_prog) && pass;
	glDeleteShader(fs);

	glGenProgramPipelines(1, &pipe);
	glUseProgramStages(pipe, GL_VERTEX_SHADER_BIT, vs_prog);
	glUseProgramStages(pipe, GL_FRAGMENT_SHADER_BIT, fs_prog);
	glActiveShaderProgram(pipe, fs_prog);
	pass = piglit_program_pipeline_check_status(pipe) && pass;

	pass = check(GL_PROGRAM_PIPELINE_BINDING, 0) && pass;
	pass = check(GL_CURRENT_PROGRAM, 0) && pass;

	pass = piglit_check_gl_error(GL_NO_ERROR) && pass;
}
