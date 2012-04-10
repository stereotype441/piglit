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

#include "piglit-util.h"

int piglit_width = 512;
int piglit_height = 256;
int piglit_window_mode = GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_STENCIL;

namespace {

const int pattern_width = 256;
const int pattern_height = 256;
const int supersample_factor = 16;

class Fbo
{
public:
	void init(bool multisampled, int width, int height);
	void set_viewport();

	int width;
	int height;
	GLuint handle;
	GLuint tex;
};

void
Fbo::init(bool multisampled, int width, int height)
{
	this->tex = 0;
	this->width = width;
	this->height = height;

	glGenFramebuffers(1, &handle);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle);

	/* Color buffer */
	if (multisampled) {
		GLuint rb;
		glGenRenderbuffers(1, &rb);
		glBindRenderbuffer(GL_RENDERBUFFER, rb);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4,
						 GL_RGBA, width, height);
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,
					  GL_COLOR_ATTACHMENT0,
					  GL_RENDERBUFFER, rb);
	} else {
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D,
			     0 /* level */,
			     GL_RGBA /* internalformat */,
			     width,
			     height,
			     0 /* border */,
			     GL_RGBA /* format */,
			     GL_BYTE /* type */,
			     NULL /* data */);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
				       GL_COLOR_ATTACHMENT0,
				       GL_TEXTURE_2D,
				       tex,
				       0 /* level */);
	}

	/* Stencil buffer */
	GLuint stencil;
	glGenRenderbuffers(1, &stencil);
	glBindRenderbuffer(GL_RENDERBUFFER, stencil);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, multisampled ? 4 : 0,
					 GL_STENCIL_INDEX8, width, height);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
				  GL_RENDERBUFFER, stencil);

	/* Depth buffer */
	if (!multisampled) { // TODO
		GLuint depth;
		glGenRenderbuffers(1, &depth);
		glBindRenderbuffer(GL_RENDERBUFFER, depth);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 0,
						 GL_DEPTH_COMPONENT24,
						 width, height);
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,
					  GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
					  depth);
	}

	if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer not complete\n");
		piglit_report_result(PIGLIT_FAIL);
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void
Fbo::set_viewport()
{
	glViewport(0, 0, width, height);
}

class DownsampleProg
{
public:
	void compile();
	void run(const Fbo *src_fbo, int dstX0, int dstY0, int dstX1, int dstY1);

private:
	GLint prog;
	GLuint vertex_buf;
	GLuint vao;
};

void
DownsampleProg::compile()
{
	static const char *vert =
		"#version 130\n"
		"in vec2 pos;\n"
		"in vec2 texCoord;\n"
		"out vec2 texCoordVarying;\n"
		"void main()\n"
		"{\n"
		"  gl_Position = vec4(pos, 0.0, 1.0);\n"
		"  texCoordVarying = texCoord;\n"
		"}\n";

	static const char *frag =
		"#version 130\n"
		"uniform sampler2D samp;\n"
		"uniform int supersample_factor;\n"
		"in vec2 texCoordVarying;\n"
		"void main()\n"
		"{\n"
		"  vec4 sum = vec4(0.0);\n"
		"  ivec2 pixel = ivec2(texCoordVarying);\n"
		"  for (int i = 0; i < supersample_factor; ++i) {\n"
		"    for (int j = 0; j < supersample_factor; ++j) {\n"
		"      sum += texelFetch(\n"
		"          samp, pixel * supersample_factor + ivec2(i, j), 0);\n"
		"    }\n"
		"  }\n"
		"  gl_FragColor = sum / (supersample_factor * supersample_factor);\n"
		"}\n";

	/* Compile program */
	piglit_require_GLSL_version(130);
	prog = piglit_CreateProgram();
	GLint vs = piglit_compile_shader_text(GL_VERTEX_SHADER, vert);
	piglit_AttachShader(prog, vs);
	GLint fs = piglit_compile_shader_text(GL_FRAGMENT_SHADER, frag);
	piglit_AttachShader(prog, fs);
	piglit_BindAttribLocation(prog, 0, "pos");
	piglit_BindAttribLocation(prog, 1, "texCoord");
	piglit_LinkProgram(prog);
	if (!piglit_link_check_status(prog)) {
		piglit_report_result(PIGLIT_FAIL);
	}

	/* Set up uniforms */
	piglit_UseProgram(prog);
	piglit_Uniform1i(piglit_GetUniformLocation(prog, "supersample_factor"),
			 supersample_factor);
	piglit_Uniform1i(piglit_GetUniformLocation(prog, "samp"), 0);

	/* Set up vertex array object */
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/* Set up vertex input buffer */
	glGenBuffers(1, &vertex_buf);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float),
			      (void *) 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float),
			      (void *) (2*sizeof(float)));

	/* Set up element input buffer to tesselate a quad into
	 * triangles
	 */
	unsigned int indices[6] = { 0, 1, 2, 0, 2, 3 };
	GLuint element_buf;
	glGenBuffers(1, &element_buf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		     GL_STATIC_DRAW);
}

