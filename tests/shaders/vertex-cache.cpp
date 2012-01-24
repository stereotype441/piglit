/*
 * Copyright © 2012 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/**
 * Theory of operation:
 *
 * - Use a fletcher checksum (easy to compute)
 * - Draw a uniform grid of small triangles, where each grid position is drawn in pseudo-random order.
 * - Both FS and VS compute checksums, so that we are certain that neither attribute data nor varying data is corrupted.
 * - Attribute/varying 0 contains the checksum
 * - Attribute/varying 1 contains the screen position
 * - Remaining attributes/varyings contain pseudo-random data.
 * - Use a deterministic pseudo-random number generator for repeatability (and uniformity across OSes)
 * - FS outputs a small number if ok, large if bad
 * - Draw using a blend mode that is additive with saturation, so that we will catch (a) parts of screen covered by no triangles, (b) parts of screen covered by too many triangles, (c) parts of screen where checksum failed.
 * - Optionally, confirm varyings using XFB.
 *
 * TODO: need to rewrite everything below this line.
 */

compile_vs(unsigned num_attributes, unsigned num_varyings)
{
	std::string vs_text;
	char tmp[1024];
	vs_text += "#version 130\n";
	sprintf(tmp, "#define NUM_ATTRIBUTES %u\n", num_attributes);
	vs_text += tmp;
	sprintf(tmp, "#define NUM_VARYINGS %u\n", num_varyings);
	vs_text += tmp;
	vs_text +=
		"\n"
		"uniform uvec4 attribute_multipliers[NUM_ATTRIBUTES];\n"
		"uniform uvec4 attribute_offsets[NUM_ATTRIBUTES];\n"
		"uniform uvec4 varying_multipliers[NUM_VARYINGS];\n"
		"uniform uvec4 varying_offsets[NUM_VARYINGS];\n"
	for (unsigned i = 0; i < num_attributes; ++i) {
		sprintf(tmp, "in uvec4 attribute_%u;\n", i);
		vs_text += tmp;
	}
	vs_text +=
		"flat out uvec4 varyings[NUM_VARYINGS];\n"
		"\n"
		"void main()\n"
		"{\n"
		"  uint grid_index = attribute_0.x;\n"
		"  uvec2 grid_coords = uvec2(grid_index % 65536u,\n"
		"                            grid_index / 65536u);\n"
		"  gl_Position = vec4(grid_coords - grid_offset, 0.0,\n"
		"                     grid_scale);\n"
		"\n"
		"  bool attributes_ok = true;\n";
	for (unsigned i = 0; i < num_attributes; ++i) {
		sprintf(tmp,
			"  if (attribute_%u != "
			"attribute_multipliers[%u] * grid_index + "
			"attribute_offsets[%u]) {\n",
			i, i, i);
		vs_text += tmp;
		vs_text +=
			"    attributes_ok = false;\n"
			"  }\n";
	}
	vs_text +=
		"  if (attributes_ok) {\n"
		"    for (int i = 0; i < NUM_VARYINGS; ++i) {\n"
		"      varyings[i] = varying_multipliers[i] * grid_index +"
		"                    varying_offsets[i];\n"
		"    }\n"
		"  } else {\n"
		"    for (int i = 0; i < NUM_VARYINGS; ++i {\n"
		"      varyings[i] = uvec4(0u);\n"
		"    }\n"
		"  }\n"
		"}\n";
}

compile_fs(unsigned num_varyings)
{
	char tmp[1024];
	fs_text += "#version 130\n";
	sprintf(tmp, "#define NUM_VARYINGS %u\n", num_varyings);
	fs_text += tmp;
	fs_text +=
		"\n"
		"uniform uvec4 varying_multipliers[NUM_VARYINGS];\n"
		"uniform uvec4 varying_offsets[NUM_VARYINGS];\n"
		"flat in uvec4 varyings[NUM_VARYINGS];\n"
		"\n"
		"void main()\n"
		"{\n"
		"  uint grid_index = varyings[0].x;\n"
		"  uvec2 grid_coords = uvec2(grid_index % 65536u,\n"
		"                            grid_index / 65536u);\n"
		"  uvec2 expected_grid_coords = uvec2(gl_FragCoord.xy) /\n"
		"                               grid_size;\n"
		"\n"
		"  bool varyings_ok = true;\n"
		"  for (int i = 0; i < NUM_VARYINGS; ++i) {\n"
		"    if (varyings[i] !=\n"
		"        varying_multipliers[i] * grid_index +\n"
		"        varying_offsets[i]) {\n"
		"      varyings_ok = false;\n"
		"    }\n"
		"  }\n"
		"  if (varyings_ok) {\n"
		"    gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
		"  } else {\n"
		"    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"  }\n"
		"}\n";
}
