// [config]
// expect_result: fail
// glsl_version: 1.20
// [end config]
//
// GL_ARB_shader_stencil_export should not be available to vertex shaders.

#version 120

#extension GL_ARB_shader_stencil_export: require
void main()
{
  gl_FragStencilRefARB = 0;
}
