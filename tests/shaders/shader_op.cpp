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

#include "piglit-util-gl-common.h"
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <set>

using namespace std;

PIGLIT_GL_TEST_CONFIG_BEGIN

	config.supports_gl_compat_version = 10;
	config.window_visual = PIGLIT_GL_VISUAL_RGB | PIGLIT_GL_VISUAL_ALPHA | PIGLIT_GL_VISUAL_DOUBLE;

PIGLIT_GL_TEST_CONFIG_END

// TODO: use the new feature that allows multiple outputs

#define ELEMENTS(x) (sizeof(x) / sizeof(x[0]))

enum shader_stage_enum
{
	SHADER_STAGE_VERTEX,
	SHADER_STAGE_GEOMETRY,
	SHADER_STAGE_FRAGMENT,
};
const unsigned NUM_SHADER_STAGES = SHADER_STAGE_FRAGMENT + 1;

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
	shader_stage_enum shader_stage;
	input_delivery_enum input_delivery;
	output_action_enum output_action;
	output_delivery_enum output_delivery;
};


struct static_type_info
{
	const char *name;
	const GLenum gl_type; // TODO: needed?
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
	for (unsigned i = 0; i < ELEMENTS(known_types); i++) {
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
	float f[16];
	int i[16];
	unsigned u[16];
	bool b[16];
};


struct test_vector_info
{
	vector<value_info> inputs;
	vector<value_info> outputs;
	float tolerance;
};


struct test_info
{
	string snippet;
	variable_set inputs;
	variable_set outputs;
	vector<test_vector_info> test_vectors;
};
struct test_info the_test;


struct stage_info
{
	variable_set input_vars;
	variable_set output_vars;
	variable_set uniform_vars;
	variable_set local_vars;
	string snippet;
};


class pipeline_info
{
public:
	pipeline_info(unsigned version,
		      const vector<shader_stage_enum> &stages,
		      const variable_set &inputs,
		      const variable_set &outputs);
	void mark_used_varnames(const variable_set &vars);
	void add_snippet(shader_stage_enum stage, const string &snippet);
	void add_snippet_top(shader_stage_enum stage, const string &snippet);
	void route_inputs(shader_stage_enum stage, const variable_set &inputs);
	void route_inputs_or_uniforms(shader_stage_enum stage,
				      const variable_set &inputs,
				      input_delivery_enum delivery);
	void add_locals(shader_stage_enum stage, const variable_set &locals);
	void route_outputs(shader_stage_enum stage, const variable_set &outputs,
			   output_delivery_enum delivery);
	variable_set route_data(shader_stage_enum starting_stage,
				shader_stage_enum ending_stage,
				const variable_set &vars);
	string get_shader(shader_stage_enum stage) const;
	bool is_stage_present(shader_stage_enum stage) const
	{
		return stages.find(stage) != stages.end();
	}
	void dump_shaders() const;
	void add_comparison_logic(shader_stage_enum stage,
				  const variable_set &outputs,
				  const options &opts);
	void copy_data(shader_stage_enum stage, const variable_set &inputs,
		       const variable_set &outputs);
	variable_info make_unique_var(variable_provenance_enum provenance,
				      const type_desc &type,
				      const string &hint);
	variable_set generate_parallel_vars(variable_set const &vars);
	shader_stage_enum last_non_fs_stage() const;
	GLuint compile() const;
	void set_uniform_values();

private:
	unsigned version;
	map<shader_stage_enum, stage_info> stages;
	variable_set input_vars;
	variable_set output_vars;
	variable_set uniform_vars;
	set<string> varnames_used;
	variable_info gs_iterator;
};


pipeline_info::pipeline_info(unsigned version,
			     const vector<shader_stage_enum> &stages,
			     const variable_set &inputs,
			     const variable_set &outputs)
	: version(version),
	  gs_iterator(VARIABLE_PROVENANCE_UNUSED,
		      type_desc(find_type("int"), 0), "unused")
{
	mark_used_varnames(inputs);
	mark_used_varnames(outputs);
	for (unsigned i = 0; i < stages.size(); i++) {
		this->stages[stages[i]] = stage_info();
		if (stages[i] == SHADER_STAGE_GEOMETRY) {
			type_desc int_type(find_type("int"), 0);
			this->gs_iterator
				= make_unique_var(VARIABLE_PROVENANCE_GS_ITER,
						  int_type, "i");
			this->stages[stages[i]].local_vars.push_back(
				this->gs_iterator);
		}
	}
}


void
pipeline_info::mark_used_varnames(const variable_set &vars)
{
	for (unsigned i = 0; i < vars.size(); i++)
		this->varnames_used.insert(vars[i].name);
}


void
pipeline_info::add_snippet(shader_stage_enum stage, const string &snippet)
{
	assert(is_stage_present(stage));
	this->stages[stage].snippet.append(snippet);
}


void
pipeline_info::add_snippet_top(shader_stage_enum stage, const string &snippet)
{
	assert(is_stage_present(stage));
	this->stages[stage].snippet = snippet + this->stages[stage].snippet;
}


void
pipeline_info::route_inputs(shader_stage_enum stage,
			    const variable_set &inputs)
{
	assert(is_stage_present(stage));

	variable_set interstage_vars;
	if (stage == SHADER_STAGE_GEOMETRY) {
		variable_set array_vars;
		this->stages[stage].local_vars.append(inputs);
		stringstream s;
		for (unsigned i = 0; i < inputs.size(); i++) {
			variable_info interstage_var
				= make_unique_var(inputs[i].provenance,
						  inputs[i].type,
						  inputs[i].hint);
			interstage_vars.push_back(interstage_var);
			variable_info array_var
				= interstage_var.make_array_var(3);
			array_vars.push_back(array_var);
			s << inputs[i].name << " = " << array_var.name << "["
			  << gs_iterator.name << "];" << endl;
		}
		add_snippet_top(SHADER_STAGE_GEOMETRY, s.str());
		this->stages[stage].input_vars.append(array_vars);
	} else {
		interstage_vars = inputs;
		this->stages[stage].input_vars.append(interstage_vars);
	}

	for (int i = stage - 1; i >= 0; i--) {
		if (!is_stage_present((shader_stage_enum) i))
			continue;
		variable_set in_vars = generate_parallel_vars(inputs);
		copy_data((shader_stage_enum) i, in_vars, interstage_vars);
		interstage_vars = in_vars;
	}

	this->input_vars.append(interstage_vars);
}


void
pipeline_info::route_inputs_or_uniforms(shader_stage_enum stage,
					const variable_set &inputs,
					input_delivery_enum delivery)
{
	assert(is_stage_present(stage));
	switch (delivery) {
	case INPUT_DELIVERY_UNIFORM:
		this->stages[stage].uniform_vars.append(inputs);
		this->uniform_vars.append(inputs);
		break;
	case INPUT_DELIVERY_IN:
		route_inputs(stage, inputs);
		break;
	}
}


void
pipeline_info::add_locals(shader_stage_enum stage, const variable_set &locals)
{
	assert(is_stage_present(stage));
	this->stages[stage].local_vars.append(locals);
}


void
pipeline_info::route_outputs(shader_stage_enum stage,
			     const variable_set &outputs,
			     output_delivery_enum delivery)
{
	assert(is_stage_present(stage));
	this->stages[stage].output_vars.append(outputs);

	variable_set interstage_vars = outputs;
	for (unsigned i = stage + 1; i < NUM_SHADER_STAGES; i++) {
		if (!is_stage_present((shader_stage_enum) i))
			continue;
		variable_set out_vars = generate_parallel_vars(outputs);
		copy_data((shader_stage_enum) i, interstage_vars, out_vars);
		interstage_vars = out_vars;
	}

	this->output_vars.append(interstage_vars);
}


variable_set
pipeline_info::route_data(shader_stage_enum starting_stage,
			  shader_stage_enum ending_stage,
			  const variable_set &vars)
{
	assert(starting_stage < ending_stage);
	this->stages[starting_stage].output_vars.append(vars);

	variable_set interstage_vars = vars;
	for (int i = starting_stage + 1; i < ending_stage; i++) {
		if (!is_stage_present((shader_stage_enum) i))
			continue;
		variable_set out_vars = generate_parallel_vars(vars);
		copy_data((shader_stage_enum) i, interstage_vars, out_vars);
		interstage_vars = out_vars;
	}

	this->stages[ending_stage].input_vars.append(interstage_vars);
	return interstage_vars;
}


string
indent(const string &input_str)
{
	const char *in = input_str.c_str();
	stringstream s;
	while (*in) {
		const char *newline_pos = strchr(in, '\n');
		if (newline_pos == NULL) {
			s << "   " << in;
			break;
		} else {
			s << "   " << string(in, newline_pos + 1);
			in = newline_pos + 1;
		}
	}
	return s.str();
}


string
pipeline_info::get_shader(shader_stage_enum stage) const
{
	/* Figure out the correct keywords for this shader stage and
	 * GLSL version
	 */
	const char *input_qualifier;
	const char *output_qualifier;
	bool input_needs_flat = false;
	bool output_needs_flat = false;
	if (version >= 130) {
		input_qualifier = "in";
		output_qualifier = "out";
		switch (stage) {
		case SHADER_STAGE_VERTEX:
			output_needs_flat = true;
			break;
		case SHADER_STAGE_GEOMETRY:
			input_needs_flat = true;
			output_needs_flat = true;
			break;
		case SHADER_STAGE_FRAGMENT:
			input_needs_flat = true;
			break;
		}
	} else {
		switch (stage) {
		case SHADER_STAGE_VERTEX:
			input_qualifier = "attribute";
			output_qualifier = "varying";
			break;
		case SHADER_STAGE_GEOMETRY:
			input_qualifier = "varying in";
			output_qualifier = "varying out";
			break;
		case SHADER_STAGE_FRAGMENT:
			input_qualifier = "varying";
			output_qualifier = NULL; /* should not be used */
			break;
		}
	}

	stringstream s;
	s << "#version " << this->version << endl; // TODO: ES
	// TODO: extensions

	/* Generate global input/output declarations. */
	const stage_info &si = this->stages.at(stage);
	s << si.input_vars.make_declarations(input_qualifier,
					     input_needs_flat);
	s << si.output_vars.make_declarations(output_qualifier,
					      output_needs_flat);
	s << si.uniform_vars.make_declarations("uniform", false);

	/* Generate main function. */
	s << "void main()" << endl;
	s << "{" << endl;
	s << indent(si.local_vars.make_declarations(NULL, false));
	if (stage == SHADER_STAGE_GEOMETRY) {
		s << "   for (" << this->gs_iterator.name << " = 0; "
		  << this->gs_iterator.name << " < 3; "
		  << this->gs_iterator.name << "++) {" << endl;
		s << indent(indent(si.snippet));
		s << "      EmitVertex();" << endl;
		s << "   }" << endl;
	} else {
		s << indent(si.snippet);
	}
	s << "}" << endl;

	return s.str();
}


static const char * const stage_names[] = {
	"vertex",
	"geometry",
	"fragment",
};


void
pipeline_info::dump_shaders() const
{
	assert(ELEMENTS(stage_names) == NUM_SHADER_STAGES);

	for (unsigned i = 0; i < NUM_SHADER_STAGES; i++) {
		if (!is_stage_present((shader_stage_enum) i))
			continue;
		cout << "[" << stage_names[i] << " shader]" << endl;
		cout << get_shader((shader_stage_enum) i);
		cout << endl;
	}
}


void
pipeline_info::add_comparison_logic(shader_stage_enum stage,
				    const variable_set &outputs,
				    const options &opts)
{
	static const char * const green = "vec4(0.0, 1.0, 0.0, 1.0)";
	static const char * const red = "vec4(1.0, 0.0, 0.0, 1.0)";
	type_desc float_type(find_type("float"), 0);
	variable_info tolerance_var
		= make_unique_var(VARIABLE_PROVENANCE_TOLERANCE, float_type,
				  "tolerance");
	variable_set comparison_inputs;
	// TODO: only add tolerance if needed
	comparison_inputs.push_back(tolerance_var);
	type_desc vec4_type(find_type("vec4"), 0);
	variable_info result_var
		= make_unique_var(VARIABLE_PROVENANCE_COLOR, vec4_type,
				  "color");
	variable_set comparison_outputs;
	comparison_outputs.push_back(result_var);
	stringstream s;
	s << result_var.name << " = " << green << ";" << endl;
	for (unsigned i = 0; i < outputs.size(); i++) {
		// TODO: matrices
		// TODO: non-floats
		const variable_info &actual = outputs[i];
		variable_info expected
			= make_unique_var(actual.provenance, actual.type,
					  string("expected_") + actual.name);
		comparison_inputs.push_back(expected);
		if (actual.is_floating()) {
			s << "if (" << tolerance_var.name << " == 0) {"
			  << endl;
			s << "   if (" << actual.name << " != "
			  << expected.name << ")" << endl;
			s << "      " << result_var.name << " = " << red << ";"
			  << endl;
			s << "} else {" << endl;
			s << "   if (distance(" << actual.name << ", "
			  << expected.name << ") > tolerance)" << endl;
			s << "      " << result_var.name << " = " << red << ";"
			  << endl;
			s << "}" << endl;
		} else {
			s << "if (" << actual.name << " != "
			  << expected.name << ")" << endl;
			s << "   " << result_var.name << " = " << red << ";"
			  << endl;
		}
	}
	add_snippet(stage, s.str());
	route_outputs(stage, comparison_outputs, opts.output_delivery);
	route_inputs_or_uniforms(stage, comparison_inputs, opts.input_delivery);
}


void
pipeline_info::copy_data(shader_stage_enum stage, const variable_set &inputs,
			 const variable_set &outputs)
{
	stage_info &si = this->stages[stage];
	si.input_vars.append(inputs);
	si.output_vars.append(outputs);
	stringstream s;
	for (unsigned i = 0; i < inputs.size(); i++) {
		s << outputs[i].name << " = " << inputs[i].name;
		if (stage == SHADER_STAGE_GEOMETRY)
			s << "[" << this->gs_iterator.name << "]";
		s << ";" << endl;
	}
	si.snippet.append(s.str());
}


variable_info
pipeline_info::make_unique_var(variable_provenance_enum provenance,
			       const type_desc &type, const string &hint)
{
	if (this->varnames_used.find(hint) == this->varnames_used.end()) {
		this->varnames_used.insert(hint);
		return variable_info(provenance, type, hint);
	}

	for (unsigned i = 0; ; i++) {
		stringstream s;
		s << hint << i;
		string candidate_name = s.str();
		if (this->varnames_used.find(candidate_name)
		    == this->varnames_used.end()) {
			this->varnames_used.insert(candidate_name);
			return variable_info(provenance, type, candidate_name,
					     hint);
		}
	}
}


variable_set
pipeline_info::generate_parallel_vars(const variable_set &vars)
{
	variable_set new_vars;
	for (unsigned i = 0; i < vars.size(); i++) {
		variable_info info
			= make_unique_var(vars[i].provenance, vars[i].type,
					  vars[i].hint);
		new_vars.push_back(info);
	}
	return new_vars;
}


shader_stage_enum
pipeline_info::last_non_fs_stage() const
{
	unsigned i = SHADER_STAGE_GEOMETRY;
	while (!is_stage_present((shader_stage_enum) i))
		i--;
	return (shader_stage_enum) i;
}


GLuint
pipeline_info::compile() const
{
	// TODO: don't try to run geometry shader tests if unsupported by driver
	static GLenum const stage_targets[] = {
		GL_VERTEX_SHADER,
		GL_GEOMETRY_SHADER,
		GL_FRAGMENT_SHADER,
	};
	assert(ELEMENTS(stage_targets) == NUM_SHADER_STAGES);

	GLuint prog = glCreateProgram();

	GLuint shaders[NUM_SHADER_STAGES];
	for (unsigned i = 0; i < NUM_SHADER_STAGES; i++) {
		if (!is_stage_present((shader_stage_enum) i))
			continue;
		shaders[i] = glCreateShader(stage_targets[i]);
		string shader_string = get_shader((shader_stage_enum) i);
		const char *shader_cstr = shader_string.c_str();
		glShaderSource(shaders[i], 1, &shader_cstr, NULL);
		glCompileShader(shaders[i]);
		GLint ok;
		glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &ok);
		if (!ok) {
			GLchar *info;
			GLint size;
			glGetShaderiv(shaders[i], GL_INFO_LOG_LENGTH, &size);
			info = (GLchar *) malloc(size);
			glGetShaderInfoLog(shaders[i], size, NULL, info);
			cout << "Failed to compile " << stage_names[i]
			     << " shader: " << endl;
			cout << shader_string;
			cout << endl;
			cout << "Error message:" << endl;
			cout << info << endl;
			free(info);
			piglit_report_result(PIGLIT_FAIL);
		}
		glAttachShader(prog, shaders[i]);
	}
	glLinkProgram(prog);
	for (unsigned i = 0; i < NUM_SHADER_STAGES; i++) {
		if (!is_stage_present((shader_stage_enum) i))
			continue;
		glDeleteShader(shaders[i]);
	}
	GLint ok;
	glGetProgramiv(prog, GL_LINK_STATUS, &ok);
	if (!ok) {
		GLchar *info;
		GLint size;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &size);
		info = (GLchar *) malloc(size);
		glGetProgramInfoLog(prog, size, NULL, info);
		cout << "Failed to link shaders:" << endl;
		dump_shaders();
		cout << "Error message:" << endl;
		cout << info << endl;
		free(info);
		piglit_report_result(PIGLIT_FAIL);
	}
	glUseProgram(prog);
	if (!piglit_check_gl_error(GL_NO_ERROR))
	{
		cout << "Error while trying to use shaders:" << endl;
		dump_shaders();
		piglit_report_result(PIGLIT_FAIL);
	}
	return prog;
}


