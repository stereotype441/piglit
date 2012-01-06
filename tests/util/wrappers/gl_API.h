void glNewList(GLuint list, GLenum mode);

void glEndList();

void glCallList(GLuint list);

void glCallLists(GLsizei n, GLenum type, const GLvoid * lists);

void glDeleteLists(GLuint list, GLsizei range);

GLuint glGenLists(GLsizei range);

void glListBase(GLuint base);

void glBegin(GLenum mode);

void glBitmap(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte * bitmap);

void glColor3b(GLbyte red, GLbyte green, GLbyte blue);

void glColor3bv(const GLbyte * v);

void glColor3d(GLdouble red, GLdouble green, GLdouble blue);

void glColor3dv(const GLdouble * v);

void glColor3f(GLfloat red, GLfloat green, GLfloat blue);

void glColor3fv(const GLfloat * v);

void glColor3i(GLint red, GLint green, GLint blue);

void glColor3iv(const GLint * v);

void glColor3s(GLshort red, GLshort green, GLshort blue);

void glColor3sv(const GLshort * v);

void glColor3ub(GLubyte red, GLubyte green, GLubyte blue);

void glColor3ubv(const GLubyte * v);

void glColor3ui(GLuint red, GLuint green, GLuint blue);

void glColor3uiv(const GLuint * v);

void glColor3us(GLushort red, GLushort green, GLushort blue);

void glColor3usv(const GLushort * v);

void glColor4b(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);

void glColor4bv(const GLbyte * v);

void glColor4d(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);

void glColor4dv(const GLdouble * v);

void glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

void glColor4fv(const GLfloat * v);

void glColor4i(GLint red, GLint green, GLint blue, GLint alpha);

void glColor4iv(const GLint * v);

void glColor4s(GLshort red, GLshort green, GLshort blue, GLshort alpha);

void glColor4sv(const GLshort * v);

void glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);

void glColor4ubv(const GLubyte * v);

void glColor4ui(GLuint red, GLuint green, GLuint blue, GLuint alpha);

void glColor4uiv(const GLuint * v);

void glColor4us(GLushort red, GLushort green, GLushort blue, GLushort alpha);

void glColor4usv(const GLushort * v);

void glEdgeFlag(GLboolean flag);

void glEdgeFlagv(const GLboolean * flag);

void glEnd();

void glIndexd(GLdouble c);

void glIndexdv(const GLdouble * c);

void glIndexf(GLfloat c);

void glIndexfv(const GLfloat * c);

void glIndexi(GLint c);

void glIndexiv(const GLint * c);

void glIndexs(GLshort c);

void glIndexsv(const GLshort * c);

void glNormal3b(GLbyte nx, GLbyte ny, GLbyte nz);

void glNormal3bv(const GLbyte * v);

void glNormal3d(GLdouble nx, GLdouble ny, GLdouble nz);

void glNormal3dv(const GLdouble * v);

void glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz);

void glNormal3fv(const GLfloat * v);

void glNormal3i(GLint nx, GLint ny, GLint nz);

void glNormal3iv(const GLint * v);

void glNormal3s(GLshort nx, GLshort ny, GLshort nz);

void glNormal3sv(const GLshort * v);

void glRasterPos2d(GLdouble x, GLdouble y);

void glRasterPos2dv(const GLdouble * v);

void glRasterPos2f(GLfloat x, GLfloat y);

void glRasterPos2fv(const GLfloat * v);

void glRasterPos2i(GLint x, GLint y);

void glRasterPos2iv(const GLint * v);

void glRasterPos2s(GLshort x, GLshort y);

void glRasterPos2sv(const GLshort * v);

void glRasterPos3d(GLdouble x, GLdouble y, GLdouble z);

void glRasterPos3dv(const GLdouble * v);

void glRasterPos3f(GLfloat x, GLfloat y, GLfloat z);

void glRasterPos3fv(const GLfloat * v);

void glRasterPos3i(GLint x, GLint y, GLint z);

void glRasterPos3iv(const GLint * v);

void glRasterPos3s(GLshort x, GLshort y, GLshort z);

void glRasterPos3sv(const GLshort * v);

void glRasterPos4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w);

void glRasterPos4dv(const GLdouble * v);

void glRasterPos4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);

void glRasterPos4fv(const GLfloat * v);

void glRasterPos4i(GLint x, GLint y, GLint z, GLint w);

void glRasterPos4iv(const GLint * v);

void glRasterPos4s(GLshort x, GLshort y, GLshort z, GLshort w);

void glRasterPos4sv(const GLshort * v);

void glRectd(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);

void glRectdv(const GLdouble * v1, const GLdouble * v2);

void glRectf(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);

void glRectfv(const GLfloat * v1, const GLfloat * v2);

void glRecti(GLint x1, GLint y1, GLint x2, GLint y2);

void glRectiv(const GLint * v1, const GLint * v2);

void glRects(GLshort x1, GLshort y1, GLshort x2, GLshort y2);

void glRectsv(const GLshort * v1, const GLshort * v2);

void glTexCoord1d(GLdouble s);

void glTexCoord1dv(const GLdouble * v);

void glTexCoord1f(GLfloat s);

void glTexCoord1fv(const GLfloat * v);

void glTexCoord1i(GLint s);

void glTexCoord1iv(const GLint * v);

void glTexCoord1s(GLshort s);

void glTexCoord1sv(const GLshort * v);

void glTexCoord2d(GLdouble s, GLdouble t);

void glTexCoord2dv(const GLdouble * v);

void glTexCoord2f(GLfloat s, GLfloat t);

void glTexCoord2fv(const GLfloat * v);

void glTexCoord2i(GLint s, GLint t);

void glTexCoord2iv(const GLint * v);

void glTexCoord2s(GLshort s, GLshort t);

void glTexCoord2sv(const GLshort * v);

void glTexCoord3d(GLdouble s, GLdouble t, GLdouble r);

void glTexCoord3dv(const GLdouble * v);

void glTexCoord3f(GLfloat s, GLfloat t, GLfloat r);

void glTexCoord3fv(const GLfloat * v);

void glTexCoord3i(GLint s, GLint t, GLint r);

void glTexCoord3iv(const GLint * v);

void glTexCoord3s(GLshort s, GLshort t, GLshort r);

void glTexCoord3sv(const GLshort * v);

void glTexCoord4d(GLdouble s, GLdouble t, GLdouble r, GLdouble q);

void glTexCoord4dv(const GLdouble * v);

void glTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q);

void glTexCoord4fv(const GLfloat * v);

void glTexCoord4i(GLint s, GLint t, GLint r, GLint q);

void glTexCoord4iv(const GLint * v);

void glTexCoord4s(GLshort s, GLshort t, GLshort r, GLshort q);

void glTexCoord4sv(const GLshort * v);

void glVertex2d(GLdouble x, GLdouble y);

void glVertex2dv(const GLdouble * v);

void glVertex2f(GLfloat x, GLfloat y);

void glVertex2fv(const GLfloat * v);

void glVertex2i(GLint x, GLint y);

void glVertex2iv(const GLint * v);

void glVertex2s(GLshort x, GLshort y);

void glVertex2sv(const GLshort * v);

void glVertex3d(GLdouble x, GLdouble y, GLdouble z);

void glVertex3dv(const GLdouble * v);

void glVertex3f(GLfloat x, GLfloat y, GLfloat z);

void glVertex3fv(const GLfloat * v);

void glVertex3i(GLint x, GLint y, GLint z);

void glVertex3iv(const GLint * v);

void glVertex3s(GLshort x, GLshort y, GLshort z);

void glVertex3sv(const GLshort * v);

void glVertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w);

void glVertex4dv(const GLdouble * v);

void glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);

void glVertex4fv(const GLfloat * v);

void glVertex4i(GLint x, GLint y, GLint z, GLint w);

void glVertex4iv(const GLint * v);

void glVertex4s(GLshort x, GLshort y, GLshort z, GLshort w);

void glVertex4sv(const GLshort * v);

void glClipPlane(GLenum plane, const GLdouble * equation);

void glColorMaterial(GLenum face, GLenum mode);

void glCullFace(GLenum mode);

void glFogf(GLenum pname, GLfloat param);

void glFogfv(GLenum pname, const GLfloat * params);

void glFogi(GLenum pname, GLint param);

void glFogiv(GLenum pname, const GLint * params);

void glFrontFace(GLenum mode);

void glHint(GLenum target, GLenum mode);

void glLightf(GLenum light, GLenum pname, GLfloat param);

void glLightfv(GLenum light, GLenum pname, const GLfloat * params);

void glLighti(GLenum light, GLenum pname, GLint param);

void glLightiv(GLenum light, GLenum pname, const GLint * params);

void glLightModelf(GLenum pname, GLfloat param);

void glLightModelfv(GLenum pname, const GLfloat * params);

void glLightModeli(GLenum pname, GLint param);

void glLightModeliv(GLenum pname, const GLint * params);

void glLineStipple(GLint factor, GLushort pattern);

void glLineWidth(GLfloat width);

void glMaterialf(GLenum face, GLenum pname, GLfloat param);

void glMaterialfv(GLenum face, GLenum pname, const GLfloat * params);

void glMateriali(GLenum face, GLenum pname, GLint param);

void glMaterialiv(GLenum face, GLenum pname, const GLint * params);

void glPointSize(GLfloat size);

void glPolygonMode(GLenum face, GLenum mode);

void glPolygonStipple(const GLubyte * mask);

void glScissor(GLint x, GLint y, GLsizei width, GLsizei height);

void glShadeModel(GLenum mode);

void glTexParameterf(GLenum target, GLenum pname, GLfloat param);

void glTexParameterfv(GLenum target, GLenum pname, const GLfloat * params);

void glTexParameteri(GLenum target, GLenum pname, GLint param);

void glTexParameteriv(GLenum target, GLenum pname, const GLint * params);

void glTexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid * pixels);

void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels);

void glTexEnvf(GLenum target, GLenum pname, GLfloat param);

void glTexEnvfv(GLenum target, GLenum pname, const GLfloat * params);

void glTexEnvi(GLenum target, GLenum pname, GLint param);

void glTexEnviv(GLenum target, GLenum pname, const GLint * params);

