# copyright defined in abiupcx/LICENSE.txt

cmake_minimum_required (VERSION 3.8)
project(abiupcx VERSION 0.1 LANGUAGES CXX C)

set(default_build_type "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

option(ABIUPCX_NO_INT128 "disable use of __int128" OFF)
option(ABIUPCX_ONLY_LIBRARY "define and build the ABIUPCX library" OFF)

if(NOT DEFINED SKIP_SUBMODULE_CHECK)
  execute_process(COMMAND git submodule status --recursive
                  COMMAND grep -c "^[+\-]"
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  OUTPUT_VARIABLE submodule_status
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(submodule_status GREATER 0)
     message(FATAL_ERROR "git submodules are not up to date.
Please run the command 'git submodule update --init --recursive'.")
  endif()
endif()

find_package(Threads)
include(GNUInstallDirs)

add_library(abiupcx STATIC src/abi.cpp src/crypto.cpp include/upcx/fpconv.c)
target_include_directories(abiupcx PUBLIC 
                          "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include" 
                          "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/external/rapidjson/include"
                          "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

if(ABIUPCX_NO_INT128)
target_compile_definitions(abiupcx PUBLIC ABIUPCX_NO_INT128)
endif()

add_library(abiupcx_module MODULE src/abiupcx.cpp src/abi.cpp src/crypto.cpp include/upcx/fpconv.c)
target_include_directories(abiupcx_module PUBLIC 
                          "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include;" 
                          "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/external/rapidjson/include" 
                          "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(abiupcx_module ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(abiupcx_module PROPERTIES OUTPUT_NAME "abiupcx")

enable_testing()

add_executable(test_abiupcx src/test.cpp src/abiupcx.cpp src/ship.abi.cpp)
target_link_libraries(test_abiupcx abiupcx ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_abiupcx COMMAND test_abiupcx)

if(NOT ABIUPCX_NO_INT128)
    add_executable(test_abiupcx_template src/template_test.cpp src/abiupcx.cpp)
    target_link_libraries(test_abiupcx_template abiupcx ${CMAKE_THREAD_LIBS_INIT})
    add_test(NAME test_abiupcx_template COMMAND test_abiupcx_template)
endif()

add_executable(test_abiupcx_key src/key_test.cpp src/abiupcx.cpp)
target_link_libraries(test_abiupcx_key abiupcx ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_abiupcx_key COMMAND test_abiupcx_key)

add_executable(test_abiupcx_reflect src/reflect_test.cpp)
target_include_directories(test_abiupcx_reflect PRIVATE include)
add_test(NAME test_abiupcx_reflect COMMAND test_abiupcx_reflect)

# Causes build issues on some platforms
# add_executable(test_abiupcx_sanitize src/test.cpp src/abiupcx.cpp src/abi.cpp src/crypto.cpp include/upcx/fpconv.c)
# target_include_directories(test_abiupcx_sanitize PRIVATE include external/outcome/single-header external/rapidjson/include external/date/include)
# target_link_libraries(test_abiupcx_sanitize abiupcx -fno-omit-frame-pointer -fsanitize=address,undefined ${CMAKE_THREAD_LIBS_INIT})
# target_compile_options(test_abiupcx_sanitize PUBLIC -fno-omit-frame-pointer -fsanitize=address,undefined)
# add_test(NAME test_abiupcx_sanitize COMMAND test_abiupcx_sanitize)

# add_executable(fuzzer src/fuzzer.cpp src/abiupcx.cpp)
# target_include_directories(fuzzer PRIVATE external/outcome/single-header external/rapidjson/include external/date/include)
# target_link_libraries(fuzzer -fsanitize=fuzzer,address,undefined,signed-integer-overflow -fstandalone-debug ${CMAKE_THREAD_LIBS_INIT})
# target_compile_options(fuzzer PRIVATE -fsanitize=fuzzer,address,undefined,signed-integer-overflow -fstandalone-debug)

if (CMAKE_CXX_COMPILER_ID MATCHES Clang|AppleClang)
    target_compile_options(abiupcx PRIVATE -Wall -Wextra -Wno-unused-parameter -fcolor-diagnostics)
    target_compile_options(test_abiupcx PRIVATE -Wall -Wextra -Wno-unused-parameter -fcolor-diagnostics)
endif()

if (NOT ABIUPCX_ONLY_LIBRARY)
add_subdirectory(tools)
endif()


install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/upcx DESTINATION include)

install(TARGETS abiupcx
  EXPORT abiupcx
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT abiupcx
  DESTINATION "share/abiupcx"
  FILE abiupcx-targets.cmake
)

export(TARGETS abiupcx 
       FILE ${CMAKE_CURRENT_BINARY_DIR}/share/abiupcx/abiupcx-targets.cmake)
configure_file(abiupcx-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/share/abiupcx/abiupcx-config.cmake COPYONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/share/abiupcx/abiupcx-config.cmake
        DESTINATION "share/abiupcx")