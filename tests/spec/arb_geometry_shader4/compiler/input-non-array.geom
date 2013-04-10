// [config]
// expect_result: fail
// glsl_version: 1.10
// check_link: true
// require_extensions: GL_ARB_geometry_shader4
// [end config]

#version 110
#extension GL_ARB_geometry_shader4: enable

/* Shouldn't be allowed--geometry shader inputs must be arrays */
varying in vec4 foo;

void main()
{
}
