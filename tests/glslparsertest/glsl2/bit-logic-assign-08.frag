// [config]
// expect_result: fail
// glsl_version: 1.30
//
// # NOTE: Config section was auto-generated from file
// # NOTE: 'glslparser.tests' at git revision
// # NOTE: 6cc17ae70b70d150aa1751f8e28db7b2a9bd50f0
// [end config]

// Expected: FAIL, glsl == 1.30
//
// Description: bit-and assignment with argument type (ivec2, uint)
//
// From page 50 (page 56 of PDF) of the GLSL 1.30 spec:
// "The fundamental types of the operands (signed or unsigned) must match"

#version 130
void main() {
    ivec2 v = ivec2(0, 1);
    v &= uint(7);
}
