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

int piglit_width = 16;
int piglit_height = 16;
int piglit_window_mode = GLUT_RGBA;

namespace {

const int max_miplevel = 10;

GLuint image_tex;
GLuint depth_tex;

GLuint
create_mipmapped_tex(int dim, GLenum format)
{
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	for (int level = 0; dim > 0; ++level, dim /= 2) {
		glTexImage2D(GL_TEXTURE_2D, level, format,
			     dim, dim,
			     0,
			     format, GL_UNSIGNED_BYTE, NULL);
	}
	return tex;
}

void
set_up_framebuffer_for_miplevel(int level)
{
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
			       GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
			       image_tex, level);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
			       GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
			       depth_tex, level);
	GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "FBO incomplete at miplevel %d\n",
			level);
		piglit_report_result(PIGLIT_FAIL);
	}
}

extern "C" void
piglit_init(int argc, char **argv)
{
	bool workaround = false;

	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "workaround") == 0)
			workaround = true;
		else {
			printf("Huh?\n");
			piglit_report_result(PIGLIT_FAIL);
		}
	}

	bool pass = true;

	image_tex = create_mipmapped_tex(1 << max_miplevel, GL_RGBA);
	depth_tex = create_mipmapped_tex(1 << max_miplevel,
						GL_DEPTH_COMPONENT);

	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);

	for (int level = 0; level <= max_miplevel; ++level) {
		set_up_framebuffer_for_miplevel(level);
		glClearDepth(double(level) / max_miplevel);
		glClear(GL_DEPTH_BUFFER_BIT);
		if (workaround) {
			pass = piglit_probe_rect_depth(0, 0, 1, 1,
						       double(level)
						       / max_miplevel);
		}
	}

	for (int level = 0; level <= max_miplevel; ++level) {
		printf("Probing miplevel %d depth\n", level);
		set_up_framebuffer_for_miplevel(level);
		int dim = 1 << (max_miplevel - level);
		pass = piglit_probe_rect_depth(0, 0, dim, dim,
					       double(level) / max_miplevel)
			&& pass;
	}

	piglit_report_result(pass ? PIGLIT_PASS : PIGLIT_FAIL);
}

extern "C" enum piglit_result
piglit_display()
{
	/* Should never be reached */
	return PIGLIT_FAIL;
}

};
