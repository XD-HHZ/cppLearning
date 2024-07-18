cmake_minimum_required(VERSION 3.0)
project(MySylarWeb)

include (cmake/utils.cmake)
SET(CMAKE_BUILD_TYPE "Debug")###
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb") ###
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall") ###
set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_CXX_FLAG "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined")

include_directories(.)
include_directories(/usr/local/include)

link_directories(/usr/local/lib)

#find_library(YAMLCPP yaml-cpp)
#message("***", ${YAMLCPP})

set(LIB_SRC
	sylar/address.cc
	sylar/bytearray.cc
    sylar/config.cc
	sylar/fd_manager.cc
    sylar/fiber.cc
	sylar/http/http.cc
	sylar/http/http_parser.cc
	sylar/http/http11_parser.rl.cc
	sylar/http/httpclient_parser.rl.cc
	sylar/hook.cc
	sylar/iomanager.cc
    sylar/log.cpp
    sylar/scheduler.cc
	sylar/socket.cc
	sylar/tcp_server.cc
	sylar/timer.cc
    sylar/thread.cpp
    sylar/util.cpp
    )

add_library(sylar SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(sylar) #__FILE__
#add_library(sylar_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES(sylar_static PROPERTIES OUTPUT_NAME "sylar")

set(LIB_LIB
    sylar
	dl
    pthread
	yaml-cpp
    )

add_executable(test tests/test.cc)
add_dependencies(test sylar)
force_redefine_file_macro_for_sources(test) #__FILE__
target_link_libraries(test sylar ${LIB_LIB})

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config sylar)
force_redefine_file_macro_for_sources(test_config) #__FILE__
target_link_libraries(test_config sylar ${LIB_LIB})

add_executable(test_thread tests/test_thread.cpp)
add_dependencies(test_thread sylar)
force_redefine_file_macro_for_sources(test_thread) #__FILE__
target_link_libraries(test_thread sylar ${LIB_LIB})

add_executable(test_util tests/test_util.cc)
add_dependencies(test_util sylar)
force_redefine_file_macro_for_sources(test_util) #__FILE__
target_link_libraries(test_util sylar ${LIB_LIB})

add_executable(test_fiber tests/test_fiber.cc)
add_dependencies(test_fiber sylar)
force_redefine_file_macro_for_sources(test_fiber) #__FILE__
target_link_libraries(test_fiber sylar ${LIB_LIB})

add_executable(test_scheduler tests/test_scheduler.cc)
add_dependencies(test_scheduler sylar)
force_redefine_file_macro_for_sources(test_scheduler) #__FILE__
target_link_libraries(test_scheduler sylar ${LIB_LIB})

add_executable(test_iomanager tests/test_iomanager.cc)
add_dependencies(test_iomanager sylar)
force_redefine_file_macro_for_sources(test_iomanager) #__FILE__
target_link_libraries(test_iomanager sylar ${LIB_LIB})

add_executable(test_hook tests/test_hook.cc)
add_dependencies(test_hook sylar)
force_redefine_file_macro_for_sources(test_hook) #__FILE__
target_link_libraries(test_hook sylar ${LIB_LIB})

add_executable(test_address tests/test_address.cc)
add_dependencies(test_address sylar)
force_redefine_file_macro_for_sources(test_address) #__FILE__
target_link_libraries(test_address sylar ${LIB_LIB})

add_executable(test_socket tests/test_socket.cc)
add_dependencies(test_socket sylar)
force_redefine_file_macro_for_sources(test_socket) #__FILE__
target_link_libraries(test_socket sylar ${LIB_LIB})

add_executable(test_bytearray tests/test_bytearray.cc)
add_dependencies(test_bytearray sylar)
force_redefine_file_macro_for_sources(test_bytearray) #__FILE__
target_link_libraries(test_bytearray sylar ${LIB_LIB})

add_executable(test_http tests/test_http.cc)
add_dependencies(test_http sylar)
force_redefine_file_macro_for_sources(test_http) #__FILE__
target_link_libraries(test_http sylar ${LIB_LIB})

add_executable(test_http_parser tests/test_http_parser.cc)
add_dependencies(test_http_parser sylar)
force_redefine_file_macro_for_sources(test_http_parser) #__FILE__
target_link_libraries(test_http_parser sylar ${LIB_LIB})

add_executable(test_tcp_server tests/test_tcp_server.cc)
add_dependencies(test_tcp_server sylar)
force_redefine_file_macro_for_sources(test_tcp_server) #__FILE__
target_link_libraries(test_tcp_server sylar ${LIB_LIB})

add_executable(echo_server examples/echo_server.cc)
add_dependencies(echo_server sylar)
force_redefine_file_macro_for_sources(echo_server) #__FILE__
target_link_libraries(echo_server sylar ${LIB_LIB})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
