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

/** \file depthstencil-render-miplevels.cpp
 *
 * Test that data rendered to color, depth, and stencil textures
 * always lands at the correct miplevel.
 *
 * This test operates by creating a set of texture buffers, attaching
 * them to a framebuffer one miplevel at a time, and rendering
 * different data into each miplevel.  Then it verifies, using
 * glReadPixels, that the correct data appears at each miplevel.
 *
 * This is useful in diagnosing bugs such as:
 *
 * - Incorrect miplevels being attached to the framebuffer
 *
 * - Miplevels being laid out incorrectly in memory (e.g. in an
 *   overlapping fashion)
 *
 * Usage: depthstencil-render-miplevels <buffer_combination> [options]
 *
 *  buffer_combination:          buffer attachments (other than color->RGBA):
 *  color                        None
 *  stencil                      stencil->DEPTH_STENCIL
 *  depth_x                      depth->DEPTH_STENCIL
 *  depth                        depth->DEPTH_COMPONENT
 *  d16                          depth->DEPTH_COMPONENT16
 *  depth_x_and_stencil          depth->DEPTH_STENCIL, stencil->DEPTH_STENCIL
 *  stencil_and_depth_x          (as above, but stencil attached first)
 *  depth_and_stencil            depth->DEPTH_COMPONENT, stencil->DEPTH_STENCIL
 *  stencil_and_depth            (as above, but stencil attached first)
 *  depth_stencil_shared         depth->DEPTH_STENCIL<-stencil
 *  stencil_depth_shared         (as above, but stencil attached first)
 *  depth_stencil_single_binding depth_stencil->DEPTH_STENCIL
 *
 * Note: the buffer attachments are diagrammed above as
 * "attachment_point->BUFFER_TYPE".  So, for example:
 *
 * - "depth->DEPTH_COMPONENT, stencil->DEPTH_STENCIL" means there is a
 *   texture of type GL_DEPTH_COMPONENT attached to
 *   GL_DEPTH_ATTACHMENT, and a separate texture of type
 *   GL_DEPTH_STENCIL attached to GL_STENCIL_ATTACHMENT.
 *
 * - "depth->DEPTH_STENCIL<-stencil" means there is a single texture
 *   of type GL_DEPTH_STENCIL attached to both GL_DEPTH_ATTACHMENT and
 *   GL_STENCIL_ATTACHMENT.
 *
 * - "depth_stencil->DEPTH_STENCIL" means there is a single texture of
 *   type GL_DEPTH_STENCIL attached to the attachment point
 *   GL_DEPTH_STENCIL_ATTACHMENT.
 *
 * Available options:
 *
 * - detach_between_miplevels: causes all textures to be detached from
 *   the framebuffer when switching from one miplevel to the next, so
 *   that the textures for each miplevel get bound to an empty
 *   framebuffer.
 *
 * - sequential: causes the result of rending each miplevel to be
 *   tested before moving on to the next miplevel.  If this option is
 *   not specified, then all miplevels are rendered before any
 *   miplevels are tested.
 */

#include "piglit-util.h"

int piglit_width = 16;
int piglit_height = 16;
int piglit_window_mode = GLUT_RGBA;

