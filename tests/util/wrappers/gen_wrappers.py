import collections
import os.path
import sys
import xml.dom
import xml.dom.minidom

def child_elements(node):
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            yield child

class UnexpectedElement(Exception):
    def __init__(self, elem, context = None):
        if context is None:
            if elem.parentNode.nodeType == xml.dom.Node.DOCUMENT_NODE:
                context = 'at top level'
            else:
                context = 'inside element "{0}"'.format(
                    elem.parentNode.tagName)
        Exception.__init__(self, 'Unexpected element "{0}" {1}'.format(
                elem.tagName, context))

class Param(object):
    def __init__(self, name, typ):
	self.__name = name
	self.__type = typ

    @property
    def name(self):
        return self.__name

    @property
    def typ(self):
        return self.__type

    @property
    def decl(self):
        return '{0} {1}'.format(self.__type, self.__name)

def xml_to_param(param_xml):
    return Param(param_xml.getAttribute('name'),
		 param_xml.getAttribute('type'))

class Function(object):
    def __init__(self, rettype, name, params):
	self.__rettype = rettype
	self.__name = name
	self.__params = tuple(params)

    @property
    def rettype(self):
        return self.__rettype or 'void'

    @property
    def opt_return(self):
        return 'return ' if self.__rettype else ''

    @property
    def name(self):
        return self.__name

    @property
    def param_decls(self):
	if self.__params:
	    return ', '.join(p.decl for p in self.__params)
	else:
	    return 'void'

    @property
    def param_names(self):
        return ', '.join(p.name for p in self.__params)

    @property
    def param_types(self):
	if self.__params:
	    return ', '.join(p.typ for p in self.__params)
	else:
	    return 'void'

    @property
    def wrapper_function_sig(self):
        return '{s.rettype} gl{s.name}({s.param_decls})'.format(s = self)

    @property
    def glew_typedef_name(self):
	return 'pfngl{s.name}proc'.format(s = self).upper()

def xml_to_function(func_xml):
    name = func_xml.getAttribute('name')
    params = []
    rettype = None
    for item in child_elements(func_xml):
	if item.tagName == 'param':
	    if item.getAttribute('padding') != 'true':
		params.append(xml_to_param(item))
	elif item.tagName == 'return':
	    rettype = item.getAttribute('type')
	elif item.tagName == 'glx':
	    # TODO: don't know what to do with this yet
	    pass
	else:
	    raise UnexpectedElement(item)
    return Function(rettype, name, params)


class Enum(object):
    def __init__(self, name, value):
	self.__name = name
	self.__value = value

    @property
    def name(self):
	return self.__name

    @property
    def value(self):
	return self.__value

def xml_to_enum(enum_xml):
    return Enum(enum_xml.getAttribute('name'), enum_xml.getAttribute('value'))


class Api(object):
    def __init__(self):
	self.__functions = []
	self.__enums = []

    @property
    def functions(self):
	return self.__functions

    @property
    def enums(self):
	return self.__enums

    def traverse(self, filename):
        doc = xml.dom.minidom.parse(filename)

        if doc.documentElement.tagName != 'OpenGLAPI':
            raise UnexpectedElement(doc.documentElement)

        # TODO: category is a bad name.
        for category in child_elements(doc.documentElement):
            if category.tagName == 'xi:include':
                self.traverse(os.path.join(os.path.dirname(filename), category.getAttribute('href')))
                continue
            if category.tagName != 'category':
                raise UnexpectedElement(category)
            for item in child_elements(category):
                if item.tagName == 'function':
                    self.__functions.append(xml_to_function(item))
                elif item.tagName == 'enum':
		    self.__enums.append(xml_to_enum(item))
                elif item.tagName == 'type':
                    # TODO: handle this.
                    pass
                else:
                    raise UnexpectedElement(item)

    def generate_c_file_contents(self):
	contents = []
	for fn in self.functions:
	    contents.append("""\
{s.wrapper_function_sig}
{{
\tstatic {s.rettype} (*function_pointer)({s.param_types}) = NULL;

\tif (function_pointer == NULL) {{
\t\tfunction_pointer = ({s.rettype} (*)({s.param_types}))
\t\t\tglGetProcAddress((const GLubyte *) "gl{s.name}");
\t\tif (function_pointer == NULL) {{
\t\t\tprintf("Implementation does not support function \\"{s.name}\\"\\n");
\t\t\tpiglit_report_result(PIGLIT_FAIL);
\t\t}}
\t}}

\t{s.opt_return}function_pointer({s.param_names});
}}
""".format(s = fn))
	return ''.join(contents)

    def generate_h_file_contents(self):
	contents = []
	for fn in self.functions:
	    contents.append("""\
typedef {s.rettype} (*{s.glew_typedef_name})({s.param_types});
{s.wrapper_function_sig};
""".format(s = fn))
	for en in self.enums:
	    contents.append("""\
#define GL_{s.name} {s.value}
""".format(s = en))
	return ''.join(contents)



file_to_parse = sys.argv[1]

api = Api()
api.traverse(file_to_parse)

with open(sys.argv[2], 'w') as f:
    f.write(api.generate_c_file_contents())
with open(sys.argv[3], 'w') as f:
    f.write(api.generate_h_file_contents())
