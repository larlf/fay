project(fay)

#版本检查
cmake_minimum_required(VERSION 2.6)

#设置Debug版的编译参数，Windows下只支持Debug版
if(WIN32)
	set(CMAKE_C_FLAGS 				"-DWIN32 /MT /EHsc -D_WIN32_WINNT=0x0501")
	set(CMAKE_C_FLAGS_DEBUG			"/MTd -DDEBUG /Zi")
	set(CMAKE_CXX_FLAGS				"-DWIN32 /MT /EHsc -D_WIN32_WINNT=0x0501")
	set(CMAKE_CXX_FLAGS_DEBUG		"/MTd -DDEBUG /Zi")
	set(type win64)
else()
	set(CMAKE_CXX_FLAGS "-fPIC -std=c++0x -g -O2 -static -Wall -pipe")
	set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -std=c++0x -DDEBUG")
	set(type linux)
endif()

#设置输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(src
	src/fay.l
	src/fay.y
	src/fay_runtime.h
	src/fay_runtime.cpp
	src/fay_vm.h
	src/fay_vm.cpp
	src/fay_lang_const.h
	src/fay_lang_const.cpp
	src/fay_lang_utils.h
	src/fay_lang_utils.cpp
	src/fay_lang_ins.h
	src/fay_lang_ins.cpp
	src/fay_compiler.h
	src/fay_compiler.cpp
	src/fay_compiler_ast.h
	src/fay_compiler_ast.cpp
	src/fay_compiler_pst.h
	src/fay_compiler_pst.cpp
	src/fay_compiler_nodes.h
	src/fay_compiler_nodes.cpp
	src/fay_compiler_parser.h
	src/fay_compiler_parser.cpp
	src/fay_compiler_tokens.cpp
	src/main.cpp
)

link_directories(
	${PROJECT_BINARY_DIR}/../bin
	${PROJECT_SOURCE_DIR}/deps/${type}/mirage/lib
	${PROJECT_SOURCE_DIR}/deps/${type}/jansson/lib
	${PROJECT_SOURCE_DIR}/deps/${type}/iconv/lib
	${PROJECT_SOURCE_DIR}/deps/${type}/vld/lib
)
include_directories(
	deps/${type}/mirage/inc
	deps/${type}/jansson/inc
	deps/${type}/iconv/inc
	deps/${type}/vld/inc
)
add_executable(fay ${src})

if(WIN32)
	#link_directories(${PROJECT_SOURCE_DIR}/deps/win32/lib ${PROJECT_BINARY_DIR}/../bin)
	target_link_libraries(fay winmm.lib mirage.lib jansson.lib libiconv.lib)
else()
	#link_directories(${PROJECT_SOURCE_DIR}/deps/linux/lib ${PROJECT_BINARY_DIR}/../bin)
	target_link_libraries(fay)
endif()


