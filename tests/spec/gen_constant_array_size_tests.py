import itertools
import math

def clamp(x, minVal, maxVal):
    if minVal > maxVal:
	return None
    return min(max(x,minVal),maxVal)

def smoothstep(edge0, edge1, x):
    if edge0 >= edge1:
	return None
    t = clamp((x-edge0)/(edge1-edge0),0.0,1.0)
    return t*t*(3-2*t)

def vlength(v):
    return math.sqrt(sum([x*x for x in v]))

def distance(x, y):
    if len(x) != len(y):
	return None
    return vlength([x[i] - y[i] for i in xrange(len(x))])

def dot(x, y):
    if len(x) != len(y):
	return None
    return sum([x[i] * y[i] for i in xrange(len(x))])

def cross(x, y):
    if not (len(x) == len(y) == 3):
	return None
    return [x[1]*y[2] - y[1]*x[2], x[2]*y[0] - y[2]*x[0], x[0]*y[1] - y[0]*x[1]]

def normalize(v):
    vlen = vlength(v)
    if vlen == 0.0:
	return None
    return [x/vlen for x in v]

def faceforward(N, I, Nref):
    if not (len(Nref) == len(I) == len(Nref)):
	return None
    if dot(Nref, I) < 0.0:
	return N
    else:
	return [-x for x in N]

def reflect(I, N):
    if len(I) != len(N):
	return None
    dot_product = dot(N, I)
    return [I[i] - 2*dot_product*N[i] for i in xrange(len(I))]

def refract(I, N, eta):
    if len(N) != len(I):
	return None
    if len(eta) != 1:
	return None
    eta = eta[0]
    k = 1.0 - eta*eta*(1.0-dot(N,I)*dot(N,I))
    if k < 0.0:
	return [0.0 for x in I]
    else:
	return [eta * I[i] - (eta * dot(N, I) + math.sqrt(k)) * N[i] for i in xrange(len(I))]

# A standard set of test points
pts = [-0.9, 0.0, 0.9]

# A standard set of test vectors, including length 1 vectors
vecs = [
    [-0.9], [0.0], [0.9],
    [0.3, 0.4], [0.4, 0.3], [-1.0, 0.0],
    [1.0, 2.0, 3.0], [4.0, 5.0, 6.0], [0.0, 0.0, -1.0],
    [1.0, 2.0, 3.0, 4.0], [0.4, 0.3, 0.2, 0.1], [-0.5, -0.5, -0.5, -0.5],
    ]

normalized_vecs = [v for v in [normalize(v) for v in vecs] if v is not None]

unary_math_functions = [
    ('radians', lambda deg: math.pi/180.0*deg, pts),
    ('degrees', lambda rad: 180.0/math.pi*rad, pts),
    ('sin', math.sin, pts),
    ('cos', math.cos, pts),
    ('tan', math.tan, pts),
    ('asin', math.asin, pts),
    ('acos', math.acos, pts),
    ('atan', math.atan, pts),
    ('exp', math.exp, pts),
    ('log', math.log, [1.0, 2.0, 3.0]),
    ('exp2', lambda x: math.pow(2.0, x), pts),
    ('log2', lambda x: math.log(x)/math.log(2.0), [1.0, 2.0, 3.0]),
    ('sqrt', math.sqrt, [0.0, 1.0, 2.0]),
    ('inversesqrt', lambda x: 1.0/math.sqrt(x), [1.0, 2.0, 3.0]),
    ('sign', lambda x: x/abs(x) if x != 0.0 else 0.0, pts),
    ('floor', math.floor, pts),
    ('ceil', math.ceil, pts),
    ('fract', lambda x: x - math.floor(x), pts),
    ]

binary_math_functions = [
    ('atan', math.atan2, [-1.0, 1.0], [-1.0, 1.0], ['ss', 'vv']),
    ('pow',
     lambda x, y: None if x == 0.0 and y <= 0.0 else math.pow(x, y),
     [0.0, 1.0, 2.0], pts, ['ss', 'vv']),
    ('mod', lambda x, y: x-y*math.floor(x/y), pts,
     [-2.0, -1.0, -0.5, 0.5, 1.0, 2.0], ['ss', 'vs', 'vv']),
    ('min', min, pts, pts, ['ss', 'vv', 'vs']),
    ('max', max, pts, pts, ['ss', 'vv', 'vs']),
    ('step', lambda x, edge: 0.0 if x < edge else 1.0, pts, pts,
     ['ss', 'vv', 'sv']),
    ]

ternary_math_functions = [
    ('clamp', clamp, pts, pts, pts, ['sss', 'vvv', 'vss']),
    ('mix', lambda x, y, a: x*(1-a)+y*a, pts, pts, [0.1, 0.5, 0.9],
     ['sss', 'vvv', 'vvs']),
    ('smoothstep', smoothstep, [-1.0, 0.0, 1.0], [-0.5, 0.0, 0.5],
     pts, ['sss', 'vvv', 'ssv']),
    ]

