/*
 * Copyright © 2011 Intel Corporation
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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#if defined(_MSC_VER)
#define bool BOOL
#define true 1
#define false 0
#else
#include <stdbool.h>
#endif
#include <string.h>
#include <ctype.h>
#if defined(_WIN32)
#include <stdlib.h>
#else
#include <libgen.h>
#endif
#include "piglit-util.h"
#include "vbo_tools.h"
#include <vector>
#include <string>
#include <errno.h>

enum SpecialVertexAttribLocs
{
	VERTEX_ATTRIB_VERTEX = -1,
	VERTEX_ATTRIB_NORMAL = -2,
	VERTEX_ATTRIB_COLOR = -3,
	VERTEX_ATTRIB_SECONDARY_COLOR = -4,
	VERTEX_ATTRIB_INDEX = -5,
	VERTEX_ATTRIB_EDGE_FLAG = -6,
	VERTEX_ATTRIB_FOG_COORD = -7,
	VERTEX_ATTRIB_TEX_COORD = -8,
};


/* Fake GLenum value to represent the boolean type in the table below. */
const GLenum GL_BOOLEAN = 0;


struct type_table_entry {
	bool is_allowed() const;

	const char *type; /* NULL means end of table */
	GLenum enum_value;
	bool is_floating;
	bool is_signed; /* Only if not is_floating */
	int min_value; /* Only if (not is_floating) and is_signed */
	unsigned int max_value; /* Only if is_integral */
	size_t size;
} const type_table[] = {
	/* type      enum_value         is_floating is_signed min-value    max_value   size */
	{ "boolean", GL_BOOLEAN,        false,      false,    0,           1,          sizeof(GLboolean) },
	{ "byte",    GL_BYTE,           false,      true,     -0x80,       0x7f,       sizeof(GLbyte)    },
	{ "ubyte",   GL_UNSIGNED_BYTE,  false,      false,    0,           0xff,       sizeof(GLubyte)   },
	{ "short",   GL_SHORT,          false,      true,     -0x8000,     0x7fff,     sizeof(GLshort)   },
	{ "ushort",  GL_UNSIGNED_SHORT, false,      false,    0,           0xffff,     sizeof(GLushort)  },
	{ "int",     GL_INT,            false,      true,     -0x80000000, 0x7fffffff, sizeof(GLint)     },
	{ "uint",    GL_UNSIGNED_INT,   false,      false,    0,           0xffffffff, sizeof(GLuint)    },
	{ "float",   GL_FLOAT,          true,       false,    0,           0,          sizeof(GLfloat)   },
	{ "half",    GL_HALF_FLOAT,     true,       false,    0,           0,          sizeof(GLhalf)    },
	{ "double",  GL_DOUBLE,         true,       false,    0,           0,          sizeof(GLdouble)  },
	{ NULL,      0,                 true,       false,    0,           0,          0                 }
};


bool type_table_entry::is_allowed() const
{
	if (this->enum_value == GL_BOOLEAN) {
		/* boolean only allowed for edge flag */
		return false; /* TODO */
	}
	/* TODO: handle other attrib locs */
	return (this->is_floating || (this->is_signed && this->size >= sizeof(GLshort)));
}


static const type_table_entry *
decode_type(const char *type)
{
	for (int i = 0; type_table[i].type; ++i) {
		if (0 == strcmp(type, type_table[i].type))
			return &type_table[i];
	}

	printf("Unrecognized type: %s\n", type);
	piglit_report_result(PIGLIT_FAIL);
	return NULL;
}


class vertex_attrib_description
{
public:
	vertex_attrib_description(const char *text);
	bool parse_datum(const char **text, void *data) const;
	void setup(size_t *offset, size_t stride) const;

	const type_table_entry *data_type;
	size_t count;

private:
	GLint attrib_loc;
};


