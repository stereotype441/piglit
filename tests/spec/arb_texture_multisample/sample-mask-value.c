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
    GLint maxMaskWords = 0, result = 0, mask = 0;
    int i;

    piglit_require_extension("GL_ARB_texture_multisample");

    glGetIntegerv(GL_MAX_SAMPLE_MASK_WORDS, &maxMaskWords);

    for (i=0; i < maxMaskWords; i++) {
        glGetIntegeri_v(GL_SAMPLE_MASK_VALUE, i, &mask);

        if (!piglit_check_gl_error(GL_NO_ERROR)) {
            printf("Could not get word %d of sample mask value\n", i);
            piglit_report_result(PIGLIT_FAIL);
        }

        if ((GLuint)mask != ~0u) {
            printf("Initial mask for word %d is bogus; expected all bits set, got %08x\n",
                   i, mask);
        }
    }

    printf("Checking that correct errors are generated for out of bounds\n");
    glGetIntegeri_v(GL_SAMPLE_MASK_VALUE, maxMaskWords, &result);

    if (!piglit_check_gl_error(GL_INVALID_VALUE))
        piglit_report_result(PIGLIT_FAIL);

    piglit_report_result(PIGLIT_PASS);
}
