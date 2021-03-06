/* [config]
 * expect_result: pass
 * glsl_version: 1.10
 * require_extensions: GL_ARB_shader_texture_lod
 * [end config]
 */
#extension GL_ARB_shader_texture_lod: require

uniform sampler2DShadow s;
varying vec4 coord;
varying vec2 dPdx;
varying vec2 dPdy;

void main()
{
  gl_FragColor = shadow2DProjGradARB(s, coord, dPdx, dPdy);
}
