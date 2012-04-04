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

int piglit_width = TILE_SIZE * NUM_HORIZ_TILES;
int piglit_height = TILE_SIZE * NUM_VERT_TILES;
int piglit_window_mode = GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE;

GLuint fbo;

class Tile
{
public:
	Tile(int tile_num);
	void populate_vertices(float (&vertices)[4][2]);

private:
	int x_tile;
	int y_tile;
	float v0;
	float v1;
	bool rotated;
};

Tile::Tile(int tile_num)
	: x_tile(tile_num % NUM_HORIZ_TILES),
	  y_tile(NUM_VERT_TILES - 1 - tile_num / NUM_HORIZ_TILES),
	  v0(float(tile_num % (NUM_TOTAL_TILES/2)) / (NUM_TOTAL_TILES/2)),
	  v1(1.0 - v0),
	  rotated(tile_num >= NUM_TOTAL_TILES/2)
{
}

void
Tile::populate_vertices(float (&vertices)[4][2])
{
	/* Compute quad coordinates in uv space */
	float quad[4][2] = {
		{ 0, 0 },
		{ 0, v0 },
		{ 1, v1 },
		{ 1, 0 }
	};

	/* Rotate if necessary */
	if (rotated) {
		for (int i = 0; i < 4; ++i) {
			float u = quad[i][0];
			float v = quad[i][1];
			quad[i][0] = v;
			quad[i][1] = 1.0 - u;
		}
	}

	/* Scale down to tile size and apply offset */
	float x_offset = float(2*x_tile) / NUM_HORIZ_TILES - 1.0;
	float y_offset = float(2*y_tile) / NUM_VERT_TILES - 1.0;
	for (int i = 0; i < 4; ++i) {
		vertices[i][0] =
			x_offset + quad[i][0] * (2.0 / NUM_HORIZ_TILES);
		vertices[i][1] =
			y_offset + quad[i][1] * (2.0 / NUM_VERT_TILES);
	}
}

void
draw_pattern(void)
{
	float vertices[NUM_TOTAL_TILES][4][2];
	unsigned int indices[NUM_TOTAL_TILES][6];

	for (int i = 0; i < NUM_TOTAL_TILES; ++i) {
		Tile(i).populate_vertices(vertices[i]);
		indices[i][0] = 4*i;
		indices[i][1] = 4*i + 1;
		indices[i][2] = 4*i + 2;
		indices[i][3] = 4*i;
		indices[i][4] = 4*i + 2;
		indices[i][5] = 4*i + 3;
	}

	glVertexPointer(2, GL_FLOAT, sizeof(vertices[0][0]), &vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawElements(GL_TRIANGLES, 6*NUM_TOTAL_TILES, GL_UNSIGNED_INT,
		       indices);
}

enum piglit_result
piglit_display(void)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	draw_pattern();
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, 256, 256, 0, 0, piglit_width, piglit_height,
			  GL_COLOR_BUFFER_BIT, GL_NEAREST);

	piglit_present_results();

	return PIGLIT_PASS;
}

void
piglit_init(int argc, char **argv)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);

	GLuint rb;
	glGenRenderbuffers(1, &rb);
	glBindRenderbuffer(GL_RENDERBUFFER, rb);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA,
					 256, 256);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				  GL_RENDERBUFFER, rb);

	if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer not complete\n");
		piglit_report_result(PIGLIT_FAIL);
	}
}
