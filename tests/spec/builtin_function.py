import itertools
import numpy as np

# dictionary mapping test suite name to a list of test cases, where
# each test case has the form (function_name, arguments,
# expected_result).  Each element of arguments, and the whole of
# expected_result, may be either a floating point type or a
# numpy.ndarray representing a vector or matrix.
test_suites = {}

def make_constant(value):
    """Return GLSL code that constructs the given constant value.
    Value may be a floating point type or a numpy.ndarray representing
    a vector."""
    if isinstance(value, float):
	return str(value)
    else:
	assert isinstance(value, np.ndarray)
	assert len(value.shape) == 1
	vector_length = value.shape[0]
	assert 2 <= vector_length <= 4
	return 'vec{0}({1})'.format(
	    vector_length, ', '.join(str(x) for x in value))

def _arctan2(y, x):
    if x == y == 0.0:
	return None
    return np.arctan2(y, x)

def _pow(x, y):
    if x < 0.0:
	return None
    if x == 0.0 and y <= 0.0:
	return None
    return np.power(x, y)

def _clamp(x, minVal, maxVal):
    if minVal > maxVal:
	return None
    return min(max(x, minVal), maxVal)

def _smoothstep(edge0, edge1, x):
    if edge0 >= edge1:
	return None
    t = _clamp((x-edge0)/(edge1-edge0),0.0,1.0)
    return t*t*(3.0-2.0*t)

def _normalize(x):
    return x/np.linalg.norm(x)

def _faceforward(N, I, Nref):
    if np.dot(Nref, I) < 0.0:
	return N
    else:
	return -N

def _reflect(I, N):
    return I-2*np.dot(N,I)*N

def _refract(I, N, eta):
    k = 1.0-eta*eta*(1.0-np.dot(N,I)*np.dot(N,I))
    if k < 0.0:
	return I*0.0
    else:
	return eta*I-(eta*np.dot(N,I)+np.sqrt(k))*N

