if (UPCX_COMPILE_TEST_CONTRACTS)
   add_contract(upcx.token upcx.token ${CMAKE_CURRENT_SOURCE_DIR}/src/upcx.token.cpp)

   if(TARGET upcx.token.wasm)
      set(TOKEN_TARGET upcx.token.wasm)
   else()
      set(TOKEN_TARGET upcx.token)
   endif()

   target_include_directories(${TOKEN_TARGET}
      PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/include)

   set_target_properties(${TOKEN_TARGET}
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ricardian/upcx.token.contracts.md.in ${CMAKE_CURRENT_BINARY_DIR}/ricardian/upcx.token.contracts.md @ONLY )

   target_compile_options( ${TOKEN_TARGET} PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR}/ricardian -R${CMAKE_CURRENT_BINARY_DIR}/ricardian )
else()
   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/bin/upcx.token.abi ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY )
   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/bin/upcx.token.wasm ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY )
endif()
