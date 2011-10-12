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
	VERTEX_ATTRIB_POSITION = -1,
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
	const char *type; /* NULL means end of table */
	GLenum enum_value;
	bool is_floating;
	bool is_signed; /* Only if not is_floating */
	int min_value; /* Only if (not is_floating) and is_signed */
	unsigned int max_value; /* Only if is_integral */
	size_t size;
} type_table[] = {
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


static type_table_entry *
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

	type_table_entry *data_type;
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

	if (name != "gl_Position") {
		/* TODO */
		printf("Unexpected name.  Got: %s\n", name.c_str());
		piglit_report_result(PIGLIT_FAIL);
	}
	this->attrib_loc = VERTEX_ATTRIB_POSITION;
	if (this->count < 2 || this->count > 4) {
		printf("Count must be between 2 and 4.  Got: %lu\n", count);
		piglit_report_result(PIGLIT_FAIL);
	}
}


GLushort float_to_half(float f); /* TODO */


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
			*((GLushort *) data) = float_to_half((float) value);
			break;
		}
	} else if (this->data_type->is_signed) {
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
	} else {
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
	}
	*text = endptr;
	return true;
}


class vbo_data
{
public:
	vbo_data(std::string const &text);
	void setup() const;

private:
	void parse_header_line(const std::string &line);
	void parse_data_line(const std::string &line, unsigned int line_num);
	void parse_line(std::string line, unsigned int line_num);

	bool header_seen;
	std::vector<vertex_attrib_description> attribs;
	std::vector<char> raw_data;
	size_t row_size;
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
			this->row_size += desc.data_type->size * desc.count;
			pos = column_header_end + 1;
		}
	}
}


void
vbo_data::parse_data_line(const std::string &line, unsigned int line_num)
{
	/* Allocate space in raw_data for this line */
	size_t old_size = this->raw_data.size();
	this->raw_data.resize(old_size + this->row_size);
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
	: header_seen(false), row_size(0)
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

void setup_vbos_from_text(const char *text_start, const char *text_end)
{
	std::string text(text_start, text_end);
	vbo_data(text).setup();
}
