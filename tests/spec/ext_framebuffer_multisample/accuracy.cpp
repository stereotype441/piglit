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

const int NUM_HORIZ_TILES = 8;
const int NUM_VERT_TILES = 8;
const int NUM_TOTAL_TILES = NUM_HORIZ_TILES * NUM_VERT_TILES;
const int TILE_SIZE = 32;
const int UPSAMPLE_FACTOR = 16;

int piglit_width = TILE_SIZE * NUM_HORIZ_TILES;
int piglit_height = TILE_SIZE * NUM_VERT_TILES;
int piglit_window_mode = GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE;

class Fbo
{
public:
	Fbo(bool multisampled, int width, int height);
	void set_viewport();

	GLuint handle;
	GLuint tex;

private:
	int width;
	int height;
};

Fbo::Fbo(bool multisampled, int width, int height)
	: tex(0), width(width), height(height)
{
	glGenFramebuffers(1, &handle);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle);

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

Fbo *multisample_fbo = NULL;
Fbo *upsample_fbo = NULL;

class DrawProg
{
public:
	DrawProg();
	void use();
	void set_rotated(bool rotated);
	void set_size(float x, float y);
	void set_offset(float x, float y);

private:
	GLint prog;
	GLint rotated_loc;
	GLint size_loc;
	GLint offset_loc;
};

DrawProg::DrawProg()
{
	static const char *vert =
		"#version 130\n"
		"uniform bool rotated;\n"
		"uniform vec2 size;\n"
		"uniform vec2 offset;\n"
		"in vec2 pos;\n"
		"void main()\n"
		"{\n"
		"  vec2 pos2 = pos;\n"
		"  if (rotated)\n"
		"    pos2 = vec2(pos2.y, 1.0 - pos2.x);\n"
		"  pos2 *= size;\n"
		"  pos2 += offset;\n"
		"  gl_Position = vec4(pos2, 0.0, 1.0);\n"
		"}\n";
	static const char *frag =
		"#version 130\n"
		"void main()\n"
		"{\n"
		"  gl_FragColor = vec4(1.0);\n"
		"}\n";

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
	rotated_loc = piglit_GetUniformLocation(prog, "rotated");
	size_loc = piglit_GetUniformLocation(prog, "size");
	offset_loc = piglit_GetUniformLocation(prog, "offset");
}

void
DrawProg::use()
{
	piglit_UseProgram(prog);
}

void
DrawProg::set_rotated(bool rotated)
{
	piglit_Uniform1i(rotated_loc, rotated ? 1 : 0);
}

void
DrawProg::set_size(float x, float y)
{
	piglit_Uniform2f(size_loc, x, y);
}

void
DrawProg::set_offset(float x, float y)
{
	piglit_Uniform2f(offset_loc, x, y);
}

DrawProg *draw_prog = NULL;

class DownsampleProg
{
public:
	DownsampleProg();
	void use();
	void set_samp(int texture_num);

private:
	GLint prog;
	GLint upsample_factor_loc;
	GLint samp_loc;
};

DownsampleProg::DownsampleProg()
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
		"uniform int upsample_factor;\n"
		"in vec2 texCoordVarying;\n"
		"void main()\n"
		"{\n"
		"  vec4 sum = vec4(0.0);\n"
		"  ivec2 pixel = ivec2(texCoordVarying);\n"
		"  for (int i = 0; i < upsample_factor; ++i) {\n"
		"    for (int j = 0; j < upsample_factor; ++j) {\n"
		"      sum += texelFetch(\n"
		"          samp, pixel * upsample_factor + ivec2(i, j), 0);\n"
		"    }\n"
		"  }\n"
		"  gl_FragColor = sum / (upsample_factor * upsample_factor);\n"
		"}\n";

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
	upsample_factor_loc = piglit_GetUniformLocation(prog, "upsample_factor");
	samp_loc = piglit_GetUniformLocation(prog, "samp");
}

void
DownsampleProg::use()
{
	piglit_UseProgram(prog);
	piglit_Uniform1i(upsample_factor_loc, UPSAMPLE_FACTOR);
}

void
DownsampleProg::set_samp(int texture_num)
{
	piglit_Uniform1i(samp_loc, texture_num);
}

