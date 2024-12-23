# Set the project name
project (cjson)

# Add a library with the above sources
add_library(${PROJECT_NAME} cJSON.c cJSON_Utils.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
