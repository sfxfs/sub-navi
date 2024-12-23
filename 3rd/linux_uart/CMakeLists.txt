project(linux_uart)

add_library(${PROJECT_NAME} linux_uart.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} clog)
