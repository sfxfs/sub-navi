# Set the project name
project (log_c)

# Add a library with the above sources
add_library(${PROJECT_NAME} log.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
