cmake_minimum_required( VERSION 3.8 )

project( UPCXIO )
include(CTest) # suppresses DartConfiguration.tcl error
enable_testing()

if ("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
   message(WARNING "CMAKE_INSTALL_PREFIX is set to /usr/local. This is not recommended.")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

include( GNUInstallDirs )
include( InstallDirectoryPermissions )
include( MASSigning )

set( BLOCKCHAIN_NAME "UPCXIO" )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_EXTENSIONS ON )
set( CXX_STANDARD_REQUIRED ON)

set(VERSION_MAJOR 2)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
#set(VERSION_SUFFIX rc3)

if(VERSION_SUFFIX)
    set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_SUFFIX}")
else()
    set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
endif()

set( CLI_CLIENT_EXECUTABLE_NAME clupcx )
set( NODE_EXECUTABLE_NAME nodupcx )
set( KEY_STORE_EXECUTABLE_NAME kupcxd )
set( RODUPCX_EXECUTABLE_NAME rodupcx )
set( TESTER_EXECUTABLE_NAME upcxio-tester )

# http://stackoverflow.com/a/18369825
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0)
        message(FATAL_ERROR "GCC version must be at least 8.0!")
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
        message(FATAL_ERROR "Clang version must be at least 5.0!")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wsign-compare -Wrange-loop-analysis")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
endif()

if ("${CMAKE_GENERATOR}" STREQUAL "Ninja")
   add_compile_options(-fdiagnostics-color=always)
endif()

set(BUILD_DOXYGEN FALSE CACHE BOOL "Build doxygen documentation on every make")
set(ENABLE_MULTIVERSION_PROTOCOL_TEST FALSE CACHE BOOL "Enable nodupcx multiversion protocol test")

# add defaults for openssl
if(APPLE AND UNIX AND "${OPENSSL_ROOT_DIR}" STREQUAL "")
   set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl@1.1")
endif()

# WASM runtimes to enable. Each runtime in this list will have:
#  * definition UPCXIO_<RUNTIME>_RUNTIME_ENABLED defined in public libchain interface
#  * ctest entries with --runtime
# TODO reenable later
if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND NOT WIN32)
   if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux" AND "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
      list(APPEND UPCXIO_WASM_RUNTIMES upcx-vm-oc)
      # UPCX VM OC requires LLVM, but move the check up here to a central location so that the UpcxioTester.cmakes
      # can be created with the exact version found
      find_package(LLVM REQUIRED CONFIG)
     if(LLVM_VERSION_MAJOR VERSION_LESS 7 OR LLVM_VERSION_MAJOR VERSION_GREATER_EQUAL 12)
        message(FATAL_ERROR "UPCXIO requires an LLVM version 7 through 11")
     endif()
   endif()
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND NOT WIN32)
   if(CMAKE_SYSTEM_PROCESSOR STREQUAL x86_64)
      list(APPEND UPCXIO_WASM_RUNTIMES upcx-vm upcx-vm-jit)
   endif()
endif()

if(UNIX)
  if(APPLE)
    set(whole_archive_flag "-force_load")
    set(no_whole_archive_flag "")
  else()
    set(whole_archive_flag "--whole-archive")
    set(no_whole_archive_flag "--no-whole-archive")
  endif()
else()
  set(whole_archive_flag "--whole-archive")
  set(no_whole_archive_flag "--no-whole-archive")
endif()

set(Boost_USE_MULTITHREADED      ON)
set( Boost_USE_STATIC_LIBS ON CACHE STRING "ON or OFF" )
# Most boost deps get implictly picked up via fc, as just about everything links to fc. In addition we pick up
# the pthread dependency through fc.
find_package(Boost 1.67 REQUIRED COMPONENTS program_options unit_test_framework)

if( APPLE AND UNIX )
# Apple Specific Options Here
    message( STATUS "Configuring UPCXIO on macOS" )
    set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -Wno-deprecated-declarations" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated-declarations" )
else()
    # Linux Specific Options Here
    message( STATUS "Configuring UPCXIO on Linux" )
    set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall" )
    if ( FULL_STATIC_BUILD )
      set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
    endif ( FULL_STATIC_BUILD )

    if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
        if( CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 4.0.0 OR CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.0.0 )
            set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-partial-specialization" )
        endif()
    endif()
endif()

option(UPCXIO_ENABLE_DEVELOPER_OPTIONS "enable developer options for UPCXIO" OFF)

# based on http://www.delorie.com/gnu/docs/gdb/gdb_70.html
# uncomment this line to tell GDB about macros (slows compile times)
# set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -gdwarf-2 -g3" )

set(ENABLE_COVERAGE_TESTING FALSE CACHE BOOL "Build UPCXIO for code coverage analysis")