componentwise_math_functions = []
for fname, meaning, pts in unary_math_functions:
    componentwise_math_functions.append((fname, meaning, [pts], ['s', 'v']))
for fname, meaning, x_pts, y_pts, forms in binary_math_functions:
    componentwise_math_functions.append((fname, meaning, [x_pts, y_pts], forms))
for fname, meaning, x_pts, y_pts, z_pts, forms in ternary_math_functions:
    componentwise_math_functions.append((fname, meaning, [x_pts, y_pts, z_pts], forms))

vector_functions = [
    ('length', vlength, [vecs]),
    ('distance', distance, [vecs, vecs]),
    ('dot', dot, [vecs, vecs]),
    ('cross', cross, [vecs, vecs]),
    ('normalize', normalize, [vecs]),
    ('faceforward', faceforward, [vecs, vecs, vecs]),
    ('reflect', reflect, [vecs, normalized_vecs]),
    ('refract', refract, [normalized_vecs, normalized_vecs, [[0.5], [2.0]]])
    ]

class TestCreator(object):
    def __init__(self, version):
	assert isinstance(version, int)
	assert 100 <= version <= 999

	self.var_name_counters = {}
	self.test_code = []
	self.test_arrays = []
	self.version = version

    def gen_var_name(self, prefix = 'v'):
	if prefix not in self.var_name_counters:
	    self.var_name_counters[prefix] = 0
	self.var_name_counters[prefix] += 1
	return '{0}{1}'.format(prefix, self.var_name_counters[prefix])

    def test_int(self, val):
	# val should evaluate to a reasonable array size.
	array_name = self.gen_var_name('a')
	self.test_code.append('float[{0}] {1};'.format(val, array_name))
	self.test_arrays.append(array_name)

    def test_scalar(self, val):
	# val should evaluate to a reasonable array size when
	# converted to an int.
	assert self.version >= 120
	self.test_int('int({0})'.format(val))

    def test_const_var(self, typ, val):
	const_name = self.gen_var_name('c')
	self.test_code.append(
	    'const {0} {1} = {2};'.format(typ, const_name, val))
	if typ == 'int':
	    self.test_int(const_name)
	else:
	    self.test_scalar(const_name)

    def test_field_of_const_struct(self):
	struct_name = self.gen_var_name('S')
	self.test_code.append(
	    'struct {0} {{ int a; int b; }};'.format(struct_name))
	self.test_int('{0}(1, -1).a'.format(struct_name))
	self.test_int('{0}(-1, 1).b'.format(struct_name))

    def test_math(self, val, expected):
	assert isinstance(expected, list)
	self.test_scalar('length({0} - {1}) < 0.001'.format(self.make_vector(expected), val))

    def group_by_scalar_inputs(self, scalar_args, test_cases):
	groups = {}
	for inputs, expected_output in test_cases:
	    group = tuple(inputs[i] for i in scalar_args)
	    if group not in groups:
		groups[group] = []
	    groups[group].append((inputs, expected_output))
	return groups

    def make_vector(self, vec):
	assert 1 <= len(vec) <= 4
	if len(vec) == 1:
	    return str(vec[0])
	else:
	    return 'vec{0}({1})'.format(len(vec), ', '.join(str(x) for x in vec))

    def make_componentwise_test(self, fname, num_args, scalar_args, test_cases):
	num_vector_elems = len(test_cases)
	scalar_inputs = test_cases[0][0]
	args = []
	for i in xrange(num_args):
	    if i in scalar_args:
		args.append(str(scalar_inputs[i]))
	    else:
		args.append(self.make_vector([test_cases[j][0][i] for j in xrange(num_vector_elems)]))
	expected_vector = [expected for inputs, expected in test_cases]
	return '{0}({1})'.format(fname, ', '.join(args)), expected_vector

    def math_tests(self):
	for fname, meaning, test_pts_list, forms in componentwise_math_functions:
	    test_cases = []
	    for inputs in itertools.product(*test_pts_list):
		expected_output = meaning(*inputs)
		if expected_output is not None:
		    test_cases.append((inputs, expected_output))
	    for form in forms:
		num_args = len(form)
		vector_args = [i for i in xrange(num_args) if form[i] == 'v']
		scalar_args = [i for i in xrange(num_args) if form[i] != 'v']
		groups = self.group_by_scalar_inputs(scalar_args, test_cases)
		for group in groups.values():
		    num_test_cases = len(group)
		    for i in xrange(0, num_test_cases, 4):
			yield self.make_componentwise_test(
			    fname, num_args, scalar_args, group[i:i+4])

	for fname, meaning, test_vecs_list in vector_functions:
	    for vecs in itertools.product(*test_vecs_list):
		expected_output = meaning(*vecs)
		if expected_output is not None:
		    if not isinstance(expected_output, list):
			expected_output = [expected_output]
		    yield '{0}({1})'.format(fname, ', '.join(self.make_vector(x) for x in vecs)), expected_output

    def test_built_in_functions(self):
	# First test length, since the other tests are built out of
	# it.
	self.test_scalar('length(1.25) == 1.25')
	self.test_scalar('length(-1.25) == 1.25')
	self.test_math('length(vec2(3.0, 4.0))', [5.0])
	self.test_math('length(vec2(-3.0, 4.0))', [5.0])
	self.test_math('length(vec2(3.0, -4.0))', [5.0])
	self.test_math('length(vec2(-3.0, -4.0))', [5.0])
	for val, expected in self.math_tests():
	    self.test_math(val, expected)

    def make_all_tests(self):
	# Literal values
	self.test_int('1')
	if self.version >= 120:
	    self.test_scalar('true')
	    self.test_scalar('1.0')

	# Variables declared as const
	self.test_const_var('int', '1')
	if self.version >= 120:
	    self.test_const_var('bool', 'true')
	    self.test_const_var('float', '1.0')
	    # TODO: non-scalars

	if self.version >= 120:
	    # Get an element of a constant array
	    self.test_int('int[2](1, -1)[0]')
	    self.test_int('int[2](-1, 1)[1]')

	    # Get a length of a constant array
	    self.test_int('float[2](-1.0, -1.0).length()')

	    # Get an field of a constant structure
	    self.test_field_of_const_struct()

	    # Get a component of a constant vector
	    self.test_int('ivec4(-1, 1, -1, -1).y')

	    # Swizzle a constant vector
	    self.test_int('ivec2(-1, 1).yx.x')

	    # The above tests also exercise constructors.

	    # Built-in function calls
	    self.test_built_in_functions()

	# Test the one constructor that is allowed as a constant expression in
	# GLSL 1.10.
	self.test_int('int(1)')

	# Expression all of whose components are const
	self.test_int('(1)')
	self.test_int('+1')
	self.test_int('-(-1)')
	if self.version >= 120:
	    self.test_scalar('!false')
	self.test_int('1 * 1')
	self.test_int('2 / 2')
	self.test_int('1 + 0')
	self.test_int('2 - 1')
	if self.version >= 120:
	    self.test_scalar('1 < 2')
	    self.test_scalar('2 > 1')
	    self.test_scalar('1 <= 1')
	    self.test_scalar('1 >= 1')
	    self.test_scalar('1 == 1')
	    self.test_scalar('1 != 2')
	    self.test_scalar('true && true')
	    self.test_scalar('false ^^ true')
	    self.test_scalar('false || true')
	    self.test_int('false ? -1 : 1')
	    self.test_int('true ? 1 : -1')
	self.test_int('1, 2')

	print '/* [config]'
	print ' * expect_result: pass'
	print ' * glsl_version: {0}.{1}'.format(str(self.version)[0], str(self.version)[1:])
	print ' * [end config]'
	print ' *'
	print ' * This file was automatically generated by gen_constant_tests.py'
	print ' */'
	print '#version {0}'.format(self.version)
	print
	for line in self.test_code:
	    print line
	print
	print 'void main()'
	print '{'
	array_accessors = ['{0}.length()'.format(var_name) for var_name in self.test_arrays]
	print '  gl_position = vec4({0});'.format('\n                     + '.join(array_accessors))
	print '}'

