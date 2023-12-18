if( UPCXIO_COMPILE_TEST_CONTRACTS )
   set(UPCXIO_WASM_OLD_BEHAVIOR "Off")
   find_package( upcxio.cdt REQUIRED )
endif()

add_subdirectory(upcxio.bios)
add_subdirectory(upcxio.boot)