void glTexGend(GLenum coord, GLenum pname, GLdouble param);

void glTexGendv(GLenum coord, GLenum pname, const GLdouble * params);

void glTexGenf(GLenum coord, GLenum pname, GLfloat param);

void glTexGenfv(GLenum coord, GLenum pname, const GLfloat * params);

void glTexGeni(GLenum coord, GLenum pname, GLint param);

void glTexGeniv(GLenum coord, GLenum pname, const GLint * params);

void glFeedbackBuffer(GLsizei size, GLenum type, GLfloat * buffer);

void glSelectBuffer(GLsizei size, GLuint * buffer);

GLint glRenderMode(GLenum mode);

void glInitNames();

void glLoadName(GLuint name);

void glPassThrough(GLfloat token);

void glPopName();

void glPushName(GLuint name);

void glDrawBuffer(GLenum mode);

void glClear(GLbitfield mask);

void glClearAccum(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

void glClearIndex(GLfloat c);

void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

void glClearStencil(GLint s);

void glClearDepth(GLclampd depth);

void glStencilMask(GLuint mask);

void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);

void glDepthMask(GLboolean flag);

void glIndexMask(GLuint mask);

void glAccum(GLenum op, GLfloat value);

void glDisable(GLenum cap);

void glEnable(GLenum cap);

void glFinish();

void glFlush();

void glPopAttrib();

void glPushAttrib(GLbitfield mask);

void glMap1d(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble * points);

void glMap1f(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat * points);

void glMap2d(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble * points);

void glMap2f(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat * points);

void glMapGrid1d(GLint un, GLdouble u1, GLdouble u2);

void glMapGrid1f(GLint un, GLfloat u1, GLfloat u2);

void glMapGrid2d(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);

void glMapGrid2f(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);

void glEvalCoord1d(GLdouble u);

void glEvalCoord1dv(const GLdouble * u);

void glEvalCoord1f(GLfloat u);

void glEvalCoord1fv(const GLfloat * u);

void glEvalCoord2d(GLdouble u, GLdouble v);

void glEvalCoord2dv(const GLdouble * u);

void glEvalCoord2f(GLfloat u, GLfloat v);

void glEvalCoord2fv(const GLfloat * u);

void glEvalMesh1(GLenum mode, GLint i1, GLint i2);

void glEvalPoint1(GLint i);

void glEvalMesh2(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);

void glEvalPoint2(GLint i, GLint j);

void glAlphaFunc(GLenum func, GLclampf ref);

void glBlendFunc(GLenum sfactor, GLenum dfactor);

void glLogicOp(GLenum opcode);

void glStencilFunc(GLenum func, GLint ref, GLuint mask);

void glStencilOp(GLenum fail, GLenum zfail, GLenum zpass);

void glDepthFunc(GLenum func);

void glPixelZoom(GLfloat xfactor, GLfloat yfactor);

void glPixelTransferf(GLenum pname, GLfloat param);

void glPixelTransferi(GLenum pname, GLint param);

void glPixelStoref(GLenum pname, GLfloat param);

void glPixelStorei(GLenum pname, GLint param);

void glPixelMapfv(GLenum map, GLsizei mapsize, const GLfloat * values);

void glPixelMapuiv(GLenum map, GLsizei mapsize, const GLuint * values);

void glPixelMapusv(GLenum map, GLsizei mapsize, const GLushort * values);

void glReadBuffer(GLenum mode);

void glCopyPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);

void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels);

void glDrawPixels(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels);

void glGetBooleanv(GLenum pname, GLboolean * params);

void glGetClipPlane(GLenum plane, GLdouble * equation);

void glGetDoublev(GLenum pname, GLdouble * params);

GLenum glGetError();

void glGetFloatv(GLenum pname, GLfloat * params);

void glGetIntegerv(GLenum pname, GLint * params);

void glGetLightfv(GLenum light, GLenum pname, GLfloat * params);

void glGetLightiv(GLenum light, GLenum pname, GLint * params);

void glGetMapdv(GLenum target, GLenum query, GLdouble * v);

void glGetMapfv(GLenum target, GLenum query, GLfloat * v);

void glGetMapiv(GLenum target, GLenum query, GLint * v);

void glGetMaterialfv(GLenum face, GLenum pname, GLfloat * params);

void glGetMaterialiv(GLenum face, GLenum pname, GLint * params);

void glGetPixelMapfv(GLenum map, GLfloat * values);

void glGetPixelMapuiv(GLenum map, GLuint * values);

void glGetPixelMapusv(GLenum map, GLushort * values);

void glGetPolygonStipple(GLubyte * mask);

const GLubyte * glGetString(GLenum name);

void glGetTexEnvfv(GLenum target, GLenum pname, GLfloat * params);

void glGetTexEnviv(GLenum target, GLenum pname, GLint * params);

void glGetTexGendv(GLenum coord, GLenum pname, GLdouble * params);

void glGetTexGenfv(GLenum coord, GLenum pname, GLfloat * params);

void glGetTexGeniv(GLenum coord, GLenum pname, GLint * params);

void glGetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid * pixels);

void glGetTexParameterfv(GLenum target, GLenum pname, GLfloat * params);

void glGetTexParameteriv(GLenum target, GLenum pname, GLint * params);

void glGetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat * params);

void glGetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint * params);

GLboolean glIsEnabled(GLenum cap);

GLboolean glIsList(GLuint list);

void glDepthRange(GLclampd zNear, GLclampd zFar);

void glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);

void glLoadIdentity();

void glLoadMatrixf(const GLfloat * m);

void glLoadMatrixd(const GLdouble * m);

void glMatrixMode(GLenum mode);

void glMultMatrixf(const GLfloat * m);

void glMultMatrixd(const GLdouble * m);

void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);

void glPopMatrix();

void glPushMatrix();

void glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);

void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

void glScaled(GLdouble x, GLdouble y, GLdouble z);

void glScalef(GLfloat x, GLfloat y, GLfloat z);

void glTranslated(GLdouble x, GLdouble y, GLdouble z);

void glTranslatef(GLfloat x, GLfloat y, GLfloat z);

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

void glArrayElement(GLint i);

void glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);

void glDisableClientState(GLenum array);

void glDrawArrays(GLenum mode, GLint first, GLsizei count);

void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices);

void glEdgeFlagPointer(GLsizei stride, const GLvoid * pointer);

void glEnableClientState(GLenum array);

void glGetPointerv(GLenum pname, GLvoid ** params);

void glIndexPointer(GLenum type, GLsizei stride, const GLvoid * pointer);

void glInterleavedArrays(GLenum format, GLsizei stride, const GLvoid * pointer);

void glNormalPointer(GLenum type, GLsizei stride, const GLvoid * pointer);

void glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);

void glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);

void glPolygonOffset(GLfloat factor, GLfloat units);

void glCopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);

void glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);

void glCopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);

void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);

void glTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid * pixels);

void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels);

GLboolean glAreTexturesResident(GLsizei n, const GLuint * textures, GLboolean * residences);

void glBindTexture(GLenum target, GLuint texture);

void glDeleteTextures(GLsizei n, const GLuint * textures);

void glGenTextures(GLsizei n, GLuint * textures);

GLboolean glIsTexture(GLuint texture);

void glPrioritizeTextures(GLsizei n, const GLuint * textures, const GLclampf * priorities);

void glIndexub(GLubyte c);

void glIndexubv(const GLubyte * c);

void glPopClientAttrib();

void glPushClientAttrib(GLbitfield mask);

void glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

void glBlendEquation(GLenum mode);

void glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid * indices);

void glColorTable(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid * table);

void glColorTableParameterfv(GLenum target, GLenum pname, const GLfloat * params);

void glColorTableParameteriv(GLenum target, GLenum pname, const GLint * params);

void glCopyColorTable(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);

void glGetColorTable(GLenum target, GLenum format, GLenum type, GLvoid * table);

void glGetColorTableParameterfv(GLenum target, GLenum pname, GLfloat * params);

void glGetColorTableParameteriv(GLenum target, GLenum pname, GLint * params);

void glColorSubTable(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid * data);

void glCopyColorSubTable(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);

void glConvolutionFilter1D(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid * image);

void glConvolutionFilter2D(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * image);

void glConvolutionParameterf(GLenum target, GLenum pname, GLfloat params);

void glConvolutionParameterfv(GLenum target, GLenum pname, const GLfloat * params);

void glConvolutionParameteri(GLenum target, GLenum pname, GLint params);

void glConvolutionParameteriv(GLenum target, GLenum pname, const GLint * params);

void glCopyConvolutionFilter1D(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);

void glCopyConvolutionFilter2D(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);

void glGetConvolutionFilter(GLenum target, GLenum format, GLenum type, GLvoid * image);

void glGetConvolutionParameterfv(GLenum target, GLenum pname, GLfloat * params);

void glGetConvolutionParameteriv(GLenum target, GLenum pname, GLint * params);

void glGetSeparableFilter(GLenum target, GLenum format, GLenum type, GLvoid * row, GLvoid * column, GLvoid * span);

void glSeparableFilter2D(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * row, const GLvoid * column);

void glGetHistogram(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid * values);

void glGetHistogramParameterfv(GLenum target, GLenum pname, GLfloat * params);

void glGetHistogramParameteriv(GLenum target, GLenum pname, GLint * params);

void glGetMinmax(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid * values);

void glGetMinmaxParameterfv(GLenum target, GLenum pname, GLfloat * params);

void glGetMinmaxParameteriv(GLenum target, GLenum pname, GLint * params);

void glHistogram(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);

void glMinmax(GLenum target, GLenum internalformat, GLboolean sink);

void glResetHistogram(GLenum target);

void glResetMinmax(GLenum target);

void glTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * pixels);

void glTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * pixels);

void glCopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

void glActiveTexture(GLenum texture);

void glClientActiveTexture(GLenum texture);

void glMultiTexCoord1d(GLenum target, GLdouble s);

void glMultiTexCoord1dv(GLenum target, const GLdouble * v);

void glMultiTexCoord1f(GLenum target, GLfloat s);

void glMultiTexCoord1fv(GLenum target, const GLfloat * v);