namespace {

GLuint color_tex;
GLuint depth_tex;
GLuint stencil_tex;
bool attach_depth = false;
bool attach_stencil = false;
bool shared_attachment = false;
bool attach_together = false;
bool attach_stencil_first = false;
bool depth_attachment_lacks_stencil = false;
bool depth_attachment_16bit = false;
bool detach_between_miplevels = false;
bool sequential = false;
int miplevel0_size;
int max_miplevel;

/* Create a mipmapped texture with the given dimensions and internal format. */
GLuint
create_mipmapped_tex(GLenum internal_format)
{
	GLenum format = internal_format == GL_DEPTH_COMPONENT16
		? GL_DEPTH_COMPONENT : internal_format;
	GLenum type = format == GL_DEPTH_STENCIL
		? GL_UNSIGNED_INT_24_8 : GL_UNSIGNED_BYTE;
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	for (int level = 0; level <= max_miplevel; ++level) {
		int dim = miplevel0_size >> level;
		glTexImage2D(GL_TEXTURE_2D, level, internal_format,
			     dim, dim,
			     0,
			     format, type, NULL);
		if (!piglit_check_gl_error(GL_NO_ERROR))
			piglit_report_result(PIGLIT_FAIL);
	}
	return tex;
}

/* Attach the proper miplevel of each texture to the framebuffer */
void
set_up_framebuffer_for_miplevel(int level)
{
	if (detach_between_miplevels) {
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
				       GL_COLOR_ATTACHMENT0,
				       GL_TEXTURE_2D, 0, 0);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
				       GL_DEPTH_ATTACHMENT,
				       GL_TEXTURE_2D, 0, 0);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
				       GL_STENCIL_ATTACHMENT,
				       GL_TEXTURE_2D, 0, 0);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
				       GL_DEPTH_STENCIL_ATTACHMENT,
				       GL_TEXTURE_2D, 0, 0);
	}
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
			       GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
			       color_tex, level);
	if (attach_together) {
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
				       GL_DEPTH_STENCIL_ATTACHMENT,
				       GL_TEXTURE_2D, depth_tex, level);
	} else if (attach_stencil_first) {
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
				       GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
				       stencil_tex, level);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
				       GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
				       depth_tex, level);
	} else {
		if (attach_depth) {
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
					       GL_DEPTH_ATTACHMENT,
					       GL_TEXTURE_2D,
					       depth_tex, level);
		}
		if (attach_stencil) {
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
					       GL_STENCIL_ATTACHMENT,
					       GL_TEXTURE_2D,
					       stencil_tex, level);
		}
	}

	/* Some implementations don't support certain buffer
	 * combinations, and that's ok, provided that the
	 * implementation reports GL_FRAMEBUFFER_UNSUPPORTED.
	 * However, if the buffer combination was supported at
	 * miplevel 0, it should be supported at all miplevels.
	 */
	GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_UNSUPPORTED && level == 0) {
		printf("This buffer combination is unsupported\n");
		piglit_report_result(PIGLIT_SKIP);
	} else if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FBO incomplete at miplevel %d\n", level);
		piglit_report_result(PIGLIT_FAIL);
	}
}

/* Using glClear, set the contents of the color, depth, and stencil
 * buffers (if present) to a value that is unique to this miplevel.
 */
void
populate_miplevel(int level)
{
	float float_value = float(level + 1) / (max_miplevel + 1);
	GLbitfield clear_mask = GL_COLOR_BUFFER_BIT;

	glClearColor(float_value, float_value, float_value, float_value);
	if (attach_depth) {
		glClearDepth(float_value);
		clear_mask |= GL_DEPTH_BUFFER_BIT;
	}
	if (attach_stencil) {
		glClearStencil(level + 1);
		clear_mask |= GL_STENCIL_BUFFER_BIT;
	}

	glClear(clear_mask);
}

/* Test that every pixel in the color, depth, and stencil buffers (if
 * present) is equal to the value set by populate_miplevel.
 */
bool
test_miplevel(int level)
{
	bool pass = true;
	int dim = miplevel0_size >> level;
	float float_value = float(level + 1) / (max_miplevel + 1);
	float expected_color[] = {
		float_value, float_value, float_value, float_value
	};

	printf("Probing miplevel %d color\n", level);
	pass = piglit_probe_rect_rgba(0, 0, dim, dim, expected_color) && pass;

	if (attach_depth) {
		printf("Probing miplevel %d depth\n", level);
		pass = piglit_probe_rect_depth(0, 0, dim, dim, float_value)
			&& pass;
	}

	if (attach_stencil) {
		printf("Probing miplevel %d stencil\n", level);
		pass = piglit_probe_rect_stencil(0, 0, dim, dim, level + 1)
			&& pass;
	}

	return pass;
}

void
print_usage_and_exit(char *prog_name)
{
	printf("Usage: %s <texture_size> <buffer_combination> [options]\n"
	       "    buffer_combination:          buffer attachments (other than color->RGBA):\n"
	       "    color                        None\n"
	       "    stencil                      stencil->DEPTH_STENCIL\n"
	       "    depth_x                      depth->DEPTH_STENCIL\n"
	       "    depth                        depth->DEPTH_COMPONENT\n"
	       "    depth_x_and_stencil          depth->DEPTH_STENCIL, stencil->DEPTH_STENCIL\n"
	       "    stencil_and_depth_x          (as above, but stencil attached first)\n"
	       "    depth_and_stencil            depth->DEPTH_COMPONENT, stencil->DEPTH_STENCIL\n"
	       "    stencil_and_depth            (as above, but stencil attached first)\n"
	       "    depth_stencil_shared         depth->DEPTH_STENCIL<-stencil\n"
	       "    stencil_depth_shared         (as above, but stencil attached first)\n"
	       "    depth_stencil_single_binding depth/stencil->DEPTH_STENCIL\n"
	       "Available options:\n"
	       "    detach_between_miplevels\n"
	       "    sequential\n", prog_name);
	piglit_report_result(PIGLIT_FAIL);
}

