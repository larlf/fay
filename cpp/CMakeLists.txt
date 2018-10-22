project(fay)

#版本检查
cmake_minimum_required(VERSION 2.6)

#设置Debug版的编译参数，Windows下只支持Debug版
if(WIN32)
	set(CMAKE_C_FLAGS 				"-DWIN32 /MT /EHsc -D_WIN32_WINNT=0x0502")
	set(CMAKE_C_FLAGS_DEBUG			"/MTd -DDEBUG /Zi")
	set(CMAKE_CXX_FLAGS				"-DWIN32 /GR /MT /EHsc -D_WIN32_WINNT=0x0502")
	set(CMAKE_CXX_FLAGS_DEBUG		"/MTd -DDEBUG /Zi")
	set(type win64)
else()
	set(CMAKE_CXX_FLAGS "-fPIC -std=c++0x -g -O2 -static -Wall -pipe -frtti")
	set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -std=c++0x -DDEBUG")
	set(type linux)
endif()

#设置输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

include_directories(
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/deps/${type}/mirage/inc
	${PROJECT_SOURCE_DIR}/deps/${type}/jansson/inc
	${PROJECT_SOURCE_DIR}/deps/${type}/iconv/inc
	${PROJECT_SOURCE_DIR}/deps/${type}/vld/inc
)

link_directories(
	${PROJECT_BINARY_DIR}/bin/Debug
	${PROJECT_SOURCE_DIR}/deps/${type}/mirage/lib
	${PROJECT_SOURCE_DIR}/deps/${type}/jansson/lib
	${PROJECT_SOURCE_DIR}/deps/${type}/iconv/lib
	${PROJECT_SOURCE_DIR}/deps/${type}/vld/lib
)

#########################################################

set(mirror_src
	src/mirror_sys_termcolor.h
	src/mirror_sys_termcolor.cpp
	src/mirror_sys_const.h
	src/mirror_sys_const.cpp
	src/mirror_sys_trace.h
	src/mirror_sys_trace.cpp
	src/mirror_utils_lang.h
	src/mirror_utils_lang.cpp
	src/mirror_utils_string.h
	src/mirror_utils_string.cpp
	src/mirror_utils_time.h
	src/mirror_utils_time.cpp
	src/mirror_utils_log.h
	src/mirror_utils_log.cpp
	src/mirror_utils_file.h
	src/mirror_utils_file.cpp
	src/mirror_data_buffer.h
	src/mirror_data_buffer.cpp
)

add_library(mirror STATIC ${mirror_src})

#########################################################

set(faylib_src
	src/fay_const.h
	src/fay_const.cpp
	src/fay_object.h
	src/fay_object.cpp
	src/fay_type.h
	src/fay_type.cpp
	src/fay_lexer.h
	src/fay_lexer.cpp
	src/fay_token.h
	src/fay_token.cpp
	src/fay_token_stack.h
	src/fay_token_stack.cpp
	src/fay_ast.h
	src/fay_ast.cpp
	src/fay_parser.h
	src/fay_parser.cpp
	src/fay_lang.h
	src/fay_lang.cpp
	src/fay_builder.h
	src/fay_builder.cpp
	src/fay_inst.h
	src/fay_inst.cpp
	src/fay_stack.h
	src/fay_stack.cpp
	src/fay_internal_fun.h
	src/fay_internal_fun.cpp
)

add_library(fay STATIC ${faylib_src})

#########################################################

set(fayvm_src
	src/fayvm_main.cpp
)

add_executable(fayvm ${fayvm_src})

if(WIN32)
	target_link_libraries(fayvm fay mirror.lib winmm.lib mirage.lib jansson.lib libiconv.lib)
else()
	target_link_libraries(fayvm fay mirror.lib)
endif()

#########################################################

set(fayc_src
	src/fayc_main.cpp
)

add_executable(fayc ${fayc_src})

if(WIN32)
	target_link_libraries(fayc mirror.lib winmm.lib mirage.lib jansson.lib libiconv.lib)
else()
	target_link_libraries(fayc mirror.lib)
endif()

#########################################################

set(test_src
	src/test_main.cpp
	src/test_utils.h
	src/test_utils.cpp
	src/test_fay.h
	src/test_fay.cpp
)

add_executable(test ${test_src})

if(WIN32)
	target_link_libraries(test mirror fay winmm.lib mirage.lib jansson.lib libiconv.lib)
else()
	target_link_libraries(test mirror fay)
endif()




