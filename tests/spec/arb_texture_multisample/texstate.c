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

static bool
check_texlevelparameter_int(GLuint target, GLuint level,
        char const *name, GLuint pname, GLint expected_value)
{
    GLint actual_value;
    glGetTexLevelParameteriv(target, level, pname, &actual_value);
    if (!piglit_check_gl_error(GL_NO_ERROR))
        return false;

    if (actual_value != expected_value) {
        printf("Expected %s value of %d but got %d\n",
                name, expected_value, actual_value);
        return false;
    }

    return true;
}

void
piglit_init(int argc, char **argv)
{
    GLuint tex2d;
    bool pass = true;
    piglit_require_extension("GL_ARB_texture_multisample");

    /* check that new image state required by ARB_texture_multisample
     * exists, and has correct defaults. Tests against a non-multisample
     * texture target, since this state exists on all images. */

    glGenTextures(1, &tex2d);
    glBindTexture(GL_TEXTURE_2D, tex2d);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    pass = check_texlevelparameter_int(GL_TEXTURE_2D, 0, "GL_TEXTURE_SAMPLES",
            GL_TEXTURE_SAMPLES, 0) && pass;
    pass = check_texlevelparameter_int(GL_TEXTURE_2D, 0, "GL_TEXTURE_FIXED_SAMPLE_LOCATIONS",
            GL_TEXTURE_FIXED_SAMPLE_LOCATIONS, GL_TRUE) && pass;

    piglit_report_result(pass ? PIGLIT_PASS : PIGLIT_FAIL);
}
