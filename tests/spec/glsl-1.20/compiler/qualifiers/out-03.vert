/*
 * [config]
 * glsl_version: 1.20
 * expect_result: pass
 * [end_config]
 */

#version 120

void f(out int x);

void
main() {
    float x;
    f(x);
    gl_Position = gl_Vertex;
}
