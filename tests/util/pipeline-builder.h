/*
 * Copyright © 2013 Intel Corporation
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


#include "piglit-util-gl-common.h"
#include <string>
#include <set>

using namespace std;

class shader_builder
{
public:
	shader_builder(GLenum stage);

	void add_uniform(const string &type, const string &name);

	void add_input(bool flat, const string &type, const string &name);

	void add_output(bool flat, const string &type, const string &name);

	void copy_data(const string &input_name, const string &output_name);

	string str() const;

	static string indent(const string &input,
			     const string &indentation = "  ");

	const GLenum stage;

	/**
	 * Global declarations.
	 */
	string globals;

	/**
	 * Statements that should be included in the shader's main()
	 * function.
	 */
	string main;
};


class pipeline_builder
{
public:
	pipeline_builder();

	/**
	 * Reserve the given name, to ensure that it won't be used by
	 * the pipeline_builder for any automatically-generated
	 * variable names.
	 */
	void reserve_name(const string &name);

	/**
	 * Generate a name (one that hasn't already been passed to
	 * reserve_name) by taking the given base string and appending
	 * an integer if necessary.
	 */
	string generate_name(const string &base);

	/**
	 * Add a shader stage to the pipeline.
	 *
	 * \c stage should be an enum suitable for passing to
	 * glCreateShader(), e.g. GL_VERTEX_SHADER or
	 * GL_FRAGMENT_SHADER.
	 */
	void add_stage(GLenum stage);

	/**
	 * Get the shader_builder object for a given stage of the
	 * pipeline.
	 *
	 * \c stage should be an enum suitable for passing to
	 * glCreateShader(), e.g. GL_VERTEX_SHADER or
	 * GL_FRAGMENT_SHADER.
	 */
	shader_builder *get_stage(GLenum stage);

	/**
	 * Route a vertex attribute through the pipeline to
	 * gl_Position, and return the name of the vertex attribute.
	 */
	string route_gl_position_input();

	void route_color_output(GLenum stage, bool flat, const string &type,
				const string &name);

	string route_xfb_output(GLenum stage, bool flat, const string &type,
				const string &name);

	GLenum next_stage(GLenum stage);

	void dump_shaders();

private:
	set<string> reserved_names;

	shader_builder *vs_builder;

	shader_builder *fs_builder;
};
