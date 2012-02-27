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

typedef void (*GLDEBUGPROCARB)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,GLvoid *userParam);

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

void glewInit();

// Prevent gl.h from being included, since it will attempt to define
// the functions we've already defined.
#define __gl_h_

#ifdef __cplusplus
} /* end extern "C" */
#endif
