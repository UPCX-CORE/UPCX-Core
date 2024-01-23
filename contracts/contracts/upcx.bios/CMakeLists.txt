if (UPCX_COMPILE_TEST_CONTRACTS)
   add_contract(upcx.bios upcx.bios ${CMAKE_CURRENT_SOURCE_DIR}/src/upcx.bios.cpp)

   target_include_directories(upcx.bios
      PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/include)

   set_target_properties(upcx.bios
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ricardian/upcx.bios.contracts.md.in ${CMAKE_CURRENT_BINARY_DIR}/ricardian/upcx.bios.contracts.md @ONLY )

   target_compile_options( upcx.bios PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR}/ricardian -R${CMAKE_CURRENT_BINARY_DIR}/ricardian )
else()
   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/bin/upcx.bios.abi ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY )
   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/bin/upcx.bios.wasm ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY )
endif()
