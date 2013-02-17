// [config]
// expect_result: fail
// glsl_version: 1.50
// check_link: true
// [end config]

#version 150

out block {
    vec4 iface_var;
} inst;

void main()
{
}

