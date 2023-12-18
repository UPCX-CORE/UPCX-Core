add_executable(test_rodupcx_cli test_rodupcx_cli.cpp)
target_link_libraries(test_rodupcx_cli
    PRIVATE rodupcx_lib fc amqpcpp ${CMAKE_DL_LIBS} ${PLATFORM_SPECIFIC_LIBS}
    PRIVATE Boost::unit_test_framework
)

add_test(NAME test_rodupcx_cli
    COMMAND programs/rodupcx/tests/test_rodupcx_cli
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
