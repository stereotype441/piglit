#include "piglit-util-gl-common.h"

PIGLIT_GL_TEST_CONFIG_BEGIN

    config.supports_gl_compat_version = 30;

    config.window_visual = PIGLIT_GL_VISUAL_RGBA | PIGLIT_GL_VISUAL_DOUBLE;

PIGLIT_GL_TEST_CONFIG_END

enum piglit_result
piglit_display(void)
{
    return PIGLIT_FAIL;
}

void
piglit_init(int argc, char **argv)
{
    /* test some new error cases */

    GLuint fbo, tex;
    glGenFramebuffers(1, &fbo);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, tex);
    glTexImage3DMultisample(GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
            4, GL_RGBA, 64, 64, 2, GL_TRUE);

    if (!piglit_check_gl_error(GL_NO_ERROR)) {
        printf("should be no error so far\n");
        piglit_report_result(PIGLIT_FAIL);
    }

    /* binding a negative layer should fail */
    glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0, -1);

    if (!piglit_check_gl_error(GL_INVALID_VALUE)) {
        printf("glFramebufferTextureLayer w/ negative layer must "
                "emit GL_INVALID_VALUE but did not\n");
        piglit_report_result(PIGLIT_FAIL);
    }

    piglit_report_result(PIGLIT_PASS);
}
