if (UPCX_COMPILE_TEST_CONTRACTS)
   add_contract(upcx.boot upcx.boot ${CMAKE_CURRENT_SOURCE_DIR}/src/upcx.boot.cpp)

   target_include_directories(upcx.boot
      PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/include)

   set_target_properties(upcx.boot
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ricardian/upcx.boot.contracts.md.in ${CMAKE_CURRENT_BINARY_DIR}/ricardian/upcx.boot.contracts.md @ONLY )

   target_compile_options( upcx.boot PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR}/ricardian -R${CMAKE_CURRENT_BINARY_DIR}/ricardian )
else()
   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/bin/upcx.boot.abi ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY )
   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/bin/upcx.boot.wasm ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY )
endif()