void glMultiTexCoord1i(GLenum target, GLint s);

void glMultiTexCoord1iv(GLenum target, const GLint * v);

void glMultiTexCoord1s(GLenum target, GLshort s);

void glMultiTexCoord1sv(GLenum target, const GLshort * v);

void glMultiTexCoord2d(GLenum target, GLdouble s, GLdouble t);

void glMultiTexCoord2dv(GLenum target, const GLdouble * v);

void glMultiTexCoord2f(GLenum target, GLfloat s, GLfloat t);

void glMultiTexCoord2fv(GLenum target, const GLfloat * v);

void glMultiTexCoord2i(GLenum target, GLint s, GLint t);

void glMultiTexCoord2iv(GLenum target, const GLint * v);

void glMultiTexCoord2s(GLenum target, GLshort s, GLshort t);

void glMultiTexCoord2sv(GLenum target, const GLshort * v);

void glMultiTexCoord3d(GLenum target, GLdouble s, GLdouble t, GLdouble r);

void glMultiTexCoord3dv(GLenum target, const GLdouble * v);

void glMultiTexCoord3f(GLenum target, GLfloat s, GLfloat t, GLfloat r);

void glMultiTexCoord3fv(GLenum target, const GLfloat * v);

void glMultiTexCoord3i(GLenum target, GLint s, GLint t, GLint r);

void glMultiTexCoord3iv(GLenum target, const GLint * v);

void glMultiTexCoord3s(GLenum target, GLshort s, GLshort t, GLshort r);

void glMultiTexCoord3sv(GLenum target, const GLshort * v);

void glMultiTexCoord4d(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);

void glMultiTexCoord4dv(GLenum target, const GLdouble * v);

void glMultiTexCoord4f(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);

void glMultiTexCoord4fv(GLenum target, const GLfloat * v);

void glMultiTexCoord4i(GLenum target, GLint s, GLint t, GLint r, GLint q);

void glMultiTexCoord4iv(GLenum target, const GLint * v);

void glMultiTexCoord4s(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);

void glMultiTexCoord4sv(GLenum target, const GLshort * v);

void glLoadTransposeMatrixf(const GLfloat * m);

void glLoadTransposeMatrixd(const GLdouble * m);

void glMultTransposeMatrixf(const GLfloat * m);

void glMultTransposeMatrixd(const GLdouble * m);

void glSampleCoverage(GLclampf value, GLboolean invert);

void glCompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid * data);

void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * data);

void glCompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid * data);

void glCompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data);

void glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid * data);

void glCompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid * data);

void glGetCompressedTexImage(GLenum target, GLint level, GLvoid * img);

void glBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

void glFogCoordf(GLfloat coord);

void glFogCoordfv(const GLfloat * coord);

void glFogCoordd(GLdouble coord);

void glFogCoorddv(const GLdouble * coord);

void glFogCoordPointer(GLenum type, GLsizei stride, const GLvoid * pointer);

void glMultiDrawArrays(GLenum mode, const GLint * first, const GLsizei * count, GLsizei primcount);

void glMultiDrawElements(GLenum mode, const GLsizei * count, GLenum type, const GLvoid ** indices, GLsizei primcount);

void glPointParameterf(GLenum pname, GLfloat param);

void glPointParameterfv(GLenum pname, const GLfloat * params);

void glPointParameteri(GLenum pname, GLint param);

void glPointParameteriv(GLenum pname, const GLint * params);

void glSecondaryColor3b(GLbyte red, GLbyte green, GLbyte blue);

void glSecondaryColor3bv(const GLbyte * v);

void glSecondaryColor3d(GLdouble red, GLdouble green, GLdouble blue);

void glSecondaryColor3dv(const GLdouble * v);

void glSecondaryColor3f(GLfloat red, GLfloat green, GLfloat blue);

void glSecondaryColor3fv(const GLfloat * v);

void glSecondaryColor3i(GLint red, GLint green, GLint blue);

void glSecondaryColor3iv(const GLint * v);

void glSecondaryColor3s(GLshort red, GLshort green, GLshort blue);

void glSecondaryColor3sv(const GLshort * v);

void glSecondaryColor3ub(GLubyte red, GLubyte green, GLubyte blue);

void glSecondaryColor3ubv(const GLubyte * v);

void glSecondaryColor3ui(GLuint red, GLuint green, GLuint blue);

void glSecondaryColor3uiv(const GLuint * v);

void glSecondaryColor3us(GLushort red, GLushort green, GLushort blue);

void glSecondaryColor3usv(const GLushort * v);

void glSecondaryColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);

void glWindowPos2d(GLdouble x, GLdouble y);

void glWindowPos2dv(const GLdouble * v);

void glWindowPos2f(GLfloat x, GLfloat y);

void glWindowPos2fv(const GLfloat * v);

void glWindowPos2i(GLint x, GLint y);

void glWindowPos2iv(const GLint * v);

void glWindowPos2s(GLshort x, GLshort y);

void glWindowPos2sv(const GLshort * v);

void glWindowPos3d(GLdouble x, GLdouble y, GLdouble z);

void glWindowPos3dv(const GLdouble * v);

void glWindowPos3f(GLfloat x, GLfloat y, GLfloat z);

void glWindowPos3fv(const GLfloat * v);

void glWindowPos3i(GLint x, GLint y, GLint z);

void glWindowPos3iv(const GLint * v);

void glWindowPos3s(GLshort x, GLshort y, GLshort z);

void glWindowPos3sv(const GLshort * v);

void glBindBuffer(GLenum target, GLuint buffer);

void glBufferData(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);

void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data);

void glDeleteBuffers(GLsizei n, const GLuint * buffer);

void glGenBuffers(GLsizei n, GLuint * buffer);

void glGetBufferParameteriv(GLenum target, GLenum pname, GLint * params);

void glGetBufferPointerv(GLenum target, GLenum pname, GLvoid ** params);

void glGetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid * data);

GLboolean glIsBuffer(GLuint buffer);

GLvoid * glMapBuffer(GLenum target, GLenum access);

GLboolean glUnmapBuffer(GLenum target);

void glGenQueries(GLsizei n, GLuint * ids);

void glDeleteQueries(GLsizei n, const GLuint * ids);

GLboolean glIsQuery(GLuint id);

void glBeginQuery(GLenum target, GLuint id);

void glEndQuery(GLenum target);

void glGetQueryiv(GLenum target, GLenum pname, GLint * params);

void glGetQueryObjectiv(GLuint id, GLenum pname, GLint * params);

void glGetQueryObjectuiv(GLuint id, GLenum pname, GLuint * params);

void glBlendEquationSeparate(GLenum modeRGB, GLenum modeA);

void glDrawBuffers(GLsizei n, const GLenum * bufs);

void glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);

void glStencilOpSeparate(GLenum face, GLenum sfail, GLenum zfail, GLenum zpass);

void glStencilMaskSeparate(GLenum face, GLuint mask);

void glAttachShader(GLuint program, GLuint shader);

void glBindAttribLocation(GLuint program, GLuint index, const GLchar * name);

void glCompileShader(GLuint shader);

GLuint glCreateProgram();

GLuint glCreateShader(GLenum type);

void glDeleteProgram(GLuint program);

void glDeleteShader(GLuint program);

void glDetachShader(GLuint program, GLuint shader);

void glDisableVertexAttribArray(GLuint index);

void glEnableVertexAttribArray(GLuint index);

void glGetActiveAttrib(GLuint program, GLuint index, GLsizei  bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name);

void glGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name);

void glGetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei * count, GLuint * obj);

GLint glGetAttribLocation(GLuint program, const GLchar * name);

void glGetProgramiv(GLuint program, GLenum pname, GLint * params);

void glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog);

void glGetShaderiv(GLuint shader, GLenum pname, GLint * params);

void glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog);

void glGetShaderSource(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * source);

GLint glGetUniformLocation(GLuint program, const GLchar * name);

void glGetUniformfv(GLuint program, GLint location, GLfloat * params);

void glGetUniformiv(GLuint program, GLint location, GLint * params);

void glGetVertexAttribdv(GLuint index, GLenum pname, GLdouble * params);

void glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat * params);

void glGetVertexAttribiv(GLuint index, GLenum pname, GLint * params);

void glGetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid ** pointer);

GLboolean glIsProgram(GLuint program);

GLboolean glIsShader(GLuint shader);

void glLinkProgram(GLuint program);

void glShaderSource(GLuint shader, GLsizei count, const GLchar ** string, const GLint * length);

void glUseProgram(GLuint program);

void glUniform1f(GLint location, GLfloat v0);

void glUniform2f(GLint location, GLfloat v0, GLfloat v1);

void glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);

void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

void glUniform1i(GLint location, GLint v0);

void glUniform2i(GLint location, GLint v0, GLint v1);

void glUniform3i(GLint location, GLint v0, GLint v1, GLint v2);

void glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);

void glUniform1fv(GLint location, GLsizei count, const GLfloat * value);

void glUniform2fv(GLint location, GLsizei count, const GLfloat * value);

void glUniform3fv(GLint location, GLsizei count, const GLfloat * value);

void glUniform4fv(GLint location, GLsizei count, const GLfloat * value);

void glUniform1iv(GLint location, GLsizei count, const GLint * value);

void glUniform2iv(GLint location, GLsizei count, const GLint * value);

void glUniform3iv(GLint location, GLsizei count, const GLint * value);

void glUniform4iv(GLint location, GLsizei count, const GLint * value);

void glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);

void glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);

void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);

void glValidateProgram(GLuint program);

void glVertexAttrib1d(GLuint index, GLdouble x);

void glVertexAttrib1dv(GLuint index, const GLdouble * v);

void glVertexAttrib1f(GLuint index, GLfloat x);

void glVertexAttrib1fv(GLuint index, const GLfloat * v);

void glVertexAttrib1s(GLuint index, GLshort x);

void glVertexAttrib1sv(GLuint index, const GLshort * v);

void glVertexAttrib2d(GLuint index, GLdouble x, GLdouble y);

void glVertexAttrib2dv(GLuint index, const GLdouble * v);

void glVertexAttrib2f(GLuint index, GLfloat x, GLfloat y);