void
DownsampleProg::run(const Fbo *src_fbo, int dstX0, int dstY0, int dstX1, int dstY1)
{
	float x0 = dstX0 * 2.0 / piglit_width - 1.0;
	float x1 = dstX1 * 2.0 / piglit_width - 1.0;
	float y0 = dstY0 * 2.0 / piglit_height - 1.0;
	float y1 = dstY1 * 2.0 / piglit_height - 1.0;
	float w = src_fbo->width / supersample_factor;
	float h = src_fbo->height / supersample_factor;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, src_fbo->tex);

	piglit_UseProgram(prog);
	glBindVertexArray(vao);

	float vertex_data[4][4] = {
		{ x0, y0, 0, 0 },
		{ x0, y1, 0, h },
		{ x1, y1, w, h },
		{ x1, y0, w, 0 }
	};
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data,
		     GL_STREAM_DRAW);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *) 0);
}

class ManifestProgram
{
public:
	virtual void compile() = 0;
	virtual void run(float x0, float y0, float x1, float y1) = 0;
};

class ManifestStencil : public ManifestProgram
{
public:
	virtual void compile();
	virtual void run(float x0, float y0, float x1, float y1);

private:
	GLint prog;
	GLint color_loc;
	GLuint vertex_buf;
	GLuint vao;
};

void
ManifestStencil::compile()
{
	static const char *vert =
		"#version 130\n"
		"in vec2 pos;\n"
		"void main()\n"
		"{\n"
		"  gl_Position = vec4(pos, 0.0, 1.0);\n"
		"}\n";

	static const char *frag =
		"#version 130\n"
		"uniform vec4 color;\n"
		"void main()\n"
		"{\n"
		"  gl_FragColor = color;\n"
		"}\n";

	/* Compile program */
	piglit_require_GLSL_version(130);
	prog = piglit_CreateProgram();
	GLint vs = piglit_compile_shader_text(GL_VERTEX_SHADER, vert);
	piglit_AttachShader(prog, vs);
	GLint fs = piglit_compile_shader_text(GL_FRAGMENT_SHADER, frag);
	piglit_AttachShader(prog, fs);
	piglit_BindAttribLocation(prog, 0, "pos");
	piglit_LinkProgram(prog);
	if (!piglit_link_check_status(prog)) {
		piglit_report_result(PIGLIT_FAIL);
	}

	/* Set up uniforms */
	piglit_UseProgram(prog);
	color_loc = piglit_GetUniformLocation(prog, "color");

	/* Set up vertex array object */
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/* Set up vertex input buffer */
	glGenVertexArrays(1, &vertex_buf);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float),
			      (void *) 0);

	/* Set up element input buffer to tesselate a quad into
	 * triangles
	 */
	unsigned int indices[6] = { 0, 1, 2, 0, 2, 3 };
	GLuint element_buf;
	glGenBuffers(1, &element_buf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		     GL_STATIC_DRAW);
}

void
ManifestStencil::run(float x0, float y0, float x1, float y1)
{
	static float colors[8][4] = {
		{ 0.0, 0.0, 0.0, 1.0 },
		{ 0.0, 0.0, 1.0, 1.0 },
		{ 0.0, 1.0, 0.0, 1.0 },
		{ 0.0, 1.0, 1.0, 1.0 },
		{ 1.0, 0.0, 0.0, 1.0 },
		{ 1.0, 0.0, 1.0, 1.0 },
		{ 1.0, 1.0, 0.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 }
	};

	piglit_UseProgram(prog);
	glBindVertexArray(vao);

	float vertex_data[4][2] = {
		{ x0, y0 },
		{ x0, y1 },
		{ x1, y1 },
		{ x1, y0 }
	};
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data,
		     GL_STREAM_DRAW);

	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glEnable(GL_STENCIL_TEST);
	for (int i = 0; i < 8; ++i) {
		glStencilFunc(GL_EQUAL, i, 0x07);
		piglit_Uniform4fv(color_loc, 1, colors[i]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *) 0);
	}

	glDisable(GL_STENCIL_TEST);
}

