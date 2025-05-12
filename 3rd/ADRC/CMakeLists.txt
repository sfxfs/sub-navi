project(adrc)

add_library(${PROJECT_NAME}
            adrc.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
