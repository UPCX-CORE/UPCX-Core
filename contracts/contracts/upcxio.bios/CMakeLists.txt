if (EOSIO_COMPILE_TEST_CONTRACTS)
   add_contract(upcxio.bios upcxio.bios ${CMAKE_CURRENT_SOURCE_DIR}/src/upcxio.bios.cpp)

   target_include_directories(upcxio.bios
      PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/include)

   set_target_properties(upcxio.bios
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ricardian/upcxio.bios.contracts.md.in ${CMAKE_CURRENT_BINARY_DIR}/ricardian/upcxio.bios.contracts.md @ONLY )

   target_compile_options( upcxio.bios PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR}/ricardian -R${CMAKE_CURRENT_BINARY_DIR}/ricardian )
else()
   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/bin/upcxio.bios.abi ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY )
   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/bin/upcxio.bios.wasm ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY )
endif()