if(ENABLE_COVERAGE_TESTING)
  SET(CMAKE_C_FLAGS   "--coverage ${CMAKE_C_FLAGS}")
  SET(CMAKE_CXX_FLAGS "--coverage ${CMAKE_CXX_FLAGS}")
  find_program( LCOV_PATH lcov )
  find_program( LLVMCOV_PATH  llvm-cov )
  find_program( GENHTML_PATH NAMES genhtml)
endif()

include(utils)

if ("${CORE_SYMBOL_NAME}" STREQUAL "")
  set( CORE_SYMBOL_NAME "SYS" )
endif()
string(TOUPPER ${CORE_SYMBOL_NAME} CORE_SYMBOL_NAME)

string(LENGTH ${CORE_SYMBOL_NAME} CORE_SYMBOL_NAME_LENGTH)
if (CORE_SYMBOL_NAME_LENGTH GREATER 7)
  message(FATAL_ERROR "CORE_SYMBOL_NAME length must be between 1 and 7 characters")
endif()

message( STATUS "Using '${CORE_SYMBOL_NAME}' as CORE symbol name" )

if ("${UPCXIO_ROOT_KEY}" STREQUAL "")
   set(UPCXIO_ROOT_KEY "UPCX6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV")
endif()

message( STATUS "Using '${UPCXIO_ROOT_KEY}' as public key for 'upcxio' account" )

find_package( Gperftools QUIET )
if( GPERFTOOLS_FOUND )
    message( STATUS "Found gperftools; compiling UPCXIO with TCMalloc")
    #if doing this by the book, simply link_libraries( ${GPERFTOOLS_TCMALLOC} ) here. That will
    #give the performance benefits of tcmalloc but since it won't be linked last
    #the heap profiler & checker may not be accurate. This here is rather undocumented behavior
    #to stuff a library toward the end of the link list
    set(CMAKE_C_STANDARD_LIBRARIES "${CMAKE_C_STANDARD_LIBRARIES} ${GPERFTOOLS_TCMALLOC}")
    set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} ${GPERFTOOLS_TCMALLOC}")
endif()

add_subdirectory( libraries )
add_subdirectory( plugins )
add_subdirectory( programs )
add_subdirectory( scripts )
add_subdirectory( unittests )
add_subdirectory( contracts )
add_subdirectory( tests )
add_subdirectory( tools )

option(DISABLE_WASM_SPEC_TESTS "disable building of wasm spec unit tests" OFF)

if (NOT DISABLE_WASM_SPEC_TESTS)
add_subdirectory( upcxio-wasm-spec-tests/generated-tests )
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

install_directory_permissions(DIRECTORY ${CMAKE_INSTALL_FULL_SYSCONFDIR}/upcxio)

install(FILES testnet.template DESTINATION ${CMAKE_INSTALL_FULL_SYSCONFDIR}/upcxio/launcher)
install_directory_permissions(DIRECTORY ${CMAKE_INSTALL_FULL_SYSCONFDIR}/upcxio)
install_directory_permissions(DIRECTORY ${CMAKE_INSTALL_FULL_SYSCONFDIR}/upcxio/launcher)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/testnet.template ${CMAKE_CURRENT_BINARY_DIR}/etc/upcxio/launcher/testnet.template COPYONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/contracts/contracts/upcxio.bios/upcxio.bios.abi DESTINATION ${CMAKE_INSTALL_FULL_SYSCONFDIR}/upcxio/contracts)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/contracts/contracts/upcxio.bios/upcxio.bios.wasm DESTINATION ${CMAKE_INSTALL_FULL_SYSCONFDIR}/upcxio/contracts)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/contracts/contracts/upcxio.boot/upcxio.boot.abi DESTINATION ${CMAKE_INSTALL_FULL_SYSCONFDIR}/upcxio/contracts)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/contracts/contracts/upcxio.boot/upcxio.boot.wasm DESTINATION ${CMAKE_INSTALL_FULL_SYSCONFDIR}/upcxio/contracts)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.in ${CMAKE_CURRENT_BINARY_DIR}/version.hpp)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/version.hpp DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/upcxio)

set(UPCX_ROOT_DIR ${CMAKE_BINARY_DIR})
configure_file(${CMAKE_SOURCE_DIR}/CMakeModules/upcxio-config.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/upcxio/upcxio-config.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/CMakeModules/UpcxioTesterBuild.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/upcxio/UpcxioTester.cmake @ONLY)