class TilingProjMatrix
{
public:
	TilingProjMatrix(int tile_width, int tile_height,
			 int x_offset, int y_offset);

	float values[4][4];
};

TilingProjMatrix::TilingProjMatrix(int tile_width, int tile_height,
				   int x_offset, int y_offset)
{
	/* Need a projection matrix such that:
	 * xc = ((xe + 1) * pattern_width/2 - x_offset) * 2/tile_width - 1
	 * yc = ((ye + 1) * pattern_height/2 - y_offset) * 2/tile_height - 1
	 * zc = ze
	 * wc = we = 1.0
	 *
	 * Therefore
	 * xc = pattern_width / tile_width * xe
	 *    + pattern_width / tile_width - x_offset * 2 / tile_width - 1
	 * yc = pattern_height / tile_height * ye
	 *    + pattern_height / tile_height - y_offset * 2 / tile_height - 1
	 * zc = ze
	 * wc = we = 1.0
	 */
	memset(values, 0, sizeof(values));
	values[0][0] = float(pattern_width) / tile_width;
	values[0][3] = values[0][0] - x_offset * 2.0 / tile_width - 1.0;
	values[1][1] = float(pattern_height) / tile_height;
	values[1][3] = values[1][1] - y_offset * 2.0 / tile_height - 1.0;
	values[2][2] = 1.0;
	values[3][3] = 1.0;
}

class TestPattern
{
public:
	virtual void compile() = 0;
	virtual void draw(const TilingProjMatrix *proj) = 0;
};

class Triangles : public TestPattern
{
public:
	virtual void compile();
	virtual void draw(const TilingProjMatrix *proj);

private:
	GLint prog;
	GLuint vertex_buf;
	GLuint vao;
	GLint proj_loc;
	int num_tris;
};

void Triangles::compile()
{
	/* Triangle coords within (-1,-1) to (1,1) rect */
	static const float pos_within_tri[][2] = {
		{ -0.5, -1.0 },
		{  0.0,  1.0 },
		{  0.5, -1.0 }
	};

	/* Number of triangle instances across (and down) */
	int tris_across = 8;

	/* Total number of triangles drawn */
	num_tris = tris_across * tris_across;

	/* Scaling factor uniformly applied to triangle coords */
	float tri_scale = 0.8 / tris_across;

	/* Amount each triangle should be rotated compared to prev */
	float rotation_delta = M_PI * 2.0 / num_tris;

	/* Final scaling factor */
	float final_scale = 0.95;

	static const char *vert =
		"#version 130\n"
		"#extension GL_ARB_draw_instanced: require\n"
		"in vec2 pos_within_tri;\n"
		"uniform float tri_scale;\n"
		"uniform float rotation_delta;\n"
		"uniform int tris_across;\n"
		"uniform float final_scale;\n"
		"uniform mat4 proj;\n"
		"\n"
		"void main()\n"
		"{\n"
		"  vec2 pos = tri_scale * pos_within_tri;\n"
		"  float rotation = rotation_delta * gl_InstanceID;\n"
		"  pos = mat2(cos(rotation), sin(rotation),\n"
		"             -sin(rotation), cos(rotation)) * pos;\n"
		"  int i = gl_InstanceID % tris_across;\n"
		"  int j = tris_across - 1 - gl_InstanceID / tris_across;\n"
		"  pos += (vec2(i, j) * 2.0 + 1.0) / tris_across - 1.0;\n"
		"  pos *= final_scale;\n"
		"  gl_Position = proj * vec4(pos, 0.0, 1.0);\n"
		"}\n";

	static const char *frag =
		"#version 130\n"
		"void main()\n"
		"{\n"
		"  gl_FragColor = vec4(1.0);\n"
		"}\n";

	/* Compile program */
	piglit_require_GLSL_version(130);
	piglit_require_extension("GL_ARB_draw_instanced");
	prog = piglit_CreateProgram();
	GLint vs = piglit_compile_shader_text(GL_VERTEX_SHADER, vert);
	piglit_AttachShader(prog, vs);
	GLint fs = piglit_compile_shader_text(GL_FRAGMENT_SHADER, frag);
	piglit_AttachShader(prog, fs);
	piglit_BindAttribLocation(prog, 0, "pos_within_tri");
	piglit_LinkProgram(prog);
	if (!piglit_link_check_status(prog)) {
		piglit_report_result(PIGLIT_FAIL);
	}

	/* Set up uniforms */
	piglit_UseProgram(prog);
	piglit_Uniform1f(piglit_GetUniformLocation(prog, "tri_scale"),
			 tri_scale);
	piglit_Uniform1f(piglit_GetUniformLocation(prog, "rotation_delta"),
			 rotation_delta);
	piglit_Uniform1i(piglit_GetUniformLocation(prog, "tris_across"),
			 tris_across);
	piglit_Uniform1f(piglit_GetUniformLocation(prog, "final_scale"),
			 final_scale);
	proj_loc = piglit_GetUniformLocation(prog, "proj");

	/* Set up vertex array object */
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/* Set up vertex input buffer */
	glGenBuffers(1, &vertex_buf);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos_within_tri), pos_within_tri,
		     GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, ARRAY_SIZE(pos_within_tri[0]), GL_FLOAT,
			      GL_FALSE, sizeof(pos_within_tri[0]), (void *) 0);
}

