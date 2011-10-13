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

enum allowed_types {
	ALLOW_NONE    =     0,
	ALLOW_BOOLEAN =   0x1,
	ALLOW_BYTE    =   0x2,
	ALLOW_UBYTE   =  0x04,
	ALLOW_SHORT   =  0x08,
	ALLOW_USHORT  =  0x10,
	ALLOW_INT     =  0x20,
	ALLOW_UINT    =  0x40,
	ALLOW_FLOAT   =  0x80,
	ALLOW_HALF    = 0x100,
	ALLOW_DOUBLE  = 0x200,

	ALLOW_VERTEX_POINTER = ALLOW_SHORT | ALLOW_INT | ALLOW_FLOAT
	                     | ALLOW_HALF | ALLOW_DOUBLE,
	ALLOW_NORMAL_POINTER = ALLOW_BYTE | ALLOW_SHORT | ALLOW_INT
	                     | ALLOW_FLOAT | ALLOW_HALF | ALLOW_DOUBLE,
	ALLOW_COLOR_POINTER = ALLOW_BYTE | ALLOW_UBYTE | ALLOW_SHORT
	                    | ALLOW_USHORT | ALLOW_INT | ALLOW_UINT
	                    | ALLOW_FLOAT | ALLOW_HALF | ALLOW_DOUBLE,
	ALLOW_INDEX_POINTER = ALLOW_UBYTE | ALLOW_SHORT | ALLOW_INT
	                    | ALLOW_FLOAT | ALLOW_DOUBLE,
	ALLOW_FOG_COORD_POINTER = ALLOW_FLOAT | ALLOW_HALF | ALLOW_DOUBLE,
	ALLOW_TEX_COORD_POINTER = ALLOW_SHORT | ALLOW_INT | ALLOW_FLOAT
	                        | ALLOW_HALF | ALLOW_DOUBLE,
	ALLOW_EDGE_FLAG_POINTER = ALLOW_BOOLEAN,
	ALLOW_VERTEX_ATTRIB_POINTER = ALLOW_BYTE | ALLOW_UBYTE | ALLOW_SHORT
	                            | ALLOW_USHORT | ALLOW_INT | ALLOW_UINT
	                            | ALLOW_FLOAT | ALLOW_HALF | ALLOW_DOUBLE,
	ALLOW_VERTEX_ATTRIB_I_POINTER = ALLOW_BYTE | ALLOW_UBYTE | ALLOW_SHORT
	                              | ALLOW_USHORT | ALLOW_INT | ALLOW_UINT,
};

typedef void set_pointer_func(int count, GLenum type, size_t stride, void *pointer);

void set_vertex_pointer(int count, GLenum type, size_t stride, void *pointer)
{
	glVertexPointer(count, type, stride, pointer);
	glEnableClientState(GL_VERTEX_ARRAY);
}

void set_normal_pointer(int count, GLenum type, size_t stride, void *pointer)
{
	(void) count;
	glNormalPointer(type, stride, pointer);
	glEnableClientState(GL_NORMAL_ARRAY);
}

void set_color_pointer(int count, GLenum type, size_t stride, void *pointer)
{
	glColorPointer(count, type, stride, pointer);
	glEnableClientState(GL_COLOR_ARRAY);
}

struct attrib_type_table_entry {
	const char *name; /* NULL means end of table */
	set_pointer_func *setter;
	size_t min_count;
	size_t max_count;
	allowed_types allow_flags;
} const attrib_type_table[] = {
	/* name        setter              min_count max_count allow_flags */
	{ "gl_Vertex", set_vertex_pointer, 2,        4,        ALLOW_VERTEX_POINTER },
	{ "gl_Normal", set_normal_pointer, 3,        3,        ALLOW_NORMAL_POINTER },
	{ "gl_Color",  set_color_pointer,  3,        4,        ALLOW_COLOR_POINTER  },
	/* TODO: add more */
	{ NULL,        NULL,               0,        0,        ALLOW_NONE           }
};


/* Fake GLenum value to represent the boolean type in the table below. */
const GLenum GL_BOOLEAN = 0;


struct type_table_entry {
	const char *type; /* NULL means end of table */
	allowed_types allow_flag;
	GLenum enum_value;
	bool is_floating;
	bool is_signed; /* Only if not is_floating */
	int min_value; /* Only if (not is_floating) and is_signed */
	unsigned int max_value; /* Only if is_integral */
	size_t size;
} const type_table[] = {
	/* type      allow_flag     enum_value         is_floating is_signed min-value    max_value   size */
	{ "boolean", ALLOW_BOOLEAN, GL_BOOLEAN,        false,      false,    0,           1,          sizeof(GLboolean) },
	{ "byte",    ALLOW_BYTE,    GL_BYTE,           false,      true,     -0x80,       0x7f,       sizeof(GLbyte)    },
	{ "ubyte",   ALLOW_UBYTE,   GL_UNSIGNED_BYTE,  false,      false,    0,           0xff,       sizeof(GLubyte)   },
	{ "short",   ALLOW_SHORT,   GL_SHORT,          false,      true,     -0x8000,     0x7fff,     sizeof(GLshort)   },
	{ "ushort",  ALLOW_USHORT,  GL_UNSIGNED_SHORT, false,      false,    0,           0xffff,     sizeof(GLushort)  },
	{ "int",     ALLOW_INT,     GL_INT,            false,      true,     -0x80000000, 0x7fffffff, sizeof(GLint)     },
	{ "uint",    ALLOW_UINT,    GL_UNSIGNED_INT,   false,      false,    0,           0xffffffff, sizeof(GLuint)    },
	{ "float",   ALLOW_FLOAT,   GL_FLOAT,          true,       false,    0,           0,          sizeof(GLfloat)   },
	{ "half",    ALLOW_HALF,    GL_HALF_FLOAT,     true,       false,    0,           0,          sizeof(GLhalf)    },
	{ "double",  ALLOW_DOUBLE,  GL_DOUBLE,         true,       false,    0,           0,          sizeof(GLdouble)  },
	{ NULL,      ALLOW_NONE,    0,                 true,       false,    0,           0,          0                 }
};


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
	const attrib_type_table_entry *attrib_type;
};


static const attrib_type_table_entry *
determine_attrib_type(const std::string &name)
{
	for (int i = 0; attrib_type_table[i].name; ++i) {
		if (name == attrib_type_table[i].name)
			return &attrib_type_table[i];
	}

	printf("Unexpected vbo column name.  Got: %s\n", name.c_str());
	piglit_report_result(PIGLIT_FAIL);

	/* Should not be reached, but return NULL to avoid compiler warning */
	return NULL;
}


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

	this->attrib_type = determine_attrib_type(name);
	if (this->count < this->attrib_type->min_count ||
	    this->count > this->attrib_type->max_count) {
		printf("Count must be between %lu and %lu.  Got: %lu\n",
		       this->attrib_type->min_count, this->attrib_type->max_count, count);
		piglit_report_result(PIGLIT_FAIL);
	}

	if (!(this->data_type->allow_flag & this->attrib_type->allow_flags)) {
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
	int count = this->count;
	GLenum type = this->data_type->enum_value;
	void *pointer = (void *) *offset;
	this->attrib_type->setter(count, type, stride, pointer);
	*offset += this->data_type->size * this->count;
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
