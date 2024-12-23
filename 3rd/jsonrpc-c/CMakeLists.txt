project(jsonrpc-c)

add_library(${PROJECT_NAME} jsonrpc-c.c mjsonrpc.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} cjson clog ${ev})