# TODO 1.10 non-const?:
# - post- and pre- increment and decrement
# - op= expressions (+=, -=, *=, /=)

# TODO 1.20:
# - structure constructors
# - array constructors?
# - non-integer constructors?
# - swizzling
# - op= expressions that were reserved in GLSL1.10? (%=, <<=, >>=, &=, |=, ^=)
# - operators that were reserved in GLSL1.10? (%, &, |, ^, ~, >>, <<)
# - >, <, >=, <=, ==, !=, &&, ||, ^^, !
# - ?:
# - postfix_expression[integer_expression]
# - function calls
# - postfix_expression DOT FIELD_SELECTION
# - constructors
# - implicit conversion int -> float, ivec2 -> vec2, ivec3 -> vec3, ivec4 -> vec4
# - vector/matrix constructor with a single argument
# - vector constructor with multiple, possibly mixed, arguments (scalars, vectors, matrices), including with stuff dropped
# - matrix constructor with multiple, possibly mixed, arguments (scalars, vectors), including with stuff dropped
# - matrix constructor with a single matrix argument (size doesn't necessarily match)
# - conversion of parameters in vector and matrix constructors

# TODO later:
# - Function parameters declared as const should be rejected

if __name__ == '__main__':
    TestCreator(120).make_all_tests()