vertex_attrib_description::vertex_attrib_description(const char *text)
{
	/* Split the column header into name/type/count fields */
	const char *first_slash = strchr(text, '/');
	if (first_slash == NULL) {
		printf("Column headers must be in the form name/type/count.  Got: %s\n",
		       text);
		piglit_report_result(PIGLIT_FAIL);
	}
	std::string name(text, first_slash);
	const char *second_slash = strchr(first_slash + 1, '/');
	if (second_slash == NULL) {
		printf("Column headers must be in the form name/type/count.  Got: %s\n",
		       text);
		piglit_report_result(PIGLIT_FAIL);
	}
	std::string type_str(first_slash + 1, second_slash);
	this->data_type = decode_type(type_str.c_str());
	char *endptr;
	this->count = strtoul(second_slash + 1, &endptr, 10);
	if (*endptr != '\0') {
		printf("Column headers must be in the form name/type/count.  Got: %s\n",
		       text);
		piglit_report_result(PIGLIT_FAIL);
	}

	if (name != "gl_Vertex") {
		/* TODO */
		printf("Unexpected vbo column name.  Got: %s\n", name.c_str());
		piglit_report_result(PIGLIT_FAIL);
	}
	this->attrib_loc = VERTEX_ATTRIB_VERTEX;
	if (this->count < 2 || this->count > 4) {
		printf("Count must be between 2 and 4.  Got: %lu\n", count);
		piglit_report_result(PIGLIT_FAIL);
	}

	if (!this->data_type->is_allowed()) {
		printf("Type %s not allowed for %s\n", type_str.c_str(),
		       name.c_str());
		piglit_report_result(PIGLIT_FAIL);
	}
}


bool
vertex_attrib_description::parse_datum(const char **text, void *data) const
{
	char *endptr;
	errno = 0;
	if (this->data_type->is_floating) {
		double value = strtod(*text, &endptr);
		if (errno == ERANGE) {
			printf("Could not parse as double\n");
			return false;
		}
		switch (this->data_type->enum_value) {
		case GL_DOUBLE:
			*((GLdouble *) data) = value;
			break;
		case GL_FLOAT:
			*((GLfloat *) data) = (float) value;
			break;
		case GL_HALF_FLOAT:
			*((GLushort *) data) = piglit_float_to_half((float) value);
			break;
		}
	} else if (this->data_type->is_signed) {
		long value = strtol(*text, &endptr, 0);
		if (errno == ERANGE) {
			printf("Could not parse as signed integer\n");
			return false;
		}
		if (value > (long) this->data_type->max_value) {
			printf("Value too large.  Must be %u or less\n",
			       this->data_type->max_value);
			return false;
		}
		if (value < this->data_type->min_value) {
			printf("Value too small.  Must be %d or greater\n",
			       this->data_type->min_value);
			return false;
		}
		switch (this->data_type->enum_value) {
		case GL_BYTE:
			*((GLbyte *) data) = (GLbyte) value;
			break;
		case GL_SHORT:
			*((GLshort *) data) = (GLshort) value;
			break;
		case GL_INT:
			*((GLint *) data) = (GLint) value;
			break;
		}
	} else {
		unsigned long value = strtoul(*text, &endptr, 0);
		if (errno == ERANGE) {
			printf("Could not parse as unsigned integer\n");
			return false;
		}
		if (value > this->data_type->max_value) {
			printf("Value too large.  Must be %u or less\n",
			       this->data_type->max_value);
			return false;
		}
		switch (this->data_type->enum_value) {
		case GL_BOOLEAN:
			*((GLboolean *) data) = (GLboolean) value;
			break;
		case GL_UNSIGNED_BYTE:
			*((GLubyte *) data) = (GLubyte) value;
			break;
		case GL_UNSIGNED_SHORT:
			*((GLushort *) data) = (GLushort) value;
			break;
		case GL_UNSIGNED_INT:
			*((GLuint *) data) = (GLuint) value;
			break;
		}
	}
	*text = endptr;
	return true;
}


