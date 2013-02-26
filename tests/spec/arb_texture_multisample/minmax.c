#include "piglit-util-gl-common.h"
#include "minmax-test.h"

PIGLIT_GL_TEST_CONFIG_BEGIN

    config.supports_gl_compat_version = 10;
    config.supports_gl_core_version = 31;
    config.window_visual = PIGLIT_GL_VISUAL_RGB;

PIGLIT_GL_TEST_CONFIG_END

void
piglit_init(int argc, char **argv)
{
    piglit_require_extension("GL_ARB_texture_multisample");
    piglit_print_minmax_header();

    piglit_test_min_int(GL_MAX_SAMPLE_MASK_WORDS, 1);
    piglit_test_min_int(GL_MAX_COLOR_TEXTURE_SAMPLES, 1);
    piglit_test_min_int(GL_MAX_DEPTH_TEXTURE_SAMPLES, 1);
    piglit_test_min_int(GL_MAX_INTEGER_SAMPLES, 1);

    piglit_report_result(piglit_minmax_pass ? PIGLIT_PASS : PIGLIT_FAIL);
}

enum piglit_result
piglit_display(void)
{
    return PIGLIT_FAIL;
}
