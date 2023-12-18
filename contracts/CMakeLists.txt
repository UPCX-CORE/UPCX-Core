### Build contracts with cdt if UPCXIO_COMPILE_TEST_CONTRACTS is on
include(ExternalProject)

if( UPCXIO_COMPILE_TEST_CONTRACTS )
   set(UPCXIO_WASM_OLD_BEHAVIOR "Off")
   find_package(upcxio.cdt REQUIRED)

   set(CMAKE_ARGS_VAL -DCMAKE_TOOLCHAIN_FILE=${UPCXIO_CDT_ROOT}/lib/cmake/upcxio.cdt/UpcxioWasmToolchain.cmake -DUPCXIO_COMPILE_TEST_CONTRACTS=${UPCXIO_COMPILE_TEST_CONTRACTS} )
   if( USE_UPCXIO_CDT_1_7_X)
      list(APPEND CMAKE_ARGS_VAL -DUSE_UPCXIO_CDT_1_7_X=${USE_UPCXIO_CDT_1_7_X})
   endif()
   message( STATUS "Building contracts in directory `upcx/contracts/`" )
   ExternalProject_Add(
      bios_boot_contracts_project
      SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/contracts
      BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/contracts
      CMAKE_ARGS ${CMAKE_ARGS_VAL}
      UPDATE_COMMAND ""
      PATCH_COMMAND ""
      TEST_COMMAND ""
      INSTALL_COMMAND ""
      BUILD_ALWAYS 1
   )
else()
  message( STATUS "Not building contracts in directory `upcx/contracts/`" )
  add_subdirectory(contracts)
endif()
