if (UPCX_COMPILE_TEST_CONTRACTS)
   add_contract(upcx.token upcx.token ${CMAKE_CURRENT_SOURCE_DIR}/src/upcx.token.cpp)

   target_include_directories(upcx.token
      PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/include)

   set_target_properties(upcx.token
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ricardian/upcx.token.contracts.md.in ${CMAKE_CURRENT_BINARY_DIR}/ricardian/upcx.token.contracts.md @ONLY )

   target_compile_options( upcx.token PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR}/ricardian -R${CMAKE_CURRENT_BINARY_DIR}/ricardian )
else()
   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/bin/upcx.token.abi ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY )
   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/bin/upcx.token.wasm ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY )
endif()
