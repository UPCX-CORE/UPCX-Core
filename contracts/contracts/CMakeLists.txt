if( UPCX_COMPILE_TEST_CONTRACTS )
   set(UPCX_WASM_OLD_BEHAVIOR "Off")
   find_package( upcx.cdt REQUIRED )
endif()

add_subdirectory(upcx.bios)
add_subdirectory(upcx.boot)
add_subdirectory(upcx.token)