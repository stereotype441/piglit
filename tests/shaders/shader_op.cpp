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

/**
 * \file shader_op.cpp
 *
 * Run a snippet of shader code in a variety of different scenarios,
 * testing that a given set of input values produces a desired set of
 * output values.  Through command-line options, caller is able to
 * adjust:
 *
 * Which type of shader runs the snippet:
 *   -vs = vertex shader
 *   -gs = geometry shader
 *   -fs = fragment shader
 *
 * How the inputs are delivered to the snippet:
 *   -uniform = via uniforms
 *   -in = via shader "in" variables
 *
 * What is done with the output of the snippet:
 *   -compare = immediately checked against expected output
 *   -out = output using a shader "out" variable
 *   -hybrid = output using a shader "out" variable, checked against expected
 *             output in the fragment shader (incompatible with -fs)
 *
 * How the result is delivered to the CPU:
 *   -xfb = via transform feedback (incompatible with -fs and -hybrid)
 *   -color = by rendering it to a color fbo
 *
 * If any of these options is not given, the cartesian product of all
 * possibilities is tested.  Note that some combinations may not be
 * supported by the implementation (e.g. some implementations don't
 * support transform feedback), and other combinations may be
 * incompatible with a particular test (e.g. type "bool" can't be used
 * for ins/outs).  These combinations will be automatically skipped.
 *
 * An additional option, "-debug", can be used to display exactly what
 * shaders were used in each test.
 *
 * In all cases, the output will show exactly which set of test inputs
 * failed, as well as the expected output and tolerance.  In "-out"
 * mode, the actual result will also be shown.
 *
 * In "-out -color" and "-hybrid -color" modes, each test vector shows
 * up on the screen as a 16x16 red or green square (red=fail,
 * green=pass).
 *
 *
 * The test to run is described in an input file whose name is
 * specified on the command line.  Here is an example input file:
 *
 *   [require]
 *   GLSL >= 1.10
 *
 *   [signature]
 *   in vec2 x
 *   in vec2 y
 *   out vec2 theta
 *
 *   [snippet]
 *   theta = atan(x, y);
 *
 *   [test]
 *   tolerance abs 0.0001
 *   tolerance rel 0.001
 *   x 0.0 0.0
 *   y 1e-10 1e-10
 *   theta 0.0 3.1415927
 *
 *   y 0.1 -0.1
 *   theta 0.0 3.1415927
 *
 *   x -0.1 -0.1
 *   theta -0.78539819 -2.3561945
 *
 * The [require] section describes the conditions which must be met by
 * the implementation for the test to be run.  The test is only run if
 * all requirements are met.
 *
 * The [signature] section describes the inputs and outputs to the
 * test, and the type of each.
 *
 * The [snippet] section contains the code snippet, which is included
 * in the shader.  It should be one or more GLSL statements which
 * cause the outputs to be computed based on the inputs.
 *
 * The [test] section contains the input values that should be tested.
 * It consists of test vectors separated by blank lines.  Each test
 * vector specifies a value for each input, an expected value for each
 * output, and optionally, "tolerance rel" and "tolerance abs" values.
 * If any given value is not specified, the value from the previous
 * test vector is assumed.
 *
 * "tolerance rel" specifies a tolerance relative to the size of the
 * expected output (where "size" for vectors and matrices is measured
 * in a root-mean-square sense).  "tolerance abs" specifies an
 * absolute tolerance.  These are combined into a single tolerance by
 * computing max(size * tolerance_rel, tolerance_abs).  If the
 * resulting value is zero, the outputs are required to match the
 * expected values exactly.  Otherwise each output is required to be
 * within tolerance of its expected value (with the distance measured
 * by root-mean-square).
 *
 * "tolerance rel" and "tolerance abs" both default to 1e-5.
 * Tolerance is ignored for integer and boolean outputs.
 */

#include "pipeline-builder.h"
#include "piglit-util-gl-common.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

