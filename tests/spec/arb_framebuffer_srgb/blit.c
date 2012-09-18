/*
 * Copyright © 2012 Intel Corporation
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

/** \file blit.c
 *
 * Measure the sRGB behaviour of blits, varying the following variables:
 *
 * - Source framebuffer type (linear, sRGB, or window system)
 * - Source framebuffer sample count (single-sampled or multisampled)
 * - Destination framebuffer type (linear, sRGB, or window system)
 * - Destination framebuffer sample count (single-sampled or multisampled)
 * - FRAMEBUFFER_SRGB setting (enabled or disabled)
 * - Blit type (stretched or 1:1)
 */

#include "piglit-util-gl-common.h"

const int PATTERN_WIDTH = 256;
const int PATTERN_HEIGHT = 64;

PIGLIT_GL_TEST_MAIN(2 * PATTERN_WIDTH, PATTERN_HEIGHT,
		    GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);

enum hypothesis_enum {
	HYPOTHESIS_DIRECT_COPY,
	HYPOTHESIS_SRGB_DECODE,
	HYPOTHESIS_SRGB_ENCODE,
	HYPOTHESIS_NO_ACTION,
};

static char *hypothesis_names[] = {
	"Direct copy",
	"SRGB decode",
	"SRGB encode",
	"No action"
};

static const unsigned num_hypotheses = 4;

static GLuint linear_fbo;
static GLuint srgb_fbo;
static GLuint linear_msaa_fbo;
static GLuint srgb_msaa_fbo;
static GLint prog;

static char *vs_text = \
	"#version 120\n"
	"void main()\n"
	"{\n"
	"  gl_Position = gl_Vertex;\n"
	"}\n";

static char *fs_text = \
	"#version 120\n"
	"void main()\n"
	"{\n"
	"  float x = gl_FragCoord.x;\n"
	"  gl_FragColor = vec4((x - 0.5) / 255.0);\n"
	"}\n";

static GLuint
setup_fbo(GLenum internalformat, GLsizei num_samples)
{
	GLuint fbo, rb;
	glGenFramebuffers(1, &fbo);
	glGenRenderbuffers(1, &rb);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rb);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, num_samples,
					 internalformat, 2 * PATTERN_WIDTH,
					 PATTERN_HEIGHT);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				  GL_RENDERBUFFER, rb);
	return fbo;
}

void
piglit_init(int argc, char **argv)
{
	GLint vs, fs;

	piglit_require_gl_version(21);
	piglit_require_extension("ARB_framebuffer_object");
	piglit_require_extension("ARB_framebuffer_sRGB");

	vs = piglit_compile_shader_text(GL_VERTEX_SHADER, vs_text);
	fs = piglit_compile_shader_text(GL_FRAGMENT_SHADER, fs_text);
	prog = piglit_link_simple_program(vs, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

	linear_fbo = setup_fbo(GL_RGBA, 0);
	srgb_fbo = setup_fbo(GL_SRGB8_ALPHA8, 0);
	linear_msaa_fbo = setup_fbo(GL_RGBA, 1);
	srgb_msaa_fbo = setup_fbo(GL_SRGB8_ALPHA8, 1);
}

static int
compute_expected(int value, enum hypothesis_enum hypothesis)
{
	double value_floating = value / 255.0;

	switch (hypothesis) {
	case HYPOTHESIS_DIRECT_COPY:
		return value;
	case HYPOTHESIS_SRGB_ENCODE:
		if (value_floating <= 0) {
			value_floating = 0.0;
		} else if (value_floating < 0.0031308) {
			value_floating *= 12.92;
		} else if (value_floating < 1) {
			value_floating = 1.055 * pow(value_floating, 0.41666)
				- 0.055;
		} else {
			value_floating = 1.0;
		}
		return (int) round(value_floating * 255.0);
	case HYPOTHESIS_SRGB_DECODE:
		if (value_floating <= 0.04045) {
			value_floating /= 12.92;
		} else {
			value_floating = pow((value_floating + 0.055)/1.055,
					     2.4);
		}
		return (int) round(value_floating * 255.0);
	case HYPOTHESIS_NO_ACTION:
		return 0;
	}
	printf("Unexpected hypothesis\n");
	piglit_report_result(PIGLIT_FAIL);
	return 0;
}

static double
test_hypothesis(GLubyte *data, enum hypothesis_enum hypothesis)
{
	double sum_squared_error = 0.0;
	unsigned x, y, component;
	unsigned count;
	for (y = 0; y < PATTERN_HEIGHT; ++y) {
		for (x = 0; x < PATTERN_WIDTH; ++x) {
			for (component = 0; component < 3; ++component) {
				int expected_value =
					compute_expected(x, hypothesis);
				int actual_value = data[(y * PATTERN_WIDTH + x)
							* 4 + component];
				double error = expected_value - actual_value;
				sum_squared_error += error * error;
				++count;
			}
		}
	}

	return sqrt(sum_squared_error / count);
}

static void
analyze_image(GLuint fbo, int x_offset)
{
	GLubyte *data = malloc(PATTERN_WIDTH * PATTERN_HEIGHT * 4);
	unsigned i;
	double best_rms_error;
	enum hypothesis_enum best_hypothesis;
	GLuint single_sampled_fbo = 0;
	if (fbo == linear_msaa_fbo)
		single_sampled_fbo = linear_fbo;
	else if (fbo == srgb_msaa_fbo)
		single_sampled_fbo = srgb_fbo;
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	if (single_sampled_fbo) {
		/* We need to do a resolve before we can read pixels */
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, single_sampled_fbo);
		glClear(GL_COLOR_BUFFER_BIT);
		glBlitFramebuffer(0, 0, 2 * PATTERN_WIDTH, PATTERN_HEIGHT,
				  0, 0, 2 * PATTERN_WIDTH, PATTERN_HEIGHT,
				  GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, single_sampled_fbo);
	}
	glReadPixels(x_offset, 0, PATTERN_WIDTH, PATTERN_HEIGHT, GL_RGBA,
		     GL_UNSIGNED_BYTE, data);
	for (i = 0; i < num_hypotheses; ++i) {
		double rms_error = test_hypothesis(data, i);
		if (i == 0 || rms_error < best_rms_error) {
			best_rms_error = rms_error;
			best_hypothesis = i;
		}
	}
	printf("%s (error=%f)", hypothesis_names[best_hypothesis],
	       best_rms_error);
}

