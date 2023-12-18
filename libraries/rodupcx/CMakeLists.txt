file(GLOB_RECURSE HEADERS "include/*.hpp" "include/*.h")

add_library( rodupcx_lib
             embedded_rodupcx.cpp
             rodupcx.cpp
             wasm_ql.cpp
             ${HEADERS}
           )

target_link_libraries( rodupcx_lib 
                       PUBLIC abiupcx chain_kv upcxio_chain fc softfloat
                     )

target_include_directories( rodupcx_lib
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
                          )