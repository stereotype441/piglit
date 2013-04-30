#include "piglit-util-gl-common.h"

PIGLIT_GL_TEST_CONFIG_BEGIN

	config.supports_gl_compat_version = 10;

	config.window_width = 512;
	config.window_height = 512;
	config.window_visual = PIGLIT_GL_VISUAL_RGBA | PIGLIT_GL_VISUAL_DOUBLE;

PIGLIT_GL_TEST_CONFIG_END

void
piglit_init(int argc, char **argv)
{
	int x, y;
	char *endptr = NULL;
	GLuint fb, color_rb;

	if (argc != 3) {
		printf("Wrong # of args\n");
		piglit_report_result(PIGLIT_FAIL);
	}
	x = strtol(argv[1], &endptr, 0);
	if (endptr != argv[1] + strlen(argv[1])) {
		printf("Bad x\n");
		piglit_report_result(PIGLIT_FAIL);
	}
	y = strtol(argv[2], &endptr, 0);
	if (endptr != argv[2] + strlen(argv[2])) {
		printf("Bad y\n");
		piglit_report_result(PIGLIT_FAIL);
	}

	glGenFramebuffers(1, &fb);
	glGenRenderbuffers(1, &color_rb);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb);
	glBindRenderbuffer(GL_RENDERBUFFER, color_rb);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 0, GL_RGBA32F, 2048, 2048);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color_rb);
	glScissor(x, y, 1, 1);
	glEnable(GL_SCISSOR_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	glFinish();
	piglit_report_result(PIGLIT_PASS);
}

enum piglit_result
piglit_display(void)
{
	return PIGLIT_FAIL;
}