extern "C" void
piglit_init(int argc, char **argv)
{
	if (argc < 3) {
		print_usage_and_exit(argv[0]);
	}

	/* argv[1]: texture size */
	{
		char *endptr = NULL;
		miplevel0_size = strtol(argv[1], &endptr, 0);
		if (endptr != argv[1] + strlen(argv[1]))
			print_usage_and_exit(argv[0]);

		/* Now figure out the appropriate value of max_miplevel for this size. */
		max_miplevel = 0;
		while ((miplevel0_size >> (max_miplevel + 1)) > 0)
			++max_miplevel;
	}

	/* argv[2]: buffer combination */
	if (strcmp(argv[2], "color") == 0) {
		/* Use default values of all parameters */
	} else if (strcmp(argv[2], "stencil") == 0) {
		attach_stencil = true;
	} else if (strcmp(argv[2], "depth_x") == 0) {
		attach_depth = true;
	} else if (strcmp(argv[2], "depth") == 0) {
		attach_depth = true;
		depth_attachment_lacks_stencil = true;
	} else if (strcmp(argv[2], "d16") == 0) {
		attach_depth = true;
		depth_attachment_lacks_stencil = true;
		depth_attachment_16bit = true;
	} else if (strcmp(argv[2], "depth_x_and_stencil") == 0) {
		attach_depth = true;
		attach_stencil = true;
	} else if (strcmp(argv[2], "depth_and_stencil") == 0) {
		attach_depth = true;
		attach_stencil = true;
		depth_attachment_lacks_stencil = true;
	} else if (strcmp(argv[2], "stencil_and_depth_x") == 0) {
		attach_depth = true;
		attach_stencil = true;
		attach_stencil_first = true;
	} else if (strcmp(argv[2], "stencil_and_depth") == 0) {
		attach_depth = true;
		attach_stencil = true;
		attach_stencil_first = true;
		depth_attachment_lacks_stencil = true;
	} else if (strcmp(argv[2], "depth_stencil_shared") == 0) {
		attach_depth = true;
		attach_stencil = true;
		shared_attachment = true;
	} else if (strcmp(argv[2], "stencil_depth_shared") == 0) {
		attach_depth = true;
		attach_stencil = true;
		shared_attachment = true;
		attach_stencil_first = true;
	} else if (strcmp(argv[2], "depth_stencil_single_binding") == 0) {
		attach_depth = true;
		attach_stencil = true;
		shared_attachment = true;
		attach_together = true;
	} else {
		print_usage_and_exit(argv[0]);
	}

	/* Remaining args: options */
	for (int i = 3; i < argc; ++i) {
		if (strcmp(argv[i], "detach_between_miplevels") == 0) {
			detach_between_miplevels = true;
		} else if (strcmp(argv[i], "sequential") == 0) {
			sequential = true;
		} else {
			print_usage_and_exit(argv[0]);
		}
	}

	bool pass = true;

	color_tex = create_mipmapped_tex(GL_RGBA);

	if (attach_depth) {
		if (depth_attachment_16bit) {
			depth_tex = create_mipmapped_tex(GL_DEPTH_COMPONENT16);
		} else if (depth_attachment_lacks_stencil) {
			depth_tex = create_mipmapped_tex(GL_DEPTH_COMPONENT);
		} else {
			depth_tex = create_mipmapped_tex(GL_DEPTH_STENCIL);
		}
	}

	if (attach_stencil) {
		if (shared_attachment) {
			stencil_tex = depth_tex;
		} else {
			stencil_tex = create_mipmapped_tex(GL_DEPTH_STENCIL);
		}
	}

	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);

	if (sequential) {
		for (int level = 0; level <= max_miplevel; ++level) {
			set_up_framebuffer_for_miplevel(level);
			populate_miplevel(level);
			pass = test_miplevel(level) && pass;
		}
	} else {
		for (int level = 0; level <= max_miplevel; ++level) {
			set_up_framebuffer_for_miplevel(level);
			populate_miplevel(level);
		}
		for (int level = 0; level <= max_miplevel; ++level) {
			set_up_framebuffer_for_miplevel(level);
			pass = test_miplevel(level) && pass;
		}
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
