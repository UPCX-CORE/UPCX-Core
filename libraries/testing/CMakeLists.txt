file(GLOB HEADERS "include/upcxio/testing/*.hpp")

configure_file(contracts.hpp.in include/testing_contracts/contracts.hpp ESCAPE_QUOTES)
add_library(upcxio_testing_contracts INTERFACE)
target_include_directories(upcxio_testing_contracts INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/include/testing_contracts)

## SORT .cpp by most likely to change / break compile
add_library( upcxio_testing
             tester.cpp
             tester_network.cpp
             ${HEADERS}
        )

target_link_libraries( upcxio_testing upcxio_testing_contracts upcxio_chain fc chainbase Logging IR WAST WASM Runtime )
target_include_directories( upcxio_testing
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/../wasm-jit/Include"
                                   "${CMAKE_BINARY_DIR}/unittests/include"
                            )

if(MSVC)
  set_source_files_properties( db_init.cpp db_block.cpp database.cpp block_log.cpp PROPERTIES COMPILE_FLAGS "/bigobj" )
endif(MSVC)

set_target_properties( upcxio_testing PROPERTIES PUBLIC_HEADER "${HEADERS}" )
install( TARGETS upcxio_testing
   RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
   LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
   ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
   PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/upcxio/testing
)
install_directory_permissions( DIRECTORY ${CMAKE_INSTALL_FULL_INCLUDEDIR}/upcxio/testing )

