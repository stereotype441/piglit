# Copyright 2012 Intel Corporation
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice (including the next
# paragraph) shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

# This script generates a C file (and corresponding header) allowing
# Piglit to dispatch calls to OpenGL based on an XML description of
# the GL api (and extensions).
#
# Invoke this script with 3 command line arguments: the XML input
# filename, the C output filename, and the header outpit filename.
#
#
# The input looks like this:
#
# <OpenGLAPI>
#
#   <!-- A category name is either the name of an extension
#        (e.g. "GL_ARB_vertex_buffer_object") or a GL version (e.g.
#        "1.0").
#        -->
#   <category name="GL_ARB_vertex_buffer_object">
#
#     <!-- Enum names omit the "GL_" prefix -->
#     <enum name="FRONT" value="0x0404"/>
#
#     <!-- Function names omit the "gl" prefix.  So, for example the
#          following defines a function
#
#          GLvoid *glMapBuffer(GLenum target, GLenum access);
#
#          The alias attribute declares that this function is
#          synonymous to another function defined elsewhere.  So the
#          following declaration specifies that "glMapBuffer" is
#          synonymous with "glMapBufferARB".
#
#          Note: a return type of "void" is assumed if there is no
#          <return> element.
#          -->
#     <function name="MapBuffer" alias="MapBufferARB">
#         <param name="target" type="GLenum"/>
#         <param name="access" type="GLenum"/>
#         <return type="GLvoid *"/>
#     </function>
#
#   </category>
#
# </OpenGLAPI>
#
# The input file may refer to other files in the same directory using
# XInclude syntax (http://www.w3.org/TR/xinclude/).
#
#
# The generated header consists of the following:
#
# - A typedef for each function, of the form that would normally
#   appear in gl.h or glext.h, e.g.:
#
#   typedef GLvoid * (*PFNGLMAPBUFFERPROC)(GLenum, GLenum);
#   typedef GLvoid * (*PFNGLMAPBUFFERARBPROC)(GLenum, GLenum);
#
# - A set of extern declarations for "dispatch function pointers".
#   There is one dispatch function pointer for each set of synonymous
#   functions in the GL API, e.g.:
#
#   extern PFNGLMAPBUFFERPROC __piglit_dispatch_glMapBuffer;
#
# - A set of #defines mapping each function name to the corresponding
#   dispatch function pointer, e.g.:
#
#   #define glMapBuffer __piglit_dispatch_glMapBuffer
#   #define glMapBufferARB __piglit_dispatch_glMapBuffer
#
# - A #define for each enum in the GL API, e.g.:
#
#   #define GL_FRONT 0x0404
#
# - A #define for each extension, e.g.:
#
#   #define GL_ARB_vertex_buffer_object
#
#
# The generated C file consists of the following:
#
# - A stub function corresponding to each set of synonymous functions
#   in the GL API.  The stub function first calls
#   __check_initialized().  Then it determines which of the synonymous
#   names the implementation supports (by consulting the current GL
#   version and/or the extension string), and calls either
#   __get_core_proc() or __get_ext_proc() to get the function pointer.
#   It stores the result in the dispatch function pointer, and then
#   calls it.  If the implementation does not support any of the
#   synonymous names, it calls __unsupported().  E.g.:
#
#   /* glMapBuffer (GL 1.5) */
#   /* glMapBufferARB (GL_ARB_vertex_buffer_object) */
#   static GLvoid * stub_glMapBuffer(GLenum target, GLenum access)
#   {
#     __check_initialized();
#     if (__check_version(15))
#       __piglit_dispatch_glMapBuffer = (PFNGLMAPBUFFERPROC) __get_core_proc("glMapBuffer");
#     else if (__check_extension("GL_ARB_vertex_buffer_object"))
#       __piglit_dispatch_glMapBuffer = (PFNGLMAPBUFFERARBPROC) __get_ext_proc("glMapBufferARB");
#     else
#       __unsupported("MapBuffer");
#     return __piglit_dispatch_glMapBuffer(target, access);
#   }
#
# - A declaration for each dispatch function pointer, e.g.:
#
#   PFNGLMAPBUFFERPROC __piglit_dispatch_glMapBuffer = stub_glMapBuffer;
#
# - An initialization function, initialize_dispatch_pointers(), which
#   resets each dispatch pointer to the corresponding stub function.

import collections
import os.path
import sys
import xml.etree.ElementTree
import xml.etree.ElementInclude


# Generate a top-of-file comment cautioning that the file is
# auto-generated and should not be manually edited.
def generated_boilerplate():
    return """\
/**
 * This file was automatically generated by the script {0!r}.
 *
 * DO NOT EDIT!
 *
 * To regenerate, run "make piglit_dispatch_gen" from the toplevel directory.
 */
""".format(os.path.basename(sys.argv[0]))


# Internal representation of a category.
#
# - For a category representing a GL version, Category.typ is 'GL' and
#   Category.data is 10 times the GL version (e.g. 21 for OpenGL
#   version 2.1).
#
# - For a category representing an extension, Category.typ is
#   'extension' and Category.data is the extension name (including the
#   'GL_' prefix).
class Category(collections.namedtuple('Category', 'typ data')):
    # Generate a human-readable representation of the category (for
    # use in generated comments).
    def __str__(self):
	if self.typ == 'GL':
	    return 'GL {0}.{1}'.format(self.data // 10, self.data % 10)
	elif self.typ == 'extension':
	    return self.data
	else:
	    raise Exception(
		'Unexpected category type {0!r}'.format(self.typ))


# Convert a category attribute string (from XML) to a Category object.
def decode_category(category_name):
    try:
	gl_version = float(category_name)
	return Category('GL', int(round(gl_version*10)))
    except ValueError:
	return Category('extension', category_name)


# Internal representation of a function parameter.
#
# - Param.name is the name of the parameter.
#
# - Param.typ is the C type of the parameter.
Param = collections.namedtuple('Param', 'name typ')


# Convert a function parameter from XML to a Param object.
def xml_to_param(param_xml):
    return Param(param_xml.attrib['name'],
		 param_xml.attrib['type'])


# Internal representation of a function signature.
#
# - Signature.rettype is the C type of the function's return value (as
#   a string), or None if the function returns void.
#
# - Signature.params is a tuple of Param objects representing the
#   function's parameters.
class Signature(collections.namedtuple('Signature', 'rettype params')):
    def __new__(self, rettype, params):
	return super(Signature, self).__new__(self, rettype, tuple(params))

    # Generate a string representing the function signature in C.
    #
    # - name is inserted before the opening paren--use '' to generate
    #   an anonymous function type signature.
    #
    # - If anonymous_args is True, then the signature contains only
    #   the types of the arguments, not the names.
    def c_form(self, name, anonymous_args):
	if self.params:
	    if anonymous_args:
		param_decls = ', '.join(p.typ for p in self.params)
	    else:
		param_decls = ', '.join('{0} {1}'.format(p.typ, p.name)
					for p in self.params)
	else:
	    param_decls = 'void'
        return '{rettype} {name}({param_decls})'.format(
	    rettype = self.rettype or 'void', name = name,
	    param_decls = param_decls)


# Internal representation of a GL function.
#
# - Function.name is the name of the function, without the 'gl'
#   prefix.
#
# - Function.sig is the function signature, a Signature object.
#
# - Function.alias is the name of a synonymous function, or None if no
#   alias was declared.
#
# - Function.category is the category in which the function was
#   declared, a Category object.
class Function(collections.namedtuple('Function', 'name sig alias category')):
    # Name of the function, with the 'gl' prefix.
    @property
    def gl_name(self):
	return 'gl' + self.name

    # Name of the function signature typedef corresponding to this
    # function.  E.g. for the glGetString function, this is
    # 'PFNGLGETSTRINGPROC'.
    @property
    def typedef_name(self):
	return 'pfn{0}proc'.format(self.gl_name).upper()


# Convert a function from XML to a Function object.
def xml_to_function(func_xml, category):
    name = func_xml.attrib['name']
    alias = func_xml.get('alias')
    params = []
    rettype = None
    for param in func_xml.findall('param'):
	if param.get('padding') != 'true':
	    params.append(xml_to_param(param))
    ret = func_xml.findall('return')
    if len(ret) > 1:
	raise Exception('Too many <return> elements')
    if len(ret) == 1:
	rettype = ret[0].attrib['type']
    return Function(name, Signature(rettype, params), alias, category)


# Internal representation of an enum.
#
# - Enum.name is the name of the enum, without the 'GL_' prefix.
#
# - Enum.value is the value of the enum, a string in either decimal or
#   hex format.
Enum = collections.namedtuple('Enum', 'name value')


# Convert an enum from XML to an Enum object.
def xml_to_enum(enum_xml):
    return Enum(enum_xml.attrib['name'], enum_xml.attrib['value'])


# Data structure keeping track of which function names are known, and
# which names are synonymous with which other names.
class SynonymMap(object):
    def __init__(self):
	# __name_to_synonyms maps from a function name to the set of
	# all names that are synonymous with it (including itself).
	self.__name_to_synonyms = {}

    # Add a single function name which is not (yet) known to be
    # synonymous with any other name.  No effect if the function name
    # is already known.
    def add_singleton(self, name):
	if name not in self.__name_to_synonyms:
	    self.__name_to_synonyms[name] = set([name])
	return self.__name_to_synonyms[name]

    # Add a pair of function names, and note that they are synonymous.
    # Synonymity is transitive, so if either of the two function names
    # previously had known synonyms, all synonyms are combined into a
    # single set.
    def add_alias(self, name, alias):
	name_ss = self.add_singleton(name)
	alias_ss = self.add_singleton(alias)
	combined_set = name_ss | alias_ss
	for n in combined_set:
	    self.__name_to_synonyms[n] = combined_set

    # Get a python set object representing all the function names that
    # are synonymous to the given name, including the name itself.
    def get_synonyms_for(self, name):
	return self.__name_to_synonyms[name]


# Data structure keeping track of a set of synonymous functions.  Such
# a set is called a "dispatch set" because it corresponds to a single
# dispatch pointer.
class DispatchSet(object):
    # Initialize a dispatch set given a list of Function objects.
    def __init__(self, functions):
	# Sort functions by category, with GL categories preceding
	# extensions.
	self.__functions = tuple(
	    sorted(functions, key = self.__function_category_sort_key))

    # A tuple containing all of the Function objects in this dispatch
    # set.  The functions are sorted so that core GL functions appear
    # before extensions.
    @property
    def functions(self):
	return self.__functions

    # The first Function object in DispatchSet.functions.  This
    # "primary" function is used to name the dispatch pointer and the
    # stub function.
    @property
    def primary_function(self):
	return self.__functions[0]

    # The name of the dispatch pointer that should be generated for
    # this dispatch set.
    @property
    def dispatch_name(self):
	return '__piglit_dispatch_' + self.primary_function.gl_name

    # The name of the stub function that should be generated for this
    # dispatch set.
    @property
    def stub_name(self):
	return 'stub_' + self.primary_function.gl_name

    @staticmethod
    def __function_category_sort_key(fn):
	if fn.category.typ == 'GL':
	    return 0, fn.category.data
	elif fn.category.typ == 'extension':
	    return 1, fn.category.data
	else:
	    raise Exception(
		'Unexpected category type {0!r}'.format(category.typ))


# Data structure keeping track of all of the known functions and
# enums, and synonym relationships that exist between the functions.
class Api(object):
    def __init__(self):
	self.__functions = []
	self.__enums = []
	self.__synonyms = SynonymMap()
	self.__function_map = collections.defaultdict(list)
	self.__extensions = set()

    # A list of Enum objects representing all the enums in the API.
    @property
    def enums(self):
	return self.__enums

    # A SynonymMap object recording which function names are
    # synonymous with which other names.
    @property
    def synonyms(self):
	return self.__synonyms

    # An unordered set of all of the extension names declared in the
    # API, as Python strings.
    @property
    def extensions(self):
	return self.__extensions

    # Add a single Function object to the API, updating synonyms
    # appropriately.
    def add_function(self, fn):
	self.__functions.append(fn)
	self.__function_map[fn.name].append(fn)
	if fn.alias:
	    self.__synonyms.add_alias(fn.name, fn.alias)
	else:
	    self.__synonyms.add_singleton(fn.name)

    # Generate a list of DispatchSet objects representing all sets of
    # synonymous functions in the API.  The resulting list is sorted
    # by DispatchSet.stub_name.
    def compute_dispatch_sets(self):
	sets = []
	remaining_names = set(f.name for f in self.__functions)
	while remaining_names:
	    name = remaining_names.pop()
	    synonym_set = self.__synonyms.get_synonyms_for(name)
	    functions = []
	    for name in synonym_set:
		for function in self.__function_map[name]:
		    functions.append(function)
	    sets.append(DispatchSet(functions))
	    for n in synonym_set:
		remaining_names.discard(n)
	sets.sort(key = lambda ds: ds.stub_name)

	return sets

    # Generate a list of Function objects representing all functions
    # in the API.  The resulting list is sorted by function name.
    def compute_unique_functions(self):
	functions = []
	names_used = set()
	for f in self.__functions:
	    if f.name in names_used:
		continue
	    functions.append(f)
	functions.sort(key = lambda f: f.name)
	return functions


# Read the given input file and return an Api object containing the
# data in it.
def read_xml(filename):
    api = Api()
    os.chdir(os.path.dirname(filename))
    root = xml.etree.ElementTree.parse(filename).getroot()
    xml.etree.ElementInclude.include(root)

    for category_xml in root.findall('.//category'):
	category = decode_category(category_xml.attrib['name'])
	if category.typ == 'extension':
	    api.extensions.add(category.data)
	for func in category_xml.findall('function'):
	    api.add_function(xml_to_function(func, category))
	for enum in category_xml.findall('enum'):
	    api.enums.append(xml_to_enum(enum))

    return api


# Generate the stub function for a given DispatchSet.
def generate_stub_function(ds):
    f0 = ds.primary_function

    # First figure out all the conditions we want to check in order to
    # figure out which function to dispatch to, and the code we will
    # execute in each case.
    condition_code_pairs = []
    for f in ds.functions:
	if f.category.typ == 'GL':
	    getter = '__get_core_proc'
	    if f.category.data == 10:
		# Function has always been available--no need to check
		# a condition.
		condition = 'true'
	    else:
		condition = '__check_version({0})'.format(
		    f.category.data)
	elif f.category.typ == 'extension':
	    getter = '__get_ext_proc'
	    condition = '__check_extension("{0}")'.format(
		f.category.data)
	else:
	    raise Exception(
		'Unexpected category type {0!r}'.format(f.category.type))

	if f.name == 'TexImage3DEXT':
	    # Special case: glTexImage3DEXT has a slightly different
	    # type than glTexImage3D (argument 3 is a GLenum rather
	    # than a GLint).  This is not a problem, since GLenum and
	    # GLint are treated identically by function calling
	    # conventions.  So when calling get_proc_address() on
	    # glTexImage3DEXT, cast the result to PFNGLTEXIMAGE3DPROC
	    # to avoid a warning.
	    typedef_name = 'PFNGLTEXIMAGE3DPROC'
	else:
	    typedef_name = f.typedef_name

	code = '{0} = ({1}) {2}("{3}");'.format(
	    ds.dispatch_name, typedef_name, getter, f.gl_name)

	condition_code_pairs.append((condition, code))

    # Finally, if none of the previous conditions were satisfied, then
    # the given dispatch set is not supported by the implementation,
    # so we want to call the __unsupported() function.
    condition_code_pairs.append(
	('true', '__unsupported("{0}");'.format(f0.name)))

    # Start the stub function
    stub_fn = 'static {0}\n'.format(
	f0.sig.c_form(ds.stub_name, anonymous_args = False))
    stub_fn += '{\n'
    stub_fn += '\t__check_initialized();\n'

    # Output code that checks each condition in turn and executes the
    # appropriate case.  To make the generated code more palatable
    # (and to avoid compiler warnings), we convert "if (true) FOO;" to
    # "FOO;" and "else if (true) FOO;" to "else FOO;".
    if condition_code_pairs[0][0] == 'true':
	stub_fn += '\t{0}\n'.format(condition_code_pairs[0][1])
    else:
	stub_fn += '\tif ({0})\n\t\t{1}\n'.format(*condition_code_pairs[0])
	for i in xrange(1, len(condition_code_pairs)):
	    if condition_code_pairs[i][0] == 'true':
		stub_fn += '\telse\n\t\t{0}\n'.format(
		    condition_code_pairs[i][1])
		break
	    else:
		stub_fn += '\telse if ({0})\n\t\t{1}\n'.format(
		    *condition_code_pairs[i])

    # Output the call to the dispatch function.
    stub_fn += '\t{0}{1}({2});\n'.format(
	'return ' if f0.sig.rettype else '',
	ds.dispatch_name,
	', '.join(p.name for p in f0.sig.params))
    stub_fn += '}\n'
    return stub_fn


# Generate the initialize_dispatch_pointers() function, which sets
# each dispatch pointer to point to the corresponding stub function.
def generate_dispatch_pointer_initializer(dispatch_sets):
    result = []
    result.append('static void\n')
    result.append('initialize_dispatch_pointers()\n')
    result.append('{\n')
    for ds in dispatch_sets:
	result.append(
	    '\t{0} = {1};\n'.format(ds.dispatch_name, ds.stub_name))
    result.append('}\n')
    return ''.join(result)


# Generate the C source and header files for the API.
def generate_code(api):
    c_contents = [generated_boilerplate()]
    h_contents = [generated_boilerplate()]

    unique_functions = api.compute_unique_functions()

    # Emit typedefs for each name
    for f in unique_functions:
	h_contents.append(
	    'typedef {0};\n'.format(
		f.sig.c_form('(*{0})'.format(f.typedef_name),
			     anonymous_args = True)))

    dispatch_sets = api.compute_dispatch_sets()

    for ds in dispatch_sets:
	f0 = ds.primary_function

	# Emit comment block
	comments = '\n'
	for f in ds.functions:
	    comments += '/* {0} ({1}) */\n'.format(f.gl_name, f.category)
	c_contents.append(comments)
	h_contents.append(comments)

	# Emit extern declaration of dispatch pointer
	h_contents.append(
	    'extern {0} {1};\n'.format(f0.typedef_name, ds.dispatch_name))

	# Emit defines aliasing each GL function to the dispatch
	# pointer
	for f in ds.functions:
	    h_contents.append(
		'#define {0} {1}\n'.format(f.gl_name, ds.dispatch_name))

	# Emit stub function
	c_contents.append(generate_stub_function(ds))

	# Emit initializer for dispatch pointer
	c_contents.append(
	    '{0} {1} = {2};\n'.format(
		f0.typedef_name, ds.dispatch_name, ds.stub_name))

    # Emit dispatch pointer initialization function
    c_contents.append(generate_dispatch_pointer_initializer(dispatch_sets))

    # Emit enum #defines
    for en in api.enums:
	h_contents.append('#define GL_{s.name} {s.value}\n'.format(s = en))

    # Emit extension #defines
    h_contents.append('\n')
    for ext in sorted(api.extensions):
	h_contents.append('#define {0}\n'.format(ext))

    return ''.join(c_contents), ''.join(h_contents)


if __name__ == '__main__':
    file_to_parse = sys.argv[1]
    api = read_xml(file_to_parse)

    c_contents, h_contents = generate_code(api)
    with open(sys.argv[2], 'w') as f:
	f.write(c_contents)
    with open(sys.argv[3], 'w') as f:
	f.write(h_contents)
