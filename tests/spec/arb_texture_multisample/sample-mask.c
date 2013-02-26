#include "piglit-util-gl-common.h"

PIGLIT_GL_TEST_CONFIG_BEGIN

    config.supports_gl_compat_version = 30;
    config.window_visual = PIGLIT_GL_VISUAL_RGB;

PIGLIT_GL_TEST_CONFIG_END

enum piglit_result
piglit_display(void)
{
    return PIGLIT_FAIL;
}

void
piglit_init(int argc, char **argv)
{
    GLint maskOn = 0;
    piglit_require_extension("GL_ARB_texture_multisample");

    printf("Checking GL_SAMPLE_MASK exists\n");
    if (glIsEnabled(GL_SAMPLE_MASK)) {
        printf("GL_SAMPLE_MASK enabled by default, should be disabled.\n");
        piglit_report_result(PIGLIT_FAIL);
    }

    if (!piglit_check_gl_error(GL_NO_ERROR))
        piglit_report_result(PIGLIT_FAIL);

    printf("Checking GL_SAMPLE_MASK works with GetIntegerv\n");
    glGetIntegerv(GL_SAMPLE_MASK, &maskOn);

    if (!piglit_check_gl_error(GL_NO_ERROR))
        piglit_report_result(PIGLIT_FAIL);

    if (maskOn) {
        printf("GetIntegerv(GL_SAMPLE_MASK) true by default, should be false\n");
        piglit_report_result(PIGLIT_FAIL);
    }

    piglit_report_result(PIGLIT_PASS);
}