void Triangles::draw(const TilingProjMatrix *proj)
{
	glClear(GL_COLOR_BUFFER_BIT);

	piglit_UseProgram(prog);
	piglit_UniformMatrix4fv(proj_loc, 1, GL_TRUE, &proj->values[0][0]);
	glBindVertexArray(vao);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 3, num_tris);
}

class Sunburst : public TestPattern
{
public:
	virtual void compile();

protected:
	GLint prog;
	GLint rotation_loc;
	GLint depth_loc;
	GLint proj_loc;
	GLuint vao;
	int num_tris;

private:
	GLuint vertex_buf;
};

void Sunburst::compile()
{
	/* Triangle coords within (-1,-1) to (1,1) rect */
	static const float pos_within_tri[][2] = {
		{ -0.3, -0.8 },
		{  0.0,  1.0 },
		{  0.3, -0.8 }
	};

	/* Total number of triangles drawn */
	num_tris = 7;

	static const char *vert =
		"#version 130\n"
		"in vec2 pos_within_tri;\n"
		"uniform float rotation;\n"
		"uniform float depth;\n"
		"uniform mat4 proj;\n"
		"\n"
		"void main()\n"
		"{\n"
		"  vec2 pos = pos_within_tri;\n"
		"  pos = mat2(cos(rotation), sin(rotation),\n"
		"             -sin(rotation), cos(rotation)) * pos;\n"
		"  gl_Position = proj * vec4(pos, depth, 1.0);\n"
		"}\n";

	static const char *frag =
		"#version 130\n"
		"uniform float depth;\n" // TODO
		"void main()\n"
		"{\n"
		"  gl_FragColor = vec4((depth + 1.0) / 2.0);\n" // TODO
		"}\n";

	/* Compile program */
	piglit_require_GLSL_version(130);
	piglit_require_extension("GL_ARB_draw_instanced");
	prog = piglit_CreateProgram();
	GLint vs = piglit_compile_shader_text(GL_VERTEX_SHADER, vert);
	piglit_AttachShader(prog, vs);
	GLint fs = piglit_compile_shader_text(GL_FRAGMENT_SHADER, frag);
	piglit_AttachShader(prog, fs);
	piglit_BindAttribLocation(prog, 0, "pos_within_tri");
	piglit_LinkProgram(prog);
	if (!piglit_link_check_status(prog)) {
		piglit_report_result(PIGLIT_FAIL);
	}

	/* Set up uniforms */
	piglit_UseProgram(prog);
	rotation_loc = piglit_GetUniformLocation(prog, "rotation");
	depth_loc = piglit_GetUniformLocation(prog, "depth");
	piglit_Uniform1f(depth_loc, 0.0);
	proj_loc = piglit_GetUniformLocation(prog, "proj");

	/* Set up vertex array object */
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/* Set up vertex input buffer */
	glGenBuffers(1, &vertex_buf);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos_within_tri), pos_within_tri,
		     GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, ARRAY_SIZE(pos_within_tri[0]), GL_FLOAT,
			      GL_FALSE, sizeof(pos_within_tri[0]), (void *) 0);
}

class StencilSunburst : public Sunburst
{
public:
	virtual void draw(const TilingProjMatrix *proj);
};

