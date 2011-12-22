/*
 * Copyright © 2010 Intel Corporation
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
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *
 */

/** @file glsl-max-varyings.c
 *
 * Tests whether each varying can be used at all numbers of varyings up to
 * GL_MAX_VARYING_FLOATS / 4.
 *
 * With the "xfb" option, also test that all varyings can be captured
 * using transform feedback, up to
 * GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS / 4.
 */

#include "piglit-util.h"

#define MAX_VARYING 32

/* 10x10 rectangles with 2 pixels of pad.  Deal with up to 32 varyings. */
int piglit_width = (2 + MAX_VARYING * 12), piglit_height = (2 + MAX_VARYING * 12);
int piglit_window_mode = GLUT_RGB | GLUT_DOUBLE;

static GLboolean use_xfb = GL_FALSE;
static const char *xfb_varying_array[MAX_VARYING];
static GLuint xfb_buf;

#define MIN2(A, B) ((A) < (B) ? (A) : (B))

/* Generate a VS that writes to num_varyings vec4s, and put
 * interesting data in data_varying with 0.0 everywhere else.
 */
static GLint get_vs(int num_varyings, int data_varying)
{
	GLuint shader;
	unsigned i;
	char code[2048], temp[2048];

	code[0] = 0;
	for (i = 0; i < num_varyings; i++) {
		sprintf(temp, "varying vec4 v%d;\n", i);
		strcat(code, temp);
	}

	sprintf(temp,
		"attribute vec4 green;\n"
		"attribute vec4 red;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = (gl_ModelViewProjectionMatrix * \n"
		"			gl_Vertex);\n"
		);
	strcat(code, temp);

	for (i = 0; i < num_varyings; i++) {
		if (i == data_varying)
			sprintf(temp, "	v%d = green;\n", i);
		else
			sprintf(temp, "	v%d = red;\n", i);
		strcat(code, temp);
	}

	sprintf(temp,
		"}\n"
		);
	strcat(code, temp);

	shader = piglit_compile_shader_text(GL_VERTEX_SHADER, code);
	/* printf("%s\n", code); */

	return shader;
}

/* Generate a FS that does operations on num_varyings, yet makes only
 * data_varying contribute to the output.
 *
 * We could make a single FS per num_varyings that did this by using a
 * uniform for data_varying and some multiplication by comparisons
 * (see glsl-routing for an example), but since we're linking a new
 * shader each time anyway, this produces a simpler FS to read and
 * verify.
 */
static GLint get_fs(int num_varyings, int data_varying)
{
	GLuint shader;
	unsigned i;
	char code[2048], temp[2048];

	code[0] = 0;
	for (i = 0; i < num_varyings; i++) {
		sprintf(temp, "varying vec4 v%d;\n", i);
		strcat(code, temp);
	}

	sprintf(temp,
		"uniform float zero;\n"
		"uniform float one;\n"
		"void main()\n"
		"{\n"
		"	vec4 val = vec4(0.0);\n"
		);
	strcat(code, temp);

	for (i = 0; i < num_varyings; i++) {
		if (i == data_varying)
			sprintf(temp, "	val += one * v%d;\n", i);
		else
			sprintf(temp, "	val += zero * v%d;\n", i);
		strcat(code, temp);
	}

	sprintf(temp,
		"	gl_FragColor = val;\n"
		"}\n"
		);
	strcat(code, temp);

	/* printf("%s\n", code); */
	shader = piglit_compile_shader_text(GL_FRAGMENT_SHADER, code);

	return shader;
}

/**
 * Initialize xfb_varying_array to contain the names of the varyings
 * used by get_vs and get_fs.
 */
static void
init_xfb_varying_array()
{
	int i;
	for (i = 0; i < MAX_VARYING; ++i) {
		char *buf = malloc(4);
		sprintf(buf, "v%d", i);
		xfb_varying_array[i] = buf;
	}
}

static int
coord_from_index(int index)
{
	return 2 + 12 * index;
}

static GLboolean
check_xfb_output(int max_varyings, int xfb_varyings, int green_varying)
{
	GLboolean pass = GL_TRUE;
	int vertex, varying;
	float green[4] = {0.0, 1.0, 0.0, 0.0};
	float red[4] = {1.0, 0.0, 0.0, 0.0};
	float (*buffer)[4] = glMapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER,
					 GL_READ_ONLY);

	for (vertex = 0; vertex < 6; ++vertex) {
		for (varying = 0; varying < xfb_varyings; ++varying) {
			float *expected =
				varying == green_varying ? green : red;
			float *actual =
				buffer[vertex * max_varyings + varying];
			if (memcmp(expected, actual, 4 * sizeof(float)) != 0) {
				printf("When testing %i varyings\n",
				       max_varyings);
				printf("And setting varying %i to green\n",
				       green_varying);
				printf("Got incorrect transform feedback data "
				       "for vertex %i, varying %i\n", vertex,
				       varying);
				printf("Expected (%f, %f, %f, %f)\n",
				       expected[0], expected[1], expected[2],
				       expected[3]);
				printf("Actual (%f, %f, %f, %f)\n",
				       actual[0], actual[1], actual[2],
				       actual[3]);
				pass = GL_FALSE;
			}
		}
	}

	glUnmapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER);

	return pass;
}

