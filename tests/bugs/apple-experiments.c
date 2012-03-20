#include "piglit-util.h"

int piglit_width = 100, piglit_height = 100;
int piglit_window_mode = GLUT_RGB | GLUT_DOUBLE;

typedef void (*PFNGLATTACHOBJECTARBPROC_LONG) (unsigned long containerObj, unsigned long obj);

void piglit_init(int argc, char **argv)
{
	int i;
	GLuint prog, vshader, fshader;
	unsigned long fshader2;
	GLsizei count;
	GLuint shaders[16];
	GLhandleARB shadersARB[16];
	PFNGLATTACHOBJECTARBPROC_LONG glAttachObjectARB_long = (PFNGLATTACHOBJECTARBPROC_LONG) glAttachObjectARB;
	printf("sizeof(GLuint) == %ld\n", sizeof(GLuint));
	printf("sizeof(GLhandleARB) == %ld\n", sizeof(GLhandleARB));
	prog = glCreateProgram();
	printf("glCreateProgram() => %d\n", prog);
	vshader = glCreateShader(GL_VERTEX_SHADER);
	printf("glCreateShader(GL_VERTEX_SHADER) => %d\n", vshader);
	fshader = glCreateShader(GL_FRAGMENT_SHADER);
	printf("glCreateShader(GL_FRAGMENT_SHADER) => %d\n", fshader);
	glAttachShader(prog, vshader);
	printf("glAttachShader(%d, %d)\n", prog, vshader);
	//glAttachShader(prog, fshader);
	fshader2 = 0x100000000ul | fshader;
	glAttachObjectARB_long(prog, fshader2);
	printf("glAttachShaderARB(%d, %ld)\n", prog, fshader2);
	glGetAttachedShaders(prog, 8, &count, shaders);
	printf("glGetAttachedShaders(%d, 8, &count, shaders):\n", prog);
	printf("  count = %d\n", count);
	for (i = 0; i < 16; ++i) {
		printf("  shaders[%d] = %d\n", i, shaders[i]);
	}
	glGetAttachedObjectsARB(prog, 8, &count, shadersARB);
	printf("glGetAttachedShadersARB(%d, 8, &count, shadersARB):\n", prog);
	printf("  count = %d\n", count);
	for (i = 0; i < 16; ++i) {
		printf("  shadersARB[%d] = %ld\n", i, (unsigned long) shadersARB[i]);
	}
}

enum piglit_result
piglit_display(void)
{
	return PIGLIT_PASS;
}