PIGLIT_GL_TEST_CONFIG_BEGIN

	config.supports_gl_compat_version = 10;
	config.window_visual = PIGLIT_GL_VISUAL_RGB | PIGLIT_GL_VISUAL_ALPHA | PIGLIT_GL_VISUAL_DOUBLE;

PIGLIT_GL_TEST_CONFIG_END

// TODO: use the new feature that allows multiple outputs


enum input_delivery_enum
{
	INPUT_DELIVERY_UNIFORM,
	INPUT_DELIVERY_IN,
};


enum output_action_enum
{
	OUTPUT_ACTION_COMPARE,
	OUTPUT_ACTION_OUT,
	OUTPUT_ACTION_HYBRID,
};


enum output_delivery_enum
{
	OUTPUT_DELIVERY_XFB,
	OUTPUT_DELIVERY_COLOR,
};


struct options
{
	GLenum shader_stage;
	input_delivery_enum input_delivery;
	output_action_enum output_action;
	output_delivery_enum output_delivery;
};


struct static_type_info
{
	const char *name;
	const GLenum gl_type;
	bool is_floating;
	const GLenum base_type;
	unsigned components;
};


static const static_type_info known_types[] = {
	{ "float", GL_FLOAT,             true,  GL_FLOAT,             1 },
	{ "vec2",  GL_FLOAT_VEC2,        true,  GL_FLOAT,             2 },
	{ "vec3",  GL_FLOAT_VEC3,        true,  GL_FLOAT,             3 },
	{ "vec4",  GL_FLOAT_VEC4,        true,  GL_FLOAT,             4 },
	{ "int",   GL_INT,               false, GL_INT,               1 },
	{ "ivec2", GL_INT_VEC2,          false, GL_INT,               2 },
	{ "ivec3", GL_INT_VEC3,          false, GL_INT,               3 },
	{ "ivec4", GL_INT_VEC4,          false, GL_INT,               4 },
	{ "uint",  GL_UNSIGNED_INT,      false, GL_UNSIGNED_INT,      1 },
	{ "uvec2", GL_UNSIGNED_INT_VEC2, false, GL_UNSIGNED_INT,      2 },
	{ "uvec3", GL_UNSIGNED_INT_VEC3, false, GL_UNSIGNED_INT,      3 },
	{ "uvec4", GL_UNSIGNED_INT_VEC4, false, GL_UNSIGNED_INT,      4 },
	{ "bool",  GL_BOOL,              false, GL_BOOL,              1 },
	{ "bvec2", GL_BOOL_VEC2,         false, GL_BOOL,              2 },
	{ "bvec3", GL_BOOL_VEC3,         false, GL_BOOL,              3 },
	{ "bvec4", GL_BOOL_VEC4,         false, GL_BOOL,              4 },
	/* TODO: matrix types */
};


const static_type_info *
find_type(const string &type_name)
{
	for (unsigned i = 0; i < ARRAY_SIZE(known_types); i++) {
		if (type_name == known_types[i].name)
			return &known_types[i];
	}
	return NULL;
}


class type_desc
{
public:
	type_desc(const static_type_info *static_info, unsigned array_size);
	string str() const;

	const static_type_info *static_info;
	unsigned array_size;
};


type_desc::type_desc(const static_type_info *static_info, unsigned array_size)
	: static_info(static_info),
	  array_size(array_size)
{
}


string
type_desc::str() const
{
	if (this->array_size == 0) {
		return this->static_info->name;
	} else {
		stringstream s;
		s << this->static_info->name << "[" << this->array_size << "]";
		return s.str();
	}
}


enum variable_provenance_enum
{
	VARIABLE_PROVENANCE_INPUT = 0,
	VARIABLE_PROVENANCE_OUTPUT = 0x10000,
	VARIABLE_PROVENANCE_UNUSED = 0x20000,
	VARIABLE_PROVENANCE_GS_ITER,
	VARIABLE_PROVENANCE_TOLERANCE,
	VARIABLE_PROVENANCE_COLOR,
	VARIABLE_PROVENANCE_POS,
};


