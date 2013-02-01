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


#include "pipeline-builder.h"
#include <sstream>
#include <iostream>

using namespace std;


shader_builder::shader_builder(GLenum stage)
	: stage(stage)
{
}


void
shader_builder::add_uniform(const string &type, const string &name)
{
	/* TODO: handle pre-GLSL-1.30 syntax */
	stringstream ss;
	ss << "uniform " << type << " " << name << ";" << endl;
	this->globals += ss.str();
}


void
shader_builder::add_output(bool flat, const string &type, const string &name)
{
	/* TODO: handle pre-GLSL-1.30 syntax */
	stringstream ss;
	if (flat && this->stage != GL_FRAGMENT_SHADER)
		ss << "flat ";
	ss << "out " << type << " " << name << ";" << endl;
	this->globals += ss.str();
}


void
shader_builder::add_input(bool flat, const string &type, const string &name)
{
	/* TODO: handle pre-GLSL-1.30 syntax */
	stringstream ss;
	if (flat && this->stage != GL_VERTEX_SHADER)
		ss << "flat ";
	ss << "in " << type << " " << name << ";" << endl;
	this->globals += ss.str();
}


void
shader_builder::copy_data(const string &input_name, const string &output_name)
{
	/* TODO: geometry shaders will need special logic here */
	stringstream ss;
	ss << output_name << " = " << input_name << ";" << endl;
	this->main += ss.str();
}


string
shader_builder::str() const
{
	/* TODO: handle versions other than 130 */
	stringstream ss;
	ss << "#version 130" << endl;
	ss << this->globals;
	ss << endl;
	ss << "void main()" << endl;
	ss << "{" << endl;
	ss << indent(this->main);
	ss << "}" << endl;
	return ss.str();
}


string
shader_builder::indent(const string &input,
		       const string &indentation)
{
	const char *s = input.c_str();
	stringstream ss;
	while (*s != '\0') {
		const char *newline_pos = strchr(s, '\n');
		if (newline_pos == NULL) {
			ss << indentation << s;
			break;
		} else {
			ss << indentation << string(s, newline_pos + 1);
			s = newline_pos + 1;
		}
	}
	return ss.str();
}


pipeline_builder::pipeline_builder()
	: vs_builder(NULL),
	  fs_builder(NULL)
{
}


void
pipeline_builder::reserve_name(const string &name)
{
	this->reserved_names.insert(name);
}


string
pipeline_builder::generate_name(const string &base)
{
	if (this->reserved_names.find(base) == this->reserved_names.end()) {
		this->reserved_names.insert(base);
		return base;
	}

	for (unsigned i = 0; ; i++) {
		stringstream s;
		s << base << i;
		string candidate_name = s.str();
		if (this->reserved_names.find(candidate_name)
		    == this->reserved_names.end()) {
			this->reserved_names.insert(candidate_name);
			return candidate_name;
		}
	}
}


void
pipeline_builder::add_stage(GLenum stage)
{
	switch (stage) {
	case GL_VERTEX_SHADER:
		if (this->vs_builder == NULL)
			this->vs_builder = new shader_builder(stage);
		break;
	case GL_FRAGMENT_SHADER:
		if (this->fs_builder == NULL)
			this->fs_builder = new shader_builder(stage);
		break;
	default:
		assert(!"Unsupported stage");
		break;
	}
}


shader_builder *
pipeline_builder::get_stage(GLenum stage)
{
	switch (stage) {
	case GL_VERTEX_SHADER:
		return this->vs_builder;
	case GL_FRAGMENT_SHADER:
		return this->fs_builder;
	default:
		assert(!"Unsupported stage");
		return NULL;
	}
}


string
pipeline_builder::route_gl_position_input()
{
	string var_name = generate_name("pos");
	/* TODO: handle Geometry shaders */
	this->vs_builder->add_input(false, "vec4", var_name);
	this->vs_builder->copy_data(var_name, "gl_Position");
	return var_name;
}


void
pipeline_builder::route_color_output(GLenum stage, bool flat,
				     const string &type,
				     const string &orig_name)
{
	string name = orig_name;
	this->get_stage(stage)->add_output(flat, type, name);
	while (stage != GL_FRAGMENT_SHADER) {
		stage = next_stage(stage);
		string new_name = generate_name(orig_name);
		shader_builder *stage_builder = this->get_stage(stage);
		stage_builder->add_input(flat, type, name);
		stage_builder->add_output(flat, type, new_name);
		stage_builder->copy_data(name, new_name);
		name = new_name;
	}
}


string
pipeline_builder::route_xfb_output(GLenum stage, bool flat, const string &type,
				   const string &orig_name)
{
	string name = orig_name;
	this->get_stage(stage)->add_output(flat, type, name);
	stage = next_stage(stage);
	while (stage != GL_FRAGMENT_SHADER && stage != GL_NONE) {
		string new_name = generate_name(orig_name);
		shader_builder *stage_builder = this->get_stage(stage);
		stage_builder->add_input(flat, type, name);
		stage_builder->add_output(flat, type, new_name);
		stage_builder->copy_data(name, new_name);
		name = new_name;
		stage = next_stage(stage);
	}
	return name;
}


GLenum
pipeline_builder::next_stage(GLenum stage)
{
	do {
		switch (stage) {
		case GL_NONE:
			stage = GL_VERTEX_SHADER;
			break;
		case GL_VERTEX_SHADER:
			stage = GL_FRAGMENT_SHADER;
			break;
		case GL_FRAGMENT_SHADER:
			return GL_NONE;
			break;
		default:
			assert(!"Unsupported stage");
			return GL_NONE;
		}
	} while (get_stage(stage) == NULL);

	return stage;
}


static const char *stage_name(GLenum stage)
{
	switch (stage) {
	case GL_VERTEX_SHADER:
		return "vertex";
	case GL_FRAGMENT_SHADER:
		return "fragment";
	default:
		assert(!"Unsupported stage");
		return "???";
	}
}


void
pipeline_builder::dump_shaders()
{
	GLenum stage = GL_NONE;
	while (true) {
		stage = next_stage(stage);
		if (stage == GL_NONE)
			break;
		cout << "[" << stage_name(stage) << " shader]" << endl;
		cout << get_stage(stage)->str();
		cout << endl;
	}
}