void
vertex_attrib_description::setup(size_t *offset, size_t stride) const
{
	/* TODO: Require appropriate version of GL */
	/* TODO: Deal with multiple function names */
	assert (this->attrib_loc == VERTEX_ATTRIB_VERTEX); /* TODO */
	glVertexPointer(this->count, this->data_type->enum_value, stride, (void *) *offset);
	glEnableClientState(GL_VERTEX_ARRAY);
	*offset += this->data_type->size;
}


class vbo_data
{
public:
	vbo_data(std::string const &text);
	size_t setup() const;

private:
	void parse_header_line(const std::string &line);
	void parse_data_line(const std::string &line, unsigned int line_num);
	void parse_line(std::string line, unsigned int line_num);

	bool header_seen;
	std::vector<vertex_attrib_description> attribs;
	std::vector<char> raw_data;
	size_t stride;
	size_t num_rows;
};



static bool
is_blank_line(const std::string &line)
{
	for (size_t i = 0; i < line.size(); ++i) {
		if (!isspace(line[i]))
			return false;
	}
	return true;
}


void
vbo_data::parse_header_line(const std::string &line)
{
	size_t pos = 0;
	while (pos < line.size()) {
		if (isspace(line[pos])) {
			++pos;
		} else {
			size_t column_header_end = pos;
			while (column_header_end < line.size() &&
			       !isspace(line[column_header_end]))
				++column_header_end;
			std::string column_header = line.substr(pos, column_header_end);
			vertex_attrib_description desc(column_header.c_str());
			attribs.push_back(desc);
			this->stride += desc.data_type->size * desc.count;
			pos = column_header_end + 1;
		}
	}
}


void
vbo_data::parse_data_line(const std::string &line, unsigned int line_num)
{
	/* Allocate space in raw_data for this line */
	size_t old_size = this->raw_data.size();
	this->raw_data.resize(old_size + this->stride);
	char *data_ptr = &this->raw_data[old_size];

	const char *line_ptr = line.c_str();
	for (size_t i = 0; i < this->attribs.size(); ++i) {
		for (size_t j = 0; j < this->attribs[i].count; ++j) {
			if (!this->attribs[i].parse_datum(&line_ptr, data_ptr)) {
				printf("At line %u of [vertex data] section\n", line_num);
				printf("Offending text: %s\n", line_ptr);
				piglit_report_result(PIGLIT_FAIL);
			}
			data_ptr += this->attribs[i].data_type->size;
		}
	}

	++this->num_rows;
}


void
vbo_data::parse_line(std::string line, unsigned int line_num)
{
	/* Ignore end-of-line comments */
	line = line.substr(0, line.find('#'));

	/* Ignore blank or comment-only lines */
	if (is_blank_line(line))
		return;

	if (!this->header_seen) {
		this->header_seen = true;
		parse_header_line(line);
	} else {
		parse_data_line(line, line_num);
	}
}


vbo_data::vbo_data(const std::string &text)
	: header_seen(false), stride(0), num_rows(0)
{
	unsigned int line_num = 1;

	size_t pos = 0;
	while (pos < text.size()) {
		size_t end_of_line = text.find('\n', pos);
		if (end_of_line == std::string::npos)
			end_of_line = text.size();
		parse_line(text.substr(pos, end_of_line), line_num++);
		pos = end_of_line + 1;
	}
}


size_t
vbo_data::setup() const
{
	GLuint buffer_handle;
	glGenBuffers(1, &buffer_handle);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_handle);
	glBufferData(GL_ARRAY_BUFFER, this->stride * this->num_rows, &this->raw_data[0], GL_STATIC_DRAW);

	size_t offset = 0;
	for (size_t i = 0; i < attribs.size(); ++i)
		attribs[i].setup(&offset, this->stride);

	/* Leave buffer bound for later draw calls */

	return this->num_rows;
}


size_t setup_vbos_from_text(const char *text_start, const char *text_end)
{
	std::string text(text_start, text_end);
	return vbo_data(text).setup();
}