/* contains the name of a variable and its provenance.
 */
class variable_info
{
public:
	variable_info(variable_provenance_enum provenance,
		      const type_desc &type, const string &name);
	variable_info(variable_provenance_enum provenance,
		      const type_desc &type, const string &name,
		      const string &hint);
	bool is_floating() const;
	variable_info make_array_var(unsigned size) const;

	variable_provenance_enum provenance;
	type_desc type;
	string name;
	string hint;
};


variable_info::variable_info(variable_provenance_enum provenance,
			     const type_desc &type, const string &name)
	: provenance(provenance),
	  type(type),
	  name(name),
	  hint(name)
{
}


variable_info::variable_info(variable_provenance_enum provenance,
			     const type_desc &type, const string &name,
			     const string &hint)
	: provenance(provenance),
	  type(type),
	  name(name),
	  hint(hint)
{
}


bool
variable_info::is_floating() const
{
	return this->type.static_info->is_floating;
}


variable_info
variable_info::make_array_var(unsigned size) const
{
	// Can't make a nested array
	assert(this->type.array_size == 0);

	type_desc new_type(this->type.static_info, size);

	return variable_info(this->provenance, new_type, this->name,
			     this->hint);
}


class variable_set : public vector<variable_info>
{
public:
	void append(const vector<variable_info> &vars);
	string make_declarations(const char *qualifier, bool needs_flat) const;
	const variable_info *find_name(const string &name);
};


void
variable_set::append(const vector<variable_info> &vars)
{
	for (unsigned i = 0; i < vars.size(); i++)
		this->push_back(vars[i]);
}


string
variable_set::make_declarations(const char *qualifier, bool needs_flat) const
{
	stringstream s;
	for (unsigned i = 0; i < this->size(); i++) {
		const variable_info &v = (*this)[i];
		if (needs_flat && !v.is_floating())
			s << "flat ";
		if (qualifier)
			s << qualifier << " ";
		s << v.type.str() << " " << v.name << ";" << endl;
	}
	return s.str();
}


const variable_info *
variable_set::find_name(const string &name)
{
	for (unsigned i = 0; i < this->size(); i++) {
		if ((*this)[i].name == name)
			return &(*this)[i];
	}
	return NULL;
}


union value_info
{
	GLfloat f[16];
	GLint i[16];
	GLuint u[16];
	GLint b[16];
};


struct test_vector_info
{
	vector<value_info> inputs;
	vector<value_info> outputs;
	value_info tolerance;
};


struct test_info
{
	string snippet;
	variable_set inputs;
	variable_set outputs;
	vector<test_vector_info> test_vectors;
};
struct test_info the_test;




class test_script_processor
{
public:
	test_script_processor();

	void process_line(const string &line);
	void finish();
	bool parse_value(const char *token_start, const variable_info &v,
			 value_info *value_out);

private:
	enum section_enum
	{
		SECTION_NONE,
		SECTION_REQUIRE,
		SECTION_SIGNTAURE,
		SECTION_SNIPPET,
		SECTION_TEST,
	};

	void enter_section(section_enum s);
	void process_requirement_line(const string &line);
	bool process_signature_line(const string &line);
	void process_snippet_line(const string &line);
	bool process_test_line(const string &line);

	section_enum section;
	stringstream snippet_in_progress;
	test_vector_info current_test_vector;
	bool pending_test_vector;
};


test_script_processor::test_script_processor()
	: section(SECTION_NONE)
{
}


