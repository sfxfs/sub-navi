project(wit_c_sdk)

add_library(${PROJECT_NAME}
            wit_c_sdk.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