set(UPCX_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
configure_file(${CMAKE_SOURCE_DIR}/CMakeModules/upcxio-config.cmake.in ${CMAKE_BINARY_DIR}/modules/upcxio-config.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/modules/upcxio-config.cmake DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/cmake/upcxio)
configure_file(${CMAKE_SOURCE_DIR}/CMakeModules/UpcxioTester.cmake.in ${CMAKE_BINARY_DIR}/modules/UpcxioTester.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/modules/UpcxioTester.cmake DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/cmake/upcxio)

configure_file(${CMAKE_SOURCE_DIR}/LICENSE
               ${CMAKE_BINARY_DIR}/licenses/upcxio/LICENSE COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/softfloat/COPYING.txt
               ${CMAKE_BINARY_DIR}/licenses/upcxio/LICENSE.softfloat COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/wasm-jit/LICENSE
               ${CMAKE_BINARY_DIR}/licenses/upcxio/LICENSE.wavm COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/fc/secp256k1/upstream/COPYING
               ${CMAKE_BINARY_DIR}/licenses/upcxio/LICENSE.secp256k1 COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/fc/include/fc/crypto/webauthn_json/license.txt
               ${CMAKE_BINARY_DIR}/licenses/upcxio/LICENSE.rapidjson COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/fc/src/network/LICENSE.go
               ${CMAKE_BINARY_DIR}/licenses/upcxio/LICENSE.go COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/yubihsm/LICENSE
               ${CMAKE_BINARY_DIR}/licenses/upcxio/LICENSE.yubihsm COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/upcx-vm/LICENSE
               ${CMAKE_BINARY_DIR}/licenses/upcxio/LICENSE.upcx-vm COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/rocksdb/LICENSE.Apache
               ${CMAKE_BINARY_DIR}/licenses/upcxio/LICENSE.rocksdb COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/rocksdb/LICENSE.leveldb
               ${CMAKE_BINARY_DIR}/licenses/upcxio/LICENSE.leveldb COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/amqp-cpp/LICENSE
               ${CMAKE_BINARY_DIR}/licenses/upcxio/LICENSE.amqpcpp COPYONLY)
file(DOWNLOAD https://raw.githubusercontent.com/jtv/libpqxx/master/COPYING 
     ${CMAKE_BINARY_DIR}/licenses/upcxio/LICENSE.libpqxx)

install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_FULL_DATAROOTDIR}/licenses/upcxio/ COMPONENT base)
install(FILES libraries/softfloat/COPYING.txt DESTINATION ${CMAKE_INSTALL_FULL_DATAROOTDIR}/licenses/upcxio/ RENAME LICENSE.softfloat COMPONENT base)
install(FILES libraries/wasm-jit/LICENSE DESTINATION ${CMAKE_INSTALL_FULL_DATAROOTDIR}/licenses/upcxio/ RENAME LICENSE.wavm COMPONENT base)
install(FILES libraries/fc/secp256k1/upstream/COPYING DESTINATION ${CMAKE_INSTALL_FULL_DATAROOTDIR}/licenses/upcxio/ RENAME LICENSE.secp256k1 COMPONENT base)
install(FILES libraries/fc/include/fc/crypto/webauthn_json/license.txt DESTINATION ${CMAKE_INSTALL_FULL_DATAROOTDIR}/licenses/upcxio/ RENAME LICENSE.rapidjson COMPONENT base)
install(FILES libraries/fc/src/network/LICENSE.go DESTINATION ${CMAKE_INSTALL_FULL_DATAROOTDIR}/licenses/upcxio/ COMPONENT base)
install(FILES libraries/yubihsm/LICENSE DESTINATION ${CMAKE_INSTALL_FULL_DATAROOTDIR}/licenses/upcxio/ RENAME LICENSE.yubihsm COMPONENT base)
install(FILES libraries/upcx-vm/LICENSE DESTINATION ${CMAKE_INSTALL_FULL_DATAROOTDIR}/licenses/upcxio/ RENAME LICENSE.upcx-vm COMPONENT base)
install(FILES libraries/rocksdb/LICENSE.Apache DESTINATION ${CMAKE_INSTALL_FULL_DATAROOTDIR}/licenses/upcxio/ RENAME LICENSE.rocksdb COMPONENT base)
install(FILES libraries/rocksdb/LICENSE.leveldb DESTINATION ${CMAKE_INSTALL_FULL_DATAROOTDIR}/licenses/upcxio/ RENAME LICENSE.leveldb COMPONENT base)
install(FILES libraries/amqp-cpp/LICENSE DESTINATION ${CMAKE_INSTALL_FULL_DATAROOTDIR}/licenses/upcxio/ RENAME LICENSE.amqpcpp COMPONENT base)
install(FILES ${CMAKE_BINARY_DIR}/licenses/upcxio/LICENSE.libpqxx DESTINATION ${CMAKE_INSTALL_FULL_DATAROOTDIR}/licenses/upcxio/ COMPONENT base)

add_custom_target(base-install
  COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}"
  COMMAND "${CMAKE_COMMAND}" -DCMAKE_INSTALL_COMPONENT=base -P "${CMAKE_BINARY_DIR}/cmake_install.cmake"
  USES_TERMINAL
)

get_property(_CTEST_CUSTOM_TESTS_IGNORE GLOBAL PROPERTY CTEST_CUSTOM_TESTS_IGNORE)
file(WRITE "${CMAKE_BINARY_DIR}/CTestCustom.cmake" "SET(CTEST_CUSTOM_TESTS_IGNORE ${_CTEST_CUSTOM_TESTS_IGNORE})")

include(package)
include(doxygen)