void
test_script_processor::process_line(const string &input_line)
{
	const char *line_begin = input_line.c_str();
	const char *line_end = line_begin + input_line.size();

	/* Strip comments and trailing whitespace, unless inside
	 * [snippet]
	 */
	if (*line_begin == '[' || this->section != SECTION_SNIPPET) {
		const char *comment_pos = strchr(line_begin, '#');
		if (comment_pos != NULL)
			line_end = comment_pos;
		while (line_begin < line_end && isspace(line_end[-1]))
			line_end--;
	}

	string line(line_begin, line_end);

	if (line == "[require]") {
		enter_section(SECTION_REQUIRE);
	} else if (line == "[signature]") {
		enter_section(SECTION_SIGNTAURE);
	} else if (line == "[snippet]") {
		enter_section(SECTION_SNIPPET);
	} else if (line == "[test]") {
		enter_section(SECTION_TEST);
	} else if (line_begin[0] == '[') {
		cout << "Unrecognized section " << line << endl;
		piglit_report_result(PIGLIT_FAIL);
	} else {
		bool ok = true;
		switch (this->section) {
		case SECTION_REQUIRE:
			if (line_begin != line_end)
				process_requirement_line(line);
			break;
		case SECTION_SIGNTAURE:
			if (line_begin != line_end)
				ok = process_signature_line(line);
			break;
		case SECTION_SNIPPET:
			process_snippet_line(line);
			break;
		case SECTION_TEST:
			ok = process_test_line(line);
			break;
		case SECTION_NONE:
			if (line_begin != line_end) {
				cout << "Extra text before first section: "
				     << line << endl;
				piglit_report_result(PIGLIT_FAIL);
			}
			break;
		}
		if (!ok) {
			cout << "Parse error at line: " << line << endl;
			piglit_report_result(PIGLIT_FAIL);
		}
	}
}


void
test_script_processor::finish()
{
	enter_section(SECTION_NONE);
}


void
test_script_processor::enter_section(section_enum s)
{
	switch (this->section) {
	case SECTION_SNIPPET:
		if (the_test.snippet != "") {
			cout << "Multiple [snippet] sections" << endl;
			piglit_report_result(PIGLIT_FAIL);
		}
		the_test.snippet = this->snippet_in_progress.str();
		break;
	case SECTION_TEST:
		if (this->pending_test_vector)
			(void) process_test_line("");
		break;
	default:
		break;
	}

	this->section = s;

	switch (this->section) {
	case SECTION_TEST:
		this->current_test_vector = test_vector_info();
		for (unsigned i = 0; i < the_test.inputs.size(); i++) {
			this->current_test_vector.inputs.push_back(
				value_info());
		}
		for (unsigned i = 0; i < the_test.outputs.size(); i++) {
			this->current_test_vector.outputs.push_back(
				value_info());
		}
		this->pending_test_vector = false;
		break;
	default:
		break;
	}
}


void
test_script_processor::process_requirement_line(const string &line)
{
	// TODO
}


const char *
skip_whitespace(const char *s)
{
	while (*s != '\0' && isspace(*s))
		s++;
	return s;
}


const char *
get_token(const char *s)
{
	while (*s != '\0' && !isspace(*s))
		s++;
	return s;
}


bool
test_script_processor::process_signature_line(const string &line)
{
	const char *token_start = skip_whitespace(line.c_str());
	const char *token_end = get_token(token_start);
	string direction(token_start, token_end);
	bool is_input;
	if (direction == "in")
		is_input = true;
	else if (direction == "out")
		is_input = false;
	else
		return false;
	token_start = skip_whitespace(token_end);
	token_end = get_token(token_start);
	if (token_end == token_start)
		return false;
	string type_str(token_start, token_end);
	const static_type_info *type_info = find_type(type_str);
	if (type_info == NULL)
		return false;
	type_desc type(type_info, 0);
	token_start = skip_whitespace(token_end);
	token_end = get_token(token_start);
	if (token_end == token_start)
		return false;
	string name(token_start, token_end);
	token_start = skip_whitespace(token_end);
	if (*token_start != '\0')
		return false;
	unsigned provenance;
	if (is_input) {
		provenance = VARIABLE_PROVENANCE_INPUT
			+ the_test.inputs.size();
	} else {
		provenance = VARIABLE_PROVENANCE_OUTPUT
			+ the_test.outputs.size();
	}
	variable_info v((variable_provenance_enum) provenance, type, name);
	(is_input ? the_test.inputs : the_test.outputs).push_back(v);
	return true;
}


