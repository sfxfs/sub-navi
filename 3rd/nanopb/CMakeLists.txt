project(nanopb)

add_library(${PROJECT_NAME}
            pb_common.c
            pb_decode.c
            pb_encode.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
