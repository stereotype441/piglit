#include <stdint.h>
#include <stddef.h>

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
typedef int64_t GLint64EXT;
typedef uint64_t GLuint64EXT;
typedef GLint64EXT  GLint64;
typedef GLuint64EXT GLuint64;
typedef struct __GLsync *GLsync;

typedef char GLchar;

typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

typedef ptrdiff_t GLintptrARB;
typedef ptrdiff_t GLsizeiptrARB;

typedef char GLcharARB;
typedef unsigned int GLhandleARB;

typedef void piglit_dispatch_function(void);

typedef piglit_dispatch_function *piglit_get_proc_address_function(const char *);

typedef void piglit_error_function(const char *);

typedef enum {
	PIGLIT_DISPATCH_GL,
	PIGLIT_DISPATCH_GL_FWD,
	PIGLIT_DISPATCH_ES1,
	PIGLIT_DISPATCH_ES2
} piglit_dispatch_api;

void piglit_dispatch_init(piglit_dispatch_api api,
			  piglit_get_proc_address_function *get_core_proc,
			  piglit_get_proc_address_function *get_ext_proc,
			  piglit_error_function *unsupported_proc,
			  piglit_error_function *failure_proc);

#include "generated_dispatch.h"

#define GLEW_VERSION_1_2 (piglit_get_gl_version() >= 12)
#define GLEW_VERSION_1_3 (piglit_get_gl_version() >= 13)
#define GLEW_VERSION_1_4 (piglit_get_gl_version() >= 14)
#define GLEW_VERSION_1_5 (piglit_get_gl_version() >= 15)
#define GLEW_VERSION_2_0 (piglit_get_gl_version() >= 20)
#define GLEW_VERSION_3_0 (piglit_get_gl_version() >= 30)
#define GLEW_VERSION_3_2 (piglit_get_gl_version() >= 32)
#define GLEW_3DFX_texture_compression_FXT1 (piglit_is_extension_supported("3DFX_texture_compression_FXT1"))
#define GLEW_ARB_ES2_compatibility (piglit_is_extension_supported("ARB_ES2_compatibility"))
#define GLEW_ARB_depth_texture (piglit_is_extension_supported("ARB_depth_texture"))
#define GLEW_ARB_draw_buffers (piglit_is_extension_supported("ARB_draw_buffers"))
#define GLEW_ARB_fragment_program (piglit_is_extension_supported("ARB_fragment_program"))
#define GLEW_ARB_fragment_shader (piglit_is_extension_supported("ARB_fragment_shader"))
#define GLEW_ARB_geometry_shader4 (piglit_is_extension_supported("ARB_geometry_shader4"))
#define GLEW_ARB_imaging (piglit_is_extension_supported("ARB_imaging"))
#define GLEW_ARB_occlusion_query (piglit_is_extension_supported("ARB_occlusion_query"))
#define GLEW_ARB_shader_objects (piglit_is_extension_supported("ARB_shader_objects"))
#define GLEW_ARB_shader_texture_lod (piglit_is_extension_supported("ARB_shader_texture_lod"))
#define GLEW_ARB_shading_language_100 (piglit_is_extension_supported("ARB_shading_language_100"))
#define GLEW_ARB_texture_compression_bptc (piglit_is_extension_supported("ARB_texture_compression_bptc"))
#define GLEW_ARB_texture_compression_rgtc (piglit_is_extension_supported("ARB_texture_compression_rgtc"))
#define GLEW_ARB_texture_env_combine (piglit_is_extension_supported("ARB_texture_env_combine"))
#define GLEW_ARB_texture_float (piglit_is_extension_supported("ARB_texture_float"))
#define GLEW_ARB_texture_non_power_of_two (piglit_is_extension_supported("ARB_texture_non_power_of_two"))
#define GLEW_ARB_texture_rg (piglit_is_extension_supported("ARB_texture_rg"))
#define GLEW_ARB_vertex_program (piglit_is_extension_supported("ARB_vertex_program"))
#define GLEW_ARB_vertex_shader (piglit_is_extension_supported("ARB_vertex_shader"))
#define GLEW_ATI_separate_stencil (piglit_is_extension_supported("ATI_separate_stencil"))
#define GLEW_ATI_texture_compression_3dc (piglit_is_extension_supported("ATI_texture_compression_3dc"))
#define GLEW_EXT_blend_minmax (piglit_is_extension_supported("EXT_blend_minmax"))
#define GLEW_EXT_fog_coord (piglit_is_extension_supported("EXT_fog_coord"))
#define GLEW_EXT_geometry_shader4 (piglit_is_extension_supported("EXT_geometry_shader4"))
#define GLEW_EXT_stencil_wrap (piglit_is_extension_supported("EXT_stencil_wrap"))
#define GLEW_EXT_texture_compression_latc (piglit_is_extension_supported("EXT_texture_compression_latc"))
#define GLEW_EXT_texture_compression_rgtc (piglit_is_extension_supported("EXT_texture_compression_rgtc"))
#define GLEW_EXT_texture_compression_s3tc (piglit_is_extension_supported("EXT_texture_compression_s3tc"))
#define GLEW_EXT_texture_sRGB (piglit_is_extension_supported("EXT_texture_sRGB"))
#define GLEW_EXT_transform_feedback (piglit_is_extension_supported("EXT_transform_feedback"))
#define GLEW_NV_blend_square (piglit_is_extension_supported("NV_blend_square"))
#define GLEW_NV_geometry_shader4 (piglit_is_extension_supported("NV_geometry_shader4"))
#define GLEW_OES_compressed_paletted_texture (piglit_is_extension_supported("OES_compressed_paletted_texture"))
void glewInit();

// Prevent gl.h from being included, since it will attempt to define
// the functions we've already defined.
#define __gl_h_

#ifdef __cplusplus
} /* end extern "C" */
#endif
