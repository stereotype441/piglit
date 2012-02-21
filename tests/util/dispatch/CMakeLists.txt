execute_process(
	COMMAND ${python} ${CMAKE_CURRENT_SOURCE_DIR}/gen_dispatch.py ${CMAKE_CURRENT_SOURCE_DIR}/gen/gl_API.xml ${CMAKE_CURRENT_BINARY_DIR}/wrappers.c ${CMAKE_CURRENT_BINARY_DIR}/wrappers.h
	RESULT_VARIABLE gen_dispatch_result)
if(NOT gen_dispatch_result EQUAL 0)
	message(FATAL_ERROR "gen_dispatch.py failed")
endif(NOT gen_dispatch_result EQUAL 0)

set(
	PIGLIT_DISPATCH_INCLUDE_PATH ${CMAKE_CURRENT_BINARY_DIR}
	PARENT_SCOPE)