void
pipeline_info::set_uniform_values()
{
	// TODO
	for (unsigned i = 0; i < this->uniform_vars.size(); i++) {
		cout << "Setting value of " << this->uniform_vars[i].name
		     << endl;
	}
}


pipeline_info *
plan_shaders(unsigned version, const options &opts, const test_info &test)
{
	/* Bail out if options are incompatible */
	if (opts.output_action == OUTPUT_ACTION_HYBRID
	    && opts.shader_stage == SHADER_STAGE_FRAGMENT) {
		return NULL;
	}
	if (opts.output_delivery == OUTPUT_DELIVERY_XFB
	    && opts.shader_stage == SHADER_STAGE_FRAGMENT) {
		return NULL;
	}
	if (opts.output_delivery == OUTPUT_DELIVERY_XFB
	    && opts.output_action == OUTPUT_ACTION_HYBRID) {
		return NULL;
	}

	vector<shader_stage_enum> stages;
	stages.push_back(SHADER_STAGE_VERTEX);
	stages.push_back(opts.shader_stage);
	if (opts.output_delivery == OUTPUT_DELIVERY_COLOR)
		stages.push_back(SHADER_STAGE_FRAGMENT);
	pipeline_info *pipeline
		= new pipeline_info(version, stages, test.inputs,
				    test.outputs);

	pipeline->add_snippet(opts.shader_stage, "/* TEST SNIPPET BEGIN */\n");
	pipeline->add_snippet(opts.shader_stage, test.snippet);
	pipeline->add_snippet(opts.shader_stage, "/* TEST SNIPPET END */\n");
	pipeline->route_inputs_or_uniforms(opts.shader_stage, test.inputs,
					   opts.input_delivery);
	switch (opts.output_action) {
	case OUTPUT_ACTION_COMPARE:
		pipeline->add_locals(opts.shader_stage, test.outputs);
		pipeline->add_comparison_logic(opts.shader_stage,
					       test.outputs, opts);
		break;
	case OUTPUT_ACTION_OUT:
		pipeline->route_outputs(opts.shader_stage, test.outputs,
					opts.output_delivery);
		break;
	case OUTPUT_ACTION_HYBRID: {
		variable_set outputs_in_fs
			= pipeline->route_data(opts.shader_stage,
					       SHADER_STAGE_FRAGMENT,
					       test.outputs);
		pipeline->add_comparison_logic(SHADER_STAGE_FRAGMENT,
					       outputs_in_fs, opts);
	}
		break;
	}

	// TODO: fix fragment outputs if version < 130

	// TODO: are there some circumstances where we don't need to set gl_Position?
	if (true) {
		shader_stage_enum stage = pipeline->last_non_fs_stage();
		type_desc vec4_type(find_type("vec4"), 0);
		variable_info position_var
			= pipeline->make_unique_var(VARIABLE_PROVENANCE_POS,
						    vec4_type, "pos");
		stringstream s;
		s << "gl_Position = " << position_var.name << ";" << endl;
		pipeline->add_snippet(stage, s.str());
		variable_set position_inputs;
		position_inputs.push_back(position_var);
		pipeline->route_inputs(stage, position_inputs);
	}

	return pipeline;
}


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