void
StencilSunburst::draw(const TilingProjMatrix *proj)
{
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	piglit_UseProgram(prog);
	piglit_UniformMatrix4fv(proj_loc, 1, GL_TRUE, &proj->values[0][0]);
	glBindVertexArray(vao);
	for (int i = 0; i < num_tris; ++i) {
		glStencilFunc(GL_ALWAYS, i+1, 0xff);
		piglit_Uniform1f(rotation_loc, M_PI * 2.0 * i / num_tris);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glDisable(GL_STENCIL_TEST);
}

class DepthSunburst : public Sunburst
{
public:
	virtual void draw(const TilingProjMatrix *proj);
};

void
DepthSunburst::draw(const TilingProjMatrix *proj)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	piglit_UseProgram(prog);
	piglit_UniformMatrix4fv(proj_loc, 1, GL_TRUE, &proj->values[0][0]);
	glBindVertexArray(vao);
	for (int i = 0; i < num_tris; ++i) {
		/* Draw triangles in a haphazard order so we can
		 * verify that depth comparisons sort them out
		 * properly.
		 */
		int triangle_to_draw = (i * 3) % num_tris;
		piglit_Uniform1f(depth_loc,
				 float(num_tris - 1 - triangle_to_draw * 2)
				 / num_tris);
		piglit_Uniform1f(rotation_loc,
				 M_PI * 2.0 * triangle_to_draw / num_tris);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	glDisable(GL_DEPTH_TEST);
}

class Stats
{
public:
	Stats();

	void record(float error)
	{
		++count;
		sum_squared_error += error * error;
	}

	void summarize();

private:
	int count;
	double sum_squared_error;
};

Stats::Stats()
	: count(0), sum_squared_error(0.0)
{
}

void
Stats::summarize()
{
	printf("  count = %d\n", count);
	if (count != 0) {
		if (sum_squared_error != 0.0) {
			printf("  RMS error = %f\n",
			       sqrt(sum_squared_error / count));
		} else {
			printf("  Perfect output\n");
		}
	}
}

class Test
{
public:
	Test(TestPattern *pattern, ManifestProgram *manifest_program,
	     bool test_resolve);
	void init();
	void run();

private:
	void draw_test_image();
	void draw_reference_image();
	void measure_accuracy();

	TestPattern *pattern;
	ManifestProgram *manifest_program;
	bool test_resolve;

	Fbo multisample_fbo;
	Fbo supersample_fbo;
	DownsampleProg downsample_prog;
};

Test::Test(TestPattern *pattern, ManifestProgram *manifest_program,
	   bool test_resolve)
	: pattern(pattern),
	  manifest_program(manifest_program),
	  test_resolve(test_resolve)
{
}

void
Test::init()
{
	/* TODO: choose whether to test small multisample_fbo by command line arg */
	multisample_fbo.init(true /* multisampled */,
			     pattern_width / 4, pattern_height / 4);
	supersample_fbo.init(false /* multisampled */,
			     1024, 1024);

	pattern->compile();
	downsample_prog.compile();
	if (manifest_program)
		manifest_program->compile();

	/* Only do depth testing in those parts of the test where we
	 * explicitly want it
	 */
	glDisable(GL_DEPTH_TEST);
}

void
Test::draw_test_image()
{
	int num_h_tiles = pattern_width / multisample_fbo.width;
	int num_v_tiles = pattern_height / multisample_fbo.height;
	for (int h = 0; h < num_h_tiles; ++h) {
		for (int v = 0; v < num_v_tiles; ++v) {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER,
					  multisample_fbo.handle);
			multisample_fbo.set_viewport();
			int x_offset = h * multisample_fbo.width;
			int y_offset = v * multisample_fbo.height;
			TilingProjMatrix proj(multisample_fbo.width,
					      multisample_fbo.height,
					      x_offset,
					      y_offset);
			pattern->draw(&proj);

			if (test_resolve) {
				/* TODO: adapt for non-stencil */
				glBindFramebuffer(GL_READ_FRAMEBUFFER,
						  multisample_fbo.handle);
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
				glViewport(0, 0, piglit_width, piglit_height);
				glBlitFramebuffer(0, 0, multisample_fbo.width,
						  multisample_fbo.height,
						  x_offset, y_offset,
						  x_offset + multisample_fbo.width,
						  y_offset + multisample_fbo.height,
						  GL_STENCIL_BUFFER_BIT, GL_NEAREST);
				if (manifest_program)
					manifest_program->run(-1, -1, 0, 1);
			} else {
				if (manifest_program)
					manifest_program->run(-1, -1, 1, 1);

				glBindFramebuffer(GL_READ_FRAMEBUFFER,
						  multisample_fbo.handle);
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
				glViewport(0, 0, piglit_width, piglit_height);
				glBlitFramebuffer(0, 0, multisample_fbo.width,
						  multisample_fbo.height,
						  x_offset, y_offset,
						  x_offset + multisample_fbo.width,
						  y_offset + multisample_fbo.height,
						  GL_COLOR_BUFFER_BIT, GL_NEAREST);
			}
		}
	}
}