# GLSL built-in functions that operate on vectors in componentwise
# fashion.  Each entry in the table is (name, arity,
# python_equivalent, signatures, test_inputs).
#
# python_equivalent is a Python function which operates on scalars,
# and simulates GLSL function.  This function should return None in
# any case where the output of the GLSL function is undefined.
#
# signatures is a list of strings containing s's and v's to represent
# the various combinations of vectors and scalars that can be used as
# arguments.  For example, if signatures is ['vv', 'vs'], then the
# function can either be called with vectors for both arguments, or a
# vector for the first argument and a scalar for the second.
# Signatures should not contain a string of all s's--this is implied.
#
# test_inputs is a list, the ith element of which is a list of values
# that are suitable for use as the ith argument of the function.
_componentwise_functions = [
    ('radians', 1, np.radians, ['v'], [np.linspace(-180.0, 180.0, 4)]),
    ('degrees', 1, np.degrees, ['v'], [np.linspace(-np.pi, np.pi, 4)]),
    ('sin', 1, np.sin, ['v'], [np.linspace(-np.pi, np.pi, 4)]),
    ('cos', 1, np.cos, ['v'], [np.linspace(-np.pi, np.pi, 4)]),
    ('tan', 1, np.tan, ['v'], [np.linspace(-np.pi, np.pi, 4)]),
    ('asin', 1, np.arcsin, ['v'], [np.linspace(-1.0, 1.0, 4)]),
    ('acos', 1, np.arccos, ['v'], [np.linspace(-1.0, 1.0, 4)]),
    ('atan', 1, np.arctan, ['v'], [np.linspace(-2.0, 2.0, 4)]),
    ('atan', 2, _arctan2, ['vv'], [np.linspace(-2.0, 2.0, 3), np.linspace(-2.0, 2.0, 3)]),
    ('pow', 2, _pow, ['vv'], [np.linspace(0.0, 2.0, 4), np.linspace(-2.0, 2.0, 4)]),
    ('exp', 1, np.exp, ['v'], [np.linspace(-2.0, 2.0, 4)]),
    ('log', 1, np.log, ['v'], [np.linspace(0.01, 2.0, 4)]),
    ('exp2', 1, np.exp2, ['v'], [np.linspace(-2.0, 2.0, 4)]),
    ('log2', 1, np.log2, ['v'], [np.linspace(0.01, 2.0, 4)]),
    ('sqrt', 1, np.sqrt, ['v'], [np.linspace(0.0, 2.0, 4)]),
    ('inversesqrt', 1, lambda x: 1.0/np.sqrt(x), ['v'], [np.linspace(0.1, 2.0, 4)]),
    ('abs', 1, np.abs, ['v'], [np.linspace(-1.5, 1.5, 5)]),
    ('sign', 1, np.sign, ['v'], [np.linspace(-1.5, 1.5, 5)]),
    ('floor', 1, np.floor, ['v'], [np.linspace(-2.0, 2.0, 4)]),
    ('ceil', 1, np.ceil, ['v'], [np.linspace(-2.0, 2.0, 4)]),
    ('fract', 1, lambda x: x-np.floor(x), ['v'], [np.linspace(-2.0, 2.0, 4)]),
    ('mod', 2, lambda x, y: x-y*np.floor(x/y), ['vs', 'vv'], [np.linspace(-1.9, 1.9, 4), np.linspace(-2.0, 2.0, 4)]),
    ('min', 2, min, ['vv', 'vs'], [np.linspace(-2.0, 2.0, 4), np.linspace(-2.0, 2.0, 4)]),
    ('max', 2, max, ['vv', 'vs'], [np.linspace(-2.0, 2.0, 4), np.linspace(-2.0, 2.0, 4)]),
    ('clamp', 3, _clamp, ['vvv', 'vss'], [np.linspace(-2.0, 2.0, 4), np.linspace(-1.5, 1.5, 3), np.linspace(-1.5, 1.5, 3)]),
    ('mix', 3, lambda x, y, a: x*(1-a)+y*a, ['vvv', 'vvs'], [np.linspace(-2.0, 2.0, 2), np.linspace(-3.0, 3.0, 2), np.linspace(0.0, 1.0, 4)]),
    ('step', 2, lambda edge, x: 0.0 if x < edge else 1.0, ['vv', 'sv'], [np.linspace(-2.0, 2.0, 4), np.linspace(-2.0, 2.0, 4)]),
    ('smoothstep', 3, _smoothstep, ['vvv', 'ssv'], [np.linspace(-1.9, 1.9, 4), np.linspace(-1.9, 1.9, 4), np.linspace(-2.0, 2.0, 4)]),
]

_std_vectors = [
    -1.33,
     0.85,
     np.array([-0.10, -1.20]),
     np.array([-0.42, 0.48]),
     np.array([-0.03, -0.85, -0.94]),
     np.array([1.67, 0.66, 1.87]),
     np.array([-1.65, 1.33, 1.93, 0.76]),
     np.array([0.80, -0.15, -0.51, 0.0])
     ]
_std_vectors3 = [
     np.array([-0.03, -0.85, -0.94]),
     np.array([1.67, 0.66, 1.87]),
     ]
