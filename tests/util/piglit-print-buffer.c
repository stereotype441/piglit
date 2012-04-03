/*
 * Copyright (c) The Piglit project 2007
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "piglit-print-buffer.h"

void
piglit_print_readpixels(int width, int height, GLenum format, GLenum type)
{
	int x;
	int y;

	uint8_t *pixels = 0;
	int components = 0;
	int component_size = 0;
	int pixel_size = 0;

	int x_tick_width = 0;
	int y_tick_width = 0;
	int component_width = 0;
	int pixel_width = 0;
	int cell_width = 0;

	switch (type) {
	case GL_UNSIGNED_BYTE:
		component_size = 1;
		component_width = 2;
		break;
	case GL_UNSIGNED_INT:
		component_size = 4;
		component_width = 8;
		break;
#ifdef USE_OPENGL
	case GL_UNSIGNED_INT_24_8:
		/* print format: dddddd|ss */
		component_size = 4;
		component_width = 9;
		break;
#endif
	case GL_FLOAT:
		/* print format: 1234.123456 */
		component_size = 4;
		component_width = 11;
		break;
	default:
		fprintf(stderr, "not implemented\n");
		abort();
		break;
	}

	switch (format) {
#ifdef USE_OPENGL
	case GL_DEPTH_COMPONENT:
	case GL_DEPTH_STENCIL:
		components = 1;
		break;
#endif
	case GL_RGB:
		components = 3;
		break;
	case GL_RGBA:
		components = 4;
		break;
	default:
		fprintf(stderr, "not implemented\n");
		abort();
		break;
	}

	x_tick_width = (int) ceil(log(width) / log(10));;
	y_tick_width = (int) ceil((log(height) / log(10)));
	pixel_width = components * (component_width + 1) - 1;
	cell_width = 1 + (int) fmax(x_tick_width, pixel_width);

	pixel_size = components * component_size;
	pixels = calloc(width * height, pixel_size);
	glReadPixels(0, 0, width, height, format, type, pixels);
	assert(glGetError() == 0);

	for (y = height - 1; y >= 0; --y) {
		/* Print y tick. */
		printf("%*d|", y_tick_width, y);

		/* Print row of pixels. */
		for (x = 0; x < width; ++x) {
			const uint8_t *pixel = &pixels[pixel_size * (y * width + x)];
			int c; // component iterator

			/* Print leading space. */
			const int leading_space = cell_width - pixel_width;
			int j;
			for (j = 0; j < leading_space; ++j) {
				printf(" ");
			}

			/* Print one pixel. */
			for (c = 0; c < components; ++c) {
			   switch (type) {
			   case GL_UNSIGNED_BYTE:
				   printf("%2.2x", ((GLubyte*)pixel)[c]);
				   break;
			   case GL_UNSIGNED_INT:
				   printf("%8x", ((GLuint*)pixel)[c]);
				   break;
   #ifdef USE_OPENGL
			   case GL_UNSIGNED_INT_24_8: {
				   GLuint depth = (((GLuint*)pixel)[c]) >> 8;
				   GLubyte stencil = (((GLuint*)pixel)[c]) & 0xff;
				   printf("%6x|%2x", depth, stencil);
				   break;
			   }
   #endif
			   case GL_FLOAT:
				   printf("%11.6f", ((GLfloat*)pixel)[c]);
				   break;
			   default:
				   assert(0);
				   break;
			   }

			   if (c != components - 1) {
			      printf(",");
			   }
			}
		}
		printf("\n");
	}

	/* Print x axis. */
	for (x = 0; x < y_tick_width; ++x) {
		printf("-");
	}
	printf("+");
	for (x = 0; x < cell_width * width; ++x) {
		printf("-");
	}
	printf("\n");

	/* Print x ticks. */
	for (x = 0; x < y_tick_width; ++x) {
		printf(" ");
	}
	printf("|");
	for (x = 0; x < width; ++x) {
		printf("%*d", cell_width, x);
	}
	printf("\n");
}
