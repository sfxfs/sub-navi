project(jsonrpc-c)

add_library(${PROJECT_NAME} jsonrpc-c.c mjsonrpc.c)
target_link_libraries(${PROJECT_NAME} cjson ${ev})
