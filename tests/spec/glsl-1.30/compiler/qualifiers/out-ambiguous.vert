// [config]
// expect_result: fail
// glsl_version: 1.30
// [end config]
#version 130
vec4 f(in vec4 x, out ivec4 y)
{
  y = ivec4(0);
  return vec4(0.0);
}

vec4 f(in vec4 x, out vec4 y)
{
  y = vec4(0.0);
  return vec4(0.0);
}

void main()
{
  ivec4 x_actual = ivec4(1, 3, 3, 7);
  vec4 y_actual;
  vec4 f_result = f(x_actual, y_actual);
  gl_Position = f_result + y_actual;
}