DownsampleProg *downsample_prog = NULL;

class TestShape
{
public:
	TestShape(int tile_num);
	void draw(float x_size, float y_size, float x_offset, float y_offset);
	void draw_tile();
	void draw_reference();

private:
	int x_tile;
	int y_tile;
	int y_tile_neg;
	float v0;
	float v1;
	bool rotated;
};

TestShape::TestShape(int tile_num)
	: x_tile(tile_num % NUM_HORIZ_TILES),
	  y_tile(tile_num / NUM_HORIZ_TILES),
	  y_tile_neg(NUM_VERT_TILES - 1 - y_tile),
	  v0(float(tile_num % (NUM_TOTAL_TILES/2)) / (NUM_TOTAL_TILES/2)),
	  v1(1.0 - v0),
	  rotated(tile_num >= NUM_TOTAL_TILES/2)
{
}

void
TestShape::draw(float x_size, float y_size, float x_offset, float y_offset)
{
	/* Compute quad coordinates in uv space */
	float quad[4][2] = {
		{ 0, 0 },
		{ 0, v0 },
		{ 1, v1 },
		{ 1, 0 }
	};

	unsigned int indices[6] = { 0, 1, 2, 0, 2, 3 };

	draw_prog->use();
	draw_prog->set_rotated(rotated);
	draw_prog->set_size(x_size, y_size);
	draw_prog->set_offset(x_offset, y_offset);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(quad[0]),
			      &quad);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
}

void
TestShape::draw_tile()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, multisample_fbo->handle);
	multisample_fbo->set_viewport();
	draw(2.0, 2.0, -1.0, -1.0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, multisample_fbo->handle);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	int x0 = TILE_SIZE * x_tile;
	int x1 = TILE_SIZE * (x_tile + 1);
	int y0 = TILE_SIZE * y_tile_neg;
	int y1 = TILE_SIZE * (y_tile_neg + 1);
	glBlitFramebuffer(0, 0, TILE_SIZE, TILE_SIZE, x0, y0, x1, y1,
			  GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void
TestShape::draw_reference()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, upsample_fbo->handle);
	upsample_fbo->set_viewport();
	draw(2.0, 2.0, -1.0, -1.0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, piglit_width, piglit_height);
	downsample_prog->use();
	glActiveTexture(GL_TEXTURE0);
	downsample_prog->set_samp(0);
	glBindTexture(GL_TEXTURE_2D, upsample_fbo->tex);
	float x0 = float(x_tile) / NUM_HORIZ_TILES * 2.0 - 1.0;
	float x1 = float(x_tile + 1) / NUM_HORIZ_TILES * 2.0 - 1.0;
	float y0 = float(y_tile_neg) / NUM_VERT_TILES * 2.0 - 1.0;
	float y1 = float(y_tile_neg + 1) / NUM_VERT_TILES * 2.0 - 1.0;
	float vertices[4][2] = {
		{ x0, y0 },
		{ x0, y1 },
		{ x1, y1 },
		{ x1, y0 }
	};
	float texCoords[4][2] = {
		{ 0, 0 },
		{ 0, TILE_SIZE },
		{ TILE_SIZE, TILE_SIZE },
		{ TILE_SIZE, 0 }
	};
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]),
			      &vertices);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(texCoords[0]),
			      &texCoords);
	unsigned int indices[6] = { 0, 1, 2, 0, 2, 3 };
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
}

void
draw_pattern()
{
	for (int i = 0; i < NUM_TOTAL_TILES; ++i) {
		TestShape(i).draw_reference();
	}
}

enum piglit_result
piglit_display()
{
	draw_pattern();

	piglit_present_results();

	return PIGLIT_PASS;
}

void
piglit_init(int argc, char **argv)
{
	multisample_fbo = new Fbo(true, /* multisampled */
				  TILE_SIZE, TILE_SIZE);
	upsample_fbo = new Fbo(false, /* multisampled */
			       TILE_SIZE * UPSAMPLE_FACTOR,
			       TILE_SIZE * UPSAMPLE_FACTOR);

	downsample_prog = new DownsampleProg();
	draw_prog = new DrawProg();
	draw_prog->use();
}
