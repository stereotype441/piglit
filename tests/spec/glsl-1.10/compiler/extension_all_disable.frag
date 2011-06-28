// [config]
// expect_result: fail
// glsl_version: 1.10
//
// # NOTE: Config section was auto-generated from file
// # NOTE: 'glslparser.tests' at git revision
// # NOTE: 6cc17ae70b70d150aa1751f8e28db7b2a9bd50f0
// [end config]

#extension all: disable

uniform sampler2DRect samp; // Not allowed since all extensions disabled

void main()
{
	gl_FragColor = texture2DRect(samp, vec2(0.0, 0.0));
}
