#include "piglit-util-gl-common.h"

PIGLIT_GL_TEST_CONFIG_BEGIN

	config.supports_gl_compat_version = 10;

	config.window_width = 128;
	config.window_height = 128;
	config.requires_displayed_window = true;
	config.window_visual = PIGLIT_GL_VISUAL_DOUBLE | PIGLIT_GL_VISUAL_RGB;

PIGLIT_GL_TEST_CONFIG_END

void
piglit_init(int argc, char **argv)
{
}

enum piglit_result
piglit_display(void)
{
	//	piglit_swap_buffers();
	glDrawBuffer(GL_FRONT);
	glClearColor(0.0, 1.0, 0.0, 1.0);
	glFlush();
	glFinish();
	return PIGLIT_PASS;
}
