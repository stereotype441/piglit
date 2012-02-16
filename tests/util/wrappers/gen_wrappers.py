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

Param = collections.namedtuple('Param', 'name typ')

def xml_to_param(param_xml):
    return Param(param_xml.getAttribute('name'),
		 param_xml.getAttribute('type'))

class Signature(collections.namedtuple('Signature', 'rettype params')):
    def __init__(self, rettype, params):
	super(Signature, self).__init__(rettype, tuple(params))

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


class Function(object):
    def __init__(self, name, sig):
	self.__name = name
	self.__sig = sig

    @property
    def sig(self):
	return self.__sig

    @property
    def name(self):
        return self.__name

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
    return Function(name, Signature(rettype, params))


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

def read_xml(filename, api):
    doc = xml.dom.minidom.parse(filename)

    if doc.documentElement.tagName != 'OpenGLAPI':
	raise UnexpectedElement(doc.documentElement)

    # TODO: category is a bad name.
    for category in child_elements(doc.documentElement):
	if category.tagName == 'xi:include':
	    read_xml(os.path.join(os.path.dirname(filename), category.getAttribute('href')), api)
	    continue
	if category.tagName != 'category':
	    raise UnexpectedElement(category)
	for item in child_elements(category):
	    if item.tagName == 'function':
		api.functions.append(xml_to_function(item))
	    elif item.tagName == 'enum':
		api.enums.append(xml_to_enum(item))
	    elif item.tagName == 'type':
		# TODO: handle this.
		pass
	    else:
		raise UnexpectedElement(item)

def generate_code(api):
    c_contents = []
    h_contents = []
    for fn in api.functions:
	gl_name = 'gl' + fn.name
	typedef_name = 'pfn{0}proc'.format(gl_name).upper()

	# typedef
	h_contents.append(
	    'typedef {0};\n'.format(fn.sig.c_form('(*{0})'.format(typedef_name), anonymous_args = True)))

	# dispatch stub
	c_contents.append("""\
static {signature}
{{
\t{typedef_name} function_pointer = ({typedef_name})
\t\tglGetProcAddress((const GLubyte *) "{gl_name}");
\tif (function_pointer == NULL) {{
\t\tprintf("GetProcAddress failed for \\"{gl_name}\\"\\n");
\t\tpiglit_report_result(PIGLIT_FAIL);
\t}}

\t{gl_name} = function_pointer;
\t{opt_ret}function_pointer({params});
}}
""".format(signature = fn.sig.c_form('stub_' + gl_name,
				     anonymous_args = False),
	   gl_name = gl_name, typedef_name = typedef_name,
	   opt_ret = 'return ' if fn.sig.rettype else '',
	   params = ', '.join(p.name for p in fn.sig.params)))

	# function pointer
	h_contents.append('extern {0} {1};\n'.format(typedef_name, gl_name))
	c_contents.append(
	    '{0} {1} = stub_{1};\n'.format(typedef_name, gl_name))

    for en in api.enums:
	h_contents.append('#define GL_{s.name} {s.value}\n'.format(s = en))

    return ''.join(c_contents), ''.join(h_contents)

file_to_parse = sys.argv[1]

api = Api()
read_xml(file_to_parse, api)

c_contents, h_contents = generate_code(api)
with open(sys.argv[2], 'w') as f:
    f.write(c_contents)
with open(sys.argv[3], 'w') as f:
    f.write(h_contents)