static GLboolean
draw(int num_varyings, int max_xfb_varyings)
{
	GLboolean pass = GL_TRUE;
	int data_varying, xfb_varyings;
	float green[4][4] = { {0.0, 1.0, 0.0, 0.0},
			      {0.0, 1.0, 0.0, 0.0},
			      {0.0, 1.0, 0.0, 0.0},
			      {0.0, 1.0, 0.0, 0.0} };
	float red[4][4] = { {1.0, 0.0, 0.0, 0.0},
			    {1.0, 0.0, 0.0, 0.0},
			    {1.0, 0.0, 0.0, 0.0},
			    {1.0, 0.0, 0.0, 0.0} };

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
			      green);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
			      red);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (data_varying = 0; data_varying < num_varyings; data_varying++) {
		GLuint prog, vs, fs;
		GLint loc;

		vs = get_vs(num_varyings, data_varying);
		fs = get_fs(num_varyings, data_varying);

		prog = glCreateProgram();
		glAttachShader(prog, vs);
		glAttachShader(prog, fs);

		glBindAttribLocation(prog, 1, "green");
		glBindAttribLocation(prog, 2, "red");

		if (use_xfb) {
			xfb_varyings = MIN2(num_varyings, max_xfb_varyings);
			piglit_TransformFeedbackVaryings(prog, xfb_varyings,
							 xfb_varying_array,
							 GL_INTERLEAVED_ATTRIBS);
		}

		glLinkProgram(prog);
		if (!piglit_link_check_status(prog))
			piglit_report_result(PIGLIT_FAIL);

		glUseProgram(prog);

		loc = glGetUniformLocation(prog, "zero");
		if (loc != -1) /* not used for num_varyings == 1 */
			glUniform1f(loc, 0.0);

		loc = glGetUniformLocation(prog, "one");
		assert(loc != -1); /* should always be used */
		glUniform1f(loc, 1.0);

		if (use_xfb) {
			float initial_buffer[MAX_VARYING * 6][4];
			glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, xfb_buf);
			memset(initial_buffer, 0, sizeof(initial_buffer));
			glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER,
				     sizeof(initial_buffer), initial_buffer,
				     GL_STREAM_READ);
			piglit_BindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0,
					      xfb_buf);
			piglit_BeginTransformFeedback(GL_TRIANGLES);
		}

		piglit_draw_rect(coord_from_index(data_varying),
				 coord_from_index(num_varyings - 1),
				 10,
				 10);

		if (use_xfb) {
			piglit_EndTransformFeedback();
			pass = check_xfb_output(num_varyings, xfb_varyings,
						data_varying)
				&& pass;
		}

		glDeleteShader(vs);
		glDeleteShader(fs);
		glDeleteProgram(prog);
	}

	return pass;
}

enum piglit_result
piglit_display(void)
{
	GLint max_components;
	int max_varyings, row, col;
	int max_xfb_varyings = 0;
	GLboolean pass = GL_TRUE, warned = GL_FALSE;

	piglit_ortho_projection(piglit_width, piglit_height, GL_FALSE);

	glGetIntegerv(GL_MAX_VARYING_FLOATS, &max_components);
	max_varyings = max_components / 4;

	printf("GL_MAX_VARYING_FLOATS = %i\n", max_components);

	if (max_varyings > MAX_VARYING) {
		printf("test not designed to handle >%d varying vec4s.\n"
		       "(implementation reports %d components)\n",
		       max_components, MAX_VARYING);
		max_varyings = MAX_VARYING;
		warned = GL_TRUE;
	}

	if (use_xfb) {
		GLint max_xfb_components;
		glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS,
			      &max_xfb_components);
		max_xfb_varyings = max_xfb_components / 4;

		printf("GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS = %i\n",
		       max_xfb_components);
	}

	glClearColor(0.5, 0.5, 0.5, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);

	for (row = 0; row < max_varyings; row++) {
		pass = draw(row + 1, max_xfb_varyings) && pass;
	}

	for (row = 0; row < max_varyings; row++) {
		for (col = 0; col <= row; col++) {
			GLboolean ok;
			float green[3] = {0.0, 1.0, 0.0};

			ok = piglit_probe_rect_rgb(coord_from_index(col),
						   coord_from_index(row),
						   10, 10,
						   green);
			if (!ok) {
				printf("  Failure with %d vec4 varyings used "
				       "in varying index %d\n",
				       row + 1, col);
				pass = GL_FALSE;
				break;
			}
		}
	}

	glutSwapBuffers();

	if (!pass)
		return PIGLIT_FAIL;
	if (warned)
		return PIGLIT_WARN;
	else
		return PIGLIT_PASS;
}

static void
print_usage_and_exit(char *prog_name)
{
	printf("Usage: %s [options]\n"
	       "  permissible options:\n"
	       "    xfb  Also capture varyings with transform feedback\n",
	       prog_name);
	exit(1);
}

void piglit_init(int argc, char **argv)
{
	int i;

	/* Interpret command line args */
	for (i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "xfb") == 0)
			use_xfb = GL_TRUE;
		else
			print_usage_and_exit(argv[0]);
	}

	if (!GLEW_VERSION_2_0) {
		printf("Requires OpenGL 2.0\n");
		piglit_report_result(PIGLIT_SKIP);
	}

	if (use_xfb) {
		piglit_require_transform_feedback();
		init_xfb_varying_array();
		glGenBuffers(1, &xfb_buf);
	}

	printf("Vertical axis: Increasing numbers of varyings.\n");
	printf("Horizontal axis: Which of the varyings contains the color.\n");
}