static void
test_readback(GLuint fbo, char *fbo_name)
{
	printf("Testing readback in %s fbo: ", fbo_name);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT);
	piglit_draw_rect(-1, -1, 1, 2);
	analyze_image(fbo, 0);
	printf("\n");
}

static void
test_blit(GLuint src_fbo, char *src_fbo_name,
	  GLuint dst_fbo, char *dst_fbo_name,
	  bool srgb_enable, bool scaled)
{
	printf("Testing %s blit %s -> %s (FRAMEBUFFER_SRGB %s): ",
	       scaled ? "scaled" : "1:1",
	       src_fbo_name, dst_fbo_name,
	       srgb_enable ? "enabled" : "disabled");

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst_fbo);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, src_fbo);
	glClear(GL_COLOR_BUFFER_BIT);
	piglit_draw_rect(-1, -1, 1, 2);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, src_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst_fbo);
	if (srgb_enable)
		glEnable(GL_FRAMEBUFFER_SRGB);
	glBlitFramebuffer(0, 0, PATTERN_WIDTH, scaled ? 1 : PATTERN_HEIGHT,
			  PATTERN_WIDTH, 0, PATTERN_WIDTH * 2, PATTERN_HEIGHT,
			  GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glDisable(GL_FRAMEBUFFER_SRGB);
	analyze_image(dst_fbo, PATTERN_WIDTH);
	printf("\n");
}

enum piglit_result
piglit_display()
{
	bool pass = true;
	GLint fbos[] = { 0, linear_fbo, srgb_fbo,
			 linear_msaa_fbo, srgb_msaa_fbo };
	char *fbo_names[] = { "winsys", "linear", "srgb",
			      "linear MSAA", "srgb MSAA" };
	const unsigned num_fbos = 5;
	unsigned i, j, srgb_enable, scaled;

	glUseProgram(prog);
	glDisable(GL_FRAMEBUFFER_SRGB);

	for (i = 0; i < num_fbos; ++i)
		test_readback(fbos[i], fbo_names[i]);

	for (i = 0; i < num_fbos; ++i) {
		for (j = 0; j < num_fbos; ++j) {
			for (srgb_enable = 0; srgb_enable < 2; ++srgb_enable) {
				for (scaled = 0; scaled < 2; ++scaled) {
					test_blit(fbos[i], fbo_names[i],
						  fbos[j], fbo_names[j],
						  srgb_enable, scaled);
				}
			}
		}
	}

	piglit_present_results();

	return pass ? PIGLIT_PASS : PIGLIT_FAIL;
}
