#include "piglit-util.h"

#define SIZE 100

int piglit_width = SIZE, piglit_height = SIZE;
int piglit_window_mode = GLUT_RGBA | GLUT_DOUBLE;

static const char *vert =
	"#version 120\n"
	"void main()\n"
	"{\n"
	"  gl_Position = gl_Vertex;\n"
	"}\n";

static const char *frag =
	"#version 120\n"
	"void main()\n"
	"{\n"
	"  gl_FragColor = vec4(0.5);\n"
	"}\n";

enum piglit_result
piglit_display(void)
{
	float vertices[][2] = {
		{ -1, 1 },
		{ 0, 1 },
		{ -1, 0 }
	};

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glVertexPointer(2, GL_FLOAT, sizeof(vertices[0]), &vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
       	glDrawArrays(GL_TRIANGLES, 0, 3);
	piglit_present_results();

	return PIGLIT_PASS;
}

void
piglit_init(int argc, char **argv)
{
	int vs, fs, prog;
	GLuint fbo, rb;

	piglit_require_GLSL_version(120);
	vs = piglit_compile_shader_text(GL_VERTEX_SHADER, vert);
	fs = piglit_compile_shader_text(GL_FRAGMENT_SHADER, frag);
	prog = piglit_link_simple_program(vs, fs);
	piglit_UseProgram(prog);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glGenRenderbuffers(1, &rb);
	glBindRenderbuffer(GL_RENDERBUFFER, rb);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA, SIZE, SIZE);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rb);
	if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("OMG! Framebuffer not complete!\n");
		piglit_report_result(PIGLIT_FAIL);
	}
}