void
Test::draw_reference_image()
{
	int downsampled_width = supersample_fbo.width / supersample_factor;
	int downsampled_height = supersample_fbo.height / supersample_factor;
	int num_h_tiles = pattern_width / downsampled_width;
	int num_v_tiles = pattern_height / downsampled_height;
	for (int h = 0; h < num_h_tiles; ++h) {
		for (int v = 0; v < num_v_tiles; ++v) {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER,
					  supersample_fbo.handle);
			supersample_fbo.set_viewport();
			int x_offset = h * downsampled_width;
			int y_offset = v * downsampled_height;
			TilingProjMatrix proj(downsampled_width,
					      downsampled_height,
					      x_offset,
					      y_offset);
			pattern->draw(&proj);

			if (manifest_program)
				manifest_program->run(-1, -1, 1, 1);

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glViewport(0, 0, piglit_width, piglit_height);
			downsample_prog.run(&supersample_fbo,
					    x_offset + pattern_width,
					    y_offset,
					    x_offset + pattern_width
					    + downsampled_width,
					    y_offset + downsampled_height);
		}
	}
}

void
Test::measure_accuracy()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glViewport(0, 0, piglit_width, piglit_height);

	float *reference_data = new float[pattern_width * pattern_height * 4];
	glReadPixels(pattern_width, 0, pattern_width, pattern_height, GL_RGBA,
		     GL_FLOAT, reference_data);

	float *test_data = new float[pattern_width * pattern_height * 4];
	glReadPixels(0, 0, pattern_width, pattern_height, GL_RGBA,
		     GL_FLOAT, test_data);

	Stats unlit_stats;
	Stats partially_lit_stats;
	Stats totally_lit_stats;
	for (int y = 0; y < pattern_height; ++y) {
		for (int x = 0; x < pattern_width; ++x) {
			for (int c = 0; c < 4; ++c) {
				int pixel_pos = 4*(y*pattern_width + x) + c;
				float ref = reference_data[pixel_pos];
				float test = test_data[pixel_pos];
				if (ref <= 0.0)
					unlit_stats.record(test - ref);
				else if (ref >= 1.0)
					totally_lit_stats.record(test - ref);
				else
					partially_lit_stats.record(test - ref);
			}
		}
	}

	printf("Pixels that should be unlit\n");
	unlit_stats.summarize();
	printf("Pixels that should be totally lit\n");
	totally_lit_stats.summarize();
	printf("Pixels that should be partially lit\n");
	partially_lit_stats.summarize();
	// TODO: generate piglit result
}

void
Test::run()
{
	draw_test_image();
	draw_reference_image();
	measure_accuracy();
}

Test *test = NULL;

void
print_usage_and_exit(char *prog_name)
{
	printf("Usage: %s <test_type>\n"
	       "  where <test_type> is one of:\n"
	       "    color: test downsampling of color buffer\n"
	       "    stencil_draw: test drawing using MSAA stencil buffer\n"
	       "    stencil_resolve: test resolve of MSAA stencil buffer\n",
	       prog_name);
	piglit_report_result(PIGLIT_FAIL);
}

extern "C" void
piglit_init(int argc, char **argv)
{
	if (argc != 2)
		print_usage_and_exit(argv[0]);
	if (strcmp(argv[1], "color") == 0) {
		test = new Test(new Triangles(), NULL, false);
	} else if (strcmp(argv[1], "stencil_draw") == 0) {
		test = new Test(new StencilSunburst(), new ManifestStencil(), false);
	} else if (strcmp(argv[1], "stencil_resolve") == 0) {
		test = new Test(new StencilSunburst(), new ManifestStencil(), true);
	} else if (strcmp(argv[1], "depth_draw") == 0) {
		test = new Test(new DepthSunburst(), NULL, false);
	} else {
		print_usage_and_exit(argv[0]);
	}

	test->init();
}

extern "C" enum piglit_result
piglit_display()
{
	test->run();

	piglit_present_results();

	return PIGLIT_PASS;
}

};
