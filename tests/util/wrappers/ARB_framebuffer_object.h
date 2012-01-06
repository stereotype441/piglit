GLboolean glIsRenderbuffer(GLuint renderbuffer);

void glBindRenderbuffer(GLenum target, GLuint renderbuffer);

void glDeleteRenderbuffers(GLsizei n, const GLuint * renderbuffers);

void glGenRenderbuffers(GLsizei n, GLuint * renderbuffers);

void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);

void glRenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);

void glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint * params);

GLboolean glIsFramebuffer(GLuint framebuffer);

void glBindFramebuffer(GLenum target, GLuint framebuffer);

void glDeleteFramebuffers(GLsizei n, const GLuint * framebuffers);

void glGenFramebuffers(GLsizei n, GLuint * framebuffers);

GLenum glCheckFramebufferStatus(GLenum target);

void glFramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);

void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);

void glFramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);

void glFramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);

void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);

void glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint * params);

void glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

void glGenerateMipmap(GLenum target);

