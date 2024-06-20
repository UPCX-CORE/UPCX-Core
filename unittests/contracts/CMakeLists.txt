# will be implictly used for any compilation unit if not overrided by SYSTEM_INCLUDE_FOLDERS parameter
# these directories go as -isystem <dir> to avoid warnings from code of third-party libraries

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/upcx.bios/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/upcx.bios/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/upcx.msig/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/upcx.msig/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/upcx.system/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/upcx.system/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/upcx.token/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/upcx.token/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/upcx.wrap/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/upcx.wrap/)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/old_versions/v1.6.0-rc3/upcx.bios/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/old_versions/v1.6.0-rc3/upcx.bios/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/old_versions/v1.7.0-develop-preactivate_feature/upcx.bios/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/old_versions/v1.7.0-develop-preactivate_feature/upcx.bios/)
