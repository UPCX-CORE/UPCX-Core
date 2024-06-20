file(GLOB HEADERS "include/upcx/state_history_plugin/*.hpp")
add_library( state_history_plugin
             state_history_plugin.cpp
             ${HEADERS} )

target_link_libraries( state_history_plugin state_history chain_plugin upcx_chain appbase )
target_include_directories( state_history_plugin PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
