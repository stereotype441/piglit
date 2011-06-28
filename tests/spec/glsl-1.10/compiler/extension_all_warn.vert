// [config]
// expect_result: pass
// glsl_version: 1.10
// require_extensions: GL_ARB_explicit_attrib_location
// [end config]
//
// "#extension all: warn" should enable the GL_ARB_explicit_attrib_location
// extension.  (It will produce a warning, but the compilation won't fail).

#version 110
#extension all: warn
layout(location = 0) in vec4 vertex;

void main()
{
	gl_Position = vertex;
}