_normalized_vectors = [_normalize(x) for x in _std_vectors]
_std_matrices = [
    np.array([[ 1.60,  0.76],
	      [ 1.53, -1.00]]), # mat2
    np.array([[-0.13, -0.87],
	      [-1.40,  1.40]]), # mat2
    np.array([[-1.11,  1.67, -0.41],
	      [ 0.13,  1.09, -0.02],
	      [ 0.56,  0.95,  0.24]]), # mat3
    np.array([[-1.69, -0.46, -0.18],
	      [-1.09,  1.75,  2.00],
	      [-1.53, -0.70, -1.47]]), # mat3
    np.array([[-1.00, -0.55, -1.08,  1.79],
	      [ 1.77,  0.62,  0.48, -1.35],
	      [ 0.09, -0.71, -1.39, -1.21],
	      [-0.91, -1.82, -1.43,  0.72]]), # mat4
    np.array([[ 0.06,  1.31,  1.52, -1.96],
	      [ 1.60, -0.32,  0.51, -1.84],
	      [ 1.25,  0.45,  1.90, -0.72],
	      [-0.16,  0.45, -0.88,  0.39]]), # mat4
    np.array([[ 0.09,  1.30,  1.25],
	      [-1.19,  0.08,  1.08]]), # mat3x2
    np.array([[-0.36, -1.08, -0.60],
	      [-0.53,  0.88, -1.79]]), # mat3x2
    np.array([[-0.46,  1.94],
	      [-0.45, -0.75],
	      [ 1.03, -0.50]]), # mat2x3
    np.array([[ 1.38, -1.08],
	      [-1.27,  1.83],
	      [ 1.00, -0.74]]), # mat2x3
    np.array([[ 1.81, -0.87,  0.81,  0.65],
	      [-1.16, -1.52,  0.25, -1.51]]), # mat4x2
    np.array([[ 1.93, -1.63,  0.29,  1.60],
	      [ 0.49,  0.27,  0.14,  0.94]]), # mat4x2
    np.array([[ 0.16, -1.69],
	      [-0.80,  0.59],
	      [-1.74, -1.43],
	      [-0.02, -1.21]]), # mat2x4
    np.array([[-1.02,  0.74],
	      [-1.64, -0.13],
	      [-1.59,  0.47],
	      [ 0.30,  1.13]]), # mat2x4
    np.array([[-0.27, -1.38, -1.41, -0.12],
	      [-0.17, -0.56,  1.47,  1.86],
	      [-1.85, -1.29,  1.77,  0.01]]), # mat4x3
    np.array([[-0.47, -0.15,  1.97, -1.05],
	      [-0.20,  0.53, -1.82, -1.41],
	      [-1.39, -0.19,  1.62,  1.58]]), # mat4x3
    np.array([[ 1.42, -0.86,  0.27],
	      [ 1.80, -1.74,  0.04],
	      [-1.88, -0.37,  0.43],
	      [ 1.37,  1.90,  0.71]]), # mat3x4
    np.array([[-1.72,  0.09,  0.45],
	      [-0.31, -1.58,  1.92],
	      [ 0.14,  0.18, -0.56],
	      [ 0.40, -0.77,  1.76]]), # mat3x4
    ]

# GLSL built-in functions that operate on vectors/matrices as a whole.
# Each entry in the table is (name, arity, python_equivalent,
# signature, test_inputs).
#
# python_equivalent is a Python function which operates on scalars,
# and simulates GLSL function.  This function should return None in
# any case where the output of the GLSL function is undefined.
# However, it need not check that the lengths of the input vectors are
# all the same.
#
# signature is a string containing s's, v's, and m's to represent
# which arguments are vectors, matrices, and scalars.  For example, if
# signature is 'vs', then the first argument is a vector and the
# second argument is a scalar.
#
# test_inputs is a list, the ith element of which is a list of vectors
# and/or scalars that are suitable for use as the ith argument of the
# function.
_vector_and_matrix_functions = [
    ('length', 1, np.linalg.norm, 'v', [_std_vectors]),
    ('distance', 2, lambda x, y: np.linalg.norm(x-y), 'vv', [_std_vectors, _std_vectors]),
    ('dot', 2, np.dot, 'vv', [_std_vectors, _std_vectors]),
    ('cross', 2, np.cross, 'vv', [_std_vectors3, _std_vectors3]),
    ('normalize', 1, _normalize, 'v', [_std_vectors]),
    ('faceforward', 3, _faceforward, 'vvv', [_std_vectors, _std_vectors, _std_vectors]),
    ('reflect', 2, _reflect, 'vv', [_std_vectors, _normalized_vectors]),
    ('refract', 3, _refract, 'vvs', [_normalized_vectors, _normalized_vectors, [0.5, 2.0]]),
    ('matrixCompMult', 2, lambda x, y: x*y, 'mm', [_std_matrices, _std_matrices]),
]

