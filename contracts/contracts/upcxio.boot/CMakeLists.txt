if (EOSIO_COMPILE_TEST_CONTRACTS)
   add_contract(upcxio.boot upcxio.boot ${CMAKE_CURRENT_SOURCE_DIR}/src/upcxio.boot.cpp)

   target_include_directories(upcxio.boot
      PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/include)

   set_target_properties(upcxio.boot
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ricardian/upcxio.boot.contracts.md.in ${CMAKE_CURRENT_BINARY_DIR}/ricardian/upcxio.boot.contracts.md @ONLY )

   target_compile_options( upcxio.boot PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR}/ricardian -R${CMAKE_CURRENT_BINARY_DIR}/ricardian )
else()
   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/bin/upcxio.boot.abi ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY )
   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/bin/upcxio.boot.wasm ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY )
endif()