void glVertexAttrib2fv(GLuint index, const GLfloat * v);

void glVertexAttrib2s(GLuint index, GLshort x, GLshort y);

void glVertexAttrib2sv(GLuint index, const GLshort * v);

void glVertexAttrib3d(GLuint index, GLdouble x, GLdouble y, GLdouble z);

void glVertexAttrib3dv(GLuint index, const GLdouble * v);

void glVertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z);

void glVertexAttrib3fv(GLuint index, const GLfloat * v);

void glVertexAttrib3s(GLuint index, GLshort x, GLshort y, GLshort z);

void glVertexAttrib3sv(GLuint index, const GLshort * v);

void glVertexAttrib4Nbv(GLuint index, const GLbyte * v);

void glVertexAttrib4Niv(GLuint index, const GLint * v);

void glVertexAttrib4Nsv(GLuint index, const GLshort * v);

void glVertexAttrib4Nub(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);

void glVertexAttrib4Nubv(GLuint index, const GLubyte * v);

void glVertexAttrib4Nuiv(GLuint index, const GLuint * v);

void glVertexAttrib4Nusv(GLuint index, const GLushort * v);

void glVertexAttrib4bv(GLuint index, const GLbyte * v);

void glVertexAttrib4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);

void glVertexAttrib4dv(GLuint index, const GLdouble * v);

void glVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

void glVertexAttrib4fv(GLuint index, const GLfloat * v);

void glVertexAttrib4iv(GLuint index, const GLint * v);

void glVertexAttrib4s(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);

void glVertexAttrib4sv(GLuint index, const GLshort * v);

void glVertexAttrib4ubv(GLuint index, const GLubyte * v);

void glVertexAttrib4uiv(GLuint index, const GLuint * v);

void glVertexAttrib4usv(GLuint index, const GLushort * v);

void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);

void glUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);

void glUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);

void glUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);

void glUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);

void glUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);

void glUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);

void glActiveTextureARB(GLenum texture);

void glClientActiveTextureARB(GLenum texture);

void glMultiTexCoord1dARB(GLenum target, GLdouble s);

void glMultiTexCoord1dvARB(GLenum target, const GLdouble * v);

void glMultiTexCoord1fARB(GLenum target, GLfloat s);

void glMultiTexCoord1fvARB(GLenum target, const GLfloat * v);

void glMultiTexCoord1iARB(GLenum target, GLint s);

void glMultiTexCoord1ivARB(GLenum target, const GLint * v);

void glMultiTexCoord1sARB(GLenum target, GLshort s);

void glMultiTexCoord1svARB(GLenum target, const GLshort * v);

void glMultiTexCoord2dARB(GLenum target, GLdouble s, GLdouble t);

void glMultiTexCoord2dvARB(GLenum target, const GLdouble * v);

void glMultiTexCoord2fARB(GLenum target, GLfloat s, GLfloat t);

void glMultiTexCoord2fvARB(GLenum target, const GLfloat * v);

void glMultiTexCoord2iARB(GLenum target, GLint s, GLint t);

void glMultiTexCoord2ivARB(GLenum target, const GLint * v);

void glMultiTexCoord2sARB(GLenum target, GLshort s, GLshort t);

void glMultiTexCoord2svARB(GLenum target, const GLshort * v);

void glMultiTexCoord3dARB(GLenum target, GLdouble s, GLdouble t, GLdouble r);

void glMultiTexCoord3dvARB(GLenum target, const GLdouble * v);

void glMultiTexCoord3fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r);

void glMultiTexCoord3fvARB(GLenum target, const GLfloat * v);

void glMultiTexCoord3iARB(GLenum target, GLint s, GLint t, GLint r);

void glMultiTexCoord3ivARB(GLenum target, const GLint * v);

void glMultiTexCoord3sARB(GLenum target, GLshort s, GLshort t, GLshort r);

void glMultiTexCoord3svARB(GLenum target, const GLshort * v);

void glMultiTexCoord4dARB(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);

void glMultiTexCoord4dvARB(GLenum target, const GLdouble * v);

void glMultiTexCoord4fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);

void glMultiTexCoord4fvARB(GLenum target, const GLfloat * v);

void glMultiTexCoord4iARB(GLenum target, GLint s, GLint t, GLint r, GLint q);

void glMultiTexCoord4ivARB(GLenum target, const GLint * v);

void glMultiTexCoord4sARB(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);

void glMultiTexCoord4svARB(GLenum target, const GLshort * v);

void glLoadTransposeMatrixfARB(const GLfloat * m);

void glLoadTransposeMatrixdARB(const GLdouble * m);

void glMultTransposeMatrixfARB(const GLfloat * m);

void glMultTransposeMatrixdARB(const GLdouble * m);

void glSampleCoverageARB(GLclampf value, GLboolean invert);

void glCompressedTexImage3DARB(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid * data);

void glCompressedTexImage2DARB(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * data);

void glCompressedTexImage1DARB(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid * data);

void glCompressedTexSubImage3DARB(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data);

void glCompressedTexSubImage2DARB(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid * data);

void glCompressedTexSubImage1DARB(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid * data);

void glGetCompressedTexImageARB(GLenum target, GLint level, GLvoid * img);

void glPointParameterfARB(GLenum pname, GLfloat param);

void glPointParameterfvARB(GLenum pname, const GLfloat * params);

void glWeightbvARB(GLint size, const GLbyte * weights);

void glWeightsvARB(GLint size, const GLshort * weights);

void glWeightivARB(GLint size, const GLint * weights);

void glWeightfvARB(GLint size, const GLfloat * weights);

void glWeightdvARB(GLint size, const GLdouble * weights);

void glWeightubvARB(GLint size, const GLubyte * weights);

void glWeightusvARB(GLint size, const GLushort * weights);

void glWeightuivARB(GLint size, const GLuint * weights);

void glWeightPointerARB(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);

void glVertexBlendARB(GLint count);

void glCurrentPaletteMatrixARB(GLint index);

void glMatrixIndexubvARB(GLint size, const GLubyte * indices);

void glMatrixIndexusvARB(GLint size, const GLushort * indices);

void glMatrixIndexuivARB(GLint size, const GLuint * indices);

void glMatrixIndexPointerARB(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);

void glWindowPos2dARB(GLdouble x, GLdouble y);

void glWindowPos2fARB(GLfloat x, GLfloat y);

void glWindowPos2iARB(GLint x, GLint y);

void glWindowPos2sARB(GLshort x, GLshort y);

void glWindowPos2dvARB(const GLdouble * v);

void glWindowPos2fvARB(const GLfloat * v);

void glWindowPos2ivARB(const GLint * v);

void glWindowPos2svARB(const GLshort * v);

void glWindowPos3dARB(GLdouble x, GLdouble y, GLdouble z);

void glWindowPos3fARB(GLfloat x, GLfloat y, GLfloat z);

void glWindowPos3iARB(GLint x, GLint y, GLint z);

void glWindowPos3sARB(GLshort x, GLshort y, GLshort z);

void glWindowPos3dvARB(const GLdouble * v);

void glWindowPos3fvARB(const GLfloat * v);

void glWindowPos3ivARB(const GLint * v);

void glWindowPos3svARB(const GLshort * v);

void glGetVertexAttribdvARB(GLuint index, GLenum pname, GLdouble * params);

void glGetVertexAttribfvARB(GLuint index, GLenum pname, GLfloat * params);

void glGetVertexAttribivARB(GLuint index, GLenum pname, GLint * params);

void glVertexAttrib1dARB(GLuint index, GLdouble x);

void glVertexAttrib1dvARB(GLuint index, const GLdouble * v);

void glVertexAttrib1fARB(GLuint index, GLfloat x);

void glVertexAttrib1fvARB(GLuint index, const GLfloat * v);

void glVertexAttrib1sARB(GLuint index, GLshort x);

void glVertexAttrib1svARB(GLuint index, const GLshort * v);

void glVertexAttrib2dARB(GLuint index, GLdouble x, GLdouble y);

void glVertexAttrib2dvARB(GLuint index, const GLdouble * v);

void glVertexAttrib2fARB(GLuint index, GLfloat x, GLfloat y);

void glVertexAttrib2fvARB(GLuint index, const GLfloat * v);

void glVertexAttrib2sARB(GLuint index, GLshort x, GLshort y);

void glVertexAttrib2svARB(GLuint index, const GLshort * v);

void glVertexAttrib3dARB(GLuint index, GLdouble x, GLdouble y, GLdouble z);

void glVertexAttrib3dvARB(GLuint index, const GLdouble * v);

void glVertexAttrib3fARB(GLuint index, GLfloat x, GLfloat y, GLfloat z);

void glVertexAttrib3fvARB(GLuint index, const GLfloat * v);

void glVertexAttrib3sARB(GLuint index, GLshort x, GLshort y, GLshort z);

void glVertexAttrib3svARB(GLuint index, const GLshort * v);

void glVertexAttrib4dARB(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);

void glVertexAttrib4dvARB(GLuint index, const GLdouble * v);