void
test_script_processor::process_snippet_line(const string &line)
{
	this->snippet_in_progress << line << endl;
}


bool
test_script_processor::process_test_line(const string &line)
{
	if (line == "") {
		if (this->pending_test_vector) {
			// TODO: compute tolerance.
			the_test.test_vectors.push_back(
				this->current_test_vector);
			this->pending_test_vector = false;
		}
	} else {
		this->pending_test_vector = true;
		// TODO: handle tolerance
		const char *token_start = skip_whitespace(line.c_str());
		const char *token_end = get_token(token_start);
		string name(token_start, token_end);
		const variable_info *v = the_test.inputs.find_name(name);
		if (v == NULL)
			v = the_test.outputs.find_name(name);
		if (v == NULL)
			return false;
		value_info value;
		if (!parse_value(skip_whitespace(token_end), *v, &value))
			return false;
		if (v->provenance < VARIABLE_PROVENANCE_OUTPUT) {
			unsigned i = v->provenance - VARIABLE_PROVENANCE_INPUT;
			this->current_test_vector.inputs[i] = value;
		} else {
			unsigned i = v->provenance
				- VARIABLE_PROVENANCE_OUTPUT;
			this->current_test_vector.outputs[i] = value;
		}
	}
	return true;
}


bool
test_script_processor::parse_value(const char *token_start,
				     const variable_info &v,
				     value_info *value_out)
{
	const char *token_end;
	for (unsigned i = 0; i < v.type.static_info->components; i++) {
		switch (v.type.static_info->base_type) {
		case GL_FLOAT:
			value_out->f[i]
				= strtof(token_start, (char **) &token_end);
			if (token_end == token_start)
				return false;
			break;
		default:
			// TODO
			return false;
		}
		if (*token_end != '\0' && !isspace(*token_end))
			return false;
		token_start = skip_whitespace(token_end);
	}
	if (*token_end != '\0')
		return false;
	return true;
}


void
process_test_script(const char *script_name)
{
	unsigned text_size;
	char *text = piglit_load_text_file(script_name, &text_size);
	if (text == NULL) {
		cout << "Could not read file \"" << script_name << "\""
		     << endl;
		piglit_report_result(PIGLIT_FAIL);
	}

	test_script_processor processor;
	const char *pos = text;
	while (*pos != '\0') {
		const char *line_end = strchrnul(pos, '\n');
		processor.process_line(string(pos, line_end));
		if (*line_end == '\0')
			break;
		pos = line_end + 1;
	}
	processor.finish();

	free(text);
}


class shader_op_pipeline_builder : public pipeline_builder
{
public:
	explicit shader_op_pipeline_builder(const options *opts);

	bool plan_shaders();

private:
	void add_input(GLenum stage, const string &type, const string &name);
	void add_output(GLenum stage, bool flat, const string &type,
			const string &name);

	const options *opts;
};


shader_op_pipeline_builder::shader_op_pipeline_builder(const options *opts)
	: opts(opts)
{
}


