// FOO
#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef short GLshort;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;
#if defined(_MSC_VER) && _MSC_VER < 1400
typedef __int64 GLint64EXT;
typedef unsigned __int64 GLuint64EXT;
#elif defined(_MSC_VER) || defined(__BORLANDC__)
typedef signed long long GLint64EXT;
typedef unsigned long long GLuint64EXT;
#else
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#include <inttypes.h>
#  endif
#include <stdint.h>
typedef int64_t GLint64EXT;
typedef uint64_t GLuint64EXT;
#endif
typedef GLint64EXT  GLint64;
typedef GLuint64EXT GLuint64;
typedef struct __GLsync *GLsync;
typedef double GLclampd;
typedef void GLvoid;
#if defined(_MSC_VER) && _MSC_VER < 1400
typedef __int64 GLint64EXT;
typedef unsigned __int64 GLuint64EXT;
#elif defined(_MSC_VER) || defined(__BORLANDC__)
typedef signed long long GLint64EXT;
typedef unsigned long long GLuint64EXT;
#else
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#include <inttypes.h>
#  endif
typedef int64_t GLint64EXT;
typedef uint64_t GLuint64EXT;
#endif
typedef GLint64EXT  GLint64;
typedef GLuint64EXT GLuint64;
typedef struct __GLsync *GLsync;

typedef char GLchar;

#include <stddef.h> // TODO: make more portable

typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

typedef ptrdiff_t GLintptrARB;
typedef ptrdiff_t GLsizeiptrARB;

typedef char GLcharARB;
typedef unsigned int GLhandleARB;

#include "wrappers.h"

#define GLEW_VERSION_1_3 (piglit_get_gl_version() >= 13)
#define GLEW_VERSION_1_4 (piglit_get_gl_version() >= 14)
#define GLEW_VERSION_1_5 (piglit_get_gl_version() >= 15)
#define GLEW_VERSION_2_0 (piglit_get_gl_version() >= 20)
#define GLEW_VERSION_3_0 (piglit_get_gl_version() >= 30)
#define GLEW_ARB_depth_texture (piglit_is_extension_supported("ARB_depth_texture"))
#define GLEW_ARB_fragment_program (piglit_is_extension_supported("ARB_fragment_program"))
#define GLEW_ARB_fragment_shader (piglit_is_extension_supported("ARB_fragment_shader"))
#define GLEW_ARB_shader_objects (piglit_is_extension_supported("ARB_shader_objects"))
#define GLEW_ARB_shading_language_100 (piglit_is_extension_supported("ARB_shading_language_100"))
#define GLEW_ARB_texture_env_combine (piglit_is_extension_supported("ARB_texture_env_combine"))
#define GLEW_ARB_texture_non_power_of_two (piglit_is_extension_supported("ARB_texture_non_power_of_two"))
#define GLEW_ARB_vertex_program (piglit_is_extension_supported("ARB_vertex_program"))
#define GLEW_ARB_vertex_shader (piglit_is_extension_supported("ARB_vertex_shader"))
#define GLEW_EXT_transform_feedback (piglit_is_extension_supported("EXT_transform_feedback"))
#define glewInit() do { } while (0)

// Why is this necessary?
#define GL_COMPRESSED_LUMINANCE_ALPHA_3DC_ATI 0x8837

#ifdef __cplusplus
} /* end extern "C" */
#endif