def _make_simple_test_cases(name, arity, python_equivalent, test_inputs, match_sizes = ()):
    """Construct test cases for the given function where each input is
    drawn from the cartesian product of test_inputs.  Disregard cases
    where python_equivalent returns None.  If match_sizes is given,
    then check the length of each input vector indexed by it, and
    disregard cases where vector lengths do not match."""
    def measure(x):
	if isinstance(x, float):
	    return (1,) # scalar
	else:
	    assert isinstance(x, np.ndarray)
	    return x.shape
    test_cases = []
    for inputs in itertools.product(*test_inputs):
	lengths = [measure(inputs[i]) for i in match_sizes]
	if not all(x == lengths[0] for x in lengths):
	    continue
	expected_output = python_equivalent(*inputs)
	if expected_output is not None:
	    test_cases.append((name, inputs, expected_output))
    return test_cases

def _vectorize_test_cases(scalar_test_cases, signature, vector_length):
    """Combine the given scalar test cases into vector test cases,
    where possible, by allowing the arguments corresponding to 'v's in
    signature to be vectors with length vector_length."""
    # Group test cases according to the values passed to the arguments
    # that must be scalar.
    arity = len(signature)
    scalar_arguments = [i for i in xrange(arity) if signature[i] != 'v']
    grouped_test_cases = {}
    for test_case in scalar_test_cases:
	name, arguments, expected_result = test_case
	key = (name, tuple(arguments[i] for i in scalar_arguments))
	if key not in grouped_test_cases:
	    grouped_test_cases[key] = []
	grouped_test_cases[key].append(test_case)
    # For each group, combine the test cases into vectors.  If the
    # test cases do not evenly divide into vectors of length
    # vector_length, wrap around as necessary to ensure that every
    # test case is included.
    vectorized_test_cases = []
    for key, test_cases in grouped_test_cases.items():
	name_0, arguments_0, expected_result_0 = test_cases[0]
	for i in xrange(0, len(test_cases), vector_length):
	    test_cases_to_vectorize = []
	    for j in xrange(vector_length):
		test_cases_to_vectorize.append(
		    test_cases[(i + j) % len(test_cases)])
	    args = []
	    for j in xrange(arity):
		if signature[j] == 'v':
		    args.append(
			np.array(
			    [arguments[j]
			     for name, arguments, expected_result
			     in test_cases_to_vectorize]))
		else:
		    args.append(arguments_0[j])
	    result = np.array(
		[expected_result
		 for name, arguments, expected_result
		 in test_cases_to_vectorize])
	    vectorized_test_cases.append((name_0, tuple(args), result))
    return vectorized_test_cases

for name, arity, python_equivalent, signatures, test_inputs in \
	_componentwise_functions:
    if name in test_suites:
	test_suite_name = '{0}{1}'.format(name, arity)
    else:
	test_suite_name = name
    assert test_suite_name not in test_suites
    assert 's'*arity not in signatures
    scalar_test_cases = _make_simple_test_cases(
	name, arity, python_equivalent, test_inputs)
    test_suites[test_suite_name] = list(scalar_test_cases)
    for signature in signatures:
	for vector_length in (2, 3, 4):
	    test_suites[test_suite_name].extend(
		_vectorize_test_cases(
		    scalar_test_cases, signature, vector_length))

for name, arity, python_equivalent, signature, test_inputs in _vector_and_matrix_functions:
    test_suite_name = name
    assert test_suite_name not in test_suites
    vector_arguments = [i for i in xrange(arity) if signature[i] != 's']
    test_suites[test_suite_name] = _make_simple_test_cases(
	name, arity, python_equivalent, test_inputs, vector_arguments)

for name, ts in test_suites.items():
    print '{0}:'.format(name)
    for fname, args, expected in ts:
	print '  {0}({1}) == {2}'.format(fname, ', '.join(str(x) for x in args), expected)