bool
shader_op_pipeline_builder::plan_shaders()
{
	/* Bail out if options are incompatible */
	if (opts->output_action == OUTPUT_ACTION_HYBRID
	    && opts->shader_stage == GL_FRAGMENT_SHADER) {
		return false;
	}
	if (opts->output_delivery == OUTPUT_DELIVERY_XFB
	    && opts->shader_stage == GL_FRAGMENT_SHADER) {
		return false;
	}
	if (opts->output_delivery == OUTPUT_DELIVERY_XFB
	    && opts->output_action == OUTPUT_ACTION_HYBRID) {
		return false;
	}

	add_stage(GL_VERTEX_SHADER);
	if (opts->shader_stage == GL_FRAGMENT_SHADER
	    || opts->output_delivery == OUTPUT_DELIVERY_COLOR) {
		add_stage(GL_FRAGMENT_SHADER);
	}
	/* TODO: add geometry shader stage if necessary */
	shader_builder *test_stage = get_stage(opts->shader_stage);

	for (unsigned i = 0; i < the_test.inputs.size(); i++)
		reserve_name(the_test.inputs[i].name);
	for (unsigned i = 0; i < the_test.outputs.size(); i++)
		reserve_name(the_test.outputs[i].name);

	/* Add snippet */
	/* TODO: modify snippet for geometry shader */
	test_stage->main += "/* TEST SNIPPET BEGIN */\n";
	test_stage->main += the_test.snippet;
	test_stage->main += "/* TEST SNIPPET END */\n";

	/* Route inputs */
	for (unsigned i = 0; i < the_test.inputs.size(); i++) {
		add_input(opts->shader_stage, the_test.inputs[i].type.str(),
			  the_test.inputs[i].name);
	}

	switch (opts->output_action) {
	case OUTPUT_ACTION_COMPARE:
		assert(!"TODO");
		break;
	case OUTPUT_ACTION_OUT:
		for (unsigned i = 0; i < the_test.outputs.size(); i++) {
			/* TODO: keep track of the output name (or perhaps index). */
			bool flat = !the_test.outputs[i].type.static_info
				->is_floating;
			add_output(opts->shader_stage, flat,
				   the_test.outputs[i].type.str(),
				   the_test.outputs[i].name);
		}
		break;
	case OUTPUT_ACTION_HYBRID:
		assert(!"TODO");
		break;
	}

	/* TODO: are there some circumstances where we don't need to
	 * set gl_Position?
	 */
	(void) route_gl_position_input();

	return true;
}


void
shader_op_pipeline_builder::add_input(GLenum stage, const string &type,
				      const string &name)
{
	/* TODO: keep track of a mapping between input name and input
	 * number.
	 */
	switch (opts->input_delivery) {
	case INPUT_DELIVERY_UNIFORM:
		get_stage(stage)->add_uniform(type, name);
		break;
	case INPUT_DELIVERY_IN:
		assert(!"TODO");
		break;
	}
}


void
shader_op_pipeline_builder::add_output(GLenum stage, bool flat,
				       const string &type, const string &name)
{
	switch (opts->output_delivery) {
	case OUTPUT_DELIVERY_COLOR:
		/* TODO: keep track of a mapping between color number
		 * and output number.
		 */
		route_color_output(stage, flat, type, name);
		break;
	case OUTPUT_DELIVERY_XFB:
		/* TODO: keep track of a mapping between xfb name and
		 * output number.
		 */
		route_xfb_output(stage, flat, type, name);
		break;
	}
}


piglit_result
run_test(const options *opts)
{
	shader_op_pipeline_builder pb(opts);
	if (!pb.plan_shaders()) {
		cout << "Unsupported options" << endl;
		return PIGLIT_FAIL;
	}

	pb.dump_shaders();

	assert(!"TODO");
	return PIGLIT_FAIL;
}


void
piglit_init(int argc, char **argv)
{
	// TODO: choose file on command line
	process_test_script("/home/pberry/tmp/shader_op.txt");
}


piglit_result
piglit_display()
{
	// TODO: parse opts
	options opts;
	opts.shader_stage = GL_FRAGMENT_SHADER;
	opts.input_delivery = INPUT_DELIVERY_UNIFORM;
	opts.output_action = OUTPUT_ACTION_OUT;
	opts.output_delivery = OUTPUT_DELIVERY_COLOR;
	return run_test(&opts);
}
