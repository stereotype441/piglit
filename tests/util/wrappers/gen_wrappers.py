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
    def __init__(self, param_xml):
        self.__name = param_xml.getAttribute('name')
        self.__type = param_xml.getAttribute('type')

    @property
    def name(self):
        return self.__name

    @property
    def typ(self):
        return self.__type

    @property
    def decl(self):
        return '{0} {1}'.format(self.__type, self.__name)

class Function(object):
    def __init__(self, func_xml):
        self.__name = func_xml.getAttribute('name')
        self.__params = []
        self.__rettype = None
        for item in child_elements(func_xml):
            if item.tagName == 'param':
                if item.getAttribute('padding') != 'true':
                    self.__params.append(Param(item))
            elif item.tagName == 'return':
                self.__rettype = item.getAttribute('type')
            elif item.tagName == 'glx':
                # TODO: don't know what to do with this yet
                pass
            else:
                raise UnexpectedElement(item)

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
        return ', '.join(p.decl for p in self.__params)

    @property
    def param_names(self):
        return ', '.join(p.name for p in self.__params)

    @property
    def param_types(self):
        return ', '.join(p.typ for p in self.__params)

    @property
    def wrapper_function_sig(self):
        return '{s.rettype} gl{s.name}({s.param_decls})'.format(s = self)

    @property
    def wrapper_function_decl(self):
        return """\
{s.wrapper_function_sig};
""".format(s = self)

    @property
    def wrapper_function_def(self):
        return """\
{s.wrapper_function_sig}
{{
\tstatic {s.rettype} (*func)({s.param_types}) = NULL;

\tif (func == NULL) {{
\t\tfunc = glGetProcAddress("{s.name}");
\t\tif (func == NULL) {{
\t\t\tprintf("Implementation does not support function \\"{s.name}\\"\\n");
\t\t\tpiglit_report_result(PIGLIT_FAIL);
\t\t}}
\t}}

\t{s.opt_return}func({s.param_names});
}}
""".format(s = self)


def read_xml(filename):
    result = []

    def traverse(filename):
        doc = xml.dom.minidom.parse(filename)

        if doc.documentElement.tagName != 'OpenGLAPI':
            raise UnexpectedElement(doc.documentElement)

        # TODO: category is a bad name.
        for category in child_elements(doc.documentElement):
            if category.tagName == 'xi:include':
                traverse(os.path.join(os.path.dirname(filename), category.getAttribute('href')))
                continue
            if category.tagName != 'category':
                raise UnexpectedElement(category)
            for item in child_elements(category):
                if item.tagName == 'function':
                    result.append(Function(item))
                elif item.tagName == 'enum':
                    # TODO: handle this.
                    pass
                elif item.tagName == 'type':
                    # TODO: handle this.
                    pass
                else:
                    raise UnexpectedElement(item)

    traverse(filename)
    return result


file_to_parse = sys.argv[1]

h_file = []
c_file = []

for fn in read_xml(file_to_parse):
    h_file.append(fn.wrapper_function_decl)
    c_file.append(fn.wrapper_function_def)

with open(sys.argv[2], 'w') as f:
    f.write(''.join(c_file))
with open(sys.argv[3], 'w') as f:
    f.write(''.join(h_file))