void glVertexAttrib4fARB(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

void glVertexAttrib4fvARB(GLuint index, const GLfloat * v);

void glVertexAttrib4sARB(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);

void glVertexAttrib4svARB(GLuint index, const GLshort * v);

void glVertexAttrib4NubARB(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);

void glVertexAttrib4NubvARB(GLuint index, const GLubyte * v);

void glVertexAttrib4bvARB(GLuint index, const GLbyte * v);

void glVertexAttrib4ivARB(GLuint index, const GLint * v);

void glVertexAttrib4ubvARB(GLuint index, const GLubyte * v);

void glVertexAttrib4usvARB(GLuint index, const GLushort * v);

void glVertexAttrib4uivARB(GLuint index, const GLuint * v);

void glVertexAttrib4NbvARB(GLuint index, const GLbyte * v);

void glVertexAttrib4NsvARB(GLuint index, const GLshort * v);

void glVertexAttrib4NivARB(GLuint index, const GLint * v);

void glVertexAttrib4NusvARB(GLuint index, const GLushort * v);

void glVertexAttrib4NuivARB(GLuint index, const GLuint * v);

void glVertexAttribPointerARB(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);

void glEnableVertexAttribArrayARB(GLuint index);

void glDisableVertexAttribArrayARB(GLuint index);

void glProgramStringARB(GLenum target, GLenum format, GLsizei len, const GLvoid * string);

void glBindProgramARB(GLenum target, GLuint program);

void glDeleteProgramsARB(GLsizei n, const GLuint * programs);

void glGenProgramsARB(GLsizei n, GLuint * programs);

GLboolean glIsProgramARB(GLuint program);

void glProgramEnvParameter4dARB(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);

void glProgramEnvParameter4dvARB(GLenum target, GLuint index, const GLdouble * params);

void glProgramEnvParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

void glProgramEnvParameter4fvARB(GLenum target, GLuint index, const GLfloat * params);

void glProgramLocalParameter4dARB(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);

void glProgramLocalParameter4dvARB(GLenum target, GLuint index, const GLdouble * params);

void glProgramLocalParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

void glProgramLocalParameter4fvARB(GLenum target, GLuint index, const GLfloat * params);

void glGetProgramEnvParameterdvARB(GLenum target, GLuint index, GLdouble * params);

void glGetProgramEnvParameterfvARB(GLenum target, GLuint index, GLfloat * params);

void glGetProgramLocalParameterdvARB(GLenum target, GLuint index, GLdouble * params);

void glGetProgramLocalParameterfvARB(GLenum target, GLuint index, GLfloat * params);

void glGetProgramivARB(GLenum target, GLenum pname, GLint * params);

void glGetProgramStringARB(GLenum target, GLenum pname, GLvoid * string);

void glGetVertexAttribPointervARB(GLuint index, GLenum pname, GLvoid ** pointer);

void glBindBufferARB(GLenum target, GLuint buffer);

void glBufferDataARB(GLenum target, GLsizeiptrARB size, const GLvoid * data, GLenum usage);

void glBufferSubDataARB(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid * data);

void glDeleteBuffersARB(GLsizei n, const GLuint * buffer);

void glGenBuffersARB(GLsizei n, GLuint * buffer);

void glGetBufferParameterivARB(GLenum target, GLenum pname, GLint * params);

void glGetBufferPointervARB(GLenum target, GLenum pname, GLvoid ** params);

void glGetBufferSubDataARB(GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid * data);

GLboolean glIsBufferARB(GLuint buffer);

GLvoid * glMapBufferARB(GLenum target, GLenum access);

GLboolean glUnmapBufferARB(GLenum target);

void glGenQueriesARB(GLsizei n, GLuint * ids);

void glDeleteQueriesARB(GLsizei n, const GLuint * ids);

GLboolean glIsQueryARB(GLuint id);

void glBeginQueryARB(GLenum target, GLuint id);

void glEndQueryARB(GLenum target);

void glGetQueryivARB(GLenum target, GLenum pname, GLint * params);

void glGetQueryObjectivARB(GLuint id, GLenum pname, GLint * params);

void glGetQueryObjectuivARB(GLuint id, GLenum pname, GLuint * params);

void glDeleteObjectARB(GLhandleARB obj);

GLhandleARB glGetHandleARB(GLenum pname);

void glDetachObjectARB(GLhandleARB containerObj, GLhandleARB attachedObj);

GLhandleARB glCreateShaderObjectARB(GLenum shaderType);

void glShaderSourceARB(GLhandleARB shader, GLsizei count, const GLcharARB ** string, const GLint * length);

void glCompileShaderARB(GLhandleARB shader);

GLhandleARB glCreateProgramObjectARB();

void glAttachObjectARB(GLhandleARB containerObj, GLhandleARB obj);

void glLinkProgramARB(GLhandleARB program);

void glUseProgramObjectARB(GLhandleARB program);

void glValidateProgramARB(GLhandleARB program);

void glUniform1fARB(GLint location, GLfloat v0);

void glUniform2fARB(GLint location, GLfloat v0, GLfloat v1);

void glUniform3fARB(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);

void glUniform4fARB(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

void glUniform1iARB(GLint location, GLint v0);

void glUniform2iARB(GLint location, GLint v0, GLint v1);

void glUniform3iARB(GLint location, GLint v0, GLint v1, GLint v2);

void glUniform4iARB(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);

void glUniform1fvARB(GLint location, GLsizei count, const GLfloat * value);

void glUniform2fvARB(GLint location, GLsizei count, const GLfloat * value);

void glUniform3fvARB(GLint location, GLsizei count, const GLfloat * value);

void glUniform4fvARB(GLint location, GLsizei count, const GLfloat * value);

void glUniform1ivARB(GLint location, GLsizei count, const GLint * value);

void glUniform2ivARB(GLint location, GLsizei count, const GLint * value);

void glUniform3ivARB(GLint location, GLsizei count, const GLint * value);

void glUniform4ivARB(GLint location, GLsizei count, const GLint * value);

void glUniformMatrix2fvARB(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);

void glUniformMatrix3fvARB(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);

void glUniformMatrix4fvARB(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);

void glGetObjectParameterfvARB(GLhandleARB obj, GLenum pname, GLfloat * params);

void glGetObjectParameterivARB(GLhandleARB obj, GLenum pname, GLint * params);

void glGetInfoLogARB(GLhandleARB obj, GLsizei maxLength, GLsizei * length, GLcharARB * infoLog);

void glGetAttachedObjectsARB(GLhandleARB containerObj, GLsizei maxLength, GLsizei * length, GLhandleARB * infoLog);

GLint glGetUniformLocationARB(GLhandleARB program, const GLcharARB * name);

void glGetActiveUniformARB(GLhandleARB program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLcharARB * name);

void glGetUniformfvARB(GLhandleARB program, GLint location, GLfloat * params);

void glGetUniformivARB(GLhandleARB program, GLint location, GLint * params);

void glGetShaderSourceARB(GLhandleARB shader, GLsizei bufSize, GLsizei * length, GLcharARB * source);

void glBindAttribLocationARB(GLhandleARB program, GLuint index, const GLcharARB * name);

void glGetActiveAttribARB(GLhandleARB program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLcharARB * name);

GLint glGetAttribLocationARB(GLhandleARB program, const GLcharARB * name);

void glBlendColorEXT(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

void glPolygonOffsetEXT(GLfloat factor, GLfloat bias);

void glTexImage3DEXT(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * pixels);

void glTexSubImage3DEXT(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * pixels);

void glGetTexFilterFuncSGIS(GLenum target, GLenum filter, GLfloat * weights);

void glTexFilterFuncSGIS(GLenum target, GLenum filter, GLsizei n, const GLfloat * weights);

void glTexSubImage1DEXT(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid * pixels);

void glTexSubImage2DEXT(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels);

void glCopyTexImage1DEXT(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);

void glCopyTexImage2DEXT(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);

void glCopyTexSubImage1DEXT(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);

void glCopyTexSubImage2DEXT(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);

void glCopyTexSubImage3DEXT(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

void glGetHistogramEXT(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid * values);

void glGetHistogramParameterfvEXT(GLenum target, GLenum pname, GLfloat * params);

void glGetHistogramParameterivEXT(GLenum target, GLenum pname, GLint * params);

void glGetMinmaxEXT(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid * values);

void glGetMinmaxParameterfvEXT(GLenum target, GLenum pname, GLfloat * params);

void glGetMinmaxParameterivEXT(GLenum target, GLenum pname, GLint * params);

void glHistogramEXT(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);

void glMinmaxEXT(GLenum target, GLenum internalformat, GLboolean sink);

void glResetHistogramEXT(GLenum target);

void glResetMinmaxEXT(GLenum target);

void glConvolutionFilter1DEXT(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid * image);

void glConvolutionFilter2DEXT(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * image);

void glConvolutionParameterfEXT(GLenum target, GLenum pname, GLfloat params);

void glConvolutionParameterfvEXT(GLenum target, GLenum pname, const GLfloat * params);

void glConvolutionParameteriEXT(GLenum target, GLenum pname, GLint params);

void glConvolutionParameterivEXT(GLenum target, GLenum pname, const GLint * params);

void glCopyConvolutionFilter1DEXT(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);

void glCopyConvolutionFilter2DEXT(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);

void glGetConvolutionFilterEXT(GLenum target, GLenum format, GLenum type, GLvoid * image);

void glGetConvolutionParameterfvEXT(GLenum target, GLenum pname, GLfloat * params);

void glGetConvolutionParameterivEXT(GLenum target, GLenum pname, GLint * params);

void glGetSeparableFilterEXT(GLenum target, GLenum format, GLenum type, GLvoid * row, GLvoid * column, GLvoid * span);

void glSeparableFilter2DEXT(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * row, const GLvoid * column);

void glColorTableSGI(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid * table);

void glColorTableParameterfvSGI(GLenum target, GLenum pname, const GLfloat * params);

void glColorTableParameterivSGI(GLenum target, GLenum pname, const GLint * params);

void glCopyColorTableSGI(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);

void glGetColorTableSGI(GLenum target, GLenum format, GLenum type, GLvoid * table);

void glGetColorTableParameterfvSGI(GLenum target, GLenum pname, GLfloat * params);

void glGetColorTableParameterivSGI(GLenum target, GLenum pname, GLint * params);

void glPixelTexGenParameteriSGIS(GLenum pname, GLint param);

void glPixelTexGenParameterivSGIS(GLenum pname, const GLint * params);

void glPixelTexGenParameterfSGIS(GLenum pname, GLfloat param);

void glPixelTexGenParameterfvSGIS(GLenum pname, const GLfloat * params);

void glGetPixelTexGenParameterivSGIS(GLenum pname, GLint * params);

void glGetPixelTexGenParameterfvSGIS(GLenum pname, GLfloat * params);

void glTexImage4DSGIS(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLint border, GLenum format, GLenum type, const GLvoid * pixels);

void glTexSubImage4DSGIS(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint woffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLenum format, GLenum type, const GLvoid * pixels);

GLboolean glAreTexturesResidentEXT(GLsizei n, const GLuint * textures, GLboolean * residences);

void glBindTextureEXT(GLenum target, GLuint texture);

void glDeleteTexturesEXT(GLsizei n, const GLuint * textures);

void glGenTexturesEXT(GLsizei n, GLuint * textures);

GLboolean glIsTextureEXT(GLuint texture);

void glPrioritizeTexturesEXT(GLsizei n, const GLuint * textures, const GLclampf * priorities);

void glDetailTexFuncSGIS(GLenum target, GLsizei n, const GLfloat * points);

void glGetDetailTexFuncSGIS(GLenum target, GLfloat * points);

void glSharpenTexFuncSGIS(GLenum target, GLsizei n, const GLfloat * points);

void glGetSharpenTexFuncSGIS(GLenum target, GLfloat * points);

void glSampleMaskSGIS(GLclampf value, GLboolean invert);

void glSamplePatternSGIS(GLenum pattern);

void glArrayElementEXT(GLint i);

void glColorPointerEXT(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer);

void glDrawArraysEXT(GLenum mode, GLint first, GLsizei count);

void glEdgeFlagPointerEXT(GLsizei stride, GLsizei count, const GLboolean * pointer);

void glGetPointervEXT(GLenum pname, GLvoid ** params);

void glIndexPointerEXT(GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer);

void glNormalPointerEXT(GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer);

void glTexCoordPointerEXT(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer);

void glVertexPointerEXT(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid * pointer);

void glBlendEquationEXT(GLenum mode);

void glSpriteParameterfSGIX(GLenum pname, GLfloat param);

void glSpriteParameterfvSGIX(GLenum pname, const GLfloat * params);

void glSpriteParameteriSGIX(GLenum pname, GLint param);

void glSpriteParameterivSGIX(GLenum pname, const GLint * params);

void glPointParameterfEXT(GLenum pname, GLfloat param);

void glPointParameterfvEXT(GLenum pname, const GLfloat * params);

GLint glGetInstrumentsSGIX();

void glInstrumentsBufferSGIX(GLsizei size, GLint * buffer);

GLint glPollInstrumentsSGIX(GLint * marker_p);

void glReadInstrumentsSGIX(GLint marker);

void glStartInstrumentsSGIX();

void glStopInstrumentsSGIX(GLint marker);

void glFrameZoomSGIX(GLint factor);

void glTagSampleBufferSGIX();

void glReferencePlaneSGIX(const GLdouble * equation);

void glFlushRasterSGIX();

void glFogFuncSGIS(GLsizei n, const GLfloat * points);

void glGetFogFuncSGIS(GLfloat * points);

void glImageTransformParameteriHP(GLenum target, GLenum pname, GLint param);

void glImageTransformParameterfHP(GLenum target, GLenum pname, GLfloat param);

void glImageTransformParameterivHP(GLenum target, GLenum pname, const GLint * params);

void glImageTransformParameterfvHP(GLenum target, GLenum pname, const GLfloat * params);

void glGetImageTransformParameterivHP(GLenum target, GLenum pname, GLint * params);

void glGetImageTransformParameterfvHP(GLenum target, GLenum pname, GLfloat * params);

void glColorSubTableEXT(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid * data);

void glCopyColorSubTableEXT(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);

void glHintPGI(GLenum target, GLint mode);

void glColorTableEXT(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid * table);

void glGetColorTableEXT(GLenum target, GLenum format, GLenum type, GLvoid * table);

void glGetColorTableParameterivEXT(GLenum target, GLenum pname, GLint * params);

void glGetColorTableParameterfvEXT(GLenum target, GLenum pname, GLfloat * params);

void glGetListParameterfvSGIX(GLuint list, GLenum pname, GLfloat * params);

void glGetListParameterivSGIX(GLuint list, GLenum pname, GLint * params);

void glListParameterfSGIX(GLuint list, GLenum pname, GLfloat param);

void glListParameterfvSGIX(GLuint list, GLenum pname, const GLfloat * params);

void glListParameteriSGIX(GLuint list, GLenum pname, GLint param);

void glListParameterivSGIX(GLuint list, GLenum pname, const GLint * params);

void glIndexMaterialEXT(GLenum face, GLenum mode);

void glIndexFuncEXT(GLenum func, GLclampf ref);

void glLockArraysEXT(GLint first, GLsizei count);

void glUnlockArraysEXT();

void glCullParameterdvEXT(GLenum pname, GLdouble * params);

void glCullParameterfvEXT(GLenum pname, GLfloat * params);

void glFragmentColorMaterialSGIX(GLenum face, GLenum mode);

void glFragmentLightfSGIX(GLenum light, GLenum pname, GLfloat param);

void glFragmentLightfvSGIX(GLenum light, GLenum pname, const GLfloat * params);

void glFragmentLightiSGIX(GLenum light, GLenum pname, GLint param);

void glFragmentLightivSGIX(GLenum light, GLenum pname, const GLint * params);

void glFragmentLightModelfSGIX(GLenum pname, GLfloat param);

void glFragmentLightModelfvSGIX(GLenum pname, const GLfloat * params);

void glFragmentLightModeliSGIX(GLenum pname, GLint param);

void glFragmentLightModelivSGIX(GLenum pname, const GLint * params);

void glFragmentMaterialfSGIX(GLenum face, GLenum pname, GLfloat param);

void glFragmentMaterialfvSGIX(GLenum face, GLenum pname, const GLfloat * params);

void glFragmentMaterialiSGIX(GLenum face, GLenum pname, GLint param);

void glFragmentMaterialivSGIX(GLenum face, GLenum pname, const GLint * params);

void glGetFragmentLightfvSGIX(GLenum light, GLenum pname, GLfloat * params);

void glGetFragmentLightivSGIX(GLenum light, GLenum pname, GLint * params);

void glGetFragmentMaterialfvSGIX(GLenum face, GLenum pname, GLfloat * params);

void glGetFragmentMaterialivSGIX(GLenum face, GLenum pname, GLint * params);

void glLightEnviSGIX(GLenum pname, GLint param);

void glDrawRangeElementsEXT(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid * indices);

void glApplyTextureEXT(GLenum mode);

void glTextureLightEXT(GLenum pname);

void glTextureMaterialEXT(GLenum face, GLenum mode);

void glAsyncMarkerSGIX(GLuint marker);

GLint glFinishAsyncSGIX(GLuint * markerp);

GLint glPollAsyncSGIX(GLuint * markerp);

GLuint glGenAsyncMarkersSGIX(GLsizei range);

void glDeleteAsyncMarkersSGIX(GLuint marker, GLsizei range);

GLboolean glIsAsyncMarkerSGIX(GLuint marker);

void glVertexPointervINTEL(GLint size, GLenum type, const GLvoid ** pointer);

void glNormalPointervINTEL(GLenum type, const GLvoid ** pointer);

void glColorPointervINTEL(GLint size, GLenum type, const GLvoid ** pointer);

void glTexCoordPointervINTEL(GLint size, GLenum type, const GLvoid ** pointer);

void glPixelTransformParameteriEXT(GLenum target, GLenum pname, GLint param);

void glPixelTransformParameterfEXT(GLenum target, GLenum pname, GLfloat param);

void glPixelTransformParameterivEXT(GLenum target, GLenum pname, const GLint * params);

void glPixelTransformParameterfvEXT(GLenum target, GLenum pname, const GLfloat * params);

void glSecondaryColor3bEXT(GLbyte red, GLbyte green, GLbyte blue);

void glSecondaryColor3bvEXT(const GLbyte * v);

void glSecondaryColor3dEXT(GLdouble red, GLdouble green, GLdouble blue);

void glSecondaryColor3dvEXT(const GLdouble * v);

void glSecondaryColor3fEXT(GLfloat red, GLfloat green, GLfloat blue);

void glSecondaryColor3fvEXT(const GLfloat * v);

void glSecondaryColor3iEXT(GLint red, GLint green, GLint blue);

void glSecondaryColor3ivEXT(const GLint * v);

void glSecondaryColor3sEXT(GLshort red, GLshort green, GLshort blue);

void glSecondaryColor3svEXT(const GLshort * v);

void glSecondaryColor3ubEXT(GLubyte red, GLubyte green, GLubyte blue);

void glSecondaryColor3ubvEXT(const GLubyte * v);

void glSecondaryColor3uiEXT(GLuint red, GLuint green, GLuint blue);

void glSecondaryColor3uivEXT(const GLuint * v);

void glSecondaryColor3usEXT(GLushort red, GLushort green, GLushort blue);

void glSecondaryColor3usvEXT(const GLushort * v);

void glSecondaryColorPointerEXT(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);

void glTextureNormalEXT(GLenum mode);

void glMultiDrawArraysEXT(GLenum mode, const GLint * first, const GLsizei * count, GLsizei primcount);

void glMultiDrawElementsEXT(GLenum mode, const GLsizei * count, GLenum type, const GLvoid ** indices, GLsizei primcount);

void glFogCoordfEXT(GLfloat coord);

void glFogCoordfvEXT(const GLfloat * coord);

void glFogCoorddEXT(GLdouble coord);

void glFogCoorddvEXT(const GLdouble * coord);

void glFogCoordPointerEXT(GLenum type, GLsizei stride, const GLvoid * pointer);

void glTangent3bEXT(GLbyte tx, GLbyte ty, GLbyte tz);

void glTangent3bvEXT(const GLbyte * v);

void glTangent3dEXT(GLdouble tx, GLdouble ty, GLdouble tz);

void glTangent3dvEXT(const GLdouble * v);

void glTangent3fEXT(GLfloat tx, GLfloat ty, GLfloat tz);

void glTangent3fvEXT(const GLfloat * v);

void glTangent3iEXT(GLint tx, GLint ty, GLint tz);

void glTangent3ivEXT(const GLint * v);

void glTangent3sEXT(GLshort tx, GLshort ty, GLshort tz);

void glTangent3svEXT(const GLshort * v);

void glBinormal3bEXT(GLbyte bx, GLbyte by, GLbyte bz);

void glBinormal3bvEXT(const GLbyte * v);

void glBinormal3dEXT(GLdouble bx, GLdouble by, GLdouble bz);

void glBinormal3dvEXT(const GLdouble * v);

void glBinormal3fEXT(GLfloat bx, GLfloat by, GLfloat bz);

void glBinormal3fvEXT(const GLfloat * v);

void glBinormal3iEXT(GLint bx, GLint by, GLint bz);

void glBinormal3ivEXT(const GLint * v);

void glBinormal3sEXT(GLshort bx, GLshort by, GLshort bz);

void glBinormal3svEXT(const GLshort * v);

void glTangentPointerEXT(GLenum type, GLsizei stride, const GLvoid * pointer);

void glBinormalPointerEXT(GLenum type, GLsizei stride, const GLvoid * pointer);

void glPixelTexGenSGIX(GLenum mode);

void glFinishTextureSUNX();

void glGlobalAlphaFactorbSUN(GLbyte factor);

void glGlobalAlphaFactorsSUN(GLshort factor);

void glGlobalAlphaFactoriSUN(GLint factor);

void glGlobalAlphaFactorfSUN(GLfloat factor);

void glGlobalAlphaFactordSUN(GLdouble factor);

void glGlobalAlphaFactorubSUN(GLubyte factor);

void glGlobalAlphaFactorusSUN(GLushort factor);

void glGlobalAlphaFactoruiSUN(GLuint factor);

void glReplacementCodeuiSUN(GLuint code);

void glReplacementCodeusSUN(GLushort code);

void glReplacementCodeubSUN(GLubyte code);

void glReplacementCodeuivSUN(const GLuint * code);

void glReplacementCodeusvSUN(const GLushort * code);

void glReplacementCodeubvSUN(const GLubyte * code);

void glReplacementCodePointerSUN(GLenum type, GLsizei stride, const GLvoid * pointer);

void glColor4ubVertex2fSUN(GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y);

void glColor4ubVertex2fvSUN(const GLubyte * c, const GLfloat * v);

void glColor4ubVertex3fSUN(GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);

void glColor4ubVertex3fvSUN(const GLubyte * c, const GLfloat * v);

void glColor3fVertex3fSUN(GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);

void glColor3fVertex3fvSUN(const GLfloat * c, const GLfloat * v);

void glNormal3fVertex3fSUN(GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);

void glNormal3fVertex3fvSUN(const GLfloat * n, const GLfloat * v);

void glColor4fNormal3fVertex3fSUN(GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);

void glColor4fNormal3fVertex3fvSUN(const GLfloat * c, const GLfloat * n, const GLfloat * v);

void glTexCoord2fVertex3fSUN(GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);

void glTexCoord2fVertex3fvSUN(const GLfloat * tc, const GLfloat * v);

void glTexCoord4fVertex4fSUN(GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

void glTexCoord4fVertex4fvSUN(const GLfloat * tc, const GLfloat * v);

void glTexCoord2fColor4ubVertex3fSUN(GLfloat s, GLfloat t, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);

void glTexCoord2fColor4ubVertex3fvSUN(const GLfloat * tc, const GLubyte * c, const GLfloat * v);

void glTexCoord2fColor3fVertex3fSUN(GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);

void glTexCoord2fColor3fVertex3fvSUN(const GLfloat * tc, const GLfloat * c, const GLfloat * v);

void glTexCoord2fNormal3fVertex3fSUN(GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);

void glTexCoord2fNormal3fVertex3fvSUN(const GLfloat * tc, const GLfloat * n, const GLfloat * v);

void glTexCoord2fColor4fNormal3fVertex3fSUN(GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);

void glTexCoord2fColor4fNormal3fVertex3fvSUN(const GLfloat * tc, const GLfloat * c, const GLfloat * n, const GLfloat * v);

void glTexCoord4fColor4fNormal3fVertex4fSUN(GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

void glTexCoord4fColor4fNormal3fVertex4fvSUN(const GLfloat * tc, const GLfloat * c, const GLfloat * n, const GLfloat * v);

void glReplacementCodeuiVertex3fSUN(GLuint rc, GLfloat x, GLfloat y, GLfloat z);

void glReplacementCodeuiVertex3fvSUN(const GLuint * rc, const GLfloat * v);

void glReplacementCodeuiColor4ubVertex3fSUN(GLuint rc, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);

void glReplacementCodeuiColor4ubVertex3fvSUN(const GLuint * rc, const GLubyte * c, const GLfloat * v);

void glReplacementCodeuiColor3fVertex3fSUN(GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);

void glReplacementCodeuiColor3fVertex3fvSUN(const GLuint * rc, const GLfloat * c, const GLfloat * v);

void glReplacementCodeuiNormal3fVertex3fSUN(GLuint rc, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);

void glReplacementCodeuiNormal3fVertex3fvSUN(const GLuint * rc, const GLfloat * n, const GLfloat * v);

void glReplacementCodeuiColor4fNormal3fVertex3fSUN(GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);

void glReplacementCodeuiColor4fNormal3fVertex3fvSUN(const GLuint * rc, const GLfloat * c, const GLfloat * n, const GLfloat * v);

void glReplacementCodeuiTexCoord2fVertex3fSUN(GLuint rc, GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);

void glReplacementCodeuiTexCoord2fVertex3fvSUN(const GLuint * rc, const GLfloat * tc, const GLfloat * v);

void glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN(GLuint rc, GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);

void glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN(const GLuint * rc, const GLfloat * tc, const GLfloat * n, const GLfloat * v);

void glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN(GLuint rc, GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);

void glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN(const GLuint * rc, const GLfloat * tc, const GLfloat * c, const GLfloat * n, const GLfloat * v);

void glBlendFuncSeparateEXT(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

void glVertexWeightfEXT(GLfloat weight);

void glVertexWeightfvEXT(const GLfloat * weight);

void glVertexWeightPointerEXT(GLsizei size, GLenum type, GLsizei stride, const GLvoid * pointer);

void glFlushVertexArrayRangeNV();

void glVertexArrayRangeNV(GLsizei length, const GLvoid * pointer);

void glCombinerParameterfvNV(GLenum pname, const GLfloat * params);

void glCombinerParameterfNV(GLenum pname, GLfloat param);

void glCombinerParameterivNV(GLenum pname, const GLint * params);

void glCombinerParameteriNV(GLenum pname, GLint param);

void glCombinerInputNV(GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);

void glCombinerOutputNV(GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum);

void glFinalCombinerInputNV(GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);

void glGetCombinerInputParameterfvNV(GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat * params);

void glGetCombinerInputParameterivNV(GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint * params);

void glGetCombinerOutputParameterfvNV(GLenum stage, GLenum portion, GLenum pname, GLfloat * params);

void glGetCombinerOutputParameterivNV(GLenum stage, GLenum portion, GLenum pname, GLint * params);

void glGetFinalCombinerInputParameterfvNV(GLenum variable, GLenum pname, GLfloat * params);

void glGetFinalCombinerInputParameterivNV(GLenum variable, GLenum pname, GLint * params);

void glResizeBuffersMESA();

void glWindowPos2dMESA(GLdouble x, GLdouble y);

void glWindowPos2dvMESA(const GLdouble * v);

void glWindowPos2fMESA(GLfloat x, GLfloat y);

void glWindowPos2fvMESA(const GLfloat * v);

void glWindowPos2iMESA(GLint x, GLint y);

void glWindowPos2ivMESA(const GLint * v);

void glWindowPos2sMESA(GLshort x, GLshort y);

void glWindowPos2svMESA(const GLshort * v);

void glWindowPos3dMESA(GLdouble x, GLdouble y, GLdouble z);

void glWindowPos3dvMESA(const GLdouble * v);

void glWindowPos3fMESA(GLfloat x, GLfloat y, GLfloat z);

void glWindowPos3fvMESA(const GLfloat * v);

void glWindowPos3iMESA(GLint x, GLint y, GLint z);

void glWindowPos3ivMESA(const GLint * v);

void glWindowPos3sMESA(GLshort x, GLshort y, GLshort z);

void glWindowPos3svMESA(const GLshort * v);

void glWindowPos4dMESA(GLdouble x, GLdouble y, GLdouble z, GLdouble w);

void glWindowPos4dvMESA(const GLdouble * v);

void glWindowPos4fMESA(GLfloat x, GLfloat y, GLfloat z, GLfloat w);

void glWindowPos4fvMESA(const GLfloat * v);

void glWindowPos4iMESA(GLint x, GLint y, GLint z, GLint w);

void glWindowPos4ivMESA(const GLint * v);

void glWindowPos4sMESA(GLshort x, GLshort y, GLshort z, GLshort w);

void glWindowPos4svMESA(const GLshort * v);

void glMultiModeDrawArraysIBM(const GLenum * mode, const GLint * first, const GLsizei * count, GLsizei primcount, GLint modestride);

void glMultiModeDrawElementsIBM(const GLenum * mode, const GLsizei * count, GLenum type, const GLvoid * const * indices, GLsizei primcount, GLint modestride);

void glColorPointerListIBM(GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);

void glSecondaryColorPointerListIBM(GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);

void glEdgeFlagPointerListIBM(GLint stride, const GLboolean ** pointer, GLint ptrstride);

void glFogCoordPointerListIBM(GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);

void glIndexPointerListIBM(GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);

void glNormalPointerListIBM(GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);

void glTexCoordPointerListIBM(GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);

void glVertexPointerListIBM(GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);

void glTbufferMask3DFX(GLuint mask);

void glSampleMaskEXT(GLclampf value, GLboolean invert);

void glSamplePatternEXT(GLenum pattern);

void glTextureColorMaskSGIS(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);

void glDeleteFencesNV(GLsizei n, const GLuint * fences);

void glGenFencesNV(GLsizei n, GLuint * fences);

GLboolean glIsFenceNV(GLuint fence);

GLboolean glTestFenceNV(GLuint fence);

void glGetFenceivNV(GLuint fence, GLenum pname, GLint * params);

void glFinishFenceNV(GLuint fence);

void glSetFenceNV(GLuint fence, GLenum condition);

void glMapControlPointsNV(GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const GLvoid * points);

void glMapParameterivNV(GLenum target, GLenum pname, const GLint * params);

void glMapParameterfvNV(GLenum target, GLenum pname, const GLfloat * params);

void glGetMapControlPointsNV(GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLboolean packed, GLvoid * points);

void glGetMapParameterivNV(GLenum target, GLenum pname, GLint * params);

void glGetMapParameterfvNV(GLenum target, GLenum pname, GLfloat * params);

void glGetMapAttribParameterivNV(GLenum target, GLuint index, GLenum pname, GLint * params);

void glGetMapAttribParameterfvNV(GLenum target, GLuint index, GLenum pname, GLfloat * params);

void glEvalMapsNV(GLenum target, GLenum mode);

void glCombinerStageParameterfvNV(GLenum stage, GLenum pname, const GLfloat * params);

void glGetCombinerStageParameterfvNV(GLenum stage, GLenum pname, GLfloat * params);

GLboolean glAreProgramsResidentNV(GLsizei n, const GLuint * ids, GLboolean * residences);

void glBindProgramNV(GLenum target, GLuint program);

void glDeleteProgramsNV(GLsizei n, const GLuint * programs);

void glExecuteProgramNV(GLenum target, GLuint id, const GLfloat * params);

void glGenProgramsNV(GLsizei n, GLuint * programs);

void glGetProgramParameterdvNV(GLenum target, GLuint index, GLenum pname, GLdouble * params);

void glGetProgramParameterfvNV(GLenum target, GLuint index, GLenum pname, GLfloat * params);

void glGetProgramivNV(GLuint id, GLenum pname, GLint * params);

void glGetProgramStringNV(GLuint id, GLenum pname, GLubyte * program);

void glGetTrackMatrixivNV(GLenum target, GLuint address, GLenum pname, GLint * params);

void glGetVertexAttribdvNV(GLuint index, GLenum pname, GLdouble * params);

void glGetVertexAttribfvNV(GLuint index, GLenum pname, GLfloat * params);

void glGetVertexAttribivNV(GLuint index, GLenum pname, GLint * params);

void glGetVertexAttribPointervNV(GLuint index, GLenum pname, GLvoid ** pointer);

GLboolean glIsProgramNV(GLuint program);

void glLoadProgramNV(GLenum target, GLuint id, GLsizei len, const GLubyte * program);

void glProgramParameter4dNV(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);

void glProgramParameter4dvNV(GLenum target, GLuint index, const GLdouble * params);

void glProgramParameter4fNV(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

void glProgramParameter4fvNV(GLenum target, GLuint index, const GLfloat * params);

void glProgramParameters4dvNV(GLenum target, GLuint index, GLsizei num, const GLdouble * params);

void glProgramParameters4fvNV(GLenum target, GLuint index, GLsizei num, const GLfloat * params);

void glRequestResidentProgramsNV(GLsizei n, const GLuint * ids);

void glTrackMatrixNV(GLenum target, GLuint address, GLenum matrix, GLenum transform);

void glVertexAttribPointerNV(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);

void glVertexAttrib1sNV(GLuint index, GLshort x);

void glVertexAttrib1svNV(GLuint index, const GLshort * v);

void glVertexAttrib2sNV(GLuint index, GLshort x, GLshort y);

void glVertexAttrib2svNV(GLuint index, const GLshort * v);

void glVertexAttrib3sNV(GLuint index, GLshort x, GLshort y, GLshort z);

void glVertexAttrib3svNV(GLuint index, const GLshort * v);

void glVertexAttrib4sNV(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);

void glVertexAttrib4svNV(GLuint index, const GLshort * v);

void glVertexAttrib1fNV(GLuint index, GLfloat x);

void glVertexAttrib1fvNV(GLuint index, const GLfloat * v);

void glVertexAttrib2fNV(GLuint index, GLfloat x, GLfloat y);

void glVertexAttrib2fvNV(GLuint index, const GLfloat * v);

void glVertexAttrib3fNV(GLuint index, GLfloat x, GLfloat y, GLfloat z);

void glVertexAttrib3fvNV(GLuint index, const GLfloat * v);

void glVertexAttrib4fNV(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

void glVertexAttrib4fvNV(GLuint index, const GLfloat * v);

void glVertexAttrib1dNV(GLuint index, GLdouble x);

void glVertexAttrib1dvNV(GLuint index, const GLdouble * v);

void glVertexAttrib2dNV(GLuint index, GLdouble x, GLdouble y);

void glVertexAttrib2dvNV(GLuint index, const GLdouble * v);

void glVertexAttrib3dNV(GLuint index, GLdouble x, GLdouble y, GLdouble z);

void glVertexAttrib3dvNV(GLuint index, const GLdouble * v);

void glVertexAttrib4dNV(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);

void glVertexAttrib4dvNV(GLuint index, const GLdouble * v);

void glVertexAttrib4ubNV(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);

void glVertexAttrib4ubvNV(GLuint index, const GLubyte * v);

void glVertexAttribs1svNV(GLuint index, GLsizei n, const GLshort * v);

void glVertexAttribs2svNV(GLuint index, GLsizei n, const GLshort * v);

void glVertexAttribs3svNV(GLuint index, GLsizei n, const GLshort * v);

void glVertexAttribs4svNV(GLuint index, GLsizei n, const GLshort * v);

void glVertexAttribs1fvNV(GLuint index, GLsizei n, const GLfloat * v);

void glVertexAttribs2fvNV(GLuint index, GLsizei n, const GLfloat * v);

void glVertexAttribs3fvNV(GLuint index, GLsizei n, const GLfloat * v);

void glVertexAttribs4fvNV(GLuint index, GLsizei n, const GLfloat * v);

void glVertexAttribs1dvNV(GLuint index, GLsizei n, const GLdouble * v);

void glVertexAttribs2dvNV(GLuint index, GLsizei n, const GLdouble * v);

void glVertexAttribs3dvNV(GLuint index, GLsizei n, const GLdouble * v);

void glVertexAttribs4dvNV(GLuint index, GLsizei n, const GLdouble * v);

void glVertexAttribs4ubvNV(GLuint index, GLsizei n, const GLubyte * v);

void glTexBumpParameterfvATI(GLenum pname, const GLfloat * param);

void glTexBumpParameterivATI(GLenum pname, const GLint * param);

void glGetTexBumpParameterfvATI(GLenum pname, GLfloat * param);

void glGetTexBumpParameterivATI(GLenum pname, GLint * param);

GLuint glGenFragmentShadersATI(GLuint range);

void glBindFragmentShaderATI(GLuint id);

void glDeleteFragmentShaderATI(GLuint id);

void glBeginFragmentShaderATI();

void glEndFragmentShaderATI();

void glPassTexCoordATI(GLuint dst, GLuint coord, GLenum swizzle);

void glSampleMapATI(GLuint dst, GLuint interp, GLenum swizzle);

void glColorFragmentOp1ATI(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);

void glColorFragmentOp2ATI(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);

void glColorFragmentOp3ATI(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);

void glAlphaFragmentOp1ATI(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);

void glAlphaFragmentOp2ATI(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);

void glAlphaFragmentOp3ATI(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);

void glSetFragmentShaderConstantATI(GLuint dst, const GLfloat * value);

void glDrawMeshArraysSUN(GLenum mode, GLint first, GLsizei count, GLsizei width);

void glPointParameteriNV(GLenum pname, GLint param);

void glPointParameterivNV(GLenum pname, const GLint * params);

void glActiveStencilFaceEXT(GLenum face);

void glProgramNamedParameter4fNV(GLuint id, GLsizei len, const GLubyte * name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

void glProgramNamedParameter4dNV(GLuint id, GLsizei len, const GLubyte * name, GLdouble x, GLdouble y, GLdouble z, GLdouble w);

void glProgramNamedParameter4fvNV(GLuint id, GLsizei len, const GLubyte * name, const GLfloat * v);

void glProgramNamedParameter4dvNV(GLuint id, GLsizei len, const GLubyte * name, const GLdouble * v);

void glGetProgramNamedParameterfvNV(GLuint id, GLsizei len, const GLubyte * name, GLfloat * params);

void glGetProgramNamedParameterdvNV(GLuint id, GLsizei len, const GLubyte * name, GLdouble * params);

void glDepthBoundsEXT(GLclampd zmin, GLclampd zmax);

void glBlendEquationSeparateEXT(GLenum modeRGB, GLenum modeA);

void glBufferParameteriAPPLE(GLenum target, GLenum pname, GLint param);

void glFlushMappedBufferRangeAPPLE(GLenum target, GLintptr offset, GLsizeiptr size);

void glTextureRangeAPPLE(GLenum target, GLsizei length, GLvoid * pointer);

void glGetTexParameterPointervAPPLE(GLenum target, GLenum pname, GLvoid ** params);

void glBlendEquationSeparateATI(GLenum modeRGB, GLenum modeA);

void glStencilOpSeparateATI(GLenum face, GLenum sfail, GLenum zfail, GLenum zpass);

void glStencilFuncSeparateATI(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);

void glProgramEnvParameters4fvEXT(GLenum target, GLuint index, GLsizei count, const GLfloat * params);

void glProgramLocalParameters4fvEXT(GLenum target, GLuint index, GLsizei count, const GLfloat * params);

void glGetQueryObjecti64vEXT(GLuint id, GLenum pname, GLint64EXT * params);

void glGetQueryObjectui64vEXT(GLuint id, GLenum pname, GLuint64EXT * params);

void glBlendFuncSeparateINGR(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

GLhandleARB glCreateDebugObjectMESA();

void glClearDebugLogMESA(GLhandleARB obj, GLenum logType, GLenum shaderType);

void glGetDebugLogMESA(GLhandleARB obj, GLenum logType, GLenum shaderType, GLsizei maxLength, GLsizei * length, GLcharARB * debugLog);

GLsizei glGetDebugLogLengthMESA(GLhandleARB obj, GLenum logType, GLenum shaderType);

void glPointParameterfSGIS(GLenum pname, GLfloat param);

void glPointParameterfvSGIS(GLenum pname, const GLfloat * params);

void glIglooInterfaceSGIX(GLenum pname, const GLvoid * params);

void glDeformationMap3dSGIX(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, GLdouble w1, GLdouble w2, GLint wstride, GLint worder, const GLdouble * points);

void glDeformationMap3fSGIX(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, GLfloat w1, GLfloat w2, GLint wstride, GLint worder, const GLfloat * points);

void glDeformSGIX(GLenum mask);

void glLoadIdentityDeformationMapSGIX(GLenum mask);

