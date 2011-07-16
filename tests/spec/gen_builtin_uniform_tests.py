from builtin_function import *
import numpy
import os.path

this_file = os.path.basename(__file__)

def make_shader(function_name, types, offset, scale):
    base_type, num_cols, num_rows = glsl_type_info(types[0])
    shader = ['varying vec4 color;']
    for i in xrange(1, len(types)):
	shader.append('uniform {0} arg{1};'.format(types[i], i-1))
    if num_rows != 1:
	shader.append('uniform int column;')
	indexer = '[column]'
	padding = 4 - num_rows
    else:
	indexer = ''
	padding = 4 - num_cols
    shader.append('')
    shader.append('void main()')
    shader.append('{')
    shader.append('  gl_Position = gl_Vertex;')
    shader.append('  {0} result = {1}({2});'.format(
	    types[0], function_name, ', '.join('arg{0}'.format(i-1) for i in xrange(1, len(types)))))
    if base_type != 'bool':
	shader.append('  result -= {0};'.format(offset))
	shader.append('  result *= {0};'.format(scale))
    shader.append('  color = vec4(result{0}{1});'.format(indexer, ', 0.0' * padding))
    shader.append('}')
    return shader

def reference_shader():
    return [
	'varying vec4 color;',
	'',
	'void main()',
	'{',
	'  gl_FragColor = color;',
	'}',
	]

def compute_offset_and_scale(test_cases):
    low = min(numpy.min(expected_result) for args, expected_result in test_cases)
    hi = max(numpy.max(expected_result) for args, expected_result in test_cases)
    span = hi - low
    center = (hi + low)/2.0
    span *= 2.0
    if span < 1.0:
	span = 1.0
    offset = center - span/2.0
    scale = 1.0/span
    return offset, scale

def piglit_format(values):
    transformed_values = []
    for value in values:
	if isinstance(value, bool):
	    transformed_values.append(int(value))
	else:
	    transformed_values.append(value)
    return ' '.join(str(x) for x in transformed_values)

def shader_runner_type(glsl_type):
    if glsl_type == 'bool':
	return 'int'
    elif glsl_type.startswith('bvec'):
	return 'i' + glsl_type[1:]
    elif glsl_type.startswith('mat') and len(glsl_type) == 4:
	return glsl_type + 'x' + glsl_type[3]
    else:
	return glsl_type

def make_test(types, offset, scale, test_cases):
    base_type, num_cols, num_rows = glsl_type_info(types[0])
    def rescale_and_pad(value):
	if base_type == 'bool':
	    value = value*1.0
	else:
	    value = (value - offset) * scale
	value = column_major_values(value)
	while len(value) < 4:
	    value.append(0.0)
	return value
    test = []
    for test_num, test_case in enumerate(test_cases):
	args, expected = test_case
	for i in xrange(len(args)):
	    test.append('uniform {0} arg{1} {2}'.format(
		    shader_runner_type(types[i+1]), i, piglit_format(column_major_values(args[i]))))
	if num_rows != 1:
	    for column in xrange(num_cols):
		test.append('uniform int column {0}'.format(column))
		test.append('draw rect -1 -1 2 2')
		test.append('probe rgba {0} {1} {2}'.format(
			test_num, column, piglit_format(rescale_and_pad(expected[:,column]))))
	else:
	    test.append('draw rect -1 -1 2 2')
	    test.append('probe rgba {0} 0 {1}'.format(
		    test_num, piglit_format(rescale_and_pad(expected))))
    return test

for function_name, test_suite in test_suites.items():
    for types, test_cases in test_suite.items():
	offset, scale = compute_offset_and_scale(test_cases)
	filename = 'glsl-1.20/execution/built-in-functions/{0}-{1}.shader_test'.format(function_name, '-'.join(types))
	test = [
	    '[require]',
	    'GLSL >= 1.10',
	    '',
	    '[vertex shader]',
	    ]
	test.extend(make_shader(function_name, types, offset, scale))
	test.append('')
	test.append('[fragment shader]')
	test.extend(reference_shader())
	test.append('')
	test.append('[test]')
	test.extend(make_test(types, offset, scale, test_cases))
	with open(filename, 'w') as f:
	    f.write(''.join(line + '\n' for line in test))
