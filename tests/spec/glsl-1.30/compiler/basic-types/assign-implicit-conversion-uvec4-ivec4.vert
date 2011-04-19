/* [config]
 * expect_result: fail
 * glsl_version: 1.30
 * [end config]
 *
 * From page 27 (page 33 of the PDF) of the GLSL 1.30 spec:
 *
 *     "There are no implicit conversions between signed and unsigned
 *     integers."
 */
#version 130

void main()
{
	ivec4 x = ivec4(5);
	uvec4 y = x;

	gl_Position = vec4(1.0);
}
