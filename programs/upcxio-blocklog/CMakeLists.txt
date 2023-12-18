add_executable( upcxio-blocklog main.cpp )

if( UNIX AND NOT APPLE )
  set(rt_library rt )
endif()

target_include_directories(upcxio-blocklog PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries( upcxio-blocklog
        PRIVATE appbase
        PRIVATE upcxio_chain fc state_history ${CMAKE_DL_LIBS} ${PLATFORM_SPECIFIC_LIBS} )

copy_bin( upcxio-blocklog )
install( TARGETS
   upcxio-blocklog

   COMPONENT base

   RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
   LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
   ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
)
