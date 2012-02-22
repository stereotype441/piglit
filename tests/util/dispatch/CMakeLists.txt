file(MAKE_DIRECTORY ${piglit_dispatch_generated_output_dir})
execute_process(
	COMMAND ${python} ${CMAKE_CURRENT_SOURCE_DIR}/gen_dispatch.py ${CMAKE_CURRENT_SOURCE_DIR}/gen/gl_API.xml ${piglit_dispatch_gen_output_dir}/gen_dispatch.c ${piglit_dispatch_gen_output_dir}/gen_dispatch.h
	RESULT_VARIABLE gen_dispatch_result)
if(NOT gen_dispatch_result EQUAL 0)
	message(FATAL_ERROR "gen_dispatch.py failed")
endif(NOT gen_dispatch_result EQUAL 0)