void
piglit_init(int argc, char **argv)
{
	// TODO: choose file on command line
	process_test_script("/home/pberry/tmp/shader_op.txt");
}


piglit_result
run_test(const options &opts, const test_info &test)
{
	// TODO: determine version dynamically
	pipeline_info *pipeline = plan_shaders(130, opts, the_test);
	// pipeline->dump_shaders();

	GLuint prog = pipeline->compile();
	glClearColor(0.5, 0.5, 0.5, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);

	if (opts.input_delivery == INPUT_DELIVERY_UNIFORM) {
		cout << "Running " << test.test_vectors.size()
		     << " test vectors." << endl;
		for (unsigned i = 0; i < test.test_vectors.size(); i++) {
			pipeline->set_uniform_values();
			// TODO: use vbos
			piglit_draw_rect(-1, -1, 2, 2);
		}
	}
	// TODO: do a real test

	glUseProgram(0);
	glDeleteProgram(prog);
	piglit_present_results();
	return PIGLIT_PASS;
}


piglit_result
piglit_display()
{
	// TODO: parse opts
	options opts;
	opts.shader_stage = SHADER_STAGE_VERTEX;
	opts.input_delivery = INPUT_DELIVERY_UNIFORM;
	opts.output_action = OUTPUT_ACTION_HYBRID;
	opts.output_delivery = OUTPUT_DELIVERY_COLOR;
	return run_test(opts, the_test);
}
