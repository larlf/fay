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

#########################################################

set(mirror_src
	src/mirror_sys_termcolor.h
	src/mirror_sys_termcolor.cpp
	src/mirror_sys_api.h
	src/mirror_sys_api.cpp
	src/mirror_sys_const.h
	src/mirror_utils.h
	src/mirror_utils.cpp
)

add_library(mirror STATIC ${mirror_src})

#########################################################

set(src
	src/fay_main.cpp
)

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

add_executable(fay ${src})

if(WIN32)
	target_link_libraries(fay mirror.lib winmm.lib mirage.lib jansson.lib libiconv.lib)
else()
	target_link_libraries(fay mirror.lib)
endif()

#########################################################


set(test_src
	src/test_main.cpp
)

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

add_executable(test ${test_src})

if(WIN32)
	target_link_libraries(test mirror.lib winmm.lib mirage.lib jansson.lib libiconv.lib)
else()
	target_link_libraries(test mirror.lib)
endif()




